/*--------------------------------------------------------------------------!
! portboot.c: Serial Port booting driver									!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 89/04/13 <Pfoof!> It Exists.										jal	!
!               ---- OS-9/68K V2.4 released ----                            !
!               ---- OS-9/68K V3.0 released ----                            !
!               ---- OS-9/68K V3.1 released ----                            !
!---------------------------------------------------------------------------!
!                                                                           !
!              Copyright 1990 by Microware Systems Corporation              !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include	<sysboot.h>


#define C_XON	0x11
#define INDELAY	0x20000

error_code portboot()
{
	register u_char		*buffer;
	register int32		inchr;
	u_char				fsize[4];
	int32				filesize,
						pbbufsize = 256 + 256;
	u_char				*pbbuf, *bootpath;
	char				breakchr = '~';
	register error_code error = SUCCESS;

	if ((error = extract(&pbbufsize, &pbbuf)) != SUCCESS) {
		outstr("Can't allocate memory for communications buffers.\n");
		return error;
	} else bootpath = pbbuf + 256;
	
	/*------------------------------------------------------------------!
	! Present user with a choice to boot, go to "talk mode", or quit	!
	!------------------------------------------------------------------*/
	outstr("\nBOOT FROM SERIAL COMMUNICATIONS PORT\n\n");
	outstr("Is your host system ready to send?\n");
	outstr("\(Are you logged in, positioned in the correct directory etc...\)\n\n");
	do {
		outstr("y\(es\) -- host is ready, try to boot\n");
		outstr("n\(o\) --- will put you in \"talk mode\" to the host system\n");
		outstr("q\(uit\) - will return you to the boot driver menu\n\n\(y/n/q\) ---> ");
		instr(pbbuf, 256);
		OutChar('\n');
		inchr = makelower(*pbbuf);
		switch (inchr) {
			
			case 'y':
				break;
				
			case 'n':
				outstr("You are entering \"talk mode\" to the host system.\n");
				outstr("Select a \"break\" character (the default is \"~\"): ");
				instr(pbbuf, 256);
				if (*pbbuf != 0)
					breakchr = *pbbuf;
				talkmode(&breakchr);
				OutChar('\n');
				break;
				
			case 'q':
				insert(pbbufsize, pbbuf);
				return E_NOTRDY;
				
			default:
				OutChar('"');
				outstr(pbbuf);
				outstr("\" is not a valid response.\n\n");
				inchr = 0;
				break;
		}
	} while (inchr == 0);	

	/*------------------------------------------!
	! get the pathlist to the bootfile from the	!
	! "current" directory on the host machine	!
	!------------------------------------------*/
	outstr("Enter the pathlist to your bootfile:\n---> ");
	instr(bootpath, 256);
	
	/*------------------------------------------------------!
	! initialize the port to the host and flush it clean	!
	!------------------------------------------------------*/
	PortInit();
	OutPort('\n');
	flushall();
	OutPort(0x18);
	flushall();

	/*----------------------------------------------!
	! build a "shell" command line to tell the host	!
	! system to send the bootfile to this port		!
	!----------------------------------------------*/
	strcpy(pbbuf, "sload ");
	buffer = pbbuf + 6;
	strcpy(buffer, bootpath);
	poutstr(pbbuf);
	OutPort('\n');
	flushline();
	
	/*--------------------------------------!
	! Get bootfile size from host system	!
	!--------------------------------------*/
	inchr = inwait();
	if ((inchr != ERROR) && (inchr != '\l')) {
		fsize[0] = inchr;
	} else fsize[0] = inlwait();
 	fsize[1] = inlwait();
	fsize[2] = inlwait();
	fsize[3] = inlwait();
	if ((filesize = *((int32 *) &fsize[0])) == ERROR) {
		OutPort('\n');
		fsize[0] = inlwait();
	 	fsize[1] = inlwait();
		fsize[2] = inlwait();
		fsize[3] = inlwait();
		outstr("An error occurred on the host system.\n");
		insert(pbbufsize, pbbuf);
		return error = *((int32 *) &fsize[0]);
	}

	/*-----------------------------------!
	! allocate RAM needed (if necessary) !
	!-----------------------------------*/
	if ((error = getbootmem(filesize)) != SUCCESS) {
		outstr("Can't allocate memory for the bootfile.\n");
		insert(pbbufsize, pbbuf);
		return error;
	}
	outstr("The size of the bootfile being loaded is ");
	inttoascii(filesize, pbbuf);
	outstr(pbbuf);
	outstr(" bytes.\n");
	outstr("Now reading bootfile from host system (each dot = 1K bytes)\n");
	OutPort('\n');
	if (read_serial_boot(filesize) != filesize) {
		outstr("\nAn error occurred while reading the bootfile.\n");
		return E_READ;
	}
	OutChar('\n');
	bootsize = filesize;
	insert(pbbufsize, pbbuf);
	return SUCCESS;
}	


/*--------------------------------------------------------------!
! read_serial_boot: Read the bootfile in from the host system	!
!                   via the serial communications port			!
!--------------------------------------------------------------*/
read_serial_boot(filesize)
u_int32 filesize;
{
	register int32	xcount, bfcount;
	register u_char	*buffer;
	register int	inchr;
	
	bfcount = 0;
	xcount = filesize;
	buffer = bootram;

	while (--xcount >= 0) {
		if ((inchr = inlwait()) == ERROR)
			return ERROR;
		*buffer++ = inchr;
		bfcount++;
		if ((bfcount & 0xFFFFFC00) == bfcount)
			OutChar('.');
	}
	return bfcount;
}


pinline(ptr, size)
register char		*ptr;
register int32		size;
{
	register int32	dcount;
	register int32	bcount = 0;
	register int	inchr;
	
	/*------------------------------!
	! Discard any leftover linefeed	!
	!------------------------------*/
	if (((inchr = inwait())!= ERROR) && (inchr != '\l')) {
		bcount++;
 		*ptr++ = inchr;
		if (--size < 0)
			return bcount;
	}
	
	while (--size >= 0) {
		if ((inchr = inlwait()) == ERROR)
			return ERROR;
		bcount++;
		*ptr++ = inchr;
		if (inchr == '\n')
			break;
	}
	*ptr++ = 0;
	return bcount;
}


/*------------------------------------------!
! flushline: Throw away all input until the	!
!            communications port is silent	!
!------------------------------------------*/
flushall()
{
	while (inwait() != ERROR);
}


/*--------------------------------------------------!
! flushline: Throw away all input until a carriage	!
!            return is received or we time out		!
!--------------------------------------------------*/
flushline()
{
	register int	inchr;
	
	while (((inchr = inwait()) != '\n') && (inchr != ERROR));
}


/*----------------------------------------------------------------------!
! poutstr: Send a null terminated string out the communications port	!
!----------------------------------------------------------------------*/
poutstr(ptr)
register char	*ptr;
{
	while (*ptr != 0)
		OutPort(*ptr++);
}


/*----------------------------------------------------------!
! inlwait: Wait a really long time for a character at the	!
!         communications port or until we time out			!
!----------------------------------------------------------*/		  
inlwait()
{
	int32	retries, inchr;
	
	for (retries = 6; --retries >= 0;) {
		if ((inchr = inwait()) != ERROR)
			break;
	}
	return inchr;
}

/*--------------------------------------------------------------!
! inwait: Wait for a character at the communications port until	!
!         a character is received or we time out				!
!--------------------------------------------------------------*/		  
inwait()
{
	int32	dcount, inchr;
	
	for (dcount = INDELAY; --dcount >= 0;) {
		if ((inchr = ChekPort()) != ERROR)
			break;
	}
	return inchr;
}

