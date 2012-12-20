 opt -l
 nam dmam.d
 ttl - mc68340 DMA module definitions

**********************************************************************
* Definitions for the mc68340 DMA module
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  90/11/05  history starts.                                  Rwb
*               ---- OS-9 V2.4 Port Pak Release #2 ----
*               ---- OS-9/68k V3.0 Release ----

 ifndef ModuleIO
ModuleIO equ $FFFFF000  The base address of Short I/O space.
 endc

DMABase equ $780            Offset of DMA from system MCR
DMAMap equ ModuleIO+DMABase Default location of the DMA

*  There are two separate autonomus DMA controllers available:
DMA_Chip1 equ DMAMap
DMA_Chip2 equ DMAMap+$20

*  The registers are defined below as relative to the Controller 
*   that is in use.
*
*  The following 4 registers are Supervisor State only access.
DMAMCR   equ  0   DMA module configuration register
DMATEST  equ  2   Test Register (common to both DMA channels)
DMAINTL  equ  4   DMA Interrupt Level
DMAINTV  equ  5   DMA Interrupt Vector

*
*  These are supervisor or user accessable:
DMACCR    equ  $8  Channel control register
DMACSR    equ  $A  Channel Status Register
DMAFCR    equ  $B  Channel Function Code Register
DMASADDR  equ  $C  DMA source address register
DMADADDR  equ  $10 DMA destination address register
DMABTXFR  equ  $14 DMA Byte transfer counter

****************  MCRx  *********************************
*   Module Configuration Register
* 

*     Note:  These are really common bits, they appear in both
*            MCRs but are really the same bits.

b_MCRSTP equ 15  If asserted, will stop the system clock
b_DMAFRZ equ 13   Bit position of the freeze bits

DMA_FRZ0 equ  0  Ignore Freeze
DMA_FRZ1 equ  (1<<b_DMAFRZ)  Reserved
DMA_FRZ2 equ  (2<<b_DMAFRZ)  Freeze on Boundary
DMA_FRZ3 equ  (3<<b_DMAFRZ)  Execution Freeze

*  These  are specific to each DMA module
b_DMASE    equ  12  Single address Enable bit position
b_DMAILM   equ  8   Interrupt Level Mask
b_DMASU    equ  7   Defines controller to be mixed or supervisor only.

*  These next are again shadows in the second MCR.
DMAMAID    equ  $F0  mask for Master Arbitration ID
b_DMAMAID  equ  4    bit position for Mater Arbitration ID

DMAIAID   equ $0F  Mask for Interrupt  Arbitraition ID
b_DMAIAID equ 0    Bit position of Interrupt ARbitration ID

***********  DMAINTL and DMAINTV *********************************
*  These are the interrupt level and vector registers.
*     Supervisor only access, individually programmable for each controller.
*
*   DMAINTL  bits 0-2  define interrupt level 1-7.
*   DMAINTV  bits 0-7  define interrupt vector (power up: $0F)
******************************************************************

************  DMACCR **********************************************
*  Channel Control register.
*
b_DMAINTB  equ 15  Allows generation of an interrupt when BRKPT is encountered
b_DMAINTN  equ 14  Enables interrupt of DMA complete.
b_DMAINTE  equ 13  Enables interrupt on DMA error.
b_DMAECO   equ 12  External Control Option (direction register)
b_DMASAPI  equ 11  Source address Pointer Increment (1, 2 or 4)
b_DMADAPI  equ 10  Destination address pointer increment.
b_DMASSIZE equ 8   (2 bits) Source size register
b_DMADSIZE equ 6   (2 bits) Destination size control.

***  The size values  ******
DMA_longword equ 0  Size is 4 bytes.
*  note: value of 3 is not used !!
DMA_word     equ 2  Size is 2 bytes.
DMA_byte     equ 1  Size is 1 byte.

b_DMAREQG  equ 4  (2 bits) Request generation control

DMA_INTERNAL equ 0  Internal request at programmable rate
DMA_EXTBURST equ 2  External request in burst mode
DMA_EXTCYCS  equ 3  External request in cycle steal mode.

b_DMABB    equ 2  (2 bits) Bus Bandwidth.  % of master bus allowed

DMA_BB25   equ 0  DMA gets 25% of the bus band width
DMA_BB50   equ 1  DMA gets 50%
DMA_BB75   equ 2  DMA gets 75%
DMA_BB100  equ 3  DMA gets 100% or all of the bus

b_DMASDADD  equ 1  Single/Dual Address Transfer
b_DMASTART  equ 0  DMA start bit.

***************  DMACSR *****************************
*  Channel Status Register

b_DMAIRQ  equ  7  Interrupt conditon has occured on this channel (if 1)
b_DMADONE equ  6  DMA done
b_DMABES  equ  5  DMA bus error on source
b_DMABED  equ  4  DMA bus error on destination
b_DMACONF equ  3  DMA configuration error
b_DMABKT  equ  2  DMA to breakpoint address has occured


*************** DMAFCR ******************************
*  DMA function code register
*     << see sim340.d for function code definitions >>
*
b_DMA_SRCFC  equ 4  (7 bits) Function code for DMA Source Address
b_DMA_DSTFC  equ 0  (7 bits) Function code for DMA Destination Address

************* DMASADDR *******************************
*  32 bit Source address for the DMA
******************************************************

************* DMADADDR *******************************
*  32 Bit Destination address for the DMA
******************************************************

************* DMABTXFR *******************************
*  32 Bit Number of bytes to transfer.
******************************************************

 pag
 opt l

