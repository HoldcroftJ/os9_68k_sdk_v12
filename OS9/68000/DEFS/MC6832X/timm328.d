 opt -l
 nam timm328.d
 ttl - MC68328 Timer Module

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
* Definitions for the MC68328 Timer Module
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
* The MC68328 contains two identical general-purpose 16-bit timers
* with a programmable prescaler and a software watchdog timer.
* At 16.67 MHZ resolution is 240 ns.
*


TIMBase equ $600            Offset of TIM from system module map
TCTLMap equ ModuleIO+TIMBase Default location of the TCTL1 register

TIMER1   equ  TCTLMap     Base address of timer 1
TIMER2   equ  TCTLMap+$0C Base address of timer 2

*
*  These register defintions are relative to the base address of the
*    timer to be used.
*

TCTL  equ $0 Timer Control Register (16 bits)
TPRER equ $2 Timer Prescaler Register (16 bits)
TCMP  equ $4 Timer Compare Register (16 bits)
TCR   equ $6 Timer Capture Register (16 bits)
TCN   equ $8 Timer Counter (16 bits)
TSTAT equ $A Timer Status Register (16 bits)

*
*  Watchdog Timer Registers relative to TCTLMap
*

WDOGCTR equ $18 Watchdog Control Register (16 bits)
WDOGCMR equ $1A Watchdog Compare Register (16 bits)
WDOGCNR equ $1C Watchdog Counter Register (16 bits)


*****  These are separate masks within the various registers ********

**********   TCTL (Timer Control Register, 16 bits)  *************
* Initialized to $0000 at reset. 
*
* bits 15:9 unused
TFRR equ $0100 Free Run/Restart bit, 1=Free-run 0=Restart,
*             timer operation after a reference event occurs
b_CAPTURE_EDGE equ 6 Capture function control, 2 bits:
*                    00=disable IRQ on capture event,
*                    01=capture on rising edge and generate IRQ on capture
*                    10=capture on falling edge and generate IRQ on capture
*                    11=capture on rising or falling edge and gen. IRQ
TOM equ $0020 Output Mode control, 1=toggle output 
*             0=active low pulse for one SYSCLK period
TIRQEN equ $0010 Reference Event IRQ Enable, 
*                1=enable IRQ on reference event, 0=disable IRQ
b_CLKSOURCE equ 1 Clock Source control, 3 bits:
*                 000=Stop count (clock disabled)
*                 001=System clock to timer
*                 010=system clock divided by 16
*                 011=TIN pin is the clock source
*                 1xx=32 KHz clock
TEN equ $0001 Timer Enable bit, 1=enabled 0=disabled


********  TPRER (Timer Prescaler Register, 16 bits)  *************
* Initialized to $0000 at reset
*
b_TPRESCALER equ 0 Divide value of prescaler, 8 bits,
*                  $00 - $FF => Divide by 1 - 256


**********  TCMP (Timer Compare Register, 16 bits)  **************
* Initialized to $FFFF at reset.
* These registers contain the value that is compared with the free-running
* counter as part of the output-compare function.
*


**********  TCR (Timer Capture Register, 16 bits)  **************              
* Initialized to $0000 at reset.
* These registers contain the counter value during a capture operation
* when an edge occures on the TIN pin.
*


**********  TCN (Timer Counter Register, 16 bits)  **************
* Initialized to $0000 at reset.
* These registers contain the value of the count of the clock source.
*


********  TSTAT (Timer Status Register, 16 bits)  ***************
* Initialized to $0000 at reset.
* Users mst clear the status bits by writing a 1 to the bit
* indicating which type of event has occurred.
*
TCAPT equ $0002 Capture event status, 1=capture event occurred
TCOMP equ $0001 Compare event status, 1=compare event occurred
b_TCOMP equ 0   Compare event status bit position


*******************  Watchdog Timer Registers  ***********************

********  WDOGCTR (Watchdog Control Register, 16 bits)  ***************
* Initialized to $0000 at reset.
*
* bits 15:4 unused
WDRST  equ $0008 Software reset control, 1=software reset is activated
WDLOCK equ $0004 read-only bit indicating WDOG lock status,
*                1=WDOG is locked, can no longer write to WDEN bit
WDFI   equ $0002 Force interrupt control bit, 1=force IRQ instead of reset 
WDEN   equ $0001 WDOG enable bit, 1=WDOG enabled


********  WDOGCMR (Watchdog Compare Register, 16 bits)  ***************
* Initialized to $FFFF at reset.
* This register contains the compare value for the watchdog time-out.
* When the counter counts up to this value it generates a system reset.
*


********  WDOGCNR (Watchdog Counter Register, 16 bits)  ***************
* Initialized to $0000 at reset.
* This register contains the current count value of the counter.
*



 pag
 opt l
