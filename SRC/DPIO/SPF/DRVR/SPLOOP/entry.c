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
 * Edition History:                                                         *
 * #   Date     Comments                                          By        *
 * --- -------- ------------------------------------------------- --------- *
 *   1 01/07/94 Created from SPF v1.0.2                                 bat *
 *   2 03/28/95 Reorganization of logical unit & driver static          bat *
 *   3 04/18/95 Split driver into DCE/DTE sides                         bat *
 *     06/30/95 <--------------- SPF 2.0 pre-release------------>       cas *
 *   4 10/09/95 Added x86 binary support                                dwj *
 *     10/13/95 <-------------------- SPF 2.0 ------------------>       dwj *
 * 202 02/18/97 Ported to x86                                           taka*
 * 203 02/28/97 Port to ARM                                             taka*
 * 204 04/10/97 TX_SIZE from descriptor                                 taka*
 *     06/26/97 <---------------- SPF v2.2 --------------------->       taka*
 * 205 07/14/97 setstat close fix for far-end-hangup registered path    taka*
 * 206 10/16/97 setstat ITE_ANSWER fix for established connection       taka*
 * 208 10/30/97 Added timer function calls in setstat for timer testing taka*
 *              ITE_TIMER_START, ITE_TIMER_STOP, ITE_TIMER_RESTART          *
 * 210 03/23/98 Removed unreferenced variables to remove compiler warnings  *
 *                                                                      taka*
 *     03/23/98 <---------------- SPF v2.2.1 ------------------->       taka*
 * 211 06/19/98 Bug Fix: DDTS 4217 - incomplete copy of mbuf chain      taka*
 *     07/19/98 <---------------- SPF v2.2.2 ------------------->       taka*
 ****************************************************************************/

/*
**	Header Files
*/

#include <SPLOOP/defs.h>

/*
**	Miscellaneous functions for SPLOOP test emulation for ITEM
*/

/* This function takes care of sending the right kind of notification
 * to the process waiting for it.
 */
u_int32 send_notify(Notify_type notify_ptr)
{
	notify_ptr->ntfy_on = ITE_ON_NONE;

	switch (notify_ptr->ntfy_class) {
		case ITE_NCL_SIGNAL: {
			_os_send(notify_ptr->ntfy_procid, notify_ptr->ntfy_sig2send);
		}

		case ITE_NCL_EVENT: {
			_os_ev_set(notify_ptr->ntfy_evid, &notify_ptr->ntfy_eval, 1);
		}
	}

	return(SUCCESS);
}

/* Add entry to the loopback path list */
error_code add_lpbk_list(Spf_drstat drstat, Spf_lustat lustat)
{
	Lpbk_list	memptr;
	u_int32		size = sizeof(lpbk_list);
	error_code	err;

	if ((err = _os_srqmem(&size, (void**)&memptr, MEM_ANY)) != SUCCESS)
		return(err);

	memptr->next 		= drstat->dr_loopback_list;
	memptr->lpbk_pd 	= lustat->lu_pathdesc;
	memptr->lpbk_updrvr = lustat->lu_updrvr;
	memptr->lpbk_dndrvr = lustat->lu_dndrvr;
	
	drstat->dr_loopback_list = memptr;	
	drstat->dr_num_lpbk_paths++;
	
	return(SUCCESS);
}

/* Delete an entry to the loopback path list */
void del_lpbk_list(Spf_drstat drstat, Spf_lustat lustat)
{
	Spf_pdstat	pd = lustat->lu_pathdesc;
	Lpbk_list	loop_list = drstat->dr_loopback_list, prev;

	if (loop_list == NULL)	return;

	prev = loop_list;
	
	while ((loop_list->lpbk_pd != pd) && (loop_list != NULL)) {
		prev = loop_list;
		loop_list = loop_list->next;
	}

	if (loop_list != NULL) {

		if (loop_list == drstat->dr_loopback_list) {
			drstat->dr_loopback_list = loop_list->next;
		} else {
			prev->next = loop_list->next;
		}

		drstat->dr_num_lpbk_paths -= 1;
					
		(void)_os_srtmem(sizeof(lpbk_list), (void *)loop_list);
	}
}

/* Find the updrvr device entry in the loopback list. If it's not
 * there, return NULL.
 */
Dev_list find_updrvr(Spf_drstat drstat, Spf_pdstat pd)
{
	Lpbk_list	loop_list = drstat->dr_loopback_list;

	while (loop_list != NULL) {
		if (loop_list->lpbk_pd == pd) {
			return(loop_list->lpbk_updrvr);
		}
		loop_list = loop_list->next;
	}		
	return(NULL);
}

/* This function finds the loopback list element for this path and
 * changes the updrvr (Called during push and pop).
 */
error_code set_new_updrvr(Spf_drstat drstat, Spf_pdstat pd, Dev_list new_updrvr)
{
	Lpbk_list	loop_list = drstat->dr_loopback_list;

	while (loop_list != NULL) {
		if (loop_list->lpbk_pd == pd) {
			loop_list->lpbk_updrvr = new_updrvr;
			return(SUCCESS);
		}
		loop_list = loop_list->next;
	}		
	return(EOS_PTHLOST);
}

/****************************************************************************
**	Device Driver Initialization Entry point
*****************************************************************************/
error_code dr_iniz(Dev_list deventry)
{
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat);

	/* IO Enabled doesn't mean we have an end to end connection,
	 * just that the protocol is happy. Since we don't have
	 * a state machine protocol, we're always happy.
	 */
	lustat->lu_ioenabled = DRVR_IOEN;

	/* If our logical unit is in the connectionless device range, our
	 * sendto LUN better also be in the correct range. We won't check
	 * here, we'll just assume they got it right in the descriptor.
	 */

	#if defined(DEBUG)
		if (debug_init(DEBUG, (Dbg_stat*)&lustat->lu_dbg, lustat->lu_dbg_name)
																!= SUCCESS) {
			lustat->lu_dbg = NULL;
		}
		debug_data(lustat->lu_dbg,"LOOP:iniz ",0);
	#endif

	return(SUCCESS);
}

/****************************************************************************
**	Device Driver Termination Entry point
*****************************************************************************/
error_code dr_term(Dev_list deventry)
{
#ifdef DEBUG
	Spf_lustat lustat = (Spf_lustat)(deventry->v_lu_stat);
	Spf_drstat drstat = (Spf_drstat)(deventry->v_dr_stat);
#endif

	DEBUG_DATA(lustat->lu_dbg,"DrTerminat", deventry);
	DEBUG_4DATA(lustat->lu_dbg, drstat->dr_att_cnt, drstat->dr_use_cnt,
								lustat->lu_att_cnt, lustat->lu_use_cnt);

	return(SUCCESS);
}

/****************************************************************************
**	Device Driver Get-Stat Entry point
*****************************************************************************/
error_code dr_getstat(Dev_list deventry, Spf_ss_pb spb)
{
#ifdef DEBUG
	Spf_drstat	drstat = (Spf_drstat)(deventry->v_dr_stat);
#endif
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat);

	DEBUG_DATA(lustat->lu_dbg,"LP:GetStat", deventry);
	DEBUG_4DATA(lustat->lu_dbg, drstat->dr_att_cnt, drstat->dr_use_cnt,
								lustat->lu_att_cnt, lustat->lu_use_cnt);

	switch (spb->code) {

		case SPF_GS_DEVENTRY: {                 /* Query for device entry   */
			*((Dev_list *)spb->param) = deventry;
			return(SUCCESS);
		}

		case SPF_SS_UPDATE: {

			Spf_update_pb upb = (Spf_update_pb)spb;
			
			DEBUG_DATA(lustat->lu_dbg,"GSUpdate:",spb->code);

			/* Since I'm always lowest on the stack, I just fill my
			 * variables and return.
			 */
			upb->stk_txsize = lustat->lu_txsize;
			upb->stk_txoffset = lustat->lu_txoffset;
			upb->stk_ioenabled = lustat->lu_ioenabled;

			DEBUG_DATA(lustat->lu_dbg,"TXsize =:", upb->stk_txsize);
			DEBUG_DATA(lustat->lu_dbg,"TXoffset =:", upb->stk_txoffset);
			DEBUG_DATA(lustat->lu_dbg,"IOenabled=", upb->stk_ioenabled);

			return(SUCCESS);
		}

		#ifdef DEBUG
			case ITE_TEST_LULIST: {

				Spf_lustat *ptr = (Spf_lustat *)spb->param;
			
				DEBUG_DATA(lustat->lu_dbg,"GSTstLuLst: ",spb->code);

				*ptr = lustat;
				return(SUCCESS);
			}
		#endif
			
	} /* End switch */

	DEBUG_DATA(lustat->lu_dbg,"GSUnknown:",spb->code);

	return(EOS_UNKSVC);
}

/****************************************************************************
**	Device Driver Set-Stat Entry point
*****************************************************************************/
error_code dr_setstat(Dev_list deventry, Spf_ss_pb spb)
{
	Spf_lustat lustat = (Spf_lustat)(deventry->v_lu_stat);
	Spf_drstat drstat = (Spf_drstat)(deventry->v_dr_stat);
	Spf_pdstat pd =  lustat->lu_pathdesc;
	error_code err;
	
	DEBUG_DATA(lustat->lu_dbg,"LP:SetStat", deventry);
	DEBUG_4DATA(lustat->lu_dbg, drstat->dr_att_cnt, drstat->dr_use_cnt,
								lustat->lu_att_cnt, lustat->lu_use_cnt);

	switch (spb->code) {

		case SPF_SS_OPEN: {			/* hook to new upper/lower protocols */

			DEBUG_DATA(lustat->lu_dbg, "LP  SsOpen", lustat->LU_CURRPATH);
			DEBUG_DATA(lustat->lu_dbg, "LP  PathD ",
											lustat->lu_pathdesc);
			DEBUG_DATA(lustat->lu_dbg, "LP  Calist", 
											&drstat->dr_call_list);
			
			/* 
			 * If the LUN is in range of the call list array, put it in.
			 */
			if (lustat->lu_num < MAX_CALL_ENDPTS) {

/* I think this was commented out because the code depends on only one
 * process opening a particular device descriptor. The only descriptor
 * that can be opened multiple times is the /loop descriptor.
 *
 * However, because it's commented out I might have made a change to allow
 * multiple paths opening the same device descriptor, so I'll leave it
 * ifdef'd out for now.
 */
#if 0
				/* see if someone's already opened this logical unit */
				if (lustat->lu_itempath != NULL) {
					return(EOS_DEVBSY);
				}
#endif
				drstat->dr_call_list[lustat->lu_num] = lustat;
				drstat->dr_path_list[lustat->lu_num] = lustat->lu_pathdesc;
				drstat->dr_dev_list[lustat->lu_num] = deventry;

			} else {
				if ((err = add_lpbk_list(drstat, lustat)) != SUCCESS)
					return(err);
			}

			lustat->lu_felustat = NULL;
			lustat->lu_itempath = &lustat->lu_pathdesc->pd_item;

			return(SUCCESS);
		}

		case SPF_SS_PUSH:			/* push new upper protocol driver	*/
		case SPF_SS_POP:			/* push new upper protocol driver	*/
		{

			DEBUG_DATA(lustat->lu_dbg,"SS_PUSH/POP", spb->code);
			
			/* We're allowing multiple open paths to the same /loop
			 * descriptor. Therefore, we have to allocate unique
			 * per path storage for each open path because all paths
			 * will share the same driver and logical unit statics.
			 * At this point, we can copy the current updrvr and path
			 * descriptor into this storage so we always know how to loop
			 * up for that given path.
			 */
			if (lustat->lu_num >= MAX_CALL_ENDPTS) {
				return(set_new_updrvr(drstat, pd, lustat->lu_updrvr));
			}

			return(SUCCESS);
		}

		case SPF_SS_CLOSE: {		/* unhook from upper/lower protocols */

			Spf_pdstat	pd = lustat->lu_pathdesc;
			Path_type	item_path = lustat->lu_itempath;
			Device_type	item_dev = &item_path->conn_info;
			Notify_type	notify_ptr;

			DEBUG_DATA(lustat->lu_dbg,"SS_CLOSE    ",spb->code);

			if (item_path == &pd->pd_item) {

				/* Connection oriented devices: IF we're closing the path,
				 * this code checks to see if the person we're connected
				 * do is waiting for a far end hangup notification. If
				 * so, we send it. Real call control drivers would
				 * have to worry about closing paths that are in the
				 * process of connecting or hanging up too. Since we
				 * emulate those as 1 operation, we don't.
				 */

				if (lustat->lu_num < MAX_CALL_ENDPTS) {

					if (item_dev->dev_callstate == ITE_CS_ACTIVE) {

						if (lustat->lu_felustat) {
							if (lustat->lu_felustat->lu_itempath) {
								notify_ptr = &lustat->lu_felustat->lu_itempath->notify_list[ITE_ON_FEHANGUP];

								/* Check to see if someone's registered for
								 * notification on far end hangup.
								 */
								if (notify_ptr->ntfy_on != ITE_ON_NONE)
									send_notify(notify_ptr);
							}
							lustat->lu_felustat->lu_felustat = NULL;
 							lustat->lu_felustat = NULL;
						}
						item_dev->dev_callstate = ITE_CS_IDLE;
					}

					drstat->dr_call_list[lustat->lu_num] = NULL;
					drstat->dr_path_list[lustat->lu_num] = NULL;
					drstat->dr_dev_list[lustat->lu_num] = NULL;
				}
			
				lustat->lu_itempath = NULL;
			}

			if (lustat->lu_num >= MAX_CALL_ENDPTS) {
				del_lpbk_list(drstat, lustat);
			}

			return(SUCCESS);
		}

		case ITE_DIAL: {				/* Initiate a far end connection */

			Ite_conn_pb	cpb = (Ite_conn_pb)spb;
			Spf_lustat	their_lustat;
			Path_type	item_path = lustat->lu_itempath,
						their_itempath;
			Device_type	item_dev = &item_path->conn_info,
						their_itemdev;
			Addr_type	our_addr = cpb->our_num,
						their_addr = cpb->their_num;
			Notify_type	incall_ntfy_ptr;
			u_int16		their_lun;

			DEBUG_DATA(lustat->lu_dbg,"ITE_DIAL    ", cpb);

			if (our_addr)
				_os_move(our_addr, &item_dev->dev_ournum, sizeof(addr_type));

			if (their_addr)
				_os_move(their_addr,&item_dev->dev_theirnum,sizeof(addr_type));

			/* ITEM allows application developers to write for and run over
			 * any kind of network. Therefore, if an application writer
			 * wants to be comprehensive, they will always do a dial even
			 * if it might be a connectionless network. Therefore we shouldn't
			 * return an error if we are support this kind of portability.
			 * So, we just return successfully.
			 */
			if (item_dev->dev_callstate == ITE_CS_CONNLESS) {

				DEBUG_DATA(lustat->lu_dbg,"ClDevDial  ", cpb);

				return(SUCCESS);
			}

			/* Now that we've taken care of connectionless, we just
			 * have to worry about loopback and connection oriented.
			 * We're going to allow loopback to emulate a connection
			 * also using connect/hangup, but unlike connection oriented,
			 * we will allow loopback read/write data without call control
			 * or the correct call state.
			 */
			if (item_dev->dev_callstate & ITE_CS_IDLE) {

				if (lustat->lu_num < LU_FIRST_CONNLESS) {

					/* This piece deals with the connection oriented
					 * specifics of figuring out the far end logical
					 * unit static.
					 */

					/* If caller didn't pass in a path, use the one
					 * in the device_type structure of the path type.
					 */
					if (their_addr == NULL) {
						their_addr = &item_dev->dev_theirnum;
					}

					/* Now try to get the right far end logical unit out
					 * of the call list in the driver static.
					 */
					their_lun = (u_int16)their_addr->addr[0];

					their_lustat = drstat->dr_call_list[their_lun];

					/* If there's no far end, give up */					
					if (their_lustat == NULL) {
						DEBUG_DATA(lustat->lu_dbg,"NoFarEnd ", drstat);

						return(ENETUNREACH);
					}
				} else {
					/* It's easy for loopback, far end logical unit static
					 * is our logical unit static.
					 */
					their_lustat = lustat;
				}

				their_itempath = their_lustat->lu_itempath;
				their_itemdev = &their_itempath->conn_info;

				/* This checks to see if the far end is listening
				 * for an incoming call. If it isn't return network
				 * unreachable.
				 */
				if (their_itemdev->dev_rcvr_state != ITE_ASGN_ANY) {

					if (their_itemdev->dev_rcvr_state != ITE_ASGN_THEIRNUM) {
						DEBUG_DATA(lustat->lu_dbg,"NoRcvr ",
													their_itemdev);

						return(ENETUNREACH);
					} else {
							
						if (their_itemdev->dev_theirnum.addr[0] !=
														lustat->lu_num) {
							DEBUG_DATA(lustat->lu_dbg,"Incompat ",
													their_itemdev);

							return(ENETUNREACH);
						}
					}
				}

				/* If we execute this code, we've got a valid far end.
				 * We still need to check one more thing before being
				 * able to complete the notification.
				 */
				incall_ntfy_ptr = &their_itempath->notify_list[ITE_ON_INCALL];
								
				if (incall_ntfy_ptr->ntfy_on == ITE_ON_NONE) {
					DEBUG_DATA(lustat->lu_dbg,"ThrNtfyOff", 0);

					return(ENETUNREACH);
				} else {

					/* Set up the logical unit so data xfer will happen
					 * correctly and modify call states.
					 */

					/* Notice that this code could potentially blow over a
					 * current call if the far end is already connected to
					 * someone and has registered to receive more incoming
					 * calls.
					 *
					 * To ensure this doesn't happen, when apps are active on
					 * a call, they shouldn't register to receive notification
					 * of incoming call until they disconnect from the
					 * current call.
					 *
					 * Or this code could be rewritten to check for this
					 * scenario and return a far end busy error if the
					 * far end already has an active call.
					 */
					lustat->lu_felustat = their_lustat;
					their_lustat->lu_felustat = lustat;
					item_dev->dev_callstate = ITE_CS_CONNEST;
					their_itemdev->dev_callstate = ITE_CS_INCALL;
					strcpy(their_itemdev->dev_display, "INCALL:LogUnit #");
					their_itemdev->dev_display[16] = lustat->lu_num + '0';
					their_itemdev->dev_display[17] = 0;
					
					send_notify(incall_ntfy_ptr);
				}

			} else {	/* Wrong call state */

				DEBUG_DATA(lustat->lu_dbg,"BdCallState", 0);

				return(EOS_DEVBSY);
			}

			return(SUCCESS);
		}

		case ITE_ANSWER: {
			Spf_lustat	their_lustat = (Spf_lustat)lustat->lu_felustat;
			Path_type	item_path = lustat->lu_itempath;
			Device_type	item_dev = &item_path->conn_info;
			Notify_type	notify_ptr;

			DEBUG_DATA(lustat->lu_dbg,"ITE_ANSWER ", 0);

			/* If there is already a call in progress on the
			 * path or this is a connectionless device,
			 * don't bother continuing.
			 */
			if (item_dev->dev_callstate == ITE_CS_CONNLESS) {
				DEBUG_DATA(lustat->lu_dbg,"CnLsDevice ", 0);

				return(EOS_UNKSVC);
			}

			if (item_dev->dev_callstate & (ITE_CS_CONNEST |
										ITE_CS_ACTIVE | ITE_CS_CONNTERM)) {
				DEBUG_DATA(lustat->lu_dbg,"DeviceBusy", 0);
				return(EOS_DEVBSY);
			}
			
			/* If the app is trying to answer a call that doesn't exist,
			 * return a device not ready error.
			 */
			if (their_lustat == NULL) {
				return(EOS_NOTRDY);
			}
				
			/* Set both ends call state to ACTIVE */				
			item_dev->dev_callstate = ITE_CS_ACTIVE;
			lustat->lu_ioenabled = DRVR_IOEN;
			their_lustat->lu_itempath->conn_info.dev_callstate = ITE_CS_ACTIVE;
			their_lustat->lu_ioenabled = DRVR_IOEN;

			/* Notify the call originator of connect if needed */
			notify_ptr = &their_lustat->lu_itempath->notify_list[ITE_ON_CONN];
			
			if (notify_ptr->ntfy_on != ITE_ON_NONE)
				send_notify(notify_ptr);

			return(SUCCESS);
		}

		case ITE_HANGUP: {
			Spf_lustat	their_lustat = (Spf_lustat)lustat->lu_felustat;
			Path_type	item_path = lustat->lu_itempath;
			Device_type	item_dev = &item_path->conn_info;
			Notify_type	notify_ptr;

			DEBUG_DATA(lustat->lu_dbg,"ITE_HANGUP ", 0);

			/* If a connectionless device attempts to answer,
			 * return an unknown service error.
			 */
			if (item_dev->dev_callstate == ITE_CS_CONNLESS)
				return(EOS_UNKSVC);

			/* If there's no far end, just return successfully */
			if (their_lustat == NULL)
				return(SUCCESS);

			/* Set both ends call state to IDLE */				
			item_dev->dev_callstate = ITE_CS_IDLE;

			lustat->lu_felustat = NULL;

			their_lustat->lu_itempath->conn_info.dev_callstate = ITE_CS_IDLE;

			/* Notify the call originator of far end hangup if needed */
			notify_ptr = &their_lustat->lu_itempath->notify_list[ITE_ON_FEHANGUP];
			
			if (notify_ptr->ntfy_on != ITE_ON_NONE)
				send_notify(notify_ptr);

			return(SUCCESS);
		}

		/* Tracking the following assignment/removals is handled by the file
		 * manager by updating the notification array as needed. However,
		 * before logging it, it calls the protocol stack to allow
		 * the protocol drivers a chance to clean up their private
		 * informations pertaining to the assignment if needed.
		 */

		case ITE_FEHANGUP_ASGN:
		case ITE_FEHANGUP_REM: {
			Device_type	item_dev = &lustat->lu_itempath->conn_info;

			DEBUG_DATA(lustat->lu_dbg,"ITE_FEHANGUP", spb->code);

			/* Don't allow connectionless paths to do any call control
			 * stuff.
			 */
			if (item_dev->dev_callstate == ITE_CS_CONNLESS)
				return(EOS_UNKSVC);

			return(SUCCESS);
		}

		case ITE_RCVR_ASGN:
		case ITE_RCVR_REM: {
			Device_type	item_dev = &lustat->lu_itempath->conn_info;

			DEBUG_DATA(lustat->lu_dbg,"ITE_RCVR   ", spb->code);

			/* Don't allow connectionless paths to do any call control
			 * stuff.
			 */
			if (item_dev->dev_callstate == ITE_CS_CONNLESS)
				return(EOS_UNKSVC);

			return(SUCCESS);

		}

		case ITE_LINKDWN_ASGN:
		case ITE_LINKDWN_REM: {

			DEBUG_DATA(lustat->lu_dbg,"ITE_LNKDWN ", 0);

			/* There really is no concept of link down since this driver
			 * doesn't actually send data out over any wires, but we'll
			 * let the users log this anyway.
			 */
			return(SUCCESS);
		}

#ifdef _ATM
		case SPF_SS_VC_SET:	{ /* for atm testing */

		  return SUCCESS;
		}
#endif

		case ITE_TIMER_START: {
			Spf_pdstat 	pd = lustat->lu_pathdesc;
			Timer_pb 	tpb = (Timer_pb)spb->param;

			DEBUG_DATA(lustat->lu_dbg,"TIMER_START",0);

			/* The caller will have filled out from the timer type on
			 * down. If they want a timer_call_type of CALL_SIGNAL,
			 * then we need to fill out the proc2wake variable.
			 *
			 * Otherwise, everything else is already set up to do the call.
			 */
			if (tpb->timer_call_type == CALL_SIGNAL) {
				tpb->t_pwake = pd->pd_common.pd_cproc;
			}
			
			err = timer_start(deventry, tpb);

			DEBUG_DATA(lustat->lu_dbg,"TimerID=", tpb->timer_id);
			DEBUG_DATA(lustat->lu_dbg,"MsecInt=", tpb->timer_interval);
			DEBUG_DATA(lustat->lu_dbg,"TickInt=", tpb->timer_tick_int);
			DEBUG_DATA(lustat->lu_dbg,"TickOff=", tpb->timer_off_tick);

			if (tpb->timer_type == TIMER_ONE_SHOT) {
				DEBUG_DATA(lustat->lu_dbg,"OneShot", 0);
			} else {
				DEBUG_DATA(lustat->lu_dbg,"Cyclic", 0);
			}				

			if (tpb->timer_call_type == CALL_FUNCTION) {
				DEBUG_DATA(lustat->lu_dbg,"FuncCall", 0);
				DEBUG_DATA(lustat->lu_dbg,"Func2call", tpb->t_func);
				DEBUG_DATA(lustat->lu_dbg,"FuncPb=", tpb->t_pb);
			} else {
				DEBUG_DATA(lustat->lu_dbg,"Signal", 0);
				DEBUG_DATA(lustat->lu_dbg,"Prc2Wake=", tpb->t_pwake);
				DEBUG_DATA(lustat->lu_dbg,"Sig2Send=", tpb->t_sig);
			}

			return(err);
		}
		
		case ITE_TIMER_RESTART: {
			Timer_pb 	tpb = (Timer_pb)spb->param;

			DEBUG_DATA(lustat->lu_dbg,"TMR_RESTART", tpb->timer_id);

			return(timer_restart(deventry, tpb));
		}
		
		case ITE_TIMER_STOP: {
			Timer_pb 	tpb = (Timer_pb)spb->param;

			DEBUG_DATA(lustat->lu_dbg,"TIMER_STOP", tpb->timer_id);

			return(timer_stop(deventry, tpb));
		}
	}

	DEBUG_DATA(lustat->lu_dbg,"SS(unknown) ",spb->code);

	return(EOS_UNKSVC);
}

/****************************************************************************
**	Device Driver Downgoing-Data Entry point
*****************************************************************************/
error_code dr_downdata(Dev_list deventry, Mbuf mb)
{
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat),
				their_lustat;
	Spf_drstat	drstat = (Spf_drstat)(deventry->v_dr_stat);
	Dev_list	up_de, their_dev;
	Path_type	item_path = lustat->lu_itempath;
	Device_type	item_dev = &item_path->conn_info;
	Mbuf		mbq = mb;
	u_int16		sendto_lun;
	Spf_pdstat	their_pd;
	

	#ifdef DEBUG
		if (mb) {
			DEBUG_DATA(lustat->lu_dbg, "LP  DwnDat", mb);
		} else {
			DEBUG_DATA(lustat->lu_dbg, "LP  EmpDat",0);
		}
	#endif

	/* The goal here is to get the device entry of the driver intended for
	 * this packet. For connection oriented, it's easy.
	 * But for connectionless networks, we'll have to get the address and
	 * use it to index into the call list to see if the far end is there,
	 * then send the data to it (making sure that the far end is also
	 * connectionless).
	 */
	if (lustat->lu_num < LU_FIRST_CONNLESS) {	/* Connection oriented	*/

		if ((lustat->lu_felustat) == NULL) {
			DEBUG_DATA(lustat->lu_dbg,"NoFarEnd   ", lustat);

			return_mbuf(mb);
			return(EOS_NOTRDY);
		}

		up_de = lustat->lu_felustat->lu_updrvr;
		their_pd = lustat->lu_felustat->lu_pathdesc;
		their_dev = deventry;

		#ifdef DEBUG
			their_lustat = lustat->lu_felustat;
		#endif

	} else if (lustat->lu_num >= MAX_CALL_ENDPTS) {	/* Loopback device	*/

		/* Find the up driver based on the path descriptor */
		if ((up_de = find_updrvr(drstat, lustat->lu_pathdesc)) == NULL)
			return(EOS_WRITE);
		their_pd = lustat->lu_pathdesc;
		their_lustat = lustat;
		their_dev = deventry;

	} else {										/* Connectionless	*/

		/* If our connectionless path is only using loopback, use the
		 * ITEM addressing. Otherwise, the ITEM addressing is being used by
		 * protocols above, so use the logical unit value we tucked
		 * away at dr_iniz time.
		 *
		 * A better way of doing this would be at PUSH time,
		 * save away the last known sendto address in the lu_sendto_lun
		 * field. Then, the caller could actually set the sendto address
		 * before pushing the protocols under test.
		 */
		if (item_dev->dev_ournum.addr_class == ITE_ADCL_LPBK) {
			DEBUG_DATA(lustat->lu_dbg, "LP  LPBK  ", 0);

			sendto_lun = (u_int16)item_dev->dev_theirnum.addr[0];
		} else {
			sendto_lun = lustat->lu_sendto_lun;
		}

		DEBUG_DATA(lustat->lu_dbg, "LP  FELun ", sendto_lun);
		
		if ((their_lustat = drstat->dr_call_list[sendto_lun]) == NULL) {
			DEBUG_DATA(lustat->lu_dbg,"FENotInList", drstat);

			return_mbuf(mb);
			return(EOS_NOTRDY);
		}

		up_de = their_lustat->lu_updrvr;

		if ((their_pd = drstat->dr_path_list[sendto_lun]) == NULL) {
			DEBUG_DATA(lustat->lu_dbg, "ERLPPdNotF", 0);
		}

		if ((their_dev = drstat->dr_dev_list[sendto_lun]) == NULL) {
			DEBUG_DATA(lustat->lu_dbg, "ERLPDvNtFd", 0);
		}
		
		DEBUG_DATA(lustat->lu_dbg, "LP  FEUpDr", up_de);
		DEBUG_DATA(lustat->lu_dbg, "LP  FELust", their_lustat);
		DEBUG_DATA(lustat->lu_dbg, "LP  FEDrvr", their_dev);
	}


	/* Notice the following code segment gets a new mbuf, copies the
	 * old mbuf into the new mbuf and gets rid of the old mbuf. This is
	 * to better emulate the maximum transfer rate if the network was faster
	 * than the processor could feed it and we had to feed the transmit
	 * FIFO byte-by-byte.
	 *
	 * We also use the DR_FMCALLUP_PKT call to emulate the entire path
	 * through which the buffers must travel (up SPF's receive thread).
	 */


	/* Send the mbuf to the rx thread... */
	while ((mb = m_deq(&mbq)) != NULL) {

		Mbuf newmb;
		Mbuf mbp;
		error_code err;
		u_int32 size = m_len_p(mb), *ui32ptr;
		u_char * New_data;

		mb->m_qnext = NULL;

		/* get a "receive" mbuf */
		if ((err = get_mbuf(deventry, size + 8, &newmb)) != SUCCESS) {
			DEBUG_DATA(lustat->lu_dbg, "CantGetMbuf", size);

			return_mbuf(mb);
			return(err);
		}

		newmb->m_size = 0;
		newmb->m_offset += 8;
		New_data = DATA_PTR(newmb);

		/* copy entire packet chain into new mbuf */
		for (mbp=mb; mbp!=NULL; mbp=mbp->m_pnext) {

			/* copy data from "transmit" mbuf to "receive" mbuf */
			memcpy(New_data,DATA_PTR(mbp),mbp->m_size);
			newmb->m_size += mbp->m_size;
			New_data += mbp->m_size;
		}

		/* free up the "transmitted" mbuf */
		return_mbuf(mb);

		#if defined(DEBUG)
		{
			u_int32* tmp = (u_int32*)DATA_PTR(newmb);
			debug_4data(their_lustat->lu_dbg,0x55503a20,tmp[0],tmp[1],tmp[2]);
		}
		#endif

		/* Using the unused area after the mbuf header, store the
		 * desired updriver device entry and path descriptor pointer.
		 * This will allow us to route the packet correctly when it
		 * comes through our updata entry point... [jmh]
		 */
		ui32ptr = (u_int32 *)((u_char *) newmb + sizeof(mbuf));
		ui32ptr++;
		*(ui32ptr++) = (u_int32) up_de;
		*ui32ptr = (u_int32) their_pd;

		DEBUG_DATA(lustat->lu_dbg, "LP  DNDE", deventry);
		DEBUG_DATA(lustat->lu_dbg, "LP  DNPD", their_pd);
		DEBUG_DATA(lustat->lu_dbg, "LP  Mbuf", newmb);

		/* give it to the receive thread to process to give back to ourself */
		if ((err = DR_FMCALLUP_PKT(deventry, their_dev, newmb)) != SUCCESS) {
			DEBUG_DATA(lustat->lu_dbg,"FMCallupErr", 0);

			return_mbuf(newmb);
			return(err);
		}
		DEBUG_DATA(lustat->lu_dbg, "LPFMClupOK", newmb);
	}

	return(SUCCESS);
}

/*****************************************************************************
 *	Device Driver Upcoming-Data Entry point
******************************************************************************/
error_code dr_updata(Dev_list deventry, Mbuf mb)
{
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat);
	Dev_list	up_de;
	u_int32		*ui32ptr;


	#ifdef DEBUG
		if (mb) {
			DEBUG_DATA(lustat->lu_dbg, "LP  UPDATA", mb);
		} else {
			DEBUG_DATA(lustat->lu_dbg, "empty UPDATA",0);
		}
	#endif

	/* Since all incoming packets come from ourselves (sent to us via
	 * our downdata entry point), find the desired updriver and path
	 * descriptor we saved within the mbuf in downdata... [jmh]
	 */
	ui32ptr = (u_int32 *)((u_char *) mb + sizeof(mbuf));
	ui32ptr++;
	up_de = (Dev_list) *(ui32ptr++);
	lustat->lu_pathdesc = (Spf_pdstat) *ui32ptr;

	DEBUG_DATA(lustat->lu_dbg, "LP  UPPD  ", lustat->lu_pathdesc);
	DEBUG_DATA(lustat->lu_dbg, "LP  UPDE  ", up_de);

	/* Upsy-daisy... */
	return(SMCALL_UPDATA(deventry, up_de, mb));
}

