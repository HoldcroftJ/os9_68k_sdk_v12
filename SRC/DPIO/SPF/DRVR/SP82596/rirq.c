/****************************************************************************
 *           Copyright 1989-1998 by Microware Systems Corporation           *
 *                           All Rights Reserved                            *
 *                         Reproduced Under License                         *
 *                                                                          *
 * This software is confidential property of Microware Systems Corporation, *
 * and is provided under license for internal development purposes only.    *
 * Reproduction, publication, distribution, or creation of derivative works *
 * in any form to any party other than the licensee is strictly prohibited, *
 * unless expressly authorized in writing by Microware Systems Corporation. *
 *                                                                          *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *   1 ??/??/?? Creation ported from ISP                               mil
 *   3 07/15/97 Ported to OS9/68k                                      jpd
 *   4 08/27/97 Bug fix                                                taka
 *   6 11/05/97 Added RFD_ERRS to rirq.c and i82596.h                  pjw
 *   7 12/03/97 Added code to support netstat utility                  jk
 *              lu_updrvr -> lu_ether_drvr in DR_FMCALLUP_PKT          jk
 *   8 01/21/98 Removed compiler and linker warnings                   jk
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *   9 04/06/98 Fixed a few MAKE_LOCAL_ADDR_SWAP casts to u_int32      tmd
 *              (DDTS MHQco03924)
 *  11 06/22/99 Integrated Kei Thomsen's changes, fixed mbuf leak.     jk
 *  13 06/10/00 Don't pass mbuf up unless we can get a new one         rds
 *  15 08/17/01 Corrected PVTRAM mode buffer handling.                 KeiT
 ****************************************************************************/

#include <SP82596/defs.h>

#ifdef PVTRAM

Mbuf supply_mbuf(Dev_list deventry, Mbuf old_mbuf)
{
	Spf_drstat	drstat = (Spf_drstat)(deventry->v_dr_stat);
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat);
	Mbuf		mbuf;
	int32		status;
	status_reg	old_sr;


	if (old_mbuf == NULL) {
		status = get_mbuf (deventry, RMBUF_SIZE + sizeof (Dev_list)*2, &mbuf);
		old_sr = maskirq(lustat->lu_irqlvl);
		if (status == SUCCESS) {
	        mbuf->m_pnext = 0;        /* ensure pnext is 0 */
			mbuf->m_qnext = lustat->lu_recvfree;
			lustat->lu_recvfree = mbuf;
			mbuf->m_offset += sizeof(Dev_list);	/* Reserve space for the Dev_list entry */
		} else {
			lustat->lu_needrecvfree++;
			drstat->dr_iestat.ru_norbd++;	/* Count this somewhere... */
			mbuf = NULL;
		}
		restirq(old_sr);
	} else {
		Dev_list *ndp;
		old_sr = maskirq(lustat->lu_irqlvl);
		mbuf = old_mbuf;  /* use old mbuf */
		mbuf->m_offset = MBHDR_SZ;
		ndp = mtod(mbuf, Dev_list*);
		*ndp = deventry;
		mbuf->m_offset += (sizeof(Dev_list) * 2);
		mbuf->m_qnext = mbuf->m_pnext = NULL;
		mbuf->m_flags = 0;
		restirq(old_sr);
	}
	return mbuf;
}

#else	/* not PVTRAM */

void reclaim_rbds(RBD* rbd)
{
	RBD *nrbd;

	while (rbd) {
		nrbd = (RBD *)MAKE_CPU_ADDR((u_int32)rbd->rbd_beNext);
		m_free_p(rbd->rbd_mbuf);
		rbd->rbd_mbuf = 0;
		rbd = nrbd;
	}
}

Mbuf supply_mbuf(Dev_list deventry, Mbuf old_mbuf)
{
	Spf_drstat	drstat = (Spf_drstat)(deventry->v_dr_stat);
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat);
	Mbuf		mbuf;
	RBD			*rbd, *tailrbd, *lrbd;
	status_reg	old_sr;
	int32		status;

	/*
	** get mbuf to hold RBD and complete ethernet frame
	*/
	if (old_mbuf == NULL) {
		/* Count number of supplied buffers (KeiT) */
		int n = 0;
		old_sr = maskirq(lustat->lu_irqlvl);
#if defined (Flushing)
		_os_cache(6);               /* Flush data cache and turn it off */
#endif
		for (rbd = lustat->lu_rbd_head;
			rbd && (rbd != (RBD *)MAKE_CPU_ADDR((u_int32)rbd->rbd_beNext));
			rbd = (RBD *)MAKE_CPU_ADDR((u_int32)rbd->rbd_beNext)) {
				n++;
		}
		for (rbd = lustat->lu_rbd_free;
			rbd && (rbd != (RBD *)MAKE_CPU_ADDR((u_int32)rbd->rbd_beNext));
			rbd = (RBD *)MAKE_CPU_ADDR((u_int32)rbd->rbd_beNext)) {
				n++;
		}
#if defined (Flushing)
		_os_cache(5);               /* Flush data cache and turn it on */
#endif
		restirq(old_sr);

		/* We dont want more than lu_nrfd (32) + 2 buffer */
		if (n >= lustat->lu_nrfd+2)
		{
			lustat->lu_needrecvfree = 0; /* We dont need more ! */
			return(0);
		}
#ifdef _MPFSH
		status = get_mbuf (deventry, RMBUF_SIZE + sizeof (Dev_list) + 2, &mbuf);
#else
		status = get_mbuf (deventry, RMBUF_SIZE + sizeof (Dev_list), &mbuf);
#endif
		if (status != SUCCESS) {
			old_sr = maskirq(lustat->lu_irqlvl);
			lustat->lu_needrecvfree++;
			drstat->dr_iestat.recv_nobuf++;
			restirq(old_sr);

#if defined (DEBUG)
			if (lustat->lu_needrecvfree > 30)
				DEBUG6 ((void *)status, (void *)lustat->lu_needrecvfree);
#endif
			return NULL;
		}
		mbuf->m_offset += sizeof(Dev_list);
	} else {
		Dev_list *ndp;
		old_sr = maskirq(lustat->lu_irqlvl);
		mbuf = old_mbuf;	/* use old mbuf */
		mbuf->m_offset = MBHDR_SZ;
		ndp = mtod(mbuf, Dev_list*);
		*ndp = deventry;
		/*
		** Move the offset to leave room for 2 device list pointers (ours and
		** spenet's). In the code above this we only add room for one pointer
		** because get_mbuf leaves room for the other one.
		*/
		mbuf->m_offset += (sizeof(Dev_list) * 2);
		mbuf->m_qnext = mbuf->m_pnext = NULL;
		mbuf->m_flags = 0;
		restirq(old_sr);
	}

#if defined (Flushing)
	_os_cache(6);               /* Flush data cache and turn it off */
#endif

	/*
	** The mbuf looks like this:
	**
	** +-----------------------------+
	** | Mbuf header (0x10 bytes)    |
	** +-----------------------------+
	** | Space for SPF (0x08 bytes)  |
	** +-----------------------------+
	** | RBD for i82596 (0x20 bytes) |
	** ------------------------------+
	** | dirty align (2 bytes) _MPFSH|
	** +-----------------------------+
	** | Received data (0x610 bytes) |
	** +-----------------------------+
	*/
	rbd = (RBD *)MAKE_CPU_ADDR(mtod(mbuf, u_int32));/* rbd at head of mbuf */
	rbd->rbd_leBuffer =     /* data stored after rbd */
#if defined (_MPFSH)
		(char *)MAKE_LOCAL_ADDR_SWAP((u_int32)(rbd+1) + 2); /* dirty align */
#else
		(char *)MAKE_LOCAL_ADDR_SWAP((u_int32)(rbd+1));
#endif
	rbd->rbd_size = RMBUF_SIZE - sizeof(RBD);               /* plenty of space */
	rbd->rbd_mbuf = mbuf;                                   /* ptr to mbuf */
	rbd->rbd_acount = rbd->rbd_zero1 = rbd->rbd_zero2 = 0;
	rbd->rbd_leNext = rbd->rbd_beNext = NULL;

	/* 17-Jun-99 Kehres */
	mbuf->m_qnext = lustat->lu_recvfree;
	lustat->lu_recvfree = mbuf;

	old_sr = maskirq(lustat->lu_irqlvl);                            /* mask irq */
	tailrbd = lustat->lu_rbd_tail;

	/*
	** prefetch bit set in last entry means chip is working
	** on last RBD.  In this case, add the mbuf allocated to the rbd_free
	** list (instead of the rbd_head list, which is being used by the chip).
	** The RBDs on rbd_free list are put on the rbd head list when the chip
	** finally runs out of mbufs on the rbd_head list (gives the
	** "no rbd resources" irq).
	*/
	if (tailrbd == 0 || tailrbd->rbd_size & RBD_PREFETCH) {
		if (lrbd = lustat->lu_rbd_free) {
			rbd->rbd_leNext = (RBD *)MAKE_LOCAL_ADDR_SWAP((u_int32)lrbd);
			rbd->rbd_beNext = (RBD *)MAKE_LOCAL_ADDR((u_int32)lrbd);
		} else {
		lustat->lu_rbd_fend = rbd;
		rbd->rbd_size |= RBD_LAST;
		}
		lustat->lu_rbd_free = rbd;
		drstat->dr_iestat.rbd_enqueued++;
		if ((lustat->lu_scb->scb_status & SCB_RU_READY) == 0) {
			start_ru(deventry);             /*  try to start RU */
		}
	} else {
		/* if between the time RBD_PREFETCH was checked and end of
		** this else the RU finishes receiving the (last) packet it
		** will set RBD_LAST.  Since we clear it here we *MAY* either
		** end up with an extra empty RBD at the end (giant packet)
		** or an RBD chain with the last two RBDs marked as LAST.
		** In the former case the giant packet will end up getting
		** pitched in recv_irq().  In the latter case things should
		** work out okay. - gkm
		*/
		rbd->rbd_size |= RBD_LAST;
		tailrbd->rbd_leNext = (RBD *)MAKE_LOCAL_ADDR_SWAP((u_int32)rbd);
		tailrbd->rbd_beNext = (RBD *)MAKE_LOCAL_ADDR((u_int32)rbd);
		tailrbd->rbd_size &= ~RBD_LAST;
		lustat->lu_rbd_tail = rbd;
		drstat->dr_iestat.rbd_tailadds++;
	}
#if defined (Flushing)
	_os_cache(5);               /* Flush data cache and turn it on */
#endif

	restirq(old_sr);

	return mbuf;
}
#endif

#ifdef PVTRAM
error_code recv_irq_sub0 (Dev_list deventry, RFD *rfd)
{
	Spf_drstat	drstat = (Spf_drstat)(deventry->v_dr_stat);
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat);
	u_short		len;
	u_int32		*mb_start;
	Mbuf		mbuf;
	u_int8		*pkt;

	if (mbuf = lustat->lu_recvfree) {
		lustat->lu_recvfree = mbuf->m_qnext;
		mbuf->m_qnext = NULL;
		len = (rfd->rfd_acount & RFD_SIZEMASK);
#if defined (Flushing)
		_os_cache(4);               /* Flush data cache */
#endif
		memcpy(mtod(mbuf, char *), (char *)rfd->rfd_da, len);
		mbuf->m_size = len;
		mb_start =(u_int32 *)(((int)mbuf + MBHDR_SZ +sizeof(void *)));
		*mb_start = (u_int32)deventry;

		if (lustat->lu_updrvr != NULL) {
			DEBUG4((void *)99, (void *)-1);

			/* update if_data statistics */
			/* is this a broadcast? */
			pkt = mtod(mbuf, u_int8 *);
			if ((pkt[0] == 0xff) && (pkt[1] == 0xff) && (pkt[2] == 0xff) &&
					(pkt[3] == 0xff) && (pkt[4] == 0xff) && (pkt[5] == 0xff)) {
			    mbuf->m_flags |= M_BCAST;
				lustat->if_imcasts++;

			/* is this a multicast? */
			} else if (pkt[0] & 1) {
			    mbuf->m_flags |= M_MCAST;
				lustat->if_imcasts++;
			}

			DR_FMCALLUP_PKT(deventry, lustat->lu_ether_drvr, mbuf);
			supply_mbuf (deventry, NULL);

			/* increment if_data structure */
			lustat->if_ipackets++;

		} else {
			m_free_p (mbuf);        /* Drop */
		}
		drstat->dr_iestat.recv_count++;
	} else {
		supply_mbuf (deventry, NULL);
		lustat->lu_needrecvfree = lustat->lu_nrfd - 1;
		drstat->dr_iestat.recv_nobuf++;
	}
	return SUCCESS;
}
#else   /* not PVTRAM */
error_code recv_irq_sub1 (Dev_list deventry, RBD *rbd)
{
	Spf_drstat drstat = (Spf_drstat)(deventry->v_dr_stat);
	Spf_lustat lustat = (Spf_lustat)(deventry->v_lu_stat);

	/* check if it is same as in our static storage */
	if (rbd == (RBD *)MAKE_CPU_ADDR((u_int32)lustat->lu_rbd_head)) {
		/* Yes.  Is it zero? */
		lustat->lu_rbd_head = (RBD *)MAKE_CPU_ADDR((u_int32)rbd->rbd_beNext);
		if (lustat->lu_rbd_head == NULL)
			/* must be last RBD.  zap the tail too. */
			lustat->lu_rbd_tail = NULL;
	} else {
		/* %*$@#! chip & static don't agree */
		RBD *actrbd;
		/* Count that we have lost the pointer */
		drstat->dr_iestat.rbd_headlost++;

		/* Free all lost buffers */
		for (actrbd = (RBD *)MAKE_CPU_ADDR((u_int32)lustat->lu_rbd_head);
			actrbd && (rbd != actrbd) && (rbd != (RBD *)MAKE_CPU_ADDR((u_int32)actrbd->rbd_beNext));
			actrbd = (RBD *)MAKE_CPU_ADDR((u_int32)actrbd->rbd_beNext))
		{
			Mbuf mbuf;

			drstat->dr_iestat.rbd_kill_heads++;  /* Count the killed buffers */
			mbuf = actrbd->rbd_mbuf;

			m_free_p(mbuf);                      /* Free the lost buffer */
			lustat->lu_needrecvfree++;           /* Give me a new buffer (please) */
		}

		lustat->lu_rbd_head = (RBD *)MAKE_CPU_ADDR((u_int32)rbd->rbd_beNext);
		if (lustat->lu_rbd_head == NULL)
			/* must be last RBD.  zap the tail too. */
			lustat->lu_rbd_tail = NULL;
	} /* if (rbd == lustat->lu_rbd_head) */

	return SUCCESS;
}

error_code recv_irq_sub2 (Dev_list deventry, RBD *rbd)
{
	Spf_drstat	drstat = (Spf_drstat)(deventry->v_dr_stat);
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat);
	u_int32		*mb_start;
	Mbuf		mbuf;
	u_int8		*pkt;

	/* if an unused RBD is found or a giant packet was
	 * received, just pitch the packets. */
	if ( !(rbd->rbd_acount & RBD_USED) || !(rbd->rbd_acount & RBD_EOF)) {
		/*
		** if RBD_EOF is not set the packet spans bufs
		** (ie, it is a giant packet. In either case we need
		** to free all the associated rbd's and mbufs.  This
		** could be handled by returning the RBDs to the
		** end of the list, but since giant or unused
		** packets are unusual it is probably not worth
		** the effort.
		*/
		pitch_rbds(lustat, rbd);        /* pitch 'em */

		if ( !(rbd->rbd_acount & RBD_USED) ) {
			drstat->dr_iestat.recv_unused++;/* inc unused ctr */
			/* count error in if_data structure */
			lustat->if_ierrors++;
		}
		if ( !(rbd->rbd_acount & RBD_EOF ) ) {
			drstat->dr_iestat.recv_giant++; /* inc giant ctr */
			/* count error in if_data structure */
			lustat->if_ierrors++;
		}
	} else {
		/* marked USED & not a giant packet */
		mbuf = rbd->rbd_mbuf;
		mbuf->m_size = (rbd->rbd_acount & RBD_SIZEMASK);
#if defined (_MPFSH)
		M_PAD(mbuf, sizeof(RBD) + 2);   /* skip RBD in mbuf and ALIGN */
#else
		M_PAD(mbuf, sizeof(RBD));       /* skip RBD in mbuf */
#endif
		mbuf->m_qnext = NULL;   /* ensure no queue list */
		/*Put the device entry right behind the SPF Header
		 *information at the beginning of the mbuf*/
		mb_start =(u_int32 *)(((int)mbuf + MBHDR_SZ +sizeof(void *)));
		*mb_start = (u_int32)deventry;

		if (lustat->lu_updrvr!=NULL) {
			if (lustat->lu_needrecvfree<(lustat->lu_nrfd-1)) {
				pkt = mtod(mbuf, u_int8 *);

				/* update if_data statistics */
				if (pkt[0] & 0x1) {
					lustat->if_imcasts++;
					if ((pkt[0] == 0xff) && (pkt[1] == 0xff) && (pkt[2] == 0xff) &&
						(pkt[3] == 0xff) && (pkt[4] == 0xff) && (pkt[5] == 0xff)) {
						mbuf->m_flags |= M_BCAST;
					} else {
						mbuf->m_flags |= M_MCAST;
					}
				}

				/*
				** OK, here's the deal. This used to just pass the packet up
				** the stack and then try to supply a new mbuf to the chip.
				** The big problem with this is that we can hang the driver
				** in low mbuf situations. If we completely starve the chip
				** we hang. We can't get new mbufs because we can't get a
				** recv interrupt, and we can't get a recv interrupt because
				** we don't have any mbufs.
				**
				** The second problem is the code to resupply mbufs is way,
				** Waaaaayyyy too complicated and full of errors. The worst
				** of which is it will almost immediately bus error in high
				** packet situations. At some point all this code should be
				** thrown out and rewritten from scratch. it CAN NOT BE
				** FIXED by adding more needless complication to the existing
				** code base.
				**
				** The change I have made is to not pass an mbuf up the stack
				** unless we can get one to replace it. This is the method
				** used by most of the other SPF Ethernet drivers. The only
				** drawback to this is we can't quite get as many packets
				** up the stack in a low mbuf situation as we always keep
				** lustat->lu_nrfd number of mbufs allocated to the driver.
				** The huge advantage is that it works!
				*/
				if (supply_mbuf(deventry, NULL) == NULL) {
					/* Give this one back */
					supply_mbuf(deventry, mbuf);
				} else {
					/* Pass it up to the stack */
					DR_FMCALLUP_PKT(deventry, lustat->lu_ether_drvr, mbuf);
				}

				/* increment if_data structure */
				lustat->if_ipackets++;

			} else {
				supply_mbuf (deventry, mbuf); /* allocate old one */
				supply_mbuf (deventry, NULL); /* allocate new one, because we are low on buffer */
			}
		} else {
			DEBUG5 ((void*)lustat->lu_needrecvfree, (void *)lustat->lu_updrvr);
			m_free_p (mbuf);        /* Drop */
		}
		drstat->dr_iestat.recv_count++;
	}       /* if used or giant */

	return SUCCESS;
}
#endif

/* handle receiver interrupt
*/
void recv_irq(Dev_list deventry)
{
	Spf_drstat	drstat = (Spf_drstat)(deventry->v_dr_stat);
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat);
	RFD			*rfd, *t_rfd;
	u_short		iter = 0;
	u_int16		stat1;
#ifndef PVTRAM
	RBD			*rbd;
#endif

	drstat->dr_iestat.recv_irq++;

	if (lustat->lu_ioenabled != DRVR_IOEN) {
		return;
	}

	t_rfd = (RFD *)MAKE_CPU_ADDR((u_int32)lustat->lu_rfd_head->rfd_bePrev);

#ifndef PVTRAM
	if ((t_rfd->rfd_stat2 & RFD_LAST) == 0) {
		/* lost end pointer -- PANIC NOW! */
		SYSDBG((void*)0x1111,NULL);             /* should never happen */
		return;
	}
#endif

#ifdef PVTRAM
	if (lustat->lu_recvfree == NULL) {
		/* Oh, no Mbuf's allocated right now? Try to allocate one. */
		supply_mbuf(deventry, NULL);
	}
#endif

	/*
	** Loop less than MAX_LOOP_CNT times since we have interrupts masked
	** (i.e. be kind to SCF )
	*/
	for (rfd = (RFD *)MAKE_CPU_ADDR((u_int32)lustat->lu_rfd_head);
		rfd != t_rfd && iter < MAX_LOOP_CNT;
		rfd = (RFD *)MAKE_CPU_ADDR((u_int32)rfd->rfd_beNext), iter++) {
		/* ready for us? */
		if ((stat1 = rfd->rfd_stat1) & RFD_COMPLETE) {
			if (stat1 & RFD_OK) {
#ifdef PVTRAM
				recv_irq_sub0 (deventry, rfd);
#else   /* ! PVTRAM */
				/* set rbd to chip supplied RBD pointer */
				rbd = (RBD *)MAKE_CPU_ADDR_SWAP((u_int32)(rfd->rfd_leRbd));

				if (recv_irq_sub1 (deventry, rbd) != SUCCESS)
					return;

				if (recv_irq_sub2 (deventry, rbd) != SUCCESS)
					return;

				rfd->rfd_leRbd = RFD_RBD_NOTUSED; /* reset for next use */
			} else { /* ! if (stat1 & RFD_OK) */
				if (stat1 & RFD_NORES) {
					DEBUG4 ((void *)0x66, (void *)rfd);
					rfd->rfd_stat1 = 0;
					rfd->rfd_acount = 0;
					break;/* chip ran out of RBDs here, need to restart RU*/
				} else {
					if(stat1 & RFD_ERRS) {
					/*
					** could be length error if configured to check length,
					** CRC error, alignment error, DMA overrun, frame too
					** short, No EOP for bit stuffing or receive collision
					*/
					SYSDBG((void*)0x4242,NULL);/* chip saved a bad frame? */
					break;
					}
				}
#endif
			}
			DEBUG4 ((void *)0x77, (void *)rfd);
			rfd->rfd_stat1 = 0;
			rfd->rfd_acount = 0;
		} else { /* if ((stat1 = rfd->rfd_stat1) & RFD_COMPLETE) */
			/* !((stat1 = rfd->rfd_stat1) & RFD_COMPLETE) */
			if (stat1 & RFD_BUSY) {
				break;          /* caught up with chip */
			} else {
#ifdef PVTRAM
				drstat->dr_iestat.only_one_rbd++;	/* Only for statistic... */
#endif
				return;
			}
		} /*  if ((stat1 = rfd->rfd_stat1) & RFD_COMPLETE) */

		/* drag the EOL pointer with us */
		/* Do not test if the last packet is busy, because it can be busy ! */
		/* Just overwrite the last buffer */
		rfd->rfd_stat2 |= RFD_LAST;             /* Set first ... */
		((RFD *)MAKE_CPU_ADDR((u_int32)rfd->rfd_bePrev))->rfd_stat2 &= ~RFD_LAST;       /* ... & then clear */

	}       /* for (loop through all RFDs */

	if (iter > drstat->dr_iestat.recv_loops) {
		drstat->dr_iestat.recv_loops = iter;
	}
	lustat->lu_rfd_head = rfd;

#ifndef PVTRAM
	/* Check for: Only one or zero buffer in the RBD List and no more
				  buffer in RBD free. (KeiT) */
	if ((lustat->lu_rbd_head == lustat->lu_rbd_tail) &&
		(lustat->lu_rbd_free == 0) && (lustat->lu_rbd_fend == 0))
	{
		lustat->lu_needrecvfree = lustat->lu_nrfd-1;    /* We have absoult nothing (or just one) ! */
		drstat->dr_iestat.only_one_rbd++;
	}

#endif
	/* see if we are short on receive mbufs */
	while (lustat->lu_needrecvfree) {
		lustat->lu_needrecvfree--;
		if (supply_mbuf(deventry, NULL) == NULL) break;
	}

	/* restart receiver if not ready */
	if ((lustat->lu_scb->scb_status & SCB_RU_READY) == 0) {
		start_ru(deventry);
	}

#if defined (Flushing) && !defined(PVTRAM)
	_os_cache(4);               /* Flush data cache */
#endif

	return;

} /* end of recv_irq */

void start_ru(Dev_list deventry)
{
	Spf_drstat drstat = (Spf_drstat)(deventry->v_dr_stat);
	Spf_lustat lustat = (Spf_lustat)(deventry->v_lu_stat);
#ifdef PVTRAM
	lustat->lu_scb->leRfa
		= (RFD *)MAKE_LOCAL_ADDR_SWAP((u_int32)lustat->lu_rfd_head);
	lustat->lu_scb->scb_cmd |= SCB_RU_START_CMD;
	I82596_CHANATTN(lustat);
	drstat->dr_iestat.recv_rustart++;
#else
	RBD			*rbd;
	u_int8		start_ru = 1;

#if defined (Flushing)
	_os_cache(6);               /* Flush data cache and turn it off */
#endif

	if (lustat->lu_scb->scb_status & SCB_RU_NORES)
		drstat->dr_iestat.ru_nores++;

	if ((lustat->lu_scb->scb_status & (SCB_RU_NORBD | SCB_RU_NORES))
		|| (lustat->lu_rbd_head == lustat->lu_rbd_tail)) {
		/* check if there are some mbufs on the rbd_free list ...
		 * Check to see if rbd_free is non-zero.  rbd_free maybe equal
		 * to rbd_fend, (in case of only one free rbd).  Conversely, both
		 * may be zero, we do not want to do anything here in that case.
		 */
		if (lustat->lu_rbd_free != 0) { /* We have a free RBD list */
			/* ...if so, put them on the rbd_head list (the chip's list) ...*/

			/* Normaly, we dont have a buffer left at this moment in the head/tail.
			 * But it happend very often, during high network load!
			 * So, just return the buffers to the mbuf pool.
			 * If we dont do so, we lost the pointer to that buffers.
			 *
			 * If we have any data in it... sorry  (KeiT)
			 */
			if (lustat->lu_rbd_head) {
				drstat->dr_iestat.rbd_kill_head++;   /* Count, how often this happen */
			}
			for (rbd = lustat->lu_rbd_head;
					rbd && (rbd != (RBD *)MAKE_CPU_ADDR((u_int32)rbd->rbd_beNext));
					rbd = (RBD *)MAKE_CPU_ADDR((u_int32)rbd->rbd_beNext))
			{
				Mbuf mbuf;

				drstat->dr_iestat.rbd_kill_heads++;  /* Count the killed buffers */

				mbuf = rbd->rbd_mbuf;

				m_free_p(mbuf);                     /* Free the lost buffer */

				lustat->lu_needrecvfree++;           /* Give me a new buffer (please) */
			}
			/* Now we can be sure, that there is no lost buffer left in
			 * our head/tail list.
			 */
			rbd = (RBD *)MAKE_CPU_ADDR((u_int32)lustat->lu_rbd_free);
			lustat->lu_rbd_head = rbd;
			lustat->lu_rbd_tail = lustat->lu_rbd_fend;
			lustat->lu_rfd_head->rfd_leRbd
					= (RBD *)MAKE_LOCAL_ADDR_SWAP((u_int32)(rbd));
			/* ... and null out the rbd_free list */
			lustat->lu_rbd_free = lustat->lu_rbd_fend = NULL;
			drstat->dr_iestat.rbd_listadd++;
		}
		else
		{
			lustat->lu_needrecvfree++;              /* We are low on RBD's ! */
		}
		drstat->dr_iestat.ru_norbd++;
	}

	/* if there are no RBDs on rbd_head list, skip start_ru */
	if (MAKE_LOCAL_ADDR_SWAP((u_int32)lustat->lu_rbd_head) == 0x0)
	start_ru = 0;

	if (start_ru) {
		lustat->lu_rfd_head->rfd_leRbd
			= (RBD *)MAKE_LOCAL_ADDR_SWAP((u_int32)(lustat->lu_rbd_head));
		lustat->lu_scb->leRfa
			= (RFD *)MAKE_LOCAL_ADDR_SWAP((u_int32)(lustat->lu_rfd_head));
		lustat->lu_scb->scb_cmd |= SCB_RU_START_CMD;
		I82596_CHANATTN(lustat);
		drstat->dr_iestat.recv_rustart++;
	} else {
		drstat->dr_iestat.recv_norustart++;
	}
#if defined (Flushing)
	_os_cache(5);               /* Flush data cache and turn it on */
#endif
#endif
}

#ifndef PVTRAM
/* pitch_rbds: pitches RBDs pointed to by rbd, until it comes to the end of
** the list or the RBD_EOF bit is set.  Sets head & tail pointers in
** ifstat.
*/
void pitch_rbds(Spf_lustat lustat, RBD *rbd)
{
	RBD  *rbdNext;
	int  rbdLast;
	Mbuf mbuf;

	rbd = (RBD *)MAKE_CPU_ADDR((u_int32)rbd);

	do {
		rbdLast = (rbd->rbd_acount & RBD_EOF);
		rbdNext = (RBD *)MAKE_CPU_ADDR((u_int32)rbd->rbd_beNext);
		mbuf = rbd->rbd_mbuf;
		m_free_p(mbuf);
		lustat->lu_needrecvfree++;      /* inc shortfall */
		rbd = rbdNext;
		/*
		** check rbdNext as a double check we dont walk off
		** the end (though rbdLast should be set at the end.
		*/
	} while (!rbdLast && rbdNext);

	/* update rbd head, if it is zero mark tail as zero too */
	if ( (lustat->lu_rbd_head = rbd) == NULL) {
		lustat->lu_rbd_tail = NULL;
	}
#if defined (Flushing)
	_os_cache(4);               /* Flush data cache */
#endif

}
#endif
