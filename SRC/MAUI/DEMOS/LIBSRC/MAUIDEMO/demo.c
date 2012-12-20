/*******************************************************************************
*
* DESCRIPTION:	Common gfx functions for demo programs. The caller can use demo_init()
*               and demo_term() before calling functions in this file, or else they are 
*		responsible for calling the necessary individual *_init() functions 
*		needed before calling functions in this file.
*
* AUTHOR:		Todd Earles
*
* CREATED ON:	02/06/96
*
* COPYRIGHT:	Copyright 1994 Microware Systems Corporation. All Rights
*				Reserved. Use and/or distribution subject to terms and
*				conditions of applicable Microware license agreement(s).
*
*/
#define DEMO_MAIN
#include "democom.h"

#include <alarm.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>

/*******************************************************************************
* NAME:		demo_random()
*
* USAGE:	Compute a random number within the specified range inclusive.
*/
int demo_random(int min, int max)
{
	int r1, r2;

	r1 = rand();
	r2 = r1 / ((RAND_MAX + 1) / (max - min + 1)) + min;
	if (r2 < min) {
		r2 = min;
	}
	if (r2 > max) {
		r2 = max;
	}
	return r2;
}


/*******************************************************************************
* NAME:		demo_set_timeout(seconds);
*
* USAGE:	Set flag "demo_notdone" with alarm in seconds
*/

static void demo_signal_handler(int signum)
{
  GET_DEMO_TLDATA();

  demo_notdone = FALSE;
}

error_code demo_set_timeout(u_int32 time)
{
  alarm_id alrm_id;

  GET_DEMO_TLDATA();

  demo_notdone = TRUE;

  /* Install the signal handler */
  signal (SIGQUIT, demo_signal_handler);

  return _os_alarm_set(&alrm_id, SIGQUIT, time*CLOCKS_PER_SEC);
}

/*******************************************************************************
* NAME:		demo_error(error_code ec, char *str, ...)
*
* USAGE:	Print error message
*/

#ifdef _OSK
#define ERRCODE_BITS	8			/* Bits for each part of error code */
#define ERRCODE_MASK	0xff		/* Mask for each part of error code */
#endif
#ifdef _OS9000
#define ERRCODE_BITS	16			/* Bits for each part of error code */
#define ERRCODE_MASK	0xffff		/* Mask for each part of error code */
#endif

error_code demo_error(error_code ec, char *fmt, ...)
{
  va_list args;

  va_start (args, fmt);
  fprintf (stderr, "Error #%d:%d `",
           ec >> ERRCODE_BITS, ec & ERRCODE_MASK);
  vfprintf (stderr, fmt, args);
  fprintf (stderr, "'\n");
  fflush(stderr);
  va_end (args);

  return ec;
}
