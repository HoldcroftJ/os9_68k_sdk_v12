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
!	2 95-02-06	C++ support addeed										rcb !
!	3 96-05-14	Added optional prototypes (& cleaned up existing ones)	rry !
!   4 98-02-20  Added this header                                       nan !
!   5 01-03-01  Added const to parameters as appropriate                mgh !
!   6 01-04-26  Added missing _prealloc_rand prototype					rry !
!                                                                           !
`--------------------------------------------------------------------------*/

/* ANSI - <stdlib.h> */

#if !defined(_STDLIB_H)
#define _STDLIB_H

#include "ansi_c.h"

#if defined(_WCHAR_T) && !defined(__cplusplus)
typedef _WCHAR_T	wchar_t;
#undef _WCHAR_T
#endif /* _WCHAR_T */

#if defined(_SIZE_T)
typedef _SIZE_T		size_t;
#undef _SIZE_T
#endif /* _SIZE_T */

#define	EXIT_SUCCESS	0
#define EXIT_FAILURE	1

#define MB_CUR_MAX (_get_mb_len())

#define RAND_MAX	32767

typedef struct
{
	int quot;
	int rem;
} div_t;

typedef struct
{
	long int quot;
	long int rem;
} ldiv_t;

#if defined(__STDC__) || defined(_ANSI_EXT) || defined(__cplusplus)
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int						_get_mb_len _OP((void));
double					atof _OP((const char *));
int						atoi _OP((const char *));
long int				atol _OP((const char *));
double					strtod _OP((const char *, char **));
long int				strtol _OP((const char *, char **, int));
unsigned long int		strtoul _OP((const char *, char **, int));
int						rand _OP((void));
void					srand _OP((unsigned int));
void					*calloc _OP((size_t, size_t));
void					free _OP((void *));
void					*malloc _OP((size_t));
void					*realloc _OP((void *, size_t));
void					abort _OP((void));
int						atexit _OP((void (*)(void)));
void					exit _OP((int));
char					*getenv _OP((const char *));
int						system _OP((const char *));
void					*bsearch _OP((const void *, const void *, size_t,
							size_t, int (*)(const void *, const void *)));
void					qsort _OP((void *, size_t, size_t,
							int (*)(const void *, const void *)));
int						abs _OP((int));
div_t					div _OP((int, int));
long					labs _OP((long int));
ldiv_t					ldiv _OP((long int, long int));
int						mblen _OP((const char *, size_t));
int						mbtowc _OP((wchar_t *, const char *, size_t));
int						wctomb _OP((char *, wchar_t));
size_t					mbstowcs _OP((wchar_t *, const char *, size_t));
size_t					wcstombs _OP((char *, const wchar_t *, size_t));

#ifdef _OS9THREAD
#ifndef _TYPES_H
#include <types.h>
#endif

error_code				_prealloc_rand _OP((void));
#endif

#if defined(_OPT_PROTOS)
unsigned int _atou _OP((const char *));
int _dtoa _OP((double, char *, int, int, int *));
void *ebrk _OP((unsigned int));
void _exit _OP((int));
#if defined(__cplusplus) || defined(_ANSI_EXT) || defined(__STDC__)
int _freemem _OP((void));
int _stacksiz _OP((void));
#else
int freemem _OP((void));
int stacksiz _OP((void));
#endif
void _freemin _OP((int));
char *ibrk _OP((unsigned int));
void *_lcalloc _OP((unsigned long, unsigned long));
void _lfree _OP((void *, unsigned long));
void *_lmalloc _OP((unsigned long));
void *_lrealloc _OP((void *, unsigned long, unsigned long));
void _mallocmin _OP((unsigned int));
void *sbrk _OP((unsigned int));
#endif /* _OPT_PROTOS */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _STDLIB_H */
