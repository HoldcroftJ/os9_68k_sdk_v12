/*--------------------------------------------------------------------------!
! binboot.c: Binary Bootfile booting driver                                 !
+---------------------------------------------------------------------------+
!   Edition History:                                                        !
!   #   Date    Comments                                                By  !
!   -- -------- ------------------------------------------------------- --- !
!   00 93/08/26 Genesis.                                                ats !
!	01 93/09/02 Eliminate redundant rounding.   						ats !
!               ---- OS-9 V3.0 Released ----								!
!               ---- OS-9 V3.1 Released ----								!
!---------------------------------------------------------------------------!
!                                                                           !
!              Copyright 1993 by Microware Systems Corporation              !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include    <sysboot.h>

#define		LOOP	for(;;)
#define		CR		0x0d
#define		BS		0x08
#define		BEL		0x07	

error_code binboot()
{
	char	inchar, newprmpt;
	u_int32	newint, vallen;
	error_code		error = SUCCESS;
	u_char	*loadbuf;

	/* prompt for the loadfile size */
	
	newint = 0;
	newprmpt = TRUE;

	LOOP {
		if (newprmpt) {
			outstr("\nEnter loadfile size<cr>: ");
			vallen = outint(newint);
			newprmpt = FALSE;
		}
		
		inchar = getinchar();
		
		if (inchar == CR) {
			if (newint == 0) {
				outstr("\nSorry, loadfile cannot be of size zero!\n");
				newprmpt = TRUE;
				OutChar(BEL);
				continue;
			} else
				break;
		}
			
		if (inchar == BS) {
			if (vallen == 0) {
				newprmpt = TRUE;
				OutChar(BEL);
				continue;
			}
			OutChar(' ');
			OutChar(BS);
			newint /= 10;
			vallen--;
			continue;
		}
		
		if (('0' <= inchar) && (inchar <= '9')) {
			if (newint & 0xf0000000) {
				newprmpt = TRUE;
				OutChar(BEL);
				continue;
			}
			newint *= 10;
			newint += inchar - '0';
			vallen++;
			continue;
		}
		newprmpt = TRUE;
		OutChar(BEL);
	}

	/* get the memory for the loadfile */	
	if ((error = getbootmem(newint)) != SUCCESS) {
		outstr("\nSorry, can't allocate memory for loadfile!\n");
		OutChar(BEL);
		return(error);
	}
	
	/* prompt for loadfile ready */
	do {
		outstr("\nIs the loadfile image ready at ");
		outhex((int) bootram);
		OutChar(':');
		
		if ((inchar = ask_ynq(TRUE)) == 'q') {
			return E_NOTRDY;
		}
			
	} while (inchar == 'n');

	bootsize = newint;
	return SUCCESS;
}

