/*--------------------------------------------------------------------------!
! syscon.c: System specific boot configuration routines for the MC68340BCC  !
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!   00 93/12/07 Created													arb	!
!				---- OS-9/68K V3.0.1 Release ----							!
!---------------------------------------------------------------------------!
!                                                                           !
!            Copyright 1993 by Microware Systems Corporation                !
!                        Reproduced Under License                           !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include <sysboot.h>
#include "systype.h"

extern error_code sysreset(), romboot();

#ifdef NOBUG
int errno;          /* normally defined by the debugger */
u_char trapflag;    /* .....  as above  .............   */
#endif

#ifdef _UCC
u_int32 _stklimit = 0x80000;  /* bit to limit _stkhandler calls
                                 from clib.l calls */
/*
 * Dummy _stkhandler routine for clib.l calls
 */
_stkhandler()
{
}
#endif

extern error_code sysreset();
char *nulstr = "";    /* single copy of a null string */

/*--------------------------------------------------------------!
! getbootmethod: This function allows the developer to select	!
! the booting method algorithm best suited for the system.	!
!--------------------------------------------------------------*/
int getbootmethod()
{
#if 1
	int sys_switches;
	extern int readswitches();  /* read patchable location */

	bdrivers_qty = 2;		    /* number of boot drivers */
	
	if( ((sys_switches = readswitches()) & ROMSWITCH) ) {
		/* rom boot switch over-rides all other switch settings */
		iniz_boot_driver(romboot, nulstr, nulstr, nulstr);
		iniz_boot_driver(sysreset, nulstr, nulstr, nulstr);
		vflag = FALSE;
		return AUTOSELECT;
	}

	/*----------------------------------------------------------------------!
	! Initialize the boot functions											!
	+-----------------------------------------------------------------------+
	! NOTE: The order of initialization determines the order of priority	!
	!		when using the "AUTOSELECT" booting method.						!
	!----------------------------------------------------------------------*/
	if (sys_switches & MENUSWITCH){
		iniz_boot_driver(romboot, "ROM", "Boot from ROM", "C");
		iniz_boot_driver(sysreset, nulstr, "Restart the system", "Q");
		vflag = TRUE;    /* turn on the error messages */
	
		return USERSELECT;    /* menu-driven user selectable */
	}

	if ((sys_switches & BOOTSWITCH) == 0) {
		/* boot selection is menu-driven user selectable */
		vflag = TRUE;
		return USERSELECT;
	} else {
		/* booting is auto-selected in the order of initialization. */
		vflag = TRUE;
		return AUTOSELECT;
	}
#else
    return AUTOSELECT;
#endif
}

/*----------------------------------------------------------------------!
! getboottype: When the boot method (determined by the above function)	!
! is set to SWITCHSELECT, this function allows the developer to select	!
! the actual booting type (device, ROM, etc...) according to hardware	!
! switches, non-volatile RAM or hard-code a single boot device type	!
!----------------------------------------------------------------------*/
Bdrivdef getboottype()
{
	return E_UNKSVC;    /* unused in this implementation (just in case..) */
}

/* rlm - unresolved symbol? */
void flush_cache()
{
}

