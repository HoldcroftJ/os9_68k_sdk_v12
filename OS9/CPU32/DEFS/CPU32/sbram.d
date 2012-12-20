 opt -l
 nam sbram.d
 ttl Definitions for the mc683xx Standby Ram 

**********************************************************************
* Definitions for the mc683xx Standby Ram module
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  89/10/11  history starts.                                  Rwb
*               ---- OS-9/68K V2.4 Release ----
*               ---- OS-9 V2.4 Port Pak Release #2 ----
*               ---- OS-9/68k V3.0 Release ----
*

****  Note that these defs are set up so that the they can be used
****   with the file mc683xx.d. 

 ifeq (CPUType-68332)
RAMBLOCK equ $7FFB00   64 byte block boundry for mc68332.
 else
RAMBLOCK equ $7FFB00   64 byte block boundry for mc68332.
 endc

 ifndef ShortIO
ShortIO equ $FFF000  The base address of Short I/O space.
 endc

 ifdef REMAPMCR
RAMMap equ RAMBLOCK Places module controls here after sysinit MCR setup
 else
RAMMap equ ShortIO+$B00 default location of the QSM
 endc

****  Power-up reset location of RAMMCR ************
RAMABS equ $FFFB00

*******************************************************************
***  These are the 3 Control Registers for RAM control and mapping.

RAMMCR  equ $00 Module configuration register
RAMTST0 equ $02 Test Registter
RAMBAR  equ $04 Base Address/Status Register

********************************************************************
**** These are masks and bit patterns for the various registers ****

********** RAMMCR **********
STOP equ 15 Stop Control ( 0 = Ram normal, 1 = Ram locked out at lpstop )
RASP equ 8  Ram Array Space ( 0 = unrestricted, 1 = Supervisor only )

********** RAMTST **********
SDTEST  equ 9  soft defect test function ( allows test selection )
RTBA    equ 8  Base address register RW enable ( 0 = RAMMBAR locked )

********** RAMBAR **********
RBAR  equ 3  RAM array base address ( bits 15->3 control A23->A11 )
RAMDS equ 0  RAM Array Disabled Flag  ( 0 = RAM enable, 1 = disabled )

 opt l
