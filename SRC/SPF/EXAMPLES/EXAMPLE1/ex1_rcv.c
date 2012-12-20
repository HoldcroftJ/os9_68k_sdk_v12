/****************************************************************************
 *  ITEM example #1 receive                                                 *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *   1 06/30/95 <---------------- SPF v2.0(pre) ---------------->      jmh
 *   2 01/08/96 Removed compiler warnings.                             jmh
 *   3 02/28/97 Ported to ARM                                          taka
 *     06/27/97 <---------------- SPF v2.2 --------------------->      taka
 *     03/25/98 Removed unreferenced variable to fix compiler warning
 *              <---------------- SPF v2.2.1 ------------------->      taka
 *   4 07/19/98 <---------------- SPF v2.2.2 ------------------->      taka
 ****************************************************************************/

/* The function of the receiver in example 1 is to
 * accept incoming calls from any caller and read the
 * incoming data.  After reading in the incoming
 * message, it sends the RESPONSE_MSG message back to the
 * sender. The receiver also demonstrates the caller
 * identification capabilities of ITEM if the network
 * supports caller ID.
 */

/* ex1_rcv.c
 *
 * This source code is the connection receiver. It opens
 * an ITEM path, and uses the receiver assignment call to
 * wait for an incoming connection, then reads the
 * incoming data and sends the RESPONSE_MSG response.
 * Note on connectionless networks the
 * receiver assignment error EOS_CONN will occur. The
 * application should take this to mean that they are
 * attempting to receive data on a connectionless network,
 * therefore the read should happen without waiting for
 * the notification by the system software.
 */

_asm("_sysedit: equ 4");		/* set edition to #4 */


/* include files:
 *     modes.h  for file access modes (FAM_READ and FAM_WRITE)
 *     const.h  for various constants (SUCCESS)
 *     signal.h for signal value constants (SIGQUIT and SIGINT)
 *     cglob.h  for external _glob_data variable (needed for
 *                _os_intercept call)
 *     string.h for string manipulation prototypes
 *     item.h   for ITEM structures (device_type, notify_type, addr_type)
 *                and for ITEM function prototypes
 */
#include <stdio.h>
#include <stdlib.h>
#include <modes.h>
#include <const.h>
#include <signal.h>
#include <cglob.h>
#include <string.h>
#include <SPF/item.h>


/* sleep function prototype */
int sleep(unsigned);


/* system-specific definitions:
 * DEVICE       is the our ITEM device
 * RESPONSE_MSG is the message to send the ex1_snd.c program upon
 *                receiving a message.
 * Modify these parameters for your particular setup.
 */
#define DEVICE			"/loopc1"
#define RESPONSE_MSG	"Message Received"


/* Define the signals used for notification */
#define INCALL_SIG		0x2001
#define DATAVAIL_SIG	0x2002
#define FEHANGUP_SIG	0x2003


/* Global variables for the receiver application:
 *    incall_flag and datavail_flag are set to 1 by the sighand
 *    function upon receiving INCALL_SIG or DATAVAIL_SIG, respectively.
 *    connected_flag is set to 1 by the main program once a connection
 *    has been established.  connected_flag is reset to 0 by the
 *    sighand function upon receiving a FEHANGUP_SIG signal.
 *    exit_flag lets the main program know when the user has pressed
 *    CTRL-E or CTRL-C to exit the program.
 */
u_int8	incall_flag, connected_flag, datavail_flag, exit_flag;


/* signal handler function -- its purpose is to intercept any
 * incoming signal and set the appropriate global flag variable.  Signal
 * handlers are important due to the asynchronous nature of network
 * communication.  As a general rule, I/O should not be performed
 * within the signal handler function.
 */
void sighand(int rcvd_signal)
{
	switch(rcvd_signal) {

		case SIGQUIT:
		case SIGINT:
		case SIGHUP:
			exit_flag = 1;
			break;

		case INCALL_SIG:
			incall_flag = 1;
			break;

		case FEHANGUP_SIG:
			connected_flag = 0;
			break;

		case DATAVAIL_SIG:
			datavail_flag = 1;
			break;

	}

	_os_rte();	/* return to program from signal handler */
} /* End signal handler */


void main(void)
{
	/* main program variables:
	 * ite_path     = path to our DEVICE
	 * device_info  = structure used to obtain call statistics and
	 *                  information, including the caller-id string
	 * incall_npb   = incoming call notification parameter block
	 * fehangup_npb = far-end hangup notification parameter block
	 * datavail_npb = data available notification parameter block
	 * rcv_size     = used to remember the size of our data receive packets
	 * snd_size     = used to remember the size of our data send packets
	 * rcv_buffer   = data receive buffer
	 * snd_buffer   = data send buffer
	 * err          = used for error checking
	 */
	path_id		ite_path;
	device_type	device_info;
	notify_type	incall_npb, fehangup_npb, datavail_npb;
	u_int32		rcv_size, snd_size;
	u_char		rcv_buffer[32], snd_buffer[32];
	error_code	err;

	/* Most applications will need a signal handler due to
	 * the asynchronous nature of using a network device.
	 * Be sure to reset any global notification flags to zero!
	 */
	if ((err = _os_intercept(sighand, _glob_data)) != SUCCESS)
	{
		printf("Error %03d:%03d installing signal handler\n", err/256, err%256);
		exit(0);
	}
	incall_flag = connected_flag = datavail_flag = exit_flag = 0;


	/* set up our notification blocks to let us receive signals for
	 * far-end hangup (FEHANGUP_SIG), incoming call (INCALL_SIG), and
	 * data available (DATAVAIL_SIG).  Notice we're merely setting
	 * up the parameter blocks... we'll use these parameter blocks
	 * later to request notification.
	 */
	incall_npb.ntfy_class    = ITE_NCL_SIGNAL;
	incall_npb.ntfy_timeout  = 50;		/* no timeout for incoming calls */
	incall_npb.ntfy_sig2send = INCALL_SIG;

	fehangup_npb.ntfy_class    = ITE_NCL_SIGNAL;
	fehangup_npb.ntfy_timeout  = 10;	/* 10 second timeout */
	fehangup_npb.ntfy_sig2send = FEHANGUP_SIG;

	datavail_npb.ntfy_class    = ITE_NCL_SIGNAL;
	datavail_npb.ntfy_timeout  = 10;	/* 10 second timeout */
	datavail_npb.ntfy_sig2send = DATAVAIL_SIG;


	/* Open the ITEM path to our DEVICE for both READ and WRITE */
	printf("opening path...\n");
	if ((err = ite_path_open(DEVICE, FAM_READ | FAM_WRITE, &ite_path, NULL)) != SUCCESS) {
		printf("Error %03d:%03d on ite_path_open\n", err/256, err%256);
		exit(0);
	}
	printf("ite_path_open(%s) successful\n", DEVICE);


	/* the big loop -- loop forever (waiting for calls and answering them)
	 * until the user hits CTRL-E or CTRL-C to exit.
	 */
	while (exit_flag == 0) {

		/* initialize incall and connected flags */
		incall_flag = connected_flag = 0;

		/* Ensure we don't have a data_available assignment on our
		 * ITEM path left over from the previous time through the loop.
		 */
		if ((err = ite_data_avail_rmv(ite_path)) != SUCCESS) {
			printf("Error %03d:%03d Removing data available assignment\n", err/256, err%256);
		}

		/* Request notification of an incoming call.  This is set up by
		 * the ite_ctl_rcvrasgn (receiver assignment) call.  Notice we are
		 * passing the address to our incall notification block to tell
		 * ITEM to send a INCALL_SIG upon noticing an incoming call.
		 */
		if ((err = ite_ctl_rcvrasgn(ite_path, NULL, &incall_npb)) != SUCCESS) {
			printf("Error %03d:%03d performing receiver assignment\n", err/256, err%256);
		}
		printf("Waiting for incoming call...\n");

		/* Sleep until an incoming call.  Remember, although sleep(0) will
		 * sleep forever, our process will be awakened whenever a signal
		 *  is received.
		 */
		sleep(0);

		/* Do we have an incoming call?
		 * If so, let's use ite_ctl_connstat to get the caller-id
		 * string and display it.  It is possible to perform an
		 * ite_ctl_disconnect(ite_path, NULL) to refuse a connection if
		 * we are screening calls based on their caller-id strings.
		 * After displaying the caller-id string, let`s answer the
		 * incoming call using ite_ctl_answer and set our connected_flag.
		 * We also need to request notification of when the ex1_snd.c
		 * program disconnects from us (far-end hangup).
		 */
		if (incall_flag) {
			if ((err = ite_ctl_connstat(ite_path, &device_info)) != SUCCESS) {
				printf("Error %03d:%03d performing ite_ctl_connstat\n", err/256, err%256);
				exit(0);
			}
			printf("Incoming caller-id: <%s>\n", device_info.dev_display);

			/* Note that we are not using an in-band path (ib_path), so
			 * the second parameter in the ite_ctl_answer call must
			 * be NULL.
			 */
			if ((err = ite_ctl_answer(ite_path, NULL, NULL)) != SUCCESS) {
				printf("Error %03d:%03d from ite_ctl_answer\n", err/256, err%256);
				exit(0);
			}
			printf("Connected\n");
			incall_flag = 0;
			connected_flag = 1;

			/* request notification upon far-end hangup */
			if ((err = ite_fehangup_asgn(ite_path, &fehangup_npb)) != SUCCESS) {
				printf("Error %03d:%03d performing fehangup assignment\n", err/256, err%256);
				exit(0);
			}

		} else {

			/* We awoke from a signal, but it was not due to an incoming
			 * call.  It's probably the user wanting to exit the program.
			 * Let's remove our receiver assignment (INCALL_SIG).
			 */
			if ((err = ite_ctl_rcvrrmv(ite_path)) != SUCCESS) {
				printf("Error %03d:%03d from receiver remove\n", err/256, err%256);
			}
		}

		/* As long as we are connected to the sender program, stay in
		 * this loop.
		 */
		while (connected_flag) {

			/* Request ITEM to notify us when data is available from the
			 * sender program.  Notice we're using datavail_npb to have
			 * the DATAVAIL_SIG sent when data is available.
			 */
			if ((err = ite_data_avail_asgn(ite_path, &datavail_npb)) != SUCCESS) {
				printf("Error %03d:%03d performing data available assignment\n", err/256, err%256);
				exit(0);
			}

			/* Now, wait for the incoming data packet.  Let's sleep for
			 * five seconds longer than our requested timeout.  After
			 * sleeping, if we have not been notified of available data
			 * but we're still connected, report a timeout condition.
			 */
			if (!datavail_flag && connected_flag)
			{
				sleep(datavail_npb.ntfy_timeout + 5);
			}

			if (!datavail_flag && connected_flag)
			{
				printf("Timeout waiting for data from sender\n");
				exit(0);
			}

			/* We've been notified of either data available
			 * or far-end hangup.  If hangup has occurred,
			 * we'll simply drop out of our while loop and
			 * wait for another incoming call.
			 * If data is been available, we need to find out
			 * how many bytes the incoming data packet contains.
			 */
			if (datavail_flag)
			{
				if ((err = ite_data_ready(ite_path, &rcv_size)) != SUCCESS) {
					printf("Error %03d:%03d on ite_data_ready\n", err/256, err%256);
					exit(0);
				}

				/* Knowing how many bytes are to be read, let's read
				 * the incoming data into our rcv_buffer.
				 */
				if ((err = ite_data_read(ite_path, rcv_buffer, &rcv_size)) != SUCCESS) {
					printf("Error %03d:%03d during hello world read\n", err/256, err%256);
					exit(0);
				} else {
					printf("ite_data_read() result: [%s]\n", rcv_buffer);
				}

				/* reset our datavail_flag */
				datavail_flag = 0;

				/* Let's send our RESPONSE_MSG back to the sender
				 * program to acknowledge we received the data.
				 */
				strcpy((char *)snd_buffer, RESPONSE_MSG);
				snd_size = strlen((char *)RESPONSE_MSG) + 1;
				if ((err = ite_data_write(ite_path, snd_buffer,	&snd_size)) != SUCCESS) {
					printf("Error %03d:%03d during ite_data_write\n", err/256, err%256);
					continue;
				}
			}
		}

		/* We reach this point only if we've been disconnected by
		 * the sending program.  Report this fact to the user.
		 */
		printf("Disconnected\n\n");
		connected_flag = 0;
	}

	/* We reach this point only if the user has asked to exit using
	 * CTRL-E or CTRL-C.  Let's close our ITEM path and exit.
	 */
	if ((err = ite_path_close(ite_path)) != SUCCESS)
	{
		printf("Error %03d:%03d during ite_path_close\n", err/256, err%256);
	}

	exit(0);
} /* End ex1_rcv.c */

