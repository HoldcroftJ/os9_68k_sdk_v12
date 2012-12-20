/****************************************************************************
 * Example to show connectionless and connection-oriented ITEM communication*
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *  1  06/30/95 <---------------- SPF v2.0(pre) -------------->        jmh
 *  2  01/08/96 Removed compiler warnings.                             jmh
 *  3  03/31/97 added connectionless device                            taka
 *     06/27/97 <---------------- SPF v2.2 ------------------->        taka
 *  4  07/19/98 <---------------- SPF v2.2.2 ----------------->        taka
 ****************************************************************************/

_asm("_sysedit: equ 4");	/* set edition to #4 */


/* include files:
 *     modes.h  for various file access modes (FAM_READ and FAM_WRITE)
 *     const.h  for various constants (SUCCESS)
 *     cglob.h  for external _glob_data variable (needed for
 *                _os_intercept call)
 *     signal.h for signal handler prototypes
 *     string.h for string manipulation prototypes
 *     item.h   for ITEM structures (device_type, notify_type) and for
 *                ITEM function prototypes
 */
#include <stdio.h>
#include <stdlib.h>
#include <modes.h>
#include <const.h>
#include <cglob.h>
#include <signal.h>
#include <string.h>
#include <SPF/item.h>


/* sleep function prototype */
int sleep(unsigned);


/* System-specific definitions:
 *     DEVICE      = our ITEM loopback device
 *     DATA_STRING = the message to send and receive on the ITEM path
 */
#define DEVICE_CONNLESS	"/loop1"
#define DEVICE_CONN_ORI	"/loop"
#define DATA_STRING		"This is example #3 data."


/* Define the signals used for notification */
#define SIG_CONNECT		0x2001
#define SIG_INCALL		0x2002
#define SIG_DATAVAIL	0x2003
#define SIG_FEHANGUP    0x2004


/* Global variables:
 *     connect_flag  = set to 1 by sighandler when a SIG_CONNECT is received.
 *     incall_flag   = set to 1 by sighandler when a SIG_INCALL is received.
 *     datavail_flag = set to 1 by sighandler when a SIG_DATAVAIL is received.
 *     fehangup_flag = set to 1 by sighandler when a SIG_FEHANGUP is received.
 */
u_int8	connect_flag, incall_flag, datavail_flag, fehangup_flag;


/* signal handler function -- its purpose is to intercept any incoming
 * signal and set the appropriate global flag variable.
 */
void sighandler(int signal)
{
	switch (signal)
	{
		case SIG_CONNECT:
			connect_flag = 1;
			break;

		case SIG_INCALL:
			incall_flag = 1;
			break;

		case SIG_DATAVAIL:
			datavail_flag = 1;
			break;

		case SIG_FEHANGUP:
			fehangup_flag = 1;
			break;

		default:
			/* spurrious signal received */
			break;
	}

	_os_rte();	/* return to program from signal handler */
} /* End signal handler */


void main(void)
{
	/* main program variables:
	 *     ite_path     = path to our DEVICE
	 *     data_length  = used to store length of DATA_STRING message
	 *     count        = used in read/write functions to give # of bytes
	 *     buffer       = receive storage buffer
	 *     device_info  = structure used to obtain caller-id string
	 *     connect_npb  = connection notification parameter block
	 *     incall_npb   = incoming call notification parameter block
	 *     datavail_npb = data available notification parameter block
	 *     fehangup_npb = far-end hangup notification parameter block
	 *     err          = used for error checking
	 */
	path_id		ite_path;
	u_int32		data_length, count;
	u_char		buffer[256];
	device_type	device_info;
	notify_type	connect_npb, incall_npb, datavail_npb, fehangup_npb;
	error_code	err;

	/* set up our notification parameter blocks for connection,
	 * incoming call, and data available.  Notice we're merely
	 * setting up the parameter blocks... we'll use them later
	 * to request notification.
	 */
	connect_npb.ntfy_class = ITE_NCL_SIGNAL;
	connect_npb.ntfy_timeout = 10;			/* 10 second timeout */
	connect_npb.ntfy_sig2send = SIG_CONNECT;

	incall_npb.ntfy_class = ITE_NCL_SIGNAL;
	incall_npb.ntfy_timeout = 10;			/* 10 second timeout */
	incall_npb.ntfy_sig2send = SIG_INCALL;

	datavail_npb.ntfy_class = ITE_NCL_SIGNAL;
	datavail_npb.ntfy_timeout = 10;			/* 10 second timeout */
	datavail_npb.ntfy_sig2send = SIG_DATAVAIL;

	fehangup_npb.ntfy_class = ITE_NCL_SIGNAL;
	fehangup_npb.ntfy_timeout = 10;			/* 10 second timeout */
	fehangup_npb.ntfy_sig2send = SIG_FEHANGUP;


	/* Initialize data_length, our signal flags, and our signal handler. */
	data_length = strlen(DATA_STRING);
	connect_flag = incall_flag = datavail_flag = fehangup_flag = 0;
	if ((err = _os_intercept(sighandler, _glob_data)) != SUCCESS)
	{
		printf("Error %03d:%03d from _os_intercept\n", err/256, err%256);
		exit(0);
	}


	printf("\n** START OF CONNECTIONLESS COMMUNICATION **\n");
	/* For connectionless communication, the loopback descriptors
	 * contain the default addressing information.
	 */

	/* Open an ITEM path to our DEVICE for both READ & WRITE access. */
	if ((err = ite_path_open(DEVICE_CONNLESS, FAM_READ | FAM_WRITE, &ite_path,
															 NULL)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_path_open (%s)\n", err/256, err%256,
															DEVICE_CONNLESS);
		exit(0);
	}
	printf("ite_path_open call successful.\n");


	/* Using our data available notification parameter block, we
	 * ask ITEM to send us a SIG_DATAVAIL signal when data is
	 * ready to be read from our ITEM path.
	 */
	if ((err = ite_data_avail_asgn(ite_path, &datavail_npb)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_data_avail_asgn\n", err/256, err%256);
		exit(0);
	}


	/* write data to ite_path... since we are using a loopback driver,
	 * our data will come right back to us, triggering a SIG_DATAVAIL
	 * signal to be sent.
	 */
	count = data_length;
	if ((err = ite_data_write(ite_path, DATA_STRING, &count)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_data_write\n", err/256, err%256);
		exit(0);
	}


	/* Sleep until data available signal is received. */
	if (datavail_flag == 0) sleep(0);
	if (datavail_flag == 0)
	{
		printf("SIG_DATAVAIL not received on ite_path!\n");
		exit(0);
	}
	printf("SIG_DATAVAIL signal received correctly... %d bytes of data\n", count);


	/* Determine how many bytes of data need to be read.  It should
	 * be the same number of bytes we wrote out!
	 */
	if ((err = ite_data_ready(ite_path, &count)) != SUCCESS)
	{
		printf("Error %03d:%03d back from ite_data_ready\n", err/256, err%256);
		exit(0);
	}

	if (count != data_length)
	{
		printf("Received length is not correct!\n");
		printf("%d bytes received... should be %d bytes.\n", count, data_length);
	}


	/* Knowing how many bytes there are, read the data into
	 * our buffer.  Be sure to null-terminate the string.
	 */
	if ((err = ite_data_read(ite_path, buffer, &count)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_data_read\n", err/256, err%256);
		exit(0);
	}
	buffer[count] = '\0';


	/* Display the send/receive results. */
	printf("Data sent: <%s>\nData received: <%s>\n", DATA_STRING, buffer);


	/* Close our ITEM path. */
	if ((err = ite_path_close(ite_path)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_path_close on ite_path\n", err/256,
																	err%256);
	}
	printf("ite_path_close call successful.\n");


	printf("\n** START OF CONNECTION-ORIENTED COMMUNICATION **\n");
	/* For connection-oriented communication, the loopback descriptors
	 * may or may-not contain the default address information.  In
	 * this example, we assume the addressing information is held
	 * in the descriptor.  Below, we reinitialize our global flags.
	 */
	connect_flag = incall_flag = datavail_flag = fehangup_flag = 0;


	/* Open an ITEM path to our DEVICE for READ and WRITE access. */
	if ((err = ite_path_open(DEVICE_CONN_ORI, FAM_READ | FAM_WRITE, &ite_path,
															 NULL)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_path_open (%s)\n", err/256, err%256,
															DEVICE_CONN_ORI);
		exit(0);
	}
	printf("ite_path_open call successful.\n");


	/* The first thing a receiver program wants to do is a receiver
	 * assignment to be notified of any incoming calls.  We use our
	 * incall notification parameter block to ask ITEM to send us a
	 * SIG_INCALL signal when we have an incoming call.
	 */
	if ((err = ite_ctl_rcvrasgn(ite_path, NULL, &incall_npb)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_ctl_rcvrasgn\n", err/256, err%256);
		exit(0);
	}


	/* For a caller program, a call is placed using the ite_ctl_connect
	 * function.  We need to pass in our connect notification parameter
	 * block so ITEM sends a SIG_CONNECT when a connection is established.
	 */
	if ((err = ite_ctl_connect(ite_path, NULL, NULL, &connect_npb)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_ctl_connect\n", err/256, err%256);
		exit(0);
	}
	printf("ite_ctl_connect call successful.\n");


	/* The receiver program will be awaiting an incoming call signal
	 * (SIG_INCALL).
	 */
	if (incall_flag == 0) sleep(0);

	if (incall_flag == 0)
	{
		printf("Error -- SIG_INCALL not received!\n");
		exit(0);
	}
	printf("SIG_INCALL signal received correctly.\n");


	/* After receiving a SIG_INCALL signal, the receiver program can
	 * look at the caller-id information.  If this is not important,
	 * this can be omitted.  Using this technique, incoming calls can
	 * be screened.  An ite_ctl_disconnect can be used to refuse an
	 * incoming call.
	 */
	if ((err = ite_ctl_connstat(ite_path, &device_info)) != SUCCESS)
	{
		printf("Error %03d:%03d performing ite_ctl_connstat\n", err/256, err%256);
		exit(0);
	}
	printf("Incoming caller-id: <%s>\n", device_info.dev_display);


	/* After noticing an incoming call, the receiver program answers
	 * the call using the ite_ctl_answer function.  This will establish
	 * a connection and send a SIG_CONNECT to the caller program.
	 */
	if ((err = ite_ctl_answer(ite_path, NULL, NULL)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_ctl_answer\n", err/256, err%256);
		exit(0);
	}
	printf("ite_ctl_answer call successful.\n");


	/* After issuing an ite_ctl_connect call, the caller program
	 * will wait for a SIG_CONNECT to be sent, meaning a connection
	 * has been established.
	 */
	if (connect_flag == 0) sleep(0);

	if (connect_flag == 0)
	{
		printf("Error -- SIG_CONNECT never received after ite_ctl_answer!\n");
		exit(0);
	}
	printf("SIG_CONNECT signal received correctly.\n");


	/* Both the caller and receiver programs will want to be
	 * notified on far-end hangup (the other party disconnects).
	 * This is done using the ite_fehangup_asgn call with our
	 * far-end hangup notification parameter block (fehangup_npb).
	 */
	if ((err = ite_fehangup_asgn(ite_path, &fehangup_npb)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_fehangup_asgn\n", err/256, err%256);
		exit(0);
	}


	/* Just as we sent our DATA_STRING in a connectionless
	 * environment (above), we'll now send our DATA_STRING the
	 * exact same way since our connection has now been established.
	 * We start by having the receiver program request ITEM send
	 * a SIG_DATAVAIL when data is available for reading.
	 */
	if ((err = ite_data_avail_asgn(ite_path, &datavail_npb)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_data_avail_asgn\n", err/256, err%256);
		exit(0);
	}


	/* Now, the sender will write data to the connected ITEM path.
	 * This will cause the SIG_DATAVAIL signal to be sent to the
	 * receiving program.
	 */
	count = data_length;
	if ((err = ite_data_write(ite_path, DATA_STRING, &count)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_data_write\n", err/256, err%256);
		exit(0);
	}


	/* The receiver program will await the SIG_DATAVAIL signal. */
	if (datavail_flag == 0) sleep(0);

	if (datavail_flag == 0)
	{
		printf("SIG_DATAVAIL not received on ite_path!\n");
		exit(0);
	}
	printf("SIG_DATAVAIL signal received correctly... %d bytes of data\n", count);


	/* Determine how many bytes of data need to be read.  It should
	 * be the same number of bytes we wrote out!
	 */
	if ((err = ite_data_ready(ite_path, &count)) != SUCCESS)
	{
		printf("Error %03d:%03d back from ite_data_ready\n");
		exit(0);
	}

	if (count != data_length)
	{
		printf("Received length is not correct!\n");
		printf("%d bytes received... should be %d bytes.\n", count, data_length);
	}


	/* Knowing how many bytes there are, read the data into
	 * our buffer.  Be sure to null-terminate the string.
	 */
	if ((err = ite_data_read(ite_path, buffer, &count)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_data_read\n", err/256, err%256);
		exit(0);
	}
	buffer[count] = '\0';


	/* Display the send/receive results. */
	printf("Data sent: <%s>\nData received: <%s>\n", DATA_STRING, buffer);


	/* Our caller program now must disconnect from the receiver
	 * program using the ite_ctl_disconnect function.  This causes
	 * a SIG_FEHANGUP to be sent to the other end of the connection
	 * if fehangup notification is active.  Because we are
	 * not using an in-band path (ib_path), the second parameter of
	 * the ite_ctl_disconnect call must be zero.
	 */
	if ((err = ite_ctl_disconnect(ite_path, NULL)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_ctl_disconnect\n", err/256, err%256);
	}


	/* The other party would now receive a SIG_FEHANGUP signal */
	if (fehangup_flag == 0) sleep(0);

	if (fehangup_flag == 0)
	{
		printf("Error -- SIG_FEHANGUP never received after ite_ctl_disconnect!\n");
	}
	else
	{
		printf("SIG_FEHANGUP signal received correctly.\n");
	}


	/* Close our ITEM path. */
	if ((err = ite_path_close(ite_path)) != SUCCESS)
	{
		printf("Error %03d:%03d from ite_path_close on ite_path\n", err/256, err%256);
	}
	else
	{
		printf("ite_path_close call successful.\n");
	}


	/* Exit program */
	printf("** END OF EXAMPLE #3 **\n\n");
	exit(0);
}

