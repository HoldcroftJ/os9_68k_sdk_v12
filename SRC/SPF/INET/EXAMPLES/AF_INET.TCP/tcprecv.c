/****************************************************************************
 *  Edition History:
 *  #   Date    Comments                                                By
 * --- -------- -----------------------------------------------------  ----
 *   0 ??/??/?? Birth                                                   ???
 *   8 07/25/96 Fixed endian problems so it works on 386                rds
 *   9 05/02/97 Converted include files to BSD headers                  pjw
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  10 01/13/98 Clean up code, fix bus error if no parameters specified rds
 *  11 03/30/98 Removed invalid mode (S_IGWRITE) for OS9000             rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  12 02/20/01 Added nonblocking mode                                  mgh
 ****************************************************************************/
_asm("_sysedit: equ 12");

/* <<<<<<<<<<<<<<<<<<<<<<<<<<< tcprecv.c >>>>>>>>>>>>>>>>>>>>>>>>>> */
/*
**	Syntax:    tcprecv <file>
**	Function:  receives a file across a stream (TCP) socket
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

#define INIT		77		/* commands */
#define DATA		78
#define END			79
#define FMODE		S_IWRITE

#if defined(_OSK)
	#define _os_sleep(t,s) _os9_sleep(t)
#else
    signal_code sig = 0;
#endif

/* Global Variables */
char msgbuf[20480];


/* Function Prototypes */
void main(int argc, char* argv[], char* envp[]);
int ioctl(unsigned int, unsigned int, caddr_t);

void main(int argc, char* argv[], char* envp[])
{
	int s;
	int sx;
	int size;
	int flags = 0;
	int totbytes = 0;
	int noblock = 0;
	path_id ofile;
	u_int32 vflag = 0;
	u_int32 count = 1;
	u_int32 tics;
	struct sockaddr_in ls_addr;
	struct sockaddr_in to;

	if ((argc <= 1) || (argc > 4) || (argv[1][0] == '-')) {
		fprintf(stderr,"tcprecv <file> [nonblocking [verbose]]\n");
		exit(0);
	}

	if ((errno = _os_create(argv[1], FMODE, &ofile,
								S_IREAD|S_IWRITE)) != SUCCESS) {
		fprintf(stderr, "can't open file '%s'\n",argv[1]);
		exit(errno);
	}

 	if ((sx = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr,"can't open socket\n", errno);
		_os_close(ofile);
		exit(errno);
	}
	   
	memset(&ls_addr, 0, sizeof(ls_addr));
	ls_addr.sin_family = AF_INET;
	ls_addr.sin_port =  htons(PORT_NUM);
	ls_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sx, (struct sockaddr*)&ls_addr, sizeof ls_addr) == -1) {
		fprintf(stderr,"can't bind socket\n");
		_os_close(sx);
		_os_close(ofile);
		exit(errno);
	}

	if (listen(sx, 1) < 0) {
		fprintf(stderr,"tcp_listen - failed!\n");
		_os_close(sx);
		_os_close(ofile);
		exit(errno);
	}

	size = sizeof(struct sockaddr_in);
	if ((s = accept(sx, (struct sockaddr*)&to, &size)) < 0) {
		fprintf(stderr, "can't accept\n");
		_os_close(s);
		_os_close(sx);
		_os_close(ofile);
		exit(errno);
	}
	_os_close(sx);

	printf("connected to %s port %d\n", inet_ntoa(to.sin_addr),
			ntohs(to.sin_port));

	if (argc >= 3) {
		printf("using non-blocking sockets\n");
		noblock = IO_ASYNC;
		if (ioctl(s, FIONBIO, (caddr_t)&noblock)) {
			fprintf(stderr, "can't set socket nonblocking\n");
			_os_close(ofile);
			_os_close(s);
			_os_close(sx);
			exit(errno);
		}
	} else {
		printf("using blocking sockets\n");
	}
	if (argc == 4) vflag = 1 ;

	do {
		count = sizeof(msgbuf);
		/*
		** Choose either the _os_read or recv line below as
		**   desired to choose input function
		** recv() sets errno
		*/
/*		if ((errno = _os_read(s,msgbuf,&count)) != SUCCESS) { */
		if ((count = recv(s,msgbuf,count,flags)) == (u_int32) -1) { 
	
			if (errno == EOS_EOF) {
				break; /*at end of file*/
			}
			if (noblock & (errno == EWOULDBLOCK)) {
				if (vflag)
					printf("Total Bytes read: %d. Read would block, sleeping...",
						totbytes);
				tics = 10;
				_os_sleep(&tics, &sig);	
				count = 1;					/* force one more interation */
				if (vflag) printf("trying read again.\n");
			} else {
				fprintf(stderr, "can't recv (cnt=%d)\n", count);
				exit(errno);
			}
		} else if (count > 0) {
			if ((errno = _os_write(ofile, msgbuf, &count)) != SUCCESS) {
				fprintf(stderr, "can't write output\n");
				exit(errno);
			}
			totbytes += count;
		}
	} while (count);

	_os_close(s);
	_os_close(ofile);
	printf("read %d bytes\n", totbytes);

	exit(0);
}
