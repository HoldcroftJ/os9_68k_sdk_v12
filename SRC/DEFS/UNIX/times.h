/*--------------------------------------------------------------------------,
! edition history                                                           !
!   #   date     comments                                               by  !
!  -- -------- -------------------------------------------------------- --- !
!   1 95-05-10  Created                                                 rcb !
!	2 96-05-12	Optional prototypes added								rry !
!	3 97-04-08	Prototypes now defined when compiling C++				tag !
!   4 98-02-20  Added this header                                       nan !
!                                                                           !
`--------------------------------------------------------------------------*/

/*
 * Structure returned by times()
 */
#if !defined(_TIMES_H)
#define	_TIMES_H

#ifndef ctime
#include <time.h>
#endif

struct tms {
	clock_t	tms_utime;		/* user time */
	clock_t	tms_stime;		/* system time */
	clock_t	tms_cutime;		/* user time, children */
	clock_t	tms_cstime;		/* system time, children */
};

#if defined(__cplusplus)
extern "C" {
#	define _OPT_PROTOS		/* C++ needs prototypes */
#endif /* __cplusplus */

#if defined(_OPT_PROTOS)
#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#else
#define _OP(x) ()
#endif


int times _OP((struct tms *));


#undef _OP
#endif /* _OPT_PROTOS */

#if defined(__cplusplus)
}
#	undef _OPT_PROTOS
#endif /* __cplusplus */

#endif /* _TIMES_H */
