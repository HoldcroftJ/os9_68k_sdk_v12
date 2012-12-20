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
 *  02 10/21/98 Added command line options                             rds
 *  03 10/22/98 Ported to Unix                                         rds
 *  04 09/12/01 Ported to Linux                                        GbG
 ****************************************************************************/

#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

#if defined(linux)
#include </usr/include/getopt.h>
#endif

#if defined(_OSK)||defined(_OS9000)
_asm("_sysedit: equ 4");

#include <modes.h>
extern char *optarg;
extern int *optind;
int getopt();
#define close _os_close
#endif

#define PORT 4433
#define GROUP 0xe10000ac
#define MESSAGE "This is a multicast test"

void usage(void);

u_char ttl;
short port;
u_char loop;
struct in_addr addr;
struct in_addr group;
int verbose;
char buf[4096];

#if defined(linux)
int main (int argc, char *argv[]);
int main (int argc, char *argv[])
#else
void main (int argc, char *argv[])
#endif
{
	int msock;
	int ch;
	struct sockaddr_in to;
	struct hostent *h;

	verbose = 0;
	ttl = 1;
	loop = 0;
	port = htons(PORT);
	addr.s_addr = htonl(INADDR_ANY);
	group.s_addr = htonl(GROUP);
	strcpy(buf, MESSAGE);

	while ((ch = getopt(argc, argv, "vlt:p:i:g:m:?")) != EOF) {
		switch(ch) {
			case 'v':
				verbose = 1;
				break;
			case 'l':
				loop = 1;
				break;
			case 't':
				ttl = atoi(optarg);
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
			case 'm':
				strcpy(buf, optarg);
				break;
			case '?':
			default:
				usage();
				exit(1);
		}
	}
				

	/*
	*********************************************************************
	** Open multicast socket
	*********************************************************************
	*/
	if (verbose)
		printf("Opening socket\n");

	if ((msock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		printf("Can't open multicast socket [%d]\n", errno);
		exit(errno);
	}


	/*
	*********************************************************************
	** Set requested TTL value
	*********************************************************************
	*/
	if (verbose)
		printf("Setting TTL to %d\n", ttl);

	if (setsockopt(msock, IPPROTO_IP, IP_MULTICAST_TTL,
					&ttl, sizeof(ttl)) < 0) {
		printf("Can't set multicast TTL to %d [%d]\n", ttl, errno);
		exit(errno);
	}

	/*
	*********************************************************************
	** Enable/Disable loopback reception
	*********************************************************************
	*/
	if (verbose)
		printf("Turning local loopback %s\n", loop? "on": "off");

	if (setsockopt(msock, IPPROTO_IP, IP_MULTICAST_LOOP,
					&loop, sizeof(loop)) < 0) {
		printf("Can't turn local loopback %s [%d]\n", loop? "on": "off", errno);
		exit(errno);
	}

	/*
	*********************************************************************
	** Select requested interface to send packet
	*********************************************************************
	*/
	if (verbose) {
		printf("Selecting interface %s\n", inet_ntoa(addr));
	}

	if (setsockopt(msock, IPPROTO_IP, IP_MULTICAST_IF,
					&addr, sizeof(addr)) < 0) {
		printf("Can't select interface %s\n", inet_ntoa(addr));
		exit(errno);
	}	


	/*
	*********************************************************************
	** Send packet
	*********************************************************************
	*/
	memset(&to, 0, sizeof(to));
#if !defined(linux)
	to.sin_len = sizeof(to);
#endif
	to.sin_family = AF_INET;
	to.sin_port = port;
	to.sin_addr.s_addr = group.s_addr;

	if (verbose)
		printf("Sending message to %s\n", inet_ntoa(group));

	if (sendto(msock, buf, strlen(buf) + 1, 0,
			(struct sockaddr *)&to, sizeof(to)) == -1){
		printf("Can't send multicast data\n");
		exit(errno);
	}

	if (verbose)
		printf("Message sent\n");


	close(msock);
	exit(0);
}

void usage(void)
{
	printf("Syntax:   msend [<opts>]\n");
	printf("Function: Send multicast packets\n");
	printf("Options:\n");
	printf("    [-v] [-l] [-t ttl] [-p port] [-g group] [-i interface] [-m message]\n");
	printf("\n");
	printf("    -v   Enable verbose mode (Default: off)\n");
	printf("    -l   Enable loopback reception of packets (Default: off)\n");
	printf("    -t   Set TTL of output packets (Default: 1)\n");
	printf("    -p   Set port of output packets (Default: %d)\n", PORT);
	printf("    -g   Select destination group (Default: %s)\n", inet_ntoa(group));
	printf("    -i   Select outgoing interface (Default: route table lookup of group)\n");
	printf("    -m   Select message to send (Default: %s)\n", MESSAGE);
}
