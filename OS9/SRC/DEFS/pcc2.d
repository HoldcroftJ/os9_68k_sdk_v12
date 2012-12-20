 opt -l
********************
* Definitions for PCC device - VME167
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  91/04/23  Created for the mVme167                          Rwb
* 02  91/05/03  Added some bit defs and merged several reg defs  Rwb

*
*  Conventions used in this file:
*
*   b_XXXX  denotes a bit number definition (note access size quoted)
*   m_XXXX  denotes a bit mask (again, note access size quoted)

*
*  Register definitions:
*   Defined by OFFSET from Base Address rather than Absolute Address.
*   Most registers are byte/word/longword accessable. (exceptions as noted)
*

PCCChipID  equ  $0   Chip Identification Register         (byte)
PCCChipRV  equ  $1   Chip Revision Register               (byte)

*
* PCCGCR General Control Register
*
PCCGCR        equ  $2       General Control Register             (byte)

b_PCCGCR_Fast  equ  0        BBRAM access speed (1=Fast, 0=Slow)
m_PCCGCR_Fast  equ  (1<<b_PCCGCR_Fast)
b_PCCGCR_MIEN  equ  1        Master Interrupt Enable
m_PCCGCR_MIEN  equ  (1<<b_PCCGCR_MIEN)
b_PCCGCR_C040  equ  2        CPU040 Should be set for MC68040 CPU in use
m_PCCGCR_C040  equ  (1<<b_PCCGCR_C040)

*
* PCCVBR PCC IRQ Vector Base Register
*
*   Bits 4-7 define the base interrupt vector.  The PCC defines a block
*   of 16 interrupts prioritize as below.
*     
PCCVBR      equ  $3       PCC IRQ Vector Base Register         (byte)
m_PCCVBR    equ ($F<<4)   Masks appropriate bits

*
* Interrupt Vector assignments
*   The vector base is programmed into PCCVBR and the resultant vectors
*   produced are then:
*        PCCVBR+n
*
*  0  Printer port BSY Interrupt
*  1  Printer port PE Interrupt
*  2  Printer port Select Interrupt
*  3  Printer port Fault Interrupt
*  4  Printer port Ack Interrupt
*
*  5   NCR 53C710 Interrupt
*  6   Intel 82549CA LANC Error Interrupt
*  7   Intel 82549CA LANC Interrupt
*
*  8   PCC Timer 2 Interrupt
*  9   PCC Timer 1 Interrupt
* $A   PCC GPIO Interrupt
*
*   NOTE:  Auto-vector NOT used, CD2401 provides own vector
* $B   PCC SCC Modem Interrupt (auto-vector only)
* $C   PCC SCC RX Interrupt    (auto-vector only)
* $D   PCC SCC TX Interrupt    (auto-vector only)

*
*   Timers and GPIO
PCCTk1Cmp  equ  $4   Tick Timer 1 Compare Register        (lword)
PCCTk1Cnt  equ  $8   Tick Timer 1 Count Register          (lword)
PCCTk2Cmp  equ  $C   Tick Timer 2 Compare Register        (lword)
PCCTk2Cnt  equ  $10  Tick Timer 2 Count Register          (lword)
PCCTkPrSc  equ  $14  Prescaler Count Register             (byte)
PCCTkAdj   equ  $15  Prescaler Clock Adjust               (byte)

*
* PCCTk2Ctl and  PCCTk1Ctl -  Tick Timer Control Registers
*
PCCTk2Ctl    equ   $16     Tick Timer 2 Control Register        (byte)
PCCTk1Ctl    equ   $17     Tick Timer 1 Control Register        (byte)

b_PCCTk_CEN   equ   0            Counter Enable (1=Increment  0=Decriment)
m_PCCTk_CEN   equ   (1<<b_PCCTk_CEN)
b_PCCTk_COC   equ   1            Clear on Compare (1 = reset count on compare)
m_PCCTk_COC   equ   (1<<b_PCCTk_COC)
b_PCCTk_COVF  equ   2            Clear on overflow 
m_PCCTk_COVF  equ   (1<<b_PCCTk_COVF)
b_PCCTk_OVF   equ   4            A 4 Bit overflow counter
m_PCCTk_OVF   equ   ($f<<b_PCCTk_OVF)

*
* PCCGPICR  - General Purpose Interrupt Control
*
PCCGPICR         equ  $18     General Purpose Interrupt Control    (byte)

*
* PCCGPCtl  General Purpose I/O Pin Control
*
PCCGPCtl       equ  $19     General Purpose I/O Pin Control      (byte)

PCCGPCtl_GPO   equ  (1<<0)  GPOE level select - (1 == HIGH, 0 == LOW)
PCCGPCtl_GPE   equ  (1<<1)  GPOE enable  (1 = PCC Drives , 0 == External Drive)
PCCGPCtl_GPI   equ  (1<<2)  Reflects stat of GPIO pin (J3 pin 19, ext. reset)

*
* PCCTk1ICR and PCCTk2ICR - Tick Timer Interrupt Control Registers
*
PCCTk2ICR        equ  $1A    Tick Timer 2 Interrupt Control       (byte)
PCCTk1ICR        equ  $1B    Tick Timer 2 Interrupt Control       (byte)

*  CD2401 Serial Chip Interface
PCCSCErr   equ  $1C  SCC Error Status Register            (byte)
PCCSCMicr  equ  $1D  SCC Modem Interrupt Control          (byte)
PCCSCTicr  equ  $1E  SCC Transmit Interrupt Control       (byte)
PCCSCRicr  equ  $1F  SCC Receiver Interrupt Control       (byte)
PCCSCMPiak equ  $23  SCC Modem Pseudo IACK generation     (byte)
PCCSCTPiak equ  $25  SCC Transmit Pseudo IACK generation  (byte)
PCCSCRPiak equ  $27  SCC Receiver Pseudo IACK generation  (byte)

* Intel 82549CA LANC (EtherNet LAN controller)
PCCLNErr   equ  $28  LANC Error Status Register           (byte)
PCCLNicr   equ  $2A  LANC Interrupt Control               (byte)

*
* PCCLNBErr -  LANC Bus Error Interrupt Control
*
PCCLNBEr            equ  $2B    LANC Bus Error Interrupt Control     (byte)

b_PCCLNBEr_Lvl      equ  0    Interrupt Request Level Mask (0 = no interrupt)
m_PCCLNBEr_Lvl      equ  $7
b_PCCLNBEr_IntClr   equ  3    Interrupt Status Clear bit (1 == Clear)
m_PCCLNBEr_IntClr   equ  (1<<b_PCCLNBEr_IntClr)
b_PCCLNBEr_IntEnb   equ  4    Interrupt Enable Bit (1 == Enable)
m_PCCLNBEr_IntEnb   equ  (1<<b_PCCLNBEr_IntEnb)
b_PCCLNBEr_IntPend  equ  5    TRUE = LANC Bus error interrupt pending
m_PCCLNBEr_IntPend  equ  (1<<b_PCCLNBEr_IntPend)
b_PCCLNBEr_SnpCtl   equ  6    Mask for Snoop Control Bits
m_PCCLNBEr_SnpCtl   equ  (3<<b_PCCLNBEr_SnpCtl)

* NCR 53C710 SCSI Chip Interface
PCCSCSIErr equ  $2C  SCSI Error Status Register           (byte)
PCCSCSIicr equ  $2F  SCSI Interrupt Control Register      (byte)

* Parallel I/O port (Printer Port)
PCCPPackCt  equ  $30  Printer Port ACK control            (byte)
PCCPPFlticr equ  $31  Printer Port FAULT Int.  Control    (byte)
PCCPPSELicr equ  $32  Printer Port SEL Int.  Control      (byte)
PCCPPPEicr  equ  $33  Printer Port PE  Int.  Control      (byte)
PCCPPBSYicr equ  $34  Printer Port BSY Int.  Control      (byte)

*
* PCCPPInStat - Printer Port Input Status
*
PCCPPInStat      equ  $36           Printer Port Input Status           (byte)

b_PPBusy           equ  0
m_PCCPPInStat_BSY  equ  (1<<b_PPBusy) Printer Busy input pin
b_PPpe             equ  1
m_PCCPPInStat_PE   equ  (1<<b_PPpe)   PE input pin  (paper empty)
b_PPsel            equ  2
m_PCCPPInStat_SEL  equ  (1<<b_PPsel)  SEL input pin  (select)
b_PPflt            equ  3
m_PCCPPInStat_FLT  equ  (1<<b_PPflt)  FAULT input pin (printer fault)
b_PPack            equ  4
m_PCCPPInStat_ACK  equ  (1<<b_PPack)  ACK input pin
b_PPint            equ  7
m_PCCPPInStat_PINT equ  (1<<b_PPint)  Interrupt Status (1 == Pending interrupt)

*
* PCCPPCntrl -  Printer Port Control Register
*
PCCPPCntrl       equ  $37    Printer Port Control Register       (byte)

PCCPPCntrl_MAN   equ  (1<<0) Manual Strobe Control (0 = Auto Strobe)
PCCPPCntrl_FAST  equ  (1<<1) Strobe timing (Auto mode only, BCLK dependent)
PCCPPCntrl_STB   equ  (1<<2) Manual Strobe (1 = Strobe high, 0 = strobe low)
PCCPPCntrl_INP   equ  (1<<3) Printer Input Prime (1 == Activated)
PCCPPCntrl_DOEN  equ  (1<<4) Printer Data Output Enable (1=Output, 0=Input)

*
PCCChipSpeed equ $38  Chip Speed Register (see below)     (word only!)
PCCPPData   equ  $3A  Printer Port Data Register          (byte)

*
* PCC interrupt level and mask level registers
*
* PCCPriLvl  - PCC Priority Level register
*
PCCPriLvl      equ  $3E  PCC Priority Level Register         (byte)
m_PCCPriLvl    equ  $7  

*
* PCCIntMsk - PCC Interrupt mask level
*
PCCIntMsk      equ  $3F  PCC Interrupt mask level            (byte)
m_PCCIntMsk    equ  $7

******
*  These definitions have general application accross a number of 
*  registers.   The register offsets are defined above.
*****

*
* PCCSCErr   -  SCC Error Status Register
* PCCSCSIErr -  SCSI Error Status Register
* PCCLNErr   -  LANC Error Status Register
*   If a bus error occurs durring the Devices DMA cycle then this register
*   can be used to determine the cause of the error.
*
PCCErr_SCLR    equ  (1<<0)  Status clear (Write to 1 Clears bits 1 - 3)
PCCErr_LTO     equ  (1<<1)  Local Time Out
PCCErr_EXT     equ  (1<<2)  Vme Bus Access Time out
PCCErr_PrTy    equ  (1<<3)  DRAM parity check error
PCCErr_Rtry    equ  (1<<4)  Retry was required

*
* PCCSCMicr   -  SCC Modem Interrupt Control
* PCCSCTicr   -  SCC Transmit Interrupt Control
* PCCSCRicr   -  SCC Receiver Interrupt Control
* PCCLNicr    -  LANC Interrupt Control
* PCCSCSIicr  -  SCSI Interrupt Control
* PCCPPIackCt -  Printer Port IACK control
* PCCPPFlticr -  Printer Port FAULT Int.  Control
* PCCPPSELicr -  Printer Port SEL Int.  Control
* PCCPPPEicr  -  Printer Port PE  Int.  Control
* PCCPPBSYicr -  Printer Port BSY Int.  Control
* PCCTk1ICR   -  Tick Timer Interrupt Control Register 1
* PCCTk2ICR   -  Tick Timer Interrupt Control Register 2
* PCCGPICR    -  General Purpose Interrupt Control
*
b_PCCicr_Lvl     equ  0
m_PCCicr_Lvl     equ  ($7)   Interrupt Request Level (0 = no interrupts)

b_PCCicr_AVEC    equ  3  Auto-Vector Interrupt Enable (NOTE: not used!)
m_PCCicr_AVEC    equ  (1<<b_PCCicr_AVEC)

b_PCCicr_ICLR    equ  3  Edge Interrupt Clear bit [SCSI, LANC, PP only]
m_PCCicr_ICLR    equ  (1<<b_PCCicr_ICLR)

b_PCCicr_IEN     equ  4  Interrupt Enable Bit  (1 == Enabled)
m_PCCicr_IEN     equ  (1<<b_PCCicr_IEN)

b_PCCicr_INT     equ  5  Interrupt Status  ( 1 == IRQ pending)
m_PCCicr_INT     equ  (1<<b_PCCicr_INT)

*  for the LANC, SCSI, and PP these two bits are also available.....
b_PCCicr_MEL     equ 6  Interrupt Mode (1 == Edge, 0 == Level)
m_PCCicr_MEL     equ  (1<<b_PCCicr_MEL)

b_PCCicr_PLTY    equ  7 Interrupt Polarity (1 = rise/high, 0 = fall/low)
m_PCCicr_PLTY    equ  (1<<b_PCCicr_PLTY)

 opt l
