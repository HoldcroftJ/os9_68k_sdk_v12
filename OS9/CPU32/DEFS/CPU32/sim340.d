 opt -l
 nam sim.d
 ttl Definitions for the mc68340 SIM (Systems Integration Module)

**********************************************************************
* Definitions for the mc68340 SIM (Systems Integration Module)
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  90/11/05  history starts.                                  Rwb
*               ---- OS-9 V2.4 Port Pak Release #2 ----
*               ---- OS-9/68k V3.0 Release ----

***************************************************************************
*  MBAR  - module base address register.  (long word)
*     This register is accessable at  $03FF00 via the MOVES
*     instruction.  By writing Bits 31-12 the location of the various
*     modules can be moved to any 4K boundry within the memory map.
*       Internal module addressing is determined as below:
*           Bits 31-12 define the location the internal module base
*           Bits 11-7  decode the individual modules
*           Bits 6-0   decode the registers within the modules.
*
*   This Register is programmed by 'SysInit'(1) such that the CPU modules
*   appear at the address defined by "ModuleIO".  
*
MBAR    equ $3FF00      Alway appears here (use MOVES to access.)
b_MBARv equ 0           mapped address for modules not valid until TRUE (1)

 ifndef ModuleIO
ModuleIO equ $FFFFF000  The base address of Chip's Module I/O space.
 endc

SIMBase equ $000        Offset of SIM with the System module map
MCRMap equ ModuleIO+SIMBase default location of the MCR

********************************************************************
*  These are the mc68340 SIM common registers.  These are used to set up
*  the chip configuration.  Unless otherwise noted, these are 8 bit 
*  registers.  They are defined here to be used referenced from
*  MCRMap.
MCR    equ $00 Module configuration register ( 16 bits )
SYNCR  equ $04 Clock Synthesizer control ( 16 bits)

*********    External Bus Interface   ************
AVR    equ $06 Autovector Register
RSR    equ $07 Reset Status 
PORTA  equ $11 Port A data
DDRA   equ $13 Port A data direction
PPARA1 equ $15 Port A Pin Assignment Register 1
PPARA2 equ $17 Port A Pin Assignment Register 2
PORTB  equ $19 Port B data
PORTB1 equ $1B Port B data 1 
DDRB   equ $1D Port B data direction 
PPARB  equ $1F Port B Pin assignment

*********   System Protection         ************
SWIV   equ $20 Software Interrupt Vector
SYPCR  equ $21 System Protection Control
PICR   equ $22 Periodic Interrupt Control  ---\
PIVR   equ $23 Periodic Interrupt Vector      |
PISR   equ $24 Periodic Interrupt Scale       |-- System Tick
PITR   equ $25 Periodic Interrupt Timing   ---/
SWSR   equ $27 Software Service 

*********    Chip Select             ************
CS0AM   equ $40 Chip Select Address Mask  (CS0)
CS0BA   equ $44 Chip Select Base Address
CS1AM   equ $48 Chip Select Address Mask  (CS1)
CS1BA   equ $4C Chip Select Base Address
CS2AM   equ $50 Chip Select Address Mask  (CS2)
CS2BA   equ $54 Chip Select Base Address
CS3AM   equ $58 Chip Select Address Mask  (CS3)
CS3BA   equ $5C Chip Select Base Address


*****  These are separate masks within the various registers ********

********************  MCR  *************************
*  Bit positions:
b_FRZ1  equ 14 IFREEZEB asserted will freeze Software Watchdog counters
b_FRZ2  equ 13 IFREEZEB asserted will freeze Periodic Interrupt counter.
b_FIRQ  equ 12 Full Interrupt Request Mode
b_SHEN  equ  8 Show cycle enable (2 bits)
b_SUPV  equ  7 Supervisor/unrestricted data space
b_IARBX equ  0 Interrupt arbitration bits  ( 0 = mask internal modules )
*                    ( set up to allow arbitration [normal = $F] )

FS_enable equ (1<<b_FRZ1) freeze software watchdog counter enable
FB_enable equ (1<<b_FRZ2) freeze periodic interrupt counter enable
SV_only   equ (1<<b_SUPV) restricts register access to supervisor only (default)

* IARBX are the interrupt arbitration bits.  This sets the interrupt priority
*   for this module.  $1 is lowest, $F is highest, $0 turns it off

* show cycle/external arbitration values
SHDEAE equ (0<<b_SHEN) show cycles disabled, external arbitration enabled
SHEEAD equ (1<<b_SHEN) show cycles enabled, external arbitration disabled
SHEEAE equ (2<<b_SHEN) show cycles enables, external arbitration enabled
SHIABG equ (3<<b_SHEN) SHEEAE, but BG freezes internal activity

********************  AVR  *************************
*  If bit corresponding to the interrupt level is set, the SIM will
*  generate an auto-vector for the interrupt level, else the device
*  interrupting must provide the vector.
AV_enable equ 1 enable autovector
AV1_mask equ (AV_enable<<1)  mask for vector 1 
AV2_mask equ (AV_enable<<2)  mask for vector 2
AV3_mask equ (AV_enable<<3)  mask for vector 3 
AV4_mask equ (AV_enable<<4)  mask for vector 4
AV5_mask equ (AV_enable<<5)  mask for vector 5
AV6_mask equ (AV_enable<<6)  mask for vector 6 
AV7_mask equ (AV_enable<<7)  mask for vector 7 

********************  RSR  *************************
EXT    equ $80  External Reset ( 1 = last reset caused by external signal )
POW    equ $40  Power up reset ( 1 = last reset caused by powerup circuit )
SWDOG  equ $20  Software Watchdog ( 1 = watchdog caused reset )
HLT    equ $10  Halt monitor ( 1 = reset caused by system protection monitor)
LOC    equ $04  Loss of clock ( 1 = loss of frequency reference occured )
SYS    equ $02  System Reset ( 1 = CPU executed reset instruction )

********************  SWIV  ***********************
*   Vector for Watchdog timer timeout (if in  irq mode )
*
*   Initialized to the Uninitialized interrupt vector ($0F) at reset
****************************************************

********************  PIVR  ***********************
*   Vector for Periodic interrupt timer. (if in  irq mode )
*
*   Initialized to the Uninitialized interrupt vector ($0F) at reset
****************************************************

********************  SSR  ***********************
*   This register contains the pre-scaler enables for
*  both the periodic interrupt timer and the software
*  watchdog timer.
****************************************************
SWP   equ $1 Software watchdog select (1 = sys reset on timeout, 0 = irq 7)
SWPrescl  equ (1<<SWP) Software watchdog prescale enable

PTP   equ $0 Periodic timer prescaler control ( 1 = 512 prescale )
PITPrescl equ (1<<PTP) Periodic interrupt prescale enable

********************  PITR  ************************
PITC   equ $00FF Periodic Interrupt Timing register  ( count value, 8 bits)

********************  PICR  ************************
PIRQL  equ $0300 Periodic interrupt request level ( 3 bits )
PIV    equ $00FF Periodic interrupt Vector ( 8 bits )

b_PIRLvl equ 8 Bit position of the interrupt request level

********************  SYPCR  ***********************
**  Bit positions:                                **
b_SWE    equ $7 Software watchdog enable (1 = enable, 0 = disable)
b_SWRI   equ $6 Software watchdog Reset/interrupt select (0 = cause interrupt)
b_SWT    equ $4 Softwre watchdog timing ( 2 bits )  [See table in manual]
b_DBE    equ $3 Double Bus Fault Enable (1 = sys reset on timeout, 0 = irq 7)
b_BME    equ $2 Bus monitor external enable
b_BMT    equ $0 Bus mononitor timout period select ( 2 bits ) 

SWEnable  equ (1<<b_SWE) Software watchdog enable
BMEnable  equ (1<<b_BME) Bus monitor enable

*   Bus monitor timeout periods
BMT64 equ 0  64 system clocks
BMT32 equ 1  32 system clocks
BMT16 equ 2  16 system clocks
BMT08 equ 3   8 system clocks

** SWT is 2 bits, dependent upon SWPrescl for value
**   range of times also dependent on internal or external clock.

********************  SWSR  **************************
*  Software Service Register - when watchdog is in
*     effect, must write the series  $55, $AA to this
*     register to prevent the watchdog timout.
******************************************************

********************  SYNCR  ***********************
SYNW   equ $8000 Frequency control bit [prescaler tap, VCO speed]
SYNX   equ $4000 Freqency control bit [prescaler, speed x2]
SYNY   equ $3F00 modulus down counter value
SLIMP  equ $0010 Limp mode ( 1 = loss of crystal reference detected )
SCLOCK equ $0008 Synthesizer lock ( 1 = VCO locked, 0 = enabled, not locked)
RSTEN  equ $0004 Reset enable ( 1 = Loss of crystal caused reset )
STSIM  equ $0002 Stop mode system integration clock
STEXT  equ $0001 Stop mode external clock ( 1 = CLKOUT driven, 0 = held low)

*******************   PORTA ************************
*  PPARA1 and PPARA2 combine to select between addresses
*  and discreete I/O port.  If a bit is set, the pin will
*  act as discrete I/O.  Note: PPARA1 bits override bits in
*  PPARA2.
*     Power-Up default is to set them as I/O port. (PPARA1 all 1s )
*

*  These are masks to produce IACK output on port A pins.
IA7_PPARA2 equ (1<<7)  IACK for level 7
IA6_PPARA2 equ (1<<6)  IACK for level 6
IA5_PPARA2 equ (1<<5)  IACK for level 5
IA4_PPARA2 equ (1<<4)  IACK for level 4
IA3_PPARA2 equ (1<<3)  IACK for level 3
IA2_PPARA2 equ (1<<2)  IACK for level 2
IA1_PPARA2 equ (1<<1)  IACK for level 1

********************  DDRA  ************************
***   Bit set to 1 indicates that pin is output
***   Bit set to 0 indicates that pin is input
****
**    bits 7 to 0 correspond to DD7 through DD0 respectively
****************************************************

********************  PORTA  ************************
***  This register is only significant if configured to 
***  be discrete I/O
***      Direction can be set for In or Out as defined by
***   DDRA (above).
****************************************************

*******************  PPARB  ************************
**  This reigster selects between IRQ, MODCK, and
**  discrete I/O.  It's function is modified by the
**  FIRQ bit (see MCR Bit 12 above).
**
**  Power up default is IRQ.
****************************************************

***************** PORTB, PORTB1 ********************
**  Registers used only if Port B is defined as discrete
**  I/O.  These may be either in or out dependent on
**  direction register (DDRB)
****************************************************
*****************  CSxAM  **************************
**   Chip Select Address Mask Registers
*  Bits 31-8 provide the ability to individually mask any of the
*  upper 24 bits of address thus providing a range of access.
*
b_AMRAM  equ 8 Address mask
b_AMRFC  equ 4 Function code mask
b_AMRDD  equ 2 Bit position of DTACK delay bits
b_AMRPS  equ 0 Bit position of port size bits

*    Port size values:
AMR16BIT  equ 1  Port is 16 bits wide
AMR8BIT   equ 2  Port is 8 bits wide
AMRNODSK  equ 3  No DSACK response by CSSM

* Wait State values
AMRW0  equ  0  0 wait states
AMRW1  equ  1  1 wait state
AMRW2  equ  2  2 wait states
AMRW3  equ  3  3 wait states

*****************  CSxBA  **************************
**   Chip Select Base Address Registers
**
**  Bits 31-8 define the upper 24 bits of the address
**      decoded by the chip select. (modified by CSxAM above.)
**
b_BARBA   equ 8  Base address
b_BARFC   equ 4  Bit position for the function codes
b_BARWP   equ 3  Allows write protection for address range.
b_BARFT   equ 2  When set, allows internal DTACK generation.
b_BARVL   equ 0  Should be Set to 1 indicating valid entry has been made.

BARWP     equ  1 If set (b_BARWP) Then the address space is write protected.
BARFTE    equ  1 If set (b_BARFT) then the address uses Fast Termination.
BARVL     equ  1 Set to indicate chip select entry is valid.

*  This is a table of function codes
FC_RES1  equ  0  Reserved (Motorola)
FC_UDS   equ  1  User Data Space
FC_UPS   equ  2  User Program Space
FC_RESU  equ  3  Reserved (User)
FC_RES2  equ  4  Reserved (Motorola)
FC_SDS   equ  5  Supervisor Data Space
FC_SPS   equ  6  Supervisor Program Space
FC_SCPU  equ  7  Supervisor CPU-Space Operation (e.g. MOVES)
FC_DMA   equ  8  (resides $8 to $F)  DMA space.
 
 pag
 opt l
