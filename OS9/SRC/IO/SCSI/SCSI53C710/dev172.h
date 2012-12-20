/*
 *  dev172.h - Motorola mVme172 SBC specific definitions 
 */

 /*
  * Copyright 1991, 1996 by Microware Systems Corporation
  * Reproduced Under License
  *
  * This source code is the proprietary confidential property of
  * Microware Systems Corporation, and is provided to licensee
  * solely for documentation and educational purposes. Reproduction,
  * publication, or distribution in any form to any party other than
  * the licensee is strictly prohibited.
  */

/*
 *  Because there is a bug in the NCR 53C710 that renders the select
 *  timeout unreliable, we will use a MCChip timer register (#4) to 
 *  generate a software controlled select timeout.  
 *
 *  Refer to  DEFS/mcchip.d and DEFS/mcchip.h  ( MCChip definitions )
 *            DEFS/systype.d   ( mVme172port specific definitions )
 */ 

/*
 *  Count and compare register offsets.  (LWORD offsets)
 */
#define SELECTTO   1000000	/* one seconds worth of timeout (1 usec) */
#ifdef _UCC
_asm("SELECTO equ 1000000");
#else
@SELECTO equ 1000000
#endif

#define NCR710Base 0xFFF47000				/* base address of NCR chip 	*/
#ifdef _UCC
_asm("NCR710Base equ $fff47000");
#else
@NCR710Base equ $fff47000
#endif

#ifdef CBOOT
#define MIRQLVL 2
#define	MVECTOR		(MCChip->IVBR + MCVSCSI)
#endif

#define TVECTOR		(MCChip->IVBR + MCVT4)

/* Snoop control lines
 *
 *  This defines how the LSC0 and LSC1 lines driven by the NCR 53C710
 *  during it's time as bus master.
 *       Modes :=
 *   Read:  Supply Dirty data and Leave Dirty
 *   Write: Sink Byte/Word/Longword Data
 */
#define LSC1 1      /* SC1 driven to 1 */
#define LSC0 0      /* SC0 driven to 0 */

#define LSCX (((LSC1<<1)|(LSC0))<<BN_SNPC)  /* in proper place for register */

