#if !defined(_IOEDT_H)
#define _IOEDT_H

/*
 * $Header:   /h0/MWOS/SRC/DPIO/DEFS/VCS/ioedt.h_v   1.0   01 Nov 1994 14:49:50   BrentT  $
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
!																			!
!  Edition History:                                                         !
!   #   Date    Comments                                                By  !
!  -- -------- -------------------------------------------------------- --- !
!   1 90/04/03 Created.													rg	!
!   2 95/08/28 Added C++ support										dwj	!
!                                                                           !
`--------------------------------------------------------------------------*/

/* File Manager - Device Driver interface edition numbers. */
#define EDT_SCF		1	/* Sequential Character File Manager */
#define EDT_RBF		1	/* Random Block File Manager */
#define EDT_PIPE 	1	/* Pipe File Manager */
#define EDT_SBF		1	/* Sequential Block File Manager */
#define EDT_NFM		1	/* OS9-Network File Manager */
#define EDT_CDFM	1	/* Compact disc File Manager */
#define EDT_UCM		1	/* User communication File Manager */
#define EDT_SOCK	1	/* Socket communication File Manager */
#define EDT_PTTY	1	/* Pseudo-keyboard File Manager */
#define EDT_GFM		1	/* Graphics file File Manager */

#if defined(_ANSI_EXT) || defined(__STDC__)

#if !defined(_TYPES_H)
#include <types.h>
#endif

/*	C++ support	*/
#ifdef __cplusplus
extern "C" {
#endif

error_code _os_gs_edt(path_id, u_int32 *);

/*	End C++	*/
#ifdef __cplusplus
}
#endif

#endif

#endif
