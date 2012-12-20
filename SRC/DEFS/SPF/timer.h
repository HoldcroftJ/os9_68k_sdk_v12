/****************************************************************************
 * ID:    %A%
 * Date:  %G%
 ****************************************************************************
 * SPF timer service API definitions file
 ****************************************************************************
 * Copyright 1995 by Microware Systems Corporation                          *
 * Reproduced Under License                                                 *
 *                                                                          *
 * This source code is the proprietary confidential property of             *
 * Microware Systems Corporation, and is provided to licensee               *
 * solely for documentation and educational purposes. Reproduction,         *
 * publication, or distribution in any form to any party other than         *
 * the licensee is strictly prohibited.                                     *
 ****************************************************************************
 * Edition History:                                                         *
 * #   Date     Comments                                             By     *
 * --- -------- ------------------------------------------------- --------- *
 *  1  02/23/96 Creation(For my uncle Carl Schwaderer)               cas    *
 *  2  03/25/96 Added the timer_statistics call.                     cas    *
 *  3  10/22/96 <---------------- SPF v2.1.1 ------------------->    cas    *
 *  3  06/26/97 <---------------- SPF V2.2 --------------------->    taka   *
 *  2  07/20/98 <---------------- SPF v2.2.2 ------------------->    taka   *
 ****************************************************************************/

#ifndef _SPFTIMER
#define _SPFTIMER 1

#include <types.h>
#include <const.h>

#define MIN_TICK_INTERVAL	2		/* Minimum allowed timer tick interval	*/

#ifdef _OS9000
#include <process.h>
#define FLAGS       TH_SPOWN                /* System process alarm flag    */
#else
#define FLAGS       2                       /* TH_SPOWN for OS-9 */
#endif

typedef struct timer_pb timer_pb;
typedef struct timer_pb *Timer_pb;

/****************************************************************************
 * Timer parameter block structure. 
 *
 *	The caller may read, but NEVER WRITE OR MANIPULATE the following
 *	variables in this structure:
 *  
 *		timer_next, timer_prev, timer_id, timer_delta
 * 
 *	The caller is responsible for initializing the following variables in
 *	this structure for correct operation of the timer service:
 *
 *		timer_type, timer_call_type, call_type union, timer_duration
 *
 */
struct timer_pb {
	/** The following parameters can be modified only by the timer service **/
	Timer_pb	timer_next,		/* Next timer in the list kept by service	*/
				timer_prev;		/* Previous timer in list kept by service	*/
	void		*timer_stat;	/* Timer service's private storage area		*/
	u_int32		timer_tick_int,	/* Tick interval based on timer_interval	*/
				timer_off_tick;	/* The tick value when this times out		*/
	u_int16		timer_id;		/* Timer ID assigned by the service 		*/
	/** The rest of the variables are modified only by the user (driver)   **/
	u_int8		timer_type;		/* Timer type assigned by the user			*/
		#define TIMER_ONE_SHOT	1
		#define TIMER_CYCLIC	2
	u_int8		timer_call_type;	/* Call type assigned by the user		*/
		#define CALL_FUNCTION	1
		#define CALL_SIGNAL		2
	u_int32		timer_interval;		/* In milliseconds						*/
	union {
		struct {
			void	(*timeout_function)(Timer_pb tpb);	/* Function to call	*/
			void	*timeout_params;		/* Parameter block for function	*/
		} function_call;
		struct {
			u_int32	sig2send;				/* Signal number to send		*/
			u_int32	proc2wake;				/* Sleeping process number		*/
		} sig;
		struct {
			void	*x;
		} long_jump;
	} call_type;
		#define t_func	call_type.function_call.timeout_function
		#define t_pb	call_type.function_call.timeout_params
		#define t_ljump	call_type.long_jump.x
		#define t_sig	call_type.sig.sig2send
		#define t_pwake	call_type.sig.proc2wake
};

/* Initialize a timer parameter block for a callback function.
 *
 * SYNTAX: TPB_INIT_CB(Timer_pb,
 *			TIMER_ONE_SHOT/TIMER_CYCLIC,
 *			Timer Interval in milliseconds,
 *			timeout function to call,
 *			pointer to timeout parameters)
 */
#define TPB_INIT_CB(tmrpb, tmrtype, tmrint, tmrfunc, tmrparam) \
	(tmrpb)->timer_type = tmrtype; (tmrpb)->timer_call_type = CALL_FUNCTION;\
	(tmrpb)->timer_interval = tmrint; (tmrpb)->t_func = tmrfunc;\
	(tmrpb)->t_pb = tmrparam

/* Initialize a timer parameter block for a signal to be sent.
 *
 * SYNTAX: TPB_INIT_CB(Timer_pb,
 *			TIMER_ONE_SHOT/TIMER_CYCLIC,
 *			Timer Interval in milliseconds,
 *			Signal to send)
 */
#define TPB_INIT_SIG(tmrpb, tmrtype, tmrint, tmrsig) \
	(tmrpb)->timer_type = tmrtype; (tmrpb)->timer_call_type = CALL_SIGNAL;\
	(tmrpb)->timer_interval = tmrint; (tmrpb)->t_sig = tmrsig

/* Timer environment parameter block. There are rare occasions where drivers
 * might want to know system tick values so they can make efficient use
 * of timers. This parameter block gives back these statistics.
 *
 * Usually drivers will not need this, but it is available if needed.
 */
typedef struct timer_env {
	u_int16	ticks_per_sec,
			rsv1;
	u_int32	*curr_tick_ptr;
} timer_env, *Timer_env;

#ifdef __cplusplus
	extern "C" {
#endif

/* Initiate a registered timer. */
error_code timer_start(Dev_list deventry, Timer_pb tpb);

/* Restart the timer. This will restart a cyclic timer, or push out
 * the time at which a one-shot timer will time out.
 */
error_code timer_restart(Dev_list deventry, Timer_pb tpb);

/* Stop the timer, delete from list */
error_code timer_stop(Dev_list deventry, Timer_pb tpb);

/* Get the timer environment */
error_code timer_environ(Dev_list deventry, Timer_env tenv_pb);

#ifdef __cplusplus
	}
#endif

#endif /* _SPFTIMER */
