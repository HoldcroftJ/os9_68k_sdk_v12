******************************************************************************
* System Definitions for RUSSBOX Virtual OS-9/68k System
******************************************************************************
** Copyright 1995, 1996 by Microware Systems Corporation                    **
** Reproduced Under License                                                 **
**                                                                          **
** This source code is the proprietary confidential property of             **
** Microware Systems Corporation, and is provided to licensee               **
** solely for documentation and educational purposes. Reproduction,         **
** publication, or distribution in any form to any party other than         **
** the licensee is strictly prohibited.                                     **
******************************************************************************
** Edition History:                                                         **
** #   Date     Comments                                          By        **
** --- -------- ------------------------------------------------- --------- **
**  1  95/09/5  Creation                                            rws     **
**  2  95/11/13 Added ROM_128K macro                                rws     ** 
**  3  96/05/13 Updated copyright date.                             ats     **
**  4  96/05/15 Added FASTCONS, FASTCOMM option to SCF descriptors. ats     **
**  5  96/05/17 Tailored startup process, extensions lists,         ats     **
**              memlist and ROM definitions for release.                    **
**              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
**  6  96/07/22 Removed SDS_DEBUG build option.                     bat/ats **
**              Updated memory lists/maps.                                  **
**              Added /dd default device option.                            **
**              Added pwrman module to PreIO list.                          **
**              Added (optional) abort328 & dbgentry modules to             **
**                  PreIO list.                                             **
**              Added (optional) SysMbuf module to Extens list.             **
**  7  96/07/24 Switched serial ports to match board labels.        ats     **
**  8  96/08/02 General cleanup.                                    ats     **
**              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
**  9  96/11/15 Removed ifdef's around /dd default device           bat     **
**              Added RamDisk descriptor definitions                        **
**              Changed initial execute module to shell &                   **
**                automagically fork ndpd/nppd                              **
** 10  99/07/23 Changed chip selects to allow for PCMCIA space.             **
**              Added defines for PCMCIA TFFS/FTL flash disk usage.         **
**              Reduced ROM/RAM wait states for fast parts.         rkw	    **
** 11  99/08/04 Limit the rom spc search list to speed booting      mgh     **
**              changed shell to mshell as first process                    **
******************************************************************************

 opt -l

 nam systype.d
 ttl systype file for the RUSSBOX Virtual OS-9/68k System

********************************
* general bit definitions
*
Bit0 equ 0
Bit1 equ 1
Bit2 equ 2
Bit3 equ 3
Bit4 equ 4
Bit5 equ 5
Bit6 equ 6
Bit7 equ 7

********************************
* Conditional Assembly Variables

MC68000 equ 68000
CPUTyp  set 68000    *the 68302 is equivalent to a 68000
CPUType set MC68000  *define the cpu module in use

* pull in any defs specific to this device
*
* use <m328defs>

*VBRBase equ 0 undefine so boot.a will equate it to 0
RAMVects equ 1 exception vectors are in RAM 
*PARITY equ 0  parity memory off since label is not defined.


 pag
*******************************
* System Memory Definitions
*
* These are used by the MemDefs macro (for rom_com.l -- see 
* /MWOS/OS9/SRC/ROM/COMMON/bootglob.d and boot.a) and the
* MemList macro (for the init module -- see
* /MWOS/OS9/SRC/SYSMODS/INIT/init.a)
* to describe the system memory structure.
*
* For the MemDefs and MemList macros we will assume 4MB of RAM in one
* contiguous bank (externally decoded) and will let sysinit.a determine if
* more is present, contiguous with that (currently the system will support
* up to 8MB of RAM contiguous from $0-$800000), configuring things accordingly.
* The boot code thereafter can figure out how much RAM is available.
*
* Also for MemDefs and MemList, the macro ROM_<n>K will be used to define
* the size of the ROMs; eg., if ROM512K is defined, 512KB is available for
* the ROM.

***** SRAM definitions *******
*
TRANS       equ 0       *no address translation
*ProbeSize  equ $1000   *block probe size = 4K; set to 64K to speed up boot
ProbeSize equ $10000    *block probe size = 64K

CPUBeg    equ 0         *physical start of cpu memory
CPUSize   equ $400000   *4MB SRAM

MapOut    equ $400      *1K for vector table

***** ROM definitions ********
* skip vectors in Bootrom (also: start address CAN'T be 0).
*
*BootRom     equ $f00400 *the location of the boot ROM after sysinit
BootRom     equ $f00000 *the location of the boot ROM after sysinit
 ifdef ROM_512k
BootRomsz equ $080000 *512KB ROM
 else
BootRomsz equ $100000 *1 MB EPROM
 endc

***************************
* Memory list definitions 
*

Mem.Beg equ CPUBeg+MapOut   *start of memory search
Mem.End equ CPUBeg+CPUSize  *end of memory search

Spc.Beg equ BootRom+10000   *beginning of ROM area to search
Spc.End equ BootRom+BootRomsz

MemDefs macro
*
* used by boot code to find RAM for boot and kernel data
* and to find modules
*
 dc.l Mem.Beg,Mem.End the normal memory search list
 dc.l 0
 dc.l Spc.Beg,Spc.End the special memory search list
 dc.l 0
 dc.l 0,0,0,0,0,0,0,0,0,0 free bytes for patching
 endm

*********************************
* init module configuration
*

CONFIG macro

* specific defs for the Eval Board
MainFram dc.b "RUSSBOX System",0

* name of initial module to execute
SysStart dc.b "sysgo",0
* parameters to pass to initial module
SysParam dc.b "",0

SysDev   dc.b "/dd",0         * default disk
ConsolNm dc.b "/term",0       * console terminal pathlist
ClockNm  dc.b "tk_vcore",0     * clock module name

PreIO dc.b "OS9PreIO "
 ifdef PWRMAN
	dc.b "pwrman "
 endc
	dc.b 0

Extens  dc.b "OS9P2 "
 dc.b "fpu "
 ifdef SYSMBUF
	dc.b "SysMbuf "
 endc
 ifdef PWRMAN
	dc.b "sysif "
 endc
	dc.b 0

*
* memory list definitions (user adjustable)
*
 align
MemList
*
* Colored memory definition list, used by kernel to determine how much
* RAM is available, for allocating memory buffers, for creating modules
* in memory, and for loading modules into memory.
*
* MemType  type, priority, attributes, blksiz, addr limits, name, DMA-offset
*
* on-board ram covered by "boot rom memory list" - doesn't need parity iniz
*
 MemType SYSRAM,250,B_USER,ProbeSize,Mem.Beg,Mem.End,OnBoard,0
 dc.l 0 terminate list
OnBoard dc.b "RUSSBOX System RAM",0 
 endm

 ifdef _INITMOD
* (Other default values may be reset here)
*PollSz set 8
*DevCnt set 8
*Procs  set 16
*Paths  set 16
SnoopExt  equ 1  No external caches
Compat  set 0
*Compat  set (1<<5)  TEST: don't start clock on cold start
*Config set (1<<3) TEST: disable system-state time-slicing
 endc

 pag
********************************
* System Interrupt Structure
*
* Interrupt Levels
*
SERLevel equ 3  *VCORE TERM0 serial console (autovect)
PICLevel equ 6  *VCORE TIM0 Timer0 (autovect)

********************************
* Interrupt Vector assignments 
*
ABORTVect equ 31*4 Abort Button: Level 7 Autovector
SERVect   equ 26 Debug Port: Level 2 Autovector
PICVect   equ 30 Periodic Interrupt: Level 6 Autovector

*********************************
* System I/O addressing
*
* The I/O of the system is all mapped within a block
* named the GIO block.
GIO_BASE    equ $a00000     *VCORE GIO module base

PICBase     equ GIO_BASE+$00

*********************************
* System console (TERM0)
*
* The system has a system console, TERM0, located within the
* GIO address space. this is a simple UART with registers
* located at odd addresses. After boot, this would become /term.
*
* TERM1, if present, would follow in the register map and be mapped
* to device /t1.

TermBase    equ GIO_BASE+$20
*T1Base     equ GIO_BASE+$28

********************************
* Definitions for tick module
*
ClkLevel equ PICLevel *define tick irq level
ClkPort  equ PICBase  *define the base address
ClkVect  equ PICVect  *vector
ClkPrior equ 5        *IRQ polling priority
TicksSec equ 100      *10 msec per tick

********************************
* Place master clock register settings here
*
* Goal is to derive a master clock satisfying TicksSec as defined above.
* For RUSSBOX the clock setup will be minimal, as it is designed to be
* close to what is required by default.


 page

********************
* OEM Global Data Usage:
*
* The OEM Global Data area is used for system specific needs.
* Such things as saved images of write-only registers, etc. can
* be stored here.
* The RUSSBOX GIO will be designed to minimize the need for such
* things.
*
    org 0           *base offset starts at OEM_Glob
OEMGlob_TERM0_1  do.w 1      *
 do.b 128-32-. reserved for system usage
 do.b 128      user definable usage
*

 pag

************************************************
* SCF device descriptor definitions
* (used only by SCF device descriptor modules)
*
* SCFDesc: Port,Vector,IRQlevel,Priority,Parity,BaudRate,DriverName
*
* BaudRate of $0e =>  9,600 baud
* BaudRate of $0f => 19,200 baud
*
TERM macro
 ifdef  FASTCONS
	SCFDesc TermBase,SERVect,SERLevel,5,$00,$0f,sc_vcs
 else
	SCFDesc TermBase,SERVect,SERLevel,5,$00,$0e,sc_vcs
 endc
* default descriptor values can be changed here
DevCon dc.w OEMGlob_TERM0_1     *offset in OEM global storage
 endm

T1 macro
 ifdef  FASTCOMM
	SCFDesc TermBase,SERVect,SERLevel,5,$00,$0f,sc_vcs
 else
	SCFDesc TermBase,SERVect,SERLevel,5,$00,$0e,sc_vcs
 endc
* default descriptor values can be changed here
DevCon dc.w OEMGlob_TERM0_1     *offset in OEM global storage
 endm

*!P1 macro
*!	SCFDesc P1Base,SERVect,SERLevel,5,$00,$0e,sc_vcp
*!* default descriptor values can be changed here
*!DevCon dc.w OEMGlob_TERM0_1   *offset in OEM global storage
*! endm

 pag

********************
* Ram Disk Descriptor definitions
* 
* RBFDesc Port,Vector,IRQLevel,Priority,DriverName,DriveOptions
*          

* volatile ram disk
DiskR0 macro
 RBFDesc 0,0,0,0,ram,ramdisk
SectTrk set 512 sectors/track (768k of sectors)
 endc DEVTAPE
 endm

* non-volatile ram disk
* 
* WARNING - EXAMPLE ONLY
DiskR1 macro
 RBFDesc $c00000,0,0,0,ram,nvramdisk
SectTrk set 8192 sectors/track (2048k of sectors)
 endm


*!********************
*!* Flash System Memory Definitions
*!*
*!* These are used to make descriptors for the flash file system.
*!
*!FlashBaseA		equ $00800000	Base address of FLASH when coldstarting from ROM
*!FlashSize		equ $400000		size of FLASH memory
*!FlashSecSize	equ 512			Size of sector size
*!
*!********************
*!* Flash disk descriptor definiions
*!*
*!* FLASHDesc Port,Size,Source,Width,MFGID,DEVID,Driver name,MTD name
*!*
*!FlashRRF0 macro
*! FLASHDesc FlashBaseA,FlashSize,0,32,0,0,rbftl,mtd
*! endm


 opt l
* end of file

 pag

************************************************************
* The following definitions are only used by the bootstrap
* ROM source and can not be modified by the user.
*
************
*   Bit definitions for the boot "switches"
*       There is a "patchable" ROM location for the
*       boot method whose bits are defined as below.
*
*   Note:  1 = on/TRUE   0 = off/FALSE
b_DBSwitch  equ Bit4
DebugSwitch equ (1<<Bit4) debugger enable switch
BootSwitch  equ (1<<Bit3) boot selection switch
MenuSwitch  equ (1<<Bit2) menu selection switch
RomSwitch   equ (1<<Bit1) boot from rom 

** definitions not used by io68681.a
*ConsType equ M68681 console debug port
*CommType equ M68681 port B of the 68681 is the aux port
Cons_Adr equ TermBase console device address
Comm_Adr equ T0Base auxilliary device address
Trm_Baud equ $00 console device baud rate (not used)
Cons_Par equ $00 console device parity
Comm_Par equ $00 auxillary device parity

*
* These two labels are obsolete under "SysBoot" but are
* still required to link in "boot.a"
*
SysDisk set 0
FDsk_Vct set 0

 pag

 opt l
