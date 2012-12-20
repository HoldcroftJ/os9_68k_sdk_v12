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

/*----------------------------------------------------------------------!
! inttoascii: Convert a 32 bit unsigned integer value to up to ten		!
! characters of numeric ASCII.  The result is deposited in the buffer	!
! and the advanced buffer pointer is returned.							!
!----------------------------------------------------------------------*/
char *inttoascii(value, bufptr)
register u_int32	value;
register char		*bufptr;
{
	register u_int32	scratch;
	register u_int32	divisor = 1000000000;
	register char		lzflag = FALSE;
	
	while (divisor != 0){
		scratch = value / divisor;
		if (lzflag == FALSE) {
			if ((scratch != 0) || (divisor <= 100)) {
				lzflag = TRUE;
			}
		}
		if (lzflag != FALSE) {
			*bufptr++ = 0x30 | scratch;
			value = value - (scratch * divisor);
		}
		divisor = divisor / 10;
	}
	*bufptr = 0;
	return bufptr;
}


