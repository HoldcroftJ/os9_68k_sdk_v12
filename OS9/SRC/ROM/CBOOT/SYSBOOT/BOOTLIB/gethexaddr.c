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

#include <types.h>
#include <gendefs.h>

/*--------------------------------------!
! gethexaddr: input an address in hex	!
!--------------------------------------*/
void *gethexaddr()
{
	register int16		counter;
	register int		inchr;
	register u_int32	hexval;
	register u_int32	address = 0;
	
	for (counter = 0; counter <= 8;) {
		for (;;) {
			while ((inchr = InChChek()) == ERROR);
			inchr = (char) inchr;
			if ((inchr == 'Q') || (inchr == 'q')) {
				outstr("\n\n");
				return ((void *)ERROR2);
			}
			if (inchr == '\n') {
				outstr("\n\n");
				if (counter != 0) {
					return ((void *)address);
				} else return ((void *)ERROR);
			} else if (inchr == BACKSPACE) {
				if (counter > 0) {
					OutChar(inchr);
					OutChar(' ');
					OutChar(inchr);
					address = address >> 4;
					--counter;
				}
				break;
			} else if ((counter < 8) && ((hexval = convhex(inchr)) != ERROR)) {
				OutChar(inchr);
				break;
			} else OutChar(BEEP);
		}
		if (inchr == BACKSPACE)
			continue;
		address = (address << 4) | hexval;
		counter++;
	}
}
