/*  SysType.h  - system specific definitions for the mVme167.
 *
 *      These definitions must agree with the definitions that are
 *   put forth in systype.d.  If you change systype.d and not 
 *   systype.h, you could be in for some surprises.
 *
 *  Revision History:
 *    Date        Reason                                      Who
 *  --------  --------------------------------------------   ----
 *  91/04/29  Created for the mVme167                        Rwb
 *  91/10/08  Reorganized NVRAM layout (removed CPUSpeed)    wwb
 *  93/06/23  Adapted new NVRAM structure. UCC support       ats
 *  93/08/05  Added platform dependent NVRAM version.        ats
 *  97/05/13  ******** version 3.0.3 release *************   gdw
 */

#ifndef _types
#include <types.h>						/* for reconfig and syscon */
#endif
#include <gendefs.h>					/* general definitions file */

#define	SHORTIOBASE	0xFFFF0000			/* address of "VME short I/O" space	*/
#define IOBASE      0xFFF40000			/* on-board IO */
#define ATN_MODE	FLG_NOATN			/* no parity, no atn, asynch. */
#define SCSIBASE  	IOBASE+0x7000		/* base address NCR 53C710 */

/*
 * Definitions for non-volatile ram in the on-board 48t02 chip
 */
#define NVRAMBASE		0xFFFC0000		/* truly non-volatile ram in 48t08	*/

#define NVV_MVME167		2

#define MEMC40_1Base	IOBASE+0x3000
#define MEMC40_2Base	IOBASE+0x3100

/*  for the Ram Disk Booter (currently unused and unsupported) */
#define RDISKBASE	0x0					/* base address of NV RAM disk */
#ifdef _UCC
_asm("RDISKBASE equ 0");
#else
@RDISKBASE equ 0
#endif
/*     Remember that the above should agree!!!  */

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
#define TEAC_SCSI_ID 	4			/* Teac tape drive SCSI controller ID	*/
#define OWN_SCSI_ID		7 			/* Own SCSI I.D.						*/
#ifdef _UCC
_asm("VIPER_SCSI_ID equ 4");
_asm("TEAC_SCSI_ID equ 4");
#else
@VIPER_SCSI_ID equ		4	/* assembler label needed for assembler code	*/
@TEAC_SCSI_ID equ		4	/* assembler label needed for assembler code	*/
#endif

/*
 * Boot Descriptor Options for SCSI-SCCS Hard Drive.
 */
#ifdef	SCCS
#define CTRLRID	SCCS_SCSI_ID
#define SCSILUN 0
#endif
