/*
 * $Header:   /h0/MWOS/SRC/DEFS/HW/VCS/mcecc.h_v   1.8   30 Jun 1997 01:14:56   packager  $
 */

/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1996 by Microware Systems Corporation              |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, distribution, or creation of derivative works |
|  in any form to any party other than the licensee is strictly prohibited, |
|  unless expressly authorized in writing by Microware Systems Corporation. |
|                                                                           |
|---------------------------------------------------------------------------|
/* mcecc.h - Definitions for MCECC DRAM Controller Chip 
 *          ( mVme167/mVme187/mVme166/mVme1601-PPC eta'al )
 *
 * History:
 * Date       Who     Reason
 * ---------  ----  ------------------------------------------------
 * 21 Feb 94  Rwb   Created based mcecc.d ed.2 for OS-9 MVME167
 *                  ---- OS-9/68K V3.0.2 Released ----
 *                  ---- OS-9000/x86 V2.1 Released ----             
 *                  ---- OS-9000/PPC V2.1.1 Released ----           
 *                  ---- OS-9000/ARMv3 V2.2 Released ----           
 *                  ---- OS-9000/ARMv3 V2.2.1 Released ----         
 *                  $$              <RELEASE_INFO>                $$
 */

#ifndef _MCECC_H_

#define _MCECC_H_

#ifndef _MEMC040_H_
#include "memc040.h"			/* memc040/mcecc common reg defs */
#endif /* _MEMC040_H_ */

/*
 *  Conventions used in this file:
 *
 *   B_XXXX  denotes an OR maskable bit definition (note access size quoted)
 *   M_XXXX  denotes a bit mask (again, note access size quoted)
 *   c_xxxx  denotes a numeric constant
 *
 *
 *  The MCECC chip is a two-chip, four-way interleaving DRAM controller
 *  ASIC for the MC68040-type bus.  It was designed in part for use with
 *  the mVme167/187/PPC processors.
 *
 *  The first set of registers have layouts compatible with that of the
 *  MEMC040, so those registers will not be duplicated here. Except for
 *  specific bits that are named differently or needed that were not
 *  defined for the MEMC040.
 */

/* the hardware is..... */
typedef struct mceccchip {

	memc_t	memc;			/* memc040 common register set      (00-1C)    */
	u_int32 mcdcr,			/* Data Control                       (20)     */
	        mcscr,			/* Scrubber Control                   (24)     */
	        mcsprh,			/* Scrubber period (high byte)        (28)     */
	        mcsprl,			/* Scrubber period (low byte)         (2C)     */
	        mccpre,			/* Chip Prescale register             (30)     */
	        mcstr,			/* Scrubber time on/off               (34)     */
	        mcspreh,		/* Scrubber prescale (high byte)      (3C)     */
	        mcsprel,		/* Scrubber prescale (low bye)        (40)     */
	        mcstmh,			/* Scrubber timer (high byte)         (44)     */
	        mcstml,			/* Scrubber timer (low byte)          (48)     */
	        mcaddch,		/* Scrub Address counter (high)       (4C)     */
	        mcaddcih,		/* Scrub Address counter (intermh)    (50)     */
	        mcaddcil,		/* Scrub Address counter (interml)    (54)     */
	        mcaddcl,		/* scrub Address counter (low)        (58)     */
	        mcerrlog,		/* Scrub Error Logger                 (5C)     */
	        mceraddh,		/* Error Address (high)               (60)     */
	        mceraddih,		/* Error Address (interm high)        (64)     */
	        mceraddil,		/* Error Address (interm low)         (68)     */
	        mceraddl,		/* Error Address (low)                (6C)     */
	        mcersyn,		/* Error Syndrome                     (70)     */
            mcdeflt1,		/* Defaults register 1                (74)     */
	        mcdeflt2;       /* Defaults register 2                (78)     */

} mcecc_t, *Mcecc_p;


#define B_MCRCR_NCEBEN B_MCRCR_PAREN  /* non-correctable error = parity error */

/* MCECC specific registers below - only have what I've needed so far... */

#define B_MCDCR_ZFILL	0x10	/* Zero fill memory during scrub */
#define B_MCDCR_DERC	0x20	/* Disable error correction when set */

/*
 * MCSR - Scrubber control register 
 */
#define B_MCSCR_IDIS	0x01	/* Image disable when set             */
#define B_MCSCR_SBEIEN	0x02	/* Single-bit error interrupt enable  */
#define B_MCSCR_SCRBEN	0x04	/* Scrubber enable                    */
#define B_MCSCR_SCRB	0x10	/* Scrubber status                    */

/*
 * MCSTR --	Scrubber Time On/Off Register
 */
#define M_MCSTR_STOFF	0x07	/* Scrubber time off field */
#define M_MCSTR_STON	0x38	/* Scrubber time on field */

#define B_MCSTR_SRDIS	0x80	/* Disable read during scrub */

/* constansts that releate to time-on/time-off fields */
#define C_MCSTR_MIN		equ		0
#define C_MCSTR_16		equ		1
#define C_MCSTR_32		equ		2
#define C_MCSTR_64		equ		3
#define C_MCSTR_128		equ		4
#define C_MCSTR_256		equ		5
#define C_MCSTR_512		equ		6
#define C_MCSTR_MAX		equ		7

#endif /* _MEMC040_H_ */
