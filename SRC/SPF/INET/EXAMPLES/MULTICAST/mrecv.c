/****************************************************************************
 *           Copyright 1989-2001 by Microware Systems Corporation           *
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
 *  01 07/14/98 Created                                                rds
 *  02 10/21/98 Added usage help, cleaned up interface                 rds
 *  03 10/22/98 Ported to Unix                                         rds
 *  04 12/08/98 Fix -p option to accept a parameter                    rds
 *     12/08/98 Now set SO_REUSEPORT socket option                     rds
 *  05 09/12/01 Ported to Linux                                        GbG
 ****************************************************************************/

#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>

#if defined(linux)
#include </usr/include/getopt.h>
/* SO_REUSEPORT is not yet support under Linux. But is planned. */
#if !defined(SO_REUSEPORT)
#define SO_REUSEPORT SO_REUSEADDR
#endif
#endif

#if defined(_OSK)||defined(_OS9000)
_asm("_sysedit: equ 5");

#include <modes.h>
#include <cglob.h>
#include <const.h>
extern char *optarg;
extern int *optind;
int getopt();
#define close _os_close
#else
#include <arpa/inet.h>
#endif

#define PORT 4433
#define GROUP 0xe10000ac

void usage(void);

short port;
int verbose;
int done = 0;
struct in_addr addr;
struct in_addr group;

char buf[4096];

#if defined(_OSK)||defined(_OS9000)
void sighand(int sig)
{
	done = 1;
	_os_rte();
}
#endif

#if defined(linux)
int main (int argc, char *argv[]);
int main (int argc, char *argv[])
#else
void main (int argc, char *argv[])
#endif
{
	int nbytes;
	int msock;
	int size;
	int ch;
	int on = 1;
	struct ip_mreq m;
	struct sockaddr_in from;
	struct hostent *h;

#if defined(_OSK)||defined(_OS9000)
	if (_os_intercept(sighand, _glob_data) != SUCCESS){
		printf("Can't install signal handler\n");
		exit(errno);
	}
#endif

	verbose = 0;
	port = htons(PORT);
	addr.s_addr = htonl(INADDR_ANY);
	group.s_addr = htonl(GROUP);

	while ((ch = getopt(argc, argv, "vp:g:i:?")) != EOF) {
		switch(ch) {
			case 'v':
				verbose = 1;
				break;
			case 'p':
				port = htons(atoi(optarg));
				break;
			case 'i':
				if ((h = gethostbyname(optarg)) == NULL) {
					printf("Can not resolve %s\n", optarg);
					exit(errno);
				}
				memcpy(&addr, h->h_addr_list[0], h->h_length);
				break;
			case 'g':
				if ((h = gethostbyname(optarg)) == NULL) {
					printf("Can not resolve %s\n", optarg);
					exit(errno);
				}
				memcpy(&group, h->h_addr_list[0], h->h_length);
				break;
			case '?':
			default:
				usage();
				exit(1);
		}
	}

	/*
	***********************************************************************
	** Open and prepare multicast socket
	***********************************************************************
	*/
	if ((msock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		printf("Can't open multicast socket [%d]\n", errno);
		exit(errno);
	}

	if (setsockopt(msock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1){
		printf("Can't set SO_REUSEADDR socket option [%d]\n", errno);
		exit(errno);
	}

	if (setsockopt(msock, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on)) == -1){
		printf("Can't set SO_REUSEPORT socket option [%d]\n", errno);
		exit(errno);
	}

	/*
	***********************************************************************
	** Join multicast group
	***********************************************************************
	*/
	m.imr_multiaddr.s_addr = group.s_addr;
	m.imr_interface.s_addr = addr.s_addr;
	if (verbose) {
		printf("Joining multicast group %s", inet_ntoa(group));
		printf(" on interface %s\n", inet_ntoa(addr));
	}

	if (setsockopt(msock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
					&m, sizeof(m)) < 0) {
		strcpy(buf, inet_ntoa(group));
		printf("Can't join multicast group %s on interface %s [%d]\n",
						buf, inet_ntoa(addr), errno);
		exit(errno);
	}	


	/*
	***********************************************************************
	** Bind multicast socket
	***********************************************************************
	*/
	memset((void *)&from, 0, sizeof(from));
#if !defined(linux)
	from.sin_len = sizeof(addr);
#endif
	from.sin_family = AF_INET;
	from.sin_port = port;
	from.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(msock, (struct sockaddr *)&from, sizeof(from)) == -1){
		printf("Can't bind socket [%d]\n", errno);
		exit(errno);
	}

	/*
	***********************************************************************
	** Receive packets
	***********************************************************************
	*/
	memset((void *)&from, 0, sizeof(from));
	size = sizeof(from);
	printf("Waiting for incoming multicasts\n");
	while (!done && (nbytes = recvfrom(msock, buf, sizeof(buf), 0,
			(struct sockaddr *)&from, &size)) != -1){
		printf("Received %d byte multicast from %s\n", nbytes, inet_ntoa(from.sin_addr));
		if (verbose)
			printf("   Text: [%s]\n", buf);
		size = sizeof(from);
	}

	if( !done) {
		printf("recvfrom error [%d]\n", errno);
		exit(errno);
	}

	/*
	***********************************************************************
	** Leave multicast group
	***********************************************************************
	*/
	m.imr_multiaddr.s_addr = group.s_addr;
	m.imr_interface.s_addr = addr.s_addr;
	if (verbose) {
		printf("Dropping multicast group %s", inet_ntoa(group));
		printf(" on interface %s\n", inet_ntoa(addr));
	}

	if (setsockopt(msock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
					&m, sizeof(m)) < 0) {
		strcpy(buf, inet_ntoa(group));
		printf("Error leaving multicast group %s\n", inet_ntoa(group));
		exit(errno);
	}

	close(msock);
	exit(0);
}

void usage(void)
{
	printf("Syntax:   mrecv [<opts>]\n");
	printf("Function: Receive multicast packets\n");
	printf("Options:\n");
	printf("    [-v] [-p port] [-g group] [-i interface]\n");
	printf("\n");
	printf("    -v   Enable verbose mode (Default: off)\n");
	printf("    -p   Set port for selecting incoming packets (Default: %d)\n", PORT);
	printf("    -g   Select destination group (Default: %s)\n", inet_ntoa(group));
	printf("    -i   Select receiving interface (Default: route table lookup of group)\n");
}
