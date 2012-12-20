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
!	2 97-03-14	Altered offsetof so it can handle constant structures	tsb !
!   3 98-02-20  Added this header                                       nan !
!                                                                           !
`--------------------------------------------------------------------------*/

/* ANSI - <stddef.h> */

#if !defined(_STDDEF_H)
#define _STDDEF_H

#include "ansi_c.h"

#if defined(_SIZE_T)	
typedef _SIZE_T				size_t;
#undef _SIZE_T
#endif /* _SIZE_T */

#if defined(_WCHAR_T) && !defined(__cplusplus)
typedef _WCHAR_T			wchar_t;
#undef _WCHAR_T
#endif /* _WCHAR_T */

typedef _PTRDIFF_T			ptrdiff_t;

#define offsetof(type, mem_desig) ( \
  (size_t)((char *)&((type *)0)->mem_desig - (char *)0 ) )

#endif /* _STDDEF_H */
