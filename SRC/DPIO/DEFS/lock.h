#if !defined(_LOCK_H)
#define _LOCK_H

/*
 * $Header:   /h0/MWOS/SRC/DPIO/DEFS/VCS/lock.h_v   1.0   01 Nov 1994 14:49:54   BrentT  $
 * $Revision:   1.0  $
 */

/*--------------------------------------------------------------------------,
!                                                                           !
!              Copyright 1989 by Microware Systems Corporation              !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!---------------------------------------------------------------------------!
!                                                                           !
!  Edition History:                                                         !
!   #   Date    Comments                                                By  !
!  -- -------- -------------------------------------------------------- --- !
!   1 87/02/23 Began modifications for Portable OS9                     DJN !
!     89/02/21 Made usable by kernel (locks moved into kernel).			afh !
!   2 95/08/28 Added C++ Support										dwj	!
!																			!
`--------------------------------------------------------------------------*/

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if !defined(_PROCESS_H)
#include <process.h>
#endif


/* Kernel lock structure definition */
typedef struct lock_desc *lock_id;
typedef struct lock_desc {
	lock_id
		l_id;					/* lock identifier */
	Pr_desc
		l_owner,				/* process id of current owner */
		l_lockqn,				/* next process in lock list */
		l_lockqp;				/* previous process in lock list */
} lk_desc, *Lk_desc;

#if defined(_ANSI_EXT) || defined(__STDC__)

/*	C++ support	*/
#ifdef __cplusplus
extern "C" {
#endif

error_code _os_acqlk(lk_desc *, signal_code *);
error_code _os_caqlk(lk_desc *);
error_code _os_crlk(lk_desc **);
error_code _os_dellk(lk_desc *);
error_code _os_rellk(lk_desc *);
error_code _os_waitlk(lk_desc *, signal_code *);

/*	End C++	*/
#ifdef __cplusplus
}
#endif

#endif

#endif
