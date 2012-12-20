/****************************************************************************
 *  ID:		%A%
 *  Date:	%G%
 ****************************************************************************
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
 *  01 03/06/96 Initial                                                billw
 *  16 04/17/97 First cut at threading.                                sr
 *  17 05/15/97 In rcv_loop, commented out unwanted code. (hdlc_thd.c) sr
 *     05/13/97   ***** SPF LAN Comm Pack v3.1 Release ********
 *  18 14/97/97 In ite_hangup and ss_port_done, send SIGWAKE to rcv    sr
 *              thread.  Sending any other signal will queue up the
 *              signals and never get dequeued. (entry.c)
 *  19 09/16/97 In ite_hangup, send signal to txthread. (entry.c)      sr
 *              In evwait, set the value of event back to 0, if we
 *              were asked to stop_txthread. (hdlc_thd.c)
 *  20 10/01/97 Added spf_sleep.  Free mbuf on error conditions.       tmd
 *  20 10/01/97 Added spf_sleep & SMCALL. (entry.c and hdlc_thd.c)
 *  21 10/02/97 Added call to terminate_thread to free up memory and   sr
 *              return process descriptor.  We do not call _os_exit
 *              in thread_exit anymore. (hdlc_thd.c)
 *  21 10/03/97 Added STACK_ALLOC defines for all processors. (entry.c)
 *  22 10/06/97 Call add_notify to remove ourselves from the list      sr
 *              in dr_term. (entry.c)
 *              Use _os_* functions instead of macros. (fcs_calc.c
 *              entry.c)
 *              If zeroed _OS_ macros.  Use dpio functions. (defs.h)
 *  23 10/10/97 In ite_hangup make sure that tx thread is done with    sr
 *              its thing before touching the event. (entry.c)
 *  24 01/19/98 Fixed to avoid compiler warning messages               tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  25 01/18/99 make event names unique for each descriptor from lun   mgh
 *  26 08/15/00 updated edition number because of fixing memory leak   zw
 *              problem in thread.c.
 *  27 12/12/00 Compiled with the new thread library to work with the  sr
 *              threaded kernel.
 *  28 10/02/01 CF9782: Fixed irq_* prototypes						   rry
 ****************************************************************************/

#ifndef _history_h_
#define _history_h_


/*
**	Edition/Revision Numbers
*/

#ifdef SYSEDIT

_asm("_sysedit: equ 28");
_asm("_sysattr: equ 0xA001");

#endif


#ifdef DESC_SYSEDIT

_asm("_sysedit: equ 16");
_asm("_sysattr: equ 0x8000");

#endif

#endif /* _history_h_ */
