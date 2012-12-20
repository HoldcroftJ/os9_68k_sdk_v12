/****************************************************************************
 *  $Workfile:   isr.c  
 *  $Revision:   0.1 
 ****************************************************************************
 *  SPF Device Driver ISR for use with the LANCE ethernet chip.		    *
 ****************************************************************************
 *  Copyright 1996 by BWI                      				*
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  BWI, and is provided to licensee		    			*
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited, by BWI                           *
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date       Comments                                           By   *
 *  --- --------    ----------------------------------------------- ------- *
 *    0 26FEB95     Creation                                           bw   *
 *    4 09/03/96    pop and push bug fix
 *   6 01/22/97 Pass our dev_entry in 4 bytes just before the data  of  sr  *
 *              of the upgoing packet.                                      *
 *   7 01/27/97 While doing get_mbuf, get 4 extra bytes, that way our   sr  *
 *              data space is not reduced because of the dev_list ptr.      *
 *   9 02/17/97 isr.c: Updated to not sub, but add offset for dev_entry pjw *
 *     02/19/97    <***** SPF LAN Comm Pack v3.0 Release*********>          *
 *     04/17/97 typecast dev_entry to U-inet32 for mb_start assign     tmd  *
 *     05/05/97    <***** SPF LAN Comm Pack v3.1 Release*********>          *
 *  10 10/23/97 Added/Uncommented BSD statistics keeping                tmd *
 *              Set broadcast/multicast bit in mbuf                     tmd *
 *  11 01/08/98  Removed unused params, init_rmds, allocate_recv_mbuf. 
 *               Removed unused variable, allocate_recv_mbuf.            jk *
 *     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              *
 ****************************************************************************/

#include <SPLANCE/defs.h>

void recv_irq(Dev_list dev_entry);
void xmit_irq(Dev_list dev_entry);

void swabytes( void *data)
{
	u_int8	*byte_ptr,
			temp;

	byte_ptr = (u_int8 *)data;
	temp = byte_ptr[0];
	byte_ptr[0] = byte_ptr[1];
	byte_ptr[1] = temp;
	return;
}


/*************************************************************************
 *  Call file manager to process a received mbuf
 ************************************************************************/
error_code process_mbuf(Dev_list dev_entry, Mbuf mb)
{
	Spf_lustat lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	u_int32    *mb_start;
	u_int8     *pkt;

#if 0
	if (mb == mb->m_qnext)  _os_sysdbg((void *)dev_entry, (void *)mb);
#endif

	/* check for valid mbuf and valid updrvr */
	if ((mb != NULL)) {
		if (lustat->lu_ether_drvr == NULL) {
			m_free_p(mb);
		} else {
#ifndef LCLMEM
			Dev_list *new_de_ptr;

			new_de_ptr = (Dev_list*)(mtod(mb, char *) - sizeof(Dev_list));
/*			*new_de_ptr = lustat->lu_updrvr; */
			*new_de_ptr = lustat->lu_ether_drvr; 
#endif
			/* Before giving the packet, stash our dev_entry ptr so
             * spenet finds it.   
             */
			mb_start =(u_int32 *)(((int)mb + MBHDR_SZ +sizeof(void *)));
        
			/* Now save the dev_entry pointer itself. */
			*mb_start = (u_int32) dev_entry;

			/* update input statistics */
			/* is this a broadcast? */
			pkt = mtod(mb, u_int8 *);
			if ((pkt[0] == 0xff) && (pkt[1] == 0xff) && (pkt[2] == 0xff) &&
			    (pkt[3] == 0xff) && (pkt[4] == 0xff) && (pkt[5] == 0xff)) {
				mb->m_flags |= M_BCAST;
				lustat->if_imcasts++;

			/* is this a multicast? */
			} else if (pkt[0] & 1) {
				mb->m_flags |= M_MCAST;
				lustat->if_imcasts++;
			}
			
			/* call file manager driver call-up routine to handle
			 * incoming data. Since the mbuf is coming from a
			 * "hardware" driver use DR_FMCALLUP_PKT, not SMCALL_UPDATA
			 */
			DR_FMCALLUP_PKT(dev_entry, lustat->lu_ether_drvr, mb);
		}
		return(SUCCESS);
	} else {
		return(!SUCCESS);
	}
}



/***********************************************************************
 *      		Interrupt Service Routine
 ***********************************************************************/

/*
 *      interrupt service routine glue-code for OS-9
 */
_asm("
isr_os9:
	move.l  a2,d0				; put Dev_list in a2 into d0
	bsr     isr				; call interrupt service routine
	tst.l   d0				; see if SUCCESS returned
	beq.s   isr_os9_exit			; if so, return
	ori     #Carry,ccr			; else E_NOTME returned--set carry bit
isr_os9_exit:
	rts					; return
");

error_code	isr(Dev_list dev_entry)
{
	Spf_lustat	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	Ifs     	ifstat = (Ifs)(&lustat->ifstat);
	u_int16 	le_status;

/* did the lance cause this irq? */
#ifdef VME147
	if ((*((u_char *)LANICR) | LANICR_IRQ) == 0) {
		ifstat->v_lestat.unk_irq++;
		return 0;
	}
#endif
	if (((le_status = ifstat->v_leregs->rdp) & LE_INTR) == 0) {
		ifstat->v_lestat.unk_irq++;
		return (EOS_NOTME);		/* nope */
	}


/*	do 
	{
*/

	if (le_status & (LE_RINT | LE_MISS)) {
       		recv_irq(dev_entry);
        }
        /* Check for transmit */
	if (le_status & LE_TINT) {
		xmit_irq(dev_entry);
	}

/* error interrupt? */
	if (le_status & LE_ERR) {
		if (le_status & LE_BABL) {
			ifstat->v_lestat.err_babble++;
			lustat->if_oerrors++;
		}
		if (le_status & LE_CERR) {
			ifstat->v_lestat.err_lcar++;
			lustat->if_oerrors++;
		}
		if (le_status & LE_MISS) {
			ifstat->v_lestat.err_missed++;
			lustat->if_ierrors++;
#ifdef DONT_DO_THIS
/* This will cause an MBUF leak. The only pointer to the MBUFS already
allocated is in the rmd. If we overwrite the pointer by doing init_rmds
without returning the mbuf that has been previously allocated, the
previously allocated mbuf will be lost.
*/
#ifndef LCLMEM
			init_rmds(ifstat);	/* prime the rmd's again */
#endif
#endif
		}
		ifstat->v_leregs->rdp = (LE_BABL | LE_MISS | LE_MERR | LE_CERR);
	}

/* ensure IRQ's are enabled for the device */
	ifstat->v_leregs->rdp = LE_INEA;
	return SUCCESS;
}

/***********************************************************************
 *                      Load tmd
 ***********************************************************************/
/*
        load_tmd - put mbuf into a xmit slot.
        Return 1 if one was available or 0 if not.
*/

int load_tmd(Ifs ifstat, Mbuf mb)
{
	Le_md mdp;
	Mbuf *mb_start;
#ifdef LCLMEM
	register char *dbuf;
	u_int32 ncnt;
#endif

#if defined(DATA_CACHE)
	flush_datacache();
#endif
	if ((mdp = ifstat->v_xmit_head) != ifstat->v_xmit_tail) {	/* tmd free? */
		if (ifstat->v_xmit_tail == NULL)
			ifstat->v_xmit_tail = ifstat->v_xmit_head;	/* mark head slot for irq service */
		if (++ifstat->v_xmit_head >= ifstat->v_xmit_rend)
			ifstat->v_xmit_head = &(*ifstat->v_xmit_ring)[0];
#ifndef LCLMEM
		/*
		 * At this point we run into a problem.  Since the lance wants a
		 * pointer to the data and not to the mbuf, we give it that.  But
		 * later when interrupt service wants to free the mbuf, there is
		 * only a pointer to the data.  Since this may not have any
		 * predictable relationship to the actual location of the mbuf,
		 * we will put the pointer to the mbuf in the longword immediately
		 * preceeding the data area which was allocated as a header (txoffset).
		 * we will free the mbuf immediately after the lance transmits the
		 * data.
		 */
		/* put data pointer in ring _first_ */
		mdp->md_loaddr = mtod(mb, u_int32);			/* lo 16-bits of addr */
		mdp->md_hiaddr = (mtod(mb, u_int32)) >> 16;	/* hi 8-bits of addr */
		/* then set pointer to mbuf! */
		mb_start = mtod(mb, Mbuf *);
		mb_start--;
		*mb_start = mb;
		mdp->md_bcnt = -(u_int32)mb->m_size;			/* msg size */
#else
		dbuf = (char *)((mdp->md_loaddr | (mdp->md_hiaddr << 16) |
						ifstat->v_hiaddr) +
						ifstat->v_lan_adj);
		(void)m_move(mb, 0, ncnt = m_len_p(mb), dbuf);
		mdp->md_bcnt = -(ncnt);					/* msg size */
		m_free_p(mb);
#endif
		mdp->md_mcnt = 0;								/* error summary */
		mdp->md_flag = (MD_OWN | MD_STP | MD_ENP);		/* bonzai! */
		ifstat->v_leregs->rdp = LE_TDMD | LE_INEA;		/* xmit now! */
	} else mdp = NULL;
	return ((int)mdp);
}

/***********************************************************************
 *                      xmit interrupt
 ***********************************************************************/


void xmit_irq(Dev_list dev_entry)
{
	Spf_lustat	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	Ifs			ifstat = (Ifs)(&lustat->ifstat);

	register Le_md mdp;
	register Mbuf *mb_start;
/* 	register unsigned char idx = 0; */

/* this can happen if the LANCE clears the OWN bit in the xmd and
   we eat it before it posts the IRQ.  Nothing really wrong here
   so just nop this irq.
*/
	if (ifstat->v_xmit_tail == NULL)
		ifstat->v_lestat.no_tail_irqs++;

	while (mdp = ifstat->v_xmit_tail) {
		if (mdp->md_flag & MD_OWN)
			break;	/* lance not finished */

		if (mdp->md_flag & MD_ERR) {			/* xmit error occurred? */
#ifdef not_used
			if (mdp->md_tmd3 & (MD_XBUF | MD_UFLO)) {
				*((char*)0xfec04000) = 0xc0;
				while(1);
			}
#endif
			if (mdp->md_tmd3 & MD_XBUF)
				ifstat->v_lestat.err_tbuf++;
			if (mdp->md_tmd3 & MD_UFLO)
				ifstat->v_lestat.err_uflo++;
			if (mdp->md_tmd3 & MD_LCOL)
				lustat->if_collisions = ++ifstat->v_lestat.err_lcol;
			if (mdp->md_tmd3 & MD_LCAR)
				ifstat->v_lestat.err_lcar++;
			if (mdp->md_tmd3 & MD_RTRY)
				ifstat->v_lestat.err_retry++;
			lustat->if_oerrors++;
		} else {		/* successful transmission */
			if (mdp->md_flag) {
				lustat->if_opackets = ++ifstat->v_lestat.xmit_count;
				if (mdp->md_flag & MD_MORE)
					ifstat->v_lestat.err_more++;
				if (mdp->md_flag & MD_ONE)
					ifstat->v_lestat.err_one++;
				if (mdp->md_flag & MD_DEF)
					ifstat->v_lestat.err_defer++;
			} else {
				ifstat->v_lestat.already_seen++;	/* ??? huh ??? */
			}
		}
#ifndef LCLMEM
		/*
		 * Now find the pointer to the mbuf.  As set above, we find the
		 * pointer to the mbuf in the longword immediately preceeding the
		 * data area.
		 */
		mb_start = (Mbuf *)(ifstat->v_hiaddr |
				((u_int32)mdp->md_hiaddr << 16) |
				 (u_int32)mdp->md_loaddr);
		#ifdef DEBUG
		    debug_data(lustat->lu_dbg, "LE  XmtIrq", (u_int32)mb_start);
		#endif

		mb_start--;			/* convert to mbuf pointer */

		/* m_free_p just in case there are any stragglers */
		#ifdef DEBUG
		    debug_data(lustat->lu_dbg, "LE  FreeMb", (u_int32)*mb_start);
		#endif

		m_free_p(*mb_start);
#endif
		mdp->md_flag = 0;
		if (++ifstat->v_xmit_tail >= ifstat->v_xmit_rend)
			ifstat->v_xmit_tail = &(*ifstat->v_xmit_ring)[0];
		if (ifstat->v_xmit_tail == ifstat->v_xmit_head)
			ifstat->v_xmit_tail = NULL;
	}

/* any enqueued mbufs ready to go? */
	while (ifstat->v_outq && ifstat->v_xmit_head != ifstat->v_xmit_tail)
		load_tmd(ifstat, m_deq(&ifstat->v_outq));

	ifstat->v_leregs->rdp = LE_TINT | LE_INEA;	/* clear xmit irq */
	ifstat->v_lestat.xmit_irq++;
}

/***********************************************************************
 *                      recv interrupt
 ***********************************************************************/

/*************************************/
#ifndef LCLMEM
/*
	Allocate and initialize receive message descriptor
*/
Mbuf allocate_recv_mbuf(Le_md mdp, int16 size)
{
	Mbuf mb;
	char *addr;
	
 	if ((get_mbuf(DUMMY_UPDRVR, size + sizeof(Dev_list), &mb))
								   == SUCCESS) {
		/* Increment the offset by 4, to save space for stashing
		 * our dev_entry for spenet.
		 */
		mb->m_offset += sizeof(Dev_list);
		addr = mtod(mb, char *);
		mdp->md_loaddr = (u_int32)addr;			/* stuff lo 16-bits */
		mdp->md_hiaddr = ((u_int32)addr) >> 16;		/* stuff hi  8-bits */
		mdp->md_bcnt = -(u_int32)size;			/* lance buffer size */
		mdp->md_flag = MD_OWN;				/* give to lance */
  	} else nop();						/* breakpoint post */
	return mb;
}

/*************************************/
/*
	Place a receive mbuf on a rmd entry.  Called at initialization time
	and by irq service when mbuf error occurs.
*/
error_code init_rmds(Ifs ifstat)
{
	Le_md mdp;
	u_char ncmd;
	/* u_char ncnt = ifstat->v_rpcnt; */
	
	mdp = &(*ifstat->v_recv_ring)[0];

#if defined(DATA_CACHE)
	flush_datacache();
#endif
	for (ncmd = 0; ncmd < ifstat->v_rpcnt;  ncmd++, ++mdp)
		if (mdp->md_flag == 0 && allocate_recv_mbuf(mdp, LANCE_ETHER_MAX) == NULL)
			return ENOBUFS;
	return 0;
}
#endif /* !LCLMEM */

/*************************************/
void recv_irq(Dev_list dev_entry)
{
    Spf_lustat	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
    Ifs			ifstat = (Ifs)(&lustat->ifstat);

	Le_md mdp, mds;
	Mbuf mb;
#ifdef notdef
	auto int32 netin_event = ifdev->if_man->v_netin_event;
#endif
/*	int32 oldval = 0; */
/* 	unsigned short status_reg = ifstat->v_leregs->rdp; */
	int iter = 0;
	u_short temp_flag;
#ifdef LCLMEM
	char *dbuf;
	int32 status;
#endif
		
	mds = mdp = ifstat->v_recv_head;
	do {
#if defined(DATA_CACHE)
		flush_datacache();
#endif
		if (((temp_flag = mdp->md_flag) & MD_OWN) == 0) {/* something for us? */
			if (temp_flag) {
				if (temp_flag & MD_ERR) {	/* error in packet? */
					if (temp_flag & MD_FRAM) ifstat->v_lestat.err_frame++;
					if (temp_flag & MD_CRC)  ifstat->v_lestat.err_crc++;
					if (temp_flag & MD_OFLO) ifstat->v_lestat.err_oflo++;
					if (temp_flag & MD_RBUF) ifstat->v_lestat.err_rbuf++;
					lustat->if_ierrors++;
					mdp->md_flag = MD_OWN;		/* give buffer back to lance */
				} else { /* no error in packet */
					if ((temp_flag & (MD_STP|MD_ENP)) == (MD_STP|MD_ENP)) {
#ifdef LCLMEM
						if ((lustat->lu_ioenabled == DRVR_IOEN) &&
/*						(status = get_mbuf(lustat->lu_updrvr, mdp->md_mcnt, &mb)) == SUCCESS) { */
						(status = get_mbuf(lustat->lu_ether_drvr,
							mdp->md_mcnt + sizeof(Dev_list), &mb)) == SUCCESS) {
							/* inc m_offset to make room for dev_entry ptr */
							mb->m_offset += sizeof(Dev_list);
							/* length of message */
							/* take off last 4 bytes, which used as LANCE FCS(CRC) */
							mb->m_size = mdp->md_mcnt - 4;	

							dbuf = (char *) ((mdp->md_loaddr | 
										(mdp->md_hiaddr << 16) |
										ifstat->v_hiaddr ) +
										ifstat->v_lan_adj);
							memcpy(mtod(mb, char *), dbuf, mdp->md_mcnt);
							mdp->md_flag = MD_OWN;		/* give buffer back to lance */
							lustat->if_ipackets = ++ifstat->v_lestat.recv_count;

							/* Call file manager to process received mbuf */
							process_mbuf(dev_entry, mb);
						} else {
							/* no memory for a new mbuf or no updrvr */
							ifstat->v_lestat.no_uplink++;
							mdp->md_flag = MD_OWN;		/* give buffer back to lance */
						}
#else /* ifndef LCLMEM */
						/* Subtract 2 times the sizeof(Dev_list) for the two
						 * dev_entry pointers.
						 */
						if (lustat->lu_ioenabled == DRVR_IOEN) {
							mb = (Mbuf)(ifstat->v_hiaddr |
							((u_int32)mdp->md_hiaddr << 16) |
							(u_int32)mdp->md_loaddr -
							(2 * sizeof(Dev_list)));
							mb--;			/* back up to mbuf header */
							/* length of message */
							/* take off last 4 bytes, which used as LANCE FCS(CRC) */
							mb->m_size = mdp->md_mcnt - 4;	
							mdp->md_flag ^= (MD_STP | MD_ENP);
							mdp->md_hiaddr = 0xf0;		/* zap the... */
							mdp->md_loaddr = 0xdead;	/* ...pointer address */

							lustat->if_ipackets = ++ifstat->v_lestat.recv_count;

							/* Call file manager to process received mbuf */
							process_mbuf(dev_entry, mb);
							allocate_recv_mbuf(mdp, LANCE_ETHER_MAX);
						} else {
							/* no updrvr */
							ifstat->v_lestat.no_uplink++;
							mdp->md_flag = MD_OWN;		/* give buffer back to lance */
						}							
#endif /* ifdef LCLMEM */
					} else {
						/*
							chained packet received!  This is not expected
							and the only explaination is that someone sent
							us a packet larger than if_mtu.  This driver and
							the observed behaviour of BSD4.3 drivers limit
							the ethernet packet sizes to 1500 bytes.
							
							For now, ignore the packet...
						*/
						ifstat->v_lestat.err_bogus++;
						lustat->if_ierrors++;

						mdp->md_flag = MD_OWN;
#ifdef not_used
						ifstat->v_leregs->rdp = LE_TSTOP;
						*((char *)0xfec04000) = 0xe0;
						while (1) ;
#endif
					} /* done with non-error packet */
				} /* done with packet; error or not */
#ifdef LCLMEM
			} else mdp->md_flag = MD_OWN;	/* shouldn't happen, but... */
#else
			} else allocate_recv_mbuf(mdp, LANCE_ETHER_MAX); /* this md needs an mbuf*/
#endif
		} /* else still owned by lance, ignore */
		if (++mdp >= ifstat->v_recv_rend)		/* next mdp */
			mdp = &(*ifstat->v_recv_ring)[0];
	iter++;
	} while (mdp != mds);

	ifstat->v_recv_head = mdp;			/* next one to look at */
	ifstat->v_leregs->rdp = LE_RINT | LE_INEA;/* clear the recv interrupt */
	ifstat->v_lestat.recv_irq++;
}
