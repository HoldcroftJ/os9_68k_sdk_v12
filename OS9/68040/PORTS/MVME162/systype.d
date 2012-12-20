 opt -l
*
* System Definitions for MVME162 System
*
********************
* Edition History
*   date    comments                                            by
* -------- ---------------------------------------------------- ---
* 93/02/12 adapted from mvme167 version.                        ats
* 93/04/07 Removed old NVRAM structure definitions.				ats
* 93/04/15 Add HD/ED floppy definitions.						ats
* 93/07/21 Started rework for MWOS structure.					ats
* 93/08/16 Extend non-cacheable region down to C-page.          ats
* 93/08/17 Fixed special memory list definitions with BPLOAD.   ats
* 93/11/11 Adopt RAMLOAD flag instead of BPLOAD.				ats
*		   Delete ROMDISK variations.
*		   Cleaned up residual 167 definitions.
*		   Folded in Warren's new tape descriptors.
* 93/11/12 Added GCSR_brd_default.								ats
* 93/11/15 Reorganized special search list.						ats
*		   Increased local bus timeout.							ats
* 93/12/08 Changed blocks/buffer in tape descriptors to normal. ats
* 94/01/04 Add ROM utilities to colored memory list for all     ats
*          but hard disk boots.
* 94/02/10 Bumped up MDirSz for all but hard disk boots.        ats
* 94/03/08 Added definitions for extra serial ports on 'LX.     ats
* 94/03/09 Added definitions for ECC DRAM on 'LX.               ats
*          Added definitions for large SRAM.
*          Relocated vector table to beginning of system memory.
*          Changed CPUSize to minimum system memory size (1MB).
*          Reworked memory list for static system memory and
*            "Buddy" memory allocator.
* 94/04/08 Changed/extended SRAM definitions and memory lists   ats
*            for NVRAM.
*          Defined NVRAM memory color (0x10) for memory lists.
*          Reworked SRAM ramdisk descriptor.
*          Documented 2nd 16MB ECC into memory lists.
* 94/04/27 Added new ROM space defs and memory list entries.    ats
*          Fixed init module compat flag.
*          Added MEMC040 controller defs that MCECC defs require.
* 94/08/18 Added FPU to init module extensions list.            ats
* 94/08/30 Memory lists for both standard and buddy allocator.  ats
* 95/02/27 Second serial chip on 'LX needs unique vector#.      ats
*          Priorities should be unique for all serial ports.  
* 95/09/30 Added vector base for IP2 on 'FX.                    ats
* 95/11/17 Added port-specific NVRam version limits.            ats
*          ---- V3.0.2 MVME162 BSP released ----
* 96/02/22 Corrected port address of rnv descriptor.            ats
* 96/03/12 Conditionalized fpu/fpsp in extensions list.         ats
* 97/05/21 ******** BSP V3.0.3 released **********************  gdw
* 99/05/13 changed to 1meg spl search area to accomdate larger  mgh
*            bootfiles now shipping in rom.  Reordered search list
*            so FF800000 area is searched first.  When OS-9 rom is
*            in at FF800000, rb and lr boots are very fast.  Since
*            the bootup is slower when motbug is present, the
*            search of FF8 area before FFA is less noticeable.
*          Add sysmbuf and OS9P3 to Extens list
*          Add DDDISK to possible initial disk device list (/dd)
* 99/07/18 Add Config for setting system state cacheing mode    mgh
* 01/02/23 Update Flash Definition Description to make it       mdu
*            clearer. Fixed its macros to make it work better
* 01/05/04 Removed conditional on fpsp so bootlist determines   mgh
*            which module will be used
*

********************
* Conditional Assembly Variables
*
VME162 equ 162 default value for VME162

********************
* System Memory Definitions
*
* These are used by the MemDefs (for rom) and MemList (for init module)
* macros to describe the system ram structure.
*

*	The following definitions are applicable when coldstarting from ROM.
*	If coldstarting 162Bug from FLASH, the address spaces are swapped.

FlashBase	equ $FFA00000	Base address of FLASH when coldstarting from ROM
FlashSize	equ $100000		size of FLASH memory

RomBase		equ	$FF800000	Base address or ROM when coldstarting from ROM
RomSize		equ $100000		size of ROM

Rom2Base	equ $FF900000
Rom4Base	equ $FFB00000

*	On-board RAM definitions follow

SRAMBugSize equ $10000 Saving 64K for the 162Bug space.
SRAMBase equ $FFE00000 Reserve it at beginning of SRAM.
SRAM2MBase equ $FFC00000 Large SRAM is elsewhere

SRAMSize equ $80000 full size of SRAM
SRAMSmall equ $20000 default SRAM on `LX
SRAM2MSize equ $200000 size of large SRAM option

 ifdef BIGSRAM

SRAMBegin equ SRAM2MBase
SRAMEnd equ SRAM2MBase+SRAM2MSize

 else  BIGSRAM

SRAMBegin equ SRAMBase
 ifdef LXSRAM
SRAMEnd equ SRAMBase+SRAMSmall
 else  LXSRAM
SRAMEnd equ SRAMBase+SRAMSize
 endc  LXSRAM

 endc  BIGSRAM

 ifdef NOMBUG
SRAMAvail equ SRAMBegin
 else  NOMBUG
SRAMAvail equ SRAMBegin+SRAMBugSize
 endc  NOMBUG

 ifdef HISRAM
 ifdef LOSRAM
 error Check SRAM definition flags in makefile
 endc
 endc

CPUSize equ $100000

ProbeSize equ $10000 block search size (64k)

*  Note on Local RAM addressing and translation:
*      The 4MB version may only map on 4MB boundries and
*      the 16MB on 16MB boundries.  SysInit will use Mem.Beg
*      and TRANSLATE values in creating the values to load
*      into the LRC and LCSR.
*
TRANSLATE equ 0 address translation factor

 ifdef INHOUSE
*CPUBeg equ 0x80000000 start of cpu memory
CPUBeg equ 0 start of cpu memory
 else
CPUBeg equ 0 physical start of cpu memory
 endc INHOUSE

VBRBase equ CPUBeg base address of vectors
VTblSize equ 256*4 size of vector table

* ifeq CPUBeg
* if DRAM mapped at zero, then don't use 0 - 0x10
*MapOut equ $10 map out first 16 bytes
*MapOut equ 0
* else
*MapOut equ 0
* endc MapOut
MapOut equ VTblSize

Mem.Beg equ CPUBeg+MapOut start of memory search
Mem.End equ CPUBeg+CPUSize end of memory search
Mem.E2MB equ Mem.End+$100000 end of 2MB memory block
Mem.E4MB equ Mem.E2MB+$200000 end of 4MB memory block
Mem.E8MB equ Mem.E4MB+$400000 end of 8MB memory block
Mem.E16MB equ Mem.E8MB+$800000 end of 16MB memory block
Mem.E32MB equ Mem.E16MB+$1000000 end of 32MB memory block

*
* Spc area search is for use by ROM MemDefs searches for booters
*
 ifdef	ROMSTB
*
* Search entire 1meg space
*
Rom.SpcSrch		equ	$0
Rom.SSSize		equ	$100000
 else	ROMSTB
*
* Skip search of CBOOT ROM area
*
Rom.SpcSrch		equ	$20000
Rom.SSSize		equ	$8000
 endc	ROMSTB

*
* Utils area search used by init module MemList seaches
*
Rom.Utils		equ	$0
Rom.UtlSize		equ	$100000

Rom1.Beg		equ	RomBase
 ifdef	RAMLOAD
Rom2.Beg		equ	Mem.End
 else	RAMLOAD
Rom2.Beg		equ	Rom2Base
 endc	RAMLOAD
Rom3.Beg		equ FlashBase
Rom4.Beg		equ Rom4Base

Rom1.Spc		equ	Rom1.Beg+Rom.SpcSrch
Rom1.SpcEnd		equ	Rom1.Spc+Rom.SSSize
Rom1.Utils		equ	Rom1.Beg+Rom.Utils
Rom1.UtlEnd		equ	Rom1.Utils+Rom.UtlSize

Rom2.Spc		equ	Rom2.Beg+Rom.SpcSrch
Rom2.SpcEnd		equ	Rom2.Spc+Rom.SSSize
Rom2.Utils		equ	Rom2.Beg+Rom.Utils
Rom2.UtlEnd		equ	Rom2.Utils+Rom.UtlSize

Rom3.Spc		equ	Rom3.Beg+Rom.SpcSrch
Rom3.SpcEnd		equ	Rom3.Spc+Rom.SSSize
Rom3.Utils		equ	Rom3.Beg+Rom.Utils
Rom3.UtlEnd		equ	Rom3.Utils+Rom.UtlSize

Rom4.Spc		equ	Rom4.Beg+Rom.SpcSrch
Rom4.SpcEnd		equ	Rom4.Spc+Rom.SSSize
Rom4.Utils		equ	Rom4.Beg+Rom.Utils
Rom4.UtlEnd		equ	Rom4.Utils+Rom.UtlSize

*  These definitions are for the NVRAM and how it is used by the
*  reconfig program.  (Must agree with systype.h and SCSI booter)
*
NVRAMBase      equ $FFFC0000   Base address of 48t08 NVRAM.
NVRAM_SWITCHES equ NVR_SWITCHES

NVR_VER_MIN		equ	1
NVR_VER_MAX		equ 2

* Motorola configuration memory definitions
* (as defined/used by Motorola 167/162 debugger)
*
MotConfig equ $16F8 offset of Motorola debugger configuration area in NVRAM
MotConfSize equ 2048 size of the area



********************
* Hardware type definitions
*
CPUTyp   set 68040  (68040 !!!! )define the processor in use
MPUChip  set CPUTyp define the processor in use
CPUType  set VME162 define the cpu module in use
MPUClock set 33000000 Default value of 33 MHz chosen,  used by rtc48t08.


********************
* Device Addresses
*  The VME and  PCC chips have been designed specifically for use with
*  the MC68040.  This board makes use of a new memory controller, the
*  MEMC040.  There may be up to 2 of these on the board.  The board also
*  contains an NCR 53c710 SCSI controller chip, Cirrus Logic CD2401
*  4 port serial chip, an Intel 82596 LAN controller, a MK48t08 RCT, and
*  128K bytes of SRAM.
*
ShortIOBase  equ $FFFF0000     VME short I/O space base address
IOBase       equ $FFF00000     On-board I/O devices start here
VMEchBase    equ IOBase+$40000 VMEchip base address (LCSR)
GCSRBase     equ VMEchBase+$100 VMEchip base address (GCSR)
MCchBase     equ IOBase+$42000 MCchip base address (Memory controller)
MEMC40_1Base equ IOBase+$43000 MEMC040 #1
MEMC40_2Base equ IOBase+$43100 MEMC040 #2
TERMBase     equ IOBase+$45000 Zilog 85230 SCC
TRM2Base     equ IOBase+$45800 Zilog 85230 SCC (2nd on 'LX)
LANBase      equ IOBase+$46000 Intel 82596 LANC
SCSIBase     equ IOBase+$47000 NCR 53C710 SCSI controller chip
RTCBase      equ IOBase+$C1FF8 MK48t08 RTC chip clock register base address
 pag
 use <vmechip2.d>		Definitions for the VMEChip2
 use <mcchip.d>			Definitions for the MCchip
 use <memc040.d)		Definisions for the MEMC040 memory controller
 use <mcecc.d>			Definitions for the MCECC memory conatroller


********************
* System State Cacheing Mode
*  Set system state cacheing mode to copyback (no Page tables)
*
 ifdef Config
Config set SSM_CMode
 endc

 ifdef BOOT320
VME320Vect equ 240 VME320 disk
VME320Base equ ShortIOBase+$B000 VME320 disk controller (NOTE: odd-byte addressable only)
 endc BOOT320

***
*  Default modes for VMEBus access
VMEReqLVL  equ 3                          Request bus on level 3
VMEReqMode equ DMACgc_RWD                 Default to Release When Done mode
VMEbto     equ (VATO_3200<<b_VMEARB_vato) VME Bus Access timeout
VMEGblto   equ (LBTO_256<<b_VMEARB_gto)   VME Grant to cycle completion time

DMACgc_default equ (VMEReqLVL<<b_DMACgc_RQL)+(VMEReqMode)

***
*  VMEBus to local RAM access attribute defaults
*
SAWin1A_a equ ATTR_sup+ATTR_a32+ATTR_a24+ATTR_dat 
SAWin1A_b equ ATTR_d64+ATTR_blk
SAWin1A equ SAWin1A_a+SAWin1A_b Slave Attributes 1

* Local Bus Time Out default
*
LCLbto equ (LBTO_256<<b_VMEARB_lbto)        Local Bus timeout at 256 usec

* VMEChip arbiter/timeout/prescaler register default
*
VMEARB_default equ VMEbto+VMEGblto+LCLbto

* default GCSR definitions
* (map group address to Motorola default group, decoder disabled)
*
GCSR_grp_default	equ	$D2
GCSR_brd_default	equ $F

*

***
*  Default definitions for the attributes of the F page and VME Short IO
*  
*  short IO defined as Supervisor access, D16, no write-post
*
SIO_default equ LBCTL_lbvmio_i1en+LBCTL_lbvmio_i1su+LBCTL_lbvmio_i1si

*  F page is defined as Supervisor Data, no write-post
*
FPG_default equ LBCTL_lbvmio_i2en+LBCTL_lbvmio_i2su

***
*  Default IRQ levels for the VMEChip
*
ILVLR1_default equ 0  None are Set or enabled here (ABORT is in SysInit)

ILVLR2_default equ 0  None are Set or enabled here.
ILVLR3_default equ 0  None are Set or enabled here.

*  map the VME IRQ's to the corresponding Local IRQ level
*
ILVLR4a equ (VMELevel7<<b_ILVLR4_vme7)
ILVLR4b equ (VMELevel6<<b_ILVLR4_vme6)+(VMELevel5<<b_ILVLR4_vme5)
ILVLR4c equ (VMELevel4<<b_ILVLR4_vme4)+(VMELevel3<<b_ILVLR4_vme3)
ILVLR4d equ (VMELevel2<<b_ILVLR4_vme2)+(VMELevel1<<b_ILVLR4_vme1)
ILVLR4_default equ ILVLR4a+ILVLR4b+ILVLR4c+ILVLR4d

*
*  default IRQ enables set by SysInit2
*
IBIERa equ 0 (ABORT set in SysInit)
IBIERb equ m_IRQ_vme7+m_IRQ_vme6+m_IRQ_vme5+m_IRQ_vme4
IBIERc equ m_IRQ_vme3+m_IRQ_vme2+m_IRQ_vme1

LBIER_default equ IBIERa+IBIERb+IBIERc

 pag
*
TermBase equ TERMBase+4 SCC port A (Bootstrap terminal)
*TermBase	equ	TERMBase	alternate for testing
T1Base equ TermBase-4   SCC port B (Boot Download port)
*T1Base		equ	TermBase+4	alternate for testing
T2Base equ TRM2Base+4   SCC2 port A ('LX only)
T3Base equ T2Base-4     SCC2 port B ('LX only)

P1Base equ T1Base     replaces T1 when in use
P2Base equ T2Base     replaces T2 when in use
P3Base equ T3Base     replaces T3 when in use

*********************************************************************
*              VME167 On-Board Interrupt Structure                  *
*********************************************************************

*
*  This list is in the VMEChip2 internal priority order
*      NOTE: Interrupt levels that are not used or enabled by this port
*            are somewhat arbitrary and can be set as needed.
*
ACFAILLevel equ 7 ACFAIL*
ABORTLevel  equ 7 ABORT pushbutton
SYSFLLevel  equ 7 VMEBus SYSFAIL
VMEWPLevel  equ 7 VMEBus Master write post error
PELevel     equ 7 Parity error interrupt
VIRQ1ELevel equ 0 Not Currently Used
VCT2Level   equ 6 VMEChip Timer 2  (not enabled)
VCT1Level   equ 0 VMEChip Timer 1  (not enabled)
VIACKLevel  equ 0 VME Interrupter IACK has been processed (not enabled)
DMACLevel   equ 1 DMA Controller (not enabled)
SIG3Level   equ 0 Signal 3 from GCSR (not enabled)
SIG2Level   equ 0 Signal 2 from GCSR (not enabled)
SIG1Level   equ 0 Signal 1 from GCSR (not enabled)
SIG0Level   equ 0 Signal 0 from GCSR (not enabled)
LM1Level    equ 0 Location Monitor 1 (GCSR)  (not enabled) 
LM0Level    equ 0 Location Monitor 0 (GCSR)  (not enabled)
SWI7Level   equ 0 Software Interrupt 7 (not enabled)
SWI6Level   equ 0 Software Interrupt 6 (not enabled)
SWI5Level   equ 0 Software Interrupt 5 (not enabled)
SWI4Level   equ 0 Software Interrupt 4 (not enabled) 
SWI3Level   equ 0 Software Interrupt 3 (not enabled)
SWI2Level   equ 0 Software Interrupt 2 (not enabled) 
SWI1Level   equ 0 Software Interrupt 1 (not enabled)
SWI0Level   equ 0 Software Interrupt 1 (not enabled)
SpareLevel  equ 0 Spare interrupt (not enabled)
VMELevel7   equ 7 VMEBus Interrupt Level 7
VMELevel6   equ 6 VMEBus Interrupt Level 6
VMELevel5   equ 5 VMEBus Interrupt Level 5
VMELevel4   equ 4 VMEBus Interrupt Level 4
VMELevel3   equ 3 VMEBus Interrupt Level 3
VMELevel2   equ 2 VMEBus Interrupt Level 2
VMELevel1   equ 1 VMEBus Interrupt Level 1

*
* These are interrupts originated at or by the MCChip
*
LANCLevel   equ 5 Intel 82549 LANC interrupt must be high priority.
SCSILevel   equ 2 NCR 53C710 SCSI chip 
SerLevel    equ 3 Cirrus Logic 2401 Serial chip
TckTmrLev equ 6 MCchip timer 2 is system tick
TermLev   equ SerLevel All interrupts from the chip are set at same level
T1Level equ TermLev user terminal #2
T2Level equ TermLev user terminal #3
T3Level equ TermLev user terminal #4
P1Level equ TermLev terminal #2 used as a printer.
P2Level equ TermLev terminal #3 used as a printer.
P3Level equ TermLev terminal #4 used as a printer.

*********************************************************************
*             On-Board Interrupt Vector Assignments
*********************************************************************
*
*  The vector that is presented at the time of the interrupt is programmed
*  into the VMEChip2 via the IRQVBR register (defined in VmeChip2.d).
*
*  The VMEChip internal circuitry defines a priority for these interrupts
*  internally.  The order of the definitions of the vectors below
*  reflect this internal priority.
*      VMEBus ACFAIL is highest
*      Abort Switch  is second
*          .
*          .
*      VMEBus IRQ1 lowest
*

*  Note:  can only define high 4 bits of Vector.

VmeChVBR0_default  equ $60
VmeChVBR1_default  equ $70

ACFAILVect equ VmeChVBR0_default+$F  ACFAIL*
SYSFLVect  equ VmeChVBR0_default+$D VMEBus SYSFAIL line
VMEWPVect  equ VmeChVBR0_default+$C VMEBus Master write post error
PEVect     equ VmeChVBR0_default+$B Parity error interrupt
VIRQ1EVect equ VmeChVBR0_default+$A VMEIRQ1 ( Not Currently Used )
VCT2Vect   equ VmeChVBR0_default+$9 VMEChip Timer 2  (system tick)
VCT1Vect   equ VmeChVBR0_default+$8 VMEChip Timer 1  (not enabled)
VIACKVect  equ VmeChVBR0_default+$7 VME Interrupter IACK (not enabled)
DMACVect   equ VmeChVBR0_default+$6 DMA Controller (not enabled)
SIG3Vect   equ VmeChVBR0_default+$5 Signal 3 from GCSR (not enabled)
SIG2Vect   equ VmeChVBR0_default+$4 Signal 2 from GCSR (not enabled)
SIG1Vect   equ VmeChVBR0_default+$3 Signal 1 from GCSR (not enabled)
SIG0Vect   equ VmeChVBR0_default+$2 Signal 0 from GCSR (not enabled)
LM1Vect    equ VmeChVBR0_default+$1 Location Monitor 1 (GCSR)  (not enabled) 
LM0Vect    equ VmeChVBR0_default+$0 Location Monitor 0 (GCSR)  (not enabled)
SWI7Vect   equ VmeChVBR1_default+$F Software Interrupt 7 (not enabled)
SWI6Vect   equ VmeChVBR1_default+$D Software Interrupt 6 (not enabled)
SWI5Vect   equ VmeChVBR1_default+$E Software Interrupt 5 (not enabled)
SWI4Vect   equ VmeChVBR1_default+$C Software Interrupt 4 (not enabled) 
SWI3Vect   equ VmeChVBR1_default+$B Software Interrupt 3 (not enabled)
SWI2Vect   equ VmeChVBR1_default+$A Software Interrupt 2 (not enabled) 
SWI1Vect   equ VmeChVBR1_default+$9 Software Interrupt 1 (not enabled)
SWI0Vect   equ VmeChVBR1_default+$8 Software Interrupt 1 (not enabled)
SpareVect  equ VmeChVBR1_default+$7 Spare interrupt (not enabled)

*
* These are interrupts originated at or by the MCChip.
*

ABORTVect	equ MCdefIV+MCvecABT	Abort switch
TckTmrVect	equ	MCdefIV+MCvecT1		Tick timer 1 - must match ClkPort below
SCSIVect	equ MCdefIV+MCvecSCSI	SCSI vector

*
* Interrupt base vector for IP2 ('FX models only)
*
IP2Vect		equ $4A		(source offsets range from 0-4)

*  Zilog 85230 Serial Chip -- Note: Base vectors for the ports
*    must be the same between channels.
*
SerVect		equ $48
TermVect	equ SerVect Pass base vector to the driver, it does the rest.
T1Vect		equ SerVect user terminal #2
P1Vect		equ SerVect terminal #2 used as a printer.

SerVct2		equ $49 (for 2nd SCC on 'LX)
T2Vect		equ SerVct2 user terminal #3
T3Vect		equ SerVct2 user terminal #4
P2Vect		equ SerVct2 terminal #3 used as a printer.
P3Vect		equ SerVct2 terminal #4 used as a printer.


 pag
********************
* Clock port definitions
*
* The tick for the system is the MCChip counter/timer #1,
* but it can be plugged for any MCchip timer, or any VMEChip2
* timer if tk162.a is built with -aTKVME.
*
****************************************
ClkLevel equ TckTmrLev tick irq level
ClkVect equ TckTmrVect tick timer vector
ClkPrior equ 1 this device is the only one on the vector
ClkPort equ MCchBase+MCT1CR Port is control register for MCChip timer #1.

 ifdef FASTTICK
TicksSec equ 1000 ticks per second ( period = 1 msec )
 else
TicksSec equ 100 ticks per second ( period = 10 msec )
 endc
 pag
********************
* Configuration module constants
*    used only by init module
*
 ifndef DEVHARD
MDirSz set 128 override default for floppy, tape & ROM
 endc

CONFIG macro

MainFram dc.b "Motorola VME162",0
 ifdef DEVTAPE
* init module for tape booting
SysStart dc.b "mshell",0 name of initial module to execute
* parameters to pass to initial module
SysParam dc.b "tapestart; ex sysgo",C$CR,0 
* default device 
SysDev dc.b "/dd",0 initial system disk pathlist
 else DEVTAPE
 ifdef BKPLANE
* init module for backplane booting
SysStart dc.b "mshell",0 name of initial module to execute
* parameters to pass to initial module
SysParam dc.b "chx cmds; sys/startup  &",C$CR,0
* default device
SysDev dc.b "/rp",0 initial system disk pathlist
 else BKPLANE
* init module for disk/rom booting
SysStart dc.b "sysgo",0 name of initial module to execute
* parameters to pass to initial module
SysParam dc.b 0 none
* default device 
 ifdef DDDISK
SysDev dc.b "/dd",0 initial system disk pathlist
 else
 ifdef RAMDISK
SysDev dc.b "/rnv",0 initial system disk pathlist
 else
 ifdef ROMBOOT
SysDev set 0 no disk device for rom booting
 else
 ifdef DEVHARD
SysDev dc.b "/h0",0 initial system disk pathlist for hard disk
 else
SysDev dc.b "/d0",0 initial system disk pathlist for floppy disk
 endc DEVHARD
 endc ROMBOOT
 endc RAMDISK
 endc BKPLANE
 endc DEVTAPE
 endc DDDISK
ConsolNm dc.b "/term",0  console terminal pathlist
ClockNm dc.b "tk162",0 clock module name

*  NOTE that "snooper" module must appear before "syscache" module
*  NOTE: Bootlist should only include FPSP or FPU, not both.  Init module
*          now being made to allow for either with same init.
*
Extens dc.b "OS9P2 snoop162 syscache ssm " include snooping, cacheing, mmu
  dc.b "fpsp" include FPSP
  dc.b "fpu" include fpu
  dc.b " sysmbuf OS9P3"
  dc.b 0

* memory list definitions for init module (user adjustable)
 align
MemList
* MemType  type, priority, attributes, blksiz, addr limits, name, DMA-offset

 ifndef NVRAMCLR
NVRAMCLR equ $10 makeup NVRAM color type
 endc NVRAMCLR
 
 ifndef BUDDYMEM

 ifdef	LOSRAM
  MemType SYSRAM,250,B_USER,ProbeSize,CPUBeg,Mem.End,OBPrime,0
  MemType NVRAMCLR,0,B_NVRAM,ProbeSize,Mem.End,Mem.E2MB,NVPrime,0
 else	LOSRAM
* Select 16- or 32-MB of memory (or smaller if you prefer)
  MemType SYSRAM,250,B_USER,ProbeSize,CPUBeg,Mem.E16MB,OBPrime,0
*  MemType SYSRAM,250,B_USER,ProbeSize,CPUBeg,Mem.E32MB,OBPrime,0
 endc	LOSRAM

 else	BUDDYMEM

 MemType SYSRAM,250,B_USER,ProbeSize,CPUBeg,Mem.End,OBPrime,0
 ifdef LOSRAM * Option when 2MB SRAM an no DRAM
  MemType NVRAMCLR,0,B_NVRAM,ProbeSize,Mem.End,Mem.E2MB,NVPrime,0
 else  LOSRAM
  MemType SYSRAM,250,B_USER,ProbeSize,Mem.End,Mem.E2MB,OBPrime,0
 endc  LOSRAM
 MemType SYSRAM,250,B_USER,ProbeSize,Mem.E2MB,Mem.E4MB,OBPrime,0
 MemType SYSRAM,250,B_USER,ProbeSize,Mem.E4MB,Mem.E8MB,OBPrime,0
 MemType SYSRAM,250,B_USER,ProbeSize,Mem.E8MB,Mem.E16MB,OBPrime,0
* Optional definition for 2nd 16MB ECC memory
* MemType SYSRAM,250,B_USER,ProbeSize,Mem.E16MB,Mem.E32MB,OBPrime,0

 endc	BUDDYMEM

* Optional definitions for upper SRAM as non-volatile module storage
 ifdef HISRAM
  ifdef NOMBUG
   MemType NVRAMCLR,0,B_NVRAM,ProbeSize,SRAMBegin,SRAMBegin+$10000,NVPrime,0
  endc NOMBUG
  MemType NVRAMCLR,0,B_NVRAM,ProbeSize,SRAMBegin+$10000,SRAMBegin+$20000,NVPrime,0
  ifndef LXSRAM
   MemType NVRAMCLR,0,B_NVRAM,ProbeSize,SRAMBegin+$20000,SRAMBegin+$40000,NVPrime,0
   MemType NVRAMCLR,0,B_NVRAM,ProbeSize,SRAMBegin+$40000,SRAMBegin+$80000,NVPrime,0
   ifdef BIGSRAM
    MemType NVRAMCLR,0,B_NVRAM,ProbeSize,SRAMBegin+$80000,SRAMBegin+$100000,NVPrime,0
    MemType NVRAMCLR,0,B_NVRAM,ProbeSize,SRAMBegin+$100000,SRAMBegin+$200000,NVPrime,0
   endc  BIGSRAM
  endc  LXSRAM
 endc  HISRAM

* The following entries include the ROM Utilities area for module searches
 ifndef DEVHARD
* MemType SYSRAM,1,B_ROM,ProbeSize,Rom4.Utils,Rom4.UtlEnd,ROMBank4,0
* MemType SYSRAM,1,B_ROM,ProbeSize,Rom3.Utils,Rom3.UtlEnd,ROMBank3,0
* MemType SYSRAM,1,B_ROM,ProbeSize,Rom2.Utils,Rom2.UtlEnd,ROMBank2,0
* MemType SYSRAM,1,B_ROM,ProbeSize,Rom1.Utils,Rom1.UtlEnd,ROMBank1,0
 endc

 dc.l 0 terminate list

OBPrime dc.b "On-Board RAM",0
ROMBank1 dc.b "ROM Bank 1",0
ROMBank2 dc.b "ROM Bank 2",0
ROMBank3 dc.b "ROM Bank 3",0
ROMBank4 dc.b "ROM Bank 4",0
NVPrime dc.b "Battery-backed SRAM",0

* cache mode over-rides for SSM(MMU):  These entries will apply to USER
* state cache modes.  The default cache mode is specified in ssm040
* (write-thru), and these entries may be used to specify over-rides
* to the ssm module's default.
*
 CPUALIGN
CacheList
* NOTE that these have been constructed to match the regions defined
* in the MemType entries above.

 ifndef	BUDDYMEM

 ifdef	LOSRAM 
  CacheType CPUBeg,Mem.End,CopyBack
  CacheType Mem.End,Mem.E2MB,CopyBack
 else	LOSRAM
* Select one or the other (or match your custom entry)
  CacheType CPUBeg,Mem.E16MB,CopyBack
*  CacheType CPUBeg,Mem.E32MB,CopyBack
 endc	LOSRAM

 else	BUDDYMEM

 CacheType CPUBeg,Mem.End,CopyBack
 CacheType Mem.End,Mem.E2MB,CopyBack
 CacheType Mem.E2MB,Mem.E4MB,CopyBack
 CacheType Mem.E4MB,Mem.E8MB,CopyBack
 CacheType Mem.E8MB,Mem.E16MB,CopyBack
* Optional definition for 2nd 16MB ECC memory
* CacheType Mem.E16MB,Mem.E32MB,CopyBack

 endc	BUDDYMEM

* Optional entries when upper SRAM used for module storage
 ifdef HISRAM
  ifdef NOMBUG
   CacheType SRAMBegin,SRAMBegin+$10000,CopyBack
  endc   NOMBUG
  CacheType SRAMBegin+$10000,SRAMBegin+$20000,CopyBack
  ifndef LXSRAM
   CacheType SRAMBegin+$20000,SRAMBegin+$40000,CopyBack
   CacheType SRAMBegin+$40000,SRAMBegin+$80000,CopyBack
   ifdef BIGSRAM
    CacheType SRAMBegin+$80000,SRAMBegin+$100000,CopyBack
    CacheType SRAMBegin+$100000,SRAMBegin+$200000,CopyBack
   endc  BIGSRAM
  endc  LXSRAM
 endc  HISRAM
 
 CacheType 0xc0000000,0xffffffff,CISer declare USER state same as ssm default

 dc.l -1 terminate list

 endm

* (Other default values may be reset here)

 ifdef _INITMOD

 ifdef FASTTICK
Slice set 20
 endc

Compat set 0 fast irqs, use stop instruction, enable ghost modules
*Compat set ZapMem in-house: patternized memory

NoDataDis set 1 don't disable d-caches when in I/O
SnoopExt set 1 no external caches in system

 endc
 pag
*******************
* OEM Global Definitions
*
 org 0


*******************
*
* SCF device descriptor definitions
*    used only by scf device descriptor modules
*
* SCFDesc: Port,Vector,IRQlevel,Priority,Parity,BaudRate,DriverName
*

*
TERM macro
 ifdef FASTCONS
 SCFDesc TermBase,TermVect,TermLev,5,$00,$0f,sc162
 else
 SCFDesc TermBase,TermVect,TermLev,5,$00,$0e,sc162
 endc 
DevCon dc.b 0
 endm

T1 macro
 ifdef FASTCOMM
 SCFDesc T1Base,T1Vect,T1Level,6,$00,$0f,sc162
 else
 SCFDesc T1Base,T1Vect,T1Level,6,$00,$0e,sc162
 endc
DevCon dc.b 0
 endm

T2 macro
 SCFDesc T2Base,T2Vect,T2Level,7,$00,$0e,sc162
DevCon dc.b 0
 endm

T3 macro
 SCFDesc T3Base,T3Vect,T3Level,8,$00,$0e,sc162
DevCon dc.b 0
 endm

P1 macro
 SCFDesc P1Base,P1Vect,P1Level,6,$00,$0e,sc162
Mode set Share_+ISize_+Write_
DevCon dc.b 0
 endm

P2 macro
 SCFDesc P2Base,P2Vect,P2Level,7,$00,$0e,sc162
Mode set Share_+ISize_+Write_
DevCon dc.b 0
 endm

P3 macro
 SCFDesc P3Base,P3Vect,P3Level,8,$00,$0e,sc162
Mode set Share_+ISize_+Write_
DevCon dc.b 0
 endm

 pag
************************************************************************
* These definitions are only used by the bootstrap ROM source.         *
************************************************************************

*  Because sysinit must search for and set up the MEMC040 chips and their
*  related ram, must have some RAM durring the search for stack and 
*  bus error vector.  We have reserved the last 512 bytes of the 128Kb
*  SRAM for this purpose.  If the SRAM is used by a user application 
*  (especially as module space) care should be taken to enforce this 
*  reservation.

*  The only switches used on the CPU are RAM switches.  If the mVme050
*  is present, then there is a switch block that will take precedence over
*  the RAM switches.
CPU_SWITCHES equ NVRAMBase+NVRAM_SWITCHES location of NVRAM switch byte.

* debugger enable controls
*
* if VME050 front panel switch 5 = closed, the debugger
*    is enabled
*
DebugBit equ 4 test bit four
DebugSw equ 1<<DebugBit debugger enable switch

* boot device controls
*
* if VME050 front panel switch 4 = closed, boot from
*   floppy (else hard disk)
* 
BootBit equ 3 test bit three
BootSw equ 1<<BootBit boot selection switch

*
*  This characterizes the serial devices for the ROM I/O

ConsType equ ZA
Cons_Adr equ TermBase console device address
 
CommType equ ZB
Comm_Adr equ T1Base auxilliary device address

*
*  information as to how to handle the RAM in the memory search.
*
RAMVects equ 1 exception vectors are RAM
*PARITY equ 1 initialize parity memory

*  this could be set up to point at the ROM space on board
*    for the inclusion of modules in the ROM.  It is as a 
*    whole outdated since the Colored memory system is in place.
*
*   Note that if 'stb' files are included in the boot rom, the
*     definitions below should point to the ROM so that ROMBUG 
*     will find them.
*

*
* Needed by Rom.
*
SysDisk equ 0 this sets boot device address
FDsk_Vct equ 0 Obsolete, needed for Boot.a

*
*  This is the ROM memory search list.
*    NOTE: The Rom Booters search the list in reverse order so the
*      most likely location is last in the list.
*    NOTE 2: Rom area 2 and 4 may be searched by the kernel but we
*      leave them out of the rom's search to speed booting since we
*      are now searching approximately 1 meg of rom/flash area.
*
MemDefs macro
 dc.l Mem.Beg,Mem.End the normal memory search list
 dc.l 0
* dc.l Rom2.Spc,Rom2.SpcEnd the special memory search list
* dc.l Rom4.Spc,Rom4.SpcEnd the special memory search list
 dc.l Rom3.Spc,Rom3.SpcEnd the special memory search list
 dc.l Rom1.Spc,Rom1.SpcEnd the special memory search list
 dc.l 0
 dc.l 0,0,0,0,0,0,0,0 free bytes for patching
 endm

* general bit definitions
Bit0 equ 0
Bit1 equ 1
Bit2 equ 2
Bit3 equ 3
Bit4 equ 4
Bit5 equ 5
Bit6 equ 6
Bit7 equ 7

********************
* Ram Disk Descriptor definitions
*
* RBFDesc Port,Vector,IRQLevel,Priority,DriverName,DriveOptions
*

* volatile ram disk
DiskR0 macro
 RBFDesc 0,0,0,0,ram,ramdisk
 ifdef DEVTAPE
SectTrk set 12288 sectors/track (3.0Meg of sectors)
 else
SectTrk set 3072 sectors/track (768k of sectors)
 endc DEVTAPE
 endm

* non-volatile ram disk
*

DiskRx macro
 RBFDesc SRAMAvail,0,0,0,ram,nvramdisk
SectTrk set (SRAMEnd-SRAMAvail)/256 sectors/track (512k-64k of sectors)
SegAlloc set 1 one sector/segment (for small disks)
 endm

*
* Descriptors for the TEAC FC-1
*

TEACFC1_TargID equ 6 scsi id of TEAC FC-1 SCSI floppy drive
TEACFC1_LUN equ 0 scsi id of TEAC FC-1 SCSI floppy drive

**********************************************************************
* floppy disk drive on the TEAC embedded SCSI floppy (235HS and 235JS)
*
*
DiskD0 macro
 RBFDesc (SCSIBase+TEACFC1_TargID),SCSIVect,SCSILevel,5,rbteac,uv380
* Default disk values may be changed here
StepRate set 3 6ms step rate
Intrleav set 1
SectOffs set 1
NoVerify set ON verify off for hard disk
MaxCount set 1<<24 practical max byte-count to pass
ScsiOpts set scsi_atn disconnect supported
Control set FmtEnabl+MultEnabl format enable, multi-sector i/o
ScsiLun set TEACFC1_LUN Logical unit number on controller
CtrlrID set TEACFC1_TargID scsi id of controller
DevCon dc.b "scsi162",0 low-level driver module
 endm

DiskHD380 macro
 RBFDesc (SCSIBase+TEACFC1_TargID),SCSIVect,SCSILevel,5,rbteac,hd380
* Default disk values may be changed here
StepRate set 3 6ms step rate
Intrleav set 1
SectOffs set 1
NoVerify set ON verify off for hard disk
MaxCount set 1<<24 practical max byte-count to pass
ScsiOpts set scsi_atn disconnect supported
Control set FmtEnabl+MultEnabl format enable, multi-sector i/o
ScsiLun set TEACFC1_LUN Logical unit number on controller
CtrlrID set TEACFC1_TargID scsi id of controller
DevCon dc.b "scsi162",0 low-level driver module
 endm

DiskED380 macro
 RBFDesc (SCSIBase+TEACFC1_TargID),SCSIVect,SCSILevel,5,rbteac,ed380
* Default disk values may be changed here
StepRate set 3 6ms step rate
Intrleav set 1
SectOffs set 1
NoVerify set ON verify off for hard disk
MaxCount set 1<<24 practical max byte-count to pass
ScsiOpts set scsi_atn disconnect supported
Control set FmtEnabl+MultEnabl format enable, multi-sector i/o
ScsiLun set TEACFC1_LUN Logical unit number on controller
CtrlrID set TEACFC1_TargID scsi id of controller
DevCon dc.b "scsi162",0 low-level driver module
 endm

DiskPCD0 macro
 PCFDesc (SCSIBase+TEACFC1_TargID),SCSIVect,SCSILevel,5,rbteac,pcdos380
* Default disk values may be changed here
StepRate set 6 3mSec step rate
Intrleav set 2
Control set MultEnabl+FmtDsabl non-formatable, multi-sector i/o
ScsiOpts set scsi_atn disconnect supported
Trys set 3 retry counter
ScsiLun set TEACFC1_LUN Logical unit number on controller
CtrlrID set TEACFC1_TargID scsi id of controller
DevCon dc.b "scsi162",0 low-level driver module
PrmSize equ *-DevCon
 endm

SCCS_TargID equ 0 scsi id of SCCS hard disk
SCCS.2_TargID equ 1 scsi id of 2nd SCCS hard disk
SCCS_LUN equ 0 hard disk lun on wren
VIPER_TargID equ 4 scsi id of Viper tape drive
VIPER_LUN equ 0 tape lun on viper

*
*  There are now two different hard disk drivers:
*
* sccs hard drive on SCSI bus is "/h0" device
*     driver may have a physical/logical relationship that is not 1:1
*
* vccs hard drive on SCSI bus is "/h0" device
*     these drives always have a 1:1 physical/logical sector mapping
*
DiskH0 macro
 ifdef VCCS
 RBFDesc SCSIBase+SCCS_TargID,SCSIVect,SCSILevel,5,rbvccs,autosize
 else
 RBFDesc SCSIBase+SCCS_TargID,SCSIVect,SCSILevel,5,rbsccs,autosize
 endc VCCS

* Default disk values may be changed here
DiskKind set Hard
BitDns set Single
TrkDns set Single
DnsTrk0 set Single
DrvNum set 0 logical device number
StepRate set 0 step period
DiskType set DiskKind+(DnsTrk0<<5)
Density set BitDns+(TrkDns<<1)
SegAlloc set 32 segment allocation factor
Intrleav set 1
TrkOffs set 0 no track offsets
SectOffs set 0 no sector offsets

 ifdef VCCS
SectSize set 0 lss=pss
 endc VCCS

NoVerify set ON verify off for hard disk
* format protected, multi-sector i/o, autosize media.
Control set AutoEnabl+FmtDsabl+MultEnabl
Trys set 7 retry counter
ScsiLun set SCCS_LUN Logical unit number on controller
WrtPrecomp set 0 precomp cylinder
RedWrtCrnt set 0 reduced write current cylinder
ParkCyl set 0 park heads cylinder
LSNOffset set 0 partition offset
TotalCyls set 0 total media cylinders
CtrlrID set SCCS_TargID scsi id of controller
MaxCount set 1<<24 practical max byte-count to pass
DevCon dc.b "scsi162",0 low-level driver module
 endm

*
*  There are now two different hard disk drivers:
*
* sccs hard drive on SCSI bus is "/h1" device
*       drive may not have a 1:1 physical/logical sector map
*
* vccs hard drive on SCSI bus is "/h1" device
*		drive must have a 1:1 logical to physical mapping
*
DiskH1 macro
 ifdef VCCS
 RBFDesc SCSIBase+SCCS.2_TargID,SCSIVect,SCSILevel,5,rbvccs,autosize
 else
 RBFDesc SCSIBase+SCCS.2_TargID,SCSIVect,SCSILevel,5,rbsccs,autosize
 endc VCCS
 
* Default disk values may be changed here
DiskKind set Hard
BitDns set Single
TrkDns set Single
DnsTrk0 set Single
DrvNum set 0 logical device number
StepRate set 0 step period
DiskType set DiskKind+(DnsTrk0<<5)
Density set BitDns+(TrkDns<<1)
SegAlloc set 32 segment allocation factor
Intrleav set 1
TrkOffs set 0 no track offsets
SectOffs set 0 no sector offsets

 ifdef VCCS
SectSize set 0 lss = pss
 endc VCCS

NoVerify set ON verify off for hard disk
* format protected, multi-sector i/o, autosize media.
Control set AutoEnabl+FmtDsabl+MultEnabl
Trys set 7 retry counter
ScsiLun set SCCS_LUN Logical unit number on controller
WrtPrecomp set 0 precomp cylinder
RedWrtCrnt set 0 reduced write current cylinder
ParkCyl set 0 park heads cylinder
SectSize set 512 bytes/sector
LSNOffset set 0 partition offset
TotalCyls set 0 total media cylinders
CtrlrID set SCCS.2_TargID scsi id of controller
MaxCount set 1<<24 practical max byte-count to pass
DevCon dc.b "scsi162",0 low-level driver module
 endm

 pag
*********************
* SBF Tape Descriptors
*

 ifdef VIPER
**********************
*  SBFDesc macro:  port, vector, IRQlevel, IRQpriority, driver name.
*
TapeMT0 macro

* user changable device descriptor defaults

DrvNum set 0 drive number
NumBlks set 8 number of blocks (buffered)
BlkSize set 0x8000 LOGICAL block size (MUST be multiple of 512)
DrvPrior set 1000 priority of "sbf" process

IRQPrior set 5 polling priority (within vector)

ScsiID set 4 scsi id of viper
ScsiLUN set 0 viper lun always 0.

DevCon dc.b "scsi162",0

 SBFDesc SCSIBase+ScsiID,SCSIVect,SCSILevel,IRQPrior,"sbviper"
ScsiOpts set scsi_atn disconnect supported
 endm

**********************
*  SBFDesc macro:  port, vector, IRQlevel, IRQpriority, driver name.
*
TapeMT1 macro

* user changable device descriptor defaults

DrvNum set 0 drive number
NumBlks set 8 number of blocks (buffered)
BlkSize set 0x8000 LOGICAL block size (MUST be multiple of 512)
DrvPrior set 1000 priority of "sbf" process

IRQPrior set 5 polling priority (within vector)

ScsiID set 3 scsi id of viper
ScsiLUN set 0 viper lun always 0.

 SBFDesc SCSIBase+ScsiID,SCSIVect,SCSILevel,IRQPrior,"sbviper"
DevCon dc.b "scsi162",0

ScsiOpts set scsi_atn disconnect supported
 endm

**********************
*  SBFDesc macro:  port, vector, IRQlevel, IRQpriority, driver name.
*
TapeDAT macro

* user changable device descriptor defaults

DrvNum set 0 drive number
NumBlks set 8 number of blocks (buffered)
BlkSize set 0x8000 LOGICAL block size (MUST be multiple of 512)
DrvPrior set 1000 priority of "sbf" process

IRQPrior set 5 polling priority (within vector)

ScsiID set 3 scsi id of DAT drive
ScsiLUN set 0 lun always 0.

 SBFDesc SCSIBase+ScsiID,SCSIVect,SCSILevel,IRQPrior,"sbviper"
DevCon dc.b "scsi162",0

ScsiOpts set scsi_atn disconnect supported
 endm

 endc VIPER

 ifdef EXABYTE

**********************
*  SBFDesc macro:  port, vector, IRQlevel, IRQpriority, driver name.
*
TapeMT2 macro

* user changable device descriptor defaults

DrvNum set 0 drive number
NumBlks set 8 number of blocks (buffered)
BlkSize set 0x8000 LOGICAL block size (MUST be multiple of 512)
DrvPrior set 1000 priority of "sbf" process

IRQPrior set 5 polling priority (within vector)

ScsiID set 5 scsi id of exabyte
ScsiLUN set 0 exabyte lun always 0.

 SBFDesc SCSIBase+ScsiID,SCSIVect,SCSILevel,IRQPrior,"sbgiga"
DevCon dc.b "scsi162",0

ScsiOpts set scsi_atn disconnect supported
 endm

 endc EXABYTE

 ifdef TEACMT2
**********************
*  SBFDesc macro:  port, vector, IRQlevel, IRQpriority, driver name.
*
TapeMT0 macro

* user changable device descriptor defaults

DrvNum set 0 drive number
NumBlks set 8 number of blocks (buffered)
BlkSize set 0x6000 LOGICAL block size (MUST be multiple of 512)
DrvPrior set 1000 priority of "sbf" process

IRQPrior set 5 polling priority (within vector)

ScsiID set 4 scsi id of teac
ScsiLUN set 0 teac lun always 0.

DevCon dc.b "scsi162",0

 SBFDesc SCSIBase+ScsiID,SCSIVect,SCSILevel,IRQPrior,"sbteac"
ScsiOpts set scsi_atn disconnect supported
 endm

 endc TEACMT2

 pag
   
********************
* Flash System Memory Definitions
*
* These are used to make descriptors for the flash file system.
* The Base Address is the Base Address of ROM (FF800000) + the Size
* of the Flash Disk.
*
* The reason we only use 512k of Flash Disk is that the first 512k of
* the whole Flash is used to store the OS-9 Coreboot, and leave the rest
* of the Flash to make a Flash Disk
 
FlashDBase      equ $FFA00000   Base address of FLASH Disk
FlashDSize	equ $100000	Size of FLASH Disk - 1 MB
FlashSecSize    equ 512         Size of sector size

********************
* Flash disk descriptor definiions
*
* FLASHDesc Port,Size,Source,Width,MFGID,DEVID,Driver name,MTD name
*
FlashRRF0 macro
 FLASHDesc FlashDBase,FlashDSize,0,8,0,0,rbftl,mtd
 endm
		    
* end of file

