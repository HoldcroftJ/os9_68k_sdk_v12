 opt -l
*
* System Definitions for MVME177 System
*
********************
* Edition History
*   date    comments                                            by
* -------- ---------------------------------------------------- ---
* 94/04/19 Started MVME177 initial version (based on 167).		wwb
* 95/02/10 Deactivate testing values.                           ats
* 95/03/02 Added port-specific NVRAM version limits.            ats
*          <<<---- OS-9/68K V3.0.2 Beta release ---->>>
*          <<<---- OS-9/68K V3.0.2 Release ---->>>
* 99/07/13 changed to 1meg spl search area to accomdate larger  mgh
*            bootfiles now shipping in rom.  Reordered search list
*            so FF800000 area is searched first.  When OS-9 rom is
*            in at FF800000, rb and lr boots are very fast.  Since
*            the bootup is slower when motbug is present, the
*            search of FF8 area before FFA is less noticeable.
*          Add sysmbuf and OS9P3 to Extens list
*          Add DDDISK to possible initial disk device list (/dd)
* 99/07/16 Limited rom special search area to 32k to cover      mgh
*            beginning of rom boot.  Other search lists should be
*            added to init module.
* 99/07/18 add comments for system state cacheing set           mgh
*

********************
* Conditional Assembly Variables
*
VME177 equ 177 default value for VME177

********************
* System Memory Definitions
*
* These are used by the MemDefs (for rom) and MemList (for init module)
* macros to describe the system ram structure.
*
SRAMBugSize equ $10000 Saving 64K for the 162Bug space.
SRAMBase equ $FFE00000 Reserve it at beginning of SRAM.

SRAMSize equ $20000 full size of SRAM 128K bytes
VTblSize equ 256*4

 ifdef MBUGTRC
SRAMResSize equ VTblSize space for vector table
SRAMRes equ SRAMBase+SRAMSize-SRAMResSize
 else  MBUGTRC
SRAMResSize equ 0 vector space will overlap SRAM usage
SRAMRes equ SRAMBase
 endc  MBUGTRC

SRAMBegin equ SRAMBase+SRAMBugSize Static RAM (available at power up)..
SRAMEnd equ SRAMBase+SRAMSize-SRAMResSize

VBRBase equ SRAMRes base address of vectors

DRAMMax equ $04000000 maximum board size is 64Meg
DRAMSize equ $00400000

 ifdef RAMLOAD
CPUSize equ DRAMSize/2
 else  RAMLOAD
CPUSize equ DRAMSize
 endc  RAMLOAD

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

 ifeq CPUBeg
* if DRAM mapped at 0, then "addresses 0 - 0x100" unusable
*MapOut equ $100 map out first 256
MapOut equ 0
 else
MapOut equ 0 use all of DRAM
 endc MapOut

Mem.Beg equ CPUBeg+MapOut start of memory search
Mem.End equ CPUBeg+CPUSize end of memory search
Max.Beg equ CPUBeg+DRAMSize Begining of expansion RAM on the CPU
Max.End equ CPUBeg+DRAMMax end of maximum sized board (32Meg)

RomSize equ $80000*2*2 set size of roms in use (2 x 272024) (16-bit wide)

 ifdef	ROMSTB
Rom.SpcSrch		equ $0
Rom.SSSize		equ $100000
 else	ROMSTB
Rom.SpcSrch		equ	$20000
Rom.SSSize		equ	$8000
 endc	ROMSTB

* Rom.Utils is used by init module search lists
Rom.Utils		equ	0
Rom.UtlSize		equ $100000

Rom1.Beg		equ $ff800000
 ifdef	RAMLOAD
Rom2.Beg		equ Mem.End
 else	RAMLOAD
Rom2.Beg		equ	$ffa00000
 endc	RAMLOAD

Rom1.Spc		equ Rom1.Beg+Rom.SpcSrch
Rom1.SpcEnd		equ Rom1.Spc+Rom.SSSize
Rom1.Utils		equ Rom1.Beg+Rom.Utils
Rom1.UtlEnd		equ	Rom1.Utils+Rom.UtlSize

Rom2.Spc		equ Rom2.Beg+Rom.SpcSrch
Rom2.SpcEnd		equ Rom2.Spc+Rom.SSSize
Rom2.Utils		equ Rom2.Beg+Rom.Utils
Rom2.UtlEnd		equ	Rom2.Utils+Rom.UtlSize

*  These definitions are for the NVRAM and how it is used by the
*  reconfig program.  (Must agree with systype.h and SCSI booter)
*
NVRAMBase      equ $FFFC0000   Base address of 48t08 NVRAM.
NVRAM_SWITCHES equ NVR_SWITCHES

NVR_VER_MIN		equ 2	Minimum version supported for port
NVR_VER_MAX		equ 2	Maximum version supported for port

* Motorola configuration memory definitions
* (as defined/used by Motorola 177 debugger)
*
MotConfig equ $1EF8 offset of Motorola debugger configuration area in NVRAM
MotConfSize equ 256 size of the area
MotCPUSpeed equ 48 offset to CPU Speed value (char[4])



********************
* Hardware type definitions
*
CPUTyp   set 68060  (68060 !!!! )define the processor in use
MPUChip  set CPUTyp define the processor in use
CPUType  set VME177 define the cpu module in use
MPUClock set 33000000 Default value of 33 MHz chosen,  used by rtc48t08.


********************
* Device Addresses
*  The VME and  PCC chips have been designed specifically for use with
*  the MC68060.  This board makes use of a new memory controller, the
*  MEMC040.  There may be up to 2 of these on the board.  The board also
*  contains an NCR 53c710 SCSI controller chip, Cirrus Logic CD2401
*  4 port serial chip, an Intel 82596 LAN controller, a MK48t08 RCT, and
*  128K bytes of SRAM.
*
ShortIOBase  equ $FFFF0000     VME short I/O space base address
IOBase       equ $FFF00000     On-board I/O devices start here
VMEchBase    equ IOBase+$40000 VMEchip base address (LCSR)
GCSRBase     equ VMEchBase+$100 VMEchip base address (GCSR)
PCCchBase    equ IOBase+$42000 PCCchip base address (Peripheral controller)
MEMC40_1Base equ IOBase+$43000 MEMC040 #1
MEMC40_2Base equ IOBase+$43100 MEMC040 #2
TERMBase     equ IOBase+$45000 Cirrus Logic DC2401 Serial Chip
LANBase      equ IOBase+$46000 Intel 82596 LANC
SCSIBase     equ IOBase+$47000 NCR 53C710 SCSI controller chip
RTCBase      equ IOBase+$C1FF8 MK48t08 RTC chip clock register base address
PRNTBase     equ PCCchBase     Parallel Port (PCC2 Chip)
 pag
 use <pcc2.d>       Definitions for the PCC2
 use <vmechip2.d>   Definitions for the VMEChip2
 use <memc040.d>    Definitions for the MEMC040 memory controller
 use <mcecc.d>		Definitions for the MCECC memory controller

 
********************
* System State Cacheing Mode
*  Set system state cacheing mode to copyback (no Page tables)
* Current set to 0 by init.a for Write Thru mode for use with ethernet
*
* ifdef Config
*Config set SSM_CMode
* endc
 
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
LCLbto equ (LBTO_8<<b_VMEARB_lbto)        Local Bus timeout at 8 usec

* VMEChip arbiter/timeout/prescaler register default
*
VMEARB_default equ VMEbto+VMEGblto+LCLbto

* default GCSR definitions
* (map group address to Motorola default group, decoder disabled)
*
GCSR_default equ ($CC<<24)+($F<<20)

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
* Terminal devices need pointer to global register set.
*   The Channel number is encoded into the low byte of the
*   address.  The CD2401 contains 4 autonomus channels.
TermBase equ TERMBase CL-CD2401 port 1 (Bootstrap terminal)
T1Base equ TermBase+1 CL-CD2401 port 2 (Boot Download port)
P1Base equ T1Base     replaces T1 when in use
T2Base equ TermBase+2 CL-CD2401 port 3
P2Base equ T2Base     replaces T2 when in use
T3Base equ TermBase+3 CL-CD2401 port 4
P3Base equ T3Base     replaces T3 when in use

*********************************************************************
*              VME177 On-Board Interrupt Structure                  *
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
* These are interrupts originated at or by the PCC
*
GPIOLevel   equ 0 PCC GPIO (not enabled)
PCCTK2Level equ 6 PCC Timer 2 -- Used as the System Tick (tk177)
PCCTK1Level equ 0 PCC Timer 1 (not enabled)

LANCLevel   equ 5 Intel 82549 LANC interrupt must be high priority.
SCSILevel   equ 2 NCR 53C710 SCSI chip 
SerLevel    equ 3 Cirrus Logic 2401 Serial chip
SCMILevel   equ SerLevel Modem Interrupt
SCTILevel   equ SerLevel Transmit Interrupt
SCRILevel   equ SerLevel Receive Interrupt

PPACKLevel  equ 1 Parallel port ACK
PPFLTLevel  equ 1 Parallel port FAULT
PPSELLevel  equ 0 Parallel port SELECT  (not currently enabled)
PPPELevel   equ 0 Parallel port Paper Empty  (not currently enabled)
PPBSYLevel  equ 0 Parallel port BUSY (not currently enabled)
	

TckTmrLev equ VCT2Level Vme chip timer 2 is system tick
TermLev   equ SerLevel All interrupts from the chip are set at same level
T1Level equ TermLev user terminal #2
P1Level equ TermLev terminal #2 used as a printer.
T2Level equ TermLev user terminal #3
P2Level equ TermLev terminal #3 used as a printer.
T3Level equ TermLev user terminal #4
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
ABORTVect  equ VmeChVBR0_default+$E ABORT pushbutton
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
* PCCVBR2 dependent
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
* These are interrupts originated at or by the PCC.  
*

PCCVectBase set $60 PCC Base Vector (uses 96 - 110) (high 4 bits only)

GPIOVect   equ PCCVectBase+$A PCC GPIO (not enabled)
PCCTK2Vect equ PCCVectBase+$8 PCC Timer 2 -- Used as the System Tick (tk177)
PCCTK1Vect equ PCCVectBase+$9 PCC Timer 1 (not enabled)
LANEVect   equ PCCVectBase+$6 Intel 82549 LANC Error
LANCVect   equ PCCVectBase+$7 Intel 82549 LANC Interrupt
SCSIVect   equ PCCVectBase+$5 NCR 53C710 SCSI chip 
PPACKVect  equ PCCVectBase+$4 Parallel port ACK
PPFLTVect  equ PCCVectBase+$3 Parallel port FAULT
PPSELVect  equ PCCVectBase+$2 Parallel port SELECT  (not currently enabled)
PPPEVect   equ PCCVectBase+$1 Parallel port Paper Empty  (not currently enabled)
PPBSYVect  equ PCCVectBase+$0 Parallel port BUSY (not currently enabled)

*   Cirrus Logic 2401 Serial chip genrates it's own vector.
SerVect    equ 112        Receiver Exception 
SCMIVect   equ SerVect+1  Modem Interrupt
SCTIVect   equ SerVect+2  Transmit Interrupt
SCRIVect   equ SerVect+3  Receive Interrupt

TckTmrVect equ VCT2Vect Vme chip timer 2 is system tick

*  Cirrus Logic CD2401 Serial Chip -- Note: Base vectors for the ports
*    must not be the same between channels.  Each channel produces 4
*    discrete vectors.  (X0-X3)
*
TermVect   equ SerVect Pass base vector to the driver, it does the rest.
T1Vect equ TermVect+$04 user terminal #2
P1Vect equ TermVect+$04 terminal #2 used as a printer.
T2Vect equ TermVect+$08 user terminal #3
P2Vect equ TermVect+$08 terminal #3 used as a printer.
T3Vect equ TermVect+$0C user terminal #4
P3Vect equ TermVect+$0C terminal #4 used as a printer.


 pag
********************
* Clock port definitions
*
*  The tick for the system is the VMEChip2 counter/timer #2.
*
****************************************
ClkLevel equ TckTmrLev tick irq level
ClkVect equ TckTmrVect tick timer vector
ClkPrior equ 1 this device is the only one on the vector
ClkPort equ VMEchBase ticker is in the VME chip

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
MainFram dc.b "Motorola VME177",0
 ifdef DEVTAPE
* init module for tape booting
SysStart dc.b "mshell",0 name of initial module to execute
* parameters to pass to initial module
SysParam dc.b "tapestart; ex sysgo",C$CR,0
* default device 
SysDev dc.b "/dd",0 initial system disk pathlist
 else
* init module for disk/rom booting
SysStart dc.b "sysgo",0 name of initial module to execute
* parameters to pass to initial module
SysParam dc.w 0 none
* default device 
 ifdef ROMBOOT
SysDev set 0 no disk device for rom booting
 else
 ifdef DDDISK
SysDev dc.b "/dd",0 initial system disk pathlist for hard disk
 else
 ifdef DEVHARD
SysDev dc.b "/h0",0 initial system disk pathlist for hard disk
 else
SysDev dc.b "/d0",0 initial system disk pathlist for floppy disk
 endc DEVHARD
 endc ROMBOOT
 endc DDDDISK
 endc DEVTAPE
ConsolNm dc.b "/term",0  console terminal pathlist
ClockNm dc.b "tk177",0 clock module name

*  NOTE that "snooper" module must appear before "syscache" module
*
* include mmu, snooping, caching.
Extens dc.b "OS9P2 snoop177 syscache ssm fpsp fpu intsp"
  dc.b " sysmbuf" initialize sysmbuf system call if present
  dc.b " OS9P3" add another generic name for users
  dc.b 0 Terminate the Extens list.
	  

* memory list definitions for init module (user adjustable)
 align
MemList
* MemType  type, priority, attributes, blksiz, addr limits, name, DMA-offset

* ROM searched RAM.  Does not need parity init.
* In Minimum configuraton, there is a 4MByte block
*
 MemType SYSRAM,250,B_USER,ProbeSize,Mem.Beg,Mem.End,OBPrime,0

* Optional RAM:  addressed from 4Meg - 8/16/32Meg
* Requires Parity initialization
*
 MemType SYSRAM,240,B_USER+B_PARITY,ProbeSize,Max.Beg,Max.End,OBSecnd,0

 ifndef DEVHARD
* MemType SYSRAM,1,B_ROM,ProbeSize,Rom2.Utils,Rom2.UtlEnd,ROMBank2,0
* MemType SYSRAM,1,B_ROM,ProbeSize,Rom1.Utils,Rom1.UtlEnd,ROMBank1,0
 endc

 dc.l 0 terminate list

OBPrime dc.b "Primary On-Board RAM",0
OBSecnd dc.b "Secondary On-Board RAM",0
ROMBank1 dc.b "ROM Bank 1",0
ROMBank2 dc.b "ROM Bank 2",0

* cache mode over-rides for SSM(MMU):  These entries will apply to USER
* state cache modes.  The default cache mode is specified in ssm060
* (write-thru), and these entries may be used to specify over-rides
* to the ssm module's default.
*
 CPUALIGN
CacheList
* NOTE that these have been constructed to match the regions defined
* in the MemType entries above.

 CacheType Mem.Beg,Mem.End,CopyBack
 CacheType Max.Beg,Max.End,CopyBack
* CacheType Mem.Beg,Mem.End,WrtThru
* CacheType Max.Beg,Max.End,WrtThru

 CacheType 0xe0000000,0xffffffff,CISer declare USER state same as ssm default

 dc.l -1 terminate list

 endm

* (Other default values may be reset here)

 ifdef _INITMOD

 ifdef FASTTICK
*Slice set 20
Slice set 1
 endc

*Compat set 0
Compat set ZapMem in-house: patternized memory

NoSnoop060 set 1 68060 on-chip caches are NOT h/w coherent
SnoopExt set 1 no external caches in system

 endc
 pag
*******************
* OEM Global Definitions
*
 org 0
CD2401_0 do.b 1 CD2401 Device #1: irqs set flag
 do.b 128-32-. reserved
 do.b 128 user definable


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
 SCFDesc TermBase,TermVect,TermLev,5,$00,$0f,sc177
 else
 SCFDesc TermBase,TermVect,TermLev,5,$00,$0e,sc177
 endc 
DevCon dc.b CD2401_0
 endm

T1 macro
 ifdef FASTCOMM
 SCFDesc T1Base,T1Vect,T1Level,5,$00,$0f,sc177
 else
 SCFDesc T1Base,T1Vect,T1Level,5,$00,$0e,sc177
 endc
DevCon dc.b CD2401_0
 endm

P1 macro
 SCFDesc P1Base,P1Vect,P1Level,5,$00,$0e,sc177
Mode set Share_+ISize_+Write_
DevCon dc.b CD2401_0
 endm

T2 macro
 SCFDesc T2Base,T2Vect,T2Level,5,$00,$0e,sc177
DevCon dc.b CD2401_0
 endm

P2 macro
 SCFDesc P2Base,P2Vect,P3Level,5,$00,$0e,sc177
Mode set Share_+ISize_+Write_
DevCon dc.b CD2401_0
 endm

T3 macro
 SCFDesc T3Base,T3Vect,T3Level,5,$00,$0e,sc177
DevCon dc.b CD2401_0
 endm

P3 macro
 SCFDesc P3Base,P3Vect,P3Level,5,$00,$0e,sc177
Mode set Share_+ISize_+Write_
DevCon dc.b CD2401_0
 endm

* Parallel port on the PCC chip 
P macro
 SCFDesc PRNTBase,PPACKVect,PPACKLevel,5,$00,$00,scp177
Mode set Share_+ISize_+Write_
DevCon set 0
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

SRAMResv equ (SRAMBase+SRAMSize-512) reserve 512 bytes of SRAM for mem probe

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

 ifndef CD2401
CD2401 set 2401
 endc
*
*  This characterizes the serial devices for the ROM I/O

Cons_Adr equ TermBase console device address
 
 ifndef FASTCONS
ConsBaud set $40      console baud rate at 9600
ConsClk  set $1       clock prescaler (RCOR_CS1, see DEFS/cd2401.d)
 else
ConsBaud set $81      console Baud rate at 19200
ConsClk  set $0       clock prescaler (RCOR_CS0, see DEFS/cd2401.d)
 endc 
Cons_Par  equ 0 console device parity unused
Cons_Vect equ TermVect

CommType equ CD2401 aux port
Comm_Adr equ T1Base auxilliary device address
 ifndef FASTCOMM
CommBaud set $40      comm port 9600
CommClk  set $1       clock prescaler (RCOR_CS1, see DEFS/cd2401.d)
 else
CommBaud set $81      comm port at 19200
CommClk  set $1       clock prescaler (RCOR_CS0, see DEFS/cd2401.d)
 endc
Comm_Par equ 0 comm device parity unused 
Comm_Vect equ T1Vect

*  The CD2401 has a clock input that is internally divided by 2048
*    and used to clock several internal registers.  The TPR must
*    be programmed such that:
*        1 milli-second = sysclk (HZ) / 2048 / TPR
*
*  minimum legal value for the register is $A, which coresponds to an
*  input clock of ~20MHz.  The mVme167 clocks the chip at 20MHz.
*     (see initext.a)
*
TPRDefault equ $A  Time period register must be set to 1ms


*  Rom Access times in nano-seconds (patchable in ROM)
*
RomAAccess equ 200
RomBAccess equ 200

*  SRAM access time in nano-seconds (patchable in ROM)
*
SRAMAccess equ 100

* Maximum/Minimum bus clock delay values for ROM/SRAM
* (used by sysinit)
*
Max_ROMbusclks equ 11
Min_ROMbusclks equ 4
Max_RAMbusclks equ 6
Min_RAMbusclks equ 3

* ROM size code (patchable in ROM)
*
RomSizeCode equ LBCTL_SIZE_8M default 27C202 eproms

*
*  information as to how to handle the RAM in the memory search.
*
RAMVects equ 1 exception vectors are RAM
PARITY equ 1 initialize parity memory

*
* Needed by Rom.
*
SysDisk equ 0 this sets boot device address
FDsk_Vct equ 0 Obsolete, needed for Boot.a

*
*  This is the ROM memory search list.
*
*    NOTE: The Rom Booters search the list in reverse order so the
*      most likely location is last in the list.
*
MemDefs macro
 dc.l Mem.Beg,Mem.End the normal memory search list
 dc.l 0
 dc.l Rom2.Spc,Rom2.SpcEnd the special memory search list
 dc.l Rom1.Spc,Rom1.SpcEnd the special memory search list
 dc.l 0
 dc.l 0,0,0,0,0,0,0,0,0,0,0,0 free bytes for patching
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
* WARNING - EXAMPLE ONLY

DiskR1 macro
 RBFDesc $c00000,0,0,0,ram,nvramdisk
SectTrk set 8192 sectors/track (2048k of sectors)
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
DevCon dc.b "scsi177",0 low-level driver module
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
DevCon dc.b "scsi177",0 low-level driver module
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
*     drive may have a physical/logical relationship that is not 1:1
*
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
LSNOffset set 0 partition offset
TotalCyls set 0 total media cylinders
CtrlrID set SCCS_TargID scsi id of controller
MaxCount set 1<<24 practical max byte-count to pass
DevCon dc.b "scsi177",0 low-level driver module
 endm

*
*  There are now two different hard disk drivers:
*
* sccs hard drive on SCSI bus is "/h1" device
*        drive may not have a 1:1 physical/logical sector map
*
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

* PHYSICAL SECTOR SIZE = 512
SectSize set 0 Physical sector and LOG. SECT SIZE = SAME
NoVerify set ON verify off for hard disk
* format protected, multi-sector i/o, autosize media.
Control set AutoEnabl+FmtDsabl+MultEnabl
Trys set 7 retry counter
ScsiLun set SCCS_LUN Logical unit number on controller
WrtPrecomp set 0 precomp cylinder
RedWrtCrnt set 0 reduced write current cylinder
ParkCyl set 0 park heads cylinder

 ifdef VCCS
SectSize set 0 lss = pss
 endc VCCS

LSNOffset set 0 partition offset
TotalCyls set 0 total media cylinders
CtrlrID set SCCS.2_TargID scsi id of controller
MaxCount set 1<<24 practical max byte-count to pass
DevCon dc.b "scsi177",0 low-level driver module
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

DevCon dc.b "scsi177",0

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
DevCon dc.b "scsi177",0

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
DevCon dc.b "scsi177",0

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
DevCon dc.b "scsi177",0

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

ScsiID set 4 scsi id of viper
ScsiLUN set 0 viper lun always 0.

DevCon dc.b "scsi177",0

 SBFDesc SCSIBase+ScsiID,SCSIVect,SCSILevel,IRQPrior,"sbteac"
ScsiOpts set scsi_atn disconnect supported
 endm

 endc TEACMT2

 opt l
* end of file

