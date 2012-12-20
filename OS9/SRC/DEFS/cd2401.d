 opt -l
********************
* Definitions for Cirrus Logic CD2401 Serial Interface Chip
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  91/04/26  Created                                          Rwb
* 02  91/09/03  Fixed some minor errors.                         wwb
* 03  91/09/04  Added b_COR1_ignore.                             wwb
* 

*
*  Conventions used in this file:
*
*   b_XXXX  denotes a bit number definition (note access size quoted)
*   m_XXXX  denotes a bit mask (again, note access size quoted)

*
*  This chip features 4 full duplex serial ports, ASYNC or SYNC operation,
*  and an on-chip DMA controller.  BISYNC, HDLC, and X.21 are supported
*  on all channels.  
*
* Due to the Little-Endian/Big-Endian differences between different
*  devices, the few registers that are larger than a byte are define
*  as being two registers.  The definitions below are defined for 
*  use with the Motorola version of the device.

***************
* Global Registers
*  This is a single set of "Global" registers that are available in
*  all modes of operation.

*
* GFRCR -   Global Firmware Revision Code
*       Register must read non-zero before configuing the chip.
GFRCR   equ  $81

*
* TFTC -   Transmit FIFO Transfer Count
*      Number of spaces available in the Transmit FIFO
TFTC    equ  $80

*
* REOIR -  Receive of Interrupt Register
* TEOIR -  Transmit End of Interrupt Register
* MEOIR -  Modem End of Interrupt Register
*    The last access to the chip at the end of an interrupt must be
*    a write to this register.  This will cause the CD2041's internal
*    context stack to be restored.  Depending on the interrupt, a parameter
*    may need to be included in the write.

REOIR   equ  $84
b_REOIR_trmbuf  equ  7 Set 1 terminates current buffer in DMA Mode
b_REOIR_disexc  equ  6 DMA Mode only:  Discard Exception Character.
b_REOIR_settm2  equ  5 Set general timer 2 (Sync) [0=don't, 1= load timer2]
b_REOIR_settm1  equ  4 Set general timer 1 (Sync) [0=don't, 1= load timer1]
b_REOIR_noxfr   equ  3 Must be set 1 if no transfer of data during interrupt
m_REOIR_gap     equ  7 Low 3 bits indicate optional size of gap in DMA buffer


TEOIR   equ  $85
b_TEOIR_trmbuf  equ  7 Set 1 terminates current buffer in DMA Mode
b_TEOIR_eof     equ  6 Set 1 indicates End of Frame in sysc mode
b_TEOIR_settm2  equ  5 Set general timer 2 (Sync) [0=don't, 1= load timer2]
b_TEOIR_settm1  equ  4 Set general timer 1 (Sync) [0=don't, 1= load timer1]
b_TEOIR_noxfr   equ  3 Must be set 1 if no transfer of data during interrupt
 
MEOIR   equ  $86
*  can start up an interrupt generating timer at end of interrupt
b_MEOIR_settm2  equ 5  Set general timer 2 (Sync) [0=don't, 1= load timer2]
b_MEOIR_settm1  equ 4  Set general timer 1 (Sync) [0=don't, 1= load timer1]

*
* Interrupt Status Registers
*
MISR        equ  $8B Modem/Timer Interrupt Status Register
b_MISR_dsr  equ  7   DSR Changed  (Enables for change detection: COR4 and COR5)
b_MISR_cd   equ  6   CD  Changed
b_MISR_cts  equ  5   CTS Changed
b_MISR_tm2  equ  1   General Timer 2 timed out.
b_MISR_tm1  equ  0   General Timer 1 timed out.

TISR    equ  $8A  Transmit Interrupt Status Register
b_TISR_berr equ 7 0 == No Bus Error, 1 == bus error detected (address availble)
b_TISR_eof  equ 6 Transmit end of frame indication (DMA mode)
b_TISR_eob  equ 5 Transmit end of buffer (DMA mode)
b_TISR_ur   equ 4 Transmit Under-run detected (Sync)
b_TISR_exb  equ 3 Buffer that had exception: 0 = Buffer A, 1 = Buffer B
b_TISR_te   equ 1 Transmitter Empty
b_TISR_thr  equ 0 Transfer FIFO below the FIFO threshold.

*  The receiver interrupt status register changes it's definition dependent
*  on the operational mode of the chip.  Each of the flavors is represented
*  below.  
RISRl   equ  $89  receive Interrupt Status Register low
*  HDLC Mode only..
b_HDL_RISRl_eof   equ 6 Receiving data frame is essentially complete
b_HDL_RISRl_rxabt equ 5 Received Abort sequence 
b_HDL_RISRl_crc   equ 4 CRC error on current frame
b_HDL_RISRl_oe    equ 3 Over-run error (last complete character flagged)
b_HDL_RISRl_resid equ 2 Residual Indication - last char of frame was partial
b_HDL_RISRl_clrd  equ 0 Clear Detect (x.21 data transfer phase clear signal)

* Asynchronous Mode
b_RISRl_to    equ  7  Timeout - Receive FIFO empty, no data received
b_RISRl_scdt  equ  4  Special character detected  (see below)
m_RISRl_scdt  equ  (7<<b_RISRl_scdt) Mask to get value (see below)
b_RISRl_oe    equ  3  Over-run Error
b_RISRl_pe    equ  2  Parity Error
b_RISRl_fe    equ  1  Framing Error - Bad stop bit detected
b_RISRl_brk   equ  0  Break Detected

** Special character detected values for Asynchronus mode:
RSPC_none equ (0<<b_RISRl_scdt)  None detected
RSPC_sc1  equ (1<<b_RISRl_scdt)  Special character 1 detected
RSPC_sc2  equ (2<<b_RISRl_scdt)  Special character 2 detected
RSPC_sc3  equ (3<<b_RISRl_scdt)  Special character 3 detected  [COR3]
RSPC_sc4  equ (4<<b_RISRl_scdt)  Special character 4 detected  [COR3]
RSPC_scr  equ (7<<b_RISRl_scdt)  Special character range match [COR3]

* Bisynchronous Mode
b_BIS_RISRl_eof   equ 6 Receiving data frame is essentially complete
b_BIS_RISRl_rxabt equ 5 Received Abort sequence 
b_BIS_RISRl_crc   equ 4 CRC error on current frame
b_BIS_RISRl_oe    equ 3 Over-run error (last complete character flagged)

* X.21 Mode
b_X21_RISRl_lval  equ  7  Lead Value 0 = On, 1 = off
b_X21_RISRl_scdt  equ  4  Special character detected  (see below)
m_X21_RISRl_scdt  equ  (7<<b_RISRl_scdt) Mask to get value (see below)
b_X21_RISRl_oe    equ  3  Over-run Error
b_X21_RISRl_pe    equ  2  Parity Error
b_X21_RISRl_lchg  equ  0  Lead Change - may precede special character

** Special character detected values for X.21 mode:
X21_RSPC_none equ (0<<b_RISRl_scdt)  None detected
X21_RSPC_sc1  equ (1<<b_RISRl_scdt)  Special character 1 detected
X21_RSPC_sc2  equ (2<<b_RISRl_scdt)  Special character 2 detected
X21_RSPC_sc3  equ (3<<b_RISRl_scdt)  Special character 3 detected  [COR3]
X21_RSPC_al0  equ (4<<b_RISRl_scdt)  All 0 condition
X21_RSPC_al1  equ (4<<b_RISRl_scdt)  All 1 condition
X21_RSPC_alt  equ (4<<b_RISRl_scdt)  Alternating 1/0 condition
X21_RSPC_sync equ (7<<b_RISRl_scdt)  Sync detect

* This register is available only in the DMA mode of operation
RISRh   equ  $88  Receive Interrupt Status Register high
b_RISRh_berr equ 7 1 == Bus Error occured
b_RISRh_eof  equ 6 1 == Data frame complete (Sync DMA only)
b_RISRh_eob  equ 5 1 == End of receive buffer
b_RISRh_bfr  equ 3 0 == Buffer A status, 1 == Buffer B status

*
* MIR -   Modem Interrupt Register
* TIR -   Transmit Interrupt Register
* RIR -   Receive Interrupt Register
*
MIR     equ  $EF
TIR     equ  $EC
RIR     equ  $ED

b_IR_men  equ  7              enable is set
b_IR_mact equ  6              active
b_IR_meoi equ  5              end of interrupt
b_IR_vct  equ  2              vector bits
m_IR_vct  equ  (3<<b_IR_vct)    .. mask for the above
b_IR_mcn  equ  0              channel requiring service
m_IR_mcn  equ  3                 .. mask for the above

*  Definition for the men/mact/meoi values:
IRidle  equ 0               Idle
IRirna  equ (4<<b_IR_meoi) interrupt requested, but not asserted
IRirqa  equ (6<<b_IR_meoi) interrupt is asserted
IRiack  equ (2<<b_IR_meoi) interrupt is acknowledged
IRirqd  equ (1<<b_IR_meoi) interrupt service routine is complete

*
* STK - Stack Register
*   This register is a four-deep-by-two-bit-wide stack containing the internal
*   interrupt nesting history.  (See the manual)
*
STK     equ  $E2

*
* TPR -   Time Period Register
*    Must provide a pre-scalar value to create a 1 ms clock period.
*   A clock is created by the chip which is  BCLK/2048.  This register
*   must then divide that to create 1 ms.
*
TPR     equ  $DA

*
* CAR -   Channel Access Register
*   During non-interrupt operation, the host writes this register to gain
*     access to the various channels register sets.
*   During interrupt operation, the chip supplies the channel number 
*     being serviced.  The host may not write this channel during 
*     interrupt service,  the only accessable register set is that of
*     the channel being serviced.
*
*   2 Bits:  0-1 describe channels 0-3.
CAR     equ  $EE

*
* DMR -   DMA Mode Register
*    NOTE:  WRITE ONLY REGISTER!! don't use RMW type instructions here
DMR     equ  $F6
DMA_8   equ   8   Perform only 8 bit DMA cycles
DMA_16  equ   0   Perform 16 bit DMA whenever possible

*
*  Data Registers
*
*   These registers are virtual, only the current active channel 
*   version can be reached at any given time.
*
RDR     equ  $F8  Receiver Data Register
TDR     equ  $F8  Transmit Data Register

*
* Priority Interrupt Level Registers
*    Program with level will be presented fat IACKIN* time.
*
PILR2   equ  $E0  Priority Interrupt Level Register 2
PILR3   equ  $E1  Priority Interrupt Level Register 3
PILR1   equ  $E3  Priority Interrupt Level Register 1

***************
*  Per Channel General Registers
*  Each channel of the device has a series of registers that are unique
*  to that channel alone.  These are detailed below.  These are available
*  only at certain times.  Refer to the manual.
*
*  Note: Register set below is ordered to functional group rather than
*        numerical order.
*

*
*  LICR -   Local Interrupting Channel Register
*    Always contains the number of interrupting channel being served.
*    If no interrupt acknowledged, channel number is in CAR.
*
LICR     equ   $26
b_LICR_chan  equ  2                 Channel number being serviced
m_LICR_chan  equ  (3<<b_LICR_chan)  ...  mask for the above

*  values for the channel being serviced
ICHAN0  equ  (0<<b_LICR_chan)  Channel 0
ICHAN1  equ  (1<<b_LICR_chan)  Channel 1
ICHAN2  equ  (2<<b_LICR_chan)  Channel 2
ICHAN3  equ  (3<<b_LICR_chan)  Channel 3

*
* LIVR -   Local Interrupt Vector Register
*   Chip defines 4 Vectors per channel.  It also controls the low two
*   bits of the vector to indicate which form of interrupt is being
*   generated.
*
LIVR     equ   $09

* these are the interrupt types generated (vector|value)
m_LIVR_grp equ 3 Mask vector to obtain the values....
LIVR_grp1  equ 1 Modem Signal Change Interrupt/General Timer Interrupt
LIVR_grp2  equ 2 Transmit Dat Interrupt
LIVR_grp3  equ 3 Receive Data Interrupt
LIVR_grpe  equ 0 Receive Excpetion Interrupt

*
* CCmdR -  Channel Command Register
*   NOTE:  must be a zero issuing the next command
*
CCmdR      equ   $13

b_CCmdR_mode   equ  7   If set 1, Controls timers, else, general control

* IFF bit 7 == 0:
b_CCmdR_clrch  equ  6   Clear Channel
b_CCmdR_initch equ  5   Initialize Channel
b_CCmdR_rstall equ  4   Reset all
b_CCmdR_entx   equ  3   Enable Transmitter
b_CCmdR_distx  equ  2   Disable Transmitter
b_CCmdR_enrc   equ  1   Enable Receiver
b_CCmdR_disrc  equ  0   Disable Receiver

* IFF bit 7 == 1:
b_CCmdR_clrt1  equ 6    Clear Timer 1
b_CCmdR_clrt2  equ 5    Clear Timer 2

*
* STCR -   Special Transmit Command Register
*   Note:  must be 0 before next command can be issued
*
STCR     equ   $12

b_STCR_abort equ 6  Abort Transmission (HDLC)
b_STCR_apcmp equ 5  Append Complete (Async DMA mode)
b_STCR_sndsc equ 3  Send Special Character(s)
*  a 3 bit number is used in bits 0-2 to describe one of:
*     ALL other values are reserved
SSPC_ch1   equ  1 Send special character 1
SSPC_ch2   equ  2 Send special character 2
SSPC_ch3   equ  3 Send special character 3
SSPC_ch4   equ  4 Send special character 4

*
* IER -  Interrupt Enable Register
*
IER      equ   $11
b_IER_mpcd  equ  7  Modem Pin Change Detect
b_IER_ret   equ  5  Receive Exception Timeout
b_IER_rxd   equ  3  Receive FIFO threshold has been reached
b_IER_timer equ  2  General Timer timeout (group 1 on any timer)
b_IER_txmty equ  1  Transmitter empty
b_IER_txd   equ  0  Transmit exception or FIFO threshold reached

*
*  COR1 -   Channel Options Register 1
*      Note:  definitions change dependent on mode...
COR1     equ   $10

*  HDLC Mode
b_HDLC_COR1_aflo  equ 7  Address field length (0 = 1 octet, 1 = 2 octets)
b_HDLC_COR1_clrd  equ 6  Clear Detect for X.21 data (1 = enabled, 0 = disabled)
b_HDLC_COR1_admd  equ 4  Addressing modes......
m_HDLC_COR1_admd  equ (3<<b_HDLC_COR1_admd) ... mask for modes
b_HDLC_COR1_flgs  equ 0  Inter-frame flag option ...
m_HDLC_COR1_flgs  equ $F     ... mask for flag option (see man.)

* Asynchronous, Bisynchronous and X.21 Mode
b_COR1_parity equ 7  Parity (1 == Odd parity, 0 == Even parity)
b_COR1_parmd  equ 5  Parity Mode
m_COR1_parmd  equ (3<<b_COR1_parmd) ... Parity Mode mask
b_COR1_ignore equ 4  Ignore receive data parity
b_COR1_len    equ 0  Character length select (see below)
m_COR1_len    equ $F 

* parmd can be coded as below:
PAR_nopar  equ 0   no parity
PAR_forpar equ 1   force parity
PAR_parity equ 2   Normal parity
Par_res    equ 3   Reserved, do not use

* character lengths are defined as:
CLEN_5   equ  4  5 bits
CLEN_6   equ  5  6 bits
CLEN_7   equ  6  7 bits
CLEN_8   equ  7  8 bits

*
* COR2 - Channel Option Register 2
*    Note:  bits change dependent on the chip mode.
*
COR2     equ   $17

* HDLC mode
b_COR2_HDLC_fcs equ 6  FCS Append (0 = no CRC and end of frame, 1 = CRC passed)

* HDLC and  Asynchronous modes only:
b_COR2_rts equ 2  RTS Automatic Output Enable  ( 1 = auto, 0 = manual )
b_COR2_cts equ 1  CTS Automatic Output Enable  ( 1 = auto, 0 = manual )
b_COR2_dsr equ 0  DSR Automatic Output Enable  ( 1 = auto, 0 = manual )

* Aysnchronous Only:
b_COR2_ixm  equ 7 IFF TXIBE TRUE: causes Tx halt durring in-band transmition
b_COR2_ibe  equ 6 Transmit In-Band Flow Control Enable
b_COR2_etc  equ 5 Embedded Transmitter Command Enable  (see manual)
b_COR2_rlm  equ 3 Enable Remode Loopback Mode

* Bisynchronous Mode
b_COR2_BIS_lrc  equ 7 Longitudinal Redundancy Check (0 = CRC16, 1 = LRC)
b_COR2_BIS_bcc  equ 6 BCC Append (0 = no BCC at end of frame, 1 = append BCC)
b_COR2_BIS_ebc  equ 5 0 = ASCII character set, 1 = EBCDIC character set

* X.21 mode only:
b_COR2_X21_etc  equ 5 Embedded Transmitter Command Enable (see manual)

*
* COR3 -   Channel Options Register 3
*   Note: Meaning of bits change based on current mode.
*
COR3     equ   $16

* HDLC only:
b_COR3_HDLC_sndpad equ 7  1 == Send pat before flag when leaving Mark idling
b_COR3_HDLC_alt1   equ 6  1 = AAh (Manchester/NRZ) pad, 0 = 0h (NRZI) pad
b_COR3_HDLC_fcs    equ 5  1 = disable FCS generation, 0 = normal FCS mode
b_COR3_HDLC_idle   equ 3  1 = idle in flag, 0 = idle in mark
m_COR3_HDLC_pad    equ 7  character count (sync) [1-4 valid]

* Asynchronous Mode only:
b_COR3_escde  equ 7  Extended Special Character Detect enable
b_COR3_rde    equ 6  Range Detect Enable
b_COR3_fct    equ 5  Flow Control Transparancy Mode (1 = flow control to host)
b_COR3_scde   equ 4  Special Character Detection (schr1 and schr2) enable
m_COR3_stbt   equ 7  Mask for stop bits

*  Stop bits are defined as below (all other values reserved!)
STOP1  equ 2  1 stop bit
STOP15 equ 3  1.5 stop bits
STOP2  equ 4  2 stop bits

*  Bisynchronous Mode
b_COR3_BIS_sndpad equ 7 1 == Send pad before sending SYN
b_COR3_BIS_s55    equ 6 1 = 55h as pad, 0 = AAh as pad
b_COR3_BIS_idle   equ 3 1 = idle in SYN,  1 = idle in mark
m_COR3_BIS_pad    equ 7 (1-4 for 1-4 pad characters)

* X.21 mode only:
b_COR3_X21_ssde equ 6 Steady State Detect Enable
b_COR3_X21_stsy equ 5 Strip SYN
b_COR3_X21_scde equ 4 Special Character Detect Enable

*
* COR4 -   Channel Options Register 4
*  Note: All modes, same meanings!!
*
COR4     equ   $15

b_COR4_dsrzd equ 7  Detect 1->0 transition on DSR line
b_COR4_cdzd  equ 6  Detect 1->0 transition on CD line
b_COR4_ctszd equ 5  Detect 1->0 transition on CTS line
m_COR4_fifot equ $F Binary encoded depth of FIFO

*
* COR5 -   Channel Options Register 5
*
COR5     equ   $14
b_COR4_dsrod equ 7  Detect 0->1 transition on DSR line
b_COR4_cdod  equ 6  Detect 0->1 transition on CD line
b_COR4_ctsod equ 5  Detect 0->1 transition on CTS line
m_COR4_rxthr equ $F Binary encoded, depth of FIFO for flow control

*
* COR6 -   Channel Options Register 6
*
COR6     equ   $18

b_COR6_crlf equ 5 EOL (<CR><LF>) action control [Receiver] (see below)
m_COR6_crlf equ (7<<b_COR6_crlf) 
b_COR6_brk  equ 3 Break action control (see below)
m_COR6_brk  equ (3<<b_COR6_brk)
b_COR6_par  equ 0 Parity error action control (see below)
m_COR6_par  equ (7<<b_COR6_par)

* codes that define EOL action control  (<LF> == NL)
RCRLF_nospec equ  0  No special action
RCRLF_nltocr equ  1  translate NL to <CR>
RCRLF_crtonl equ  2  translate <CR> to NL
RCRLF_xlbth  equ  3  <CR> xlated to NL and NL xlated to <CR>
RCRLF_crdel  equ  4  <CR> discarded
RCRLF_crdxl  equ  5  <CR> discarded and NL xlated to <CR>
RCRLF_crdl   equ  6  <CR> discarded
RCRLF_crdx   equ  7  <CR> discarded and NL xlated to <CR>

* Break Actions:
BRK_genex  equ 0 Generate exception on break
BRK_xlton  equ 1 Translate Break to NULL
BRK_resv   equ 2 Reserved
BRK_discd  equ 3 Discard character

* Parity error actions:   (all other values reserved!!)
PAR_genex  equ 0 Generate exception on parity error
PAR_xlton  equ 1 Translate error character to NULL
PAR_ignor  equ 2 Ignor error, character passed as good data
PAR_discd  equ 3 Discard error character
PAR_xlate  equ 5 Translate to sequence: $FF, $00, <err char.>, pass as good

*
* COR7 -   Channel Options Register 7
*    NOTE:  Asynchronus mode only!
*
COR7     equ   $07

b_COR7_istrip equ 7 Set 1 strips characters to 7 bits after processing
b_COR7_lne    equ 6 0 = process all as possible special characters
b_COR7_fcoerr equ 5 0 = Error characters not processed as possible special char
b_COR7_crlf   equ 0 EOL action control  [Transmit]  (see below)
m_COR7_crlf   equ (3<<b_COR7_crlf)

* Codes that define <CR><LF> handling for transmitter
TCRLF_nospec  equ 0 No special handling
TCRLF_crtonl  equ 1 Translate <CR> to NL
TCRLF_nltoeol equ 2 Translate NL to <CR><NL>
TCRLF_xlate   equ 3 <CR> trnaslate to <NL> and <NL> translated to <CR><NL>

*
* CMR -   Channel Mode Register
*
CMR      equ   $1B

b_CMR_rxmode equ 7 Receive Transfer Mode  (0 = interrupt, 1 = DMA)
b_CMR_txmode equ 6 Transmit Transfer Mode (0 = interrupt, 1 = DMA)
m_CMR_pcmode equ 7 Protocol Mode Select

* Protocol Select values:  (all others reserved)
CMR_hdlc   equ  0  HDLC 
CMR_bis    equ  1  Bisynchronous
CMR_asy    equ  2  Asynchronous
CMR_x21    equ  3  X.21

*
* CSR -   Channel Status Register (Sync Modes)
*    NOTE:  Different Mode Different Meanings
CSR      equ   $1A

*  all protocols 
b_CSR_rxen    equ 7  Receiver Enabled:  1 == enabled, 0 == disabled
b_CSR_txen    equ 3  Transmitter Enabled: 1 == enabled, 0 == disabled

* HDLC Mode
b_CSR_HDLC_rxfl equ  6  Receiver Flag: 1 == enabled, 0 == disabled
b_CSR_HDLC_rxfr equ  5  Receiver frame: 1 == enabled, 0 == disabled
b_CSR_HDLC_rxmk equ  4  Receiver Mark: 1 == enabled, 0 == disabled
b_CSR_HDLC_txfl equ  2  Transmitter Flag: 1 == enabled, 0 == disabled
b_CSR_HDLC_txfr equ  1  Transmitter frame: 1 == enabled, 0 == disabled
b_CSR_HDLC_txmk equ  0  Transmitter mark: 1 == enabled, 0 == disabled

* Asynchronous mode:
b_CSR_rxflon equ  6  Receiver Flow Off: 1 == normal, 0 == xoff
b_CSR_rxfof  equ  5  Receiver Flow On: 1 == normal, 0 == xon
b_CSR_txflon equ  2  Transmitter Flow Off: 1 == normal, 0 == xoff
b_CSR_txfof  equ  1  Transmitter Flow On: 1 == normal, 0 == xon

* Bisynchronous Mode
b_CSR_BIS_ritb equ  6  Receive ITB
b_CSR_BIS_rxfr equ  5  Receive frame (1 == frame in progress)
b_CSR_BIS_titb equ  2  Transmit ITB
b_CSR_BIS_txfr equ  1  Transmition of frame in progress

* X.21 mode
b_CSR_X21_rxspc  equ 5  Receive special
b_CSR_X21_txspc  equ 5  Transmit special

*
* DMABSTS -   DMA Buffer Status
*
DMABSTS  equ   $19

b_DMABSTS_restam  equ  6  Reset Append Mode
b_DMABSTS_append  equ  4  Append (1 = append buffer in use, 0 = not)
b_DMABSTS_ntbuf   equ  3  Next transmit buffer (0 ->A, 1->B)
b_DMABSTS_tbusy   equ  2  buffer in use?  0 = none, 1 = current tx buffer
b_DMABSTS_nrbuf   equ  1  Next Receive buffer (0->A, 1->B)
b_DMABSTS_rbusy   equ  0  buffer in use?  0 = none, 1 = current rx buffer

LNXT     equ   $2E  LNext Character

*
*  These registers are used to define flow/control and speical 
*  sequence handling.
SCHR1    equ   $1F  Special Character Register 1  (Async)
RFAR1    equ   $1F  Receive Frame Address Register 1 (Sync)  

SCHR2    equ   $1E  Special Character Register 2  (Async)
RFAR2    equ   $1E  Receive Frame Address Register 2 (Sync)  

SCHR3    equ   $1D  Special Character Register 3  (Async)
RFAR3    equ   $1D  Receive Frame Address Register 3 (Sync)  

SCHR4    equ   $1C  Special Character Register 4  (Async)
RFAR4    equ   $1C  Receive Frame Address Register 4 (Sync)  

*
*  In synchronous modes, reception of a frame can be qualified by a sequence
*  of characters via these registers
SCRl     equ   $23  Special Character Range low  (Async)
SCRh     equ   $22  Special Character Range high (Async)

* 
* (Async)  Controls the receive and transmit data timeout period.
*
RTPR     equ   $24  Receive Timeout Period Register (Async)
RTPRl    equ   $25  Receive Timeout Period Register low  (Async)
RTPRh    equ   $24  Receive Timeout Period Register high (Async)
TTR      equ   $29  Transimit timer Register (Async)

*
*  (SYNC)  General use timers
*
GT1l     equ   $2B  General Timer 1 low  (Sync)
GT1h     equ   $2A  General Timer 1 high  (Sync)
GT2      equ   $29  General Timer 2  (Sync)

*
*  RFOC -   Receive FIFO Output Count
*
RFOC     equ   $30

m_RFOC_foc  equ $1f Receive data count. (number of bytes available)

*
*  these contain the address of the current buffer in use
TCBADRL  equ   $3A  Transmit Buffer Address Lower
TCBADRU  equ   $38  Transmit Buffer Address Upper

RCBADRL  equ   $3E  Receive Buffer Address Lower
RCBADRU  equ   $3C  Receive Buffer Address Upper

*
*  These are used to set the address of the buffers
ARBADRL  equ   $42  A Receive Buffer Address Upper and Lower
ARBADRU  equ   $40
BRBADRL  equ   $46  B Receive Buffer Address Upper and Lower
BRBADRU  equ   $44

*
*  Used to describe the size of the buffers.
ARBCNT   equ   $4A  A Receive Buffer Count
BRBCNT   equ   $48  B Receive Buffer Count

*
*  Buffer status registers:
*
ARBSTS   equ   $4F  A Receive Buffer Status
BRBSTS   equ   $4E  B Receive Buffer Status

b_RBSTS_berr  equ 7 1 == Bus error, 0 = no bus error
b_RBSTS_eof   equ 6 0 = buffer does not terminate frame, 1 = EOF
b_RBSTS_eob   equ 5 0 = end of buffer reached
b_RBSTS_owner equ 0 0 = buffer free for CD2400, 1 buffer not free


*
*  Transmit buffer address, count, and status
*
ATBADRL  equ   $52  A Transmit Buffer Address Upper and Lower
ATBADRU  equ   $50
BTBADRL  equ   $56  B Transmit Buffer Address Upper and Lower
BTBADRU  equ   $54

ATBCNT   equ   $5A  A Transmit Buffer Count
BTBCNT   equ   $58  B Transmit Buffer Count

ATBSTS   equ   $5F  A Transmit Buffer Status
BTBSTS   equ   $5E  B Transmit Buffer Status

b_TBSTS_berr  equ 7 1 == Bus error, 0 = no bus error
b_TBSTS_eof   equ 6 0 = buffer does not terminate frame, 1 = EOF
b_TBSTS_eob   equ 5 0 = end of buffer reached
b_TBSTS_appnd equ 3 0 = no data will be appended , 1 = data will be appended
b_TBSTS_irq   equ 1 0 = no interrupt, 1 = interrupt at end of buffer
b_TBSTS_owner equ 0 0 = buffer free for CD2400, 1 buffer not free

*
* Modem Signal Value Registers
MSVR_RTS equ   $DE  Modem Signal Value Register
MSVR_DTR equ   $DF  Modem Signal Value Register

b_MSVR_dsr   equ  7  Current state of Data Set Ready input
b_MSRV_cd    equ  6  Current stat of Carrier Detect input
b_MSRV_cts   equ  5  Current state of Clear to Send input
b_MSRV_dtr   equ  4  0 = DTR on TXCout/DTR pin, 1 = Transmit clock on pin
b_MSRV_opt   equ  2  Read only: 1 = 2401, 2 = 2400
b_MSRV_dtro  equ  1 Current state of Data Terminal Ready output
b_MSRV_rtso  equ  0 Current state of the Clear to Send output

*
*  Per-Channel Baud Rate Generators
*
TBPR     equ   $C3  Transmit Baud rate Period Register (period divisor)
RBPR     equ   $CB  Receive Baud rate Period register

TCOR     equ   $C0  Transmit Clock Option Register

b_TCOR_sel equ 5 Select clock source for transmit (see below)
m_TCOR_sel equ (7<<b_TCOR_sel)
b_TCOR_x1  equ 3 Times 1 external clock select
b_TCOR_llm equ 1 Enable local loopback mode

RCOR     equ   $C8  Receive Clock Option Register

b_RCOR_tlv  equ 7  Reflects logical value of transmit data pin
b_RCOR_dpll equ 5  DPLL mode enable
b_RCOR_dmod equ 3  DPLL mode select value
m_RCOR_dmod equ (3<<b_RCOR_dmod)
m_RCOR_clks equ 7 Clock Select

* values for RCOR_dmod
RCOR_NRZ   equ 0  NRZ
RCOR_NRZI  equ 1  NRZI
RCOR_MAN   equ 2  Manchester

* values for RCOR_clks  (all other values reserved)
RCOR_CS0  equ 0 Clock 0
RCOR_CS1  equ 1 Clock 1
RCOR_CS2  equ 2 Clock 2
RCOR_CS3  equ 3 Clock 3
RCOR_CS4  equ 4 Clock 4
RCOR_EXT  equ 6 External

*
*  CPSR -   CRC Polynomial Select Register
*
CPSR     equ   $D6

b_CPSR_crcp equ 0  1 == Bisync, 0 = HDLC

* end of "CD2401.d"
 opt l

