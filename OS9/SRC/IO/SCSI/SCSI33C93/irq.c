/* _________________________ irq.c _________________________
 *
 *  This module contains the following low level interrupt 
 * service routines for the WD33C93:
 *
 * irq(), notifies mainline that an irq has occured
 * chk_reset(), tracks the SCSI RESET interrupt for the Motorola MVME147
 * popdebug() from irqasm.a is included below using the use
 *      assembly directive, calls the system debugger
 */
 
/*
 * Copyright 1988, 1989, 1990, 1995 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	"scsi33c93.h"

/* function in irqsup.c */
extern u_char wd_wait (register Lstat lst, register Hardware hw);

extern int kick (Lstat lst, CntrlBlk cnt); 	/* in kick.c */



/*
 * Interrupt Service Routine for WD33C93 SCSI controller
 *		Call with:
 *			lst, low-level static storage
 *			hw, wd33c93 hardware pointer
 *		Returns:
 *			0 to tell kernel interrupt does not come from WD33C93
 *			1 to tell kernel interrupt has been serviced
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int irq 
(
	register Lstat lst,
	register Hardware hw
)    
#else 
int irq (lst, hw)
register Lstat lst; 
register Hardware hw;
#endif
{
	register u_char devstat;      /* wd33c93 status */
	register CntrlBlk cnt;        /* current command/control block */
	register u_int zero = 0;      /* an interesting constant */
	int	dowake,                   /* wakeup process flag */
	    sink,                     /* junk holder */
	    i;                        /* used as a loop counter */

#if defined (VME147) && defined (USEDMA)
	register u_char dmastat;      /* dma device status */
	register Dmahardware dmahw;   /* dma hardware ptr */

	/* special code to detect SCSI resets, in this module below */
	int chk_reset (Lstat lst);    
#endif

 	/* check to see if irq is ours */
	devstat = HW(address_reg);				/* read wd33c93 auxstatus reg */
	
	/* when arriving here, we must determine if we caused the interrupt.
	 *   The heavily defined code below produces the equivalent of:
	 *
	 *   if( not_our_interrupt )
	 *       return 1;   (not ours)
	 */
#ifdef VME147
#ifdef USEDMA
	dmahw = lst->dmabase;					/* get dma hardware ptr */
	dmastat = dmahw->dmaicr;				/* get dma irq status */
#ifdef DMAIRQ
	/* check for "our" irq */
	if (((devstat & AUX_IRQ) == 0) && ((dmastat & DMA_IRQ) == 0)) {
#else
	/* check for "our" irq */
	if ((devstat & AUX_IRQ) == 0) {
#endif
#else
	/* check for "our" irq */
	if ((devstat & AUX_IRQ) == 0) {
#endif
#else 	/* all others use this to determine interrupt occurance */
	/* check for "our" irq */
	if ((devstat & AUX_IRQ) == 0) {
#endif

#ifdef VME147
		/* in case of vme147, check for SCSI reset interrupt */
		return (chk_reset(lst));
#else
		/* in case of all others, the interrupt was not us */		
		return 1;						/* return not us */
#endif
	}

	/* interrupt from wd33c93 and/or dma device */
	do {	
		dowake = FALSE;							/* default no wakeup */
		cnt = lst->curr_cmd;					/* current command/control block */
		lst->auxstatus = 0;						/* prevent old status */
		
		if( devstat & AUX_LCI ) {		/* command rejected */
			if( lst->freshcmd) {		/* reconnect during command submission */
				cnt->cbk_status &= ~CBK_SUBMIT;
				cnt->cntreg_valid = FALSE;
			}
		}
	
		/* if irq from wd33c93, get status & clear irq */
		if (devstat & AUX_IRQ) {
			/* wd33c93 had irq pending, clear it */
			lst->auxstatus = devstat;			/* save auxstatus */
			HW(address_reg) = STATUS;			/* read out irq cause */
			devstat = HW(reg_file);
		}

#ifdef USEDMA
#ifdef VME147
		/* if irq from dma device, get status & clear irq */
		if (dmastat & DMA_IRQ){
			/* dma device had irq pending, clear it */
			dmastat = dmahw->dmactrl;			/* get dma status */
			cnt->dmastatus = dmastat;			/* save dma status */
			dmahw->dmactrl = 0;					/* disable dma device */
			sink = (int)dmahw->dmaicr;			/* read dma irq control */
			sink &= ~DMA_IRQ_ENAB;				/* clear irq enable */
			dmahw->dmaicr = (u_char)sink;		/* clear dma irq */
		}
#endif
#ifdef VME620
		/* disable dma device */
		HW(dma_control) = DMA_DMAOFF;
#endif
#endif

		/* service wd33c93 irq termination */
		if (lst->auxstatus & AUX_IRQ) {		/* wd33c93 interrupt pending */
			dowake = route_irq( hw, cnt, lst, devstat );
        	lst->freshcmd = FALSE;			/* served it's purpose */
		}

#if defined (VME147) && defined (USEDMA)
		/* service dma termination */
		if (dmastat & DMA_DONE) {
			/* check for errors */
			if (dmastat & DMA_ERRMASK) {	/* dma stopped due to error */
				/* stop wd33c93 */
				HW(address_reg) = COMMAND;		/* point at command register */
				sink = ABORT;
				HW(reg_file) = sink;			/* issue abort scsi cmd */
				dowake = TRUE;					/* activate waiting process */
		
				/* wait for wd33c93 and record status */
				cnt->cmd_status = wd_wait(lst, hw);
				for (sink = 0; sink == WD_DELAY; sink++);	/* delay 7uS */
				HW(address_reg) = COMMAND;		/* point at command register */
				sink = RESET;
				HW(reg_file) = sink;			/* reset the device */
				/* wait for wd33c93 */
				sink = (int)wd_wait(lst,hw);
			}
		} else if (dowake) {
			/*
		 	 * no dma irq, BUT wakeup required.
		  	 * in this case assume that dma irq was "late" arriving
		 	 * and thus dma needs to be shutdown
		 	 */
			dmahw->dmactrl = 0;					/* disable dma */
			sink = (int)dmahw->dmaicr;			/* get dma irq status */
			sink &= ~DMA_IRQ_ENAB;				/* clear irq enable */
			dmahw->dmaicr = sink;				/* clear dma irq */
		}
#endif

#ifdef VME620
		HW(control) = VMEIRQ_DIS;				/* MUST toggle the enable */
#endif

#if 0
#if defined (VME147) && !defined (USEDMA)
		HW(address_reg) = STATUS;				/* clear irq */
		devstat = HW(reg_file);
#endif
#endif

	/* if necessary wake up the current command owner */
		if (dowake) {
			_fev_set (cnt->event_id, CBLOCKWAKE, FALSE );
			lst->curr_cmd = &lst->cntb[SELFID];   /* place for messages */
			lst->busfree = TRUE;
		}

		/* if bus is free, start next command off if one ready */
		if( lst->busfree ) {
			i = 8;
			while( --i >= 0 ) {
				if( i == SELFID )
					continue;				/* my own ID is not active */

				cnt =  &lst->cntb[i];		/* set pointer to the control block */
	
				/* if a command is waiting, start it up */			
				if( ((sink = cnt->cbk_status) & CBK_VALID) == CBK_VALID)
					if( !(sink & CBK_SUBMIT ) ) {
						kick( lst, cnt );	/* start new command */
						break;
					}
			}
		}

#if defined (USEDMA) && defined (VME147)
		dmastat = dmahw->dmaicr;	/* refresh local copy of DMA status */
#endif
		devstat = HW(address_reg);	/* new copy of 33c93 status */

#ifdef DMAIRQ
	} while( ((devstat & AUX_IRQ) == AUX_IRQ) || ((dmastat & DMA_IRQ) == DMA_IRQ));
#else
	} while( (devstat & AUX_IRQ) == AUX_IRQ );
#endif

#ifdef VME620	
	HW(control) = VMEIRQ_DIS;				/* MUST toggle the enable */
	HW(control) = VMEIRQ_EN;				/* reenable irqs for next phase */
#endif

	return 0;								/* record that irq serviced */
}



#ifdef VME147
/*
 *  If the board being used is a Motorola MVME147 then we include this
 *  code to track the SCSI RESET interrupt.
 *
 *		Call with:
 *			lst, low-level static storage
 *		Return:
 *			TRUE to indicate interrupt is _not_ from wd33c93
 *			FALSE to indicate interrupt _is_ from wd33c93
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int chk_reset
(
	Lstat lst
)
#else
int chk_reset( lst )
Lstat lst;		
#endif
{
	register u_char *scsireset = (u_char *)SCSI_ICR; /* PCC SCSI int status */
	register u_char rstdat;
	extern void popdebug();	/* call to system debuger, code in irqasm.a */
	
	if( ((rstdat = *scsireset) & SCSI_ICR_REST) ) {
		/* if SCSI RESET has occured... */
		lst->scsiresets++;							/* one more occurance */
#ifdef INTTRAP
		popdebug(lst);								/* go into debugger */
#endif
		/* clear the reset (leaving the enable and interrupt level intact */
		*scsireset = (rstdat & SCSI_ICR_CLR) | SCSI_ICR_REIP;

		return FALSE;								/* it was our interrupt! */
	} else
		return TRUE;								/* not our interrupt! */
}

#ifdef _UCC
_asm(" use ./irqasm.a");
#else
@ use ./irqasm.a
#endif

#endif
