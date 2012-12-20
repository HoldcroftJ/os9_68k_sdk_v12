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
 *   1 04/04/97 Creationd from SPF                                     mil
 *   4 08/27/97 Bug fix                                                taka
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  11 06/22/99 Integrated Kei Thomsen's changes, fixed mbuf leak.     jk
 ****************************************************************************/

/*
**	Header Files
*/

#define SYSEDIT
#include <SP82596/defs.h>

/*
**	SPF Device Driver Initialized Global Static Storage
*/

spf_drstat dr_stat = {

	/* general SPF fields */
	SPF_VERSION,				/* dr_version */
	NULL,						/* dr_fmcallup */
	dr_iniz,					/* dr_iniz */
	dr_term,					/* dr_term */
	dr_getstat,					/* dr_getstat */
	dr_setstat,					/* dr_setstat */
	dr_downdata,				/* dr_downdata */
	dr_updata,					/* dr_updata */
	0,							/* dr_att_cnt */
	NULL,						/* dr_lulist */
	DR_ALLOC_LU_PERPORT,		/* dr_lumode */
	{ 0 },						/* dr_rsv1[] */
	0,							/* dr_use_cnt */

	/*Driver specific initialized storage */
	'm','y','d','r','s','p','e','c',		/* dr_demarc[] */
	{ 0 },
};

#ifdef _OS9000
_asm("_m_share: equ dr_stat");
#else
_asm("_m_usage: equ dr_stat+$8000");
#endif
