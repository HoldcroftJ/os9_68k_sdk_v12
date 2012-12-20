 opt -l
 nam uartm328.d
 ttl - MC68328 Universal Asynchronous Receiver/Transmitter Module

**********************************************************************
* Copyright 1995, 1996 by Microware Systems Corporation              *
* Reproduced Under License                                           *
*                                                                    *
* This source code is the proprietary confidential property of       *
* Microware Systems Corporation, and is provided to licensee         *
* solely for documentation and educational purposes. Reproduction,   *
* publication, or distribution in any form to any party other than   *
* the licensee is strictly prohibited.                               *
**********************************************************************
* Definitions for the MC68328 UART Module
*
* Edition History:
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  95/08/31  history starts.                                  rws
*               --- OS-9/68K MC68328 Support Beta2 Release ---
**********************************************************************

 ifndef ModuleIO
 ifndef StdAddr
ModuleIO equ $FFFFF000  Base address of chip's internal regs (32 bits)
 else
ModuleIO equ $FFF000    Base address of chip's internal regs (24 bits)
 endc
 endc

SERBase equ $900        Offset of SER with System module map
SERMap equ ModuleIO+SERBase Default location of the SERIAL module

*
* All registers are 16 bits and are referenced from SERMap.
*

USTCNT equ $0 UART Status/Control Register
UBAUD  equ $2 UART Baud Control Register
URX    equ $4 UART RX Register
UTX    equ $6 UART TX Register
UMISC  equ $8 UART Misc Register


*****  These are separate masks within the various registers ********

************  USTCNT (UART Status/Control Register)  ****************
* Initialized to $0000 at reset.
* This register controls the overall UART operation.
*
UART_ENBL   equ $8000 UART Enable, 1=enabled 0=disabled
RX_ENBL     equ $4000 Reciever Enable, 1=receiver enabled 0=disabled
TX_ENBL     equ $2000 Transmitter Enable, 1=transmitter enabled 0=disabled

RX_CLK_CONT equ $1000 Receiver Operating Mode Control,
*                     1=1x mode 0=16x mode
PARITY_ENBL equ $0800 1=parity enabled 0=parity disabled
ODD_EVEN    equ $0400 1=odd parity 0=even parity
STOP_BITS   equ $0200 1=2stop bits 0=1stop bit
DATA_BITS   equ $0100 1=8 data bits 0=7 data bits

GPIO_IRQ_ENBL equ $0080 1=GPIO interrupt enabled 0=disabled
CTS_IRQ_ENBL  equ $0040 1=CTS interrupt enabled 0=disabled

RX_FULL_ENBL  equ $0020 1=RX FULL interrupt enabled 0=disabled
RX_HALF_ENBL  equ $0010 1=RX HALF FULL interrupt enabled 0=disabled
RX_RDY_ENBL   equ $0008 1=RX interrupt enabled 0=disabled

TX_EMPTY_ENBL equ $0004 1=TX EMTPY interrupt enabled 0=disabled
TX_HALF_ENBL  equ $0002 1=TX HALF EMPTY interrupt enabled 0=disabled
TX_AVAIL_ENBL equ $0001 1=TX AVAILABLE interrupt enabled 0=disabled


**************  UBAUD (UART Baud Control Register)  ******************
* Initialized to $003F at reset.
* This register controls the operation of the baud-rate generator and 
* the GPIO pin.
*
* 16x clock = clock source / [(65-PRESCALER) x 2^DIVIDE]
*
GPIO_DELTA equ $8000 1=GPIO pin changed state, clear by writing 0 to bit
GPIO_PIN   equ $4000 Current value of GPIO pin
GPIO_DIR   equ $2000 GPIO pin direction, 1=output 0=input
GPIO_SRC   equ $1000 GPIO Source control when GPIO_DIR=0 (input), 
*                    1=GPIO driven by bit clock from baud generator
*                    0=GPIO driven by GPIO bit

BAUD_SRC   equ $0800 Clock Source to baus-rate generator control,
*                    1=source is GPIO (GPIO_DIR must be 0 [input])
*                    0=source is system clock
b_DIVIDE   equ 8 Clock frequcy divide control, 3 bits:
*                000=divide by 1
*                001=divide by 2
*                010=divide by 4
*                011=divide by 8
*                100=divide by 16
*                101=divide by 32
*                110=divide by 64
*                111=divide by 128
* bits7:6 unused
b_BPRESCALER equ 0 Baud-rate generator prescaler divide value control, 6 bits:
*                  prescaler divide value = 65 (decimal) - PRESCALER


************  URX (UART Receiver Register)  ****************
* Initialized to $0000 at reset.
*
RX_FIFO_FULL equ $8000 read-only, 1=RX FIFO Full (interrupt posted) 0=not
RX_FIFO_HALF equ $4000 read-only, 1=RX FIFO > Half full 0=not
RX_DATA_RDY  equ $2000 read-only, 1=data in recieve FIFO 0=not
* bit 12 always 0
RX_OVRUN     equ $0800 read-only, 1=FIFO overrun detected 0=not
RX_FRAME_ERR equ $0400 read-only, 1=character has framing error 0=not
RX_BREAK     equ $0200 read-only, 1=character is a break character
RX_PARITY_ERR equ $0100 read-only, 1=current char has parity error
b_RX_DATA equ 0 read-only, 8 bits, contains next receive character in FIFO


************  UTX (UART Transmitter Register)  ****************
* Initialized to $0000 at reset.
*
TX_FIFO_EMPTY equ $8000 read-only, 1=transmitter FIFO empty
TX_FIFO_HALF  equ $4000 read-only, 1=transmitter FIFO < half full
TX_AVAIL      equ $2000 1=transmitter needs data
TX_SEND_BRERAK equ $1000 forces transmitter to send a break char when 1
TX_IGNORE_CTS equ $0800 ignores CTS pin when 1
* bit 10 always 0
TX_CTS_STATUS equ $02000 current status of CTS pin
TX_CTS_DELTA  equ $01000 CTS change of state indicator, 1=state changed
b_TX_DATA     equ 0 data to be transmitted, read as all 0 though


************  UMISC (UART Miscellaneous Register)  ****************
* Initialized to $0000 at reset.                                                
*
* bit 15 reserved
CLK_SRC    equ $4000 1x bit clock source control, 1=GPIO pin 0=baud generator
FORCE_PERR equ $2000 parity error force if parity is enabled (for
*                    debugging purposes), 1=force errors 0=normal parity
LOOP       equ $1000 loopback control, 1=internally connect transmitter
*                    ouput to receiver input 0=don't
* bits 11:8 reserved
RTS_CONT   equ $0080 RTS control, 1=RTS pin controlled by receiver FIFO 
*                    (when FIFO is full RTS is negated)
*                    0=RTS pin controlled by the RTS bit
RTS_BIT    equ $0040 RTS pin state while RTS_CONT is 0
IRDA_ENBL  equ $0020 IrDA interface enable, 1=IrDA operation 0=NRZ operation
IRDA_LOOP  equ $0010 debugging control:
*                    1=connect IR transmit to IR receiver 0=no IR loop
* bits 3:0 unused




 pag
 opt l
