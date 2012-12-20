/*****************************************************************************
** Power Manager Extensions entry points                                    **
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
**   1 06/14/95 creation                                          bat       **
**   5 12/21/95 ported to PwrMan (for 68328 board)                rmm       **
**   6 01/05/96 added in the subroutine module stuff              rmm       **
**   7 01/10/96 changed subroutine module interface to P2         rmm       **
**   8 01/15/96 split into 3 files & modules                      rmm       **
**   9 05/31/96 updated for PwrMan v1.0.Beta structures           bat       **
**  10 09/11/96 updated cast's for new sysglob structure          bat       **
**  11 01/12/97 added DEINIT routine for pwrman detach            ecm       **
**     01/17/97 <--------------- PwrMan v2.1 --------------->               **
*****************************************************************************/


/*
** Header Files
*/

#define _SYSEDIT
#include <defs.h>

/* Trying to do the following setup:
**      fpb   = parameter block containing subcode and version number
**      ldptr = Pwrman's global structure
*/

/*
** System-Call Entry Point
*/
error_code PwrExt_entry(F_pwrman_pb pb, Pwrman_globals ldptr)
{
	error_code err;
	Localdata extptr = ldptr->pwrext_mem;

	switch(pb->subcode) {

		/* Initialize pwrext module - srqmem() some memory for globals 
		** to be stored in.  Store Pwrman's entry point in this area to
		** be called as a default case.
		*/
		case PWREXT_INIT:
		{
			u_int32 size = sizeof(localdata);
			u_int32 color = 0;

			/* if we need more than 4 bytes for local "global" variables,
			** then we'll need to srqmem some memory for PwrExt use
			*/
			
			/* assume that the current value in entrycb is PwrMan's
			** entry point.  srqmem() some memory and grab it to use
			** as a default case for any calls not customized by PwrExt
			*/
			if ((err=_os_srqmem(&size,(void**)&extptr,color)) != SUCCESS) {
				return(err);
			}

			/* initialize the newly acquired memory & install ourselves
			** as the default PwrMan entry point, saving the previous
			** default in our globals
			*/
			extptr->pwrman_func = ldptr->pwrman_func;
			ldptr->pwrext_mem = extptr;
			ldptr->pwrman_func = PwrExt_entry;

			return(SUCCESS);

		} /* PWREXT_INIT */


		case PWREXT_DEINIT:
		{
			u_int32 size = sizeof(localdata);

			/* reinstall the saved pointer as the default PwrMan */
			/* entry point, and null the pwrext_mem pointer out */

			ldptr->pwrman_func = extptr->pwrman_func;
			ldptr->pwrext_mem = NULL;

			/* return memory to system */

			if ((err=_os_srtmem(size,(void*)extptr)) != SUCCESS) {
				return(err);
			}


			return(SUCCESS);

		} /* PWREXT_DEINIT */


#if 0
		/* This is where the customized change function will go.  I'm just
		** passing back a bogus error to make sure I hit the function.
		*/
		case PWRMAN_CHANGE:
		{
			return(999);

		} /* PWRMAN_CHANGE */
#endif
	
	} /* switch */



	/* if the call is not "switched" out, call PwrMan's entry point, */
	/* and have him deal with it */

	return((*extptr->pwrman_func)(pb, ldptr));
		
} /* PwrExt_entry */


