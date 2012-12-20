* Copyright 1991 by Microware Systems Corporation
* Reproduced Under License

* This source code is the proprietary confidential property of
* Microware Systems Corporation, and is provided to licensee
* solely for documentation and educational purposes. Reproduction,
* publication, or distribution in any form to any party other than
* the licensee is strictly prohibited.

***************
* Edition History
*
* #   date      Comments                                        by
* -- -------- ------------------------------------------------- ---
* 01 91/01/06 Collected from a series of files and made globaly 
*             accessable, added BITSET, BITCLR                  Rwb
*             ---- OS-9/68k V2.4.2 Release ----
*             ---- OS-9/68k V2.4.3 Release ----
* 02 93/06/08 removed ERRATA040 conditionals - must keep code	wwb
*             forever if supporting old 68040 masks.
*             ---- OS-9/68k V3.0 Release ----
*             ---- OS-9/68k V3.1 Release ----


********************
* REGMOVE  macro.
* REGMOVE2 macro.
* MEMMOVE  macro.
* GLOBMOVE macro.
*
* These macros are needed so that we can define the access method
* of the device.  For normal devices, simple ".b" instructions
* are all that is required.  For the MVME165, each register of the
* on-board devices are accessed as the LOW BYTE of a LONG-WORD.  Thus, the
* following types of cases need to be considered:
*
*         NOTE: <direction> = "to" device or "from" device.



*  1.  Move data between the device and a register (note that we
*      consider immediate data as the same case).  In this case
*      we simply have to change references from .b to .l, as the
*      pertinant data is in the LOW BYTE of the LONG WORD.
*
*         Syntax:  REGMOVE <direction>,<src>,<dest>
*
*         Note that the direction is assumed to be 'to' the device.
*
REGMOVE macro
 ifne \#-3
 fail longio.m: REGMOVE macro requires three arguments
 endc
 ifeq (CPUType-VME165)
 ifeq '\1'-'to'
 move.l \2,\3
 else
 ifeq '\1'-'from'
 andi.l #0xffffff00,\3 keep high part of dest intact
 move.l \3,-(sp) save it
 move.l \2,\3 get I/O data
 andi.l #0xff,\3 clear off high junk
 or.l (sp)+,\3 combine high part with .b data
 else
 fail longio.m: REGMOVE macro requires 'to' or 'from' as arg #1.
 endc 'from'
 endc 'to'
 else
 move.b \2,\3 do normal style move.b <src>,<dest>
 endc MVME165
 endm

*  2.  Move data between the device and a register.  This is a
*      special case of REGMOVE, as the syntax (An,Dn.w) gets
*      seen as 2 parameters to the macro.  This macro only
*      appears once.
*
*         Syntax:   REGMOVE2 <src reg>,(<dest base>,<index>)
*
*                   e.g. move.b d0,(a0,d2.w)
*
*         Note that the direction is assumed to be 'to' the device.
*
REGMOVE2 macro
 ifne \#-3
 fail longio.m: REGMOVE2 macro requires three arguments (sic)
 endc
 ifeq (CPUType-VME165)
 move.l \1,\2,\3
 else
 move.b \1,\2,\3 do normal style move.b <src>,(an,dn.w)
 endc MVME165
 endm

*  3.  Move data between memory and the device.  In this case, the
*      size of the memory datum will be BYTE, while the I/O Port
*      size is LONG.  Thus we need to reference memory and I/O
*      using different sized datum.
*
*         Syntax:  MEMMOVE <direction>,<src>,<dest>.
*
MEMMOVE macro
 ifne \#-3
 fail longio.m: MEMMOVE macro requires three arguments
 endc
 ifeq (CPUType-VME165)
 move.l d0,-(sp) save register
 ifeq '\1'-'to'
 moveq.l #0,d0 sweep d0
 move.b \2,d0 get memory src datum
 move.l d0,\3 set device dest datum
 else
 ifeq '\1'-'from'
 move.l \2,d0 get device src datum
 move.b d0,\3 set memory dest datum
 else
 fail longio.m: MEMMOVE macro requires 'to' or 'from' as arg #1.
 endc 'from'
 endc 'to'
 movem.l (sp)+,d0 restore register, KEEP CCR status
* this next nop is required in case the following is a movem
* this is a "early silicon issue", and we must keep it if we
* are support old masks
 nop
 else
 move.b \2,\3 do normal style move.b <src>,<dest>
 endc MVME165
 endm

*  4.  Move data between oem global area and the device.  This is
*      a special case of MEMMOVE, due to the fact that the first
*      parameter [ (a6,d0.w) ] from our point of view is 2 parameters
*      from the assembler's view.  Thus, a special case...
*
*         Syntax:  GLOBMOVE <dest>
*
GLOBMOVE macro
 ifne \#-1
 fail longio.m: GLOBMOVE macro requires three arguments
 endc
 ifeq (CPUType-VME165)
 move.l d0,-(sp) save register
 move.b (a6,d0.w),d0 get memory src datum
 move.l d0,\1 set device dest datum
 movem.l (sp)+,d0 restore register, KEEP CCR status
* this next nop is required in case the following is a movem
* this is a "early silicon issue", and we must keep it if we
* are support old masks
 nop
 else
 move.b (a6,d0.w),\1 do normal style move.b <src>,<dest>
 endc MVME165
 endm

*  5. bset on an I/O register is emulated.  
*      
*      
*      Syntax:   BITSET <bit #>,<dest>
*
BITSET macro
 ifne \#-2
 fail longio.m: BITSET macro requires two arguments
 endc
 ifeq (CPUType-VME165)
 move.l d0,-(a7)
 move.l \2,d0 get the registers current contents
 bset.l \1,d0 set the bit required
 move.l d0,\2 send out to the device
 move.l (a7)+,d0
 else
 bset.b \1,\2
 endc MVME165
 endm

*  5. bclr on an I/O register is emulated.  
*      
*      
*      Syntax:   BITCLR <bit #>,<dest>
*
BITCLR macro
 ifne \#-2
 fail longio.m: BITCLR macro requires two arguments
 endc
 ifeq (CPUType-VME165)
 move.l d0,-(a7)
 move.l \2,d0 get the registers current contents
 bclr.l \1,d0 set the bit required
 move.l d0,\2 send out to the device
 move.l (a7)+,d0
 else
 bclr.b \1,\2
 endc MVME165
 endm

