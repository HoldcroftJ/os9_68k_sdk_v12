/*
 * Copyright 1990, 1991, 1993 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

/*----------------------------------------------------------------------!
! hw_probe: test for the existence of hardware at the passed in address	!
!           via bus trap handling										!
! C-syntax: error_code hw_probe(hdwe_addr)								!
!			register char	*hdwe_addr;									!
!																		!
! Passed: (d0) = Hardware address to be tested							!
! Returns: (d0) = SUCCESS or error code if error occurred				!
!----------------------------------------------------------------------*/
#ifdef OSK

/*----------------------------------------------!
! WARNING - THIS IS 68xxx SPECIFIC CODE			!
!----------------------------------------------*/
#ifdef _UCC
_asm("
 use ./hwprbasm.a
 ");
#else
@ use ./hwprbasm.a
#endif

#endif OSK
