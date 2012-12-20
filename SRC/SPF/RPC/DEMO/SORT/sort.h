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
 *  01 10/21/93 <--------- OS-9 NFS 1.3 Released ------------------->
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#define MAXSORTSIZE 64
#define MAXSTRINGLEN 64

typedef char *str;
bool_t xdr_str();


struct sortstrings {
	struct {
		u_int ss_len;
		str *ss_val;
	} ss;
};
typedef struct sortstrings sortstrings;
bool_t xdr_sortstrings();


#define SORTPROG ((u_long)22855)
#define SORTVERS ((u_long)1)
#define SORT ((u_long)1)
extern sortstrings *sort_1();

