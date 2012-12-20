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

#include <gendefs.h>

/*--------------------------------------------------------------!
! powerof2: Convert a value into a power of two (bit position)	!
!--------------------------------------------------------------*/
int powerof2(value)
register u_int32	value;
{
	register int	powercount = 0;
	
	if (value == 0)
		return ERROR;
	while (value > 1) {
		value >>= 1;
		powercount++;
	}
	return powercount;
}
