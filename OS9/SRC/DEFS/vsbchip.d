 opt -l
 ttl VSBChip.d - VSB chip definitions
********************
* Definitions for the MVSB2400 VSB bus master/slave interface chip.
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  90/01/12  Creation                                         Rwb
* 02  90/08/13  Updated when dealing with MVME165 Rev2.5 spec.   wwb
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
* Register Offsets - from VSBchbase
*
VSBCSR equ 0 control and status register for the chip (16 bit)
VSBBCR equ 4 block transfer byte count register (32 bit)

* NOTE: the VSBADR register is NOT accessible on the MVME165.
VSBADR equ 8 address decode register (32 bit)


********************
* CSR definitions
*
vsb_not_scon equ 1<<0 system controller (0 = enabled; 1 = disabled)
vsb_not_block equ 1<<2 block xfer mode (0 = enabled; 1 = disabled)
vsb_not_ten equ 1<<3 68010 mode (0 = 68010 mpu; 1 = 68020+)
vsb_bounce equ 1<<4 enable VSB bounce mode (0 = disabled; 1 = enabled)
vsb_not_readonly equ 1<<5 VSB read-only mode (0 = enabled; 1 = disabled)
vsb_not_master equ 1<<6 enable master mode (0 = enabled; 1 = disabled)
vsb_ento_msk equ %11<<8 VSB timeout mask

* timeout values for ENTO0 & 1
*
ento64 equ %00<<8 64uS
ento128 equ %01<<8 128 uS
ento256 equ %10<<8 256 uS
entooff equ %11<<8 timouts disabled.

vsb_not_fair equ 1<<10 "fair" request mode (0 = enabled; 1 = disabled)
vsb_errmsk equ %11111<<11 error status mask

* VSB error status flags
*
b_vsb_wrerr equ 11 0 if attempt to write to VSB when in read-only mode
b_vsb_to equ 12 0 if VSB timeout occurred
b_vsb_asack0 equ 13 state if ASACK0* when b_vsb_err detected low
b_vsb_asack1 equ 14 state if ASACK1* when b_vsb_err detected low
b_vsb_err equ 15 0 if ERR* asserted

 opt l

* end of vsbchip.d

