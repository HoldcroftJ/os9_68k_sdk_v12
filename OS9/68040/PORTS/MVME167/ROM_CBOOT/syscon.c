/*--------------------------------------------------------------------------!
! syscon.c: System specific boot configuration routines for the VME167		!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!   00 91/04/29 Created for the mVme167                                 Rwb !
!	01 91/12/05 Added BootP (MVME374).									wwb	!
!	02 91/12/16 Added BootP (MVME167).									wwb	!
!	03 91/12/23 Fixed readswitch50 - now use global version.			wwb	!
!   04 92/08/24 Repaired problems with booter count                     Rwb !
!   05 93/06/21 Modified for UCC compatibility mode.                    ats !
!   06 93/06/23 Adapt new NVRAM structure.                              ats !
!   07 93/07/28 Adapt new MWOS directory structure. Add backplane       ats !
!               booter.                                                     !
!   08 93/08/05 Add platform dependent NVRAM version.                   ats !
!   09 93/08/26 Added binary loadfile image booter.                     ats !
!      97/05/13 ******* version 3.0.3 release ************************* gdw
!---------------------------------------------------------------------------!
!                                                                           !
!            Copyright 1991, 1993 by Microware Systems Corporation          !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include	<sysboot.h>
#include	<nvram.h>
#include	"systype.h"
#include 	<vme050.h>

#ifdef NOBUG
int		errno;			/* normally defined by the debugger */
u_char 	trapflag;		/* .....  as above  .............   */
#endif NOBUG

/*
 * Declaration and initialization of system specific global variables
 * necessary for correct operation.
 */
int 		scsibase = SCSIBASE;	/* SCSI interface base address	*/
u_char		viper_id;				/* Viper tape controller ID		*/
#ifdef	INHOUSE
u_char		teac_id;				/* Teac tape controller ID		*/
#endif	INHOUSE
Sect_zero	rdiskbase;				/* NV RAM disk base address		*/

#ifdef _UCC
u_int32		_stklimit = 0x80000;	/* bit to limit _stkhandler calls
									   from clib.l calls */
#endif
									   
/*
 * (phony functions) Declarations for variables embedded in the ROM.
 */
extern void				Viper_ID(),
#ifdef	INHOUSE
						Teac_ID(),
#endif	INHOUSE
						RAMDisk_Base();

/*
 * Declarations for real functions.
 */
extern error_code		sysreset(),
						boot_hd_sccs(),				/* sccs hard drive 		*/
						bootsccs(),					/* sccs boot entry 		*/
						boot_fd_teac(),				/* TEAC FC-1 floppy 	*/
						bootviper(),				/* Archive Viper Tape 	*/
#ifdef	INHOUSE
						bootteac(),					/* TEAC cassette tape   */
#endif	INHOUSE
#ifdef	BOOTP_ME
						boot_374(),					/* MVME374 Ethernet		*/
#endif
#ifdef	BOOTP_IE
						boot_ie(),					/* I82596 Ethernet		*/
#endif
#ifdef	DOWNLOAD
						loaderboot(),				/* download tester		*/
#endif
						binboot(),
						reconfig();

extern char				getinchar();
extern void				checknvram();

/*
 * Boot descriptor declarations.
 */
extern struct rbf_opt	sccs_hbd,
						fteac_fbd;

#include "reconfig.c"

char *nulstr = "";		/* single copy of a null string */

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
 *  the booting method algorithm best suited for the system.
 */
int getbootmethod()
{
	register u_char     sys_switches;
	register Bdrivdef	booter_ptr;
	register char		count, booterno;
	int                 retval;				/* return value */
	nv_ram              nvimage;			/* shadow image of nv ram */
	extern u_char       readswitch50();
	extern void         nvrw();				/* nv ram read and write */

	
	/* get copy of NVRAM and insure integrity */
	checknvram( &nvimage );

	/* initialize tape device id's on scsi bus */
	viper_id = *((u_char *) ((void *)Viper_ID));	/* get viper tape SCSI ID		*/
#ifdef	INHOUSE
	teac_id = *((u_char *) ((void *)Teac_ID));	/* get teac tape SCSI ID		*/
#endif	INHOUSE

	/* get nv or RAM disk base	*/
	rdiskbase = (Sect_zero) *((u_int32 *) ((void *)RAMDisk_Base));
	
	/*
	 * Initialize the boot drivers.
	 *
	 * NOTE: The order of initialization determines the order of priority
	 *		when using the "AUTOSELECT" booting method, and the order is
	 *		determined by the "boot order array" in the nv_ram.
	 */
	/*
	 * Read the switches to determine the booting algorithm.
 	 */
	sys_switches = readswitch50();
	
	if( (sys_switches & ROMSWITCH) ) {
		/* rom boot switch over-rides all other switch settings */
		iniz_boot_driver(romboot, nulstr, nulstr, nulstr);
		iniz_boot_driver(sysreset, nulstr, nulstr, nulstr);
		vflag = FALSE;
		return AUTOSELECT;
	}

	if ((sys_switches & (MENUSWITCH | BOOTSWITCH)) == 0) {
		/* boot selection is menu-driven user selectable */
		vflag = TRUE;
		retval =  USERSELECT;
	} else {
		/* booting is auto-selected in the order of initialization */
		vflag = FALSE;
		retval = AUTOSELECT;
	}
	
	/*
	 * count the maximum number of legitimate boot drivers initialized in
	 * the "boot order array" in nv_ram.  Put the result in "bdrivers_qty".
 	 */
	bdrivers_qty = 0;
	for (count = 0; (booterno = nvimage.nvr_booter[count++].booterno) != 0;) {
		if (booterno <= BDRVMAX)
			bdrivers_qty++;
	}

	/*
	 * Initialize the boot driver definition array
	 * according to the boot order array in nv_ram.
	 */
	for (count = 0; (booterno = nvimage.nvr_booter[count++].booterno) != 0;) {
		if (booterno <= BDRVMAX) {
			booter_ptr = &booters[booterno - 1];
			iniz_boot_driver(booter_ptr->driver_entry, booter_ptr->driver_name,
							 booter_ptr->menuline, booter_ptr->idstring);
		}
	}

	/* also initialize the alternate functions and system functions. */
	if( !(sys_switches & MENUSWITCH) ) {
		bdrivers_qty += NOALTFUNCS;
		for (count = 0; count < NOALTFUNCS; count++) {
			booter_ptr = &altfuncs[count];
			iniz_boot_driver(booter_ptr->driver_entry, booter_ptr->driver_name,
							 booter_ptr->menuline, booter_ptr->idstring);
		}
	}
		
	/* now include the system functions */
	bdrivers_qty += NOSYSFUNCS;  /* add in the number of system functions */
	for (count = 0; count < NOSYSFUNCS; count++) {
		booter_ptr = &sysfuncs[count];
		iniz_boot_driver(booter_ptr->driver_entry, booter_ptr->driver_name,
							 booter_ptr->menuline, booter_ptr->idstring);
	}
	return retval;
}

Nv_ram getnvrbase()
{
	return ((Nv_ram)NVRAMBASE);
}

int getbdrvmax()
{
	return(BDRVMAX);
}

int getnvrversion()
{
	return(NVV_MVME167);
}

/*
 * getboottype: When the boot method (determined by the above function)
 * is set to SWITCHSELECT, this function allows the developer to select
 * the actual booting type (device, ROM, etc...) according to hardware
 * switches, non-volatile RAM or hard-code a single boot device type.
 */
Bdrivdef getboottype()
{
	return NULL;
}

/* boot_hd_sccs: Boot from SCSI-SCCS hard drive */
error_code boot_hd_sccs()
{
	defopts = &sccs_hbd;	/* iniz the path descriptor pointer	*/
	return bootsccs();		/* call sccs boot function			*/
}

/* boot_fd_teac: Boot from TEAC SCSI floppy drive */
error_code boot_fd_teac()
{
	defopts = &fteac_fbd;    
	return bootfteac();     
}

/*
 *  nvrw - routine to read and write the NVRAM for the mVme167 card.
 *
 *  void nvrw( shadowptr , nvptr, direction, size )
 *     char *shadowptr -- pointer to the shadow image of the NVRAM
 *     nvptr           -- address of the NVRAM
 *     direction       -- if TRUE write NVRAM, if FALSE, read it
 *     size            -- number of bytes in the shadow image
 */
#ifdef _UCC
_asm(" use ./nvrw.a");
#else
@ use ./nvrw.a
#endif

/*
 * NOTE: THIS IS ASSEMBLER CODE... Stick the base address of the nvram
 * RAM disk and the Viper tape drive SCSI controller I.D. into the ROM
 * so that it shows up in the map.  This allows end users to find the
 * places to patch these things in more easily.
 */
#ifdef _UCC
_asm("
RAMDisk_Base: dc.l RDISKBASE
Viper_ID:     dc.b VIPER_SCSI_ID
Teac_ID:      dc.b TEAC_SCSI_ID
 align
");
#else
#asm
RAMDisk_Base: dc.l RDISKBASE
Viper_ID:     dc.b VIPER_SCSI_ID
Teac_ID:      dc.b TEAC_SCSI_ID
 align
#endasm
#endif

#include <readswitch50.c>

