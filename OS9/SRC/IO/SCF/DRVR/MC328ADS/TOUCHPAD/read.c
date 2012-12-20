/*****************************************************************************
** ID:     @(#) read.c 1.4@(#)
** Date:   7/11/96
******************************************************************************
 * This module contains the following functions:
 * read() reads a character from the MC68328ADS Touchpanel device
 * write() writes a character to the MC68328ADS Touchpanel device
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
 * Function to get a character from the tpaddrvr.
 *
 *		Call with:
 *			Pointer to put character into
 *			Pointer to current process descriptor
 *		Return:
 *			Error code
 */
error_code read 
(
	char *inp,
	procid *proc_desc
)
{
	error_code error = SUCCESS;	/* error holder (return value) */
	u_int32 ticks = 0;		/* sent to _os9_sleep() to sleep indefinitely */
	status_reg old_mask;	/* used to save current sr IRQ mask level */

	status_reg mask_irq (status_reg mask);	/* function below */
	
	/* a process has set up a data-ready signal */
	if (datapid != 0)
		return(EOS_DEVBSY);		/* device is busy for reading */

	/* input buffer overrun error (coordinates lost) -- indicate error */
	if (scfstat.v_err == INP_BUF_OVERRUN)
	{
		error = EOS_READ;
		scfstat.v_err = NO_ERROR;
		bytes_avail = 0;
	}
	else
	{
		/* mask touchpad interrupt so there can be no contention on 
		 * bytes_avail and pktbuf[] between read() and alarm_h() */
/* 		old_mask = mask_irq (mask_tpad_level); */
		old_mask = mask_irq (0x0700);

		/* wait for data to become available */
		if (bytes_avail == 0)
		{
			/* data is not available--will put process
			 * to sleep (i.e. block process) and have 
			 * alarm_irqsvc() wake process up when data 
			 * is available
			 */
			 
back_to_sleep:

			/* arrange wake up signal */
			scfstat.v_sysio.v_wake = scfstat.v_sysio.v_busy;
				
			mask_irq (old_mask);	/* unmask alarm interrupt */
		
			/* sleep until woken up by signal from alarm handler */
			_os9_sleep (&ticks);
			
			/* at this point, either data is available or a signal
			 * has occurred */
			
			/* mask touchpad interrupt again to read bytes_avail */
/*			old_mask = mask_irq (mask_tpad_level); */
			old_mask = mask_irq (0x0700);

			if (bytes_avail == 0)
				/* check for deadly I/O signal being present or for Condemn 
				 * bit being set */
				if (proc_desc->_signal < SIGDEADLY ||
				   (proc_desc->_state & PS_CONDEMN) == PS_CONDEMN)
				{
					scfstat.v_err = NO_ERROR;
					bytes_avail = 0;
					mask_irq (old_mask);	/* unmask interrupts */
					return (EOS_READ);
				}
				else
					/* if no data is available go back to sleep */
					goto back_to_sleep;		/* well, how did I get here? */
		}

		*inp = pktbuf[pktbuf_index];	/* return character */

		/* update data packet index */	
		pktbuf_index = (pktbuf_index + 1) % PKTBUFSIZ;
    
		bytes_avail--;

		mask_irq (old_mask);			/* unmask alarm interrupt */
	}

	return (error);
}



/*
 * Function to output a character.
 * Note: the touchpad is a read-only device, so the only return possible
 * is an error condition.
 *
 *		Call with:
 *			Character to write
 *          Pointer to path descriptor
 *		Return:
 *			Error code (0 if none)
 */
error_code write 
(
	char outp,
	Pathdesc path_desc
)
{
    return (EOS_UNKSVC);	
}



/* 
 * status_reg mask_irq (status_reg mask_level)
 *
 * Inclusion of routine to mask interrupts to called for level
 */
_asm("
*
* Routine to set Status Register's Interrupt Priority Mask bits
*
* WARNING - THIS IS 68xxx SPECIFIC CODE
* Note also that stack is kept long-aligned.
*
* Usage:	status_reg mask_irq (status_reg mask_level)
*
* Call with:
*	New IRQ mask level
* Return:
*	Current IRQ mask level
*
mask_irq:
 link a5,#0
 movem.l d0/d1,-(sp) make space and save d1
 moveq.l #0,d1 sweep reg
 move.w sr,d1 get current sr
 andi.w #IntMask,d1 isolate current interrupt mask bits
 move.l d1,0(sp) return current irq level in d0
 move.w sr,d1 copy current sr (again)
 andi.w #IntEnab,d1 clear current irq flags
 or.w d0,d1 add in desired irq mask level 
 move.w d1,sr mask interrupts
 movem.l (sp)+,d0/d1 return original mask
 unlk a5
 rts
");
