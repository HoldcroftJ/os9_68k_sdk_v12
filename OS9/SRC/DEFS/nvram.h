/* nvram.h */

/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1998 by Microware Systems Corporation              |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, modification, distribution, or creation of    |
|  derivative works in any form to any party other than the Licensee is     |
|  strictly prohibited, unless expressly provided for under the terms of a  |
|  mutually executed written license agreement for this software between    |
|  the end-user and Microware Systems Corporation, or otherwise expressly   |
|  authorized in writing by Microware Systems Corporation.  Licensee shall  |
|  reproduce this copyright notice exactly as provided herein on any copies |
|  of the software that Licensee is authorized to distribute.               |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
|  01 98/07/29 Added edition history.                                  wwb  |
|				---- OS-9/68K V3.1 Release ----								|
|  02 00/01/1X Added comments for SWITHCES and additional fields       mgh  | 
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/
#ifndef _OS9_SRC_DEFS_NVRAM_H
#define _OS9_SRC_DEFS_NVRAM_H

#define BDRVTSZ		16
#define BDRVSLTS	BDRVTSZ-1

/*
 * Common substructure
 */
typedef struct boentry {
	u_char		booterno;
	u_char		scsi_id;
	u_char		scsi_lun;
	u_char		dummy;
} bo_entry, *Bo_entry;

/*
 * Proposed 'universal' nvram structure
 */
typedef struct nvram { /* 256 bytes in length */
	u_int16		nvr_newsync;		/*00 new short sync code ("0xAB54")			*/
	u_int16		nvr_version;		/*02 new nvram version id					*/
	u_char		nvr_spcrst;			/*04 new special reset flag					*/
	u_char		nvr_switches;		/*05 Microware "standard" boot "switches"	*/
	u_char		nvr_brdid;			/*06 backplane driver board id number		*/
	u_char		nvr_grpid;			/*07 backplane driver group id number		*/
	u_char		nvr_vmeint;			/*08 VME interrupt mask (V1 flag)			*/
	u_char      nvr_slven;			/*09 slave address enable					*/
	u_char		nvr_scsirst;		/*0a SCSI reset enable (at V2)				*/
	u_char		nvr_rsvd2;			/*0b reserved (zeroed)						*/
	bo_entry	nvr_booter[BDRVTSZ];/*0c-4b boot order array					*/
	u_int32		nvr_memsize;		/*4c amount of mem on board					*/
	u_int32		nvr_slvaddr;		/*50 vme address of local memory			*/
	u_int32		nvr_pad1;			/*54 pad to 8 byte boundry	(zero this out)	*/
	u_int16		nvr_cons_cntl;		/*58 Console control bits	(zero this out)	*/
	u_char		nvr_cons_prty;		/*5a Console Parity Byte					*/
	u_char		nvr_cons_baud;		/*5b Console Baud Rate						*/
	u_int16		nvr_comm_cntl;		/*5c Communication port control bits(RSVD)	*/
	u_char		nvr_comm_prty;		/*5e Communication port Parity Byte			*/
	u_char		nvr_comm_baud;		/*5f Communication port Baud Rate			*/
	u_int32		nvr_ip_addr; 		/*60 This machine's IP Address            	*/
	u_int32		nvr_ip_mask; 		/*64 This machine's IP Network Mask       	*/
	u_int32		nvr_ip_gate; 		/*68 This machine's default route gateway 	*/
	u_int32		nvr_tftphost;		/*6c TFTP server's IP Address				*/
	u_int32		nvr_dsttime;		/*70 seconds after Midnight for next change */
	u_int32		nvr_dstdate;		/*74 Date of next DST Change (Julian)       */
	u_int32		nvr_dstoffset;		/*78 Seconds offset from Std to DST         */
	u_int32		nvr_dststat;		/*7c Status bits for DST                    */
	u_int32		nvr_pad[31];		/*	 pad out the structure (zero these out!)*/
	u_int16		nvr_lrsvd;			/*fc last reserved word (zero these out!)	*/
	u_int16		nvr_cksum;			/*fe new checksum of nvram structure		*/
} nv_ram, *Nv_ram;

#define NVR_SWITCHES 	5

/*
 * With this new structure, nvr_newsync and nvr_version overlay the
 * area previously occupied by nvr_sync. With this, code can detect
 * between the old and new formats at runtime. If old PROMs are 
 * installed on a board with a newer nvram structure, the old
 * PROMS will not recognize the new structure and rebuild a default
 * structure recognizable to them.
 *
 * The old reset counter, nvr_reset, is replaced with a new special
 * reset flag, nvr_spcrst. This flag is zero except when a reset is
 * forced by sysinit.a (to reinitialize hardware - previously done 
 * with nvr_SoftRST on the 167 & 162), or when a new RESET+ABORT
 * pushbutton sequence is initiated, which is intended to force boot
 * reconfiguration. Usage of the soft-reset flag will cause it to
 * get set back to zero after the forced reset, preserving nvram
 * integrity. Setting the Abort switch flag will invalidate 
 * the checksum.
 *
 * Nvram will be considered bad if nvr_newsync, nvr_version, or
 * nvr_cksum mismatch, in which case hardware is initialized to
 * platform specific defaults and the reconfiguration dialog is
 * forced. Successful reconfiguration updates the nvram fields
 * to the values selected, forcing updates to nvr_newsync, 
 * nvr_version, and nvr_cksum, and clearing nvr_spcrst.
 *
 */

#define NVRNEWSYNC	0xAB54

#define NVR_VER_MIN 1
#define NVR_VER_MAX 2

/* NVRAM version changes:
 *  1: Original version.
 *  2: Changed nvr_vmeint from single flag to bit mask.
 *     Added SCSI reset flag.
 */

#define NVV_VINT_MASK	2
#define NVV_SRST_FLAG	2
 

/* platform specific nvram r/w routine to be supplied by syscon.c */
#if		defined(__STDC__) || defined(_ANSI_EXT) || defined(__cplusplus)
extern void nvrw(void);
#else
extern void nvrw();
#endif


/* 'C' definitions of assembly routines */
#if		defined(__STDC__) || defined(_ANSI_EXT) || defined(__cplusplus)
extern int goodnvr(void);
extern u_int16 nwck16(u_char *, u_int32);
#else
extern int goodnvr();
extern u_int16 nwck16();
#endif


/* reconfiguration subroutines */
#if		defined(__STDC__) || defined(_ANSI_EXT) || defined(__cplusplus)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
extern error_code reconfig(void);
extern error_code rc_int(char *, u_int32 *, u_int32, u_int16);
extern error_code rc_endis(char *, u_char *, u_char, u_int16);
extern error_code rc_btlist(u_char *, u_char *, u_int16);
extern error_code rc_vmeints(u_char *, u_char *, u_int16);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#else
extern error_code reconfig();
extern error_code rc_int();
extern error_code rc_endis();
extern error_code rc_btlist();
extern error_code rc_vmeints();
#endif


/* flags for reconfiguration subroutines */
#define NVR_RCFORCE		1
#define NVR_RCDEFAULT	2
#define NVR_RCHEX		4


/*----------------------------------------------------------------------!
! Switch definitions for MVME050 Card                                   !
! SWITCH 2                                                              !
!		0 = Switches 3&4 select boot method                             !
!       1 = Rom Boot forced                                             !
!                                                                       !
! SWITCH 3	4	(ORIGINAL DEFINITIONS/USER MANUAL)                      !
!        0  0	Sysboot builds user select menu                         !
!        0	1	sysboot uses Auto Select algorithm                      !
!		 1  0	Boots from Primary   Device                             !
!        1  1	Boots from Secondary Device                             !
!                                                                       !
! SWITCH 3 4	(147,167,162,177,172 Menu system Implementation)        !
!        0 0	Syscon builds user select menu                          !
!        0 1	Use Auto select sequence                                !
!        1 0	Use Auto select sequence                                !
!        1 1	Use Auto select sequence                                !
!                                                                       !
! SWITCH 5                                                              !
!		0 = Rombugger enabled                                           !
!       1 = Rombugger disabled                                          !
!                                                                       !
!   NOTE:   MVME050  HWSWITCHES: CLOSED/ON = 0, OPEN/OFF = 1            !
!   NOTE 2: readswitch50.c returns 050 switch value or nvram switch     !
!            settings if the 050 card is not present                    !
!   NOTE 3: Switch 8 blanks the digit display on the MVME050 Card and   !
!            should not otherwise be allocated                          !
!----------------------------------------------------------------------*/

/*----------------------------------------------------------------------!
! NV_RAM or VME050 Switch definitions.                                  !
!-----------------------------------------------------------------------!
! SWITCH 1 / BIT 1: UNUSED                                              !
!       0 = UNUSED                                                      !
!       1 = UNUSED                                                      !
!-----------------------------------------------------------------------!
! SWITCH 2 / BIT 2: ROMSWITCH                                           !
!   Forces boot from rom image Boot                                     !
!   NOTE: Overides other Booting switches below.                        !
!       0 = Rom Forced Auto boot off                           (Default)!
!       1 = Rom Auto boot sequence of (Romboot,sysreset) ON             !
!-----------------------------------------------------------------------!
! SWITCH 3 / BIT 3: MENUSWITCH, AUTOBOOT                                !
!   controls whether to boot from menu choice or auto-sequence.         !
!       0 = User Selects Boot Device from Menu (Primary)       (Default)!
!       1 = auto-select sequence               (Secondary)              !
!-----------------------------------------------------------------------!
! SWITCH 4 / Bit 4: BOOTSWITCH                                          !
!   if switch3 or switch4 = 1, do autoboot                              !
!       0 = Use Menu based booter selection                    (Default)!
!       1 = Use Switch based booter selection                           !
!-----------------------------------------------------------------------!
! SWITCH 5 / Bit 5: DEBUGSWITCH, DISABLE_DEBUG                          !
!   Controls the enabling of the debugger                               !
!       0 = debugger enabled                                   (Default)!
!       1 = debugger disabled                                           !
!-----------------------------------------------------------------------!
! SWITCH 6 / BIT 6: UNUSED                                              !
!       0 = closed: UNUSED                                              !
!       1 = open:   UNUSED                                              !
!-----------------------------------------------------------------------!
! SWITCH 7 / BIT 7: UNUSED                                              !
!       0 = closed: UNUSED                                              !
!       1 = open:   UNUSED                                              !
!-----------------------------------------------------------------------!
! SWITCH 8 / BIT 8: UNUSED                                              !
!       0 = closed: UNUSED                                              !
!       1 = open:   UNUSED                                              !
!----------------------------------------------------------------------*/
#define SWITCH1			0x01	/* %00000001 = UNUSED */
#define SWITCH2			0x02	/* %00000010 = ROMSWITCH */
#define SWITCH3			0x04	/* %00000100 = MENUSWITCH */
#define SWITCH4			0x08	/* %00001000 = BOOTSWITCH */
#define SWITCH5			0x10	/* %00010000 = DEBUGSWITCH */
#define SWITCH6			0x20	/* %00100000 = UNUSED */
#define SWITCH7			0x40	/* %01000000 = UNUSED */
#define SWITCH8			0x80	/* %10000000 = UNUSED */

#define ROMSWITCH       SWITCH2 /* boot from rom (autoselect) */
#define MENUSWITCH      SWITCH3 /* menu selection switch    */
#define BOOTSWITCH      SWITCH4 /* boot selection switch    */
#define DEBUGSWITCH     SWITCH5 /* debugger enable switch   */

/*----------------------------------------------------------------------!
! Definitions for nvr_dstStat field Status/flag bits                    !
!----------------------------------------------------------------------*/
#define ADSTEN_B		0x01	/* Auto DST Enable 0=off, 1=on         */
#define INDST_B			0x02	/* 0=off/std time, 1=On/DST Time       */
#define RTCCHG_B		0x04	/* RTC Time Changes 0=No,1=yes         */
#define RTCDST_B		0x08	/* RTC Time 0=std, 1=DST               */


#endif	/* _OS9_SRC_DEFS_NVRAM_H */
