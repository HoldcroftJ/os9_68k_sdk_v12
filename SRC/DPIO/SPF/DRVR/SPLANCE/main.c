/****************************************************************************
 *  File:   @(#) main.c 1.1@(#)
 *  Date:   4/21/95
 ****************************************************************************
 *  SPF LANCE Ethernet Device Driver initialized static storage.            *
 ****************************************************************************
 *  Copyright 1994 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date       Comments                                           By   *
 *  --- --------    ----------------------------------------------- ------- *
 *    0 01/11/94    Creation                                            bat *
 *    1 01/11/94    More additions                                      bat *
 *    3 01/27/94    Added wait-for-connection ability                   bat *
 *    4 01/28/94    Moved hw definitions to ports directory             bat *
 *      03/15/94    <***************** SPF v1.0 ******************>         *
 *    5 03/28/95    Ported for SPF 2.0                                  cas *
 *      06/30/95 <--------------- SPF v2.0(pre) ---------------->        jk *
 *	27FEB96	    modified for LANCE               Bill West Inc.     bw  *
 *    4 09/03/96	bug fix --- correct number of entries               *
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release*********>         *
 *      05/05/97    <***** SPF LAN Comm Pack v3.1 Release*********>         *
 *     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              *
 ****************************************************************************/

/*
 *	Header Files
 */

#define SYSEDIT 
#include <SPLANCE/defs.h>

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
	'l','e','d','r','s','p','e','c'		/* dr_demarc[] */

};

#ifdef _OS9000
_asm("_m_share: equ dr_stat");
#else
_asm("_m_usage: equ dr_stat+$8000");
#endif

