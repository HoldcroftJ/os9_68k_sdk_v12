/*
 * $Header:   /h0/MWOS/SRC/DEFS/HW/VCS/vmechip2.h_v   1.16   26 Sep 1997 17:05:56   allan  $
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
 * vmechip2.h - Motorola VME Interface chip Version 2 definitions.
 *          ( mVme167/mVme187/mVme166/mVme1601 PPC eta'al )
 *
 * History:
 * Date       Who     Reason
 * ---------  ----  ------------------------------------------------
 * 11 Feb 94  Rwb   Created based on vmechip2.d from OS9 port, first
 *                     level edit, enough to do tick timer code.
 * 13 Jul 94  GbG   Error in struct for VMECHIP2 (no intcrl)
 * 07 Mar 95  GbG   Volatile added with ANSI and NON ANSI cases
 * 25 May 95  ats   Added default vmechip2 address & properly casted const.
 * 26 May 95  ats   Added structure for timer control register.
 * 15 Jun 95  ats   Refined vmetcr structure.
 *                  ---- OS-9/68K V3.0.2 Released ----
 *                  ---- OS-9000/PPC V2.0.1 Released ----
 *                  ---- OS-9/68K V3.0.3 System-state Debug Support Release --
 *                  ---- OS-9000/x86 V2.1 Released ----
 *                  ---- OS-9000/PPC V2.1.1 Released ----                       
 *                  ---- OS-9000/ARMv3 V2.2 Released ---- 
 *                  ---- OS-9000/ARMv3 V2.2.1 Released ----
 *                  $$                <RELEASE_INFO>                   $$ *
 */

#ifndef _VMWCHIP2_H_

#define _VMWCHIP2_H_

#ifndef _TYPES_H
#include "types.h"
#endif /*  _TYPES_H */

/*
 *  The VMEChip2 is a VME Bus interface chip that contains a pair of
 *  general purpost timers, a D64 capable DMA  (local bus->VME Bus ONLY!!),
 *  and the VME/Local Bus address translation registers.  This chip is
 *  long word (32 Bit) access only.  This governs the methodology of access
 *  to the chip.
 *
 *  Conventions used in this file:
 *
 *   B_XXXX  denotes a bit number definition (note access size quoted)
 *   M_XXXX  denotes a bit mask (again, note access size quoted)
 *
 *
 *  Due to the complexity and vararies of programming this part, the
 *  user is directed to consult the Motorola Manual for the Particular
 *  board on which this part is located.
 *
 *  For the MVME167/187:
 *  MVME167/MVME187 Single Board Computers Programmer's Reference Guide`
 *    (MVME187PG/D1  Copyright Motorola 1991)
 */

#ifndef	VMEchip2Base
#define	VMEchip2Base	0xfff40000
#endif

#if !defined  (__STDC__) && !defined (_ANSI_EXT) && !defined (volatile)
#define volatile /* the old compiler doesn't understand 'volatile' */
#endif
 
/* LCSR
 * This is a definition of the Local Control and Status register set
 * of the VMEChip2.  Note that while it is possible to do D8/D16/D32
 * reads from this portion of the chip,  ALL writes to this portion of
 * the chip must be D32.
 */

typedef volatile struct vmechip2 {
	u_int32  vmeslave_se1,		/* VME Slave End and Start address 1     (0)  */
	         vmeslave_se2,		/* VME Slave End and Start address 2     (4)  */
	         vmeslave_ta1,		/* VME Slave Address translation 1       (8)  */
	         vmeslave_ta2,		/* VME Slave Address translation 2       (C)  */
	         vmeslave_atr,		/* VME Slave attributes (snoop,AM, ect)  (10) */
             lcbslave_se1,		/* local bus Slave End & Start address 1 (14) */
	         lcbslave_se2,		/* local bus Slave End & Start address 2 (18) */
	         lcbslave_se3,		/* local bus Slave End & Start address 3 (1C) */
	         lcbslave_se4,		/* local bus Slave End & Start address 4 (20) */
	         lcbslave_ta1,		/* lclb Slave Address translation 4      (24) */
	         lcbslave_atr,		/* lclb Slave attributes (snoop,AM, ect) (28) */
	         vmeslave_gcsr,		/* VME bus slave GCSR Group, Lcl bus ctrl(2C) */
	         dmacctrl_1,   		/* DMAC control, Local/VME requester ctrl(30) */
	         dmacctrl_2,  		/* DMAC control register 2               (34) */
	         dmaclclad_cntr,	/* DMAC local bus address counter        (38) */
	         dmacvmead_cntr,	/* DMAC VME bus address counter          (3C) */
	         dmacbyte_cntr,		/* DMAC Byte count register              (40) */
	         dmactblad_cntr,	/* DMAC Table Address Counter.           (44) */
	         vmeintr_ctrl,		/* DMAC control, VME interr, MPU Stat    (48) */
	         vmelcltim_ctrl,	/* Timer/counter controls (Watchdog, ect)(4C) */
	         tick1_compare,		/* Tick Timer 1 Compare Register         (50) */
	         tick1_counter,		/* Tick Timer 1 Counter Register         (54) */
	         tick2_compare,		/* Tick Timer 2 Compare Register         (58) */
	         tick2_counter,		/* Tick Timer 2 Counter Register         (5C) */
	         board_ctrl,		/* Various board controls                (60) */
	         prescaler_cntr,	/* Prescaler Counter                     (64) */
	         lclbus_status,		/* Local bus status interrupter status   (68) */
	         lclbus_enable,		/* local bus interrupt enable register   (6C) */
	         swinter,			/* Software Interrupt and Inter. clear   (70) */
			 intclr,			/* Interrupt Clear Register              (74) */
	         interrupt_lvl1,	/* Interrupt Level register 1            (78) */
	         interrupt_lvl2,	/* Interrupt Level register 2            (7C) */
	         interrupt_lvl3,	/* Interrupt Level register 3            (80) */
	         interrupt_lvl4,	/* Interrupt Level register 4            (84) */
	         irqvbr_ioctrl;		/* Chip Vector base and I/O control      (88) */

} vmech2_t, *Vmech2_p;

#define	VMEChip2	((Vmech2_p)VMEchip2Base)	/* properly casted constant */

/*  GCSR
 *  This is a definition of the Global Control and Status register set
 *  that is also included in the VMEChip2.  Note that accesses to this
 *  portion of the VMEChip two can be either of D32 or D8 for both read
 *  and write.
 *
 *  NOTE:  two offsets are provided:
 *          (x)  indicates local bus offset.
 *          <x>  indicates VME Bus offset.
 *  This definition is for the GCSR as seen from the local bus.
 */
typedef volatile struct vmech2gcsr {

	u_int16  gcsr_revid,		/* Bits 0->7: ID, Bits: 8->15: Rev.  (0)  <0> */
	           hwres1;
	u_int8   gcsr_lmsig,		/* Location Monitor and Signal bits  (4)  <2> */
	         gcsr_status;		/* GCSR status and control register  (5)  <3> */

	u_int16    hwres2,
	         gcsr_gpreg1,		/* General Purpose Register 1        (8)  <4> */
	           hwres3,
	         gcsr_gpreg2,		/* General Purpose Register 2        (10) <8> */
	           hwres4,
	         gcsr_gpreg3,		/* General Purpose Register 3        (14) <A> */
	           hwres5,
	         gcsr_gpreg4,		/* General Purpose Register 4        (18) <C> */
	           hwres6,
	         gcsr_gpreg5;		/* General Purpose Register 5        (1C) <E> */

} gcsr_t, *GCSR_p;

#define	VMEChip2GCSR	((GCSR_p)(VMEchip2Base+0x100))	/* properly casted constant */

/*
 *  Due to the requirement of D32 access to the LCSR and the libral mixture
 *  of purpose within some of the registers, we define the various 
 *  structures for ease of use and readability in 'C' code.  The longword
 *  within the LCSR is referenced within the comments for the various 
 *  structures.
 */

/*
 *  Tick Timers - There are 2 complete tick timers available on the
 *  VMEChip2.  Both of these timers increment on 1 micro-second (usec)
 *  ticks.  Setting the Compare registers to time in usec increments and
 *  enabling the interrupt is the normal means of usage.
 *
 * counter and compare registers are 32 bits wide:
 *
 *  Tick1::  tick1_compare and tick1_counter 
 *  Tick2::  tick2_compare and tick2_counter
 *  
 *  Control for the timers is located in the board status and control
 *  register. ( board_ctrl )
 */

typedef union board_control {

		u_int32  bdctrl_lng;

		volatile struct {
			u_int8	bdctrl_sys,		/* sys controller, board fail, others */
			        bdctrl_wdt,		/* primarily watchdog timer control   */
			        bdctrl_tk2,		/* tick timer 2 control register      */
			        bdctrl_tk1;		/* tick timer 1 control register      */
		} bdctrl_reg;

} bdctrl_t, *Bdctrl_p;

/* reference:  bdctrl_t.bdctrl_sys - system controls */
#define BDCTRL_SYSCON  0x40			/* system controller (1 == Board is!) */
#define BDCTRL_SFFL    0x20			/* set high to assert SYSFAIL         */

/*
 * Timer Control Registers (bdctrl_t.bdctr_tk1/2)
 */
#define B_BDCTRL_TKEN   0x01		/* enable ticker */
#define B_BDCTRL_TKCOC  0x02		/* Set 1 := Reset counter on compare. */
#define B_BDCTRL_TKCOVF 0x04		/* Clear Counter overflow register */
#define M_BDCTRL_TKOVF  0xF0		/* mask to obtain overflow count */

typedef struct vmetcr {	/* alternative structure access */
	u_int	OVF:4,			/* Overflow Counter */
			unused:1,
			COVF:1,			/* Clear Overflow */
			COC:1,			/* Clear on Compare */
			CEN:1;			/* Counter Enable */
} *VMEc2TCR;

/*
 * Local Bus Interruptor Status Register
 * Local Bus Interruptor Enable Register
 * Local Bus Interrupt Clear Register and Software Interrupt Set.
 *
 *  These registers maintain the same relative postition for the
 *  devices they represent.  As such  one set of defs for each of the
 *  bits in three registers.   
 */

#define LBIREG_ACF  0x80000000		/* VMEbus AC fail */
#define LBIREG_AB   0x40000000		/* Abort Switch */
#define LBIREG_SYSF 0x20000000		/* VME Bus SYSFAIL */
#define LBIREG_MWP  0x10000000		/* VME Bus Master Write Post Error */
#define LBIREG_PE   0x08000000		/* External Interrupt (parity error) */
#define LBIREG_VI1E 0x04000000		/* VME Bus IRQ1 edge-sensitive interrupt */
#define LBIREG_TIC1 0x01000000		/* Tick timer 1 */
#define LBIREG_TIC2 0x02000000		/* Tick Timer 2 */
#define LBIREG_VIA  0x00800000		/* VME Bus Interrupter Acknowledge */
#define LBIREG_DMAC 0x00400000		/* DMA Controller Interrupt */
#define LBIREG_SIG3	0x00200000		/* GCSR SIG3 interrupt  */
#define LBIREG_SIG2	0x00100000		/* GCSR SIG2 interrupt  */
#define LBIREG_SIG1	0x00080000		/* GCSR SIG1 interrupt  */
#define LBIREG_SIG0	0x00040000		/* GCSR SIG0 interrupt  */
#define LBIREG_LM1	0x00020000		/* GCSR LM1 (location monitor) interrupt  */
#define LBIREG_LM0	0x00010000		/* GCSR LM0 (location monitor) interrupt  */
#define LBIREG_SW7  0x00008000		/* Software Interrupt 7 */
#define LBIREG_SW6  0x00004000		/* Software Interrupt 6 */
#define LBIREG_SW5  0x00002000		/* Software Interrupt 5 */
#define LBIREG_SW4  0x00001000		/* Software Interrupt 4 */
#define LBIREG_SW3  0x00000800		/* Software Interrupt 3 */
#define LBIREG_SW2  0x00000400		/* Software Interrupt 2 */
#define LBIREG_SW1  0x00000200		/* Software Interrupt 1 */
#define LBIREG_SW0  0x00000100		/* Software Interrupt 0 */
#define LBIREG_SPR  0x00000080		/* Spare Bit,  Not used,  Reserved */
#define LBIREG_VME7 0x00000040		/* VME Bus Interrupt Level 7 */
#define LBIREG_VME6 0x00000020		/* VME Bus Interrupt Level 6 */
#define LBIREG_VME5 0x00000010		/* VME Bus Interrupt Level 5 */
#define LBIREG_VME4 0x00000008		/* VME Bus Interrupt Level 4 */
#define LBIREG_VME3 0x00000004		/* VME Bus Interrupt Level 3 */
#define LBIREG_VME2 0x00000002		/* VME Bus Interrupt Level 2 */
#define LBIREG_VME1 0x00000001		/* VME Bus Interrupt Level 1 */

/*
 * Interrupt Level Registers
 *
 *  These registers allow the specification of interrupt level for a 
 *  particular device.
 */
typedef union irqlevreg1 {

	u_int32  irqlev1_lng;			/* as a longword */

	volatile struct {
		u_int8  irqlev1_acf,		/* AC Fail and Abort */
		        irqlev1_sysf,		/* SYSFAIL and Write post */
		        irqlev1_pe,			/* parity error and IRQ1E */
		        irqlev1_tk2;		/* tick timers 1 and 2  */
	} irqlev1_reg;

} ilev1_t;

/* reference: ilev1.irqlev1_reg.irqlev1_tk2 */
#define M_TICK2_LEV 0x70		/* Tick Timer 2 interrupt level */
#define M_TICK1_LEV 0x07		/* Tick Timer 1 interrupt level */
	         
#endif /*  _VMWCHIP2_H_ */
