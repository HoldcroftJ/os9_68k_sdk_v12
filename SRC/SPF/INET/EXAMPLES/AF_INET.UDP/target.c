/****************************************************************************
 * Edition History:
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *   0 ??/??/?? Birth                                                  ???
 *   8 05/21/96 Fixed endian problems so it works on 386               rds
 *   9 10/18/96 Declared pkt static to "avoid" stack overflow          tmd
 *  10 03/31/97 Converted to SPF BSD 4.4                               rds
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  11 12/15/97 Removed timing information                             rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/
_asm("_sysedit: equ 11");

/* <<<<<<<<<<<<<<<<<<<<<<< target.c >>>>>>>>>>>>>>>>>>>> */
/*
**	Syntax:		target
**	Function:	receives packets from beam over a datagram
**				socket and displays number of packets
*/

/* Header Files */

#include <stdio.h>
#include <stdlib.h>
#include <types.h>
#include <errno.h>
#include <string.h>
#include <modes.h>
#include <ctype.h>

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
	int count;
	int namelen;
	int packetsrecv;
	u_int32 bytesrecv;
	static struct packet pkt;
	struct sockaddr_in name;

	if ((argc < 1) || (argc > 2) || ((argc == 2) && (!isdigit(argv[1][0])))) {
		printf("usage: target [<port>]\n");
		exit(0);
	}

	/* open up datagram (UDP) socket */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		fprintf(stderr, "target: socket failed\n");
		exit(errno);
	}

	/* bind socket (pick proper "well-known" port number) */
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = INADDR_ANY;
	if (argc >= 2) {
		name.sin_port = htons(atoi(argv[1]));
	} else {
		name.sin_port = htons(PORT);
	}
	if (bind(s, (struct sockaddr*)&name, sizeof(name)) == -1) {
		fprintf(stderr, "target: bind failed to port '%d'\n",ntohs(name.sin_port));
		_os_close(s);
		exit(errno);
	}

	printf("Waiting for packets...\n");

	/* wait for start packet */
	while (1) {

		/* get a packet (and find out who sent it to us) */
		namelen = sizeof(name);
		if ((count = recvfrom(s, (char*)&pkt, sizeof(pkt), 0,
								(struct sockaddr*)&name,&namelen)) == -1) {
			fprintf(stderr, "target: recv failed\n");
			_os_close(s);
			exit(errno);
		}

		if (pkt.type != htonl(START)){
			printf("out of sequence packet received\n");
			continue;
		} else {
			break;
		}
	}

	bytesrecv = packetsrecv = 0;

	/* loop until all packet are receved */
	printf("Begin transfer\n");
	do {
		namelen = sizeof(name);
		if ((count = recvfrom(s, (char*)&pkt, sizeof(pkt), 0,
							(struct sockaddr*)&name,&namelen)) == -1) {
			fprintf(stderr, "target: recv failed\n");
			_os_close(s);
			exit(errno);
		}
		bytesrecv += count;
		packetsrecv++;
	} while (pkt.type == ntohl(NORMAL));

	/* if we didn't get and END packet, print error */
	if (pkt.type != ntohl(END)) {
		printf("expected an END packet\n");
	}

	/* print out summary */
	printf("Transfer complete\n");
	printf("    Packets received:  %d\n",packetsrecv);
	printf("    Bytes received:  %d\n",bytesrecv);

	/* cleanup and exit */
	_os_close(s);
	exit(0);

}	/* end of main */
