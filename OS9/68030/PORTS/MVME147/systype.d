* System Definitions for MVME147 System
*
* opt -l
 pag
********************
* Edition History
*   date    comments                                            by
* -------- ---------------------------------------------------- ---
* 88/05/23 genesis                                              wwb
* 88/06/16 added Extens to CONFIG.                              wwb
* 88/07/12 added Syscache to Extens.                            wwb
* 88/08/01 updated DevCon for scsi33c93/rb5400 pair.            wwb
* 88/08/10 added r0 macro.                                      wwb
*           ---- OS-9/68K V2.2 Release ---- 
* 88/09/06 made ticks/second easier to change.                  wwb
* 88/09/12 updated for delta box integration (omti scsi id,     wwb
*          low-level driver name changed to "scsi147").
* 88/11/23 converted hard disk descriptors for "autosize".      wwb
* 88/11/29 converted to DevPak-OMTI/Delta version.              wwb
*           ---- OS-9/68K V2.2: Delta Release ---- 
* 89/01/05 added StackSz definition for InterNet.               wwb
* 89/01/11 made tapeboot ramdisk be 3Meg (for InterNet).        wwb
*           ---- OS-9/68K V2.2: InterNet Release ---- 
* 89/02/10 removed "included/excluded" conditionals             wwb
*          (now "ifdef"), added ABORTVECT for ROMBUG.
* 89/03/01 added colored memory defs.                           wwb
* 89/03/17 added _INITMOD definition for Compat setup.          wwb
* 89/03/27 made DEVHARD be defined via makefile.                wwb
* 89/04/18 removed ABORTVECT definition till valid for ROMBUG.  wwb
* 89/04/21 made printer devices non-sharable.                   wwb
* 89/05/23 reorganized floppy names for 2.3 release.            wwb
* 89/07/29 added defs for non-volatile ram in 48t02 chip        jal
* 89/08/01 added conditionals and disk macros for makefile      jal
*          switchable DELTA or DEVPAK disk descriptor versions
*           ---- OS-9/68K V2.3 Release ---- 
* 89/09/18 Added conditionals for ROMPAK                        Rwb
*           ---- OS-9/68K V2.3 Update Release ---- 
* 89/11/29 Merged ROMPAK version into standard release.         wwb
*           ---- OS-9/68K V2.3 Update #2 Release ---- 
* 90/04/30 added VARSECT conditional to allow switching between wwb
*          variable sector driver (rbvccs) and deblocking
*          driver (rbsccs), added cache defs for init.a,
*          updated descriptor addresses to combine port address
*          with scsi id.
* 90/06/28 changed Compat2 byte definition to release version   Rwb
* 90/07/16 Changed sectsize for hard drives under varsect to 0  Rwb
* 90/07/17 Added rbteac descriptor macro                        Rwb
* 90/08/28 Turned on NOVERIFY flag for rbteac macro             Rwb
* 90/10/02 Corrected Compat2 definitions                        Rwb
*          ---- OS-9/68K V2.4 released ----
* 91/11/18 added support for using Timer2 as tick timer.        wwb
* 93/07/12 Started rework for MWOS structure.					ats
* 93/07/23 Added defs for 320 booter support.                   ats
* 93/07/23 Adapt for new NVRAM structure.                       ats
* 93/08/09 Use FASTTICK definition for TicksSec.                ats
*          Added fpu into init extensions module list.
*          Folded in Warren's reworked disk and tape
*          descriptors (from KALI).
* 93/08/10 Correct ROM special search list to support stubs     ats
*          loaded into DRAM for testing.
* 93/09/22 Reorganized special search list.						ats
*          MapOut only for MBUGTRC testing.
*          ---- OS-9/68K V3.0 released ----
* 93/11/11 Adopt RAMLOAD flag instead of BPLOAD.				ats
* 93/12/20 Changed blocks/buffer in tape descriptors to normal. ats
* 94/01/04 Add ROM utilities to colored memory list for all     ats
*          but hard disk boots.
* 94/01/12 Correct DrvNum for VCCS/SCCS/OMTI /h1 drives.        ats
* 94/01/19 Correct port address for SCSI drives.                ats
* 94/02/10 Bumped up MDirSz for all but hard disk boots.        ats
* 95/11/17 Added port-specific NVRam version limits.            ats
* 95/12/01 Corrected scf descriptor priorities.                 ats
*          ---- OS-9/68K V3.0.2 released ----
* 00/09/20 changed to 1meg spl search area to accomdate larger  mgh
*            bootfiles now shipping in rom.  Reordered search list
*            so FF800000 area is searched first.  When OS-9 rom is
*            in at FF800000, rb and lr boots are very fast.  Since
*            the bootup is slower when motbug is present, the
*            search of FF8 area before FFA is less noticeable.
*          Add sysmbuf and OS9P3 to Extens list
*          Add DDDISK to possible initial disk device list (/dd)
* 00/09/25 Remove searching of rom area from init module search mgh
*			 lists.
*          ---- OS-9/68K V3.2 released ----
*
********************
* Conditional Assembly Variables
*
VME147 equ 147 default value for VME147


********************
* System Memory Definitions
*
* These are used by the MemDefs (for rom) and MemList (for init module)
* macros to describe the system memory structure.
*
VTblSize equ 256*4 size of vector table
VBRBase equ 0 base address of vectors
TRANS equ 0 no address translation
ProbeSize equ $1000 block search size (4K)

DRAMSize equ $00400000 assume 4Meg version MVME147
*UserMemEnd equ $00800000 search stops here (8Meg)
DRAMMax equ $02000000 largest is 32MB?

 ifdef RAMLOAD
CPUSize equ DRAMSize/2
 else  RAMLOAD
CPUSize equ DRAMSize
 endc  RAMLOAD

CPUBeg equ 0 physical start of cpu memory

 ifdef MBUGTRC
MapOut equ $6000 24K for Mot Bug (8K more than documented)
 else  MBUGTRC
MapOut equ $400 1K for vector table
 endc  MBUGTRC

Mem.Beg equ CPUBeg+MapOut start of memory search
Mem.End	equ CPUBeg+CPUSize end of memory search
Max.Beg equ CPUBeg+DRAMSize
Max.End equ CPUBeg+DRAMMax

 ifdef	ROMSTB
*
* Search entire 1Meg Rom space
*
Rom.SpcSrch		equ	$0
Rom.SSSize		equ $100000
 else	ROMSTB
Rom.SpcSrch		equ	$20000
Rom.SSSize		equ	$8000
 endc	ROMSTB

Rom.Utils		equ	Rom.SpcSrch+Rom.SSSize
 ifdef	ROMSTB
Rom.UtlSize		equ $38000
 else	ROMSTB
Rom.UtlSize		equ	$40000
 endc	ROMSTB

Rom1.Beg		equ $ff800000 start address of bank #1
 ifdef	RAMLOAD
Rom2.Beg		equ	Mem.End
 else	RAMLOAD
Rom2.Beg		equ $ffa00000 start address of bank #2
 endc	RAMLOAD

Rom1.Spc		equ Rom1.Beg+Rom.SpcSrch
Rom1.SpcEnd		equ Rom1.Spc+Rom.SSSize
Rom1.Utils		equ Rom1.Beg+Rom.Utils
Rom1.UtlEnd		equ Rom1.Utils+Rom.UtlSize

Rom2.Spc		equ Rom2.Beg+Rom.SpcSrch
Rom2.SpcEnd		equ Rom2.Spc+Rom.SSSize
Rom2.Utils		equ Rom2.Beg+Rom.Utils
Rom2.UtlEnd		equ Rom2.Utils+Rom.UtlSize

NVRAMBase equ $FFFE0000 base of non-volatile ram in 48t02 chip

NVR_VER_MIN		equ	1
NVR_VER_MAX		equ 2

CPU_SWITCHES equ NVRAMBase+NVR_SWITCHES CPU switch byte in nv_ram

********************
* Hardware type definitions
*
CPUTyp set 68030 define the processor in use
MPUChip set CPUTyp define the processor in use (tk48t02.a)

MPUClock set 20000000 for rtc48t02

CPUType set VME147 define the cpu module in use

ClocFreq set 100 5Mhz PCLK (for sc8x30)

 pag
********************
* PCC definitions
*
 use <pcc.d>

 pag
********************
* VMEchip definitions
*
 use <vmechip.d>

 pag
********************
* System Interrupt Structure
*

* Interrupt Levels

ACFAILLevel equ 7 ACFAIL* level
BERRLevel equ 7 BUSERR* level
ABORTLevel equ 7 ABORT level

TermLev equ 3 console port level
T1Level equ TermLev user terminal #2
T2Level equ TermLev user terminal #3
T3Level equ TermLev user terminal #4
P1Level equ T1Level serial printer (port #2)
P2Level equ T2Level serial printer (port #3)
P3Level equ T3Level serial printer (port #4)
PLevel equ 2 parallel printer

* Interrupt Vector assignments

* NOTE: the PCC uses vectors 64-79 and the VMEchip uses vectors 80-87
* (these are defined in 'pcc.d' and 'vmechip.d')

TermVect equ 88 user terminal #1
T1Vect equ TermVect user terminal #2 (MUST be same as "other side")
P1Vect equ T1Vect serial printer (port 2)
T2Vect equ 89 user terminal #3
P2Vect equ T2Vect serial printer (port 3)
T3Vect equ T2Vect user terminal #4 (MUST be same as "other side")
P3Vect equ T3Vect serial printer (port 4)

 ifdef ROMBUG
* map PCC abort vector

* reinstall this definition when ROMBUG can handle abort switch
* arriving on vectors other than auto level 7
*ABORTVECT equ ABORTVect use pcc.d's value
 endc ROMBUG


********************
* Device Addresses
*
BBRam equ $fffe0000 BBRam base address (2040 bytes)
RTCBase equ $fffe07f8 RTC base address
PCCBase equ $fffe1000 PCC base address
LANCEBase equ $fffe1800 LANCE base address
VMEchBase equ $fffe2000 VMEchip base address
PBase equ $fffe2800 printer data/status port

TermBase equ $fffe3002 user terminal #1 base address
T1Base equ $fffe3000 user terminal #2 base address
P1Base equ T1Base serial printer port 2 base address
T2Base equ $fffe3802 user terminal #3 base address
P2Base equ T2Base serial printer port 3 base address
T3Base equ $fffe3800 user terminal #4 base address
P3Base equ T3Base serial printer port 4 base address

SCSIBase equ $fffe4000 SCSI port base address

* ifdef INHOUSE
VME320Vect equ 240 VME320 disk
VME320Base equ $ffffb000 VME320 disk controller (NOTE: odd-byte addressable only)
* endif INHOUSE

********************
* LANCE definitions
*
* Note: these are VME147 specific defs:  'pcc.d' holds
* all generic LANCE definitions
*
lan_id equ $778 address of module's LANCE id (offset from BBRam base)
lan_base147 equ $200000 base station address of VME147 series modules


********************
* VMEchip:  default setups
*
gsel_addr equ $f disable GCSR on bus


********************
* PCC:  default setups
*
slv_mode equ 0 a24/a32 base = 0, lance base = 0, no WAITRMC


********************
* Clock port definitions
*
 ifdef TIMER2
ClkLevel equ Timer2Level tick irq level
ClkVect equ Timer2Vect tick timer vector
 else Timer 1
ClkLevel equ Timer1Level tick irq level
ClkVect equ Timer1Vect tick timer vector
 endc TIMER2

ClkPrior equ 0 tick timer has exclusive use of vector
ClkPort equ PCCBase tick timer base address

oneh.usec equ 16 counter value for 100uSec period
max.rate equ 10000 number of 100uSec periods in one second

 ifdef FASTTICK
TicksSec equ 1000 ticks per second
 else
TicksSec equ 100 ticks per second
 endc

TPreload equ 65536-(oneh.usec*max.rate/TicksSec) preload value

WDTLimit equ 15 watch-dog timer tick limit (<=15)
*** this one for showing "tick service" delays
*WDTLimit equ 1 watch-dog timer tick limit (<=15)

*WDTMode equ (WDTLimit<<4)+wdt_wdrst+wdt_enab wdt operation mode
*WDTMode equ (WDTLimit<<4)+wdt_enab wdt operation mode
WDTMode equ 0 wdt operation mode = OFF

 pag
********************
* Configuration module constants
*    used only by init module
*
 ifndef DEVHARD
MDirSz set 128 override default for floppy, tape & ROM
 endc

CONFIG macro
MainFram dc.b "Motorola VME147",0

 ifdef DEVTAPE
* init module for tape booting
SysStart dc.b "shell",0 name of initial module to execute
* parameters to pass to initial module
SysParam dc.b "tapestart; ex sysgo",C$CR,0 
SysDev dc.b "/dd",0 initial system disk pathlist
 else

* init module for disk or rom booting
SysStart dc.b "sysgo",0 name of initial module to execute
* parameters to pass to initial module
SysParam dc.b 0 none

* default system device
 ifdef DDDISK
SysDev dc.b "/dd",0 Default disk as initial system disk
 else

 ifdef ROMBOOT
SysDev set 0 no disk device for rom booting
 else

 ifdef DEVHARD
SysDev dc.b "/h0",0 initial system disk pathlist (hard drive)
 else
SysDev dc.b "/d0",0 initial system disk pathlist (floppy)
 endc DEVHARD
 endc ROMBOOT
 endc DDDISK
 endc DEVTAPE

ConsolNm dc.b "/term",0  console terminal pathlist
ClockNm dc.b "tk147",0 clock module name
Extens dc.b "OS9P2 syscache ssm fpu sysmbuf OS9P3",0 include mmu, caching, fpu emul

* memory list definitions for init module (user adjustable)
 align
MemList
* MemType  type, priority, attributes, blksiz, addr limits, name, DMA-offset

* on-board ram covered by "rom memory list" - doesn't need parity iniz
 MemType SYSRAM,250,B_USER,ProbeSize,CPUBeg,Mem.End,OnBoard,CPUBeg+TRANS

* note: users's of MVME147's with ram > 4meg should add the additional
* ram by specifying another "on-board" ram area here and modify the
* "off-board" ram area definition

* off-board expansion ram (slower than on-board)
 MemType SYSRAM,240,B_USER+B_PARITY,ProbeSize,Max.Beg,Max.End,OffBoard,0

* rom-search areas
* ifndef DEVHARD
* MemType SYSRAM,1,B_ROM,ProbeSize,Rom2.Utils,Rom2.UtlEnd,Bank2,0
* MemType SYSRAM,1,B_ROM,ProbeSize,Rom1.Utils,Rom1.UtlEnd,Bank1,0
* endc 
 
 dc.l 0 terminate list

OnBoard dc.b "On-Board RAM",0
OffBoard dc.b "VME bus RAM",0
Bank1 dc.b "ROM Bank #1",0
Bank2 dc.b "ROM Bank #2",0

 endm


* (Other default values may be reset here)

 ifdef _INITMOD

 ifdef FASTTICK
*Slice set 10 ticks/slice
*Slice set 7 ticks/slice
Slice set 1 ticks/slice
 endc

* there are no external caches on a 147, thus they are coherent.
* define the label SnoopExt to indicate this to init.a
SnoopExt set 1

*     If there are no boards on the VME bus that will perform DMAs
*     this can be "uncommented".  The change will tell the
*     kernel not to disable the data cache during I/O operations.
*NoDataDis set 1

*Compat set 0 fast irqs, use stop instruction, enable ghost modules
Compat set ZapMem in-house: patternized memory
 endc

StackSz set 4096 InterNet requires 2K (minimum) irq stack, 4K recommended.

 pag
*******************
*
* SCF device descriptor definitions
*    used only by scf device descriptor modules
*
* SCFDesc: Port,Vector,IRQlevel,Priority,Parity,BaudRate,DriverName
*
TERM macro
 ifdef FASTCONS
 SCFDesc TermBase,TermVect,TermLev,5,$00,$0f,sc147
 else
 SCFDesc TermBase,TermVect,TermLev,5,$00,$0e,sc147
 endc
* default descriptor values can be changed here
DevCon set 0
 endm

T1 macro
 ifdef FASTCOMM
 SCFDesc T1Base,T1Vect,T1Level,6,$00,$0f,sc147
 else
 SCFDesc T1Base,T1Vect,T1Level,6,$00,$0e,sc147
 endc
DevCon set 0
 endm

P1 macro
 ifdef FASTCOMM
 SCFDesc P1Base,P1Vect,P1Level,6,$00,$0f,sc147
 else
 SCFDesc P1Base,P1Vect,P1Level,6,$00,$0e,sc147
 endc
DevCon set 0
Mode set Share_+ISize_+Write_ device capabilities
 endm

T2 macro
 SCFDesc T2Base,T2Vect,T2Level,7,$00,$0e,sc147
DevCon set 0
 endm

P2 macro
 SCFDesc P2Base,P2Vect,P2Level,7,$00,$0e,sc147
DevCon set 0
Mode set Share_+ISize_+Write_ device capabilities
 endm

T3 macro
 SCFDesc T3Base,T3Vect,T3Level,8,$00,$0e,sc147
DevCon set 0
 endm

P3 macro
 SCFDesc P3Base,P3Vect,P3Level,8,$00,$0e,sc147
DevCon set 0
Mode set Share_+ISize_+Write_ device capabilities
 endm

P macro
 SCFDesc PBase,PVect,PLevel,5,$00,$00,scp147
DevCon set 0
Mode set Share_+ISize_+Write_ device capabilities
 endm

 pag
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


********************
* SCSI Definitions for WD33C93 and SCSI Peripherals
*
SelfID equ 7 scsi id of wd33c93 on SCSI bus

SCCS_TargID equ 0 scsi id of SCCS hard disk
SCCS.2_TargID equ 1 scsi id of 2nd SCCS hard disk
VIPER_TargID equ 4 scsi id of Viper tape drive
OMTI_TargID equ 6 scsi id of OMTI5400 on SCSI bus
TEACFC1_TargID equ 6 scsi id of TEAC FC-1 SCSI floppy drive

SCCS_LUN equ 0 hard disk lun on wren
VIPER_LUN equ 0 tape lun on viper
TEACFC1_LUN equ 0 scsi id of TEAC FC-1 SCSI floppy drive

OMTI_FD_LUN equ 2 Drive select number for floppy on omti5400
OMTI_HD_LUN equ 0 Drive select number for hard disk on omti5400

* remap labels for "boot33c93.a"
*TargID equ OMTI_TargID set controller's scsi id
*HDNum equ OMTI_HD_LUN set h/d lun
*FDNum equ OMTI_FD_LUN set f/d lun

DD581 equ $87 Double density 80 track DS sector offset = 1
DD580 equ $A7 Double density 80 track DS sector offset = 0
*** note: type DD580 requires special omti5400 firmware
FDType equ DD581 use normal firmware

 ifeq FDType-DD581
fd_base equ 1 sectors start at 1
 else
fd_base equ 0 sectors start at 0
 endc

********************
* RBF device descriptor definitions
*    used only by rbf descriptor modules
*

* RBFDesc Port,Vector,IRQLevel,Priority,DriverName,DriveOptions
*
 ifdef TEACFC1

* floppy disk drive on the TEAC embedded SCSI floppy (235HS and 235JS)
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
DevCon dc.b "scsi147",0 low-level driver module
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
DevCon dc.b "scsi147",0 low-level driver module
PrmSize equ *-DevCon
 endm

 else *OMTI5400

* floppy disk drives on OMTI5400
DiskD0 macro
 RBFDesc (SCSIBase+OMTI_TargID),SCSIVect,SCSILevel,5,rb5400,uv580
* Default disk values may be changed here
DrvNum set 2 logical device number
StepRate set 3 6ms step rate
Intrleav set 4
SectOffs set fd_base

* omti controller can transfer 255 (max) blocks. Dma counter can
* handle 24-bit counts.
MaxCount set SectSize*255+1 practical max byte-count to pass

Control set FmtEnabl+MultEnabl format enable, multi-sector i/o
ScsiLun set OMTI_FD_LUN Logical unit number on controller
CtrlrID set OMTI_TargID scsi id of controller
DevCon dc.b "scsi147",0 low-level driver module
 endm

DiskD0_OLD macro
 RBFDesc (SCSIBase+OMTI_TargID),SCSIVect,SCSILevel,5,rb5400,dd580
* Default disk values may be changed here
DrvNum set 2 logical device number
StepRate set 3 6ms step rate
DnsTrk0 set Double 5.25" floppy, double density on track 0
SectTrk set 16 sectors per track
SectTrk0 set 16 sectors per track, track 0
Intrleav set 4
SectOffs set fd_base

* omti controller can transfer 255 (max) blocks. Dma counter can
* handle 24-bit counts.
MaxCount set SectSize*255+1 practical max byte-count to pass

Control set FmtEnabl+MultEnabl format enable, multi-sector i/o
ScsiLun set OMTI_FD_LUN Logical unit number on controller
CtrlrID set OMTI_TargID scsi id of controller
DevCon dc.b "scsi147",0 low-level driver module
 endm

DiskDW macro
 RBFDesc (SCSIBase+OMTI_TargID),SCSIVect,SCSILevel,5,rb5400,dd580
* Default disk values may be changed here
DrvNum set 2 logical device number
StepRate set 3 6ms step rate
DnsTrk0 set Double 5.25" floppy, double density on track 0
SectTrk set 16 sectors per track
SectTrk0 set 16 sectors per track, track 0
Intrleav set 4
SectOffs set fd_base

* omti controller can transfer 255 (max) blocks. Dma counter can
* handle 24-bit counts.
MaxCount set SectSize*255+1 practical max byte-count to pass

Control set FmtEnabl+MultEnabl format enable, multi-sector i/o
ScsiLun set OMTI_FD_LUN Logical unit number on controller
CtrlrID set OMTI_TargID scsi id of controller
DevCon dc.b "scsi147",0 low-level driver module
 endm

 endc TEACFC1
 pag
********************
* Hard Disk Configuration Definitions
*
 ifdef VCCS
NoOMTI set 1
 endc
 ifdef SCCS
NoOMTI set 1
 endc

 ifdef NoOMTI
*
* ccs hard drive on SCSI bus is "/h0" device
*
DiskH0 macro
 ifdef VCCS
 RBFDesc (SCSIBase+SCCS_TargID),SCSIVect,SCSILevel,5,rbvccs,autosize
 else
 RBFDesc (SCSIBase+SCCS_TargID),SCSIVect,SCSILevel,5,rbsccs,autosize
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
 ifdef VCCS
SectSize set 0 If VARSECT defined, LOG. SECT SIZE = SAME
 endc
MaxCount set 1<<24 dma counter and rbsccs/rbvccs can handle 24-bit count

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
ScsiOpts set scsi_atn disconnect supported
DevCon dc.b "scsi147",0 low-level driver module
 endm

*
* this "/h1" device is for testing extra ccs drives
*
DiskH1 macro
 ifdef VCCS
 RBFDesc (SCSIBase+SCCS.2_TargID),SCSIVect,SCSILevel,5,rbvccs,autosize
 else
 RBFDesc (SCSIBase+SCCS.2_TargID),SCSIVect,SCSILevel,5,rbsccs,autosize
 endc VCCS
 
* Default disk values may be changed here
DiskKind set Hard
BitDns set Single
TrkDns set Single
DnsTrk0 set Single

 ifdef VCCS
DrvNum set 0 logical device number
 else
DrvNum set 1 logical device number
 endc VCCS

StepRate set 0 step period
DiskType set DiskKind+(DnsTrk0<<5)
Density set BitDns+(TrkDns<<1)
SegAlloc set 16 segment allocation factor
Intrleav set 1
TrkOffs set 0 no track offsets
SectOffs set 0 no sector offsets

* PHYSICAL SECTOR SIZE = 512
 ifdef VCCS
SectSize set 0 If VARSECT defined, LOG. SECT SIZE = SAME
 endc
MaxCount set 1<<24 dma counter and rbsccs/rbvccs can handle 24-bit count

NoVerify set ON verify off for hard disk
Control set AutoEnabl+FmtDsabl+MultEnabl format protected, multi-sector i/o
Trys set 7 retry counter
ScsiLun set SCCS_LUN Logical unit number on controller
WrtPrecomp set 0 precomp cylinder
RedWrtCrnt set 0 reduced write current cylinder
ParkCyl set 0 park heads here
LSNOffset set 0 partition offset
TotalCyls set 0 total media cylinders
CtrlrID set SCCS.2_TargID scsi id of controller`
ScsiOpts set scsi_atn disconnect supported
DevCon dc.b "scsi147",0 low-level driver module
 endm

 else *OMTI5400

* define and select supported drives for OMTI5400
*
Mini3425 equ 1 Miniscribe 3425 20M drive
Mini3650 equ 2 Miniscribe 3650 40M drive
WrenII equ 3 CDC Wren II 40M drive

HardType set Mini3425 select hard disk to use
*HardType set Mini3650 select hard disk to use
*HardType set WrenII select hard disk to use

 ifeq HardType-Mini3425

* hard disk parameters for Miniscribe 3425 (20Meg)
*
Cylinders set 612 number of cylinders
Head_Drv set 4 number of heads
Park_Cyl set 656 parking cylinder
Stp_Rate set 0 9uS step period
Sct_Trk set 32 sectors per track
Sct_Trk0 set 32 sectors per track, track 0, side 0
Low_Write set 0 starting cylinder for reduced write current (0=none)
Pre_Comp set 128 starting cylinder for write precompensation (1=all)
 endc

 ifeq HardType-Mini3650

* hard disk parameters for Miniscribe 3650 (40Meg)
*
Cylinders set 809 number of cylinders
Head_Drv set 6 number of heads
Park_Cyl set 852 parking cylinder
Stp_Rate set 0 9uS step period
Sct_Trk set 32 sectors per track
Sct_Trk0 set 32 sectors per track, track 0, side 0
Low_Write set Cylinders starting cylinder for reduced write current (0=none)
Pre_Comp set 128 starting cylinder for write precompensation (1=all)
 endc

 ifeq HardType-WrenII

* hard disk parameters for CDC Wren II drive (40Meg)
*
Cylinders set 925 number of cylinders
Head_Drv set 5 number of heads
Park_Cyl set 0 parking cylinder
Stp_Rate set 0 9uS step period
Sct_Trk set 32 sectors per track
Sct_Trk0 set 32 sectors per track, track 0, side 0
Low_Write set 0 starting cylinder for reduced write current (0=none)
Pre_Comp set 1 starting cylinder for write precompensation (1=all)
 endc

*
* winchester drive on OMTI5400 is "/h0" device
*
DiskH0 macro
 RBFDesc (SCSIBase+OMTI_TargID),SCSIVect,SCSILevel,5,rb5400
 
* Default disk values may be changed here
DiskKind set Hard
BitDns set Single
TrkDns set Single
DnsTrk0 set Single
DrvNum set 0 logical device number
StepRate set Stp_Rate step period
DiskType set DiskKind+(DnsTrk0<<5)
Density set BitDns+(TrkDns<<1)
Cylnders set Cylinders
Heads set Head_Drv
SectTrk set Sct_Trk sectors per track
SectTrk0 set Sct_Trk0 sectors per track, track 0
SegAlloc set Sct_Trk segment allocation factor
Intrleav set 1
TrkOffs set 0 no track offsets
SectOffs set 0 no sector offsets

* PHYSICAL SECTOR SIZE = 512
*SectSize set 512

* omti controller can transfer 255 (max) blocks. Dma counter can
* handle 24-bit counts.
MaxCount set SectSize*255+1 practical max byte-count to pass

NoVerify set ON verify off for hard disk
Control set FmtDsabl+MultEnabl format protected, multi-sector i/o
Trys set 7 retry counter
ScsiLun set OMTI_HD_LUN Logical unit number on controller
WrtPrecomp set Pre_Comp precomp cylinder
RedWrtCrnt set Low_Write reduced write current cylinder
ParkCyl set Park_Cyl park heads here
LSNOffset set 0 partition offset
TotalCyls set Cylnders total media cylinders
CtrlrID set OMTI_TargID scsi id of controller
DevCon dc.b "scsi147",0 low-level driver module
 endm

*
* winchester drive on OMTI5400 is "/h1" device
*
DiskH1 macro
 RBFDesc (SCSIBase+OMTI_TargID),SCSIVect,SCSILevel,5,rb5400
 
* Default disk values may be changed here
DiskKind set Hard
BitDns set Single
TrkDns set Single
DnsTrk0 set Single
DrvNum set 1 logical device number
StepRate set Stp_Rate step period
DiskType set DiskKind+(DnsTrk0<<5)
Density set BitDns+(TrkDns<<1)
Cylnders set Cylinders
Heads set Head_Drv
SectTrk set Sct_Trk sectors per track
SectTrk0 set Sct_Trk0 sectors per track, track 0
SegAlloc set Sct_Trk segment allocation factor
Intrleav set 1
TrkOffs set 0 no track offsets
SectOffs set 0 no sector offsets

* PHYSICAL SECTOR SIZE = 512
*SectSize set 512

* omti controller can transfer 255 (max) blocks. Dma counter can
* handle 24-bit counts.
MaxCount set SectSize*255+1 practical max byte-count to pass

NoVerify set ON verify off for hard disk
Control set FmtDsabl+MultEnabl format protected, multi-sector i/o
Trys set 7 retry counter
ScsiLun set OMTI_HD_LUN Logical unit number on controller
WrtPrecomp set Pre_Comp precomp cylinder
RedWrtCrnt set Low_Write reduced write current cylinder
ParkCyl set Park_Cyl park heads here
LSNOffset set 0 partition offset
TotalCyls set Cylnders total media cylinders
CtrlrID set OMTI_TargID scsi id of controller
DevCon dc.b "scsi147",0 low-level driver module
 endm

 endc *OMTI5400

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

DevCon dc.b "scsi147",0

 SBFDesc (SCSIBase+ScsiID),SCSIVect,SCSILevel,IRQPrior,"sbviper"
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

 SBFDesc (SCSIBase+ScsiID),SCSIVect,SCSILevel,IRQPrior,"sbviper"
DevCon dc.b "scsi147",0

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

 SBFDesc (SCSIBase+ScsiID),SCSIVect,SCSILevel,IRQPrior,"sbviper"
DevCon dc.b "scsi147",0

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

 SBFDesc (SCSIBase+ScsiID),SCSIVect,SCSILevel,IRQPrior,"sbgiga"
DevCon dc.b "scsi147",0

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

DevCon dc.b "scsi147",0

 SBFDesc (SCSIBase+ScsiID),SCSIVect,SCSILevel,IRQPrior,"sbteac"
ScsiOpts set scsi_atn disconnect supported
 endm

 endc TEACMT2

 pag
********************************
* These definitions are only used by the bootstrap ROM source
* and can not be modified by the user.
*

ConsType equ ZA console debug port
CommType equ ZB aux port

RAMVects equ 1 exception vectors are RAM
PARITY equ 1 initialize parity memory

Cons_Adr equ TermBase console device address
Comm_Adr equ T1Base auxilliary device address
*Cons_Adr equ T2Base alternate ports for testing
*Comm_Adr equ T3Base

* baud-rate default (9600) over-ride for ioz8530.a
 ifdef FASTCONS
ConsBaud set 6 console=19200
 endc
 ifdef FASTCOMM
CommBaud set 6 comm port=9600
 endc

SysDisk equ SCSIBase this sets boot device address
FDsk_Vct equ SCSIVect floppy disk vector


********************
* Optional VME050 Controls
*

* debugger enable controls
*
* if front panel switch 5 = closed, the debugger is enabled
*
DebugBit equ 4 test bit four
DebugSw equ 1<<DebugBit debugger enable switch

* boot device controls
*
* if front panel switch 4 = closed, boot from floppy (else hard disk)
* 
BootBit equ 3 test bit three
BootSw equ 1<<BootBit boot selection switch

*
*	This is the ROM memory search list.
*
MemDefs macro
 dc.l Mem.Beg,Mem.End the normal memory search list
 dc.l 0
 dc.l Rom2.Spc,Rom2.SpcEnd the special memory search list (bank #2)
 dc.l Rom1.Spc,Rom1.SpcEnd the special memory search list (bank #1)
 dc.l 0
 dc.l 0,0,0,0,0,0,0,0,0,0,0,0 free bytes for patching
 endm

 ifdef INHOUSE
 use ../vme319_20/vme319_20.d
 endc INHOUSE

 opt l
