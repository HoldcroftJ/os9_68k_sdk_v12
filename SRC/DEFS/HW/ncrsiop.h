/*--------------------------------------------------------------------------,
|                                                                           |
|          Copyright 1991, 1995-1998 by Microware Systems Corporation       |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, distribution, or creation of derivative works |
|  in any form to any party other than the licensee is strictly prohibited, |
|  unless expressly authorized in writing by Microware Systems Corporation. |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
| 01 96/08/03 Added edition history.                                  rak   |
|             ---- OS-9000/x86 V2.1 Released ----                           |
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
| 02 96/08/26 Added burst opcode fetch define for 825.                GbG   |
| 03 97/04/14 Merged SCSISIOP/ncrsiop.h defs.                         GbG   |
|             Added Symbios 875 defs.                                 GbG   |
|             ---- OS-9000/PPC V2.1.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
| 04 98/01/28 Fixed compiler warnings.                                GbG   |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- SCSI Common Sub-Component v1.0 Released ----             |
|             $$                    <RELEASE_INFO>                     $$   |
`--------------------------------------------------------------------------*/

#include <types.h>
 
/*
 * NCR SIOP definitions.
 */

#if defined(NCR53C710)

/*   NCR 53C710 Register Definition */

typedef volatile struct ncr53c710 {
	u_int8 sien,			/* Interrupt Enable */
	       sdid,			/* Destination ID */
	       scntl1,			/* Control register 1 */
	       scntl0,			/* Control register 0 */
	       socl,			/* Output control latch */
	       sodl,			/* Output data latch */
	       sxfer,			/* Synchronus Transfer register */
	       scid,			/* Chip ID register */
	       sbcl,			/* Bus Control Lines register */
	       sbdl,			/* Bus data lines register */
	       sidl,			/* Input data latch register */
	       sfbr,			/* First byte received register */
	       sstat2,			/* status register 2 */
	       sstat1,			/* status register 1 */
	       sstat0,			/* status register 0 */
	       dstat,			/* DMA status register */
	       *dsa,			/* data structure address (Nexus/Command) */
	       ctest3,			/* chip test registers */
	       ctest2,
	       ctest1,
	       ctest0,
	       ctest7,
	       ctest6,
	       ctest5,
	       ctest4;
	u_int32  temp;			/* temporary holding register */
	u_int8 lcrc,			/* longitudinal parity register */
	       ctest8,			/* chip test register */
	       istat,			/* interrupt status register */
	       dfifo,			/* DMA FIFO control register */
	       dcmd,			/* SIOP command and byte count register */
	       dbc[3],			/*    byte count portion of above 24 bits */
	       *dnad,			/* DMA buffer pointer (next address) */
	       *dsp,			/* SIOP scripts pointer register */
	       *dsps,			/* SIOP scripts pointer save register */
	       scratch3,		/* general purpose scratch registers */
	       scratch2,
	       scratch1,
	       scratch0,
	       dcntl,			/* DMA control register */
	       dwt,				/* DMA watchdog timer register */
	       dien,			/* DMA interrupt enable */
	       dmode;			/* DMA operation mode register */
	u_int32 adder;			/* SIOP address adder result register. */
} *Siop_p;			/* end of hardware defintion */

/*
 *  Bit definitions for the chip registers.
 *
 *     Note:   many of the bits described below are for illustration and
 *             debugging only as they are controlled by script operations.
 */
 
/* sien & sstat0  - SCSI Interrupt Enable / SCSI Status zero */
#define B_PM     0x80		/* Phase Mismatch (i) , ATN active (t) */
#define B_FCMP   0x40		/* Function Complete */
#define B_STO    0x20		/* selection (i), reselection (t) timeout */
#define B_SEL    0x10 		/* Selected or Reselected */
#define B_SGE    0x08 		/* SCSI Gross Error */
#define B_UDC    0x04		/* Unexpected Disconnect */
#define B_RRST   0x02		/* SCSI Bus Reset received */
#define B_PAR    0x01		/* Parity Error */

/* sdid & scid - Chip and Destination SCSI ID registers */
#define B_ID7   0x80		/* Set indcates ID = 7 */
#define B_ID6   0x40
#define B_ID5   0x20
#define B_ID4   0x10
#define B_ID3   0x08
#define B_ID2   0x04
#define B_ID1   0x02
#define B_ID0   0x01		/* Set indicates  ID = 0 */

/*  scntl1 -  SCSI Control 1 */
#define B_EXC   0x80		/* Extra Clock Cycle of Data Setup */
#define B_ADB   0x40		/* Assert SCSI data Bus */
#define B_ESR   0x20		/* Enable Selection and Reselection  */
#define B_CON   0x10		/* Connected (flag: Connected to Bus) */
#define B_RST   0x08		/* Assert SCSI RST signal */
#define B_AESP  0x04		/* Assert even SCSI parity (force bad parity) */
#define B_SND   0x02		/* Start SCSI Send Operation */
#define B_RCV   0x01		/* Start SCSI Receive Operation */

/*  scntl0 -  SCSI Control 0 */
#define B_ARB1  0x80		/* arbitration mode 1 */
#define B_ARB0  0x40		/* arbitration mode 0 */
#define B_START 0x20		/* Start sequence */
#define B_WATN  0x10		/* Select with ATN on start sequence */
#define B_EPC   0x08		/* Enable parity checking */
#define B_EPG   0x04		/* Enable parity generation */
#define B_AAP   0x02		/* Assert ATN on Parity Error */
#define B_TRG   0x01		/* Target mode */

/* socl (Read/Write)  & sbcl (Read Only) - Input/Output Bus control latch */
#define B_AREQ   0x80		/* Assert REQ ( Request ) */
#define B_AACK   0x40		/* Assert ACK ( acknowledge ) */
#define B_ABSY   0x20		/* Assert BSY (busy )  */
#define B_ASEL   0x10 		/* Assert SEL (select) */
#define B_AATN   0x08 		/* Assert ATN (attention) */
#define B_AMSG   0x04		/* Assert MSG (message) */
#define B_AC_D   0x02		/* Assert CD  ( Command/Data ) */
#define B_AI_O   0x01		/* Assert IO  ( Input/Output ) */

	/* sbcl write uses these */
#define B_SXFD1  0x02					/* synchronous SCSI clock divider.... */
#define B_SXFD0  0x01					/* ... see dcntl below */
#define M_DIV1   B_SXFD0				/* divide by 1 */
#define M_DIV15  B_SXFD1				/* divide by 1.5 */
#define M_DIV2   (B_SXFD1 | B_SXFD0)	/* divide by 2 */
#define USEDCNTL 0						/* if 0, uses DCNTL rate for transfers */

/* sxfer - SCSI Transfer register */
#define B_DHP     0x80		/* Disable Halt on: parity err (i), ATN (t) */
#define BN_TP0    0x04		/* Bits 4-6 describe synch. xfer period */
#define BN_OFS    0x00		/* Bits 3-0 describe synch. offset */
#define MAX_XFERP 0x07		/* largest value for xferp clocks */

#define MAX_SYNCOFFSET 0x8 	/* maximum REQ/ACK offset */
#define MIN_PERIOD     25   /* minimum allowable period message (100 nsec) */
#define SXFR_PMSK      0x70	/* mask of the Transfer period bits */
#define SXFR_OMSK      0x0F	/* mask of the Offset size mask */

/* dstat - DMA status register */
#define B_DFE   0x80		/* DMA FIFO empty */
#define B_RES   0x40		/* ....  Reserved .... */
#define B_BF    0x20		/* Bus Fault */
#define B_ABRT  0x10		/* Aborted */
#define B_SSI   0x08		/* Script Step Interrupt */
#define B_SIR   0x04		/* Script interrupt Instruction Received */
#define B_WTD   0x02		/* Watchdog Timeout Detected */
#define B_IID   0x01		/* Illegal Instruction Detected */

/* sstat1 - SCSI Status One register */
#define B_ILF   0x80		/* SIDL SCSI Input Data  Latch full */
#define B_ORF   0x40		/* SODR register full */
#define B_OLF   0x20		/* SODL register full */
#define B_AIP   0x10		/* Arbitraton In progress */
#define B_LOA   0x08		/* Lost Arbitration */
#define B_WOA   0x04		/* Won Arbitration */
#define B_CRST  0x02		/* Current state of RST line (not latched ) */
#define B_CSDP  0x01		/* Current state of Parity line (not latched ) */

/* sstat2 -  SCSI status 2 */
#define BN_FF   0x4 		/* FIFO flags (binary: number of bytes in fifo */
#define B_SDP   0x08		/* Latched SCSI Parity (parity of byte in SIDL ) */
#define B_MSG   0x04		/* Current condition of MSG ....*/
#define B_CD    0x02		/*  ....   CD  .... */
#define B_IO    0x01		/*  ....   IO  */

/* ctest2 - Chip test register 2 (Read only) */
#define B_RSIGP  0x40  /* if TRUE indicates SIGP set (auto clear SIGP) */
#define B_RSOFF  0x20  /* TRUE:=  synch offset count == 0 (i) */
                       /*         synch offset count == Max offset (t) */
#define B_RSFP   0x10  /* Read Parity of last byte read from FIFO */
#define B_RDFP   0x08  /* Read FIFO parity */
#define B_TEOP   0x04  /* SCSI True end of Process */
#define B_DREQ   0x02  /* Data Request Status */
#define B_DACK   0x01  /* Data Acknowledge Status */

/* ctest0 - Chip Test register 0 */
#define B_DTD    0x1   /* data transfer direction ( TRUE == From SCSI ) */

/* ctest7 - Chip Test register 7 */
#define B_CDIS   0x80	/* TRUE:= Cache Burst Disable */
#define BN_SNPC  0x05	/* location of snoop control bits */
#define B_DISST  0x10	/* TRUE:= Disable Selection time out */
#define B_PDFP   0x08	/* FIFO parity when in programmed I/O (not used) */
#define B_EVP    0x04	/* TRUE:= generate parity on host bus */
#define B_TT1    0x02 	/* Assert TT1 on synchronous host bus */
#define B_DIFF   0x01	/* Enable Differential SCSI mode (not used) */

/* ctest8 - Chip test register 8 */
#define B_FLF   0x08		/* Flush DMA FIFO (TRUE:= flush - not auto-clear) */
#define B_CLF   0x04		/* Clear DMA and SCSI FIFO's */
#define B_FPM   0x02		/* Fetch Pin Mode */
#define B_SPM   0x01		/* Snoop Pins Mode */

/* istat - interrupt status register */
#define B_SABRT   0x80		/*  Set 1 - Will cause current operation to abort */
#define B_SRST    0x40		/*  Set 1 - Cause Software Reset of chip  */
#define B_SSIGP   0x20		/*  Signal process */
#define B_SSCON   0x08 		/*  Connected status bit  */
#define B_SSIP    0x02		/*  SCSI Interrupt Pending */
#define B_SDIP    0x01		/*  DMA Interrupt Pending */

/* dmode - DMA mode register */
#define B_BL1   0x80		/* Burst length 1 */
#define B_BL0   0x40		/* Burst length 0 */
#define B_FC2   0x20		/* Function Code 2 */
#define B_FC1   0x10		/*  ""      ""   1 */
#define B_FC0   0x08		/*  ""      ""   0 */
#define B_PD    0x04		/* Program or Data select */
#define B_FAM   0x02		/* Fixed address mode */
#define B_MAN   0x01		/* Manual Start mode */

/* dien - DMA interrupt enable */
#define B_EBF     0x20		/* Bus Fault */
#define B_EABRT   0x10 		/* Aborted */
#define B_ESSI    0x08 		/* Script Step Interrupt */
#define B_ESIR    0x04		/* Script Interrupt Instruction Received */
#define B_EWTD    0x02		/* Watchdog timeout enabled */
#define B_EILI    0x01		/* Illegal instruction received */

/* dcntl - DMA control register */
#define B_CF1   0x80		/* clock frequency bit 1 (core clock divisor) */
#define B_CF0   0x40		/* clock frequency bit 0 */
#define BN_CF   0x06		/* Clock frequency location (bit number)  */
#define B_EA    0x20		/* Enable Ack */
#define B_SSM   0x10 		/* Single Step Mode */
#define B_LLM   0x08 		/* Enable low level mode */
#define B_ESTD  0x04		/* Start DMA Operation */
#define B_FSTA  0x02		/* Fast Arbitration of local bus */
#define B_COM   0x01		/* 53c700 compatablilty mode */

/*  various and sundry masks that are used by the driver
 */
#define DBC_MSK   0xFFFFFF	/* SIOP dbc register is 24 bits   */
#define DFIFO_MSK 0x7F		/* SIOP dfifo is has 7 valid bits */
#define RESID_OP  0x7F		/* mask applied to determine residual in DMA FIFO */

#define SIOP_INT_MSK 0x0000FFFF	/* only the low word is currently used */

#define PH_MASK    0x0F  /* phase is described in low 4 bits */


#elif defined(NCR53C8XX)
 
/*   NCR 53C810 Register Definition */

typedef volatile struct ncr53c8xx_regs
{
	u_int8	scntl0,		/* 00 SCSI Control 0 	*/
			scntl1,		/* 01 SCSI Control 1 	*/
			scntl2,		/* 02 SCSI Control 2 	*/
			scntl3,		/* 03 SCSI Control 3 	*/
			scid,		/* 04 SCSI Chip ID   	*/
			sxfer,		/* 05 SCSI Transfer		*/
			sdid,		/* 06 SCSI Destination ID	*/
			gpreg,		/* 07 General Purpose Bits	*/
			sfbr,		/* 08 SCSI First Byte Received	*/
			socl,		/* 09 SCSI Output Control Latch */
			ssid,		/* 0A SCSI Selector ID	*/
			sbcl,		/* 0B SCSI Bus Control Lines	*/
			dstat,		/* 0C DMA Status 		*/
			sstat0,		/* 0D SCSI Status 0		*/
			sstat1,		/* 0E SCSI Status 1		*/
			sstat2;		/* 0F SCSI Status 2		*/
	u_int32	dsa;		/* 10-13 Data Structure Address	*/
	u_int8	istat,		/* 14 Interrupt Status 	*/
			res1[3],
			ctest0,		/* 18 Chip Test 0		*/	
			ctest1,		/* 19 Chip Test 1		*/
			ctest2,		/* 1A Chip Test 2		*/
			ctest3,		/* 1B Chip Test 3		*/
			temp[4],	/* 1C-1F Temporary Stack		*/
			dfifo,		/* 20 DMA FIFO			*/
			ctest4,		/* 21 Chip Test 4		*/
			ctest5,		/* 22 Chip Test 5		*/
			ctest6,		/* 23 Chip Test 6		*/
			dbc[3],		/* 24-26 DMA Byte Count */
			dcmd;		/* 27 DMA Command		*/
	u_int32	dnad,		/* 28-2B DMA Next Address for Data	*/
			dsp,		/* 2C-2F DMA SCRIPTS Pointer	*/
			dsps;		/* 30-33 DMA SCRIPTS Pointer Save 	*/
	u_int8	scratch0,
			scratch1,
			scratch2,
			scratch3;/* 34-37 General Purpose Scratch Pad A	*/

	u_int8	dmode,		/* 38 DMA Mode */
			dien,		/* 39 DMA Interrupt Enable 	*/
			dwt,		/* 3A DMA Watchdog Timer 	*/
			dcntl;		/* 3B DMA Control			*/
	u_int32	adder;	/* 3C-3F Sum output of internal adder */
	u_int8	sien0,		/* 40 SCSI Interrupt Enable 0	*/
			sien1,		/* 41 SCSI Interrupt Enable 1	*/
			sist0,		/* 42 SCSI Interrupt Status 0	*/
			sist1,		/* 43 SCSI Interrupt Status 1	*/
			slpar,		/* 44 SCSI Longitudial Parity	*/
			res2,		/* 45 */
			macntl,		/* 46 Memory Access Control		*/
			gpcntl,		/* 47 General Purpose Control	*/
			stime0,		/* 48 SCSI Timer 0				*/
			stime1,		/* 49 SCSI Timer 1				*/
			respid,		/* 4A Response ID				*/
			res3,		/* 4B */
			stest0,		/* 4C SCSI Test 0	*/
			stest1,		/* 4D SCSI Test 1	*/
			stest2,		/* 4E SCSI Test 2	*/
			stest3,		/* 4F SCSI Test 3	*/
			sidl,		/* 50 SCSI Input Data Latch		*/
			sidl1,		/* 51 */
			res4[2],	/* 52-53 */
			sodl,		/* 54 SCSI Output Latsh 		*/
			sodl1,		/* 55 SCSI Output Latsh 		*/
			res5[2],	/* 56-57 */
			sbdl,		/* 58 SCSI Bus Data Lines		*/
			sbdl1,		/* 58 SCSI Bus Data Lines		*/
			res6[2];	/* 5a-5b */
	u_int32	scratchB;/* 5C-5F General Purpose Scratch Pad B */ 
} *Siop_p;



/*	scntl0,		 00 SCSI Control 0 	*/
#define	B_SCNTL0_ARB1	(1<<7)	/* Arbitration Mode */
#define	B_SCNTL0_ARB0	(1<<6)
#define	B_SCNTL0_START	(1<<5)	/* Start Seruence	*/
#define B_SCNTL0_WATN	(1<<4)	/* Select with SATN/on a start sequence */
#define B_SCNTL0_EPC	(1<<3)	/* Enable parity checking */
#define B_SCNTL0_AAP	(1<<1)	/* Assert SATN/on parity error */
#define B_SCNTL0_TRG	(1<<0)	/* Target mode */

#define ARB_SIMPLE	0
#define ARB_FULL	(B_ARB1|B_ARB0)

/*	scntl1,		 01 SCSI Control 1 	*/
#define	B_SCNTL1_EXC	(1<<7)	/* Extra clock cycle of data setup */
#define B_SCNTL1_ADB	(1<<6)	/* Assert SCSI data bus	*/
#define B_SCNTL1_DHP	(1<<5)	/* Disable Halt on Parity Error(I) or ATN
								   (T)	*/
#define B_SCNTL1_CON	(1<<4)	/* Connected */
#define B_SCNTL1_RST	(1<<3)	/* Assert SCSI RST/signal */
#define B_SCNTL1_AESP	(1<<2)	/* Assert even SCSI parity(force bad parity) */
#define B_SCNTL1_IARB	(1<<1)	/* Immediate Arbitration */
#define B_SCNTL1_SST	(1<<0)	/* Start SCSI Transfer	 */


/*	scntl2,	 02 SCSI Control 2 	*/
#define B_SCNTL2_SDU	(1<<7)	/* SCSI Disconnect Unexpected */

/* Following bits exist on NCR53C82x only. */
#define B_SCNTL2_CMODE  (1<<6)		/* Chained Mode */
#define B_SCNTL2_WIDE_SEND (1<<3) 	/* Wide SCSI Send */
#define B_SCNTL2_WIDE_RECV (1<<0) 	/* Wide SCSI Receive */



/*	scntl3,		 03 SCSI Control 3 	*/
#define B_SCNTL3_ULTRA	(1<<7)	/* ULTRA Enable */
#define B_SCNTL3_SCF2	(1<<6)	/* Synchronous Clock Conversion Factor */
#define B_SCNTL3_SCF1	(1<<5)
#define B_SCNTL3_SCF0	(1<<4)
#define B_SCNTL3_WIDE	(1<<3)	/* Wide Enable */
#define SCNTL3_SCF_MASK (B_SCNTL3_SCF2|B_SCNTL3_SCF1|B_SCNTL3_SCF0)

#define B_SCNTL3_CCF2 	(1<<2)	/* Clock Conversion Factor */
#define B_SCNTL3_CCF1	(1<<1)
#define B_SCNTL3_CCF0	(1<<0)
#define SCNTL3_CCF_MASK (B_SCNTL3_CCF2|B_SCNTL3_CCF1|B_SCNTL3_CCF0)
#define USE_SCNTL3_CCF	0

/* Bit 3 exists on NCR53C82x olny. */
#define	B_SCNTL3_ENWIDE	(1<<3)	/* Enable Wide SCSI */


/* scid,		 04 SCSI Chip ID   	*/
#define	B_SCID_RRE		(1<<6)	/* Enable Response to Reselection */
#define B_SCID_SRE		(1<<5)	/* Enable Response to Selection	*/

/* NCR53C810 */
#define	SCID_ENC_MASK 	   0x07
/* NCR53C82x */
#define	SCID_ENC_WIDE_MASK 0x0f

/* sxfer,		 05 SCSI Transfer		*/
#define CLEAR_SYNC_VALUE 0

#define	B_SXFER_TP2		(1<<7)	/* SCSI Synchronouse Transfer Period */
#define	B_SXFER_TP1		(1<<6)
#define	B_SXFER_TP0		(1<<5)
#define BN_SXFER_TP		5
#define SXFER_TP_MASK	0xE0
#define MIN_PERIOD     25   /* minimum allowable period message (100 nsec) */
#define MAX_XFERP		0x7

#define	B_SXFER_MO3		(1<<3)	/* MAX SCSI Synchronouse offset */
#define	B_SXFER_MO2		(1<<2)
#define	B_SXFER_MO1		(1<<1)
#define	B_SXFER_MO0		(1<<0)
#define SXFER_MO_MASK	0x0f
#define MAX_SYNCOFFSET  B_SXFER_MO3 /* maximum REQ/ACK offset */

#define SYNC_XFERP4		0
#define SYNC_XFERP5		B_TP0
#define SYNC_XFERP6		B_TP1
#define SYNC_XFERP7		(B_TP0|B_TP1)
#define SYNC_XFERP8		B_TP2
#define SYNC_XFERP9		(B_TP0|B_TP2)
#define SYNC_XFERP10	(B_TP1|B_TP2)
#define SYNC_XFERP11	(B_TP0|B_TP1|B_TP2)

#define SYNC_OFF0		0
#define SYNC_OFF1		B_MO0
#define SYNC_OFF2		B_MO1
#define SYNC_OFF3		(B_MO0|B_MO1)
#define SYNC_OFF4		B_MO2
#define SYNC_OFF5		(B_MO0|B_MO2)
#define SYNC_OFF6		(B_MO1|B_MO2)
#define SYNC_OFF7		(B_MO0|B_MO1|B_MO2)
#define SYNC_OFF8		B_MO3

/* sdid,		 06 SCSI Destination ID	*/
#define	SDID_ENC_MASK		0x07
#define	SDID_ENC_WIDE_MASK	0x0f

/* gpreg,		 07 General Purpose Bits	*/

/* Bit 4-2 exist on NCR53C82x only. */ 
#define B_GPREG_GPIO4		(1<<4)	/* General purpose bit 4-2 */
#define B_GPREG_GPIO3		(1<<3)
#define B_GPREG_GPIO2		(1<<2)	

/* NCR53c8xx						*/
#define B_GPREG_GPIO1		(1<<1)	/* General purpose bit 1-0 */
#define B_GPREG_GPIO0		(1<<0)

/* sfbr,		 08 SCSI First Byte Received	*/
/* socl,		 09 SCSI Output Control Latch */
#define B_SOCL_REQ	(1<<7)	/* Assert SCSI REQ/signal */
#define B_SOCL_ACK	(1<<6)	/* Assert SCSI ACK/signal */
#define	B_SOCL_BSY	(1<<5)	/* Assert SCSI BSY/signal */
#define B_SOCL_SEL	(1<<4)	/* Assert SCSI SEL/signal */
#define B_SOCL_ATN	(1<<3)	/* Assert SCSI ATN/signal */
#define B_SOCL_MSG	(1<<2)	/* Assert SCSI MSG/signal */
#define B_SOCL_CD	(1<<1)	/* Assert SCSI CD/signal */
#define B_SOCL_IO	(1<<0)	/* Assert SCSI IO/signal */


/*	ssid,		 0A SCSI Selector ID	*/
#define B_SSID_VAL			(1<<7)	/* SCSI Valid bit */
#define SSID_ENC_MASK		0x07
#define SSID_ENC_WIDE_MASK	0x0f

/* sbcl,		 0B SCSI Bus Control Lines	*/
#define B_SBCL_REQ	(1<<7)	/* SREQ/status	*/
#define B_SBCL_ACK	(1<<6)	/* SACK/status	*/
#define B_SBCL_BSY	(1<<5)	/* SBSY/status	*/
#define B_SBCL_SEL	(1<<4)	/* SSEL/status	*/
#define B_SBCL_ATN	(1<<3)	/* SATN/status	*/
#define B_SBCL_MSG	(1<<2)	/* SMSG/status	*/
#define B_SBCL_CD	(1<<1)	/* SCD/status	*/
#define B_SBCL_IO	(1<<0)	/* SIO/status	*/


/* dstat,		 0C DMA Status 		*/
#define B_DSTAT_DFE		(1<<7)	/* DMA FIFO empty */
#define B_DSTAT_MDPE	(1<<6)	/* Master Data Parity Enable	*/
#define B_DSTAT_BF		(1<<5)	/* Bus fault	*/
#define B_DSTAT_ABRT	(1<<4)	/* Aborted		*/
#define	B_DSTAT_SSI		(1<<3)	/* Signal step interrupt		*/
#define B_DSTAT_SIR		(1<<2)	/* SCRIPTS interrupt instruction */
#define B_DSTAT_IID		(1<<0)	/* Illegal instruction detected */

/* sstat0,		 0D SCSI Status 0		*/
#define B_SST0_ILF		(1<<7)	/* SIDL full	*/
#define B_SST0_ORF		(1<<6)	/* SODR	full	*/
#define B_SST0_OLF		(1<<5)	/* SODL	full	*/
#define B_SST0_AIP		(1<<4)	/* Arbitration in progress	*/
#define B_SST0_LOA		(1<<3)	/* Lost arbitration */
#define B_SST0_WOA		(1<<2)	/* Won arbitration	*/
#define B_SST0_RST		(1<<1)	/* SCSI reset signal	*/
#define B_SST0_SDP		(1<<0)	/* SCSI Parity signal	*/

/* sstat1,		 0E SCSI Status 1		*/
#define B_SST1_FF3		(1<<7)	/* FIFO flags	*/
#define B_SST1_FF2		(1<<6)	
#define B_SST1_FF1		(1<<5)	
#define B_SST1_FF0		(1<<4)
#define B_SST1_SDPL		(1<<3)	/* latched SCSI parity	*/
#define B_SST1_MSG		(1<<2)	/* SCSI MSG/signal		*/
#define B_SST1_CD		(1<<1)	/* SCSI C/D/signal		*/
#define B_SST1_IO		(1<<0)	/* SCSI I/O/signal		*/

#define SST1_FF_MASK	0xf0

/* sstat2;		 0F SCSI Status 2		*/
#define B_SST2_LDSC		(1<<1)	/* Last Disconnect		*/

/* Following bits exist on NCR53C82x only. */
#define B_SST2_SIDL_MSBF	(1<<7)	/* SIDL Most Significant Byte Full	*/
#define B_SST2_ORF			(1<<6)	/* SODR	full	*/
#define B_SST2_OLF			(1<<5)	/* SODL	full	*/
#define B_SST2_SODR_MSBF	(1<<6)	/* SODR Most Significant Byte Full	*/
#define B_SST2_SODL_MSBF	(1<<5)	/* SODL Most Significant Byte Full	*/
#define B_SST2_LSP			(1<<3)  /* Latched SCSI Parity */
#define B_SST2_SDP1_SIG		(1<<0)	/* SCSI SDP1 Signal */

/* u_int32	dsa;		 10-13 Data Structure Address	*/
/* u_int8	istat,		 14 Interrupt Status 	*/
#define B_ISTAT_ABRT	(1<<7)	/* Abort operation		*/
#define B_ISTAT_SRST	(1<<6)	/* Software reset		*/
#define B_ISTAT_SIGP	(1<<5)	/* Signal process		*/
#define B_ISTAT_SEM		(1<<4)	/* Semaphore			*/
#define B_ISTAT_CON		(1<<3)	/* Connected			*/
#define B_ISTAT_INTF	(1<<2)	/* Interrupt on the Flay	*/
#define B_ISTAT_SIP		(1<<1)	/* SCSI interrupt pending 	*/
#define B_ISTAT_DIP		(1<<0)	/* DMA interrupt pending	*/


/* ctest0,		 18 Chip Test 0		*/	
/* ctest1,		 19 Chip Test 1		*/
#define CTEST1_FMT_MASK	0xf0	/* Byte Empty in DMA FIFO MASK	*/
#define CTEST1_FFL_MASK	0x0f	/* Byte Full in DMA FIFO MASK	*/

/* ctest2,		 1A Chip Test 2		*/
#define B_CTEST2_DDIR	(1<<7)	/* Data Transfer Direction	*/
#define B_CTEST2_SIGP	(1<<6)	/* Signal process	*/
#define B_CTEST2_CIO	(1<<5)	/* Configured as I/O	*/
#define B_CTEST2_CM		(1<<4)	/* Configured as Memory	*/
#define B_CTEST2_TEOP	(1<<2)	/* SCSI true end of process	*/
#define B_CTEST2_DREQ	(1<<1)	/* Data request status	*/
#define B_CTEST2_DACK	(1<<0)	/* Data acknowledge status	*/

/* ctest3,		 1B Chip Test 3		*/
#define CTEST3_CHIP_REV_MASK	0xf0
#define B_CTEST3_FLF	(1<<3)	/* Flush DMA FIFO	*/
#define B_CTEST3_CLF	(1<<2)	/* Clear DMA FIFO	*/
#define B_CTEST3_FM		(1<<1)	/* Fetch pin mode	*/
#define B_CTEST3_WRIE	(1<<0)	/* Write and Invalidate Enable */

/* temp[4],	 1C-1F Temporary Stack		*/
/* dfifo,		 20 DMA FIFO			*/
#define DLFIFO_MSK 0x3FF	/* SIOP dfifo large */
#define DFIFO_MSK 0x7F		/* SIOP dfifo is has 7 valid bits */
#define RESID_OP  0x7F		/* mask applied to determine residual in DMA FIFO */

/* ctest4,		 21 Chip Test 4		*/
#define B_CTEST4_BDIS	(1<<7)	/* Burst Disable	*/
#define B_CTEST4_ZMOD	(1<<6)	/* High impendance mode	*/
#define	B_CTEST4_ZSD	(1<<5)	/* SCSI Data High Impedance	*/
#define B_CTEST4_SRTM	(1<<4)	/* Shadow Register Test Mode	*/
#define B_CTEST4_MPEE	(1<<3)	/* Master Parity Error Enable	*/
#define B_CTEST4_FBL2	(1<<2)	/* FIFO byte control	2-0     */
#define B_CTEST4_FBL1	(1<<1)
#define B_CTEST4_FBL0	(1<<0)

#define CTEST4_FBL_MASK	0x07

/* ctest5,		 22 Chip Test 5		*/
#define B_CTEST5_ADCK	(1<<7)	/* Clock address incrementor	*/
#define	B_CTEST5_BBCK	(1<<6)	/* Clock byte counter	*/
#define	B_CTEST5_DFS	(1<<5)	/* DMA FIFO SIZE */
#define B_CTEST5_MASR	(1<<4)	/* Master control for set or reset palses */
#define	B_CTEST5_DDIR	(1<<3)	/* DMA direction */
#define	B_CTEST5_BL2	(1<<2)	/* Burst length bit 2 */
#define	B_CTEST5_B09	(1<<1)	/* Fifo */
#define	B_CTEST5_B08	(1<<0)	/* Fifo */

/* ctest6,		 23 Chip Test 6		*/
/* dbc[3],		 24-26 DMA Byte Count */
#define DBC_MSK		0xffffff

/* dcmd,		 27 DMA Command		*/
/* dnad[4],	 28-2B DMA Next Address for Data	*/
/* dsp[4],		 2C-2F DMA SCRIPTS Pointer	*/

/* dsps[4],	 30-33 DMA SCRIPTS Pointer Save 	*/
#define SIOP_INT_MSK	0X0000FFFF
/* scratchA[4], 34-37 General Purpose Scratch Pad A	*/
#define PH_MASK 0x0f

/* dmode,		 38 DMA Mode */
#define B_DMODE_BL1		(1<<7)		/* Burst length	*/
#define B_DMODE_BL0 	(1<<6)		
#define B_DMODE_SIOM 	(1<<5)		/* Source I/O Memory Enable	*/
#define B_DMODE_DIOM	(1<<4)		/* Destination I/O Memory Enable	*/
#define B_DMODE_ER		(1<<3)		/* Enable Read Line			*/
#define B_DMODE_ERMP 	(1<<2)		/* Enable Read Multiple */
#define B_DMODE_BOF 	(1<<1)		/* 825 Burst op-code fetch enable */
#define B_DMODE_MAN		(1<<0)		/* Manual Start Mode		*/

/* dien,		 39 DMA Interrupt Enable 	*/
#define B_DIEN_MDPE	(1<<6)		/* Master Data Parity Error */
#define B_DIEN_BF	(1<<5)		/* Bus fault	*/
#define B_DIEN_ABRT	(1<<4)		/* Aborted */
#define B_DIEN_SSI	(1<<3)		/* Single step interrupt	*/
#define B_DIEN_SIR	(1<<2)		/* SCRIPTS interrupt instruction received */
#define B_DIEN_IID	(1<<0)		/* Illegal instruction detected */


/* dwt,		 3A DMA Watchdog Timer 	*/
/* dcntl,	 3B DMA Control			*/

#define B_DCNTL_CLSE	(1<<7)	/* Cache Line Size */
#define B_DCNTL_PFF		(1<<6)	/* Pre-fetch Flush */
#define B_DCNTL_PFEN	(1<<5)	/* Pre-fetch Enable */
#define B_DCNTL_SSM		(1<<4)	/* Single-step mode */
#define B_DCNTL_IRQM 	(1<<3)	/* IRQ Mode	*/
#define B_DCNTL_STD		(1<<2)	/* Start DMA Operation	*/
#define B_DCNTL_IRQD	(1<<1)	/* IRQ Disable */
#define B_DCNTL_COM		(1<<0)	/* 53c710 compatibility	*/

/* adder[4],	 3C-3F Sum output of internal adder */
/* sien0,		 40 SCSI Interrupt Enable 0	*/
#define B_SIEN0_MA	(1<<7)	/* SCSI Phase Mismatch(I). SCSI ATN Condition(T) */
#define B_SIEN0_CMP (1<<6)	/* Function Complete */
#define B_SIEN0_SEL	(1<<5)	/* Selected */
#define B_SIEN0_RSL	(1<<4)	/* Reselected */
#define B_SIEN0_SGE	(1<<3)	/* SCSI Gross error */
#define B_SIEN0_UDC	(1<<2)	/* Unexpected Disconnect	*/
#define B_SIEN0_RST	(1<<1)	/* SCSI Reset Condition	*/
#define B_SIEN0_PAR	(1<<0)	/* SCSI Parity Error	*/


/* sien1,		 41 SCSI Interrupt Enable 1	*/
#define B_SIEN1_STO	(1<<2)	/* Selection or Reselection Time-out */
#define B_SIEN1_GEN	(1<<1)	/* General Purpose Timer Expired	*/
#define B_SIEN1_HTH	(1<<0)	/* Handshake to Handshake timer Expired	*/

/* sist0,		 42 SCSI Interrupt Status 0	*/
#define B_SIST0_MA	(1<<7)	/* SCSI Phase Mismatch(I). SCSI ATN Condition(T) */
#define B_SIST0_CMP (1<<6)	/* Function Complete */
#define B_SIST0_SEL	(1<<5)	/* Selected */
#define B_SIST0_RSL	(1<<4)	/* Reselected */
#define B_SIST0_SGE	(1<<3)	/* SCSI Gross error */
#define B_SIST0_UDC	(1<<2)	/* Unexpected Disconnect	*/
#define B_SIST0_RST	(1<<1)	/* SCSI Reset Condition	*/
#define B_SIST0_PAR	(1<<0)	/* SCSI Parity Error	*/

/* sist1,		 43 SCSI Interrupt Status 1	*/
#define B_SIST1_STO	(1<<2)	/* Selection or Reselection Time-out */
#define B_SIST1_GEN	(1<<1)	/* General Purpose Timer Expired	*/
#define B_SIST1_HTH	(1<<0)	/* Handshake to Handshake timer Expired	*/

/* slpar,		 44 SCSI Longitudial Parity	*/
/* macntl,		 46 Memory Access Control		*/
#define B_MAC_TYP3	(1<<7)	/* Chip Type	*/
#define B_MAC_TYP2	(1<<6)	
#define B_MAC_TYP1	(1<<5)
#define B_MAC_TYP0	(1<<4)
#define B_MAC_DWR	(1<<3)	/* Data WR	*/
#define B_MAC_DRD	(1<<2)	/* Data RD	*/
#define B_MAC_PSCPT	(1<<1)	/* Pointer SCRIPTS	*/
#define B_MAC_SCPTS (1<<0)	/* SCRIPTS	*/
#define MAC_CHIP_TYPE_MASK	0xf0

/* gpcntl,		 47 General Purpose Control	*/
#define B_GPCNTL_ME	(1<<7)	/* Master Enable	*/
#define B_GPCNTL_FE	(1<<6)	/* Fetch Enable		*/
/* Bit 4-2 exsit on NCR53C82x only */
#define B_GPCNTL_GPIO4	(1<<4)	/* GPIO1 Enable	*/
#define B_GPCNTL_GPIO3	(1<<3)	/* GPIO1 Enable	*/
#define B_GPCNTL_GPIO2	(1<<2)	/* GPIO1 Enable	*/

#define B_GPCNTL_GPIO1	(1<<1)	/* GPIO1 Enable	*/
#define B_GPCNTL_GCIO0	(1<<0)	/* GPIO0 Enable	*/

/* stime0,		 48 SCSI Timer 0				*/
#define STIME0_HTH_MASK	0xf0
#define STIME0_SEL_MASK	0x0f

#define STIME0_DISABLE	0x00
#define STIME0_100u		0x01
#define STIME0_200u		0x02
#define STIME0_400u		0x03
#define STIME0_800u		0x04
#define STIME0_1600u	0x05
#define STIME0_3200u	0x06
#define STIME0_6400u	0x07
#define STIME0_13m		0x08
#define STIME0_26m		0x09
#define STIME0_51m		0x0a
#define STIME0_102m		0x0b
#define STIME0_205m		0x0c
#define STIME0_410m		0x0d
#define STIME0_820m		0x0e
#define STIME0_1600m	0x0f


/* stime1,		 49 SCSI Timer 1				*/
#define STIME1_GEN_MASK	0x0f

/* respid,		 4A Response ID				*/
/* res3,		 4B */
/* stest0,		 4C SCSI Test 0	*/
#define B_STEST0_SLT	(1<<3)	/* Selection Response Logic Test	*/
#define B_STEST0_ART	(1<<2)	/* Arbitration Priotity Encoder		*/
#define B_STEST0_SOZ	(1<<1)	/* SCSI Synchronouse Offset Zero	*/
#define B_STEST0_SOM	(1<<0)	/* SCSI Synchronouse Offset Maximum */

/* stest1,		 4D SCSI Test 1	*/
#define B_STEST1_SCLK	(1<<7)	/* SCLK	*/
#define B_STEST1_SISO	(1<<6)	/* SCSI Isolation Mode */
#define B_STEST1_DBLEN	(1<<3)	/* SCLK Doubler Enable */
#define B_STEST1_DBLSEL	(1<<2)	/* SCLK Doubler Select */

/* stest2,		 4E SCSI Test 2	*/
#define B_STEST2_SCE		(1<<7)	/* SCSI Control Enable	*/
#define B_STEST2_ROF		(1<<6)	/* Reset SCSI Offset	*/

/* BIT 5 exists on NCR53C82X only */
#define B_STEST2_DIFF		(1<<5)	/* Differencial mode */

#define B_STEST2_SLB		(1<<4)	/* SCSI Loopback Mode	*/
#define B_STEST2_SZM		(1<<3)	/* SCSI High-impedance mode */

/* BIT 2 exists on NCR53C82X only */
#define B_STEST2_AWS		(1<<2)	/* Always Wide SCSI		*/

#define B_STEST2_EXT		(1<<1)	/* Extend SREQ/SACK Filtering	*/
#define B_STEST2_LOW		(1<<0)	/* SCSI Low level Mode	*/

/* stest3,		 4F SCSI Test 3	*/
#define B_STEST3_TE		(1<<7)	/* SCSI TolerANT Enable	*/
#define B_STEST3_STR	(1<<6)	/* SCSI FIFO Test Read	*/
#define B_STEST3_HSC	(1<<5)	/* Halt SCSI Clock	*/
#define B_STEST3_DSI	(1<<4)	/* Disable Signle Initiator Response	*/
#define B_STEST3_TTM	(1<<2)	/* Timer Test Mode	*/
#define B_STEST3_CSF	(1<<1)	/* Clear SCSI FIFO	*/
#define B_STEST3_STW	(1<<0)	/* SCSI FIFO Test Write	*/

/* sidl,		 50 SCSI Input Data Latch		*/
/* sodl,		 54 SCSI Output Latsh 		*/
/* sbdl,		 58 SCSI Bus Data Lines		*/
/* scratchB[4]; 5C-5F General Purpose Scratch Pad B */ 

#endif

/*
 * misc definitions.
*/

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


