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
 *  02 07/18/97 Fixed help syntax.                                     apw
 *  13 12/09/97 Removed compiler warnings                              rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

_asm("_sysedit: equ 13");

#include <stdio.h>
#include <stdlib.h>
#include <RPC/rpc.h>
#include "sort.h"

int32 callrpc(char *, int, int, int, xdrproc_t, void *, xdrproc_t, void *);

main(argc, argv)
	int argc;
	char **argv;
{
	char *machinename;
	struct sortstrings args, res;
	int i;

	if (argc < 3) {
		fprintf(stderr,"Syntax:   rsort machinename [s1 ...]\n");
		fprintf(stderr,"Function: sort strings on remote host\n");
		fprintf(stderr,"Options:  none.\n");
		exit(1);
	}
	machinename = argv[1];
	args.ss.ss_len = argc - 2;     /* substract off progname, machinename */
	args.ss.ss_val = &argv[2];
	res.ss.ss_val = (char **)NULL;

	if ((i = callrpc(machinename, SORTPROG, SORTVERS, SORT,
	    xdr_sortstrings, &args, xdr_sortstrings, &res)))
	{
	    fprintf(stderr, "%s: call to sort service failed. ", argv[0]);
	    clnt_perrno((enum clnt_stat)i);
	    fprintf(stderr, "\n");
	    exit(1);
	}

	for (i = 0; i < res.ss.ss_len; i++) {
		printf("%s\n", res.ss.ss_val[i]);
	}

	/* should free res here */
	exit(0);
}

