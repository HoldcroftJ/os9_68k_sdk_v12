/*  SysType.h  - system specific definitions for the mVme162.
 *
 *      These definitions must agree with the definitions that are
 *   put forth in systype.d.  If you change systype.d and not 
 *   systype.h, you could be in for some surprises.
 *
 *  Revision History:
 *    Date        Reason                                      Who
 *  --------  --------------------------------------------   ----
 *  93/01/26  Adapted from 167								 ats
 *  93/04/07  Removed old NVRAM structure definitions		 ats
 *  93/06/04  Modified for UCC compatibility mode            ats
 *  93/08/17  Added platform dependent NVRAM version         ats
 *	93/11/12  Updated to NVRAM version 2					 ats
 *  94/04/29  Added MCECC definitions                        ats
 */

#ifndef _types
#include <types.h>						/* for reconfig and syscon */
#endif
#include <gendefs.h>					/* general definitions file */

#define	SHORTIOBASE	0xFFFF0000			/* address of "VME short I/O" space	*/
#define IOBASE      0xFFF40000			/* on-board IO */
#define ATN_MODE	FLG_NOATN			/* no parity, no atn, asynch. */

#ifndef	BOOT327
#define SCSIBASE 	IOBASE+0x7000		/* base address on-borad NCR 53C710 */
#else	
#define SCSIBASE	SHORTIOBASE+0xa600	/* base address of MVME327 */
#endif	/* BOOT327 */

#ifdef	BOOT320
#ifdef _UCC
_asm("VME320Base equ $ffffb000");
#else
@VME320Base equ $ffffb000
#endif
#endif	/* BOOT320 */

/*
 * Definitions for non-volatile ram in the on-board 48t02 chip
 */
#define NVRAMBASE		0xFFFC0000		/* truly non-volatile ram in 48t08	*/

#define NVV_MVME162		2

#define MCECC_1Base		IOBASE+0x3000
#define MCECC_2Base		IOBASE+0x3100

/*  for the PROM Disk Booter */
#define PADDRMSK	0xFFFE0000		/* mask to calculate PROM base address */
#define PROMLEN		0x00080000-256
#define DRAMDISK	0x00080000		/* loc to load PROM disk image */

/*  for the SRam Disk Booter */
#ifdef _UCC
_asm("RDISKBASE equ $FFE10000");
_asm("RDISKEND  equ $FFE80000-256");
#else
@RDISKBASE equ $FFE10000				# start search addr for RAM disk
@RDISKEND  equ $FFE80000-256			# end search addr for 512K SRAM
#endif
/*     Remember that the above should agree!!!  */
/*	AND should match SRAMBase/End in systype.d !!!	*/

/*
 * This is the address of the on-cpu boot configuration switch byte.
 */
#define CPU_SWITCHES	(NVRAMBASE + NVR_SWITCHES)

/*
 * Microware "standard" SCSI controller I.D. defs.
 */
#ifdef	CORES
#define SCCS_SCSI_ID	1			/* SCSI SCCS Controller I.D.			*/
#else
#define SCCS_SCSI_ID	0			/* SCSI SCCS Controller I.D.			*/
#endif

#define VIPER_SCSI_ID 	4			/* Viper tape drive SCSI controller ID	*/
#ifdef _UCC
_asm("VIPER_SCSI_ID equ 4");
_asm("TEAC_SCSI_ID equ 4");
#else
@VIPER_SCSI_ID equ		4	/* assembler label needed for assembler code	*/
@TEAC_SCSI_ID equ       4   /* assembler label needed for assembler code    */
#endif
#define OWN_SCSI_ID		7 			/* Own SCSI I.D.						*/

/*
 * Boot Descriptor Options for SCSI-SCCS Hard Drive.
 */
#ifdef	SCCS
#define CTRLRID	SCCS_SCSI_ID
#define SCSILUN 0
#endif
