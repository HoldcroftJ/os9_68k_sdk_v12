 ttl 68070 DMA device defs
 opt -l
 page
*************************************
*
* DMA 68070 device definitions for the DMAC integerated into the SCC68070
*
*    Note:  All the register offsets (except general control register)
*           must have the channel number appended.  The channel number
*           times 64 plus the register offset given here will reach
*           the correct register.
*
********************************
* Hardware register declarations
*
* NOTE: These defs are for Channel 1 of the DMAC only!!!
*
DMABase equ $80004000  

StatusRg equ 0 channel status register (byte)
ErrorReg equ 1 channel error register (byte)
DevCtlRg equ 4 device control register (byte)
OpsCtlRg equ 5 operation control register (byte)
SeqCtlRg equ 6 sequence control register (byte)
ChlCtlRg equ 7 channel control register (byte)
MemTfrCt equ $A memory transfer counter (word)
MemAdrRg equ $C memory address register (long)
DevAdrRg equ $14 device address register (long)
ChlPriRg equ $2D channel priority register (byte)

MemFunCd equ $29 memory function codes (byte)
DevFunCd equ $31 device function codes (byte)

ChnlSize equ $40 size of one channel register set


*
* device control register (R/W) *** Channel 1 ONLY ***
*
BurstMod equ $00 burst transfer mode
StealMod equ $80 cycle steal mode without hold

DevAckRy equ $30 device with *ACK and *READY, implicitly addressed

Dev16Bit equ $08 16 bit device
Dev8Bit equ $00 8 bit device


*
* Operation control register (R/W) *** Channel 1 ONLY ***
*
MemToDev equ $00 transfer from memory to device
DevToMem equ $80 transfer from device to memory
XfrDir_B equ 7 transfer direction bit number

ByteSize equ $00 operation size = byte
WordSize equ $10 operation size = word

ReqInit equ 2 

*
* Sequence Control Register (R/W) *** Channel 1 ONLY ***
*
MemNoCnt equ 0 memory address register does not count
MemCntUp equ 4 memory address register counts up

DevCntUp equ 1 *** Channel 2 only ****
*
* Channel Control Register (R/W)  *** Channel 1 ONLY ***
*
NoOpPend equ $00 no operation is pending
StartOp  equ $80 start operation
Start_B  equ 7 bit number of start operation bit

NoAbort  equ $00 operation not aborted
OpAbort  equ $10 operation aborted
Abort_B  equ 4 bit number of abort op bit

IntrptDi equ 0 interrupts disabled
IntrptEn equ 8 interrupts enabled
Intrpt_B equ 3 bit number of interrupt enable

NoIntr equ 0
IntLev1 equ 1
IntLev2 equ 2
IntLev3 equ 3
IntLev4 equ 4
IntLev5 equ 5
IntLev6 equ 6
IntLev7 equ 7

 page
*
* Channel Status Register (R/W)  *** Channel 1 ONLY ***
*
*     writing a one into any bit clears that status
*     any written zero bits do not affect the status
*
OpNoComp equ $00 operation incomplete
OperComp equ $80 operation complete
OpComp_B equ 7 bit number of operation complete bit

DevTrmAb equ $00 device termination abnormal
DevTrmNo equ $20 device termination normal
DevTrm_B equ 5 bit number of device termination status

ErrorSet equ $10 error occurred and is noted in CER
Error_B  equ 4 bit number of error flag bit

ActiveCh equ 8 channel considered active
Active_B equ 3 bit number of active channel flag bit


*
* Channel Error Register (R only) *** Channel 1 ONLY ***
*
ErOpTimg equ $02 operation timing error
ErBusMem equ $09 bus error
ErSAbort equ $11 software abort

*
* Channel Priority Register (R/W)  *** This register is really a NOP ***
*
ChPrior0 equ 0 channel priority of zero
ChPrior1 equ 1 channel priority of one
ChPrior2 equ 2 channel priority of two
ChPrior3 equ 3 channel priority of three


* end of file
 opt l
