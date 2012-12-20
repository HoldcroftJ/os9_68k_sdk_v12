/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1999 by Microware Systems Corporation              |
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
|  01 99/12/12 Added edition history.                                  kimk |
|  02 99/12/17 Added IO bases addresses                                 mgh  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/
/*
 * useful m68360 defines that are likely defined elsewhere
 */
#define CB(x) 	((volatile u_int8  *)(x))
#define CW(x) 	((volatile u_int16 *)(x))
#define CL(x) 	((volatile u_int32 *)(x))

#ifndef IMMR_S
#define IMMR_S	(ldptr->immr)		/* current IMMR location */
#endif

#define PADIR	CW(IMMR_S+0x550)		/* port A direction reg */
#define PAPAR	CW(IMMR_S+0x552)		/* port A pin assignment reg */
#define PAODR	CW(IMMR_S+0x554)		/* port A open drain reg */
#define PADAT	CW(IMMR_S+0x556)		/* port A data reg */

#define PCDIR	CW(IMMR_S+0x560)		/* port C direction reg */
#define PCPAR	CW(IMMR_S+0x562)		/* port C pin assignment reg */
#define PCSO	CW(IMMR_S+0x564)		/* port C special options reg */
#define PCDAT	CW(IMMR_S+0x566)		/* port C data reg */
#define PCINT	CW(IMMR_S+0x568)		/* port C interrupt control reg */

#define PIPC	CW(IMMR_S+0x6b2)		/* PIP configuration reg */
#define PTPR	CW(IMMR_S+0x6b6)		/* PIP timing parameters reg */
#define PBDIR	CL(IMMR_S+0x6b8)		/* port B direction reg */
#define PBPAR	CL(IMMR_S+0x6bc)		/* port B pin assignment reg */
#define PBODR	CW(IMMR_S+0x6c2)		/* port B open drain reg */
#define PBDAT	CL(IMMR_S+0x6c4)		/* port B data reg */

#define TGCR	CW(IMMR_S+0x580)		/* timer global config reg */
#define TMR1	CW(IMMR_S+0x590)		/* timer1 mode reg */
#define TMR2	CW(IMMR_S+0x592)		/* timer2 mode reg */
#define TRR1	CW(IMMR_S+0x594)		/* timer1 reference reg */
#define TRR2	CW(IMMR_S+0x596)		/* timer2 reference reg */
#define TCR1	CW(IMMR_S+0x598)		/* timer1 capture reg */
#define TCR2	CW(IMMR_S+0x59a)		/* timer2 capture reg */
#define TCN1	CW(IMMR_S+0x59c)		/* timer1 counter */
#define TCN2	CW(IMMR_S+0x59e)		/* timer2 counter */
#define TMR3	CW(IMMR_S+0x5a0)		/* timer3 mode reg */
#define TMR4	CW(IMMR_S+0x5a2)		/* timer4 mode reg */
#define TRR3	CW(IMMR_S+0x5a4)		/* timer3 reference reg */
#define TRR4	CW(IMMR_S+0x5a6)		/* timer4 reference reg */
#define TCR3	CW(IMMR_S+0x5a8)		/* timer3 capture reg */
#define TCR4	CW(IMMR_S+0x5aa)		/* timer4 capture reg */
#define TCN3	CW(IMMR_S+0x5ac)		/* timer3 counter */
#define TCN4	CW(IMMR_S+0x5ae)		/* timer4 counter */
#define TER1	CW(IMMR_S+0x5b0)		/* timer1 event register */
#define TER2	CW(IMMR_S+0x5b2)		/* timer2 event register */
#define TER3	CW(IMMR_S+0x5b4)		/* timer3 event register */
#define TER4	CW(IMMR_S+0x5b6)		/* timer4 event register */

#define CPCR	CW(IMMR_S+0x5c0)		/* comm processor command register */
#define RCCR	CW(IMMR_S+0x5c4)		/* RISC configuration register */
#define RTER	CW(IMMR_S+0x5d6)		/* RISC Timers event register */
#define RTMR	CW(IMMR_S+0x5da)		/* RICS Timers mask register */

#define CICR	CL(IMMR_S+0x540)		/* CPM interrupt configuration register */
#define CIPR	CL(IMMR_S+0x544)		/* CPM interrupt pending register */
#define CIMR	CL(IMMR_S+0x548)		/* CPM interrupt mask register */

#define TM_BASE	CW(IMMR_S+DPRAM+0xdb0)	/* RISC timer table base address */
#define TM_CMD	CL(IMMR_S+DPRAM+0xdb8)	/* RISC timer command register */

#define CHGBITS(val, mask, newbits) (((val) & ~(mask)) | (newbits))


/* Base address of Internal Registers */
#define REGB    	(0x1000)   		

/* 68360 IO Base Address offsets in DPRam */
#define SCC1_BASE   (0xC00)
#define MISC_BASE   (0xCB0)
#define SCC2_BASE   (0xD00)
#define SPI_BASE    (0xD80)
#define TIMER_BASE  (0xDB0)
#define SCC3_BASE   (0xE00)
#define IDMA1_BASE  (0xE70)
#define SMC1_BASE   (0xE80)
#define SCC4_BASE   (0xF00)
#define IDMA2_BASE  (0xF70)
#define SMC2_BASE   (0xF80)

/* MC68360 SIM registers (a select few) */
#define PICR		(REGB+0x26)
#define PITR 		(REGB+0x2a)

