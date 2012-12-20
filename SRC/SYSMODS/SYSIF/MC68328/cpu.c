/*****************************************************************************
** 328 System Module                                                        **
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
**   1 02/01/96 Creation                                          jgm       **
**   2 06/04/96 Cleaned up to ship out with the Beta release      jgm       **
*****************************************************************************/


/*
** Header Files
*/

#include <defs.h>

/*
**	Initialize the CPU
*/
error_code cpu_init(Localdata ldptr)
{
	char 			device[PWR_IDLEN] = "cpu";  /* name of the device to register */
	error_code   	err;			/* error code temp */
	Pwr_devcond		devpwrdef=NULL; /* Device Energy Condition Definition, NULL for this Example */

	/* initialize the hardware */
	ldptr->regs = REGBASE; /* Set the actual port address */

	/* Tell PwrMan to call us on power-down */
	err = _os_pwr_reg(device,dr_pwrman,ldptr,devpwrdef);
	if (err == SUCCESS){
		/* PwrMan in system and successful -- continue */
	}
	else if (err == EOS_UNKSVC) {
		/* no PwrMan in system -- continue */
		return(err);
	} else {
		/* PwrMan in system and unsuccessful -- return */
		 return(err);
	}
	return(SUCCESS);
}


/*
** De-Initialize the CPU
*/
error_code cpu_term(Localdata ldptr)
{
	char			device[PWR_IDLEN] = "cpu"; 	/* name of the device to register */
	Pwr_devcond		devpwrdef=NULL; /* Device Energy Condition Definition, NULL for this Example */
	
	/* Tell PwrMan to forget our previous registration */
	(void)_os_pwr_unreg(device,dr_pwrman,ldptr,devpwrdef);
	
	return(SUCCESS);
}

/*
** Function to turn the CPU to the ON state
*/
error_code cpu_on(Localdata ldptr)
{
	status_reg oldirq;
	oldirq = irq_save(); irq_disable(); /* mask irqs */
	{
		/* Add code here, if any */
		
		ldptr->regs->pll_regs.pctlr = PLL_DCTLR; /* Disable power control */
	}
	irq_restore(oldirq);	/* unmask irqs */

	return(SUCCESS); /* Return a success */
}


/*
** Function to put the CPU into the IDLE state
*/
error_code cpu_idle(Localdata ldptr)
{
	status_reg	oldirq;
	u_int16	state = MC68328_PWR_CPU_IDLE; /* cpu to idle mode */
	
	oldirq = irq_save(); irq_disable();     /* mask irqs */
	{
		ldptr->regs->irq_regs.imr &= ~IMR_RTC; /* mask the RTC bit to enable the interrupt */
		ldptr->regs->irq_regs.iwr = IWR_RTC; /* Enable ALL interrupts to wakeup cpu */
		ldptr->regs->rtc_regs.ctl  = RTC_ENABLE; /* make sure the rtclock is enabled */

		/* call common-loop handler */
		(void)common(ldptr,state);
	}
	irq_restore(oldirq); /* unmask irqs */

	ldptr->regs->irq_regs.imr |= IMR_RTC; /* disable the RTC interrupt */
	
	return(SUCCESS); /* Return a success */
}


/* We have these separated because you may want to enable/disable the interrupt	*/
/* mask and wakeup registers for different devices and/or interrupts for each  	*/
/* state you transition to.  For this particular state table, the idle and the	*/
/* suspend states are the same.  The only difference there is is to indicate	*/
/* what state you are transitioning to because one shuts off the PLL and one	*/
/* goes into a doze mode.  Each has to set different registers.					*/

/*
** Function to put the CPU into the SUSPEND state
*/
error_code cpu_suspend(Localdata ldptr)
{
	status_reg	oldirq;
	u_int16	state = MC68328_PWR_CPU_SUSPEND; /* cpu to suspend mode */

	oldirq = irq_save(); irq_disable();				/* mask irqs */
	{
		ldptr->regs->irq_regs.imr &= ~IMR_RTC; /* mask the RTC bit to enable the interrupt */
		ldptr->regs->irq_regs.iwr = IWR_RTC; /* enable ALL interrupts to wakeup cpu */
		ldptr->regs->rtc_regs.ctl = RTC_ENABLE; /* make sure the rtclock is enabled */

		/* call common loop handler */
		(void)common(ldptr,state);
	}
	irq_restore(oldirq);                    /* unmask irqs */

	ldptr->regs->irq_regs.imr |= IMR_RTC; /* disable the RTC interrupt */
	
	return(SUCCESS); /* Return a success */
}

/*
** Entry point for the call back function.  The void * parameter is for any
** extra parameter you may want PWRMAN to store and send back to the call
** back function.  For this example, we don't use this parameter.
*/
error_code dr_pwrman(void *ldptr, pwr_level pwrlvl, void *devpb)
{
	switch (pwrlvl){		/* switch to the level requested */

		/* cpu to power up(on) state */
		case MC68328_PWR_CPU_ON:
		{
			return(cpu_on((Localdata)ldptr));
		}

		/* cpu to idle state */
		case MC68328_PWR_CPU_IDLE:
		{
			return(cpu_idle((Localdata)ldptr));
		}

		/* cpu to suspend state */
		case MC68328_PWR_CPU_SUSPEND:
		{
			return(cpu_suspend((Localdata)ldptr));
		}
		
		/* cpu driver to terminate */
		case MC68328_PWR_CPU_TERM:
		{
			return(cpu_term((Localdata)ldptr));
		}
	}
	return(EOS_UNKSVC);
}

/*
**	CPU-Management Idle Routine
**
**	ASSUME: interrupts are masked up to maximum level before calling idle()!
*/
void common(Localdata ldptr, u_int16 state)
{
	u_int32 sec = ldptr->sysglob->d_elapse / ldptr->sysglob->d_tcksec; /* compute the # of seconds to sleep */
#ifdef REV2G15E
#else
	u_int32 wakeuptemp; /* temp variable for 1 minute interrupts */
#endif
	u_int32 active; /* temp variable to see if there are any active processes */
	status_reg srirq;

	/* check for minimum sleep period */
	if ((ldptr->sysglob->d_elapse == 0) || (sec > MINSLEEP)) {

		u_int16 save_pitr; /* turn the ticker on/off */
		status_reg save_irq;

		/* set real-time clock 1 minute interrupt for any timed sleeps */
		if (ldptr->sysglob->d_elapse > 0) {

			u_int32 min; /* temp variable for the minutes */
			u_int32 hour; /* temp variable for the hour of day */

			/* compute real-time clock wake-up time */
#ifdef REV2G15E
			sec -= (MINSLEEP - 1); /* have the processor wakeup a few seconds before actual time */
#else
			sec = (sec - 2); /* since we have to wakeup each minute, wakeup a ways before */
			ldptr->wakeupmins = sec / (60); /* how many times to do the 1 minute alarm */
#endif
			ldptr->hour_flag = 0; /* set hour flag to indicate no 24-hour interrupt has occurred */

			/* Some of this may look as if it is not needed, but there were some */
			/* hardware bugs we had to get a around in order to wakeup for a	 */
			/* sleep or a alarm for REV1G15E.									 */

			if ((hour = sec / (60 * 60)) >= 24) {
#ifdef REV2G15E
				/* sleep for a maximum of 24 hours */
				hour = 23;
				min = 59;
				sec = 59;
				/* start the real-time clock alarm interrupt */
				if (rtc_alarm_set(ldptr,sec,min,hour) != SUCCESS) {
					return; /* can't start RTC alarm interrupt */
				}
#else
				ldptr->wakeupmins = 1; /* Should only wakeup once */
				/* start the real-time clock 24 hour interrupt */
				if (rtc_hour_set(ldptr) != SUCCESS) {
					/* can't start 24 hour interrupt */
					return;
				}
#endif
			} else {
#ifdef REV2G15E
				sec -= (hour * (60 * 60)); /* calculate the number of seconds */
				min = sec / (60); /* and the number of minutes which we should */
				sec -= (min * (60)); /* be down for */
				/* start the real-time clock alarm interrupt */
				if (rtc_alarm_set(ldptr,sec,min,hour) != SUCCESS) {
					return; /* can't start RTC alarm interrupt */
				}
#else
				/* start the real-time clock 1 minute interrupt */
				if (rtc_min_set(ldptr) != SUCCESS){
					/* can't start RTC 1 minute interrupt */
					return;
				}
#endif
			}
		}

		/* turn off ticker */
		save_pitr = *SIM_PITR;
		*SIM_PITR = 0;

		/* get current time and save it in static storage */
		currentime(ldptr);

		/* save current irq mask */
		save_irq = irq_save();

		/* only power down for a maximum of 24 hours */
		/* With the 328 chip, this means to wakeup when the RTC clock turns 12:00 am */
		if (ldptr->sysglob->d_elapse == 0){
			/* wake up after 24 hours */
			if (rtc_hour_set(ldptr) != SUCCESS){
				/* can't start 24 hour interrupt */
				return;
			}
			/* call the check routine to see if there are any active processes */
			if ((active = check(ldptr)) == 0){
				HIGH(ldptr->regs->pll_regs.pllcr,SYSCLK); /* chip bug with "runt" problem */
				LOW(ldptr->regs->pll_regs.pllcr,SYSCLK_LOW); /* divide clock by 4; check the */
				if (state == MC68328_PWR_CPU_IDLE){						/* hardware Errata for description of bug */
					ldptr->regs->pll_regs.pctlr = PLL_PCTLR; /* go to a doze mode */
					_asm("  stop #$2000"); /* go into STOP with interrupt mask of 0 */
				} else {
					HIGH(ldptr->regs->pll_regs.pllcr,PORTBIT); /* shut down the PLL */
					_asm("  stop #$2000"); /* go into STOP with interrupt mask of 0 */
				}
			}
		} else {
#ifdef REV2G15E
			if ((active = check(ldptr)) == 0){
				HIGH(ldptr->regs->pll_regs.pllcr,SYSCLK); /* chip bug with "runt" problem */
				LOW(ldptr->regs->pll_regs.pllcr,SYSCLK_LOW); /* divide clock by 4; check the */
				if (state == MC68328_PWR_CPU_IDLE){						/* hardware Errata for description of bug */
					ldptr->regs->pll_regs.pctlr = PLL_PCTLR; /* go to a doze mode */
					_asm("  stop #$2000"); /* go into STOP with interrupt mask of 0 */
				} else {
					HIGH(ldptr->regs->pll_regs.pllcr,PORTBIT); /* shut down the PLL */
					_asm("  stop #$2000"); /* go into STOP with interrupt mask of 0 */
				}
			}
#else
			/* set up temporary variable to power up if any other interrupt besides */
			/* an RTC interrupt comes in. */
			wakeuptemp = 0;
		
			/* check to see if time has expired and it was an RTC interrupt that came in */
			while ((ldptr->wakeupmins != 0) && (ldptr->wakeupmins != wakeuptemp)){
				wakeuptemp = ldptr->wakeupmins; /* set to how many minutes are left */
				/* call the check routine to see if there are any active processes */
				if ((active = check(ldptr)) == 0){
					HIGH(ldptr->regs->pll_regs.pllcr,SYSCLK); /* chip bug with "runt" problem */
					LOW(ldptr->regs->pll_regs.pllcr,SYSCLK_LOW); /* divide clock by 4; check the */
					if (state == MC68328_PWR_CPU_IDLE){						/* hardware Errata for description of bug */
						ldptr->regs->pll_regs.pctlr = PLL_PCTLR; /* go to a doze mode */
						_asm("  stop #$2000"); /* go into STOP with interrupt mask of 0 */
					} else {
						HIGH(ldptr->regs->pll_regs.pllcr,PORTBIT); /* shut down the PLL */
						_asm("  stop #$2000"); /* go into STOP with interrupt mask of 0 */
					}
				}
			}
#endif
		}

		/* Because of hardware bug, need to set the system clock frequency back to normal */
		HIGH(ldptr->regs->pll_regs.pllcr,SYSCLK_LOW); /* set the clock back to normal */
		LOW(ldptr->regs->pll_regs.pllcr,SYSCLK); /* set the clock back to normal */
		
		/* woke up from STOP: restore irq mask */
		irq_restore(save_irq);

		/* turn off real-time clock interrupts */
		(void)rtc_stop(ldptr);
		
		/* calculate the time we were not at full power */
		downtime(ldptr);

		/* turn on ticker */
		*SIM_PITR = save_pitr;
	}
	return;
}


/* This section sets up the current time.  It also calculates the time we */
/* were down!!!!!														  */
void downtime(Localdata ldptr)
{
	/* local variables for the time and day and the number of ticks we were down */
	u_int32 sc, mn, hr;
	u_int32 date,ticks,ticks1,ticks2;
	u_int32 time, dummy, hmstemp;
	status_reg svirq;

#ifdef REV2G15E
/* This is for the updated REV2G15E silicon.  Here is some Errata on it.
*
* You will get an accurate reading any time you read the Hours, Minutes
* and Seconds. There is one gotcha though.  When the registers are read,
* two reads and a compare must be made.  This ensures that even if
* a 1 second "tick" occurs during the read or between hi word and
* low word reads, the value will still be correct.  The following code
* example shows how to read the registers.
*
*      TimeReg          equ     $FFFB00
*
*      GetTime:
*           move.w SR,-(SP)          ; save interrupt Status Register
*           move.w #$2700,SR     ; disable interrupts
*           move.l A0,-(SP)          ; save A0
*           lea TimeReg,A0          ; put $FFFB00 into A0
*           move.l (A0),D0          ; make the first read
*           cmp.l (A0),D0          ; make the second read and compare
*           beq.s DONE          ; compare good, exit
*           move.l (A0),D0          ; compare fails, make another read
*      DONE     movea.l (SP)+,A0     ; restore A0
*           move.w (SP)+,SR          ; restore Status Register
*           rts
*
* This example leaves the full 32-bit time value in D0.  If word reads or
* byte reads are needed, the same basic procedure should be followed.
*
*/
	svirq = irq_save(); irq_disable(); /* mask interrupts */
	{
		hmstemp = ldptr->regs->rtc_regs.hmsr; /* get the first read */
		if (hmstemp != ldptr->regs->rtc_regs.hmsr) /* compare second read */
		{
			hmstemp = ldptr->regs->rtc_regs.hmsr; /* if failed, then get another read */
		}
	}
	irq_restore(svirq); /* unmask interrupts */
	
	sc = (hmstemp & SECONDBITS); /* get the seconds value */
	mn = (hmstemp & MINUTEBITS); /* get the minutes value */
	mn = ((mn >> 16) & 0x0000003F); /* mask out the minute bits */
	hr = (hmstemp & HOURBITS); /* get the hours value */
	hr = ((hr >> 24) & 0x0000001F); /* mask out the hours bits */
	
#else

	volatile u_int32 i = 0;		/* need to keep this as volitile because */
								/* the compiler will optimize it at compile time */

	/* This is here to wait approx. 1 second because of a hardware bug         */
	/* We could not sleep or set an alarm because the ticker is not active     */
	/* yet at this point.                                                      */
	/* On Errata 1G58E for December 6, 1995, it states the Real Time Clock     */
	/* provides incorrect values when updates occur while the CPU clock is     */
	/* inactive (CPU sleeping).  Explanation:  The Real Time Clock keeps       */
	/* accurate time however, when updates occur (1 second "tick"), the read   */
	/* buffer register is updated only when the CPU clock is active (not sleep */
	/* or doze mode).  This means that if the clock is read only in response   */
	/* to a Real Time Clock wakeup event, the time value read will be          */
	/* incorrect.  Solution:  Utilize the 1 Minute or 1 Second interrupt to    */
	/* trigger time keeping functions in software.                             */
	
	while(i<200000){
		i++; /* delay for approx 1 second */
			 /* OPTIONAL, may want to use 1 second interrupt */
	}


	/* This next section is to calculate the time we were not at full power */
	/* and to generate the updated time(current time).  We get the hour,min,*/
	/* and sec from the RTC hardware.  This will give me the current time.  */
	/* We then take the time we kept in the globals, which is the time we   */
	/* stored before we went to a lower power state, and calculate the time */
	/* we were not at full power and update it to the current time.			*/

	svirq = irq_save(); irq_disable(); /* mask interrupts */
	{
		hmstemp = ldptr->regs->rtc_regs.hmsr; /* get the first read */
		if (hmstemp != ldptr->regs->rtc_regs.hmsr) /* compare second read */
		{
			hmstemp = ldptr->regs->rtc_regs.hmsr; /* if failed, then get another read */
		}
	}
	irq_restore(svirq); /* unmask interrupts */

	/* get the current time */
	sc = (hmstemp & SECONDBITS); /* get the seconds value */
	mn = (hmstemp & MINUTEBITS); /* get the minutes value */
	mn = ((mn >> 16) & 0x0000003F); /* mask out the minute bits */
	hr = (hmstemp & HOURBITS); /* get the hours value */
	hr = ((hr >> 24) & 0x0000001F); /* mask out the hours bits */
	                                
#endif

	/* compute the tick count before we were down */
	ticks1 = (ldptr->hour * 3600);
	ticks1 += (ldptr->min * 60);
	ticks1 += ldptr->sec;

	/* compute the tick count currently */
	ticks2 = (hr * 3600);
	ticks2 += (mn * 60);
	ticks2 += sc;

	/* force OS sleep/alarm recomputation */
	ldptr->sysglob->d_elapse = 1;

	/* set up the time for a hex format */
	time = (sc | (mn<<8) | (hr<<16) | (0<<24));

	/* Check to see if the 24 Hour interrupt flag was set */
	if (ldptr->hour_flag == 1) {
		ticks = (MAXSEC) - ticks1; /* subtract max # of secs by the tick count before sleep */
		ticks *= ldptr->sysglob->d_tcksec; /* take the tick count times the tick/sec rate */

		/* update system global fields */
		ldptr->sysglob->d_ticks += ticks;
		ldptr->sysglob->d_slice = 0;
                
		date = ldptr->date; /* get the date from static storage */
		date++; /* increment the date by one because it is midnight */
		(void)_os_gregorian(&dummy,&date); /* change the date from Julian to Gregorian */

		/* set the clock to the appropriate values */
		(void)_os9_setime(time,date);

		/* set the hour flag back to 0 */
		ldptr->hour_flag = 0;

		return;
	} else {
		ticks = ticks2 - ticks1; /* subtract tick counts from each other */
		ticks *= ldptr->sysglob->d_tcksec; /* take the tick count times the tick/sec rate */

		/* update system global fields */
		ldptr->sysglob->d_ticks += ticks;
		ldptr->sysglob->d_slice = 0;
		
		/* get the date from static storage */
		date = ldptr->date;
		(void)_os_gregorian(&dummy,&date); /* change the date from Julian to Gregorian */
		
		/* set the clock to the appropriate values */
		(void)_os9_setime(time,date);

		return;
	}
}

/*
** This function just stores the time before we power down to a state other than
** full power!!!!!!!
*/
void currentime(Localdata ldptr)
{
	/* local variables for the time and day and the number of ticks we were down */
	u_int32 sc, mn, hr;
	u_int32 date;
	u_int32 time,ticks;
	u_int16 day;

	/* get current time */
	sc = (ldptr->regs->rtc_regs.hmsr & SECONDBITS);
	mn = (ldptr->regs->rtc_regs.hmsr & MINUTEBITS); /* get the minute value */
	mn = ((mn >> 16) & 0x0000003F);
	hr = (ldptr->regs->rtc_regs.hmsr & HOURBITS); /* get the hour value */
	hr = ((hr >> 24) & 0x0000001F);

	/* save the real-time in static storage */
	ldptr->sec = sc;
	ldptr->min = mn;
	ldptr->hour = hr;

	/* get the system date before we go to a power down mode */
	(void)_os9_getime(1,&time,&date,&day,&ticks);
	ldptr->date = date; /* save the date in Static storage */

	return;
}

/*
** Check to see if there are any active processes pending
*/
u_int32 check(Localdata ldptr)
{
	Sysglobs sysglob = ldptr->sysglob;
	procid* aproc = FAKEHD(procid*,sysglob->d_activq[0],_queueN);
	status_reg oldirq;

	oldirq = irq_save(); irq_disable();      /* mask irqs */
	{
		/* check for empty active process queue (head pointing to itself) */
		if (aproc->_queueN == aproc) {
			irq_restore(oldirq);
			return(0); /* Go ahead and power down because there are no active process */
		}
	}
	irq_restore(oldirq);   /* unmask irqs */
	return(1); /* Can't power down because there is an active process */
}

/***********************************************************/
