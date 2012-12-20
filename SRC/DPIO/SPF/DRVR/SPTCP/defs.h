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
 *  01 ??/??/?? Created
 *  22 02/04/97 Added tcp_output to driver statics                     rds
 *  24 02/11/97 Add TCPWINDOW and SENDBUFFER to lustat                 rds
 *  25 02/13/97 add dr_max_offset to driver statics                    rds
 *  26 02/15/97 spfdefs.h split into defs.h and sptcp.h                rds
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *  30 03/18/97 Merged tcp_tmr into sptcp                              rds
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *     05/20/97 <--------- SPF LAN Comm Pack v3.1.1 Release -------->
 *  52 08/24/97 removed SPF/spsin.h and dr_max_offset                  rds
 *  63 03/04/98 tcp_do_rfc1323 is now set using descriptor value       rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#include <SPTCP/history.h>

/*
** required so that spsin.h and spf.h include protocol driver specific
** information
*/
#define SPF_DRVR

/****************************/
struct list_hdr {
	void	*next;
	void	*prev;
	u_int32		count;
	};

typedef struct list_hdr list_hdr, *List_hdr;
/****************************/


/***********************************************************************
 * Note that spf.h cant be included here because it requires SPF_LUSTAT,
 * SPF_DRSTAT and SPF_LUSTAT_INT to be defined
 ***********************************************************************/

/*
** Device-specific driver static storage (one copy per driver).  Again,
** must be defined before spf.h is included.
*/
#define SPF_DRSTAT \
	char		dr_demarc[8];		/* Demarc for driver specifics portion */\
	u_int32		dr_num_tcp_paths;	/* # of open paths to TCP */\
	Dev_list	deventry;			/* device list entry */\
	list_hdr	dr_pp;				/* Per path storage list header */\
	void		*dr_cglobs; \
	timer_pb	tcp_fast; \
	timer_pb	tcp_slow;


/*
** device-specific logical unit static fields.  Must be defined before
** spf.h is included.  I decided to stick all this in the lustat instead
** of the drstat because most entry points already figure out the lustat
** pointer, why find out the drstat pointer too?
*/

#define SPF_LUSTAT \
	u_int32		lu_dflt_mss; \
	u_int32		lu_tcp_window; \
	u_int32		lu_send_buffer; \
	u_int32		lu_rfc1323;

#define SPF_LUSTAT_INIT \
	DFLT_MSS, \
	TCPWINDOW, \
	SENDBUFFER, \
	DO_RFC1323

/* SPF definitions */

#include <SPF/spf.h>
