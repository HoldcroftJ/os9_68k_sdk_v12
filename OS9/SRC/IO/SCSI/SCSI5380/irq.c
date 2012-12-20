/*
 * irq - interrupt routines for ncr5380 low level module.
 */

/*
 * Copyright 1988, 1989, 1990, 1991 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	"scsi5380.h"

/*
 * irq: service routine for 5380 interrupt
 */
int irq(lst, hw)
register Lstat		lst;			/* low-level static storage */
register Hardware 	hw;				/* ncr5380 hardware ptr */
{
	register char	sink;			/* junk collector */
#ifdef USEDMA
#ifdef JNMS
	register Dmahardware dma;		/* dma hardware pointer */
	register int dmastat;			/* image of the dma status register */
	register int wake;				/* flag to awaken process */
	
	wake = TRUE;		/* unless intermediate disconnect, wake up driver */

	if ( ((sink = HW(BUSSTAT)) & INT_ACT) != 0) {	/* IFF our interrupt */
		if( (sink & ~(INT_ACT | PMATCH)) == 0 ) {	/* look for valid status */
			if( (HW(STATREG) & ~RST_BT) == 0 ) {	/* IFF all 0, SCSI reset */
				lst->v_SCSIResCnt++;				/* increment reset count */
				lst->v_SCSIReset = TRUE;			/* flag reset has occured */
				if( !lst->v_CmdActive )
					wake = FALSE;		/* no command active, inhibit signal */
			}
		} else {	/* our interrupt, check for DMA active but not done. */
			/* may be intermediate disconnect (from non-standard )  */
			dma = lst->v_dmabase;
			if( (dmastat = dma->dh_status) & ACTIVECH )	/* DMA is active */
				if( !(dmastat & OPERCOMP ) )	/* if no process active */
					wake = FALSE;				/* inhibit wake-up */
		}

		sink = HW(RESET);					/* clear interrupt status */
		if( wake )
			_fev_signl(lst->v_irqevt, 0);	/* wake current process */
		return SUCCESS;

	} else {
		lst->v_NotMeCnt++;	/* keep count of how often this happens */
		return E_NOTME;		/* not our irq, return error. */
	}
}

#endif JNMS

#ifdef PCB1782
	/* if it was a 5380 interrupt, process it as such */
	sink = HW(BUSSTAT);
	if ((HW(BUSSTAT) & INT_ACT) != 0) {
		sink = HW(RESET);				/* reset interrupt register */

	/* if not our irq, return error */
	} else return E_NOTME;

	/* signal the waiting process that an irq has occurred */
	_fev_signl(lst->v_irqevt, 0);
	return SUCCESS;
}
#endif PCB1782

#else
	/* Non-DMA versions of the driver use this */
	/* if it was a 5380 interrupt, process it as such */
	if ((HW(BUSSTAT) & INT_ACT) != 0) {
		lst->v_phasechg = TRUE;	/* signal a phase change occurred	*/
		sink = HW(RESET);		/* reset interrupt register	 */
	} else return E_NOTME;

	return SUCCESS;
}
#endif ( else case to USEDMA )

#ifdef USEDMA
#ifdef PCB1782
/*
 * dmairq: service routine for dma interrupt.
 */
int dmairq(lst, hw)
register Lstat		lst;			/* low-level static storage */
register Hardware 	hw;				/* ncr5380 hardware ptr */
{
	int				sink;			/* junk collector */

	if (((lst->v_dmabase)->dh_chlctrl & (1 << INTRPT_B)) != 0) {
		reset_dma(lst);		/* if yes, read DMA registers, clear irq */

	/* if not our irq, return error */
	} else return E_NOTME;

	/* signal the waiting process that an irq has occurred */
	_fev_signl(lst->v_dmaevt, 0);

	return SUCCESS;
}

#ifdef _UCC
_asm(" use irq1782.a");
#else
@ use irq1782.a
#endif

#endif
#endif
