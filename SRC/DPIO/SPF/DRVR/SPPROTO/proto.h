/****************************************************************************
 *  SPF Device Driver function prototypes.                                  *
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
 *   4 08/28/95 Added C++ support                                      dwj
 *     10/13/95 <------------------ SPF 2.0 -------------------->      dwj
 * 206 06/26/97 <---------------- SPF v2.2 --------------------->      taka
 * 211 07/19/98 <---------------- SPF v2.2.2 ------------------->      taka
 * 212 04/26/99 Removed prototypes duplicated in regs.h and defconv.h  rds
 ****************************************************************************/

/*
**	Device Driver Entry Point Function Prototypes
*/

/*	C++ support	*/
#ifdef __cplusplus
extern "C" {
#endif

/* entry.c */
extern error_code dr_iniz(Dev_list deventry);
extern error_code dr_term(Dev_list deventry);
extern error_code dr_getstat(Dev_list deventry, Spf_ss_pb pb);
extern error_code dr_setstat(Dev_list deventry, Spf_ss_pb pb);
extern error_code dr_downdata(Dev_list deventry, Mbuf mb);
extern error_code dr_updata(Dev_list deventry, Mbuf mb);

/*
**	Other Function Prototypes
*/

/*	End C++	*/
#ifdef __cplusplus
}
#endif

