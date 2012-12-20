/****************************************************************************
 *  SPF example/test program                                                *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *   1 03/09/95 Created (from SPF:sptest)                              bat
 *   2 01/08/96 Updated and removed compiler warnings.                 jmh
 *     06/27/97 <-------------- SPF v2.2 --------------------->        taka
 *     03/25/98 Removed additional compiler warnings                   taka
 *              <-------------- SPF v2.2.1 ------------------->        taka
 *   4 07/19/98 <-------------- SPF v2.2.2 ------------------->        taka
 *   5 02/11/00 Added support for testing X.25                         rds
 ****************************************************************************/

_asm("_sysedit: equ 5");

/*
**	Header Files
*/

#include <stdio.h>
#include <stdlib.h>
#include <types.h>
#include <process.h>
#include <ctype.h>
#include <const.h>
#include <errno.h>
#include <modes.h>
#include <signal.h>
#include <string.h>
#include <module.h>
#include <SPF/spf.h>

int sleep(unsigned);


/*
**	Global Variables
*/
u_char buf[10000] = {0};
u_int32 COUNT = 1;
u_int32 BUFSZ = 100;
path_id path = 0;


/*
**	Signal Handler
*/
void sighand(int sig)
{
	switch (sig) {
		case SIGINT :
		case SIGQUIT :
			fprintf(stderr,"Termination signal received\n");
			_os_close(path);
			_os_exit(SUCCESS);
			break;
		case SIGHUP:
			fprintf(stderr,"SIGHUP received\n");
			break;
		default :
			fprintf(stderr,"Unknown signal received %d\n",sig);
			break;
	}
}

/*
**	Send Data
*/
error_code send_data(void)
{
	u_int32 loop;
	u_int32 count;
	u_int32 byte;

	printf("Sending data: \n");

	for (loop=1;loop<=COUNT;loop++) {

		printf(">%d>",loop);
		fflush(stdout);
		for(byte=0;byte<BUFSZ;byte++) {
			buf[byte] = loop;
		}
		count = BUFSZ;
		if ((errno = _os_write(path,buf,&count)) != SUCCESS) {
			printf("\nERROR: %s\n",strerror(errno));
			return(errno);
		}
	}
	printf("\nSUCCESSFUL\n");

	return(SUCCESS);
}

/*
 *	Receive Data
 */
error_code recv_data(void)
{
	u_int32 loop;
	u_int32 count;

	printf("Receiving data: \n");

	for (loop=1;loop<=COUNT;loop++) {

		buf[0] = 0;
		count = BUFSZ;
		if ((errno = _os_read(path,buf,&count)) != SUCCESS) {
			printf("\nERROR: %s\n",strerror(errno));
			return(errno);
		}
		if (buf[0] != (loop%256)) {
			printf("\nERROR: Out of Order Number Received\n");
			return(EOS_READ);
		}
		printf("<%d<",loop);
		fflush(stdout);
	}
	printf("\nSUCCESSFUL\n");

	return(SUCCESS);
}

/*
 *	Main Program
 */
void main(int argc, char *argv[])
{
	error_code 	err = SUCCESS;

	/* set up signal handler */
	signal(SIGINT,sighand);
	signal(SIGQUIT,sighand);
	signal(SIGHUP,sighand);

	/* check command line arguements */
	if ((argc < 3) || (argv[1][0] == '-')) {
		printf("Syntax: %s </device> <DTE/DCE> [<count>] [<bufsize]\n",argv[0]);
		printf("            default count = %d,  default bufsize = %d\n", COUNT, BUFSZ);
		printf("LAPB example:\n");
		printf("      spf_test /loopcl6/lapb_dce DCE 20 100\n");
		printf("      spf_test /loopcl5/lapb_dte DTE 20 100\n");
		printf("X.25 example:\n");
		printf("      spf_test \"/loopcl6/lapb_dce/x25dce#DCE|VC@\" DCE 20 100\n");
		printf("      spf_test \"/loopcl5/lapb_dte/x25dte#DTE|VC\" DTE 20 100\n\n");
		_os_close(path);
		_os_exit(SUCCESS);
	}

	/* get new count */
	if (argc >= 4) {
		COUNT = atoi(argv[3]);
	}
	printf("COUNT = [%d]\n",COUNT);

	/* get new buffer size */
	if (argc >= 5) {
		BUFSZ = atoi(argv[4]);
		if (BUFSZ > sizeof(buf)) {
			printf("ERROR: maximum buffer size = '%d'\n");
			_os_close(path);
			_os_exit(EOS_PARAM);
		}
	}
	printf("BUFSZ = [%d]\n",BUFSZ);

	/* open indicated device */
	printf("Opening device [%s].....", argv[1]);
	fflush(stdout);
	if ((errno = _os_open(argv[1], S_IREAD|S_IWRITE, &path)) != SUCCESS) {
		printf("ERROR: %s\n", strerror(errno));
		_os_close(path);
		_os_exit(errno);
	}
	printf("SUCCESSFUL\n");

	if (strcmp(argv[2],"DCE") == 0) {

		/* receive data */
		if ((errno = recv_data()) != SUCCESS) {
			_os_close(path);
			_os_exit(err);
		}

		/* send data */
		if ((errno = send_data()) != SUCCESS) {
			_os_close(path);
			_os_exit(err);
		}
		/*
		** If we are using a connection based protocol (ie. X.25), we will
		** receive a SIGHUP when the DCE side closes and we won't have to
		** wait the full 10 seconds.
		*/
		printf("Waiting for transfer to complete...\n");
		sleep(10);

	} else if (strcmp(argv[2],"DTE") == 0) {

		printf("DTE\n");

		/* send data */
		if ((errno = send_data()) != SUCCESS) {
			_os_close(path);
			_os_exit(err);
		}

		/* receive data */
		if ((errno = recv_data()) != SUCCESS) {
			_os_close(path);
			_os_exit(err);
		}

	} else {

		printf("ERROR: Unknown Command [%s]\n",argv[2]);
		_os_close(path);
		_os_exit(EOS_ILLARG);
	}

	_os_close(path);
	_os_exit(SUCCESS);
}
