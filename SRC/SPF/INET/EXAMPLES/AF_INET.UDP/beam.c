/****************************************************************************
 * Edition History:
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *   0 ??/??/?? Birth                                                  ???
 *   8 05/21/96 Fixed endian problems so it works on 386               rds
 *   9 07/18/96 Another endian problem fixed                           rds
 *  10 10/18/96 Declared buffer static to "avoid" stack overflow       tmd
 *  11 03/31/97 Converted to SPF BSD 4.4                               rds
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  12 12/15/97 netdb.h is now in SPF/BSD instead of INET              rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/
_asm("_sysedit: equ 12");

/* <<<<<<<<<<<<<<<<<<<<<<< beam.c >>>>>>>>>>>>>>>>>>>> */
/*
**	Syntax:    beam <target> <count> [<port>]
**	Function:  sends packets over a datagram socket
*/

/* Header Files */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <modes.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/* Macro Definitions */

#define PKT_SIZE	1000		/* packet size */
#define PORT		20000		/* udp port number */

#define START		1			/* packet types */
#define NORMAL		2
#define END			3


/* Type Definitions */

struct packet {
	u_int32		type;
	u_int32		size;
	u_int32		count;
	char		buf[PKT_SIZE - 12];
} packet, *Packet;


/* main() : initial program entry point */
void main(int argc, char* argv[], char* envp[])
{
	int s;
	int i;
	int count;
	struct hostent *host;
	struct sockaddr_in sockname;
	static struct packet pkt;

	/* check for proper number of arguments */
	if ((argc < 3) || (argv[1][0] == '-')) {
		printf("usage: beam <hostname|ip-address> <count> [<port>]\n");
		exit(0);
	}

	/* get number of packets to beam */
	count = atoi(argv[2]);

	/* open up datagram (UDP) socket */
	memset(&sockname, 0, sizeof(sockname));
	sockname.sin_family = AF_INET;
	sockname.sin_port = 0;
	sockname.sin_addr.s_addr = INADDR_ANY;
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		fprintf(stderr, "beam: socket call failed\n");
		exit(errno);
	}

	/* bind socket (let system pick our port number) */
	if (bind(s, (struct sockaddr*)&sockname, sizeof(sockname)) < 0) {
		fprintf(stderr, "bind failed to host\n");
		_os_close(s);
		exit(errno);
	}

	/* get information concerning the host we'd like to beam to */
	sockname.sin_port = 0;
	sockname.sin_addr.s_addr = INADDR_ANY;
	if ((host = gethostbyname(argv[1])) != (struct hostent *)0) {
		sockname.sin_family = host->h_addrtype;
		memcpy(&sockname.sin_addr.s_addr, host->h_addr, 4);
	} else {
		u_int32 addr = inet_addr(argv[1]);
		sockname.sin_family = AF_INET;
		memcpy(&sockname.sin_addr.s_addr, &addr, 4);
	}
	endhostent();
	if (argc > 3) {
		sockname.sin_port = htons(atoi(argv[3]));
	} else {
		sockname.sin_port = htons(PORT);
	}

	/* set up socket for send */

	#ifdef USE_CONNECT
		/* "connect" UDP socket -- we're only going to talk to this host */
		if (connect(s, (struct sockaddr *)&sockname, sizeof(sockname)) < 0) {
			fprintf(stderr, "beam: cannot connect\n");
			_os_close(s);
			exit(errno);
		}
	#endif

	printf("beaming...\n");

	/* set up packets for transfer and transfer them all */
	pkt.size = htonl(PKT_SIZE);
	for (i = 0; i <= count; i++) {

		if (i == 0) {
			pkt.type = htonl(START);
		} else if (i >= count) {
			pkt.type = htonl(END);
		} else {
			pkt.type = htonl(NORMAL);
		}
		pkt.count = htonl(i);

		/* send data to target */
		#ifdef USE_CONNECT
			if (send(s, &pkt, ntohl(pkt.size), 0) < 0) {
				fprintf(stderr, "beam: send failed\n");
				_os_close(s);
				exit(errno);
			}
		#else
			if (sendto(s, (char*)&pkt, ntohl(pkt.size), 0,
						(struct sockaddr*)&sockname, sizeof(sockname)) < 0) {
				fprintf(stderr, "beam: sendto failed\n");
				_os_close(s);
				exit(errno);
			}
		#endif
	}

	/* cleanup and exit */
	_os_close(s);
	exit(0);

}	/* end of main */
