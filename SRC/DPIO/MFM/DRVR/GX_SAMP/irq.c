/*******************************************************************************
*
* DESCRIPTION :
*
*	Interrupt service functions.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  11/28/95  TJE  First version
*/
#include <defs.h>
#include <events.h>

/*******************************************************************************
* INTERRUPT SERVICE ROUTINE PROTOTYPES AND NAMES
*******************************************************************************/

error_code hw_isr(Dev_list dev_entry);

#ifdef _OSK
	extern void hw_isr_os9(void);
	#define HW_ISR hw_isr_os9	/* OS-9 interrupt service routine */
#else
	#define HW_ISR hw_isr		/* OS-9000 interrupt service routine */
#endif

/*******************************************************************************
* NAME:		dr_init_irqs()
*
* USAGE:	Initialize IRQs
*/
error_code dr_init_irqs(Dev_list dev_entry)
{
	error_code ec;
	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);

	/* Enable retrace interrupts */
	if ((ec = _os_irq(lustat->irq_num, lustat->irq_priority, HW_ISR, dev_entry))
		!= SUCCESS) {
		return ec;
	}

	*?*?* IF YOUR HARDWARE REQUIRES YOU TO SETUP NON-STANDARD I/O REGISTERS TO
	*?*?* ENABLE INTERRUPTS, YOU SHOULD INCLUDE CODE TO DO SO HERE.

	/* If on a PC, then unmask the interrupt in the PIC */
#ifdef PC_AT
	pic = inc(0x21);
	outc(0x21, pic & ~IRQ_PIC_MASK);
#endif

	return SUCCESS;
}

/*******************************************************************************
* NAME:		dr_term_irqs()
*
* USAGE:	Terminate IRQs
*/
error_code dr_term_irqs(Dev_list dev_entry)
{
	error_code ec, result = SUCCESS;
	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);

	*?*?* IF YOUR HARDWARE REQUIRES YOU TO SETUP NON-STANDARD I/O REGISTERS TO
	*?*?* DISABLE INTERRUPTS, YOU SHOULD INCLUDE CODE TO DO SO HERE.

	/* If on a PC, then mask the interrupt in the PIC */
#ifdef PC_AT
	pic = inc(0x21);
	outc(0x21, pic | IRQ_PIC_MASK);
#endif

	/* Disable retrace interrupts */
	if ((ec =_os_irq(lustat->irq_num, lustat->irq_priority, NULL, dev_entry))
		!= SUCCESS) {
		result = ec;
	}

	return result;
}

/*******************************************************************************
* NAME:		hw_isr_os9()
*
* USAGE:	OS9 ISR routine. This is glue code to get to the real (OS-9000
*			style) ISR routine.
*/
#if defined(_OSK)
_asm("
hw_isr_os9:
	move.l	a2,d0				; put Dev_list in a2 into d0
	bsr		hw_isr				; call interrupt service routine
	tst.l	d0					; see if SUCCESS returned
	beq.s	hw_isr_os9_exit		; if so, return
	ori		#Carry,ccr			; else E_NOTME returned--set carry bit
hw_isr_os9_exit
	rts
");
#endif

/*******************************************************************************
* NAME:		hw_isr()
*
* USAGE:	Interrupt service routine.
*/
error_code hw_isr(Dev_list dev_entry)
{
	error_code ec;
	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);
	int32 event_val;

	/* Check if this is our interrupt. Reading the status register also */
	/* clears the interrupt if it is ours. */

	*?*?* INCLUDE CODE HERE TO SEE IF YOUR HARDWARE WAS THE SOURCE OF THE
	*?*?* INTERRUPT. IF IT WAS NOT, THEN YOU SHOULD RETURN THE ERROR
	*?*?* "EOS_NOTME".

	/* Pulse the retrace event */
	event_val = 1;
	_os_ev_pulse(lustat->irq_event_id, &event_val, 1);

	return SUCCESS;
}
