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

/*--------------------------------------------------!
! instr: get a string from the console input device	!
!--------------------------------------------------*/
char *instr(str, count)
register char	*str;
register int	count;
{
    register char *s,ch;

    s = str;
	while((ch = InChar()) != '\n') {
        switch(ch) {
       	    case 1: /* ctl-a */
               	while (*s) {
               		OutChar(*s++);
               		--count;
               	}
                break;
       	    case 8: /* backspace */
               	if (s > str) {
                    outstr(" \x8");
       	            --s;
       	            ++count;
               	}
                else 
       	        	OutChar(' ');
            case 17: /* ignore xon */
       	    case 19: /* ignore xoff */
                break;
       	    case 24: /* ctl-x */
               	while (s > str) {
                    outstr("\x8 \x8");
       	            --s;
       	            ++count;
               	}
                break;
       	    default:
       	    	if (--count == 0) {
                    outstr("\x8 \x8");
                    ++count;
                    break;
                }
               	*s++ = ch;
                break;
       	}
    }
    *s = '\0';
    return str;
}
