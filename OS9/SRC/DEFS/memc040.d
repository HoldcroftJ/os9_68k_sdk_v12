 opt -l
********************
* Definitions for MEMC040 DRAM Controller Chip - VME167
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  91/04/26  Created                                          Rwb
* 02  91/10/04  Updated bit defs: controller regs can be         wwb
*               accessed as bytes, thus bit defs changed from
*               long-word form to normal byte form.
* 03  93/08/27  Added value for MEMC040 chip id.                 ats
* 

*
*  Conventions used in this file:
*
*   b_XXXX  denotes a bit number definition (note access size quoted)
*   m_XXXX  denotes a bit mask (again, note access size quoted)

*
*  The MEMC040 chip is a four-way interleaving DRAM controller ASIC
*  for the MC68040-type bus.  It was designed in part for use with
*  the mVme167/187 processors.

*
* MCID - Chip ID Register
*
*   Note:  writting an $F to this register generates a software interrupt.
*
MCID     equ   0

c_MEMC040	equ		$80		Value of MCID register for MEMC040

*
* MCRV - Chip Revision Register
*
MCRV     equ   4

*
* MCCFG - Memory Configuration Register
*
MCCFG    equ   8

b_MCCFG_fstrd  equ  5  Reflects the state of the FASTREAD input pin
b_MCCFG_extpe  equ  4  Reflects the state of the EXTPEN pin
b_MCCFG_wpb    equ  3  Reflects the write-per-bit mode.
b_MCCFG_msiz   equ  0
m_MCCFG_msiz   equ  (7<<b_MCCFG_msiz) Reflects the size strapping

*  size is based at 4MB and is doubled per increment with values
*  6 and 7 reserved  (i.e 1 = 8MB, 2 = 16MB ..... 5 = 128Mb)

*
* MCASR - Alternate Status Register
*    Only valid  when WPB* status bit is logic 1.
*
MCASR     equ  $C

*
* MCACR - Alternate Control Register
*    Only valid  when WPB* status bit is logic 1.
*
MCACR     equ  $10

*
* MCBADR - Base Address Register
*
*   All 8 bits used to program Address bits 31-24.
MCBADR    equ  $14

*
* MCRCR - RAM Control Register
*
MCRCR    equ  $18

b_MCRCR_ad22   equ  6
m_MCRCR_ad22   equ  (3<<b_MCRCR_ad22) These two bits determine address (23-22)

b_MCRCR_swait  equ  4  Snoop Wait: 1 = wait for MI*, 0 = don't wait
b_MCRCR_wwp    equ  3  Write Wrong Parity (1 = do it -- Testing only!)
b_MCRCR_parint equ  2  Parity Interrupt enable  (see table below)
b_MCRCR_paren  equ  1  Parity Enable (see table below)
b_MCRCR_ramen  equ  0  RAM enable (1 == enable, 0 == disable)

* b_MCRCR_paren and b_MCRCR_parint work together to produce the
*   following truth table:
*
*  paren     parint    CPU     Alternate
*    0         0      None     None
*    0         1      Int      None
*    1         0      CHECK    CHECK
*    1         1      Int      CHECK
*
* where:  None  == No checking done
*         Int   == CPU recieves parity interrupt on parity error
*         CHECK == TEA generated on parity error
*
*  If interrupt is to be generated, must enable the parity error interrupt
*  in the VMEChip2.
*
*  If b_MCCFG_expe is 0, all parity checking is disabled reguardless of
*    these bits.

*
* MCBCR - Bus Clock Register
*    Should be programmed to the operating clock frequency.
*    (ie. $21 for 33MHz)
*
MCBCR    equ   $1C

* end of "memc040.d"
 opt l
