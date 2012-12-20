/*--------------------------------------------------------------------------!
! systype.h: System specific defs for 'C' language code for the VME147		!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	01 89/07/12 history started.                                        wwb !
!	02 89/08/02 history continues...									jal !
!				  <<<---- OS-9/68000 V2.3 Release ---->>>					!
!	03 89/10/19 added TEAC_SCSI_ID.										wwb	!
!				  <<<---- OS-9/68000 V2.3 Update Release ---->>>			!
!	04 89/11/14 added INHOUSE support.									wwb !
!				  <<<---- OS-9/68000 V2.3 Update #2 Release ---->>>			!
!	05 89/12/18 added CORES support (for laying down with UNIX).		wwb	!
!   06 90/07/19 added ROMSWITCH for the mVme050 switch definitions      Rwb !
!   07 90/10/10 made CORES default disk id = 0 (lay down with OS-9000.  wwb	!
!               ---- OS-9/68K V2.4 released ----                            !
!   08 93/06/14 Modified for UCC compatibility mode.                    ats !
!   09 93/07/01 Adapted new NVRAM structure                             ats !
!   10 93/07/22 Adapt for new MWOS directory structure.                 ats !
!   11 93/08/04 Add platform specific NVRAM version (updated to 2).		ats !
!               ---- OS-9/68K V3.0 released ----                            !
!---------------------------------------------------------------------------!
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

#ifndef _types
#include <types.h>			/* This is here for use by high level stuff		*/
#endif

#include <gendefs.h>		/* call in the general definitions file			*/

#define	SHORTIOBASE		0xFFFF0000		/* address of "VME short I/O" space	*/

#define VMEGCSR_GR0		0xFFFE2021		
#define VMEChip_IDMask	0xF

#include	<vme050.h>		/* get VME050 defs					*/

#define	RDISKBASE		0x0				/* start of NV on reset ram disk	*/
#ifdef _UCC
_asm("RDISKBASE equ 0");
#else
@RDISKBASE equ 0					/* assembler label for assembler code	*/
#endif

/*--------------------------------------------------------------!
! Definitions for non-volatile ram in the on-board 48t02 chip	!
!--------------------------------------------------------------*/
#define NVRAMBASE		0xFFFE0000		/* truly non-volatile ram in 48t02	*/

#define NVV_MVME147		2				/* latest NVRAM version for the 147 */

/*------------------------------------------------------------------!
! This is the address of the on-cpu boot configuration switch byte	!
!------------------------------------------------------------------*/
#define CPU_SWITCHES	(NVRAMBASE + NVR_SWITCHES)

/*--------------------------------------------------------------!
! This is the number of times needed to press the reset button	!
! in order to trigger the boot system reinitialization code		!
!--------------------------------------------------------------*/
#define MAXRESETS		5

/*--------------------------------------------------!
! Microware "standard" SCSI controller I.D. defs	!
!--------------------------------------------------*/
#ifdef	CORES
#define SCCS_SCSI_ID	0			/* SCSI SCCS Controller I.D.			*/
#else
#define SCCS_SCSI_ID	0			/* SCSI SCCS Controller I.D.			*/
#endif

#define VIPER_SCSI_ID 	4			/* Viper tape drive SCSI controller ID	*/
#define TEAC_SCSI_ID 	4			/* Teac tape drive SCSI controller ID	*/
#define OMTI_SCSI_ID	6 			/* Omti SCSI Controller I.D.			*/
#define OWN_SCSI_ID		7 			/* Own SCSI I.D.						*/
#ifdef _UCC
_asm("VIPER_SCSI_ID equ 4");
_asm("TEAC_SCSI_ID equ 4");
#else
@VIPER_SCSI_ID equ		4	/* assembler label needed for assembler code	*/
@TEAC_SCSI_ID equ		4	/* assembler label needed for assembler code	*/
#endif

/*----------------------!
! specific definitions	!
!----------------------*/
#define SCSIBASE		0xFFFE4000		/* SCSI Hardware Base Address	*/
#define SCSILEVEL		4				/* PCC SCSI port level			*/
#define ATN_MODE		FLG_NOATN		/* no parity, no atn, asynch.	*/

#ifdef	INHOUSE

#ifdef _UCC
_asm("VME320Base equ $ffffb000");
#else
@VME320Base equ $ffffb000 VME320 disk controller (odd byte addressable only)
#endif

#endif

/*--------------------------------------------------!
! Boot Descriptor Options for SCSI-SCCS Hard Drive	!
!---------------------------------------------------*/
#ifdef	SCCS
#define CTRLRID	SCCS_SCSI_ID
#define SCSILUN 0
#endif

/*----------------------------------------------!
! Boot Descriptor Options for OMTI5400 Drives	!
!----------------------------------------------*/
#ifdef	OMTI5400

#define CTRLRID	OMTI_SCSI_ID

#ifdef	FLOPPY
#define SCSILUN 2
#else
#define SCSILUN 0
#endif
#endif
