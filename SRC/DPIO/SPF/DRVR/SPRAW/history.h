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
 *  02 10/07/97 Added support for MBT_SENDTO flag in dr_downdata       rds
 *  03 10/07/97 Fixed m_copy for multiple datagram recipients          rds
 *  04 11/22/97 Changed ifnet to struct ifnet **ifnet                  rds
 *  05 12/02/97 Removed INIT_ENTRY, ADD_ENTRY, DROP_ENTRY from entry.c rds
 *  06 01/07/98 Fixed mbuf leak in rip_ctloutput, removed warnings     rds
 *  07 02/04/98 Add copyout calls for security in SPF_GS_SYMBOLS       rds
 *              Added syscopy, removed compiler warnings               rds
 *  08 03/30/98 Fixed rip_ctloutput when setting IP_HDRINCL (raw_ip.c) rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  09 05/14/98 Swap globals before calling ifa_ifwithaddr(MHQco04104) rds
 *  10 05/28/98 Fixed rip_input on connected/bound sockets(MHQco04157) rds
 *  11 09/30/98 Added m_dup, now works on packet chains (raw_ip.c)     rds
 *  12 10/30/98 Fixed problem with m_dup changing mbuf's m_alloc size  rds
 *  13 07/27/00 Added support for bypassing SPF when reading sockets   rds
 *  14 02/13/01 No code change - linked with new socketvar.h           rds
 *  15 03/17/01 Added call to ip_ctloutput (raw_ip.c)                  rds
 *  16 06/23/01 Don't overwrite flags and type in m_dup (bsd_subr.c)   rds
 ****************************************************************************/

/*
**	Edition/Revision Numbers
*/
#ifdef SYSEDIT

_asm("_sysedit: equ 16");
_asm("_sysattr: equ 0xA000");

#endif /* SYSEDIT */

/* descriptor edition number */
#ifdef DESC_SYSEDIT

_asm("_sysedit: equ 2");
_asm("_sysattr: equ 0x8000");

#endif /* DESC_SYSEDIT */
