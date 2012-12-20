/*--------------------------------------------------------------------------!
! romboot.c: ROM booting drivers											!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 89/01/04 Hacked and translated from "sysboot.a"					jal	!
!               ---- OS-9/68K V2.4 released ----                            !
!	01 91/07/03 Fixed restoration of SR of 68000/008.					wwb	!
!   02 91/07/15 Test for type of CPU size reference was incorrect       Rwb	!
!	03 91/08/26 Made compatible with "sysboot.c" SR fixup.				wwb	!
!               ---- OS-9/68K V2.4.3 released ----                          !
!	04 92/08/07 fixed problems from new defs.							wwb	!
!   05 93/05/17 Modified for Ultra C. Extracted assembly code.          ats !
!	06 93/09/01 Added load-n-go feature.                                ats !
!   07 93/09/22 Cleared load_n_go when done.							ats !
!               ---- OS-9/68K V3.0 released ----                         	!
!   08 94/03/09 Fixed first call to goodmodule().                       ats !
!               ---- OS-9/68K V3.0.1 released ----                         	!
!	09 97/08/14 Added "quiet" boot support (fastboot).					wwb	!
!   10 99/02/09 fixed module search while loop to not double subtract   mgh !
!                the size of a found kernel (CF Problem 4521)				!
!   11 99/07/08 Changed load booter version to continue finding modules mgh !
!               without reguard to the end of a rom block.                  !
!				Changed return point for rom boot to return size of entire  !
!                 boot so it will be added to the rom search list allowing  !
!                 the search lists to be shorted to look for where the boot !
!                 can start rather than covering the whole area             !
!				Changed goodmodule to not check/require romsize				!
!               ---- OS-9/68K V3.1 released ----                         	!
!   12 00/01/10 Changed comments to match above statement.  NOTE: Contiguous!
!                module seaching takes longer here but allows us to pass up !
!                much smaller search lists to the kernel, speeding booting. !
!---------------------------------------------------------------------------!
!                                                                           !
!				Copyright 1990, 1991, 1992, 1993, 1994, 1997				!
!						by Microware Systems Corporation					!
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include	<sysboot.h>

#define		BEL		0x07

/*--------------------------------------!
! Static storage variable declarations	!
!--------------------------------------*/
extern 		bus_trap_rom();

u_int32			sizer;			/* module size or word (2)		*/
mod_exec		*modptr;		/* pointer to possible module	*/
u_int32			load_n_go = FALSE;	/* load and go flag:		*/
									/*	FALSE executes in-place */
									/*	TRUE moves ROM loadfile	*/
									/*		to RAM for speed	*/

extern u_char	*mpuptr;		/* MPUType pointer				*/

extern u_char	MPUType;		/* define as extern so compiler believes us */

extern		checkquiet();		/* asm routine that checks for quiet booting */
extern u_int32	*crystalptr;	/* ptr to boot method flags		*/
error_code goodmodule();		/* module header tester			*/
error_code romboot();			/* forward declaration 			*/


/*--------------------------------------------------------------------------!
! loadrom: This driver executes romboot() with the load-n-go flag set so	!
! that the bootfile will be loaded into system ram and run from there for	!
! speed.																	!
!--------------------------------------------------------------------------*/
error_code loadrom()
{
	error_code	stat;
	
	load_n_go = TRUE;
	stat = romboot();
	load_n_go = FALSE;
	
	return stat;
}


/*--------------------------------------------------------------------------!
! romboot: This driver does a search through the ROM list.  If it finds		!
! a module with good header parity and the module's name is "kernel", then	!
! walks the boot aread to find the end of contiguous modules.  It will then !
! return the size of the contiguous boot in a search list entry.            !
!--------------------------------------------------------------------------*/
error_code romboot()
{
	register mod_exec	*kmodptr;		/* pointer to kernel module			*/
	register u_int32	romsize,		/* ROM size							*/
						loadsize;		/* loadfile size					*/
	register u_int32	*romptr;		/* ROM list pointer					*/
	register error_code	error;			/* error code						*/
	u_int32				quietboot;		/* non-zero if "quiet" boot			*/
	u_int32				orgbustrap;		/* org. address of bus trap handler	*/
 	u_char				*tflagptr;		/* pointer to trapflag 				*/
 	u_char				tflag_image;	/* image of trapflag 				*/

	/*----------------------------------------------!
	! Check to see if there's a ROM list to search	!
	!----------------------------------------------*/
	if ((romptr = sysrommap) >= endrommap) {
		outstr("No \"special memory\" list is available.\n");
		return E_PARAM;
	}
		
	/*----------------------------------------------------------------------!
	! Develop a pointer to the REAL address of the debugger's "trapflag"	!
	!----------------------------------------------------------------------*/
	tflagptr = &trapflag - sysb_gptr + (u_char *) exjmptab;

	/*------------------------------------------------------------------!
	! Save and clear the trapflag and install the bus trap handler in	!
	! the exception jump table for the memory search routine.			!
	!------------------------------------------------------------------*/
	tflag_image = *tflagptr;					/* save an image of trapflag*/
	*tflagptr = 0;									/* clear the trapflag	*/
	orgbustrap = setexcpt(BUS_TRAP, bus_trap_rom);

	/*----------------------------------!
	! Search ROM list for kernel module	!
	!----------------------------------*/
	quietboot = checkquiet();				/* set booting mode */

	while (romptr < endrommap) {
		modptr = (mod_exec *) *romptr++;	/* get ptr to ROM beginning	*/
		romsize = *romptr++;	/* get size of ROM			*/
		/*------------------------------------------------------------------!
		! print memory search message:										!
		! "Now searching memory ($STRTADDR - $ENDADDR) for an OS-9 Kernel...!
		!------------------------------------------------------------------*/

		if (!quietboot) {
			outstr("Now searching memory \($");
			Out4Hex(modptr);
			outstr(" - $");
			Out4Hex(((u_int32) modptr) + romsize - 1);
			outstr("\) for an OS-9 Kernel...\n");
		}
		do {
			sizer = 2;
			error = goodmodule(TRUE);
			if (error == SUCCESS)
				break;
			(u_int32) modptr += sizer;
		} while ((romsize -= sizer) > 0);
		if (error == SUCCESS)
			break;
	}
	setexcpt(BUS_TRAP, orgbustrap);	/* reset Bus Error exception	*/
	*tflagptr = tflag_image;		/* restore the trapflag			*/
	OutChar('\n');					/* print carriage return		*/
	if (error != SUCCESS) {
		outstr("No OS-9 kernel was found.\n");
		return error;
	}
	if (!quietboot) {
		outstr("An OS-9 kernel module was found at $");
		Out4Hex(modptr);				/* print module address		*/
		OutChar('\n');					/* add carriage return		*/
	}

/* Removed so we can find entire boot */	
/*	if (!load_n_go) {				 */
/*		bootram = (u_char *) modptr; */	/* return kernel pointer	*/
/*		bootsize = sizer;			 */	/* return kernel size		*/
/*		return error;				 */
/*	}								 */


	kmodptr = modptr;					/* save pointer to kernel	*/
	(u_int32) modptr += sizer;			/* and move on..(add size of kernel)*/

	/* Scan for good modules in the bootfile */

	/* do this again for the scan... */
	tflag_image = *tflagptr;			/* save an image of trapflag*/
	*tflagptr = 0;						/* clear the trapflag		*/
	orgbustrap = setexcpt(BUS_TRAP, bus_trap_rom);
	
	/* Find the rest of the boot modules */
	do {
		/* The search may still access past end of rom in goodmodule */
		error = goodmodule(FALSE);
		if (error != SUCCESS)
			break;
		(u_int32) modptr += sizer;
	} while (error == SUCCESS);
	
	setexcpt(BUS_TRAP, orgbustrap);	/* reset Bus Error exception	*/
	*tflagptr = tflag_image;		/* restore the trapflag			*/

	loadsize = (u_int32)modptr - (u_int32)kmodptr;

/* Return size of entire found boot if we're doing a rom boot */
	if (!load_n_go) {
		bootram = (u_char *) kmodptr;	/* return kernel pointer	*/
		bootsize = loadsize;				/* return Boot size		*/
		return SUCCESS;
	}

	/* Move the bootfile (modules found) to system memory */	

	if ((error = getbootmem(loadsize)) != SUCCESS) {
		outstr("\nSorry, can't allocate memory for loadfile!\n");
		OutChar(BEL);
		return error;
	}
	memcpy(bootram, (char *)kmodptr, loadsize);
	bootsize = loadsize;
	
	return SUCCESS;
}


/*------------------------------------------------------------------!
! goodmodule: Module header check extracted from romboot() for      !
!             load-n-go feature.                                    !
!------------------------------------------------------------------*/
error_code goodmodule(kerchk)
u_int32		kerchk;
{
	mod_exec	*rmodptr = modptr;		/* pointer to module header	*/
	u_int16		parity;					/* parity accumulator		*/
	int16		wordcount;				/* header word count		*/
	char		*modname,				/* module name pointer		*/
				*kername;				/* Kernel name pointer		*/
	error_code	error = E_MNF;			/* error code				*/
	
	if (setjmp(gp_btrap) == 0) {
		if (modptr->_mh._msync == MODSYNC) {
			parity = -1;
			for (wordcount = (VHPCNT + 2)/2; --wordcount >= 0;)
				parity ^= *((u_int16 *) rmodptr)++;
			if (parity == 0) {	/* is the module header parity correct?	*/
				error = SUCCESS;
				sizer = modptr->_mh._msize;	/* get module size			*/
				if (!kerchk) return error;	/* not kernel check, we're OK	*/
				kername = "Kernel";			/* get Kernel name pointer	*/
				modname = (char *)((u_int32) modptr + modptr->_mh._mname);
			 	while (*modname != 0) {
					if (((*modname++ ^ *kername++) & ~0x20) != 0) {
						error = E_MNF;	/* return "module not found"	*/
						break;
					}
				}
				/*----------------------------------!
				! end of kernel name really found?	!
				!----------------------------------*/
				if (error == SUCCESS) {
					if (*kername != 0) {
						error = E_MNF;	/* if no, return "module not found"	*/
					} else return error;
				}
			} else error = E_BMHP;
		} else error = E_MNF;
	}
	return error;
}


/*------------------------------------------------------------------!
! bus_trap_rom: This is the bus trap handler for the memory search.	!
!------------------------------------------------------------------*/
/*   NOTE:  This is the original code written in 'C'.  It has been
 *    replaced with an assembly language version below.  This new
 *    version preserves the pre-exception SR so that the Trap flags
 *    are left intact.
 */
/*
bus_trap_rom()
{
	register u_int32	rsizer;

	rsizer = (u_int32) modptr;
	rsizer += chunksize;
	rsizer &= -(chunksize);
	rsizer -= (u_int32) modptr;
	sizer = rsizer;
	longjmp(gp_btrap, 1);
}
*/
/*
 *  bus_trap_rom:   This is the assembly language version of the above
 *     code.  It will restore the SR and thereby the Trace flags.
 */
#ifdef _UCC
_asm(" use rombtasm.a");
#else
@ use rombtasm.a
#endif
