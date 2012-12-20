#if !defined(_ANSI_H)
#define _ANSI_H

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
!   1 92/03/17	Created													rcb !
!	2 92/05/22	Changed _VA_LIST to be void *							rcb !
!	3 95/02/06	Added conditionals for C++								rcb !
!	4 96/04/08	Modified _SIZE_T and _TIME_T							ra  !
!	5 97/02/28	_WCHAR_T no longer #defined when compiling C++ code		tag !
!	6 98/02/20	Added this edition history								nan !
!                                                                           !
`--------------------------------------------------------------------------*/

/* the _LONG_64 is only for testing purposes */
#if defined(_LONG_64) && ( defined(_MPFCMX) || defined(_MPFMIPS) )
#define _PTRDIFF_T	signed int
#define _SIZE_T		unsigned int
#define _TIME_T		unsigned int
#else
#define _PTRDIFF_T	long
#define _SIZE_T		unsigned long
#define _TIME_T		unsigned long
#endif

#if !defined(__cplusplus)
#define _WCHAR_T	unsigned int
#endif
#define _CLOCK_T	unsigned long
#define _VA_LIST	unsigned char *

#if defined(NULL)
#undef NULL
#endif

#if defined(__cplusplus)
#define NULL            0
#else
#define NULL		(void *)0
#endif

#endif /* _ANSI_H */

