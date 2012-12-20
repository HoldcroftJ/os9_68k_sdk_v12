
/*
 * Copyright 1993, 1996 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */
         
/*--------------------------------------------------------------------------!
! syscon.c: Boot configuration routines from the OEM example target.		!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	01 93/10/28 Genesis.												ats	!
!      96/07/09 Adopted for the MC327ADS port.                          ats !
!               ---- OS-9/68K MC68328 Support Beta2 Release ----            !
!--------------------------------------------------------------------------*/

#include	<sysboot.h>

#ifdef NOBUG
int		errno;
u_char	trapflag;
#endif


#ifdef _UCC
u_int32		_stklimit = 0x80000;	/* big to limit _stkhandler calls
										from clib.l calls */
#endif

/*
 * Declarations for real functions
 */
extern error_code	sysreset(),
					binboot();

char	*nulstr = "";					/* only need one of these */

#ifdef _UCC
/*
 * Dummy _stkhandler routine for clib.l calls
 */
_stkhandler()
{
}
#endif


/*
 * getbootmethod: This function allows the developer to select
 * the booting method algorithm best suited for the system.
 */
int getbootmethod()
{
	/*
	 * Initialize the boot drivers. 
	 *
	 *  NOTE: The order of initialization determines the order of
	 *  priority when using the "AUTOSELECT" booting method.
	 */
	iniz_boot_driver(binboot, nulstr,
		"Boot Manually Loaded Bootfile Image", "ml");
	iniz_boot_driver(romboot, "ROM", "Boot from ROM", "ro");
	iniz_boot_driver(loadrom, "ROM", "Load from ROM", "lr");
	iniz_boot_driver(sysreset, nulstr, "Restart the system", "q");

/*	vflag = TRUE; */
	return USERSELECT;
}

/*
 * getboottype: When the boot method (determined by the above function)
 * is set to SWITCHSELECT, this function allows the developer to select
 * the actual booting type (device, ROM, etc...) according to hardware
 * switches, non-volatile RAM or hard-code a single boot device type
 * NOTE: for this devpak, this is a dummy function.
 */
Bdrivdef getboottype()
{
	return NULL;
}

