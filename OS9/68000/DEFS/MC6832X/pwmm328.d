 opt -l
 nam pwmm328.d
 ttl - MC68328 Pulse Width Modulator Module

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
* Definitions for the MC68328 PWM Module
*
* Edition History:
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  95/08/31  history starts.                                  rws
*               --- OS-9/68K MC68328 Support Beta2 Release ---
* 02  96/10/08  corrected PWMMap to use PWMBase instead of       rws
*               SPIMBase
**********************************************************************

 ifndef ModuleIO
 ifndef StdAddr
ModuleIO equ $FFFFF000  Base address of chip's internal regs (32 bits)
 else
ModuleIO equ $FFF000    Base address of chip's internal regs (24 bits)
 endc
 endc


******************
* The PWM provides sound generation and motor control
*


PWMBase equ $500         Offset of PWM from system module map
PWMMap equ ModuleIO+PWMBase Default location of PWMC register

*
* All registers are 16 bits and are referenced from PWMMap.
*

PWMC   equ $0 PWM Control Register
PWMP   equ $2 PWM Period Register
PWMW   equ $4 PWM Width Register
PWMCNT equ $6 PWM Counter


*****  These are separate masks within the various registers ********

*************  PWMC (PWM Control Register, 16 bits)  ****************
* Initialized to $0000 at reset. 
* This register controls the overall PWM operation plus gives
* output status.
*
PWMIRQ   equ $8000 Period-compare interrupt indicator,
*                  1=PWM period rolled over, 0=no rollover
*                  bit clears itself upon being read
PWMIRQEN equ $4000 PWM interrupt control, 1=IRQ enabled 0=disabled
* bits 13:9 unused
PWM_LOAD equ $0100 Forces a new period=>loads period and width regs and
*                  automatically clears itself after the load is completed
PWM_PIN  equ $0080 PWM output pin status, 1=PWM output is high 0=low
* bit 6 is 0
PWM_POL  equ $0020 PWM output polarity control, 1=inverted polarity
PWMEN    equ $0010 PWM enable, 1=PWM enabled 0=disbaled
* bit 3 is 0
b_PWMCLKSEL equ 0  output divider control, 3 bits:
*                  000=divide by 2
*                  001=divide by 4
*                  010=divide by 8
*                  011=divide by 16
*                  100=divide by 32
*                  101=divide by 64
*                  110=divide by 128
*                  111=divide by 256


*******  PWMP (PWM Period Register, 16 bits)  **********
* Initialized to $0000 at reset.
* This register controls the PWM period.  When the counter value matches
* the value in this register an interrupt is posted and the counter reset
* to start another period.
*


*******  PWMW (PWM Width Register, 16 bits)  **********
* Initialized to $0000 at reset.
* This register controls the pulse width.  When the counter matches
* the value in this register the output is reset for the duration of the
* period.  If the value in this register is higher than the value in the
* period register the output will never be reset.
*


*******  PWMCNT (PWM Counter, 16 bits)  **********
* Initialized to $0000 at reset.
* This read-only register contains the current count value.
*



 pag
 opt l
