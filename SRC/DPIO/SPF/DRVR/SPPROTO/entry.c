/****************************************************************************
 *  SPF Device Driver entry points.                                         *
 ****************************************************************************
 *  Copyright 1995 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *   1 01/07/95 Created from SPF v1.0.2                                bat
 *   2 03/13/95 Ported from SMloop                                     bat
 *   3 03/29/95 Reorganization of logical unit & driver static         bat
 *     06/30/85 <------------- SPF 2.0 pre-release ------------->      cas
 *   4 10/09/95 Added x86 binary support                               dwj
 *   5 10/09/95 Changed SPF_SS_DEVENTRY to SPF_GS_DEVENTRY             dwj
 *     10/13/95 <------------------- SPF 2.0 ------------------->      dwj
 * 205 02/18/97 Ported to x86                                          taka
 * 206 03/13/97 Add per path storage                                   taka
 * 206 04/17/97 <---------------- SPF v2.2 (beta)--------------->      taka
 * 207 05/01/97 Check if there is a donwdriver for dr_downdata         cas
 *     06/26/97 <---------------- SPF V2.2 --------------------->      taka
 * 208 10/23/97 Drop the packets if the per-path storage is not found
 *              at dr_downdata                                         taka
 * 209 10/29/97 Add attach and use count information for DEBUG         taka
 * 211 07/19/98 <---------------- SPF v2.2.2 ------------------->      taka*
 ****************************************************************************/

/*
**	Header Files
*/

#include <SPPROTO/defs.h>

/*
** Device Driver Initialization Entry point. Any special considerations
** or initialization that the protocol needs to make to the driver static
** or logical unit static areas should be done here. Nothing path-specific:
** That's done in the setstat entry point at the SS_OPEN setstat.
**
** The SPF File manager will call the dr_iniz entry point of the driver
** only when the logical unit attach count is 1 (i.e. the 1st attach
** to a particular logical unit).                         ^^^^^^^^^^
*/
error_code dr_iniz(Dev_list deventry)
{
	Spf_lustat lustat = (Spf_lustat)(deventry->v_lu_stat);

	/* If the protocol state machine has any messages to be sent back and
	 * forth between the 2 endpoints, the I/O enabled flag should remain
	 * disabled until the end to end protocol has been successfully initialized.
	 *
	 * In this example, we set the flag immediately.
	 */
	lustat->lu_ioenabled = DRVR_IOEN;						/* enable I/O */

	/* This piece of code creates a debug data module that you can link to
	 * and look at when in rombug to aid you in troubleshooting your driver.
	 * If you'd like debugging on, you define the DEBUG macro in the
	 * spfdrvr.mak (or spfdbg.mak which has already been modified) makefile
	 * and include debug_mod.l which contains the calls that you'll see
	 * throughout this driver source.
	 *
	 * Convention has it that when making the debug version, the object
	 * is stored in the CMDS/BOOTOBJS/SPF/MON directory and when debugging
	 * is off, the object is stored in the CMDS/BOOTOBJS/SPF directory.
	 */
#if defined(DEBUG)
	if (debug_init(DEBUG,(Dbg_stat*)&lustat->lu_dbg,lustat->lu_dbg_name)
																!= SUCCESS) {
		lustat->lu_dbg = NULL;
	}

	debug_data(lustat->lu_dbg,"PRIniz     ", (u_int32)deventry);
#endif

	return(SUCCESS);
}

/*
** Device Driver Termination Entry point. This entry point allows the driver
** to clean up before the operating system gets rid of this particular
** logical unit.
**
** This entry point is called by the SPF file manager only on the last detach
** from a particular logical unit.                                ^^^^^^^^^^^
*/
error_code dr_term(Dev_list deventry)
{
#ifdef DEBUG
	Spf_drstat drstat = (Spf_drstat)(deventry->v_dr_stat);
#endif
	Spf_lustat lustat = (Spf_lustat)(deventry->v_lu_stat);

	/* Again, I/O is disabled on a protocol when the end-end protocol
	 * terminates. Depending on the protocol, this may be on terminate,
	 * or closing of the last path on a particular logical unit.
	 */
	lustat->lu_ioenabled = DRVR_IODIS;						/* disable I/O */

	DEBUG_DATA(lustat->lu_dbg, "PRTerm   ", deventry);
	DEBUG_4DATA(lustat->lu_dbg, drstat->dr_att_cnt, drstat->dr_use_cnt,
								lustat->lu_att_cnt, lustat->lu_use_cnt);

	return(SUCCESS);
}

/*
** Device Driver Get-Stat Entry point. This entry point should handle
** any protocol specific getstats.
**
** The SPF file manager calls this entry point whenever an unknown getstat
** code is received. Likewise, if you're protocol driver doesn't understand
** the getstat, pass it down to the next lower driver in the stack.
*/
error_code dr_getstat(Dev_list deventry, Spf_ss_pb pb)
{
#ifdef DEBUG
	Spf_drstat drstat = (Spf_drstat)(deventry->v_dr_stat);
#endif
	Spf_lustat lustat = (Spf_lustat)(deventry->v_lu_stat);
	error_code err;
	
	DEBUG_DATA(lustat->lu_dbg, "PR:GetStat", deventry);
	DEBUG_4DATA(lustat->lu_dbg, drstat->dr_att_cnt, drstat->dr_use_cnt,
								lustat->lu_att_cnt, lustat->lu_use_cnt);

	switch (pb->code) {

        case SPF_GS_PROTID: {
            pb->param = (void *)SPF_PR_SPPROTO;
            return(SUCCESS);
        }

		case SPF_GS_UPDATE:	{	/* Attempting to get update statistics	*/

			Spf_update_pb	upb = (Spf_update_pb)pb;
 			Spf_ppstat ppentry;

			DEBUG_DATA(lustat->lu_dbg, "PRUpdate  ", lustat->lu_pathdesc);

			if ((err = pps_find_entry(lustat, lustat->lu_pathdesc, &ppentry))
																!= SUCCESS) {
				return err;
			}
			
			if (ppentry->pp_dndrvr) {

                if ((err = SMCALL_GS(deventry, ppentry->pp_dndrvr, pb))
	                											!= SUCCESS) {
	                return(err);
				}

				DEBUG_DATA(lustat->lu_dbg, "PRPpStat", ppentry);
				/* Now Update per my protocol status */

				/* We have no fragmenting ability, but we also allow
				 * as big a packet as can be sent. So we won't make
				 * any changes to upb->stk_txsize that came from below.
				 */
				if (upb->stk_txsize > lustat->lu_txsize) {
					upb->stk_txsize = lustat->lu_txsize;
				}

				/* If we need, We keep the header and trailer sizes below	
				 *
				 * ppentry->pp_stk_txoffset  = upb->stk_txoffset;
				 * ppentry->pp_stk_txtrailer = upb->stk_txtrailer;
				 */
				
				/* If there are bytes that need to be allowed for the
				 * header, we would add them to what came from below:
				 *
				 * upb->stk_txoffset = upb->stk_txoffset + lustat->lu_txoffset;
				 */

				/* If this is a hold on close protocol
				 * add the hold on close parameter
				 *
				 * upb->stk_hold_on_close += 1;
				 */
				 
				/* We either send up IO DISABLED if we are, or if we are
				 * enabled, send up whatever the lower one put in there.
				 */
				if (lustat->lu_ioenabled == DRVR_IODIS)
					upb->stk_ioenabled = DRVR_IODIS;

			} else {	/* We're the lowest driver on the stack */

				/* If no one's below us, copy our stats into the
				 * parameter block and return.
				 */
				upb->stk_txsize = lustat->lu_txsize;
				upb->stk_txoffset = lustat->lu_txoffset;
				upb->stk_ioenabled = lustat->lu_ioenabled;

				/* If this is a hold on close protocol
				 * add the hold on close parameter
				 *
				 * upb->stk_hold_on_close = 1;
				 */
				 
			}

			return(SUCCESS);

		} /* End SPF_SS_UPDATE */

		default: {

			Spf_ppstat ppentry;

			DEBUG_DATA(lustat->lu_dbg, "PRGsUnknwn", pb->code);

			if ((err = pps_find_entry(lustat, lustat->lu_pathdesc, &ppentry))
																!= SUCCESS) {
				return err;
			}

			DEBUG_DATA(lustat->lu_dbg, "PRPpStat", ppentry);			
			if (pb->updir == SPB_GOINGDWN) {
				if (ppentry->pp_dndrvr) {
					return(SMCALL_GS(deventry, ppentry->pp_dndrvr, pb));
				}
			} else {
				return(SMCALL_GS(deventry, ppentry->pp_updrvr, pb));
			}
		} /* End default */

	} /* End switch */

	return(EOS_UNKSVC);		/* If we get this far, return unknown service	*/
}

/*
** Device Driver Set-Stat Entry point. This entry point should handle any
** protocol specific setstats and any ITEM specific setstats that this
** protocol is responsible for.
**
** The SPF file manager calls this entry point whenever an unknown setstat
** code is received and when there are setstats that the protocol is
** responsible for in part. The setstat codes to be aware of that you might
** have to handle are: SPF_SS_FLOWOFF, SPF_SS_FLOWON, SS_LUOPT. See the SPF
** Internals and Processing section for more details on what is expected of
** the drivers for these setstats.
**
** Other setstat codes not included below, but if you're protocol deals
** with these expected codes, put them in:
**
**	ITE_DIAL:	An ite_ctl_connect() call has been made by the application.
**				The driver is expected to initiate a far end connection
**				using the address_type information in the device_type
**				structure. The device_type structure is found in the
**				path descriptor, pd_item structure. Once the connection
**				confirmation is received by the protocol, they should
**				send a notification to the process that initiated the
**				connection to signify that the connection has been established.
**
**	ITE_ANSWER:	The user has been notified of an incoming call and wishes to
**				answer it. The driver is expected to communicate with the
**				network to create an active connection. Once the protocol
**				determines that the connection is active, it will notify
**				the process that made the ITE_ANSWER call to inform it of
**				the exstablished connection.
**
**	ITE_HANGUP:	The connection is trying to be hung up. The protocol should
**				initiate call hangup procedures and return.
**
**	ITE_FEHANGUP_ASGN:	This allows the caller to be notified when the far
**						end hangs up. The file manager takes care of inserting
**						the notification request into the notification array.
**						The driver gets called to allow it to set any driver
**						specific flags to keep track of this registration.
**						When the driver gets notification of
**						far end hangup, it should use the notification array
**						element for far end hangup to send notification
**						to the process registered to receive it.
**	ITE_FEHANGUP_RMV:	The file manager takes care of taking the caller
**						out of the notification array, but also allows the
**						driver a chance to clear out any driver specific
**						information about the notification assignment.
**	ITE_RCVR_ASGN:		This allows the caller to be notified when on an
**						incoming call. The file manager takes care of inserting
**						the notification request into the notification array.
**						The driver gets called to allow it to set any driver
**						specific flags to keep track of this registration.
**						When the driver gets receives and incoming call,
**						it should use the notification array element for
**						INCALL to send notification	to the process registered
**						to receive it.
**	ITE_RCVR_RMV:		The file manager takes care of taking the caller
**						out of the notification array, but also allows the
**						driver a chance to clear out any driver specific
**						information about the notification assignment.
**	ITE_LINKDWN_ASGN:	This allows the caller to be notified when the end-to-
**						end link goes down. The file manager takes care of
**						inserting the notification request into the
**						notification array. The driver gets called to allow it
**						to set any driver specific flags to keep track of this
**						registration. TYpically, when the protocol driver is
**						idle for some timeout period, or a hardware driver
**						gets an interrupt that the line is broken, the driver
**						should use the notification array element for LINKDWN
**						to send notification to the process registered
**						to receive it.
**	ITE_LINKDWN_RMV:	The file manager takes care of taking the caller
**						out of the notification array, but also allows the
**						driver a chance to clear out any driver specific
**						information about the notification assignment.
**	ITE_ENDPGM_ASGN:	This allows the caller to be notified when the end of
**						MPEG-2 program is noticed by the MPEG-2 netowkr device.
**						The file manager takes care of inserting the
**						notification request into the notification array. The
**						driver gets called to allow it to set any driver
**						specific flags to keep track of this registration.
**						The driver should use the notification array element
**						for ENDPGM to send notification to the process
**						registered to receive it.
**	ITE_ENDPGM_RMV:		The file manager takes care of taking the caller
**						out of the notification array, but also allows the
**						driver a chance to clear out any driver specific
**						information about the notification assignment.
**
**	NOTE: Note that for connectionless protocols, the convention is that any
**	call control setstats will cause connectionless protocols to return
**	an EOS_UNKSVC to the caller.
**
** ITEM call setstats are: ITE_LINKDWN_ASGN, ITE_RCVR_ASGN, ITE_ENDPGM_ASGN,
** ITE_FEHANGUP_ASGN, and their respective _REM calls. See the section
** on ITEM for specifics on what SPF does with these setstats and what is
** expected of the drivers on these setstats.
*/
error_code dr_setstat(Dev_list deventry, Spf_ss_pb pb)
{
#ifdef DEBUG
	Spf_drstat drstat = (Spf_drstat)(deventry->v_dr_stat);
#endif
	Spf_lustat lustat = (Spf_lustat)(deventry->v_lu_stat);

	DEBUG_DATA(lustat->lu_dbg, "PR:SetStat", deventry);
	DEBUG_4DATA(lustat->lu_dbg, drstat->dr_att_cnt, drstat->dr_use_cnt,
								lustat->lu_att_cnt, lustat->lu_use_cnt);

	switch(pb->code) {

		case SPF_SS_OPEN: {		/* hook to new upper/lower protocols */

			Spf_ppstat		ppentry;
			error_code		err;

			DEBUG_DATA(lustat->lu_dbg, "PROpen   ", lustat->lu_pathdesc);

			lustat->lu_pps_size = sizeof(spf_ppstat);
			DEBUG_DATA(lustat->lu_dbg, "PR  PPSIZE", sizeof(spf_ppstat));

			if ((err = pps_add_entry(deventry, &ppentry)) != SUCCESS) {
				DEBUG_DATA(lustat->lu_dbg, "ERPRAddPpS", 0);

				return err;
			}

			DEBUG_DATA(lustat->lu_dbg, "PRAddPpSt ", ppentry);

			return SUCCESS;
		}

#if 0
		/* If this is a hold on close protocol */
        case SPF_SS_APPCLOSE: {
			Spf_ppstat		ppentry;
			error_code		err;

			DEBUG_DATA(lustat->lu_dbg, "PRAppClose", lustat->lu_pathdesc);

			if ((err = pps_find_entry(lustat, lustat->lu_pathdesc, &ppentry))
																!= SUCCESS) {
				return err;
			}
			
			DEBUG_DATA(lustat->lu_dbg, "PRPpStat", ppentry);

			DR_FMCALLUP_CLOSE(deventry, (Spf_pdstat)(ppentry->pd));
							
        	return SUCCESS;
        }
#endif
				 
		case SPF_SS_CLOSE: {		/* unhook from upper/lower protocols */

			DEBUG_DATA(lustat->lu_dbg, "PRClose  ", lustat->lu_pathdesc);

			return (pps_del_entry(lustat, lustat->lu_pathdesc));
		}

		case SPF_SS_PUSH: {		/* Push protocol above this driver */

			Spf_ppstat		ppentry;
			error_code		err;
			
			DEBUG_DATA(lustat->lu_dbg, "PRPush   ", lustat->lu_pathdesc);

			if ((err = pps_find_entry(lustat, lustat->lu_pathdesc, &ppentry))
																!= SUCCESS) {
				return err;
			}

			DEBUG_DATA(lustat->lu_dbg, "PRPpStat", ppentry);

            ppentry->pp_updrvr = lustat->lu_updrvr;

			return(SUCCESS);
		}

		case SPF_SS_POP: {		/* Pop the protocol above this driver	*/

			Spf_ppstat		ppentry;
			error_code		err;

			DEBUG_DATA(lustat->lu_dbg, "PRPop    ", lustat->lu_pathdesc);

			if ((err = pps_find_entry(lustat, lustat->lu_pathdesc, &ppentry))
																!= SUCCESS) {
				return err;
			}
			
			DEBUG_DATA(lustat->lu_dbg, "PRPpStat", ppentry);

            ppentry->pp_updrvr = lustat->lu_updrvr;

			return(SUCCESS);
		}

		default: { /* Send unknown code down if there's a driver below */

			Spf_ppstat		ppentry;
			error_code		err;
			
			DEBUG_DATA(lustat->lu_dbg, "PRSsUnknwn", pb->code);

			if ((err = pps_find_entry(lustat, lustat->lu_pathdesc, &ppentry))
																!= SUCCESS) {
				return err;
			}

			DEBUG_DATA(lustat->lu_dbg, "PRPpStat", ppentry);
			
			if (pb->updir == SPB_GOINGDWN) {
				if (ppentry->pp_dndrvr) {
					return(SMCALL_SS(deventry, ppentry->pp_dndrvr, pb));
				}
			} else {
				return(SMCALL_SS(deventry, ppentry->pp_updrvr, pb));
			}
		} /* End default */
	}

	return(EOS_UNKSVC);
}

/*
** Device Driver Downgoing-Data Entry point. This is the place where
** data being transmitted through the protocol is encapsulated with any
** headers used by the protocol, then sent down to the next lower driver
** in the stack. Note that the lu_txoffset field tells the SPF file manager
** how many bytes to leave at the front of the packet for the protocol header.
** Therefore, you can back up the pointer in the mbuf to add on the header
** information, saving excessive copies.
**
** The SPF file manager calls this whenever transmit (write) data is
** sent from the application. (or higher layer drivers).
*/
error_code dr_downdata(Dev_list deventry, Mbuf mb)
{
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat);
	Spf_ppstat	ppentry;
	error_code	err;

	if (mb) {
		DEBUG_DATA(lustat->lu_dbg, "PRDnMbData", mb);
		DEBUG_MBUF_DOWN(lustat->lu_dbg, mb);
	} else {
		DEBUG_DATA(lustat->lu_dbg, "PRDnMbEpty",0);
	}

	if ((err = pps_find_entry(lustat, lustat->lu_pathdesc, &ppentry))
																!= SUCCESS) {
		m_free_p(mb);           /* sorry, drop the packet */
		return err;
	}
	
	DEBUG_DATA(lustat->lu_dbg, "PRPpStat", ppentry);

	if (ppentry->pp_dndrvr != NULL) {
		/* just send the data down to lower protocols */
		return(SMCALL_DNDATA(deventry, ppentry->pp_dndrvr, mb));
	} else {
		m_free_p(mb);
		return(EOS_NODNDRVR);
	}
}

/*
** Device Driver Upcoming-Data Entry point. This is where incoming data
** destined for one of the paths using this protocol comes in. It is
** important to understand how the data gets from the receive interrupt
** service routine of the driver through the stack, and up to the
** file manager. For details about this, read the section about data
** reception through the SPF file manager in the Telecommunications manual.
**
** One important note:: Receive interrupt service routines should use
** the DR_FMCALLUP_PKT() macro to send the data up to higher protocol
** drivers. This way the packet stops being processed in interrupt service
** routine context and is processed on SPF's built in receive thread
** process. Higher layer protocol drivers should use the SMCALL_UPDATA
** macro to send data up the protocol stack.
*/
error_code dr_updata(Dev_list deventry, Mbuf mb)
{
	Spf_lustat 	lustat = (Spf_lustat)(deventry->v_lu_stat);
	Spf_ppstat	ppentry;
	u_int32		*addr;
	error_code	err;

	if (mb) {
		DEBUG_DATA(lustat->lu_dbg, "PRUpMbData", mb);
		DEBUG_MBUF_UP(lustat->lu_dbg, mb);
	} else {
		DEBUG_DATA(lustat->lu_dbg, "PRUpMbEpty", mb);
	}

	if (lustat->lu_pathdesc) {

		/* CAUTION!!!
		 * Since SPPROTO runs on top of SPLOOP AND SPLOOP sets its
		 * up driver's path descriptor, we find the per path storage
		 * by path descriptor. In most cases this is NOT True. Use
		 * pps_find_entry_by_offset() to find the per path storage.
		 */
	 
		if ((err = pps_find_entry(lustat, lustat->lu_pathdesc, &ppentry))
																!= SUCCESS) {
			m_free_p(mb);
			return err;
		}
	
		DEBUG_DATA(lustat->lu_dbg, "PRPpbyPd", ppentry);

		/* just send the data to upper protocols */
		return(SMCALL_UPDATA(deventry, ppentry->pp_updrvr, mb));
	}
	
	/* an address written in the buffer directs the path to go up */

	addr = mtod(mb, u_int32*);
	if ((err = pps_find_entry_by_offset(lustat, 24, 4, addr, &ppentry))
																!= SUCCESS) {
		m_free_p(mb);
		return err;
	}

	DEBUG_DATA(lustat->lu_dbg, "PRPpbyAd", ppentry);

	/* just send the data to upper protocols */
	return (SMCALL_UPDATA(deventry, ppentry->pp_updrvr, mb));

#if 0
	/* there is no per path storage necessary for this protocol */
	/* all packets go up to the lustat's updriver */
	/* just send the data to upper protocols */
	return(SMCALL_UPDATA(deventry, lustat->lu_updrvr, mb));
#endif
}
