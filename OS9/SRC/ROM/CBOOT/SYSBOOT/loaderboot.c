/*--------------------------------------------------------------------------!
! loaderboot.c: RAM loaded boot driver										!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 89/01/04 Hacked and translated from "sysboot.a"					jal	!
!               ---- OS-9/68K V2.4 released ----                            !
!               ---- OS-9/68K V3.0 released ----                            !
!               ---- OS-9/68K V3.1 released ----                            !
!---------------------------------------------------------------------------!
!                                                                           !
!              Copyright 1990 by Microware Systems Corporation              !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include	<sysboot.h>

/*----------------------------------------------------------------------!
! loaderboot:  This driver allows the debugging of downloaded code		!
! which	will do the actual system boot.  The routines below are called	!
! to enter and debug downloaded system boot code.						!
+----------------------------------------------------------------------*/
error_code loaderboot()
{
	register error_code	(*jmpaddr)(); /* address of boot routine to execute	*/
	register error_code	error = SUCCESS;

	for (;;) {
		outstr("Assuming you are testing a boot loader.\n");
		outstr("Please enter its entry address (in hex): $");
		jmpaddr = gethexaddr();
		if ((error_code) jmpaddr == ERROR2)
			error = E_NOTRDY;
			break;
		if ((error_code) jmpaddr != ERROR)
			break;
		outstr("\n\nYou MUST enter an address!!\n\(Type 'Q' to quit.\)\n");
	}
	if (error == SUCCESS) {
		error = call_any_bdriver(jmpaddr);
	}
	return error;
}
