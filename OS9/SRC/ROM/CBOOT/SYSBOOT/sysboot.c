/*--------------------------------------------------------------------------!
! sysboot.c: boot selection program called by "boot.a"						!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 89/01/03 Hacked into being from csysboot.c						jal	!
!	01 89/07/18 Repaired input line parsing code						jal	!
!	02 89/08/09 Fixed initialization of "ctrlr_id" to work for all		jal	!
!				booting algorithms											!
!   03 90/09/05 Repaired trappit() to restore pre-exception SR.         Rwb !
!               ---- OS-9/68K V2.4 released ----                            !
!	04 91/08/21 Fixed sr restoration for 68000/008.						wwb	!
!               ---- OS-9/68K V2.4.3 released ----                          !
!	05 92/08/07 fixed problems from new defs.							wwb	!
!   06 93/02/08 Added checks for inactive boot menu items				ats !
!   07 93/05/17 Modified for Ultra C compatibility mode. Extraced       ats !
!               assembly code into separate file.                           !
!               ---- OS-9/68K V3.0 released ----                         	!
!	08 97/08/15 added support for boot method flags.					wwb	!
!               ---- OS-9/68K V3.1 released ----                         	!
!---------------------------------------------------------------------------!
!                                                                           !  
!				Copyright 1990, 1991, 1992, 1993, 1997						!
!					by Microware Systems Corporation						!
!						Reproduced Under License							!
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include	<sysboot.h>

extern	error_code	checkmodhead(modhead);
extern	error_code	call_any_bdriver(driver_entry);

/*--------------------------!
! sysboot static storage	!
!--------------------------*/
u_char			*sysb_gptr;			/* sysboot's global data area pointer	*/
u_int32			sysb_dsize;			/* sysboot 'C' technology data size		*/

Bootjmptab		bootjmp;			/* pointer to the boot ROM entry table	*/
u_char			*portaddr;			/* base address of the hardware device	*/
Dumb_mem		freememlist;		/* pointer to the free memory list		*/
void			*exjmptab;			/* pointer to the exception jump table	*/
void			*sysrommap;			/* start address of system ROM list		*/
void			*endrommap;			/* end address of system ROM list		*/
u_int32			chunksize;			/* memory chunk size					*/
u_char			*bootram;			/* pointer to start of bootfile RAM		*/
u_int32			bootsize;			/* size of the bootfile					*/
u_int32			memsize;			/* size of RAM scarfed from system		*/
Bdrivdef		bootdriver;			/* boot driver definition array pointer	*/
char			*inbuff;			/* input buffer pointer					*/
char			*outbuff;			/* output buffer pointer				*/
u_char			*hbuff;				/* tape header or sector zero buffer	*/
u_char			bdrivers_qty = 8;	/* the maximum number of boot drivers	*/
u_char			vflag;				/* verbose flag: 0 = no error messages	*/
u_char			ctrlr_id = 0xFF;	/* SCSI controller default I.D. override*/
									/*(0xFF = don't override device default)*/
jmp_buf			systrap;			/* trap longjmp register structure 		*/
jmp_buf			gp_btrap;			/* general purpose bus trap longjmp struct*/
char			*argarray[MAXARGS];	/* input line argument array			*/
error_code		(*inizdriver)(),	/* pointer to driver's init routine		*/
				(*readdriver)(),	/* pointer to driver's read routine		*/
				(*rwnddriver)(),	/* pointer to driver's rewind routine	*/
				(*termdriver)();	/* pointer to driver's terminate routine*/
u_char			*mpuptr;			/* MPUType pointer */
u_char			*crystalptr;		/* Crystal (boot method) pointer.		*/

extern void		getargs();

extern u_char	MPUType;			/* define as extern so compiler believes us */
extern u_char	Crystal;			/* ditto								*/

error_code sysboot()
{
	register Bdrivdef	bdriver;
	register char		*msgptr;
	register char		bootmethod;
	register int32		driverno;
	register int32		length;	
	int32				maxlength;
	char				*tmsgptr;
	error_code			error;

#ifdef OSK	
	fixmemlist();
#endif

	/*--------------------------------------------------------------!
	! allocate "IOBUFSIZE * 2" bytes of memory for i/o buffers		!
	!--------------------------------------------------------------*/
	error = IOBUFSIZE * 2;
	if ((error = extract(&error, &inbuff)) != SUCCESS) {
		outstr("\"SysBoot\" can't allocate memory for I/O buffers.\n");
		return error;
	}
	outbuff = inbuff + IOBUFSIZE;
	
	/* develop pointer to MPUType variable */
	
	mpuptr = &MPUType - sysb_gptr + (u_char *) exjmptab;

	/* develop boot method flags ptr */
	crystalptr = &Crystal - sysb_gptr + (u_char *) exjmptab;
	
	bootmethod = getbootmethod();

	switch (bootmethod) {
		
		case USERSELECT:	
		/*------------------------------------------------------------------!
		! USERSELECT: This case prints a menu and allows the user to select	!
		! the booting procedure.											!
		!------------------------------------------------------------------*/
		do {
			/*--------------------------!
			! reinitialize variables	!
			!--------------------------*/
			ctrlr_id = 0xFF;	/* reset SCSI controller I.D. override	*/
			error = SUCCESS;	/* reset any bogus error code			*/

			maxlength = 0;
			bdriver = bootdriver;
			for (driverno = 0; driverno < barray_count; driverno++) {
				if ((msgptr = bdriver->menuline) == NULL)
					break;
				length = strlen(msgptr);
				if (maxlength < length)
					maxlength = length;
				bdriver++;
			}
			/*----------------------!
			! Print the menu header	!
			!----------------------*/
			OutChar('\n');
			msgptr = "BOOTING PROCEDURES AVAILABLE";
			length = strlen(msgptr);
			if (maxlength < length)
				maxlength = length;
			outstr(msgptr);
			outstr(" -");
			for (length = maxlength - strlen(msgptr); --length >= 0;)
				OutChar('-');
			outstr(" <INPUT>\n\n");
			
			/*------------------!
			! Print the menu	!
			!------------------*/
			bdriver = bootdriver;
			for (driverno = 0; driverno < barray_count; driverno++) {
				if ((msgptr = bdriver->menuline) == NULL)
					break;
				outstr(msgptr);
				outstr(" -");
				for (length = maxlength - strlen(msgptr); --length >= 0;)
					OutChar('-');
				outstr(" <");
				if (bdriver->idstring)
					outstr(bdriver->idstring);
				else
					outstr("(N/A)");
				outstr(">\n");
				bdriver++;
			}
			outstr("\nSelect a boot method from the above menu: ");
			instr(inbuff, 256);
			OutChar('\n');

			bdriver = bootdriver;
			for (driverno = 0; driverno < barray_count; driverno++,bdriver++) {
				if (bdriver->idstring == NULL) 
					continue;
				else if (streq(bdriver->idstring, inbuff) != FALSE)
					break;
			}
			/*------------------------------------------------------!
			! if the first parameter on the input line matches the	!
			! input string of a known boot function, then parse the	!
			! input line for the SCSI controller I.D. parameter		!
			!------------------------------------------------------*/
			if (driverno < barray_count) {
				getargs(inbuff);
				if ((msgptr = argarray[2]) != NULL) {
					outstr("Too many arguments!\n");
					error = E_DUMMY;
				} else if ((msgptr = argarray[1]) != NULL) {
					while (isdigit(*msgptr) != 0)
						msgptr++;
					if ((*msgptr != 0) || ((ctrlr_id = atoi(argarray[1])) > 7)) {
						outstr("Illegal controller I.D. : ");
						outstr(argarray[1]);
						outstr(" \(Legal range is 0 - 7\)\n");
						error = E_DUMMY;
					}
				}
				if (error == SUCCESS)
					error = callboot(bdriver);

			/*--------------------------------------------------!
			! No match for input string in boot function array	!
			!--------------------------------------------------*/
			} else {
				error = E_DUMMY;
				outstr("The input string: \"");
				outstr(inbuff);
				outstr("\", doesn't select a boot driver.\nTry again...\n\n");
			}
		} while	(error != SUCCESS);
		break;

		case SWITCHSELECT:
		/*--------------------------------------------------------------!
		! SWITCHSELECT: This case indicates that hardware switches are	!
		! used for boot device selection								!
		!--------------------------------------------------------------*/
		if ((bdriver = getboottype()) != (Bdrivdef) NULL)
			error = callboot(bdriver);
		break;


		case AUTOSELECT:
		/*------------------------------------------------------------------!
		! AUTOSELECT: This case indicates AUTOMATIC BOOT SELECTION			!
		+-------------------------------------------------------------------+
		! The automatic booting procedure follows the heirarchy set in the	!
		! boot device array in static storage.  This heirarchy should be	!
		! initialized by the system specific routine: "getmethod()"			!
		!------------------------------------------------------------------*/
		error = E_DUMMY;
		bdriver = bootdriver;
		for (driverno = 0; driverno < barray_count; driverno++, bdriver++) {
			if (bdriver->idstring == NULL) {
				outstr("...Skipping ");
				outstr(bdriver->menuline);
				outstr(" since H/W is N/A.\n");
				continue;
			}
			strcpy(inbuff, bdriver->idstring);
			if (vflag != FALSE) {
				outstr("Now trying to ");
				outstr(bdriver->menuline);
				outstr(".\n");
			}
			if ((error = callboot(bdriver)) == SUCCESS)
				break;
		}
	}
	insert(IOBUFSIZE * 2, inbuff);	/* return the i/o buffer to the system	*/
	if (bootdriver != NULL)
		insert(bdrivers_qty * sizeof(bdrivdef), bootdriver);

	return error;
}

void getargs(lineptr)
register char		*lineptr;
{
	register char	**argv;
	register char	*arg;
	register char	nxchar;
	register int	argc;

	/*--------------------------!
	! Clear the argument array	!
	!--------------------------*/
	argv = argarray;
	argc = MAXARGS;
	while (--argc > 0)
		*argv++ = NULL;
	
	/*----------------------------------!
	! copy input line to output buffer	!
	!----------------------------------*/
	arg = outbuff;
	while (*lineptr != 0)
		*arg++ = *lineptr++;
	*arg = 0;
	
	/*--------------------------------------!
	! initialize the argument pointer array	!
	!--------------------------------------*/
	argv = argarray;
	argc = 0;
	arg = lineptr = outbuff;
	while (*lineptr != 0) {
		switch (*lineptr) {
			case ' ':
			case ',':
			case ':':
			case ';':
				*lineptr = 0;
				*argv++ = arg;
				if (++argc >= MAXARGS)
					return;
				while (((nxchar = *++lineptr) == ' ') ||
						(nxchar == ',') ||
						(nxchar == ':') ||
						(nxchar == ';'))
					continue;
				arg = lineptr;
				break;
			default:
				++lineptr;
		}
	}
	if (*arg != 0)
		*argv = arg;
}
	
/*--------------------------------------------------------------------------!
! callboot: Call the boot driver whose "boot driver definition structure"	!
!           pointer was passed in											!
!--------------------------------------------------------------------------*/
callboot(bdriver)
register Bdrivdef	bdriver;
{
	register char		*msgptr;
	u_int32				orgbustrap;		/* org. addr of bus trap handler	*/
	u_int32				orgaddtrap;		/* org. addr of address trap handler*/
	u_int32				orgilltrap;		/* org. addr of ill inst trap handler*/
	u_int32				orgliftrap;		/* org. addr of line F trap handler	*/
	register error_code error;
	
	/*------------------------------------------------------!
	! Install the trap handlers in the exception jump table	!
	!------------------------------------------------------*/
	orgbustrap = setexcpt(BUS_TRAP, trappit);
	orgaddtrap = setexcpt(ADDR_TRAP, trappit);
	orgilltrap = setexcpt(ILL_TRAP, trappit);
	orgliftrap = setexcpt(LINF_TRAP, trappit);

	/*--------------------------------------------------!
	! set up jump return point for exception handler	!
	!--------------------------------------------------*/
	if ((error = setjmp(systrap)) == SUCCESS) {
		
		/*--------------------------------------!
		! Clear pointers to driver functions	!
		!--------------------------------------*/
		inizdriver = readdriver = rwnddriver = termdriver 
					= (error_code (*)()) NULL;
			
		/*------------------------------------------------------!
		! call the boot driver (according to language type)		!
		!------------------------------------------------------*/
		error = call_any_bdriver(bdriver->driver_entry);

	} else {
		/*--------------------------------------!
		! Handle exception error calculation	!
		! and print an error message			!
		!--------------------------------------*/
		switch (error/4) {
			
			case BUS_TRAP:
				error = E_BUSERR;
				msgptr = "A Bus Error";
				break;
	
			case ADDR_TRAP:
				error = E_ADRERR;
				msgptr = "An Address Error";
				break;
	
			case ILL_TRAP:
				error = E_ILLINS;
				msgptr = "An Illegal Instruction";
				break;
	
			case LINF_TRAP:
				msgptr = "A Line 'F'";
				error = E_1111;
				break;
				
			default:
				msgptr = "An Unknown";
		}
		if (vflag != FALSE) {
			outstr(msgptr);
			outstr(" Exception was taken!\n");
		}
	}
	
	/*------------------------------------------------------!
	! Re-install the original trap handler addresses in the	!
	! exception jump table									!
	!------------------------------------------------------*/
	setexcpt(BUS_TRAP, orgbustrap);		/* reset Bus Error exception		*/
	setexcpt(ADDR_TRAP, orgaddtrap);	/* reset Address Error exception	*/
	setexcpt(ILL_TRAP, orgilltrap);		/* reset Illegal Address Trap		*/
	setexcpt(LINF_TRAP, orgliftrap);	/* reset Line F Trap				*/
			
	/*------------------------------------------------------!
	! If a boot driver was called and no error occurred,	!
	! Check the bootfile for a valid module header.			!
	!------------------------------------------------------*/	
	if (error == SUCCESS)
		error = checkmodhead(bootram);

	if (error != SUCCESS) {

		/*------------------------------------------!
		! return any memory allocated for bootfiles	!
		!------------------------------------------*/
		if ((bootram != NULL) && (memsize != 0))
			insert(memsize, bootram);
		bootram = NULL;
		memsize = 0;
		
		/*--------------------------!		
		! Print an error message	!
		!--------------------------*/
		if (vflag != FALSE) {
			outstr("This error occurred during the ");
			outstr(bdriver->driver_name);
			outstr(" boot driver!\n");
			outstr("The OS-9 error code is: #");
			msgptr = inttoascii((u_char)(error >> 8), outbuff);
			*msgptr++ = ':';
			msgptr = inttoascii((u_char) error, msgptr);
			*msgptr++ = 0;
			outstr(outbuff);
			outstr(".\n\n");
		}
	} else outstr("A valid OS-9 bootfile was found.\n");
	return error;
}

/*----------------------------------------------------------------------!
! call_any_bdriver: call the boot driver (according to language type)	!
!----------------------------------------------------------------------*/
error_code call_any_bdriver(driver_entry)
register error_code (*driver_entry)();
{
	if (*((u_int32 *) ((void *)driver_entry)) == ASM_SYNC) {
		return aboot_glue(driver_entry);
	} else return (*driver_entry)();
}

/*----------------------------------------------------------!
! checkmodhead: Check the integrity of the module header	!
!----------------------------------------------------------*/
error_code checkmodhead(modhead)
register mod_exec	*modhead;
{
	if (modhead->_mh._msync != MODSYNC) {
		outstr("The boot module has a bad header!\n");
		return E_BMID;
	} else return SUCCESS;
}


/*--------------------------------------------------!
! trappit: This is the trap handler for boo boo's	!
!--------------------------------------------------*/
#ifdef _UCC
_asm(" use sysbtasm.a");
#else
@ use sysbtasm.a
#endif

trap_it(vectaddr)
register u_int32	vectaddr;
{
	longjmp(systrap, vectaddr);
}
