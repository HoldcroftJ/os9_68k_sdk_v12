/*****************************************************************************
 *  QUICC Driver Entry points.                                               *
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
 *  --- ------- ------------------------------------------------------ ----- *
 *  01 10/07/96 Creation                                                     *
 *  02 02/02/97 In dr_getstat, call get_eaddr to get hardware address. sr    *
 *  03 02/05/97 In dr_term, disable pic.                               sr    *
 *  04 02/06/97 In dr_downdata, if qe_xmit does not return SUCCESS     sr    *
 *              enqueue mbuf for retry.                                      *
 *     02/19/97 <***** SPF LAN Comm Pack v3.0 Release*********>              *
 *     05/05/97 <***** SPF LAN Comm Pack v3.1 Release*********>              *
 *  09 08/11/97 Added SCC channel dependent code                       taka  *
 *     10/22/97 <***** OS-9000 MBX BSP Release v1.0 **********>              *
 *  12 11/25/97 Added code to support netstat:                               *
 *               case ITE_GETADDR and SPF_GS_SYMBOLS in dr_getstat,          *
 *               function check_access                                 jk    *
 *  14 01/21/98 Removed compiler and linker warnings                   jk    *
 *  15 03/05/98 In dr_downdata, in case of "scoop", get a new mbuf,    jk/sr *
 *              set the offset, then call m_move. Other clean up.            *
 *     04/02/98 <***** SPF LAN Comm Pack v3.2 Release*********>              *
 *  16 02/12/99 Set stk_int_flags in dr_getstat (SPF_SS_UPDATE)        rds   *
 *  17 11/15/99 CF6721: fix transmit mbuf leak, improve performance          *
 *              in xmit_irq().                                         gjb   *
 *  18 12/28/99 Added multicast support                                rds   *
 *  19 02/10/00 Added support for limited transmit queue               rds   *
 *              Cache flush mbufs here instead of in isr               rds   *
 *  20 02/11/00 Removed MC68360 conditional/get_eaddr() args           kimk  *
 *  21 08/02/00 Fix the problem for CF8554                             mdu   *
 *  22 08/17/00 CF2733: Add the check user access rights before doing        *
 *              the memory copy. Also, remove the DEV_PB structure     mdu   *
 *  23 12/07/00 CF9967: Added KeiT's changes:                          mdu   *
 *              - Use lu_txoffset instead of offset 4                        *
 *              - Allocate new mbuf if not enough space is left in           *
 *                front of the packet                                        *
 *  26 02/19/01 Added C_ADDR to cache_cctl()			       sbh   *
 *  27 08/15/01 CF7420: multicast support								taka *
 ****************************************************************************/

/*
 *	Header Files
 */
#include <SPQUICC/defs.h>
#include <SPF/spinet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <memory.h>

/*
** Multicast defines
*/
error_code ChangeMulticast(int, Spf_lustat, struct sockaddr *);
void set_group_bitmap(Spf_lustat, u_int32);

typedef struct mc_list {
	struct mc_list *next;
	u_int16 addr[3];
} mc_list, *Mc_list;

#ifdef _OSK
	#define _OS_IRQ(a,b,c,d,e) _os9_irq(a,b,c,d,e)
#else
	#define _OS_IRQ(a,b,c,d,e) _os_irq(a,b,c,d)
#endif

/*----------------------------------------*/
/********************/
/* Type definitions */
/********************/

static error_code check_access(process_id, u_int32, u_int32, void *);

/*-----------------------------------------*/
/***************************************************************************
 *	Device Driver Initialization entry point
 ***************************************************************************/
error_code dr_iniz(Dev_list dev_entry)
{
	Spf_lustat 	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
#ifdef _OS9000
	Spf_drstat  drstat = (Spf_drstat)(dev_entry->v_dr_stat);
#endif
	error_code 	err;

#if defined(DEBUG)
	if (debug_init(DEBUG,(Dbg_stat *)&lustat->lu_dbg,lustat->lu_dbg_name) != SUCCESS) {
		lustat->lu_dbg = NULL;
	}
	algn_dbg_data(lustat->lu_dbg,(u_char *) "MACdr_iniz ",10, 0);
#endif

	/*
	** If we have a descriptor edition before 14 lu_qsize won't be 0, so
	** just set it here to make sure. Also, if lu_qlimit is 0 set it to
	** the default.
	*/
	lustat->lu_qsize = 0;
	if (lustat->lu_qlimit == 0) {
		lustat->lu_qlimit = MAX_QUEUE;
	}

#ifdef _OS9000
	/****************************
		Get Cache routine
	*****************************/

	_os_scache(C_GETCCTL, &drstat->cache_cctl, &lustat->lu_cache_static, 0, 0);

#endif

	/****************************
		Install the ISR
	 ****************************/
	if ((err = _OS_IRQ(lustat->lu_vector, lustat->lu_priority,
							qe_isr, dev_entry, lustat->lu_port)) != SUCCESS) {
		return(err);
	}

	/***************************************************
       	Ensure that no interrupts are received until
       	a path is opened to the device. (or else a
       	system state exception error will occur. The
       	lu_updrvr parameter in lustat is initialized by
       	SPF, but not until a path is opened. The value
       	stored in lu_updrvr is used to process the
       	received data. If you try to send data up before
       	this value is init'd, you will be writing to
       	someone elses memory (that's a BAD thing...right?)
       	The QUICC's interrupts will be turned on in the
       	SS_OPEN setstat (called every time a path open is
       	performed).
   	***************************************************/

	err = init_hdw(dev_entry);

	DEBUG_DATA(lustat->lu_dbg, "MAC:STAT  ", 0);
	DEBUG_DATA(lustat->lu_dbg, "lu_att_cnt ", lustat->lu_att_cnt);
	DEBUG_DATA(lustat->lu_dbg, "lu_use_cnt ", lustat->lu_use_cnt);
	DEBUG_DATA(lustat->lu_dbg, "lu_port ", lustat->lu_port);
	DEBUG_DATA(lustat->lu_dbg, "lu_num ", lustat->lu_num);
	DEBUG_DATA(lustat->lu_dbg, "lu_ioenabl ", lustat->lu_ioenabled);
	DEBUG_DATA(lustat->lu_dbg, "lu_txsize ", lustat->lu_txsize);
	DEBUG_DATA(lustat->lu_dbg, "lu_txoffse ", lustat->lu_txoffset);
	DEBUG_DATA(lustat->lu_dbg, "lu_updrvr ", lustat->lu_updrvr);
	DEBUG_DATA(lustat->lu_dbg, "lu_dndrvr ", lustat->lu_dndrvr);

	/* signal cpu is initialized and is up and running */

	lustat->lu_running = 1;

	return(err);
}

/**************************************************************************
 *	Device Driver Termination entry point
 **************************************************************************/
error_code dr_term(Dev_list dev_entry)
{
	Spf_lustat	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	status_code mask;

	/* turn off all interrupts from the hardware */
	term_hdw( dev_entry);

	/* mask the MSR external interrupt */
	mask = irq_maskget();

	/* Chip is no longer available */
	lustat->lu_running = 0;

#ifdef _OS9000
	_pic_disable(lustat->lu_vector);
#endif

	/* remove interrupt service routine */
	_OS_IRQ(lustat->lu_vector, lustat->lu_priority, NULL,
										dev_entry, lustat->lu_port);

	irq_restore(mask);

	/* ensure that i/o queues are free of mbufs */
	m_free_q(&lustat->lu_outq);

	DEBUG_DATA(lustat->lu_dbg, "HDW:dr_term  ", 0);
	DEBUG_DATA(lustat->lu_dbg, "HDW:attcnt", lustat->lu_att_cnt);
	return(SUCCESS);
}

/************************************************************************
 *  Device Driver GetStat entry point
 ************************************************************************/
error_code dr_getstat(Dev_list dev_entry, Spf_ss_pb spb)
{
	Spf_lustat lustat = (Spf_lustat)(dev_entry->v_lu_stat);

	DEBUG_DATA(lustat->lu_dbg, "GetStat", spb->code);

	switch (spb->code) {

		case SPF_SS_UPDATE: {
			Spf_update_pb upb = (Spf_update_pb)spb;

			/* Since I'm always lowest on the stack, I just fill my
			 * variables and return.
			 */
			upb->stk_txsize = lustat->lu_txsize;
			upb->stk_txoffset = lustat->lu_txoffset;
			upb->stk_ioenabled = lustat->lu_ioenabled;
			upb->stk_int_flags = IFF_BROADCAST | IFF_MULTICAST;

			return(SUCCESS);
		}

		case SPF_GS_ETHADDR: {
				u_int8	*temp_ptr;

			temp_ptr = (u_int8 *)spb->param;
			get_eaddr(dev_entry, temp_ptr);

			return(SUCCESS);
		}

		case ITE_GETADDR: {
			/* The format of the ite_conn_pb passed in is as follows:
			 *      code = ITE_GETADDR
			 *      size = sizeof ite_conn_pb
			 *      cctl_pb = 0
			 *      notify_pb = 0
			 *      our_num = address of user supplied Addr_type struct
			 *      their_num = address of user supplied Addr_type struct
			 */

			error_code err;
			Pd_com path_desc = &lustat->lu_pathdesc->pd_common;

			Ite_conn_pb conn_ptr = (Ite_conn_pb) spb;

			if (conn_ptr->our_num) {
				if ( ((path_desc->pd_state & PD_SYSTATE) == 0) &&
					 (err = check_access(lustat->LU_CURRPROC,
										 sizeof(addr_type),
										 S_IWRITE, conn_ptr->our_num))  )
					return(err);
				*conn_ptr->our_num = lustat->lu_item_addr;
			}

			if (conn_ptr->their_num) {
				if ( ((path_desc->pd_state & PD_SYSTATE) == 0) &&
					 (err = check_access(lustat->LU_CURRPROC,
										 sizeof(addr_type),
										 S_IWRITE, conn_ptr->their_num)) )
					return(err);
				conn_ptr->their_num->addr_class = ITE_ADCL_NONE;
				conn_ptr->their_num->addr_subclass = ITE_ADSUB_NONE;
			}
			return(SUCCESS);
		}

		case SPF_GS_HWSTAT:  {

			error_code err;
			Pd_com path_desc = &lustat->lu_pathdesc->pd_common;
			
			if ( ((path_desc->pd_state & PD_SYSTATE) == 0) &&
					 (err = check_access(lustat->LU_CURRPROC,
										 sizeof(spf_lustat),
										 S_IWRITE, spb->param)) )
				return(err);
			memcpy ((void *)spb->param, lustat, sizeof(spf_lustat));
			return (SUCCESS);
		}

		case SPF_GS_SYMBOLS: {
			struct nlist *their_nl;
			struct nlist *our_nl;
			/*
			Pd_com path_desc = &lustat->lu_pathdesc->pd_common;
			*/
#if !defined(HAS_NO_CHECKS)
			u_int32 real_size = 0;

			/* traverse array to count the elements */
			for (their_nl = (struct nlist *)spb->param;
				 their_nl->n_name;
				 their_nl++)
				real_size++;

			real_size *= sizeof(*their_nl);

#endif /* !defined(HAS_NO_CHECKS) */

			/* traverse their array */
			for (their_nl = (struct nlist *)spb->param;
				 their_nl->n_name;
				 their_nl++) {

				/* traverse our array and compare symbols */
				for (our_nl = lustat->my_symbols;
					 our_nl->n_type;
					 our_nl++) {

					if ( (strcmp(our_nl->n_name, their_nl->n_name) == 0) &&
											   (their_nl->n_value == 0 )    ) {
						their_nl->n_value = our_nl->n_value;
						their_nl->n_type = N_ABS;
					}
				}
			}

			return(SUCCESS);
		}

		default: {
			return(EOS_UNKSVC);
		}

	}
} /* dr_getstat */


/***********************************************************************
 *  Device Driver SetStat entry point
 ***********************************************************************/
error_code dr_setstat(Dev_list dev_entry, Spf_ss_pb ss_pb)
{
	Spf_drstat     drstat = (Spf_drstat)(dev_entry->v_dr_stat);
	Spf_lustat     lustat = (Spf_lustat)(dev_entry->v_lu_stat);

	DEBUG_DATA(lustat->lu_dbg, "MAC:SetStat", ss_pb->code);

	switch (ss_pb->code) {

		case SPF_SS_OPEN: {       /* hook to new upper/lower protocols    */

			DEBUG_DATA(lustat->lu_dbg, "MAC:SsOpen", ss_pb->code);
			DEBUG_DATA(lustat->lu_dbg, "MAC:attcnt", lustat->lu_att_cnt);

			return(SUCCESS);
		}

		case SPF_SS_HWEN: {

			DEBUG_DATA(lustat->lu_dbg, "DrEnabled", ss_pb->code);
			DEBUG_DATA(lustat->lu_dbg, "MAC:attcnt", lustat->lu_att_cnt);

			/* Enable the driver for I/O    */
			lustat->lu_ioenabled = DRVR_IOEN;

			DEBUG_DATA(lustat->lu_dbg, "DrEnabldOK", ss_pb->code);

			return(SUCCESS);
		}

		case SPF_SS_CLOSE: {      /* unhook from upper/lower protocols    */

			DEBUG_DATA(lustat->lu_dbg, "MACSsClose", ss_pb->code);

			if(drstat->dr_use_cnt == 0)
				lustat->lu_ioenabled = DRVR_IODIS;

			return(SUCCESS);
		}

		case SPF_SS_HWDIS: {

			DEBUG_DATA(lustat->lu_dbg, "DrDisable", ss_pb->code);
			DEBUG_DATA(lustat->lu_dbg, "MAC:attcnt", lustat->lu_att_cnt);

			/* Disable the driver for I/O    */
			lustat->lu_ioenabled = DRVR_IODIS;

			DEBUG_DATA(lustat->lu_dbg, "DrDisabOK", ss_pb->code);

			return(SUCCESS);
		}

		case SPF_SS_PUSH:	{

			DEBUG_DATA(lustat->lu_dbg, "MAC:SsPush", ss_pb->code);

			if (lustat->lu_ether_drvr == NULL) {
				lustat->lu_ether_drvr = lustat->lu_updrvr;
			}

			return(SUCCESS);
		}

		case SPF_SS_POP: {  /* push/pop the protocol above this drive   */

			DEBUG_DATA(lustat->lu_dbg, "MAC:SsPop", ss_pb->code);

			return(SUCCESS);
		}

		case SS_LUOPT: {		/* set logical unit options         */
			/* update hardware here */
			return(SUCCESS);
		}

		case IP_SS_IOCTL: {
			struct bsd_ioctl *arg_ptr;
			struct ifreq *ifr;

			arg_ptr = ss_pb->param;
			ifr = arg_ptr->arg;
			switch(arg_ptr->cmd) {
				case SIOCADDMULTI:
				case SIOCDELMULTI:
					return (ChangeMulticast(arg_ptr->cmd, lustat, &ifr->ifr_addr));
			}
			return(EOPNOTSUPP);
		}

		default: {
			return(EOS_UNKSVC);
		}

	} /* switch */
}/* dr_setstat */

/*************************************************************************
 *  Device Driver Updata (Read) entry point !!! NOT USED !!!
 *************************************************************************/
error_code dr_updata(Dev_list dev_entry, Mbuf mb)
{
	m_free_p(mb);
	return(SUCCESS);
}

/*************************************************************************
 *	Device Driver Downdata (Write) entry point
 *************************************************************************/
error_code dr_downdata(Dev_list dev_entry, Mbuf mb)
{
	Spf_lustat	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
#ifdef _OS9000
	Spf_drstat	drstat = (Spf_drstat)(dev_entry->v_dr_stat);
#endif
	int32       plen;
	Mbuf        temp, newmb;
	int32       status = SUCCESS;

#if !defined _MPFPOWERPC
	status_reg	old_irq = 0;
#else
	status_code stat_code;
#endif

	/* sanity check */
	if (mb == NULL) {
		return 0;		/* Get a proper error code placed in here.	*/
	}

	/* forget it if the chip isn't ready yet */
	if (!lustat->lu_running) {
		m_free_p(mb);
		return(E_NOTRDY);
	}

	/* message too long for ethernet? */
	if ((plen = m_len_p(mb)) > ETHERMAX) {
		m_free_p(mb);                 /* sorry, drop the packet */
		lustat->if_oerrors++;
		return(EMSGSIZE);
	}

	/*
	now scoop the mbuf chain into one contigous mbuf for xmit.
	This is only a temporary measure until the driver is changed to
	use the packet-chaining feature of the QUICC.  Scoop only if necessary,
	however.
	*/
	if ((plen > mb->m_size) || (mb->m_offset < (sizeof(mbuf) + lustat->lu_txoffset))) {
		/* We do this, since by doing a copy we will get an mbuf with offset
		 * just after start of the mbuf header, and since we need 4 bytes
		 * to stash the mbuf pointer (for later use), we would write on top
		 * of the mbuf header if we use m_copy (or m_pullup).
		 */

		if ((newmb = m_getn(plen + lustat->lu_txoffset, &status)) == 0) {
			m_free_p(mb);             /* Free the mbuf */
			lustat->if_oerrors++;
			return (status);            /* m_getn failed  */
		}

		/* We now know we have a good mbuf to move data into */

		/* save 4 bytes for stashing ptr to start of mbuf used in qe_xmit
	 	 * and xmit_irq.
	 	 */

		newmb->m_offset += lustat->lu_txoffset;

		/* Use m_move to copy the data into the new mbuf */
		if ((newmb->m_size = m_move(mb, 0, plen, mtod(newmb,char *))) == 0) {
			/* No data moved! */
			m_free_p(newmb);          /* Free the new mbuf */
			m_free_p(mb);             /* Free the old mbuf */
            lustat->if_oerrors++;
			return (EBUFTOOSMALL);
		}

        /* save old MBuf flags */
		newmb->m_flags = mb->m_flags & (~(SPF_NOFREE || SPF_DONE));
		
		/* Now free the old mbuf and assign newmb to mb */
		temp = mb;
		m_free_p(temp);
		mb = newmb;

        DEBUG_DATA(lustat->lu_dbg, "Mv Mb Off", mb);
		DEBUG_DATA(lustat->lu_dbg, "m_offset", mb->m_offset);
		DEBUG_DATA(lustat->lu_dbg, "m_size", mb->m_size);
	} /* end scoop */

	/* If broadcast or multicast, inc counter.  Not a great place to do
	   this since may get xmit error, but the mbuf is freed in load_xmit. */
	if (mb->m_flags & (M_BCAST | M_MCAST)) {
		lustat->if_omcasts++;
	}

#ifdef _OS9000
	drstat->cache_cctl(C_FLDATA | C_ADDR, mtod(mb, void *), mb->m_size);
#endif

	/* now give mbuf data to QUICC */
#ifdef _MPFPOWERPC
	/* For PowerPC, irqmask does not mean anything? */
	stat_code = irq_maskget();
	/* irq_disable(); */
#else
	if (lustat->lu_irqmask != 0) {
		old_irq = irq_change((status_reg)lustat->lu_irqmask);
	}
#endif

	DEBUG_DATA(lustat->lu_dbg, "QE  LoadTm", mb);
	DEBUG_MBUF_DOWN(lustat->lu_dbg, mb);

	{
		QUICC_BD *nextxbd = lustat->next_tx_bd;

		/* First, check to see if there is an open buffer descriptor for this
		** mbuf.  If so, transmit it.  */
		if ((nextxbd->buf != 0) || (qe_xmit(dev_entry, mb) != SUCCESS)) {
			if (lustat->lu_qsize < lustat->lu_qlimit) {
				DEBUG_DATA(lustat->lu_dbg, "dd m_enq",mb);
				m_enq(&lustat->lu_outq, mb);
				lustat->lu_qsize++;
			} else {
				DEBUG_DATA(lustat->lu_dbg, "dd drop", mb);
				m_free_p(mb);
			}
		}
	}

#ifdef _MPFPOWERPC
	/* For PowerPC, irqmask does not mean anything? */
	irq_restore(stat_code);
	/* irq_enable(); */
#else
	if (lustat->lu_irqmask != 0) {
		irq_restore(old_irq);
	}
#endif

	return(SUCCESS);
}


/* routine to see if user can read/write memory location */

static error_code
check_access(process_id proc_id, u_int32 size, u_int32 mode, void *where)
{
#if !defined(HAS_NO_CHECKS)
	pr_desc *pdesc;
	error_code err;

	if (err = _os_findpd(proc_id, &pdesc))
		return(err);

	if (err = _os_chkmem(size, mode, where, pdesc)) {
		return(err);
	}
#endif /* !defined(HAS_NO_CHECKS) */

	return(0);
}


error_code ChangeMulticast(int op, Spf_lustat lustat, struct sockaddr *sa)
{
	error_code err;
	u_int32 size;
	struct mc_list *p, *q;
	u_int16 tl, tm, th;
	u_int16 channel;
	QUICC *qp;
	struct ethernet_pram *Ethernet;

	/*
	** We currently only support multicasts with IP so return an error
	** if it is anything else.
	*/
	if (sa->sa_family != AF_INET)
		return (EAFNOSUPPORT);

	/*
	** Map the request multicast address into the appropriate hardware
	** address. Remember that taddr_X is little endian so we setup
	** these temporary variables that way.
	*/
	tl = 0x0001;
	tm = 0x005e | ((sa->sa_data[3] & 0x7f) << 8);
	th = ((u_int8) sa->sa_data[4]) | (sa->sa_data[5] << 8);

	qp = lustat->qp;
	channel = lustat->lu_channel;
	Ethernet = (struct ethernet_pram * )lustat->lu_port;

	if (op == SIOCADDMULTI) {
		/*
		** If/when we have the ability to be a multicast router, mrouted
		** needs to see all multicast packets. The way it does that is to
		** use INADDR_ANY. In this case we will manipulate the group
		** hash table directly, but we need to stop the ethernet channel
		** first.
		*/
		if (((struct sockaddr_in *)sa)->sin_addr.s_addr == INADDR_ANY) {
			set_group_bitmap(lustat, 0xffffffff);
			return(SUCCESS);
		}
		size = sizeof(struct mc_list);
		if ((err = _os_srqmem(&size, (void **)&p, MEM_ANY)) != SUCCESS) {
			return err;
		}
		Ethernet->taddr_l = p->addr[0] = tl;
		Ethernet->taddr_m = p->addr[1] = tm;
		Ethernet->taddr_h = p->addr[2] = th;
		p->next = lustat->mc_addrs;
		lustat->mc_addrs = p;
		while (qp->cp_cr & CMD_FLAG)
			;
		qp->cp_cr = SET_ENET_GROUP + (channel << 6) + CMD_FLAG;
		while (qp->cp_cr & CMD_FLAG)
			;
		return(SUCCESS);
	}

	if (op == SIOCDELMULTI) {
		if (((struct sockaddr_in *)sa)->sin_addr.s_addr != INADDR_ANY) {
			/*
			** If we are removing a specific multicast address we need
			** to pull that entry out of our mc_addrs list.
			*/
			p = lustat->mc_addrs;
			q = NULL;
			while (p) {
				/*
				** No need to compare p->addr[0] because it will always
				** be the same for IP multicast addresses.
				*/
				if ((p->addr[2] == th) && (p->addr[1] == tm)) {
					if (q)
						q->next = p->next;
					else
						lustat->mc_addrs = p->next;
					_os_srtmem(sizeof(struct mc_list), (void *)p);
					break;
				}
				q = p;
				p = p->next;
			}
		}

		/*
		** Reset the group bitmap to all zeros and then re-add all
		** the remaining multicast addresses.
		*/
		set_group_bitmap(lustat, 0x00000000);
		p = lustat->mc_addrs;
		while (p) {
			Ethernet->taddr_l = p->addr[0];
			Ethernet->taddr_m = p->addr[1];
			Ethernet->taddr_h = p->addr[2];
			while (qp->cp_cr & CMD_FLAG)
				;
			qp->cp_cr = SET_ENET_GROUP + (channel << 6) + CMD_FLAG;
			while (qp->cp_cr & CMD_FLAG)
				;
			p = p->next;
		}
		return(SUCCESS);
	}

	return (EOPNOTSUPP);
}

void set_group_bitmap(Spf_lustat lustat, u_int32 new_val)
{
	QUICC *qp;
	u_int16 channel;
	struct ethernet_pram *Ethernet;
	status_code mask;

	qp = lustat->qp;
	channel = lustat->lu_channel;
	Ethernet = (struct ethernet_pram * )lustat->lu_port;

	/* mask the MSR external interrupt */
	mask = irq_maskget();

	/*
	** Stop the transmitter and clear the ENT and ENR bits in gsmr
	*/
	while (qp->cp_cr & CMD_FLAG)
		;
	qp->cp_cr = GR_STOP_TX + (channel << 6) + CMD_FLAG;
	while (qp->cp_cr & CMD_FLAG)
		;
	while (!(qp->scc_regs[channel].scc_scce & ETHERNET_GRA))
		;
	qp->scc_regs[channel].scc_scce = ETHERNET_GRA;
	qp->scc_regs[channel].scc_gsmra &= ~(GSMR_ENT | GSMR_ENR);

	/*
	** Clear or set the group address bitmap depending on the parameter
	** we received.
	*/
	Ethernet->gaddr1 = new_val;
	Ethernet->gaddr2 = new_val;
	Ethernet->gaddr3 = new_val;
	Ethernet->gaddr4 = new_val;

	/*
	** Restart the receiver/transmitter and set ENT and ENR in gsmr
	*/
	while (qp->cp_cr & CMD_FLAG)
		;
	qp->cp_cr = ENTER_HUNT_MODE + (channel << 6) + CMD_FLAG;
	while (qp->cp_cr & CMD_FLAG)
		;
	qp->cp_cr = RESTART_TX + (channel << 6) + CMD_FLAG;
	while (qp->cp_cr & CMD_FLAG)
		;
	qp->scc_regs[channel].scc_gsmra |= (GSMR_ENT | GSMR_ENR);

	irq_restore(mask);
}
