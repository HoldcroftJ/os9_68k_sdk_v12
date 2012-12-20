 opt -l
 nam lcdcm328.d
 ttl Definitions for the MC68328 LCD Controller Module

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

LCDBase equ $A00        Offset of LCD Module with the System module map
LSSAMap equ ModuleIO+LCDBase default location of the LSSA register

********************************************************************
* These are the MC68328 LCD Controller Module registers.  
* These are used to set up the chip's LCD Controller configuration.  
* They are defined here to be referenced from LSSAMap.

LSSA   equ $000 Screen Starting Address Register (32 bits)
LVPW   equ $005 Virtual Page Width Register (8 bits)
LXMAX  equ $008 Screen Width Register (16 bits)
LYMAX  equ $00A Screen Height Register (16 bits)
LCXP   equ $018 Cursor X Position (16 bits)
LCYP   equ $01A Cursor Y Position (16 bits)
LCWCH  equ $01C Cursor Width and Height Register (16 bits)
LBLKC  equ $01F Blink Control Register (8 bits)
LPICF  equ $020 Panel Interface Configuration Register (8 bits)
LPOLCF equ $021 Polarity Configuration Register (8 bits)
LACDRC equ $023 Alternate Crystal Direction Rate Control Register (8 bits)
LPXCD  equ $025 Pixel Clock Divide Register (8 bits)
LCKCON equ $027 Clocking Control Register (8 bits)
LLBAR  equ $029 Last Buffer Address Register
LOTCR  equ $02B Octet Terminal Count Register (8 bits)
LPOSR  equ $02D Panning Offset Register (8 bits)
LFRCM  equ $031 Frame Rate Modulation Control Register (8 bits)
LGPMR  equ $032 Gray Palette Mapping Register (16 bits)

 
 pag
 opt l
