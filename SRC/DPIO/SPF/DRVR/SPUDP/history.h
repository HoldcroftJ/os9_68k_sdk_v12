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
 *  01 06/29/97 Creation                                               rds
 *  02 09/22/97 Changed symbol initialization slightly for netstat     tmd
 *  03 10/07/97 Added support for MBT_SENDTO flag in dr_downdata       rds
 *  04 10/07/97 Fixed m_copy for multiple datagram recipients          rds
 *  05 11/06/97 Removed references to stat.h                           tmd
 *  06 12/02/97 Removed INIT_ENTRY, ADD_ENTRY, DROP_ENTRY from entry.c rds
 *  07 02/04/98 Added syscopy, removed compiler warnings               rds
 *              Add copyout calls for security in SPF_GS_SYMBOLS       rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  08 05/14/98 Added ";" for _asm labels for compiler bug MHQco03955  tmd
 *  09 09/30/98 Added m_dup, now works on packet chains (udp_usrreq.c) rds
 *  10 10/30/98 Fixed problem with m_dup changing mbuf's m_alloc size  rds
 *  11 10/16/98 Added udp_ctloutput to support multicast socket opts   rds
 *  12 07/27/00 Added support for bypassing SPF when reading sockets   rds
 *  13 02/13/01 No code change - linked with new socketvar.h           rds
 *  14 06/23/01 Don't overwrite flags and type in m_dup (bsd_subr.c)   rds
 ****************************************************************************/

/*
**	Edition/Revision Numbers
*/
#ifdef SYSEDIT

_asm("_sysedit: equ 14");
_asm("_sysattr: equ 0xA000");

#endif /* SYSEDIT */

/* descriptor edition number */
#ifdef DESC_SYSEDIT

_asm("_sysedit: equ 7");
_asm("_sysattr: equ 0x8000");

#endif /* DESC_SYSEDIT */
