/*****************************************************************************
** ID:     @(#) main.c 1.4@(#)
** Date:   7/9/96
******************************************************************************
 *  Test application for MC68328ADS Touchpanel device driver
******************************************************************************
** Copyright 1995 by Microware Systems Corporation                          **
** Reproduced Under License                                                 **
**                                                                          **
** This source code is the proprietary confidential property of             **
** Microware Systems Corporation, and is provided to licensee               **
** solely for documentation and educational purposes. Reproduction,         **
** publication, or distribution in any form to any party other than         **
** the licensee is strictly prohibited.                                     **
******************************************************************************
** Edition History:                                                         **
** #   Date     Comments                                          By        **
** --- -------- ------------------------------------------------- --------- **
**   1 06/14/95 creation                                          rws       **
**   2 01/11/96 modified for Emerald usage                        rws       **
**   3 03/16/96 modified os_link                                  pbt       **
**   4 04/13/99 fix casting error                                 srm       **
*****************************************************************************/

#define NO_STATICS_SHORTCUTS
#include <touchpad.h>

#include <stdio.h>
#include <modes.h>
#include <sgstat.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define BAD_CMD 0x10

#define SIG_DRDY 2001

volatile int drdy = 0, sleep = 0;

extern int _errmsg(int nerr, char *msg, ...);
extern void intercept (void (*sighand)());
extern error_code _os_getstat (path_id path, u_int32 code, void *pb);
extern error_code _os_setstat (path_id path, u_int32 code, void *pb);
extern int _ss_ssig (int path, int sigcode);
extern int _ss_rel (int path);

void sighand (int signal)
{
	_os_sigmask (1);	/* mask signals during signal handler */

	switch (signal)
	{
		case 2:		/* ^E = keyboard abort signal */
		case 3:		/* ^C = keyboard interrupt signal */
			_errmsg (0, "Termination signal received\n");
			_errmsg (0, "sleep = %d, drdy = %d\n\n",sleep,drdy);
			exit (signal);
			
		case SIG_DRDY:	/* signal we're looking for */
			/* only purpose of signal is to wake up main() */
			drdy++;
			break;
			
		default:
			 _errmsg (0, "Unknown signal received ==> %d\n", signal);
			 exit (1);
	}

	_os_sigmask (-1);	/* unmask signals upon exit of signal handler */
	_os_rte();			/* cannot use 'return', must use _os_rte() upon 
						 * return from signal handler */
}


void main (void)
{
	path_id tpad_path;			/* path number to /tpad */
	/* path_id nv_path = 0; */		/* path number to /nv0 */
	error_code error;			/* error holder */
	u_int32 ticks = 0;			/* sent to _os9_sleep() to sleep indefinitely */
	u_int32 incount;			/* # bytes available from /tpad */
	u_int32 count;				/* # bytes to read in _os_read() */
	u_int32 loop_var;
	u_int32 x_pixels, y_pixels;
	error_code ec;
	
	/* struct sgbuf opt_buffer; */	/* for _os_opt() usage */

	/* variables used for _os_link() */
	char *mod_name = "tpad";
	mod_dev *mod_head;
	void *trash;
	u_int16 type_lang = 0;

	/* used for test of setstat(SS_DCmd) and getstat(SS_DCmd) */
	dcmd_struct tstate;
	u_short X_min, Y_min, X_max, Y_max; 

	u_char pktbuf[PKTBUFSIZ];	/* TOUCHPAD SCF Driver data packet */

	printf ("I am alive\n");

	/* link to the touchpad descriptor and get xsize and ysize info */
	if ((ec = _os_link (&mod_name, (mh_com **)&mod_head, (void **)&trash, 
				&type_lang, (u_int16 *)&trash)) != SUCCESS) {
		_errmsg (0, "Can't link to tpad (error 0x%2x)\n", ec);
		exit (1);
	}
	x_pixels = *(u_int32 *)((char *)mod_head + mod_head->_mdevcon);
	y_pixels = *(u_int32 *)((char *)mod_head + mod_head->_mdevcon + 4);
	printf ("Display resolution: XSIZE = %d, YSIZE = %d.\n", x_pixels, y_pixels);

	/* set up signal handler */
	intercept (sighand);
	printf ("Installed signal handler\n");
	
	/****************************************
	 *   test of init() and putstat(SS_Open)
	 ****************************************/

	if ( (error = _os_open ("/tpad", FAM_READ, &tpad_path)) != SUCCESS) {
		_errmsg (0, "Can't open path to /tpad (error 0x%2x)\n", error);
		exit (1);
	}
	else
		printf ("Opened path to /tpad (tpad_path # = %d)\n", tpad_path);

	printf ("\n");

	/****************************************
	 *   test of getstat(SS_EOF)
	 ****************************************/
	if ( (error = _os_gs_eof (tpad_path)) != SUCCESS) {
		_errmsg (0, "getstat(SS_EOF) unexpectedly returned 0x%2x\n", error);
		goto out;
	}
	else
		printf ("getstat(SS_EOF) was successful\n");
		
	/****************************************
	 *   test of getstat(SS_Ready) for EOS_NOTRDY
	 ****************************************/
	if ( (error = _os_gs_ready (tpad_path, &incount)) != EOS_NOTRDY) {
		_errmsg (0, "getstat(SS_Ready) failed (error 0x%2x)\n", error);
		goto out;
	}
	else
		printf ("getstat(SS_Ready) returned EOS_NOTRDY as expected\n");
		
	/****************************************
	 *   test of getstat(SS_DCmd = EOS_UNKSVC)
	 ****************************************/
	tstate.command = BAD_CMD;
	if ( (error = _os_getstat (tpad_path, SS_DCmd, (void *) &tstate)) != EOS_UNKSVC) {
		_errmsg (0, "getstat(SS_DCmd=BAD_CMD) unexpectedly returned %d\n", error);
		goto out;
	}
	else
		printf ("getstat(SS_DCmd=BAD_CMD) returned EOS_UNKSVC as expected\n");
		
	/****************************************
	 *   test of getstat(EOS_UNKSVC)
	 ****************************************/
	if ( (error = _os_getstat (tpad_path, SS_Joy, NULL)) != EOS_UNKSVC) {
		_errmsg (0, "getstat(SS_Joy) unexpectedly returned %d\n", error);
		goto out;
	}
	else
		printf ("getstat(SS_Joy) returned EOS_UNKSVC as expected\n");
		
	/****************************************
	 *   test of putstat(SS_DCmd = EOS_UNKSVC)
	 ****************************************/
	tstate.command = BAD_CMD;
	if ( (error = _os_setstat (tpad_path, SS_DCmd, (void *) &tstate)) != EOS_UNKSVC) {
		_errmsg (0, "putstat(SS_DCmd=BAD_CMD) unexpectedly returned %d\n", error);
		goto out;
	}
	else
		printf ("putstat(SS_DCmd=BAD_CMD) returned EOS_UNKSVC as expected\n");

	/****************************************
	 *   test of putstat(EOS_UNKSVC)
	 ****************************************/
	if ( (error = _os_setstat (tpad_path, SS_Joy, NULL)) != EOS_UNKSVC) {
		_errmsg (0, "putstat(SS_Joy) unexpectedly returned %d\n", error);
		goto out;
	}
	else
		printf ("putstat(SS_Joy) returned EOS_UNKSVC as expected\n");

	/****************************************
	 *   test of putstat(SS_Relea) for EOS_NOTRDY
	 ****************************************/
	if (_ss_rel (tpad_path) != -1) {
		_errmsg (0, "putstat(SS_Relea) was somehow successful\n");
		goto out;
	}
	else
		printf ("putstat(SS_Relea) failed as expected (error 0x%2x)\n", errno);
				
	/****************************************
	 *   test of write()
	 ****************************************/
	count = 1;
	pktbuf[0] = '4';
	if ( (error = _os_write (tpad_path, &pktbuf[0], &count)) == SUCCESS) {
		_errmsg (0, "write() was somehow successful\n");
		goto out;
	}
	else
		printf ("write() failed as expected (error 0x%2x)\n", error);
	
	printf ("\n");

	/****************************************
	 *  test of getstat(CALIBRATE)
	 ****************************************/

	printf ("*** CALIBRATION TEST ***\n\n");

	/* read calibration values -- should be DEFAULT values */
	tstate.command = CALIBRATE;
	if ( (error = _os_getstat (tpad_path, SS_DCmd, (void *) &tstate)) != SUCCESS) {
		_errmsg (0, "Error on getstat(CALIBRATE) ((error 0x%2x)\n", error);
		goto out;
	}
	else
	{
		printf ("Default Calibration values:\n");
		printf ("%s = 0x%04x\n%s = 0x%04x\n%s = 0x%04x\n%s = 0x%04x\n",
				"tstate.ss_specific.cal_data.ss_X_min",
				tstate.ss_specific.cal_data.ss_X_min,
				"tstate.ss_specific.cal_data.ss_Y_min",
				tstate.ss_specific.cal_data.ss_Y_min,
				"tstate.ss_specific.cal_data.ss_X_max",
				tstate.ss_specific.cal_data.ss_X_max,
				"tstate.ss_specific.cal_data.ss_Y_max",
				tstate.ss_specific.cal_data.ss_Y_max);
	}

	/****************************************
	 *  test of setstat(GET_POINT) -- Point 1
	 ****************************************/

touch_origin:

	/* inform driver to get point 1 */
	printf ("\n*** TOUCH ORIGIN ***\n\n");
	tstate.command = GET_POINT;
	tstate.ss_specific.gfx_point.x_pos = 0;
	tstate.ss_specific.gfx_point.y_pos = 0;
	if ( (error = _os_setstat (tpad_path, SS_DCmd, (void *) &tstate)) != SUCCESS) {
		_errmsg (0, "Error on setstat(SS_DCmd = GET_POINT) ((error 0x%2x)\n", error);
		goto out;
	}
	else
		printf ("setstat(SS_DCmd = GET_POINT) returned OK\n");

	/****************************************
	 *   test of read() to block until data ready
	 ****************************************
	 * for this test signals will not be used, read() will wait
	 * until data is available
	 */
	count = PKTBUFSIZ;
	while ( (error = _os_read (tpad_path, &pktbuf, &count)) != SUCCESS)
	{
		_errmsg (0, "_os_read() error 0x%2x\n", error);
	}

	/****************************************
	 *   Print results
	 ****************************************/
	for (loop_var = 0; loop_var < PKTBUFSIZ; loop_var++)
		printf ("pktbuf[%d] = 0x%2x ", loop_var, pktbuf[loop_var]);
	printf ("\n");

	if (pktbuf[0] & PKTBUF_PM_BYTE0)
	{
		if (pktbuf[0] == PKTBUF_PD_BYTE0)
			printf ("Pointer Down detected.\n");
		else if (pktbuf[0] == PKTBUF_PM_BYTE0)
			printf ("Pointer Move detected.\n");
		else if (pktbuf[0] == PKTBUF_PU_BYTE0)
			printf ("Pointer Up detected.\n");
		else {
			_errmsg (0, "Bad Pointer code in pktbuf[0]\n\n");
			goto touch_origin;
		}
		printf ("x_pos = %d, ", 
			((pktbuf[1] & 0x07) << 7) |
			 (pktbuf[2] & 0xFF));  
		printf ("y_pos = %d\n", 
			((pktbuf[1] & 0x38) << 4) |
			 (pktbuf[3] & 0xFF)); 
	}
	else
	{
		_errmsg (0, "Bad Pointer code in pktbuf[0]\n\n");
		goto touch_origin;
	}

	/* wait until Pointer Up message received */
	printf ("*Release touchpanel*\n");
	while (1)
	{
		count = PKTBUFSIZ;
		while ( (error = _os_read (tpad_path, &pktbuf, &count)) != SUCCESS)
		{
			_errmsg (0, "_os_read() error 0x%2x\n", error);
		}
		if (pktbuf[0] == PKTBUF_PU_BYTE0)
			break;
	}

	/****************************************
	 *  test of setstat(GET_POINT) -- Point 2
	 ****************************************/

touch_fc:

	/* inform driver to get point 2 */
	printf ("\n*** TOUCH FAR CORNER ***\n\n");
	tstate.command = GET_POINT;
	tstate.ss_specific.gfx_point.x_pos = x_pixels - 1;
	tstate.ss_specific.gfx_point.y_pos = y_pixels - 1;
	if ( (error = _os_setstat (tpad_path, SS_DCmd, (void *) &tstate)) != SUCCESS) {
		_errmsg (0, "Error on setstat(SS_DCmd = GET_POINT) ((error 0x%2x)\n", error);
		goto out;
	}
	else
		printf ("setstat(SS_DCmd = GET_POINT) returned OK\n");
	
	/****************************************
	 *   test of read() to block until data ready
	 ****************************************
	 * for this test signals will not be used, read() will wait
	 * until data is available
	 */
	count = PKTBUFSIZ;
	while ( (error = _os_read (tpad_path, &pktbuf, &count)) != SUCCESS)
	{
		_errmsg (0, "_os_read() error 0x%2x\n", error);
	}

	/****************************************
	 *   Print results
	 ****************************************/
	for (loop_var = 0; loop_var < PKTBUFSIZ; loop_var++)
		printf ("pktbuf[%d] = 0x%2x ", loop_var, pktbuf[loop_var]);
	printf ("\n");

	if (pktbuf[0] & PKTBUF_PM_BYTE0)
	{
		if (pktbuf[0] == PKTBUF_PD_BYTE0)
			printf ("Pointer Down detected.\n");
		else if (pktbuf[0] == PKTBUF_PM_BYTE0)
			printf ("Pointer Move detected.\n");
		else if (pktbuf[0] == PKTBUF_PU_BYTE0)
			printf ("Pointer Up detected.\n");
		else {
			_errmsg (0, "Bad Pointer code in pktbuf[0]\n\n");
			goto touch_fc;
		}
		printf ("x_pos = %d, ", 
			((pktbuf[1] & 0x07) << 7) |
			 (pktbuf[2] & 0xFF));  
		printf ("y_pos = %d\n", 
			((pktbuf[1] & 0x38) << 4) |
			 (pktbuf[3] & 0xFF)); 
	}
	else
	{
		_errmsg (0, "Bad Pointer code in pktbuf[0]\n\n");
		goto touch_fc;
	}

	/* wait until Pointer Up message received */
	printf ("*Release touchpanel*\n\n");
	while (1)
	{
		count = PKTBUFSIZ;
		while ( (error = _os_read (tpad_path, &pktbuf, &count)) != SUCCESS)
		{
			_errmsg (0, "_os_read() error 0x%2x\n", error);
		}
		if (pktbuf[0] == PKTBUF_PU_BYTE0)
			break;
	}

	/****************************************
	 *  test of getstat(CALIBRATE)
	 ****************************************/
	/* calibration values should be real values now, different from DEFAULT values */
	tstate.command = CALIBRATE;
	if ( (error = _os_getstat (tpad_path, SS_DCmd, (void *) &tstate)) != SUCCESS) {
		_errmsg (0, "Error on getstat(CALIBRATE) ((error 0x%2x)\n", error);
		goto out;
	}
	else
	{
		printf ("New Calibration Values:\n");
		printf ("%s = 0x%04x\n%s = 0x%04x\n%s = 0x%04x\n%s = 0x%04x\n",
				/* save real values for later */
				"tstate.ss_specific.cal_data.ss_X_min",
				X_min = tstate.ss_specific.cal_data.ss_X_min,
				"tstate.ss_specific.cal_data.ss_Y_min",
				Y_min = tstate.ss_specific.cal_data.ss_Y_min,
				"tstate.ss_specific.cal_data.ss_X_max",
				X_max = tstate.ss_specific.cal_data.ss_X_max,
				"tstate.ss_specific.cal_data.ss_Y_max",
				Y_max = tstate.ss_specific.cal_data.ss_Y_max);
	}

	/****************************************
	 *  test of setstat(CALIBRATE)
	 ****************************************/
	/* put back in DEFAULT values */
	tstate.command = CALIBRATE;
	tstate.ss_specific.cal_data.ss_X_min = X_MIN_DEFAULT;
	tstate.ss_specific.cal_data.ss_Y_min = Y_MIN_DEFAULT;
	tstate.ss_specific.cal_data.ss_X_max = X_MAX_DEFAULT;
	tstate.ss_specific.cal_data.ss_Y_max = Y_MAX_DEFAULT;
	if ( (error = _os_setstat (tpad_path, SS_DCmd, (void *) &tstate)) != SUCCESS) {
		_errmsg (0, "Error on setstat(CALIBRATE) ((error 0x%2x)\n", error);
		goto out;
	}
	else
		printf ("\nsetstat(CALIBRATE) returned OK\n");

	/****************************************
	 *  test of getstat(CALIBRATE)
	 ****************************************/
	/* should read back DEFAULT values */
	tstate.command = CALIBRATE;
	if ( (error = _os_getstat (tpad_path, SS_DCmd, (void *) &tstate)) != SUCCESS) {
		_errmsg (0, "Error on getstat(CALIBRATE) ((error 0x%2x)\n", error);
		goto out;
	}
	else
		printf ("%s = 0x%04x\n%s = 0x%04x\n%s = 0x%04x\n%s = 0x%04x\n",
				"tstate.ss_specific.cal_data.ss_X_min",
				tstate.ss_specific.cal_data.ss_X_min,
				"tstate.ss_specific.cal_data.ss_Y_min",
				tstate.ss_specific.cal_data.ss_Y_min,
				"tstate.ss_specific.cal_data.ss_X_max",
				tstate.ss_specific.cal_data.ss_X_max,
				"tstate.ss_specific.cal_data.ss_Y_max",
				tstate.ss_specific.cal_data.ss_Y_max);

	/****************************************
	 *  test of setstat(CALIBRATE)
	 ****************************************/
	/* put back in real values */
	tstate.command = CALIBRATE;
	tstate.ss_specific.cal_data.ss_X_min = X_min;
	tstate.ss_specific.cal_data.ss_Y_min = Y_min;
	tstate.ss_specific.cal_data.ss_X_max = X_max;
	tstate.ss_specific.cal_data.ss_Y_max = Y_max;
	if ( (error = _os_setstat (tpad_path, SS_DCmd, (void *) &tstate)) != SUCCESS) {
		_errmsg (0, "Error on setstat(CALIBRATE) ((error 0x%2x)\n", error);
		goto out;
	}
	else
		printf ("\nsetstat(CALIBRATE) returned OK\n");

	/****************************************
	 *  test of getstat(CALIBRATE)
	 ****************************************/
	/* should read back real values */
	tstate.command = CALIBRATE;
	if ( (error = _os_getstat (tpad_path, SS_DCmd, (void *) &tstate)) != SUCCESS) {
		_errmsg (0, "Error on getstat(CALIBRATE) ((error 0x%2x)\n", error);
		goto out;
	}
	else
		 printf ("%s = 0x%04x\n%s = 0x%04x\n%s = 0x%04x\n%s = 0x%04x\n",
				"tstate.ss_specific.cal_data.ss_X_min",
				tstate.ss_specific.cal_data.ss_X_min,
				"tstate.ss_specific.cal_data.ss_Y_min",
				tstate.ss_specific.cal_data.ss_Y_min,
				"tstate.ss_specific.cal_data.ss_X_max",
				tstate.ss_specific.cal_data.ss_X_max,
				"tstate.ss_specific.cal_data.ss_Y_max",
				tstate.ss_specific.cal_data.ss_Y_max);

	printf ("\n");
				

#if 0	/* For some insofarasyet unknown reason, the following code dealing
		 * with /nv0/calib affects the touchpad driver in the following way:
		 *  after getting the first or second calibration point (and
		 *  maybe even one additional point after that, depending on
		 *  whether or not tpadtest is run after reset), the touchpad
		 *  driver thinks that the touchpad is forever touched, even when
		 *  it isn't (i.e., the driver's static storage variable 'touched'
		 *  is always YES).
		 */

	/****************************************
	 *  get/put calibration values from/in EEPROM
	 ****************************************/
	/* open path to EEPROM's calibration data file */
	if ( (error = _os_open ("/nv0/calib", FAM_READ+FAM_WRITE, &nv_path)) != SUCCESS) 
	{
		_errmsg (0, "/nv0/calib does not exist (error 0x%2x)\n", error);
		if ( (error = _os_create ("/nv0/calib", FAM_READ+FAM_WRITE, &nv_path,
						S_IREAD+S_IWRITE+S_IOREAD+S_IOWRITE)) != SUCCESS)
		{
			_errmsg (0, "Cannot create /nv0/calib (error 0x%2x)\n", error);
			goto out;
		}
		else 
		{
			_errmsg (0, "Created /nv0/calib (nv_path # = %d)\n", nv_path);
			_errmsg (0, "Storing calibration values\n");
			_os_close (nv_path); 
		}
	}
	else
	{
		_errmsg (0, "Opened path to /nv0/calib (nv_path # = %d)\n", nv_path);
		_errmsg (0, "Retrieving calibration values from /nv0/calib\n");
		_os_close (nv_path); 
	}

#endif


	printf ("*** SSIG TEST ***\n\n"); 

	/* mask signals for signal setup */
	if ( (error = _os_sigmask(1)) != SUCCESS) {
		_errmsg (0, "Can't mask signals (error 0x%2x)\n", error);
		goto out;
	}
	else
		printf ("Masked signal\n");

	/****************************************
	 *   test of putstat(SS_SSig) for SUCCESS
	 ****************************************/
	if (_ss_ssig (tpad_path, SIG_DRDY) == -1) {
		_errmsg (0, "putstat(SS_SSig) can't setup signal on data ready\n");
		goto out;
	}
	else
		printf ("A signal will be sent upon data ready\n");
				
	/****************************************
	 *   test of putstat(SS_SSig) for EOS_NOTRDY
	 ****************************************/
	if (_ss_ssig (tpad_path, SIG_DRDY) != -1) {
		_errmsg (0, "putstat(SS_SSig) somehow successfully completed\n");
		goto out;
	}
	else
		printf ("putstat(SS_SSig) failed as expected (error 0x%2x)\n", errno);
				
	/****************************************
	 *   test of putstat(SS_Relea) for SUCCESS
	 ****************************************/
	if (_ss_rel (tpad_path) == -1) {
		_errmsg (0, "putstat(SS_Relea) failed (error 0x%2x)\n", errno);
		goto out;
	}
	else
		printf ("putstat(SS_Relea) was successful\n");
				
	/* unmask signals */
	if ( (error = _os_sigmask(0)) != SUCCESS) {
		_errmsg (0, "Can't unmask signals (error 0x%2x)\n", error);
		goto out;
	}
	else
		printf ("Unmasked signal\n\n");


	printf ("*** TOUCHPAD TEST ***\n\n"); 

	while (1)
	{
		/* wait for data */
		
		/* mask signals for signal setup */
		if ( (error = _os_sigmask(1)) != SUCCESS) {
			_errmsg (0, "Can't mask signals (error 0x%2x)\n", error);
			goto out;
		}
		else
			printf ("Masked signal\n");

		if (_ss_ssig (tpad_path, SIG_DRDY) == -1) {
			_errmsg (0, "putstat(SS_SSig) can't setup signal on data ready\n");
			goto out;
		}
		else
			printf ("A signal will be sent upon data ready\n");
				
		_os9_sleep (&ticks);	/* sleep until woken up by signal */
		sleep++;

		/* _os9_sleep() does an _os_sigmask(0) internally, no need to repeat this */

		/* process data -- at this point a signal has come in and woken us up */
		
		/****************************************
		 *   test of getstat(SS_Ready) for SUCCESS
		 ****************************************/
		if ( (error = _os_gs_ready (tpad_path, &incount)) != SUCCESS) {
			_errmsg (0, "/tpad can't return # bytes available (error 0x%2x)\n", error);
			goto out;
		}
		else
			printf ("/tpad bytes available = %d\n", incount);
		
		if (incount != PKTBUFSIZ) {
			_errmsg (0, "incount != %d, unexpected # bytes available (%d)\n", 
				PKTBUFSIZ, incount);
			goto out;
		}
		
		/****************************************
		 *   test of read()
		 ****************************************/
		/* get header byte (1   0   0   0   0   0   bt1 bt0) */
		count = 1;
		if ( (error = _os_read (tpad_path, &pktbuf[0], &count)) != SUCCESS) {
			_errmsg (0, "_os_read() error 0x%2x\n", error);
			continue;
		}
		if (!(pktbuf[0] & 0x80))
			continue;	/* wait until first byte */
		
		/****************************************
		 *   test of getstat(SS_Ready) for SUCCESS
		 ****************************************/
		if ( (error = _os_gs_ready (tpad_path, &incount)) != SUCCESS) {
			_errmsg (0, "/tpad can't return # bytes available (error 0x%2x)\n", error);
			goto out;
		}
		
		if (incount != PKTBUFSIZ - 1) {
			_errmsg (0, "incount != %d, unexpected # bytes available (%d)\n", 
				PKTBUFSIZ - 1, incount);
			continue;
		}
		
		/****************************************
		 *   test of read()
		 ****************************************/
		/* get second byte (0   0   Y9  Y8  Y7  X9  X8  X7) */
		count = 1;
		if ( (error = _os_read (tpad_path, &pktbuf[1], &count)) != SUCCESS) {
			_errmsg (0, "_os_read() error 0x%2x\n", error);
			continue;
		}
		
		/****************************************
		 *   test of getstat(SS_Ready) for SUCCESS
		 ****************************************/
		if ( (error = _os_gs_ready (tpad_path, &incount)) != SUCCESS) {
			_errmsg (0, "/tpad can't return # bytes available (error 0x%2x)\n", error);
			continue;
		}
		
		if (incount != PKTBUFSIZ - 2) {
			_errmsg (0, "incount != %d, unexpected # bytes available (%d)\n",
				PKTBUFSIZ - 2, incount);
			continue;
		}
		
		/****************************************
		 *   test of read()
		 ****************************************/
		/* get third byte (0   X6  X5  X4  X3  X2  X1  X0) */
		count = 1;
		if ( (error = _os_read (tpad_path, &pktbuf[2], &count)) != SUCCESS) {
			_errmsg (0, "_os_read() error 0x%2x\n", error);
			continue;
		}
		
		/****************************************
		 *   test of getstat(SS_Ready) for SUCCESS
		 ****************************************/
		if ( (error = _os_gs_ready (tpad_path, &incount)) != SUCCESS) {
			_errmsg (0, "/tpad can't return # bytes available (error 0x%2x)\n", error);
			continue;
		}
		
		if (incount != PKTBUFSIZ - 3) {
			_errmsg (0, "incount != %d, unexpected # bytes available (%d)\n",
				PKTBUFSIZ - 3, incount);
			continue;
		}
		
		/****************************************
		 *   test of read()
		 ****************************************/
		/* get fourth byte (0   Y6  Y5  Y4  Y3  Y2  Y1  Y0) */
		count = 1;
		if ( (error = _os_read (tpad_path, &pktbuf[3], &count)) != SUCCESS) {
			_errmsg (0, "_os_read() error 0x%2x\n", error);
			continue;
		}
		
		/****************************************
		 *   Print results
		 ****************************************/
		for (loop_var = 0; loop_var < PKTBUFSIZ; loop_var++)
			printf ("pktbuf[%d] = 0x%2x ", loop_var, pktbuf[loop_var]);
		printf ("\n");

		if (pktbuf[0] & PKTBUF_PM_BYTE0)
		{
			if (pktbuf[0] == PKTBUF_PD_BYTE0)
				printf ("Pointer Down detected.\n");
			else if (pktbuf[0] == PKTBUF_PM_BYTE0)
				printf ("Pointer Move detected.\n");
			else if (pktbuf[0] == PKTBUF_PU_BYTE0)
				printf ("Pointer Up detected.\n");
			else {
				_errmsg (0, "Bad Pointer code in pktbuf[0]\n\n");
				continue;
			}
			printf ("x_pos = %d, ", 
				((pktbuf[1] & 0x07) << 7) |
				 (pktbuf[2] & 0xFF));  
			printf ("y_pos = %d\n", 
				((pktbuf[1] & 0x38) << 4) |
				 (pktbuf[3] & 0xFF)); 
		}
		else
		{
			_errmsg (0, "Bad Pointer code in pktbuf[0]\n\n");
			continue;
		}

		printf ("\n"); 
	}		

	/* should not get here */
	/* _errmsg (0, "Well, how did I get here?\n"); */
	
out:

	/****************************************
	 *   test of term() and putstat(SS_Close)
	 ****************************************/
	_errmsg (0, "\nBye!\n\n");
	_os_close (tpad_path);	/* _os_close() forces unlink of tester and tpad */
	exit (1);
}
