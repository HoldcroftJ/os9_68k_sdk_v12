/*****************************************************************************
 * QUICC Driver Init routine.                                                *
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
 *  02 02/03/97 Save space in our mbuf for passing dev_list ptr up.    sr    *
 *     02/19/97 <***** SPF LAN Comm Pack v3.0 Release*********>              *
 *     05/05/97 <***** SPF LAN Comm Pack v3.1 Release*********>              *
 *  09 08/11/97 Added SCC channel dependent code                        taka *
 *  10 09/24/97 added OS9 68360 support                                 taka *
 *     10/22/97 <***** OS-9000 MBX BSP Release v1.0 **********>              *
 *  12 11/25/97 added code to init lustat->lu_item_addr for netstat       jk *
 *  14 01/21/98 removed compiler and linker warnings                      jk *
 *  15 03/05/98 Invalidate mbuf data area, even if CACHE_INHIBITED_BUFS jk/sr*
 *              is defined, since this is not in the inhibited area.         *
 *     04/02/98 <***** SPF LAN Comm Pack v3.2 Release*********>              *
 *  19 02/10/00 Changed incorrect write_longs to write_words            rds  *
 *              Enable ETHERNET_BSY                                     rds  *
 *  20 02/10/00 Removed extra argument to get_eaddr()                   kimk *
 *              Reworked init parameters slightly                            *
 *  26 02/19/01 Added C_ADDR to cache_cctl()				sbh  *
 ****************************************************************************/

/*
  * MPC821 <-> MC68160 Connections:
  *
  * Setup MPC821 to work with MC68160 Enhanced Ethernet
  * Serial Tranceiver as follows:
  *
  * MPC821 Signal                MC68160  Comments
  * PA-14 ETHTX      -------->   TX       Eth. Port Transmit Data
  * PB-19 E_TENA     -------->   TENA     Eth. Transmit Port Enable
  * PA-7 ETHTCK      <--------   TCLK     Eth. Port Transmit Clock
  * PA-15 ETHRX      <--------   RX       Eth. Port Receive Data
  * PC-10 E_RENA     <--------   RENA     Eth. Receive Enable
  * PA-6 ETHRCK      <--------   RCLK     Eth. Port Receive Clock
  * PC-11 E_CLSN     <--------   CLSN     Eth. Port Collision Indication
  * (BCSR1) ETHEN*   -------->   CS2      Eth. Port Enable
  * PC-6 TPSQEL*     -------->   TPSQEL   Twisted Pair Signal Quality Error Test Enable
  * PC-5 TPFLDL*     -------->   TPFLDL   Twisted Pair Full-Duplex
  * PC-4 ETHLOOP     -------->   LOOP     Eth. Port Diagnostic Loop-Back
  *
  */

#include <SPQUICC/defs.h>

#ifndef RXFCR
#define RXFCR	0x18		/* RX byte order, MPU-specific FCR or AT bits for SDMA */
#endif
#ifndef TXFCR
#define TXFCR	0x18		/* TX byte order, MPU-specific FCR or AT bits for SDMA */
#endif

error_code init_hdw(Dev_list deventry) {
	Spf_lustat 	lustat = (Spf_lustat)(deventry->v_lu_stat);
#ifdef _OS9000
	Spf_drstat  drstat = (Spf_drstat)(deventry->v_dr_stat);
#endif
	QUICC_BD 	  *bd;			/* buffer descriptor pointer 	*/
	QUICC		    *qp;			/* pointer to QUICC registers 	*/
	int 		    i, status = 0;
	Mbuf        mbptr;			/* mbuf pointer					*/
	u_int32     channel;		/* which SCC Channel number		*/
	u_int8      eaddr[6];		/* For Ethernet address */
	struct ethernet_pram *ep;	/* pointer to QUICC ethernet pram */

	ep = (struct ethernet_pram *)lustat->lu_port;
	lustat->qp = qp = (QUICC*)((unsigned)ep  & PRAMSCC_MASK);
												/* point to quicc registers */

	lustat->lu_channel = channel = ((u_int32)ep>>8) & 0x3;
							/* channel 0,0x1c00; channel 1,0x1d00 */
                            /* for 68360, channel 2,0xe00 ; channel 3,0xf00 */
							/* for   860, channel 2,0x1e00; channel 3,0x1f00 */

#ifdef CHECK_LOCK
	{
		error_code	status;

		if (status = getLock(lustat, channel))
			return status;
	}
#endif
		
	/* just in case shut down ethernet */
	write_long((void *) &qp->scc_regs[channel].scc_gsmra, 0x1088000c);

#ifndef NO_CONFIG_PINS
	/* first set up the various bits of the port registers */
	config_pins( qp, channel );
#endif
#ifndef NO_CONFIG_ETRX
	/* now set up the tranceiver - this is board dependant */
	config_ETRX( ETRX_TR_EN, qp, channel );
#endif

	/* next step - set up the parameter RAM 	*/

	write_word((void *) &ep->rbase, BD_SCC1_Eth +(channel * BD_Eth_Size));
									/* base of bd area, this channel, Ethernet*/
	write_word((void *) &ep->tbase, BD_SCC1_Eth +(channel * BD_Eth_Size) +
										lustat->lu_nrdesc*sizeof(QUICC_BD));
	write_word((void *) &ep->tbptr, BD_SCC1_Eth +(channel * BD_Eth_Size) +
										lustat->lu_nrdesc*sizeof(QUICC_BD));
	write_byte(&ep->rfcr, RXFCR);					/* mot byte ordering		*/
	write_byte(&ep->tfcr, TXFCR);					/* mot byte ordering		*/
	write_word((void *) &ep->mrblr, 0x5f0);			/* max receive buffer len	*/
	write_word((void *) &qp->cp_cr, INIT_RXTX_PARAMS+(channel << 6) + CMD_FLAG);	/* init params 	*/
	write_long((void *) &ep->c_mask, 0xdebb20e3);	/* 32 bit ccitt crc 		*/
	write_long((void *) &ep->c_pres, 0xffffffff);	/* 32 bit ccitt crc 		*/
	write_long((void *) &ep->crcec, 0);				/* crc error counter 		*/
	write_long((void *) &ep->alec, 0);				/* clignment error counter 	*/
	write_long((void *) &ep->disfc, 0);				/* discard frame err counter*/
	write_word((void *) &ep->pads, 0x8888);			/* short frame pad chars 	*/
	write_word((void *) &ep->ret_lim, 15);			/* retry limit 				*/
	write_word((void *) &ep->mflr, 1518);			/* max frame length 		*/
	write_word((void *) &ep->minflr, 64);			/* min frame length			*/
	write_word((void *) &ep->maxd1, 1520);			/* max dma1 length 			*/
	write_word((void *) &ep->maxd2, 1520);			/* max dma2 length 			*/
	write_word((void *) &ep->gaddr1, 0);			/* group address filter 	*/
	write_word((void *) &ep->gaddr2, 0);
	write_word((void *) &ep->gaddr3, 0);
	write_word((void *) &ep->gaddr4, 0);
	write_word((void *) &ep->iaddr1, 0);			/* individual address filter	*/
	write_word((void *) &ep->iaddr2, 0);
	write_word((void *) &ep->iaddr3, 0);
	write_word((void *) &ep->iaddr4, 0);

	write_word((void *) &ep->taddr_h, 0);
	write_word((void *) &ep->taddr_m, 0);
	write_word((void *) &ep->taddr_l, 0);

	/* Get Ethernet address */
	get_eaddr(deventry, eaddr);

	/* setup out physical address		 */
	write_word((void *) &ep->paddr_h, (eaddr[5] << 8) | eaddr[4]);
	write_word((void *) &ep->paddr_m, (eaddr[3] << 8) | eaddr[2]);
	write_word((void *) &ep->paddr_l, (eaddr[1] << 8) | eaddr[0]);

    /* now setup rx BD's before starting up ethernet */
	bd = (QUICC_BD *) ((u_int32) qp + BD_BASE + BD_SCC1_Eth + (channel * BD_Eth_Size));
						/* DB_OFFSET = BD_SCC1_Eth + (channel * BD_Eth_Size); */

    lustat->next_rx_bd = bd;
    lustat->base_rx_bd = bd;

	for(i=0 ; i < lustat->lu_nrdesc; i++) {
		bd->length = 0;
		/* If we do not know the ether_drvr (up_drvr), send in our own
		 * dev_list pointer to be suffixed to the mbuf header.
		 */

		if ((status = get_mbuf(((lustat->lu_ether_drvr == NULL) ? deventry :
			     lustat->lu_ether_drvr), ETHERMAX + sizeof(Dev_list), &mbptr))
																!= SUCCESS) {
			/* This should not happen */
			return(status);
		}
		/* Save space to stash away the dev_entry ptr */
		mbptr->m_offset += sizeof(Dev_list);
		write_long((void *) &bd->buf, mtod(mbptr, u_int32));

#ifdef _OS9000
		/* Invalidate Cache */
		if (drstat->cache_cctl) {
			drstat->cache_cctl(C_INVDATA | C_ADDR, mtod(mbptr, u_int32 *), ETHERMAX);
		}
#endif

		write_word((void *) &bd->length, 0);
		write_word((void *) &bd->status, R_E | R_I);	/* set status bits */
		bd++;					/* point to next bd */
	}
	bd--;						/* backup one */
	write_word((void *) &bd->status, read_word((void *) &bd->status) | R_W);	/* set wrap bit */

 	/* we are not xmitting yet so just set up */
	bd = (QUICC_BD *)((u_int32)qp + BD_BASE +
	                             (unsigned)read_word((void *) &ep->tbase));

    lustat->next_tx_bd = bd;
    lustat->base_tx_bd = bd;
	lustat->trans_bd   = lustat->base_tx_bd;

	for(i=0 ; i < lustat->lu_ntdesc; i++) {
		write_long((void *) &bd->buf, 0);
		write_word((void *) &bd->length, 0);
		write_word((void *) &bd->status, T_PAD | T_L | T_C | T_I);
		bd++;					/* point to next bd */
	}
	bd--;						/* backup one */

	/* set wrap bit */
	write_word((void *) &bd->status, read_word((void *) &bd->status) | T_W);

    write_long((void *) &qp->intr_cimr,
		read_long((void *) &qp->intr_cimr) | (INTR_SCC1>>channel));

#ifdef _OS9000
	/* enable irq */
	_pic_enable(lustat->lu_vector);
#endif

	write_long((void *) &qp->scc_regs[channel].scc_gsmrb, 0);

	/* clear ethernet event */
	write_word((void *) &qp->scc_regs[channel].scc_scce, 0xffff);

    /* Enable TXE, RXF, BSY, TXB and RXB interrupts (0x001f) */
    write_word((void *) &qp->scc_regs[channel].scc_sccm,
			ETHERNET_TXB|ETHERNET_RXB|ETHERNET_RXF|ETHERNET_BSY|ETHERNET_TXE);

	/* scc1 conf register is set to rx automatic control of cls & ren,
	 * more setup time for eest and ethernet mode ENR & ENT are not enabled.
	 */
	write_long((void *) &qp->scc_regs[channel].scc_gsmra, 0x1088000c);

	/* ethernet protocol specific mode register
	 * 32 bit CRC, normal operation and begin
	 * searching for the start frame delimiter 22 bits after RENA
	 */
	write_word((void *) &qp->scc_regs[channel].scc_psmr, 0x080a);

	write_word((void *) &qp->scc_regs[channel].scc_dsr, 0xd555);
#ifndef NO_CONFIG_TENA
	config_TENA( qp, channel);				/* set up RTS as TENA */
#endif
	write_long((void *) &qp->scc_regs[channel].scc_gsmra, 0x1088003c);	/* enable rx & tx */

	return SUCCESS;
}

u_int8 read_byte(void *addr) {
#ifndef	CACHE_INHIBITED_BUFS
	if (drstat->cache_cctl) {
		drstat->cache_cctl(C_INVDATA | C_ADDR, (u_int32 *) addr, sizeof(u_int8));
	}
#endif
	return (inc(addr));
}

u_int16 read_word(void *addr) {
#ifndef	CACHE_INHIBITED_BUFS
	if (drstat->cache_cctl) {
		drstat->cache_cctl(C_INVDATA | C_ADDR, (u_int32 *) addr, sizeof(u_int16));
	}
#endif
	return (inw(addr));
}

u_int32 read_long(void *addr) {
#ifndef	CACHE_INHIBITED_BUFS
	if (drstat->cache_cctl) {
		drstat->cache_cctl(C_INVDATA | C_ADDR, (u_int32 *) addr, sizeof(u_int32));
	}
#endif
	return (inl(addr));
}


void write_byte(void *addr, u_int8 data) {
	outc(addr, data);
#ifndef	CACHE_INHIBITED_BUFS
	if (drstat->cache_cctl) {
		drstat->cache_cctl(C_INVDATA | C_ADDR, (u_int32 *) addr, sizeof(u_int8));
	}
#endif
}

void write_word(void *addr, u_int16 data) {
	outw(addr, data);
#ifndef	CACHE_INHIBITED_BUFS
	if (drstat->cache_cctl) {
		drstat->cache_cctl(C_INVDATA | C_ADDR, (u_int32 *) addr, sizeof(u_int16));
	}
#endif
}

void write_long(void *addr, u_int32 data) {
	outl(addr, data);
#ifndef	CACHE_INHIBITED_BUFS
	if (drstat->cache_cctl) {
		drstat->cache_cctl(C_INVDATA | C_ADDR, (u_int32 *) addr, sizeof(u_int32));
	}
#endif
}

	/* Note:  For the 821/860ADS boards, the hardware address has to be
	 * picked from the descriptor.
	 */
void get_eaddr(Dev_list deventry, u_char *eaddr) {

	Spf_lustat 	lustat = (Spf_lustat)(deventry->v_lu_stat);
	int		i;
	u_char		flag = 0;

/*
Non-null address in descriptor overrides hardware address from
BBRAM or PROM.
*/
	for(i = 5; i >= 0; i--) {
		eaddr[i] = lustat->ether_addr[i];
		flag |= eaddr[i];
	}

#ifdef HW_GET_EADDR
    /* some hardware has the ethernet address in an eeprom, etc. and
    ** can be retrieved. */
    if (flag == 0)
      {
        /* descriptor has specified 00:00:00:00:00:00 as ether_addr */
        error_code err;

        err = hw_get_eaddr(eaddr);
        if (err == SUCCESS)
          memcpy(&lustat->ether_addr,eaddr,6);
      }
#endif

        /* set up ITEM address structure for ITE_GETADDR */
        memset(&lustat->lu_item_addr, 0, sizeof(lustat->lu_item_addr));
        lustat->lu_item_addr.addr_class = ITE_ADCL_ENET;
        lustat->lu_item_addr.addr_subclass = ITE_ADSUB_NONE;
        lustat->lu_item_addr.addr_size = 6;
        memcpy(&lustat->lu_item_addr.addr, lustat->ether_addr, 6);
}
