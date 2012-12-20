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
 *     10/13/95 <------------------- SPF 2.0 ------------------->       dwj *
 * 202 02/18/97 Ported to x86                                           taka*
 * 203 02/20/97 Port to ARM                                             taka*
 * 204 06/26/97 <---------------- SPF v2.2 --------------------->       taka*
 * 208 11/04/97 <---------------- SPF v2.2.1 ------------------->       taka*
 * 211 07/19/98 <---------------- SPF v2.2.2 ------------------->       taka*
 ****************************************************************************/

/*
**	Header Files
*/

/* DPIO definitions */
#ifdef _OSK
#include <defconv.h>
#endif

/* System definitions */

#include <const.h>
#include <errno.h>
#include <signal.h>
#include <events.h>
#include <memory.h>
#include <string.h>
#include <sg_codes.h>
#include <modes.h>
#include <module.h>
#include <process.h>

#ifdef DEBUG
#include <dbg_mod.h>
#define DEBUG_DATA(a,b,c)		debug_data(a,b,(u_int32)c)
#define DEBUG_MBUF_UP(a,b)		debug_mbuf_up(a,b,48)
#define DEBUG_MBUF_DOWN(a,b)	debug_mbuf_down(a,b,48)
#define DEBUG_4DATA(a,b,c,d,e)	debug_4data(a,(u_int32)b,(u_int32)c,(u_int32)d, (u_int32)e)
#else
#define DEBUG_DATA(a,b,c)
#define DEBUG_MBUF_UP(a,b)
#define DEBUG_MBUF_DOWN(a,b)
#define DEBUG_4DATA(a,b,c,d,e)
#endif

/* The operation of the loopback driver is dependant on what the logical
 * unit number of the device is.
 *
 * From LUN 0 through LU_FIRST_CONNLESS-1 is the range where all
 * connection oriented devices are stored in dr_call_list.
 *
 * From LU_FIRST_CONNLESS to MAX_CALL_ENDPTS-1 is the range where all
 * connectionless path types are stored.
 *
 * Anything >= MAX_CALL_ENDPTS is assumed to be loopback to itself
 *
 */
#define LU_FIRST_CONNLESS	5
#define MAX_CALL_ENDPTS		9	/* Maximum # "endpoints" using the driver	*/

/* #include <SPF/item_pvt.h> */

/* sploop-Specific Driver Definitions */
typedef struct lpbk_list	lpbk_list;
typedef struct lpbk_list*	Lpbk_list;

/* device-specific driver static fields */
#define SPF_DRSTAT \
	char		dr_demarc[8];						/* Driver demarc	*/\
	Spf_lustat	dr_call_list[MAX_CALL_ENDPTS];		/* Call list array	*/\
	Spf_pdstat	dr_path_list[MAX_CALL_ENDPTS];	/* pathdesc list array  */\
	Dev_list	dr_dev_list[MAX_CALL_ENDPTS];	/* device list array	*/\
	u_int8		dr_other1[7];						/* unused			*/\
	u_int8		dr_num_lpbk_paths;				/* # lpbk paths in list	*/\
	Lpbk_list	dr_loopback_list;				/* Loopback path list	*/

/* device-specific logical unit static fields */
#define SPF_LUSTAT \
	char		lu_demarc[8];	/* Demarkation for lu specifics			*/\
	void*		lu_dbg;			/* debugging pointer 					*/\
	char		lu_dbg_name[16];/* NAme of the debug data module		*/\
	Path_type	lu_itempath;	/* Ptr to ITEM path_type using this LU	*/\
	Spf_lustat	lu_felustat;	/* Far end logical unit static storage	*/\
	u_char		lu_sendto_lun,	/* Connless send to Log Unit Number		*/\
				lu_res1[3];		/* Long word alignmnent					*/\
	u_int32		lu_drop_count,  /* packet count for automatic drop		*/\
				lu_reserv;

/* For all macros defined in the SPF_LUSTAT_INIT macro below, these
 * macro values are defined in DEFS/spf_desc.h in this directory.
 * Since the driver pulls this file in and doesn't pull spf_desc.h in,
 * we need a default value so we don't get a compile error. So, we
 * include default values for all macros in the SPF_LUSTAT_INIT macro
 * here. (You'll notice it's the same way in spf.h)
 */
#ifndef THEIRLUN
#define THEIRLUN	0
#endif

#ifndef DEBUG_NAME
#define DEBUG_NAME	"dbg_sploop"
#endif

/* Initialized data for the driver specific portion of the logical
 * unit structure.
 */
#define SPF_LUSTAT_INIT \
	'l','p','s','p','e','c','i','f',	/* Demarcation for lu specifics	*/\
	NULL,						/* lu_dbg		*/\
	{DEBUG_NAME},				/* lu_dbg_name	*/\
	NULL,						/* lu_itempath	*/\
	NULL,						/* lu_felustat	*/\
	THEIRLUN,					/* lu_sendto_lun*/\
	{0},						/* lu_res1[]	*/\
	0,							/* lu_drop_count*/\
	0

/* SPF Definitions */

/* Since in spf.h, it checks for SPF_LUSTAT to be defined, we must pull this
 * .h file in AFTER we define the driver and logical unit specific variables.
 * We also need to define SPF_DRVR so we get all the structures included in
 * spf.h pulled in for making an SPF driver.
 */
#define SPF_DRVR
#include <SPF/spf.h>

/* After spf.h is defined, now we can define our loopback list structure
 * with the path descriptor in it.
 */
struct lpbk_list {
	Lpbk_list	next;
	Spf_pdstat	lpbk_pd;
	Dev_list	lpbk_updrvr,
				lpbk_dndrvr;
};

#include <SPLOOP/history.h>
#include <SPLOOP/proto.h>

#ifdef _ATM
#include <SPF/atm_pvt.h>
#endif
