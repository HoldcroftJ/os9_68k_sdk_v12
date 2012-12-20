/*
 * term - terminate use of the ncr5380 low level module	.
 */

/*
 * Copyright 1988, 1989, 1990, 1991 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee solely
 * for documentation and educational purposes. Reproduction, publication
 * or distribution in any form to any party other than the licensee is
 * strictly prohibited.
 */

#include	"scsi5380.h"

/*
 * term: terminate use of low level driver
 */
int term(dd, dmod)
mod_dev					*dd;		/* device descriptor */
Datmod					dmod;		/* data module */
{
	register Lstat		lst;		/* low level statics */
	register int		portaddr;	/* temporary holder for port addr */
	register error_code error;      /* the error code */
	int					sink;		/* put junk here */
	Hardware			hw;			/* pointer to 5380 scsi chip */
#ifdef USEDMA
#if (defined(JNMS) || defined(PCB1782))
	Dmahardware			dmahw;		/* ptr to dma hardware */
#endif
#endif
	char				name[12];	/* build event names here */

	error = SUCCESS;			/* until proven otherwise */
	lst = dmod->dm_statics;		/* get pointer to stats */
	dmod->dm_users--;			/* decrement user count */
	
	/* unlink the data module access event  */
	if( dmod->dm_devent )
		if (sink = _fev_unlnk(dmod->dm_devent))
			error = sink;	/* keep the error */

#if (defined(JNMS) || defined(PCB1782))
	/* unlink the IRQ event  */
	if( dmod->dm_irqevt )
		if (sink = _fev_unlnk(dmod->dm_irqevt))
			error = sink;	/* keep the error */
#endif

#ifdef PCB1782
	/* unlink the dma IRQ event  */
	if( lst->v_dmaevt ) 
		if (sink = _fev_unlnk(lst->v_dmaevt))
			error = sink;	/* keep the error */
#endif

	/* if last user of data module, disable hardware devices */
	if (dmod->dm_users == 0) {
		/* get scsi controller address and mask SCSI ID bits (ed.7) */
		hw = (Hardware)((long)rbfstat.v_sysio.v_port & SCSIIDMASK);
		
#if ~(defined(JNMS) || defined(PCB1782))
		HW(MODEREG) = 0;				/* ensure not in DMA mode */
		sink = HW(RESET);				/* clear interrupts 	 */
		HW(INITCMND) = 0;		/* negate: RST, TestMode, DiffEnab, ACK */
								/*         BSY, SEL, ATN, DataBus */
#endif

#ifdef USEDMA
#ifdef JNMS
		/* remove the DMA monitor alarm */
		if( lst->v_monAID )
			_f_delete_alarm( lst->v_monAID );
		sink = HW(RESET);				/* clear interrupts */
#endif
#endif
		/* delete the data module access event */
		portaddr = ((int) rbfstat.v_sysio.v_port) & SCSIIDMASK;
		strcpy (name, "evt");
		lxtoa (&name[3], portaddr);
		if (sink = _fev_delet(name, &sink))
			error = sink;					/* keep the error  */
		
		/* remove the device from the irq polling system  */
		if (sink = _f_irq(dd->_mvector, dd->_mpriority, 0, lst, 0))
			error = sink;					/* keep the error  */

#ifdef USEDMA
#ifdef PCB1782
		/* remove the dma device from the irq polling system  */
		if (sink = _f_irq(DMA_VECT, dd->_mpriority, 0, lst, 0))
			error = sink;					/* keep the error  */

		/* delete the dma IRQ event  */
		strcpy (name, "dma");
		lxtoa (&name[3], portaddr);
		if (sink = _fev_delet(name, &sink))
			error = sink;					/* keep the error  */
#endif
		/* delete the IRQ event  */
		strcpy (name, "irq");
		lxtoa (&name[3], portaddr);
		if (sink = _fev_delet(name, &sink))
			error = sink;					/* keep the error  */
#endif
		/* return low-level statics */
		if (lst != NULL)
			error = _f_srtmem(dmod->dm_statsize, lst);
	}
	
	/* unlink the data module  */
	if (sink = _f_unlink(dmod->dm_modhdr))
		error = sink;						/* keep the error  */

	return error;							/* return any/latest error  */
}
