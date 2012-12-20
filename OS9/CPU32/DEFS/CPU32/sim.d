 opt -l
 nam sim.d
 ttl Definitions for the mc683xx SIM (Systems Integration Module)

**********************************************************************
* Definitions for the mc683xx SIM (Systems Integration Module)
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  89/10/10  history starts.                                  Rwb
*               ---- OS-9/68K V2.4 Release ----
*               ---- OS-9 V2.4 Port Pak Release #2 ----
*               ---- OS-9/68k V3.0 Release ----
*

ShortIO equ $FFF000  The base address of Short I/O space.
AlterIO equ $7FF000  Alternate location for Systems modules
SIMBase equ $A00     Offset of SIM with the System module map

 ifdef REMAPMCR

MCRMap equ AlterIO+SIMBase Places module controls here after sysinit MCR setup
 else
MCRMap equ ShortIO+SIMBase default location of the MCR
 endc

****  This is the powerup location of the MCR.  The mc683xx has the
*   MCR at this location after power up.  The location may be changed
*   by writting Bit 6 of the MCR to 0.
ABSMCR equ ShortIO+SIMBase

********************************************************************
*  These are the mc683xx common registers.  These are used to set up
*  the chip configuration.  Unless otherwise noted, these are 8 bit 
*  registers.  They are defined here to be used freferenced from
*  MCRMap.

MCR    equ $00 Module configuration register ( 16 bits )
SIMTR  equ $02 Module Test register ( 16 bits )
SYNCR  equ $04 Clock Synthesizer control ( 16 bits)

*********    External Bus Interface   ************
RSR    equ $07 Reset Status 
PORTE  equ $11 Port E data
PORTE1 equ $13 Port E data 1 
DDRE   equ $15 Port E data direction 
PEPAR  equ $17 Port E Pin assignment
PORTF  equ $19 Port F data
PORTF1 equ $1B Port F data 1 
DDRF   equ $1D Port F data direction 
PFPAR  equ $1F Port F Pin assignment

*********   System Protection         ************

SYPCR  equ $21 System Protection Control
PICR   equ $22 Periodic Interrupt Control  ---\
PIVR   equ $23 Periodic Interrupt Vector      |
PISR   equ $24 Periodic Interrupt Scale       |-- System Tick
PITR   equ $25 Periodic Interrupt Timing   ---/
SWSR   equ $27 Software Service 

*********        Test               ************
TSTMSRA equ $30 Test Module Master Shift A (16 bits)
TSTMSRB equ $32 Test Module Master Shift B (16 bits)
TSTSC   equ $34 Test Module Shift Count (16 bits)
TSTRC   equ $36 Test Module Repetition Counter (16 bits)
TSTCR1  equ $38 Test Module Control 1 (16 bits)
DREG    equ $3A Test Module Distributed Register

*********    Chip Select             ************
PORTC   equ $41 Port C data
CSPAR   equ $44 Chip Select Pin Assignment (32 Bits)
CSBRBT  equ $48 Chip Select Base Boot (16 bit)
CSORBT  equ $4A Chip Select Option Boot (16 bit)
CSBR0   equ $4C Chip Select Base 0      --\
CSOR0   equ $4E Chip Select Option 0      |
CSBR1   equ $50 Chip Select Base 1        |
CSOR1   equ $52 Chip Select Option 1      |
CSBR2   equ $54 Chip Select Base 2        |
CSOR2   equ $56 Chip Select Option 2      |
CSBR3   equ $58 Chip Select Base 3        |
CSOR3   equ $5A Chip Select Option 3      |
CSBR4   equ $5C Chip Select Base 4        |
CSOR4   equ $5E Chip Select Option 4      |----   ( 16 bits )
CSBR5   equ $60 Chip Select Base 5        |
CSOR5   equ $62 Chip Select Option 5      |
CSBR6   equ $64 Chip Select Base 6        |
CSOR6   equ $66 Chip Select Option 6      |
CSBR7   equ $68 Chip Select Base 7        |
CSOR7   equ $6A Chip Select Option 7      |
CSBR8   equ $6C Chip Select Base 8        |
CSOR8   equ $6E Chip Select Option 8      |
CSBR9   equ $70 Chip Select Base 9        |
CSOR9   equ $72 Chip Select Option 9      |
CSBR10  equ $74 Chip Select Base 10       |
CSOR10  equ $76 Chip Select Option 10  ___/

*****  These are separate masks within the various registers ********

********************  MCR  *************************
*  Bit positions:
FRZSW equ 14 Freeze software enable
FRZBM equ 13 Freeze bus monitor enable
SLVEN equ 11 Slave mode enabled 
SHEN  equ  8 Show cycle enable (2 bits)
SUPV  equ  7 Supervisor/unrestricted data space
MM    equ  6 Module mapping ( 1 = $FFF000,  0 = $7FF000 )
IARBX equ  0 Interrupt arbitration bits  ( 0 = mask internal modules )
*                    ( set up to allow arbitration [normal = $F] )

FS_enable equ (1<<FRZSW) freeze software enable
FB_enable equ (1<<FRZBM) freeze bus monitor
SL_enable equ (1<<SLVEN) slave mode enable
SV_only   equ (1<<SUPV) restricts register access to supervisor only (default)
MM_short  equ (1<<MM) sets internal registers to short IO access (REMAPMCR)

* IARBX are the interrupt arbitration bits.  This sets the interrupt priority
*   for this module.  $1 is lowest, $F is highest, $0 turns it off

* show cycle/external arbitration values
SHDEAE equ (0<<SHEN) show cycles disabled, external arbitration enabled
SHEEAD equ (1<<SHEN) show cycles enabled, external arbitration disabled
SHEEAE equ (2<<SHEN) show cycles enables, external arbitration enabled
SHIABG equ (3<<SHEN) SHEEAE, but BG freezes internal activity


********************  SIMTR  **********************
MSKNUM equ $FC00 Mask number for the part (read only)
SOSELR equ $00C0 Scan out select (2 bits)
SHIRQ  equ $0030 Show Interrupt Request (2 bits)
FBIT   equ $00C0 Force Bits (2 bits)
BWC    equ $0003 Bandwidth Control Bits (2 bits)

********************  RSR  *************************
EXT    equ $80  External Reset ( 1 = last reset caused by external signal )
POW    equ $40  Power up reset ( 1 = last reset caused by powerup circuit )
SWDOG  equ $20  Software Watchdog ( 1 = watchdog caused reset )
HLT    equ $10  Halt monitor ( 1 = reset caused by system protection monitor)
LOC    equ $04  Loss of clock ( 1 = loss of frequency reference occured )
SYS    equ $02  System Reset ( 1 = CPU executed reset instruction )
TST    equ $01  Test submodule caused reset

********************  SYPCR  ***********************
**  Bit positions:                                **
SWE    equ $7 Software watchdog enabe ( 1 = enable, 0 = disable )
SWP    equ $6 Software watchdog prescale
SWT    equ $4 Softwre watchdog timing   ( 2 bits )
HME    equ $3 Halt monitor enable ( 1 = enable, 0 = disable )
BME    equ $2 Bus monitor external enable
BMT    equ $0 Bus mononitor timout period select ( 2 bits )

SWEnable equ (1<<SWE) Software watchdog enable
SWPrescl equ (1<<SWP) Software watchdog prescale enable
BMEnable equ (1<<BME) Bus monitor enable

*   Bus monitor timeout periods
BMT64 equ 0  64 system clocks
BMT32 equ 1  32 system clocks
BMT16 equ 2  16 system clocks
BMT08 equ 3   8 system clocks

** SWT is 2 bits, dependent upon SWPrescl for value
**   range of times also dependent on internal or external clock

********************  SSR  **************************
*  Software Service Register - when watchdog is in
*     effect, must write the series  $55, $AA to this
*     register to prevent the watchdog timout.
*****************************************************

********************  PITR  ************************
PTP    equ $0100 Periodic timer prescaler control ( 1 = 512 prescale )
PITC   equ $00FF Periodic Interrupt Timing register  ( count value, 8 bits)

b_PITps equ 8 Bit position of the interrupt prescaler

********************  PICR  ************************
PIRQL  equ $0300 Periodic interrupt request level ( 3 bits )
PIV    equ $00FF Periodic interrupt Vector ( 8 bits )

b_PIRLvl equ 8 Bit position of the interrupt request level

********************  SYNCR  ***********************
SYNW   equ $8000 Frequency control bit [prescaler tap, VCO speed]
SYNX   equ $4000 Freqency control bit [prescaler, speed x2]
SYNY   equ $3F00 modulus down counter value
EDIV   equ $0080 E-clock divide rate 
SLIMP  equ $0010 Limp mode ( 1 = loss of crystal reference detected )
SCLOCK equ $0008 Synthesizer lock ( 1 = VCO locked, 0 = enabled, not locked)
RSTEN  equ $0004 Reset enable ( 1 = Loss of crystal caused reset )
STSIM  equ $0002 Stop mode system integration clock
STEXT  equ $0001 Stop mode external clock ( 1 = CLKOUT driven, 0 = held low)
 
********************  PEPAR  ***********************
***   Bit set to 1 indicates that pin is bus signal described
***   Bit set to 0 indicates lines as I/O pins defined by DDRE (below)
PEPA7  equ $80  SIZ1
PEPA6  equ $40  SIZ1
PEPA5  equ $20  DS
PEPA4  equ $10  AS
PEPA3  equ $08  RMC
PEPA2  equ $04  AVEC
PEPA1  equ $02  DSACK1
PEPA0  equ $01  DSACK0

********************  DDRE  ************************
***   Bit set to 1 indicates that pin is output
***   Bit set to 0 indicates that pin is input
****
**    bits 7 to 0 correspond to DDE7 through DDE0 respectively
****************************************************

**************** PORTE, PORTE1  ********************
**  Port E data registers.
**    A read of port e data register returns the value at the pin
**     only if the pin is configured as a discrete output. 
**     Otherwise, the value read is the value stored in the register.
***
**  bits 7 to 0 correspond to PE7 to PE0 respectively
****************************************************

********************  PFPAR  ***********************
***   Bit set to 1 indicates that pin is bus signal described
***   Bit set to 0 indicates lines as I/O pins defined by DDRE (below)
PFPA7  equ $80  IRQ7
PFPA6  equ $40  IRQ6
PFPA5  equ $20  IRQ5
PFPA4  equ $10  IRQ4
PFPA3  equ $08  IRQ3
PFPA2  equ $04  IRQ2
PFPA1  equ $02  IRQ1
PFPA0  equ $01  MODCK

********************  DDRF  ************************
***   Bit set to 1 indicates that pin is output
***   Bit set to 0 indicates that pin is input
****
**    bits 7 to 0 correspond to DDF7 through DDF0 respectively
****************************************************

**************** PORTF, PORTF1  ********************
**  Port F data registers.
**    A read of port F data register returns the value at the pin
**     only if the pin is configured as a discrete output. 
**     Otherwise, the value read is the value stored in the register.
***
**  bits 7 to 0 correspond to PF7 to PF0 respectively
****************************************************

*******************  CREG  *************************
BUSY   equ $8000  Test Submodule Busy Status Bit ( Test submodule is busy )
TMARM  equ $4000  Test Mode Aremed Status bit ( 1 = TSTME asserted )
COMP   equ $2000  Compare Status Bit
IMBTST equ $1000  Intermodule Bus Test
CPUTR  equ $0800  CPU Test Register
QBIT   equ $0400  Quotient Bit
MUXEL  equ $0200  Multiplexer Select Bit
ACUT   equ $0010  Activate Circuit Under Test
SCONT  equ $0008  Start Continuous Operation
SSHOP  equ $0004  Start Shifting Operation
SATO   equ $0002  Start Automatic Test Opewration
ETM    equ $0001  Enter Test mode

*******************  DREG  *************************
WAITX  equ $0700 Wait counter preset (3 bits)
MSRAX  equ $00E0 Master Shift register A bits 18-16
MSRAC  equ $0010 Master Shift register A configuration 
MSRBX  equ $000E Master Shift register B bits 18-16
MSRBC  equ $0001 Master Shift register B configuration

*************** TSTMSRA, TSTMSRB *******************
*  Both are 16 bit registers that can be programed to
*    be 19 bit serial signature or 16 bit shift registers
*    via DREG configuration bits.
****************************************************

***************** TSTSC, TSTRC *********************
***  Determines the number of bits responce bits
***    shifted from module under test to the shift
***    registers.
****************************************************

***************** TSTCR1 ***************************
**   Reps counter  (16 bits)  number of repetitions
**    in automatic mode.
****************************************************

*********** CSPAR0 and CSPAR1 **********************
**  These 2 registers allow function selection of
**    function codes for each of of the Chip Selects
**      There are 2 bits per CS in each of
**      the 2 registers as below.
**   These are defined as bit positions with the register and can
**    be used like this:   (CS_DefFun<<SIMCS04)|(CS_CSByte<<SIMCS03)
**         to set up values for CS04 and CS03.  Access as a long word.

SIMCS05  equ 28  CS5 ( FC2 )    --\
SIMCS04  equ 26  CS4 ( FC1 )      |
SIMCS03  equ 24  CS3 ( BGACK )    |-- CSPAR0
SIMCS02  equ 22  CS2 ( BGACK )    |
SIMCS01  equ 20  CS1 ( BG )       |
SIMCS00  equ 18  CS0 ( BR )       |
SIMCSBT  equ 16   CSBOOT         --/

SIMCS10  equ 8   CS10 (A23)    --\
SIMCS09  equ 6   CS09 (A22)      |
SIMCS08  equ 4   CS08 (A21)      |-- CSPAR1
SIMCS07  equ 2   CS07 (A20)      |
SIMCS06  equ 0   CS06 (A19)    --/

**  Bit patterns for programming CS register **
CS_DisOut equ %00  Discrete output ( E-clock on A23 )
CS_DefFun equ %01  Default Function, pins perform as in parenthesis.
CS_CSByte equ %10  Chip select ( 8 bit port )
CS_CSWord equ %11  Chip select ( 16 bit port )

*************** CSBARBT and CSBAR0-10 **********
*  These are the base address and block size registers.
*      bits 15->3 are the base address. This address must be a
*        multiple of the block size as only the most significant
*        bits are used to generate the address.
*      bits 2->0 are the block size.
*
*   NOTE:  If block size of 1M bytes is selected, and an external 
*    device requires an address A19->A0 , pin A19 must be selected as
*    an address line ( see above ) and cannot be used as a chip
*    select.

CSBASE equ $fff80 Base Address
CSBLKS equ $00007 Block Size

***  Block Size Bit patterns *****
*           Pattern   Size   Lines Compared
*           -------   -----  --------------
CSB_2K   equ %000      2K      a11-a23
CSB_8K   equ %001      8K      a13-a23
CSB_16K  equ %010      16K     a14-a23
CSB_64K  equ %011      64K     a15-a23
CSB_128K equ %100      128K    a16-a23
CSB_256K equ %101      256K    a17-a23
CSB_512K equ %110      512K    a18-a23
CSB_1M   equ %111      1M      a19-a23

*************** CSORBT and CSOR0-10 ***************
CSMODE   equ $8000  Mode
CSBYTE   equ $6000  Byte  
CSRW     equ $1800  R/W
CSSTRB   equ $0400  STRB  
CSDSACK  equ $03C0  DSACK ( see below )
CSSPACE  equ $0030  SPACE ( %00 = CPU, %01 = User, %10 = Supv, %11 = S/U )
CSIPL    equ $000E  IPL   ( see below )
CSAVEC   equ $0001  AVEC  ( 1 = off, 0 = on )

*  Mode selection values
CSM_async equ 0<<15 Asynchronous
CSM_sync  equ 1<<15 Synchronous

** Upper lower byte options values **
CSB_off    equ %00<<13  off
CSB_lower  equ %01<<13  Lower
CSB_upper  equ %10<<13  Upper
CSB_both   equ %11<<13  Both

** Read/Write selection **
CSRW_resvd  equ %00<<11  Rsvd
CSRW_read   equ %01<<11  Read
CSRW_write  equ %10<<11  Write
CSRW_both   equ %11<<11  Both 

** Address Strobe/Data Strobe
CSTRB_addr equ 0<<10 Address strobe
CSTRB_data equ 1<<10 Data strobe

** DSACK bit patterns **
DS_00Wait equ %0000<<6 Wait States
DS_01Wait equ %0001<<6   "    "
DS_02Wait equ %0010<<6   "    "
DS_03Wait equ %0011<<6   "    "
DS_04Wait equ %0100<<6   "    "
DS_05Wait equ %0101<<6   "    "
DS_06Wait equ %0110<<6   "    "
DS_07Wait equ %0111<<6   "    "
DS_08Wait equ %1000<<6   "    "
DS_09Wait equ %1001<<6   "    "
DS_10Wait equ %1010<<6   "    "
DS_11Wait equ %1011<<6   "    "
DS_12Wait equ %1100<<6   "    "
DS_13Wait equ %1101<<6 Wait States
DS_Fterm  equ %1110<<6 Fast Termination
DS_Extrn  equ %1111<<6 External DSACK

** Address space access modes  ( selects function code )
CSAD_cpu  equ %00<<4 CPU address space 
CSAD_usp  equ %01<<4 User space
CSAD_ssp  equ %10<<4 Supervisor space
CSAD_all  equ %11<<4 Supervisor/user space

** Interrupt Priority Level
CIPL_any   equ  %000<<1  any level
CIPL_il1   equ  %001<<1  interrupt level 1
CIPL_il2   equ  %010<<1  interrupt level 2
CIPL_il3   equ  %011<<1  interrupt level 3
CIPL_il4   equ  %100<<1  interrupt level 4
CIPL_il5   equ  %101<<1  interrupt level 5
CIPL_il6   equ  %110<<1  interrupt level 6
CIPL_il7   equ  %111<<1  interrupt level 7

 opt l
