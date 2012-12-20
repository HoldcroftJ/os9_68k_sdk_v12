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
 * #   Date     Comments                                                By  *
 * --- -------- -----------------------------------------------------  ---- *
 *   1 01/07/94 Created from SPF v1.0.2                                 bat *
 *   2 03/28/95 Reorganization of logical unit & driver static          bat *
 *   3 04/18/95 Split driver into DCE/DTE sides                         bat *
 *     06/30/95 <--------------- SPF 2.0 pre-release------------>       cas *
 *   4 20/06/95 SPF 2.0 release SPLOOP supporting multiple opens        cas *
 *              to /loop                                                cas *
 * 200 10/13/95 <-------------------- SPF 2.0 ------------------>       dwj *
 * 201 01/16/96 Cleaned up per the code walkthrough on 1/5 & 1/11       cas *
 * 202 02/18/97 Ported to x86                                           taka*
 * 203 02/28/97 Port to ARM                                             taka*
 * 204 04/10/97 TX_SIZE from descriptor                                 taka*
 *     04/17/97 <---------------- SPF v2.2 --------------------->       taka*
 * 205 07/14/97 setstat close fix for far-end-hangup registered path    taka*
 * 206 10/16/97 setstat ITE_ANSWER fix for established connection       taka*
 * 207 10/29/97 added attach and use count information for debug        taka*
 * 208 10/30/97 Added timer function calls in setstat for timer testing taka*
 * 209 02/26/98 Added -ao=sc optimization option for making             taka*
 * 210 03/23/98 Removed unreferenced variables to remove compiler warnings  *
 *                                                                      taka*
 *     03/23/98 <---------------- SPF v2.2.1 ------------------->       taka*
 * 211 06/19/98 Bug Fix: DDTS 4217 - incomplete copy of mbuf chain      taka*
 *     07/19/98 <---------------- SPF v2.2.2 ------------------->       taka*
 * 212 10/02/01 CF9782: irq_* prototypes in some SPF drivers are bad	rry *
 ****************************************************************************/

/*
**	Edition/Revision Numbers
*/

/* Driver edition number information */
#ifdef SYSEDIT

_asm("_sysedit: equ 212");
_asm("_sysattr: equ 0xA000");

#endif

/* Descriptor edition number information */
#ifdef DESC_SYSEDIT

_asm("_sysedit: equ 211");
_asm("_sysattr: equ 0x8000");

#endif
