/****************************************************************************
 *  ID:		@(#) sort.x 1.3@(#)
 *  Date:	4/5/98
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
 *     04/22/96 <--------- NFS 1.4 Server Release ------------------>
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

const MAXSORTSIZE  = 64;
const MAXSTRINGLEN = 64;

typedef	string  str<MAXSTRINGLEN>;  /* the string itself */

struct sortstrings {
    str ss<MAXSORTSIZE>;
};

program SORTPROG {
    version SORTVERS {
        sortstrings SORT(sortstrings) = 1;
    } = 1;
} = 22855;
