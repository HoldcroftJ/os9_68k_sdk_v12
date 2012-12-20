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
 *  01 07/15/97 Created                                                rds
 *  02 11/05/97 Added IP_SS_SYSCTL                                     rds
 *  03 03/10/98 Added IP_INT_SETSTAT & IP_INT_GETSTAT                  tmd
 *  04 03/10/98 Added forward declaration for spf_ss_pb                rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  05 10/14/99 Added IP_GS_SOCKNAME and IP_GS_PEERNAME                rds
 *  06 02/04/01 Added IP_SS_RTMSG for use by spenet                    rds
 ****************************************************************************/
#ifndef _SPINET_H
#define _SPINET_H

struct spf_ss_pb;

/* Driver Setstat Codes */

#define SS_SPINET_BASE  (SPF_PR_IP << 16)

/*--------------------------------------------------*/
/* IP Protocol Driver Codes                         */
/*--------------------------------------------------*/

#define IP_GS_IOCTL		(SS_SPINET_BASE + 1)
#define IP_SS_IOCTL		(SS_SPINET_BASE + 1)
#define IP_GS_SYMBOLS	(SS_SPINET_BASE + 2)
#define IP_SS_SYSCTL	(SS_SPINET_BASE + 3)
#define IP_INT_GETSTAT	(SS_SPINET_BASE + 4)
#define IP_INT_SETSTAT	(SS_SPINET_BASE + 5)
#define IP_GS_SOCKNAME	(SS_SPINET_BASE + 6)
#define IP_GS_PEERNAME	(SS_SPINET_BASE + 7)
#define IP_SS_RTMSG		(SS_SPINET_BASE + 8)

#define SPIPDESC		"/ip0"
#define SPTCPDESC		"/tcp0"
#define SPUDPDESC		"/udp0"

/*
** The #1 is for backwards compatibility. The 1 indicates to spip the new
** style socket library is being used.
*/
#define SPTCPIPDESC		"/ip0#1/tcp0"
#define SPUDPIPDESC		"/ip0#1/udp0"
#define SPRAWIPDESC		"/ip0#1/raw0"
#define SPROUTEDESC		"/ip0#1/route0"
#define SPLOCALDESC		"/ip0#1/local0"

/* structure for IP_INT_[GS]ETSTAT */
typedef struct {
		char *int_name;
		struct spf_ss_pb *spf_pb;
} ip_int_setstat;

typedef ip_int_setstat ip_int_getstat;

#endif /* _SPINET.H */
