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
 *   1 04/04/97 Creation                                               mil
 *   3 07/15/97 Ported to OS9-68k                                      jpd
 *   4 08/27/97 Bug fix                                                taka
 *   6 09/29/97 Added MVME172 support                                  pjw
 *   8 01/21/98 Removed compiler and linker warnings                   jk
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  11 06/22/99 Integrated Kei Thomsen's changes, fixed mbuf leak.     jk
 *  12 08/29/99 Turn cache back on if returning early due to error     rds
 *              changed lu_ether_addr to lu_item_addr.addr             rds
 *  15 08/17/01 Corrected flushing in PVTRAM mode.                     KeiT
 ****************************************************************************/

/*
**	Header Files
*/

#include <SP82596/defs.h>

#if defined (DEBUG)
void debug1 (void *q, void *p) {}
void debug2 (void *q, void *p) {}
void debug3 (void *q, void *p) {}
void debug4 (void *q, void *p) {}
void debug5 (void *q, void *p) {}
void debug6 (void *q, void *p) {}
void debug7 (void *q, void *p) {}
#endif


#if defined (_MPFSH)
void I82596_CHANATTN(Spf_lustat lustat)
{
	u_int32 *port = (u_int32 *)((u_int32)lustat->lu_ca & 0xfff0ffff);
	nop(); nop (); nop (); nop (); nop ();
	*port = 0;
}
#endif

void read_eaddr (Spf_lustat lustat)
{
#if defined(mz8554)    || \
    defined(ie7140)    || \
    defined(hkvf82596) || \
    defined (spdve7708)
	u_int8* ptr;
#endif
	/* get data from ID prom */
#if defined(sp167) || defined(sp162) || defined(spgenesis) || defined(sp177) || defined(sp172)
	memcpy(lustat->lu_item_addr.addr, BBRAM_ETHERADDR, 6);
#elif defined(mz8554) || defined(ie7140)
	idptr = (u_int8 *)(lustat->lu_desc.idpromaddr + 3);
	for (ptr = (u_int8 *)lustat-lu_item_addr.addr, count=32; count--; idptr += 4) {
		*ptr++ = *idptr;
	}
#elif defined(hkvf82596)
	if (lustat->v_isV4d) {
		ptr = (u_char *)0x00030006;	/* rom (might) store TIC image here */
		if (ptr[0] == 00 && ptr[1] == 0x80 && ptr[2] == 0xf9) {
			memcpy(lustat->lu_item_addr.addr, ptr, 6);
		} else {
			/* default to something (almost) reasonable */
			lustat->lu_item_addr.addr[0] = 0x00;
			lustat->lu_item_addr.addr[1] = 0x80;
			lustat->lu_item_addr.addr[2] = 0xF9;
			lustat->lu_item_addr.addr[3] = 0x2D;
			lustat->lu_item_addr.addr[4] = 0x00;
			lustat->lu_item_addr.addr[5] = 0x00;
		}
	} else {
		idptr = (u_int8 *)(lustat->lu_desc.idpromaddr + 0x2047);
		for (ptr = lustat->lu_item_addr.addr, count=8; count--; idptr += 8)
		*ptr++ = *idptr;
	}
#elif defined (spdve7708)
	{
    	int i;
    	u_char *ram = BBRAM + BBRAM_MAC_OFFSET;

    	for (i=0, ptr = lustat->lu_item_addr.addr; i<6; i++, ram+=4)
        	*ptr ++ = *(ram+3);
    }
#endif		/* #if defined(mz8554) || defined(ie7140) */
}	/* void read_eaddr () */


#if defined(_OSK)
_asm("
SWAPWORDS:
	swap d0 		;too bad C can't do this!
	rts
");
#endif

error_code i82596CMDAdd(Dev_list deventry, u_int32 cmd, void* cdata, u_int32 clen)
{
	Spf_drstat	drstat = (Spf_drstat)(deventry->v_dr_stat);
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat);
	CB 			*cb;
	CB 			*tcb;
	status_reg 	oldsr;

	if ((cmd & CB_CMDMASK) != CB_CMD_XMIT) {
		oldsr = maskirq(lustat->lu_irqlvl);
		cb = lustat->lu_cb_free;			/* get entry to use */
		if (cb == NULL) {				/* out of CBs? */
			restirq (oldsr);
			drstat->dr_iestat.cmd_dropped++;
			return 1;
		}

 		lustat->lu_cb_free = (CB *)MAKE_CPU_ADDR_SWAP((u_int32)cb->cb_leNext);

		restirq(oldsr);
		DEBUG2 ((void *)1, cdata);
		if (cdata && clen)
			memcpy((void*)cb->cmd.ia, (u_int8*)cdata, clen);
			/* copy in command data (non-XMIT commands only) */
		} else {
			cb = (CB *)cdata;	/* ready-to-go from i82596Output */
		}

		/*
		 * load up another command
		 */
		drstat->dr_iestat.cmd_issued++;
		cb->cb_status = 0;
		cb->cb_command = CB_LAST | CB_IRQ | cmd;
#if defined (_MPFSH) /**/
		copy_long ((u_int16 *)&cb->cb_leNext, (u_int32)NULL);
#else
		cb->cb_leNext = NULL;
#endif

		/*
		 * add command to command chain
		 */
		oldsr = maskirq(lustat->lu_irqlvl);
#if defined (Flushing) && !defined(PVTRAM)
		_os_cache(6);			/* Flush data cache and turn it off */
#endif
		if (lustat->lu_cb_head)	{	/* existing command chain? */
			if (lustat->lu_cb_next) {		/* setup command chain */
				tcb = (CB *)MAKE_CPU_ADDR((u_int32)lustat->lu_cb_next);
				do {
					if(tcb->cb_leNext)
						tcb =  (CB *)MAKE_CPU_ADDR_SWAP((u_int32)tcb->cb_leNext);
					else
						break;
				} while(tcb);
				tcb->cb_leNext = (CB *)MAKE_LOCAL_ADDR_SWAP((u_int32)cb);
				tcb->cb_command &= ~(CB_LAST | CB_IRQ);
				drstat->dr_iestat.cmd_links++;
			} else {				/* xirq will restart this new chain */
				lustat->lu_cb_next = cb;	/* move next ptr to this CB */
				drstat->dr_iestat.needxirqstart++;
			}
    	} else {			/* no existing chain: CU must be idle */
			lustat->lu_cb_head = cb;

		    /*
		     * start command unit
		     */
		    lustat->lu_scb->leCbl = (CB *)MAKE_LOCAL_ADDR_SWAP((u_int32)cb);
		    drstat->dr_iestat.cu_starts++;
		    lustat->lu_scb->scb_cmd |= SCB_CU_START_CMD;
		    DEBUG3 (0, (void *)lustat->lu_scb);
		    I82596_CHANATTN(lustat);
		}
#if defined (Flushing) && !defined(PVTRAM)
		_os_cache(5);				/* Flush data cache and turn it on */
#endif
		restirq(oldsr);

		return SUCCESS;
}

u_int32 i82596Output(Dev_list deventry, Mbuf mesg)
{
	Spf_drstat	drstat = (Spf_drstat)(deventry->v_dr_stat);
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat);
	TCB 		*tcb;
	CB 			*cb;
	u_int16 	add_failed, mlen, plen, xcmd;

#ifdef TIMER
	lustat->lu_timer[lustat->lu_tidx++] = *(u_int32 *)(*((u_int32*)0)+0x54);
	if (lustat->lu_tidx >= NTIMER) lustat->lu_tidx = 0;
#endif

#ifdef PVTRAM
	{		/* always for PVTRAM */
#else
#if defined (Flushing)
	_os_cache(6);				/* Flush data cache and turn it off */
#endif		
	if (lustat->lu_chipmode & CM_XMIT_FLEX) {
#endif
		status_reg oldsr;
		/*
		 * get next CB entry to load.
		 */
		oldsr = maskirq(lustat->lu_irqlvl);
		cb = lustat->lu_cb_free;			/* get entry to use */
		if (cb == NULL) {			/* caught up with chip? */
			/*
			** We ran out of control blocks so we have nowhere to
			** put this mbuf. We SHOULD drop the first mbuf in the
			** queue and insert this one at the end, but it is
			** easier to just drop this one. Also, turn the cache
			** back on and enable interrupts.
			*/
#if defined (Flushing)
			_os_cache(5);			/* turn cache back on */
#endif
			restirq(oldsr);
			m_free_p(mesg);
			drstat->dr_iestat.cmd_dropped++;
			return 1;
		}
		lustat->lu_cb_free = (CB *)MAKE_CPU_ADDR_SWAP((u_int32)cb->cb_leNext);
		/* setup next entry to use */
		restirq(oldsr);

		tcb = (TCB *)MAKE_CPU_ADDR((u_int32)&cb->cmd.tcb);
		/* point to TCB location */
		xcmd = CB_CMD_XMIT;			/* CB transmit command */
	}

	plen = mlen = m_len_p(mesg);
	
#ifndef PVTRAM
	if (lustat->lu_chipmode & CM_XMIT_SIMP) {
		/*
		 * Work-around for "Errata 2.0 - Truncated Frame on Transmit"
		 * In this case, see if there is room for the CB in the mbuf.
		 * If not, copy data into new mbuf.  Build CB/TCB/DATA in
		 * one block of memory and use "simplified mode" to transmit.
		 *
		 * This whole section of code is a gross hack, but then we're
		 * trying to work around an equally gross chip bug!
		 */
		#define CB_HEAD (sizeof(CB) - TCB_SIZE_ADJ + sizeof(Mbuf))
		if (m_unpad(mesg, CB_HEAD))	{		/* can't fit CB in mbuf? */
			Mbuf nmb;
			int32 stat;

			if ((nmb = m_getn(mlen + CB_HEAD, &stat)) == NULL) {
				/*
				** We don't have room in the original mbuf and can't
				** get a new one, so I guess just pitch it. If we
				** are doing cache flushing, also turn the cache
				** back on.
				*/
#if defined (Flushing)
				_os_cache(5);			/* turn cache back on */
#endif
				m_free_p(mesg);
				return 1;
			}
			m_move(mesg, 0, plen, mtod(nmb, char *) + CB_HEAD);	/* copy data */
			m_free_p(mesg);
			mesg = nmb;
			drstat->dr_iestat.xmit_scoops++;
		}

		cb = (CB *)MAKE_CPU_ADDR(mtod(mesg, u_int32));
#ifdef _UCC
#if defined (_MPFSH)
		memcpy ((void *)cb, &mesg, sizeof(Mbuf)); /* stash mbuf pointer here */
		cb = (CB *) ( (char *)cb + sizeof(Mbuf *) );  /* inc cb by 4 */
#else
		*(Mbuf *)cb    = mesg;		/* stash mbuf pointer here */
		cb = (CB *) ( (char *)cb + sizeof(Mbuf *) );  /* inc cb by 4 */
#endif
#else
		*((Mbuf*)cb)++ = mesg;		/* stash mbuf pointer here */
#endif
		tcb = (TCB *)MAKE_CPU_ADDR((u_int32)&cb->cmd.tcb);
		/* point to TCB location */
		xcmd = CB_CMD_XMIT;		  /* CB transmit command */
#if defined (_MPFSH) /**/
		DEBUG7 ((void *)cb, (void *)&tcb->leTbdptr);
		copy_long ((u_int16 *)&tcb->leTbdptr, MBUF_MARKER);
#else
		tcb->leTbdptr = (TBD*)MBUF_MARKER;  /* not used (our mbuf marker) */
#endif
		tcb->tcbcnt = TCB_EOF | mlen;
		tcb->zeroes = 0;
	} else {
		TBD *tbd, *ltbd;
		Mbuf ms;
		u_int16 ntbd;

		/*
		 * flexible mode, use pointers using TBDs
		 *
		 * assume NTBD is sufficient to map any mbuf passed by the called
		 * otherwise, the packet is discarded.   Also assume that an mbuf
		 * segment contains no more than 16k of data.  TBD count is limited.
		 * xmit mbuf consumption is limited by the number of CB entries.
		 */
		tbd = (TBD *)MAKE_CPU_ADDR((u_int32)tcb->tbd); /* point to first tbd */
		tcb->leTbdptr = (TBD*)MAKE_LOCAL_ADDR_SWAP((u_int32)tbd); /* store in TCB */
		tcb->tcbcnt = 0;		/* no bytes in TCB, but not EOF */
		tcb->zeroes = 0;

		ms = tcb->xmbuf = mesg;
		ltbd = 0;
		ntbd = NTBD;
		do {
			if (ms->m_size) {
				if (ltbd) {
					ltbd->tbd_leNext = (TBD*)MAKE_LOCAL_ADDR_SWAP((u_int32)tbd);
					ltbd->tbdcnt &= TBD_CNTMASK;
				}
				tbd->tbdcnt = TBD_EOF | ms->m_size;
				tbd->tbd_leData = (char *)MAKE_LOCAL_ADDR_SWAP(mtod(ms, u_int32));
				tbd->zeroes = 0;
				mlen -= ms->m_size;
				ltbd = tbd++;
			}
			ms = ms->m_pnext;
		} while (mlen && ms && ntbd--);

		if (mlen)	/* if short packet, bump size of last segment */
			ltbd->tbdcnt = TBD_EOF | ((ltbd->tbdcnt & TBD_CNTMASK) + mlen);
		xcmd |= XMIT_SF;
	}
#else
	/*
	 * PVTRAM, simplified mode: just copy the data into the tcb
	 * no tbd is used.
	 */
	tcb->leTbdptr = TBD_NOT_USED;	/* not used in simplified mode */
	tcb->tcbcnt = TCB_EOF | mlen;
	tcb->zeroes = 0;
	m_move(mesg, 0, plen, (char *)tcb->xmitdata);  /* copy data into tcb */
#if defined (Flushing)
	_os_cache(4);				/* Flush data cache */
#endif		
#endif

	if (!(add_failed = i82596CMDAdd(deventry, xcmd, (void*)cb, 0)))
		drstat->dr_iestat.xmit_count++;
	drstat->dr_iestat.xmit_trys++;

#ifdef PVTRAM
	m_free_p(mesg);		/* finished with mbuf data */
#else
	if (add_failed)
		m_free_p(mesg);	/* finished with mbuf data */
#if defined (Flushing)
	_os_cache(5);				/* Flush data cache and turn it on */
#endif		
#endif

	return add_failed;
}


/************************************************************************
*
* i82596MemInit - initializes shared memory
*
* This routine breaks up the shared memory into a SCP, ISCP, SCB and two
* ring lists:  the command block ring and the frame descriptor ring list.
* The first element in each ring is recorded in the SCB and device descriptors
* for later use.  The simplified memory model of the chip is being used.
*
* After this routine the shared memory block will appear as follows:
*
* +---------------------+	<--- lustat->lu_shram(must be 16 byte aligned)
* |   64 bytes zeroes   |
* +---------------------+
* |        SCP          |		SCP address must be 16 byte aligned
* +---------------------+
* |        ISCP         |
* +---------------------+
* |        SCB          |
* +---------------------+
* |        CB           |
* +---------------------+
* |        RFA          |	<---	ring buffer of received frames
* +---------------------+
* |        ...          |
* |        ...          |
* |        ...          |
* |        ...          |
* |        ...          |
* +---------------------+
* |                     |
* | XMIT DATA BUFFERS   |	<---	for PVTRAM only
* |                     |
* +---------------------+
*
*/


/*
 * return size of memory required by
 * i82596 for data structures.
 *
 * Make sure this matches with "initialize base pointers to structures"
 * close below in i82596meminit.
 */
u_int32 i82596totmem(Spf_lustat lustat)
{
	return
		sizeof(SCP) +
		sizeof(ISCP) +
		sizeof(SCB) +
		lustat->lu_ncbl * sizeof(CB) +
		lustat->lu_nrfd * sizeof(RFD);
}

error_code i82596meminit(Dev_list deventry)
{
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat);
	RFD *rfd, *lrfd;
	CB  *cb;
	u_int16 ncount;
	Mbuf mbuf;
	u_int32 status;
#ifndef PVTRAM
	RBD *rbd, *lrbd;
	Mbuf mlist;
#endif

	/* initialize base pointers to chip structures */
	lustat->lu_scp  = (SCP *) MAKE_P3_OFFSET(lustat->lu_shram);
	lustat->lu_iscp = (ISCP *)(lustat->lu_scp  + 1);
	lustat->lu_scb  = (SCB *) (lustat->lu_iscp + 1);
	lustat->lu_cb_base  = (CB *)  (lustat->lu_scb  + 1);
	lustat->lu_rfd_base = (RFD *) (lustat->lu_cb_base + lustat->lu_ncbl);

	DEBUG5((void *)0, (void *)lustat->lu_scp);
	DEBUG5((void *)1, (void *)lustat->lu_iscp);
	DEBUG5((void *)2, (void *)lustat->lu_scb);
	DEBUG5((void *)3, (void *)lustat->lu_cb_base);
	DEBUG5((void *)4, (void *)lustat->lu_rfd_base);

	/*
	 * ensure we run in linear mode.  & with ~SYSBUS_EBE to shut of enhanced
	 * (32-bit) big endian scheme, so that we retain use of the old 16 bit
	 * big-endian scheme.  SYSBUS_EBE was only implemented in STEP-B chips
	 */
	lustat->lu_scp->scp_sysbus =
		(lustat->lu_sysbus & ~(SYSBUS_MODEMASK | SYSBUS_EBE)) | SYSBUS_LINEAR;

	/* point SCP to ISCP for chip */
	lustat->lu_scp->scp_leISCP = (ISCP *)MAKE_LOCAL_ADDR_SWAP ((u_int32)lustat->lu_iscp);
	lustat->lu_scp->scp_beISCP = (ISCP *)MAKE_LOCAL_ADDR ((u_int32)lustat->lu_iscp);

	lustat->lu_iscp->iscp_busy = ISCP_NOT_BUSY;	/* initialize */

	/* point ISCP to SCB for chip */
	lustat->lu_iscp->iscp_leSCB = (SCB *)MAKE_LOCAL_ADDR_SWAP((u_int32)lustat->lu_scb);
	lustat->lu_iscp->iscp_beSCB = (SCB *)MAKE_LOCAL_ADDR((u_int32)lustat->lu_scb);

#if 0
	/* now setup the SCB */
	lustat->lu_scb->leRfa
		= (RFD *)MAKE_LOCAL_ADDR_SWAP((u_int32)lustat->lu_rfd_base);
	lustat->lu_scb->leCbl
		= (CB *)MAKE_LOCAL_ADDR_SWAP((u_int32)lustat->lu_cb_base);
#endif

	lustat->lu_scb->tonTimer = lustat->lu_t_on;
	lustat->lu_scb->toffTimer = lustat->lu_t_off;

	/* now setup the free CB list */
	for (cb = (CB *)MAKE_CPU_ADDR((u_int32)lustat->lu_cb_base)
       , lustat->lu_cb_free = NULL
       , ncount = lustat->lu_ncbl; ncount--; cb++) {
		cb->cb_leNext = (CB *)MAKE_LOCAL_ADDR_SWAP((u_int32)lustat->lu_cb_free);
		lustat->lu_cb_free = cb;
	}
	lustat->lu_cb_head = lustat->lu_cb_next = NULL;

#ifdef PVTRAM
	/*
	 * Allocate mbufs for receiving.  recv_irq will copy received frames
	 * from the chip's memory into these mbufs.
	 */
	for (ncount = lustat->lu_nrfd; ncount--; ) {
		status = get_mbuf (deventry, 1536 + sizeof (Dev_list), &mbuf);
		if (mbuf) {
			mbuf->m_qnext = lustat->lu_recvfree;
			lustat->lu_recvfree = mbuf;
		} else {
			m_free_q(&lustat->lu_recvfree);
			return status;
		}
	}
#else
	/*
	 * Allocate mbufs for data and RBDs.  The i82596 will receive data
	 * into the data portion of the mbufs.  Hopefully, only one mbuf will
	 * be required to hold the data.  There is plenty of space (1552 bytes) so
	 * even if a giant packet is received, there is little chance of spanning
	 * buffers.  Two extra mbufs are allocated so that there are extra ones to
	 * receive in while we are down to the last RFD.  We allocate only 2 extra
	 * on the assumption that not too many giant packets would be received.
	 */
	lrbd = NULL;
	mlist = NULL;
	for (ncount = lustat->lu_nrfd + 2; ncount--; ) {
#ifdef _MPFSH
		status = get_mbuf (deventry, RMBUF_SIZE + sizeof(Dev_list) + 2 , &mbuf);
#else
		status = get_mbuf (deventry, RMBUF_SIZE + sizeof(Dev_list), &mbuf);
#endif
		if (status == SUCCESS) {
			mbuf->m_offset += sizeof(Dev_list);
			rbd = (RBD *)MAKE_CPU_ADDR((u_int32)mtod(mbuf, RBD *));
			rbd->rbd_acount = rbd->rbd_zero1 = rbd->rbd_zero2 = 0;
			if (lrbd) {
				lrbd->rbd_leNext = (RBD *)MAKE_LOCAL_ADDR_SWAP((u_int32)rbd);
				lrbd->rbd_beNext = (RBD *)MAKE_LOCAL_ADDR((u_int32)rbd);
			} else {
				lustat->lu_rbd_head = (RBD *)MAKE_CPU_ADDR((u_int32)rbd);
			}
#ifdef _MPFSH
			rbd->rbd_leBuffer=(char *)MAKE_LOCAL_ADDR_SWAP((u_int32)(rbd+1)+2); /* and ALIGN */
#else
			rbd->rbd_leBuffer=(char *)MAKE_LOCAL_ADDR_SWAP((u_int32)(rbd+1));
#endif

			/* data stored after rbd */
			rbd->rbd_size = RMBUF_SIZE - sizeof(RBD);/* plenty of space */
			rbd->rbd_mbuf = (Mbuf)mbuf;
			mbuf->m_qnext = mlist;	/* keep linked list only in case... */
			mlist = mbuf;			/* ...of m_get failure in this loop */
			lustat->lu_recvfree = mbuf; /*for dr_term to free mbuf */
		} else {				/* ...in which case... */
			m_free_q(&mlist);		/* ...we end up here! */
			return status;
		}
		lrbd = rbd;
	}
	rbd->rbd_size |= RBD_LAST;
	rbd->rbd_leNext = rbd->rbd_beNext = NULL;
	lustat->lu_rbd_tail = rbd;

	lustat->lu_rbd_free = NULL;
	lustat->lu_rbd_fend = NULL;
#endif

	/* now setup the RFD list */
	lrfd = (void *)0;
	for (rfd = (RFD *)MAKE_CPU_ADDR((u_int32)lustat->lu_rfd_base),
		ncount = lustat->lu_nrfd; ncount--; rfd++) {
		rfd->rfd_stat1 = rfd->rfd_stat2 = 0;
		rfd->rfd_leNext = (RFD *)MAKE_LOCAL_ADDR_SWAP((u_int32)(rfd+1));
		rfd->rfd_beNext = (RFD *)MAKE_LOCAL_ADDR((u_int32)(rfd + 1));
		rfd->rfd_bePrev = (RFD *)MAKE_LOCAL_ADDR((u_int32)lrfd);
		rfd->rfd_acount = 0;
		rfd->rfd_leRbd = RFD_RBD_NOTUSED;
#ifdef PVTRAM
		rfd->rfd_size = ETHER_MAX;
#else
		rfd->rfd_size = 0;		/* no data stored in RFD */
		rfd->rfd_stat2 |= RFD_FLEXIBLE;	/* we're using flexible mode */
#endif
		lrfd = rfd;
	}

	/* make last one point to first */
	rfd--;
	rfd->rfd_stat2 |= RFD_LAST;		/* mark end of list for chip */
	rfd->rfd_leNext = (RFD *)MAKE_LOCAL_ADDR_SWAP((u_int32)lustat->lu_rfd_base);
	lustat->lu_rfd_head = (RFD *)MAKE_CPU_ADDR((u_int32)lustat->lu_rfd_base);
	rfd->rfd_beNext = (RFD *)MAKE_LOCAL_ADDR((u_int32)lustat->lu_rfd_base);
	lustat->lu_rfd_base->rfd_bePrev
		= (RFD *)MAKE_LOCAL_ADDR((u_int32)rfd); /* make first rfd point to last */

#ifndef PVTRAM
	/*
	 * first RFD points to head of RBD list
	 */
	lustat->lu_rfd_head->rfd_leRbd =
		(RBD *)MAKE_LOCAL_ADDR_SWAP((u_int32)lustat->lu_rbd_head);
#endif

#if defined (Flushing)
	_os_cache(4);               /* Flush data cache */
#endif
	return 0;
}

#if defined(sp167) || defined(sp162) || defined(spgenesis) || defined(sp177) || defined(sp172)

void doport_command(Spf_lustat lustat, u_int8* shram, u_int32 opcode)
{
	u_int16 *port = lustat->lu_port; 		/* a4 */
	u_int32 value = (u_int32)shram & ~0xf;	/* d4 */

	value |= opcode;		/* port command operation */

	nop();

	*port++ = value;		/* lo word */
	*port = (value>>16);	/* hi word */
#if defined (Flushing)
	_os_cache(4);			/* Flush data cache */
#endif
}

#elif defined(hkvf82596)

void doport_command(lustat, shram, opcode)
Spf_lustat lustat;		/* a2 */
u_int8 *shram;		/* a3 */
u_int8 opcode;
{
	u_int32 *port; 						/* a4 */
	u_int32 value = (u_int32)shram & ~0xf;	/* d4 */

	/*	flushCache(); */
	value |= opcode;		/* port command operation */

	port = lustat->lu_isV4d ?
	HV4D_PORT((u_int32)lustat->lu_port):
	CMSE_PORT((u_int32)lustat->lu_port);

@ swap d4
  *port = value;
  *port = value;
  *port = value;
  *port = value;
}

#elif defined(mz8554) || defined(ie7140)

void doport_command(lustat, shram, opcode)
Spf_lustat  lustat;		/* a2 */
u_int8 *shram;		/* a3 */
u_int8 opcode;
{
#if defined(ie7140)
	u_int32 *port = lustat->lu_port; 		/* a4 */
#else
	u_int16 *port = lustat->lu_port; 		/* a4 */
#endif
	u_int32 value = (u_int32)shram & ~0xf;	/* d4 */

	value |= opcode;		/* port command operation */

	nop();
	*port = (value>>16);
	*port = value;
}

#elif defined (spdve7708)
void doport_command (Spf_lustat lustat, u_int8 *shram, u_int32 opcode)
{
	u_int16 *port = lustat->lu_port;
	u_int32 value = (u_int32)shram & ~0xf;

	value = MAKE_LOCAL_ADDR(value);
	value |= opcode;

#if 1
	nop (); nop (); nop (); nop ();
#endif
	port[1] = value & 0xffff;
	port[0] = (value >>16) & 0xffff;

	return;
}
#endif	/* board type sp167/sp162/ifgenesis/sp177/hkvf82596/mz8554/ie7140 */

#if defined (_MPFSH)
u_int32 get_long (u_int16 *p)
{
	return p[0]<<16 | p[1];
}

int cmp_long (u_int16 *p, u_int32 val)
{
	return ((p[0] == (val >>16)&0xffff) && p[1] == val & 0xffff);
}

void copy_long (u_int16 *p, u_int32 val)
{
	p[1] = val >>16;
	p[0] = val & 0xffff;
}

u_int32 SWAPWORDS(u_int32 orig)
{
	return((orig>>16)|(orig<<16));
}

#endif

#if defined (spdve7708)
void led1 (Spf_lustat lustat, int v)
{
	u_int8 *dga = (u_int8 *)lustat->lu_dga;
	if (v)
		v = 0x80;
	else
		v = 0;
	dga[3] = (dga[3] & ~0x80) | v;
	return;
}

void led2 (Spf_lustat lustat, int v)
{
	u_int8 *dga = (u_int8 *)lustat->lu_dga;
	if (v)
		v = 0x40;
	else
		v = 0;
	dga[3] = (dga[3] & ~0x40) | v;
	return;
}
#endif
