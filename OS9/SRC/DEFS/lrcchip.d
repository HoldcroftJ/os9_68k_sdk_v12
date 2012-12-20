 opt -l
 ttl LRCChip.d - Local Resource Controller definitions
********************
* Definitions for the MVME165 LRC
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  90/01/12  Creation                                         Rwb
* 02  90/08/13  Updated per Rev2.5 Spec.                         wwb
* 03  91/04/12  Fixed definitions in bus control register.       wwb
*               (initial doc. was incorrect).
*               <<<---- OS-9/68040 V2.4 Release ---->>>
* 

* needs to be removed for final version:
 ifndef VME165
VME165 equ 165
 endc
 ifndef CPUType
CPUType set VME165  for NOW!
*CPUType set 0 final
 endc


********************
* Register Offsets (from LRCchbase)
*
* NOTE:  On the MVME165, the LRC is accessed as a LONG WORD port.
*        Bits 31-24 are never driven by the LRC, thus they will
*        always be $FF.  The number of significant bits in a
*        register is dependant upon the register being accessed.
*
LRC_TimeCon0 equ $00 Timer 0 Time Constant (32 bits)
LRC_TimeCon1 equ $04 Timer 1  "   "   "   "  "  "
LRC_TimeCon2 equ $08 Timer 2  "   "   "   "  "  "
LRC_TimeCnt0 equ $0C Timer 0 Current count register (32 bits) [Read only]
LRC_TimeCnt1 equ $10 Timer 1   "   "   "   "  "  "
LRC_TimeCnt2 equ $14 Timer 2   "   "   "   "  "  "
LRC_TimeCtrl0 equ $18 Timer 0 Control Register (8 bits: 0 - 7)
LRC_TimeCtrl1 equ $1C Timer 1   "   "   "   "  "  "
LRC_TimeCtrl2 equ $20 Timer 2   "   "   "   "  "  "
LRC_GenCtrl equ $24 General control register (8 bits: 0 - 7 )
LRC_BusCtrl equ $28 Bus control (8 bits: 0 - 7)
LRC_IntCtrl0 equ $2C Interrupt control register 0 (8 bits: 0 - 7)
LRC_IntCtrl1 equ $30 Interrupt control register 1 (8 bits: 0 - 7)
LRC_StatsReg equ $34 Chip status (18 bits: 0 - 17)
LRC_RamAddr equ $38 Ram Address (20 bits: 0 - 19)
LRC_BerrStat equ $3C Bus Error Status. (bit 0 only) (NOTE: cleared on read)


********************
* LRC_TimeConX - timer constant registers
*
* These registers hold the timer constant values for each timer.
* They count UP, at a rate of 1Mhz (i.e. 1uS).
* When the timer counts up to the value in the timer constant,
* the corresponding timeout status is reflected in the Status Register.
* The inital value for the timer is a 1.
*
* If interrupts are enabled for the timer, then a timeout will 
* generate an interrupt as follows:
*
*   Timer 0 - Level 7
*   Timer 1 - Level 6
*   Timer 2 - Level 5



********************
* LRC_TimeCTLx - Timer Control Registers
*

* bits 7 - 4 are common to each register (timer 0, 1 or 2)
*
lrc_ten equ 1<<7 enable timer N
lrc_tld equ 1<<6 load counter N when 0->1 transition
lrc_tcycl equ 1<<5 timer N cycle mode (reload counter and continue count)
lrc_tie equ 1<<4 timer N interrupt enable

* bits 3 - 0 control overall functions
*
* timer output functions are controlled by timer 0 control register
*
lrc_tomd equ 1<<0 timer output mode: 0=square wave o/p, 1=pulse output (1uS)
lrc_toen equ 1<<1 enable timer output pin
lrc_tos0 equ %00<<2 select output of timer 0
lrc_tos1 equ %01<<2 select output of timer 1
lrc_tos2 equ %10<<2 select output of timer 2
lrc_tost equ %11<<2 test mode

* timer input functions are controller by timer 1 control register
*
lrc_timd equ 1<<0 timer input mode: 0=count rising edges of i/p pin,
*                 * 1=h/w timer enable
lrc_tien equ 1<<1 enable timer input pin
lrc_tis0 equ %00<<2 select input to timer 0
lrc_tis1 equ %01<<2 select input to timer 1
lrc_tis2 equ %10<<2 select input to timer 2
lrc_tisn equ %11<<2 none

* timer misc. fucntions are controller by timer 2 control register
*
lrc_ttst equ 1<<0 test function only
lrc_twden equ 1<<1 enable watchdog timer to reset pin
lrc_twd0 equ %00<<2 select wdt from timer 0
lrc_twd1 equ %01<<2 select wdt from timer 1
lrc_twd2 equ %10<<2 select wdt from timer 2
lrc_twdn equ %11<<2 none


********************
* LRC_GenCtrl - general control register
*
lrc_rsto equ 1<<0 assert RESET Output if 1
lrc_paron equ 1<<1 parity error enable
lrc_wwpar equ 1<<2 write-wrong parity
*
* "slow" eproms are:  BCLK = 25MHz, 250nS
*                            33MHz, 200nS
* "fast" eproms are:         25MHz, 150nS
*                            33MHz,  85nS
lrc_fast equ 1<<3 0=slow eprom acknowledge, 1 = fast eprom acknowledge
lrc_scfreq equ 1<<4 serial clock freq:  0=3.6864Mhz, 1=2.4576MHz


********************
* LRC_BusCntrl - Bus Control Register
*
* Note:  the issue of how these control bits function is tied
*        to the VSB Chip mode and the board's implementation.
*        Refer to the relevant documentation for full details.
lrc_romd equ 1<<0 VSB read only mode (see also: VSBChip VSBRD)
lrc_roen equ 1<<1 VSB read only enable (writes are via VME)
lrc_vsbmd equ 1<<2 VSB Mode (A31=0 & E0000000-E3FFFFFF = VSB)
lrc_vsben equ 1<<3 VSB Enable
lrc_va24 equ 1<<4 VME A24 Address option for addresses < 16Meg


********************
* LRC_IntCtrl0 - Interrupt Control Register 0
*
* These are the interrupt enables (1 = enabled)
*
lrc_l7g4 equ 1<<0 level 7, grp 4
lrc_l7g2 equ 1<<1       7      2
lrc_l6g4 equ 1<<2       6      4
lrc_l6g2 equ 1<<3       6      2
lrc_l5g2 equ 1<<4       5      2
lrc_l4g2 equ 1<<5       4      2
lrc_l3g2 equ 1<<6       3      2
lrc_gie equ 1<<7 global interrupt enable

 ifeq (CPUType-VME165)
* map generic names to functions on the MVME165
lrc_parerr equ lrc_l7g4 parity error irq: level 7
lrc_vsbirq6 equ lrc_l6g4 VSB irq: level 6
lrc_ticktmr6 equ lrc_l6g2 tick timer: level 6
lrc_duart equ lrc_l5g2 duart (68681): level 5
lrc_vsbirq4 equ lrc_l4g2 VSB irq: level 3
lrc_tick3 equ lrc_l3g2 tick timer: level 3
 endc VME165


********************
* LRC_IntCtrl1 - Interrupt Control Register 1
*
lrc_l7g5 equ 1<<0 level 7, group 5
lrc_l7g6 equ 1<<1       7,       6
lrc_vectmsk equ %1111<<4 base vector mask

 ifeq (CPUType-VME165)
* map generic name to functions on the MVME165
lrc_abort equ lrc_l7g5 abort switch: level 7
lrc_acfail equ lrc_l7g6 ACFAIL* : level 7
 endc VME165


********************
* LRC_StatsReg - Interrupt Status Register
*
* The LRC status register has the following characteristics:
*
* Bit 17 (WDT) is not cleared by RESET, but IS cleared by Power-On-Reset
* All bits may be cleared by writing a 1 to the specific status bit,
* with the exception of bits 3, 9, 13, 15 & 16, which are instantaneous
* values, and cannot be cleared.
*
lrc_stat_acfail equ 1<<0 ACFAIL
lrc_stat_abort equ 1<<1 Abort switch
lrc_stat_parerr equ 1<<2 Parity error
lrc_stat_vsb6 equ 1<<3 VSB (unlatched) level 6

lrc_stat_tmr0 equ 1<<4 timer 0 timeout status
lrc_stat_tmr1 equ 1<<5 timer 1 timeout status
lrc_stat_tmr2 equ 1<<6 timer 2 timeout status

lrc_stat_tick6 equ 1<<8 tick timer level 6
lrc_stat_duart equ 1<<9 duart (unlatched)
lrc_stat_vsb3 equ 1<<10 VSB (unlatched) level 3
lrc_stat_tick3 equ 1<<11 tick timer level 3

lrc_stat_por equ 1<<12 power on reset
lrc_stat_sysfail.ul equ 1<<13 SYSFAIL* (unlatched)
lrc_stat_sysfail equ 1<<14 SYSFAIL* (latched)
lrc_stat_abort.ul equ 1<<15 Abort (unlatched)

lrc_stat_acfail.ul equ 1<<16 ACFAIL* (unlatched)
lrc_stat_wdt equ 1<<17 watch dog timer

lrc_stat_all equ $25fff mask to clear ALL pending irqs


********************
*  LRC_RamAddr - local ram address control
*
lrc_rambase_msk equ %111111111111 RA31 - RA20 address bits for local ram
lrc_mska_msk equ %1111111<<12 MSKA26 - MSKA20 (mask address bits)

 ifeq (CPUType-VME165)
lrc_addmsk_4M equ %0000011<<12 address mask for 4M version
lrc_addmsk_16M equ %0001111<<12 address mask for 16M version

 opt l

* end of lrcchip.d

