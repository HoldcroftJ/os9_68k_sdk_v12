/*--------------------------------------------------------------------------,
|                                                                           |
|            Copyright 1995-2001 by Microware Systems Corporation           |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, modification, distribution, or creation of    |
|  derivative works in any form to any party other than the Licensee is     |
|  strictly prohibited, unless expressly provided for under the terms of a  |
|  mutually executed written license agreement for this software between    |
|  the end-user and Microware Systems Corporation, or otherwise expressly   |
|  authorized in writing by Microware Systems Corporation.  Licensee shall  |
|  reproduce this copyright notice exactly as provided herein on any copies |
|  of the software that Licensee is authorized to distribute.               |
|---------------------------------------------------------------------------|
|                                                                           |
| flshcach.c - ROM Cache flush module                                       |
|                                                                           |
|                                                                           |
| Edition History:                                                          |
| #   Date    Comments													By	|
| -- -------- -------------------------------------------------------	---	|
| 01 95/07/06 Prehistoric												ats	|
| 02 95/07/14 History started											gls	|
| 03 95/09/13 Added parameter for cache type specification.				ats	|
|             Fixed instruction cache flushing on PPC 603.					|
| 04 95/09/13 Added PPC 604 type.										lac	|
| 05 95/11/06 Added PPC 403 data flushing.								lac	|
| 06 96/02/16 Added PPC 821 type.										lac	|
| 07 96/03/08 Added PPC 602 type.										lac	|
| 08 96/03/15 Added PPC 602 type initialization.						lac	|
|             ---- OS9000/PPC V2.0 ----                                     |
| 09 96/04/10 Added PPC 603e support (pvr = 6).							ats	|
| 10 96/06/18 Fixed bug in 821 cache flushing.							dwj	|
| 11 96/06/19 Fixed bug in 821 cache flushing scaling pointer.			dwj	|
|             ---- OS9000/PPC V2.0.1 ----                                   |
|             ---- OS-9000/x86 V2.1 Released ----                           |
| 12 96/10/14 Linked with new p2lib to fix getrinf().					gdb	|
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
|             ---- OS-9/68K V3.0.3 System-state Debug Support Release ---   |
| 13 96/12/18 Added sync's to 821 specific data flush cache case.		dwj	|
| 14 97/01/09 Added PPC 604e support (pvr == 9).						lac	|
| 15 97/01/21 Added ARM 7110 support.									dwj	|
| 16 97/01/27 Added MIPS 3081 support.									sep	|
| 17 97/02/04 Fixed ARM cputype to be 3									dwj	|
| 18 97/04/17 Added run time check for 821/801.							rkw	|
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
| 19 97/05/19 Added syncs prior to dc_cst access for 821/801.			rkw	|
|             ---- OS-9000/PPC V2.1.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
| 20 97/07/08 Enhanced 800 series runtime check to include 823.			rkw	|
| 21 97/07/14 Added StrongARM 110 support.								dwj	|
| 22 97/09/02 Added StrongARM 1100 support.								dwj	|
|             ---- Modular Rom Sub-system v1.0 Released ----                |
| 23 97/10/06 Added SPARClite support.									gdw	|
| 24 97/10/06 Added (and fixed) MWKK's SH7708 code.						gdb	|
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- OS-9000/SPARC V2.2.7 Released ----                       |
| 25 98/02/06 Added PPC 750 support.									gdb	|
| 26 98/05/08 Fixed the MIPS routines for IDT processors.				gdw	|
|             ---- Modular Rom v1.1 Sub-component Released ----             |
|             ---- Modular ROM v1.2 Sub-component Released ----             |
| 27 98/07/27 Added a write buffer flush fo MIPS.						gdw	|
| 28 99/03/09 Added SH7750 and SH7709A support.							dwj	|
| 29 99/06/18 Fixed many minor problems for SH4.						dwj	|
| 30 99/12/28 Adding SA1110 support.									rkw	|
| 31 00/01/24 Fix SH-3/SH-4 cache flush routines.						and	|
| 32 00/01/21 Added MIPS4000 support.									dwj	|
| 33 00/02/10 Adding PPC860p support.                                   rkw |
| 34 00/02/11 CF7826 - Recognize 603e PID7 as well as PID6              ajk |
| 35 00/03/01 Changed 604e support to also recognize PVR values of 10   rds |
| 36 00/04/08 Adding SA1200 (IXP1200) processor support.                rkw |
| 37 00/05/02 Changing MIPS4000 to MIPS64.								dwj	|
| 38 00/05/12 Added MIPS QED 5200-series processor to MIPS64.           rmf |
| 39 00/05/16 Added Invalidate operation for StrongARMs.					|
|             Added Dcache-on short circuit for SA.                     rkw |
| 40 00/05/22 Added support for the PPC401 and PPC405 processors.       cds |
| 41 00/05/22 Removed MIPS64 cpu checks.								dwj	|
| 42 00/06/22 Access cache registers from P2 area in SH-3 cache_flush.	and |
| 43 00/09/07 Add Toshiba 39xx to MIPS3000 code.                        gdw |
| 44 00/09/21 add copy back support to the MIPS 39xx flush.             gdw |
| 45 00/08/10 Adding MIPS32 support.									dwj	|
| 46 00/10/05 Adding MIPS32 IDT334 support.								dwj	|
| 47 01/02/05 Added 403/5 invalidate operations							rry |
| 48 01/02/07 Added XScale (80200 ARMV5) support.                       gdw |
| 49 01/02/16 Working XScale.                                           gdw |
| 50 01/02/23 Xscale won't flush mini cache of oem_globals not setup.   gdw |
| 51 01/04/04 Adding syncs to MIPS32 for greater stability.				dwj	|
| 52 01/04/26 Adding IDT355 support.									dwj	|
| 53 01/05/03 Fixing IDT flushing, needed to flush 2nd way.				dwj	|
| 54 01/05/18 fix xscale partial flushes.                               gdw |
| 55 01/07/09 enhance 401 support.                                      gdw |
| 56 01/10/03 Added sh5m support                                        gjb |
`--------------------------------------------------------------------------*/

#define	EDITION		56

_asm("_sysedit: equ %0", __obj_constant(EDITION));

#define NEWINFO
#include <rom.h>

#ifdef _MPFMIPS3000
#define _MPFIDT3081
#define _MPFTX3900
#endif

#include <types.h>
#include <const.h>
#include <errno.h>
#include <regs.h>

#ifdef	_OSK
extern void flush_cache(u_int32 cputype); /* supplied by OSK flush*.l files */
#endif



#ifdef	_MPF386
#ifndef	NOT_MICRONICS	/* external flag to select non-Micronics module */
#define	_MICRONICS		/* Micronics module is the default */
#endif
#endif

#if defined(DEBUG)
void printstr(Rominfo, char *str);
void itoaout(Rominfo, u_int32 number);
static void ohexb(Rominfo, u_int32 y);
#endif /* DEBUG */

#ifdef _MPFMIPS3000
void idt_cache_flush(u_int32 *, u_int32, u_int8);
void tx_cache_flush (u_int32 *, u_int32, u_int8, Rominfo);
void tx_icache_flush (u_int32 * addr, u_int32 size);
void tx_dcache_flush (u_int32 * addr, u_int32 size);
#endif

#if defined(_MPFMIPS32)
void	query_i_cache(u_int32* , u_int32*);
void	query_d_cache(u_int32* , u_int32*);
void	query_idt_i_cache(u_int32* , u_int32*);
void	query_idt_d_cache(u_int32* , u_int32*);
void	dflush_mipidt(u_int32, u_int32);
void	iflush_mipidt(u_int32, u_int32);
#endif

#if defined(_MPFMIPS64)
void iflush_mip4(u_int32, u_int32);
void dflush_mip4(u_int32, u_int32);
void sflush_mip4(u_int32, u_int32);
#endif

#ifdef _MPFSH3
u_int32		ccr_read(void *);
void		ccr_write(void *, u_int32);
#define WAY 4
#define LINES_7709 128
#define LINES_7709A 256
#define SHIFTER_7709 11
#define SHIFTER_7709A 12
#define MAXLOOP_7709 500
#define MAXLOOP_7709A 1000
#endif

#ifdef _MPFSH4
u_int32		ccr_read(void *);
void		ccr_write(void *, u_int32);
#define CACHE_BLKSZ 32
#define	DLINES 512
#endif

#ifdef _MPFARM
void xscale_cache_flush(u_int32*, u_int32, u_int8, Rominfo);
#endif

#ifdef	_MPFPOWERPC
#include <getset.h>
/*
 *	These should be pulled in from the appropriate regXXX.h file, but until
 *	this source file is split out for different processors....
 */
#define	HID0_DCE	0x00004000
#define	HID0_DCI	0x00000400

/*
 *	821 cache control status & commands
 */
#define IC_CST_IEN			0x80000000
#define	IC_CST_ENABLE		0x02000000
#define	IC_CST_DISABLE		0x04000000
#define	IC_CST_UNLOCK_ALL	0x0A000000
#define	IC_CST_INVALIDATE	0x0C000000
#define DC_CST_DEN			0x80000000
#define	DC_CST_ENABLE		0x02000000
#define	DC_CST_DISABLE		0x04000000
#define	DC_CST_UNLOCK_ALL	0x0A000000
#define	DC_CST_INVALIDATE	0x0C000000

/*
 * 8xx IMMR Part Mask Value
 *
 * The 800 series PowerPC currently does not make use of the 
 * PVR to distinguish processors. There is a coralation 
 * between the IMMR and the processor and cache/mmu being used.
 * The PARTNUM subfield in the IMMR (bits 16-23) will have the 
 * following values:
 *
 *          Part#          Mask#
 *   821    0x00           0x00 on up
 *   860    0x00           0x00 on up
 *   860T   0x00           0x00 on up  (Rev C and below, 50Mhz)   
 *   801    0x10           0x00 on up
 *   823    0x20           0x00 on up
 *   850    0x20           0x01 on up
 *	 860p   0x05           0x00 on up
 *   860p   0x40 (old)     0x00 on up
 *
 *  The following mask can be used to make a runtime check.
 */
#define IMMR_801_823_850 0x3000
#define IMMR_860P        0x4500
void size_401(u_int32 *, u_int32 *);
#endif

#ifdef _MPFSH5M
void cache_flush_sh5m(u_int32 *addr, u_int32 size, u_int8 type, Rominfo rinf);
#endif

/* cache flushing service */

void cache_flush(u_int32 *addr, u_int32 size, u_int8 type, Rominfo rinf)
{
#ifdef	_OSK
	_asm(-100, " nop "); /* ensure writes are completed */
	flush_cache(rinf->hardware->cpu68k.cc_cputype);
#endif

#ifdef _MPSH5M
	cache_flush_sh5m(addr,size,type,rinf);
#endif

#ifdef	_MPFSPARC
	int	proc_id;
	_asm(-100,"
	rd		psr,%0
	",
	__reg_gen(__obj_modify(proc_id))
	);
	proc_id = proc_id & 0xff000000;

	switch (proc_id) {
/* if its a SPARClite then hit the apropriate cache invalidate register */
		case 0x0f000000:
			if (type == HW_CACHETYPE_DATA) {
	_asm(-100,"
	sethi	hi(1000),%1
	or		g0,lo(1000),%1
	or		g0,3,%0
	sta		%0,[%1]0xe
	sethi	hi(80001000),%1
	or		g0,lo(80001000),%1
	sta		%0,[%1]0xe
	",
	__reg_gen(),
	__reg_gen()
	);
			} else {
	_asm(-100,"
	sethi	hi(1000),%1
	or		g0,lo(1000),%1
	or		g0,3,%0
	sta		%0,[%1]0xc
	sethi	hi(80001000),%1
	or		g0,lo(80001000),%1
	sta		%0,[%1]0xc
	",
	__reg_gen(),
	__reg_gen()
	);
			}
			break;
		default:
			break;
	}
#endif

#ifdef	_MPF386
	switch(rinf->hardware->cpu386.cc_cputype) {
		case 80386:
#ifdef	_MICRONICS	/* this code is specific to Micronics motherboards */
			{
				u_char cache_state;
				volatile char *cache_cntrl = (char *)0x80C00002;
				
				cache_state = *cache_cntrl;
				*cache_cntrl = 0;				/* disable cache */
				*cache_cntrl = cache_state;		/* restore cache state */
			}
#endif
			_asm(-100, "
				jmp %0
%0
				nop
				nop
				", __label());
			break;

		default: /* 80486 & Pentium */
			/* do a "write-back invalidate cache" instruction */
			_asm(-100, "
				dc.b $0F
				dc.b $09 wbinvd instruction
				");
	}
#endif

#ifdef _MPFARM
/*  #define ARM710A  0x????7100
 *  #define ARM810   0x????8100 */
#define SA110   0x4401A100
#define SA1100  0x4401A110
#define SA1110  0x4401B110
#define SA1110B 0x6901B110
#define SA1200  0x6901C120
#define ARM720T 0x41807200
#define XS80200 0x69052000
	u_int32 id = 0;
	u_int32 CntlReg = 0;

	switch(rinf->hardware->cpuarm.cc_cputype) 
	{
	
	case 3:
		_asm(" mcr p15,0,r7,c7,c0");
		_asm(" mov r0,r0");
		_asm(" mov r0,r0");
		_asm(" mov r0,r0");
		break;
	case 4:
    case 5:  /* both 4 & 5 can look at the ID register */
		_asm(" mrc p15,0,%0,c0,c0",__reg_gen(__obj_modify(id)));
		id &= 0xFFFFFFF0;	/*	Mask off revision number	*/

		if (id == XS80200)
        {
          /* skip the wrapper from now on */
          rinf->hardware->flush_cache = &xscale_cache_flush;
          xscale_cache_flush(addr, size, type, rinf);
          break;
        }

		_asm(" mrc p15,0,%0,c1,c1",__reg_gen(__obj_modify(CntlReg))); /* get cntl reg */

		/*	Check for processor type	*/
		if (id == SA110)
		{
#define CACHE_BLKSZ 32
#define DCACHE_SIZE (1024*16)
#define D_CACHE_ON_MASK 0x4

			if (type == HW_CACHETYPE_INST)
			{
				/*	Flush instruction cache	*/
				_asm(-100," mcr p15,0,%0,c7,c5,0    * Flush all of I cache
							mov r0,r0				* Nop
							mov r0,r0				* Nop
							mov r0,r0				* Nop
							mov r0,r0				* Nop
					", __reg_gen());
			}

	  		/* now that finished with Icache, if the Dcache is not even on, its not worth
			 * going through the flush motion.
			 */
			if(!(CntlReg & D_CACHE_ON_MASK))
				break;

			if (type == HW_CACHETYPE_DATA)
			{
				u_int32  *end_addr;
				u_int32 *flshblk;

				if (size == 0)
				{
					flshblk = NULL;
					/*	Fill cache from address 0	*/
						_asm(-100,"	add %1,%0,%3	* find end of read region
%5
							ldr %2,[%0],%4		* fill cache & inc. ptr.
							cmp %0,%1			* At the end??
							bne %5
							mov r0,r0			* NOP
							mov r0,r0			* NOP
							mcr p15,0,%1,c7,c10,4	* drain write buffer
							mov r0,r0			* NOP
							mov r0,r0			* NOP
						",__reg_gen(__obj_modify(flshblk)),
						__reg_gen(),
						__reg_gen(),
						__obj_constant(DCACHE_SIZE),
						__obj_constant(CACHE_BLKSZ),
						__label());

				}
				else
				{
					/* flush specific addresses from the cache	*/
					for (end_addr = (u_int32 *) ((u_int32)addr + size - 1);
							addr < end_addr;addr += (CACHE_BLKSZ/sizeof(addr)))
					{
						_asm(-100,"	mcr p15,0,%0,c7,c10,1   * clean entry
									mcr p15,0,%0,c7,c6,1	* flush entry
									mov r0,r0               * Nop
									mov r0,r0               * Nop
									mov r0,r0               * Nop
								",__reg_gen(__obj_modify(addr)));
					}
					_asm(-100,"	mcr p15,0,%0,c7,c10,1   * clean entry
								mcr p15,0,%0,c7,c6,1    * flush entry
								mov r0,r0               * Nop
								mov r0,r0               * Nop
								mov r0,r0               * Nop
								mcr p15,0,%0,c7,c10,4	* drain write buff
							",__reg_gen(__obj_modify(end_addr)));
				}
			}

			if (type == HW_CACHETYPE_DATA_INV)
			{
				u_int32  *end_addr;
				
				if (size == 0)
				{				 
				/* Flash Invalidate all of Data cache cache	*/
				_asm(-100," mcr p15,0,%0,c7,c6,0    * Flush all of D cache
							mov r0,r0				* Nop
							mov r0,r0				* Nop
							mov r0,r0				* Nop
				", __reg_gen());

				}
				else
				{
					/* Invalidate specific addresses from the cache	*/
					for (end_addr = (u_int32 *) ((u_int32)addr + size - 1);
							addr < end_addr;addr += (CACHE_BLKSZ/sizeof(addr)))
					{
						_asm(-100,"	mcr p15,0,%0,c7,c6,1	* Invalidate entry.
									mov r0,r0               * Nop
									mov r0,r0               * Nop
									mov r0,r0               * Nop
								",__reg_gen(__obj_modify(addr)));
					}
					_asm(-100," mcr p15,0,%0,c7,c6,1    * Invalidate entry
								mov r0,r0               * Nop
								mov r0,r0               * Nop
								mov r0,r0               * Nop
								mcr p15,0,%0,c7,c10,4	* drain write buff
							",__reg_gen(__obj_modify(end_addr)));
				}
			}

			break;
#undef CACHE_BLKSZ
#undef DCACHE_SIZE
#undef D_CACHE_ON_MASK
		}

		if ((id == SA1100) || (id == SA1110) || (id == SA1110B) || (id == SA1200))
		{
#define CACHE_BLKSZ 32
#define DCACHE_SIZE (1024*8)
#define FLUSH_MEMORY_11X0 0xE0000000      /* flushable memory area for SA11x0 */
#define FLUSH_MEMORY_1200 FLUSH_MEMORY_11X0      /* flushable memory area for SA1200 */
#define FLUSH_MINI_OFFSET_11X0 0x00100000 /* Historic (1meg systlb) offset for SA11x0 */
#define FLUSH_MINI_OFFSET_1200 FLUSH_MINI_OFFSET_11X0 /* offset for mini flush on IXP1200 */
#define MINI_SIZE 512                     /* size of mini cache */
#define D_CACHE_ON_MASK 0x4

			u_int32 flush_memory = FLUSH_MEMORY_11X0;
			u_int32 flush_mini_offset = FLUSH_MINI_OFFSET_11X0;

			if(id == SA1200)
			{
				flush_memory = FLUSH_MEMORY_1200;
				flush_mini_offset = FLUSH_MINI_OFFSET_1200;
			}

			if (type == HW_CACHETYPE_INST)
			{
				/*	Flush instruction cache	*/
				_asm(-100," mcr p15,0,%0,c7,c5,0    * Flush all of I cache
							mov r0,r0				* Nop
							mov r0,r0				* Nop
							mov r0,r0				* Nop
							mov r0,r0				* Nop
					", __reg_gen());
			}

	  		/* now that finished with Icache, if the Dcache is not even on, its not worth
			 * going through the flush motion.
			 */
			if(!(CntlReg & D_CACHE_ON_MASK))
				break;

			if (type == HW_CACHETYPE_DATA)
			{
				u_int32  *end_addr;
				u_int32 *flshblk;

				if (size == 0)
				{
					flshblk = (u_int32 *)flush_memory;
					/*	The FLUSH_MEMORY is an area for flushing */
					_asm(-100,"	add %1,%0,%3	* find end of read region
%5
							ldr %2,[%0],%4		* fill cache & inc. ptr.
							cmp %0,%1			* At the end??
							bne %5
							mcr p15,0,%1,c7,c6,0	* flush cache
							mov r0,r0			* NOP
							mov r0,r0			* NOP
							mcr p15,0,%1,c7,c10,4	* drain write buffer
							mov r0,r0			* NOP
							mov r0,r0			* NOP
							mov r0,r0			* NOP
							mov r0,r0			* NOP
						",__reg_gen(__obj_modify(flshblk)),
						__reg_gen(),
						__reg_gen(),
						__obj_constant(DCACHE_SIZE),
						__obj_constant(CACHE_BLKSZ),
						__label());

					/*	Now need to hit mini cache area	*/
					flshblk = (u_int32 *) (flush_memory+flush_mini_offset);
					_asm(-100," add %1,%0,%3	* find end of read region
							mov %2,%1			* save end address
%5
							ldr %6,[%0],%4		* fill in way 1
							ldr %6,[%1],%4		* fill in way 2
							cmp %0,%2
							bne %5
							mov r0,r0			* NOP
							mov r0,r0			* NOP

						",__reg_gen(__obj_modify(flshblk)),
						__reg_gen(),
						__reg_gen(),
						__obj_constant(MINI_SIZE),
						__obj_constant(CACHE_BLKSZ),
						__label(),
						__reg_gen());

				}
				else
				{
					/* flush specific addresses from the cache	*/
					for (end_addr = (u_int32 *) ((u_int32)addr + size - 1);
							addr < end_addr;addr += (CACHE_BLKSZ/sizeof(addr)))
					{
						_asm(-100,"	mcr p15,0,%0,c7,c10,1   * clean entry
									mcr p15,0,%0,c7,c6,1	* flush entry
									mov r0,r0               * Nop
									mov r0,r0               * Nop
									mov r0,r0               * Nop
								",__reg_gen(__obj_modify(addr)));
					}
					_asm(-100,"	mcr p15,0,%0,c7,c10,1   * clean entry
								mcr p15,0,%0,c7,c6,1    * flush entry
								mov r0,r0               * Nop
								mov r0,r0               * Nop
								mov r0,r0               * Nop
								mcr p15,0,%0,c7,c10,4	* drain write buff
							",__reg_gen(__obj_modify(end_addr)));

					/*	Now need to hit mini cache area	*/
					flshblk = (u_int32 *) (flush_memory+flush_mini_offset);
					_asm(-100," add %1,%0,%3	* find end of read region
							mov %2,%1			* save end address
%5
							ldr %6,[%0],%4		* fill in way 1
							ldr %6,[%1],%4		* fill in way 2
							cmp %0,%2
							bne %5
							mov r0,r0			* NOP
							mov r0,r0			* NOP

						",__reg_gen(__obj_modify(flshblk)),
						__reg_gen(),
						__reg_gen(),
						__obj_constant(MINI_SIZE),
						__obj_constant(CACHE_BLKSZ),
						__label(),
						__reg_gen());

				}
            }

			if (type == HW_CACHETYPE_DATA_INV)
			{
				u_int32  *end_addr;
				
				if (size == 0)
				{				 
				/* Flash Invalidate all of Data cache cache	*/
				_asm(-100," mcr p15,0,%0,c7,c6,0    * Flush all of D cache
							mov r0,r0				* Nop
							mov r0,r0				* Nop
							mov r0,r0				* Nop
				", __reg_gen());

				}
				else
				{
					/* Invalidate specific addresses from the cache	*/
					for (end_addr = (u_int32 *) ((u_int32)addr + size - 1);
							addr < end_addr;addr += (CACHE_BLKSZ/sizeof(addr)))
					{
						_asm(-100,"	mcr p15,0,%0,c7,c6,1	* Invalidate entry.
									mov r0,r0               * Nop
									mov r0,r0               * Nop
									mov r0,r0               * Nop
								",__reg_gen(__obj_modify(addr)));
					}
					_asm(-100," mcr p15,0,%0,c7,c6,1    * Invalidate entry
								mov r0,r0               * Nop
								mov r0,r0               * Nop
								mov r0,r0               * Nop
								mcr p15,0,%0,c7,c10,4	* drain write buff
							",__reg_gen(__obj_modify(end_addr)));
				}
			}
			break;

#undef CACHE_BLKSZ
#undef DCACHE_SIZE
#undef FLUSH_MEMORY_11X0
#undef FLUSH_MEMORY_1200
#undef MINI_SIZE
#undef FLUSH_MINI_OFFSET_1200
#undef FLUSH_MINI_OFFSET_11X0
#undef D_CACHE_ON_MASK
		}

		if (id == ARM720T) {
		  /* ARM720T has a unified instruction and data cache.
		  ** 4 way, 16 bytes per line, 8K.  Note, this relies on the
		  ** fact that the low level vectors code (membegin in romcore) is
		  ** in cache.  The high level cache routine handles this better. */
#define CACHE_BLKSZ 16
#define CACHE_WAY 4
#define CACHE_BYTES_PER_WAY 2048
#define CACHE_WAY_MASK (CACHE_BYTES_PER_WAY-1)

		  volatile u_int8 val;
		  u_int8 *ptr, *end;
		  int i;
		  addr = (u_int32*)((u_int32)rinf->rom->rom_vectors | ((u_int32)addr & CACHE_WAY_MASK));
		  for (i = 0; i < CACHE_WAY+1; i++) {
			ptr = (u_int8*)addr;
			end = ptr + size;
			
			while (ptr <= end) {
			  val = *ptr;
			  ptr += CACHE_BLKSZ;
			}
			
			addr += (CACHE_BYTES_PER_WAY >> 2);
		  }
#undef CACHE_BLKSZ
#undef CACHE_WAY
#undef CACHE_BYTES_PER_WAY		  
#undef CACHE_WAY_MASK
		}
	default:
		_asm(" mov r0,r0");
		_asm(" mov r0,r0");
		_asm(" mov r0,r0");

	}
#endif

#ifdef _MPFMIPS3000
	switch(rinf->hardware->cpuarm.cc_cputype)
	{
		case 3041:
		case 3051:
		case 3052:
		case 3071:
		case 3081:
			idt_cache_flush(addr, size, type);
		    break;
		case 3900:
		case 3901:
		case 3902:
		case 3904:
		case 3927:
			tx_cache_flush(addr, size, type, rinf);
			break;
	}
#endif

#if defined(_MPFMIPS32)
	{
		switch(rinf->hardware->cpumips.cc_cputype)
		{
			case MIPS32_IDT355:
			case MIPS32_IDT334:
			{
				u_int32	icache_size = 0;
				u_int32	icache_linesize = 0;
				u_int32	dcache_size = 0;
				u_int32	dcache_linesize = 0;

				query_idt_i_cache(&icache_size, &icache_linesize);
				query_idt_d_cache(&dcache_size, &dcache_linesize);

				if (type == HW_CACHETYPE_DATA)
				{
					dflush_mipidt(dcache_size, dcache_linesize);
				}
				if (type == HW_CACHETYPE_INST)
				{
					iflush_mipidt(icache_size, icache_linesize);
				}

			}
			break;
			default:
			{
				/* First, found how big cache stuff is */
				u_int32	icache_lines = 0;
				u_int32	icache_linesize = 0;
				u_int32	dcache_lines = 0;
				u_int32	dcache_linesize = 0;
				u_int32	line;
				u_int32	addr;

				query_i_cache(&icache_lines, &icache_linesize);
				query_d_cache(&dcache_lines, &dcache_linesize);

				/* Now flush dcaches */
				for(line = 0, addr = SEG_KSEG0; line < dcache_lines;
											line++, addr += dcache_linesize)
				{
					_asm("
						cache	DCACHE_INDEX_WB_INVALIDATE,0(%0)
					",__reg_gen(addr));
				}
				/* Sync up instructions */
				_asm(" sync");
				_asm(" sync");
				_asm(" sync");
				_asm(" sync");
				_asm(" sync");

				/* Now flush icaches */
				for(line = 0, addr = SEG_KSEG0; line < icache_lines;
											line++, addr += icache_linesize)
				{
					_asm("
						cache	ICACHE_INDEX_INVALIDATE,0(%0)
					",__reg_gen(addr));
				}
				/* Sync up instructions */
				_asm(" sync");
				_asm(" sync");
				_asm(" sync");
				_asm(" sync");
				_asm(" sync");
				_asm(" sync");
			}
		}
	}
#endif

#if defined(_MPFMIPS64)
	{
		/* First, find out how big cache stuff is */
		u_int32 icache_size = 0;
		u_int32 dcache_size = 0;
		u_int32 scache_size = 0;
		u_int32 icache_linesize = 0;
		u_int32 dcache_linesize = 0;
		u_int32 scache_linesize = 0;
		_asm("
*
* static void _size_cache(void)
*
* Routine to determine cache sizes by looking at the R4xxx
* CONFIG register.  Sizes are returned in registers, as
* follows:
*
*  %2 = icache size
*  %3 = dcache size
*  %4 = icache linesize
*  %5 = dcache linesize
*  %6 = scache size - secondary cache
*  %7 = scache linesize
*

_size_cache
	mfc0	%0,$C0_CONFIG						get CONFIG register

* get I-cache size in bytes
	andi	%1,%0,CFG_ICMASK					and with i-cache size mask
	srl		%1,%1,CFG_ICSHIFT					shift down
	ori		%2,zero,0x1000
	sllv	%2,%2,%1							I-cache size = 2**12+IC
* get d-cache size in bytes
	andi	%1,%0,CFG_DCMASK					and with d-cache size mask
	srl		%1,%1,CFG_DCSHIFT					shift down
	ori		%3,zero,0x1000
	sllv	%3,%3,%1							D-cache size = 2**12+DC
* determine instruction cache line size
	ori		%4,zero,32
	andi	%1,%0,CFG_IB
	bne		%1,zero,%8
	nop
	ori		%4,zero,16
* determine data cache line size
%8
	ori		%5,zero,32
	andi	%1,%0,CFG_DB
	bne		%1,zero,%9
	nop
	ori		%5,zero,16
%9
	or		%6,zero,zero						assume no scache
	or		%7,zero,zero						assume scache line size = 0
	andi	%1,%0,CFG_C_UNCACHED				test CONFIG register
	bne		%1,zero,%10
	nop
	lui		%6,0x10								assume 1MB scache
	lui		%1,hi(CFG_SBMASK)
	and		%1,%0,%1
	srl		%1,%1,CFG_SBSHIFT
	ori		%7,zero,16
	sllv	%7,%7,%1
%10

	",
	__reg_gen(),									/* %0 */
	__reg_gen(),									/* %1 */
	__reg_gen(__obj_modify(icache_size)),			/* %2 */
	__reg_gen(__obj_modify(dcache_size)),			/* %3 */
	__reg_gen(__obj_modify(icache_linesize)),		/* %4 */
	__reg_gen(__obj_modify(dcache_linesize)),		/* %5 */
	__reg_gen(__obj_modify(scache_size)),			/* %6 */
	__reg_gen(__obj_modify(scache_linesize)),		/* %7 */
	__label(),										/* %8 */
	__label(),										/* %9 */
	__label()										/* %10 */
	);

		if (type == HW_CACHETYPE_INST)
		{
			iflush_mip4(icache_size, icache_linesize);
		}
		if (type == HW_CACHETYPE_DATA)
		{
			dflush_mip4(dcache_size, dcache_linesize);
		}
		sflush_mip4(scache_size, scache_linesize);
	}
#endif

#ifdef	_MPFPOWERPC
	u_int32 *end;

	switch(rinf->hardware->cpuppc.cc_cputype) {

    case 401: {
      u_int32   linesi, linesd;
      
      size_401(&linesi, &linesd);

#define WORDS_PER_LINE	4

			if (type == HW_CACHETYPE_INST) {
				if (size == 0) {
					/* invalidate all congruence classes */
					for (addr = NULL, end = addr + (WORDS_PER_LINE * linesi);
							addr < end; addr += WORDS_PER_LINE)
						_asm(" iccci 0,%0 ", __reg_gen(__obj_modify(addr)));
				} else {
					for (end = addr + (size / 4) - 1; addr < end;
							addr += WORDS_PER_LINE)
						_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(addr)));
					_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(end)));
				}
				_asm(-100, " isync ");
			}
			else if (type == HW_CACHETYPE_DATA) {
				if (size == 0) {
					u_int32 off2 = (WORDS_PER_LINE * 4 * linesd) * 1;

					/*
					 *	To flush cache entire cache, first fill it with
					 *	known contents then insure those contents are
					 *	written to memory
					 */
					for (addr = NULL, end = addr + (WORDS_PER_LINE * linesd);
							addr < end; addr += WORDS_PER_LINE)
					{
						_asm(16, "
							lwz %1,0(%0) load set one
							lwzx %1,%0,%2 load set two
							dcbst 0,%0 insure set one is written
							dcbst %0,%2 insure set two is written
							",
							__reg_base(__obj_modify(addr)),		/* %0 */
							__reg_gen(),						/* %1 */
							__reg_base(__obj_modify(off2))		/* %2 */
						);
					}
				} else {
					/* flush specific addresses from the cache */
					for (end = addr + (size / 4) - 1; addr < end;
							addr += WORDS_PER_LINE)
						_asm(" dcbf 0,%0 ", __reg_base(__obj_modify(addr)));
					_asm(" dcbf 0,%0 ", __reg_base(__obj_modify(end)));
				}
				_asm(-100, " sync ");	/* ensure writes are completed */

			}
			/*
			 * NOTE: We'd like to have used the HW_CACHETYPE_DATA_INV define
			 * here, but since all the PowerPC flshcach modules are built at
			 * one time, we have no way to determine if we're a 40x at compile
			 * time. Thus, HW_CACHETYPE_DATA_INV is 2 (see rom.h).
			 */
			else if (type == 4 /* HW_CACHETYPE_DATA_INV */ && size) {
				/*
				 * invalidate specific addresses from the cache, you cannot
				 * invalidate the entire data cache, that's not nice and would
				 * likely cause a crash.
				 */
				for (end = addr + (size / 4) - 1; addr < end;
						addr += WORDS_PER_LINE)
					_asm(" dcbi 0,%0 ", __reg_base(__obj_modify(addr)));
				_asm(" dcbi 0,%0 ", __reg_base(__obj_modify(end)));

				_asm(-100, " sync ");	/* ensure writes are completed */
			}
    }
			break;

#undef	WORDS_PER_LINE

		case 403:

#define WORDS_PER_LINE	4

			if (type == HW_CACHETYPE_INST) {

#define	NUM_LINES		64

				if (size == 0) {
					/* invalidate all congruence classes */
					for (addr = NULL, end = addr + (WORDS_PER_LINE * NUM_LINES);
							addr < end; addr += WORDS_PER_LINE)
						_asm(" iccci 0,%0 ", __reg_gen(__obj_modify(addr)));
				} else {
					for (end = addr + (size / 4) - 1; addr < end;
							addr += WORDS_PER_LINE)
						_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(addr)));
					_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(end)));
				}
				_asm(-100, " isync ");

#undef	NUM_LINES

			}
			else if (type == HW_CACHETYPE_DATA) {

#define	NUM_LINES		32

				if (size == 0) {
					u_int32 off2 = (WORDS_PER_LINE * 4 * NUM_LINES) * 1;

					/*
					 *	To flush cache entire cache, first fill it with
					 *	known contents then insure those contents are
					 *	written to memory
					 */
					for (addr = NULL, end = addr + (WORDS_PER_LINE * NUM_LINES);
							addr < end; addr += WORDS_PER_LINE)
					{
						_asm(16, "
							lwz %1,0(%0) load set one
							lwzx %1,%0,%2 load set two
							dcbst 0,%0 insure set one is written
							dcbst %0,%2 insure set two is written
							",
							__reg_base(__obj_modify(addr)),		/* %0 */
							__reg_gen(),						/* %1 */
							__reg_base(__obj_modify(off2))		/* %2 */
						);
					}
				} else {
					/* flush specific addresses from the cache */
					for (end = addr + (size / 4) - 1; addr < end;
							addr += WORDS_PER_LINE)
						_asm(" dcbf 0,%0 ", __reg_base(__obj_modify(addr)));
					_asm(" dcbf 0,%0 ", __reg_base(__obj_modify(end)));
				}
				_asm(-100, " sync ");	/* ensure writes are completed */

			}
			/*
			 * NOTE: We'd like to have used the HW_CACHETYPE_DATA_INV define
			 * here, but since all the PowerPC flshcach modules are built at
			 * one time, we have no way to determine if we're a 40x at compile
			 * time. Thus, HW_CACHETYPE_DATA_INV is 2 (see rom.h).
			 */
			else if (type == 4 /* HW_CACHETYPE_DATA_INV */ && size) {
				/*
				 * invalidate specific addresses from the cache, you cannot
				 * invalidate the entire data cache, that's not nice and would
				 * likely cause a crash.
				 */
				for (end = addr + (size / 4) - 1; addr < end;
						addr += WORDS_PER_LINE)
					_asm(" dcbi 0,%0 ", __reg_base(__obj_modify(addr)));
				_asm(" dcbi 0,%0 ", __reg_base(__obj_modify(end)));

				_asm(-100, " sync ");	/* ensure writes are completed */
			}

#undef	NUM_LINES

			break;

#undef	WORDS_PER_LINE


		case 405:

#define WORDS_PER_LINE	8

			if(type == HW_CACHETYPE_INST)
			{

#define	NUM_LINES		256

				if(size == 0)
				{
					/* invalidate all congruence classes */

					/*
					 * The iccci instruction on the 405 invalidates
					 * the entire instruction cache.
					 */
					_asm("
							xor %0,%0,%0
							iccci 0,%0 invalidate entire instruction cache
						"
						, __reg_gen());
				}
				else
				{
					for(end= addr + (size / sizeof(u_int32)) - 1
							; addr < end; addr += WORDS_PER_LINE)
						_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(addr)));

					_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(end)));
				}

				_asm(-100, " isync ");

#undef	NUM_LINES

			}
			else if (type == HW_CACHETYPE_DATA) {

#define	NUM_LINES		128

				if(size == 0)
				{
					u_int32 off2= WORDS_PER_LINE * sizeof(u_int32) * NUM_LINES;

					/*
					 *	To flush cache entire cache, first fill it with
					 *	known contents then insure those contents are
					 *	written to memory
					 */
					for(addr= NULL, end= addr + (WORDS_PER_LINE * NUM_LINES);
							addr < end; addr += WORDS_PER_LINE)
					{
						_asm(16, "
							lwz %1,0(%0) load set one
							lwzx %1,%0,%2 load set two
							dcbst 0,%0 insure set one is written
							dcbst %0,%2 insure set two is written
							",
							__reg_base(__obj_modify(addr)),		/* %0 */
							__reg_gen(),						/* %1 */
							__reg_base(__obj_modify(off2))		/* %2 */
						);
					}
				}
				else
				{
					/* flush specific addresses from the cache */
					for(end= addr + (size / sizeof(u_int32)) - 1
							; addr < end; addr += WORDS_PER_LINE)
						_asm(" dcbf 0,%0 ", __reg_base(__obj_modify(addr)));

					_asm(" dcbf 0,%0 ", __reg_base(__obj_modify(end)));
				}

				_asm(-100, " sync ");	/* ensure writes are completed */

			}
			/*
			 * NOTE: We'd like to have used the HW_CACHETYPE_DATA_INV define
			 * here, but since all the PowerPC flshcach modules are built at
			 * one time, we have no way to determine if we're a 40x at compile
			 * time. Thus, HW_CACHETYPE_DATA_INV is 2 (see rom.h).
			 */
			else if (type == 4 /* HW_CACHETYPE_DATA_INV */ && size) {
				/*
				 * invalidate specific addresses from the cache, you cannot
				 * invalidate the entire data cache, that's not nice and would
				 * likely cause a crash.
				 */
				for (end = addr + (size / 4) - 1; addr < end;
						addr += WORDS_PER_LINE)
					_asm(" dcbi 0,%0 ", __reg_base(__obj_modify(addr)));
				_asm(" dcbi 0,%0 ", __reg_base(__obj_modify(end)));

				_asm(-100, " sync ");	/* ensure writes are completed */
			}

#undef	NUM_LINES
#undef	WORDS_PER_LINE

			break;

		case 602:

#define	NUM_LINES		64
#define WORDS_PER_LINE	8

			if (type == HW_CACHETYPE_INST) {
				if (size == 0) {
					_asm(-100, "
						mfspr %0,hid0 get hardware implementation register
						ori %1,%0,$0800 set the ICFI bit
						isync
						mtspr hid0,%1 restore hid0 with ICFI bit set
						mtspr hid0,%0 restore original hid0
						",
						__reg_gen(),	/* %0 */
						__reg_gen()		/* %1 */
					);
				} else {
					for (end = addr + (size / 4) - 1; addr < end;
							addr += WORDS_PER_LINE)
						_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(addr)));
					_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(end)));
				}
				_asm(-100, " isync ");
			}
			if (type == HW_CACHETYPE_DATA) {
				if (size == 0) {
					u_int32 off2 = (WORDS_PER_LINE * 4 * NUM_LINES) * 1;

					/*
					 *	To flush cache entire cache, first fill it with
					 *	known contents then insure those contents are
					 *	written to memory
					 */
					for (addr = NULL, end = addr + (WORDS_PER_LINE * NUM_LINES);
							addr < end; addr += WORDS_PER_LINE)
					{
						_asm(16, "
							lwz %1,0(%0) load set one
							lwzx %1,%0,%2 load set two
							dcbst 0,%0 insure set one is written
							dcbst %0,%2 insure set two is written
							",
							__reg_base(__obj_modify(addr)),		/* %0 */
							__reg_gen(),						/* %1 */
							__reg_base(__obj_modify(off2))		/* %2 */
						);
					}
				} else {
					/* flush specific addresses from the cache */
					for (end = addr + (size / 4) - 1; addr < end;
							addr += WORDS_PER_LINE)
						_asm(" dcbf 0,%0 ", __reg_base(__obj_modify(addr)));
					_asm(" dcbf 0,%0 ", __reg_base(__obj_modify(end)));
				}
				_asm(-100, " sync ");	/* ensure writes are completed */
			}
			break;

#undef	NUM_LINES
#undef	WORDS_PER_LINE

		case 603:
		case 8240:
		case 8260:

#define	NUM_LINES		128
#define WORDS_PER_LINE	8
#define IS_603E(pv) (pv == 6 || pv == 7 || pv == 0x81)

			if (type == HW_CACHETYPE_INST) {
				if (size == 0) {
					_asm(-100, "
						mfspr %0,hid0 get hardware implementation register
						ori %1,%0,$0800 set the ICFI bit
						isync
						mtspr hid0,%1 restore hid0 with ICFI bit set
						mtspr hid0,%0 restore original hid0
						",
						__reg_gen(),	/* %0 */
						__reg_gen()		/* %1 */
					);
				} else {
					for (end = addr + (size / 4) - 1; addr < end;
							addr += WORDS_PER_LINE)
						_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(addr)));
					_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(end)));
				}
				_asm(-100, " sync ");
				_asm(-100, " isync ");
			}
			if (type == HW_CACHETYPE_DATA) {
				u_int32	pv = (_get_pvr() >> 16) & 0xffff;
				
				if (size == 0) {
					u_int32 off2 = (WORDS_PER_LINE * 4 * NUM_LINES) * 1,
							off3 = (WORDS_PER_LINE * 4 * NUM_LINES) * 2,
							off4 = (WORDS_PER_LINE * 4 * NUM_LINES) * 3;

					/*
					 *	To flush cache entire cache, first fill it with
					 *	known contents then insure those contents are
					 *	written to memory
					 */
					for (addr = NULL, end = addr + (WORDS_PER_LINE * NUM_LINES);
							addr < end; addr += WORDS_PER_LINE)
					{
						_asm(8, "
							lwz %1,0(%0) load set one
							lwzx %1,%0,%2 load set two
							",
							__reg_base(__obj_modify(addr)),		/* %0 */
							__reg_gen(),						/* %1 */
							__reg_base(__obj_modify(off2))		/* %2 */
						);
						if (IS_603E(pv))
							_asm(8, "
								lwzx %1,%0,%2 load set three
								lwzx %1,%0,%3 load set four
								",
								__reg_base(__obj_modify(addr)),		/* %0 */
								__reg_gen(),						/* %1 */
								__reg_base(__obj_modify(off3)),		/* %2 */
								__reg_base(__obj_modify(off4))		/* %3 */
							);
						_asm(8, "
							dcbst 0,%0 insure set one is written
							dcbst %0,%1 insure set two is written
							",
							__reg_base(__obj_modify(addr)),		/* %0 */
							__reg_base(__obj_modify(off2))		/* %1 */
						);
						if (IS_603E(pv))
							_asm(8, "
								dcbst %0,%1 insure set three is written
								dcbst %0,%2 insure set four is written
								",
								__reg_base(__obj_modify(addr)),		/* %0 */
								__reg_base(__obj_modify(off3)),		/* %1 */
								__reg_base(__obj_modify(off4))		/* %2 */
							);
					}
				} else {
					/* flush specific addresses from the cache */
					for (end = addr + (size / 4) - 1; addr < end;
							addr += WORDS_PER_LINE)
						_asm(" dcbf 0,%0 ", __reg_base(__obj_modify(addr)));
					_asm(" dcbf 0,%0 ", __reg_base(__obj_modify(end)));
				}
				_asm(-100, " sync ");	/* ensure writes are completed */
				_asm(-100, " isync ");	
			}
			break;

#undef	NUM_LINES
#undef	WORDS_PER_LINE

		case 604:

#define	NUM_LINES		128
#define WORDS_PER_LINE	8

			if (type == HW_CACHETYPE_INST) {
				if (size == 0) {
					_asm(-100, "
						mfspr %0,hid0 get hardware implementation register
						ori %0,%0,$0800 set the ICFI bit
						mtspr hid0,%0 set hid0 with ICFI bit set
						",
						__reg_gen()		/* %0 */
					);
				} else {
					for (end = addr + (size / 4) - 1; addr < end;
							addr += WORDS_PER_LINE)
						_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(addr)));
					_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(end)));
				}
				_asm(" sync ");
				_asm(" isync ");
			}
			if (type == HW_CACHETYPE_DATA) {
				u_int32 pv;
				if (size == 0) {
					u_int32 off2 = (WORDS_PER_LINE * 4 * NUM_LINES) * 1,
							off3 = (WORDS_PER_LINE * 4 * NUM_LINES) * 2,
							off4 = (WORDS_PER_LINE * 4 * NUM_LINES) * 3;

					/*
					 *	To flush cache entire cache, first fill it with
					 *	known contents then insure those contents are
					 *	written to memory
					 */
					addr = NULL;
					end = addr + (WORDS_PER_LINE * NUM_LINES);
					/*
					 *	Check for 604e, which has a 32k cache instead of the
					 *	16k cache on the 604.
					 */
					pv = (_get_pvr() >> 16);
					if ((pv == 9) || (pv == 10)){
						off2 *= 2;
						off3 *= 2;
						off4 *= 2;
						end += (WORDS_PER_LINE * NUM_LINES);
					}
					do {
						_asm(32, "
							lwz %1,0(%0) load set one
							lwzx %1,%0,%2 load set two
							lwzx %1,%0,%3 load set three
							lwzx %1,%0,%4 load set four
							dcbst 0,%0 insure set one is written
							dcbst %0,%2 insure set two is written
							dcbst %0,%3 insure set three is written
							dcbst %0,%4 insure set four is written
							",
							__reg_base(__obj_modify(addr)),		/* %0 */
							__reg_gen(),						/* %1 */
							__reg_base(__obj_modify(off2)),		/* %2 */
							__reg_base(__obj_modify(off3)),		/* %3 */
							__reg_base(__obj_modify(off4))		/* %4 */
						);
						addr += WORDS_PER_LINE;
					} while (addr < end);
				} else {
					/* flush specific addresses from the cache */
					for (end = addr + (size / 4) - 1; addr < end;
							addr += WORDS_PER_LINE)
						_asm(" dcbf 0,%0 ", __reg_base(__obj_modify(addr)));
					_asm(" dcbf 0,%0 ", __reg_base(__obj_modify(end)));
				}
				_asm(" sync ");		/* ensure writes are completed */
				_asm(" isync ");
			}
			break;

#undef	NUM_LINES
#undef	WORDS_PER_LINE

		case 750:

#define	NUM_LINES		128
#define WORDS_PER_LINE	8

			if (type == HW_CACHETYPE_INST) {
				if (size == 0) {
					_asm(-100, "
						mfspr %0,hid0 get hardware implementation register
						ori %0,%0,$0800 set the ICFI bit
						mtspr hid0,%0 set hid0 with ICFI bit set
						",
						__reg_gen()		/* %0 */
					);
				} else {
					for (end = addr + (size / 4) - 1; addr < end;
							addr += WORDS_PER_LINE)
						_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(addr)));
					_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(end)));
				}
				_asm(" sync ");
				_asm(" isync ");
			}
			if (type == HW_CACHETYPE_DATA) {
				if (size == 0) {
					u_int32 off2 = (WORDS_PER_LINE * 4 * NUM_LINES) * 1,
							off3 = (WORDS_PER_LINE * 4 * NUM_LINES) * 2,
							off4 = (WORDS_PER_LINE * 4 * NUM_LINES) * 3,
							off5 = (WORDS_PER_LINE * 4 * NUM_LINES) * 4,
							off6 = (WORDS_PER_LINE * 4 * NUM_LINES) * 5,
							off7 = (WORDS_PER_LINE * 4 * NUM_LINES) * 6,
							off8 = (WORDS_PER_LINE * 4 * NUM_LINES) * 7;

					/*
					 *	To flush cache entire cache, first fill it with
					 *	known contents then insure those contents are
					 *	written to memory
					 */
					addr = NULL;
					end = addr + (WORDS_PER_LINE * NUM_LINES);

					do {
						_asm(32, "
							lwz %1,0(%0) load set one
							lwzx %1,%0,%2 load set two
							lwzx %1,%0,%3 load set three
							lwzx %1,%0,%4 load set four
							lwzx %1,%0,%5 load set five
							lwzx %1,%0,%6 load set six
							lwzx %1,%0,%7 load set seven
							lwzx %1,%0,%8 load set eight
							dcbst 0,%0 insure set one is written
							dcbst %0,%2 insure set two is written
							dcbst %0,%3 insure set three is written
							dcbst %0,%4 insure set four is written
							dcbst %0,%5 insure set five is written
							dcbst %0,%6 insure set six is written
							dcbst %0,%7 insure set seven is written
							dcbst %0,%8 insure set eight is written
							",
							__reg_base(__obj_modify(addr)),		/* %0 */
							__reg_gen(),						/* %1 */
							__reg_base(__obj_modify(off2)),		/* %2 */
							__reg_base(__obj_modify(off3)),		/* %3 */
							__reg_base(__obj_modify(off4)),		/* %4 */
							__reg_base(__obj_modify(off5)),		/* %5 */
							__reg_base(__obj_modify(off6)),		/* %6 */
							__reg_base(__obj_modify(off7)),		/* %7 */
							__reg_base(__obj_modify(off8))		/* %8 */
						);
						addr += WORDS_PER_LINE;
					} while (addr < end);
				} else {
					/* flush specific addresses from the cache */
					for (end = addr + (size / 4) - 1; addr < end;
							addr += WORDS_PER_LINE)
						_asm(" dcbf 0,%0 ", __reg_base(__obj_modify(addr)));
					_asm(" dcbf 0,%0 ", __reg_base(__obj_modify(end)));
				}
				_asm(" sync ");		/* ensure writes are completed */
				_asm(" isync ");
			}
			break;

#undef	NUM_LINES
#undef	WORDS_PER_LINE

		case 821:
        {
        u_int32 NUM_LINEs = 128;            /* Set to 821/860 Default (4kD) */
#define WORDS_PER_LINE 16
        
        if (_get_immr() & IMMR_801_823_850) /* If 801/823 change num lines (1kD)*/
        	NUM_LINEs = 32;              
        if(_get_immr() & IMMR_860P)		    /* If it looks like an 860p (8kD) up lines */
			NUM_LINEs = 256;			    /* only smaller values will make thigs not work */
						 
			if (type == HW_CACHETYPE_INST) {
				if (size == 0) {
					_asm(-100, "
						addis %0,r0,hi(%1)	get invalidate command
						mtspr ic_cst,%0 issue command
						",
						__reg_gen(),	/* %0 */
						__obj_constant(IC_CST_INVALIDATE)		/* %1 */
					);
				} else {
					for (end = (u_int32 *) ((u_int32) addr + size  - 1);
							addr < end; addr += (WORDS_PER_LINE/sizeof(addr)))
					{
						_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(addr)));
					}
					_asm(" icbi 0,%0 ", __reg_gen(__obj_modify(end)));
				}
				_asm(-100, " isync ");
			}
			if (type == HW_CACHETYPE_DATA) {
				if (size == 0) {
					u_int32 off2 = (WORDS_PER_LINE * NUM_LINEs) * 1;

					/*
					 *	To flush cache entire cache, first fill it with
					 *	known contents then insure those contents are
					 *	written to memory
					 */
					for (addr = NULL, end = (u_int32 *) ((u_int32)addr + off2);
							addr < end; addr += (WORDS_PER_LINE/sizeof(addr)))
					{
						_asm(16, "
							lwz %1,0(%0) load set one
							lwzx %1,%0,%2 load set two
							sync
							dcbst 0,%0 insure set one is written
							sync
							dcbst %0,%2 insure set two is written
							",
							__reg_base(__obj_modify(addr)),		/* %0 */
							__reg_gen(),						/* %1 */
							__reg_base(__obj_modify(off2))		/* %2 */
						);
					}
				} else {
					/* flush specific addresses from the cache */
					for (end = (u_int32 *) ((u_int32) addr + size - 1);
							addr < end; addr += (WORDS_PER_LINE/sizeof(addr)))
					{
						_asm("	sync
								dcbf 0,%0
							", __reg_base(__obj_modify(addr)));
					}
					_asm("	sync
							dcbf 0,%0
						", __reg_base(__obj_modify(end)));
				}
				_asm(-100, " sync ");	/* ensure writes are completed */
				_asm(-100, " isync ");	/* ensure writes are completed */
			}
			break;

        }
		default:
			_asm(" isync ");
	}
#endif
#if	defined(_MPFSH3)
	u_int32         *cp = (u_int32 *)CCTL_REG;
	u_int32		*ca, cd, way, entry;
	u_int32		no_ways = WAY, no_lines = LINES_7709;
	u_int32		shifter = SHIFTER_7709;
	u_int32		entry_mask = ENTRY_MASK;
	u_int32         cp_value;

	switch(rinf->hardware->cpush.cc_cputype)
	{
		case SH7709A:
			no_lines = LINES_7709A;
			shifter = SHIFTER_7709A;
			entry_mask = ENTRY_MASK_7709A;
		case SH7708:
		case SH7708S:
		case SH7709:
			no_ways = WAY;
			if (size == 0)
			{
				if (!((cp_value = ccr_read(cp)) & CCR_WT))	/* write back mode ? */
				{
					for (way = 0; way < no_ways; way++)
					{
						for (entry = 0; entry < no_lines; entry++)
						{
							ca = (u_int32 *)
									(CCTL_ADR|(way << shifter)|(entry << 4));
							cd = ccr_read(ca);
							cd &= 0x1FFFFFFF;
							if (cd & CCTL_V)
							{
								ccr_write(ca, (cd & ~(CCTL_V | CCTL_U)));
							}
						}
					}
				}
				cp_value = ccr_read(cp);
				cp_value &= 0x2F;
				cp_value |= CCR_CF;  /* flush all entry */
				ccr_write(cp, cp_value);
				break;
			}
			size += (ENTRY_SIZE-1);
			size &= ~(ENTRY_SIZE-1);
			size >>= 4;
			while (size-- > 0)
			{
				ca = (u_int32 *)(CCTL_ADR | ((u_int32)addr & entry_mask) |
								CCTL_A);
				cd = ((u_int32)addr & BLK_MASK) | (ccr_read(ca) & 0x3f0);
				ccr_write(ca, cd);
				addr = (u_int32 *)((u_int32 )addr + ENTRY_SIZE);
			}
			break;
		default:
			break;
	}

#endif	/* _MPFSH3 */
#if	defined(_MPFSH4)
	u_int32				*cp = (u_int32 *)CCTL_REG;
	u_int32				*ca;
	u_int32				cd;
	u_int32				entry;
	u_int32				beg_addr;
	u_int32				cp_value;

	switch(rinf->hardware->cpush.cc_cputype)
	{
		case SH7750:
			/* flush instruction caches */
			if (type == HW_CACHETYPE_INST)
			{
				if (size == 0)
				{
					cp_value = ccr_read(cp);
					cp_value &= 0x89AF;		/* correct bits must be masked */
					cp_value |= CCR_ICI;	/* flush/invalidate all entries */
					ccr_write(cp, cp_value);
					break;
				}

				/* flush a range of addresses */
				/* find out how many cache lines in buffer to be flushed */
				size = (size + CACHE_BLKSZ - 1) >> 5;

				/* find beginning and ending address of buffer to be flushed */
				beg_addr = (u_int32) addr;
				beg_addr &= 0x1FFFFFFF;	/* mask off top three bits */

				/* flush by walking through the buffer via cache line size */
				while (size-- > 0)
				{
					/* determine entry in cache map */
					ca = (u_int32 *) (IC_ADDRESS|(beg_addr&IENTRY_MASK)|
									C_ASSOCIATE);
					/* derive tag address for cache entry */
					cd = (u_int32) beg_addr & BLK_MASK;
					/* write entry, ensuring it is invalidated */
					ccr_write(ca, cd);
					beg_addr += (u_int32) CACHE_BLKSZ;
				}

			}

			/* flush data cache */
			if (type == HW_CACHETYPE_DATA)
			{
				if (size == 0)
				{
					for (entry = 0; entry < DLINES; entry++)
					{
						ca = (u_int32 *) (OC_ADDRESS | (entry << 5));
						cd = ccr_read(ca);
						cd &= 0xFFFFFC03;	/* correct bits must be masked */
						if (cd & VALID)
						{
							ccr_write(ca, (cd & ~(VALID|DIRTY)));
						}
					}
					cp_value = ccr_read(cp);
					cp_value &= 0x89AF;		/* correct bits must be masked */
					cp_value |= CCR_OCI;	/* invalidate all entries */
					ccr_write(cp, cp_value);
					break;
				}

				/* flush a range of addresses */
				/* find out how many cache lines in buffer to be flushed */
				size = (size + CACHE_BLKSZ - 1) >> 5;
				beg_addr = (u_int32) addr;
				beg_addr &= 0x1FFFFFFF;
				while(size-- > 0)
				{
					_asm(" ocbp @%0         write cache block and invalidate",
					    __reg_gen(__obj_copy(beg_addr)));

					beg_addr += (u_int32) CACHE_BLKSZ;
				}
			}

			break;
		default:
			break;
	}
#endif /* _MPFSH4 */
}

#if defined(_MPFSH3) || defined(_MPFSH4)
/*
    This routine must jump into P2 space and read the CCR register.
    The r4 register will have the CCR address.
    The CCR register value is returned back in r0.
*/
_asm("
ccr_read:
    sts     pr,r2               store pr so we can return from here
    mova    @(ccr_reader,pc),r0 get address of routine
    mov.l   @(p2_bit,pc),r1     get P2 address space bit
    or      r1,r0               make the address
    jsr     @r0
    nop
    lds     r2,pr               restore p4 so we can return
    rts                         return
    nop

ccr_reader:
    nop
    mov.l   @r4,r0              Read ccr register
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    rts                          return
    nop

ccr_write:
    sts     pr,r2               store pr so we can return from here
    mova    @(ccr_writer,pc),r0 get address of routine
    mov.l   @(p2_bit,pc),r1     get P2 address space bit
    or      r1,r0               make the address
    jsr     @r0
    nop
    lds     r2,pr               restore p4 so we can return
    rts                         return
    nop

ccr_writer:
    nop
    mov.l   r5,@r4              Write ccr register
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    rts
    nop

 align 4
p2_bit  dc.l    0x20000000
");
#endif

#if defined(_MPFMIPS32)
/*
	The following are the routines that will flush the
	caches on the MIPS32 processor.
*/

_asm("
*
* CACHE instruction constant macros
*
Index_Invalidate_I		set		0
Index_WriteBack_Inv_D	set		1
Index_WriteBack_Inv_SD	set		3
Hit_Invalidate_I		set		0x10
Hit_WriteBack_Inv_D		set		0x15
Hit_WriteBack_Inv_SD	set		0x17

* cacheop(virtualaddr, sizeinbytes, cachelinesize, cacheop)
*   addr    equ t0
*   maxaddr equ t1
*   mask    equ t2
cacheop macro
* check for bad size
	blez	\\2,l\\@b
	addu	t1,\\1,\\2
* align to line boundaries
	addiu	t2,\\3,-1
	nor		t2,t2,zero
	and		t0,\\1,t2
	addiu	t1,t1,-1
	and		t1,t1,t2
* the cacheop loop
l\\@a
	cache	\\4,0(t0)
	nop
	cache	\\4,4096(t0)
	nop
	bne		t0,t1,l\\@a
	addu	t0,t0,\\3
l\\@b
		endm

 align 4

******************************************************************
* a0 has the icache_size
* a1 has the icache_linesize
iflush_mipidt:

*
* flush entire i-cache
	beq		a0,zero,icache_fl_end			check if cache size is zero
	nop										if so, exit, nothing to do.
	move	t3,a0
	move	t4,a1
*   t3 = icache_size
*   t4 = icache_linesize
*
	lui		t5,hi(SEG_KSEG0)					get KSEG0 segment
*
* Invalidate entire instruction cache:
*   t5 = start of KSEG0 segment
*   t3 = size of instruction cache
*   t4 = instruction cache line size in bytes
*
	cacheop	t5,t3,t4,Index_Invalidate_I
	sync
	sync
	sync
	sync
icache_fl_end
	jr		ra										return
	or		v0,zero,zero							return SUCCESS (delay)



******************************************************************
* a0 has the dcache_size
* a1 has the dcache_linesize
dflush_mipidt:
*
* flush entire d-cache
	beq		a0,zero,dcache_fl_end			check if cache size is zero
	nop										if so, exit, nothing to do.
	move	t3,a0
	move	t4,a1
*   t3 = dcache_size
*   t4 = dcache_linesize
*
	lui		t5,hi(SEG_KSEG0)                        get KSEG0 segment
*
* Invalidate entire data cache:
*   t5 = start of KSEG0 segment
*   t3 = size of data cache
*   t4 = data cache line size in bytes
*
	cacheop	t5,t3,t4,Index_WriteBack_Inv_D
	sync
	sync
	sync
dcache_fl_end
	jr		ra										return
	or		v0,zero,zero							return SUCCESS (delay)
");

_asm("
* a0 is pointer isize, and a1 is pointer to ilinesize
query_idt_i_cache:
	mfc0    t0,$C0_CONFIG		* get config register, it tells all

* Pull what we need
* Get i-cache size
	andi	t1,t0,CFG_ICMASK
	srl		t1,t1,CFG_ICSHIFT
	ori		t2,zero,0x200
	sllv	t2,t2,t1			* t2 has i-cache size

* Now get line size, its either 16 or 32 bytes
	ori     t5,zero,32
	andi    t1,t0,CFG_IB
	bne     t1,zero,sc1
	nop
	ori     t5,zero,16

sc1
	nop
* Report back
	sw		t2,0(a0)			* write back isize
	nop
	sw		t5,0(a1)			* write back ilinesize
	nop
	jr		ra					* return
	nop

* a0 is pointer dsize, and a1 is pointer to dlinesize
query_idt_d_cache:
	mfc0    t0,$C0_CONFIG		* get config register, it tells all

* Get d-cache size
	andi	t1,t0,CFG_DCMASK
	srl		t1,t1,CFG_DCSHIFT
	ori		t3,zero,0x200
	sllv	t3,t3,t1			* t3 has d-cache size

* Now get line size, its either 16 or 32 bytes
	ori		t6,zero,32
	andi	t1,t0,CFG_DB
	bne		t1,zero,sc2
	nop
	ori		t6,zero,16			* t6 has dlinesize

sc2
	nop
* Report back
	sw		t3,0(a0)			* write back isize
	nop
	sw		t6,0(a1)			* write back ilinesize
	nop
	jr		ra					* return
	nop


");

_asm("
* a0 is pointer to ilines, and a1 is pointer to ilinesize variable */
query_i_cache:
 dc.l	0x40088001
*	mfc0	t0,$C0_CONFIG,1
* t0 will have the contents of config1 register
* work out primary i-cache line size (4 bytes to 128 bytes)
	lui		t1,hi(CFG1_IL_BITS)		; get mask bits for i-line size 3 bit field
	ori		t1,t1,lo(CFG1_IL_BITS)
	and		t1,t1,t0				; mask out the bits
	srl		t1,t1,CFG1_IL_SHIFT		; shift to get to number 0 - 6
	ori		t5,zero,0x2				; size is multiple power of 2
	sllv	t5,t5,t1				; shift left to multiply power of 2
	sw		t5,0(a1)				; give line size back to callee function

* work out primary i-cache lines
* First, get number of sets per way (4 to 4096)
	lui		t1,hi(CFG1_IS_BITS)		; get mask bits for i-line size 3 bit field
	ori		t1,t1,lo(CFG1_IS_BITS)
	and		t1,t1,t0				; mask out the bits
	srl		t1,t1,CFG1_IS_SHIFT		; shift to get number 0 - 6
	ori		t8,zero,0x40			; sets per way is multiple power of 64
	sllv	t8,t8,t1				; shift left to multiply power of 64
* Second, get number of ways (1 to 8)
	lui		t1,hi(CFG1_IA_BITS)		; get mask bits for i-line size 3 bit field
	ori		t1,t1,lo(CFG1_IA_BITS)
	and		t1,t1,t0				; mask out the bits
	srl		t1,t1,CFG1_IA_SHIFT		; shift to get to number 0 - 7
	addiu	t9,t1,1					; Add 1 to get to number of ways
* Third, multiply t8 by t9 to find the total number of cache lines
	multu	t8,t9					; multiply sets per way by # of ways
	mflo	t1						; get number of lines
	sw		t1,0(a0)				; give number of lines back to callee func.
	jr		ra						; return
	nop


* a0 is pointer to dlines, and a1 is pointer to dlinesize variable */
query_d_cache:
 dc.l	0x40088001
*	mfc0	t0,$C0_CONFIG,1
* t0 will have the contents of config1 register
* work out primary d-cache line size (4 bytes to 128 bytes)
	lui		t1,hi(CFG1_DL_BITS)		; get mask bits for i-line size 3 bit field
	ori		t1,t1,lo(CFG1_DL_BITS)
	and		t1,t1,t0				; mask out the bits
	srl		t1,t1,CFG1_DL_SHIFT		; shift to get to number 0 - 6
	ori		t5,zero,0x2				; size is multiple power of 2
	sllv	t5,t5,t1				; shift left to multiply power of 2
	sw		t5,0(a1)				; give line size back to callee function

* work out primary d-cache lines
* First, get number of sets per way (4 to 4096)
	lui		t1,hi(CFG1_DS_BITS)		; get mask bits for i-line size 3 bit field
	ori		t1,t1,lo(CFG1_DS_BITS)
	and		t1,t1,t0				; mask out the bits
	srl		t1,t1,CFG1_DS_SHIFT		; shift to get number 0 - 6
	ori		t8,zero,0x40			; sets per way is multiple power of 64
	sllv	t8,t8,t1				; shift left to multiply power of 64
* Second, get number of ways (1 to 8)
	lui		t1,hi(CFG1_DA_BITS)		; get mask bits for i-line size 3 bit field
	ori		t1,t1,lo(CFG1_DA_BITS)
	and		t1,t1,t0				; mask out the bits
	srl		t1,t1,CFG1_DA_SHIFT		; shift to get to number 0 - 7
	addiu	t9,t1,1					; Add 1 to get to number of ways
* Third, multiply t8 by t9 to find the total number of cache lines
	multu	t8,t9					; multiply sets per way by # of ways
	mflo	t1						; get number of lines
	sw		t1,0(a0)				; give number of lines back to callee func.
	jr		ra						; return
	nop


");
#endif

#if defined(_MPFMIPS64)
/*
	The following are the routines that will flush the
	caches on the MIPS64 processor.

	Register a0 will have size of the cache.
	Register a1 will have the linesize of the cache.
*/
_asm("
*
* CACHE instruction constant macros
*
Index_Invalidate_I		set		0
Index_WriteBack_Inv_D	set		1
Index_WriteBack_Inv_SD	set		3
Hit_Invalidate_I		set		0x10
Hit_WriteBack_Inv_D		set		0x15
Hit_WriteBack_Inv_SD	set		0x17

* cacheop(virtualaddr, sizeinbytes, cachelinesize, cacheop)
*   addr    equ t0
*   maxaddr equ t1
*   mask    equ t2
cacheop macro
* check for bad size
	blez	\\2,l\\@b
	addu	t1,\\1,\\2
* align to line boundaries
	addiu	t2,\\3,-1
	nor		t2,t2,zero
	and		t0,\\1,t2
	addiu	t1,t1,-1
	and		t1,t1,t2
* the cacheop loop
l\\@a
	cache	\\4,0(t0)
	bne		t0,t1,l\\@a
	addu	t0,t0,\\3
l\\@b
		endm

 align 4

******************************************************************
* a0 has the icache_size
* a1 has the icache_linesize
iflush_mip4:

*
* flush entire i-cache
	beq		a0,zero,icache_flush_end		check if cache size is zero
	nop										if so, exit, nothing to do.
	move	t3,a0
	move	t4,a1
*   t3 = icache_size
*   t4 = icache_linesize
*
	lui		t5,hi(SEG_KSEG0)					get KSEG0 segment
*
* Invalidate entire instruction cache:
*   t5 = start of KSEG0 segment
*   t3 = size of instruction cache
*   t4 = instruction cache line size in bytes
*
	cacheop	t5,t3,t4,Index_Invalidate_I
icache_flush_end
	jr		ra										return
	or		v0,zero,zero							return SUCCESS (delay)



******************************************************************
* a0 has the dcache_size
* a1 has the dcache_linesize
dflush_mip4:
*
* flush entire d-cache
	beq		a0,zero,dcache_flush_end		check if cache size is zero
	nop										if so, exit, nothing to do.
	move	t3,a0
	move	t4,a1
*   t3 = dcache_size
*   t4 = dcache_linesize
*
	lui		t5,hi(SEG_KSEG0)                        get KSEG0 segment
*
* Invalidate entire data cache:
*   t5 = start of KSEG0 segment
*   t3 = size of data cache
*   t4 = data cache line size in bytes
*
	cacheop	t5,t3,t4,Index_WriteBack_Inv_D
dcache_flush_end
	jr		ra										return
	or		v0,zero,zero							return SUCCESS (delay)



******************************************************************
* a0 has the scache_size
* a1 has the scache_linesize
sflush_mip4:
*
* flush entire s-cache
	beq		a0,zero,scache_flush_end		check if cache size is zero
	nop										if so, exit, nothing to do.
	move	t3,a0
	move	t4,a1
*   t3 = scache_size
*   t4 = scache_linesize
*
	lui	t5,hi(SEG_KSEG0)						get KSEG0 segment
*
* Invalidate entire data cache:
*   t5 = start of KSEG0 segment
*   t3 = size of data cache
*   t4 = data cache line size in bytes
*
	cacheop	t5,t3,t4,Index_WriteBack_Inv_SD
scache_flush_end
	jr		ra									return
	or		v0,zero,zero						return SUCCESS

");

#endif

#ifdef _MPFMIPS3000
/* idt_cache_flush - invalidate all of the i or d cache. */
void idt_cache_flush(u_int32 *addr, u_int32 size, u_int8 type)
{
	if (type == HW_CACHETYPE_INST)
		type = 0;
	else
		type = 1;

	_asm("
		mfc0	%13,sr				save sr
		mtc0	zero,sr				disable interrupts
		lui		%12,hi(%3)			get uncached memory location
		lw		%0,0(%12)			flush the write-buffer
		bne		%1,zero,%14
		nop
		lui		%1,hi(SR_SWC)		swap caches to do icache
		mtc0	%1,sr
		nop
%14
 		mfc0	%0,sr				save current sr
		lui		%2,hi(~SR_PE)
		ori		%2,%2,lo(~SR_PE)
 		and		%0,%0,%2			do not inadvertently clear PE
		lui		%2,hi(SR_ISC)
		or		%2,%0,%2			isolate cache
 		mtc0	%2,sr

		xor		%1,%1,%1			check for cache
		lui		%2,0xa5a5			use distinctive pattern
		ori		%2,%2,0xa5a5
		lui		%12,hi(%3)			get memory location
		sw		%2,0(%12)			try to write to cache
		nop
		lw		%4,0(%12)			try to read from cache
		mfc0	%5,sr
		lui		%11,hi(SR_CM)
		and		%5,%5,%11
		bne		%5,zero,%8			cache miss, must be no cache
		nop
		bne		%2,%4,%8			data not equal, no cache
		nop

* we now know we have a cache. lets try to size it and invalidate as we go
		lui		%5,hi(%10)			max cache size
		or		%4,zero,%12
%6
		addiu	%4,%4,4				this is the initial pointer
		sw		zero,0(%4)			store zero
		nop	
		sw		%2,0(%12)			store the marker
		nop
		lw		%11,0(%4)			does our pointer have the marker ?
		nop
		beq		%2,%11,%7
		nop
		sb		zero,0(%4)			invalidate the entry under the pointer
		bne		%5,%4,%6			branch back and do it again
		nop
%7
		sb		zero,0(%12)			invalidate our first cache entry
%8
		mtc0	%13,sr				restore sr
		nop
	",
	__reg_gen(),									/* %0 */
	__reg_gen(type),								/* %1 */	
	__reg_gen(),									/* %2 */	
	__obj_constant(0x80000000),						/* %3 */
	__reg_gen(),									/* %4 */	
	__reg_gen(),									/* %5 */		
	__label(),										/* %6 */
	__label(),										/* %7 */	
	__label(),										/* %8 */	
	__obj_constant(0x200),							/* %9 */
	__obj_constant(0x40000),						/* %10 */
	__reg_gen(),									/* %11 */
	__reg_gen(),									/* %12 */
	__reg_gen(),									/* %13 */
	__label()										/* %14 */
	);
}

/* tx_cache_flush - invalidate all of the i or d cache. */
void tx_cache_flush(u_int32 *addr, u_int32 size, u_int8 type , Rominfo rinf)
{
	if (type == HW_CACHETYPE_INST)
	{
		/*
		 * Flush the instruction cache
		 */
#if defined (DEBUG)
		if(rinf->cons != 0)
		{
			rinf->cons->rom_puts("LL Inst. cache flush. Addr ",rinf);
			out4hex( (u_int32)addr,rinf);
            rinf->cons->rom_puts(", Size ",rinf);
            out4hex(size,rinf);
            rinf->cons->rom_puts(", type  ",rinf);
            out4hex(type,rinf);
            rinf->cons->rom_puts("\n",rinf);
		}
#endif
		tx_icache_flush(addr, size);
	}
	else
	{
		/*
		 * Flush the data cache
		 */
#if defined (DEBUG)
		if(rinf->cons != 0)
		{
			rinf->cons->rom_puts("LL Data cache flush. Addr",rinf);
			out4hex( (u_int32)addr,rinf);
            rinf->cons->rom_puts(", Size ",rinf);
            out4hex(size,rinf);
            rinf->cons->rom_puts(", type  ",rinf);
            out4hex(type,rinf);
            rinf->cons->rom_puts("\n",rinf);
		}
#endif
		tx_dcache_flush(addr, size);
	}

}

/* ----------------------------------------------------------------------
 * void tx_icache_flush (u_char *addr, u_int32 size)
 *
 * The will invalidate size bytes of the instruction cache starting at
 * address addr.
 *
 * WARNING
 * The current implementation invalidates the *entire* instruction cache
 * ----------------------------------------------------------------------
 */
_asm("
tx_icache_flush:
	mfc0	t7,$C0_CONFIG				get config
	nop									(wait for valid data)
	or		t1,zero,t7					copy this value to t1
	lui		t2,hi(~CFG_ICE)             get mask with inst. cache disabled
	ori		t2,t2,lo(~CFG_ICE)
	and		t1,t7,t2					turn off instruction cache bit
	mtc0	t1,$C0_CONFIG				disable instruction cache
	nop									(wait for this to take effect)
*
* Now disable instruction streaming
*
	b		i_lc						branch to stop instruction streaming
	nop
i_lc
*
* Set up registers prior to invalidating the instruction cache
*
    mfc0    t0,$C0_CONFIG                read the config register
    nop
    srl     t0,t0,19                    move the ICS bits to the bottom
    andi    t0,t0,0x0007                mask off just the ICS bits
	ori		t3,zero,0x0400
    sllv    t3,t3,t0                    t3 now has the cache size
	ori		t2,zero,16					t2 = cache line size (fixed at 16 bytes)
	move	t0,zero						t0 = first cache line to reference
i_l0
	cache	IndexInvalidate,0(t0)		invalidate the cache line
	addu	t0,t0,t2					increment for next cache line
	bne		t0,t3,i_l0					loop back for all cache lines
	nop
*
* Always restore the cache to its state prior to calling this function
*
	mtc0	t7,$C0_CONFIG				turn the instruction cache on
	nop									(make sure it gets written out)
*
	jr		ra							return to the caller now
	nop
"
);


/* ----------------------------------------------------------------------
 * void tx_dcache_flush (u_char *addr, u_int32 size)
 *
 * The will invalidate size bytes of the instruction cache starting at
 * address addr.
 *
 * ----------------------------------------------------------------------
 */
_asm("
tx_dcache_flush:
	sync				Synchronise until the previos instruction is done
	nop					(delay slot)
tx_d_l
	bc0f	tx_d_l		keep looping until the write buffer is empty
	nop					(delay slot)
*
* Set up registers prior to invalidating the data cache
*
    mfc0    t0,$C0_CONFIG               read the config register
    nop
    andi    t1,t0,CFG_WBON                  t1 is non-zero for copy back
    srl     t0,t0,16                    move the DCS bits to the bottom
    andi    t0,t0,0x0007                mask off just the DCS bits
	ori		t3,zero,0x0400
    sllv    t3,t3,t0                    t3 now has the cache size
	ori		t2,zero,4					t2 = data size
    lui     t0,hi(0x80000000)			t0 = first data reference
    add     t4,t3,t0                    t4 now has the ending address
* load the cache with known data so we can flush
d_l3
	lw      t5,0(t0)                    load data
	addu	t0,t0,t2					increment for next data access
	bne		t0,t4,d_l3					loop back for all data
	nop
    lui     t0,hi(0x80000000)			t0 = first data reference
    bne     t1,zero,d_l2
    nop
d_l0
	cache	HitInvalidate,0(t0)         invalidate the cache entry
	addu	t0,t0,t2					increment for next data access
	bne		t0,t4,d_l0					loop back for all data
	nop
    b       d_l1
    nop
d_l2
	cache	HitWriteBackInvalidate,0(t0) invalidate the cache entry
	addu	t0,t0,t2					increment for next data access
	bne		t0,t4,d_l2					loop back for all data
	nop
d_l1
*
* Now return
*
	jr		ra
	nop
"
);

#endif

#ifdef	_MPFPOWERPC
#define BYTES_PER_LINE 16
void size_401(u_int32 *linesi, u_int32 *linesd)
{
  u_int32 pv = (_get_pvr() >> 16) & 0xffff;

  switch(pv) {
  case 0x0021:  /* 401A1 */
    *linesi = 2048/BYTES_PER_LINE;
    *linesd = 1024/BYTES_PER_LINE;
    break;
  case 0x0022:  /* 401B2 */
    *linesi = 16384/BYTES_PER_LINE;
    *linesd = 8192/BYTES_PER_LINE;
    break;
  case 0x0023:  /* 401C2 */
    *linesi = 0;
    *linesd = 8192/BYTES_PER_LINE;
    break;
  case 0x0024:  /* 401D2 */
    *linesi = 4096/BYTES_PER_LINE;
    *linesd = 2048/BYTES_PER_LINE;
    break;
  case 0x0025:  /* 401E2 */
    *linesi = 0;
    *linesd = 0;
    break;
  case 0x0026:  /* 401F2 */
    *linesi = 2048/BYTES_PER_LINE;
    *linesd = 2048/BYTES_PER_LINE;
    break;
  case 0x0027:  /* 401G2 */
    *linesi = 8192/BYTES_PER_LINE;
    *linesd = 2048/BYTES_PER_LINE;
    break;
  default:  /* 401B2 */
    *linesi = 16384/BYTES_PER_LINE;
    *linesd = 8192/BYTES_PER_LINE;
    break;
  }
}
#undef BYTES_PER_LINE
#endif

#ifdef _MPFARM
const u_int32 cache_size[] = {512,1024,2048,4096,8192,16384,32768,65536};
const u_int32 line_size[] = {8,16,32,64};
const u_int32 assoc[] = {1,2,4,8,16,32,64,128};
#define CPR0A_DSIZE   0x001c0000 /* data cache size */
#define CPR0A_DASSOC  0x00038000 /* data cache associativity */
#define CPR0A_DLINE   0x00003000 /* data cache line length */

void xscale_cache_flush(u_int32 *addr, u_int32 size, u_int8 type, Rominfo rinf)
{
#define D_CACHE_ON_MASK 0x4
#define MINI_SIZE (2*1024)           /* size of mini cache */

  u_int32 *flush_areas = 0;
  u_int32 flush_memory = 0xe0000000;
  u_int32 flush_mini =   0xe0100000;
  u_int32 CntlReg = 0;
  u_int32 CacheType = 0;
  u_int32 dcache_size;
  u_int32 dline_size;
  u_int32  *end_addr;
  u_int32 *flshblk;

  /* get the areas as setup in sysinit2 */
  if (rinf->os)
    flush_areas = (u_int32 *)rinf->os->oem_globals;
  if (flush_areas) {
    flush_memory = *flush_areas++;
    flush_mini = *flush_areas;
  }

  /* get the control register */
  _asm(" mrc p15,0,%0,c1,c0,0",__reg_gen(__obj_modify(CntlReg)));

  /* get the cache type register */
  _asm(" mrc p15,0,%0,c0,c0,1",__reg_gen(__obj_modify(CacheType)));

  dcache_size = cache_size[(CacheType&CPR0A_DSIZE)>>18];
  dline_size = line_size[(CacheType&CPR0A_DLINE)>>12];

  if (type == HW_CACHETYPE_INST) {
    /*	Flush instruction cache	*/
	_asm(-100,"
    mcr p15,0,%0,c7,c5,0    * invalidate all of I cache & BTB
    mrc p15,0,%0,c2,c0,0    * dummy read from the ttb register
    nop
    sub pc,pc,4
    mcr p15,0,%0,c7,c10,4   * drain write and fill buffer
    mrc p15,0,%0,c2,c0,0    * dummy read from the ttb register
    nop
    sub pc,pc,4
	", __reg_gen());
  }

  /* now that we are finished with Icache, if the Dcache is not even on,
   * its not worth going through the flush motion.
   */
  if(!(CntlReg & D_CACHE_ON_MASK))
    return;

  if (type == HW_CACHETYPE_DATA)
    if (size == 0)
 {
      flshblk = (u_int32 *)flush_memory;
      /* The FLUSH_MEMORY is an alias area for flushing */
	_asm(-100,"
	add %1,%0,%2	* find end of read region
%4
	mcr  p15,0,%0,c7,c2,5 * allocate cache line
    add %0,%0,%3
	cmp %0,%1			* At the end??
	ble %4
	",__reg_gen(__obj_modify(flshblk)),
	__reg_gen(),
	__reg_gen(__obj_modify(dcache_size)),
	__reg_gen(__obj_modify(dline_size)),
	__label());

    if (flush_areas) {
      /*	Now need to hit mini cache area	*/
      flshblk = (u_int32 *) (flush_mini);
	_asm(-100,"
    add %1,%0,%2	* find end of read region
%4
	ldr %5,[%0]		* load data
    add %0,%0,%3
	cmp %0,%1
	ble %4
	",__reg_gen(__obj_modify(flshblk)),
    __reg_gen(),
	__obj_constant(MINI_SIZE),
	__reg_gen(__obj_modify(dline_size)),
	__label(),
	__reg_gen());
    }
    /* now invalidate */
	_asm(-100,"
	mcr p15,0,%0,c7,c6,0	* invalidate cache
    mrc p15,0,%0,c2,c0,0    * dummy read from the ttb register
    nop
    sub pc,pc,4
	mcr p15,0,%0,c7,c10,4	* drain write buffer
    mrc p15,0,%0,c2,c0,0    * dummy read from the ttb register
    nop
    sub pc,pc,4
	",__reg_gen());
    }

  if (type == HW_CACHETYPE_DATA)
    if (size != 0) {
      /* flush specific addresses from the cache	*/
      for (end_addr = (u_int32 *) ((u_int32)addr + size - 1);
               addr < end_addr;addr += (dline_size/sizeof(addr))) {
	_asm(-100,"
	mcr p15,0,%0,c7,c10,1   * clean entry
    mrc p15,0,%1,c2,c0,0    * dummy read from the ttb register
    nop
    sub pc,pc,4
	mcr p15,0,%0,c7,c6,1	* invalidate entry
    mrc p15,0,%1,c2,c0,0    * dummy read from the ttb register
    nop
    sub pc,pc,4
	",__reg_gen(__obj_modify(addr)),
    __reg_gen());
      }
	_asm(-100,"
	mcr p15,0,%0,c7,c10,1   * clean entry
    mrc p15,0,%1,c2,c0,0    * dummy read from the ttb register
    nop
    sub pc,pc,4
	mcr p15,0,%0,c7,c6,1    * invalidate entry
    mrc p15,0,%1,c2,c0,0    * dummy read from the ttb register
    nop
    sub pc,pc,4
	mcr p15,0,%1,c7,c10,4	* drain write buff
    mrc p15,0,%1,c2,c0,0    * dummy read from the ttb register
    nop
    sub pc,pc,4
	",__reg_gen(__obj_modify(end_addr)),
    __reg_gen());

    if (flush_areas) {
      /*	Now need to hit mini cache area	*/
      flshblk = (u_int32 *) (flush_mini);
	_asm(-100,"
    add %1,%0,%2	* find end of read region
%4
	ldr %5,[%0]		* load data
    add %0,%0,%3
	cmp %0,%1
	ble %4
	",__reg_gen(__obj_modify(flshblk)),
    __reg_gen(),
	__obj_constant(MINI_SIZE),
	__reg_gen(__obj_modify(dline_size)),
	__label(),
	__reg_gen());
    }
    }

    if (type == HW_CACHETYPE_DATA_INV)
      if (size == 0) {
        /*	invalidate entire data cache	*/
	_asm(-100,"
    mcr p15,0,%0,c7,c6,0    * invalidate all of D cache
    mrc p15,0,%0,c2,c0,0    * dummy read from the ttb register
    nop
    sub pc,pc,4
	", __reg_gen());
      }
    
    if (type == HW_CACHETYPE_DATA_INV)
      if (size != 0) {
        /* Invalidate specific addresses from the cache	*/
        for (end_addr = (u_int32 *) ((u_int32)addr + size - 1);
                  addr < end_addr;addr += (dline_size/sizeof(addr))) {
	_asm(-100,"
	mcr p15,0,%0,c7,c6,1	* Invalidate entry.
    mrc p15,0,%1,c2,c0,0    * dummy read from the ttb register
    nop
    sub pc,pc,4
	",__reg_gen(__obj_modify(addr)),
    __reg_gen());
        }
	_asm(-100,"
    mcr p15,0,%0,c7,c6,1    * Invalidate entry
    mrc p15,0,%1,c2,c0,0    * dummy read from the ttb register
    nop
    sub pc,pc,4
	mcr p15,0,%1,c7,c10,4	* drain write buff
    mrc p15,0,%1,c2,c0,0    * dummy read from the ttb register
    nop
    sub pc,pc,4
	",__reg_gen(__obj_modify(end_addr)),
    __reg_gen());
      }

#undef MINI_SIZE
#undef D_CACHE_ON_MASK
#undef CPR0A_DSIZE
#undef CPR0A_DASSOC
#undef CPR0A_DLINE
}
#endif

/* cache flush module initialization */

error_code p2start(Rominfo rinf, u_char *glbls)
{

#ifdef	_MPFPOWERPC
	/*
	 *	If the data cache is not already enabled, insure
	 *	that it is invalidated before any flushing
	 */
	switch (rinf->hardware->cpuppc.cc_cputype) {
	case 602:
	case 603:
	case 8240:
	case 8260:
		_asm("
			mfspr %0,hid0 get hardware implementation register
			andi. %1,%0,%2 is data cache on?
			bne %4 don't invalidate if cache is on
			ori %1,%0,%3 set invalidate bit
			mtspr hid0,%1 invalidate data cache
			mtspr hid0,%0 clear invalidate bit
			isync wait for invalidate to complete
%4
			",
			__reg_base(),	/*	%0  */
			__reg_base(),	/*	%1	*/
			__obj_constant(HID0_DCE),
			__obj_constant(HID0_DCI),
			__label()
		);
		break;
	case 604:
	case 750:
		_asm("
			mfspr %0,hid0 get hardware implementation register
			andi. %1,%0,%2 is data cache on?
			bne %4 don't invalidate if cache is on
			ori %1,%0,%3 set invalidate bit
			mtspr hid0,%1 invalidate data cache
			isync wait for invalidate to complete
			mtspr hid0,%0 turn data cache off again
%4
			",
			__reg_base(),	/*	%0  */
			__reg_base(),	/*	%1	*/
			__obj_constant(HID0_DCE),
			__obj_constant(HID0_DCE|HID0_DCI),
			__label()
		);
		break;
	case 821:
		_asm(-100, "
			mfspr %0,dc_cst get data cache status
			andis. %0,%0,hi(%1) is data cache on?
			bne %4 don't invalidate if cache is on
			addis %0,r0,hi(%2) unlock cache
			sync
			mtspr dc_cst,%0 issue command
			isync
			addis %0,r0,hi(%3) invalidate cache
			sync
			mtspr dc_cst,%0 issue command
			isync
%4
			",
			__reg_gen(),	/* %0 */
			__obj_constant(DC_CST_DEN),
			__obj_constant(DC_CST_UNLOCK_ALL),
			__obj_constant(DC_CST_INVALIDATE),
			__label()
		);
		break;
	}
#endif

	rinf->hardware->flush_cache = &cache_flush;
    cache_flush(0, 0, HW_CACHETYPE_INST, rinf);
	return (SUCCESS);
}

#if defined(DEBUG)
/* printstr - print a NULL terminated string using Modular Rom services */
void printstr(Rominfo rinf, char *str)
{
  if (rinf->cons)
    if (rinf->cons->rom_puts)
      rinf->cons->rom_puts(str, rinf);
}

/* itoaout - print an unsigned number in hex format using Modular Rom services*/
void itoaout(Rominfo rinf, u_int32 number)
{
	ohexb(rinf, number >> 24);
	ohexb(rinf, number >> 16);
	ohexb(rinf, number >> 8);
	ohexb(rinf, number);	      
}

/* ohexb - conversion routine */
static void ohexb(Rominfo rinf, u_int32 y)
{
	int d;
    y &= 0x000000ff;

  if (!rinf->cons)
    return;
  if (!rinf->cons->rom_putchar)
    return;
    
    d = y >> 4;
    if (d > 9)  /* if A-F add 7 to make alpha char */
        d += 7;
    rinf->cons->rom_putchar(d + '0', rinf);
    d = y & 0x0f;  
    if (d > 9)  /* if A-F add 7 to make alpha char */
        d += 7;
    rinf->cons->rom_putchar(d + '0', rinf);
}
#endif /* DEBUG */                    


#ifdef _MPFSH5M
#define CACHE_LINE_SIZE 32
#define NUM_WAYS 4

void flush_whole_dcache(u_int8 *start, u_int8 *end, int all_lines);

void cache_flush_sh5m(u_int32 *addr, u_int32 size, u_int8 type, Rominfo rinf)
{
  u_int8   *start;  /* beginning of cache line of stuff to flush */
  u_int8   *end;    /* end address to flush */
  u_int8   *cur;
  long val;
  u_int8 *iccr0;

  start = (u_int8*)((u_int32)addr & ~0x1f);
  end = (u_int8*)((u_int8*)addr + size);

  switch(rinf->hardware->cpu386.cc_cputype) {
  case CPUTYPE_SH5:	
	if (type == HW_CACHETYPE_INST) {
	  /* invalidate part/all of instruction cache */
	  iccr0 = (u_int8*)SH5M_ICCR0;
	  _asm(" getcfg %0,0,%1",
		   __reg_gen(__obj_modify(iccr0)), /* %0 */
		   __reg_gen(__obj_assign(val))   /* %1 */ );

	  if ((val & SH5M_ICE) == 0)
		/* if instruction cache is disabled, return */
		return;

	  if ((size == 0) || (size > 32 * 1024)) {
		/* invalidate whole instruction cache.  This is accomplished by writing
		** to the ICCR (instruction cache control register) */
		val = SH5M_ICI | SH5M_ICE;
		_asm(" putcfg %0,0,%1
               synci",
			 __reg_gen(__obj_modify(iccr0)), /* %0 */
			 __reg_gen(__obj_modify(val))    /* %1 */ );
	  } else {
		/* invalidate individual cache lines */
		for (cur = start; cur < end; cur += CACHE_LINE_SIZE) {
		  _asm(" icbi %0,0 ",__reg_gen(__obj_modify(cur)));
		} 
		_asm(" synci");
	  }
	}

	if (type == HW_CACHETYPE_DATA) {
	  /* flush/invalidate data cache */
	  u_int8 *occr0 = (u_int8*)SH5M_OCCR0;

	  _asm(" getcfg %0,0,%1",
		   __reg_gen(__obj_modify(occr0)), /* %0 */
		   __reg_gen(__obj_assign(val))   /* %1 */ );
	  if ((val & SH5M_ICE) == 0)
		/* return if data cache is disabled */
		return;

	  if (size == 0) {
		/* flush whole cache */
		flush_whole_dcache(start,end,1);
	  } else if (size > (64 * 1024)){
		/* flush whole cache, but only cache lines that are between start and end.
		** This method is performed rather than a simple flush by address method
	    ** because of speed. */
		flush_whole_dcache(start,end,0);
	  } else {
		/* flush the data by address */
		for (cur = start; cur < end; cur += CACHE_LINE_SIZE) {
		  _asm(" ocbp %0,0 ",__reg_gen(__obj_modify(cur)));
		} 
		_asm(" synco");
	  }
	}
	break;

  default:
	break;
  }
}


/*
** Flushes the dcache by looking at each cache line.  If 'all_lines' is true,
** then every line is flushed.  If 'all_lines' is false, then only lines with
** addresses between 'start' and 'end' will be flushed.
*/
void flush_whole_dcache(u_int8 *start, u_int8 *end, int all_lines)
{
  int index, way;
  u_int8 *tag1_base = (u_int8*)SH5M_OCACHETAG1_BASE;
  u_int8 *addr;
  u_int8 *cur;

  for (index = 0; index < 256; index++) {
	for (way = 0; way < NUM_WAYS; way++) {
	  cur = tag1_base + (way << 16) + (index << 4);
	  _asm(" getcfg %0,0,%1",
		   __reg_gen(__obj_modify(cur)),  /* %0 */
		   __reg_gen(__obj_assign(addr)) /* %1 */);
	  
	  if ((u_int32)addr & SH5M_VALID) {
		/* mask off bits 0..11 */
		addr = (u_int8*)((u_int32)addr & ~0x0fff);
		
		/* make sure address is sign extended */
		_asm(" add.l %0,zero,%0",__reg_gen(__obj_modify(addr)));
		
		/* add on bits 11..5 (refer to OCACHETAG1 register) */
		addr = (u_int8*)((u_int32)addr | ((index&0x7f)<<5));
		
		if (all_lines || ((addr >= start) && (addr < end))) {
		  /* flush */
		  _asm(" ocbp %0,0 ",__reg_gen(__obj_modify(addr)));
		}
	  }
	}
  }

  _asm(" synco ");
}
#endif
