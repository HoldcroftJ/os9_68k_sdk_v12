 opt -l
 nam spim328.d
 ttl - MC68328 Serial Peripheral Interface Master Module

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
* Definitions for the MC68328 SPIM Module
*
* Edition History:
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  95/08/31  history starts.                                  rws
*               --- OS-9/68K MC68328 Support Beta2 Release ---
**********************************************************************

 ifndef ModuleIO
 ifndef StdAddr
ModuleIO equ $FFFFF000  Base address of chip's internal regs (32 bits)
 else
ModuleIO equ $FFF000    Base address of chip's internal regs (24 bits)
 endc
 endc


******************
* The master SPI is used to to transmit or bring in data into the MC68328
* over an SPI bus.
*


SPIMBase equ $800            Offset of SPIM from system module map
SPIMMap equ ModuleIO+SPIMBase Default location of SPIMDATA register

*
* All registers are 16 bits and are referenced from SPIMMap.
*

SPIMDATA equ $0 SPIM Data Register
SPIMCONT equ $2 SPIM Control/Status Register


*****  These are separate masks within the various registers ********

***********  SPIMDATA (SPIM Data Register, 16 bits)  ****************
* Initialized to $0000 at reset. 
* This register exchanges data with external slave devices.
*


*******  SPIMCONT (SPIM Control/Status Register, 16 bits)  **********
* Initialized to $0000 at reset.
* This register controls the SPIM operation and reports its status.
*
b_DATA_RATE equ 13 Baud rate control, 3 bits, SYSCLK is divided by:
*                  000=divide by 4
*                  001=divide by 8
*                  010=divide by 16
*                  011=divide by 32
*                  100=divide by 64
*                  101=divide by 128
*                  110=divide by 256
*                  111=divide by 512
* bits 12:10 reserved
SPIMEN equ $0200 SPIM Enable, 1=enabled 0=disabled
SPIM_XCH equ $0100 SPIM Exchange control, 1=initiate exchange
*                  0=idle or exchange in progress
SPIMIRQ  equ $0080 Interrupt flag, 
*                  1=interrupt posted (clear by writing 1 to bit)
*                  0=no interrupt posted
SPIM_IRQEN equ $0040 SPIM IRQ enable, 1=enabled 0=disabled
SPIM_PHA   equ $0020 SPMCLK Phase shift control, 
*                    1=shift advance to opposite phase
*                    0=normal phase
SPIM_POL   equ $0010 SPMCLK polarity control,
*                    1=inverted polarity (idle=1)
*                    0=active-high polarity (idle=0)
b_SPIM_CLKCNT equ 0  transfer length indicator, 4 bits:
*                    0000=1 bit transfer
*                    1111=16 bit transfer


 pag
 opt l
