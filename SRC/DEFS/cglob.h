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
!   1 93-11-16  Created                                                 rry !
!	2 93-11-17	Took out usage of sysglob.h for OS-9					rry !
!	3 95-05-29	Added support for C++, generic global data pointer for		!
!				CSL and renamed a global data pointer					rry !
!   4 98-02-20  Added this header                                       nan !
!   5 00-02-21  Thread support added                                rry/ajk !
!                                                                           !
`--------------------------------------------------------------------------*/

#if !defined(_CGLOB_H)

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if !defined(_MODULE_H)
#include <module.h>
#endif

#if !defined(_STDIO_H)
#include <stdio.h>
#endif

#if defined(_OS9THREAD)

#ifndef _PTHREAD_H
#include <pthread.h>
#endif

/* thread local "global" variables */
#define	_procid		(*(u_int32 *)_pthread_local_slot(_PTHREAD_PROCID))
#define _stbot		(*(void **)_pthread_local_slot(_PTHREAD_STBOT))
#define	_sttop		(*(void **)_pthread_local_slot(_PTHREAD_STTOP))
#define	_mtop		(*(void **)_pthread_local_slot(_PTHREAD_MTOP))
#define _maxstack	(*(u_int32 *)_pthread_local_slot(_PTHREAD_MAXSTACK))
#define _stklimit	(*(int32 *)_pthread_local_slot(_PTHREAD_STKLIMIT))

#define _srqsiz		(*(u_int32 *)_pthread_local_slot(_PTHREAD_SRQSIZ))
#define _srqcsiz	(*(u_int32 *)_pthread_local_slot(_PTHREAD_SRQCSIZ))

#define _mainid		(*(process_id *)_pthread_global_slot(_PTHREAD_MAINID))

#else /* defined(_OS9THREAD) */

extern void		*_sttop,		/* stack top */
				*_mtop,			/* memory top (minimum stack value */
				*_stbot;		/* worst stack pointer so far */
extern u_int32	_procid,		/* process ID for process */
				_maxstack;		/* maximum depth the stack has reached */
extern int32	_stklimit;		/* amount of verified area left on stack */

#define _mainid	_procid			/* equivalent to _procid */

#endif /* !defined(_OS9THREAD) */


extern void
#if defined(_OSK)
	 			*_csl_a6;		/* global pointer for attached csl */
#endif
#if defined(_OS9000)
				*_csl_glob;		/* global pointer for attached csl */
#endif
extern u_int32	_totmem,		/* data size process was forked with */
				_sbsize;		/* sbrk - size of process memory */
extern u_int16	_pathcnt;		/* number of paths process was forked with */
#if defined(_OSK)
extern int		_sysglob;		/* pointer to system globals for system state process */
#endif
extern mh_exec	*_modhead;		/* pointer to process' module */
extern char		*_modname;		/* current module name */
extern void		*_glob_data;	/* global static storage pointer */
extern FILE		(*_fcbs)[];		/* pointer to _niob array */
#if (defined(_OSK) || defined(_MPF386)) && !defined(_OS9THREAD) && !defined(_OFCBS_DEFINED)
extern void		*_ofcbs;		/* pointer to compatibility _iob array */
#endif
extern char		**_environ;		/* environment pointer */
#if !defined(_ANSI_EXT) && !defined(__STDC__) && !defined(__cplusplus)
extern char		**environ;		/* environment pointer */
#endif

#define _CGLOB_H

#endif
