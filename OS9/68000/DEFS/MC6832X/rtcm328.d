 opt -l
 nam rtcm328.d
 ttl Definitions for the MC68328 Real-Time Clock Module

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
* Definitions for the mc68328 MC68328 Real-Time Clock Module
*
* Edition History
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

RTCMBase equ $B00        Offset of RTCM with the System module map
MHSRMap equ ModuleIO+RTCMBase default location of the RTCM

********************************************************************
* These are the MC68328 RTCM registers.  
* These are used to set up the chip's RTC configuration.  
* They are defined here to be referenced from MHSRMap.

RTCHMSR   equ $000 RTC Hours Minutes Seconds Register (32 bits)
RTCALARM  equ $004 RTC Alarm Register (32 bits)
RTCCTL    equ $00D RTC Control Register (8 bits)
RTCISR    equ $00F RTC Interrupt Status Register (8 bits)
RTCIENR   equ $011 RTC Interrupt Enable Register (8 bits)
RTCSTPWCH equ $013 Stopwatch Minutes (8 bits)


*****  These are separate masks within the various registers ********

****  RTCHMSR (RTC Hours Minutes Seconds Register, 32 bits)  ********
* Initialized to $00000000 at reset
*
* bits 31:29 unused
b_HOURS   equ 24 Hours, 5 bits, 28:24)
* bits 23:22 unused
b_MINUTES equ 16 Minutes, 6 bits, 21:16)
* bits 15:6 unused
b_SECONDS equ 0 Seconds, 6 bits, 5:0


***********  RTCALARM (RTC Alarm Register, 32 bits)  ****************
* Initialized to $00000000 at reset.
*
* bits 31:29 unused
b_ALM_HOURS   equ 24 Alarm's Hours setting, 5 bits, 28:24)
* bits 23:22 unused
b_ALM_MINUTES equ 16 Alarm's Minutes, 6 bits, 21:16)
* bits 15:6 unused
b_ALM_SECONDS equ 0  Alarm'sSeconds, 6 bits, 5:0


***********  RTCCTL (RTC Control Register, 8 bits)  ******************
* Initialized to $00 at reset.
*
RTC_ENABLE equ $80 RTC Enable bit, 1=enable RTC 0=disable RTC
* bit 6 unused
REF_SEL    equ $20 38.4 KHz Reference Select, 1=38.4 KHz 0=32.768 KHz
* bits 4:0 reserved


***********  RTCISR (RTC Interrupt Status Register, 8 bits)  ************
* Initialized to $00 at reset.
* A 1 in a bit => interrupt for that source occurred.
*
SEC_FLAG equ $10 1 Hz flag, 1=1 Hz interrupt occurred
DAY_FLAG equ $08 24-hour rollover flag, 1=24 hour rollover IRQ occurred
MIN_FLAG equ $04 minute IRQ, 1=minute tick occurred
SW_FLAG  equ $01 Stopwatch flag, 1=stopwatch timed-out


***  RTCIENR (RTC Interrupt Enable Register, 8 bits)  ******
* Initialized to $00 at reset.
*
* bits 6:5 unused
SEC_ENBL equ $10 1 Hz interrupt enable, 1=enabled
DAY_ENBL equ $08 24-hour interrupt enable, 1=enable
ALM_ENBL equ $40 Alarm IRQ enable, 1=enable
MIN_ENBL equ $02 Minute-tick IRQ enable, 1=enable
SW_ENBL  equ $01 Stopwatch interrupt enable, 1=enable


******  RTCSTPWCH (Stopwatch Minutes Register, 8 bits)  **********
* Initialized to $00 at reset.
*
b_SW_COUNT equ 0 Stopwatch minutes count, 6 bits 5:0, 
*                highest allowable value is 62 minutes, counts down

 
 pag
 opt l
