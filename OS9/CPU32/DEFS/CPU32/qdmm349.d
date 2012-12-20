 opt -l
 nam qdmm349.d
 ttl Definitions for the mc68349 QDMM module

**********************************************************************
* Definitions for the mc68349 QDMM module
*
* Edition History
*
* Ed.   Date.     Reason.										Who.
* --  --------  -----------------------------------------------	---
* 01  93/09/15	created.										wwb
* 02  93/09/21  updated per correct data book info.				wwb
* 03  93/09/27  updated for correct register names (data book).	wwb
*               ---- OS-9/68k V3.0 Release ----

*******************
* NOTE: all register offsets are from the MBAR base address.
*
* The MC68349 QDMM module provides 4 1K blocks of static ram.
*

* register offsets (supervisor access only)
*
QDMMMCR equ $c00 QDMM Module Configuration Register offset
QBAR0 equ $c10 QDMM Base Address Register 0 offset
QBAR1 equ $c14 QDMM Base Address Register 1 offset
QBAR2 equ $c18 QDMM Base Address Register 2 offset
QBAR3 equ $c1c QDMM Base Address Register 3 offset

* QDMM MCR definitions (word access)
*
b_QDMM_Stop equ 15 if set, module disabled (bank accesses IGNORED)
QDMM_Stop equ 1<<b_QDMM_Stop

* QDMM Base Address Registers (0-3)
*
QBARX_mask equ $fffffc00 bits 31-10 form sram block base address

b_QBARX_SU equ 2 if set, user/supervisor mode access, otherwise super only
b_QBARWP equ 1 if set, bank is write-protected
b_QBARValid equ 0 if set, base address field id valid.

QBARX_SU equ 1<<b_QBARX_SU
QBARWP equ 1<<b_QBARWP
QBARValid equ 1<<b_QBARValid

 pag
 opt l
