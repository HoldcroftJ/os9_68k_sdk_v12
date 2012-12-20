 opt -l
 nam spis328.d
 ttl - MC68328 Serial Peripheral Interface Slave Module

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
* Definitions for the MC68328 SPIS Module
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
* The slave SPI is used to interface with master SPIs to bring
* data into the MC68328.
*


SPISBase equ $700            Offset of SPIS from system module map
SPISMap equ ModuleIO+SPISBase Default location of SPISR register

*
* All registers are 16 bits and are referenced from SPISMap.
*

SPISR equ $0 SPIS Register


*****  These are separate masks within the various registers ********

****************  SPISR (SPIS Register, 16 bits)  *******************
* Initialized to $0000 at reset. 
*
SPIS_IRQ     equ $8000 interrupt flag, 1=IRQ posted
SPIS_IRQEN   equ $4000 SPIS IRW Enable, 1=interrupt enabled
SPIS_ENPOL   equ $2000 SPSEN signal polarity control, 
*                    1=SPSEN active-high 0=SPSEN active-low
SPIS_DATARDY equ $1000 Data Ready flag, 1=buffer has data 0=not
SPIS_OVRWR   equ $0800 overwrite flag, 1=data buffer ovewrwritten (data
*                      stream corrupted) 0=data buffer inract
SPIS_PHA     equ $0400 SPSCLK and SPSRxD phase relationship control,
*                      1=data is captured on SPSCLK trailing edge
*                      0=data is captured on SPSCLK leading edge
SPIS_POL     equ $0200 SPSCLK polarity control, 
*                      1=inactive state of clock is high (idle=1)
*                      0=inactive state of clock is low (idle=0)
SPISEN       equ $0100 SPIS enable, 1=enabled 0=disabled
b_SPIS_DATA  equ 0 These 8 bits are the data shifted from the external device


 pag
 opt l
