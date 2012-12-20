/*
 * irq - interrupt routine for scsisiop low level SCSI driver.
 *       (NCRSIOP SCSI I/O Processor)
 */
 
/*--------------------------------------------------------------------------,
|                                                                           |
|     Copyright 1991, 1994, 1995-1998 by Microware Systems Corporation      |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, distribution, or creation of derivative works |
|  in any form to any party other than the licensee is strictly prohibited, |
|  unless expressly authorized in writing by Microware Systems Corporation. |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
| 01 96/08/03 Added edition history.                                  rak   |
|             ---- OS-9000/x86 V2.1 Released ----                           |
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
| 02 97/04/21 Added SCSI wide support.                                GbG   |
|             ---- OS-9000/PPC V2.1.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
| 03 98/01/29 Fixed compiler warnings.                                GbG   |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- SCSI Common Sub-Component v1.0 Released ----             |
| 04 00/03/06 Fixed problem with initial chdir from system process    GbG   |
|             $$                    <RELEASE_INFO>                     $$   |
`--------------------------------------------------------------------------*/
 
#include <scsisiop.h>

/************
**  This group of routines service the various script generated interrupts.
**  The number and nature of these interrupts is governed by the script
**  that is in use.  (see v167ll.ss  - mVme167 low level script)
**  The SIOP dstat register reflects status that activates these routines.
**   [See irq() below]
*************/
 
/*
 *	Function:
 *		scripti_msgi - Script generated message in interrupt (extended message)
 * 
 *  Synopsis:
 *		void scripti_msgi( Lstat lst, Lthread lth )
 *
 *  Description:
 *		Arrival at this routine indicates that the SCRIPT has identified
 *  that an extended message has arrived.  The only expected message
 *  in expected by this driver is currently the synchronous negotiation
 *  message.  The driver further assumes that we have initiated this
 *  dialog.  There are two possible directions that we would normally
 *  take.  These are:
 *
 *     1)  If we determine that we have not initiated the negotiation
 *         we MUST respond in kind, if we are allready running
 *		   sycghronous then we will allow the request, otherwise
 *		   we will send an asynchronus version of the message.  
 *
 *      2) If we have started the negotiation we call a routine that
 *         set our registers up to properly use the result of the
 *         negotiation.
 *
 *  Script Action:
 *		When we arrive here the last byte of the message has not been
 *  ACKed on the bus.  This allows us to determine our action without
 *  the target going off and changing phases on us.  The re-entry
 *  point in the script after this interrupt should:
 *     - if we did not initiate negotiation for synchronous transfer, we
 *       set ATN and wait for message out,  we then  send the message buffer.
 *     - if message to be accepted, clear ACK and then proceed to next
 *       script operation (wait for command phase)
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void scripti_msgi( Lstat lst, Lthread lth )
#else
void scripti_msgi( lst, lth )
Lstat lst;
Lthread lth;					/* current logical thread */
#endif
{

#if defined(WIDE_SUPPORT)

	if( lst->extmsgbuffer[IN_SYN_SYNM] == WIDE_REQUEST ) {
		check_widemsg( lst, lth );
		return;
	}

#endif

	if( lst->extmsgbuffer[IN_SYN_SYNM] != SYNC_REQUEST ) {
		/* not a supported message! */
		scsi_rejectmsg( lst, lth );
		return;
	} 

	/* check unexpected synchronus request */

	if( lth->devfield.devsync.sxfr_mask & 0xf ){
		/* unexpected but will service request */
		check_synchmsg( lst, lth );
		lth->chiperror = BUSY;	/* say BUSY so retry will happen */		
		return;
	}
	
	/* check expected synchronus request */

	if	(lth->synctried & NEGOTIATE)
	{
		check_synchmsg( lst, lth );
	}
	else
		ncr_async_msg( lst, lth );

}

/*
 *	Function:
 *		scripti_badmsg - Unidentified message in received
 * 
 *  Synopsis:
 *		void scripti_badmsg( Lstat lst, Lthread lth )
 *
 *  Description:
 *		Arrival here indicates that a message in phase was completed
 *  but the message found was not identified as one of the supported 
 *  messages.
 *
 */

#if defined (__STDC__) || defined (_ANSI_EXT)
void scripti_badmsg( Lstat lst, Lthread lth )
#else
void scripti_badmsg( lst, lth )
Lstat lst;
Lthread lth;
#endif
{
	lth->chiperror = BUSY;			/* say BUSY so retry will happen */		
	scsi_rejectmsg( lst, lth);		/* reject message */
}

/*
 *	Function:
 *		scripti_reject - Reject message received from target
 * 
 *  Synopsis:
 *		void scripti_reject( Lstat lst, Lthread lth )
 *
 *  Description:
 *		Arrival here indicates that a message in phase has occured with
 *  a message reject received.  The only time this should occur is durring
 *  negotiations for synchronus transfers.  If there is a negotiation in
 *  progress, we must revert to asynchronus operation.
 *
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void scripti_reject( Lstat lst, Lthread lth )
#else
void scripti_reject( lst, lth )
Lstat lst;
Lthread lth;
#endif
{
	if( lth->synctried & NEGOTIATE )
		ncr_clear_sxfr_mask( lth );					/* remove sync per/off */
	nthread( lst, lth );							/* pick up where left off */
}

/*
 *	Function:
 *		scripti_idisc - Interrmediate disconnect occured
 * 
 *  Synopsis:
 *		void scripti_idisc( Lstat lst, Lthread lth )
 *
 *  Description:
 *		Arrival at this routine indicates that the SCRIPT has
 *  identified a disconnect messages arrival.  If the script was
 *  in a data phase, there was an interrupt indicating phase change
 *  which preceeds this interrupt.  This being the case, the thread
 *  will have been marked logical and the resume point determined by
 *  the phase change interrupt.  We thus only reschedule the bus.
 *		If the device was not in a data phase when the message phase
 *  began, we arrive here with the thread marked as physical.  We then
 *  mark the resumption point as phase determination, and reschedule the
 *  bus.
 * 
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void scripti_idisc( Lstat lst, Lthread lth )
#else 
void scripti_idisc( lst, lth )
Lstat lst;
Lthread lth;
#endif
{
	/* if already marked logical, then was in data phase, address is set.
	 *  otherwise, it wasn't in a data phase so we mark it logical and
	 *  let the siop continue at idle.
	 */
	if( lth->threadstate != TH_LOGICAL ) {

		lth->threadstate = TH_LOGICAL;		
		act_scsi_idle( lst, lth );
	}
	reschedule(lst);
}

/*
 *	Function:
 *		scripti_done - command is complete
 * 
 *  Synopsis:
 *		void scripti_ok( Lstat lst, Lthread lth )
 *
 *  Description:
 *		Arrival at this routine indicates that a command has
 *  completed.
 *
 *  See Also:
 *     scripti_busfree()
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void scripti_done( Lstat lst, Lthread lth )
#else
void scripti_done( lst, lth )
Lstat lst;
Lthread lth;
#endif
{
	lth->threadstate = TH_DONE;						/* mark it complete */
	lst->wakequeue[lst->wakes++] = lth->processid;	/* queue wakeup */
	stop_sto();
	reschedule( lst );								/* restart SIOP */
}

/*
 *	Function:
 *		scripti_resel - reselect interrupt has been generated.
 * 
 *  Synopsis:
 *		void scripti_resel( Lstat lst, Lthread lth )
 *
 *  Description:
 *		Arrival at this routine indicates that the target has
 *  reselected us.  Determine which target that has reselected us and
 *  resume at the proper place.  If there is an error on the device
 *  and we are attempting to recover,  we may have gotten an unexpected
 *  disconnect and will have to try to restart it in the error recovery
 *  routine.
 *
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void scripti_resel( Lstat lst, Lthread lth )
#else 
void scripti_resel( lst, lth )
Lstat lst;
Lthread lth;
#endif
{
#if !defined(NCR53C8XX)
	u_int8 shifter;					/* gets the SCSI ID bit */
#endif
	int32 id;						/* accumulates to SCSI ID of reselector */

	if( ncr_ph_select(lst) ) {
		/*
		 * We were in the process of attempting to select a device and 
		 * we must cancel that operation.
		 */
		lth->threadstate = TH_PENDING;		/* .. have to wait a bit! 	*/
		lst->selflg.sipflag[0] = FALSE;		/* cancel select in progress */
		stop_sto();							/* cancel timer */
	} else {

		/* were at IDLE, mark IDLE as logical */
		lth->threadstate = TH_LOGICAL;
	}

	/* now find out who it was ...
	 *  The ID of the reselector should contain both our ID and his,
	 *  we EXOR ours out and shift to locate the targets.
	 */
#if !defined(NCR53C8XX)
	shifter = lst->lcrcs.lcrcid.tid ^ (1 << lst->selfid);
	id = 0;							/* ls bit is ID 0 */
	while( !(shifter & 0x1) ) {
			shifter >>= 1;				/* slide down one */
			id++;
	}
#else
	id = lst->lcrcs.lcrcid.tid;
#endif

	lth = &lst->logthread[id];				/* pointer to current */

	if( lth == &lst->logthread[lst->selfid] ) {
		reschedule( lst );	
	} else {
		nthread( lst, lth );		
	}

}	/* end scripti_resel() */

/*
 *	Function:
 *		scripti_sigp - the SCSI bus is now free for use.
 * 
 *  Synopsis:
 *		void scripti_sigp( Lstat lst )
 *
 *  Description:
 *		Arrival at this routine indicates that a command has
 *  completed and the bus SCRIPT may be rethreaded if there is a
 *  pending thread.  If no threads are pending the SIOP will just
 *  be started up at the "wait" point.
 *
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void scripti_sigp( Lstat lst )
#else
void scripti_sigp( lst )
Lstat lst;
#endif
{

	reschedule( lst );		/* start the next in line */
}


/************
**  This group of functions services interrupts that are a result of 
**  the SIOP DMA discovering a problem.  [See irq() below]
*************/

/*
 *	Function:
 *		dma_buserror - SIOP DMA detected a bus error (LTO possible)
 * 
 *  Synopsis:
 *		void dma_buserror( Lstat lst )
 *
 *  Description:
 *		There are two distinct possibilities as to what has cause this
 *  service routine to be activated.  These are:
 *
 *      1)  The DMA received an LTO (local bus timeout) generated bus
 *          error.  If this is the cause we:
 *                save_state()  to get the residual data count and
 *                script restart address from the SIOP.  We then
 *                rethread and restart the current physical thread.
 *
 *       2) The DMA received a hard bus error.  If this is the case
 *          the script is restarted and an abort is sent to the target.
 *          We also set E_BUSERR in chip status and wake up the 
 *          process that sent in the bad pointer.
 *
 *  See Also:
 *      buscondition()   [dev167.c]
 *
 *	Caveats:
 *		This routine is an indication of a serious state of affairs.
 *  If the interrupt occurs there are several conditions that must be
 *  checked.  These are:
 *
 *		-  Check for LTO (local bus timeout).  If this is the cause
 *         the command can be reserected and all will proceed as normal.
 *         [side effect:  It is not clear at this point if the system 
 *         can complete a sysnchronus data transfer properly in the 
 *         face of LTO.  The data may be damaged or the count may be
 *         damaged.]
 *      -  If the bus error was a result of access to another bus (VME,
 *         VSB, ect.) then unless the status of that bus can be detected
 *         with a high degree of confidence there is no way of insureing
 *         that the command can be completed.  Moral of the story - 
 *         confine the NCR chip to the local bus where status may be 
 *         detectable.
 *      -  If the bus error is a result of a local parity error then there
 *         seems to be little that can be done other than sending an abort
 *         to the target as the data being sent is corrupted.
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void dma_buserror( Lstat lst )
#else
void dma_buserror( lst )
Lstat lst;
#endif
{
	Lthread lth;
	
	lth = lst->phythread;		/* pointer to current physical thread */

	
	/*  There is (or was) a silicon bug that would cause the SIOP
	 *  to do the wrong thing when a cycle was retried.  We have little
	 *  choise but to blow the command away....... sorry
	 */
	 
	/* hard error or one that is perceived to be hard has occured.
	 *  we must send abort message to the target.  There is no
	 *  reasonable way to continue the command.
	 */
	bomblog( lst, lth, DMABE );
		
	if( ncr_ph_idata(lst) || ncr_ph_odata(lst) ) {
		save_state(lst);					/* need to preserve state here */
		lth->chiperror = BUSY;				/* say BUSY so retry will happen */
		/* determine phase and react accordingly */
		scsi_idle( lst, lth );

		} else {

		lth->chiperror = BUSY;				/* say BUSY so retry will happen */
		/* blast the fifo's */
		ncr_flush_fifo( lst );
		scsi_free( lst, lth );		
	}

}

/*
 *	Function:
 *		dma_watchdog - timeout accessing the local bus ???????
 * 
 *  Synopsis:
 *		void dma_watchdog( Lstat lst )
 *
 *  Description:
 *		<<< !!!! At present, this routine should never run!!!!! >>>
 */
#if defined (__STDC__) || defined (_ANSI_EXT) 
void dma_watchdog( Lstat lst )
#else
void dma_watchdog( lst )
Lstat lst;
#endif
{
	Lthread lth;
	
	lth = lst->phythread;
	bomblog( lst, lth, DMAWD );

	ncr_flush_fifo( lst );

#if defined (__STDC__)
	lth->chiperror = EOS_HARDWARE;
#else
	lth->chiperror = E_HARDWARE;	
#endif
	scsi_free( lst, lth );		
}

/*
 *	Function:
 *		dma_illscript - illegal script instruction detected
 * 
 *  Synopsis:
 *		void dma_illscript( Lstat lst )
 *
 *  Description:
 *		The SIOP has encountered an illegal SCSI SCRIPT instruction.
 *   This is a serious problem in that if these interrupts are received,
 *   either the fixup() routine has failed or we have a serious problem
 *   in the script itself.  This interrupt should never occur outside
 *   of the debugging process for the driver.
 *
 */
#if defined (__STDC__) || defined (_ANSI_EXT) 
void dma_illscript( Lstat lst )
#else 
void dma_illscript( lst )
Lstat lst;
#endif
{
	Lthread lth;

	/* assume worst case */

	lth = lst->phythread;
	bomblog( lst, lth, ILLIS );

	if( ncr_ph_idata(lst) || ncr_ph_odata(lst) ) {
		save_state(lst);				/* need to preserve state here */
		lth->chiperror = BUSY;				/* say BUSY so retry will happen */
		/* determine phase and react accordingly */
		scsi_idle( lst, lth );

	} else {

		/* blast the fifo's */
		ncr_flush_fifo( lst );
		lth->chiperror = BUSY;		/* say BUSY so retry will happen */
		scsi_free( lst, lth ); 

	}


}

/*************
 **   This group of functions service interrupts that are a function
 **   of the SIOP running into a problem from which it simply cannot
 **   act without assistance.  The status is determined by polling the
 **   SIOP state0 register.  [See irq() below]
 *************/

/*
 *	Function:
 *		siop_par - SIOP detected parity error
 * 
 *  Synopsis:
 *		void siop_par( Lstat lst )
 *
 *  Description:
 *		When SCSI_PARITY flag is set in the flags word passed to
 *  the low level driver, the parity checking bit is set in the 
 *  control word and the interrupt on parity error is enabled.
 *		If the SIOP detects a parity error on the incomming data
 *  the status is set in "stat0" and an interrupt is generated.
 *  	If the parity error occurs during the message in phase, we must
 *  send a message reporting it to the target who should then resend the 
 *  message.
 *		If the error occured durring a data transfer phase, we simply report
 *  the error to the process.
 *
 */
#if defined (__STDC__) || defined (_ANSI_EXT) 
void siop_par( Lstat lst )
#else
void siop_par( lst )
Lstat lst;
#endif
{
	Lthread lth;
	
	lth = lst->phythread;

	/* if in message phase, report to target */
	if( ncr_ph_mess(lst) ) {
		/* message phase parity error ( should return to message in phase) */
		scsi_pemsg( lst, lth );
	} else {

		/* data phase parity error */

		scsi_free( lst, lth );
	}
		lth->chiperror = BUSY;		/* say BUSY so retry will happen */
}

/*
 *	Function:
 *		siop_rrst - SCSI BUS RESET seen.
 * 
 *  Synopsis:
 *		void siop_rrst( Lstat lst )
 *
 *  Description:
 *		This routine services the SCSI Bus Reset.  This driver
 *  is considered to be in a single initiator environment, and 
 *  thus this code should never be run.  We have however provided
 *  that we may be able to recover in case all of our warnings prove
 *  to be of no avail.
 *		The processing logic and procedure in the case of this
 *  interrupt is:
 *      -  any physical thread is considered to be broken
 *         and the structure updated to reflect this by setting
 *         E_DIDC in the 'chiperror'.
 *      -  any logical thread is considered to be broken and
 *         it's structure is updated as above.
 *      -  any pending thread is allowed to proceed as though 
 *         nothing has happend.  This routine will rethread a pending
 *         thread.
 *      -  any processes hung at the semaphore for their SCSI ID's
 *         will never know that is has happened (until the device 
 *         return check condition that is....
 */
#if defined (__STDC__) || defined (_ANSI_EXT) 
void siop_rrst( Lstat lst )
#else
void siop_rrst( lst )
Lstat lst;
#endif
{
	int8 loop;						/* loop counter */
	int32 currentstate;				/* current thread state (during loop)   */
	Lthread lth;	 				/* logical thread pointer (for loop)    */
	Lthread  idle;
	
	loop = NTHREADS;				/* # of available threads */
	
	idle = &lst->logthread[lst->selfid];	/* idle thread */
	
	while( --loop >= 0 ) {
		/* the current physical and logical threads are broken here */
		if( (lth = &lst->logthread[loop]) == idle )
			continue;		/* skip idle thread,  not really active */
		currentstate = lth->threadstate;
		
		ncr_init_sxfr_mask( lth );						/* devices return .... */
		lth->synctried = FALSE;						/* ...to asynchronus. */
		
		if( (currentstate == TH_PHYSICAL) || (currentstate == TH_LOGICAL) ) {
			lth->threadstate = TH_DONE;				/* mark completed */

#if defined (__STDC__)
			if( lth->chiperror != EOS_HARDWARE )
				lth->chiperror = EOS_DIDC;			/* set error condition */
#else
			if( lth->chiperror != E_HARDWARE )
				lth->chiperror = E_DIDC;			/* set error condition */
#endif

			lst->wakequeue[lst->wakes++] = lth->processid;	/* queue wakeup */
		}
	}
	reschedule( lst );	/* restart SIOP */
}

/*
 *	Function:
 *		siop_udc - unexptected target disconnect 
 * 
 *  Synopsis:
 *		void siop_udc( Lstat lst )
 *
 *  Description:
 *		There are several cases that are managed in this routine.
 *  The general scenario is that the target has disconnected from the
 *  bus at a point within the processing of a command in which the
 *  SIOP has been taken by surprise.  We allow him to disconnect and
 *  restart at idle or the next available command.  When he reselects
 *  however, we will abort him.
 */

#if defined (__STDC__) || defined (_ANSI_EXT) 
void siop_udc( Lstat lst )
#else 
void siop_udc( lst )
Lstat lst;
#endif
{
	Lthread  lth;						/* logical thread pointer */

	lth = lst->phythread;				/* current physical/logical  thread */

	/* blast the fifo's */
	ncr_flush_fifo( lst );
	lth->chiperror = BUSY;              /* say BUSY so retry will happen */
	lth->threadstate = TH_DONE;			/* mark completed */
	lst->wakequeue[lst->wakes++] = lth->processid;	/* queue wakeup */
	reschedule( lst );					/* restart SIOP */

}

/*
 *	Function:
 *		siop_sge - SCSI Gross Error 
 * 
 *  Synopsis:
 *		void siop_sge( Lstat lst )
 *
 *  Description:
 *		This service is a result of a serious failure.
 *  The chip and the target are in an indeterminant state, send
 *  SCSI BUS RESET.
 *
 *		If this occurs in the high level driver, then then SIOP has
 *  BUS RESET asserted and the tick interrupt will deassert this condition.
 *  This is the only case that the SIOP is not restarted prior to leaving
 *  the interrupt service routine.
 */
#if defined (__STDC__) || defined (_ANSI_EXT) 
void siop_sge( Lstat lst )
#else
void siop_sge( lst )
Lstat lst;
#endif
{
	Lthread lth;

	lth = lst->phythread;

#if defined (__STDC__)
	lth->chiperror = EOS_HARDWARE;		/* mark the culperit */
#else
	lth->chiperror = E_HARDWARE;		/* mark the culperit */
#endif

	bomblog(lst, lth, SGE );			/* log the failure */
	
#ifdef CBOOT
	SCSI_Reset(lst);			/* send a reset to the SCSI Bus */
	chipreset(lst);				/* sets the chip up properly */
	siop_rrst(lst);				/* reset our data structures */
	reschedule(lst);
#else
	lst->chipbusy = DEASERTRST;	/* mark reset in progress */
	ncr_scsi_reset( lst );	 	/* start SCSI Bus Reset */	
	_timer( ONE_TICK );			/* start timer (which will complete reset!) */
#endif	

}

/*
 *	Function:
 *		siop_sto - SCSI timeout occurred
 * 
 *  Synopsis:
 *		void siop_sto( Lstat lst )
 *
 *  Description:
 *		The SIOP has an internal timmer that will fire if 
 *  the device is connected and no activity occurs within 250ms.
 *  This timer can be used to determine access to non-existant 
 *  devices and indeed loss of continuity in the SCSI bus while
 *  connected.
 *
 *  See Also:
 *		scripti_busfree()
 *
 *  Script Issues:
 *		The script will enable and disable this bit around selection.
 *
 *  SLOW_SCSI Notes:  The on-chip timer will not be enabled, the reason
 *      being that there are a number of drives that do not respond
 *      in all cases within the 250 msec range of the timer, further,
 *      these devices will then cause the driver major problems.  The
 *      long and short of it is that you should never see this interrupt.
 */
#if defined (__STDC__) || defined (_ANSI_EXT) 
void siop_sto( Lstat lst )
#else
void siop_sto( lst )
Lstat lst;
#endif
{
	Lthread lth;

    lst->selflg.sipflag[0] = FALSE;     /* selection phase false */

	lth = lst->phythread;				/* get pointer to logical thread */
	stop_sto();							/* insure that software timer disabled */
	
	/* must be a select timeout */

#if defined (__STDC__)
	lth->chiperror = EOS_UNIT;			/* SCSI ID not present */
#else
	lth->chiperror = E_UNIT;			/* SCSI ID not present */
#endif

	lth->threadstate = TH_DONE;			/* mark it done */
	lst->wakequeue[lst->wakes++] = lth->processid;	/* queue wakeup */
	reschedule( lst );					/* restart SIOP */
}

/*
 *	Function:
 *		siop_pm - SIOP detected phase mismatch
 * 
 *  Synopsis:
 *		void siop_pm( Lstat lst )
 *
 *  Description:
 *		A phase mismatch has been detected and as such we
 *  will try to determine the cause.  There is a routine
 *  in the script that will sift through the status and message
 *  phase possibilities.  If one of those is detected, the 
 *  script will run on for a bit.
 */
#if defined (__STDC__) || defined (_ANSI_EXT) 
void siop_pm( Lstat lst )
#else
void siop_pm( lst )
Lstat lst;
#endif
{
	Lthread lth;
	
	lth = lst->phythread;			/* set pointer to thread */
	
	if( lth == &lst->logthread[lst->selfid] ) {
		/* pathalogical case, somebody has mismanaged the bus...
		 */

		scsi_free( lst, lth );
		return;

	}

	/* at least there is a logical thread. */ 		

	if( ncr_ph_idata(lst) || ncr_ph_odata(lst) ) {
		/* phase mismatch occured during data phase */
		save_state(lst);				/* need to preserve state here */
		lth->threadstate = TH_LOGICAL;	/* mark this one disconnected */
	} else {
		bomblog(lst, lth, SPM );	/* logit as an error! */
	}

	/* determine phase and react accordingly */
	act_scsi_idle( lst, lth );
	rethread( lst, lth, (int32) lth->scriptstart );
}

/*
 *	Function:
 *		irq - interrupt service entry point 
 *
 *  Synopsis:
 *		int irq( Lstat lst )
 *
 *    Returns TRUE: was our interrupt,  FALSE: was not our interrupt
 *
 *  Description:
 *		THIS IS NOT A USER ENTRY POINT.  This is the entry point that
 *  is installed for exception handing by init(). [via _f_irq()]
 *		This routine will determine if a valid interrupt is present
 *  from the SIOP and route it's operation to the appropriate service
 *  handler.  If it determines that no valid interrupt is present from
 *  the SIOP it will then return to the exception handler indicating that
 *  it was not this device that cause the interrupt.
 *
 *  RULES FOR SERVICE ROUTINES CALLED BY THIS CODE:
 *      -  must service the interrupt fully.
 *      -  must return WAS_US (indicating irq serviced)
 */
#if defined (_OS9000)
error_code irqsvc ( Datmod dmod )

#elif defined (_OSK)
#if defined (__STDC__) || defined (_ANSI_EXT) 
error_code irq (Lstat lst )
#else
error_code irq (lst )
Lstat lst;				/* low-level static storage */
#endif
#endif
{

#if defined (_OS9000)
	Lstat lst = (Lstat) dmod->dm_statics;
#endif
	
	int32 wakes,*wakeptr;				/* wake process count and queue ptr */
	error_code error;

	if( (error = ncr_irq_sort(lst )) != SUCCESS )
		return error;

	/* notify all processes waiting that have completed */
	wakeptr = &lst->wakequeue[0];
	wakes = lst->wakes;
	lst->wakes = 0;
	while( --wakes >= 0 )

#ifndef CBOOT

	{
		if (*wakeptr <= 1)		/* no signal to send or system process chdir */
			wakeptr++;			/* get nxt in line */
		else
			_os_send( *wakeptr++, SIGWAKE );
	}

#endif

	return SUCCESS;
}

	
/*
 *	Function:
 *		rethread - Start SIOP with the context requested.
 * 
 *  Synopsis:
 *		void rethread( Lstat lst, Lthread lth, int scriptent )
 *
 *  Description:
 *		This subroutine has the responsibility of activating the SIOP
 *  with the requested process as the physical thread, at the requested
 *  script location.
 *		If this call recieves a request to thread the SELFID
 *  it will rethread the script but there will not be a NULL pointer
 *  in the logical thread pointer.  This is to indicate that there
 *  is no active command.
 *
 *  Caveats:
 *		The current driver assumes an initiator only form.  In order
 *  to properly implement a target mode of operation will require a 
 *  degree of rethought, an amount of rewritting, and certainly a
 *  good deal of writting.
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void rethread( Lstat lst, Lthread const lth, int32 scriptent )
#else
void rethread( lst, lth, scriptent )
Lstat lst;						/* low level static storage */
Lthread lth;					/* logical thread to activate */
int32 scriptent;				/* offset to the appropriate start point */
#endif
{
#ifdef STATS
	dblog( lst, (int32) lth );		/* log the thread activated */
#endif
	lst->phythread = lth;
	lth->threadstate = TH_PHYSICAL;

	/* start the software controlled timer for select phase  */
	if ( (scriptent == lst->snatnaddr) || (scriptent == lst->swatnaddr ) ) {
		lst->selflg.sipflag[0] = TRUE;		/* flag select in progress */
		start_sto(0);						/* start timer */
	}

	/* kick off SIOP at requested location by giving it an address */
	coherent();

	ncr_setpar(lst, lth);	/* set parity if required */

	ncr_setsync(lst, lth);	/* set synchronous clock if required */

	ncr_rethread(lst, lth, scriptent); /* do rethread */

}

