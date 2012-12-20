 opt -l
********************
* Definitions for VMEchip2 device - VME167
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  91/04/24  Created                                          Rwb
* 02  91/10/08  added lbvm_enX bit defs.                         wwb
* 03  91/10/09  fixed problem with ATTR_dat definition, added    wwb
*               b_XX defs for ATTR_XX defs.
* 04  91/10/15  fixed "board fail in/out" defs.                  wwb
* 05  91/10/24  fixed problems with IRQVBR_vbrX defs.            wwb
* 06  91/11/05  cleanup pass for release.                        wwb
*               <<<---- OS-9/68000 V2.4.4 Release ---->>>
* 07  92/10/20  fixed mask definitions for interrupt level regs. wwb
*               (were defined as 2 bit fields, should be 3).
* 08  93/01/13  Added MISC control register definitions new      ats
*               to revision 1 of VMEchip2 on the VME162.
* 09  93/08/05  Corrected the PROM size codes.					 ats
* 10  99/02/12  Added Bit shift definitions for DMACgc Fair bits mgh
*

*
*  Conventions used in this file:
*
*   b_XXXX  denotes a bit number definition (note access size quoted)
*   m_XXXX  denotes a bit mask (again, note access size quoted)

 ifndef VMEchBase
VMEchBase   set   $FFF40000
 endc

*
*  Note:  All accesses to the VMEChip2 on the MVME167 must be LONG
*         word accesses.
*    
*    Register definitions below are OFFSETS from the device base.
*    Breakdown of the registers is by logical/functional group.
*

*  These are the Slave Address windows.
*    These registers map the local bus to a VME bus address, allowing
*    other VME masters to access the boards local memory.
*     Note:  Do NOT Program Both Windows to the SAME ADDRESS!
*
SEA1    equ   $0     Slave Ending and Start Address 1
SEA2    equ   $4     Slave Ending and Start Address 2
SAT1    equ   $8     Slave Address Translation Address and Select 1
SAT2    equ   $C     Slave Address Translation Address and Select 2
SAATTR  equ   $10    Slave Address Window Attributes

*  these are definitions that apply to the window attribute registers
*
b_ATTR_wp1   equ   8   Write Post Enable (1 = Enable, 2 = Disable)
b_ATTR_sup   equ   7   Supervisor Access (1 = TRUE, 0 = FALSE)
b_ATTR_usr   equ   6   User Access (1 = TRUE, 0 = FALSE )
b_ATTR_a32   equ   5   A32 Address (1 = ENABLE, 0 = DISABLE )
b_ATTR_a24   equ   4   A24 Address (1 = ENABLE, 0 = DISABLE )
b_ATTR_d64   equ   3   D65 Access  (1 = ENABLE, 0 = DISABLE )
b_ATTR_blk   equ   2   Block Access (1 = ENABLE, 0 = DISABLE )
b_ATTR_pgm   equ   1   Program Access (1 = ENABLE, 0 = DISABLE )
b_ATTR_dat   equ   0   Data Access (1 = ENABLE, 0 = DISABLE )

ATTR_snoop   equ   ($3<<9)  Bits defining how to drive snoop lines
ATTR_wp1     equ   (1<<b_ATTR_wp1)   Write Post Enable (1 = Enable, 2 = Disable)
ATTR_sup     equ   (1<<b_ATTR_sup)   Supervisor Access (1 = TRUE, 0 = FALSE)
ATTR_usr     equ   (1<<b_ATTR_usr)   User Access (1 = TRUE, 0 = FALSE )
ATTR_a32     equ   (1<<b_ATTR_a32)   A32 Address (1 = ENABLE, 0 = DISABLE )
ATTR_a24     equ   (1<<b_ATTR_a24)   A24 Address (1 = ENABLE, 0 = DISABLE )
ATTR_d64     equ   (1<<b_ATTR_d64)   D64 Access  (1 = ENABLE, 0 = DISABLE )
ATTR_blk     equ   (1<<b_ATTR_blk)   Block Access (1 = ENABLE, 0 = DISABLE )
ATTR_pgm     equ   (1<<b_ATTR_pgm)   Program Access (1 = ENABLE, 0 = DISABLE )
ATTR_dat     equ   (1<<b_ATTR_dat)   Data Access (1 = ENABLE, 0 = DISABLE )

*  These are the Local Bus Slave Address windows.
*    These registers are used to map the local bus to the VME bus, allowing
*    the boards local devices access to offboard VMEbus resources.
LBSEA1    equ   $14     Local Bus Slave Ending and Starting address 1
LBSEA2    equ   $18     Local Bus Slave Ending  and Starting address 2
LBSEA3    equ   $1C     Local Bus Slave Ending and Starting address 3
LBSEA4    equ   $20     Local Bus Slave Ending and Starting address 4
LBSAT4    equ   $24     Local Bus Slave Address Translation Address and select
LBSAATTR  equ   $28     Local Bus Slave Address Window Attributes Register 1

*  Attributes for the Local Bus Slaves
*  (Note:  1 byte per decoder)
*
LBSAATTR_d16     equ   (1<<7) D16 Access (1 = D16 cycles, 0 = D32 cycles)
LBSAATTR_wp      equ   (1<<6) Write Post Enable (1 = ENABLE, 0 = DISABLE )
m_LBSAATTR_am    equ   ($3f)  Address Modifier bit mask

b_LBVAT1  equ 0   Attributes register 1 (LWORD access to LBSATTR)
b_LBVAT2  equ 8   Attributes register 2
b_LBVAT3  equ 16  Attributes register 3
b_LBVAT4  equ 24  Attributes register 4

*
* GCSRADDR - VMEBus Slave GCSR Group Address Register
*     Maps the GCSR to the VMEbus.
*   NOTE: The VMEChip should be Bus Master while these registers are modified.
*
GCSRADDR         equ  $2c   (long word)

m_GCSRADDR_group     equ  ($ff<<24) Mask for the group address (A8-A15)
m_GCSRADDR_board     equ  ($f<<20)  Mask for the board (A4-A7) [LM0-LM3]

*  mask that protects this register
*
m_GCSRADDR           equ  (m_GCSRADDR_group+m_GCSRADDR_board)

*
* LBCTL -  Local Bus to VMEBus Enable Control Register
*   Note:  LWORD access.  Care must be taken not to affect the contents of
*          the GCSR address control register (high 8 bits of this register)!
*
LBCTL             equ  GCSRADDR
b_LBCTL_lbvm_en1  equ 16
b_LBCTL_lbvm_en2  equ 17
b_LBCTL_lbvm_en3  equ 18
b_LBCTL_lbvm_en4  equ 19

LBCTL_lbvm_en4    equ  (1<<b_LBCTL_lbvm_en4)  Enable Decoder 4 (1 = Enabled)
LBCTL_lbvm_en3    equ  (1<<b_LBCTL_lbvm_en3)  Enable Decoder 3 (1 = Enabled)
LBCTL_lbvm_en2    equ  (1<<b_LBCTL_lbvm_en2)  Enable Decoder 2 (1 = Enabled)
LBCTL_lbvm_en1    equ  (1<<b_LBCTL_lbvm_en1)  Enable Decoder 1 (1 = Enabled)

* Local Bus to VMEBus I/O Control Register
*
LBCTL_lbvmio_i2en equ  (1<<15)  Set 1 Enables F page ($F0000000 - $FEFFFFFF)
LBCTL_lbvmio_i2wp equ  (1<<14)  Set 1 Enables write posting on F page
LBCTL_lbvmio_i2su equ  (1<<13)  AM for F page (1 = supervisor, 0 = user)
LBCTL_lbvmio_i2pd equ  (1<<12)  AM for F page (1 = program, 0 = data)
LBCTL_lbvmio_i1en equ  (1<<11)  Set 1 enables VMEBus short I/O space decoder
LBCTL_lbvmio_i1si equ  (1<<10)  Set 1 VMEBus short I/O space is D16 access
LBCTL_lbvmio_i1wp equ  (1<<9)   Set 1 Enables write posting on short I/O page
LBCTL_lbvmio_i1su equ  (1<<8)   AM for Short I/O page (1 = supervisor, 0 = user)

* ROM Control Register  (size, bank selectable speed)
*  The banks can be programmed for access speed independently.
*  The time CS* must be valid for the ROM bank is calculated as below:
*    (T*(Bc -1)-35)
*
*   Where: T is the bus clock period.
*          Bc programmed number of bus clocks.
*
*   Values for speed range 0 - 7 representing 11 - 4 bus clocks respectivly
*
m_LBCTL_romctl    equ  $FF mask for ROM control portion of the register
b_LBCTL_romsz     equ  6
m_LBCTL_romsz     equ  ($3<<b_LBCTL_romsz)
b_LBCTL_bspd      equ  3
m_LBCTL_rombsp    equ  ($7<<b_LBCTL_bspd)
b_LBCTL_aspd      equ  0
m_LBCTL_romasp    equ  ($7<<b_LBCTL_aspd)

*  size of ROMS permitted,  Note that the size determines both A bank end and
*    B bank Start.  The largest ROM Set should be in the A bank.
*    A bank starting address is always $FF8000000, it ends at (SIZE)
*   
LBCTL_SIZE_1M equ 3 1MBit Chips; B:$FF840000 to $FF87FFFF
LBCTL_SIZE_2M equ 2 2MBit Chips; B:$FF880000 to $FF8FFFFF
LBCTL_SIZE_4M equ 1 4MBit Chips; B:$FF900000 to $FF9FFFFF
LBCTL_SIZE_8M equ 0 8MBit Chips; B:$FFFA0000 to $FFFBFFFF

*  ROM access speeds
*
RSPD_11    equ   11  Bus Clocks
RSPD_10    equ   10  
RSPD_9     equ   9
RSPD_8     equ   8 
RSPD_7     equ   7
RSPD_6     equ   6
RSPD_5     equ   5 
RSPD_4     equ   4

*  DMAC -  VMEChip2 DMA Controller Registers
*
DMACgc          equ $30  This registers has control for 3 devices (see below)
b_DMACgc_ROM0   equ 20 Set 1 ROM at 0 and $FF800000 ,Set 0 for High only
DMACgc_ROM0     equ (1<<b_DMACgc_ROM0)
m_DMACgc_sc     equ (3<<18) Snoop Control for DMAC Table walk
b_DMACgc_sramsp equ 16  Number of clocks for an SRAM cycle
m_DMACgc_sramsp equ (3<<b_DMACgc_sramsp) ... and it's mask

*  allowable values for SRAM speed:
*
SRSPD6  equ  0  6 Bus Clocks
SRSPD5  equ  1  5 Bus Clocks
SRSPD4  equ  2  4 Bus Clocks
SRSPD3  equ  3  3 Bus Clocks

* Local Bus to VMEBus Requester Control Register
*
b_DMACgc_ROBN  equ 15 Arbiter Mode:  1 = round robin, 0 = priority 
DMACgc_ROBN    equ (1<<b_DMACgc_ROBN)
b_DMACgc_DHB   equ 14 When TRUE, DWB bit has grabbed control of VMEbus
DMACgc_DHB     equ (1<<b_DMACgc_DHB)
b_DMACgc_DWB   equ 13 When set 1, VmeChip grabs bus, set 0, releases it
DMACgc_DWB     equ (1<<b_DMACgc_DWB)
b_DMACgc_Fair  equ 11 When set 1, vme bus requester operates in fair mode
DMACgc_Fair    equ (1<<b_DMACgc_Fair)
DMACgc_RWD     equ (1<<10) When set 1, requester operates in release-when-done
*                          set 0, requester operates in release-on-request
b_DMACgc_RQL   equ 8
m_DMACgc_RQL   equ (3<<b_DMACgc_RQL)  Mask, values 0-3 define VME Req. Level

* DMAC Control Register 1
*
DMACgc_halt    equ (1<<7) Set 1 causes halt at end of curren command (chain)
DMACgc_den     equ (1<<6) Set 1 enables DMAC, Always reads 0!
DMACgc_dtbl    equ (1<<5) Set 0 = direct mode, set 1 = chain mode.
b_DMACgc_dfair equ 4 When set 1, dma requester operates in fair mode
DMACgc_dfair   equ (1<<b_DMACgc_dfair) Set 1 = Fair Mode, 0 = not Fair Mode
b_DMACgc_rl    equ 2
m_DMACgc_rel   equ (3<<b_DMACgc_rl) Determines release mechanism for DMAC

*  Allowable release mechanisms for the DMA controller
*     This device has an ON timer, these defs indicate how to use it
*     to throttle the DMA access to the VMEBus.
*
DMAREL_TMRQ  equ (0<<b_DMACgc_rl) Release when timer expired _and_ BRx* active
DMAREL_TME   equ (1<<b_DMACgc_rl) Release whenever timer expired.
DMAREL_RQ    equ (2<<b_DMACgc_rl) Release whenever BRx* is active (ROR)
DMAREL_RQTM  equ (3<<b_DMACgc_rl) Release when BRx* _or_ timer expired

b_DMACgc_rql    equ 0
m_DMACgc_rql    equ (3<<b_DMACgc_rql) Mask for Request leve (0-3 for level)


* DMACcr - DMA control Register 2
*    In direct mode, these bits define the DMAC action to take place.
*    In chain mode, these bits are dynamically defined in the chain
*       buffer packet for the current command being processed.
*
DMACcr         equ  $34

DMACcr_inte    equ  (1<<15) Chain mode: 1 = interrupt on packet done
b_DMACcr_snp   equ  13
m_DMACcr_snp   equ  (3<<b_DMACcr_snp) Defines snooper control bits
DMACcr_vinc    equ  (1<<11) Set 1 causes VME address counter to increment
DMACcr_linc    equ  (1<<10) Set 1 causes local address counter increment
DMACcr_tvme    equ  (1<<9)  Direction:  1 == To VME, 0 == From VME
DMACcr_D16     equ  (1<<8)  VME access size:  (1 = D16, 0 = D32)

b_DMACcr_blk   equ  6
m_DMACcr_blk   equ  (3<<b_DMACcr_blk) Block Transfer mode control (see below)
b_DMACcr_am    equ  0
m_DMACcr_am    equ  ($3F<<b_DMACcr_am) Use standard AM codes, DMAC will set
*                                        block mode bits if required!

* allowable block transfer modes.
*   Note:  the two active block transfer modes will use byte and word
*          access at the beginning and end of the transfers.
*
DMABT_DISABLE  equ 0 Block transfers disabled
DMABT_D32      equ 1 Block transfers using D32 size when possible
DMABT_D64      equ 3 Block transfers using D64 size when possible


* DMAC_lbac - DMAC Local Bus Address Counter
*
DMAC_lbac     equ  $38  32 Bit address on local bus


* DMAC_vbac - DMAC VMEBus Address Counter
*
DMAC_vbac     equ  $3C  32 bit address on VME bus

*
* DMAC_bcnt - DMAC Byte Count Register
*
DMAC_bcnt     equ  $40  32 bit byte count register

*
* DMAC_tac - DMAC Table Address Counter
*
DMAC_tac      equ  $44  32 bit addresss (Chain mode: address of command chain)

*
* DMAC command buffer.
*    In chain mode, this buffer allows the DMAC to walk a table of these
*    buffers.
*
DMAC_TBL_RES   equ   $0   First byte unused
DMAC_TBL_CTL   equ   $1   Control Register 2 image for this command
DMAC_TBL_LBA   equ   $2   Local Bus Address for this command
DMAC_TBL_VBA   equ   $6   VMEbus Address for this command
DMAC_TBL_CNT   equ   $A   Byte count for this command
DMAC_TBL_NXT   equ   $E   Next command buffer in the chain


* VMEICR - VMEBus Interrupter Control Register
*    Defines the Interrupt generated by the VMEBus interrupter circuit,
*    some of the DMA and MPU status and an interrupt vector register
*    for the interrupter circuit.
*      NOTE:   defined for LWORD access
*
VMEICR    equ  $48

* interrupter control register
*
b_VMEICR_irq1s    equ 29
m_VMEICR_irq1s    equ (3<<b_VMEICR_irq1s) Bits define functio of IRQ1 line
VMEICR_irqc       equ (1<<28)  In Broadcast mode, set 1 removes VMEBus irq.
VMEICR_irqs       equ (1<<27)  When High, IRQ has not been acknowledged.
b_VMEICR_irql     equ 24
m_VMEICR_irql     equ (7<<b_VMEICR_irql) Define level of IRQ generated
b_VMEICR_irqvct   equ 16
m_VMEICR_irqvct   equ ($FF<<b_VMEICR_irqvct) Vector presented durring IACK

* MPU Status and DMA Interrupt Count Register
*
b_VMEICR_dmaic   equ  12
m_VMEICR_dmaic   equ  ($F<<b_VMEICR_dmaic) DMA interrupt count register mask
b_VMEICR_mclr    equ  11 Set 1 to clear status bits 7,8,9,10
VMEICR_mclr      equ  (1<<b_VMEICR_mclr) Set 1 to clear status bits 7,8,9,10
VMEICR_mlbe      equ  (1<<10) When set, MPU recieved TEA, no add. status
VMEICR_mlpe      equ  (1<<9)  When set, MPU recieved TEA, parity error
VMEICR_mlob      equ  (1<<8)  When set, MPU recieved TEA, status offboard

* DMA status register
*
VMEICR_mlto      equ  (1<<7)  When set, MPU received TEA, local bus timeout
VMEICR_dlbe      equ  (1<<6)  When set, DMAC received TEA, no status
VMEICR_dlpe      equ  (1<<5)  When set, DMAC recieved TEA, parity error
VMEICR_dlob      equ  (1<<4)  When set, DMAC recieved TEA, status offboard
VMEICR_dlto      equ  (1<<3)  When set, DMAC recieved TEA, local bus timeout
VMEICR_tbl       equ  (1<<2)  When set, DMAC recieved error on local bus
*                               reading of command table.
VMEICR_vme       equ  (1<<1)  When set, DMAC received VME BERR durring transfer
VMEICR_done      equ  (1<<0)  When set, DMAC has completed commands, no error


* VMEARB - VMEBus arbiter, DMAC throttle, various bus timeouts, WD prescaler
*
VMEARB    equ   $4C

VMEARB_arbto     equ  (1<<24) If set 1, the arbiter will remove a grant from
*                               the bus if no BBSY seen after 256usec.
b_VMEARB_dmatof  equ  21
m_VMEARB_dmatof  equ  (7<<b_VMEARB_dmatof) DMA minimum time off the VMEbus
b_VMEARB_dmaton  equ  18
m_VMEARB_dmaton  equ  (7<<b_VMEARB_dmaton) DMA maxmum time on the VMEbus
b_VMEARB_gto     equ  16
m_VMEARB_gto     equ  (3<<b_VMEARB_gto) VMEbus global timeiou value

*  DMA throttle (DMA Maximum time ON)
*
DMA_ON16   equ  0   16 usec
DMA_ON32   equ  1   32 usec
DMA_ON64   equ  2   64 usec
DMA_ON128  equ  3   128 usec
DMA_ON256  equ  4   256 usec
DMA_ON512  equ  5   512 usec
DMA_ON1024 equ  6   1024 usec
DMA_ONTDN  equ  7   On until done or no data

*  DMA throttle (DMA Minimum time OFF)
*
DMA_OF0    equ  0   Never let go unless forced off the bus
DMA_OF16   equ  1   16 usec
DMA_OF32   equ  2   32 usec
DMA_OF64   equ  3   64 usec
DMA_OF128  equ  4   128 usec
DMA_OF256  equ  5   256 usec
DMA_OF512  equ  6   512 usec
DMA_OF1024 equ  7   1024 usec

*  VMEBus arbiter Global Timeout values
*
ARB_gto8   equ  0  global timeout at 8 usec
ARB_gto64  equ  1  global timeout at 64 usec
ARB_gto256 equ  2  global timeout at 128 usec
ARB_gtodis equ  3  global timeout disabled

* VME Access, Local Bus, and Watchdog timeout control
*
b_VMEARB_vato     equ 14
m_VMEARB_vato     equ (3<<b_VMEARB_vato)  VMEBus access timeout value
b_VMEARB_lbto     equ 12
m_VMEARB_lbto     equ (3<<b_VMEARB_lbto)  Local bus timeout value

b_VMEARB_wdto     equ 8
m_VMEARB_wdto     equ ($F<<b_VMEARB_wdto) Watchdog timeout value
*                                           values 512 usec - 64 sec
*                                           not normally used by OS9

* VMEBus Timeout values
*
VATO_64    equ   0   64 usec
VATO_1000  equ   1   1000 usec (1ms)
VATO_3200  equ   2   3200 usec (32 ms)
VATO_DIS   equ   3   Timeout Disabled

*  Local Bus Timeout values
*
LBTO_8     equ   0   8 usec 
LBTO_64    equ   1   64 usec
LBTO_256   equ   2   256 usec
LBTO_DIS   equ   3   Timeout Disabled

*  Counter Prescaler register:
*
*   The various clocks in the VMEChip2 are dependent on BCLK.
*   This register must be programmed based on the following formula:
*
*   VMEARB_psctl = 256 - BCLK(MHZ)
*
*   NOTE:  $FF is an illegal value!!!
*  
b_VMEARB_psctl    equ   0
m_VMEARB_psctl    equ   ($F<<b_VMEARB_psctl)  prescaler mask


* VMETT - Tick Timer counters and compare registers
*
VMETT_tt1cmp   equ $50  Timer 1 compare register
VMETT_tt1cnt   equ $54  Timer 1 count register
VMETT_tt2cmp   equ $58  Timer 2 compare register
VMETT_tt2cnt   equ $5C  Timer 2 count register


* BDCTL - Board Control register
*  Note: defined for LWORD access
*
BDCTL          equ $60
b_BDCTL_scon   equ 30  if set, VMEChip is configured as system controller
b_BDCTL_sffl   equ 29  if set, VMEBus sysfail is asserted
b_BDCTL_brfli  equ 28  if set, BRDFAIL on VMEChip2 is asserted
b_BDCTL_purs   equ 27  set on power up reset, cleared by.....
b_BDCTL_cpur   equ 26  ...Set 1 to clear power up reset status
b_BDCTL_brflo  equ 25  if set, contributes to BRDFAIL
b_BDCTL_rswe   equ 24  if set, indicates RESET switch is enabled.


* WDTCR - Watchdog timer Control Register
*  Note: defined for LWORD access (BDCTL)
*
b_WDTCR_srst  equ  23 Set 1 generates SYSRESET to VMEBus
b_WDTCR_wdcs  equ  22 Set 1 to clear watchdog timeout status
b_WDTCR_wdcc  equ  21 Set 1 to reset watchdog counter
b_WDTCR_wdto  equ  20 If found 1, a watchdog timeout has occurred
b_WDTCR_wdbfe equ  19 If found 1, t.o. has occured, BRDFAIL asserted
b_WDTCR_wds   equ  18 If found 1, sysreset has been asserted
b_WDTCR_wdrse equ  17 When low, this bit indicates timeout occured
b_WDTCR_wden  equ  16 Watchdog timer enable.


* TT2CTL - Tick Timer 2 Control Register
*   NOTE: defined for LWORD access (BDCTL)
*
b_TT2CTL_ovfct    equ   12
m_TT2CTL_ovfct    equ   ($F<<b_TT2CTL_ovfct) Timer 2 overflow counter
b_TT2CTL_covf     equ   10  Set 1, counter reset to zero at compare
b_TT2CTL_coc      equ   9  Set 1, clears overflow counter
b_TT2CTL_en       equ   8  Set 1, enables counter


* TT1CTL - Tick Timer 1 Control Register
*   NOTE: defined for LWORD access (BDCTL)
*
b_TT1CTL_ovfct    equ   4
m_TT1CTL_ovfct    equ   ($F<<b_TT1CTL_ovfct) Timer 1 overflow counter
b_TT1CTL_covf     equ   2  Set 1, counter reset to zero at compare
b_TT1CTL_coc      equ   1  Set 1, clears overflow counter
b_TT1CTL_en       equ   0  Set 1, enables counter


* PRESCL - VMEChip2 prescaler register
*
PRESCL    equ  $64


*****
*  These 4 registers are arranged in such a way as to allow the definition
*  of the interrupt status, enable, and clear to be defined as below.
*  If a bit is defined in any given register it's bit possition is
*  common through-out the 4 register set.
*   NOTE:  Defined for LWORD access to the ports
*
* LBISR - Local Bus Interrupter Status Register   [A]
*
LBISR        equ   $68

* LBIER - Local Bus Interrupter Enable Register   [B]
*
LBIER        equ   $6C

* SSWINT - Software Interrupt Set Register        [C]
*
SSWINT       equ   $70

* INTCLR - Interrupt Clear Register               [D]
*
INTCLR       equ   $74

* VMEBus ACFAIL [A,B,D]
*
b_IRQ_acf   equ   31
m_IRQ_acf   equ   (1<<b_IRQ_acf)

* Abort Pushbutton [A,B,D]
*
b_IRQ_ab    equ   30
m_IRQ_ab    equ   (1<<b_IRQ_ab)

* VMEBus SYSFAIL [A,B,D]
*
b_IRQ_sysf  equ   29
m_IRQ_sysf  equ   (1<<b_IRQ_sysf)

* VMEBus Master Write Post error [A,B,D]
*
b_IRQ_mwp   equ   28
m_IRQ_mwp   equ   (1<<b_IRQ_mwp)

* External Interrupt (Parity Error) [A,B,D]
*
b_IRQ_pe    equ   27
m_IRQ_pe    equ   (1<<b_IRQ_pe)

* VMEBus IRQ1 edge-sensitive interrupt [A,B,D]
*
b_IRQ_vi1e  equ   26
m_IRQ_vi1e  equ   (1<<b_IRQ_vi1e)

* Tick Timer 2 [A,B,D]
*
b_IRQ_tic2  equ   25
m_IRQ_tic2  equ   (1<<b_IRQ_tic2)

* Tick Timer 1 [A,B,D]
*
b_IRQ_tic1  equ   24
m_IRQ_tic1  equ   (1<<b_IRQ_tic1)

* VMEBus interrupter acknowledge  [A,B,D]
*
b_IRQ_via   equ   23
m_IRQ_via   equ   (1<<b_IRQ_via)

* DMAC [A,B,D]
*
b_IRQ_dma   equ   22
m_IRQ_dma   equ   (1<<b_IRQ_dma)

* GCSR SIG3 [A,B,D]
*
b_IRQ_sig3  equ   21
m_IRQ_sig3  equ   (1<<b_IRQ_sig3)

* GCSR SIG2 [A,B,D]
*
b_IRQ_sig2  equ   20
m_IRQ_sig2  equ   (1<<b_IRQ_sig2)

* GCSR SIG1 [A,B,D]
*
b_IRQ_sig1  equ   19
m_IRQ_sig1  equ   (1<<b_IRQ_sig1)

* GCSR SIG0 [A,B,D]
*
b_IRQ_sig0  equ   18
m_IRQ_sig0  equ   (1<<b_IRQ_sig0)

* Location monitor 1 [A,B,D]
*
b_IRQ_lm1   equ   17
m_IRQ_lm1   equ   (1<<b_IRQ_lm1)

* Location monitor 0 [A,B,D]
*
b_IRQ_lm0   equ   16
m_IRQ_lm0   equ   (1<<b_IRQ_lm0)

* Software Interrupt 7 [A,B,C,D]
*
b_IRQ_sw7   equ   15
m_IRQ_sw7   equ   (1<<b_IRQ_sw7)

* Software Interrupt 6 [A,B,C,D]
*
b_IRQ_sw6   equ   14
m_IRQ_sw6   equ   (1<<b_IRQ_sw6)

* Software Interrupt 5 [A,B,C,D]
*
b_IRQ_sw5   equ   13
m_IRQ_sw5   equ   (1<<b_IRQ_sw5)

* Software Interrupt 4 [A,B,C,D]
*
b_IRQ_sw4   equ   12
m_IRQ_sw4   equ   (1<<b_IRQ_sw4)

* Software Interrupt 3 [A,B,C,D]
*
b_IRQ_sw3   equ   11
m_IRQ_sw3   equ   (1<<b_IRQ_sw3)

* Software Interrupt 2 [A,B,C,D]
*
b_IRQ_sw2   equ   10
m_IRQ_sw2   equ   (1<<b_IRQ_sw2)

* Software Interrupt 1 [A,B,C,D]
*
b_IRQ_sw1   equ   9
m_IRQ_sw1   equ   (1<<b_IRQ_sw1)

* Software Interrupt 0 [A,B,C,D]
*
b_IRQ_sw0   equ   8
m_IRQ_sw0   equ   (1<<b_IRQ_sw0)

* Spare:  This bit unused!!!
*
b_IRQ_spre  equ   7
m_IRQ_spre  equ   (1<<b_IRQ_spre)

* VMEBus IRQ Level 7 [A,B]
*
b_IRQ_vme7   equ   6
m_IRQ_vme7   equ   (1<<b_IRQ_vme7)

* VMEBus IRQ Level 6 [A,B]
*
b_IRQ_vme6   equ   5
m_IRQ_vme6   equ   (1<<b_IRQ_vme6)

* VMEBus IRQ Level 5 [A,B]
*
b_IRQ_vme5   equ   4
m_IRQ_vme5   equ   (1<<b_IRQ_vme5)

* VMEBus IRQ Level 4 [A,B]
*
b_IRQ_vme4   equ   3
m_IRQ_vme4   equ   (1<<b_IRQ_vme4)

* VMEBus IRQ Level 3 [A,B]
*
b_IRQ_vme3   equ   2
m_IRQ_vme3   equ   (1<<b_IRQ_vme3)

* VMEBus IRQ Level 2 [A,B]
*
b_IRQ_vme2   equ   1
m_IRQ_vme2   equ   (1<<b_IRQ_vme2)

* VMEBus IRQ Level 1 [A,B]
*
b_IRQ_vme1  equ   0
m_IRQ_vme1  equ   (1<<b_IRQ_vme1)

* special mask:  mask the VME IRQ enable bits
*
m_VMEIRQ_en equ $7f


* ILVLRx  - Interrupt Level Registers 1 through 4
*  Use these registers to assign the level for each of the VMEChip interrupts.
*  The vector that is presented at the time of the interrupt is programmed
*  into the VMEChip via the IRQVBR register (defined below).  In the comments
*  below the vector generated is defined:
*       [ xN ]      vector base register 0
*       [ yN ]      vector base register 1
*       [ ext]      generated by the IRQ source
*
*  The VMEChip internal circuitry defines a priority for these interrupts
*  internally.  The order of the definitions of the interrupts below
*  reflect this internal priority.
*      VMEBus ACFAIL is highest
*      Abort Switch  is second
*          .
*          .
*      VMEBus IRQ1 lowest
*

***
ILVLR1       equ  $78   Interrupt Level Register 1

* VMEBus ACFAIL [ $xF ]
*
b_ILVLR1_acf  equ   28
m_ILVLR1_acf   equ   (7<<b_ILVLR1_acf)

* Abort Pushbutton [ $xE ]
*
b_ILVLR1_ab   equ   24
m_ILVLR1_ab    equ   (7<<b_ILVLR1_ab)

* VMEBus SYSFAIL  [ $xD ]
*
b_ILVLR1_sysf equ   20
m_ILVLR1_sysf  equ   (7<<b_ILVLR1_sysf)

* VMEBus Master Write Post error  [ $xC ]
*
b_ILVLR1_mwp  equ   16
m_ILVLR1_mwp   equ   (7<<b_ILVLR1_mwp)

* External Interrupt (Parity Error) [ $xB ]
*
b_ILVLR1_pe   equ   12
m_ILVLR1_pe    equ   (7<<b_ILVLR1_pe) 

* VMEBus ILVLR1 edge-sensitive interrupt  [ $xA ]
*
b_ILVLR1_vi1e equ   8
m_ILVLR1_vi1e  equ   (7<<b_ILVLR1_vi1e)

* Tick Timer 2    [ $x9 ]
*
b_ILVLR1_tic2 equ   4
m_ILVLR1_tic2  equ   (7<<b_ILVLR1_tic2)

* Tick Timer 1    [ $x8 ]
*
b_ILVLR1_tic1 equ   0
m_ILVLR1_tic1  equ   (7<<b_ILVLR1_tic1)

***  
ILVLR2        equ   $7C    Interrupt Level Register 2

* VMEBus interrupter acknowledge    [ $x7 ]
*
b_ILVLR2_via  equ   28
m_ILVLR2_via   equ   (7<<b_ILVLR2_via)

* DMAC    [ $x6 ]
*
b_ILVLR2_dma  equ   24
m_ILVLR2_dma   equ   (7<<b_ILVLR2_dma)

* GCSR SIG3  [ $x5 ]
*
b_ILVLR2_sig3 equ   20
m_ILVLR2_sig3  equ   (7<<b_ILVLR2_sig3)

* GCSR SIG2  [ $x4 ]
*
b_ILVLR2_sig2 equ   16
m_ILVLR2_sig2  equ   (7<<b_ILVLR2_sig2)

* GCSR SIG1  [ $x3 ]
*
b_ILVLR2_sig1 equ   12
m_ILVLR2_sig1  equ   (7<<b_ILVLR2_sig1)

* GCSR SIG0  [ $x2 ]
*
b_ILVLR2_sig0 equ   8
m_ILVLR2_sig0  equ   (7<<b_ILVLR2_sig0)

* Location monitor 1   [ $x1 ]
*
b_ILVLR2_lm1  equ   4
m_ILVLR2_lm1   equ   (7<<b_ILVLR2_lm1)

* Location monitor 0   [ $x0 ]
*
b_ILVLR2_lm0  equ   0
m_ILVLR2_lm0   equ   (7<<b_ILVLR2_lm0)

***
ILVLR3        equ   $80   Interrupt Level Register 3  

* Software Interrupt 7   [ $yF ]
*
b_ILVLR3_sw7  equ   28
m_ILVLR3_sw7   equ   (7<<b_ILVLR3_sw7)

* Software Interrupt 6   [ $yE ]
*
b_ILVLR3_sw6  equ   24
m_ILVLR3_sw6   equ   (7<<b_ILVLR3_sw6)

* Software Interrupt 5   [ $yD ]
*
b_ILVLR3_sw5  equ   20
m_ILVLR3_sw5   equ   (7<<b_ILVLR3_sw5)

* Software Interrupt 4   [ $yC ]
*
b_ILVLR3_sw4  equ   16
m_ILVLR3_sw4   equ   (7<<b_ILVLR3_sw4)

* Software Interrupt 3   [ $yB ]
*
b_ILVLR3_sw3  equ   12
m_ILVLR3_sw3   equ   (7<<b_ILVLR3_sw3)

* Software Interrupt 2   [ $yA ]
*
b_ILVLR3_sw2  equ   8
m_ILVLR3_sw2   equ   (7<<b_ILVLR3_sw2)

* Software Interrupt 1   [ $y9 ]
*
b_ILVLR3_sw1  equ   4
m_ILVLR3_sw1   equ   (7<<b_ILVLR3_sw1)

* Software Interrupt 0   [ $y8 ]
*
b_ILVLR3_sw0  equ   0
m_ILVLR3_sw0   equ   (7<<b_ILVLR3_sw0)

***
ILVLR4        equ   $84   Interrupt Level Register 4

* Spare:  This bit unused!!!   [ $y7 ]
*
b_ILVLR4_spre equ   28
m_ILVLR4_spre  equ   (7<<b_ILVLR4_spre)

* VMEBus Level 7  [ ext ]
*
b_ILVLR4_vme7  equ   24
m_ILVLR4_vme7   equ   (7<<b_ILVLR4_vme7)

* VMEBus Level 6  [ ext ]
*
b_ILVLR4_vme6  equ   20
m_ILVLR4_vme6   equ   (7<<b_ILVLR4_vme6)

* VMEBus Level 5  [ ext ]
*
b_ILVLR4_vme5  equ   16
m_ILVLR4_vme5   equ   (7<<b_ILVLR4_vme5)

* VMEBus Level 4  [ ext ]
*
b_ILVLR4_vme4  equ   12
m_ILVLR4_vme4   equ   (7<<b_ILVLR4_vme4)

* VMEBus Level 3  [ ext ]
*
b_ILVLR4_vme3  equ   8
m_ILVLR4_vme3   equ   (7<<b_ILVLR4_vme3)

* VMEBus Level 2  [ ext ]
*
b_ILVLR4_vme2  equ   4
m_ILVLR4_vme2   equ   (7<<b_ILVLR4_vme2)

* VMEBus Level 1  [ ext ]
*
b_ILVLR4_vme1  equ   0
m_ILVLR4_vme1   equ   (7<<b_ILVLR4_vme1)


* IRQVBR - This is the vector base register for the VMEChip Interrupts
*      Note:  Defined as BYTE access.
*
*   The vectors that the VMEChip will present to the processor on behalf
*   of the interrupts defined above are a function of this register.
*   (See the comments under ILVLRx above)
*
IRQVBR     equ   $88

b_IRQVBR_vbr0  equ  28
m_IRQVBR_vbr0  equ  ($F<<b_IRQVBR_vbr0)

b_IRQVBR_vbr1  equ  24
m_IRQVBR_vbr1  equ  ($F<<b_IRQVBR_vbr1)


* IOCTLR - I/O Control Register 1 - 3
*
IOCTLR    equ    $88

b_IOCTLR_mien  equ  23  Master IRQ Control (0 = Disabled, 1 = Enabled)
b_IOCTLR_sysfl equ  22  SYSFAIL status (1 = asserted, 0 = not active )
b_IOCTLR_acfl  equ  21  ACFAIL  status  (1 = asserted, 0 = not active )
b_IOCTLR_abrtl equ  20  Abort Switch ( 1 = pressed, 0 = not pressed ) 

*  The next 4 bits set the direction for GPIOx lines
*
b_IOCTLR_gpioen3  equ  19  Set GPIO3 direction (0 = input, 1 = output)
b_IOCTLR_gpioen2  equ  18  Set GPIO2 direction (0 = input, 1 = output)
b_IOCTLR_gpioen1  equ  17  Set GPIO1 direction (0 = input, 1 = output)
b_IOCTLR_gpioen0  equ  16  Set GPIO0 direction (0 = input, 1 = output)

*  If GPIOx lines defined as outputs,  these next 4 set the level
*
b_IOCTLR_gpioset3 equ  15  Set GPIO3 level (1 = HIGH, 0 = LOW)
b_IOCTLR_gpioset2 equ  14  Set GPIO2 level (1 = HIGH, 0 = LOW)
b_IOCTLR_gpioset1 equ  13  Set GPIO1 level (1 = HIGH, 0 = LOW)
b_IOCTLR_gpioset0 equ  12  Set GPIO0 level (1 = HIGH, 0 = LOW)

*  These next 4 bits reflect the current condition of the GPIO lines
*
b_IOCTLR_gpiolv3  equ  11  GPIO3:  1 == HIGH,  0 == 0
b_IOCTLR_gpiolv2  equ  10  GPIO2:  1 == HIGH,  0 == 0
b_IOCTLR_gpiolv1  equ  9   GPIO1:  1 == HIGH,  0 == 0
b_IOCTLR_gpiolv0  equ  8   GPIO0:  1 == HIGH,  0 == 0

*  These next 8 bits read the board jumper block (J1)
*
b_IOCTLR_gpin7    equ  7   GPI7 jumper: 1 == off, 0 == on
b_IOCTLR_gpin6    equ  6   GPI6 jumper: 1 == off, 0 == on
b_IOCTLR_gpin5    equ  5   GPI5 jumper: 1 == off, 0 == on
b_IOCTLR_gpin4    equ  4   GPI4 jumper: 1 == off, 0 == on
b_IOCTLR_gpin3    equ  3   GPI3 jumper: 1 == off, 0 == on
b_IOCTLR_gpin2    equ  2   GPI2 jumper: 1 == off, 0 == on
b_IOCTLR_gpin1    equ  1   GPI1 jumper: 1 == off, 0 == on
b_IOCTLR_gpin0    equ  0   GPI0 jumper: 1 == off, 0 == on


* MISCCR - Miscellaneous Control Register
*
MISCCR		equ		$8C

b_MISCCR_mpirqen	equ		7
b_MISCCR_reverom	equ		6
b_MISCCR_dissram	equ		5	Disable SRAM decoder
b_MISCCR_dismt		equ		4
b_MISCCR_noelbbsy	equ		3
b_MISCCR_disbsyt	equ		2	
b_MISCCR_enint		equ		1	Enable local bus interrupt filters
b_MISCCR_disbgn		equ		0	Disable BGIN filters


********************************************************
**  VMEChip2  GCSR - Global Status and Control Register
**
*   The GCSR appears both on the local bus (fixed address) and
*   at a location in the VMEBus map (determined by GCSR above).
*   From the Local Bus, the GCSR registers appear as the low 16 bits of
*   7 consecutive LWORD addresses.  From the VMEBus the GCSR appears
*   as 16 consecutive WORD locations.  The bit definitions are thus
*   valid for both views of the register, only the offsets change.
*
 ifndef GCSRBase
GCSRBase equ  $FFF40100
 endc


* GCSRR -   Revision/ID register offset
*
GCSRR          equ  0   Revision/ID register offset

b_GCSRR_rev    equ  8   Chip Revision Register 
m_GCSRR_rev    equ  ($FF<<b_GCSRR_rev)   Chip Revision Register
b_GCSRR_id     equ  0
m_GCSRR_id     equ  ($FF<<b_GCSRR_id)    Chip ID Register  [$10]


*  GCSRCTRL - Location monitor, Signal, board status and control
*
GCSRCTRL       equ  4

b_GCSRCTRL_lm3   equ  15  Location Monitor 3 
b_GCSRCTRL_lm2   equ  14  Location Monitor 2
b_GCSRCTRL_lm1   equ  13  Location Monitor 1
b_GCSRCTRL_lm0   equ  12  Location Monitor 0
b_GCSRCTRL_sig3  equ  11  Signal 3
b_GCSRCTRL_sig2  equ  10  Signal 2
b_GCSRCTRL_sig1  equ  9   Signal 1  
b_GCSRCTRL_sig0  equ  8   Signal 0

b_GCSRCTRL_rst   equ  7   If set by VMEBus master, will cause local bus reset!
b_GCSRCTRL_isf   equ  6   If set, inhibits VMEChip from driving SYSFAIL
b_GCSRCTRL_bf    equ  5   If found high, this board has Board Fail active
b_GCSRCTRL_scon  equ  4   If found high, this board is system controller
b_GCSRCTRL_sysfl equ  3   If found high, this board is driving SYSFAIL
*  Note:  Bits 0, 1, and 2 are unused.


*  GCSRGP0 - GCSRGP5 are general purpose registers for inter-processor 
*    communications,  they have no defined funtion.
*
GCSRGP0   equ    $8
GCSRGP1   equ    $C
GCSRGP2   equ    $10
GCSRGP3   equ    $14
GCSRGP4   equ    $18
GCSRGP5   equ    $1C


*  These are the VMEBus offsets to the GCSR registers.
*
v_GCSRR     equ $0   Revision/ID register offset
v_GCSRCTRL  equ $2   Control/Status, Location monitors
v_GCSRGP0   equ $4   General purpose registers 0 - 5
v_GCSRGP1   equ $6
v_GCSRGP2   equ $8
v_GCSRGP3   equ $A
v_GCSRGP4   equ $C
v_GCSRGP5   equ $E


*   These are the standard address modifier codes for the VMEBus
*

* A24 operations
*
AM_SupPgm      equ  $3E  Supervisory Program
AM_SupDat      equ  $3D  Supervisory Data

AM_UsrPgm      equ  $3A  Non-Priviledged Program
AM_UsrDat      equ  $39  Non-Priviledged Data

*  A32 operations
*
AM_SupPgmExt   equ  $0E  Extended Supervisory Program
AM_SupDatExt   equ  $0D  Extended Supervisory Data

AM_UsrPgmExt   equ  $0A  Extended Non-Priviledged Program
AM_UsrDatExt   equ  $09  Extended Non-Priviledged Data

* end of "vmechip2.d"
 opt l
