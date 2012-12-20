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
!   1 92-03-17	Created                                                 rcb !
!	2 93-01-05	Added different FLT_ROUNDS for 387						rcb !
!	3 97-06-02	Fixed constants DBL_MAX & LDBL max over legal range		tag !
!   4 98-02-20  Added this header                                       nan !
!                                                                           !
`--------------------------------------------------------------------------*/

/* ANSI/IEEE for single and double precision normalized numbers */

#if !defined(_FLOAT_H)
#define _FLOAT_H

#define	FLT_RADIX		2

#if defined(_FPF387)
#define FLT_ROUNDS		0
#endif

#if defined(_FPF881) || defined(_FPFPOWERPC) || defined(_FPFCMX)  || defined(_FPFMIPS) || defined(_FPFARM) || defined(_FPFSPARC) || defined(_FPFSH) || defined(_FPFSH4) || defined(_FPFSH5M)
#define FLT_ROUNDS		1
#endif

#define	FLT_MANT_DIG	24
#define	FLT_EPSILON		1.119209290e-07f
#define	FLT_DIG			6
#define	FLT_MIN_EXP		-125
#define	FLT_MIN			1.17549435e-38f
#define	FLT_MIN_10_EXP	-37
#define	FLT_MAX_EXP		128
#define	FLT_MAX			3.40282346e+38f
#define	FLT_MAX_10_EXP	38
#define	DBL_MANT_DIG	53
#define	DBL_EPSILON		2.2204460492503131e-16
#define	DBL_DIG			15
#define	DBL_MIN_EXP		-1021
#define	DBL_MIN			2.2250738585072016e-308
#define	DBL_MIN_10_EXP	-307
#define	DBL_MAX_EXP		1024
#define	DBL_MAX			1.7976931348623156e308
#define	DBL_MAX_10_EXP	308
#define	LDBL_MANT_DIG	53
#define	LDBL_EPSILON		2.2204460492503131e-16
#define	LDBL_DIG			15
#define	LDBL_MIN_EXP		-1021
#define	LDBL_MIN			2.2250738585072016e-308
#define	LDBL_MIN_10_EXP	-307
#define	LDBL_MAX_EXP		1024
#define	LDBL_MAX			1.7976931348623156e308
#define	LDBL_MAX_10_EXP	308

/* Extensions */
#define _DBL_MAX_FRAC		.17976931348623157
#define	_DBL_MIN_FRAC		.22250738585072016
#define _DBL_EXP_BIAS		1023
#define _DBL_MAX_BIAS_EXP	2047

#endif /* _FLOAT_H */
