#include <gendefs.h>

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

/*----------------------------------------------------------!
! convhex: Convert an ASCII hex character into a hex nibble	!
!----------------------------------------------------------*/
int convhex(inchr)
register char		inchr;
{
	register int	hexval = 0;
	if ((inchr >= 'a') && (inchr <= 'f'))
		inchr -= ('a' - 'A');
	if ((inchr < '0') || ((inchr > '9') && (inchr < 'A')) || (inchr > 'F'))
		return ERROR;
	if ((hexval = inchr - '0') > 9)
		hexval -= 7;
	return hexval;
}
