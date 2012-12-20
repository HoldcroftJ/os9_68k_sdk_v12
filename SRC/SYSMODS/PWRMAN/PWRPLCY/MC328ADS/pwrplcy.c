/*****************************************************************************
** Power Manager Policy subroutine module guts                              **
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
**   6 01/05/96 added in the subroutine module stuff              rmm       **
**   7 01/10/96 changed subroutine module interface to P2         rmm       **
**   8 01/15/96 split into 3 files & modules                      rmm       **
**   9 05/30/96 modified for PwrMan v1.0.Beta structures          bat       **
**  10 09/11/96 updated cast's for new sysglob structure          bat       **
**  11 01/18/97 modified INIT and added DEINIT to pwrplcy         ecm       **
**     01/17/97 <--------------- PwrMan v2.1 --------------->               **
**     04/29/99 cleaned up to remove compiler warnings            djc       **
*****************************************************************************/


/*
** Header Files
*/

#define _SYSEDIT
#include <defs.h>


/*
** Constants
*/

/* power state table initialization array */
const devreg_init pwrstates[] = {
	/*	SystemPowerState		ID		LocalPowerState				Prioirty */

	/* ON state */
	{	PWRPLCY_STATE_ON,		"gfx",	MC68328_PWR_LCD_ON,			1			},
	{	PWRPLCY_STATE_ON,		"cpu",	MC68328_PWR_CPU_ON,			0xffffffff	},

	/* IDLE state */
	{	PWRPLCY_STATE_IDLE,		"cpu",	MC68328_PWR_CPU_IDLE,		0xffffffff	},

	/* SUSPEND state */
	{	PWRPLCY_STATE_SUSPEND,	"gfx",	MC68328_PWR_LCD_OFF,		1			},
	{	PWRPLCY_STATE_SUSPEND,	"cpu",	MC68328_PWR_CPU_SUSPEND,	0xffffffff	},

	/* end-of-list */
	{0,0,0,0},
};


/*
** System-Call Entry Point
**
** Trying to do the following setup:
**      fpb   = parameter block containing subcode and version number
**      ldptr = Pwrman's global structure
*/
error_code PwrPlcy_entry(F_pwrman_pb pb, Pwrman_globals ldptr)
{
	error_code err;
	Localdata plcyptr = ldptr->pwrplcy_mem;

	switch(pb->subcode) {

		/* Initialize the power state table, set up system globals, and
		** go through power states
		 */
		case PWRPLCY_INIT:
		{
			status_reg oldirq;
			/* Devreg_init pwrstates_ptr; */

			/* get & initialize power policy globals */
			{
	            u_int32 size = sizeof(localdata);
	            u_int32 color = 0;

				/* allocate some memory for power policy use */
				if ((err=_os_srqmem(&size,(void**)&plcyptr,color)) != SUCCESS) {
					return(err);
				}

				/* initialize the newly acquired memory */
	            ldptr->pwrplcy_mem = plcyptr;
	        }

			/* load up the power state table */
			{
				int i;
				for (i=0; (pwrstates[i].id[0] != 0); i++) {
					if ((err = pwr_add(ldptr,
										(char*)pwrstates[i].id,
										pwrstates[i].syslevel,
										pwrstates[i].priority,
										pwrstates[i].devlevel,
										NULL)) != SUCCESS) {
						return(err);
					}
				}
			}
			
			/* change to our initial power state */
			if ((err = pwr_change(ldptr,START_STATE)) != SUCCESS) {
				return(err);
			}

			/* setup ourselves to be the kernel's idle function */
			oldirq = irq_save(); irq_disable();
			{
				/* save old, and set d_switches to 0 so we can see if
				** it increments indicating there`s active processes
				** in the system
				*/
				plcyptr->old_d_switches = ldptr->sglobptr->d_switches;
				ldptr->sglobptr->d_switches = 0;
			
				/* get the current tick count to see if we can go idle */
				plcyptr->d_ticks = ldptr->sglobptr->d_ticks;

				/* save old, replace kernel's idle function parameters with our own */
				plcyptr->old_d_idledata = ldptr->sglobptr->d_idledata;
				ldptr->sglobptr->d_idledata = ldptr;

				/* save old, andreplace kernel's idle function with our own */
				plcyptr->old_d_idle = ldptr->sglobptr->d_idle;
				ldptr->sglobptr->d_idle = idle;
			}
			irq_restore(oldirq);
		
			return(SUCCESS);

		} /* PWRPLCY_INIT */

		case PWRPLCY_DEINIT:
		{
			status_reg oldirq;
			/* Devreg_init pwrstates_ptr; */

			u_int32 size;

			/* do our best to remove the pwrplcy-loaded state table */
			{
				int i;
				for (i=0; (pwrstates[i].id[0] != 0); i++) {
					if ((err = pwr_remove(ldptr,
										(char*)pwrstates[i].id,
										pwrstates[i].syslevel,
										pwrstates[i].priority,
										pwrstates[i].devlevel,
										NULL)) != SUCCESS) {
						/* throw away errors */
						return(SUCCESS);
					}
				}
			}
			
			/* change to the startup power state */
			if ((err = pwr_change(ldptr, 0x00)) != SUCCESS) {
				return(err);
			}

			/* put the kernel's idle function back */
			oldirq = irq_save(); irq_disable();
			{
				/* put d_switches back */
				ldptr->sglobptr->d_switches = plcyptr->old_d_switches;
			
				/* put kernels idle function parameters back */
				ldptr->sglobptr->d_idledata = plcyptr->old_d_idledata;

				/* put kernel's idle function back */
				ldptr->sglobptr->d_idle = plcyptr->old_d_idle;
			}
			irq_restore(oldirq);

			{
				/* get size of memory */
				size = sizeof(localdata);

				/* deinitialize the pwrplcy memory */
		        ldptr->pwrplcy_mem = NULL;

				/* deallocate memory from power policy use */
				if ((err=_os_srtmem(size,(void*)plcyptr)) != SUCCESS) {
					return(err);
				}

			}
			return(SUCCESS);

		} /* PWRPLCY_DEINIT */

		/* go to the ON state */
		case PWRPLCY_STATE_ON:
		{
			/* change to ON state */
			return(pwr_change(ldptr,PWRPLCY_STATE_ON));

		} /* PWRPLCY_STATE_ON */

		/* go to the IDLE state */
		case PWRPLCY_STATE_IDLE:
		{
			/* change to IDLE state */
			return(pwr_change(ldptr,PWRPLCY_STATE_IDLE));

		} /* PWRPLCY_STATE_IDLE */

		/* go to the SUSPEND state */
		case PWRPLCY_STATE_SUSPEND:
		{
			/* change to SUSPEND state */
			return(pwr_change(ldptr,PWRPLCY_STATE_SUSPEND));

		} /* PWRPLCY_STATE_SUSPEND */

	/* if the call is not "switched" out, return unknown service code */
	} /* switch */

	return(EOS_UNKSVC);
	
} /* PwrPlcy_entry */


/* Own idle routine that will replace the kernel's idle routine.  Assume that
** the kernel has masked interrupts.
*/
void idle(void)
{
	Sysglobs sglobptr;
	Pwrman_globals ldptr;
	Localdata plcyptr;

	/* get global pointers */
	sglobptr = get_static();
	ldptr = (void*)(sglobptr->d_idledata);
	plcyptr = ldptr->pwrplcy_mem;
	
	/* check if the correct amount of time has gone by before going idle */	
	if (sglobptr->d_ticks >= (plcyptr->d_ticks+IDLE_DELAY)) {

		/* if d_switches is 0, we're going idle */
		if ((sglobptr->d_switches) == 0) {

			/* change to SUSPEND state
			** This change will place the CPU in a "freeze" mode; An 
			** interrupt or real-time clock alarm will wake it up
			** and finally return from this call; Thus, we need to
			** change back to ON after this call returns.
			*/
			(void)pwr_change(ldptr,PWRPLCY_STATE_SUSPEND);
			(void)pwr_change(ldptr,PWRPLCY_STATE_ON);
		}

		/* set d_switches to 0 so we can see if it increments
		** indicating there`s active processes in the system
		*/
		ldptr->sglobptr->d_switches = 0;
			
		/* get the current tick count to see if we can go idle */
		plcyptr->d_ticks = ldptr->sglobptr->d_ticks;
	}

	/* re-enable interrupts before leaving back to kernel */
	irq_enable();

	return;
}

/*
** Local "_os_pwr_add()" binding w/ direct access to PwrMan
** (eg, no system call)
*/
error_code pwr_add(Pwrman_globals ldptr, char id[PWR_IDLEN], pwr_level syslevel, u_int32 priority, pwr_level devlevel, void* devpb)
{
	f_pwrman_add_pb pb;

	/* set-up parameter block */
	pb.pwrcom.subcode = PWRMAN_STATEADD;
	pb.pwrcom.edition = PWR_PB_EDITION;
	pb.id = id;
	pb.syslevel = syslevel;
	pb.priority = priority;
	pb.devlevel = devlevel;
	pb.devpb = devpb;

	/* call PwrMan directly to perform "system call" */
	return((*ldptr->pwrman_func)((F_pwrman_pb)(&pb),ldptr));
}

/*
** Local "_os_pwr_remove()" binding w/ direct access to PwrMan
** (eg, no system call)
*/
error_code pwr_remove(Pwrman_globals ldptr, char id[PWR_IDLEN], pwr_level syslevel, u_int32 priority, pwr_level devlevel, void* devpb)
{
	f_pwrman_remove_pb pb;

	/* set-up parameter block */
	pb.pwrcom.subcode = PWRMAN_STATEREMOVE;
	pb.pwrcom.edition = PWR_PB_EDITION;
	pb.id = id;
	pb.syslevel = syslevel;
	pb.priority = priority;
	pb.devlevel = devlevel;
	pb.devpb = devpb;

	/* call PwrMan directly to perform "system call" */
	return((*ldptr->pwrman_func)((F_pwrman_pb)(&pb),ldptr));
}

/*
** Local "_os_pwr_change()" binding w/ direct access to PwrMan
** (eg, no system call)
*/
error_code pwr_change(Pwrman_globals ldptr, pwr_level syslev)
{
	f_pwrman_change_pb pb;

	/* set-up parameter block */
	pb.pwrcom.subcode = PWRMAN_CHANGE;
	pb.pwrcom.edition = PWR_PB_EDITION;
	pb.syslevel = syslev;

	/* call PwrMan directly to perform "system call" */
	return((*ldptr->pwrman_func)((F_pwrman_pb)(&pb),ldptr));
}
