 opt -l
 nam serm.d
 ttl - mc68340 Serial Communications Module

**********************************************************************
* Definitions for the mc68340 Serial Communications Module
*
* Edition History:
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  90/11/05  history starts.                                  Rwb
*               ---- OS-9 V2.4 Port Pak Release #2 ----
*               ---- OS-9/68k V3.0 Release ----

 ifndef ModuleIO
ModuleIO equ $FFFFF000  The base address of Short I/O space.
 endc

SERBase equ $700            Offset of SER from system MCR  (Range $700 - $73F)
SERMap equ ModuleIO+SERBase Default location of the SERIAL module

*****************************************************************
**  Programmers Note:  The serial interface is nearly identical
**    to the mc68681 DUART in form.  The only registers defined
**    here are those that are specific to the mc68340, (e.g. Module
**    Control Register).  The remaining registers are defined
**    in the driver files; io68681.a and sc68681.a.
*****************************************************************
 
SERMMCR  equ  $0   Module Control Register
SERTEST  equ  $2   Module Test Register
SERINTL  equ  $4   Interrupt Level Register
SERINTV  equ  $5   Interrupt Vector Register
SERMR1A  equ  SERMap+$10  Address of Duart within the module (port 0)

*************  SERMMCR  **********************************
*   High byte of MCR
b_SERMSTOP  equ  15  If asserted, will freeze all clocks with the module
b_SERMFREZ  equ  13  (2 bits) determines action on IFREEZEB
b_SERMICCS  equ  12  Selects clock used for input capture pins.

SERMFREZ_ignor equ (0<<b_SERMFREZ) Ignore FREEZE
SERMFREZ_CB    equ (2<<b_SERMFREZ) Freeze on character boundary.

*  Low byte of MCR
b_SERMSU    equ   7  Selects between super. only and user/super access.
b_SERMIAID  equ   0  Set to interrupt arbitration value

*************  SERMINTL  **********************************
* Interrupt level register.
*
*  Bits 0-3 select the interrupt level for the module.
***********************************************************

*************  SERMINTV  **********************************
* Interrupt vector register.
*
*  Bits 0-7 select the interrupt vector for the module.
***********************************************************
 pag
 opt l
