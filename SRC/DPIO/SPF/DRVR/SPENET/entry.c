/****************************************************************************
 *           Copyright 1989-1997 by Microware Systems Corporation           *
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
 *   1 01/12/96 LANPACK                                                bnr
 *   2 02/05/96 updated according the code review                      tm
 *   7 09/03/96 bug fix for pop and push
 *  10 10/22/96 store hardware txoffset in per path storage
 *  11 10/29/96 Bug fix for arp request timer
 *  12 11/27/96 In del_enet_entry, check to make sure that arp_req_list sr
 *              is not NULL before accessing it.
 *  15 ??/??/?? New style........
 *  17 01/17/97 Zero out memory allocated by srqmem.                    sr
 *  18 01/23/97 Changes to ppstg to make new style spenet work.         sr
 *  19 01/28/97 Added setstats for ARP ioctl.                           sr
 *  20 01/30/97 In dr_setstat, enable/disable HW only if hardware       sr
 *              exists.
 *  21 02/03/97 Return sensible values when errors occur.               sr
 *  22 02/17/97 Verify enough space in mbuf to prepend ethernet header rds
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *  23 04/27/97 Get arp source address from per path storage           rds
 *              Convert from ISP to BSD header files                   rds
 *  24 04/28/97 Split part of defs.h into bsddefs.h                    rds
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *     05/20/97 <--------- SPF LAN Comm Pack v3.1.1 Release -------->
 *  29 12/10/97 Set pathdesc on SMCALL_UPDATA so IP knows which intfc  tmd
 *  30 12/10/97 Changed is_super to use _os_getsys instead of _os_id   tmd
 *              Added code to retrieve a single arp entry              tmd
 *              Added check_access function to verify memory access    tmd
 *  31 02/03/98 Removed spsin.h, removed compiler warnings             rds
 *     05/03/98 Commented debug stmt, in dr_downdata. Added others.     sr
 *  32 04/01/98 Cleaned up code.                                        sr
 *     04/01/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  33 09/25/98 Check for multicast packets in dr_downdata             rds
 *  34 12/14/98 Fixed potential mbuf leak in dr_updata                 rds
 *  36 02/02/99 dr_setstat/dr_getstat defaults not using pp storage    rds
 *  37 11/22/99 Many speed improvements and a couple mbuf leak fixes   rds
 *  38 01/19/00 Leave extra space in mbuf if we do a pullup            rds
 *  39 10/19/00 Fix-up word/longword alignment                         taka
 *  40 10/31/00 The pullup code added in edition 38 didn't set m_size  rds
 *  41 02/04/01 Set spip_deventry durring first SPF_SS_PUSH            rds
 *  42 02/26/01 CF10443: added SPF_GS_PROTID getstat		       sbh
 *  43 07/10/01 Remove get_sysglobs() prototype to rely on regs.h      and
 ***************************************************************************/

/*
**	Header Files
*/
#include "bsddefs.h"

int is_super(Spf_drstat);

/*
** Add a ENET per-path storage entry to the ENET path list on open.
*/
error_code add_enet_entry(Dev_list deventry, Spf_drstat drstat, Spf_lustat lustat) {
	Pp_enetlist enet_entry;
	spf_ss_pb new_pb;
	error_code err;
	u_int32 size;

	/*
	** get the ENET Per path storage + the "array" for the protocol
	** addresses for the interface
	*/
	size = sizeof(struct pp_enetlist) +
						lustat->lu_numprotoaddr * sizeof(struct addr_list);

	if ((err = _os_srqmem(&size, (void**)&enet_entry, MEM_ANY)) != SUCCESS) {
		DEBUG_DATA(lustat->lu_dbg,"ERETNoMem ", err);
		return(err);
	}
	DEBUG_DATA(lustat->lu_dbg, "ET  PpMem ", size);

	/* Zero out the entire space, to be safe */
	memset(enet_entry, 0, size);

	enet_entry->next = drstat->dr_pp_list;
	enet_entry->enet_pd = lustat->lu_pathdesc;
	enet_entry->enet_dndrvr = lustat->lu_dndrvr;

	enet_entry->hw_txoffset = 0;
	enet_entry->addr_list = (struct addr_list *) ((u_int32)enet_entry +
										sizeof(struct pp_enetlist));

	drstat->dr_pp_list = enet_entry;
	drstat->dr_num_enet_paths++;

	/*
	** We need to get the hardware address from the interface
	** below us and also enable it. We only do this if we
	** actually have a driver below us however.
	*/
	if (enet_entry->enet_dndrvr) {
		new_pb.code = SPF_GS_ETHADDR;
		new_pb.size = sizeof(enet_entry->ether_addr);
		new_pb.param = enet_entry->ether_addr;
		new_pb.updir = SPB_GOINGDWN;

		SMCALL_GS(deventry, enet_entry->enet_dndrvr, &new_pb);

		new_pb.code = SPF_SS_HWEN;
		new_pb.size = 0;
		new_pb.param = NULL;
		new_pb.updir = SPB_GOINGDWN;

		if ((err = SMCALL_SS(deventry, enet_entry->enet_dndrvr,
										&new_pb)) != SUCCESS) {
			return (err);
		}
	}

	return(SUCCESS);
}

/* Find an entry to the ENET path list. This is how we find the entry
** when a locally generated request comes in.
*/
Pp_enetlist find_enet_entry(Spf_drstat drstat, Spf_pdstat pd) {
	Pp_enetlist enet_list = drstat->dr_pp_list;

	while (enet_list != NULL) {
		if (enet_list->enet_pd == pd)
			return(enet_list);
		enet_list = enet_list->next;
	}

	return(NULL);
}

/* Delete an entry to the ENET path list */
void del_enet_entry(Dev_list dev_entry, Spf_pdstat pd) {
	Spf_drstat		drstat = (Spf_drstat)(dev_entry->v_dr_stat);
	Spf_lustat		lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	Pp_enetlist		enet_list = drstat->dr_pp_list, prev;

	if (enet_list == NULL)
		return;

	prev = enet_list;

	while ((enet_list->enet_pd != pd) && (enet_list != NULL)) {
		prev = enet_list;
		enet_list = enet_list->next;
	}

	if (enet_list != NULL) {
		if (enet_list == drstat->dr_pp_list) {
			drstat->dr_pp_list = enet_list->next;
		} else {
			prev->next = enet_list->next;
		}

		drstat->dr_num_enet_paths--;
		(void)_os_srtmem(sizeof(pp_enetlist)
					+ lustat->lu_numprotoaddr * sizeof(struct addr_list),
					(void *)enet_list);
	}
}

/***************************************************************************
** Device Driver Initialization Entry point
****************************************************************************/
error_code dr_iniz(Dev_list dev_entry) {

	Spf_lustat 	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	Spf_drstat	drstat = (Spf_drstat)(dev_entry->v_dr_stat);
	struct arpstat *arpstat = &drstat->arpstat;

	/*
	** Get system globals
	*/
	drstat->sysglobs = (Sysglobs)get_sysglobs();

	/*
	** On OS-9 we will use the number of seconds until midnight and on
	** OS-9000 we use the number of seconds since 1/1/1970. It doesn't
	** matter that these are different things because all we want is
	** a value that changes every second to prevent arp flooding.
	*/
#if defined(_OSK)
	drstat->timer_sec = &drstat->sysglobs->d_second;
#else
	drstat->timer_sec = &drstat->sysglobs->d_time;
#endif

	drstat->arpent_cache = &drstat->arpstat.arproot;

	/*
	** If this descriptor is new enough use its timeout parameters. If
	** lu_num is 5 or less leave them at their defaults.
	*/
	if (lustat->lu_num > 5) {
		drstat->timer_int = lustat->lu_timer_int * 1000;
		drstat->kill_c = lustat->lu_kill_c;
		drstat->kill_i = lustat->lu_kill_i;
	}

	/* Initialize variables in arptick's static storage */
	arpstat->arproot.next = arpstat->arproot.prev = &arpstat->arproot;
	arpstat->busy_flag = FALSE;
	arpstat->busy_count = 0;

	lustat->lu_ioenabled = 1;

#ifdef DEBUG
	if (debug_init(DEBUG,(Dbg_stat *)&lustat->lu_dbg,lustat->lu_dbg_name) != SUCCESS){
		lustat->lu_dbg = NULL;
	}
	debug_data(lustat->lu_dbg, "ET  dr_inz", 0);
#endif

	/* set up the timer parameter block for arp reply timeout */
	drstat->arpreq_timeout.timer_type = TIMER_CYCLIC;
	drstat->arpreq_timeout.timer_call_type = CALL_FUNCTION;
	drstat->arpreq_timeout.timer_interval = drstat->timer_int;
	drstat->arpreq_timeout.t_func = arptick;
	drstat->arpreq_timeout.t_pb = (void *) dev_entry;

	/* Start the timer now */
	DEBUG_DATA(lustat->lu_dbg, "ET  StTimr",
				(u_int32)drstat->arpreq_timeout.timer_interval);

	timer_start(dev_entry, &drstat->arpreq_timeout);

	DEBUG_DATA(lustat->lu_dbg, "ET  EndInz", 0);
	DEBUG_DATA(lustat->lu_dbg, "ET  LuStat", (u_int32)lustat);
	DEBUG_DATA(lustat->lu_dbg, "ET  Drstat", (u_int32)drstat);

	return(SUCCESS);
}

/*
** Device Driver Termination Entry point
*/
error_code dr_term(Dev_list dev_entry) {
	Spf_lustat	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	Spf_drstat	drstat = (Spf_drstat)(dev_entry->v_dr_stat);
	Pp_enetlist	enet_entry;

	lustat->lu_ioenabled = 0;

	#ifdef DEBUG
		if (drstat->dr_pp_list != NULL) {
			debug_data(lustat->lu_dbg, "ERETClnPp ", 0);
		}
	#endif

	/* stop the timer if it is running */
	timer_stop(dev_entry, &drstat->arpreq_timeout);

	while ((enet_entry = drstat->dr_pp_list) != NULL) {
		drstat->dr_pp_list = enet_entry->next;
		enet_entry->next = NULL;

		drstat->dr_num_enet_paths--;
		(void)_os_srtmem(sizeof(pp_enetlist) + lustat->lu_numprotoaddr,
													(void *)enet_entry);
	}

	DEBUG_DATA(lustat->lu_dbg, "ET  dr_trm", 0);

	return(SUCCESS);
}

/****************************************************************************
** Device Driver Get-Stat Entry point
*****************************************************************************/
error_code dr_getstat(Dev_list dev_entry, Spf_ss_pb pb) {
	Spf_lustat lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	Spf_drstat drstat = (Spf_drstat)(dev_entry->v_dr_stat);
	Pp_enetlist	enet_entry;
	error_code err;

	switch (pb->code) {
		case SPF_GS_PROTID: {
            		pb->param = (void *)SPF_PR_ENET;
            		return(SUCCESS);
        	}

		case SPF_GS_UPDATE:	{	/* Attempting to get update statistics	*/
			Spf_pdstat 		pd = lustat->lu_pathdesc;
			Spf_update_pb	upb = (Spf_update_pb)pb;
			Pp_enetlist		enet_entry;

			DEBUG_DATA(lustat->lu_dbg, "ET  gsUpdt", pb->code);
			DEBUG_DATA(lustat->lu_dbg, "ET  TXSize", lustat->lu_txsize);
			DEBUG_DATA(lustat->lu_dbg, "ET  TXOffs", lustat->lu_txoffset);
			DEBUG_DATA(lustat->lu_dbg, "ET  IOEnab", lustat->lu_ioenabled);

			/* If we can't find the entry for the path, return an error */
			if ((enet_entry = find_enet_entry(drstat, pd)) == NULL) {
				DEBUG_DATA(lustat->lu_dbg, "ERETNoEntr", (u_int32)drstat);
				return(EOS_PPS_NOTFND);
			}

			if (enet_entry->enet_dndrvr) {
				if ((err = SMCALL_GS(dev_entry, enet_entry->enet_dndrvr, pb))
																!= SUCCESS) {
					return(err);
				}

				/* Now Update per my protocol status */

				/* We have no fragmenting ability, but we also allow
				 * as big a packet as can be sent. So we won't make
				 * any changes to upb->stk_txsize that came from below.
				 */

				/* If there are bytes that need to be allowed for the
				 * header, we would add them to what came from below:
				 */
				 enet_entry->hw_txoffset = upb->stk_txoffset;
				 upb->stk_txoffset = upb->stk_txoffset + lustat->lu_txoffset;

				/* We either send up IO DISABLED if we are, or if we are
				 * enabled, send up whatever the lower one put in there.
				 */
				if (lustat->lu_ioenabled == DRVR_IODIS)
					upb->stk_ioenabled = DRVR_IODIS;

			} else {	/* We're the lowest driver on the stack */

				/* If no one's below us, copy our stats into the
				 * parameter block and return.
				 */
				upb->stk_txsize 	= lustat->lu_txsize;
				upb->stk_txoffset 	= lustat->lu_txoffset;
				upb->stk_ioenabled 	= lustat->lu_ioenabled;
			}

			return(SUCCESS);

		} /* End SPF_SS_UPDATE */

		case ENET_GS_STATS: {
			Enet_stat_pb	stat_pb;

			DEBUG_DATA(lustat->lu_dbg, "ET  gsGES ", pb->code);
			_asm("gsstats0:");

			/* set up a pointer to the parameter block */
			stat_pb = (Enet_stat_pb)pb->param;

			stat_pb->ip_count = drstat->ip_count;
			stat_pb->good_ip_count = drstat->ip_count;
			stat_pb->arp_count = drstat->arp_count;
			stat_pb->rarp_count = drstat->rarp_count;
			stat_pb->unknown = drstat->unknown;

			return(SUCCESS);
		}

		case SPF_GS_ARPENT: {
			error_code err;
			Pd_com path_desc = &lustat->lu_pathdesc->pd_common;

			ALGN_DBG_DATA(lustat->lu_dbg, "ET:GSARPENT", 10, pb->code);
			_asm("gsarpent0:");

			if ( ( (path_desc->pd_state & PD_SYSTATE) == 0) &&
					(err = check_access(drstat, pb->param, sizeof(struct arptab),
										S_IREAD|S_IWRITE) ) ) {
				return(err);
			}

			return(arpent_copy(&drstat->arpstat, (struct arptab*) pb->param));
		}

		case SPF_GS_ARPTBL: {
			error_code err;
			Pd_com path_desc = &lustat->lu_pathdesc->pd_common;

			ALGN_DBG_DATA(lustat->lu_dbg, "ET:GSARPTBL", 10, pb->code);
			_asm("gsarptbl0:");

			if ( ( (path_desc->pd_state & PD_SYSTATE) == 0) &&
					(err = check_access(drstat, pb->param, pb->size, S_IWRITE) ) ) {
				return(err);
			}

			return(arptbl_copy(&drstat->arpstat, (struct arptab*) pb->param,
																 &pb->size));
		}

		default: {
			DEBUG_DATA(lustat->lu_dbg, "ET  gsunk ", pb->code);
			_asm("gsdefault0:");

			if ((enet_entry = find_enet_entry(drstat, lustat->lu_pathdesc)) == NULL) {
				return(EOS_PPS_NOTFND);
			}
			if (pb->updir == SPB_GOINGDWN) {
				if (enet_entry->enet_dndrvr) {
					return(SMCALL_GS(dev_entry, enet_entry->enet_dndrvr, pb));
				}
			} else {
				if (enet_entry->enet_updrvr) {
					return(SMCALL_GS(dev_entry, enet_entry->enet_updrvr, pb));
				}
			}
		} /* End default */

	} /* End switch */

	return(EOS_UNKSVC);
}

/*
** Device Driver Set-Stat Entry point
*/
error_code dr_setstat(Dev_list dev_entry, Spf_ss_pb pb) {
	Spf_lustat lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	Spf_drstat drstat = (Spf_drstat)(dev_entry->v_dr_stat);
	Spf_pdstat pd = lustat->lu_pathdesc;
	Pp_enetlist enet_entry;
	error_code err;
	int addr_ptr;

	switch(pb->code) {

		case SPF_SS_OPEN: {
			DEBUG_DATA(lustat->lu_dbg, "ET  ssOpen", pb->code);
			_asm("ssopen0:");

			err = add_enet_entry(dev_entry, drstat, lustat);
			return(err);
		}

		case SPF_SS_CLOSE: {
			spf_ss_pb new_pb;

			DEBUG_DATA(lustat->lu_dbg, "ET  SsClos", pb->code);
			_asm("ssclose0:");

			if ((enet_entry = find_enet_entry(drstat, pd)) == NULL) {
				ALGN_DBG_DATA(lustat->lu_dbg, "ET:SSOpnNoPP", 10, pd);
				return (EOS_PPS_NOTFND);
			}

			if (pd == drstat->pd_dncache) {
				drstat->pd_dncache = NULL;
				drstat->enet_entry_dncache = NULL;
			}

			if (enet_entry == drstat->enet_entry_upcache) {
				drstat->dev_upcache = NULL;
				drstat->enet_entry_upcache = NULL;
			}

			/*
			** If this path has a hardware driver below us we need to
			** turn it off. We don't bother checking for an error
			** returned because we can't do anything about anyway.
			*/
			if (enet_entry->enet_dndrvr) {
				new_pb.code = SPF_SS_HWDIS;
				new_pb.size = 0;
				new_pb.param = NULL;
				new_pb.updir = SPB_GOINGDWN;

				SMCALL_SS(dev_entry, enet_entry->enet_dndrvr, &new_pb);
			}

			del_enet_entry(dev_entry, pd);

			return(SUCCESS);
		}

		case SPF_SS_PUSH:
		case SPF_SS_POP: {
			ALGN_DBG_DATA(lustat->lu_dbg, "ET:SSPUSH", 10, pb->code);
			_asm("sspush0:");

			if ((enet_entry = find_enet_entry(drstat, pd)) == NULL) {
				ALGN_DBG_DATA(lustat->lu_dbg, "ET:SSPNOPPStg", 10, pd);
				return (EOS_PPS_NOTFND);
			}
			if (drstat->spip_deventry == NULL) {
				/*
				** We assume the first person to push on us is spip. I suppose
				** we really should check the protocol type to be sure.
				*/
				drstat->spip_deventry = lustat->lu_updrvr;
			}
			enet_entry->enet_updrvr = lustat->lu_updrvr;
			return(SUCCESS);
		}

		case SPF_SS_SETADDR: {
			ALGN_DBG_DATA(lustat->lu_dbg, "ET:SSADDADDR", 10, pb->code);
			_asm("sssetaddr0:");

			/*
			** Make sure the caller is a super user.
			*/
			if (!is_super(drstat)) {
				return(EOS_PERMIT);
			}

			if ((enet_entry = find_enet_entry(drstat, pd)) == NULL) {
				ALGN_DBG_DATA(lustat->lu_dbg, "ET:SSAddNoPP", 10, pd);
				return (EOS_PPS_NOTFND);
			}

			/* New address will be last in the array of addresses. */
			for (addr_ptr = 0;
					(enet_entry->addr_list[addr_ptr].in_useflag != 0 &&
					addr_ptr < lustat->lu_numprotoaddr); addr_ptr++)
				;

			if (addr_ptr == lustat->lu_numprotoaddr) {
				/* No more space avalable in protocol address array. */
				return (EOS_FULL);
			}

			/* Set the in_useflag of the addr_list entry */
			enet_entry->addr_list[addr_ptr].in_useflag = 1;
			/* Copy the inaddr itself */
			memcpy(&(enet_entry->addr_list[addr_ptr].proto_addr),
				(void *) &((struct sockaddr_in *) pb->param)->sin_addr,
													sizeof(struct in_addr));

			return (SUCCESS);
		}

		case SPF_SS_DELADDR: {
			ALGN_DBG_DATA(lustat->lu_dbg, "ET:SSDELADDR", 10, pb->code);
			_asm("ssdeladdr0:");

			/*
			** Make sure the caller is a super user.
			*/
			if (!is_super(drstat)) {
				return(EOS_PERMIT);
			}

			if ((enet_entry = find_enet_entry(drstat, pd)) == NULL) {
				ALGN_DBG_DATA(lustat->lu_dbg, "ET:SSDelNoPP", 10, pd);
				return (EOS_PPS_NOTFND);
			}

			if ((addr_ptr = verify_protoaddr((struct in_addr *)
							&((struct sockaddr_in *) pb->param)->sin_addr,
								enet_entry, lustat->lu_numprotoaddr)) == -1) {
				/* address not found. Bad error code! */
				return (EADDRNOTAVAIL);
			}

			/* Reset the in_useflag of the addr_list entry */
			enet_entry->addr_list[addr_ptr].in_useflag = 0;
			return (SUCCESS);
		}

		case SPF_SS_ADDARP: {
			ALGN_DBG_DATA(lustat->lu_dbg, "ET:SSADDARP", 10, pb->code);
			_asm("ssaddarp0:");

			/*
			** Make sure the caller is a super user.
			*/
			if (!is_super(drstat)) {
				return(EOS_PERMIT);
			}

			return (arp_change(dev_entry, (struct arpreq*) pb->param));
		}

		case SPF_SS_DELARP: {
			ALGN_DBG_DATA(lustat->lu_dbg, "ET:SSDELARP", 10, pb->code);
			_asm("ssdelarp0:");

			/*
			** Make sure the caller is a super user.
			*/
			if (!is_super(drstat)) {
				return(EOS_PERMIT);
			}

			return (arp_delete(dev_entry, (struct arpreq*) pb->param));
		}

		default: {
			DEBUG_DATA(lustat->lu_dbg, "ET  Ssunk ", pb->code);
			_asm("ssdefault0:");

			if ((enet_entry = find_enet_entry(drstat, pd)) == NULL) {
				return(EOS_PPS_NOTFND);
			}
			if (pb->updir == SPB_GOINGDWN) {
				if (enet_entry->enet_dndrvr) {
					return(SMCALL_SS(dev_entry, enet_entry->enet_dndrvr, pb));
				}
			} else {
				if (enet_entry->enet_updrvr) {
					return(SMCALL_SS(dev_entry, enet_entry->enet_updrvr, pb));
				}
			}
		}
	}

	return(EOS_UNKSVC);
}

/*
** Device Driver Downgoing-Data Entry point
*/
error_code dr_downdata(Dev_list dev_entry, Mbuf mb) {

	Spf_lustat	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	Spf_drstat	drstat = (Spf_drstat)(dev_entry->v_dr_stat);
	Pp_enetlist	enet_entry;
	struct in_addr destip;
	u_int32		*mb_start;
	u_int16		dest_hwaddr[3];
	u_int16		*dest_ethaddr = dest_hwaddr;
	Mbuf		temp;
	int32		plen;
	int32		status;
	Arptab		arpent;
	Ether_header etherhead;
	u_int16		*p;
	u_int16		*src_eth;

	if (lustat->lu_pathdesc == drstat->pd_dncache) {
		enet_entry = drstat->enet_entry_dncache;
	} else {
		if ((enet_entry = find_enet_entry(drstat, lustat->lu_pathdesc)) == NULL) {
			DEBUG_DATA(lustat->lu_dbg, "ERETNoEntr", (u_int32)drstat);
			m_free_p(mb);
			return(EOS_PPS_NOTFND);
		}

		if (enet_entry->enet_dndrvr == NULL){
			m_free_p(mb);
			return(EOS_PTHLOST);
		}
		drstat->enet_entry_dncache = enet_entry;
		drstat->pd_dncache = lustat->lu_pathdesc;
	}

	#ifdef DEBUG
		if (!mb) {
			debug_data(lustat->lu_dbg, "ERETNoDnDt", 0);
		}
		debug_data(lustat->lu_dbg, "ET  DnData", (u_int32) mb->m_size);
	#endif

	/*
	** The next hop address is stored in the 4 bytes immediately preceding
	** the start of mbuf data. We need to use this address for the arp
	** table lookup.
	*/
	mb_start = mtod(mb, u_int32 *);
	destip.s_addr = *(mb_start-1);

	/*
	** Verify we have enough mbuf space for our header. If not allocate
	** a new mbuf. (RDS - Should chain instead)
	*/
	if (mb->m_offset - MBHDR_SZ < ETHERHEADSIZE){
		plen = m_len_p(mb);
		if ((temp = m_getn(plen + ETHERHEADSIZE + 16, &status)) == NULL) {
			m_free_p(mb);
			return(ENOBUFS);
		}
		/*
		** Leave 16 extra bytes at front of the new mbuf (some drivers need
		** a few bytes) and copy the old data into it. We avoid using either
		** m_copy or m_pullup because they allocate mbufs and don't preserve
		** empty space.
		*/
		temp->m_offset += (16 + ETHERHEADSIZE);
		temp->m_size = plen;
		m_move(mb, 0, plen, mtod(temp, char *));
		m_free_p(mb);
		mb = temp;
		mb_start = mtod(mb, u_int32 *);
	}

	DEBUG_DATA(lustat->lu_dbg, "ET  dstadr", destip.s_addr);

	arpent = drstat->arpent_cache;

	if (mb->m_flags & M_BCAST) {
		dest_hwaddr[0] = 0xffff;
		dest_hwaddr[1] = 0xffff;
		dest_hwaddr[2] = 0xffff;
	} else {
		if (mb->m_flags & M_MCAST) {
			ETHER_MAP_IP_MULTICAST(&destip.s_addr, (u_char *)dest_hwaddr);
		} else {
  			if (arpent->at_iaddr.s_addr == destip.s_addr) {
				arpent->at_timer = 0;
			} else {
				/* Resolve the Ethernet address from the arp cache */
				if ((arpent = arpresolve(dev_entry, enet_entry, mb, &destip,
								(u_int8 *)dest_hwaddr)) == NULL) {
					/*
					** Ethernet address was not resolved so we return here. The
					** mbuf being sent was queued by arpresolve so we don't free
					** it before returning.
					*/
					return(SUCCESS);
				}
				drstat->arpent_cache = arpent;
			}
			dest_ethaddr = (u_int16 *)arpent->at_enaddr;
		}
	}

	DEBUG_STRING(lustat->lu_dbg, mtod(mb, u_char *), mb->m_size);

	mb->m_size += sizeof(struct ether_header);
	mb->m_offset -= sizeof(struct ether_header);

	etherhead = (struct ether_header *)((u_int32)mb_start - 14);
	etherhead->ether_type = htons(ETHERTYPE_IP);

	p = (u_int16 *)etherhead;
	p[0] = dest_ethaddr[0];
	p[1] = dest_ethaddr[1];
	p[2] = dest_ethaddr[2];

	src_eth = (u_int16 *)enet_entry->ether_addr;
	p[3] = src_eth[0];
	p[4] = src_eth[1];
	p[5] = src_eth[2];

	ALGN_DBG_DATA(lustat->lu_dbg, (u_char *)"ET:DOWNDT", 10, (u_int32) mb);
	DEBUG_STRING(lustat->lu_dbg, mtod(mb, u_char *), mb->m_size);

	/* send the data down to lower protocols */
	return(SMCALL_DNDATA( dev_entry, enet_entry->enet_dndrvr, mb));
}

/*
** Device Driver Upcoming-Data Entry point
*/
error_code dr_updata(Dev_list dev_entry, Mbuf mb) {

	Spf_lustat	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	Spf_drstat	drstat = (Spf_drstat)(dev_entry->v_dr_stat);
	Pp_enetlist	enet_entry;
	u_int32		*mb_start;
	Dev_list	dndrvr_deventry;

	#ifdef DEBUG
		if (!mb) {
			debug_data(lustat->lu_dbg, "ERETNoUpDt", 0);
		}
	#endif

	DEBUG_DATA(lustat->lu_dbg, "ET  UPData", (u_int32) mb);

	/*
	** Get the deventry of the driver giving us the packet. They
	** should have stuck it in the second u_int32 after the mbuf
	** header.
	*/
	mb_start = (u_int32 *)(((int)mb + MBHDR_SZ +sizeof(void *)));
	dndrvr_deventry = (Dev_list)*mb_start;

	if (dndrvr_deventry == drstat->dev_upcache) {
		enet_entry = drstat->enet_entry_upcache;
	} else {
		if ((enet_entry = find_enet_ppstg(drstat, dndrvr_deventry)) == NULL) {
			ALGN_DBG_DATA(lustat->lu_dbg, "ET:NoPPStg", 10, 0);
			m_free_p(mb);
			return(EOS_PPS_NOTFND);
		}

		drstat->enet_entry_upcache = enet_entry;
		drstat->dev_upcache = dndrvr_deventry;
	}

	if ((mb = process_enet(dev_entry, enet_entry, mb)) == NULL) {
		/*
		** Returning NULL indicates the packet has been processed and
		** we don't have to pass it up the stack. The mbuf has been
		** free'd so we don't do it here.
		*/
		return(SUCCESS);
	}

	if (enet_entry->enet_updrvr) {
		DEBUG_DATA(lustat->lu_dbg, "ET  Updata", (u_int32)mb);

		/*
		** All Ethernet interfaces pass data up to SPIP through
		** SPENET. In order for SPIP to tell which interface the
		** data is coming from we will set the path descriptor.
		*/
		lustat->lu_pathdesc = enet_entry->enet_pd;

		DEBUG_STRING(lustat->lu_dbg, mtod(mb, u_char *), mb->m_size);

		/* send the data up to upper protocols */
		return(SMCALL_UPDATA(dev_entry, enet_entry->enet_updrvr, mb));

	} else {
		DEBUG_DATA(lustat->lu_dbg, "ERETNoUpdv", (u_int32)mb);
		/* dump the packet */
		m_free_p(mb);
		return (SUCCESS);
	}
}

static error_code
check_access(Spf_drstat drstat, void *mem_ptr, u_int32 mem_size, u_int32 mode)
{
#if !defined(HAS_NO_CHECKS)
	pr_desc		*pdesc;

	pdesc = drstat->sysglobs->d_proc;
	return(_os_chkmem(mem_size, mode, mem_ptr, pdesc));

#else /* !defined(HAS_NO_CHECKS) */
	return(0);

#endif /* !defined(HAS_NO_CHECKS) */
}

/* return true if current process is super user */
int is_super(Spf_drstat drstat)
{
	Pr_desc pdesc;

	pdesc = drstat->sysglobs->d_proc;
	return(pdesc->p_group == 0);
}

