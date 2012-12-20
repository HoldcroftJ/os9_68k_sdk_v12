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
#include "msg.h"

main(argc, argv)
	int argc;
	char *argv[];
{
	CLIENT *cl;
	int *result;
	char *server;
	char *message;


	if (argc != 3) {
		fprintf(stderr,"Syntax:   rmsg host message\n");
		fprintf(stderr,"Function: send message to remote host\n");
		fprintf(stderr,"Options:  none.\n");
		exit(1);
        }

	server = argv[1];
	message = argv[2];

	cl = clnt_create(server, MESSAGEPROG, MESSAGEVERS, "tcp");
	if (cl == NULL) {
		 clnt_pcreateerror(server);
		 exit(1);
	}

	result = printmessage_1(&message, cl);
	if (result == NULL) {
		clnt_perror(cl, server);
		exit (1);
	}

	if (*result == 0) {
		fprintf(stderr, "%s: %s couldn't print your message\n",
			argv[0], server);
		exit(1);
	}

	printf("Message delivered to %s!\n", server);
	exit (0);
}

