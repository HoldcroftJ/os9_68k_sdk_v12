/*
 * Copyright 1990 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include <sysboot.h>

/*------------------------------!
! insert: return memory to rom	!
!------------------------------*/
Dumb_mem insert(size, mem) 
register u_int32		size;			/* size of the block of memory	*/
register u_int32		*mem;			/* the memory pointer			*/
{
	register u_int32	*memptr = mem;	/* copy of the memory pointer	*/
	register u_int32	fillcount;		/* fill memory counter			*/
	u_int32				tfreememlist;	/* temporary storage for the	*/
										/* global free memory list ptr	*/
										
	/*--------------------------------------------------------------!
	! save the global free memory list pointer just in case the		!
	! memory we're returning happens to be SysBoot's system globals	!
	!---------------------------------------------------------------!
	! NOTE: This DEFINITELY happens in the "glue code" when exiting	!
	!		SysBoot when we're not running under RomBug!			!
	!--------------------------------------------------------------*/
	tfreememlist = (u_int32) freememlist;

	/*------------------------------------------------------!
	! fill memory to be returned with "SysBoot Used Me!"	!
	! NOTE: "MINALLOC" is expected to be divisible by 16!!!	!
	!------------------------------------------------------*/
	size = (size + MINALLOC - 1) & ~(MINALLOC - 1);	/* round up the size	*/
	memptr = mem;						/* get a copy of the memory pointer	*/
	for (fillcount = size/16; fillcount > 0; --fillcount) {
		*memptr++ = 0x53797342;		/* "SysB"	*/
		*memptr++ = 0x6F6F7420;		/* "oot "	*/
		*memptr++ = 0x55736564;		/* "Used"	*/
		*memptr++ = 0x204D6521;		/* " Me!"	*/
	}
	memptr = mem;					/* get a copy of the memory pointer		*/
	*memptr++ = tfreememlist;		/* point this block to "next" block		*/
	*memptr++ = size;				/* insert the size of this block		*/

	freememlist = (Dumb_mem) mem;	/* point head of the list to this block	*/
	fixmemlist();					/* if it's broke, use the sledgehammer	*/
	return freememlist;				/* return the pointer to the memory list*/
}
