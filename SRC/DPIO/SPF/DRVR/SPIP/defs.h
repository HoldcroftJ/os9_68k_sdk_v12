/****************************************************************************
 *  ID:		%A%
 *  Date:	%G%
 ****************************************************************************
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
 *  01 09/27/96 Created
 *  06 01/07/97 Routing and interface code added                       rds
 *  07 01/08/97 Added/Fixed interface info from inetdb                 tmd
 *  11 01/16/97 Added ifunit to drstat                                 rds
 *  15 02/01/97 Added lu_gateway to lustat                             rds
 *  16 02/01/97 ifdef'd out calls to splnet and splimp                 rds
 *  17 02/03/97 Added support to print routing table                   rds
 *  20 02/12/97 Keep track of max txoffset to use in SPF_SS_UPDATE     rds
 *  23 02/16/97 Renamed from spfdefs.h                                 rds
 *              Remove duplicate per path storage structures           rds
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *  27 03/14/97 Merged ip_tmr into spip                                rds
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  39 09/18/97 removed SPF/spsin.h                                    rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#include <SPIP/history.h>

/*
** Required so that spf.h includes protocol driver specific information
*/
#define SPF_DRVR

/******************************************************/
/* BSD compatible list header for doubly-linked lists */

struct list_hdr {
	void	*next;
	void	*prev;
	u_int32		count;
};

typedef struct list_hdr list_hdr, *List_hdr;

/******************************************************/

/***********************************************************************
 * Note that spf.h can't be included here because it requires SPF_LUSTAT,
 * SPF_DRSTAT and SPF_LUSTAT_INT to be defined
 ***********************************************************************/

/* Device-specific driver static storage (one copy per driver).  Again,
 * must be defined before spf.h is included.
 */
#define SPF_DRSTAT \
	char		dr_demarc[8];		/* Demarc for driver specifics portion	*/\
	u_int32		dr_num_ip_paths;	/* # of open paths to IP				*/\
	u_int16		dr_ip_ident;		/* For ident field in IP header			*/\
	u_int16		dr_reserved;		/* Long word alignment					*/\
	Dev_list	deventry;			/* device list entry 					*/\
	list_hdr	dr_pp;				/* Per path storage list header			*/\
	void		*dr_cglobs;			/* spip's globals						*/\
	timer_pb	ip_tpb;				/* parameter block for IP slow_timer	*/\
	int			proc_inetdb;		/* process_inetdb flag					*/

/* device-specific logical unit static fields.  Must be defined before
 * spf.h is included.  I decided to stick all this in the lustat instead
 * of the drstat because most entry points already figure out the lustat
 * pointer, why find out the drstat pointer too?  for spip there will
 * be only one lustat and one drstat.
 */

#define SPF_LUSTAT \
	u_int32		lu_gateway;			/* Gateway flag */\
	u_int32		lu_defttl;			/* Default TTL */\
	u_int32		lu_sbmax;			/* Max sockbuf size */

#define SPF_LUSTAT_INIT \
	GATEWAY,		\
	DEFTTL,			\
	MAXSOCKBUF

/* SPF definitions */

#include <SPF/spf.h>
