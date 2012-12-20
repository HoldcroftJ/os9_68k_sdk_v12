 opt -l
 nam timm.d
 ttl - mc68340 Timmer Module

**********************************************************************
* Definitions for the mc68340 Timmer Module
*
* Edition History:
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  90/11/05  history starts.                                  Rwb
*               ---- OS-9 V2.4 Port Pak Release #2 ----
*               ---- OS-9/68k V3.0 Release ----

 ifndef ModuleIO
ModuleIO equ $FFFFF000  The base address of Short I/O space.
 endc

TIMBase equ $600            Offset of TIM from system MCR  (Range: $600 - $67F)
TIMMap equ ModuleIO+TIMBase Default location of the TIM

*  The mc68340 has two timer modules that can be externally cascaded.
TIMER1   equ  TIMMap        Base address of timer 1
TIMER2   equ  TIMMap+$40	Base address of timer 2

*
*  These register defintions are relative to the base address of the
*    timer to be used.
*
***  Supervisor Access Only Registers
*
TIMMCR    equ   $0  Module Configuration Register.
TIMTEST   equ   $2  Timer Test Register.
TIMINTL   equ   $3  Timer Interrupt level.
TIMINTV   equ   $4  Timer Interrupt Vector Register.

***  Supervisor or User Registers
*
TIMCNTRL  equ   $6  Timer Control Register
TIMSTATUS equ   $8  Timer Status Register
TIMPRSCL  equ   $9  Timer Prescalar Register
TIMCOUNT  equ   $A  Timer Count Register
TIMPREL1  equ   $C  Timer Preload Register 1
TIMPREL2  equ   $E  Timer Preload Register 2
TIMCMPR   equ   $10 Timer Compare Register

****************   TIMMCR  ******************************************
* Timer Module Control Register
b_TIMSTP   equ  15   Turns off the stop clocks from the SBIU
b_TIMFRZ   equ  13   (2 Bits)  Determines action of IFREEZB

TIMFRZ_ignor  equ  0 Ignore the freeze signal.
TIMFRZ_freeze equ  (2<<b_TIMFRZ) Execution Freeze

b_TIMSU   equ  7  Supervisor/User access on registers $6-$10
b_TIMMAID equ  4  Master Arbitration ID  (unused)
b_TIMIAID equ  0  (3 bits) field used to arbitrate for the IMB 

****************  TIMINTL and TIMINTV  *******************************
*  Timer interrupt level and vector registers.
*
*   TIMINTL  bits 0-3 define interrupt level 1-7
*   TIMINTV  bits 0-7 define the vector for the timer interrupt.
**********************************************************************

***********************  TIMCNTRL  ***********************************
*  Timer control register.
*
b_TIMSWRB   equ  15  Set to FALSE (0) causes software reset of the counter
b_TIMINTE   equ  12  (3 bits) Set as follows:

TIM_POLL_INT  equ  (0<<b_TIMINTE)  Polling Mode
TIM_TC_INT    equ  (1<<b_TIMINTE)  Enables TC to effect IIRQB.
TIM_TG_INT    equ  (2<<b_TIMINTE)  Enables TG to effect IIRQB.
TIM_TO_INT    equ  (4<<b_TIMINTE)  Enables TO to effect IIRQB.

b_TIMTGE    equ  11  Enables TGATEB to control of prescalar and counter
b_TIMPSE    equ  10  Prescalar Select Enable
b_TIMCPE    equ   9  Counter Prescalar Enable
b_TIMCLK    equ   8  0 = TCLK (1/2 system clock freq.), 1 = TIN pin (cascade)
b_TIMPOT    equ   5  (3 bits) selects prescalar tap to be used as below:

TIMPOT_2    equ  (1<<b_TIMPOT)  /2
TIMPOT_4    equ  (2<<b_TIMPOT)  /4
TIMPOT_8    equ  (3<<b_TIMPOT)  /8
TIMPOT_16   equ  (4<<b_TIMPOT)  /16
TIMPOT_32   equ  (5<<b_TIMPOT)  /32
TIMPOT_64   equ  (6<<b_TIMPOT)  /64
TIMPOT_128  equ  (7<<b_TIMPOT)  /128
TIMPOT_256  equ  (0<<b_TIMPOT)  /256

b_TIMOM     equ  2   (3 bits) selects one of eight modes for the timer

TIMMODE_IC  equ  (0<<b_TIMOM)  Input Capture/Output Compare
TIMMODE_SW  equ  (1<<b_TIMOM)  Square wave Generator
TIMMODE_VS  equ  (2<<b_TIMOM)  Variable duty cycle Square wave generator
TIMMODE_OS  equ  (3<<b_TIMOM)  Variable width single shot pulse generator
TIMMODE_PW  equ  (4<<b_TIMOM)  Pulse Width Measurement
TIMMODE_PM  equ  (5<<b_TIMOM)  Period Measurement
TIMMODE_EV  equ  (6<<b_TIMOM)  Event Count
TIMMODE_BP  equ  (7<<b_TIMOM)  Timer By-Pass (simple test mode)

b_TIMOC     equ  0  (2 bits)  Encodes conditions under whch TOUT will change.

*  If not in Input Capture/Output Control mode...
TIMOC_HI    equ  0  High Impedance, TOUT disabled
TIMOC_TM    equ  1  Toggle mode,  SWRB active   := TOUT set 0
*                                 SWRB inactive := TOUT toggle on Time Out

TIMOC_ZM    equ  2  Zero Mode,   SWRB active   := TOUT set 0
*                                SWRB inactive := TOUT set 0 at next Time Out.

TIMOC_OM    equ  2  Zero Mode,   SWRB active   := TOUT set 0
*                                SWRB inactive := TOUT set 1 at next Time Out.

*  If in Input Capture/Output Control mode ....
TIMOC_IC_HI    equ  0  High Impedance, TOUT disabled
TIMOC_IC_TM    equ  1  Toggle mode,  SWRB active   := TOUT set 0
*                                    SWRB inactive := TOUT toggle on Time Cmp.

TIMOC_IC_ZM    equ  2  Zero Mode, SWRB active   := TOUT set 0
*                                 SWRB inactive := TOUT set 0 at next Time Cmp.

TIMOC_IC_OM    equ  2  Zero Mode,   SWRB active   := TOUT set 1
*                                SWRB inactive := TOUT set 1 at next Time Out.

*********************  TIMSTATUS  **********************************
*  Status Register
*
b_TIMSTATUS_IRQ equ 7  This bit is set 1 when an interrupt has occured.
b_TIMSTATUS_TO  equ 6  This bit set 1 when timer has "rolled-over"
b_TIMSTATUS_TG  equ 5  This bit set 1 on TGE (mode dependent, see manual)
b_TIMSTATUS_TC  equ 4  This bit set 1 on timer comapre events.
b_TIMSTATUS_TGL equ 3  This bit reflects the lovel of TGATEB
b_TIMSTATUS_ON  equ 2  If 1, the counter is enabled and counting down.
b_TIMSTATUS_OUT equ 1  This bit reflects the current condition of TOUT
b_TIMSTATUS_COM equ 0  This bit indicates  counter at or between value in cmp.

***********************  TIMPRSCL  *********************************
*  Timer Prescalar value.
*     8 Bit current value of the prescalar.
********************************************************************

***********************  TIMCOUNT  *********************************
*  Timer Count register.
*		16 Bits (read only) Current counter value.
********************************************************************

***********************  TIMPREL1  *********************************
*  Preload1 Register
*       16 bits (read/write) value that will be loaded to the count
*       register.
*********************************************************************

***********************  TIMPREL2  *********************************
*  Preload2 Register
*       16 bits (read/write) value that will be loaded to the count
*       register in the Variable Duty Cycle Square Wave and Single
*       shot pulse generator.
*********************************************************************

***********************  TIMCMPR  *********************************
*  Timer Compare Register.
*
*   16 Bit register used to provide an interrupt whenever the counter
*   counts down to a certain value or indicatine the approaching Timeout.
*******************************************************************
 pag
 opt l
