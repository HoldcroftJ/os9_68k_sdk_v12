 opt -l
 nam pllm328.d
 ttl Definitions for the MC68328 Phase-Locked Loop and Power Control Module

**********************************************************************
* Copyright 1995, 1996 by Microware Systems Corporation              *
* Reproduced Under License                                           *
*                                                                    *
* This source code is the proprietary confidential property of       *
* Microware Systems Corporation, and is provided to licensee         *
* solely for documentation and educational purposes. Reproduction,   *
* publication, or distribution in any form to any party other than   *
* the licensee is strictly prohibited.                               *
**********************************************************************
* Definitions for the mc68328 PLL module
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  95/08/30  history starts.                                  rws
*               --- OS-9/68K MC68328 Support Beta2 Release ---
**********************************************************************

 ifndef ModuleIO
 ifndef StdAddr
ModuleIO equ $FFFFF000  Base address of chip's internal regs (32 bits)
 else
ModuleIO equ $FFF000    Base address of chip's internal regs (24 bits)
 endc
 endc

PLLBase equ $200        Offset of PLL with the System module map
PLLCRMap equ ModuleIO+PLLBase default location of the PLL Control Register

********************************************************************
*  These are the MC68328 PLL registers.  
*  These are used to set up the chip's internal clock source and to control
*  the chip's power.
*  They are defined here to be referenced from PLLCRMap.

PLLCR  equ $000 PLL Control Register (16 bits)
PLLFSR equ $002 PLL Frequency Select Register (16 bits)


*********  Power Control Module Registers  ************
* The Power Control Module is within the PLL Module.
* It improves power efficiency as it allocates power (clocks) to the
* CPU core and other modules.
*
PCTLR equ $007 Power Control Register (8 bits)


*****  These are separate masks within the various registers ********

**********  PLLCR (PLL Control Register, 16 bits)  ******************
* Initialized to $2400 at reset.
*
* bits 15:14 unused
b_PIXCLKSEL equ 11 LCD pixel clock select (3 bits):
b_SYSCLKSEL equ 8  processor system clock select (3 bits):
*                  000 = VCO/2, 001 = VCO/4, 010 = VCO/8
*                  011 = VCO/16, 1XX = VCO/1
CLKEN    equ $0010 CLK0 pin enable, 1=enabled 0=disabled
DISPLL   equ $0008 Disable PLL, 1=PLL disabled 0=PLL enabled
* bit 2:0 reserved


******  PLLFSR (PLL Frequency Select Register, 16 bits)  ********
* Initialized to $0123 at reset.
*
CLK32 equ $8000 status of CLK32 signal
PROT  equ $4000 Protect bit, 1=PLLFSR cannot be written to anymore
* bits 13:12 unused
b_QC  equ 8     Q counter control bits (4 bits)
b_PC  equ 0     P counter control bits (8 bits)


**********  PCTLR (PLL Control Register, 8 bits)  ******************
* Initialized to $1F at reset.
*
PCEM  equ $80 Power Control Enable, 1=enabled
STOP  equ $40 Stop CPU clock, 1=stop 0=normal CPU clock bursts
* bit 5 always 0
b_WIDTH equ 0 Width of CPU clock bursts (5 bits):
*             00000 = 0/31 duty cycle
*             00001 = 1/31 duty cycle
*             00010 = 2/31 duty cycle
*              ...
*             11111 = 31/31 duty cycle

 
 pag
 opt l
