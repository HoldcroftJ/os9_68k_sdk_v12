/****************************************************************************
 *  SPF Device Driver definitions.                                          *
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
 *     10/13/95 <------------------- SPF 2.0 ------------------->      dwj
 * 205 02/18/97 Ported to x86                                          taka
 * 205 02/19/97 Added per path library                                 jmh
 * 206 03/13/97 Add per path storage                                   taka
 * 206 06/26/97 <---------------- SPF v2.2 --------------------->      taka
 *     03/24/98 Added the mbuf header and debug display functions to
 *              remove compiler warnings                               taka
 *              <---------------- SPF v2.2.1 ------------------->      taka
 * 211 07/19/98 <---------------- SPF v2.2.2 ------------------->      taka
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

/* Your special protocol definitions file here */

#ifdef DEBUG

#include <dbg_mod.h>

#include <SPF/mbuf.h>
extern void debug_mbuf_up(Dbg_stat debug_stat, Mbuf mbptr, u_int32 size);
extern void debug_mbuf_down(Dbg_stat debug_stat, Mbuf mbptr, u_int32 size);

#define DEBUG_DATA(a,b,c)		debug_data(a,b,(u_int32)c)
#define DEBUG_MBUF_UP(a,b)		debug_mbuf_up(a,b,32)
#define DEBUG_MBUF_DOWN(a,b)	debug_mbuf_down(a,b,32)
#define DEBUG_4DATA(a,b,c,d,e)	debug_4data(a,(u_int32)b,(u_int32)c,(u_int32)d, (u_int32)e)

#else
#define DEBUG_DATA(a,b,c)
#define DEBUG_MBUF_UP(a,b)
#define DEBUG_MBUF_DOWN(a,b)
#define DEBUG_4DATA(a,b,c,d,e)
#endif

#if 0
#include <SPF/spf.h>	/* Include if you need the app part of definitions	*/
#include <SPF/timer.h>  /* Include if timer services are required	*/
#endif

/* per path storage definitions */
#define SPF_PPSTAT \
	u_int16     pp_stk_txoffset,    /* Header req. for stk below    */\
				pp_stk_txtrailer;   /* Trailer req. for stk below   */\
	u_int32		pp_addr;			/* address						*/

/* spproto-Specific Driver Definitions */
#define SPF_DRSTAT
	/* None defined for spproto */

/* device-specific logical unit static fields.  Must be defined before spf.h 
 * is included. */
#define SPF_LUSTAT \
	void*		lu_dbg;							/* debugging pointer */\
	char		lu_dbg_name[16];				/* Name of debug module */

#ifndef DEFAULT_ADDR
#define DEFAULT_ADDR	0
#endif

#define SPF_PPSTAT_INIT \
	0,					/* pp_stk_txoffset 	*/\
	0,					/* pp_stk_txtrailer */\
	DEFAULT_ADDR		/* pp_addr			*/

#ifndef DEBUG_NAME
#define DEBUG_NAME	"dbg_proto"
#endif
	
#define SPF_LUSTAT_INIT \
	NULL,										/* lu_dbg */\
	{DEBUG_NAME}								/* lu_dbg_name */

/* SPF definitions */

#define SPF_DRVR		/* to include driver specific portions in spf.h */
#include <SPF/spf.h>

#include <SPPROTO/history.h>
#include <SPPROTO/proto.h>
