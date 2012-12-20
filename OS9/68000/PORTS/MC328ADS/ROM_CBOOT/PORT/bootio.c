
/*
 * Copyright 1993, 1996 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

/*--------------------------------------------------------------------------!
!          bootio.c: Boot I/O routines from the OEM example target.         !
+---------------------------------------------------------------------------+
!   Edition History:                                                        !
!   #   Date    Comments                                                By  !
!   -- -------- ------------------------------------------------------- --- !
!   01 93/10/28 Genesis.                                                ats !
!      96/07/09 Adopted for MC328ADS port.                              ats !
!               ---- OS-9/68K MC68328 Support Beta2 Release ----            !
!--------------------------------------------------------------------------*/
         
#include <sysboot.h>

/* my favorite loop function */
#define	LOOP	for(;;)

/* utility routines */

#define ESC	0x1b
#define	CR	0x0d
#define TAB	0x09
#define BS	0x08
#define BEL	0x07

char getinchar()
{
	char inchar;
	
	inchar = InChar();
	if ((inchar >= 'A') && (inchar <= 'Z'))
		inchar |= CASEBIT;
		
	return(inchar);
}

int outhex(h)
u_int32 h;
{
	u_int32 t, l=0;
	char	d;

	OutChar('0');
	OutChar('x');
	if (!h) {
		OutChar('0');
		return(1);
	}
			
	for (t=0x10000000; t>=1; t/=0x10)
		if (h >= t) break;				/* skip leading zeros */
		
	for (; t>=1; t/=0x10) {
		d = h / t;
		if (d <= 9)
			OutChar(d + '0');
		else
			OutChar(d - 10 + 'a');
		l++;
		h = h - d * t;
	}
	return(l);
}

int outint(i)
u_int32 i;
{
	u_int32 t, l=0;

	if (!i) {
		OutChar('0');
		return(1);
	}
		
	for (t=1000000000; t>=1; t/=10)
		if (i >= t) break;				/* skip leading zeros */
		
	for (; t>=1; t/=10) {
		OutChar( (i / t) | 0x30 );
		i = i - (i / t) * t;
		l++;
	}
	return(l);
}

void outsome(s)
u_char *s;
{
	if (!(*s))
		outstr("<none>");
	else
		outstr(s);
}

void outerase(n)
u_int32 n;
{
	int 	i;
	
	OutChar(' ');
	OutChar(BS);
	
	for (i=n-1; i>0; i--) {
		OutChar(BS);
		OutChar(' ');
		OutChar(BS);
	}
}

u_char ask_ynq(quit)
u_int32 quit;
{
	char		inchar, newval, newprmpt, valspec;
	u_int32		n;

	valspec = FALSE;
	newprmpt = TRUE;
			
	LOOP {
		if (newprmpt) {
			outstr("\n\(<yes>/<no>");
			if (quit)
				outstr("/<quit>");
			outstr("\)? ");
			if (valspec){
				if (newval == 'y')		outstr("yes");
				else if (newval == 'n')	outstr("no");
				else					outstr("quit");
			}
			newprmpt = FALSE;
		}
		
		inchar = getinchar();
				
		if (inchar == CR) {
			if (!valspec) {
				newprmpt = TRUE;
				OutChar(BEL);
				continue;
			}
			break;
		}
		if (inchar == BS) {
			if (!valspec) {
				newprmpt = TRUE;
				OutChar(BEL);
				continue;
			}
			if (newval == 'y')		n = 3;
			else if (newval == 'n')	n = 2;
			else					n = 4;
			outerase(n);
			valspec = FALSE;
			continue;
		}
		if (!valspec) {
			newval = inchar;
			if (inchar == 'y') {
				outstr("es");
				valspec = TRUE;
				continue;
			}
			if (inchar == 'n') {
				OutChar('o');
				valspec = TRUE;
				continue;
			}
			if (quit && (inchar == 'q')) {
				outstr("uit");
				valspec = TRUE;
				continue;
			}
		}
		newprmpt = TRUE;
		OutChar(BEL);
	}
	return(newval);
}

/* Dummy entry points to satisfy linker
 * until this is put into sysboot.l */
 
void		checknvram()	{}
void		outendis()		{}

error_code	rc_btlist()		{}
error_code	rc_endis()		{}
error_code	rc_int()		{}
error_code	rc_vmeints()	{}
error_code	reconfig()		{}

