/*--------------------------------------------------------------------------!
! syscon.c: System specific boot configuration routines for the VME162		!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!   01 93/02/08 Adapted from the version for the mVme167                ats !
!   02 93/04/07 Removed unused old NVRAM structure processing.			ats !
!   03 93/05/18 Modified for UCC -mode=compat. Extracted assembly       ats !
!               code into separate file.                                    !
!   04 93/08/16 MWOS structure changes.                                 ats !
!	05 93/08/17 Added platform dependent NVRAM version.                 ats !
!---------------------------------------------------------------------------!
!                                                                           !
!               Copyright 1991 by Microware Systems Corporation             !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include	<sysboot.h>
#include	<mcchip.h>
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
u_char		teac_id;				/* Teac tape controller ID		*/
u_char		*rdiskbase, *rdiskend;	/* NV RAM disk base address		*/

#ifdef _UCC
u_int32		_stklimit = 0x80000;	/* big to limit _stkhandler calls
									   from clib.l calls */
/* u_int16		paddding;				/* field to pad static storage to
									   longword boundary */
#endif

/*
 * (phony functions) Declarations for variables embedded in the ROM.
 */
extern void				Viper_ID(),
#ifdef	INHOUSE
						Teac_ID(),
#endif	INHOUSE
						RAMDisk_Base(),
						RAMDisk_End();

/*
 * Declarations for real functions.
 */
extern error_code		sysreset(),
						boot_hd_sccs(),				/* sccs hard drive 		*/
						bootsccs(),					/* sccs boot entry 		*/
						boot_fd_teac(),				/* TEAC FC-1 floppy 	*/
						bootviper(),				/* Archive Viper Tape 	*/
						boot_ie(),					/* I82596 Ethernet		*/
#ifndef UCCTEST
						boot_bp(),					/* Backplane 			*/
#endif UCCTEST
#ifdef	INHOUSE
						bootteac(),					/* TEAC cassette tape   */
#endif	INHOUSE
#ifdef	BOOT320
						boot_320_hd(),
						boot_320_fd(),
#endif	BOOT320
#ifdef	BOOT327
						boot_fd_327(),				/* 327 floppy disk		*/
#endif	BOOT327
#ifdef	BOOTP_ME
						boot_374(),
#endif	BOOTP_ME
#ifndef ROMBOOT
						boot_prom_disk(),
#endif  ROMBOOT
						boot_sram_disk(),
						boot_nvram_disk(),
						binboot(),
						reconfig();

extern char				getinchar();
extern void				checknvram();

extern u_char 			*GetRomEnd();	/* from rompak.m in sysinit.a */

/*
 * Boot descriptor declarations.
 */
extern struct rbf_opt	sccs_hbd,
#ifdef	BOOT327
						v327_fbd,
#endif	BOOT327
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
	MCc162VR			mccvr = (MCc162VR) &(MCChip->M162VR);
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
	viper_id = *((u_char *)((void *)Viper_ID));	/* get viper tape SCSI ID	*/
#ifdef	INHOUSE
	teac_id = *((u_char *)((void *)Teac_ID));	/* get teac tape SCSI ID	*/
#endif	INHOUSE

	/* get NV RAM disk base	*/
	rdiskbase = (u_char *) *((u_int32 *)((void *)RAMDisk_Base));
	rdiskend = (u_char *) *((u_int32 *)((void *)RAMDisk_End));
	
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
 	if (mccvr->V1_NOVME)
 		sys_switches = nvimage.nvr_switches;
 	else
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
			booterno -= 1;
			booter_ptr = &booters[booterno];
			if (((SDRVMIN <= booterno) && (booterno <=SDRVMAX) &&
						(mccvr->V2_NOSCSI)) ||
				    ((LDRVMIN <= booterno) && (booterno <=LDRVMAX) &&
				    	(mccvr->V3_NOLANC)) ||
				    ((VDRVMIN <= booterno) && (booterno <=VDRVMAX) &&
				    	(mccvr->V1_NOVME)))
				iniz_boot_driver(booter_ptr->driver_entry,
					booter_ptr->driver_name, booter_ptr->menuline, NULL);
			else
			    iniz_boot_driver(booter_ptr->driver_entry,
			    	booter_ptr->driver_name, booter_ptr->menuline,
			    	booter_ptr->idstring);
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
	return (BDRVMAX);
}

int getnvrversion()
{
	return(NVV_MVME162);
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

#ifdef	BOOT327
/*
 * boot_fd_327: Boot from SCSI-SCCS hard drive.
 */
error_code boot_fd_327()
{
	defopts = &v327_fbd;
	return bootvme327();
}
#endif	BOOT327

#ifndef ROMBOOT
error_code boot_prom_disk()
{
	u_char	*romdisk, *romend;
	Sect_zero romzero;
	int disklen;
	
	romdisk = GetRomEnd();
	romend = (u_char *) (((int) romdisk & PADDRMSK) + PROMLEN);
	
	for (romzero = (Sect_zero) NULL; romdisk < romend; romdisk += 2)
		if (*((u_int32 *) ((Sect_zero) romdisk)->dd_sync) == SYNC_CRUZ) {
			romzero = (Sect_zero) romdisk;
			break;
		}

	if (romzero) {
		sectzero = (Sect_zero) DRAMDISK; /* DRAM loc for PROM boot image */
		disklen = ((((romzero->dd_tot[0] << 8) + romzero->dd_tot[1]) << 8) +
			romzero->dd_tot[2]) * romzero->dd_lsnsize;
		memcpy(sectzero, romzero, disklen); /* copy PROM disk image to DRAM */
		return boot_nvram_disk(); /* and go boot it */
	} else {
		outstr("\n** No PROM disk image found **\n");
		return E_BTYP;
	}
}
#endif ROMBOOT

error_code boot_sram_disk()
{
	u_char	*ramdisk, *ramend;
	Sect_zero ramzero;
	
	ramdisk = rdiskbase;
	ramend = rdiskend;
	
	for (ramzero = (Sect_zero) NULL; ramdisk < ramend; ramdisk += 256)
		if (*((u_int32 *) ((Sect_zero) ramdisk)->dd_sync) == SYNC_CRUZ) {
			ramzero = (Sect_zero) ramdisk;
			break;
		}

	if (ramzero) {
		sectzero = (Sect_zero) rdiskbase;
		return boot_nvram_disk();
	} else {
		outstr("\n** No SRAM disk found **\n");
		return E_BTYP;
	}
	
}

extern error_code readramdisk();

error_code boot_nvram_disk()
{
	readdriver = readramdisk;
	defopts = &pathopts;
	defopts->pd_ssize = OSKSECTSIZE;
	return getboot();
}
	
/*
 *  nvrw - routine to read and write the NVRAM for the mVme162 card.
 *
 *  void nvrw( shadowptr , nvptr, direction, size )
 *     char *shadowptr -- pointer to the shadow image of the NVRAM
 *     nvptr           -- address of the NVRAM
 *     direction       -- if TRUE write NVRAM, if FALSE, read it
 *     size            -- number of bytes in the shadow image
 */
#ifdef _UCC
_asm(" use nvrw.a");
#else
@ use nvrw.a
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
RAMDisk_End:  dc.l RDISKEND
Viper_ID:     dc.b VIPER_SCSI_ID
Teac_ID:      dc.b TEAC_SCSI_ID
 align
");
#else
#asm
RAMDisk_Base: dc.l RDISKBASE
RAMDisk_End:  dc.l RDISKEND
Viper_ID:     dc.b VIPER_SCSI_ID
Teac_ID:	  dc.b TEAC_SCSI_ID
 align
#endasm
#endif

#include <readswitch50.c>

#ifdef	BOOT320
#ifdef _UCC
_asm(" use bt320.a");
#else
@ use bt320.a
#endif
#endif	BOOT320
