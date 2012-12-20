/****************************************************************************
 *  SPF Device Driver history.                                              *
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
 * 200 10/13/95 <------------------ SPF 2.0 -------------------->      dwj
 * 201 01/16/96 Updated for new stricter compiler warnings             cas
 * 202 05/07/96 <------------------ SPF 2.1 Beta Release ------->      cas
 * 203 08/06/96 Added timer library macros in the makefiles            cas
 * 204 12/30/96 Changed so objects are put in SPF dir under bootobjs.  cas
 *              Added the SPF_GS_PROTID getstat so it would be included
 * 205 02/18/97 Ported to x86                                          taka
 * 205 02/19/97 Added per path library calls                           jmh
 * 206 03/13/97 Add per path storage                                   taka
 * 206 04/17/97 <---------------- SPF v2.2 (beta)--------------->      taka
 * 207 05/01/97 Check if there is a donwdriver for dr_downdata         cas
 *     06/26/97 <---------------- SPF V2.2 --------------------->      taka
 * 208 10/23/97 Drop the packets if the per-path storage is not found
 *              at dr_downdata                                         taka
 * 209 10/29/97 Added attach and use count information for DEBUG       taka
 * 210 02/26/98 Added -ao=sc optimization option for icode linking     taka
 *              <---------------- SPF v2.2.1 ------------------->      taka
 * 211 07/19/98 <---------------- SPF v2.2.2 ------------------->      taka
 * 212 04/26/99 Removed duplicated prototypes (proto.h)                rds
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
