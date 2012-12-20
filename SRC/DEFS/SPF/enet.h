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
 *  01 ??/??/?? Created                                                ???
 *     02/21/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *     05/07/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#if !defined(_ENET_H)
#define _ENET_H

/*  Driver Setstat Code     */

#define SS_ENET_BASE  (SPF_PR_ENET << 16)


/*--------------------------------------------------*/
/* Enet Protocol Driver Codes (range = 51 - 75)     */
/*--------------------------------------------------*/
/* used in entry.c, main.c 							*/

#define ENET_GS_STATS (SS_ENET_BASE + 53)

typedef struct  s_enet_stat_pb {
    u_int32     arp_count,
                rarp_count,
                ip_count,
				good_ip_count,
				unknown;
}enet_stat_pb, *Enet_stat_pb;

#endif /* _ENET_H */
