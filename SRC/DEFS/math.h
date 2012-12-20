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
!	2 92-06-24	Changes for _ANSI_EXT pre-processor symbol				rry !
!	3 92-08-18	Added macros that produce inline code for math funcs	rcb !
!	4 93-07-27	Removed everything associated with __inline				rcb !
!	5 95-02-06	C++ support added										rcb !
!	6 96-05-12	Reorganized, added optional prototypes					rry !
!   7 98-02-20  Added this header                                       nan !
!                                                                           !
`--------------------------------------------------------------------------*/

/* ANSI - math.h */

#if !defined(_MATH_H)
#define _MATH_H

#if defined(_UCC)
extern union
{
	int int_value[2];
	double dbl_value;
} __huge_val;
#define HUGE_VAL		__huge_val.dbl_value
#else
extern int		__huge_val[2];
#define HUGE_VAL		*((double *)&__huge_val[0])
#endif /* _UCC */

#if !defined(__STDC__) && !defined(__cplusplus)
#define HUGE			1.701411733192644270e38
#define PI				3.141592653589793
#endif

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

extern double frexp _OP((double, int *));
extern double ldexp _OP((double, int));
extern double acos _OP((double));
extern double asin _OP((double));
extern double atan _OP((double));
extern double cos _OP((double));
extern double sin _OP((double));
extern double tan _OP((double));
extern double exp _OP((double));
extern double log _OP((double));
extern double log10 _OP((double));
extern double modf _OP((double, double *));
extern double pow _OP((double, double));
extern double sqrt _OP((double));
extern double ceil _OP((double));
extern double fabs _OP((double));
extern double floor _OP((double));
extern double atan2 _OP((double, double));
extern double cosh _OP((double));
extern double sinh _OP((double));
extern double tanh _OP((double));
extern double fmod _OP((double, double));

#if !defined(__STDC__) && !defined(__cplusplus)
extern double hypot _OP((double, double));
#endif /* ANSI extended or backwards compatible modes */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _MATH_H */
