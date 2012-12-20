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
 * #   Date     Comments                                          By        *
 * --- -------- ------------------------------------------------- --------- *
 *   1 01/07/94 Created from SPF v1.0.2                                 bat *
 *   2 03/28/95 Reorganization of logical unit & driver static          bat *
 *   3 04/18/95 Split driver into DCE/DTE sides                         bat *
 *     06/30/95 <--------------- SPF 2.0 pre-release------------>       cas *
 *   4 10/09/95 Added uninitialzed data support coding                  dwj *
 *     10/13/95 <--------------------- SPF 2.0 ----------------->       dwj *
 * 202 02/18/97 Ported to x86                                           taka*
 * 204 06/26/97 <--------------- SPF 2.2 ----------------------->       taka*
 * 208 11/04/97 <--------------- SPF v2.2.1 -------------------->       taka*
 * 211 07/19/98 <---------------- SPF v2.2.2 ------------------->       taka*
 ****************************************************************************/

/*
**	Header Files
*/

#define SYSEDIT
#include <SPLOOP/defs.h>

/*
**	SPF Device Driver Initialized Global Static Storage
*/

spf_drstat dr_stat = {

	/* general SPF fields */
	SPF_VERSION,			/* dr_version 		*/
	NULL,					/* dr_fmcallup 		*/
	dr_iniz,				/* dr_iniz 			*/
	dr_term,				/* dr_term 			*/
	dr_getstat,				/* dr_getstat 		*/
	dr_setstat,				/* dr_setstat 		*/
	dr_downdata,			/* dr_downdata 		*/
	dr_updata,				/* dr_updata 		*/
	0,						/* dr_att_cnt 		*/
	NULL,					/* dr_lulist 		*/
	DR_ALLOC_LU_PERPORT,	/* dr_lumode 		*/
	{ 0 },					/* dr_rsv1[] 		*/
	0,						/* dr_use_cnt 		*/

	/* SPLOOP device-specific fields */
	'l','p','s','p','e','c','i','f',	/* Demarc for dr specifics	*/
	{0},					/* dr_call_list[] 	*/
	{0},					/* dr_path_list[] 	*/
	{0},					/* dr_dev_list[] 	*/
	{0},					/* dr_other1[] 		*/
	0,						/* dr_num_lpbk_paths*/
	NULL					/* dr_loopback_list	*/
};

#ifdef _OS9000
_asm("_m_share: equ dr_stat");
#else
_asm("_m_usage: equ dr_stat+$8000");
#endif
