/*
 * $Header:   /h0/MWOS/SRC/DEFS/HW/VCS/pcc2.h_v   1.9   30 Jun 1997 01:15:56   packager  $
 */

/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1996 by Microware Systems Corporation              |
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
/*
 * pcc2.h - Motorola PCC2 (Peripheral Controller Chip) definitions.
 *          ( mVme167/mVme187/mVme166 eta'al )
 *
 * History:
 * Date       Who     Reason
 * ---------  ----  ------------------------------------------------
 * 27 Jan 94  Rwb   Created based on pcc2.d from OS9 port (file ed.2)
 *                  ---- OS-9/68K V3.0.2 Released ----
 *                  ---- OS-9000/x86 V2.1 Released ----              
 *                  ---- OS-9000/PPC V2.1.1 Released ----            
 *                  ---- OS-9000/ARMv3 V2.2 Released ----            
 *                  ---- OS-9000/ARMv3 V2.2.1 Released ----          
 *                  $$               <RELEASE_INFO>                $$
 */

#ifndef _PCC2_H_

#define _PCC2_H_

/*
 *  Conventions used in this file:
 *
 *   B_XXXX  denotes a bit number definition (note access size quoted)
 *   M_XXXX  denotes a bit mask (again, note access size quoted)
 *
 *
 *  Register definitions:
 *   Defined by OFFSET from Base Address rather than Absolute Address.
 *   Most registers are byte/word/longword accessable. (exceptions as noted)
 */

#define PCCChipID	0x0		/* Chip Identification Register    (byte)  */
#define PCCChipRV	0x1		/* Chip Revision Register          (byte)  */
#define PCCGCR		0x2		/* General Control Register        (byte)  */
#define PCCVBR		0x3		/* PCC IRQ Vector Base Register    (byte)  */
#define PCCTK1CMP	0x4		/* Tick Timer 1 Compare Register   (lword) */
#define PCCTK1CNT	0x8		/* Tick Timer 1 Count Register     (lword) */
#define PCCTK2CMP	0xC		/* Tick Timer 2 Compare Register   (lword) */
#define PCCTK2CNT	0x10	/* Tick Timer 2 Count Register     (lword) */
#define PCCTKPRSC	0x14	/* Prescaler Count Register        (byte)  */
#define PCCTKADJ	0x15	/* Prescaler Clock Adjust          (byte)  */
#define PCCTK2CTL	0x16	/* Tick Timer 2 Control Register   (byte)  */
#define PCCTK1CTL	0x17	/* Tick Timer 1 Control Register   (byte)  */
#define PCCGPICR	0x18	/* General Purpose Interr. Control (byte)  */
#define PCCGPCtl	0x19	/* General Purpose I/O Pin Control (byte)  */
#define PCCTk2ICR	0x1A	/* Tick Timer 2 Interrupt Control  (byte)  */
#define PCCTk1ICR	0x1B	/* Tick Timer 2 Interrupt Control  (byte)  */
#define PCCSCERR	0x1C	/* SCC Error Status Register       (byte)  */
#define PCCSCMICR	0x1D	/* SCC Modem Interrupt Control     (byte)  */
#define PCCSCTICR	0x1E	/* SCC Transmit Interrupt Control  (byte)  */
#define PCCSCRICR	0x1F	/* SCC Receiver Interrupt Control  (byte)  */
#define PCCSCMPIAK	0x23	/* SCC Modem Pseudo IACK genertor  (byte)  */
#define PCCSCTPIAK	0x25	/* SCC Transmit Pseudo IACK gen.   (byte)  */
#define PCCSCRPIAK	0x27	/* SCC Receiver Pseudo IACK gen.   (byte)  */
#define PCCLNERR	0x28	/* LANC Error Status Register      (byte)  */
#define PCCLNICR	0x2A	/* LANC Interrupt Control          (byte)  */
#define PCCLNBER	0x2B	/* LANC Bus Error Interrupt Ctrl   (byte)  */
#define PCCSCSIERR	0x2C	/* SCSI Error Status Register      (byte)  */
#define PCCSCSIICR	0x2F	/* SCSI Interrupt Control Register (byte)  */
#define PCCPPACKCT  0x30	/* Printer Port ACK control        (byte)  */
#define PCCPPFLTICR	0x31	/* Printer Port FAULT Int. Control (byte)  */
#define PCCPPSELICR	0x32	/* Printer Port SEL Int.  Control  (byte)  */
#define PCCPPPEICR	0x33	/* Printer Port PE  Int.  Control  (byte)  */
#define PCCPPBSYICR 0x34	/* Printer Port BSY Int.  Control  (byte)  */
#define PCCPPINSTAT	0x36	/* Printer Port Input Status       (byte)  */
#define PCCPPCNTRL	0x37	/* Printer Port Control Register   (byte)  */
#define PCCCHIPSPD	0x38	/* Chip Speed Register        (word only!) */
#define PCCPPDATA	0x3A	/* Printer Port Data Register      (byte)  */
#define PCCPRILVL	0x3E	/* PCC Priority Level Register     (byte)  */
#define PCCINTMSK	0x3F	/* PCC Interrupt mask level        (byte)  */


/*
 * PCCGCR General Control Register
 */
#define B_PCCGCR_FAST	0x01		/* BBRAM access speed (1=Fast, 0=Slow) */
#define B_PCCGCR_MIEN	0x02		/* Master Interrupt Enable */
#define B_PCCGCR_C040	0x04		/* CPU040 -- defines processor type */
#define B_PCCGCR_DR0	0x80		/* Download ROM address */

/*
 * PCCVBR PCC IRQ Vector Base Register
 *
 *   Bits 4-7 define the base interrupt vector.  The PCC defines a block
 *   of 16 interrupts prioritize as below.
 */
#define M_PCCVBR	(0xF<<4)	/* Masks appropriate bits */

/*
 *********************
 *      Processor Dependent Information:
 *   For the mVme167 which is based on the MC68040 CPU chip, the following
 *   vector information does apply.  There will be differences for systems
 *   based on other CPUs.
 *********************
 * Interrupt Vector assignments
 *   The vector base is programmed into PCCVBR and the resultant vectors
 *   produced are then:
 *        PCCVBR+n
 *
 *  0  Printer port BSY Interrupt
 *  1  Printer port PE Interrupt
 *  2  Printer port Select Interrupt
 *  3  Printer port Fault Interrupt
 *  4  Printer port Ack Interrupt
 *
 *  5   NCR 53C710 Interrupt
 *  6   Intel 82549CA LANC Error Interrupt
 *  7   Intel 82549CA LANC Interrupt
 *
 *  8   PCC Timer 2 Interrupt
 *  9   PCC Timer 1 Interrupt
 * $A   PCC GPIO Interrupt
 *
 *   NOTE:  Auto-vector NOT used, CD2401 provides own vector
 * $B   PCC SCC Modem Interrupt (auto-vector only)
 * $C   PCC SCC RX Interrupt    (auto-vector only)
 * $D   PCC SCC TX Interrupt    (auto-vector only)
 */


/*
 * PCCTk2Ctl and  PCCTk1Ctl -  Tick Timer Control Registers
 */
#define B_PCCTK_CEN		0x01	/* Counter Enable (1=Increment  0=Decrement) */
#define B_PCCTK_COC		0x02	/* Clear on Compare (1 = reset on compare)   */
#define B_PCCTK_COVF	0x04	/* Clear on overflow  */
#define M_PCCTK_OVF     0xF0	/* A 4 Bit overflow counter */


/*
 * PCCGPCtl  General Purpose I/O Pin Control
 */

#define PCCGPCTL_GPO	(1<<0)	/* GPOE level select - (1 = HIGH, 0 = LOW)    */
#define PCCGPCtl_GPE	(1<<1)	/* GPOE enable  (1 = PCC Driven , 0 = Extern) */
#define PCCGPCtl_GPI	(1<<2)	/* Reflects state of external reset pin       */


/*
 * PCCLNBErr -  LANC Bus Error Interrupt Control
 */
#define M_PCCLNBER_LVL		0x07	/*Interrupt Request Level Mask (0 = none) */
#define B_PCCLNBER_INTCLR	0x08	/* Interrupt Status Clear bit (1 = Clear) */
#define B_PCCLNBER_INTENB	0x10	/* Interrupt Enable Bit (1 == Enable)     */
#define B_PCCLNBER_INTPEND	0x20	/* TRUE = LANC Bus error inter. pending   */
#define M_PCCLNBER_SNPCTL	(3<<3)	/* Mask for Snoop Control Bits            */

/*  Parallel I/O port (Printer Port) */
#define M_PCCPPINSTAT_BSY  (1<<0)	/* Printer Busy input pin  */
#define M_PCCPPINSTAT_PE   (1<<1)	/* PE input pin  (paper empty) */
#define M_PCCPPINSTAT_SEL  (1<<2)	/* SEL input pin  (select) */
#define M_PCCPPINSTAT_FLT  (1<<3)	/* FAULT input pin (printer fault) */
#define M_PCCPPINSTAT_ACK  (1<<4)	/* ACK input pin */
#define M_PCCPPINSTAT_PINT (1<<7)	/* Interrupt Status (1 == Pending) */

/*
 * PCCPPCntrl -  Printer Port Control Register
 */
#define PCCPPCNTRL_MAN	(1<<0)	/* Manual Strobe Control (0 = Auto Strobe)    */
#define PCCPPCNTRL_FAST	(1<<1)	/* Strobe timing (Auto mode, BCLK dependent)  */
#define PCCPPCNTRL_STB	(1<<2)	/* Manual Strobe (1 = high, 0 = low)          */
#define PCCPPCNTRL_INP	(1<<3)	/* Printer Input Prime (1 == Activated)       */
#define PCCPPCNTRL_DOEN	(1<<4)	/* Printer Data Output Enable (1=Out, 0=In)   */

/******
 *  These definitions have general application accross a number of 
 *  registers.   The register offsets are defined above.
 *****/

/*
 * PCCSCErr   -  SCC Error Status Register
 * PCCSCSIErr -  SCSI Error Status Register
 * PCCLNErr   -  LANC Error Status Register
 *   If a bus error occurs durring the Devices DMA cycle then this register
 *   can be used to determine the cause of the error.
 */
#define PCCERR_SCLR		(1<<0)	/* Status clear (Write 1 Clears bits 1 - 3) */
#define PCCERR_LTO		(1<<1)	/* Local Time Out */
#define PCCERR_EXT		(1<<2)	/*Vme Bus Access Time out */
#define PCCERR_PRTY		(1<<3)	/* DRAM parity check error */
#define PCCERR_RTRY		(1<<4)	/* Retry was required */

/*
 * PCCSCMicr   -  SCC Modem Interrupt Control
 * PCCSCTicr   -  SCC Transmit Interrupt Control
 * PCCSCRicr   -  SCC Receiver Interrupt Control
 * PCCLNicr    -  LANC Interrupt Control
 * PCCSCSIicr  -  SCSI Interrupt Control
 * PCCPPIackCt -  Printer Port IACK control
 * PCCPPFlticr -  Printer Port FAULT Int.  Control
 * PCCPPSELicr -  Printer Port SEL Int.  Control
 * PCCPPPEicr  -  Printer Port PE  Int.  Control
 * PCCPPBSYicr -  Printer Port BSY Int.  Control
 * PCCTk1ICR   -  Tick Timer Interrupt Control Register 1
 * PCCTk2ICR   -  Tick Timer Interrupt Control Register 2
 * PCCGPICR    -  General Purpose Interrupt Control
 */
#define M_PCCICR_LVL	0x07	/* Interrupt Request Level (0 = no inters)   */
#define M_PCCICR_AVEC	(1<<3)	/* Auto vector enable (not used in mVme167   */
#define M_PCCICR_ICLR   (1<<3)	/* Edge Inter. Clear bit [SCSI, LANC, PP]    */
#define B_PCCICR_IEN	0x10	/* Interrupt Enable Bit  (1 == Enabled)      */
#define M_PCCICR_INT	(1<<5)	/* Interrupt Status  ( 1 == IRQ pending)     */

/*  for the LANC, SCSI, and PP these two bits are also available.....        */
#define B_PCCICR_MEL	(1<<6)	/* Interrupt Mode (1 == Edge, 0 == Level)    */
#define B_PCCICR_PLTY   0x80	/* Interrupt Polarity (1 = rise, 0 = fall)   */

#endif /*  _PCC2_H_ */
