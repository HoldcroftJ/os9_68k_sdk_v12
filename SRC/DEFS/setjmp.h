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
!	2 92-06-24	Changes for _ANSI_EXT preprocessor symbol				rry !
!	3 95-02-06	New conditionals for C++ support						rcb !
!   4 98-02-20  Added this header                                       nan !
!                                                                           !
`--------------------------------------------------------------------------*/

/* Jump register save buffer */
#if !defined(_SETJMP_H)
#define _SETJMP_H

#if defined(_OSK)

typedef int jmp_buf[16];		/* for 68000 */

#elif defined (_OS9000)

#	if defined(MPU68k) || defined(_MPF68K)
	typedef int jmp_buf[16];		/* for 68000 */
#	endif

#	if defined(MPU386) || defined(_MPF386)
	typedef int jmp_buf[8];			/* for 80386 */
#	endif

#	if defined(_MPFPOWERPC)
	typedef int jmp_buf[24];		/* for PowerPC */
#	endif

#	if defined(_MPFSH5M)
	typedef long long jmp_buf[38];	/* for SH-5media (64-bit) */
#	endif

#	if defined(_MPFMIPS)
		typedef int jmp_buf[24];	/* for MIPS */
#	endif

#	if defined(_MPFARM)
	typedef int jmp_buf[10];		/* for ARM processor (non-thumb) */
#	endif

#	if defined(_MPFSHSIM) || defined(_MPFSH)
	typedef int jmp_buf[9];		/* for SHSIM processor (for now) */
#	endif

#	if defined(_MPFSPARC)
	typedef int jmp_buf[18];		/* for SPARC processor */
#	endif

#endif

#if !defined(__STDC__) && !defined(_ANSI_EXT) && !defined(__cplusplus)
extern int __setjmp();
extern void longjmp();
#else
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
int __setjmp(jmp_buf);
void longjmp(jmp_buf, int);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* __STDC__ */

#define setjmp(env)		__setjmp((env))

#endif /* _SETJMP_H */
