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
 *     10/21/93 <--------- OS-9 NFS 1.3 Released ------------------->
 *     06/03/95 Corrected first param of xdr_array by casting it       sr
 *     04/22/96 <--------- NFS 1.4 Server Release ------------------>
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#include <RPC/rpc.h>
#include "sort.h"


bool_t
xdr_str(xdrs, objp)
	XDR *xdrs;
	str *objp;
{
	if (!xdr_string(xdrs, objp, MAXSTRINGLEN)) {
		return (FALSE);
	}
	return (TRUE);
}

bool_t
xdr_sortstrings(xdrs, objp)
	XDR *xdrs;
	sortstrings *objp;
{
	if (!xdr_array(xdrs, (caddr_t *)&objp->ss.ss_val, (u_int *)&objp->ss.ss_len, MAXSORTSIZE, sizeof(str), xdr_str)) {
		return (FALSE);
	}
	return (TRUE);
}


