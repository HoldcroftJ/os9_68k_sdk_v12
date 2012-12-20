/****************************************************************************
 *           Copyright 1989-2001 by Microware Systems Corporation           *
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
 * This file contains defines to identify Low level Gargoyle drivers.       *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *  01 08/13/96 Created                                                sr
 *              ---- OS-9000/PPC V2.0.1 Released ----
 *              ---- OS-9/68K V3.0.3 System-state Debug Support ----
 *              ---- OS-9000/PPC V2.1.1 Released ----                   
 *  02 03/05/97 Updated for LL82596                                    pjw
 *              ---- OS-9000 V2.2 Released ----                   
 *              ---- OS-9000/Brutus Board Support V1.0 Released ----
 *              ---- Low-level Protocol Modules v1.0 ----
 *              ---- Modular ROM v1.2 Sub-component Released ----
 *  03 01/13/00 Added IPPROTO_LLF8260 and IPPROTO_LLS8260              rds
 *  04 03/17/00 Added IPPROTO_LL91C94                                  rds
 *  05 04/10/00 Added IPPROTO_LLSONIC                                  dwj
 *  06 05/21/00 Added IPPROTO_LLPRO100                                 GbG
 *  07 06/04/00 Added IPPROTO_LL8139                                   GbG
 *  08 08/26/00 Added IPPROTO_LL79C961                                 GbG
 *  09 09/09/00 Added IPPROTO_LLSA9730                                 GbG
 *  10 09/17/00 Added IPPROTO_LLNE2000                                 GbG
 *  11 10/29/00 Added IPPROTO_LLFA311                                  GbG
 *  12 11/07/00 Added IPPROTO_LL8900                                   dwj
 *  13 02/02/01 Added IPPROTO_LL405GP								   rry
 ****************************************************************************/

/* Header file for all the ipproto_<lldriver> defs. */

#ifndef _ROM_IPPROTO_H
#define _ROM_IPPROTO_H

/* Only add numbers less than those specified here */

#define IPPROTO_LLE355     232
#define IPPROTO_LL405GP    233
#define IPPROTO_LL8900     234
#define IPPROTO_LLFA311    235
#define IPPROTO_LLNE2000   236
#define IPPROTO_LLSA9730   237
#define IPPROTO_LL79C961   238
#define IPPROTO_LL8139     239
#define IPPROTO_LLPRO100   240
#define IPPROTO_LLSONIC    241
#define IPPROTO_LL91C94    242
#define IPPROTO_LLS8260    243
#define IPPROTO_LLF8260    244
#define IPPROTO_LL860T     245
#define IPPROTO_LL82596    246
#define IPPROTO_LLE509     247
#define IPPROTO_LLNS83C790 248
#define IPPROTO_LL7990     249
#define IPPROTO_LLQUICC    250
#define IPPROTO_LLNS83902  251
#define IPPROTO_LL21040    252
#define IPPROTO_RDP        253
#define IPPROTO_SLIP       254

#endif	/* _ROM_IPPROTO_H */

