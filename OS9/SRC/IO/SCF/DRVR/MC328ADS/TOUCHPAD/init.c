/*****************************************************************************
** ID:     @(#) init.c 1.5@(#)
** Date:   3/14/96
******************************************************************************
 * This module contains init(), the initialization code for 
 * the MC68328ADS Touchpanel driver.
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
**	 1 09/13/95 creation                                          rws       **
*****************************************************************************/

#include "touchpad.h"


/*
 * Device initialize routine:
 *
 * This routine initializes the touchpanel device driver's static storage
 * variables and installs the touchpanel's interrupt.
 *
 *		Call with:
 *			pointer to device descriptor
 *			pointer to static storage area
 *			pointer to system variables
 *		Return:
 *			Error code (0 if none)
 */
error_code init 
(
	mod_dev *dd, 
	TStatics lst, 
	Sysglobs globals
)
{
	error_code error;		/* used to keep track of errors */
	status_reg old_mask;	/* used to save current sr IRQ mask level */

	extern status_reg mask_irq (status_reg mask);	/* in read.c */
	extern error_code pen_isr();					/* in irqs.c */

	u_int32 loop_var;

	/* 
	 * Initialize Static Storage Area
	 */
	 
	scfstat.v_err = NO_ERROR;
	port = (m68328_regs *) dd->_mport;

	/* initialize SS_SSIG signal data */
	datapid = 0;	/* no process is waiting to send a signal */

	/* initialize data packet related variables */
	bytes_avail = 0;		/* there are no bytes available */
	for (pktbuf_index = 0; pktbuf_index < PKTBUFSIZ; pktbuf_index++)
		pktbuf[pktbuf_index] = 0;
	pktbuf_index = 0;		/* output packet header first */
	
	/* initialize x_pixels and y_pixels from device descriptor's devcon field
	 * where x_pixels is at devcon (4 bytes) and y_pixels is after that
	 * (4 bytes) */
	x_pixels = *(u_int32 *)((char *)dd + dd->_mdevcon);
	y_pixels = *(u_int32 *)((char *)dd + dd->_mdevcon + 4);	
	if (x_pixels == 0 || y_pixels == 0)
		return (EOS_PARAM);	/* bad parameter */

	/* initialize touchpanel's cyclic alarm variables */
	cyc_alm_ident = 0;

	cyc_alm_period = *(u_int32 *)((char *)dd + dd->_mdevcon + 8);
	if (cyc_alm_period == 0)
		return (EOS_PARAM); /* bad parameter */

	pointer_down_flag = TRUE;

	/* initialize buffer used by alarm_h() to filter data */
	samples_per_point = *(u_int32 *)((char *)dd + dd->_mdevcon + 0xC);
	if (samples_per_point == 0 || samples_per_point > MAX_BUFF)
		return (EOS_PARAM); /* bad parameter */

	for (loop_var = 0; loop_var < samples_per_point; loop_var++)
		x_buff[loop_var] = y_buff[loop_var] = 0;

	/* 
	 * find the touchpanel's interrupt level and shift to upper byte where
	 * it is located in the Status Register,
	 * used to mask the touchpanel during critical sections in read()
	 */
	mask_tpad_level = (status_reg)(PENIRQ << 8);

	/*
	 * These are the initialized TPAD calibration values, an application's 
	 * initial action will have to be to send a setstat() code to find the 
	 * origin and then to find the far corner
	 */
	X_min = X_MIN_DEFAULT;
    Y_min = Y_MIN_DEFAULT;
    X_max = X_MAX_DEFAULT;
    Y_max = Y_MAX_DEFAULT;

	/*
	 * calculate proportion constants used to translate raw binary ADC
	 * reading of touchpanel into corresponding LCD pixel location,
	 * Note: the multiplication by 1024 is done to avoid the use
	 * of floating point numbers; in get_coor() in irqs.c the binary
	 * reading of the ADC is also multiplied by 1024 and then divided by
	 * delta_x/y to get the corresponding pixel coordinate (1024 is used,
	 * as opposed to 1000, since it is a quick shift of 10 bits).
	 */
	delta_x = (1024 * (X_max - X_min)) / (x_pixels - 1);
	delta_y = (1024 * (Y_max - Y_min)) / (y_pixels - 1);
	
	/* avoid division by zero */
	if (delta_x == 0)
		delta_x = 1;
	if (delta_y == 0)
		delta_y = 1;

	cal_flag = FALSE;	/* tell isr() to interpret next touch as regular input */
	ss_ptr = 0;			/* point to first calibration point */
	
	/* install interrupt service routine */
	if ((error = _os9_irq(dd->_mvector,dd->_mpriority,pen_isr,lst,dd->_mport))
			!= SUCCESS) 
	{
		return (error);
	}

	/* turn on touchpanel interrupt */
	old_mask = mask_irq (mask_tpad_level);
	{
		/* 
		 * configure Port J pins for Power Switching function
		 * and to wait for an interrupt
		 *	NOTE: PJ5 and PJ6 are used as CSD1 and CSD2, respectively,
		 * 	for the UART Debug Port and the ADI Parallel Interface,
		 *	they are set up in the boot as chip selects, so this driver
		 * 	must not change the states of these pins in the Port J Select
		 *	register.  Will use |= and &= operations on the Port J Select
		 *	register, all other port J registers can be set directly with
		 * 	the assignment (=) operator.
		 */
		port->pio_regs.portj.data = PJIRQWAIT;
		port->pio_regs.portj.dir = PJDIROUT;
		port->pio_regs.portj.sel |= PJSELPORT;

		/*
		 * configure Port K for SPIM usage
		 *	NOTE: PK2:0 are used for Master SPI bus, the other
		 *  PK pins are used for the Slave SPI and for the PCMCIA
		 *	hardware.  This driver must only touch PK2:0 in the Port K
		 *	Select Register.
		 */
		port->pio_regs.portk.sel &= PKSPIM;

		/*
		 * configure Port M for PENIRQn input and to use pull-up resistor
		 *	NOTE: only PM6 can be touched in this driver as other Port M
		 *	pins are used for other devices.
		 */
		port->pio_regs.portm.sel &= PENIRQ_SEL;
		port->pio_regs.portm.puen |= PENPU_EN;

		/* unmask PENIRQn interrupt */
		port->irq_regs.imr &= ~MASKPEN;
	}
	mask_irq (old_mask);

	if (error != SUCCESS)
	{
		return (error);
	}
		
	return (error);	
}
