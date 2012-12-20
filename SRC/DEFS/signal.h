/*---------------------------------------------------------------------------,
|                                                                            |
|              Copyright 2000 by Microware Systems Corporation.              |
|              All Rights Reserved.   Reproduced Under License.              |
|                                                                            |
|  This software is confidential property of Microware Systems Corporation,  |
|  and is provided under license for internal development purposes only.     |
|  Reproduction, publication, modification, distribution, or creation of     |
|  derivative works in any form to any party other than the Licensee is      |
|  strictly prohibited, unless expressly provided for under the terms of a   |
|  mutually executed written license agreement for this software between     |
|  the end-user and Microware Systems Corporation, or otherwise expressly    |
|  authorized in writing by Microware Systems Corporation.  Licensee shall   |
|  reproduce this copyright notice exactly as provided herein on any copies  |
|  of the software that Licensee is authorized to distribute.                |
|                                                                            |
|----------------------------------------------------------------------------|
|                                                                            |
| Edition History:                                                           |
| #   Date      Comments                                                By   |
| --- --------- ------------------------------------------------------- ---- |
|   1 17-Mar-92 Created                                                 rcb  |
|   2 24-Jun-92 Changes for _ANSI_EXT pre-processor symbol              rry  |
|   3 29-Jul-92 Changed signal values                                   rry  |
|   4 30-Mar-94 New signal usage comments                               wb   |
|   5 13-Feb-95 C++ support added                                       rcb  |
|   6 13-May-96 Added optional prototypes                               rry  |
|   7 28-Jul-97 ARM sig stuff added                                     tsb  |
|   8 18-Nov-97 SPARC and SH signals added                              ajk  |
|   9 20-Feb-98 Added this header                                       nan  |
|  10 17-Mar-99 Added C++ version of _os_intercept prototype            rry  |
|  11 16-Mar-99 Added SIGPRCABT for Clib/DPIO signal return value.      mgh  |
|               Value returned by OSK LIBS if process received SIGKILL       |
|  12 09-Mar-00 Added IEEE1394 signal codes, updated copyright.         cdh  |
|  13 04-May-00 Added SIGPRCABT for OS-9000.                            djl  |
|  14 07-Jun-00 Added pthread_kill() prototype as per POSIX				rry  |
|  15 08-Oct-01 CF11647: Define SIGDEADLY and fatalsignal() for both    and  |
|               _OSK and _OS9000                                             |
|  16 12-Nov-01 Fixed fixes from above for more correctness				rry  |
`---------------------------------------------------------------------------*/

#if !defined(_SIGNAL_H)
#define _SIGNAL_H

/* Signal Code Values for OS-9/68000.
*
* Signal Code range usage:
*
* Signal SIGKILL is an unconditional process abort (cannot be intercepted)
* Signal SIGWAKE causes a process to activate.  If the process is in
*    system-state (and already active) the signal will queue.  If the
*    process is in user-state (and already active) the signal will have
*    no effect.
*
* Deadly I/O signals:
*
* Signals 2 - 19 are for Microware defined global signals that are deadly
*   to I/O (i.e. cause current I/O to abort).
* Signals 20 - 25 are for Microware defined local-application signals that
*   are deadly to I/O.
* Signals 26 - 31 are for User defined signals that are deadly to I/O.
*
* Non-Deadly I/O signals:
*
* Signals 32 - 127 are for Microware defined non-deadly I/O signals used by
*   Tools.
* Signals 128 - 191 are for Microware defined non-deadly I/O signals used
*   by local-applications.
* Signals 192 - 255 are for Microware defined non-deadly I/O signals used
*   for global purposes.
* Signals 255 - 65535 are for User defined non-deadly I/O signals.
*
* The POSIX Pthread library, by default, uses Signals 40000 to 49999, but
*   this range can be changed via _pthread_setsignalrange().
*/

#if defined(_OSK)
/* OS-9 signals */
#define		SIGKILL		0	/* system abort (cannot be caught or ignored) */
#elif defined(_OS9000)
#define		SIGKILL		4	/* system abort (cannot be caught or ignored) */
#endif

#define		SIGWAKE		1	/* wake up */
#define		SIGQUIT		2	/* keyboard abort */
#define		SIGINT		3	/* keyboard interrupt */

#if defined(_OSK)
#define		SIGHUP		4	/* modem hangup signal */
#define		SIGPRCABT	5	/* DPIO/CLIB compatible return Signal Value  */
				        /* For an OS9/68k Process that is condemmed  */
					/* ie. they've received SIGKILL and are comdemmed */
#elif defined(_OS9000)
#define		SIGHUP		5	/* modem hangup signal */
#define		SIGPRCABT	6	/* same as OSK but also used to kill threads */
                                        /* after an _os_exit call.                   */
#endif

#if defined(_OSK) || defined(_OS9000)
#define		SIGDEADLY	32	/* lower signal codes may abort i/o */
#define		fatalsignal(s) (((s) > 0) && ((s) < SIGDEADLY))
#endif

#if defined(_OS9000) && !defined(__STDC__) && !defined(__cplusplus)
/* non-ANSI System defined signals and macro */
#define S_WAKE		1		/* wakeup process signal */
#define S_QUIT		2		/* keyboard abort */
#define S_INT		3		/* keyboard interrupt */
#define S_KILL		4		/* kill process signal */
#define S_HANGUP	5		/* hang up process signal */
#endif

/* Post-ANSI OS-9/OS-9000 signals */
#if defined(_MPF68K) || defined(_MPFCOLDFIRE)
#define		SIGADDR		41			/* address error */
#define		SIGCHK		42			/* chk instruction */
#define		SIGTRAPV	43			/* trapv instruction */
#define		SIGPRIV		44			/* priveledge violation */
#define		SIGTRACE	45			/* trace exception */
#define		SIG1010		46			/* 1010 line-A exception */
#define		SIG1111		47			/* 1111 line-F exception */

#if defined(_OS9000)
#define		SIGCOPRCV	48			/* coprocessor protocol violation */
#define		SIGFMTERR	49			/* format error */
#define		SIGUNIRQ	50			/* uninitialized interrupt */
#endif	/* if _OS9000 */

#endif	/* if _MPF68K */

#if defined(_MPF386)
#define		SIGGPROT	41			/* general protection */
#define		SIGSTACK	42			/* stack exception */
#define		SIGSEGNP	43			/* segment not present */
#define		SIGINVTSS	44			/* invalid TSS */
#define		SIGDBLFLT	45			/* double fault */
#define		SIGBNDCHK	46			/* boundry check */
#define		SIGBRKPT	47			/* breakpoint */
#define		SIGNMI		48			/* non-maskable interrupt */
#define		SIGDBG		49			/* debug exception */
#endif

#if defined(_MPFPOWERPC)
#define		SIGCHECK	41			/* machine check */
#define		SIGALIGN	42			/* alignment */
#define		SIGINST		43			/* instruction fetch */
#define		SIGPRIV		44			/* privilege */
#endif

#if defined(_MPFARM)
#define		SIGALIGN	41			/* alignment */
#endif

#if defined(_MPFSH)
#define		SIGALIGN	41			/* alignment */
#endif

#if defined(_MPFSPARC)
#define		SIGALIGN	41			/* alignment */
#define		SIGWINDOWOV	42			/* window overflow */
#define		SIGWINDOWUF	43			/* window underflow */
#define		SIGTAGOV	44			/* tagged add/sub overflow */
#define		SIGCPE		45			/* coprocessor exception */
#endif

#if defined(_MPFMIPS)
#define		SIGTRAP		41			/* trap instruction */
#endif

/* ANSI signals */
#define		SIGABRT		32			/* abort signal */
#define		SIGFPE		33			/* erroneous arithmetic operation (not always FPU) */
#define		SIGILL		34			/* illegal function image (illegal instruction) */
#define		SIGSEGV		35			/* segment violation (bus error) */
#define		SIGTERM		36			/* termination request */

/* POSIX signals (not directly support) */
#define		SIGALRM		37			/* alarm time elapsed */
#define		SIGPIPE		38			/* write to pipe with no readers */
#define		SIGUSR1		39			/* user signal #1 */
#define		SIGUSR2		40			/* user signal #2 */

#define _SIG_ERR	-1
#define	SIG_ERR		((void (*)())_SIG_ERR)

/* atomic signal typedef */
typedef unsigned int	sig_atomic_t;

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(_OS9THREAD)
#ifndef _PTHREAD_T
#ifndef _PTHREAD_T_TYPE
#define _PTHREAD_T_TYPE void *
#endif
typedef _PTHREAD_T_TYPE	pthread_t;
#define _PTHREAD_T
#endif
#endif

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void (*signal _OP((int, void (*)(int)))) _OP((int));
int raise _OP((int));

#if defined(_OS9THREAD)
int 	pthread_kill _OP((pthread_t _p_thread, int _p_sig));
#endif

#if defined(__cplusplus)
error_code _os_intercept(void (*)(signal_code), void *);
#else
error_code _os_intercept(void (*)(), void *);
#endif
error_code _os_rte(void);
error_code _os_send(process_id, signal_code);
error_code _os_sigmask(int32);
error_code _os_sigreset(void);
error_code _os_siglngj(void *, u_int16);

#if defined(_OSK)
error_code _os9_sleep(u_int32 *);
#endif

#if defined(_OS9000)
error_code _os_clrsigs(process_id);
error_code _os_sigrs(u_int32);
error_code _os_sleep(u_int32 *, signal_code *);
#endif

#if defined(_OPT_PROTOS)
int intercept _OP((void (*)(int)));
int kill _OP((int, int));
int pause _OP((void));
int sigmask _OP((int));
int sleep _OP((unsigned int));
int tsleep _OP((unsigned int));
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

/* special addresses */
#define _SIG_DFL	0
#define _SIG_IGN	1
#define	SIG_DFL		((void (*)())_SIG_DFL)
#define	SIG_IGN		((void (*)())_SIG_IGN)


/* Service-specific signal codes */

#define SIG_1394BUSRESET      25   /* IEEE1394 Bus Reset in progress */
#define SIG_1394BUSRESET_DONE 26   /* IEEE1394 Bus Reset complete    */


#endif /* _SIGNAL_H */
