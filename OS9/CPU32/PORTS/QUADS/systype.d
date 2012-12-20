*
* System Definitions for M68360 System
*
 opt -l

* pull in mc683xx modules specific to this device
 use <mc68360defs>

 nam systype.d
 ttl systype file for the M68360QUADS
********************************
* Edition History
*   date    comments                                            by
* -------- ---------------------------------------------------- ---
* 93/10/07  Creation                                            arb
*			---- OS-9/68K V3.0.1 Release ----
* 94/07/25	Minor updates for Allan's 2nd and 3rd versions      mgh
* 94/12/12  Added CommType and Comm_adr values for single port  mgh
*           downloading support
* 99/12/06  Move DBRBASE(s) Here from sim360.d (port specific)  mgh

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
*   MBAR is defined in sim360.d but here just for info.
*	MBAR    equ $3FF00      Alway appears here (use MOVES to access.)
*
DPRBASE equ $00020000
DPRBASES equ $00022000
SBARSET EQU $00022000   		 Dual Port RAM Base for Slave QUICC
*                                + Valid Bit
**************************
* Slave Specific Registers
**************************
SMBAR   EQU $03FF04 			Slave Module Base Address
SMBARE  EQU $03FF08 			Module Base Address Enable SLAVE


*****************************************************************
*
*  The default configuration is set up to be a M68360quads
*     system module.
*  The following are the assembly "switches" as currently defined:
*
*  MANUAL_RAM nessesary because until the chip selects are programmed
*             the mc68360 cannot reach the RAM memory.
*
*  

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

* see nv.h for definitions
MenuSwitch		equ		2	/* menu selection switch	*/
BootSwitch		equ  	4	/* boot selection switch	*/
DebugSwitch		equ		8	/* debugger enable switch	*/
RomSwitch		equ   	$10    /* boot from rom (autoselect) */
AutoSwitch		equ		$20	/* autoboot selection switch			*/
IpcSwitch		equ		$80	/* enable ipc register */

********************************
* Conditional Assembly Variables

MC68360 equ 68360
CPUTyp  set 68300   define the mpu chip family
CPUType set MC68360 define the cpu module in use

 pag
*******************************
* System Memory Definitions
*
* These are used by the MemDefs (for rom) and MemList (for init module)
* macros to describe the system ram structure.
*
VBRBase equ $400000 base address of vectors
TRANS equ 0 no address translation

BootRom   equ 0 the location of the boot ROM after sysinit
BootRomsz equ $20000 there is 128K of ROM onboard the quads

*
* Init module macro
*
Slice set 4

CONFIG macro
MainFram dc.b "Motorola Quads 68360",0
SysStart dc.b "sysgo",0 name of initial module to execute
SysParam dc.b 0 parameters to pass to initial module
SysDev dc.b "/dd",0 ram disk device
ConsolNm dc.b "/term",0 console terminal pathlist
ClockNm  dc.b "tk68360",0 clock module name
Extens dc.b "fpu sysmbuf os9p2",0

*
* memory list definitions for init module (user adjustable)
*
 align
MemList
* MemType  type, priority, attributes, blksiz, addr limits, name, DMA-offset
*
* on-board ram covered by "boot rom memory list" - doesn't need parity iniz
*
 MemType SYSRAM,250,B_USER,$1000,$400000,$C00000,OnBoard,0
 MemType SYSRAM,250,B_USER+B_ROM,$1000,$80000,$100000,OnBoard_flash,0
 MemType SYSRAM,250,B_USER+B_ROM,$1000,$400,$20000,OnBoard_rom,0
 dc.l 0 terminate list
OnBoard dc.b "On Board",0
OnBoard_flash dc.b "Flash",0
OnBoard_rom   dc.b "Rom",0

* (Other default values may be reset here)

Compat set $00 fast irqs, use stop instruction, enable sicky modules
NoDataDis equ 1  No DMA drivers (or caches for that matter)
SnoopExt  equ 1  No external caches

	endm

* timerdefs.quads file for demonstrating generic clock setups
*
* tick timer device definitions
*
* See tk68360.a if changed
*
TicksSec equ 200 		system ticks per second
*TicksSec equ 225 		system ticks per second (as close as it gets)
ClkVect  equ $de 		tick interrupt vector
ClkPrior equ 6 			priority to irq on
ClkPort  equ $20000 	tick timer hardware address

TERM macro
 SCFDesc $22e00,$fc,5,3,$00,$0E,sc68360
DevCon set 0
 endm

************************************************************
* The following definitions are only used by the bootstrap
* ROM source and can not be modified by the user.
*

ConsType equ MC68360 console debug port
Trm_Baud equ $00 console device baud rate (not used)
Cons_Adr equ SCC3_BASE console device address
*Cons_Par equ 0 		not used for 68360
CommType equ MC68360 	use same port as console (only one available)
Comm_Adr equ SCC3_BASE  use same port as console (only one available)
*Comm_Par equ 0 				

FDsk_Vct equ $0
SysDisk  equ $0
 pag
***************************
* Memory list definitions 
*
Mem.Beg equ $400400 start of normal memory area to search
Mem.End equ $500000 end of normal memory area to search

Spc.Beg equ $1000 beginning of ROM area to search quads ROM
Spc.End equ $20000 end of Quads ROM

MemDefs macro
 dc.l Mem.Beg,Mem.End									1mb dram
 dc.l 0
 dc.l Spc.Beg,Spc.End the special memory search list
 dc.l $80000,$100000	flash memory
 dc.l 0
 dc.l 0,0,0,0,0,0,0,0,0,0 free bytes for patching
 endm

 opt l
