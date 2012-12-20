
********************
* Definitions for PCC device - VME147
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  88/08/10  history starts.                                  wwb
* 02  88/11/28  made DMALevel same as SCSILevel.                 wwb
* 03  89/11/06  fixed typo (gcpr_parmask --> gpcr_parmask).      wwb
*               ---- OS-9/68K V2.3 Release ----
*               ---- OS-9/68K V2.4 Release ----
* 


********************
* System Interrupt Structure
*

* Interrupt Levels

LANCELevel equ 5 PCC LANCE level
SCSILevel equ 4 PCC SCSI port level
DMALevel equ 4 PCC SCSI DMA level
Timer1Level equ 6 PCC Timer 1 level
Timer2Level equ 6 PCC Timer 2 level
SWI1Level equ 3 PCC SWI 1 level
SWI2Level equ 2 PCC SWI 2 level

* Interrupt Vector assignments

PCCVect equ 64 PCC Base Vector (uses 64 - 79)

ACFAILVect equ PCCVect+0 ACFAIL* vector
BERRVect equ PCCVect+1 BERR* vector
ABORTVect equ PCCVect+2 ABORT vector

LANCEVect equ PCCVect+4 LANCE vector
SCSIVect equ PCCVect+5 PCC SCSI Port vector
DMAVect equ PCCVect+6 PCC SCSI DMA vector
PVect equ PCCVect+7 PCC Printer vector
Timer1Vect equ PCCVect+8 PCC Timer 1 vector
Timer2Vect equ PCCVect+9 PCC Timer 2 vector
SWI1Vect equ PCCVect+10 PCC SWI 1 vector
SWI2Vect equ PCCVect+11 PCC SWI 2 vector

 pag
********************
* PCC:  DMA definitions
*

* register offset definitions (from PCCBase)

dma_table equ 0 dma table address register
dma_data equ 4 dma data address register
dma_lcount equ 8 dma link/dfc/byte-count register
dma_dhold equ 12 dma data holding register
dma_irq equ $20 dma interrupt control register
dma_ctst equ $21 dma control/status register
dma_stat equ $23 dma status register
dma_tfc equ $25 dma table function code register

* dma link/dfc/byte-count register definitions

b_dlc_link equ 31 bit number of link bit
dlc_dfcmask equ %111<<24 dfc mask
dlc_count equ $00ffffff byte-count mask

* dma interrupt control register definitions

dirq_ilmask equ %00000111 interrupt level mask
dirq_enab equ 1<<3 dma irq enable
dirq_irq equ 1<<7 dma irq pending

* dma control/status register definitions

dcs_enab equ 1<<0 dma enable
dcs_tw equ 1<<1 table walk mode
dcs_ms equ 1<<2 transfer direction = memory-to-scsi
dcs_tberr equ 1<<3 bus error on table access
dcs_dberr equ 1<<4 bus error on data transfer
dcs_tsiz equ 1<<5 access to non-long-aligned table entry
dcs_8bit equ 1<<6 port indicates size is 8-bit
dcs_done equ 1<<7 dma completed

* dma status register definitions

dst_ll equ 1<<0 data valid: low word, low byte
dst_lm equ 1<<1 data valid: low word, middle byte
dst_um equ 1<<2 data valid: upper word, middle byte
dst_uu equ 1<<3 data valid: upper word, upper byte
dst_valmask equ dst_uu!dst_um!dst_lm!dst_ll data valid mask bits
dst_inc1 equ 1<<4 address incremented by 1
dst_inc2 equ 1<<5 address incremented by 2
dst_inc3 equ 1<<6 address incremented by 3
dst_inc4 equ 1<<7 address incremented by 4
dst_incmask equ dst_inc4!dst_inc3!dst_inc2!dst_inc1 increment address mask

* dma table address function codes register definitions

dtf_fcmask equ %00000111 function code mask

 pag
********************
* PCC:  Timer definitions
*

* register offset definitions (from PCCBase)

tim1_preload equ $10 timer 1 preload
tim1_count equ $12 timer 1 counter
tim1_irq equ $18 timer 1 irq control
tim1_ctl equ $19 timer 1 control
tim2_preload equ $14 timer 2 preload
tim2_count equ $16 timer 2 counter
tim2_irq equ $1a timer 2 irq control
tim2_ctl equ $1b timer 2 control

* timer interrupt control register definitions (both timers)

tirq_ilmask equ %00000111 timer irq level mask
tirq_enab equ 1<<3 timer irq enable
tirq_irq equ 1<<7 timer irq pending

* timer control register definitions (both timers)

tct_enab equ 1<<0 enable timer
tct_cntenab equ 1<<1 counter enable
tct_clrovf equ 1<<2 clear overflow counter
tct_ovflmask equ %11110000 overflow counters mask


********************
* PCC:  ACFAIL definitions
*
ACFAILCtl equ PCCBase+$1c ACFAIL irq control base address

acf_enab equ 1<<3 ACFAIL* irq enable
acf_state equ 1<<6 VMEbus ACFAIL* asserted
acf_irq equ 1<<7 ACFAIL* irq pending


********************
* PCC:  Watch Dog Timer definitions
*
WDTCtl equ PCCBase+$1d WDT Control base address

wdt_enab equ 1<<0 WDT enable
wdt_wdclr equ 1<<1 WDT clear
wdt_wdrst equ 1<<2 WDT activates reset
wdt_wdto equ 1<<3 WDT time-out
wdt_limask equ %11110000 WDT limit mask


********************
* PCC:  Printer definitions
*

* register offset definitions (from PBase address, NOT PCCBase)

p_irq equ PCCBase+$1e-PBase printer interrupt control register
p_ctl equ PCCBase+$1f-PBase printer control register
p_data equ 0 printer data register
p_status equ 0 printer status register

* printer interrupt control definitions

pirq_ilmask equ %00000111 printer irq level mask
pirq_enab equ 1<<3 printer irq enable
pirq_ackpol equ 1<<4 falling edge of ACK* generates irq
pirq_ackint equ 1<<5 ACK* irq pending
pirq_faltint equ 1<<6 FAULT* irq pending
pirq_irq equ 1<<7 printer irq pending

* printer control definitions

pct_mode equ 1<<0 manual strobe mode
pct_stbtim equ 1<<1 auto-mode strobe time = 1.6uS (else 6.4uS)
pct_strobe equ 1<<2 assert strobe (manual mode)
pct_prime equ 1<<3 assert input prime

* printer status definitions

pst_bsy equ 1<<3 BUSY asserted
pst_pe equ 1<<4 Paper Error asserted
pst_select equ 1<<5 printer is selected
pst_fault equ 1<<6 printer FAULT asserted
pst_ack equ 1<<7 printer ACK asserted


********************
* PCC:  Bus Error Interrupt Control definitions
*
BERRCtl equ PCCBase+$22 BERR control base address

berr_enab equ 1<<3 BERR* irq enable
berr_irq equ 1<<7 BERR* irq pending


********************
* PCC:  ABORT Interrupt Control definitions
*
ABORTCtl equ PCCBase+$24 ABORT control base address

b_abt_abt equ 6 bit # of "abort switch depressed"

abt_enab equ 1<<3 ABORT irq enable
abt_abt equ 1<<b_abt_abt ABORT switch depressed
abt_irq equ 1<<7 ABORT irq pending


********************
* PCC:  Serial Port Interrupt Control definitions
*
SerIRQCtl equ PCCBase+$26 serial port irq control base address

sirq_ilmask equ %00000111 serial port irq level mask
sirq_enab equ 1<<3 enable serial port irq
sirq_int equ 1<<4 vector from PCC (else serial chip)
sirq_irq equ 1<<7 serial port irq pending


********************
* PCC:  General Purpose Control Register definitions
*
GPCRCtl equ PCCBase+$27 GPCR base address

gpcr_paroff equ %00000000 local ram parity checking off
gpcr_paron equ %00000001 local ram parity on, BERR in current cycle
gpcr_paroff2 equ %00000010 local ram parity checking off
gpcr_paronwp equ %00000011 local ram parity on, BERR next cycle
gpcr_parmask equ gpcr_paronwp parity select mask
gpcr_wwpar equ 1<<2 write-wrong parity
gpcr_ltoenab equ 1<<3 local bus t/o enable (NOT USED)
gpcr_mintenab equ 1<<4 master irq enable
gpcr_disrst equ %101<<5 disable reset switch

gpcr_cold set gpcr_paron parity ON, all irqs OFF, reset switch ON
*gpcr_cold set gpcr_paroff parity OFF, all irqs OFF, reset switch ON


********************
* PCC:  LANCE Interrupt Control definitions
*
LANCECtl equ PCCBase+$28 LANCE interrupt control address

lan_ilmask equ %00000111 LANCE irq level mask
lan_enab equ 1<<3 LANCE irq enable
lan_irq equ 1<<7 LANCE irq pending


********************
* PCC:  General Purpose Status Register definitions
*
GPSRCtl equ PCCBase+$29 GPSR base address

b_gpsr_par equ 0 bit # of parity status
b_gpsr_res equ 1 bit # of power-up status
gpsr_par equ 1<<b_gpsr_par parity error on local ram
gpsr_res equ 1<<b_gpsr_res power-up reset occurred


********************
* PCC:  SCSI Port Interrupt Control definitions
*
SCSICtl equ PCCBase+$2a SCSI irq control base address

scsi_ilmask equ %00000111 SCSI irq level mask
scsi_enab equ 1<<3 SCSI irq enable
scsi_rst equ 1<<4 assert SCSI reset signal
scsi_rststate equ 1<<5 SCSI reset signal is asserted
scsi_rstirq equ 1<<6 SCSI reset irq pending
scsi_irq equ 1<<7 SCSI irq pending


********************
* PCC:  Slave Base Address Register definitions
*
SlvAddCtl equ PCCBase+$2b vme slave base address control

slv_a24mask equ %11<<0 vme slave A24 mask
slv_a32mask equ %11<<2 vme slave A32 mask
slv_base0 equ 0 onboard ram base address is $00000000
slv_base1 equ 1 onboard ram base address is 1 x DRAMSize
slv_base2 equ 2 onboard ram base address is 2 x DRAMSize
slv_base3 equ 3 onboard ram base address is 3 x DRAMSize
slv_waitrmc equ 1<<5 wait for vme mastership before MARMC cycles
slv_lanmask equ %11<<6 LANCE memory segment mask
slv_quad0 equ 0 LANCE accesses $00000000 - $00ffffff (1st 16Meg)
slv_quad1 equ 1 LANCE accesses $01000000 - $01ffffff (2nd 16Meg)
slv_quad2 equ 2 LANCE accesses $02000000 - $02ffffff (3rd 16Meg)
slv_quad3 equ 3 LANCE accesses $03000000 - $03ffffff (4th 16Meg)


********************
* PCC:  Software Interrupt Control Register definitions
*
SWI1Ctl equ PCCBase+$2c SWI 1 control register
SWI2Ctl equ PCCBase+$2e SWI 2 control register

swi_ilmask equ %00000111 irq level mask
swi_enab equ 1<<3 swi irq enable
swi_irq equ 1<<7 swi irq pending


********************
* PCC:  Interrupt Base Vector definitions
*
PCCVctCtl equ PCCBase+$2d PCC vector register

ivb_mask equ %11110000 PCC base vector mask

* end of 'pcc.d'

