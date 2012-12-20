/* definitions for NCR5380 SCSI Interface device
 *
 * Copyright 1988 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 * history:
 *
 * ed.  date   reason for change								who
 * -- -------  ----------------------------------------------	---
 * 00 88/12/06 history starts - hacked from various sources		jal
 *				---- OS-9/68K V3.0 Release ----
 *				---- OS-9/68K V3.1 Release ----
 */

/*------------------------------------------!
! Define register layout for the NCR5380	!
!------------------------------------------*/
#define SCSIDATA	((0 * BW) + BO)	/* Current SCSI data (R/W)				*/
#define SCSIWDAT 	SCSIDATA		/* Output data register (W)				*/
#define SASICMND	SCSIWDAT		/* commands go through data port		*/
#define SASISEL		SCSIWDAT		/* select code goes through data port	*/
#define INITCMND	((1 * BW) + BO)	/* Initiator Command (R/W)				*/
#define MODEREG		((2 * BW) + BO)	/* Mode Register (R/W)					*/
#define TARGCMND	((3 * BW) + BO)	/* Target Command (R/W)					*/
#define STATREG		((4 * BW) + BO)	/* Current SCSI bus status register		*/
#define SELENABL	STATREG			/* Select Enable Register (W)			*/
#define BUSSTAT		((5 * BW) + BO)	/* Bus and Status Register (R)			*/
#define INTDMATX	BUSSTAT			/* Start DMA send register (W)			*/
#define SCSIRDAT 	((6 * BW) + BO)	/* Input Data Register (R)				*/
#define DMATGTRX	SCSIRDAT		/* Start DMA Target Receive Register (W)*/
#define RESET		((7 * BW) + BO)	/* Reset parity/interrupts (R)			*/
#define INTDMARX	RESET			/* Start DMA initiator receive reg. (W)	*/

#ifdef VME107
#define FSTDATTX	(SCSIWDAT - 0x10)	/* fast SCSI data output port		*/
#define FSTDATRX	(SCSIRDAT - 0x10)	/* fast SCSI data input port		*/
#endif

#ifdef VME117
#define FSTDATTX	(SCSIWDAT + 0x4000)	/* fast SCSI data output port		*/
#define FSTDATRX	(SCSIRDAT + 0x4000)	/* fast SCSI data input port		*/
#endif

#ifdef PCB1782
#define FSTDATTX	SCSIDATA	/* fast SCSI data output port		*/
#define FSTDATRX	SCSIDATA	/* fast SCSI data input port		*/
#endif

/*----------------------------------!
! Bit equates for status register 1	!
!----------------------------------*/
#define RST_BT 		0x80	/* %10000000 bit 7 = reset bit		*/
#define BUSY_BT		0x40	/* %01000000 bit 6 = busy bit		*/
#define REQ_BT		0x20	/* %00100000 bit 5 = request bit	*/
#define MSG_BT		0x10	/* %00010000 bit 4 = message bit	*/
#define CMND_BT		0x08	/* %00001000 bit 3 = command bit	*/
#define IO_BT		0x04	/* %00000100 bit 2 = I/O bit		*/
#define BSEL_BT		0x02	/* %00000010 bit 1 = select bit		*/
#define DBP_BT		0x01	/* %00000001 bit 0 = parity bit		*/

#define PHS_BTS		0x1C	/* mask in bus phase bits			*/

/*----------------------------------!
! Bit equates for status register 2	!
!----------------------------------*/
#define ENDDMA		0x80	/* %10000000 bit 7 = end of DMA bit				*/
#define DMAREQ		0x40	/* %01000000 bit 6 = DMA request bit			*/
#define PARERR		0x20	/* %00100000 bit 5 = parity error bit			*/
#define INT_ACT 	0x10	/* %00010000 bit 4 = Interrupt Request Active	*/
#define PMATCH		0x08	/* %00001000 bit 3 = Phase Match Bit			*/
#define BUSYERR		0x04	/* %00000100 bit 2 = busy error bit				*/
#define BATN_BT		0x02	/* %00000010 bit 1 = attention bit				*/
#define BACK_BT		0x01	/* %00000001 bit 0 = ack bit					*/

/*----------------------------------------------!
! Bit equates for Initiator Command Register	!
!----------------------------------------------*/
#define RESET_BT	0x80	/* %10000000 bit 7 = assert RST			*/
#define TESTMODE	0x40	/* %01000000 bit 6 = Test mode bit		*/
							/* (disables all output drivers)		*/
#define AIP_BT		0x40	/* %01000000 bit 6 = arbitration in progress */
#define LA_BT		0x20	/* %00100000 bit 5 = lost arbitration	*/
#define A_ACK 		0x10	/* %00010000 bit 4 = assert ACK			*/
#define A_BSY 		0x08 	/* %00001000 bit 3 = assert BSY			*/
#define A_SEL 		0x04	/* %00000100 bit 2 = assert SEL			*/
#define A_ATN		0x02	/* %00000010 bit 1 = assert ATN			*/
#define A_DBUS 		0x01	/* %00000001 bit 0 = assert data bus	*/


/*--------------------------!
! mask equates for MODEREG	!
!--------------------------*/
#define BLK_MD		0x80 	/* if DMA mode bit = 1 DACK allowed	*/
#define TRG_MD		0x40 	/* enable target mode				*/
#define PAR_CHK 	0x20 	/* enable parity checking			*/
#define PAR_INT 	0x10 	/* enable parity interrupt			*/
#define EOP_INT 	0x08 	/* enable EOP interrupt				*/
#define BSY_INT 	0x04 	/* enable interrupt on loss of BSY	*/
#define DMA_MD 		0x02 	/* enable dma mode					*/
#define ARB_MD 		0x01 	/* enable arbitration				*/

#if defined(PCB1782)
#define DMAMODE		DMA_MD
#else
#define DMAMODE		DMA_MD
#endif

/*----------------------!
! Target Command Values	!
!----------------------*/
#define IO		1
#define CD		2 
#define MSG		4 

/*----------------------!
! Phase Mismatch Masks	!
!----------------------*/
#define DATAOUT 	0
#define DATAIN 		IO
#define COMMAND 	CD
#define STATUS 		IO+CD 
#define MSGOUT		CD+MSG 
#define MSGIN		IO+CD+MSG 

/*------------------!
! Error status bit	!
!------------------*/
#define ERRSTAT		1		/* bit 1 of status byte is set if error.  This	*/
							/* byte is returned from the controller after	*/
							/* after the command is completed.				*/
#define RECOVERR	0x18 	/* error code for recoverable error				*/
