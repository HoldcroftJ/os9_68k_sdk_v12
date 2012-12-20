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
!	2 96-02-25	Now handles 64-bit longs								tsb !
!	3 96-03-07	Included LONGLONG support for EXT_ANSI					tsb !
!	4 97-04-21	New casts for SHRT_MIN, INT_MIN, LONG_MIN, LONGLONG_MIN	tag !
!   5 98-02-20  Added this header                                       nan !
!   6 01-05-11  Took out temporary definitions for compiler             rry !
!				Changed _MPFMIPS to _MPFMIPS64								!
!   7 01-06-17  Adding support for SH-5m								rry !
!                                                                           !
`--------------------------------------------------------------------------*/

/* ANSI - <limits.h> */

#if !defined(_LIMITS_H)
#define	_LIMITS_H

#define	CHAR_BIT		8
#define	SCHAR_MIN		-128
#define	SCHAR_MAX		127
#define	UCHAR_MAX		255
#define	CHAR_MIN		SCHAR_MIN
#define	CHAR_MAX		SCHAR_MAX
#define	MB_LEN_MAX		2
#define	SHRT_MIN		-32768
#define	SHRT_MAX		32767
#define	USHRT_MAX		65535
#define	INT_MIN			LONG_MIN
#define	INT_MAX			LONG_MAX
#define	UINT_MAX		ULONG_MAX
#define	LONG_MIN		(-2147483647-1)
#define	LONG_MAX		2147483647
#define	ULONG_MAX		4294967295

#if defined(_ANSI_EXT) && (defined(_MPFMIPS64) || defined(_MPFSH5M))
#define LONGLONG_MIN    (-9223372036854775807LL-1LL)
#define LONGLONG_MAX    9223372036854775807LL
#define ULONGLONG_MAX   18446744073709551615LLU
#endif

#endif /* _LIMITS_H */
