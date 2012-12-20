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
!	2 95-02-06	Added conditionals for C++								rcb !
!	3 96-05-12	Optional prototypes added								rry !
!   4 98-02-20  Added this header                                       nan !
!                                                                           !
`--------------------------------------------------------------------------*/

/* ANSI - <string.h> */

#if !defined(_STRING_H)
#define _STRING_H

#include "ansi_c.h"

#if defined(_SIZE_T)
typedef _SIZE_T	size_t;
#undef _SIZE_T
#endif /* _SIZE_T */

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void		*memchr _OP((const void *, int, size_t));
int			memcmp _OP((const void *, const void *, size_t));
void		*memcpy _OP((void *, const void *, size_t));
void		*memmove _OP((void *, const void *, size_t));
void		*memset _OP((void *, int, size_t));
char		*strcat _OP((char *, const char *));
char		*strchr _OP((const char *, int));
char		*strerror _OP((int));
int			strcoll _OP((const char *, const char *));
int			strcmp _OP((const char *, const char *));
char		*strcpy _OP((char *, const char *));
size_t		strcspn _OP((const char *, const char *));
size_t		strlen _OP((const char *));
char		*strncat _OP((char *, const char *, size_t));
int			strncmp _OP((const char *, const char *, size_t));
char		*strncpy _OP((char *, const char *, size_t));
char		*strpbrk _OP((const char *, const char *));
char		*strrchr _OP((const char *, int));
size_t		strspn _OP((const char *, const char *));
char		*strstr _OP((const char *, const char *));
char		*strtok _OP((char *, const char *));
size_t 		strxfrm _OP((char *, const char *, size_t));

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _STRING_H */
