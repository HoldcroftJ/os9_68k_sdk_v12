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
 *  01 10/27/97 Created                                                rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#include <SPROUTE/history.h>

/*
** required so that pf.h includes protocol driver specific information
*/
#define SPF_DRVR

struct list_hdr {
	void    *next;
	void    *prev;
	u_int32  count;
};

typedef struct list_hdr list_hdr, *List_hdr;

/***********************************************************************
 * Note that spf.h can't be included here because it requires SPF_LUSTAT,
 * SPF_DRSTAT and SPF_LUSTAT_INT to be defined
 ***********************************************************************/

/*
** Device-specific driver static storage (one copy per driver).  Again,
** must be defined before spf.h is included.
*/
#define SPF_DRSTAT \
	char		dr_demarc[10];		/* Demarc for driver specifics portion */\
	u_int32		dr_num_route_paths;	/* # of open paths to SPROUTE */\
	Dev_list	deventry;			/* device list entry */\
	void		*dr_cglobs; \
	list_hdr	dr_pp;


/*
** device-specific logical unit static fields.  Must be defined before
** spf.h is included.  I decided to stick all this in the lustat instead
** of the drstat because most entry points already figure out the lustat
** pointer, why find out the drstat pointer too?
*/

#define SPF_LUSTAT

#define SPF_LUSTAT_INIT

/* SPF definitions */

#include <SPF/spf.h>
