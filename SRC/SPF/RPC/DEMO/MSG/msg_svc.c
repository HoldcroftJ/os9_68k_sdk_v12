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
 *  02 07/18/97 Added help syntax                                      apw
 *  13 12/09/97 Removed compiler warnings                              rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <RPC/rpc.h>
#include "msg.h"

static void messageprog_1();

main(int argc)
{
	SVCXPRT *transp;

	if (argc > 1) {
		fprintf(stderr,"Syntax:   msgd\n");
		fprintf(stderr,"Function: msgd is the OS-9/OS9000 server for remote messaging\n");
		fprintf(stderr,"Options:  none.\n");
		exit(1);
	}

	pmap_unset(MESSAGEPROG, MESSAGEVERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf(stderr, "cannot create udp service.\n");
		exit(1);
	}
	if (!svc_register(transp, MESSAGEPROG, MESSAGEVERS, messageprog_1, IPPROTO_UDP)) {
		fprintf(stderr, "unable to register (MESSAGEPROG, MESSAGEVERS, udp).\n");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf(stderr, "cannot create tcp service.\n");
		exit(1);
	}
	if (!svc_register(transp, MESSAGEPROG, MESSAGEVERS, messageprog_1, IPPROTO_TCP)) {
		fprintf(stderr, "unable to register (MESSAGEPROG, MESSAGEVERS, tcp).\n");
		exit(1);
	}
	svc_run();
	fprintf(stderr, "svc_run returned\n");
	exit(1);
}

static void
messageprog_1(rqstp, transp)
	struct svc_req *rqstp;
	SVCXPRT *transp;
{
	union {
		char *printmessage_1_arg;
	} argument;
	char *result;
	bool_t (*xdr_argument)(), (*xdr_result)();
	char *(*local)();

	switch (rqstp->rq_proc) {
	case NULLPROC:
		svc_sendreply(transp, xdr_void, NULL);
		return;

	case PRINTMESSAGE:
		xdr_argument = xdr_wrapstring;
		xdr_result = xdr_int;
		local = (char *(*)()) printmessage_1;
		break;

	default:
		svcerr_noproc(transp);
		return;
	}
	bzero(&argument, sizeof(argument));
	if (!svc_getargs(transp, xdr_argument, &argument)) {
		svcerr_decode(transp);
		return;
	}
	result = (*local)(&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, xdr_result, result)) {
		svcerr_systemerr(transp);
	}
	if (!svc_freeargs(transp, xdr_argument, &argument)) {
		fprintf(stderr, "unable to free arguments\n");
		exit(1);
	}
}

