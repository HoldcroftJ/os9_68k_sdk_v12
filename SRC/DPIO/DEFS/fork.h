#if !defined(_FORK_H)
#define _FORK_H

/*
 * $Header:   /h0/MWOS/SRC/DPIO/DEFS/VCS/fork.h_v   1.0   01 Nov 1994 14:49:38   BrentT  $
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
!   1 87/07/07 Created                                                  rg  !
!                                                                           !
`--------------------------------------------------------------------------*/


#if !defined(_TYPES_H)
#include <types.h>
#endif

#if !defined(_PROCESS_H)
#include <process.h>
#endif

#if !defined(_MODULE_H)
#include <module.h>
#endif

/* parameter block for forked process */

typedef struct fork_params {
	process_id
		proc_id;		/* process ID */
	owner_id
		owner;			/* group/user ID */
	prior_level
		priority;		/* process priority */
	u_int16
		path_count;		/* number of I/O paths inherited */
	u_int32
		param_size,		/* size of parameters */
		mem_size;		/* total initial memory allocation */
	u_char
		*params,		/* paramter pointer */
		*mem_end;		/* top of memory pointer */
	Mh_com
		mod_head;		/* primary module pointer */
} fork_params, *Fork_params;

#endif

