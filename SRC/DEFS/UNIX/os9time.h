/*--------------------------------------------------------------------------,
! edition history                                                           !
!   #   date     comments                                               by  !
!  -- -------- -------------------------------------------------------- --- !
!   1 95-05-10  Created                                                 rcb !
!	2 96-05-12	Added optional prototypes								rry !
!	3 97-04-08	Prototypes now defined when using C++					tag !
!   4 98-02-20  Added this header                                       nan !
!                                                                           !
`--------------------------------------------------------------------------*/

/*
 * OS-9/OS-9000 implementation specific code, enhancements and modifications:
 *
 * Copyright 1991 Microware Systems Corporation
 * All Rights Reserved
 *
 * Use and/or distribution subject to terms and conditions of applicable
 * Microware license agreement(s).
 *
 * Current Version: @(#)os9time.h	1.3 12/23/91
 */

#if !defined(_OS9TIME_H)
#define _OS9TIME_H

/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */
struct timeval {
	long	tv_sec;		/* seconds */
	long	tv_usec;	/* and microseconds */
};

struct timezone {
	int	tz_minuteswest;	/* minutes west of Greenwich */
	int	tz_dsttime;	/* type of dst correction */
};
#define	DST_NONE	0	/* not on dst */
#define	DST_USA		1	/* USA style dst */
#define	DST_AUST	2	/* Australian style dst */
#define	DST_WET		3	/* Western European dst */
#define	DST_MET		4	/* Middle European dst */
#define	DST_EET		5	/* Eastern European dst */

/*
 * Operations on timevals.
 *
 * NB: timercmp does not work for >= or <=.
 */
#define	timerisset(tvp)		((tvp)->tv_sec || (tvp)->tv_usec)
#define	timercmp(tvp, uvp, cmp)	\
	((tvp)->tv_sec cmp (uvp)->tv_sec || \
	 (tvp)->tv_sec == (uvp)->tv_sec && (tvp)->tv_usec cmp (uvp)->tv_usec)
#define	timerclear(tvp)		(tvp)->tv_sec = (tvp)->tv_usec = 0

/*
 * Names of the interval timers, and structure
 * defining a timer setting.
 */
#define	ITIMER_REAL	0
#define	ITIMER_VIRTUAL	1
#define	ITIMER_PROF	2

struct	itimerval {
	struct	timeval it_interval;	/* timer interval */
	struct	timeval it_value;	/* current value */
};

#if defined(__cplusplus)
extern "C" {
#	define _OPT_PROTOS				/* C++ needs prototypes */
#endif /* __cplusplus */

#if defined(_OPT_PROTOS)
#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#else
#define _OP(x) ()
#endif


int gettimeofday _OP((struct timeval *, struct timezone *));

#undef _OP

#endif /* _OPT_PROTOS */

#if defined(__cplusplus)
}
#	undef _OPT_PROTOS
#endif /* __cplusplus */

#endif /* _OS9TIME_H */
