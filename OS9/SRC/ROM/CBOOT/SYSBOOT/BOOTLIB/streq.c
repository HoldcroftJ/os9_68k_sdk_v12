/*
 * Copyright 1990, 1993 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include <gendefs.h>

extern	char	makelower();

/*--------------------------------------------------!
! streq: compare strings for functional equality	!
!--------------------------------------------------*/
u_int32 streq(stg1, stg2)
register char		*stg1,
					*stg2;				/* unknown string */
{
	while (*stg2 == ' ')
		stg2++;
	while (makelower(*stg1) == makelower(*stg2)) {
		stg1++;	stg2++;
		if (*stg1 == '\0')
			return TRUE;
	}
	return FALSE;
}
