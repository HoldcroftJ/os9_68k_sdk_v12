#if !defined(_SVCTBL_H)
#define _SVCTBL_H

/*
 * $Header:   /h0/MWOS/SRC/DPIO/DEFS/VCS/svctbl.h_v   1.0   01 Nov 1994 14:50:24   BrentT  $
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
!   1 88/04/25 Created                                                  afh !
!   2 95/08/28 Added C++ Support										dwj	!
!                                                                           !
`--------------------------------------------------------------------------*/


#if !defined(_TYPES_H)
#include <types.h>
#endif

#define USER_STATE 		1		/* user state service routine flag */
#define SYSTEM_STATE 	2		/* system state service routine flag */

/* service routine initialization table stucture. */
typedef struct  {
	u_int16 
		fcode;
	u_int32
		(*service)();
} svctbl, *Svctbl;

#if defined(_ANSI_EXT) || defined(__STDC__)

/*	C++ Support	*/
#ifdef __cplusplus
extern "C" {
#endif

error_code _os_setsvc(u_int32, u_int32, void *, void *);

/*	End C++	*/
#ifdef __cplusplus
}
#endif

#endif

#endif

