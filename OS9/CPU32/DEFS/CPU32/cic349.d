 opt -l
 nam cic349.d
 ttl Definitions for the mc68349 Cache/SRAM module

**********************************************************************
* Definitions for the mc68349 Cache/SRAM module
*
* Edition History
*
* Ed.   Date.     Reason.										Who.
* --  --------  -----------------------------------------------	---
* 01  93/09/15	created.										wwb
* 02  93/09/21  updated for correct data book info.				wwb
*               ---- OS-9/68k V3.0 Release ----

*******************
* NOTE: all register offsets are from the MBAR base address.
*
* The MC68349 Cache/SRAM module provides 4 banks of 512-byte
* i-cache/SRAM.  Each block can be configured to run as either
* i-cache or SRAM.

* register offsets (supervisor access only)
*
CICMCR equ $fc0 CIC Module Configuration Register offset
CICBADDR0 equ $fc4 CIC SRAM Base Address Register 0 offset
CICBADDR1 equ $fc8 CIC SRAM Base Address Register 1 offset
CICBADDR2 equ $fcc CIC SRAM Base Address Register 2 offset
CICBADDR3 equ $fd0 CIC SRAM Base Address Register 3 offset

* CICMCR definitions
*
b_LWP3 equ 12 lock/write protect bank 3
b_LWP2 equ 11 lock/write-protect bank 2
b_LWP1 equ 10 lock/write-protect bank 1
b_LWP0 equ 9 lock/write-protect bank 0
b_INV3 equ 8 invalidate cache bank 3
b_INV2 equ 7 invalidate cache bank 2
b_INV1 equ 6 invalidate cache bank 1
b_INV0 equ 5 invalidate cache bank 0
b_MD3 equ 4 cache/sram mode, bank 3 (1 = sram)
b_MD2 equ 3 cache/sram mode, bank 2 (1 = sram)
b_MD1 equ 2 cache/sram mode, bank 1 (1 = sram)
b_MD0 equ 1 cache/sram mode, bank 0 (1 = sram)
b_ENAB equ 0 sram/cache module master enable

LWP3 equ 1<<b_LWP3
LWP2 equ 1<<b_LWP2
LWP1 equ 1<<b_LWP1
LWP0 equ 1<<b_LWP0

INV3 equ 1<<b_INV3
INV2 equ 1<<b_INV2
INV1 equ 1<<b_INV1
INV0 equ 1<<b_INV0

MD3 equ 1<<b_MD3
MD2 equ 1<<b_MD2
MD1 equ 1<<b_MD1
MD0 equ 1<<b_MD0

ENAB equ 1<<b_ENAB

* CIC SRAM Base Address Registers (0-3)
*
CICBADDRX_mask equ $fffffe00 bits 31-9 form sram block base address
b_CICBADDRX_SU equ 0 if set, user/supervisor mode access, otherwise super only
CICBADDRX_SU equ 1<<b_CICBADDRX_SU

 pag
 opt l
