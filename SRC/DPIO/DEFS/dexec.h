#if !defined(_DEXEC_H)
#define _DEXEC_H

/*
 * $Header:   /h0/OS9000/SRC/DEFS/VCS/dexec.h_v   1.1   22 Sep 1993 13:57:04   robb  $
 * $Revision:   1.1  $
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
!   1 87/02/23 Created.													rg  !
!   2 95/08/28 Added C++ Support										dwj	!
`--------------------------------------------------------------------------*/


#if !defined(_TYPES_H)
#include <types.h>
#endif

/* dexec() - debug modes */
typedef enum {
	DBG_M_INACTV,				/* inactive mode (used by kernel) */
	DBG_M_HARD,					/* hard break points / full speed execution	*/
	DBG_M_SOFT,					/* soft break points / continuous execution */
	DBG_M_COUNT,				/* execute count instructions				*/
	DBG_M_CONTROL				/* execute until change of control (future)	*/
} dexec_mode;

/* dexec() return status */
typedef enum {
	DBG_S_FINISH,				/* command finished successfully			*/
	DBG_S_BRKPNT,				/* process hit break point					*/
	DBG_S_EXCEPT,				/* execption occurred during execution 		*/
	DBG_S_CHILDSIG,				/* process received signal (no intercept)	*/
	DBG_S_PARENTSIG,			/* debugger received signal					*/	
	DBG_S_CHAIN,				/* process has made an F$Chain call			*/
	DBG_S_EXIT,					/* process has made an F$Exit call			*/
	DBG_S_CONTROL,				/* process executed a jmp or bra (future)	*/
	DBG_S_WATCH					/* process hit watch point (future)			*/
} dexec_status;

#if defined(_ANSI_EXT) || defined(__STDC__)

#if !defined(_REG_H)
#if defined(_MPF68K)
#	include <reg68k.h>
#else
#	include <reg386.h>
#endif
#endif

#if !defined(_MODULE_H)
#include <module.h>
#endif

/*	C++ Support	*/
#ifdef __cplusplus
extern "C" {
#endif

error_code _os_dexec(process_id, dexec_mode, void *, u_int32 *, u_int32, void **, u_int32 *, dexec_status *, u_int32 *, void **, error_code *);
error_code _os_dexit(process_id);
error_code _os_dfork(u_int32, u_int32, char *, void *, u_int32, u_int32, process_id *, u_int32, regs *, fregs *);
error_code _os_dforkm(u_int32, u_int32, mh_com *, void *, u_int32, u_int32, process_id *, regs *, fregs *);

/*	End C++	*/
#ifdef __cplusplus
}
#endif

#endif

#endif

