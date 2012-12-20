/****************************************************************************
 *  SPF Device Driver initialized static storage.                           *
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
 *   4 10/09/95 Added uninitialzed data coding support                 dwj
 *     10/13/95 <------------------- SPF 2.0 ------------------->      dwj
 * 206 06/26/97 <---------------- SPF v2.2 --------------------->      taka
 * 211 07/19/98 <---------------- SPF v2.2.2 ------------------->      taka
 ****************************************************************************/

/*
**	Header Files
*/

#define SYSEDIT
#include <SPPROTO/defs.h>

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
};

#ifdef _OS9000
_asm("_m_share: equ dr_stat");
#else
_asm("_m_usage: equ dr_stat+$8000");
#endif

