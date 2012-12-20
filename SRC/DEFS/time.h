/*--------------------------------------------------------------------------,
!                                                                           !
!               Copyright 1998 by Microware Systems Corporation             !
!                           Reproduced Under License                        !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!---------------------------------------------------------------------------!
!                                                                           !
! edition history                                                           !
!   #   date     comments                                               by  !
!  -- -------- -------------------------------------------------------- --- !
!   1 92-03-17  Created                                                 rcb !
!	2 92-06-25	Prototypes added										rcb !
!	3 93-12-06	Included _JULBASE for ANSI & extended ANSI applications	rry !
!	4 96-05-13	Added optional prototypes; changed sign of time buffs	rry !
!   5 98-02-20  Added this header                                       nan !
!	6 98-02-25	Moved _os_julian out of OSK block to avoid warnings		nan !
!   7 00-09-18  Moved timespec from pthread.h to here as per POSIX spec	rry !
!                                                                           !
`--------------------------------------------------------------------------*/

/*
 * time.h -- time-related stuff
 *
 * We include both the OS-9 time structure and the ANSI time structures,
 * to try to make everybody happy.  There may be some name clashes
 * induced thereby, however.
 */
#if !defined(_TIME_H)
#define _TIME_H

#include "ansi_c.h"

#if defined(_SIZE_T)
typedef _SIZE_T	size_t;
#undef _SIZE_T
#endif /* _SIZE_T */

/* structure for the 'setime()' and 'getime()' calls */

typedef struct _time_buf
{
	unsigned char    t_year,
					 t_month,
					 t_day,
					 t_hour,
					 t_minute,
					 t_second;
} _time_buf;

typedef _CLOCK_T	clock_t;
typedef _TIME_T		time_t;

/* X3J11 Draft says *nowhere* that CLOCKS_PER_SEC is a constant! */

#define CLOCKS_PER_SEC	__clocks_per_sec()

struct tm {
	int	tm_sec;		/* seconds after the minute -- [0, 59]  */
	int	tm_min;		/* minutes after the hour   -- [0, 59]  */
	int	tm_hour;	/* hours since midnight		-- [0, 23]  */
	int	tm_mday;	/* day of the month			-- [1, 31]  */
	int	tm_mon;		/* months since January		-- [0, 11]  */
	int	tm_year;	/* years since 1900						*/
	int	tm_wday;	/* days since Sunday		-- [0, 6]   */
	int	tm_yday;	/* days since 1 January		-- [0, 365] */
	int	tm_isdst;	/* Daylight Savings Time flag			*/
					/* 0 = no, 1 = yes, -1 = unknown		*/
};

#if !defined(__STDC__) && !defined(__cplusplus)
struct sgtbuf {
	unsigned char    t_year,
					 t_month,
					 t_day,
					 t_hour,
					 t_minute,
					 t_second;
};

#define CLK_TCK	__clocks_per_sec()
#endif

#if defined(_OS9000)

#if !defined(__STDC__) && !defined(_ANSI_EXT) && !defined(__cplusplus)
#define JULBASE 2440587		/* julian date for Jan 1, 1970 */
#else
#define _JULBASE 2440587	/* julian date for Jan 1, 1970 */
#endif

#endif /* _OS9000 */

#if defined(__cplusplus) || defined(_ANSI_EXT) || defined(__STDC__)
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

time_t	mktime _OP((struct tm *));
time_t	time _OP((time_t *));
double	difftime _OP((time_t, time_t));
char	*asctime _OP((const struct tm *));
struct tm *gmtime _OP((const time_t *));
struct tm *localtime _OP((const time_t *));
char	*ctime _OP((const time_t *));
size_t	strftime _OP((char *, size_t, const char *, const struct tm *));
clock_t		clock _OP((void));

clock_t __clocks_per_sec _OP((void));

#if !defined(_TYPES_H)
#include <types.h>
#endif

error_code _os_getime _OP((time_t *, u_int32 *));
error_code _os_gregorian _OP((u_int32 *, u_int32 *));
error_code _os_setime _OP((time_t));
error_code _os_julian _OP((u_int32 *, u_int32 *));

#if defined(_OSK)
error_code _os9_getime _OP((u_int32, u_int32 *, u_int32 *, u_int16 *, u_int32 *));
error_code _os9_setime _OP((u_int32, u_int32));
#endif

#if defined(_OPT_PROTOS)
#if !defined(__STDC__) && !defined(__cplusplus)
int getime _OP((struct sgtbuf *));
int setime _OP((struct sgtbuf *));
#endif
int _gregorian _OP((int *, int *));
int _julian _OP((int *, int *));
time_t sys_mktime _OP((struct tm *));
int _sysdate _OP((int, int *, int *, short *, int *));
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#define	ctime(t)			asctime(localtime((t)))
#define difftime(t1, t0)	((double) ((long)(t1) - (long)(t0)))

#endif /* _TIME_H */

#if (defined(_POSIX_THREADS) || defined(_ANSI_EXT)) && !defined(_TIMESPEC_DEFINED)
/*
 * timespec structure for POSIX. This is outside the _TIME_H wrapper so
 * pthread.h can define _POSIX_THREADS and re-include time.h to get this if
 * it was missed on a previous inclusion.
 */
struct timespec {
  time_t	tv_sec;   	/* current time, in seconds */
  long		tv_nsec;    /* nanoseconds elapsed after tv_sec */
};

#define _TIMESPEC_DEFINED
#endif /* _POSIX_THREADS || _ANSI_EXT */

