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
 *   7 09/03/96 bug fix for pop and push
 *  10 10/22/96 add hardware txoffset in per path storage
 *  11 10/29/96 Bug fix for arp request timer
 *  18 01/23/97 New Style spenet works.                                sr
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *  23 04/27/97 Convert from ISP to BSD header files                   rds
 *  24 04/28/97 Split part of defs.h into bsddefs.h                    rds
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *     05/20/97 <--------- SPF LAN Comm Pack v3.1.1 Release -------->
 *  31 02/03/98 Removed SPF/spsin.h                                    rds
 *  32 04/01/98 Cleaned up code.                                       sr
 *     04/01/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  37 11/22/99 Changed drstat and lustat for new spenet               rds
 *  41 02/04/01 Added spip_deventry to driver statics                  rds
 ****************************************************************************/

#define TRUE	1
#define FALSE	0

#include <types.h>
#include "arp.h"
#include <sysglob.h>

/* SPF definitions */
#ifndef SPF_DRVR
#define SPF_DRVR
#endif

/* ENET per path info block. Pointers to the arp request retry is
 * stored here.
 */
typedef struct pp_enetlist pp_enetlist;
typedef struct pp_enetlist *Pp_enetlist;

/************************************************************
 *                                                          *
 *  Device Driver Static Specific Storage                   *
 *                                                          *
 ************************************************************/
#define SPF_DRSTAT \
	char		dr_demarc[8];		/* Demarc for driver specifics portion  */\
	u_int32		dr_num_enet_paths;	/* # of open paths to IP                */\
	Pp_enetlist	dr_pp_list;			/* Per path ENET storage list           */\
	u_int32		ip_count,			/* IP packet counter                    */\
				good_ip_count,		/* No longer used - same as ip_count    */\
				arp_count,			/* ARP packet counter                   */\
				rarp_count,			/* RARP packet counter                  */\
				unknown;			/* unrecognized ethertype counter       */\
	timer_pb	arpreq_timeout;		/* the arp_timeout timer parameter block*/\
	arpstat		arpstat;			/* ARP Static structure                 */\
	Pp_enetlist	enet_entry_dncache;	/* Per path cache for packets going down*/\
	Pp_enetlist	enet_entry_upcache;	/* Per path cache for packets coming up */\
	Spf_pdstat	pd_dncache;			/* Path descriptor cache (going down)   */\
	Dev_list	dev_upcache;		/* Device entry cache (coming up)       */\
	Arptab		arpent_cache;		/* ARP entry cache (going down)         */\
	Sysglobs	sysglobs;			/* Pointer to system globals            */\
	u_int32		*timer_sec;			/* Counter that changes every second    */\
	u_int32		timer_int;			/* Interval to run the cyclic timer     */\
	u_int32		kill_c;				/* Lifetime of complete entries (idle)  */\
	u_int32		kill_i;				/* Lifetime of incomplete entries       */\
	Dev_list	spip_deventry;		/* Spip's device entry                  */\


#ifndef TIMER_INT
#define TIMER_INT DEF_TIMER_INT * 1000
#endif

#ifndef KILL_C
#define KILL_C DEF_KILL_C
#endif

#ifndef KILL_I
#define KILL_I DEF_KILL_I
#endif

/************************************************************
 *                                                          *
 *  Logical Unit Static Specific Storage                    *
 *                                                          *
 ************************************************************/
#define SPF_LUSTAT \
	u_int32		lu_numprotoaddr;	/* Max # of protocol addresses per iface*/\
	void		*lu_dbg;			/* debugging pointer                    */\
	char		lu_dbg_name[16];	/* Name of debug data module            */\
	u_int32		lu_timer_int;		/* Interval to run cyclic timer         */\
	u_int32		lu_kill_c;			/* Lifetime of complete entries (idle)  */\
	u_int32		lu_kill_i;			/* Lifetime of incomplete entries       */\


#ifndef DEBUG_NAME
#define DEBUG_NAME "dbg_spenet"
#endif

#define SPF_LUSTAT_INIT \
	MAXADDR_PER_IFACE,	/* lu_numprotoaddr */\
	NULL,				/* lu_dbg          */\
	{DEBUG_NAME},		/* lu_dbg_name     */\
	TIMER_INT,			/* lu_timer_int    */\
	KILL_C,				/* lu_kill_c       */\
	KILL_I,				/* lu_kill_i       */\


#include <SPF/spf.h>

struct addr_list;

struct pp_enetlist {
	Pp_enetlist next;			/* 00 Next IP per path entry in list */
	Dev_list	dev_entry;
	Spf_pdstat	enet_pd;		/* 08 Pointer to the path descriptor */
	Dev_list	enet_updrvr,	/* 0c Updrvr for this instance of ENET */
				enet_dndrvr;	/* 10 dndrvr for this instance of ENET */
	struct addr_list *addr_list;/* 14 Ptr to protocol addr array for iface*/
	u_int8		ether_addr[6];	/* 18 Physical address of ethernet card	*/
	u_int16		hw_txoffset;	/* 1e hardware txoffset for ARP packet	*/
	u_int32		rsv[4];			/* Reserver for later use. */
};

#include <SPENET/history.h>
