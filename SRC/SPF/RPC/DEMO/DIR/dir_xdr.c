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
 *     04/22/96 <--------- NFS 1.4 Server Release ------------------>
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#include <RPC/rpc.h>
#include "dir.h"


bool_t
xdr_nametype(xdrs, objp)
	XDR *xdrs;
	nametype *objp;
{
	if (!xdr_string(xdrs, objp, MAXNAMELEN)) {
		return (FALSE);
	}
	return (TRUE);
}


bool_t
xdr_namelist(xdrs, objp)
	XDR *xdrs;
	namelist *objp;
{
	if (!xdr_pointer(xdrs, (char **)objp, sizeof(struct namenode), xdr_namenode)) {
		return (FALSE);
	}
	return (TRUE);
}


bool_t
xdr_namenode(xdrs, objp)
	XDR *xdrs;
	namenode *objp;
{
	if (!xdr_nametype(xdrs, &objp->name)) {
		return (FALSE);
	}
	if (!xdr_namelist(xdrs, &objp->next)) {
		return (FALSE);
	}
	return (TRUE);
}


bool_t
xdr_readdir_res(xdrs, objp)
	XDR *xdrs;
	readdir_res *objp;
{
	if (!xdr_int(xdrs, &objp->errno)) {
		return (FALSE);
	}
	switch (objp->errno) {
	case 0:
		if (!xdr_namelist(xdrs, &objp->readdir_res_u.list)) {
			return (FALSE);
		}
		break;
	}
	return (TRUE);
}
