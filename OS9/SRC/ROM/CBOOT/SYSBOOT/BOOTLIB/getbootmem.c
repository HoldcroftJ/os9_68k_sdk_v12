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

#include <types.h>
#include <gendefs.h>

extern u_int32	memsize;
extern u_char	*bootram;

/*------------------------------------------------------!
! getbootmem: allocate memory for a bootfile if needed	!
!------------------------------------------------------*/
error_code getbootmem(sizereq)
u_int32					sizereq;	/* size of RAM requested	*/
{
	register error_code	error;
	
	/*----------------------------------------------!
	! If memory is already allocated, give it back	!
	!----------------------------------------------*/
	if (bootram != NULL)
		insert(memsize, bootram);
	memsize = 0;

	/*--------------------------------------!
	! Allocate the memory for the bootfile	!
	!--------------------------------------*/	
	if ((error = extract(&sizereq, &bootram)) == SUCCESS)
		memsize = sizereq;
	return error;
}
