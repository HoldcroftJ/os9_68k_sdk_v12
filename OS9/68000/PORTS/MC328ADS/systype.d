******************************************************************************
* System Definitions for MC68328 Evaluation Board
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
 ttl systype file for the M68328 Eval Board

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

MC68328 equ 68328
CPUTyp  set 68328    the 68302 is equivalent to a 68000
CPUType set MC68328  define the cpu module in use

* pull in mc68328 definition modules specific to this device
*
 use <m328defs>

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
* The MC68328ADS board can use either 128 Kbyte or 512 Kbyte EPROMs and SRAMs,
* configured 2 each per bank (i.e. 256 Kbyte/bank or 1024 Kbyte/bank),
* for four banks each of SRAM and EPROM.
*
* For the MemDefs and MemList macros will assume 512 Kbyte parts in all SRAM
* banks, for a total of 4 Mbytes of SRAM, and will let sysinit.a determine
* if 128 Kbyte or 512 Kbyte parts are used and configure the SRAM chip 
* selects appropriately, and will let the boot figure out exactly how much 
* SRAM is available.
*
* Also for MemDefs and MemList macros, the macros ROM_128K will be used to 
* determine the size of the EPROMs: if ROM_128K is defined, then 128 Kbyte 
* EPROMs are being used and will assume a total of 1 Mbyte of EPROM exists; 
* if ROM_128K is not defined, then a total of 4 Mbytes of EPROM exists.

***** SRAM definitions *******
*
TRANS       equ 0 no address translation
*ProbeSize   equ $1000 block probe size = 4K
ProbeSize equ $10000 block probe size = 64K   * increase to 64K to speed up boot

CPUBeg    equ 0 physical start of cpu memory
CPUSize   equ $400000 4MB SRAM

MapOut    equ $400 1K for vector table

***** ROM definitions ********
* skip vectors in Bootrom (also: start address CAN'T be 0).
*
BootRom   equ $400400 the location of the boot ROM after sysinit
 ifdef ROM_128K
BootRomsz equ $100000 1 MB EPROM
 else
*BootRomsz equ $400000 4 MB EPROM
BootRomsz equ $100000 1 MB EPROM
 endc

***************************
* Memory list definitions 
*

Mem.Beg equ CPUBeg+MapOut start of memory search
Mem.End equ CPUBeg+CPUSize end of memory search

Spc.Beg equ BootRom+10000 beginning of ROM area to search
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
MainFram dc.b "Motorola M68328ADS Board",0

* name of initial module to execute
SysStart dc.b "sysgo",0
* parameters to pass to initial module
SysParam dc.b "",0

SysDev   dc.b "/dd",0         * default disk
ConsolNm dc.b "/term",0       * console terminal pathlist
ClockNm  dc.b "tk68328",0     * clock module name

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
OnBoard dc.b "M68328ADS Evaluation Board SRAM",0 
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
SERLevel equ 1  MC68681 Debug Port (non-programmable)
PICLevel equ 6  Periodic Interrupt (Timer 1--non-programmable)

********************************
* Interrupt Vector assignments 
*
ABORTVect equ 31*4 Abort Button: Level 7 Autovector
SERVect   equ 25 MC68681 Debug Port: Level 1 Autovector
PICVect   equ 30 Periodic Interrupt: Level 6 Autovector

*********************************
* System I/O addressing
*
* the MC68681 registers at every other odd address beginning
* at $00F0 0001

DUARTADDR equ $F00001   MC68681 DUART Address
ChABase   equ DUARTADDR
ChBBase   equ DUARTADDR+$10

TermBase equ ChBBase    Uses channel B of DUART.
T1Base   equ ChABase    Uses channel A of DUART.
P1Base   equ T1Base     Identical with T1,  second half of DUART.
PICBase  equ TCTLMap    Uses Timer 1 (see timm328.d)

********************************
* Definitions for tick module
*
ClkLevel equ PICLevel define tick irq level
ClkPort  equ PICBase  define the base address
ClkVect  equ PICVect  vector
ClkPrior equ 5        IRQ polling priority
TicksSec equ 100      10 msec per tick

********************************
* The internal master frequency is set by the formula:
*
*   Master Freq = XTAL input freq * [14*(PC+1)+QC+1]
*
* where PC and QC are the P Count and Q Count values in the PLL module's
* Frequency-Select Register and 1<=Q<=14 and P>=Q+1.
*
* The Timer interrupt period is determined by counting to a reference value
* a clock source divided by a prescale value and an optional 16.
*
* To maintain a 10 millisecond period, I will make the Master Frequency
* 32,768 Hz XTAL * 500 = 16.384 MHz (QC=9 and PC=34=$22),
* and have this frequency divided by 16 as the timer's clock source
* with a PRESCALER of 100, and have the timer-compare register equal to
* 10,240=$2800
*
PC_VAL    equ 34    used by sysinit.a to set system clock frequency
QC_VAL    equ 9     ditto
Rising_Edge equ 1   clock edge captured for tick interrupt
ClkSource equ $2    clock source is system clock divided by 16
ClkVal    equ $2800 number used by tk68328.a to give a 10 ms tick
PreScaler equ 0     ditto (gives a divide by 1 prescale)

 page

********************
* OEM Global Data Usage:
*
* The OEM Global Data area is used for system specific needs.
*
* M68328ADS Eval Board Serial Driver Notes:
* ---------------------------------------
* The Evaluation Board comes with an MC68681.
*
* The sc68681 serial driver requires "mask images" for the 68681's
* IMR and ACR (write-only) registers.  These images are held in
* pairs of bytes located in the OEM Global Data area.
*
* One "pair" is required for each physical 68681 used in the system,
* so that the drivers for each "side" of each device can communicate
* with each other.  The allocation of each pair is communicated to the
* driver via the DevCon section of the SCF Descriptor for each
* "logical device".  An example allocation is given below:
*
*  Device #1:  A-side port:  "TERM" - pair #1
*  Device #1:  B-side port:  "T1"   - pair #1  (also used by P1)
*  Device #2:  A-side port:  "T2"   - pair #2  (if a second module existed)
*  Device #2:  B-side port:  "T3"   - pair #2, etc...
*
* Each pair of bytes contains the current value of these registers,
* for each device in the system.  The first byte of a pair is the
* Interrupt Mask Register (IMR) image, whilst the second byte of the
* pair is the Auxiliary Control Register (ACR) image.
* 
 org 0         base offset starts at OEM_Glob
D_681_1 do.w 1 mask image for cpu's on-board MC68681
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
	SCFDesc TermBase,SERVect,SERLevel,5,$00,$0f,sc68681
 else
	SCFDesc TermBase,SERVect,SERLevel,5,$00,$0e,sc68681
 endc
* default descriptor values can be changed here
DevCon dc.w D_681_1 offset in OEM global storage
 endm

T1 macro
 ifdef  FASTCOMM
	SCFDesc T1Base,SERVect,SERLevel,5,$00,$0f,sc68681
 else
	SCFDesc T1Base,SERVect,SERLevel,5,$00,$0e,sc68681
 endc
* default descriptor values can be changed here
DevCon dc.w D_681_1 offset in OEM global storage
 endm

P1 macro
	SCFDesc P1Base,SERVect,SERLevel,5,$00,$0e,sc68681
* default descriptor values can be changed here
DevCon dc.w D_681_1 offset in OEM global storage
 endm

* Touchpad SCF descriptor: Port address is base address of MC68328 
* registers, IRQ vector is level 5 autovector, IRQ level = 5 
* (non-programmable), 
* 
TPAD macro
 SCFDesc $fffff000,29,5,5,$00,$ff,tpaddrvr
* default descriptor values can be changed here
* Adding X_PIXELS (LCD # pixels in X dimension),
*        Y_PIXELS (LCD # pixels in Y dimension), 
*        CYC_ALM_PERIOD (cyclic alarm period), and
*        SAMPLES_PER_POINT (# samples taken per time)
* passed from tpaddesc.mak
DevCon dc.l X_PIXELS,Y_PIXELS,CYC_ALM_PERIOD,SAMPLES_PER_POINT,0
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


********************
* Flash System Memory Definitions
*
* These are used to make descriptors for the flash file system.

FlashBaseA		equ $00800000	Base address of FLASH when coldstarting from ROM
FlashSize		equ $400000		size of FLASH memory
FlashSecSize	equ 512			Size of sector size

********************
* Flash disk descriptor definiions
*
* FLASHDesc Port,Size,Source,Width,MFGID,DEVID,Driver name,MTD name
*
FlashRRF0 macro
 FLASHDesc FlashBaseA,FlashSize,0,32,0,0,rbftl,mtd
 endm

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
Comm_Adr equ T1Base auxilliary device address
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

********************
* The following are the system chip select definitions.  The values set here
* determine the chip select management and thus determine the memory
* map for the device.  
*
* There are four banks of EPROM, each bank consists of two chips.  
* CSA0 controls bank 0,..., CSA3 controls bank 3.  Either 128 Kbyte x 8 or
* 512 Kbyte x 8 EPROM devices can be used.  Jumper JP28 on the ADS board can
* be left closed either way.
*
* There are four banks of SRAM, each bank consists of two chips.  
* CSB0 controls bank 0,..., CSB3 controls bank 3.  Either 128 Kbyte x 8 or 
* 512 Kbyte x 8 SRAM devices can be used.  Jumper JP 27 on the ADS board
* must be open if using 128 Kbyte parts, closed if using 512 Kbyte parts.
*
* The following memory map will be used:
*
*   NOTE: ROM_128K refers to the definition used to indicate if 
*         128 Kbyte x 8 EPROMs are used.
*
*   NOTE: The SRAM chip-selects will be configured for either
*         128 Kbyte x 8 devices or 512 Kbyte x 8 devices by code
*         in sysinit.a -- i.e. sysinit.a determines which configuration
*         the ADS board has automatically.
*                                                     Corresponding
* _CS_  _Device_                                       _I/O Port_
*
* CSA0  EPROM Bank 1:                                     none
*        ROM_128K:         $40 0000 - $44 0000 (256K) 
*       No ROM_128K:       $40 0000 - $50 0000 (1 MB) 
*
* CSA1  EPROM Bank 2:                                     PE1
*          ROM_128K:       $44 0000 - $48 0000 (256K)
*       No ROM_128K:       $50 0000 - $60 0000 (1 MB)
*
* CSA2  EPROM Bank 3:                                     PE2
*          ROM_128K:       $48 0000 - $4C 0000 (256K)
*       No ROM_128K:       $60 0000 - $70 0000 (1 MB)
*
* CSA3  EPROM Bank 3:                                     PE3
*          ROM_128K:       $4C 0000 - $50 0000 (256K)
*       No ROM_128K:       $70 0000 - $80 0000 (1 MB)
*
* CSB0  SRAM Bank 1:                                      PE4
*                          $00 0000 - $04 0000 (256K)
*                or:       $00 0000 - $10 0000 (1 MB)
*
* CSB1  SRAM Bank 2:                                      PE5
*                          $04 0000 - $08 0000 (256K)
*                or:       $10 0000 - $20 0000 (1 MB)
*
* CSB2  SRAM Bank 3:                                      PE6
*                          $08 0000 - $0C 0000 (256K)
*                or:       $20 0000 - $30 0000 (1 MB)
*
* CSB3  SRAM Bank 4:                                      PE7
*                          $0C 0000 - $10 0000 (256K)     
*                or:       $30 0000 - $40 0000 (1 MB)
*
* CSD1  M68681 Debug Port:                                PJ5
*                          $F0 0000 - $F1 0000 (64K)
* 
* CSD2  ADI Interface:                                    PJ6
*                          $F1 0000 - $F2 0000 (64K)
* 
* CSD3  PCMCIA port:       $80 0000 - $BF 0000 (4Mb)      PortK
*
* 

* Group Base Address Register (GRPBASEA-GRPBASED) 
* and Group Mask Register (GRPMASKA-GRPMASKD) definitions:

* Group A is for EPROM in the range of $0040 0000 - $007F FFFF (4 MB)
* (will assume 512 Kbyte x 8 devices for the GRPBASEA and GRPMASKA regs
*  and let the individual chip select registers handle the "fine" decoding)
BASEA equ $004<<b_GBA   A31:20 in GRPBASEA
MASKA equ $003<<b_GBM   A21:20 ignored (4 Mbyte space) in GRPMASKA

* Group B is for SRAM in the range of $0000 0000 - $003F FFFF (4 MB)
* (will assume 512 Kbyte x 8 devices for the GRPBASEB and GRPMASKB regs
*  and let the individual chip select registers handle the "fine" decoding)
BASEB equ $000<<b_GBA   A31:20 in GRPBASEB
MASKB equ $003<<b_GBM   A21:20 ignored (4 Mbyte space) in GRPMASKB

* Group C not used for Chip Select purposes

* Group D is for MC68681 and ADI peripheral usage in the range of 
* $00F0 0000 - $00F1 FFFF (128 Kbytes)
BASED equ $008<<b_GBA   A31:20 in GRPBASED
MASKD equ $007<<b_GBM   A22-20 ignored (8 Mbyte space) in GRPMASKD

* Chip-Select Register (CSxx) definitions:

* Chip select A0 is for 256 Kbyte EPROM at $0040 0000 - $0043 FFFF
* or for 1 Mbyte EPROM at $0040 0000 - $004F FFFF
* configured for 16 bit data bus width, read-only, 1 wait states
 ifdef ROM_128K
CSA0_AC equ ($40<<b_AC_AB)+BSW+($03<<b_AM_AB)+RO+1
 else
CSA0_AC equ ($40<<b_AC_AB)+BSW+($0F<<b_AM_AB)+RO+1
 endc

* Chip select A1 is for the 256 Kbyte EPROM at $0044 0000 - $0047 FFFF
* or for 1 Mbyte EPROM at $0050 0000 - $005F FFFF
* configured for 16 bit data bus width, read-only, 1 wait states
 ifdef ROM_128K
CSA1_AC equ ($44<<b_AC_AB)+BSW+($03<<b_AM_AB)+RO+1
 else
CSA1_AC equ ($50<<b_AC_AB)+BSW+($0F<<b_AM_AB)+RO+1
 endc

* Chip select A2 is for the 256 Kbyte EPROM at $0048 0000 - $004B FFFF
* or for 1 Mbyte EPROM at $0060 0000 - $006F FFFF
* configured for 16 bit data bus width, read-only, 1 wait states
 ifdef ROM_128K
CSA2_AC equ ($48<<b_AC_AB)+BSW+($03<<b_AM_AB)+RO+1
 else
CSA2_AC equ ($60<<b_AC_AB)+BSW+($0F<<b_AM_AB)+RO+1
 endc

* Chip select A3 is for the 256 Kbyte EPROM at $004C 0000 - $004F FFFF
* or for 1 Mbyte EPROM at $0070 0000 - $007F FFFF
* configured for 16 bit data bus width, read-only, 1 wait states
 ifdef ROM_128K
CSA3_AC equ ($4C<<b_AC_AB)+BSW+($03<<b_AM_AB)+RO+1
 else
CSA3_AC equ ($70<<b_AC_AB)+BSW+($0F<<b_AM_AB)+RO+1
 endc

* Chip select B0 is for the 256 Kbyte SRAM at $0000 0000 - $0003 FFFF
* or for 1 MB SRAM at $0000 0000 - $000F FFFF
* configured for 16 bit data bus width, 6 wait states
* CSB0_AC_128 equ ($00<<b_AC_AB)+BSW+($03<<b_AM_AB)+6  (6 wait state version for slow memory)
* CSB0_AC_512 equ ($00<<b_AC_AB)+BSW+($0F<<b_AM_AB)+6
*** 0 wait state for fast SRAM parts in use.
CSB0_AC_128 equ ($00<<b_AC_AB)+BSW+($03<<b_AM_AB)+0
CSB0_AC_512 equ ($00<<b_AC_AB)+BSW+($0F<<b_AM_AB)+0

* Chip select B1 is for the 256 Kbyte SRAM at $0004 0000 - $0007 FFFF 
* or for 1 MB SRAM at $0010 0000 - $001F FFFF
* configured for 16 bit data bus width, 6 wait states
* CSB1_AC_128 equ ($04<<b_AC_AB)+BSW+($03<<b_AM_AB)+6
* CSB1_AC_512 equ ($10<<b_AC_AB)+BSW+($0F<<b_AM_AB)+6
CSB1_AC_128 equ ($04<<b_AC_AB)+BSW+($03<<b_AM_AB)+0
CSB1_AC_512 equ ($10<<b_AC_AB)+BSW+($0F<<b_AM_AB)+0

* Chip select B2 is for the 256 Kbyte SRAM at $0008 0000 - $000B FFFF
* or for 1 MB SRAM at $0020 0000 - $002F FFFF
* configured for 16 bit data bus width, 6 wait states
* CSB2_AC_128 equ ($08<<b_AC_AB)+BSW+($03<<b_AM_AB)+6
* CSB2_AC_512 equ ($20<<b_AC_AB)+BSW+($0F<<b_AM_AB)+6
CSB2_AC_128 equ ($08<<b_AC_AB)+BSW+($03<<b_AM_AB)+0
CSB2_AC_512 equ ($20<<b_AC_AB)+BSW+($0F<<b_AM_AB)+0

* Chip select B3 is for the 256 Kbyte SRAM at $000C 0000 - $000F FFFF
* or for 1 MB SRAM at $0030 0000 - $003F FFFF
* configured for 16 bit data bus width, 6 wait states
* CSB3_AC_128 equ ($0C<<b_AC_AB)+BSW+($03<<b_AM_AB)+6
* CSB3_AC_512 equ ($30<<b_AC_AB)+BSW+($0F<<b_AM_AB)+6
CSB3_AC_128 equ ($0C<<b_AC_AB)+BSW+($03<<b_AM_AB)+0
CSB3_AC_512 equ ($30<<b_AC_AB)+BSW+($0F<<b_AM_AB)+0
* Chip selects C0-C3 not used for chip-select purposes

* Chip select D0 is for "dummy space" at
* $00FF 0000-$00FF FFFF configured for 64K x 16, 0 wait states
CSD0_AC equ ($FF0<<b_AC_CD)+BSW+($00F<<b_AM_CD)

* Chip select D1 is for the 64 Kbyte M68681 Debug Port at 
* $00F0 0000-$00F0 FFFF configured for 64K x 16, external DTACK
CSD1_AC equ ($F00<<b_AC_CD)+BSW+($00F<<b_AM_CD)+7

* Chip select D2 is for the 64 Kbyte ADI Port at
* $00F1 0000-$00F1 FFFF configured for 64K x 16, 3 wait states
CSD2_AC equ ($F10<<b_AC_CD)+BSW+($00F<<b_AM_CD)+3

* Chip select D3 is for PCMCIA space at
* $0080 0000-$00BF FFFF configured for 5 wait states
* CSD3 is the only CS which may be used with the PCMCIA interface.
CSD3_AC equ ($800<<b_AC_CD)+BSW+($3FF<<b_AM_CD)+5

 pag
 opt l
