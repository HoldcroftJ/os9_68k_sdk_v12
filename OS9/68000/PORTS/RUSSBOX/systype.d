*
* System Definitions for RUSSBOX 990 system
*
 opt -l

 nam systype.d
 ttl systype file for the 'RUSSBOX' 68000 system
********************************
* Edition History
*   date    comments                                            by
* -------- ---------------------------------------------------- ---
*			---- OS-9/68K V3.0.1 Release ----
* 12/07/10  Creation                                            rlm

*****************************************************************
*
*  MANUAL_RAM is not necessary in this system as RAM is available
*  from reset.
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

 ifeq 1
** (From process.a)
** Status Register Fields
TraceBit:       equ 7 trace (every instruction) bit in system byte
TraceCBit:      equ 6 trace (on flow change) bit in system byte
SupvrBit:       equ 5 supervisor bit in system byte
MSPBit:         equ 4 master stack pointer bit in system byte

Trace:          equ %10000000<<8 trace mode
TraceC:         equ %01000000<<8 trace (on flow change) mode
Supervis:       equ %00100000<<8 supervisor state
MasterSP:       equ %00010000<<8 master (supervisor) stack
InterruptSP:    equ ^MasterSP interrupt (supervisor) stack (default @ RESET)
IntMask:        equ %00000111<<8 interrupt mask
IntEnab:        equ ^IntMask
Extend:         equ %00010000 Extend bit
Negative:       equ %00001000 Negative flag
Zero:           equ %00000100 Zero flag
TwosOvfl:       equ %00000010 Two's Comp Overflow flag
Carry:          equ %00000001 Carry bit
NoCarry:        equ ^Carry

Sign:           equ $80 sign bit of a byte
SignBit:        equ 7 bit number of sign bit
 endc

********************************
* Conditional Assembly Variables

RB68k   equ 990
CPUTyp  set 68000   *define the mpu chip family
CPUType set RB68k   *define the cpu module in use

* Set this in makefile instead (how? -rlm 2012-07-22)
ROMBUG  set 1       *ROMBug is to be used
FIXED_CPUTYP set 1  *Don't determine CPU type dynamically (addr err test etc.)
RAMVects    set 1
*PARITY  set 1       *patternize RAM with $FEEDC0DE

IVect1     equ $19
IVect2     equ $1a
IVect3     equ $1b
IVect4     equ $1c
IVect5     equ $1d
IVect6     equ $1e
IVect7     equ $1f

 pag
****** RB68k (990) IO module defs ******
** TIM0 (Timer 0), TIMT (Tick Timer)
TimBase   equ $a00000

Tim0Base  equ (TimBase+$00)
Tim0_DATw       equ $00         *16-bit up counter: irq on overflow, write = clear
Tim0_CTLb       equ $03
Tim0_DIVb       equ $05         *[7:0] Div 8MHz / (1/2/4/.../128)

TimTBase  equ (TimBase+$0c)
TimT_DATb       equ $01         *8-bit down counter - 1ms per tick
TimT_LOADb      equ $03         *counter preload value (0=disabled); read to ack irq

** Tim0, TimT Register Bits
Tim0_CTLb_CntEn equ %00000100   *COUNTEN
Tim0_CTLb_Pend  equ %00000010   *irq pending
Tim0_CTLb_IrqEn equ %00000001   *irq enable
*

** RBT0 (TERM0), RBT1 (TERM1)
T0Base    equ $a00020
T1Base    equ $a00030
** reg offsets
Tn_TXb          equ $01
Tn_RXb          equ $03
Tn_CTLb         equ $05

** RBT0,RBT1 Register Bits
Tn_CTLb_BR1     equ %10000000
Tn_CTLb_BR0     equ %01000000
Tn_CTLb_BRMask  equ (Tn_CTLb_BR1+Tn_CTLb_BR0)
Tn_CTLb_TXE     equ %00100000
Tn_CTLb_RXF     equ %00010000
Tn_CTLb_RXV     equ %00001000
Tn_CTLb_TEI     equ %00000100
Tn_CTLb_RFI     equ %00000010
Tn_CTLb_IRQEN   equ %00000001
Tn_TXEBit   equ 5   *bit pos for CTLb_TXE
Tn_RXFBit   equ 4   *bit pos for CTLb_RXF
Tn_RXVBit   equ 3   *bit pos for CTLb_RXV (RX overflow)
Tn_TEIBit   equ 2   *bit pos for CTLb_TEI (Tx Empty IRQ Pend)
Tn_RFIBit   equ 1   *bit pos for CTLb_RFI (Rx Full IRQ pend)
Tn_IRQBit   equ 0   *bit pos for CTLb_IRQEN

** reg values for ROMbug console (non-irq) mode
ConsCtlValue    equ (Tn_CTLb_BR1+Tn_CTLb_BR0)

** Trace register (word)
TraceReg0       equ $a00040

 pag
*******************************
* System Memory Definitions
*
* These are used by the MemDefs (for rom) and MemList (for init module)
* macros to describe the system ram structure.
*
*!VBRBase equ $0 base address of vectors
TRANS equ 0 no address translation

BootRom   equ $f00000   *location of the boot ROM after sysinit
BootRomsz equ $80000    *512K of ROM onboard

*
* Init module macro
*
Slice set 4

CONFIG macro
MainFram dc.b "Motorola 68000 (RUSSBOX '990)",0
SysStart dc.b "sysgo",0     *name of initial module to execute
SysParam dc.b 0             *parameters to pass to initial module
SysDev   dc.b "/dd",0       *ram disk device
ConsolNm dc.b "/term",0     *console terminal pathlist
ClockNm  dc.b "tk68990",0   *clock module name
*!Extens   dc.b "fpu sysmbuf os9p2",0
Extens   dc.b "fpu",0

 align

*
* memory list definitions for init module (user adjustable)
*
MemList:
* MemType  type, priority, attributes, blksiz, addr limits, name, DMA-offset
*
* on-board ram covered by "boot rom memory list" - doesn't need parity iniz
*
 MemType SYSRAM,250,B_USER,$1000,$400,$400000,OnBoard,0
 MemType SYSRAM,250,B_USER+B_ROM,$1000,$f00000,$f80000,OnBoard_flash,0
 dc.l 0     terminate list

OnBoard:        dc.b "On Board",0
OnBoard_flash:  dc.b "Flash",0
 align

NoDataDis:  equ 1   *No DMA drivers (or caches for that matter)
SnoopExt:   equ 1   *No external caches

 endm

* (Other default values may be reset here)
 ifdef _INITMOD
Compat:     set $00 *NoStop  *fast irqs, don't use stop instruction, enable sticky modules
 endc

* timerdefs.russbox file for demonstrating generic clock setups
*
* tick timer device definitions
*
* See tk990.a if changed
*
TicksSec  equ 100         *system ticks per second
ClkVect  equ IVect6       *tick interrupt vector (IRQ6) ** Vector #, NOT address!
ClkPrior equ 1              *priority to irq on (F$Irq chain ordering)
ClkPort  equ TimTBase       *tick timer hardware address

* SCFDesc: Port,Vector,IRQlevel,Priority,Parity,BaudRate,DriverName
TERM macro
 SCFDesc T0Base,IVect3,3,1,$00,$00,sc68990
DevCon set 0
 endm

T0 macro
 SCFDesc T0Base,IVect3,3,1,$00,$00,sc68990
DevCon set 0
 endm

T1 macro
 SCFDesc T1Base,IVect3,3,1,$00,$00,sc68990
DevCon set 0
 endm

************************************************************
* The following definitions are only used by the bootstrap
* ROM source and can not be modified by the user.
*
ConsType    equ RB68k       *console debug port
Cons_Adr    equ T0Base      *console device address
Trm_Baud    equ $00         *console device baud rate (not used currently -rlm)
Cons_Par    equ 0           *not used
CommType    equ RB68k
Comm_Adr    equ T1Base
Comm_Par    equ 0

****
* No disk type storage (yet)
FDsk_Vct equ $0
SysDisk  equ $0

 pag
***************************
* Memory list definitions 
*
Mem.Beg equ $000400     *start of normal memory area to search (destructive)
Mem.End equ $400000     *end of normal memory area to search

Spc.Beg equ $f00000     *beginning of ROM area to search (non-destructive)
Spc.End equ $f80000     *end of ROM
*Spc2.Beg equ $380000   *special RAM area (ROMBug d/l for testing)
*Spc2.End equ $400000   *

MemDefs macro
    dc.l    Mem.Beg,Mem.End									1mb dram
    dc.l    0
    dc.l    Spc.Beg,Spc.End     *the special memory search list
    dc.l    0
    dc.l    0,0,0,0,0,0,0,0,0,0 *free bytes for patching
    endm

 opt l
