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
!	2 92-07-01	Changes to macros to round size & ensure type matching	rcb !
!	3 93-11-16	Added prototype for __inline_va_start__					rry !
!	4 95-02-06	C++ conditionals added									rcb !
!   5 98-02-20  Added this header                                       nan !
!   6 01-04-04  Added support for 64-bit on PowerPC						rry !
!   7 01-04-17  Adding ARM to 64-bit integer processors					rry !
!   8 01-06-17  Adding support for SH-5m								rry !
!                                                                           !
`--------------------------------------------------------------------------*/

#if !defined(_STDARG_H)
#define _STDARG_H

#include "ansi_c.h"

typedef _VA_LIST va_list;

#define va_start(va, paranm) (void)((va) = (_VA_LIST) __inline_va_start__())

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void *__inline_va_start__(void);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#else
void *__inline_va_start__();
#endif

#if defined(_MPFMIPS) || defined(_MPFSPARC) || defined(_MPFSH5M)
#define va_arg(va, type) (((type *)(va = (_VA_LIST)((((int)va) + sizeof(type) \
	+ ((sizeof(type) > 4) ? 7 : 3)) & ((sizeof(type) > 4) ? ~7 : ~3))))[-1])
#elif defined(_MPFSH4)
#define va_arg(va, type) (*(type *)((va += ((sizeof(type) + 7) & ~7)) - \
	((sizeof(type) + 7) & ~7)))
#elif defined(_MPFPOWERPC) || defined(_MPFARM)
#define va_arg(va, type) (*(type *)((va += ((sizeof(type) + (sizeof(type) > 4 ? 7 : 3)) & ~(sizeof(type) > 4 ? 7 : 3))) - \
	((sizeof(type) + (sizeof(type) > 4 ? 7 : 3)) & ~(sizeof(type) > 4 ? 7 : 3))))
#else
#define va_arg(va, type) (*(type *)((va += ((sizeof(type) + 3) & ~3)) - \
	((sizeof(type) + 3) & ~3)))
#endif

#define va_end(va) (void)((va) = (_VA_LIST)0)

#endif /* _STDARG_H */
