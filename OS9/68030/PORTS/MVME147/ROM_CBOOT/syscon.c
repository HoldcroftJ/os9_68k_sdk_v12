/*--------------------------------------------------------------------------!
! syscon.c: System specific boot configuration routines for the VME147      !
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 89/02/06 <Pfoof!!>  It exists.									jal	!
!	01 89/05/22 changed "wren" references to "sccs".					wwb !
!				updated menu text.										wwb !
!   02 89/05/31 updated menu (again).                                   wwb !
!	03 89/07/12 changed "scsi_id" to "viper_id"							wwb	!
!				added trapflag for NOBUG.								wwb	!
!	04 89/07/25 changed menu names again.								wwb	!
!	05 89/08/01 lots-o-big changes:										jal	!
!				1) redesigned the boot driver initialization to read	jal	!
!				the priority order from the nv_ram.							!
!				2) Changed the way the "switches" are read to include	jal	!
!				the VME050 generic code.									!
!               3) added "reconfig" function.							jal	!
!				4) added imbedded code constants for RAMDisk_Base and	jal	!
!				Viper_ID for easy patching 									!
!	06 89/08/03 moved assembler constants to the bottom of the file so	jal	!
!				show up in the "psect" instead of a "vsect"					!
!					<<<---- OS-9/68000/V2.3 Release ---->>>					!
!	07 89/10/19 added teac boot functions.								wwb	!
!					<<<---- OS-9/68000/V2.3 Update Release ---->>>			!
!	08 89/11/14 added INHOUSE support.									wwb !
!					<<<---- OS-9/68000/V2.3 Update #2 Release ---->>>		!
!   09 90/05/29 repaired fallthrough into reconfigure if autoboot fails Rwb !
!   10 90/07/19 added Teac FC-1 (js/hs) boot support                    Rwb !
!	11 90/09/06 added loaderboot menu option.                           wwb !
!               ---- OS-9/68K V2.4 released ----                            !
!   12 92/08/24 repaired problems with booter count                     Rwb !
!   13 92/08/31 added bootp boot options                                Rwb !
!   14 93/06/14 Modified for UCC compatibility mode.                    ats !
!   15 93/07/02 Adapted new NVRAM structure.                            ats !
!   16 93/07/23 Modified for MWOS directory structure.                  ats !
!   17 93/07/28 Added backplane booter support (from Gopal's work).     ats !
!   18 93/08/04 Add platform specific NVRAM version.					ats !
!   19 93/08/27 Added binary loadfile image booter.                     ats !
!               ---- OS-9/68K V3.0 released ----                            !
!   20 95/01/16 Add missing parameter to checknvram().                  ats !
!   21 95/11/08 Correct previous edition (#20) change.                  ats !
!               ---- OS-9/68K V3.0.2 released ----                          !
!---------------------------------------------------------------------------!
!                                                                           !
!  Copyright 1989, 1990, 1991, 1992, 1993, 1995                             !
!  by Microware Systems Corporation                                         !
!  Reproduced Under License                                                 !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include	<sysboot.h>
#include	<nvram.h>
/*#include	<wd33c93.h>	*/
#include	<scsi33c93.h> 

#include	"systype.h"

#ifdef	NOBUG
int		errno;			/* normally defined by the debugger */
u_char	trapflag;
#endif	NOBUG

/*--------------------------------------------------------------------------!
! Declaration and initialization of system specific global variables		!
! necessary for correct operation of the SCSI interface module: boot33c93	! 
!--------------------------------------------------------------------------*/
Hardware	scsibase = (Hardware)SCSIBASE; /* SCSI interface base address */
u_char		*scsi_icr = (u_char *)SCSI_ICR; /* SCSI ICR register address */
u_char		viper_id;				/* Viper tape controller ID		*/
#ifdef	INHOUSE
u_char		teac_id;				/* Teac tape controller ID		*/
#endif	INHOUSE
Sect_zero	rdiskbase;				/* NV RAM disk base address		*/

#ifdef _UCC
u_int32		_stklimit = 0x80000;	/* big to limit _stkhandler calls
									   from clib.l calls */
#endif

/*------------------------------------------------------------------!
! (phony functions) Declarations for variables embedded in the ROM	!
!------------------------------------------------------------------*/
extern void				Viper_ID(),
#ifdef	INHOUSE
						Teac_ID(),
#endif	INHOUSE
						RAMDisk_Base();

/*----------------------------------!
! Declarations for real functions	!
!----------------------------------*/
extern error_code		sysreset(),
#ifdef	INHOUSE
						boot_fd_omti(),				/* OMTI-5400 floppy		*/
						boot_hd_omti(),				/* OMTI-5400 winchester	*/
#endif	INHOUSE
						boot_hd_sccs(),				/* sccs hard drive 		*/
						bootsccs(),					/* sccs boot entry 		*/
						boot_fd_teac(),				/* TEAC FC-1 floppy 	*/
						bootviper(),				/* Archive Viper Tape 	*/
#ifdef  INHOUSE
						bootteac(),					/* TEAC cassette tape   */
#endif	INHOUSE
						boot5400(),					/* OMTI boot entry      */
#ifdef	BOOTP_BP
						boot_bp(),
#endif
#ifdef  BOOTP_LE
						boot_le(),
#endif
#ifdef  BOOTP_CN
						boot_cnp(),
#endif
#ifdef  BOOTP_EN
						boot_eni(),
#endif
#ifdef  BOOTP_ME
						boot_374(),
#endif
#ifdef	DOWNLOAD
						loaderboot(),				/* download tester		*/
#endif
#ifdef	INHOUSE
						boot_320_fd(),
						boot_320_hd(),
#endif
						binboot(),
						reconfig();

extern char				getinchar();
extern u_char			readswitch50();
extern void				checknvram();

/*------------------------------!
! Boot descriptor declarations	!
!------------------------------*/
extern struct rbf_opt	sccs_hbd,
#ifdef	INHOUSE
						omti_fbd,
						omti_hbd,
#endif	INHOUSE
						fteac_fbd;

#include "reconfig.c"

char *nulstr = "";

#ifdef _UCC
/*
 * Dummy _stkhandler routine for clib.l calls
 */
_stkhandler()
{
}
#endif

/*--------------------------------------------------------------!
! getbootmethod: This function allows the developer to select	!
! the booting method algorithm best suited for the system.		!
!--------------------------------------------------------------*/
int getbootmethod()
{
	register u_char	sys_switches;
	register Bdrivdef	booter_ptr;
	register char		count, booterno;
	int                 retval;			/* return value */
	nv_ram				nvimage;		/* shadow image of nv ram */

	checknvram( &nvimage );	/* make sure the nv_ram is initialized and valid */
	
	/* initialize tape device id's on scsi bus */
	viper_id = *((u_char *) ((void *)Viper_ID));	/* get viper tape SCSI ID		*/
#ifdef	INHOUSE
	teac_id = *((u_char *) ((void *)Teac_ID));	/* get teac tape SCSI ID		*/
#endif	INHOUSE

	/* get nvor RAM disk base	*/
	rdiskbase = (Sect_zero) *((u_int32 *) ((void *)RAMDisk_Base));
	
	/*----------------------------------------------------------------------!
	! Initialize the boot drivers											!
	+-----------------------------------------------------------------------+
	! NOTE: The order of initialization determines the order of priority	!
	!		when using the "AUTOSELECT" booting method, and the order is	!
	!		determined by the "boot order array" in the nv_ram.				!
	!----------------------------------------------------------------------*/

	/*------------------------------------------------------!
	! Read the switches to determine the booting algorithm	!
	!------------------------------------------------------*/
	sys_switches = readswitch50();
	
	if( (sys_switches & ROMSWITCH) ) {
		/* rom boot switch over-rides all other switch settings */
		iniz_boot_driver(romboot, nulstr, nulstr, nulstr);
		iniz_boot_driver(sysreset, nulstr, nulstr, nulstr);
		vflag = FALSE;
		return AUTOSELECT;
	}

	if ((sys_switches & (MENUSWITCH | BOOTSWITCH)) == 0) {
		/*----------------------------------------------!
		! boot selection is menu-driven user selectable	!
		!----------------------------------------------*/
		vflag = TRUE;
		retval =  USERSELECT;
	} else {
		/*----------------------------------------------------------!
		! booting is auto-selected in the order of initialization	!
		!----------------------------------------------------------*/
		vflag = FALSE;
		retval = AUTOSELECT;
	}

	/*----------------------------------------------------------------------!
	! count the maximum number of legitimate boot drivers initialized in	!
	! the "boot order array" in nv_ram.  Put the result in "bdrivers_qty"	!
	!----------------------------------------------------------------------*/
	bdrivers_qty = 0;
	for (count = 0; (booterno = nvimage.nvr_booter[count++].booterno) != 0;) {
		if (booterno <= BDRVMAX)
			bdrivers_qty++;
	}

	/*----------------------------------------------!
	! Initialize the boot driver definition array	!
	! according to the boot order array in nv_ram	!
	!----------------------------------------------*/
	for (count = 0; (booterno = nvimage.nvr_booter[count++].booterno) != 0;) {
		if (booterno <= BDRVMAX) {
			booter_ptr = &booters[booterno - 1];
			iniz_boot_driver(booter_ptr->driver_entry, booter_ptr->driver_name,
							 booter_ptr->menuline, booter_ptr->idstring);
		}
	}
	/*--------------------------------------------------------------!
	! also initialize the alternate functions and system functions	!
	!--------------------------------------------------------------*/
	if( !(sys_switches & MENUSWITCH) ) {
		bdrivers_qty += NOALTFUNCS;		/* add in number of alternate funcs. */
		for (count = 0; count < NOALTFUNCS; count++) {
			bdrivers_qty += NOALTFUNCS;
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
	return(NVV_MVME147);
}

/*----------------------------------------------------------------------!
! getboottype: When the boot method (determined by the above function)	!
! is set to SWITCHSELECT, this function allows the developer to select	!
! the actual booting type (device, ROM, etc...) according to hardware	!
! switches, non-volatile RAM or hard-code a single boot device type		!
!----------------------------------------------------------------------*/
Bdrivdef getboottype()
{
	return NULL;
}

#ifdef	INHOUSE
/*--------------------------------------------------!
! boot_fd_omti: Boot from Omti 5400 floppy drive	!
!--------------------------------------------------*/
error_code boot_fd_omti()
{
	defopts = &omti_fbd;	/* iniz the path descriptor pointer	*/
	return boot5400();		/* call Omti 5400 boot function		*/
}


/*----------------------------------------------!
! boot_hd_omti: Boot from Omti 5400 hard drive	!
!----------------------------------------------*/
error_code boot_hd_omti()
{
	defopts = &omti_hbd;	/* iniz the path descriptor pointer	*/
	return boot5400();		/* call Omti 5400 boot function		*/
}
#endif	INHOUSE

/*----------------------------------------------!
! boot_hd_sccs: Boot from SCSI-SCCS hard drive	!
!-----------------------------------------------*/
error_code boot_hd_sccs()
{
	defopts = &sccs_hbd;	/* iniz the path descriptor pointer	*/
	return bootsccs();		/* call sccs boot function			*/
}

/*-----------------------------------------------!
! boot_fd_teac: Boot from TEAC SCSI floppy drive !
!-----------------------------------------------*/
error_code boot_fd_teac()
{
	defopts = &fteac_fbd;    
	return bootfteac();     
}

#ifdef	INHOUSE
#ifdef	_UCC
_asm(" use bt320.a");
#else
@ use bt320.a
#endif
#endif	INHOUSE

#ifdef _UCC
_asm(" use nvrw.a");
#else
@ use nvrw.a
#endif

/*----------------------------------------------------------------------! 
! NOTE: THIS IS ASSEMBLER CODE... Stick the base address of the nvor	!
! RAM disk and the Viper tape drive SCSI controller I.D. into the ROM	!
! so that it shows up in the map.  This allows end users to find the	!
! places to patch these things in more easily.							!
!----------------------------------------------------------------------*/
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
