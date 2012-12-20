/****************************************************************************
 *  Edition History:
 *  #   Date    Comments                                                By
 * --- -------- ------------------------------------------------------  ---
 *   0 ??/??/?? Birth                                                   ???
 *   8 01/27/97 Updated for SPF nonblocking mode                        pjw
 *   9 02/14/97 Removed bind code for SPF                               tmd
 *  10 03/24/97 Removed ISP support                                     rds
 *  11 05/02/97 Converted include files to BSD headers                  pjw
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  12 01/13/98 Rewrote non-blocking code                               rds
 *  13 03/30/98 Removed invalid mode (S_IGREAD) for OS9000              rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *	14 02/20/01 Added ability to use send or write via commented out    mgh
 *                selection of code.  (Shipped as send)
 ****************************************************************************/
 _asm("_sysedit: equ 14");

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<< tcpsend.c >>>>>>>>>>>>>>>>>>>>>>>>>>> */
/*
p**	Syntax:    tcpsend <hostname> <file> <blockflag>
**	Function:  sends a file across a stream (TCP) socket
*/

/* Header Files */

#include <stdio.h>
#include <stdlib.h>
#include <const.h>
#include <signal.h>
#include <types.h>
#include <errno.h>
#include <string.h>
#include <modes.h>
#include <sg_codes.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <SPF/spf.h>
#include <UNIX/ioctl.h>

/* Macro Definitions */

#define PORT_NUM	27000
#define MAX_LOOPS	200
#define FMODE		S_IREAD

#if defined(_OSK)
	#define _os_sleep(t,s) _os9_sleep(t)
#else
	signal_code sig = 0;
#endif

/* Type Definitions */
struct data {
	int code, count;
	char data[512];
};

/* Global Variables */
struct sockaddr_in ls_addr;
/*char msgbuf[20480]; */
char msgbuf[64000];
char *ptr;

/* Function Prototypes */
void main(int argc, char* argv[], char* envp[]);
int ioctl(unsigned int, unsigned int, caddr_t);

void main(int argc, char* argv[], char* envp[])
{
	int s;
	int flags = 0; 
	int totbytes = 0;
	int noblock = 0;
	path_id ifile;
	u_int32 vflag = 0;
	u_int32 count;
	u_int32 wcount;
	u_int32 wsize;
	u_int32 tries;
	u_int32 tics;
	struct hostent *host;

	if ((argc <= 1) || (argc > 5) || (argv[1][0] == '-')) {
		fprintf(stderr, "tcpsend <hostname> <file> [nonblocking [verbose]]\n");
		exit(0);
	}

	memset(&ls_addr, 0, sizeof(ls_addr));

	if ((ls_addr.sin_addr.s_addr = inet_addr(argv[1])) == (u_long)-1){
		if ((host = gethostbyname(argv[1])) != NULL){
			memcpy(&ls_addr.sin_addr.s_addr, host->h_addr,
												host->h_length);
			ls_addr.sin_family = host->h_addrtype;
		} else {
			fprintf(stderr, "can't resolve name '%s'\n", argv[1]);
			exit(errno);
		}
		endhostent();
	} else {
		ls_addr.sin_family = AF_INET;
	}

	ls_addr.sin_port =  htons(PORT_NUM);

	if ((errno = _os_open(argv[2],FMODE,&ifile)) != SUCCESS) {
		fprintf(stderr, "can't open file '%s'\n", argv[2]);
		exit(errno);
	}

 	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		_os_close(ifile);
		exit(1);
	}

	{
		/* Customer settings */
		int sockvar = 16384;
		if (setsockopt(s, SOL_SOCKET, SO_SNDBUF, &sockvar, sizeof(int)))
			fprintf(stderr,"SI: err setting socket send buffer size");
		if (setsockopt(s, SOL_SOCKET, SO_RCVBUF, &sockvar, sizeof(int)))
			fprintf(stderr,"SI: err setting socket receive buffer size");
		sockvar = 1;
		if (setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &sockvar, sizeof(int)))
			fprintf(stderr,"SI: error setting socket TCP NODELAY option");
	}

	
	if (argc >= 4) {
		printf("using non-blocking sockets\n");
		noblock = IO_ASYNC;
		if (ioctl(s, FIONBIO, (caddr_t)&noblock)) {
			fprintf(stderr, "can't set socket nonblocking\n");
			_os_close(ifile);
			_os_close(s);
			exit(errno);
		}
	} else {
		printf("using blocking sockets\n");
	}

	if (argc == 5) vflag = 1;

	if (noblock) {
		/*
		** Non-blocking connect
		*/
		tries = MAX_LOOPS;
		while (tries) {
			if (connect(s, (struct sockaddr*)&ls_addr,
										sizeof(ls_addr)) == -1) {
				if (errno == EISCONN) {
					break;
				}
				if (errno == EINVAL) {
					int error, len;
					len = sizeof(error);
					if (getsockopt(s, SOL_SOCKET, SO_ERROR, &error,
														&len) < 0) {
						error = EINVAL;
					}
					errno = error;
					perror("connect");
					_os_close(s);
					_os_close(ifile);
					exit(1);
				}
				tics = 10;
				_os_sleep(&tics, &sig);
			} else {
				break;
			}
			tries--;
		}

		if (tries == 0){
			errno = ETIMEDOUT;
			perror("connect");
			_os_close(s);
			_os_close(ifile);
			exit(1);
		}
	} else {
		/*
		** Blocking connect
		*/
		if (connect(s, (struct sockaddr *)&ls_addr,
										sizeof(ls_addr)) == -1) {
			perror("connect");
			_os_close(s);
			_os_close(ifile);
			exit(1);
		}
	}

	printf("Connection established\n");
	printf("Sending file '%s'...\n", *argv);

	if (noblock) {
		/*
		** Non-blocking send
		*/
		count = sizeof(msgbuf);
		while ((errno = _os_read(ifile,msgbuf,&count)) == SUCCESS) {
			wcount = 0;
			ptr = msgbuf;
			while (wcount < count){
				wsize = count - wcount;

				/*
				** Choose either the _os_write or the send line below
				**   as desired to choose output method
				** send() sets< errno
				*/
/*				if ((errno = _os_write(s,ptr,&wsize)) != SUCCESS) { */
				if ((wsize = send(s,ptr,wsize,flags)) == (u_int32)-1) {
					if (errno != EWOULDBLOCK) {
						fprintf(stderr, "socket write error\n");
						_os_close(s);
						_os_close(ifile);
						exit(errno);
					} else {
						if (vflag) printf("Write would block, sleeping...");
						tics = 10;
						_os_sleep(&tics,&sig);
						if (vflag) printf("trying write again.\n");
					}
				} else {
					wcount += wsize;
					ptr += wsize;
					if (vflag) printf("wrote %d Bytes\n", wsize);
				}
			}
			totbytes += count;
			count = sizeof(msgbuf);
		}
		if (errno != EOS_EOF){
			fprintf(stderr, "read error on file\n");
			exit(errno);
		}

	} else {
		/*
		** Blocking send
		*/
		count = sizeof(msgbuf);
		while ((errno = _os_read(ifile, msgbuf, &count)) == SUCCESS) {
			wcount = count;
			ptr = msgbuf;
			/*
			** Choose either the _os_write or the send line below
			**   as desired to to choose output method
			** send() sets errno
			*/
/*			while ((errno = _os_write(s, ptr, &count)) == SUCCESS){ */
			while ((count = send(s, ptr, count, flags)) != (u_int32)-1){ 
				totbytes += count;
				if (count == wcount) {
					break;
				}
				/*
				** For some reason all the data was not written,
				** loop around and try to write the rest.
				*/
				ptr += count;
				count = wcount - count;
				wcount = count;
			}
			if (errno){
				fprintf(stderr, "socket write error\n");
				_os_close(s);
				_os_close(ifile);
				exit(errno);
			}
			count = sizeof(msgbuf);
		}
		if (errno != EOS_EOF){
			fprintf(stderr, "read error on file\n");
			exit(errno);
		}
	}

	_os_close(s);
	_os_close(ifile);
	printf("sent %u bytes\n", totbytes);

	exit(0);
}
