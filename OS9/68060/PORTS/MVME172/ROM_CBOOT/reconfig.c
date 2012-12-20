/*--------------------------------------------------------------------------!
! reconfig.c: System specific boot configuration routines for the           !
!             VME172														!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!   01 93/02/08 Adapted from the Vme167 version.                        ats !
!   02 93/04/07 Removed unused old NVRAM structure processing.          ats !
!   03 93/06/04 Modified for UCC compatibility mode.                    ats !
!	04 93/11/11 Removed the PROM disk boot option.						ats !
!				Added loadrom() and binboot().								!
!	05 93/11/12 Fixed BOOTP build options.								ats !
!				Correct default board ID and VME slave address.				!
!				Added support for NVRAM version 2.							!
!				Fix default booter list construction and more than 9		!
!					booters.												!
!	06 93/11/15 Guide reconfiguration defaults by installed options.	ats !
!	07 94/04/29 Added probes for MCECC on 'LXs.                         ats !
!               ---- V3.0.2 MVME162 BSP released ----                       !
!---------------------------------------------------------------------------!
!                                                                           !
!     Copyright 1991, 1993, 1994, 1995 by Microware Systems Corporation     !
!                          Reproduced Under License                         !  
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

/*------------------------------------------!
! Boot driver array initialization entries	!
!------------------------------------------*/
bdrivdef booters[] = {
#ifndef	BOOT327
	{boot_hd_sccs, "SCSI(SCCS) hard drive", 
		"Boot from SCSI(SCCS) hard drive", "hs", 0},
	{bootviper, "Viper tape drive", 
		"Boot from Viper tape drive", "vs", 0},
	{boot_fd_teac, "TEAC SCSI floppy drive", 
		"Boot from Teac SCSI floppy drive", "fs", 0},
#ifdef	INHOUSE
	{bootteac, "Teac tape drive",
		"Boot from Teac tape drive", "ts", 0},
#endif	INHOUSE
#endif	BOOT327
#ifdef	BOOTP_IE
	{boot_ie, "Internet BOOTP/I82596 LANC",
		"Boot from BOOTP i82596 LANC", "ie", 0 },
#endif	BOOTP_IE

#ifdef	BOOT320
	{boot_320_hd, "VME320 hard drive",
		"Boot from VME320 hard drive", "h3", 0},
	{boot_320_fd, "VME320 floppy drive",
		"Boot from VME320 floppy drive", "f3", 0},
#endif	BOOT320
#ifdef	BOOT327
	{boot_hd_sccs, "SCSI(SCCS) hard drive", 
		"Boot from SCSI(SCCS) hard drive", "hs", 0},
	{bootviper, "Viper tape drive", 
		"Boot from Viper tape drive", "vs", 0},
	{boot_fd_teac, "TEAC SCSI floppy drive", 
		"Boot from Teac SCSI floppy drive", "fs", 0},
	{boot_fd_327, "VME327 Floppy boot",
		"Boot from VME327 Floppy drive", "f7", 0},
#endif	BOOT327
#ifdef	BOOTP_BP
	{boot_bp, "VMEBus backplane",
		"Boot from BOOTP backplane", "bp", 0},
#endif	BOOTP_BP
#ifdef	BOOTP_ME
	{boot_374,    "Internet BOOTP/VME374",
		"Boot from BOOTP vme374", "me", 0 },
#endif	BOOTP_ME
		
	{boot_sram_disk, "Static RAM disk",
		"Boot from a non-volatile (Static) RAM disk", "sd", 0},
	{loadrom, "ROM", 
		"Load Bootfile from ROM", "lr", 0},
	{romboot, "ROM", 
		"Boot from ROM", "ro", 0},
	{binboot, "Human",
		"Boot Manually Loaded Bootfile Image", "ml", 0}
};

#define SDRVMIN 0
#ifndef	BOOT327
#ifndef	INHOUSE
#define SDRVNUM 3
#else	INHOUSE
#define	SDRVNUM 4
#endif	INHOUSE
#else	BOOT327
#define	SDRVNUM 0
#endif	BOOT327
#define SDRVMAX SDRVMIN+SDRVNUM-1

#define LDRVMIN SDRVMAX+1
#ifdef	BOOTP_IE
#define LDRVNUM 1
#else	BOOTP_IE
#define	LDRVNUM	0
#endif	BOOTP_IE
#define LDRVMAX LDRVMIN+LDRVNUM-1

#define VDRVMIN LDRVMAX+1

#ifdef	BOOT320
#define	VDRV320	2
#else	BOOT320
#define	VDRV320 0
#endif	BOOT320

#ifdef	BOOT327
#define	VDRV327 4
#else	BOOT327
#define VDRV327 0
#endif	BOOT327

#ifdef	BOOTP_BP
#define	VDRVBBP	1
#else	BOOTP_BP
#define	VDRVBBP	0
#endif	BOOTP_BP

#ifdef	BOOTP_ME
#define	VDRVBME 1
#else	BOOTP_ME
#define	VDRVBME 0
#endif	BOOTP_ME

#define	VDRVNUM	VDRV320+VDRV327+VDRVBBP+VDRVBME

#define VDRVMAX VDRVMIN+VDRVNUM-1

#define MDRVMIN VDRVMAX+1
#define MDRVNUM 4
#define MDRVMAX MDRVMIN+MDRVNUM-1

#define BDRVMAX MDRVMAX+1		/* number of booters */


bdrivdef sysfuncs[] = {
	{sysreset, "Restart System", 
		"Restart the system", "q", 0}
};

#define NOSYSFUNCS sizeof(sysfuncs)/sizeof(bdrivdef)

bdrivdef altfuncs[] = {
	{reconfig, "Reconfigure Boot System",
		"Reconfigure the boot system", "rc", 0}
};	

#define NOALTFUNCS sizeof(altfuncs)/sizeof(bdrivdef)

extern void TransFact();

u_int16		sztb[] =	{{0x10}, {0x20}, {0x0}, {0x40},
						 {0x40}, {0x80}, {0x0}, {0x100}};

error_code rc_dialogue(nvrimage, flags)
Nv_ram nvrimage;
u_int16 flags;
{
	MCcROR		mccror = (MCcROR) &(MCChip->ROR);
	MCc162VR	mcc162vr = (MCc162VR) &(MCChip->M162VR);
	u_int16		vme_flags,scsi_flags;
	error_code	stat;
	u_int32		cv1,dv1;
	u_char		cv2,dv2;
	u_char		cbo[16],dbo[16];
	u_char		cvi[8],dvi[8],vmemask;
	int			i,j,booterno;
	u_int32		nmem,ms1,ms2,ms3,memseg,segmult;
	
	outstr("\n\nHARDWARE RECONFIGURATION\n\n");
	
	cv1 = nvrimage->nvr_memsize;
	dv1 = 0;
	nmem = ms1 = ms2 = ms3 = 0;
	if (hw_probe(MCECC_1Base) == SUCCESS) {
		nmem++;
		ms1 = (1 << (22 + (0x07 & *((char *)MCECC_1Base + 8))));
		dv1 += ms1;
	}
	if (hw_probe(MCECC_2Base) == SUCCESS) {
		nmem++;
		ms2 = (1 << (22 + (0x07 & *((char *)MCECC_2Base + 8))));
		dv1 += ms2;
	}
	if (mccror->DZ != 6) {
		nmem++;
		ms3 = sztb[mccror->DZ] * 0x10000;
		dv1 += ms3;
	}

	if (dv1 == 0)				/* if no DRAM on-board and	*/
		if (mccror->SZ == 3) 	/* 2MB SRAM is installed	*/
			dv1 = 0x200000;

	if ((stat = rc_int("\nBoard Memory Size:", &cv1, dv1,
			(flags | NVR_RCDEFAULT | NVR_RCFORCE | NVR_RCHEX))) != 0)
		return stat;
	nvrimage->nvr_memsize = cv1;

	vme_flags = flags | (mcc162vr->V1_NOVME ? NVR_RCFORCE | NVR_RCDEFAULT :0);
	do {	
		cv1 = nvrimage->nvr_brdid;
		if ((stat = rc_int("\nBoard ID:", &cv1, 15, vme_flags)) != 0)
			return stat;
		if (cv1 > 15) {
			outstr("\n* Error * Board ID must be between 0 and 15.\n");
			outstr("          (15 = disabled)\n");
		}
	} while (cv1 > 15);
	nvrimage->nvr_brdid = cv1;
	
	do {
		cv1 = nvrimage->nvr_grpid;
		stat = rc_int("\nGroup ID:", &cv1, 0xD2, (vme_flags | NVR_RCHEX));
		if (stat != 0)
			return stat;
		if (cv1 > 0xFF)
			outstr("\n* Error * Group ID must be between 0x0 and 0xff.\n");
	} while (cv1 > 0xFF);
	nvrimage->nvr_grpid = cv1;

	if (nvrimage->nvr_version < NVV_VINT_MASK) {
		if (nvrimage->nvr_vmeint) {
			for (i=0; i<7;)
				cvi[i++] = i + '0';
			cvi[i] = 0;
		} else
			cvi[0] = 0;
	} else {
		vmemask = nvrimage->nvr_vmeint;
		for (i=0,j=0; i<7; i++) {
			vmemask >>= 1;
			if (vmemask & 1)
				cvi[j++] = i + 1 + '0';
		}
		cvi[j] = 0;
	}
	if (mcc162vr->V1_NOVME) {
		dvi[0] = 0;
	} else {
		for (i=0; i<7; )
			dvi[i++] = i + '0';
		dvi[i] = 0;
	}
	
	if ((stat = rc_vmeints(cvi, dvi, vme_flags)) != 0)
		return stat;
		
	vmemask = 0;
	i = 0;
	while (cvi[i]) {
		vmemask |= (1 << (cvi[i] - '0'));
		i++;
	}
	nvrimage->nvr_vmeint = vmemask;
	
	cv2 = nvrimage->nvr_slven;
	dv2 = !(mcc162vr->V1_NOVME);
	if ((stat = rc_endis("\nVME Slave Memory:", &cv2, dv2, vme_flags)) != 0)
		return stat;
	nvrimage->nvr_slven = cv2;

	do {	
		cv1 = nvrimage->nvr_slvaddr;
		if (mcc162vr->V1_NOVME)
			dv1 = 0;
		else
			dv1 = *((u_int32 *)((void *)TransFact));
		stat = rc_int("\nVME Slave Memory Address:", &cv1, dv1,
			vme_flags | NVR_RCHEX);
		if (stat != 0)
			return stat;
		/* find largest memory mezzanine size */
		memseg = (ms1 > ms2) ? ms1 : ms2;
		memseg = (memseg > ms3) ? memseg : ms3;
		if ((memseg == 0) && (mccror->SZ == 3)) {
			memseg = 0x200000;
			segmult = 1;
		} else {
			segmult = (ms1 + ms2 + ms3 + memseg - 1) / memseg;
			if (segmult == 3) segmult++;
			memseg *= segmult;
		}
		dv1 = (~(0 - memseg) & cv1);
		if (dv1 != 0) {
			outstr("\n* Error * Slave Memory Address must be a multiple of\n");
			outstr("          Board Memory Size");
			if (segmult > 1) {
				outstr("(rounded to ");
				outhex(memseg);
				outstr(")");
			}
			outstr(".\n");
		}
	} while (dv1 != 0);
	nvrimage->nvr_slvaddr = cv1;
	
	if (!((nvrimage->nvr_brdid != 0) || (nvrimage->nvr_brdid == 15)) &&
			(nvrimage->nvr_slven) && (nvrimage->nvr_slvaddr == 0)) {
		outstr("\n* Warning * If this is a Slave CPU, having the Slave\n");
		outstr("            Memory at zero on the VMEBus may cause\n");
		outstr("            erratic Master CPU operation !!!\n");
	}
	
	scsi_flags = flags | (mcc162vr->V2_NOSCSI ? NVR_RCFORCE | NVR_RCDEFAULT :0);
	cv2 = nvrimage->nvr_scsirst;
	dv2 = !(mcc162vr->V2_NOSCSI);
	if ((stat = rc_endis("\nSCSI Reset on ROMBUG startup:", &cv2, dv2,
			scsi_flags)) != 0)
		return stat;
	nvrimage->nvr_scsirst = cv2;
	
	outstr("\nBOOT SYSTEM RECONFIGURATION\n\n");
	
	cv2 = !(nvrimage->nvr_switches & DEBUGSWITCH);
	if ((stat = rc_endis("\nDebugger:", &cv2, TRUE, flags)) != 0)
		return stat;
	nvrimage->nvr_switches &= ~DEBUGSWITCH;
	nvrimage->nvr_switches |= (cv2 ? 0 : DEBUGSWITCH);
	
	cv2 = !(nvrimage->nvr_switches & MENUSWITCH);
	if ((stat = rc_endis("\nBoot Menu:", &cv2, TRUE, flags)) != 0)
		return stat;
	nvrimage->nvr_switches &= ~MENUSWITCH;
	nvrimage->nvr_switches |= (cv2 ? 0 : MENUSWITCH);

	i = 0;
	while ((booterno = nvrimage->nvr_booter[i].booterno) != 0) {
		if (booterno < 10)
			cbo[i] = booterno + '0';
		else
			cbo[i] = booterno - 10 + 'a';
		i++;
	}
	cbo[i] = 0;
	
	i = 0;
	while (i < BDRVMAX) {		
		if (i < 9)
			dbo[i] = i + '1';
		else
			dbo[i] = i - 9 + 'a';
		i++;
	}
	dbo[i] = 0;
	
	if ((stat = rc_btlist(cbo, dbo, flags)) != 0)
		return stat;
		
	i = 0;
	while (cbo[i]) {
		if (('0' < cbo[i]) && (cbo[i] <= '9'))
			nvrimage->nvr_booter[i].booterno = cbo[i] - '0';
		else
			nvrimage->nvr_booter[i].booterno = cbo[i] - 'a' + 10;
		i++;
	}
	nvrimage->nvr_booter[i].booterno = 0;
		
	return 0;
}

