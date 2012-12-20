 opt -l
********************
* Definitions for Motorola 68360 on-chip serial peripherals
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  93/10/21  history starts.                                  arb
*				---- OS-9/68K V3.0.1 Release ----
* 02  99/12/05  Added SMC Definitions                            mgh
* 03  00/03/17	Added b_TXERR and TXERR Definitions              mgh
* 


* Offsets from the base
* 	- Common Parameters for all SCC Modes
RBASE	equ		$0
TBASE	equ		$2
RFCR	equ		$4
TFCR	equ		$5
MRBLR	equ		$6
RBDPTR  equ		$10
TBDPTR  equ		$20

* SCC - UART specific parameter RAM
MAXIDL	equ		$38
IDLC	equ		$3a
BRKCR	equ		$3c
PAREC	equ		$3e
FRMEC	equ		$40
NOSEC	equ		$42
BRKEC	equ		$44
BRKLN	EQU		$46
UADDR1	equ		$48
UADDR2	equ		$4A
RTEMP	EQU		$4C
TOSEQ	EQU		$4E
RCCM	EQU		$60
URCCR	equ		$62
RLBC	EQU		$64

* SMC - UART specific parameter RAM
SMC_MAXIDL  equ     $28
SMC_IDLC    equ     $2a
SMC_BRKLN   EQU     $2c
SMC_BRKEC   equ     $2e
SMC_BRKCR   equ     $30
SMC_Rmask   EQU     $32

*******************
* Control Character Offsets
CHARACTER1 equ	$50
CHARACTER2 equ	$52
CHARACTER3 equ	$54
CHARACTER4 equ	$56
CHARACTER5 equ	$58
CHARACTER6 equ	$5a
CHARACTER7 equ	$5c
CHARACTER8 equ	$5e

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
* SCC Command Register Bit Definitions
*
FLG		equ		1<<0			*! Must be set when issuing a command to CR
GCI		equ		1<<6			*! Allow GCI commands
RST		equ		1<<7

* GCI Mode commands
GCITAR	equ		0<<4			*! Transmit Abort Request
GCITO	equ		1<<4			*! Timeout Command

* Other commands
INITXRX equ 	$000			*! init tx & rx parms
INITRX	equ		$100			*! init rx parms
INITTX	equ		$200			*! init tx parms
STOPTX	equ		$400			*! Stop Transmit
RESTTX	equ		$600			*! Restart Transmit
ENTHUNT	equ		$300			*! Enter Hunt Mode
RESTRX	equ		$a00			*! Reset Receiver BCS Generator (BISYNC only)



********************
* SCC Mode Register Bit Definitions
*
* Common SCC Mode Bits
ASYNC	equ		1<<7		*! Asynchronous operation only
FLC		EQU		1<<15		*! FLOW CONTROL
SL		EQU		1<<14		*! 0 = 1 BIT 1 = 2 BIT
CL5		EQU		0<<12		*! CHAR LENGTH IN BITS
CL6		EQU		1<<12
CL7		EQU		2<<12
CL8		EQU		3<<12
UM		EQU		0<<10		*! NORMAL UART MODE
FRZ		EQU		1<<9		*! FREEZE XMIT
RZS		EQU		1<<8		*! 
*
* ASYNC specific mode bits
* The modes below are set when the bit is set in the register
*
STOP2	equ		1<<14		*! Two stop bits

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
RXAM	equ		1<<7		*! address match
RXID	equ		1<<8		*! Buffer Closed on reception of IDLES
RXCM	equ		1<<9		*! Continous mode
RXA		equ		1<<10		*! Buffer contains and address byte(s)
RXC		equ		1<<11		*! Buffer contains a control character
RXI		equ		1<<12		*! Generate interrupt when this buffer filled
RXW		equ		1<<13		*! This is the final RX buffer in the table
RXE		equ		1<<15		*! The RX Buffer is empty

********************
* SCC Transmit Buffer Descriptor Command/Status Word Bit Definitions
*
TXCT	equ		1<<0		*! CTS Lost
TXNS	equ		1<<7		*! no stop bit transmitted
TXP		equ		1<<8		*! Preamble sent before data
TXCM	equ		1<<9		*! Continous mode
TXA		equ		1<<10		*! Buffer contains address byte(s)
TXCR	equ		1<<11		*! CT bit error reporting + 2 bits of idle
TXI		equ		1<<12		*! Generate interrupt when data sent
TXW		equ		1<<13		*! The is the final TX Buffer in the table
TXR		equ		1<<15		*! The TX buffer is ready for transmission

********************
* SCC Event Register (SCCE) and SCC Mask Register (SCCM) Bit Definitions
*
b_RXEV	equ		0
b_TXEV	equ		1
b_BUSY  equ 	2
b_CCREV	equ 	3
b_TXERR	equ	4
b_BRKEV	equ		5
b_IDLEV	equ		8


RXEV	equ		1<<b_RXEV	*! A buffer has been received
TXEV	equ		1<<b_TXEV	*! A buffer has been transmitted
BSYEV	equ		1<<b_BUSY	*! Character discarded due to lack of buffers
CCREV	equ		1<<b_CCREV	*! Control character received
TXERR	equ		1<<b_TXERR	*! TX Error
BRKEV	equ		1<<b_BRKEV	*! Break character received
IDLEV	equ		1<<b_IDLEV	*! IDLE sequence status changed
ABAUD	equ		1<<9		*! auto Baud

GSMR_L	equ 	$00			*! SCC1 Genenal Mode register
GSMR_H	equ 	$04			*! SCC1 General Mode 
PSMR	equ 	$08			*! SCC1 Protocol-Specific Mode
TODR	equ 	$0c			*! SCC1 Transmit on Demand
DSR		equ 	$0e			*! SCC1 Data Sync 
SCCE	equ		$10			*! SCC1 Event register
SCCM	equ 	$14			*! SCC1 Mask 
SCCS	equ		$17			*! SCC1 Status

SMCMR   equ 	$00 		*! SMC1 Genenal Mode register
SMCE    equ 	$04 		*! SMC1 Event register
SMCM    equ 	$08 		*! SMC1 Mask  
 
 opt l
