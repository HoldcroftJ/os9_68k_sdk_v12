/*****************************************************************************
** ID:     @(#) term.c 1.2@(#)
** Date:   1/18/96
******************************************************************************
 * This module contains term(), the termination code for
 * the MC68328ADS Touchpanel device.
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
**	 1 10/27/95 creation                                          rws       **
*****************************************************************************/

#include "touchpad.h"


/*
 * Device termination routine
 *      Call with:
 *          pointer to device descriptor
 *			pointer to static storage
 *      Return:
 *          Error code (0 if none)
 */
error_code term 
(
	mod_dev *dd,
	TStatics lst
)
{
	status_reg old_mask;	/* used to save current sr IRQ mask level */
	
	extern status_reg mask_irq (status_reg mask);	/* in read.c */

	/* turn-off touchpanel interrupt */
	old_mask = mask_irq (mask_tpad_level);
	{
		/* mask PENIRQn interrupt */
 		port->irq_regs.imr |= MASKPEN;

		/* reconfigure PENIRQ port for GPIO input */
		port->pio_regs.portm.sel |= ~PENIRQ_SEL;
		/* disable pull-up resistor */
		port->pio_regs.portm.puen &= ~PENPU_EN;

		/* keep Port J waiting for PENIRQ */
		port->pio_regs.portj.data = PJIRQWAIT;
		port->pio_regs.portj.dir = PJDIROUT;
		port->pio_regs.portj.sel |= PJSELPORT;

		/* stand-down SPIM:
		 * disable SPIM and disable SPIM's IRQ
		 */
		port->spim_regs.cont &= ~(SPIMEN + SPIM_IRQEN);
		/* clear any IRQ status */
		port->spim_regs.cont |= SPIM_IRQST;	
	}
	mask_irq (old_mask);

	/* remove interrupt service routine */
	(void)_os9_irq(dd->_mvector,dd->_mpriority,NULL,lst,dd->_mport);

	/* delete alarm */
	_os_salarm_delete (cyc_alm_ident);

	/* deinitialize driver static */	
	datapid = 0;
	datasig = 0;
	dataid = 0;

    return (SUCCESS);   
}
