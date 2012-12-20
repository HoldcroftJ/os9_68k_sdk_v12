/****************************************************************************
 *           Copyright 1989-1998 by Microware Systems Corporation           *
 *                           All Rights Reserved                            *
 *                         Reproduced Under License                         *
 *                                                                          *
 * This software is confidential property of Microware Systems Corporation, *
 * and is provided under license for internal development purposes only.    *
 * Reproduction, publication, distribution, or creation of derivative works *
 * in any form to any party other than the licensee is strictly prohibited, *
 * unless expressly authorized in writing by Microware Systems Corporation. *
 *                                                                          *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *   0 04/04/97 Creation                                               mil
 *   3 07/15/97 Ported to os9/68k  SPF                                 jpd
 *   4 08/27/97 Bug fix                                                taka
 *   5 09/08/97 fixed os_irq()                                         taka
 *   6 09/29/97 Added MVME172 support                                  pjw
 *   8 01/21/98 Removed compiler warning                               jk
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  11 06/22/99 Integrated Kei Thomsen's changes, fixed mbuf leak.     jk
 *  15 08/17/01 Corrected some Flushing in PVTRAM mode.                KeiT
 ****************************************************************************/


#include <SP82596/defs.h>

#ifdef _OSK

/*******************************************************************************
* NAME:         hw_isr()
*
* USAGE:        OS9 ISR routine. This is glue code to get to the real (OS-9000
*                       style) ISR routine.
*/

_asm("
hw_isr:
        move.l  a2,d0           ; put Dev_list in a2 into d0
        bsr.s   isr             ; call interrupt service routine
        tst.l   d0              ; see if SUCCESS returned
        beq.s   hw_isr_exit     ; if so, return
        ori     #Carry,ccr      ; else E_NOTME returned--set carry bit
hw_isr_exit
        rts
");

#else   /* For OS9000 */

error_code hw_isr(Dev_list dev_entry) {
    return isr(dev_entry);
}

#endif

/* the REAL i82596 interrupt service routine */
error_code isr(Dev_list dev_entry)
{
	Spf_drstat	drstat = (Spf_drstat)(dev_entry->v_dr_stat);
	Spf_lustat	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	SCB 		*scb = lustat->lu_scb;
	u_int16		status;

#if defined(sp167) || defined(sp162) || defined(spgenesis) || defined(sp177) || defined(sp172)

	/* is this us? */
	if (*LANC_IRQ_CONTROL & LANC_INT) {
#if defined (Flushing) && !defined(PVTRAM)
		_os_cache(6);               /* Flush data cache and turn it off */
#endif
		/* clear the interrupt */
		*LANC_IRQ_CONTROL |= LANC_ICLR;
		drstat->dr_iestat.dev_irq++;

		/* ack the IRQ status */
		status =  scb->scb_status;
		scb->scb_cmd |= (status & 0xf000);
		I82596_CHANATTN(lustat);

		if (status & (SCB_STATUS_FR | SCB_STATUS_RNR)) {
			recv_irq(dev_entry);	/* receive IRQ */
		}
		if (status & SCB_STATUS_CX) {
			xmit_irq(dev_entry);	/* xmit (or command) IRQ */
		}
#if defined (Flushing) && !defined(PVTRAM)
		_os_cache(5);               /* Flush data cache and turn it on */
#endif
    } else if (*LANC_BEIRQ_CONTROL & LANC_INT) {
		/* bus error IRQ? */
		drstat->dr_iestat.abort_irq++;
#ifdef SYSDBG
		_os_sysdbg((void*)0x9898,NULL);	/* serious trouble if reached */
#else
		return (SUCCESS);
#endif
    } else {
    	/* huh... */
    	drstat->dr_iestat.unk_irq++;
    	return(EOS_NOTME);
    }

#elif defined(mz8554)
	if (*MZ8554_ETHER_CONTROL(lustat->lu_addr) & MZ8554_INT_ASSERTED) {
		*MZ8554_INT_CLEAR(lustat->lu_addr) = 0;
		drstat->dr_iestat.dev_irq++;

		status =  scb->scb_status;
		scb->scb_cmd |= (status & 0xf000);	/* ack the IRQ status */
		I82596_CHANATTN(lustat);

    	if (status & (SCB_STATUS_FR | SCB_STATUS_RNR))	/* receive IRQ */
			recv_irq(dev_entry);
    	if (status & SCB_STATUS_CX)		/* xmit (or command) IRQ */
			xmit_irq(dev_entry);
    } else {
    	drstat->dr_iestat.unk_irq++;		/* huh... */
    }

#elif defined(hkvf82596)

/*H4VF_HEXDIS(~1);*/
	if (lustat->lu_isV4d) {
		u_int8 tmask;
		/*
		 * check i82596 INT pin thru vic.  It may not be us but
		 * it probably is.  (maybe) we can miss the pin due to the
		 * edge-trigger.  Just assume since we're here it must have
		 * been.
		 */
		if ((*HV4D_LIRQ1) & 0x8) {		/* is this us? */
			drstat->dr_iestat.dev_irq++;
		} else if (*HV4D_EIRQ_A_STATUS & 0x4) {
			/* abort irq? */
			drstat->dr_iestat.abort_irq++;
			*HV4D_EIRQ_A_CLR((u_int32)lustat->lu_port) = olda6;	/* clear abort IRQ */
		} else drstat->dr_iestat.unk_irq++;		/* huh... */

@ cpusha dc

		status =  scb->scb_status;
		scb->scb_cmd |= (status & 0xf000);	/* ack the IRQ status */
		I82596_CHANATTN(lustat);

		if (status & (SCB_STATUS_FR | SCB_STATUS_RNR))	/* receive IRQ */
			recv_irq(dev_entry);
		if (status & SCB_STATUS_CX)		/* xmit (or command) IRQ */
			xmit_irq(dev_entry);
		} else {
			if ((*CMSE_EIRQ_STATUS(lustat->lu_addr)) & 1) { /* is this us? */
			*CMSE_EIRQ_CLR(lustat->lu_addr) = 0;	/* clear irq */
			drstat->dr_iestat.dev_irq++;

@ cpusha dc

			status =  scb->scb_status;
			scb->scb_cmd |= (status & 0xf000);	/* ack the IRQ status */
			I82596_CHANATTN(lustat);

			if (status & (SCB_STATUS_FR | SCB_STATUS_RNR)) /* receive IRQ */
				recv_irq(dev_entry);
			if (status & SCB_STATUS_CX)		/* xmit (or command) IRQ */
				xmit_irq(dev_entry);
		} else if ((*CMSE_EIRQ_A_STATUS(lustat->lu_addr))&1) {/* abort? */
			drstat->dr_iestat.abort_irq++;
			*CMSE_EIRQ_A_STATUS(lustat->lu_addr) = 0;	/* clear abort IRQ */
		} else {
			*CMSE_EIRQ_CLR(lustat->lu_addr) = 0; /* clear irq anyway?! */
			drstat->dr_iestat.unk_irq++;		/* huh... */
		}
	}
/*H4VF_HEXDIS(~0);*/

#elif defined(ie7140)

	if (*MZ7140_INT & MZ7140_ETHER_INT) {
		*MZ7140_INT = MZ7140_ETHER_INT;		/* ack the irq */
		drstat->dr_iestat.dev_irq++;

		status =  scb->scb_status;
		scb->scb_cmd |= (status & 0xf000);	/* ack the IRQ status */
		I82596_CHANATTN(lustat);

		if (status & (SCB_STATUS_FR | SCB_STATUS_RNR))	/* receive IRQ */
			recv_irq(dev_entry);
		if (status & SCB_STATUS_CX)		/* xmit (or command) IRQ */
			xmit_irq(dev_entry);
	} else drstat->dr_iestat.unk_irq++;		/* huh? */

#elif defined (spdve7708)
	u_int32 *dga = lustat->lu_dga;

	if (dga[20] & GP0IRQ) {	/* GP0 IRQ ? */
		u_int32 *irqclr = lustat->lu_irqclr;
		*irqclr = 0;	 /* Clear IRQ */

		if (lustat->lu_ioenabled == DRVR_IOEN) {
			drstat->dr_iestat.dev_irq ++;
			status =  scb->scb_status;
			scb->scb_cmd |= (status & 0xf000);	/* ack the IRQ status */
			I82596_CHANATTN(lustat);

			if (status & (SCB_STATUS_FR | SCB_STATUS_RNR)) { /* receive IRQ */
				recv_irq(dev_entry);
			}
			if (status & SCB_STATUS_CX) {		/* xmit (or command) IRQ */
				xmit_irq(dev_entry);
			}
		} else {
			drstat->dr_iestat.unk_irq++;		/* huh? */
		}
    } else {
		drstat->dr_iestat.unk_irq++;		/* huh? */
    }
#endif

	return SUCCESS;
}

