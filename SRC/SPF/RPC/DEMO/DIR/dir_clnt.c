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
#include <time.h>
#include "dir.h"

static struct timeval TIMEOUT = { 25, 0 };

readdir_res *
readdir_1(argp, clnt)
	nametype *argp;
	CLIENT *clnt;
{
	static readdir_res res;

	bzero(&res, sizeof(res));
	if (clnt_call(clnt, READDIR, xdr_nametype, argp, xdr_readdir_res, &res, TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}

