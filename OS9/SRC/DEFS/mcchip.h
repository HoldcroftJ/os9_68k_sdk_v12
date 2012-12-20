/* MCchip definitions */

/* This chip is integral to the MVME162 board */

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
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/


#ifndef MCchipBase
#define		MCchipBase	0xFFF42000
#endif

#if	!defined(__STDC__) && !defined(_ANSI_EXT) && !defined(_cplusplus)
#ifndef	volatile
#define		volatile	/* the old compiler doesn't understand 'volatile' */
#endif
#endif

typedef volatile struct mcchip {
	u_char	ID,				/* ID Register */
			REV,			/* Revision Register */
			GCR,			/* General Control Register */
			IVBR;			/* Interrupt Vector Base Register */
	
	u_int	T1CMP,			/* Timer 1 Compare Register */
	
			T1CNT,			/* Timer 1 Counter Register */
	
			T2CMP,			/* Timer 2 Compare Register */
	
			T2CNT;			/* Timer 2 Counter Register */
	
	u_char	LSBPCR,			/* LSB Prescaler Count Register */
			PCAR,			/* Prescaler Clock Adjust Register */
			T2CR,			/* Timer 2 Control Register */
			T1CR,			/* Timer 1 Control Register */
	
			T4ICR,			/* Timer 4 Interrupt Control Register */
			T3ICR,			/* Timer 3 Interrupt Control Register */
			T2ICR,			/* Timer 2 Interrupt Control Register */
			T1ICR,          /* Timer 1 Interrupt Control Register */
	
			PEICR,			/* DRAM Parity Error Interrupt Control Register */
			SCCICR,			/* SCC Interrupt Control Register */
			T4CR,			/* Timer 4 Control Register */
			T3CR;			/* Timer 3 Control Register */
	
	u_short	DSBAR,			/* DRAM Space Base Address Register */
			SSBAR;			/* SRAM Space Base Address Register */
	
	u_char	DSSR,			/* DRAM Space Size Register */
			ROR,			/* xRAM Options Register */
			SSSR,			/* SRAM Space Size Register */
			rsvd1,
	
			LESR,			/* LANC Error Status Register */
			rsvd2,
			LICR,			/* LANC Interrupt Control Register */
			LBEICR,			/* LANC Bus Error Interrupt Control Register */
	
			SESR,			/* SCSI Error Status Register */
			GPIR,			/* General Purpose Inputs Register */
			M162VR,			/* 162 Version Register */
			SICR;			/* SCSI Interrupt Control Register */
	
	u_int	T3CMP,			/* Timer 3 Compare Register */
	
			T3CNT,			/* Timer 3 Counter Register */
	
			T4CMP,			/* Timer 4 Compare Register */
	
			T4CNT;			/* Timer 4 Counter Register */
	
	u_char	BCR,			/* Bus Clock Register */
			PATCR,			/* PROM Access Time Control Register */
			FATCR,			/* FLASH Access Time Control Register */
			ABTICR,			/* Abort Switch Interrupt Control Register */
	
			RSCR,			/* Reset Switch Control Register */
			WTCR,			/* Watchdog Timer Control Register */
			AWTBSR,			/* Access & Watchdog Time Base Select Register */
			rsvd3,
	
			DCR,			/* DRAM Control Register */
			rsvd4,
			MPUSR,			/* MPU Status Register */
			rsvd5;
	
	u_int	PCR;			/* Prescaler Count Register */
} *MCchip;

#define		MCChip	((MCchip)MCchipBase)	/* properly casted constant */

/* For now, I've only provided the register definitions I've needed. */
/* Feel free to add those required, but please keep them in order of */
/* occurrance in the MCchip.                                         */

typedef volatile struct mcctcr {	/* Timer Control Register */
	u_int	OVF:4,			/* Overflow Counter */
	
			unused:1,
			COVF:1,			/* Clear Overflow */
			COC:1,			/* Clear On Comparre */
			CEN:1;			/* Counter Enable */
} *MCcTCR;

typedef volatile struct mccicr {	/* generic Interrupt Control Register */
	u_int	LANCmisc:2,		/* defined for LANC ICRs */
			INT:1,			/* Interrupt Status */
			IEN:1,			/* Interrupt Enable */
			
			ICLR:1,			/* Interrupt Clear - not for SCSI */
			ILVL:3;			/* Interrupt Level */
} *MCcICR;

typedef volatile struct mccror {	/* xRAM Options Register */
	u_int	unused:2,
			F:1,			/* FLASH device type */
			SZ:2,			/* SRAM configuration */
			DZ:3;			/* DRAM configuration */
} *MCcROR;

typedef volatile struct mccsesr {	/* SCSI Error Status Register */
	u_int	unused:4,
	
			PRTY:1,			/* Parity check caused last local Bus Error */
			EXT:1,			/* Error going to VMEbus */
			LTO:1,			/* Local bus timeout */
			SCLR:1;			/* Status Clear */
} *MCcSESR;

typedef volatile struct mcc162vr {	/* 162 Version Register */
	u_int	V7_IPIC1:1,		/* IPIC #1 is present */
			V6_IPIC2:1,		/* IPIC #2 is present */
			V5:1,			/* PROM/FLASH mapping */
			V4_NOLC:1,		/* MC68040 is present */
			
			V3_NOLANC:1,	/* LANC is not present */
			V2_NOSCSI:1,	/* SCSI is not present */
			V1_NOVME:1,		/* VMEchip2 is not present */
			V0_33Mhz:1;		/* MPU speed 33/25 */
} *MCc162VR;
	
/* Fixed interrupt vector offsets */

#define		MCVT4		0x03		/* Timer 4 */
#define		MCVT3		0x04		/* Timer 3 */
#define		MCVSCSI		0x05		/* SCSI IRQ */
#define		MCVLERR		0x06		/* LANC Error */
#define		MCVLANC		0x07		/* LANC IRQ */
#define		MCVT2		0x08		/* Timer 2 */
#define		MCVT1		0x09		/* Timer 1 */
#define		MCVPE		0x0B		/* Parity Error (DRAM) */
#define		MCVABT		0x0E		/* Abort switch */


