/*
 *  dev167.h - Motorola mVme167 SBC specific definitions 
 */

 /*
  * Copyright 1991-1996 by Microware Systems Corporation
  * Reproduced Under License
  *
  * This source code is the proprietary confidential property of
  * Microware Systems Corporation, and is provided to licensee
  * solely for documentation and educational purposes. Reproduction,
  * publication, or distribution in any form to any party other than
  * the licensee is strictly prohibited.
  * 
  * Edit History:
  * Date       Who   Reason
  * ---------  ----  --------------------------------------------------
  * 02 Mar 95  GbG   ANSI version of driver
 */

#ifndef _DEV167_H_
#define _DEV167_H_

/*
 *  Because there is a bug in the NCR 53C710 that renders the select
 *  timeout unreliable, we will use a VMEChip2 timer register to 
 *  generate a software controlled select timeout.  
 *
 *  Refer to  DEFS/vmechip2.h  ( VMEChip2 definitions )
 *            DEFS/systype.h   ( mVme167 port specific definitions )
 */ 

#define PCCCHBASE     (IOBASE+0x42000)  /* PCCchip base address  */
#define VMECHBASE     (IOBASE+0x40000)  /* VMEchip base address (LCSR) */
#define IOBASE        0xFFF00000     /* On-board I/O devices start here  */

#ifndef _VMECHIP2_H_
#include <vmechip2.h>      /* for timer definitions */
#endif /* _VMECHIP2_H_ */

#ifndef _MEMC040_H_
#include <memc040.h>
#endif /*  _MEMC040_H_ */


#define SELECTTO   1000000  /* one seconds worth of timeout (1 usec) */

#define M_TIC1LVL 0x7   /* interrupt level mask for Tick timer 1 */

/*  these bits are replicated in terms of purpose through the LBISR, LBIER,
 *  and INTCLR registers.
 */
#define M_IRQTIC1 0x1000000     /* enable/disable interrupt, check status */

#define TT1COVF 0x4         /* Set 1, counter reset to zero at compare */
#define TT1COC  0x2         /* Set 1, clears overflow counter */
#define TT1EN   0x1         /* Set 1, enables counter */

/*
 *  These definitions refer to the level and vector for the timer.  They
 *  should always agree with the systype file.
 */
#define VmeChVBR0_default 0x60      /* Define the high 4 bits of the vector */

#define VCT1Vect VmeChVBR0_default+0x8   /*  VMEChip Timer 1  */

#ifndef _PCC2_H_
#include <pcc2.h>           /* for interrupt vector registers ect. */
#endif /* _PCC2_H_ */

#define PCC2_IEN   0x10                     /* interrupt enable bit         */
#define PCC2_SCLR  0x10000                  /* clear error conditions       */
#define PCC2_LTO   0x20000                  /* Local bus timeout status     */
#define PCC2_EXT   0x40000                  /* Bus error from VMEbus        */
#define PCC2_PTY   0x80000                  /* Memory Parity error on fetch */

/* Snoop control lines
 *  
 *  This defines how the LSC0 and LSC1 lines driven by the NCR 53C710
 *  during it's time as bus master.
 *       Modes :=
 *   Read:  Supply Dirty data and Leave Dirty
 *   Write: Sink Byte/Word/Longword Data
 */
#define LSC1 0      /* SC1 driven to 0 */
#define LSC0 1      /* SC0 driven to 1 */

#define LSCX (((LSC1<<1)|(LSC0))<<BN_SNPC)  /* in proper place for register */
   
#define NCR710Base 0xFFF47000				/* base address of NCR chip 	*/
#ifdef _UCC
_asm("NCR710Base equ $fff47000");
_asm("SELECTO equ 1000000");
#else
@NCR710Base equ $fff47000
@SELECTO equ 1000000
#endif

#define MEMC40BCR 0xFFF4301C	/* must be BCK speed in MHZ or RAM fails! */

#ifdef CBOOT
#define MIRQLVL 2
#define MVECTOR 0x65
#endif

#endif
