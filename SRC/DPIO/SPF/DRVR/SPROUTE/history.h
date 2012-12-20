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
 *  01 10/27/97 Creation                                               rds
 *  02 11/22/97 Added BSDCALL to sofree, rtalloc1, rt_setgate,         rds
 *                 ifa_ifwith*, and rn_addmask calls
 *     11/28/97 ifnet and mask_rnhead are pointers to pointers         rds
 *     11/28/97 set pid in routing messages, remove stat.h             rds
 *  03 12/02/97 Removed ADD_ENTRY and DROP_ENTRY from entry.c          rds
 *  04 02/04/98 Add copyout calls for security in SPF_GS_SYMBOLS       rds
 *              Removed compiler warnings                              rds
 *  05 02/17/98 Handle soisconnected locally instead of calling spip   rds
 *              Return EINVAL for PRU_CONNECT and PRU_BIND             rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  06 05/14/98 Added ";" for _asm labels for compiler bug MHQco03955  tmd
 *  07 11/04/98 Added buffer size check before copyout (rtsock.c)      rds
 *  08 07/27/00 Added support for bypassing SPF when reading sockets   rds
 *  09 02/13/01 No code change - linked with new socketvar.h           rds
 *  10 10/08/01 Now gets stats when setting ifm_data (rtsock.c)        rds
 ****************************************************************************/

/*
**	Edition/Revision Numbers
*/
#ifdef SYSEDIT

_asm("_sysedit: equ 10");
_asm("_sysattr: equ 0xA000");

#endif /* SYSEDIT */

/* descriptor edition number */
#ifdef DESC_SYSEDIT

_asm("_sysedit: equ 2");
_asm("_sysattr: equ 0x8000");

#endif /* DESC_SYSEDIT */
