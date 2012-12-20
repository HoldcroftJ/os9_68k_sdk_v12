/*
 * Copyright 1990, 1993 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

/*----------------------------------------------------------------------!
! SysBoot entry point assembler glue code								!
+-----------------------------------------------------------------------+
! Calling conventions for disk boot subroutine ("SysBoot"):				!
!																		!
! Passed:	(a1) = Boot ROM entry point									!
!			(a3) = Port address from DiskPort							!
!			(a4) = system free RAM list									!
!			(a5) = exception jump tbl pointer							!
!			(a6) = operating system global data ptr (4k scratch memory)	!
!			(a7) = system ROM map (SysBoot has return addr first)		!
!																		!
! Returns:	(a0) = ptr to an executable module with a valid header		!
!			(a4) = possibly updated free RAM list						!
!			(a5) = must be intact										!
!			(a7) = possibly updated system ROM list						!
!			(cc) = carry set if an error occurred						!
!			(d1.w) = error status if bootstrap failed					!
!----------------------------------------------------------------------*/
#include	<sysboot.h>

u_int32	rombug = ROMBUGSYNC;	/* global variable to identify debugger	*/

#ifdef _UCC
_asm(" use sbtglueasm.a");
#else
@ use sbtglueasm.a
#endif
