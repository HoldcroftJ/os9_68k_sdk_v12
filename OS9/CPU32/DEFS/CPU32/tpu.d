 opt -l
 nam tpu.d
 ttl Definitions for the mc68332 Time Processing Unit

**********************************************************************
* Definitions for the mc68332 TPU (Time Processing Unit)
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

TPUBase equ $E00  Offset of TPU from system MCR

 ifdef REMAPMCR
TPUMap equ AlterIO+TPUBase Places module controls here after sysinit MCR setup
 else
TPUMap equ ShortIO+TPUBase default location of the TPU
 endc


*******  These are the registers that are associated with TPU.  They are
*******      defined as offsets from TPUMap.

TPMCR   equ $00 TPU Module Configuration Register
TPMTR   equ $02 TPU Module Test Register
TPDSCR  equ $04 Development Support Control Register
TPDSSR  equ $06 Development Support Status Register
TPUICR  equ $08 Interrupt Configuration Register
TPUIER  equ $0A Interrupt Enable Register
TPCFSR1 equ $0C Channel Function Select Register 1
TPCFSR2 equ $0E Channel Function Select Register 2
TPCFSR3 equ $10 Channel Function Select Register 3
TPCFSR4 equ $12 Channel Function Select Register 4
TPHSR1  equ $14 Host Sequence Register 1
TPHSR2  equ $16 Host Sequence Register 2
TPHSRQ1 equ $18 Host Service Request Register 1
TPHSRQ2 equ $1A Host Service Request Register 2
TPCP1   equ $1C Channel Priority Register 1
TPCP2   equ $1E Channel Priority Register 2
TPISR   equ $20 Interrupt Status Register
TPLINK  equ $22 Link Register
TPSGL   equ $24 Service Grant Latch Register
TPDCN   equ $26 Decoded Channel Number Register


*######################################################################*
************  These are bit positions within the                 *******
************    registers that are defined above.                *******
*######################################################################*

**********  TPMCR **********
TPSTOP equ 15 Stop Control ( 1 = internal clocks shut down )
TCR1PS equ 13 TCR1 prescaler
TCR2PS equ 11 TCR2 prescaler 
TPEMU  equ 10 Emulation Control (1 = emulation mode, RAM locked from CPU)
TPT2CG equ 9  TCR2 clock/gate control
TPSTF  equ 8  Stop Flag  (0 = not stopped, 1 = stopped )
TPSUPV equ 7  Supervisor Data Space ( 1 = all regs. supervisor state only )
TPIAID equ 0  Interrupt Arbitration ID (priority for internal arbitration)

********** TPUICR **********
TPCIRL  equ 8  Channel Interrupt Request Level (3 bits, levels 1-7)
TPCBV   equ 4 Channel Base Vector (4 bits, creates 16 vector block)

********** TPUIER ****************************
*  Bits 15->0 control Channel 15->0 respectively:
*     0 = pahse interrupts disabled
*     1 = phase interrupts enabled
*********************************************

********** TPISR ****************************
*  Bits 15->0 reflect Channel 15->0 assertion respectively:
*     0 = phase interrupts not asserted
*     1 = phase interrupts asserted
*********************************************

********** TPCFSR1 - TPCFSR4 **********
**  These 4 registers allow function selection for the
**    16 TPU module channels.
**      There are 4 bits per channel in each of
**      the 4 registers as below.

TPCHS15  equ 12 Channel 15 --\
TPCHS14  equ 8  Channel 14   |
TPCHS13  equ 4  Channel 13   |-- TPCFSR1
TPCHS12  equ 0  Channel 12 --/

TPCHS11  equ 12 Channel 11 --\
TPCHS10  equ 8  Channel 10   |
TPCHS09  equ 4  Channel 09   |-- TPCFSR2
TPCHS08  equ 0  Channel 08 --/

TPCHS07  equ 12 Channel 07 --\
TPCHS06  equ 8  Channel 06   |
TPCHS05  equ 4  Channel 05   |-- TPCFSR3
TPCHS04  equ 0  Channel 04 --/

TPCHS03  equ 12 Channel 03 --\
TPCHS02  equ 8  Channel 02   |
TPCHS01  equ 4  Channel 01   |-- TPCFSR4
TPCHS00  equ 0  Channel 00 --/

********** TPHSR1 - TPHSR4 ************
**  These 2 registers allow function selection for the
**    16 TPU module channels.
**      There are 2 bits per channel in each of
**      the 2 registers as below.
TPHC15  equ  14  Channel 15     --\
TPHC14  equ  12      |            |
TPHC13  equ  10      |            |
TPHC12  equ  8       |            |-- TPHSR1
TPHC11  equ  6       |            |
TPHC10  equ  4       |            |
TPHC09  equ  2       |            |
TPHC08  equ  0    Channel 8     --/

TPHC07  equ  14   Channel 7     --\
TPHC06  equ  12      |            |
TPHC05  equ  10      |            |
TPHC04  equ  8       |            |-- TPHSR1
TPHC03  equ  6       |            |
TPHC02  equ  4       |            |
TPHC01  equ  2       |            |
TPHC00  equ  0    Channel 0     --/


********** TPHSRQ1 - TPHSRQ2 ************
**  These 2 registers allow detection of service
**    requests for each of the 16 TPU module channels.
**      There are 2 bits per channel in each of
**      the 2 registers as below:

TPHCRQ15  equ  14  Channel 15     --\
TPHCRQ14  equ  12      |            |
TPHCRQ13  equ  10      |            |
TPHCRQ12  equ  8       |            |-- TPHSR1
TPHCRQ11  equ  6       |            |
TPHCRQ10  equ  4       |            |
TPHCRQ09  equ  2       |            |
TPHCRQ08  equ  0    Channel 8     --/

TPHCRQ07  equ  14   Channel 7     --\
TPHCRQ06  equ  12      |            |
TPHCRQ05  equ  10      |            |
TPHCRQ04  equ  8       |            |-- TPHSR1
TPHCRQ03  equ  6       |            |
TPHCRQ02  equ  4       |            |
TPHCRQ01  equ  2       |            |
TPHCRQ00  equ  0    Channel 0     --/


********** TPCP1 - TPCP2 ************
**  These 2 registers allow allocation of priority
**    for each of the 16 TPU module channels.
**      There are 2 bits per channel in each of
**      the 2 registers as below:

TPRIOR15  equ  14  Channel 15     --\
TPRIOR14  equ  12      |            |
TPRIOR13  equ  10      |            |
TPRIOR12  equ  8       |            |-- TPCP1
TPRIOR11  equ  6       |            |
TPRIOR10  equ  4       |            |
TPRIOR09  equ  2       |            |
TPRIOR08  equ  0    Channel 8     --/

TPRIOR07  equ  14   Channel 7     --\
TPRIOR06  equ  12      |            |
TPRIOR05  equ  10      |            |
TPRIOR04  equ  8       |            |-- TPCP2
TPRIOR03  equ  6       |            |
TPRIOR02  equ  4       |            |
TPRIOR01  equ  2       |            |
TPRIOR00  equ  0    Channel 0     --/

*  prioritys are:
TP_NoPrior  equ $00  channel disabled
TP_LowPrior equ $01  Low
TP_MidPrior equ $02  middle
TP_HigPrior equ $03  high

********** TPMTR **********
INCAD  equ 12 Increment address (0 = normal, 1 = Micro PC increments)
TCR1C  equ 11 Clock Source for TCR1 ( 0 = internal, 1 = TCR2 clocks TCR1)
ACUTR  equ 9  Action taken by assertion of ACUTL ( 2 bits )
SOSEL  equ 4  Scan Out Select ( 3 bits )
SISEL  equ 2  Scan In Select ( 3 bits )
TPTMM  equ 0  Test Memory Map

********** TPDSCR **********
HOT4   equ 15 Halt on T4
BLCT   equ 10 Branch Latch Control
CLKS   equ 9  Stop Clocks
FRZ    equ 7  IMB FREEZE signal Response
UIRT   equ 6  uIRT Control: Microinstruction Register Control
TPBP   equ 5  Break on a uPC match
TPBC   equ 4
TPBH   equ 3  Break if Host Service latch asserted at beginning of phase
TPBL   equ 2  Break if Link Service latch asserted at beginning of phase
TPBM   equ 1  Break if MRL asserted at beginning of phase
TPBT   equ 0  Break if TDL asserted at beginning of phase

********** TPDSSR **********
TPBKPT equ 7  Breakpoint Asserted
TPPCBK equ 6  uPC Breakpoint
TPCHBK equ 5  Channel Register Breakpoint
TPSRBK equ 4  Service Request Breakpoint
TPTPUF equ 3  TPU freeze, asserted when TPU is halted as a result of FREEZE

********** TPLINK **********
*  Bits 15->0 indicate status
*    of link service request on channels
*    15->0 respectively.
****************************

********** TPSGL  **********
*  Bits 15->0 indicate status
*    of service grant for channels
*    15->0 respectively.
****************************

********** TPDCN  **********
*  Bits 15->0 indicate which
*    currently served channel
*    15->0 respectively.
****************************

 opt l
