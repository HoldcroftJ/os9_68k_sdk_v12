/*****************************************************************************
** ID:     @(#) irqs.c 1.7@(#)
** Date:   7/11/96
******************************************************************************
**  MC68328ADS touchpanel SCF driver interrupt service routine              **
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
**	 1 09/13/95 creation                                      rws       **
**   2 04/30/96 modified for use with one-shot alarms             rws       **
**   3 05/17/99 update to remove compiler warnings                djc       **
*****************************************************************************/

/* 
 * can't have static shortcuts here since alarm handler
 * does not support having lst passed in a6
 */
#define NO_STATICS_SHORTCUTS 
#include "touchpad.h"

#if !defined (EOS_NOTME)
#define EOS_NOTME 0xb0
#endif

typedef enum coorenum {
	X_COOR,
	Y_COOR
} coor_enum;

#if !defined (USE_CYCLIC_ALM)
/* currently have problem of target system (MC68328ADS) locking up
 * when using a cyclic alarm */
void install_one_shot (TStatics lst, Sysglobs globals);
#endif


/*
 * ISR Assembly Binding
 * 
 *    Passed:  (a2) = static storage ptr
 *             (a3) = port address
 *             (a6) = system global data ptr
 *
 *    Returns:  nothing
 *
 *	  Error Return: cc - carry bit set for interrupt not serviced
 *
 * C Driver Conventions:
 *
 *    Call:     error_code real_isr (TStatics lst, Sysglobs globals)
 *					(lst = static storage ptr,
 *				 	 globals = system global data ptr)
 *
 *    Returns:  d0.l = 0 (no error), else Error Code
 * 
 */
_asm("
pen_isr:
    move.l  a2,d0			; pass TStatics pointer
    move.l	a6,d1			; pass SysGlobs pointer
    bsr     real_isr		; call C interrupt service routine
    tst.l   d0				; see if SUCCESS returned
    beq.s   isr_exit		; if so, return
    ori     #Carry,ccr		; else E_NOTME returned--set carry bit
isr_exit
    rts
");


/*
 * MC68328ADS Touchpanel Interrupt Service Routine:
 *
 * This ISR is gotten to by a falling edge on the PENIRQn line that
 * the touchpanel is attached to.
 *
 * The ISR first turns the PENIRQn interrupt OFF and clears its status,
 * then it installs an alarm.
 * The alarm handler is responsible for filtering touchpanel
 * data, thus validating a touch, signalling an interested process, and
 * formatting Pointer Down (for an initial touch), Pointer Move (for
 * subsequent pointer movement), and Pointer Up (for when the stylus is
 * released from the touchpanel) data packets.  Upon Pointer Up detection
 * the alarm handler will re-enable the PENIRQn interrupt.
 *
 *		Call with:
 *			Pointer to static storage area
 *			Pointer to system globals
 *		Return:
 *			Error Code
 */
error_code real_isr 
(
	TStatics lst, 
	Sysglobs globals
)
{
	error_code error;				/* error holder (return value) */
	m68328_regs *port = lst->port;	/* quick access to MC68328 registers */

	/* pointer to process descriptor used to set the alarm */
	Pr_desc this_proc;  			/* ptr to this process (iniz) */
	REGISTERS regs;					/* used for installation of alarm */
	void alarm_h (TStatics lst, Sysglobs globals);	/* alarm handler below */

	/* see if interrupt was caused by PENIRQ */
	if ((port->irq_regs.isr & SPEN) == 0)
		return (EOS_NOTME);

	/* 
	 * turn touchpanel interrupt OFF and clear IRQ status 
	 */
	port->irq_regs.imr |= MASKPEN;				/* PENIRQn masked */
	port->pio_regs.portm.sel |= ~PENIRQ_SEL;	/* PENIRQn -> GPIO input */
	port->pio_regs.portm.puen &= ~PENPU_EN;		/* disable PM6 pullup res */
	port->irq_regs.isr |= MASKPEN;				/* PENIRQ status cleared */

	/* Initialize Master Serial Peripheral Interface module --
	 * will enable SPIM interrupt, but will not unmask SPIM IRQ in sim_regs.imr,
	 * but will poll SPIM_IRQST bit to determine when exchange has been
	 * completed
	 */
	port->spim_regs.cont =
		(DIV_SYSCLK_8 << b_DATA_RATE) +		/* baud = SYSCLK divided by 8 */
		 SPIMEN +							/* SPIM enabled */
		 SPIM_IRQEN +						/* SPIM IRQ enabled */
		(BIT_COUNT << b_SPIM_BITCNT);		/* load bit count */

	/*
	 * Install alarm to poll touchpanel:
	 *
	 * Note: regs is used to pass certain registers to alarm
	 * handler: in alarm_h(), d0-d7, a0-a3, and pc
	 * will be set to what regs.d[0]-d[7],a[0]-a[3],pc are
	 * set to here.  Consequently, it is not possible to set
	 * up a C interface by setting regs.a[6] to lst, so must 
	 * pass lst as a parameter to alarm_h().
	 *
	 *  regs.pc = address of alarm handler
	 *	regs.d[0] = address of static storage
	 *	regs.d[1] = address of system globals (for time stamp info)
	 *
	 * call to alarm_h() is set up for:
	 *
	 *		void alarm_h (TStatics lst, Sysglobs globals)
	 */
	regs.pc = (u_int32) alarm_h;
	regs.d[0] = (u_int32) lst;
	regs.d[1] = (u_int32) globals;

	/* Before installing the alarm it is necessary to
	 * switch the process context to that of a system process
	 * so that when "iniz tpad" is finished, the alarm doesn't
	 * get deleted with it (from OS-9 Technical Manual description
	 * of F$Alarm: "The system automatically deletes a process'
	 * pending alarms when the process dies.").  Associating
	 * the alarm with the OS insures the alarm will not get deleted
	 * until term() does so.
	 */
	this_proc = globals->d_proc;
	globals->d_proc = globals->d_sysprc;

#if defined (USE_CYCLIC_ALM)
	error = _os9_salarm_cycle (&lst->cyc_alm_ident, lst->cyc_alm_period, 
		&regs);
#else
	error = _os9_salarm_set (&lst->cyc_alm_ident, lst->cyc_alm_period, 
		&regs);
#endif

	/* change back to original process */
	globals->d_proc = this_proc;
	
	return (error);
}

#if !defined (USE_CYCLIC_ALM)
/*
 * Notice absence of error checking
 */
void install_one_shot (TStatics lst, Sysglobs globals)
{
	/* pointer to process descriptor used to set the alarm */
	Pr_desc this_proc;  			/* ptr to this process (iniz) */
	REGISTERS regs;					/* used for installation of alarm */
	void alarm_h (TStatics lst, Sysglobs globals);	/* alarm handler below */

	/*
	 * Install one-shot alarm to poll touchpanel:
	 *
	 * Note: regs is used to pass certain registers to alarm
	 * handler: in alarm_h(), d0-d7, a0-a3, and pc
	 * will be set to what regs.d[0]-d[7],a[0]-a[3],pc are
	 * set to here.  Consequently, it is not possible to set
	 * up a C interface by setting regs.a[6] to lst, so must 
	 * pass lst as a parameter to alarm_h().
	 *
	 *  regs.pc = address of alarm handler
	 *	regs.d[0] = address of static storage
	 *	regs.d[1] = address of system globals (for time stamp info)
	 *
	 * call to alarm_h() is set up for:
	 *
	 *		void alarm_h (TStatics lst, Sysglobs globals)
	 */
	regs.pc = (u_int32) alarm_h;
	regs.d[0] = (u_int32) lst;
	regs.d[1] = (u_int32) globals;

	/* Before installing the alarm it is necessary to
	 * switch the process context to that of a system process
	 * so that when "iniz tpad" is finished, the alarm doesn't
	 * get deleted with it (from OS-9 Technical Manual description
	 * of F$Alarm: "The system automatically deletes a process'
	 * pending alarms when the process dies.").  Associating
	 * the alarm with the OS insures the alarm will not get deleted
	 * until term() does so.
	 */
	this_proc = globals->d_proc;
	globals->d_proc = globals->d_sysprc;

	_os9_salarm_set (&lst->cyc_alm_ident, lst->cyc_alm_period, &regs);

	/* change back to original process */
	globals->d_proc = this_proc;

	return;
}

#endif /* !USE_CYCLIC_ALM */


/*
 * Touchpanel alarm handler
 *
 * This function is responsible for reading the touchpanel, filtering its
 * data / validating the touched point, determining if a Pointer Down,
 * Pointer Move, or Pointer Up data packet is to be formed, putting the
 * data into a buffer that the read() function outputs, and sending a
 * signal to a process when data is ready to be read.
 *
 *		Call with:
 *			Pointer to static storage area
 *			Pointer to system globals
 *		Return:
 *			Error Code
 */
void alarm_h
(
	TStatics lst,
	Sysglobs globals
)
{
	int loop_var;			/* loop variable */
	int temp;				/* used for temporary needs */
	/* int send_pd_sig = NO; */	/* send Pointer Down signal flag */
	/* int send_pm_sig = NO; */	/* send Pointer Move signal flag */
	int send_pu_sig = NO;	/* send Pointer Up signal flag */

	/* error_code ec; */

	/* variables used for calibration */
	u_short X_min_old, Y_min_old, X_max_old, Y_max_old;
	u_int32 delta_x_old, delta_y_old;	
	void calibrate (TStatics lst);	/* function below */

	u_char get_coor (coor_enum coor, TStatics lst);	/* function below */
	
	/* quick exit: previous coordinates have not yet been taken */
	if (lst->bytes_avail != 0)
/* && lst->datapid == 0 && 
		lst->scfstat.v_sysio.v_wake == 0)
*/
	{
#if !defined (USE_CYCLIC_ALM)
		install_one_shot (lst, globals);
#endif
		return;
	}

	/*
	 * Filter Data / Validate Touched Point
	 *
	 * Filter Algorithm:
	 *
	 * x_buff[] and y_buff[] are filled with consecutive samples and then a
	 * "near-neighbor" relationship is sought for all points; if there is a
	 * value within x_buff[] or y_buff[] which exceeds a certain threshold
	 * this function will just return, i.e.
	 *
	 * if all x_buff[] AND all y_buff[] values are NOT NO_TOUCH AND
	 *    all x_buff[] values are the same (+/- TOLERANCE) AND
	 *    all y_buff[] values are the same (+/- TOLERANCE),
	 * then a valid touch is assumed.
	 *
	 * If this is the first time the alarm handler has been called and
	 * a valid touch has been determined, a Pointer Down data packet is
	 * formed and a signal will be sent to the interested process.
	 *
	 * If this is NOT the first time the alarm handler has been
	 * called and a valid touch has been determined then a Proximity Check
	 * is performed on the new touched point.  If the new point is TOLERANCE
	 * pixels away from the previous point then a Pointer Move data packet is
	 * formed and a signal will be sent to the interested process, otherwise
	 * the alarm handler will just return.
	 *
	 * If all x_buff[] AND all y_buff[] values are NO_TOUCH then a Pointer Up
	 * data packet is formed and a signal will be sent to the interested
	 * process.  Afterwards, the alarm handler will remove itself and
	 * re-enable the PENIRQn interrupt.
	 */

	/* read sample_per_point points into buffer and check for Pointer Up action */
	send_pu_sig = YES;	/* assume this for now */
	for (loop_var = 0; loop_var < lst->samples_per_point; loop_var++)
	{
		lst->x_buff[loop_var] = get_coor(X_COOR,lst); 
		lst->y_buff[loop_var] = get_coor(Y_COOR,lst); 

        if (lst->x_buff[loop_var] != NO_TOUCH ||
        	lst->y_buff[loop_var] != NO_TOUCH)
        {
        	send_pu_sig = NO;	/* send_pu_sig proven wrong */
        }
	}

	/*
	 * stylus has been lifted up - signal Pointer Up
	 */
	if (send_pu_sig == YES)
	{
		/* remove alarm */
		_os_salarm_delete (lst->cyc_alm_ident);

		/*
		 * spurious interrupt --
		 * a pointer up action is seen before a pointer down action
		 */
		if (lst->pointer_down_flag == TRUE)
			goto exit;
		
		/*
		 * previous coordinates have not yet been taken, 
		 * the new coordinates are lost,
		 * indicate "input buffer overrun" error to read()
		 */
		if (lst->bytes_avail != 0)
		{
			lst->scfstat.v_err = INP_BUF_OVERRUN;
		}
		/* coordinate buffer empty, put new coordinates in buffer */
		else 
		{
			/* form Pointer Up data packet */
			lst->pktbuf[0] = PKTBUF_PU_BYTE0;
			lst->pktbuf[1] = ((lst->last_y & 0x0380) >> 4) |
							 ((lst->last_x & 0x0380) >> 7);
			lst->pktbuf[2] = lst->last_x & 0x007F;
			lst->pktbuf[3] = lst->last_y & 0x007F;

#if defined (TIME_STAMP)
			lst->pktbuf[4] = (globals->d_ticks & 0xF0000000) >> 28;
			lst->pktbuf[5] = (globals->d_ticks & 0x0FE00000) >> 21;
			lst->pktbuf[6] = (globals->d_ticks & 0x001FC000) >> 14;
			lst->pktbuf[7] = (globals->d_ticks & 0x00003F80) >> 7;
			lst->pktbuf[8] = (globals->d_ticks & 0x0000007F);
#endif
			lst->bytes_avail = PKTBUFSIZ;
		}

		/* send wakeup signal to any sleeping process (like read()) */
		if (lst->scfstat.v_sysio.v_wake != 0)
		{
			_os_send (lst->scfstat.v_sysio.v_wake, SIGWAKE);
			lst->scfstat.v_sysio.v_wake = 0;
		}

		/* send signal to interested process */
		if (lst->datapid != 0)
		{
			if (_os_send (lst->datapid, lst->datasig) != SUCCESS)
			{
				lst->bytes_avail = 0;
			}
				
			lst->datapid = 0;	/* clear down signal condition */
			lst->datasig = 0;
			lst->dataid = 0;
		}

		/* re-initialize static storage */
		lst->pointer_down_flag = TRUE;

exit:
		/* stand-down SPIM:
		 * disable SPIM and disable SPIM's IRQ
		 */
		lst->port->spim_regs.cont &= ~(SPIMEN + SPIM_IRQEN);
		/* clear any IRQ status */
		lst->port->spim_regs.cont |= SPIM_IRQST;	

		/*
		 * turn touchpanel interrupt back ON
		 */
		lst->port->pio_regs.portj.data = PJIRQWAIT;	/* config Port J */
		lst->port->pio_regs.portj.dir = PJDIROUT;		/* for touchpanel */
		lst->port->pio_regs.portj.sel |= PJSELPORT;	/* usage (see init.c) */
		lst->port->pio_regs.portm.sel &= PENIRQ_SEL;	/* config for PENIRQn */
		lst->port->pio_regs.portm.puen |= PENPU_EN;	/* enable PU resistor */
		lst->port->irq_regs.imr &= ~MASKPEN;			/* unmask PENIRQn */
        
		return;
	}

	/* at this point the stylus is on the touchpanel */

	/* filter data */
	for (loop_var = 0; loop_var < lst->samples_per_point; loop_var++)
	{
		/* return if touchpanel is still bouncing */
		if (lst->x_buff[loop_var] == NO_TOUCH ||
			lst->y_buff[loop_var] == NO_TOUCH)
		{
# if !defined (USE_CYCLIC_ALM)
			install_one_shot(lst,globals);
#endif
			return;		
		}
	}

	if (lst->samples_per_point > 1) 
	{
		for (loop_var = 0; loop_var < lst->samples_per_point; loop_var++)
		{
			/* get index of next buffer */
			temp = (loop_var + 1) % lst->samples_per_point;
	
			/*
			 * Near-Neighbor relationship looked for:
			 *
			 * the following conditionals first check to see if buff[temp]
			 * is near an edge of the touchpanel, and if not a near-neighbor
			 * check is then done
			 */
			if (lst->x_buff[temp] > TOLERANCE &&
		    	lst->x_buff[loop_var] < (lst->x_buff[temp] - TOLERANCE)
			    				||
				lst->y_buff[temp] > TOLERANCE &&
				lst->y_buff[loop_var] < (lst->y_buff[temp] - TOLERANCE)
								||
			   (lst->x_buff[temp] + TOLERANCE) < (lst->x_pixels - 1) &&
				lst->x_buff[loop_var] > (lst->x_buff[temp] + TOLERANCE)
								||
			   (lst->y_buff[temp] + TOLERANCE) < (lst->y_pixels - 1) &&
				lst->y_buff[loop_var] > (lst->y_buff[temp] + TOLERANCE))
			{
#if !defined (USE_CYCLIC_ALM)
				install_one_shot (lst, globals);
#endif
				return;	/* consecutive points are not near enough to each other */
			}
		}
	}

	/* at this point a touch has been validated */

	/*
	 * stylus has just been put down - signal Pointer Down
	 */
	if (lst->pointer_down_flag == TRUE)
	{
		lst->pointer_down_flag = FALSE;	/* put FALSE for next time thru */
		lst->last_x = lst->x_buff[0];	/* initialize last_x/y for */
		lst->last_y = lst->y_buff[0];	/* Pointer Move usage */

		/*
		 * previous coordinates have not yet been taken, 
		 * the new coordinates are lost,
		 * indicate "input buffer overrun" error to read()
		 */
		if (lst->bytes_avail != 0)
		{
			lst->scfstat.v_err = INP_BUF_OVERRUN;
		}
		/* coordinate buffer empty, put new coordinates in buffer */
		else
		{
			/* form pointer down data packet */
			lst->pktbuf[0] = PKTBUF_PD_BYTE0;

			if (lst->cal_flag == TRUE)
			{
				if (lst->ss_ptr == 0)
				{
					lst->pktbuf[1] = ((lst->ss_y0_pos & 0x0380) >> 4) |
									 ((lst->ss_x0_pos & 0x0380) >> 7);
					lst->pktbuf[2] = lst->ss_x0_pos & 0x007F;
					lst->pktbuf[3] = lst->ss_y0_pos & 0x007F;
				}
				else
				{
					lst->pktbuf[1] = ((lst->ss_y1_pos & 0x0380) >> 4) |
									 ((lst->ss_x1_pos & 0x0380) >> 7);
					lst->pktbuf[2] = lst->ss_x1_pos & 0x007F;
					lst->pktbuf[3] = lst->ss_y1_pos & 0x007F;
				}
			}
			else
			{
				lst->pktbuf[1] = ((lst->y_buff[0] & 0x0380) >> 4) |
								 ((lst->x_buff[0] & 0x0380) >> 7);
				lst->pktbuf[2] = lst->x_buff[0] & 0x007F;
				lst->pktbuf[3] = lst->y_buff[0] & 0x007F;
			}

#if defined (TIME_STAMP)
			lst->pktbuf[4] = (globals->d_ticks & 0xF0000000) >> 28;
			lst->pktbuf[5] = (globals->d_ticks & 0x0FE00000) >> 21;
			lst->pktbuf[6] = (globals->d_ticks & 0x001FC000) >> 14;
			lst->pktbuf[7] = (globals->d_ticks & 0x00003F80) >> 7;
			lst->pktbuf[8] = (globals->d_ticks & 0x0000007F);
#endif
			lst->bytes_avail = PKTBUFSIZ;
		}

		/* send wakeup signal to any sleeping process (like read()) */
		if (lst->scfstat.v_sysio.v_wake != 0)
		{
			_os_send (lst->scfstat.v_sysio.v_wake, SIGWAKE);
			lst->scfstat.v_sysio.v_wake = 0;
		}

		/* send signal to interested process */
		if (lst->datapid != 0)
		{
			if (_os_send (lst->datapid, lst->datasig) != SUCCESS)
			{
				lst->bytes_avail = 0; 
			}

			lst->datapid = 0;	/* clear down signal condition */
			lst->datasig = 0;
			lst->dataid = 0;
		}		

		if (lst->cal_flag == TRUE)
		{
			lst->cal_flag = FALSE;	/* reinitialize cal_flag */

			if (lst->ss_ptr == 0)
				lst->ss_ptr = 1;

			/* now have both points -- perform calibration */
			else 
			{
				lst->ss_ptr = 0;

				/* save old values in case calibration fails */
				X_min_old = lst->X_min;
				Y_min_old = lst->Y_min;
				X_max_old = lst->X_max;
				Y_max_old = lst->Y_max;
				delta_x_old = lst->delta_x;
				delta_y_old = lst->delta_y;

				/* perform calibration */
				calibrate (lst);

				/* sanity check */
				if (lst->X_min > X_MIN_CHECK ||
					lst->Y_min > Y_MIN_CHECK ||
					lst->X_max < X_MAX_CHECK ||
					lst->Y_max < Y_MAX_CHECK)
				{
					lst->X_min = X_min_old;
					lst->Y_min = Y_min_old;
					lst->X_max = X_max_old;
					lst->Y_max = Y_max_old;
					lst->delta_x = delta_x_old;
					lst->delta_y = delta_y_old;
				}
			}
		}
	}

	/*
	 * stylus may or may not have been moved
	 */
	else
	{
		/* do a proximity check from the last point */
		if (!(
			lst->x_buff[0] > TOLERANCE &&
		    lst->last_x < (lst->x_buff[0] - TOLERANCE)
			    			||
			lst->y_buff[0] > TOLERANCE &&
			lst->last_y < (lst->y_buff[0] - TOLERANCE)
							||
		   (lst->x_buff[0] + TOLERANCE) < (lst->x_pixels - 1) &&
			lst->last_x > (lst->x_buff[0] + TOLERANCE)
							||
		   (lst->y_buff[0] + TOLERANCE) < (lst->y_pixels - 1) &&
			lst->last_y > (lst->y_buff[0] + TOLERANCE)
		   ))
		{
#if !defined (USE_CYCLIC_ALM)
			install_one_shot (lst, globals);
#endif
			return;		/* stylus has not moved */
		}

		/*
		 * stylus has moved - signal Pointer Move
		 */

		lst->last_x = lst->x_buff[0];	/* update last_x/y */
		lst->last_y = lst->y_buff[0];	/* for next time thru */

		/*
		 * previous coordinates have not yet been taken, 
		 * the new coordinates are lost,
		 * indicate "input buffer overrun" error to read()
		 */
		if (lst->bytes_avail != 0)
		{
			lst->scfstat.v_err = INP_BUF_OVERRUN;
		}
		/* coordinate buffer empty, put new coordinates in buffer */
		else 
		{
			/* form pointer move data packet */
			lst->pktbuf[0] = PKTBUF_PM_BYTE0;
			lst->pktbuf[1] = ((lst->y_buff[0] & 0x0380) >> 4) |
							 ((lst->x_buff[0] & 0x0380) >> 7);
			lst->pktbuf[2] = lst->x_buff[0] & 0x007F;
			lst->pktbuf[3] = lst->y_buff[0] & 0x007F;

#if defined (TIME_STAMP)
			lst->pktbuf[4] = (globals->d_ticks & 0xF0000000) >> 28;
			lst->pktbuf[5] = (globals->d_ticks & 0x0FE00000) >> 21;
			lst->pktbuf[6] = (globals->d_ticks & 0x001FC000) >> 14;
			lst->pktbuf[7] = (globals->d_ticks & 0x00003F80) >> 7;
			lst->pktbuf[8] = (globals->d_ticks & 0x0000007F);
#endif
			lst->bytes_avail = PKTBUFSIZ;
		}

		/* send wakeup signal to any sleeping process (like read()) */
		if (lst->scfstat.v_sysio.v_wake != 0)
		{
			_os_send (lst->scfstat.v_sysio.v_wake, SIGWAKE);
			lst->scfstat.v_sysio.v_wake = 0;
		}

		/* send signal to interested process */
		if (lst->datapid != 0)
		{
			if (_os_send (lst->datapid, lst->datasig) != SUCCESS)
			{
				lst->bytes_avail = 0;
			}
				
			lst->datapid = 0;	/* clear down signal condition */
			lst->datasig = 0;
			lst->dataid = 0;
		}		
	}
#if !defined (USE_CYCLIC_ALM)
	install_one_shot (lst, globals);
#endif
	return;
}


/* 
 * This function reads a called for coordinate from the touchpanel
 * and returns the corresponding pixel location of the touch.
 *
 * All hardware handling is done in this function.  
 * The ADC used is a Motorola MC145050 10-bit ADC with serial interface.
 * See touchpad.h for the interface of this device to the touchpad.
 *
 * This function also handles the resetting of the touchpanel's calibration
 * values (X_min, Y_min, X_max, and Y_max) and the recalculating of delta_x 
 * and delta_y.
 *
 * If the touchpanel is not touched, NO_TOUCH will be returned.
 *
 * Touchpad origin   
 *   (X_min,Y_min)         __         
 *      LCD origin        |  |        
 *           (0,0) _______|  |_______ 
 *                |                  |
 *                |                  |
 *                |                  | 
 *                |                  | 
 *                |                  |
 *                |                  |
 *                |                  |
 *                |                  |
 *                |__________________|
 *                                    Touchpad far corner
 *                                    (X_max,Y_max)
 *                                    LCD far corner
 *                                    (x_pixels-1,y_pixels-1)
 *
 *		Call with:
 *			Which coordinate to read (X_COOR or Y_COOR)
 *			Pointer to static storage area
 *		Return:
 *			Corresponding pixel location of touch:
 *
 * Equations used:
 *		X = binary ADC reading of X position
 *		Y = binary ADC reading of Y position
 *		X_pix = corresponding pixel location of X
 *		Y_pix = corresponding pixel location of Y
 *
 *		X_pix = 1024 * (Y_max - X) / delta_x
 *		Y_pix = 1024 * (Y_max - Y) / delta_y
 */
u_char get_coor 
(
	coor_enum coor,
	TStatics lst
)
{
	u_short adc = 0;	/* ADC reading */
	u_int32 count;
	void delay (void);	/* function below */	    
	
	/* turn on power to the touchpanel */
	if (coor == Y_COOR)
		lst->port->pio_regs.portj.data = PJREADY;	/* power Y path */
	else
		lst->port->pio_regs.portj.data = PJREADX;	/* power X path */
	
	/* wait for power to stabilize */
	delay();

	/* tell adc to read appropriate channel */
	if (coor == Y_COOR)
		lst->port->spim_regs.data = Y_CHANNEL;	
	else
		lst->port->spim_regs.data = X_CHANNEL;	

	lst->port->spim_regs.cont |= SPIM_XCH;	/* initiate exchange */
	
	/* wait for completion of SPI transfer to ADC */

/*	while (!(lst->port->spim_regs.cont & SPIM_IRQST))
		;  */

	for (count = 0; count < 20; count++)
		if (lst->port->spim_regs.cont & SPIM_IRQST)
			break;

	lst->port->spim_regs.cont &= ~SPIM_IRQST;	/* clear IRQ status */

	if (count != 10)	/* an IRQ actually happened */
	{
		/* wait for rest of 44 clock cycles for ADC conversion */
		delay(); 

		/* put dummy value in data register for proper exchange */
		lst->port->spim_regs.data = 0; 

		/* get ADC reading for channel of interest */
		lst->port->spim_regs.cont |= SPIM_XCH; /* initiate exchange */

		/* wait for completion */
/*		while (!(lst->port->spim_regs.cont & SPIM_IRQST))
			;  */

		for (count = 0; count < 20; count++)
			if (lst->port->spim_regs.cont & SPIM_IRQST)
				break;

		lst->port->spim_regs.cont &= ~SPIM_IRQST;	/* clear IRQ status */

		if (count != 10)	/* transfer successful */
		{
			adc = lst->port->spim_regs.data;
		}
		else
			return (NO_TOUCH);
	}
	else
		return (NO_TOUCH);

	/* put touchpanel in low power state */
	lst->port->pio_regs.portj.data = PJIRQWAIT;
		
	/* test for calibration */
	if (lst->cal_flag == TRUE)
		if (coor == X_COOR && adc > X_MIN_DEFAULT)
		{
			if (lst->ss_ptr == 0)
				lst->ss_adc_x0 = adc;
			else
				lst->ss_adc_x1 = adc;
		}
		else if (coor == Y_COOR && adc > Y_MIN_DEFAULT)
		{
			if (lst->ss_ptr == 0)
				lst->ss_adc_y0 = adc;
			else
				lst->ss_adc_y1 = adc;
		}
		
	/* convert ADC binary reading to corresponding pixel location */
	if (coor == Y_COOR)
	{
		if (lst->cal_flag == FALSE && adc < lst->Y_min ||
			lst->cal_flag == TRUE  && adc < Y_MIN_DEFAULT)
			/* if touchpanel is not touched, Y voltage reading tends towards
			 * 0 Volts (I'm not sure why, though, it has no reference 
			 * voltage, but it is seen from debugging that the ADC
			 * returns with a value of 0x000) */
			adc = NO_TOUCH;
		else if (adc >= lst->Y_max)
			adc = lst->y_pixels - 1;
		else
			/* the numerator is mulitiplied by 1024 (i.e.  shifted 10 bits) since
			 * the denominator (delta_y) is also multiplied by 1024 in init(),
			 * this is done to avoid the use of floating point calculations
			 */
			adc = (u_char) ((1024 * ((u_int32) adc - lst->Y_min)) / lst->delta_y);
	}
	else
	{
		if (lst->cal_flag == FALSE && adc >= lst->X_max ||
			lst->cal_flag == TRUE  && adc >= X_MAX_DEFAULT)
			/* if touchpanel is not touched, X voltage reading tends towards
			 * Vcc (I'm not sure why, though, it has no reference
			 * voltage, but it is seen from debugging that the ADC
			 * returns with a value of 0x3FF) */
			adc = NO_TOUCH;
		else if (adc < lst->X_min)
			adc = 0;
		else
			/* the numerator is mulitiplied by 1024 (i.e.  shifted 10 bits) since
			 * the denominator (delta_x) is also multiplied by 1024 in init(),
			 * this is done to avoid the use of floating point calculations */
			adc = (u_char) ((1024 * ((u_int32) adc - lst->X_min)) / lst->delta_x);
	}
		
	return (adc);
}



/*
 * Touchpanel power-on delayer function, required to let power stabilize
 */
void delay (void)
{ 
	/* delay counter (made volatile so function won't be optimized away) */
	volatile int delay_var;

	for (delay_var = 1; delay_var < POWER_DELAY; delay_var++); 
}



/*
 * Function to calculate calibration values
 *		Call with:
 *			Pointer to static storage area
 *		Return:
 *			Nothing
 */
void calibrate
(
	TStatics lst
)
{	
	/* at this point have all calibration data:
	 *
	 *		[adc_x,adc_y]
	 *		(x_pixel_pos,y_pixel_pos)
	 *
	 * [X_min,Y_min]
	 * (0,0)
	 *  ____________________________________
	 * |	[ss_adc_x0,ss_adc_y0]			|
	 * |	(ss_x0_pos,ss_y0_pos)			|
	 * |		*							|
	 * |									|
	 * |									|
	 * |									|
	 * |									|
	 * |						*			|
	 * |			[ss_adc_x1,ss_adc_y1]	|
	 * |			(ss_x1_pos,ss_y1_pos)	|
	 * |____________________________________|
	 * 								[X_max,Y_max]
	 *								(x_pixels-1,y_pixels-1)
	 *
	 * delta_x = (ss_adc_x1 - ss_adc_x0) / (ss_x1_pos - ss_x0_pos)
	 * delta_y = (ss_adc_y1 - ss_adc_y0) / (ss_y1_pos - ss_y0_pos)
	 *
	 * adc_x = ss_adc_x0 + delta_x * (pos_x - ss_x0_pos)
	 * adc_y = ss_adc_y0 + delta_y * (pos_y - ss_y0_pos)
	 */
	lst->delta_x = (u_int32) abs ( (int) (1024 * 
		(lst->ss_adc_x1 - lst->ss_adc_x0) / 
		(lst->ss_x1_pos - lst->ss_x0_pos) ));

	lst->delta_y = (u_int32) abs ( (int) (1024 * 
		(lst->ss_adc_y1 - lst->ss_adc_y0) / 
		(lst->ss_y1_pos - lst->ss_y0_pos) ));

	lst->X_min = (u_short) (lst->ss_adc_x0 - 
					(lst->delta_x * lst->ss_x0_pos) / 1024);
	lst->Y_min = (u_short) (lst->ss_adc_y0 - 
					(lst->delta_y * lst->ss_y0_pos) / 1024);

	lst->X_max = (u_short) (lst->ss_adc_x0 + 
					lst->delta_x * (lst->x_pixels - 1 - lst->ss_x0_pos) / 1024);
	lst->Y_max = (u_short) (lst->ss_adc_y0 + 
					lst->delta_y * (lst->y_pixels - 1 - lst->ss_y0_pos) / 1024);

	return;
}

