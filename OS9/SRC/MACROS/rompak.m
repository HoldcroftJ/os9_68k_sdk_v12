 nam rompak.m 
 ttl ROMPAK macro and subroutine set

* Copyright 1989, 1990, 1991, 1993, 1994, 1995, 1996
* by Microware Systems Corporation
* Reproduced Under License

* This source code is the proprietary confidential property of
* Microware Systems Corporation, and is provided to licensee
* solely for documentation and educational purposes. Reproduction,
* publication, or distribution in any form to any party other than
* the licensee is strictly prohibited.

********************************
* Edition History
*  #   date   Comments                                          by
* -- -------- ------------------------------------------------- ---
* 01 89-06-03 initial version                                   Rwb
* 02 89-12-06 added register usage, etc for release.            wwb
*              ---- OS-9/68K V2.3 Release ----
*              ---- OS-9/68K V2.4 released ----
* 03 91-10-28 fixed movem typo.                                 wwb
*              ---- OS-9/68K V3.0 Release ----
* 04 94-02-18 Handled case where there is no initialized data   ats
*             section.
* 05 94-05-02 Fixed edition #4.                                 ats
* 06 94-05-13 Added PAKEND macro to find end of initext.		ats
* 07 95-05-22 Updated idata table parsing for spec cases.   rcb/ats
* 08 95-08-18 Optimized PAKEND.                                 gdb
*              ---- OS-9/68K V3.0.2 Release ----
* 09 96-05-31 Workaround l68 bug in initdata relocation tables  ats
*             when there are no initialized data pointer references
*             to the data area. This change forces 1 longword 
*             initialized data pointer in the non-remote data area.
*              ---- OS-9/68K V3.0.3 PwrMan Beta Release ----
* 10 96-07-22 repaired syntax error in PAKEND.					wwb
*              ---- OS-9/68K V3.0.3 Release ----
*              ---- OS-9/68K V3.1 Release ----
*


***
*** WARNING: l68 WORKAROUND
***
*** The linker editions #126-129 were putting 1 excess zero
*** longword after the initialized data to data references 
*** termination when there were no initialized data references
*** to the data area.
***
*** We add 1 longword of non-remote initialized data here to 
*** prevent that from happening.
***
*** To prevent usage of this workaround, add the -aL68_WORKAROUND=0
*** assembler option to the assembler command (-asa for the compiler).
***
 ifndef	L68_WORKAROUND
L68_WORKAROUND equ 1
 endc	

 ifne	L68_WORKAROUND
  vsect
  dc.l bdata
  ends
 endc
***
*** End of l68 workaround
***


**************************************
* ROMPAK definitions
*
RPsync equ 1801546857 sysinit extension module sync code
rp_sinit equ $4 jump table position for extension module to 'SysInit'
rp_sinit2 equ $8 jump table position for extension module to 'SInitTwo'
rp_etext equ $C offset to dc.l etext
rp_etmax equ $8000 maximum size of initext (32K)

* In the case of ROMPAK we will want to locate the end of ROM and
* then execute the code defined by the user (if any).

**************************************
*    This is the code to locate and execute
*  the first sysinit extension.
*
* Registers d0-d7, a0-a6 may be destroyed by this code and the code
* in the "SysInit Extension" code.
* All other registers should remain intact (e.g. VBR, USP, etc)
*
ROMPAK1 macro
 bsr GetRomEnd places pointer to possible sync code in a0
 movea.l d0,a0
 cmpi.l #RPsync,(a0) is it the sync code ( meaning module installed? )
 bne.s NoExtension
 jsr rp_sinit(a0) execute the extension code
NoExtension
 endm

**************************************
*    This is the code to locate and execute
*  the second sysinit extension.
*
* All Registers should be preserved by the extension code.
* This macro ensures that d0-d7, a0-a7 are indeed preserved.
* If the "SInitTwo Extension" code wants to modify any of the
* other registers (e.g. VBR, USP, etc), then it must preserve
* them somehow.
*
ROMPAK2 macro
 movem.l d0-d7/a0-a6,-(sp) save all regs
 bsr GetRomEnd places pointer to possible sync code in a0
 movea.l d0,a0
 cmpi.l #RPsync,(a0) is it the sync code ( meaning module installed? )
 bne.s NoExtentwo
 jsr rp_sinit2(a0) execute the extension code
NoExtentwo
 movem.l (sp)+,d0-d7/a0-a6 restore regs
 endm

*****************************************
*    This is the code to locate the
*  end of the sysinit extension module.
PAKEND macro
 movem.l d1/a0,-(sp) save registers
 bsr GetRomEnd places pointer to possible sync code in d0
 movea.l d0,a0
 cmpi.l #RPsync,(a0) is it the sync code ( meaning module installed? )
 bne.s NoPakEnd if not, d0 is end of ROM
 move.l rp_etext(a0),d1 look at the etext pointer
 cmpi.l #rp_etmax,d1 is it within bounds ?
 bls.s NewPak yup
 moveq.l #0,d0 indicate old style pak (w/o etext)
 bra.s NoPakEnd
NewPak add.l d1,d0 compute end of pak
 addi.l #16,d0 we never have initialized data, so there are 4 longword 0's here
NoPakEnd movem.l (sp)+,d1/a0 restore registers
 endm


***********************************************************
**   GetRomEnd -- locate the end of Boot ROM
*  is used to locate the end of the normal OS-9 boot rom.  It
*  does so by walking through the relocation information.
*
*    This means that the user must merge the sysinit extension code
*  to the end of the normal ROM image if it is to be found.
*
GetRomEnd:
 link a5,#0
 movem.l d1/a2,-(sp)
greoff1 movea.l #edata-*-(getrmend1-greoff1+2),a2
greoff2 cmpa.l #btext-*-(getrmend1-greoff2+2),a2 if no initialized data edata equals btext
 bne.s getrmend1 
greoff3 movea.l #etext-*-(getrmend1-greoff3+2),a2 use etext if no initialized data
getrmend1 lea.l (pc,a2.l),a2  load address of code relocation information
 move.l a2,d0 
getrmend2 tst.l (a2) Any data to code refs?
 bne.s getrmend3 skip the refs
 adda.l #4,a2 advance to the data to data refs
 move.l a2,d0
 bra.s getrmend4
getrmend3 addq.l #4,d0 ... + 4 is the first code relocation pointer
 move.w 2(a2),d1 number of words of relocation information (code)
 ext.l d1
 lsl.l #1,d1 *2 is the size in bytes
 add.l d1,d0 
 movea.l d0,a2 check if were done with code relocation
 bra.s getrmend2
getrmend4 tst.l (a2) Any data to data refs?
 bne getrmend5 skip the refs
 addq.l #4,d0 advance to end of rom
 bra.s getrmend6
getrmend5 addq.l #4,d0 ... + 4 is the first data relocation pointer
 move.w 2(a2),d1 size of the field in words.....
 ext.l d1
 lsl.l #1,d1  .... *2 is size in bytes ....
 add.l d1,d0
 movea.l d0,a2 check if we're done with data relocation
 bra.s getrmend4
getrmend6 movem.l (sp)+,d1/a2
 unlk a5
 rts

* end of file (rompak.m)

