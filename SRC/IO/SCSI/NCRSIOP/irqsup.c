/*
 * irqsup.c  - interrupt support subroutines for scsisiop low level SCSI driver.
 *       (NCRSIOP SCSI I/O Processor)
 */
 
/*--------------------------------------------------------------------------,
|                                                                           |
|           Copyright 1991-1998 by Microware Systems Corporation            |
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
|             ---- OS-9000/PPC V2.1.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
| 02 98/01/29 Fixed compiler warnings.                                GbG   |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- SCSI Common Sub-Component v1.0 Released ----             |
|             $$                    <RELEASE_INFO>                     $$   |
`--------------------------------------------------------------------------*/
 
#include	<scsisiop.h>

/*
 *  Function:
 *		reschedule - restart the SIOP with at idle or new pending thread.
 *
 *  Synopsis:
 *		void reschedule( Lstat lst)
 *
 *  Description:
 *		This subroutine will restart the SIOP in one of two ways
 *  depending on the current state of the SCSI Bus.  Note that this
 *  routine should only be called from interrupt service at such a time
 *  as the bus is in a free state.  The proceedure is:
 *
 *  default start as SELFID
 *	for( all possible legal threads ) {
 *    if( TH_PENDING ) {
 *       select this thread as next
 *    }
 *  }
 *  start SIOP
 *
 *  Caveats:
 *		The methodology of the subroutine will select as next the lowest
 *  SCSI ID thread.  This means that the devices at the lowest SCSI IDs
 *  will tend to be serviced more often.  Under the current default ordering
 *  scheme this means that the fastest devices on the bus are serviced in
 *  preference to the slow devices.
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void reschedule( Lstat lst )
#else
void reschedule( lst )
Lstat lst;
#endif
{
	Lthread nth;
	int8 loop;
	
	nth = &lst->logthread[lst->selfid];	/* init to idle thread */
	
	loop = NTHREADS;					/* init loop count */
	while( --loop >= 0 ) {				/* locate pending threads if any */
		if( lst->logthread[loop].threadstate == TH_PENDING) {
			nth = &lst->logthread[loop];

			/* fairness test. If we were the last then make sure no one else is 
				trying to gain the system. */
				
			if (lst->lastthread != loop){
				lst->lastthread = loop;
				break;
			}
		}
	}
	/* at this point, if a thread was pending, we will start it, otherwise
	 *  we will restart the chip at IDLE with the self-ID as the physical
	 *  thread.
	 */

	rethread( lst, nth, (int32) nth->scriptstart); /* restart SIOP */
}

/*
 *	Function:
 *		setasyn - send a set asynchronus message to the target  
 *  Synopsis:
 *		int setasyn( Lstat lst, Lthread lth, int period, int offset )
 *  Description:
 *		Set up the message out buffer for a synchronus request message,
 *  and restart the SIOP at the proper place.
 *
 *  See Also:
 *		check_synchmsg()
 *      scripti_msg() [irq.c]
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
error_code setasyn( Lstat lst, Lthread lth, int32 period, int32 offset )
#else 
error_code setasyn( lst, lth, period, offset )
Lstat lst;
Lthread lth;
int32 period;
int32 offset;
#endif
{
	volatile u_int8 *msg;

	msg = &lth->msgobuf[0];			/* point to message buffer */
	lth->msgoutlen = SWAPLONG(5);	/* size of the message */
	*msg++ = M_EXTENDED;			/* extended message */
	*msg++ = SYNCREQ_SIZE;			/* size of the message */
	*msg++ = SYNC_REQUEST;			/* sync negotiation */
	*msg++ = period;				/* SCSI period */
	*msg++ = offset;				/* SCSI offset */
	scsi_smsg( lst, lth );			/* send message */
	return 0;
}

/*
 *	Function:
 *		bomblog - log a serious error
 *  Synopsis:
 *		void bomblog( Lstat lst, Lthread lth )
 *  Description:
 *		Clone the info from the last serious but retriable problem.
 *
 *  Notes:  We should never reach this routine with SELFID as the
 *          logical thread.  We have seen cases of the SCSI bus being
 *          being mismanaged by devices, resulting in this behaviour.
 *
 *  See Also:
 *		irq.c
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void bomblog( Lstat lst, Lthread lth, int32 cerr )
#else
void bomblog( lst, lth, cerr )
Lstat lst;
Lthread lth;
int32 cerr;
#endif
{
#ifdef STATS
	Bmlog bptr;
	u_int32	tmp;
	Cmdblk cbkptr;

	tmp = (u_int32)lth->cbkptr;
	tmp = SCRIPT_MADDRESS(SWAPLONG(tmp));
	cbkptr = (Cmdblk) tmp;
	
	bptr = &lst->thebomb;				/* pointer to save structure */
	bptr->errlog = cerr;				/* log offense */
	bptr->devf = lth->devfield.device;	/* ID and sxfer registers */
	bptr->cdsiz = lth->datacount;		/* data count at error time */


	if( lth != &lst->logthread[lst->selfid] ) {
		/* exclude logging if we think we are idle.  When "SELFID"
		 * is the logical thread, the command block pointer and
		 * SCSI packet are not valid, and are not logged.
		 */
#ifndef CBOOT
		_os_move( (void *)&cbkptr, 
					(void *)&bptr->cmd, (u_int32)sizeof(struct cmdblk));

		_os_move( (void *)&cbkptr->_cb_cmdptr, 
				(void *)&bptr->spkt, (u_int32)sizeof(union scsipkt));
#endif
	}
#endif
}
