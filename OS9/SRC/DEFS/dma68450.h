 ttl 68450 DMA device defs
 page
*****************************************************************************
*                                                                           *
*              Copyright 1998 by Microware Systems Corporation              *
*                            All Rights Reserved                            *
*                          Reproduced Under License                         *
*                                                                           *
*  This software is confidential property of Microware Systems Corporation, *
*  and is provided under license for internal development purposes only.    *
*  Reproduction, publication, modification, distribution, or creation of    *
*  derivative works in any form to any party other than the Licensee is     *
*  strictly prohibited, unless expressly provided for under the terms of a  *
*  mutually executed written license agreement for this software between    *
*  the end-user and Microware Systems Corporation, or otherwise expressly   *
*  authorized in writing by Microware Systems Corporation.  Licensee shall  *
*  reproduce this copyright notice exactly as provided herein on any copies *
*  of the software that Licensee is authorized to distribute.               *
*                                                                           *
*****************************************************************************
*                                                                           *
* Edition History:                                                          *
* #   Date     Comments                                                By   *
* --- -------- ------------------------------------------------------- ---- *
*  01 98/07/29 Added edition history.                                  wwb  *
*				---- OS-9/68K V3.1 Release ----								*
*             $$                    <RELEASE_INFO>                     $$   *
*****************************************************************************


*************************************
*
* DMA 68450 device definitions
*
*    Note:  All the register offsets (except general control register)
*           must have the channel number appended.  The channel number
*           times 64 plus the register offset given here will reach
*           the correct register.
*

StatusRg equ 0 channel status register (byte)
ErrorReg equ 1 channel error register (byte)
DevCtlRg equ 4 device control register (byte)
OpsCtlRg equ 5 operation control register (byte)
SeqCtlRg equ 6 sequence control register (byte)
ChlCtlRg equ 7 channel control register (byte)
MemTfrCt equ $A memory transfer counter (word)
MemAdrRg equ $C memory address register (long)
DevAdrRg equ $14 device address register (long)
BasTfrCt equ $1A base transfer register (word)
BasAdrRg equ $1C base address register (long)

NmIntVct equ $25 normal interrupt vector (byte)
ErIntVct equ $27 error interrupt vector (byte)
ChlPriRg equ $2D channel priority register (byte)

MemFunCd equ $29 memory function codes (byte)
DevFunCd equ $31 device function codes (byte)
BasFunCd equ $39 base function codes (byte)

ChnlSize equ $40 size of one channel register set

GenCtlRg equ $FF general control register (byte)


*
* device control register (R/W)
*

BurstMod equ $00 burst transfer mode
StealMod equ $80 cycle steal mode without hold
StealHld equ $C0 cycle steal mode with hold

Dev68000 equ $00 68000 compatible device, explicitly addressed
Devc6800 equ $10 6800 compatible device, explicitly addressed
DevAck   equ $20 device with *ACK, implicitly addressed
DevAckRy equ $30 device with *ACK and *READY, implicitly addressed

Dev8Bit  equ $00 device port 8 bit
Dev16Bit equ $80 device port 16 bit
DevSiz_B equ 3 bit number of device port size

StatInp  equ 0 status input - peripheral ctl line
StatInpI equ 1 status input with interrupt
StartPls equ 2 start pulse, negative 1/8 clk
AbortInp equ 3 abort input

 page
*
* Operation control register (R/W)
*

MemToDev equ $00 transfer from memory to device
DevToMem equ $80 transfer from device to memory
XfrDir_B equ 7 transfer direction bit number

ByteSize equ $00 operation size = byte
WordSize equ $10 operation size = word
LongSize equ $20 operation size = long

ChainDis equ $0 chain operation disabled
ChainArr equ $8 array chaining enabled
ChainLnk equ $C linked chaining enabled

AuReqLim equ 0 auto request at rate set by GCR
AuReqMax equ 1 auto request at maximum rate
ReqInit  equ 2 *REQ line intitiates all operand transfers
ReqInitA equ 3 auto request first xfr, *REQ for all others


*
* Sequence Control Register (R/W)
*

MemNoCnt equ 0 memory address register does not count
MemCntUp equ 4 memory address register counts up
MemCntDn equ 8 memory address register counts down

DevNoCnt equ 0 device address register does not count
DevCntUp equ 1 device address register counts up
DevCntDn equ 2 device address register counts down


*
* Channel Control Register (R/W)
*

NoOpPend equ $00 no operation is pending
StartOp  equ $80 start operation
Start_B  equ 7 bit number of start operation bit

NoContin equ $00 no continue operation is pending
ContinOp equ $40 continue operation
Contin_B equ 6 bit number of continue op bit

OpNoHalt equ $00 operation not halted
OpHalted equ $20 operation halted
Halted_B equ 5 bit number of halted op bit

NoAbort  equ $00 operation not aborted
OpAbort  equ $10 operation aborted
Abort_B  equ 4 bit number of abort op bit

IntrptDi equ 0 interrupts disabled
IntrptEn equ 8 interrupts enabled
Intrpt_B equ 3 bit number of interrupt enable

 page
*
* Channel Status Register (R/W)
*
*     writing a one into any bit clears that status
*     any written zero bits do not affect the status
*

OpNoComp equ $00 operation incomplete
OperComp equ $80 operation complete
OpComp_B equ 7 bit number of operation complete bit

BlkNoCmp equ $00 block transfer incomplete
BlkComp  equ $40 block transfer complete
BlkCmp_B equ 6 bit number of block transfer complete bit

DevTrmAb equ $00 device termination abnormal
DevTrmNo equ $20 device termination normal
DevTrm_B equ 5 bit number of device termination status

ErrorSet equ $10 error occurred and is noted in CER
Error_B  equ 4 bit number of error flag bit

ActiveCh equ 8 channel considered active
Active_B equ 3 bit number of active channel flag bit

PCLTrans equ 2 transition occurred on *PCL
PCLTrn_B equ 1 bit number of PCL transition flag bit

PCLLow   equ 0 *PCL line low
PCLHigh  equ 1 *PCL line high
PCLSts_B equ 0 bit number of *PCL status bit


*
* Channel Error Register (R only)
*

ErConfig equ $01 configuration error
ErOpTimg equ $02 operation timing error

ErAdrMem equ $05 memory address error
ErAdrDev equ $06 device address error
ErAdrBas equ $07 base address error

ErBusMem equ $09 memory bus error
ErBusDev equ $0A device bus error
ErBusBas equ $0B base bus error

ErCntMem equ $0D memory count error
ErCntDev equ $0E device count error
ErCntBas equ $0F base count error

ErEAbort equ $10 external abort
ErSAbort equ $11 software abort

 page
*
* Channel Priority Register (R/W)
*

ChPrior0 equ 0 channel priority of zero
ChPrior1 equ 1 channel priority of one
ChPrior2 equ 2 channel priority of two
ChPrior3 equ 3 channel priority of three


*
* Function Code Registers (R/W)
*

UserData equ 1 user data address access
UserProg equ 2 user program address access
SupvData equ 5 supervisor data address access
SupvProg equ 6 supervisor program address access



*
* General Control Register (R/W)
*

BurstTim equ $C mask for burst time
BandwRat equ $3 mask for bandwidth ratio


* end of file

