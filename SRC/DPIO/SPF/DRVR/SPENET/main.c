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
 *   7 09/12/96 added _m_share
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *  24 04/28/97 Split part of defs.h into bsddefs.h                    rds
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *     05/20/97 <--------- SPF LAN Comm Pack v3.1.1 Release -------->  
 *  32 04/01/98 Cleaned up code.                                       sr
 *     04/01/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  37 11/22/99 Added new drstat variables                             rds
 *  41 02/04/01 Added spip_deventry to driver statics                  rds
 ****************************************************************************/

/*
**	Header Files
*/

#define SYSEDIT
#include "bsddefs.h"

/************************************************************
 *                                                          *
 *  Declare Device Driver Static Storage                    *
 *                                                          *
 ************************************************************/

spf_drstat dr_stat = {
	SPF_VERSION,           /* dr_version: Version compiled for          */
	NULL,                  /* dr_fmcallup: fm call-up entry point       */
	dr_iniz,               /* dr_iniz: driver initialize entry point    */
	dr_term,               /* dr_term: driver terminate entry point     */
	dr_getstat,            /* dr_getstat: driver getstat entry point    */
	dr_setstat,            /* dr_setstat: driver setstat entry point    */
	dr_downdata,           /* dr_downdata: driver write entry point     */
	dr_updata,             /* dr_updata: receive & send up entry point  */
	0,                     /* dr_att_cnt: # attaches to this driver     */
	NULL,                  /* dr_lulist: only used when  dr_lumode=1    */
	DR_ALLOC_LU_PERPORT,   /* lu_mode: logical unit allocation mode     */
	{ 0 },                 /* dr_rsv1[11]: RESERVED FOR FUTURE USE!     */
	0,                     /* dr_use_cnt: driver use count              */

	/*Driver specific initialized storage */
	'e','t','d','r','s','p','e','c',       /* dr_demarc[] */
	0,                     /* dr_num_enet_paths  */
	NULL,                  /* dr_pp_list         */
	0,                     /* ip_count           */
	0,                     /* good_ip_count      */
	0,                     /* arp_count          */
	0,                     /* rarp_count         */
	0,                     /* unknown            */
	{0},                   /* arpreq_timeout     */
	{0},                   /* arpstat            */
	NULL,                  /* enet_entry_dncache */
	NULL,                  /* enet_entry_upcache */
	0,                     /* pd_dncache         */
	0,                     /* dev_upcache        */
	NULL,                  /* arpent_cache       */
	0,                     /* sysglobs           */
	NULL,                  /* timer_sec          */
	DEF_TIMER_INT * 1000,  /* timer_int          */
	DEF_KILL_C,            /* kill_c             */
	DEF_KILL_I,            /* kill_i             */
	NULL,                  /* spip's deventry    */
};                           

#ifdef _OS9000
_asm("_m_share: equ dr_stat");
#else
_asm("_m_usage: equ dr_stat+$8000");
#endif


