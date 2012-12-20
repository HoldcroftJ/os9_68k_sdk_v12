/****************************************************************************
 *  ITEM example #1 send                                                    *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *   1 06/30/95 <---------------- SPF v2.0(pre) ---------------->      jmh
 *   2 01/08/96 Removed compiler warnings.                             jmh
 *   3 03/03/97 Ported to ARM                                          taka
 *     04/17/97 <---------------- SPF v2.2 --------------------->      taka
 *     03/25/98 Fixed nested comment to remove a compiler warning      taka
 *              <---------------- SPF v2.2.1 ------------------->      taka
 *   4 07/19/98 <---------------- SPF v2.2.2 ------------------->      taka
 ****************************************************************************/

/* example1_send.c
 *
 * This source code is the connection initiator. It opens
 * an ITEM path and makes a connection to the ex1_rcv.c
 * program (which must be running on the system).  After
 * making a connection, this program sends a "hello world"
 * message to the receiver program and awaits a response.
 * The reponse message is displayed and a disconnect
 * is performed before exiting.
 */

_asm("_sysedit: equ 4");	/* set edition to #4 */


/* include files:
 *     modes.h  for file access modes (FAM_READ and FAM_WRITE)
 *     const.h  for various constants (SUCCESS)
 *     cglob.h  for external _glob_data variable (needed for
 *                _os_intercept call)
 *     signal.h for signal handling prototypes
 *     string.h for string manipulation prototypes
 *     item.h   for ITEM structures (device_type, notify_type, addr_type)
 *                and for ITEM function prototypes
 */
#include <stdio.h>
#include <stdlib.h>
#include <modes.h>
#include <const.h>
#include <cglob.h>
#include <signal.h>
#include <string.h>
#include <SPF/item.h>


/* function prototype for sleep */
int sleep(unsigned);


/* system-specific definitions:
 * DEVICE      is our ITEM device
 * SND_MESSAGE is the message to send to the ex1_rcv.c program upon
 *               connection.
 * Modify these parameters for your particular test setup
 */
#define DEVICE		"/loopc0"
#define SND_MESSAGE	"hello world"


/* Define the signals used for notification */
#define CONNECT_SIG		0x2001
#define FEHANGUP_SIG    0x2002
#define DATAVAIL_SIG	0x2003


/* global variables for the sender application:
 *    connect_flag, datavail_flag, and fehangup_flag are set
 *    to 1 by the sighand function upon receiving CONNECT_SIG,
 *    DATAVAIL_SIG, or FEHANGUP_SIG, respectively.
 * define signal receive flags for use with notification */
u_int8	connect_flag, fehangup_flag, datavail_flag;


/* signal handler function -- its purpose is to intercept any
 * incoming signal and set the appropriate global flag variable.  Signal
 * handlers are important due to the asynchronous nature of network
 * communication.  As a general rule, I/O should not be performed
 * within the signal handler function.
 */
void sighand(int rcvd_signal)
{
	switch(rcvd_signal) {

		case CONNECT_SIG:
			connect_flag = 1;
			break;

		case DATAVAIL_SIG:
			datavail_flag = 1;
			break;

		case FEHANGUP_SIG:
			fehangup_flag = 1;
			break;

	}

	_os_rte();	/* return to program from signal handler */
} /* End signal handler */


void main(void)
{
	/* main program variables:
	 * dev_name = pointer to the name of our DEVICE
	 * ite_path = path to our DEVICE
	 * device_info = structure used to obtain call statistics and
	 *                 information
	 * connect_npb = connection notification parameter block
	 * fehangup_npb = far-end hangup notification parameter block
	 * datavail_npb = data available notification parameter block
	 * rcv_size     = used to remember the size of our data receive packets
	 * snd_size     = used to remember the size of our data send packets
	 * rcv_buffer   = data receive buffer
	 * snd_buffer   = data send buffer
	 * err          = used for error checking
	 */
	char		*dev_name = DEVICE;
	path_id 	ite_path;
	device_type	device_info;
					/* This structure is used to
					 * obtain call information and
					 * statistics
					 */
	notify_type	connect_npb, fehangup_npb, datavail_npb;
	u_int32		rcv_size, snd_size;
	u_char		rcv_buffer[32], snd_buffer[32];
	error_code	err;

	/* Most applications will need a signal handler due to
	 * the asynchronous nature of using a network device.
	 * Be sure to reset any signal flags being used to zero!
	 */
	if ((err = _os_intercept(sighand, _glob_data)) != SUCCESS)
	{
		printf("Error %03d:%03d installing signal handler\n", err/256, err%256);
		exit(0);
	}
	connect_flag = fehangup_flag = datavail_flag = 0;


	/* set up our notification blocks to let us receive signals for
	 * far-end hangup (FEHANGUP_SIG), connection (CONNECT_SIG), and
	 * data available (DATAVAIL_SIG).  Notice we're merely setting
	 * up the parameter blocks... we'll use these parameter blocks
	 * later to request notification.
	 */
	connect_npb.ntfy_class    = ITE_NCL_SIGNAL;
	connect_npb.ntfy_timeout  = 10;		/* 10 second timeout */
	connect_npb.ntfy_sig2send = CONNECT_SIG;

	fehangup_npb.ntfy_class    = ITE_NCL_SIGNAL;
	fehangup_npb.ntfy_timeout  = 10;	/* 10 second timeout */
	fehangup_npb.ntfy_sig2send = FEHANGUP_SIG;

	datavail_npb.ntfy_class    = ITE_NCL_SIGNAL;
	datavail_npb.ntfy_timeout  = 10;	/* 10 second timeout */
	datavail_npb.ntfy_sig2send = DATAVAIL_SIG;


	/* Open the ITEM path to our DEVICE for both READ and WRITE */
	if ((err = ite_path_open(DEVICE, FAM_READ | FAM_WRITE, &ite_path, NULL)) != SUCCESS) {
		printf("Error %03d:%03d on ite_path_open(%s)\n", err/256, err%256, dev_name);
		exit(0);
	}

	/* Now we get the device_type structure from our path.
	 * The source address information was set correctly
	 * from the ite_path_open call, but we'll check to
	 * verify it was done when we get the structure back.
	 */
	if ((err = ite_ctl_connstat(ite_path, &device_info)) != SUCCESS) {
		printf("Error %03d:%03d getting connection status for path\n", err/256, err%256);
		exit(0);
	}

	if (device_info.dev_ournum.addr_class != ITE_ADCL_LPBK) {
		printf("Address class not set during open\n");
		exit(0);
	}

	if (device_info.dev_ournum.addr_subclass != ITE_ADSUB_LUN) {
		printf("Address subclass not set during open\n");
		exit(0);
	}

	printf("\nite_open(%s) successful\n", DEVICE);


	/* Now let's make the call, shall we? Notice that the
	 * source & destination address fields are NULL since
	 * we're using a loopback driver whose descriptors
	 * contain default source/destination address information.
	 * Also notice we're using our connection notification parameter
	 * block to tell ITEM to notify us when a connection is made.
	 */
	if ((err = ite_ctl_connect(ite_path, NULL, NULL, &connect_npb)) != SUCCESS) {
		printf("Error %03d:%03d during attempt to connect\n", err/256, err%256);
		printf("Are you sure the receiver program is running?\n");
		exit(0);
	}

	/* Go to sleep and await connection notification signal.
	 * The sleep time should be slightly longer than the
	 * connection timeout value.  When a connection is made,
	 * the sleep() call will return immediately.
	 */
	sleep(connect_npb.ntfy_timeout + 5);


	/* Did we make a connection?
	 * If not, report a timeout error and exit.
	 * If yes, display a connection message along with the
	 * address we connected to.  Note for subclass ITE_ADSUB_LUN,
	 * addresses are stored as a u_int8 in the first byte of
	 * the addr field.
	 */
	if (!connect_flag) {	/* connection was not made */
		printf("Timeout error during connection attempt\n");
		exit(0);

	} else {

		printf("Connected to destination address %d\n", device_info.dev_theirnum.addr[0]);
	}


	/* Now, we'll use our fehangup_npb notification parameter block
	 * to request ITEM notify us on far-end hangup.
	 */
	if ((err = ite_fehangup_asgn(ite_path, &fehangup_npb)) != SUCCESS) {
		printf("Error %03d:%03d during fehangup signal assignment\n", err/256, err%256);
		exit(0);
	}


	/* We want to be prepared to be notified when the
	 * receiver program sends a response to us.
	 * To do this, we'll ask ITEM to notify us when data
	 * becomes available (using our datavail_npb block).
	 */
	if ((err = ite_data_avail_asgn(ite_path, &datavail_npb)) != SUCCESS) {
		printf("Error %03d:%03d during datavail signal assignment\n", err/256, err%256);
		exit(0);
	}


	/* Now that we have the end-to-end connection established.
	 * we'll send 'hello world' to the receiver program.
	 */
	strcpy((char *)snd_buffer, SND_MESSAGE);
	snd_size = strlen((char *)snd_buffer) + 1;
	if ((err = ite_data_write(ite_path, snd_buffer, &snd_size)) != SUCCESS) {
		printf("Error %03d:%03d during ite_data_write\n", err/256, err%256);
		exit(0);
	}


	/* Now, wait for the receiver to send a response.
	 * We know we've received a response when datavail_flag
	 * is set by our signal handler routine.
	 */
	if (!datavail_flag) {
		sleep(datavail_npb.ntfy_timeout + 5);
	}


	/* If our data available flag hasn't been set, we timed out
	 * while waiting for the receiver's response packet.
	 * Report the timeout error and exit.
	 */
	if (!datavail_flag)
	{
		printf("Timeout error while awaiting response\n");
		exit(0);
	}


	/* We've been notified there's a response waiting
	 * for us, so lets find out how many bytes are in the
	 * response using ite_data_ready.
	 */
	if ((err = ite_data_ready(ite_path, &rcv_size)) != SUCCESS) {
		printf("Error %03d:%03d on ite_data_ready call\n", err/256, err%256);
		exit(0);
	}


	/* Knowing how many bytes are in the response message,
	 * let's read the incoming data into our rcv_buffer.
	 */
	if ((err = ite_data_read(ite_path, rcv_buffer, &rcv_size)) != SUCCESS) {
		printf("Error %03d:%03d during ite_data_read\n", err/256, err%256);
		exit(0);
	}


	/* Display the received response. */
	printf("Response received = <%s>\n", rcv_buffer);


	/* Time to disconnect from the receiver.
	 * We are not using an in-band path (ib_path), so the
	 * second parameter of the ite_ctl_disconnect call must
	 * be set to NULL.
	 */
	if ((err = ite_ctl_disconnect(ite_path, NULL)) != SUCCESS) {
		printf("Error %03d:%03d on ite_ctl_disconnect\n", err/256, err%256);
		exit(0);
	}

	/* Close our ITEM path and exit. */
	if ((err = ite_path_close(ite_path)) != SUCCESS) {
		printf("Error %03d:%03d on ite_path_close\n", err/256, err%256);
		exit(0);
	}

	exit(0);
} /* End example1_send.c */

