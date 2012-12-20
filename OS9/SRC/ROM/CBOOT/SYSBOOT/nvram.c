
/*
 * Copyright 1993 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */
         
#include <sysboot.h>
#include <nvram.h>

/* my favorite loop function */
#define	LOOP	for(;;)

extern error_code sysreset();

/* platform specific externals */
extern error_code rc_dialogue();
extern Nv_ram getnvrbase();
extern bdrivdef booters[];

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

void outendis(f)
u_int32 f;
{
	if (f)
		outstr("enabled");
	else
		outstr("disabled");
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

/*
 * checknvram: Check the non-volatile ram in the 48t02 chip to
 * determine the validity of its contents.  If the validity
 * check fails, re-initialize the nv_ram.
 *
 * Returns: nvrbase filled with the current contents of the NVRAM.
 */
void checknvram(nvrimage)
Nv_ram nvrimage;
{
	Nv_ram		nvrbase = getnvrbase();
	u_int32		*clrptr, count;
    u_int32		bdrvmax = getbdrvmax();
    error_code	stat;

	/*  If nv_ram is corrupted, reconfigure */
	if (!goodnvr()) {

		outstr("\n\n*** ATTENTION ***\n");
		outstr("\nNVRAM was found corrupted - reconfiguration is forced.\n");

		/* get the shadow image of nvram */
		nvrw( nvrimage, nvrbase, FALSE, sizeof(nv_ram) );

		do {
			outstr("\nDo you wish to reinitialize the new OS9 area\n");
			outstr("of NVRAM before entering new values:");
			
			if (ask_ynq(FALSE) == 'y') {
				
				/* clear zero the image of the nvram */
				clrptr = (u_int32 *) nvrimage;
				for (count = (sizeof(nv_ram)/sizeof(u_int32)); count > 0; count--)
					*clrptr++ = 0;

				/* rebuild the boot order array to default priority */
				for (count = 0; count < bdrvmax; count++) {
					*((u_int32 *) &nvrimage->nvr_booter[count].booterno) =
							(count + 1) << 24;
				}
			}

			/* call the platform specific dialogue */
			rc_dialogue(nvrimage, 0);
			
			outstr("\n\nIs this the configuration you want:\n");
			outstr("\(y\)es will reconfigure and restart the system.\n");
			outstr("\(n\)o will restart this reconfiguration dialogue.");

		} while (ask_ynq(FALSE) == 'n');
		
		/* now ensure good form */		
		nvrimage->nvr_newsync = NVRNEWSYNC;
		nvrimage->nvr_version = getnvrversion();
		nvrimage->nvr_spcrst = 0;
		nvrimage->nvr_cksum =
			nwck16(nvrimage, sizeof(nv_ram) - sizeof(u_int16));
			
		/* update the NVRAM */
		nvrw( nvrimage, nvrbase, TRUE, sizeof(nv_ram) );	
		
		/* and go use it */
		sysreset();
	} else
		/* get the shadow image of nvram */
		nvrw( nvrimage, nvrbase, FALSE, sizeof(nv_ram) );	
}

error_code reconfig()
{
	nv_ram		nvr;
	Nv_ram		nvrimage = &nvr;
	Nv_ram		nvrbase = getnvrbase();
	char		inchar;
	
	/* shadow the NVRAM on the stack for use by program */
	nvrw (nvrimage, nvrbase, FALSE, sizeof(nv_ram));
	
	do {
		/* call the hardware specific dialogue manager */
		rc_dialogue(nvrimage, 0);

		outstr("\n\nIs this the configuration you want:\n");
		outstr("\(y\)es will reconfigure and restart the system.\n");
		outstr("\(n\)o will restart this reconfiguration program.\n");
		outstr("\(q\)uit will return you to the boot driver menu.");

		if ((inchar = ask_ynq(TRUE)) == 'q')
				return E_NOTRDY;
				
	} while (inchar == 'n');

	/* update the version */
	nvrimage->nvr_version = getnvrversion();
	
	/* update the checksum */
	nvrimage->nvr_cksum =
		nwck16(nvrimage, sizeof(nv_ram) - sizeof(u_int16));
					
	/* update nvram and reset */
	nvrw (nvrimage, nvrbase, TRUE, sizeof(nv_ram));
	sysreset();
}

/* reconfiguration dialog common subroutines */

error_code rc_int(prompt, cr_val, df_val, flags)
char *prompt;
u_int32 *cr_val,df_val;
u_int16 flags;
{
	char	inchar, newval, newprmpt;
	u_int32 newint, vallen;
	
	if (flags & NVR_RCFORCE)
		outstr("\nWarning: you are forced to enter the default value.");
		
	outstr(prompt);
	if (!(flags & NVR_RCFORCE)) {
		outstr(" [current=");
		if (flags & NVR_RCHEX)
			outhex(*cr_val);
		else
			outint(*cr_val);
		OutChar(']');
	}
	outstr(" [default=");
	if (flags & NVR_RCHEX)
		outhex(df_val);
	else
		outint(df_val);
	OutChar(']');
	
	newval = TRUE;
	LOOP {
		if (newval) {
			if (flags & NVR_RCDEFAULT)
				newint = df_val;
			else
				newint = *cr_val;
			newval = FALSE;
			newprmpt = TRUE;
		}
			
		if (newprmpt) {
			outstr("\n\(<new_value><cr>");
			if (!(flags & NVR_RCFORCE))
				outstr("/<tab>");
			outstr("/<esc>\)? ");
			if (flags & NVR_RCHEX)
				vallen = outhex(newint);
			else
				vallen = outint(newint);
			newprmpt = FALSE;
		}
		
		inchar = getinchar();
		
		if (inchar == ESC)
			return E_NOTRDY;
		if (inchar == CR)
			break;
		if (flags & NVR_RCFORCE) {
			newprmpt = TRUE;
			OutChar(BEL);
			continue;
		}
		if (inchar == TAB) {
			newval = TRUE;
			flags ^= NVR_RCDEFAULT;
			continue;
		}
		if (inchar == BS) {
			if (vallen == 0) {
				newprmpt = TRUE;
				OutChar(BEL);
				continue;
			}
			OutChar(' ');
			OutChar(BS);
			if (flags & NVR_RCHEX)
				newint /= 0x10;
			else
				newint /= 10;
			vallen--;
			continue;
		}
		if (('0' <= inchar) && (inchar <= '9')) {
			if (((flags & NVR_RCHEX) && (newint & 0xf0000000)) ||
					(!(flags & NVR_RCHEX) && (newint > 100000000))) {
				newprmpt = TRUE;
				OutChar(BEL);
				continue;
			}
			if (flags & NVR_RCHEX)
				newint *= 0x10;
			else
				newint *= 10;
			newint += inchar - '0';
			vallen++;
			continue;
		}
		if ((flags & NVR_RCHEX) && ('a' <= inchar) && (inchar <= 'f')) {
			if (newint & 0xf0000000) {
				newprmpt = TRUE;
				OutChar(BEL);
				continue;
			}
			newint *= 0x10;
			newint += inchar - 'a' + 10;
			vallen++;
			continue;
		}
		newprmpt = TRUE;
		OutChar(BEL);
	}
	*cr_val = newint;
	return 0;
}

error_code rc_endis(prompt, cr_val, df_val, flags)
char *prompt;
u_char *cr_val,df_val;
u_int16 flags;
{
	char	inchar, editing, newval, newprmpt;
	char	newflag;
	
	if (flags & NVR_RCFORCE)
		outstr("\nWarning: you are forced to enter the default value.");
		
	outstr(prompt);
	if (!(flags & NVR_RCFORCE)) {
		outstr(" [current=");
		outendis(*cr_val);
		OutChar(']');
	}
	outstr(" [default=");
	outendis(df_val);
	OutChar(']');

	newval = TRUE;
	LOOP {
		if (newval) {
			if (flags & NVR_RCDEFAULT)
				newflag = df_val;
			else
				newflag = *cr_val;
			newval = FALSE;
			newprmpt = TRUE;
		}
			
		if (newprmpt) {
			outstr("\n\(enabled<cr>/disabled<cr>");
			if (!(flags & NVR_RCFORCE))
				outstr("/<tab>");
			outstr("/<esc>\)? ");
			outendis(newflag);
			newprmpt = FALSE;
			editing = FALSE;
		}
		
		inchar = getinchar();
		
		if (inchar == ESC)
			return E_NOTRDY;
		if (inchar == CR)
			break;
		if (flags & NVR_RCFORCE) {
			newprmpt = TRUE;
			OutChar(BEL);
			continue;
		}
		if (inchar == TAB) {
			newval = TRUE;
			flags ^= NVR_RCDEFAULT;
			continue;
		}
		if (inchar == BS) {
			if (editing) {
				newprmpt = TRUE;
				OutChar(BEL);
				continue;
			}
			editing = TRUE;
			outerase((newflag ? 7 : 8));
			continue;
		}
		if (editing && (inchar == 'e')) {
			outstr("nabled");
			editing = FALSE;
			newflag = TRUE;
			continue;
		}
		if (editing && (inchar == 'd')) {
			outstr("isabled");
			editing = FALSE;
			newflag = FALSE;
			continue;
		}
		newprmpt = TRUE;
		OutChar(BEL);
	}
	*cr_val = newflag;
	return 0;
}

error_code rc_btlist(cr_val, df_val, flags)
u_char *cr_val, *df_val;
u_int16 flags;
{
	char	inchar,  newval, newprmpt;
	char	newlist[BDRVTSZ];
	int		i, bdrvmax = getbdrvmax();

	outstr("\nBoot Drivers Available:\n\n");
	for (i=0; i<bdrvmax; i++) {
		if (i < 9)
			OutChar(i + '1');
		else
			OutChar(i - 9 + 'a');
		outstr(" - ");
		outstr(booters[i].menuline);
		OutChar('\n');
	}
	outstr("\nThe priority of these boot drivers can be set below.\n");
	outstr("This priority determines the order that the boot drivers\n");
	outstr("will be selected when your system is configured to boot\n");
	outstr("automatically. It also determines the order that they\n");
	outstr("appear in the \"boot driver menu\" as well.\n");

	if (flags & NVR_RCFORCE)
		outstr("\nWarning: you are forced to enter the default value.");
		
	outstr("\nBooter Priority Order:");
	if (!(flags & NVR_RCFORCE)) {
		outstr(" [current=");
		outsome(cr_val);
		OutChar(']');
	}
	outstr(" [default=");
	outsome(df_val);
	OutChar(']');
	
	newval = TRUE;
	LOOP {
		if (newval) {
			i = 0;
			if (flags & NVR_RCDEFAULT) {
				while ((newlist[i] = df_val[i]) != 0)
					i++;
			} else {
				while ((newlist[i] = cr_val[i]) != 0)
					i++;
			}
			newval = FALSE;
			newprmpt = TRUE;
		}
		
		if (newprmpt) {
			outstr("\n\(<newlist><cr>");
			if (!(flags & NVR_RCFORCE))
				outstr("/<tab>");
			outstr("/<esc>\)? ");
			outstr(newlist);
			newprmpt = FALSE;
		}
		
		inchar = getinchar();
		
		if (inchar == ESC)
			return E_NOTRDY;
		if (inchar == CR)
			break;
		if (flags & NVR_RCFORCE) {
			newprmpt = TRUE;
			OutChar(BEL);
			continue;
		}
		if (inchar == TAB) {
			newval = TRUE;
			flags ^= NVR_RCDEFAULT;
			continue;
		}
		if (inchar == BS) {
			if (i == 0) {
				newprmpt = TRUE;
				OutChar(BEL);
				continue;
			}
			OutChar(' ');
			OutChar(BS);
			newlist[--i] = 0;
			continue;
		}
		if (i == BDRVSLTS) {
			newprmpt = TRUE;
			OutChar(BEL);
			continue;
		}
		if ((('0' < inchar) && (inchar <= '9')) &&
				((inchar - '0') <= bdrvmax)) {
			newlist[i++] = inchar;
			newlist[i] = 0;
			continue;
		}
		if ((('a' <= inchar) && (inchar <= 'f')) &&
				((inchar - 'a' + 10) <= bdrvmax)) {
			newlist[i++] = inchar;
			newlist[i] = 0;
			continue;
		}
		newprmpt = TRUE;
		OutChar(BEL);
	}

	do {
		cr_val[i] = newlist[i];
	} while (i-- > 0);
	
	return 0;
}

error_code rc_vmeints(cr_val, df_val, flags)
u_char *cr_val, *df_val;
u_int16 flags;
{
	char inchar, newval, newprmpt, duplicate;
	char newlist[8];
	int i,j;
	
	if (flags & NVR_RCFORCE)
		outstr("\nWarning: you are forced to enter the default value.");
		
	outstr("\nVME interrupt levels enabled:");
	if (!(flags & NVR_RCFORCE)) {
		outstr(" [current=");
		outsome(cr_val);
		OutChar(']');
	}
	outstr(" [default=");
	outsome(df_val);
	OutChar(']');
	
	newval = TRUE;
	LOOP {
		if (newval) {
			i = 0;
			if (flags & NVR_RCDEFAULT) {
				while ((newlist[i] = df_val[i]) != 0)
					i++;
			} else {
				while ((newlist[i] = cr_val[i]) != 0)
					i++;
			}
			newval = FALSE;
			newprmpt = TRUE;
		}
		
		if (newprmpt) {
			outstr("\n\(<newlist><cr>");
			if (!(flags & NVR_RCFORCE))
				outstr("/<tab>");
			outstr("/<esc>\)? ");
			outstr(newlist);
			newprmpt = FALSE;
		}
		
		inchar = getinchar();
		
		if (inchar == ESC)
			return E_NOTRDY;
		if (inchar == CR)
			break;
		if (flags & NVR_RCFORCE) {
			newprmpt = TRUE;
			OutChar(BEL);
			continue;
		}
		if (inchar == TAB) {
			newval = TRUE;
			flags ^= NVR_RCDEFAULT;
			continue;
		}
		if (inchar == BS) {
			if (i == 0) {
				newprmpt = TRUE;
				OutChar(BEL);
				continue;
			}
			OutChar(' ');
			OutChar(BS);
			newlist[--i] = 0;
			continue;
		}
		if (i == 7) {
			newprmpt = TRUE;
			OutChar(BEL);
			continue;
		}
		duplicate = FALSE;
		for (j=0; j<i; j++)
			if (inchar == newlist[j])
				duplicate = TRUE;
		if ((!duplicate) && ('0' < inchar) && (inchar <= '7')) {
			newlist[i++] = inchar;
			newlist[i] = 0;
			continue;
		}
		newprmpt = TRUE;
		OutChar(BEL);
	}
	
	do {
		cr_val[i] = newlist[i];
	} while (i-- > 0);
	
	return 0;
}
