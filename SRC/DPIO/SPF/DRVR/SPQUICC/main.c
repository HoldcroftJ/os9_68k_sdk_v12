/*****************************************************************************
 *  File:   @(#) main.c 1.6@(#)
 *  Date:   10/22/97
 *****************************************************************************
 *  SPF QUICC Ethernet Device Driver initialized static storage.             *
 *****************************************************************************
 *            Copyright 1989-1996 by Microware Systems Corporation           *
 *                            All Rights Reserved                            *
 *                          Reproduced Under License                         *
 *                                                                           *
 *  This software is confidential property of Microware Systems Corporation, *
 *  and is provided under license for internal development purposes only.    *
 *  Reproduction, publication, distribution, or creation of derivative works *
 *  in any form to any party other than the licensee is strictly prohibited, *
 *  unless expressly authorized in writing by Microware Systems Corporation. *
 *****************************************************************************
 *  Edition History:                                                         *
 *   #   Date       Comments                                           By    *
 *  --- --------    ----------------------------------------------- -------- *
 *  01 10/07/96 Creation                                                     *
 *     02/19/97 <***** SPF LAN Comm Pack v3.0 Release*********>              *
 *     05/05/97 <***** SPF LAN Comm Pack v3.1 Release*********>              *
 *  09 08/11/97 Added SCC channel dependent code                        taka *
 *     10/22/97 <***** OS-9000 MBX BSP Release v1.0 **********>              * 
 *     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              *
 *              $$                    <RELEASE_INFO>                     $$  *
 ****************************************************************************/

/*
 *	Header Files
 */

#define SYSEDIT 
#include <SPQUICC/defs.h>

spf_drstat dr_stat = {
	SPF_VERSION,				/* dr_version: Version compiled for	*/
	NULL,						/* dr_fmcallup: fm call-up entry point */
	dr_iniz,					/* dr_iniz: driver initialize entry point */
	dr_term,					/* dr_term: driver terminate entry point */
	dr_getstat,					/* dr_getstat: driver getstat entry point */
	dr_setstat,					/* dr_setstat: driver setstat entry point */
	dr_downdata,				/* dr_downdata: driver write entry point */
	dr_updata,					/* dr_updata: receive & send up entry point */
	0,							/* dr_att_cnt: # attaches to this driver */
    NULL,                       /* dr_lulist */
	DR_ALLOC_LU_PERPORT,		/* dr_lumode */
	{ 0 },						/* dr_rsv1[20]: RESERVED FOR FUTURE USE! */
	0,							/* dr_use_cnt */

	/*Driver specific initialized storage */
	NULL,                       /* Cache Control Function Pointer */
	'q','e','d','r','s','p','e','c'		/* dr_demarc[] */

};

#ifdef _OS9000
_asm("_m_share: equ dr_stat");
#else
_asm("_m_usage: equ dr_stat+$8000");
#endif

