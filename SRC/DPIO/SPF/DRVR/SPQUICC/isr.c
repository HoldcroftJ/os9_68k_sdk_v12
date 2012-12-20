/*****************************************************************************
 *  QUICC Driver misc/isr routines.                                          *
 *****************************************************************************
 *            Copyright 1989-1996 by Microware Systems Corporation           *
 *                            All Rights Reserved                            *
 *                          Reproduced Under License                         *
 *                                                                           *
 *  This software is confidential property of Microware Systems Corporation, *
 *  and is provided under license for internal development purposes only.    *
 *  Reproduction, publication, distribution, or creation of derivative works *
 *  in any form to any party other than the licensee is strictly prohibited, *
 *  unless expressly authorized in writing by Microware Systems Corporation. *
 *****************************************************************************
 *  Edition History:                                                         *
 *   #   Date       Comments                                           By    *
 *  --- --------    ----------------------------------------------- -------- *
 *  01 10/07/96 Creation                                                     *
 *  02 02/03/97 Pass our dev_entry in 4 bytes just before the data if   sr   *
 *              of the upgoing packet.                                       *
 *  04 02/06/97 In qe_xmit, don't free mbuf if we do not have desc.     sr   *
 *              In xmit_irq, before exiting check to see if there is         *
 *              enqueued data that can be sent out.                          *
 *  05 02/17/97 isr.c: Updated to not sub, but add offset for dev_entry pjw  *
 *     02/19/97 <***** SPF LAN Comm Pack v3.0 Release*********>              *
 *  07 05/09/97 isr.c: casted dev_entry to u_int32                      pjw  *
 *     05/09/97 <***** SPF LAN Comm Pack v3.1 Release*********>              *
 *  09 08/11/97 Added SCC channel dependent code                        taka *
 *  10 10/09/97 mbuf leak fix: set T_I bit for transmit status          taka *
 *  11 10/15/97 Added 68360 support                                     taka *
 *     10/22/97 <***** OS-9000 MBX BSP Release v1.0 **********>              *
 *  12 12/02/97 Added code to collect netstat data in lustat->if_data   jk   *
 *  14 01/21/98 Removed compiler and linker warnings                    jk   *
 *  15 03/05/98 Incorporated Uday's fix for mbuf leaks.                 jk/sr*
 *              In isr.c clear the scce event register, after processing     *
 *              the interrupt.  Call xmit_irq, if transmit error occurs.     *
 *              Call recv_irq in case of incomplete frames. (RXB status)     *
 *              Other code cleanup.                                          *
 *     04/02/98 <***** SPF LAN Comm Pack v3.2 Release*********>              *
 *  17 11/15/99 CF6721: fix transmit mbuf leak, improve performance          *
 *              in xmit_irq().                                          gjb  *
 *  19 02/10/00 Added support for limited transmit queue                rds  *
 *  21 05/11/00 Change logical or of bits in transmit irq to bit or     mgh  *
 *  24 08/29/00 Remove T_XX tests that didn't compile in ed 23          mgh  *
 *  25 12/07/00 CF9967: Added KeiT's changes:                           mdu  *
 *              - Changed CISR set for 68360                                 *
 *              - Invalidate the cache only for the received length,         *
 *                instead of the complete buffer (speed boost)               *
 *  26 02/19/01 Added C_ADDR to cache_cctl()				sbh  *
 ****************************************************************************/

#include <SPQUICC/defs.h>

/************************************************************************
		Transmit Routine
*************************************************************************/

/*
** this fn always returns SUCCESS, now.  The caller must ensure that there
** is an open buffer descriptor (lustat->next_tx_bd->buf == 0) before making
** this call.
*/
error_code qe_xmit( Dev_list dev_entry, Mbuf mb) {
	Spf_lustat	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	QUICC_BD  *nextxbd;
	/* QUICC_BD	*bd; */
	/* QUICC	*qp = lustat->qp; */ /* pointer to QUICC registers 	*/
	u_int32   channel = lustat->lu_channel;
	Mbuf      *mb_start;
	struct ethernet_pram *ep;	/* pointer to QUICC ethernet pram */

	ep = (struct ethernet_pram *)lustat->lu_port;

	DEBUG_DATA(lustat->lu_dbg, "Eth:Xmit", mb);

	nextxbd = lustat->next_tx_bd;


    /* for now, we don't need to check this because callee fns. are already
    ** doing this */
#if 0
	if (nextxbd->status & T_R) {	/* this bd is busy */

		/* If bd is busy don't pitch packet, return status so we can enqueue
		 * for retry.
		 */
		DEBUG_DATA(lustat->lu_dbg, "Eth:Busy", nextxbd->status);
		return(1);
	}
#endif

	/* Save pointer to start of mbuf.  To be used in the Xmit ISR to
	 * get to the beginning of the mbuf, so we can free it.
	 */

	mb_start = mtod(mb, Mbuf *);
	mb_start--;		/* Back up by a long to get to the storage area of ptr*/
	*mb_start = mb;	/* mb_start now points to beginning of mbuf. */

	nextxbd->buf = mtod(mb, unsigned char *);	/* point to data */

	nextxbd->length = mb->m_size;
	DEBUG_DATA(lustat->lu_dbg, "Xm mb_star", *mb_start);

	nextxbd->status &= (T_W | T_I);	/* clear any previous status */
	nextxbd->status |= T_R | T_PAD | T_L | T_C;	/* BONZAI */

	DEBUG_DATA(lustat->lu_dbg, "Xmitstat", nextxbd->status);

	if (nextxbd->status & T_W) {
		lustat->next_tx_bd = lustat->base_tx_bd;
	} else {
		lustat->next_tx_bd++;
	}

	if (nextxbd ==
		((QUICC_BD *)((unsigned)lustat->qp + BD_BASE + (unsigned)ep->tbptr))){
   	    /* Set transmit on demand register to have bd processed now */
		lustat->qp->scc_regs[channel].scc_todr = 0x8000;
	}

	/* increment if_data counter */
    lustat->if_opackets++;
    lustat->if_obytes += mb->m_size;

	return (0);
}

/***********************************************************************
		Interrupt Service Routine
************************************************************************/

#ifdef _OSK
/*
 *      interrupt service routine glue-code for OS-9
 */
_asm("
qe_isr:
  move.l  a2,d0           ; put Dev_list in a2 into d0
  bsr     isr             ; call interrupt service routine
  tst.l   d0              ; see if SUCCESS returned
  beq.s   qe_isr_exit     ; if so, return
  ori     #Carry,ccr      ; else E_NOTME returned--set carry bit
qe_isr_exit:
  rts                 ; return
");

#else	/* For OS9000 */

error_code qe_isr(Dev_list dev_entry) {
	return isr(dev_entry);
}

#endif

error_code isr(Dev_list dev_entry) {
	Spf_lustat  lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	QUICC       *qp = lustat->qp;	/* pointer to QUICC registers 	*/
	u_int32     channel = lustat->lu_channel;
	u_int16     le_status;

	if(lustat->lu_running) {

		if ((qp->intr_cipr & (INTR_SCC1>>channel)) == 0) {
			return (EOS_NOTME);
		}

#ifdef _OSK
        qp->intr_cisr = INTR_SCC1>>channel; /* Set bit to reset the ISR status */
#endif
		/* check for enet generated interrupts.  mask the event
		 * register against the mask register to take into
		 * account the possiblity that the interrupt is not
		 * enabled.
		 */
		le_status = qp->scc_regs[channel].scc_scce;
		if (le_status & qp->scc_regs[channel].scc_sccm) {
			DEBUG_DATA(lustat->lu_dbg, "Isr:MyIrq", le_status);

			/* handle the interrupts here
			 */

			/* receive interrupt 	*/
			if ((le_status & (ETHERNET_RXF | ETHERNET_BSY | ETHERNET_RXB)) != 0) {

				/* handle irq and clear it
				 */
				recv_irq(dev_entry);
			}

			/* transmit interrupt	*/
			if ((le_status & (ETHERNET_TXB | ETHERNET_TXE)) != 0) {

				/* handle irq and clear it
				 */
				xmit_irq(dev_entry);
			}
		}

		/* Clear the event register always*/
		lustat->qp->scc_regs[channel].scc_scce = le_status;
	} else {
		/* I am not running, how can I generate an interrupt? */
		return (EOS_NOTME);
	}

	return (SUCCESS);
}

/***********************************************************************
		Receive Interrupt
************************************************************************/

error_code recv_irq(Dev_list dev_entry) /* handle incomming packet */
{
	Spf_lustat  lustat = (Spf_lustat)(dev_entry->v_lu_stat);
#ifdef _OS9000
	Spf_drstat  drstat = (Spf_drstat)(dev_entry->v_dr_stat);
#endif
	Mbuf 	    mb, newmb;
        /*
	ENET_FRAME  *efp;
	*/
	QUICC_BD    *bd;
	u_int16	    channel = lustat->lu_channel;
 	int 	    bd_stat;
	u_int8      *pkt;

	DEBUG_DATA(lustat->lu_dbg, "RcvIrq", lustat);

	lustat->lu_qestat.recv_irq++;
	lustat->recv_scce = lustat->qp->scc_regs[channel].scc_scce;
	/* lustat->qp->scc_regs[channel].scc_scce = 0xd; */

	if (lustat->recv_scce & ETHERNET_BSY) {
		lustat->lu_qestat.recv_busy++;
#ifdef _OS9000
		lustat->lu_qestat.recv_ov++;
		lustat->qp->cp_cr = ENTER_HUNT_MODE + (channel << 6) + CMD_FLAG;
		while (lustat->qp->cp_cr & CMD_FLAG);
#endif
		lustat->if_ierrors++;
	}

	if (lustat->recv_scce & ETHERNET_RXB) {
		lustat->lu_qestat.recv_shrt++;
		lustat->if_ierrors++;
	}

	bd = lustat->next_rx_bd;

	/* Do while we own the buffer desc */
	while (((bd_stat = bd->status) & R_E) == 0) {

		if(((bd_stat & R_ERROR) == 0) && (lustat->lu_ioenabled == DRVR_IOEN)) {
			/* we only want data from packets with no errors */
			lustat->lu_qestat.recv_count++;
			/* If there is anything in the packet */
			if (bd->length >= 4) {
				mb = (Mbuf)((unsigned)bd->buf - (unsigned)sizeof(mbuf)
													- (2*sizeof(Dev_list)));
				/* We are out of mbuf's - trash the received one so we don't
				 * hang
				 */
				if (get_mbuf(lustat->lu_ether_drvr,
									   ETHERMAX + sizeof(Dev_list), &newmb) != SUCCESS) {
					DEBUG_DATA(lustat->lu_dbg, "NoMbuf", 0);
					newmb = mb;

					/* reset the header fields. */
					newmb->m_offset = sizeof(mbuf) + sizeof(Dev_list);
					newmb->m_size   = 0;
					newmb->m_flags  = MB_DATA;
				} else {

					mb->m_size = bd->length - 4;	/* we don't need crc */

					DEBUG_DATA(lustat->lu_dbg, "Recv up   ", mb);
					DEBUG_STRING(lustat->lu_dbg, mtod(mb, u_char *), mb->m_size);

					if (lustat->lu_ether_drvr == NULL) {
						m_free_p(mb);
					} else {
						/* Before giving the packet, stash our dev_entry ptr
						 * so spenet finds it.
						 */
						u_int32 *mb_start;

#ifdef _OS9000
                		/* Invalidate only packets going up and invalidate only the packet length
                        ** for the received data (DMA) 
                        */
		                drstat->cache_cctl(C_INVDATA | C_ADDR, (void *)bd->buf, bd->length);
#endif

						mb_start = (u_int32 *)(((int)mb + MBHDR_SZ + sizeof(void *)));

						/* Now save the dev_entry pointer itself. */
						*mb_start = (u_int32)dev_entry;

						/* update if_data statistics */
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
					 	 * "hardware" driver use DR_FMCALLUP_PKT, not
						 * SMCALL_UPDATA
					 	 */
						DEBUG_DATA(lustat->lu_dbg, "Send up   ", mb);
						DR_FMCALLUP_PKT(dev_entry, lustat->lu_ether_drvr, mb);

						/* increment if_data structure */
						lustat->if_ipackets++;
					}
				}
				/* save space in pre mbuf_data part to pass our dev_entry ptr
				 * to spenet.
				 */
				newmb->m_offset += sizeof(Dev_list);
				bd->buf = mtod(newmb, u_char *);
			} else {
				/* bd->length < 4 */
				DEBUG_DATA(lustat->lu_dbg, "Recv len<4", bd->buf);
			}
		} else {
			/* If we get here, the packet was not good. */
			if(bd_stat & R_LG)
				lustat->lu_qestat.recv_lg++;
			if(bd_stat & R_NO)
				lustat->lu_qestat.recv_align++;
			if(bd_stat & R_SH)
				lustat->lu_qestat.recv_short++;
			if(bd_stat & R_CR)
				lustat->lu_qestat.recv_crc++;
			if(bd_stat & R_OV)
				lustat->lu_qestat.recv_ov++;
			if(bd_stat & R_CL)
				lustat->lu_qestat.recv_col++;
			/* score only one error in if_data structure */
			lustat->if_ierrors++;

		}
		bd->length = 0;
		bd->status &= R_W;			/* clear any previous errs */
		bd->status |= R_E|R_I;			/* mark as ready */

		if(bd->status & R_W)
			lustat->next_rx_bd = lustat->base_rx_bd;
		else
			lustat->next_rx_bd++;
		bd = lustat->next_rx_bd;
	}

	return(0);
}

/***********************************************************************
		Transmit Interrupt
************************************************************************/

error_code xmit_irq(Dev_list dev_entry) /* handle outcomming packet */
{
	Spf_lustat  lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	Mbuf        *mb_start;
	QUICC_BD    *bd;
	u_int16     channel = lustat->lu_channel;
	int 	      err = 0;

	DEBUG_DATA(lustat->lu_dbg, "XmtIrq", lustat);
	lustat->lu_qestat.xmit_irq++;

	lustat->xmit_scce = lustat->qp->scc_regs[channel].scc_scce;
	/* lustat->qp->scc_regs[channel].scc_scce = 0x12; */

	if (lustat->xmit_scce & ETHERNET_TXE) {
		lustat->lu_qestat.xmit_err++;
		err++;
	} else
		lustat->lu_qestat.xmit_count++;

	bd = lustat->trans_bd;

	/* Do until we catch up and clear all xmitted bds */

	/* Uday's change */
	do {

		if((bd->status & T_R) == 0) {
			if (bd->status & (T_UN | T_CSL | T_RL | T_LC | T_DEF)) {
				if(bd->status & T_CSL) {
					lustat->lu_qestat.xmit_lcar++;
					err++;
				}
				if(bd->status & T_UN) {
					lustat->lu_qestat.xmit_udma++;
					err++;
				}
				if (bd->status & T_RL) {
					lustat->lu_qestat.xmit_rlim++;
					err++;
				}
				if(bd->status & T_LC) {
					lustat->lu_qestat.xmit_lcol++;
					err++;
				}
				if(bd->status & T_DEF) {
					lustat->lu_qestat.xmit_defr++;
					err++;
				}
			}
			/*
			 * Now find the pointer to the mbuf.  As set above, we find the
			 * pointer to the mbuf in the longword immediately preceeding
			 * the data area.
			 */
			mb_start = (Mbuf *)bd->buf;

			mb_start--;	/* Get to the actual pointer area. */
			m_free_p(*mb_start);

			bd->length = 0;
			bd->buf = 0;
			/* clear any previous status */
			write_word((void *) &bd->status,
					read_word((void *) &bd->status) & T_W);
			/* Set status bits */
			write_word((void *) &bd->status,
					read_word((void *) &bd->status) | T_PAD | T_L | T_C | T_I);

		} else {
			break;
		}

		if (bd->status & T_W) /* End of xmit bds */
			bd = lustat->base_tx_bd;
		else
			bd++;		/* next bd */
	} while (bd != lustat->next_tx_bd);

	/* Save the trans_bd value, should be next_tx_bd */

	lustat->trans_bd = bd;

 /* if an error occured, restart the transmission - see page 7-261 */
	if (err) {
		while (lustat->qp->cp_cr & CMD_FLAG)
			;
		lustat->qp->cp_cr = RESTART_TX + (channel << 6) + CMD_FLAG;	/* restart xmit */
		while (lustat->qp->cp_cr & CMD_FLAG)
			;

		/* increment if_data oerror counter */
		lustat->if_oerrors +=err;
	}

	/* Any enqueued mbufs to go and is the next_tx_bd (not trans_bd) free? */
	while (lustat->lu_outq && (lustat->next_tx_bd->buf == 0)) {
		DEBUG_DATA(lustat->lu_dbg, "Xm m_deq",lustat->lu_outq);
		(void) qe_xmit(dev_entry, m_deq(&lustat->lu_outq));
		lustat->lu_qsize--;
	}

	return(0);
}
