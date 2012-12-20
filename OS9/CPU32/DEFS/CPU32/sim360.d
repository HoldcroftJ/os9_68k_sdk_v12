 opt -l
 nam sim.d
 ttl Definitions for the mc68360 SIM (Systems Integration Module)

**********************************************************************
* Definitions for the mc68360 SIM (Systems Integration Module)
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  93/10/05  history starts.                                  ARB
*				---- OS-9/68K V3.0.1 Release ----
* 02  99/12/06  Commented out DPRBASE Equates below and move to  mgh
*                systype.d in the ports directories (port specific)
* 03  00/01/19  Fixed MBAR offset                                kimk

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
*
* DPRBASE Values now in sytype.d for each port
*DPRBASE equ $00020000
*DPRBASES equ $00022000
*SBARSET	EQU	$00022000	; Dual Port RAM Base for Slave QUICC
*                               ; + Valid Bit
**************************
* Slave Specific Registers
**************************
*SMBAR	EQU	$03FF04 ; Slave Module Base Address
*SMBARE	EQU	$03FF08 ; Module Base Address Enable SLAVE

REGB	EQU	$1000	; Base address of Internal Registers


SCC1_BASE	equ $C00	
MISC_BASE	equ $CB0
SCC2_BASE	equ $D00
SPI_BASE	equ $D80
TIMER_BASE	equ $DB0
SCC3_BASE	equ $E00
IDMA1_BASE	equ $E70
SMC1_BASE	equ $E80
SCC4_BASE	equ $F00
IDMA2_BASE	equ $F70
SMC2_BASE	equ $F80

MCRMap equ DPRBASE+REGB default location of the MCR

********************************************************************
*  These are the mc68360 SIM common registers.  These are used to set up
*  the chip configuration.  Unless otherwise noted, these are 8 bit 
*  registers.  They are defined here to be used referenced from
*  MCRMap.
MCR    equ REGB+$00 Module configuration register ( 32 bits )
AVR		equ REGB+$8 Autovector base	(8 bits)
RSR 	equ REGB+$9 reset status register (8 bits)
CLKOCR 	equ REGB+$c Clk Control register (8 bits)
PLLCR	equ REGB+$10 PLL Control Register (16 bits)
CDVCR	equ REGB+$14 Clock divider Control Register
PEPAR	equ	REGB+$16 Port E Pin Assignment register
*********   System Protection         ************
SYPCR	equ REGB+$22 System Protection Control
SWIV	equ REGB+$23 Software Interrupt Vector
PICR 	equ REGB+$26 Periodic Interrupt Control
PITR	equ REGB+$2a Periodic Interrupt Timing
SWSR	equ	REGB+$2f Software Service Register
BKAR	equ REGB+$30 Breakpoint Address register
BKCR	equ REGB+$34 Breakpoint Control Register

GMR		equ REGB+$40 Global Memory Register
MSTAT	equ REGB+$44	memory Controller Status Register
BR0		equ REGB+$50 Base Register 0
OR0		equ REGB+$54 Option register 0
BR1		equ REGB+$60 Base Register 1
OR1		equ REGB+$64 Option register 1
BR2		equ REGB+$70 Base Register 2
OR2		equ REGB+$74 Option register 2
BR3		equ REGB+$80 Base Register 3
OR3		equ REGB+$84 Option register 3
BR4		equ REGB+$90 Base Register 4
OR4		equ REGB+$94 Option register 4
BR5		equ REGB+$a0 Base Register 5
OR5		equ REGB+$a4 Option register 5
BR6		equ REGB+$b0 Base Register 6
OR6		equ REGB+$b4 Option register 6
BR7		equ REGB+$c0 Base Register 7
OR7		equ REGB+$c4 Option register 7

ICCR	equ REGB+$500	Channel Configuration Register
CMR1	equ REGB+$504	IDMA1 mode register
SAPR1	equ REGB+$508	IDMA1 source address pointer
DAPR1	equ REGB+$50c	IDMA1 destination register
BCR1	equ	REGB+$510	IDMA1 byte count register
FCR1	equ	REGB+$514	IDMA1 Function code register
CMAR1	equ REGB+$516 	IDMA1Channel Mask register
CSR1	equ REGB+$518 	IDMA1 channel status

SDSR	equ REGB+$51c	SDMA status register
SDCR	equ	REGB+$51e	SDMA configuration register
SDAR	equ	REGB+$520	SDMA Address register

CMR2	equ REGB+$526	IDMA2 mode register
SAPR2	equ REGB+$528	IDMA2 source address pointer
DAPR2	equ REGB+$52c	IDMA2 destination register
BCR2	equ	REGB+$530	IDMA2 byte count register
FCR2	equ	REGB+$534	IDMA2 Function code register
CMAR2	equ REGB+$536 	IDMA2 Channel Mask register
CSR2	equ REGB+$538 	IDMA2 channel status

CICR	equ REGB+$540	CP Interrupt Configuration 
CIPR	equ REGB+$544	CP Interrupt Pending
CIMR	equ REGB+$548	CP Interrupt Mask
CISR	equ REGB+$54c	CP In-Service 

PADIR	equ	REGB+$550	Port a Data Direction
PAPAR	equ	REGB+$552
PAODR	equ REGB+$554	Port A open Drain 
PADAT	equ REGB+$556	Port a Data 

PCDIR	equ	REGB+$560	Port c Data Direction
PCPAR	equ REGB+$562	Port C Pin Assignment
PCSO	equ REGB+$564	PORT C Special Option
PCDAT	equ REGB+$566	PORT C data 
PCINT	equ REGB+$568	PORT C INterrupt Control

TGCR	equ REGB+$580	Timer Global Configuration
TMR1	equ REGB+$590	Timer 1 mode register
TMR2	equ REGB+$592	Timer 2 mode
TRR1	equ REGB+$594	timer 1 reference
TRR2	equ	REGB+$596	timer 2 reference
TCR1	equ REGB+$598	timer 1 capture
TCR2	equ REGB+$59a	timer 2 capture
TCN1	equ REGB+$59c	timer 1 counter
TCN2	equ REGB+$59e	timer 2 counter
TMR3	equ REGB+$5a0	Timer 3 mode register
TMR4	equ REGB+$5a2	Timer 4 mode
TRR3	equ REGB+$5a4	timer 3 reference
TRR4	equ	REGB+$5a6	timer 4 reference
TCR3	equ REGB+$5a8	timer 3 capture
TCR4	equ REGB+$5aa	timer 4 capture
TCN3	equ REGB+$5ac	timer 1 counter
TCN4	equ REGB+$5ae	timer 2 counter
TER1	equ	REGB+$5b0	timer 1 event
TER2	equ	REGB+$5b2	timer 2 event
TER3	equ	REGB+$5b4	timer 3 event
TER4	equ	REGB+$5b6	timer 4 event

CR		equ	REGB+$5c0	command register
RCCR	equ	REGB+$5c4	RISC Configuration
RTER	equ REGB+$5D6	RISC Timers Event
RTMR	equ REGB+$5Da	RISC TImers Mask

BRGC1	equ	REGB+$5f0	BRG1 configuration register
BRGC2	equ	REGB+$5f4	BRG2 configuration register
BRGC3	equ	REGB+$5f8	BRG3 configuration register
BRGC4	equ	REGB+$5fc	BRG4 configuration register

GSMR_L1	equ REGB+$600	SCC1 Genenal Mode register
GSMR_H1 equ REGB+$604	SCC1 General Mode 
PSMR1	equ REGB+$608	SCC1 Protocol-Specific Mode
TODR1	equ REGB+$60c	SCC1 Transmit on Demand
DSR1	equ REGB+$60e	SCC1 Data Sync 
SCCE1	equ	REGB+$610	SCC1 Event register
SCCM1	equ REGB+$614	SCC1 Mask 
SCCS1	equ	REGB+$617	SCC1 Status

GSMR_L2	equ REGB+$620	SCC2 Genenal Mode register
GSMR_H2 equ REGB+$624	SCC2 General Mode 
PSMR2	equ REGB+$628	SCC2 Protocol-Specific Mode
TODR2	equ REGB+$62c	SCC2 Transmit on Demand
DSR2	equ REGB+$62e	SCC2 Data Sync 
SCCE2	equ	REGB+$630	SCC2 Event register
SCCM2	equ REGB+$634	SCC2 Mask 
SCCS2	equ	REGB+$637	SCC2 Status

GSMR_L3	equ REGB+$640	SCC3 Genenal Mode register
GSMR_H3 equ REGB+$644	SCC3 General Mode 
PSMR3	equ REGB+$648	SCC3 Protocol-Specific Mode
TODR3	equ REGB+$64c	SCC3 Transmit on Demand
DSR3	equ REGB+$64e	SCC3 Data Sync 
SCCE3	equ	REGB+$650	SCC3 Event register
SCCM3	equ REGB+$654	SCC3 Mask 
SCCS3	equ	REGB+$657	SCC3 Status

GSMR_L4	equ REGB+$660	SCC4 Genenal Mode register
GSMR_H4 equ REGB+$664	SCC4 General Mode 
PSMR4	equ REGB+$668	SCC4 Protocol-Specific Mode
TODR4	equ REGB+$66c	SCC4 Transmit on Demand
DSR4	equ REGB+$66e	SCC4 Data Sync 
SCCE4	equ	REGB+$670	SCC4 Event register
SCCM4	equ REGB+$674	SCC4 Mask 
SCCS4	equ	REGB+$677	SCC4 Status

SMCMR1	equ	REGB+$682	SMC1 Mode
SMCE1	equ REGB+$686	SMC1 Event
SMCM1	equ REGB+$68a	SMC1 Mask

SMCMR2	equ	REGB+$692	SMC2 Mode
SMCE2	equ REGB+$696	SMC2 Event
SMCM2	equ REGB+$69a	SMC2 Mask

SPMODE	equ REGB+$6a0	SPI Mode
SPIE	equ	REGB+$6a6	SPI event
SPIM	equ REGB+$6aa	SPI Mask
SPCOM	equ REGB+$6ad	SPI Command

PIPC	equ REGB+$6b2	PIP Configuration
PTPR	equ	REGB+$6b6	PIP Timing Parameters

PBDIR	equ REGB+$6b8	Port B Data Dir
PBPAR	equ REGB+$6bc	Port B Pin Assignment
PBODR	equ REGB+$6c2	Port B OPen Drain
PBDAT	equ REGB+$6c4	Port B Data

SIMODE	equ REGB+$6e0	SI Mode
SIGMR	equ REGB+$6e4	SI Global Mode
SISTR	equ	REGB+$6e6	SI status
SICMR	equ REGB+$6e7	SI Command
SICR	equ REGB+$6ec	SI Clock Route
SIRP	equ REGB+$6f2	SI Ram Pointers
SIRAM	equ REGB+$700	Si Routing Ram
	

*****  These are separate masks within the various registers ********

********************  MCR  *************************
*  Bit positions:
b_BR040id0 equ 29	Bus request 68040 arb ID
b_BSTM	equ 16 Bus Synchronous Timing mode 0 = async 1 = sync
b_ASTM	equ 15 arbitration Synchronization timing mode 0 = async 1 = sync
b_FRZ1  equ 14 asserted will freeze SWT & PIT
b_FRZ0  equ 13 asserted will freeze Bus Monitor Enable
b_BCLROID	equ 10 Bus CLear out Arbitration ID
b_SHEN  equ  8 Show cycle enable (2 bits)
b_SUPV  equ  7 Supervisor/unrestricted data space
b_BCLRISM equ 4
b_IARBX equ  0 Interrupt arbitration bits  ( 0 = mask internal modules )
*                    ( set up to allow arbitration [normal = $F] )

FS_enable equ (1<<b_FRZ1) freeze SWT & PIT enable
FB_enable equ (1<<b_FRZ0) freeze BUS MONITOR enable
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
DBF    equ $10  Double Bus Fault ( 1 = reset caused by DOUBLE BUS FAULT)
LOC    equ $04  Loss of clock ( 1 = loss of frequency reference occured )
SYS    equ $02  System Reset ( 1 = CPU executed reset instruction )
SRSTP  equ $01 	Soft Reset Pin ( 1 = Soft reset )

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
*********************************************
* BRG Configuration register
*
b_rst	equ	1<<17	reset brg
b_enab	equ	1<<16	enable brg
b_intclk equ	0<<14	internal clock
b_clk2	equ		1<<14	clock from clk2 pin
b_clk6	equ		2<<14	clock from clk6 pin
b_atb	equ		0<<13	normal operation
b_arben	EQU		1<<13	AUTOBAUD OPERATION
b_cd	equ		0<<1	divider
b_16	equ		1<<0	16x prescaler (required 300 baud and below)
 pag
**********************************************
* General SCC Mode Register
*
** low 32 bits
M_HDLC	EQU		0<<0	MODE IS HDLC
M_APPL	EQU		2<<0	APPLETALK
M_SS7	EQU		3<<0	SS7
M_UART	EQU		4<<0	UART
M_PROFI	EQU		5<<0	PROFIBUS
M_AHDLC	EQU		6<<0	ASYNC HDLC
M_V14	EQU		7<<0	V.14
M_BISYC	EQU		8<<0	BISYNC
M_DDCMP EQU		9<<0	DDCMP
M_ETHER	EQU		$C<<0	ETHERNET
ENT		equ 	1<<4	ENABLE TRANSMIT
ENR		equ		1<<5	ENABLE RX
DIAG	EQU		0<<6	DIAGNOSTIC MODE
TENC	EQU		0<<8	NRZ
TENC_I	EQU		1<<8	NRZI
TENC_FM	EQU		2<<8	FM0
TENC_M	EQU		4<<8	MANCHESTER
TENC_DM EQU		6<<8	DIFFERENTIAL MANCHESTER
RENC	EQU		0<<11	NRZ
RENC_I	EQU		1<<11	NRZI
RENC_FM	EQU		2<<11	FM0
RENC_M	EQU		4<<11	MANCHESTER
RENC_DM EQU		6<<11	DIFFERENTIAL MANCHESTER
RDCR	EQU		0<<14	1X CLOCK MODE
RDCR_8	EQU		1<<14	8X
RDCR_16	EQU		2<<14	16X NORMAL ASYNC MODE
RDCR_32	EQU		3<<14	32X
TDCR	EQU		0<<16	1X CLOCK MODE
TDCR_8	EQU		1<<16	8X
TDCR_16	EQU		2<<16	16X NORMAL ASYNC MODE
TDCR_32	EQU		3<<16	32X
TEND	EQU		0<<18	XMIT FRAME ENDING
TPP		EQU		0<<19	PREAMBLE PATTERN ALL ZEROS
TPP_10	EQU		1<<19	REPEATING 10'S
TPP_01	EQU		2<<19	REPEATING 01'S
TPP_11	EQU		3<<19	ALL ONES
TPL		EQU		0<<21	NO PREAMBLE
TPL_8	EQU		1<<21	8 BITS
TPL_16	EQU		2<<21	16 BITS
TPL_32	EQU		3<<21	32 BITS
TPL_48	EQU		4<<21	48 BITS
TPL_64	EQU		5<<21	64 BITS
TPL_128	EQU		6<<21	128 BITS
TINV	EQU		0<<24	DPLL XMIT INPUT INVERT DATA
RINV	EQU		0<<25	DPLL RX INPUT INVERT DATA
TSNC	EQU		0<<26	TRANMIT SENSE INFINITE
TCI		EQU		0<<28	TRANSMIT CLOCK INVERT
EDGE	EQU		0<<29	CLOCK EDGE BITS USE BOTH + & - 
EDGE_P	EQU		1<<29	CLOCK EDGE IS +
EDGE_N	EQU		2<<29	CLOCK EDGE IS -
EDGE_NO	EQU		3<<29	CLOCK EDGE IS NOT USED
** high 32 bits
RSYN	equ 	0<<0	RX SYNC
RTSM	EQU		0<<1	RTS MODE
SYNL	EQU		0<<2	SYNC PATTERN IN DSR NOT USED
SYNL_4	EQU		1<<2	4 BIT SYNC
SYNL_8	EQU		2<<2	8 BIT SYNC
SYNL_16 EQU		3<<2	16 BIT SYNC
TXSY	EQU		0<<4	NO SYNC BETWEEN TX & RX
RFW		EQU		0<<5	RX FIFO IS 32 BYTES
RFW_1	EQU		0<<5	RX FIFO IS 1 BYTE
TFL		EQU		0<<6	TX FIFO IS 32 BYTES
TFL_1	EQU		1<<6	TX FIFO IS 1 BYTE
CTSS	EQU		0<<7	CTS IS ASYNC
CDS		EQU		0<<8	CD IS ASYNC
CTSP	EQU		0<<9	CTS PULSE
CDP		EQU		0<<10	CD Pulse
TTX		EQU		0<<11	TRANSPARENT TX
TRX		EQU		0<<12	TRANSPARENT RX
REVD	EQU		0<<13	NORMAL BIT ORDER
TCRC_16	EQU		0<<14	16 BIT CCITT CRC
TCRC_BI	EQU		1<<14	CRC16
TCRC_32	EQU		2<<14	32 BIT CCITT CRC


 pag
********************
* Communication processor Interrupt Mask Register (IMR) and 
* Interrupt Pending Register (IPR)
* Bit Definitions
*
b_PB11		equ		1
b_PB10		equ		2
b_SMC2		equ		3
b_SMC1		equ		4
b_SPI		equ		5
b_PC9		equ		6
b_TIMER4	equ		7
b_PC8		equ		9
b_PC7		equ		10
b_PC6		equ		11
b_TIMER3	equ		12
b_PC5		equ		14
b_PC4		equ		15
b_RTT		equ		17
b_TIMER2	equ		18
b_IDMA2		equ		20
b_IDMA1		equ		21
b_SDMA		equ		22
b_PC3		equ		23
b_PC2		equ		24
b_TIMER1	equ		25
b_PC1		equ		26
b_SCC4		equ		27
b_SCC3		equ		28
b_SCC2		equ		29
b_SCC1		equ		30
b_PC0		equ		31

 opt l
