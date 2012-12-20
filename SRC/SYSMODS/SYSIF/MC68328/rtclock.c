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
**   1 10/02/95 Creation                                          bat       **
**   2 02/23/96 Added to use with the AD328 board                 jgm       **
**   3 06/05/96 Cleaned up to ship out with the Beta release      jgm       **
*****************************************************************************/
 
/*
**	Header Files
*/

#include <defs.h>

/*
**	Initialize Real-Time Clock Sub-System
*/
error_code rtc_init(Localdata ldptr)
{
	error_code err;

	/* set up rtc isr */
	if ((err = _os9_irq(RTC_IRQ_VECTOR,RTC_IRQ_PRIORITY,
		rtc_isr_glue,ldptr,RTCLOCK)) != SUCCESS) {
		return(err);
	}

	/* make sure everything is cleared */
	(void)rtc_stop(ldptr);

	return(SUCCESS);
}


/*
**	De-Initialize Real-Time Clock Sub-System
*/
error_code rtc_term(Localdata ldptr)
{
	/* make sure everything is cleared */
	(void)rtc_stop(ldptr);

	/* remove rtc isr */
	(void)_os9_irq(RTC_IRQ_VECTOR,RTC_IRQ_PRIORITY,NULL,ldptr,RTCLOCK);

	return(SUCCESS);
}

#ifdef REV2G15E
/*
** Set Real-Time Clock Alarm Interrupt
*/
error_code rtc_alarm_set(Localdata ldptr,u_int32 sec,u_int32 min,u_int32 hour)
{
	/* make sure alarms are off */
	(void)rtc_stop(ldptr);

	/* get the current time from the hardware */
	(void)currentime(ldptr);
	
	if ((sec = (ldptr->sec + sec)) > 59) {
		min++;
		sec = (sec % 60);
	}
	if ((min = (ldptr->min + min)) > 59) {
		hour++;
		min = (min % 60);
	}
	if ((hour = (ldptr->hour + hour)) > 23) {
		hour = (hour % 24);
	}

	/* Go ahead and set the 24-Hour interrupt also because we want to wake */
	/* up at 12:00:00 a.m. every day say we can update the date for the OS.*/
	if (rtc_hour_set(ldptr) != SUCCESS) {
		/* could not set the 24 hour alarm interrupt */
		return(1);
	}
	
	/* set the time to wake up in the alarm register */
	ldptr->regs->rtc_regs.alarm = (((0x0000001F & hour) << 24) |
		((0x0000003F & min) << 16) | ((0x0000003F & sec) << 0));

	/* Turn on rtclock alarm interrupt */
	HIGH(ldptr->regs->rtc_regs.ienr,ABIT);

	return(SUCCESS);	    
}

#else

/*
**	Set Real-Time Clock 1 Minute Interrupt
*/
error_code rtc_min_set(Localdata ldptr)
{
	status_reg oldsr;

	/* make sure the interrupts are shut off */
	(void)rtc_stop(ldptr);

	/* turn on rtclock interrupts */
	oldsr = irq_save();irq_disable(); /* mask interrupts */
	{ 
		if ((ldptr->regs->rtc_regs.isr & MIN_FLAG) != 0){
			ldptr->regs->rtc_regs.isr |= MIN_FLAG; /* must clear this bit by writing */
		}										  /* 1's to clear the interrupt */
		HIGH(ldptr->regs->rtc_regs.ienr,MBIT); /* enable the minute flag to cause */
											   /* an interrupt */
		/* Always set the 24 Hour flag */									   
		if ((ldptr->regs->rtc_regs.isr & HOUR_FLAG) != 0) { /* clear the 24 hour bit flag */
			ldptr->regs->rtc_regs.isr |= HOUR_FLAG; /* if there is a need to */
		}
		HIGH(ldptr->regs->rtc_regs.ienr,HBIT); /* enable the 24 hour bit to generate an */
											   /* interrupt */
	}
	irq_restore(oldsr); /* unmask interrupts */
	
	return(SUCCESS);
}
#endif


/*
**      Set Real-Time Clock 24-Hour Interrupt
*/
error_code rtc_hour_set(Localdata ldptr)
{
	status_reg oldsr;

	/* make sure all interrupts are shut off */
	(void)rtc_stop(ldptr);

	/* turn on rtclock hour interrupt */
	oldsr = irq_save();irq_disable(); /* mask interrupts */
	{
		/* clear the 24 hour flag if it is set */
		if ((ldptr->regs->rtc_regs.isr & HOUR_FLAG) != 0){
			ldptr->regs->rtc_regs.isr |= HOUR_FLAG; /* must clear this bit with a 1 */

			/* Go ahead and clear the MIN flag */
			/* clear the minute flag if it is set */
			if ((ldptr->regs->rtc_regs.isr & MIN_FLAG) != 0){
				ldptr->regs->rtc_regs.isr |= MIN_FLAG;
			}
		}
		HIGH(ldptr->regs->rtc_regs.ienr,HBIT); /* enable the hour flag to cause an */
											   /* interrupt every 24 hours */
	}
	irq_restore(oldsr); /* unmask interrupts */

	return(SUCCESS);
}


/*
**	Stop Real-Time Clock 1 Minute Interrupt
*/
error_code rtc_stop(Localdata ldptr)
{
	status_reg oldsr;
	
	/* turn off interrupts */
	oldsr = irq_save();irq_disable(); /* mask interrupts */
	{
#ifdef REV2G15E
		if ((ldptr->regs->rtc_regs.isr & ALM_FLAG) != 0){
			/* Must clear this bit by writing 1's to clear the interrupt */
			ldptr->regs->rtc_regs.isr |= ALM_FLAG;
			ldptr->regs->rtc_regs.alarm = 0x00000000; /* clear the alarm register field also */
		}
#else
		if ((ldptr->regs->rtc_regs.isr & MIN_FLAG) != 0){
			/* Must clear this bit by writing 1's to clear the interrupt */
			ldptr->regs->rtc_regs.isr |= MIN_FLAG;
		}
#endif
		if ((ldptr->regs->rtc_regs.isr & HOUR_FLAG) != 0){
			/* Must clear this bit by writing 1's to clear the interrupt */
			ldptr->regs->rtc_regs.isr |= HOUR_FLAG;
		}
		ldptr->regs->rtc_regs.ienr = 0x0000; /* clear all interrupts for the RTC */
	}
	irq_restore(oldsr); /* unmask interrupts */
	return(SUCCESS);
}


/*
**	Real-Time Clock Interrupt Service Routine
*/
_asm("
rtc_isr_glue:
	move.l 	a2,d0				; put ldptr in a2 into d1
	bsr	 	rtc_isr				; call C interrupt service routine
	tst.l   d0					; see if SUCCESS returned
	beq.s   rtc_isr_exit		; if so, return
	ori	 	#Carry,ccr			; else E_NOTME returned--set carry bit
rtc_isr_exit
	rts
");


/*
** Real-Time Clock Interrupt Service Routine
*/
error_code rtc_isr(Localdata ldptr)
{
	status_reg oldsr;

#ifdef REV2G15E
	/* is this us? */
	/* Check to see if this is an 24 hour interrupt or an alarm interrupt */
	if (((ldptr->regs->rtc_regs.isr & ALM_FLAG) == 0) && ((ldptr->regs->rtc_regs.isr & HOUR_FLAG) == 0)) {
		return(EOS_NOTME); /* this is not a RTC interrupt */
	}

	/* Check to see if the 24 hour interrupt went off or the alarm interrupt */
	if ((ldptr->regs->rtc_regs.isr & HOUR_FLAG) != 0) {
		/* woke up from the 24 hour interrupt */
		ldptr->hour_flag = 1; /* set the 24 hour flag */
		rtc_stop(ldptr); /* clear any pending interrupts */
		return(SUCCESS);
	} else {
		/* turn off alarm */
		rtc_stop(ldptr);
		return(SUCCESS);
	}
#else
	/* is this us? */
	/* Check to see if this is an 24 hour interrupt or a minute interrupt */
	if (((ldptr->regs->rtc_regs.isr & MIN_FLAG) == 0) && ((ldptr->regs->rtc_regs.isr & HOUR_FLAG) == 0)) {
		return(EOS_NOTME); /* this is not a RTC interrupt */
	}

	/* Check to see if the 24 hour interrupt went off or the 1 minute interrupt */
	/* Have to check both because when the 24 hour interrupt goes off it sets   */
	/* both the minute status bit and the 24 hour status bit.  When a minute    */
	/* interrupt goes off, it only sets the minute status bit.                  */
	
	if (((ldptr->regs->rtc_regs.isr & MIN_FLAG) != 0) && ((ldptr->regs->rtc_regs.isr & HOUR_FLAG) != 0)) {
		/* woke up from the 24 hour interrupt */
		ldptr->wakeupmins = 0; /* set to 0 to get out of while loop */
		ldptr->hour_flag = 1; /* set the 24 hour flag */
		rtc_stop(ldptr); /* clear any pending interrupts */
		return(SUCCESS);
	} else {                                        
		/* decrement the count by one because we just wokeup from an 1 minute interrupt */
		ldptr->wakeupmins--;

		/* check to see if we have interrupted the number of specified minutes or not */
		if (ldptr->wakeupmins == 0){
			rtc_stop(ldptr);
			return(SUCCESS);
		} else {
			/* turn on the 1 minute interrupt */
			oldsr = irq_save();irq_disable(); /* mask interrupts */
			{
				ldptr->regs->rtc_regs.isr |= MIN_FLAG; /* must clear the interrupt status register */
				HIGH(ldptr->regs->rtc_regs.ienr,MBIT); /* enable 1 minute RTC interrupt */
			}
			irq_restore(oldsr); /* unmask interrupts */
			return(SUCCESS);
		}
	}
#endif
}
