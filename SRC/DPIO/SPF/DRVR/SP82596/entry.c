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
 *   6 02/04/97 I82596 driver create                                   mil
 *   4 08/27/97 Bug fix                                                taka
 *   5 09/08/97 fixed os_irq()                                         taka
 *   7 12/03/97 Added code to support netstat:                         jk
 *               case ITE_GETADDR and SPF_GS_SYMBOLS in dr_getstat,
 *               if_data fields incremented in dr_downdata,
 *               function check_access added
 *   8 01/21/98  Removed all compiler and linker warnings              jk
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  10 06/11/99 Set stk_int_flags in SPF_GS_UPDATE (entry.c)           rds
 *  11 06/22/99 Integrated Kei Thomsen's changes, fixed mbuf leak.     jk
 *  12 08/29/99 Chip now pads short packets so don't do it here        rds
 *              Changed SPF_GS_ETHADDR to use ITEM ethernet address    rds
 *              Removed SPF_GS_DRVSTAT                                 rds
 *  15 07/17/01 Putted SPF_GS_DRVSTAT in again for iestat.c            KeiT
 ****************************************************************************/

/*
** Header Files
*/

#include <SP82596/defs.h>

static error_code check_access(process_id, u_int32, u_int32, void *);

/****************************************************************************
** Device Driver Initialization Entry point.
*****************************************************************************/
error_code dr_iniz(Dev_list deventry)
{
	#ifdef DEBUG
		Spf_lustat lustat = (Spf_lustat)(deventry->v_lu_stat);
	#endif
	error_code err;

	/* This piece of code creates a debug data module that you can link to
	 * and look at when in rombug to aid you in troubleshooting your driver.
	 */
	#ifdef DEBUG
		if (debug_init(DEBUG,(Dbg_stat*)&lustat->lu_dbg,lustat->lu_dbg_name)
																!= SUCCESS) {
			lustat->lu_dbg = NULL;
		} else {
			DEBUG_DATA(lustat->lu_dbg,"PRIniz     ", (u_int32)deventry);
			DEBUG_DATA(lustat->lu_dbg,"lustat     ", (u_int32)lustat);
			DEBUG_DATA(lustat->lu_dbg,"drstat     ", (u_int32)deventry->v_dr_stat);
		}
	#endif

	if ((err = init_drvr(deventry)) != SUCCESS) {
		return err;
	}


	return(SUCCESS);
}

/****************************************************************************
** Device Driver Termination Entry point.
****************************************************************************/
error_code dr_term(Dev_list deventry)
{
	Spf_lustat lustat = (Spf_lustat)(deventry->v_lu_stat);
#if defined (spdve7708)
	u_int32 *dga;
#endif

	DEBUG_DATA(lustat->lu_dbg,"PRTerm     ", (u_int32)deventry);


	/* PUT HARDWARE STOP COMMAND */

	/* later... */
	lustat->lu_ioenabled = DRVR_IODIS;

	I82596_RESET (lustat);

	/* walk through all the receive descriptors and return the mbufs */
	m_free_q (&lustat->lu_recvfree);

	/* PUT IRQ DISABLE */
	if (lustat->lu_shramsize)
		_os_srtmem(lustat->lu_shramsize, lustat->lu_shram);

	/* remove device from polling table */
	_OS_IRQ(lustat->lu_vector,			/* vector number */
			lustat->lu_priority,		/* polling priority */
			NULL,						/* irq handler routine (remove) */
			deventry,					/* static storage pointer */
			lustat->lu_port				/* port address */
	);

#if defined (spdve7708)
	dga = DGA_ADDR;
	dga[21] &= ~GP0IRQ_ENABLE;
#endif

	return(SUCCESS);
}


/***************************************************************************
** Device Driver Get-Stat Entry point.
****************************************************************************/
error_code dr_getstat(Dev_list deventry, Spf_ss_pb pb)
{
	Spf_lustat lustat = (Spf_lustat)(deventry->v_lu_stat);

	switch (pb->code) {
		case SPF_GS_UPDATE: {
			Spf_update_pb upb = (Spf_update_pb)pb;
			DEBUG_DATA(lustat->lu_dbg,"PRUpdate  ", (u_int32)lustat->lu_pathdesc);

			/*
			** We are the lowest driver on the stack so fill in the
			** parameter block with our stats
			*/
			upb->stk_txsize = lustat->lu_txsize;
			upb->stk_txoffset = lustat->lu_txoffset;
			upb->stk_ioenabled = lustat->lu_ioenabled;
			upb->stk_int_flags = IFF_BROADCAST;
			return(SUCCESS);

		} /* SPF_GS_UPDATE */

		case SPF_GS_ETHADDR: {
			/*
			** spenet uses this to find out what our hardware address is
			*/
			u_int8 *temp_ptr;

			temp_ptr = (u_int8 *)pb->param;
			memcpy(temp_ptr, lustat->lu_item_addr.addr, 6);
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

			Ite_conn_pb conn_ptr = (Ite_conn_pb) pb;

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

		case SPF_GS_HWSTAT: {
			memcpy ((void *)pb->param, lustat, sizeof(spf_lustat));
			return (SUCCESS);
		}

#ifndef SPF_GS_DRVSTAT
#define SPF_GS_DRVSTAT  (SPF_PR_RSVD+108)
#endif
		/* Get Driver Statistic (Added bei KeiT) */
		case SPF_GS_DRVSTAT: {
			memcpy ((void *)pb->param, deventry->v_dr_stat, sizeof(spf_drstat));
			return (SUCCESS);
		}

		case SPF_GS_SYMBOLS: {
			/*
			** netstat (among others) uses this to print our hardware
			** address and statistics.
			*/
			struct nlist *their_nl;
			struct nlist *our_nl;
			u_int32 real_size = 0;
			error_code err;
			Pd_com path_desc = &lustat->lu_pathdesc->pd_common;

			/* walk their request list, and count how many there are */
			for (their_nl = (struct nlist *)pb->param; their_nl->n_name; their_nl++)
				real_size++;

			real_size *= sizeof(*their_nl);

			if ( ((path_desc->pd_state & PD_SYSTATE) == 0) &&
						(err = check_access(lustat->LU_CURRPROC, real_size,
								S_IREAD|S_IWRITE, pb->param)) )
				return(err);

			/* walk through all of their symbols */
			for (their_nl = (struct nlist *)pb->param; their_nl->n_name; their_nl++) {
				/* walk through all of our symbols */
				for (our_nl = lustat->lance_symbols; our_nl->n_type; our_nl++) {
					if ((strcmp(our_nl->n_name, their_nl->n_name) == 0) &&
								(their_nl->n_type == 0)) {
						their_nl->n_value = our_nl->n_value;
						their_nl->n_type = N_ABS;
					}
				}
			}
			return(SUCCESS);
		}

		default: {
			DEBUG_DATA(lustat->lu_dbg,"PRGsUnknown", pb->code);

			/* If we get this far, return unknown service */
			return(EOS_UNKSVC);
		}

	} /* End switch */

}

/****************************************************************************
** Device Driver Set-Stat Entry point.
****************************************************************************/
error_code dr_setstat(Dev_list deventry, Spf_ss_pb pb)
{
	Spf_drstat drstat = (Spf_drstat)(deventry->v_dr_stat);
	Spf_lustat lustat = (Spf_lustat)(deventry->v_lu_stat);

	switch(pb->code) {
		case SPF_SS_OPEN: {
			DEBUG_DATA(lustat->lu_dbg,"PROpen     ",(u_int32)lustat->lu_pathdesc);
			return(SUCCESS);
		}

		case SPF_SS_HWEN: {
			DEBUG_DATA(lustat->lu_dbg, "HWenabled  ", pb->code);

			/* Enable the driver for I/O */
			lustat->lu_ioenabled = DRVR_IOEN;

			return(SUCCESS);
		}

		case SPF_SS_CLOSE: {
			DEBUG_DATA(lustat->lu_dbg,"PRClose    ", (u_int32)lustat->lu_pathdesc);

			/*
			** We should have received an SPF_SS_HWDIS to disable I/O,
			** but if we are closing the last path disable it just to
			** be sure.
			*/
			if(drstat->dr_use_cnt == 0)
				lustat->lu_ioenabled = DRVR_IODIS;

			return(SUCCESS);
		}

		case SPF_SS_HWDIS: {
			DEBUG_DATA(lustat->lu_dbg, "HWDisabled  ", pb->code);

			/* Disenable the driver for I/O */
			lustat->lu_ioenabled = DRVR_IODIS;

			return(SUCCESS);
		}

		case SPF_SS_PUSH: {
			DEBUG_DATA(lustat->lu_dbg,"PRPush     ", (u_int32)lustat->lu_pathdesc);

			if (lustat->lu_ether_drvr != NULL) {
				/*
				** We only allow one driver to be pushed on us.
				*/
				return (EOS_BUSY);
			}
			lustat->lu_ether_drvr = lustat->lu_updrvr;
			return(SUCCESS);
		}

		case SPF_SS_POP: {
			DEBUG_DATA(lustat->lu_dbg,"PRPop      ",(u_int32)lustat->lu_pathdesc);
			lustat->lu_ether_drvr = NULL;
			return(SUCCESS);
		}

	} /* end of switch */

	DEBUG_DATA(lustat->lu_dbg,"PRSsUnknown", pb->code);
	return(EOS_UNKSVC);
}

/*****************************************************************************
** Device Driver Downgoing-Data Entry point.
******************************************************************************/
error_code dr_downdata(Dev_list deventry, Mbuf mb)
{
	Spf_lustat lustat = (Spf_lustat)(deventry->v_lu_stat);
	status_reg old_irq = 0;
	Mbuf temp;
	int32 plen, status;

#ifdef DEBUG
	if (mb) {
		DEBUG_DATA(lustat->lu_dbg, "LE  DwnDat", (u_int32)mb);
	} else {
		DEBUG_DATA(lustat->lu_dbg, "ERLEDnMbEm", 0);
	}
#endif

	/* sanity check .. is pointer null?*/
	if (mb == NULL) return 0;

	/* forget it if the chip isn't ready yet .. pitch mbuf/packet */
	if (lustat->lu_ioenabled == DRVR_IODIS) {
		m_free_p(mb);				/* not running, drop the packet */
		return EOS_NOTRDY;
	}

	/* message too long for ethernet? */
	if ((plen = m_len_p(mb)) > ETHER_MAX) {
		m_free_p(mb);				/* sorry, drop the packet */
		lustat->if_oerrors++;
		return (EMSGSIZE);
	}

	/*
	** Version 11 and earlier of the driver checked here to see if the
	** packet was less than the minimum Ethernet length. I have changed
	** the chip initialization so it now will automatically pad short
	** packets. One potential problem is that it pads with 0x7e and not
	** 0x00. This shouldn't cause any problems with anything we ship,
	** but there is a slight possibility this will cause problems with
	** some other implementation (possibly ISP).
	**
	** If the zero padding is put back in it needs to be rewritten
	** from how it was done before. The subsequent pullup caused
	** an extra 4 system calls to be used. If the current mbuf does
	** not have room for the zero padding a new mbuf should be allocated
	** and filled in such a way that a pullup does not need to be done.
	*/

	/*
	** Someone put some code in send_packet that appears to try and
	** handle packet chains but it doesn't work. As a result we
	** need to scoop the mbuf chain.
	*/
	if (mb->m_pnext) {
		mb = m_copy(temp = mb, 0, plen, &status);
		m_free_p(temp);
		if (mb == NULL) {
			lustat->if_oerrors++;
			return(ENOBUFS);
		}
	}

	/*
	** If broadcast or multicast, increment counter. Probably shouldn't
	** do it here because the packet may get dropped or we might get
	** a transmit error.
	*/
	if ((mb->m_flags & M_BCAST) || (mb->m_flags & M_MCAST))
		lustat->if_omcasts++;

	/* now give mbuf data to hardware */
	if (lustat->lu_irqmask != 0) {
		old_irq = irq_change((status_reg)lustat->lu_irqmask);
	}

	DEBUG_DATA(lustat->lu_dbg, "HW  Send", (u_int32)mb);

	/* now give mbuf data to i82596 */
	if (i82596Output(deventry, mb))
		lustat->if_oerrors++;
	else
		lustat->if_opackets++;

	if (lustat->lu_irqmask != 0) {
		irq_restore(old_irq);
	}
	return SUCCESS;
}


/****************************************************************************
** Device Driver Upcoming-Data Entry point.
** This is the lowest device driver...this entry point should not be hit.
*****************************************************************************/
error_code dr_updata(Dev_list deventry, Mbuf mb)
{
#ifdef DEBUG
	Spf_lustat lustat = (Spf_lustat)(deventry->v_lu_stat);
#endif

#ifdef DEBUG
	if (mb) {
		DEBUG_DATA(lustat->lu_dbg, "LE  UpDate", (u_int32)mb);
	} else {
		DEBUG_DATA(lustat->lu_dbg, "ERLEUpEmpt", (u_int32)mb);
	}
#endif

	m_free_p(mb);
	return EOS_UNKSVC;
}

/* routine to see if user can read/write memory location */

static error_code
check_access(process_id proc_id, u_int32 size, u_int32 mode, void *where)
{
	pr_desc *pdesc;
	error_code err;

	if (err = _os_findpd(proc_id, &pdesc))
		return(err);

	if (err = _os_chkmem(size, mode, where, pdesc)) {
		return(err);
	}

	return(0);
}
