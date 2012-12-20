/*--------------------------------------------------------------------------!
!                                                                           !
!        Copyright 1989, 1990, 1993 by Microware Systems Corporation        !
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
	{bootviper, "Viper tape drive", 
		"Boot from Viper tape drive", "vs", 0},
#ifdef	INHOUSE
	{bootteac, "Teac tape drive", 
		"Boot from Teac tape drive", "ts", 0},
#endif	INHOUSE
	{boot_fd_teac, "TEAC SCSI floppy drive", 
		"Boot from Teac SCSI floppy drive", "fs", 0},
	{boot_hd_sccs, "SCSI(SCCS) hard drive", 
		"Boot from SCSI(SCCS) hard drive", "hs", 0},
#ifdef	INHOUSE
	{boot_fd_omti, "Omti 5400 floppy drive",
		"Boot from Omti 5400 floppy drive", "fo", 0},
	{boot_hd_omti, "Omti 5400 hard drive",
		"Boot from Omti 5400 hard drive", "ho", 0},
#endif	INHOUSE
#ifdef BOOTP_BP
	{boot_bp,	  "VMEbus backplane",
		"Boot from BOOTP backplane", "bp", 0 },
#endif
#ifdef BOOTP_LE
	{boot_le,	  "Internet BOOTP/AM7990 LANCE",
		"Boot from BOOTP am7990 LANCE", "le", 0 },
#endif
#ifdef BOOTP_CN
	{boot_cnp,	  "Internet BOOTP/ENP100i",
		"Boot from BOOTP enp100", "cn", 0 },
#endif
#ifdef BOOTP_EN
	{boot_eni,	  "Internet BOOTP/ENP10i",
		"Boot from BOOTP enp10i", "en", 0 },
#endif
#ifdef BOOTP_ME
	{boot_374,	  "Internet BOOTP/VME374",
		"Boot from BOOTP vme374", "me", 0 },
#endif
#ifdef	DOWNLOAD
	{loaderboot, "Download Boot Tester",
		"Test Download Boot File","dl", 0},
#endif
	{loadrom, "ROM", 
		"Load Bootfile from ROM", "lr", 0},
	{romboot, "ROM", 
		"Boot from ROM", "ro", 0},
	{binboot, "Human",
		"Boot Manually Loaded Bootfile Image", "ml", 0}
#ifdef	INHOUSE
	,{boot_320_fd,  "VME320 floppy drive",
		"Boot from VME320 floppy drive", "f320", 0}
	,{boot_320_hd,  "VME320 hard drive",
		"Boot from VME320 hard drive", "h320", 0}
#endif
#ifdef FANCYBOOT
	,{rdiskboot, "ram disk",
		"Boot from a non-volatile RAM disk", "rd", 0}
	,{portboot, "serial port",
		"Boot through the serial communications port", "se", 0}
#endif
};

#define BDRVMAX sizeof(booters)/sizeof(bdrivdef)	/* number of booters */

bdrivdef sysfuncs[] = {
	{sysreset, "Restart System", 
		"Restart the system", "q", 0}
};
#define NOSYSFUNCS	sizeof(sysfuncs)/sizeof(bdrivdef)	/* system functions */

bdrivdef altfuncs[] = {
	{reconfig, "Reconfigure Boot System",
		"Reconfigure the boot system", "rc", 0}
};	
#define NOALTFUNCS sizeof(altfuncs)/sizeof(bdrivdef)	/* alternate funcs */

extern void TransFact();

error_code rc_dialogue(nvrimage, flags)
Nv_ram nvrimage;
u_int16 flags;
{
	error_code	stat;
	u_int32		cv1,dv1;
	u_char		cv2,dv2;
	u_char		cbo[16],dbo[16];
	u_char		cvi[8],dvi[8],vmemask;
	int			i,j,booterno;
	u_char		okval,vmeid,maxmult;
	
	outstr("\n\nHARDWARE RECONFIGURATION\n\n");

	do {	
		cv1 = nvrimage->nvr_memsize;
		dv1 = 0x400000;
		if ((stat = rc_int("\nBoard Memory Size:", &cv1, dv1,
				(flags | NVR_RCHEX))) != 0)
			return stat;
		dv1 = cv1 / dv1;
		okval = ((~(0-0x400000) & cv1) == 0) &&
			((dv1 == 1) || (dv1 == 2) || (dv1 == 4) || (dv1 == 8));
		if (!okval) {
			outstr("\n* Error * Board Memory Size must be either\n");
			outstr("          4MB, 8MB, 16MB or 32MB.\n");
		}
	} while(!okval);
	nvrimage->nvr_memsize = cv1;
	
	do {
		cv1 = nvrimage->nvr_brdid;
		if ((stat = rc_int("\nBoard ID:", &cv1, 15, flags)) != 0)
			return stat;
		if (cv1 > 15) {
			outstr("\n* Error * Board ID must be between 0 and 15.\n");
			outstr("          (15 = disabled)\n");
		}
	} while (cv1 > 15);
	nvrimage->nvr_brdid = cv1;
	
	cv1 = nvrimage->nvr_grpid;
	if ((stat = rc_int("\nGroup ID:", &cv1, 0,
			(flags | NVR_RCDEFAULT | NVR_RCFORCE | NVR_RCHEX))) != 0)
		return stat;
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
	for (i=0; i<7; )
		dvi[i++] = i + '0';
	dvi[i] = 0;
	
	if ((stat = rc_vmeints(cvi, dvi, flags)) != 0)
		return stat;
		
	vmemask = 0;
	i = 0;
	while (cvi[i]) {
		vmemask |= (1 << (cvi[i] - '0'));
		i++;
	}
	nvrimage->nvr_vmeint = vmemask;
	
	cv2 = nvrimage->nvr_slven;
	if ((stat = rc_endis("\nVME Slave Memory:", &cv2, TRUE, flags)) != 0)
		return stat;
	nvrimage->nvr_slven = cv2;
	
	vmeid = *((u_char *)VMEGCSR_GR0) & VMEChip_IDMask;
	maxmult = (vmeid == 0) ? 3 : 27; /* vmechip ver 1 on S-series boards */
	do {
		cv1 = nvrimage->nvr_slvaddr;
		dv1 = *((u_int32 *)((void *)TransFact));
		stat = rc_int("\nVME Slave Memory Address:", &cv1, dv1,
			flags | NVR_RCHEX);
		if (stat != 0)
			return stat;
		dv1 = (~(0 - nvrimage->nvr_memsize) & cv1);
		okval = (dv1 == 0) && ((cv1 / nvrimage->nvr_memsize) <= maxmult);
		if (!okval) {
			outstr("\n* Error * Slave Memory Address must be a multiple(0 - ");
			outint(maxmult);
			outstr(")\n          of the Board Memory Size.\n");
		}
	} while (!okval);
	nvrimage->nvr_slvaddr = cv1;
	
	if (!((nvrimage->nvr_brdid == 0) || (nvrimage->nvr_brdid == 15)) &&
			(nvrimage->nvr_slven) && (nvrimage->nvr_slvaddr == 0)) {
		outstr("\n* Warning * If this is a Slave CPU, having the Slave\n");
		outstr("            Memory at zero on the VMEBus may cause\n");
		outstr("            erratic Master CPU operation !!!\n");
	}
	
	cv2 = nvrimage->nvr_scsirst;
	if ((stat = rc_endis("\nSCSI Reset on ROMBUG startup:", &cv2, TRUE,
			flags)) != 0)
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
