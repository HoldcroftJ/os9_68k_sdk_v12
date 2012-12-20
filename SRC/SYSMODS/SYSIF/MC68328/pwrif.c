/*****************************************************************************
** 328ADS System Module                                                     **
******************************************************************************
** Copyright 1995 by Microware Systems Corporation                          **
** Reproduced Under License                                                 **
**                                                                          **
** This source code is the proprietary confidential property of             **
** Microware Systems Corporation, and is provided to licensee               **
** solely for documentation and educational purposes. Reproduction,         **
** publication, or distribution in any form to any party other than         **
** the licensee is strictly prohibited.                                     **
******************************************************************************
** Edition History:                                                         **
** #   Date     Comments                                          By        **
** --- -------- ------------------------------------------------- --------- **
**   1 06/14/95 Creation                                          bat       **
**   2 12/07/95 Ported from WhirlWind-1 to Emerald SysIF          bat       **
**   3 12/21/95 Ported to PwrMan (for 68328 board)                rmm       **
**   4 01/10/96 Changed subroutine module interface to P2         rmm       **
**   5 01/15/96 Split into 3 files & modules                      rmm       **
**   6 03/28/96 Ported to the ADS328 boards                       jgm       **
**   7 06/04/96 Cleaned up to ship out with the Beta release      jgm       **
*****************************************************************************/


/*
** Header Files
*/

#include <defs.h>
#include <PWRMAN/pwrman.h>

/*
** Defines
*/

#define _SYSEDIT

/* set up static storage variables */
localdata	local_data;
_asm("_mshare: equ local_data");
Localdata 	ldptr;

/*
** Initialization Function
*/
error_code pwrif_init(Sysglobs sysglob)
{
	status_reg oldirq;
	error_code err;

	/* initialize cpu(SYSIF) global variables */
	ldptr = &local_data;
	ldptr->sysglob = sysglob;
	ldptr->regs = REGBASE;
	ldptr->sec = 0;
	ldptr->min = 0;
	ldptr->hour = 0;
	ldptr->hour_flag = 0;
	ldptr->date = 0;
#ifdef REV2G15E
#else
	ldptr->wakeupmins = 0; /* this is here because of a hardware bug with alarms */
#endif

	/* initialize sysif sub-blocks */
	if ((err = cpu_init(ldptr)) != SUCCESS) {
		return(err);
	}
	if ((err = rtc_init(ldptr)) != SUCCESS) {
		cpu_term(ldptr);
		return(err);
	}
	
	return(SUCCESS);
}
