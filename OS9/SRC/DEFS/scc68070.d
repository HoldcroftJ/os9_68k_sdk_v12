*
* SCC68070 On-Chip Peripheral Definitions
*
 opt -l
 ttl SCC68070 Peripheral definitions
 pag
********************
* Edition History
*   date    comments                                            by
* -------- ---------------------------------------------------- ---
* 88/02/22 created                                              wwb
* 88/03/14 made DCR 5 be defined for both channels.             wwb
*          ---- OS-9/68K V2.2 Release ---- 
* 88/03/21 added label for PICR2 as offset of UART070           jal
* 88/03/24 fixed UART register offsets (doc was wrong)          jal
* 89/03/03 fixed LIR address (manual says $80001000, but it     wwb
*          really is $80001001).
*           ---- OS-9/68K V2.3 Release ---- 
*           ---- CD-RTOS V1.1 Release ---- 
*           ---- OS-9/68K V2.4 Release ---- 
*

********************
* Latched Interrupt Priority Register (LIR)
*
LIR equ $80001001 LIR register address

* LIR formats

* LIR bit numbers

b_in1_pir equ 7 pending irq reset (IN1)
b_in2_pir equ 3 pending irq reset (IN2)

* LIR absolute values

in1_pir equ 1<<b_in1_pir reset pending IN1 irq
in1_ipl_mask equ %01110000 IN1 ipl mask
in2_pir equ 1<<b_in2_pir reset pending IN2 irq
in2_ipl_mask equ %00000111 IN2 ipl mask

 pag
********************
* I2C Interface
*
I2C equ $80002001 I2C interface base address

* I2C Interface Register offsets

i2dr equ 0 i2c data register
i2ar equ 2 i2c address register
i2sr equ 4 i2c status register
i2cr equ 6 i2c control register
i2cc equ 8 i2c clock control register

* I2C Status Register formats

* Status Register bit numbers

b_i2sr_mst equ 7 master/slave mode
b_i2sr_trx equ 6 transmitter/receiver mode
b_i2sr_bb equ 5 bus busy
b_i2sr_pin equ 4 pending interrupt not
b_i2sr_al equ 3 arbitration lost
b_i2sr_aas equ 2 addressed as slave
b_i2sr_ad0 equ 1 address zero
b_i2sr_lrb equ 0 last received bit

* Status Register absolute values

i2sr_mst equ 1<<b_i2sr_mst master mode
i2sr_trx equ 1<<b_i2sr_trx transmitter mode
i2sr_bb equ 1<<b_i2sr_bb bus is busy
i2sr_pin equ 1<<b_i2sr_pin NO pending interrupt
i2sr_al equ 1<<b_i2sr_al arbitration lost
i2sr_aas equ 1<<b_i2sr_aas addressed as slave
i2sr_ad0 equ 1<<b_i2sr_ad0 addressed as address 0 (general call)
i2sr_lrb equ 1<<b_i2sr_lrb no receiver acknowledge

* I2C Control Register formats

* Control Register bit numbers

b_i2cr_sel equ 6 selected
b_i2cr_eso equ 3 enable i2c bus
b_i2cr_acks equ 2 acknowledge sense

* Control Register absolute values

i2cr_sel equ 1<<b_i2cr_sel device selected
i2cr_eso equ 1<<b_i2cr_eso i2c bus enabled
i2cr_acks equ 1<<b_i2cr_acks acknowledge sense is inverse of this value

* I2C Clock Control Register format

i2cc_mask equ %00011111 divisor mask

 pag
********************
* RS-232C Interface Definitions
*
UART070 equ $80002011 rs-232c interface base address
*
* UART Register offsets
*
u_rmr equ $00 mode register
u_rsr equ $02 status register
u_cls equ $04 clock select register
u_rcr equ $06 command register
u_thr equ $08 transmit holding register
u_rhr equ $0A receive holding register

u_icr equ PICR2-UART070

 pag
********************
* On-Chip Timer
*
Timer070 equ $80002020 on-chip timer base
TOH equ $80002024

* On-Chip Timer Register offsets

t_tsr equ 0 timer status register
t_tcr equ 1 timer control register
t_rr equ 2 reload register
t_t0 equ 4 timer 0
t_t1 equ 6 timer 1
t_t2 equ 8 timer 2

* Timer Status Register formats

* Timer Status bit numbers

b_t0_ov equ 7 timer 0 overflow
b_t1_ma equ 6 timer 1 match
b_t1_cap equ 5 timer 1 capture
b_t1_ov equ 4 timer 1 overflow
b_t2_ma equ 3 timer 2 match
b_t2_cap equ 2 timer 2 capture
b_t2_ov equ 1 timer 2 overflow

* Timer Status absolute values

t0_ov equ 1<<b_t0_ov timer 0 o/f
t1_ma equ 1<<b_t1_ma timer 1 match
t1_cap equ 1<<b_t1_cap timer 1 capture
t1_ov equ 1<<b_t1_ov timer 1 o/f
t2_ma equ 1<<b_t2_ma timer 2 match
t2_cap equ 1<<b_t2_cap timer 2 capture
t2_ov equ 1<<b_t2_ov timer 2 o/f

* Timer Control Register formats

t1ev_mask equ %11000000 timer 1 event field
t1mod_mask equ %00110000 timer 1 mode field
t2ev_mask equ %00001100 timer 2 event field
t2mod_mask equ %00000011 timer 2 mode field

tx_off equ %00 timer inhibited
tx_match equ %01 timer match mode
tx_cap equ %10 timer capture mode
tx_ev equ %11 timer event counter mode

t1ev_field equ 6 shift value for t1 event field
t1mod_field equ 4 shift value for t1 mode field
t2ev_field equ 2 shift value for t2 event field
t2mod_field equ 0 shift value for t2 mode field

 pag
********************
* Peripheral Interrupt Control
*
PICR1 equ $80002045 I2C and Timer control
PICR2 equ $80002047 RS-232C Rx and Tx control

* device irq level fields

picr_i2c_mask equ %01110000 I2C interface
picr_tim_mask equ %00000111 timer
picr_rxd_mask equ %01110000 rs232 rxd
pixr_txd_mask equ %00000111 rs232 txd

 pag
********************
* On-Chip DMA Controller Definitions
*
DMA1 equ $80004000 dma controller #1 base address
DMA2 equ DMA1+$40 dma controller #2 base address
DMARst1 equ $80004007 DMA channel 1 reset register
DMARst2 equ $80004047 DMA channel 2 reset register

dma_chfield equ %11000000 controller 'n' address offset
dma_chshift equ 6 shift value for addressing controller 'n'

* DMA Register offsets (from controller base)
*
* NOTE: 1. device address counters valid for dma #2 only.
*       2. device/memory address HIGH not used.

dma_csr equ 0 channel status register
dma_cer equ 1 channel error register (read only)
dma_dcr equ 4 device control register
dma_ocr equ 5 operation control register
dma_scr equ 6 sequence control register
dma_ccr equ 7 channel control register
dma_mtch equ $a memory transfer count (high)
dma_mtcl equ $b memory transfer count (low)
dma_mach equ $c memory address counter (high)
dma_macmh equ $d memory address counter (middle high)
dma_macml equ $e memory address counter (middle low)
dma_macl equ $f memory address counter (low)
dma_dach equ $14 device address counter (high)
dma_dacmh equ $15 device address counter (middle high)
dma_dacml equ $16 device address counter (middle low)
dma_dacl equ $17 device address counter (low)
dma_cpr equ $2d channel priority register

* Channel Status/Error Register formats (WORD access method)

* Status Register bit numbers

b_chx_coc equ 15 channel operation complete
b_chx_ndt equ 13 normal device termination
b_chx_err equ 12 error occurred
b_chx_act equ 11 channel active

* Status Register absolute values

chx_coc equ 1<<b_chx_coc channel operation completed
chx_ndt equ 1<<b_chx_ndt normal device termination
chx_err equ 1<<b_chx_err terminated due to error
chx_act equ 1<<b_chx_act channel is active

* Error Register absolute values

chx_err_mask equ %0000000000011111 error status mask
chx_err_to equ %0000000000000010 timing error
chx_err_bem equ %0000000000001001 bus error - memory address
chx_err_bed equ %0000000000001010 bus error - device address
chx_err_abt equ %0000000000010001 software abort

* Device/Operation Control Register formats (WORD access method)

* Device Control bit numbers

b_chx_cy equ 15 request mode (1=cycle steal, 0=burst)
b_chx_dsz equ 11 device size (1=16 bit, 0=8 bit)

* Device Control absolute values

chx_cy equ 1<<b_chx_cy request mode = cycle steal (else burst)
chx_typ_mask equ %0011000000000000 device type mask field
chx_typ_ack equ %0011000000000000 device type = ACK/RDY
ch2_typ_68k equ %0000000000000000 device type = 68000 device (DMA2 ONLY)
ch2_dsz equ 1<<b_chx_dsz 16 bit port (else 8 bit)

* Operation Control bit numbers

b_chx_dir equ 7 transfer direction

* Operation Control absolute values

chx_dir equ 1<<b_chx_dir device-to-memory (else memory-to-device)
chx_sz8 equ %0000000000000000 operand size is byte
chx_sz16 equ %0000000000010000 operand size is word

chx_ocr_null equ %0000000000000010 NULL ocr value

* Sequence/Channel Control formats (WORD access method)

* Sequence Control absolute values

chx_mac_nc equ %0000000000000000 MAC no change (DMA2 ONLY)
chx_mac_up equ %0000010000000000 MAC count up
chx_dac_nc equ %0000000000000000 DAC no change
chx_dac_up equ %0000000100000000 DAC count up (DMA2 ONLY)

ch1_scr equ chx_mac_up+chx_dac_nc DMA1 sequence control

* Channel Control bit numbers

b_chx_go equ 7 channel start
b_chx_abt equ 4 software abort
b_chx_irq equ 3 enable interrupt

* Channel Control absolute values

chx_go equ 1<<b_chx_go start dma channel
chx_abt equ 1<<b_chx_abt software abort current operation
chx_irq equ 1<<b_chx_irq enable interrupts

chx_ipl_mask equ %0000000000000111 interrupt level mask

 pag
********************
* On-Chip MMU Definitions
*
MMU070 equ $80008000 MMU base address

* MMU Register Offsets

mmu_msr equ 0 MMU status register offset (read only)
mmu_cr equ 1 MMU control register offset (read/write)

* MMU Status Register bit numbers

b_mmu_np equ 7 segment not-present
b_mmu_stk equ 6 segment in error is stack segment
b_mmu_len equ 5 segment length violation
b_mmu_acc equ 4 attribute violation
b_mmu_sup equ 3 descriptor attribute = supervisor
b_mmu_ex equ 2 descriptor attribute = execute
b_mmu_rd equ 1 descriptor attribute = read
b_mmu_wr equ 0 descriptor attribute = write

* MMU Status Register absolute values

mmu_np equ 1<<b_mmu_np addressed segment not present
mmu_stk equ 1<<b_mmu_stk segment in error has stack attribute
mmu_len equ 1<<b_mmu_len segment has length violation
mmu_acc equ 1<<b_mmu_acc attribute violation occurred
mmu_sup equ 1<<b_mmu_sup segment had supervisor attribute
mmu_ex equ 1<<b_mmu_ex segment had execute attribute
mmu_rd equ 1<<b_mmu_rd segment had read attribute
mmu_wr equ 1<<b_mmu_wr segment had write attribute

* MMU Control Register bit numbers

b_mmu_en equ 7 MMU enable
b_mmu_nseg equ 6 number of segment control

* MMU Control Register absolute values

mmu_en equ 1<<b_mmu_en enable on-chip MMU
mmu_nseg equ 1<<b_mmu_nseg max segments = 128 (else 8)

* On-Chip Descriptor RAM Address offsets
*
* These address offsets give the addresses of the on-chip
* descriptor ram for descriptors 0 - 7.
* The "mmu_descmask" field in the address offset is used to
* determine the active descriptor (0 - 7) to address.

mmu_descfield equ %00111000 descriptor 'n' address offset
mmu_desc_n equ 1<<3 shift value for addressing descriptor 'n'

mmu_descattr equ $40 descriptor 0, attributes (word)
mmu_desclen equ $42 descriptor 0, length (word)
mmu_descres equ $44 descriptor 0, reserved (byte)
mmu_descnum equ $45 descriptor 0, segment number (byte)
mmu_descbase equ $46 descriptor 0, base address (word)

* Segment Descriptor Layouts
*
* These are the address offsets of the segment descriptor
* when it is in main-memory.

mem_descattr equ 0 segment attributes (word)
mem_desclen equ 2 segment length (word)
mem_descres equ 4 reserved field (byte)
mem_descres2 equ 5 reserved field WHEN IN-MEMORY
mem_descbase equ 6 base address

* Segment Descriptor Attributes format
*
* NOTE:  the Valid/Present flag is only valid for descriptors
* in main memory (NOT for loaded descriptors)

* as bit numbers

b_attr_vp equ 15 segment descriptor valid/present ("in-memory" only)
b_attr_sup equ 14 segment has supervisor attr
b_attr_ex equ 13 segment has execute attr
b_attr_rd equ 12 segment has read attr
b_attr_wr equ 11 segment has write attr
b_attr_stk equ 6 segment has stack attr

* as absolute values

attr_vp equ 1<<b_attr_vp descriptor valid/present
attr_sup equ 1<<b_attr_sup segment has supervisor attr
attr_ex equ 1<<b_attr_ex segment has execute attr
attr_rd equ 1<<b_attr_rd segment has read attr
attr_wr equ 1<<b_attr_wr segment has write attr
attr_stk equ 1<<b_attr_stk segment has stack attr

attr_null equ %0000000100000000 NULL case attributes (bit 8 SET)

* Segment Length format

seglen_mask equ %0000011111111111 segment length mask (11 bit)

* Segment Number format
*
* NOTE:  this is only valid for loaded descriptors

segnum_mask equ %01111111 segment number mask (7 bit)
b_segnum_flush equ 7 flush (invalidate) segment descriptor control
segnum_flush equ 1<<b_segnum_flush segment is valid

* Segment Base format

segbase_mask equ %0011111111111111 segment base mask (14 bit)

 opt l
* end of scc68070.d

