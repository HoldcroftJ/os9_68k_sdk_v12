 opt -l
 nam sim328.d
 ttl Definitions for the MC68328 SIM (Systems Integration Module)

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
* Definitions for the mc68328 SIM (Systems Integration Module)
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

SIMBase equ $000        Offset of SIM with the System module map
SCRMap equ ModuleIO+SIMBase default location of the SCR

********************************************************************
*  These are the MC68328 SIM registers.  These are used to set up
*  the chip's configuration.  They are defined here to be referenced 
*  from SCRMap.

SCR equ $000 System Configuration Register (8 bits)

*********    Interrupt Controller Block   ************
*
* 23 interrupts are supported:
* _Level_   _Source (n => asserted low signal)_
*    7      IRQ7n
*    6      SPI Slave
*    6      Timer 1
*    6      IRQ6n
*    5      PENIRQn
*    4      SPI Master
*    4      Timer 2
*    4      UART
*    4      Watchdog Timer
*    4      Real-Time Clock
*    4      Keyboard
*    4      PWM period rollover
*    4      8 General Purpose Interrupts (INT0n - INT7n)
*    3      IRQ3n
*    2      IRQ2n
*    1      IRQ1n
*
IVR    equ $300 Interrupt Vector Register (8 bits)
ICR    equ $302 Interrupt Control Register (16 bits)
IMR    equ $304 Interrupt Mask Register (32 bits)
IWR    equ $308 Interrupt Wake-up Enable Register (32 bits)
ISR    equ $30C Interrupt Status Register (32 bits)
IPR    equ $310 Interrupt-Pending Register (32 bits)


*********    Chip-Select Registers   ************
* 
* There are 16 chip select signals, grouped into 4 chip selects
* in 4 groups.  The group registers (group base address register and
* group mask register) decode the upper address bits and the chip
* select option registers decode the lower address bits.
*
GRPBASEA equ $100 Chip Select Group A Base Register (16 bits)
GRPBASEB equ $102 Chip Select Group B Base Register (16 bits)
GRPBASEC equ $104 Chip Select Group C Base Register (16 bits)
GRPBASED equ $106 Chip Select Group D Base Register (16 bits)
GRPMASKA equ $108 Chip Select Group A Mask Register (16 bits)
GRPMASKB equ $10A Chip Select Group B Mask Register (16 bits)
GRPMASKC equ $10C Chip Select Group C Mask Register (16 bits)
GRPMASKD equ $10E Chip Select Group D Mask Register (16 bits)

CSA0     equ $110 Group A Chip Select 0 Option Register (32 bits)
CSA1     equ $114 Group A Chip Select 1 Option Register (32 bits)
CSA2     equ $118 Group A Chip Select 2 Option Register (32 bits)
CSA3     equ $11C Group A Chip Select 3 Option Register (32 bits)

CSB0     equ $120 Group B Chip Select 0 Option Register (32 bits)
CSB1     equ $124 Group B Chip Select 1 Option Register (32 bits)
CSB2     equ $128 Group B Chip Select 2 Option Register (32 bits)
CSB3     equ $12C Group B Chip Select 3 Option Register (32 bits)

CSC0     equ $130 Group C Chip Select 0 Option Register (32 bits)
CSC1     equ $134 Group C Chip Select 1 Option Register (32 bits)
CSC2     equ $138 Group C Chip Select 2 Option Register (32 bits)
CSC3     equ $13C Group C Chip Select 3 Option Register (32 bits)

CSD0     equ $140 Group D Chip Select 0 Option Register (32 bits)
CSD1     equ $144 Group D Chip Select 1 Option Register (32 bits)
CSD2     equ $148 Group D Chip Select 2 Option Register (32 bits)
CSD3     equ $14C Group D Chip Select 3 Option Register (32 bits)


*****  These are separate masks within the various registers ********

**********  SCR (System Control Register, 8 bits)  ******************
* Initialized to $0C at reset
*
BETO  equ $80 Bus-Error Timeout status bit, cleared by writing a 1 to this bit
WPV   equ $40 Write-Protection Violation status bit, 
*             cleared by writing a 1 to this bit
PRV   equ $20 Priviledge Violation status bit,
*             cleared by writing a 1 to this bit
BETEN equ $10 Bus-Error Timeout Enable, 1=enabled, 0=disabled
SO    equ $08 Supervisor Only control of on-chip registers, 1=SO
DMAP  equ $04 Double Map bit, 1=>on-chip regs mapped at FFFFF000 and FFF000, 
*             0=>on-chip regs mapped at FFFFF000 only
* bit 1 reserved
WDTH8 equ $01 8-bit data-bus width select, 1=>D15:8 = data bus and
*             D7:0 = Port B I/O, 0=>D15:0 = data bus


********  IVR (Interrupt Vector Register, 8 bits)  ****************
* Initialized to $00 at reset.
* This register programs the upper 5 bits of the interrupt vector; during
* an interrupt-acknowledge cycle the lower 3 bits encode the interrupt
* level.  If the IVR is never programmed and an interrupt occurs, $0F is
* returned to indicate an uninitialized interrupt.
*
b_VECTOR  equ 3   vector bits are from 3 to 7
UNINITIRQ equ $0F uninitialized interrupt response code


*******  ICR (Interrupt Control Register, 16 bits)  ****************
* Initialized to $0000 at reset.
*
ET1     equ $8000 IRQ1 Edge-Trigger select, 1=edge-triggered, 0=level
ET2     equ $4000 IRQ2 Edge-Trigger select, 1=edge-triggered, 0=level
ET3     equ $2000 IRQ3 Edge-Trigger select, 1=edge-triggered, 0=level
ET6     equ $1000 IRQ6 Edge-Trigger select, 1=edge-triggered, 0=level
POLIRQ1 equ $0800 IRQ1 Polarity Control, 1=positive polarity, 0=negative
POLIRQ2 equ $0400 IRQ2 Polarity Control, 1=positive polarity, 0=negative
POLIRQ3 equ $0200 IRQ3 Polarity Control, 1=positive polarity, 0=negative
POLIRQ6 equ $0100 IRQ6 Polarity Control, 1=positive polarity, 0=negative
* bits 0 thru 7 unused

************  IMR (Interrupt Mask Register, 32 bits)  *************
* Initialized to $00FFFFFF at reset.
* A 1 in a bit => interrupt source is masked.
*
MASKSPIM equ $00000001 Mask Master SPI interrupt
MASKTMR2 equ $00000002 Mask Timer 2 interrupt
MASKUART equ $00000004 Mask UART interrupt
MASKWDT  equ $00000008 Mask Watchdog Timer interrupt
MASKRTC  equ $00000010 Mask RTC interrupt
* bit 5 reserved
MASKKB   equ $00000040 Mask Keyboad interrupt
MASKPWM  equ $00000080 Mask PWM interrupt
MASKINT0 equ $00000100 Mask External INT0
MASKINT1 equ $00000200 Mask External INT1
MASKINT2 equ $00000400 Mask External INT2
MASKINT3 equ $00000800 Mask External INT3
MASKINT4 equ $00001000 Mask External INT4
MASKINT5 equ $00002000 Mask External INT5
MASKINT6 equ $00004000 Mask External INT6
MASKINT7 equ $00008000 Mask External INT7
MASKIRQ1 equ $00010000 Mask IRQ1 interrupt
MASKIRQ2 equ $00020000 Mask IRQ2 interrupt
MASKIRQ3 equ $00040000 Mask IRQ3 interrupt
MASKIRQ6 equ $00080000 Mask IRQ6 interrupt
MASKPEN  equ $00100000 Mask Pen interrupt
MASKSPIS equ $00200000 Mask SPI Slave interrupt
MASKTMR1 equ $00400000 Mask Timer 1 interrupt
MASKIRQ7 equ $00800000 Mask IRQ7 interrupt
* bits 24 thru 31 not used


***  IWR (Interrupt Wake-up Enable Register, 32 bits)  ******
* Initialized to $00FFFFFF at reset.
* This control register enables the corresponding interrupt source to start
* the power-control-wakeup sequence.  While the bit is set it enables that
* interrupt to cause wakeup, while thebit cleared the funtion is disabled.
*
WUSPIM equ $00000001 Master SPI interrupt
WUTMR2 equ $00000002 Timer 2 interrupt
WUUART equ $00000004 UART interrupt
WUWDT  equ $00000008 Watchdog Timer interrupt
WURTC  equ $00000010 RTC interrupt
* bit 5 reserved
WUKB   equ $00000040 Keyboad interrupt
WUPWM  equ $00000080 PWM interrupt
WUINT0 equ $00000100 External INT0
WUINT1 equ $00000200 External INT1
WUINT2 equ $00000400 External INT2
WUINT3 equ $00000800 External INT3
WUINT4 equ $00001000 External INT4
WUINT5 equ $00002000 External INT5
WUINT6 equ $00004000 External INT6
WUINT7 equ $00008000 External INT7
WUIRQ1 equ $00010000 IRQ1 interrupt
WUIRQ2 equ $00020000 IRQ2 interrupt
WUIRQ3 equ $00040000 IRQ3 interrupt
WUIRQ6 equ $00080000 IRQ6 interrupt
WUPEN  equ $00100000 Pen interrupt
WUSPIS equ $00200000 SPI Slave interrupt
WUTMR1 equ $00400000 Timer 1 interrupt
WUIRQ7 equ $00800000 IRQ7 interrupt
* bits 24 thru 31 not used


****** ISR (Interrupt Status Register, 32 bits)  **********
* Initialized to $00000000 at reset.
* This register indicates which interrupts are asserting the processor.
* An interrupt handler can determine the source of an interrupt by examining
* this register.  A 1 in the corresponding bit => interrupt being asserted.
* For edge-triggered external interrupts, a 1 must be written to the 
* corresponding bit to clear the status bit, for all other interrupts the
* status is cleared by properly controlling the interrupting source.
*
SIRQ7 equ $00800000 IRQ7n source (edge-triggered external interrupt)
STMR1 equ $00400000 Timer 1 event source (level 6 interrupt)
SSPIS equ $00200000 Slave SPI source (level 6 interrupt)
SPEN  equ $00100000 Pen source (level 5 interrupt)
SIRQ6 equ $00080000 IRQ6n source (edge or level sensitive)
SIRQ3 equ $00040000 IRQ3n source (edge or level sensitive)
SIRQ2 equ $00020000 IRQ2n source (edge or level sensitive)
SIRQ1 equ $00010000 IRQ1n source (edge or level sensitive)
SINT7 equ $00008000 INT7n source (level 4 interrupt)
SINT6 equ $00004000 INT6n source (level 4 interrupt)
SINT5 equ $00002000 INT5n source (level 4 interrupt)
SINT4 equ $00001000 INT4n source (level 4 interrupt)
SINT3 equ $00000800 INT3n source (level 4 interrupt)
SINT2 equ $00000400 INT2n source (level 4 interrupt)
SINT1 equ $00000200 INT1n source (level 4 interrupt)
SINT0 equ $00000100 INT0n source (level 4 interrupt)
SPWM  equ $00000080 PWM period rollover module source (level 4 interrupt)
SKB   equ $00000040 Keyboard source (level 4 interrupt)
SLCDC equ $00000020 LCD Controller source
SRTC  equ $00000010 RTC source (level 4 interrupt)
SWDT  equ $00000008 Watchdog Timer source (level 4 interrupt)
SUART equ $00000004 UART module source (level 4 interrupt)
STMR2 equ $00000002 Timer 2 timed out source (level 4 interrupt)
SSPIM equ $00000001 Master SPI data transfer complete source (level 4 interrupt)


*******  IPR (Interrupt-Pending Register, 32 bits)  ****************
* Undefined at reset.
* Read-only register indicates which interrupts are pending.  If a masked
* interrupt occurs, the source will be shown here but not in the ISR;
* if a non-masked interrupt occurs, the ISR and IPR bit will be the same
* for that interrupt source.
*
PIRQ7 equ $00800000 IRQ7n source (edge-triggered external interrupt)
PTMR1 equ $00400000 Timer 1 event source (level 6 interrupt)
PSPIS equ $00200000 Slave SPI source (level 6 interrupt)
PPEN  equ $00100000 Pen source (level 5 interrupt)
PIRQ6 equ $00080000 IRQ6n source (edge or level sensitive)
PIRQ3 equ $00040000 IRQ3n source (edge or level sensitive)
PIRQ2 equ $00020000 IRQ2n source (edge or level sensitive)
PIRQ1 equ $00010000 IRQ1n source (edge or level sensitive)
PINT7 equ $00008000 INT7n source (level 4 interrupt)
PINT6 equ $00004000 INT6n source (level 4 interrupt)
PINT5 equ $00002000 INT5n source (level 4 interrupt)
PINT4 equ $00001000 INT4n source (level 4 interrupt)
PINT3 equ $00000800 INT3n source (level 4 interrupt)
PINT2 equ $00000400 INT2n source (level 4 interrupt)
PINT1 equ $00000200 INT1n source (level 4 interrupt)
PINT0 equ $00000100 INT0n source (level 4 interrupt)
PPWM  equ $00000080 PWM period rollover module source (level 4 interrupt)
PKB   equ $00000040 Keyboard source (level 4 interrupt)
PLCDC equ $00000020 LCD Controller source
PRTC  equ $00000010 RTC source (level 4 interrupt)
PWDT  equ $00000008 Watchdog Timer source (level 4 interrupt)
PUART equ $00000004 UART module source (level 4 interrupt)
PTMR2 equ $00000002 Timer 2 timed out source (level 4 interrupt)
PSPIM equ $00000001 Master SPI data transfer complete source (level 4 interrupt)


***  GRPBASEx (Group-Base Address Registers (16 bits)  ***
* Initialzed to $0000 at reset.
* Bits 15-4 define the upper 12 bits of the address (GBA31:20)
* decoded by the chip select (modified by GRPMASKx)
*
b_GBA   equ 4 Base address (GBA31:20 go into bits 15:4)
b_VALID equ 0 Validity bit, 1=>contents valid 0=>contents invalid


***  GRPMASKx (Group-Base Address Mask Registers, 16 bits)  ***
* Initialzed to $0000 at reset.
* This register adjusts the comparison range for the group.  Bits set
* to 1 compare the corresponding address line with its GRPBASE bit.
*
b_GBM equ 4 Group Base Mask (GMA31:20 go into bits 15:4)


*************  CSxx (Chip Select Registers (32 bits)  ************
* Initialzed to $00010006 at reset.
* These registers contain address compare and mask bits, as well as control
* the corresponding selectable device's bus width, write ability, and 
* wait states.  
* CSAx and CSBx registers contain A23:16 compare and mask bits, while
* CSCx and CSDx registers contain A23:12 compare and mask bits.
*
b_AC_AB equ 24 CSAx and CSBx A23:16 Address compare bits in 31:24
* CSAx and CSBx bits 23:17 unused
b_AC_CD equ 20 CSCx and CSDx A23:12 Address compare bits in 31:20
* CSCx and CSDx bits 19:17 unused

BSW equ $00010000 Data bus width of device, 0=8 bits 1=16 bits

b_AM_AB equ 8  CSAx and CSBx A23:16 address mask bits in 15:8
* CSAx and CSBx bits 7:4 unused
b_AM_CD equ 4  CSCx and CSDx A23:12 address mask bits in 15:4

RO equ $00000008 Read-only setting, 0=read/write device, 1=read only device
b_WAIT equ 0 This 3-bit field determines number of wait states before
*            internal DTACK is returned:
*            000 = zero wait states
*            001 = one wait states
*            010 = two wait states
*            011 = three wait states
*            100 = four wait states
*            101 = five wait states
*            110 = six wait states
*            111 = external DTACK

 
 pag
 opt l
