 opt -l
 nam qsm.d
 ttl Definitions for the mc68332 Queued Serial Module

**********************************************************************
* Definitions for the mc68332 QSM (Queued Serial Module)
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  89/10/11  history starts.                                  Rwb
*               ---- OS-9/68K V2.4 Release ----
*               ---- OS-9 V2.4 Port Pak Release #2 ----
*               ---- OS-9/68k V3.0 Release ----
*

****  Note that these defs are set up so that the they can be used
****   with the file mc683xx.d. 

 ifndef ShortIO
ShortIO equ $FFF000  The base address of Short I/O space.
 endc

 ifndef AlterIO
AlterIO equ $7FF000  The alternate location for systems module
 endc

QSMBase equ $C00  Offset of QSM within the System module map

 ifdef REMAPMCR
QSMMap equ AlterIO+QSMBase Places module controls here after sysinit MCR setup
 else
QSMMap equ ShortIO+QSMBase default location of the QSM
 endc


*******  These are the registers that are associated with QSM.  They are
*******      defined as offsets from QSMMap.

QMCR    equ $00 Configuration register 
QTEST   equ $02 QSM Test Register
QILR    equ $04 QSM Interrupt level register
QIVR    equ $05 Interrupt vector register

********* Asynchronous Serial Port **********
SCCR0   equ $08 Baud Rate               ---\
SCCR1   equ $0A Port Control Register      |
SCSR    equ $0C Status Register            |--  UART  ( /TERM  )
SCDR    equ $0E Data Register           ---/

QPDR    equ $15 Port data register
QPAR    equ $16 Pin assignment register
QDDR    equ $17 Data direction register

******** High Speed Synchronus Port ********
SPCR0   equ $18 Control Register 0
SPCR1   equ $1A Control Register 1
SPCR2   equ $1C Control Register 2
SPCR3   equ $1E Control Register 3
SPSR    equ $1f Status Register

*******  This is the Queue RAM ************
QSPIRD  equ $100 QSPI Receive Data
QSPITD  equ $120 QSPI Transmit Data
QSPICC  equ $140 QSPI command/control register

*######################################################################*
************  These are individual field masks and bits for the  *******
************    registers that are defined above.                *******
*######################################################################*

********** SCI - Serial Communications Interface -- UART ************
**  These registers make up the serial interface for the TERM device.

*********** QMCR ********************
SIMSTOP equ $8000 QSM Stop Enable
SIMFRZ1 equ $4000 Freeze1 - determines action when FREEZE is asserted.
SIMSUPV equ $0080 Supervisor/Unrestricted (1 = supervisor state only access)
SIMIARB equ $0006 Interrupt Arbitration Identification Number

*********** QILR **************
SCILvl  equ $07 mask for the SCI level bits
QSPILvl equ $38 mask for the QSPI level bits

b_SCILvl  equ 0  bit position that devines the level bits for the SCI
b_QSPILvl equ $3 bit position that defines level bits within the register

*********** QIVR ************************
*   This register defines a block       *
*     of 2 vectors that will be used    *
*     for both the QSPI and SCI         *
*     modules.                          *
*****************************************

*********** QPDR  *******************
*  Port Data Register - writes to this byte
*   register affect the value of the output pins.
*   All pins of the the QSM port can be programmed as
*   general purpose outputs.  Read of this register
*   returns the actual value of the pins.
*************************************

*********** QPAR  *******************
*  Selects between QSPI submodule and General Purpose I/O control
QSMPCS  equ $70 Peripheral Chip Selects 3-0
QPCSS0  equ $08 Peripheral Chip Select 0/Slave Select
QSMMOSI equ $02 Master Out Slave In
QSMMISO equ $01 Master In Slave Out

*********** QDDR  *******************
*  Selects data direction of Pins 
QSMTXD  equ $80 Transmit Data - controls sense of TXD line if SCI disabled
QSMPSC  equ $70 Peripheral Chip Selects 3-0
QSPSS0  equ $08 Peripheral Chip Select 0/Slave Select
QSDMOSI equ $02 Master Out Slave In
QSDMOSO equ $01 Master In Slave Out

*********** SPCR0  ******************
MSTR   equ $8000 Master/Slave Mode Select ( 1 = Master, 0 = Slave )
WOMQ   equ $4000 Wired-OR Mode for QSPI Pins ( 1 = open-drain outputs )
BITS   equ $3C00 Bits Per Transfer (4 bits:  8 + X = bits transfered )
CPOL   equ $0200 Clock Polarity - used to sync master/slave clocks
CPHA   equ $0100 Clock Phase
SPBAUD equ $00FF Baud Rate

*********** SPCR1  ******************
SPE   equ $8000  QSPI enable  ( 1 = start QSPI )
DSCKL equ $7F00  Delay before SCK
DTL   equ $00FF  Delay after transfer

*********** SPCR2  ******************
SPIFIE   equ $8000 SPI finished Interrupt Enable
SPIWREN  equ $4000 Wrap Enable (1 = wrap-around mode, 0 = no wrap )
SPIWRT0  equ $2000 Wrap To
SPENDQP  equ $0F00 Ending Queue Pointer
SPNEWQP  equ $000F New Queue Pointer Value

*********** SPCR1  ******************
TLOOPQ  equ $04  QSPI Loop Mode
HMIE    equ $02  HALTA and MODF Interrupt Enable
SPHALT  equ $01  Halt response:  1 = halt on queue boundry.

*********** SPSR ********************
SPIF   equ $80 QSPI Finished Flag
MODF   equ $40 Mode Fault Flag  
HALTA  equ $20 Halt Acknowledge Flag
CTPQP  equ $0F Completed Queue Pointer

*********** SCCR0 *******************
SCCBR equ $1fff Baud Rate value.  (13 bits)

*********** SCCR1 *******************
*   Bit positions:
LOOP  equ 14 Loop Mode Test ( 1 = looping, feedback path enabled )
WOMS  equ 13 Wired-OR mode for SCI Pins ( 1 = configured as output )
ILT   equ 12 Idle-Line detect type
SCPT  equ 11 Parity Type ( 1 = Odd, 0 = Even )
SCPE  equ 10 Parity Enable ( 1 = Enable, 0 = Disable )
SCM   equ 9  Mode select ( 0 = 1/8/1,  1 = 1/9/1:  start/data/stop )
WAKE  equ 8  Wake-Up by address (1)  or Idle line (0)
TIE   equ 7  Transmit Interrupt Enable  ( 1= interrupt on TDRE )
TCIE  equ 6  Transmit Complete Interrupt Enable  ( 1 = interrupt on TC)
RIE   equ 5  Reciever Interrupt Enable ( 1 = interrupt on RDRF )
ILIE  equ 4  Idle-Line Interrupt Enable ( 1 = interrupt on IDLE set )
TE    equ 3  Transmitter Enable ( 1 = TXD is SCI transmitter )
RE    equ 2  Receiver Enable ( 1 = SCI receiver enable )
RWU   equ 1  Receiver Wake-Up  ( 0 = normal operation )
SBK   equ 0  Send Break ( 1 = Send Break frame )

*********** SCSR ********************
*   Bit positions:
TDRE  equ 8 Transmit Data Register Empty Flag
b_TDRE equ 0 ( if used as byte portion of the port
SCTC  equ 7 Transmit Complete Flag
RDRF  equ 6 Receive Data Register Full Flag
RAF   equ 5 Receiver Active Flag
IDLE  equ 4 Idle-Line Detected Flag
SCOR  equ 3 Overrun Error Flag
SCNF  equ 2 Noise Error Flag
SCFE  equ 1 Framing Error Flag
SCPF  equ 0 Parity Error Flag

SCI_errmsk equ ((1<<SCPF)+(1<<SCFE)+(1<<SCNF)+(1<<SCOR))  Error Bits.

*********** SCDR ***********************************
**  This register is 9 bits long ( bit 0-8 )
**    In 8 bit mode, bit 9 has no meaning.
**
**   Read = reciever data   Write = transmit data
****************************************************
 opt l
