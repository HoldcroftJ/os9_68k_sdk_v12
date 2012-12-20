 opt -l
********************
* Definitions for MCECC DRAM Controller Chip - VME167/MVME177
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  93/08/27  Created                                          ats
* 02  93/11/04  Added scrubbing register definitions.            ats
*				---- OS-9/68K V3.0.2 Beta release ----
*               ---- OS-9/68K V3.0.2 Release ----

*
*  Conventions used in this file:
*
*   b_XXXX  denotes a bit number definition (note access size quoted)
*   m_XXXX  denotes a bit mask (again, note access size quoted)

*
*  The MCECC chip is a two-chip, four-way interleaving DRAM controller
*  ASIC for the MC68040-type bus.  It was designed in part for use with
*  the mVme167/187 processors.

*  The first set of registers have layouts compatible with that of the
*  MEMC040, so those registers will not be duplicated here. Except for
*  specific bits that are named differently or needed that were not
*  defined for the MEMC040.

b_MCRCR_nceben	equ		b_MCRCR_paren	non-correctable error == parity error

*  MCECC specific registers below - only have what I've needed so far...

MCDCR			equ		$20		Data Control Register

b_MCDCR_zfill	equ		4		Zero fill memory during scrub
b_MCDCR_derc	equ		5		Disable error correction when set

MCSCR			equ		$24		Scrubber Control Register

b_MCSCR_idis	equ		0		Image disable when set
b_MCSCR_sbeien	equ		1		Single-bit error interrupt enable
b_MCSCR_scrben	equ		3		Scrubber enable
m_MCSCR_scrben	equ		(1<<b_MCSCR_scrben)
b_MCSCR_scrb	equ		4		Scrubber status

MCSPRH			equ		$28		Scrubber Period Register (hi byte)
MCSPRL			equ		$2C		Scrubber Period Register (lo byte)

MCSTR			equ		$34		Scrubber Time On/Off Register

b_MCSTR_stoff	equ		0		Scrubber time off field
m_MCSTR_stoff	equ		(7<<b_MCSTR_stoff)

b_MCSTR_ston	equ		3		Scrubber time on field
m_MCSTR_ston	equ		(7<<b_MCSTR_ston)

b_MCSTR_srdis	equ		7		Disable read during scrub
m_MCSTR_srdis	equ		(1<<b_MCSTR_srdis)

c_MCSTR_min		equ		0
c_MCSTR_16		equ		1
c_MCSTR_32		equ		2
c_MCSTR_64		equ		3
c_MCSTR_128		equ		4
c_MCSTR_256		equ		5
c_MCSTR_512		equ		6
c_MCSTR_max		equ		7

* end of "mcecc.d"
 opt l
