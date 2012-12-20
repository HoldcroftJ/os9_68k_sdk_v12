 opt -l
********************
* Definitions for VMEchip device - VME147
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  88/08/10  history starts.                                  wwb
*               ---- OS-9/68K V2.3 Release ----
*               ---- OS-9/68K V2.4 Release ----
* 



********************
* System Interrupt Structure
*

* Interrupt Levels

WPBERRLevel equ 7 VMEchip write-posted BERR
SYSFAILLevel equ 6 SYSFAIL* level
SIGHPLevel equ 5 signalling high priority level
LM1Level equ 4 location monitor 1 level
IACKLevel equ 3 IACK level
LM0Level equ 2 location monitor 0 level
SIGLPLevel equ 1 signalling low priority level

* Interrupt Vector assignments

VMEVect equ 80 VMEchip Base Vector (uses 80 - 87)

WPBERRVect equ VMEVect+WPBERRLevel write-posted BERR vector
SYSFAILVect equ VMEVect+SYSFAILLevel SYSFAIL* vector
SIGHPVect equ VMEVect+SIGHPLevel signalling high priority vector
LM1Vect equ VMEVect+LM1Level location monitor 1 vector
IACKVect equ VMEVect+IACKLevel IACK vector
LM0Vect equ VMEVect+LM0Level location monitor 0 vector
SIGLPVect equ VMEVect+SIGLPLevel signalling low priority vector


********************
* VMEchip:  LCSR - System Controller Configuration Register definitions
*
SCCRCtl equ VMEchBase+1 SCCR base address

sccr_scon equ 1<<0 VME147 is system controller (J5: 1-2 jumpered)
sccr_sreset equ 1<<1 activate SYSRESET* (and suicide)
sccr_brdfail equ 1<<2 assert SYSFAIL*
sccr_robin equ 1<<3 arbitrate vme bus "round robin" (else priority mode)


********************
* VMEchip:  LCSR - Requester Configuration definitions
*
RQCCtl equ VMEchBase+3 RQC base address

rqc_rqlevmask equ %00000011 request level mask
rqc_rqlev0 equ 0 request bus with BR0
rqc_rqlev1 equ 1 request bus with BR1
rqc_rqlev2 equ 2 request bus with BR2
rqc_rqlev3 equ 3 request bus with BR3
rqc_rnever equ 1<<3 prevent requester from releasing bus
rqc_rwd equ 1<<4 release when done
rqc_ronr equ 1<<5 ALWAYS 0
b_rqc_dhb equ 6 cpu is bus master (bit #)
rqc_dhb equ 1<<b_rqc_dhb cpu is bus master
rqc_dwb equ 1<<7 request bus mastership


********************
* VMEchip:  LCSR - Master Configuration definitions
*
MasCCtl equ VMEchBase+5 vme master configuration base address

mas_d16 equ 1<<0 force D8 and D16 bus accesses
mas_a24 equ 1<<1 force A24 bus accesses
mas_a16 equ 1<<2 force A16 (Short I/O AM) bus accesses
mas_uat equ 1<<3 enable unaligned data transfers
mas_cfill equ 1<<4 ALWAYS 0
mas_wp equ 1<<5 enable master write-posting


********************
* VMEchip:  LCSR - Slave Configuration defintions
*
SlvCCtl equ VMEchBase+7 vme slave configuration base address

slv_d16 equ 1<<0 force D16 transfers
slv_wp equ 1<<5 enable slave write-posting
slv_en equ 1<<7 enable vme masters to access on-board ram


********************
* VMEchip:  LCSR - Timer Configuration definitions
*
TimCCtl equ VMEchBase+9 vme timer base address

* local and global t/o periods

tim_to102 equ 0 t/o period = 102uS
tim_to205 equ 1 t/o period = 205uS
tim_to410 equ 2 t/o period = 410uS
tim_toinf equ 3 t/o period = infinite

* vme access t/o periods

tim_1.6 equ 1 t/o period = 1.6mS
tim_51 equ 2 t/o period = 51mS

* register definitions

tim_ltomask equ %11<<0 local t/o period mask
tim_vmemask equ %11<<2 vme access t/o period mask
tim_gtomask equ %11<<4 global t/o period mask
tim_arbto equ 1<<6 enable vme arbitration timer


********************
* VMEchip:  LCSR - Slave Address Modifier definitions
*
SlvAMCtl equ VMEchBase+$b slave AM base address

slvam_data equ 1<<0 allow data accesses
slvam_prgrm equ 1<<1 allow program accesses
slvam_block equ 1<<2 allow block accesses (DON'T USE)
slvam_short equ 1<<3 allow short am accesses
slvam_std equ 1<<4 allow std am accesses
slvam_ext equ 1<<5 allow extended am accesses
slvam_user equ 1<<6 allow user am accesses
slvam_super equ 1<<7 allow supervisor am accesses

* the default access modes for "slave" responses is:
*
* supervisor data accesses (std or extd addressing) only
*
slvam_default equ slvam_super+slvam_ext+slvam_std+slvam_data


********************
* VMEchip:  LCSR - Master Address Modifier definitions
*
MasAMCtl equ VMEchBase+$d master AM base address

masam_am0 equ 1<<0 assert AM0
masam_am1 equ 1<<1 assert AM1
masam_am2 equ 1<<2 assert AM2
masam_am3 equ 1<<3 assert AM3
masam_am4 equ 1<<4 assert AM4
masam_am5 equ 1<<5 assert AM5
masam_amsel equ 1<<7 assert AM0-5 according to MasAMCtl contents


********************
* VMEchip:  LCSR - Interrupt Mask definitions
*
VMEIRQCtl equ VMEchBase+$f irq mask base address

virq_en1 equ 1<<1 enable vme bus level 1
virq_en2 equ 1<<2 enable vme bus level 2
virq_en3 equ 1<<3 enable vme bus level 3
virq_en4 equ 1<<4 enable vme bus level 4
virq_en5 equ 1<<5 enable vme bus level 5
virq_en6 equ 1<<6 enable vme bus level 6
virq_en7 equ 1<<7 enable vme bus level 7

virq_enall equ virq_en1+virq_en2+virq_en3+virq_en4+virq_en5+virq_en6+virq_en7


*******************
* VMEchip:  LCSR - Utility Interrupt Mask definitions
*
UIRQCtl equ VMEchBase+$11 utility irq mask base address

uirq_siglen equ 1<<1 enable SIGLP irq
uirq_lm0en equ 1<<2 enable LM0 irq
uirq_iacken equ 1<<3 enable IACK irq
uirq_lm1en equ 1<<4 enable LM1 irq
uirq_sighen equ 1<<5 enable SIGHP irq
uirq_sfien equ 1<<6 enable SYSFAIL* irq
uirq_wperren equ 1<<7 enable write-posted BERR irq


********************
* VMEchip:  LCSR - Utility Interrupt Vector definitions
*
UVectCtl equ VMEchBase+$13 utility irq vector base address

uvct_idmask equ %111<<0 vector id mask
uvct_basemask equ %11111<<3 base vector mask


********************
* VMEchip:  LCSR - Interrupt Request definitions
*
IRQRqCtl equ VMEchBase+$15 irq request base address

irqrq_mask equ %111<<0 irq request level mask


********************
* VMEchip:  LCSR - Status/ID definitions
*
IDCtl equ VMEchBase+$17 status/id register base address


********************
* VMEchip:  LCSR - Bus Error Status definitions
*
VBERRCtl equ VMEchBase+$19 vme bus error base address

vberr_lbto equ 1<<0 local t/o occurred
vberr_acto equ 1<<1 vme access t/o
vberr_vberr equ 1<<2 bus error on vme access
vberr_rmcerr equ 1<<3 CAN NEVER BE SET


********************
* VMEchip:  LCSR - GCSR Base Address Configuration definitions
*
GlbSelCtl equ VMEchBase+$1b GCSR base select

gsel_mask equ %1111<<0 GCSR short i/o base select mask


********************
* VMEchip:  GCSR - Global Register definitions
*

* the GCSR registers are accessible as LOCAL addresses to the
* on-board cpu, as well as GLOBAL addresses in the VME Short I/O
* Space.  As such, the register addresses will be defined as offsets
* from a "base" address - the LOCAL base address is within the
* VMEchip itself, the GLOBAL address is defined by the value loaded
* into the GlbSelCtl register.

GCSRBase equ VMEchBase+$20 base address - LOCAL access

* GCSR register offsets

GR0 equ 1 global register 0
GR1 equ 3 global register 1
BrdID equ 5 board identification register
GPCSR0 equ 7 general purpose csr 0
GPCSR1 equ 9 general purpose csr 1
GPCSR2 equ $b general purpose csr 2
GPCSR3 equ $d general purpose csr 3
GPCSR4 equ $f general purpose csr 4


********************
* VMEchip:  GCSR - Global Register 0
*
gr0_idmask equ %1111<<0 hardware id mask (always 0)
gr0_lm0 equ 1<<4 location monitor 0 not accessed
gr0_lm1 equ 1<<5 location monitor 1 not accessed
gr0_lm2 equ 1<<6 location monitor 2 not accessed
gr0_lm3 equ 1<<7 location monitor 3 not accessed


********************
* VMEchip:  GCSR - Global Register 1
*
gr1_siglp equ 1<<0 SIGLP set by bus master
gr1_sighp equ 1<<1 SIGHP set by bus master
gr1_brdfail equ 1<<4 BRDFAIL* asserted
gr1_isf equ 1<<5 inhibit VMEchip from asserting VMEbus SYSFAIL*
gr1_scon equ 1<<6 board is system controller (J5, 1-2 jumpered)
gr1_rh equ 1<<7 reset and hold the cpu

* end of "vmechip.d"

 opt l
