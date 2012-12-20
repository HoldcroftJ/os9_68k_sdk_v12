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

extern char *skipspac();

/**--------------------------------------------------------------
!   TalkMode -  Talk or Transparent mode causes the system to act
!               as if it were just a terminal.  This allows the
!               console to effectively be attached to two systems
!               at the command of the operator.  There is one
!               character reserved from the operators use during
!               talk mode but it is chosen by the operator at the
!               time talkmode is entered.  This character when
!               input will cause the talkmode to be exited.
*/
char *talkmode(cmdptr)
register char *cmdptr;
{
    register char breakout, chr;

	if(*(cmdptr = skipspac(cmdptr)) != '\0') {
		breakout = *cmdptr;
		PortInit();
		for (;;) {
			if((chr = InChChek()) != ERROR) {
				if(chr == breakout) {
					OutChar('\n');
					break;
				}
				OutPort(chr);
			}
			if((chr = ChekPort()) != ERROR) OutRaw(chr);
		}
	}
	else outstr("no break char\n");
	PortDeIn();
	return NULL;
}

char *skipspac(s)
register char *s;
{
    while (*s && *s == ' ') ++s;
    return s;
}

