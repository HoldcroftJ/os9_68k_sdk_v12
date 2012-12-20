 opt -l

* Copyright 1991, 1993 by Microware Systems Corporation.
* All Rights Reserved
* Reproduced Under License

*   This source code is the proprietary confidential property of
* Microware Systems Corporation, and is provided to the licensee
* for documentation and educational purposes only.  Reproduction,
* publication, or any form of distribution to any party other than
* the licensee is strictly prohibited.

********************
* Definitions for Motorola 68302 on-chip peripherals
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  91/02/21  history starts.                                  rag
* 02  93/11/03  Added baud rate frequencies for XTAL of 20000000 dwj
*				---- OS-9/68K V3.0 Release ----
* 

********************
* System Configuration Registers - fixed addresses M68000 exception vectors
*
BAR		equ		$0f2		*! Base Address Register (Word)
SCR		equ		$0f4		*! System Control Register (Longword)

********************
* System Control Register Bit Definitions
*
* System Status Bits
IPA		equ		1<<27		*! Unmasked Interrupt Request
HWT		equ		1<<26		*! Hardware Watchdog Timeout
WPV		equ		1<<25		*! Write Protect Violation
ADC		equ		1<<24		*! Address Decode Conflict

* System Control Bits
ERRE	equ		1<<22		*! External RISC request enable
VGE		equ		1<<21		*! Vector Generation Enable (Disable CPU Mode)
WPVE	equ		1<<20		*! Write Protect Violation Enable
RMCST	equ		1<<19		*! RMC Cycle special treatment
EMWS	equ		1<<18		*! External Master Wait State
ADCE	equ		1<<17		*! Address Decode Conflict Enable
BCLM	equ		1<<16		*! Bus Clear Mask
SAM		equ		1<<12		*! Sychnronous Access Mode enable

* Hardware Watchdog Bits
HWDEN	equ		1<<11		*! Watchdog enable
*							*! Bits 10-8 are the count (8us to 1ms)

* Low power (standby) mode bits
LPREC	equ		1<<7		*! Destructive Recovery from low power
LPP16	equ		1<<6		*! Low power clock prescale divide by 16 enable
LPEN	equ		1<<5		*! Low power enable
*							*! Bits 0-4 are low power clock divider selects

* Freeze Control mode bits
FRZW	equ		1<<15
FRZ2	equ		1<<14
FRZ1	equ		1<<13

*********************
* SCC Specific Parameter RAM Definitions
* Base offsets from value set in BAR above
SCC1	equ		$400		*! Base for SCC1
SCC2	equ		$500		*! Base for SCC2
SCC3	equ		$600		*! Base for SCC3

* Further Offsets from the above bases
* 	- Common Paramaters for all SCC Modes
RxBD0	equ		$00
RxBD1	equ		$08
RxBD2	equ		$10
RxBD3	equ		$18
RxBD4	equ		$20
RxBD5	equ		$28
RxBD6	equ		$30
RxBD7	equ		$38
TxBD0	equ		$40
TxBD1	equ		$48
TxBD2	equ		$50
TxBD3	equ		$58
TxBD4	equ		$60
TxBD5	equ		$68
TxBD6	equ		$70
TxBD7	equ		$78

RFCR	equ		$80
TFCR	equ		$81
MRBLR	equ		$82
RBDNUM	equ		$87
TBDNUM	equ		$93

*	- UART specific parameter RAM
MAX_IDL	equ		$9c
IDLC	equ		$9e
BRKCR	equ		$a0

PAREC	equ		$a2
FRMEC	equ		$a4
NOSEC	equ		$a6
BRKEC	equ		$a8
UADDR1	equ		$aa
UADDR2	equ		$ac
RCCR	equ		$ae

*******************
* Control Character Offsets
CHARACTER1 equ	$b0
CHARACTER2 equ	$b2
CHARACTER3 equ	$b4
CHARACTER4 equ	$b6
CHARACTER5 equ	$b8
CHARACTER6 equ	$ba
CHARACTER7 equ	$bc
CHARACTER8 equ	$be

*******************
* Control Character bit definitions
*
* Common bits
CCE			equ	1<<15	end of Control Character Table
CCR			equ	1<<14	reject character - not entered in RX buffer

* CHARACTER8 only bits
CCREA		equ	1<<13	character is ready for transmission
CCI			equ 1<<12	interrupt when character output
CCCT		equ	1<<11	Clear-To-Send lost
CCA			equ 1<<8	Address - used with multi-drop operation

********************
* Parameter Ram Registers - offset from the BAR address
BERRCN	equ		$67c		*! Bus Error Channel Number

CMR		equ		$802		*! Channel Mode Register
SAPR	equ		$804		*! Source Address Pointer
DAPR	equ		$808		*! Destination Address Pointer
BCR		equ		$80c		*! Base Count Register
CSR		equ		$80e		*! Channel Status Register
FCR		equ		$810		*! Function Code Register

GIMR	equ		$812		*! Global Interrupt Mode Register
IPR		equ		$814		*! Interrupt Pending Register
IMR		equ		$816		*! Interrupt Mask Register
ISR		equ		$818		*! In Service Register

PACNT	equ		$81e		*! Port A Count Register
PADDR	equ		$820		*! Port A Data Direction Register
PADAT	equ		$822		*! Port A Data Register
PBCNT	equ		$824		*! Port B Count Register
PBDDR	equ		$826		*! Port B Data Direction Register
PBDAT	equ		$828		*! Port B Data Register

BR0		equ		$830		*! Base Register 0
OR0		equ		$832		*! Option Register 0
BR1		equ		$834		*! Base Register 1
OR1		equ		$836		*! Option Register 1
BR2		equ		$838		*! Base Register 2
OR2		equ		$83a		*! Option Register 2
BR3		equ		$83c		*! Base Register 3
OR3		equ		$83e		*! Option Register 3

TIMER1	equ		$840		*! Base for timer unit 1
TIMER2	equ		$850		*! timer 2

TRR2    equ     $852        *! timer 2 reference register

WRR		equ		$84a		*! Watchdog Reference Register
WCN		equ		$84c		*! Watchdog Counter

CR		equ		$860		*! Command Register

SPMODE	equ		$8b0		*! SCP, SMC Mode and Clock Control Register
SIMASK	equ		$8b2		*! Serial Interface Mask Register
SIMODE	equ		$8b4		*! Serial Interface Mode Register


********************
* Configuration Register Bases from 68302 base
*
S1CON	equ		$880
S2CON	equ		$890
S3CON	equ		$8a0

********************
* Configuration Register Offsets from above values
*
SCON	equ		$02
SCM		equ		$04
DSR		equ		$06
SCCE	equ		$08
SCCM	equ		$0a
SCCS	equ		$0c

********************
* Bus Error Channel Number Values
*	Note: When these occur, the 68302 Communications Processor must be
* reset (via h/w or RST bit in command register).
*
SCC1TX	equ		0
SCC1RX	equ		1
SCC2TX	equ		2
SCC2RX	equ 	3
SCC3TX	equ 	4
SCC3RX	equ 	5

********************
* Timer Register Offsets
TMR		equ		0
TRR		equ		2
TCR		equ		4
TCN		equ		6
TER		equ		9

********************
* Timer Mode Register Values (TMR1 & TMR2)
*
TRST	equ		1<<0			*! Reset
TFRR	equ		1<<3			*! Restart timer after ref. value reached
TORI	equ		1<<4			*! Output reference interrupt enable
TOM		equ		1<<5			*! Toggle Output

* Other values for the Timer Mode Register

* Timer Input Source
TISTOP	equ		0<<1			*! Stop count
TIMSTR	equ		1<<1			*! Master Clock
TIM16	equ		2<<1			*! Master Clock divided by 16
TITIN	equ		3<<1			*! TIN pin (falling edge)

* Capture edge and interrupt enable
TCEDIS	equ		0<<6			*! Disable interrupt on capture event
TCERIS	equ		1<<6			*! Capture on rising edge & enable interrupt
TCEFAL	equ		2<<6			*! Capture on falling edge & enable interrupt
TCEANY	equ		3<<6			*! Capture on any edge & enable interrupt

********************
* SCC Command Register Bit Definitions
*
FLG		equ		1<<0			*! Must be set when issuing a command to CR
S1		equ		0<<1			*! Command is for SCC1
S2		equ		1<<1			*! Command is for SCC2
S3		equ		2<<1			*! Command is for SCC3
GCI		equ		1<<6			*! Allow GCI commands
RST		equ		1<<7

* GCI Mode commands
GCITAR	equ		0<<4			*! Transmit Abort Request
GCITO	equ		1<<4			*! Timeout Command

* Other commands
STOPTX	equ		0<<4			*! Stop Transmit
RESTTX	equ		1<<4			*! Restart Transmit
ENTHUNT	equ		2<<4			*! Enter Hunt Mode
RESTRX	equ		3<<4			*! Reset Receiver BCS Generator (BISYNC only)

********************
* SCC Configuration Register Bit Definitions
*
DIV4	equ		1<<0		*! Select divide-by-4 prescaler for clock divider
RCLK	equ		1<<12		*! Rx Clock is RCLK pin
TCLK	equ		1<<13		*! Tx Clock is TCLK pin
EXTC	equ		1<<14		*! Baud Rate generator is external source
WOMS	equ		1<<15		*! TXD driver operates in Wired-Or-Select Mode

 ifeq XTALFREQ-15360000
* Baud Rate Constants for a 15.36 MHz XTAL Frequency
B150	equ (1599<<1)+DIV4
B300	equ	(799<<1)+DIV4
B600	equ 1599<<1
B1200	equ 799<<1
B2400	equ 399<<1
B4800	equ 199<<1
B9600	equ 99<<1
B19200	equ 49<<1
B38400	equ 24<<1
 else
   ifeq XTALFREQ-16000000
* Baud Rate Constants for a 16.00 MHz XTAL Frequency
B150	equ (1666<<1)+DIV4
B300	equ	(832<<1)+DIV4
B600	equ 1666<<1
B1200	equ 832<<1
B2400	equ 416<<1
B4800	equ 207<<1
B9600	equ 103<<1
B19200	equ 51<<1
B38400	equ 25<<1
   else
     ifeq XTALFREQ-16670000
* Baud Rate Constants for a 16.67 MHz XTAL Frequency
B150	equ (1735<<1)+DIV4
B300	equ	(867<<1)+DIV4
B600	equ 1735<<1
B1200	equ 867<<1
B2400	equ 433<<1
B4800	equ 216<<1
B9600	equ 108<<1
B19200	equ 53<<1
B38400	equ 26<<1
	else
      ifeq XTALFREQ-20000000
* Baud Rate Constants for a 20 MHz XTAL Frequency
B150    equ (2082<<1)+DIV4
B300    equ (1041<<1)+DIV4
B600    equ 2082<<1
B1200   equ 1041<<1
B2400   equ 520<<1
B4800   equ 259<<1
B9600   equ 129<<1  
B19200  equ 64<<1
B38400  equ 31<<1
        endc XTALFREQ-20000000
     endc XTALFREQ-16670000
   endc XTALFREQ-16000000
 endc XTALFREQ-15360000


********************
* SCC Mode Register Bit Definitions
*
* Common SCC Mode Bits
ASYNC	equ		1<<0		*! Asynchronous operation only
ENT		equ		1<<2		*! Enable Transmitter
ENR		equ		1<<3		*! Enable Receiver
D_NORM	equ		0<<4		*! Normal diagnostic mode
D_LOOP	equ		1<<4		*! Loopback mode
D_ECHO	equ		2<<4		*! Auto-echo mode
D_SOFT	equ		3<<4		*! RTS/CTS under software control
*
* ASYNC specific mode bits
* The modes below are set when the bit is set in the register
*
STOP2	equ		1<<5		*! Two stop bits
CL8		equ		1<<8		*! 8 bits/char
PEN		equ		1<<12		*! Parity enable
R_EVEN	equ		1<<13		*! Receiver Even Parity
T_LOW	equ		1<<14		*! Transmitter Always sends 0 as parity bit
T_EVEN	equ		1<<15		*! Transmitter Even Parity
T_HIGH	equ		3<<14		*! Transmitter Always sends 1 as parity bit

********************
* Buffer Descriptor Offsets
*
Status	equ		0			*! Command/Status Word
DataLen	equ		2			*! Data length in Octets
Buffer	equ		4			*! Ptr to data buffer

********************
* SCC Receive Buffer Descriptor Command/Status Word Bit Definitions
*
RXCD	equ		1<<0		*! Carrier Detect Lost
RXOV	equ		1<<1		*! Overrun error
RXPR	equ		1<<3		*! Parity Error
RXFR	equ		1<<4		*! Framing Error
RXBR	equ		1<<5		*! Break Received
RXID	equ		1<<8		*! Buffer Closed on reception of IDLES
RXM		equ		1<<9		*! Address byte matches UADDR1
RXA		equ		1<<10		*! Buffer contains and address byte(s)
RXC		equ		1<<11		*! Buffer contains a control character
RXI		equ		1<<12		*! Generate interrupt when this buffer filled
RXW		equ		1<<13		*! This is the final RX buffer in the table
RXX		equ		1<<14		*! The RX buffer is in external memory
RXE		equ		1<<15		*! The RX Buffer is empty

********************
* SCC Receive Buffer Descriptor Command/Status Word Bit Definitions
*
TXCT	equ		1<<0		*! CTS Lost
TXP		equ		1<<9		*! Preamble sent before data
TXA		equ		1<<10		*! Buffer contains address byte(s)
TXCR	equ		1<<11		*! CT bit error reporting + 2 bits of idle
TXI		equ		1<<12		*! Generate interrupt when data sent
TXW		equ		1<<13		*! The is the final TX Buffer in the table
TXX		equ		1<<14		*! The TX Buffer is in external memory
TXR		equ		1<<15		*! The TX buffer is ready for transmission

********************
* SCC Event Register (SCCE) and SCC Mask Register (SCCM) Bit Definitions
*
b_RXEV	equ		0
b_TXEV	equ		1
b_SYEV	equ		2
b_CCREV	equ		3
b_BRKEV	equ		4
b_IDLEV	equ		5
b_CDEV	equ		6
b_CTSEV	equ		7

RXEV	equ		1<<b_RXEV	*! A buffer has been received
TXEV	equ		1<<b_TXEV	*! A buffer has been transmitted
BSYEV	equ		1<<b_SYEV	*! Character discarded due to lack of buffers
CCREV	equ		1<<b_CCREV	*! Control character received
BRKEV	equ		1<<b_BRKEV	*! Break character received
IDLEV	equ		1<<b_IDLEV	*! IDLE sequence status changed
CDEV	equ		1<<b_CDEV	*! Carrier Detect Status changed
CTSEV	equ		1<<b_CTSEV	*! Clear-To-Send status changed

********************
* SCC Interrupt Mask Register (IMR) and Interrupt Pending Register (IPR)
* Bit Definitions
*
b_ERR		equ		0
b_PB8		equ		1
b_SMC2		equ		2
b_SMC1		equ		3
b_TIMER3	equ		4
b_SCP		equ		5
b_TIMER2	equ		6
b_PB9		equ		7
b_SCC3		equ		8
b_TIMER1	equ		9
b_SCC2		equ		10
b_IDMA		equ		11
b_SDMA		equ		12
b_SCC1		equ		13
b_PB10		equ		14
b_PB11		equ		15

********************
* SCC Global Interrupt Mask Register (GIMR) Bit Definitions
*
IRQ1EDGE equ	1<<8		*! IRQ 1 edge triggered
IRQ6EDGE equ	1<<9		*! IRQ 6 edge triggered
IRQ7EDGE equ	1<<10		*! IRQ 7 edge triggered
IV1EXT	 equ	1<<12		*! External vector for level 1 interrupt
IV6EXT	 equ	1<<13		*! External vector for level 6 interrupt
IV7EXT	 equ	1<<14		*! External vector for level 7 interrupt
DED		 equ	1<<15		*! Dedicated Operational Mode

********************
* Interrupt Vector Offsets
*	- These are the low 5 bits of the vector generated
VERROR	equ		%00000
VPB8	equ		%00001		*! General Purpose Interrupt 0 (PB8)
VSMC2	equ		%00010		*! SMC2
VSMC1	equ		%00011		*! SMC1
VTIMER3	equ		%00100		*! Timer 3
VSCP	equ		%00101		*! SCP
VTIMER2	equ		%00110		*! Timer 2
VPB9	equ		%00111		*! General Purpose Interrupt 0 (PB9)
VSCC3	equ		%01000		*! SCC3
VTIMER1	equ		%01001		*! Timer 1
VSCC2	equ		%01010		*! SCC2
VIDMA	equ		%01011		*! IDMA Channel
VSDMA	equ		%01100		*! SDMA Channels Bus Error
VSCC1	equ		%01101		*! SCC1
VPB10	equ		%01110		*! General Purpose Interrupt 0 (PB10)
VPB11	equ		%01111		*! General Purpose Interrupt 0 (PB11)
VEXT1	equ		%10001		*! External Device
VEXT6	equ		%10110		*! External Device
VEXT7	equ		%10111		*! External Device

* end of "m68302.d"

 opt l
