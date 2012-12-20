/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1998 by Microware Systems Corporation              |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, modification, distribution, or creation of    |
|  derivative works in any form to any party other than the Licensee is     |
|  strictly prohibited, unless expressly provided for under the terms of a  |
|  mutually executed written license agreement for this software between    |
|  the end-user and Microware Systems Corporation, or otherwise expressly   |
|  authorized in writing by Microware Systems Corporation.  Licensee shall  |
|  reproduce this copyright notice exactly as provided herein on any copies |
|  of the software that Licensee is authorized to distribute.               |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   yy/mm/dd Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
| 62  01/01/31 Change v_port mask from bottom nibble to bottom byte    mgh  |
|                This edition history added                            mgh  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/
/* ____________________________ init.c ____________________________
 *
 *  This module contains init(), the WD33C93 scsi controller module's
 * low-level device driver initialization routine.  
 *
 * init() is called from the high-level scsi initialization routine 
 * through a device descriptor's (e.g. h0 or mt0) DevCon field.
 * 
 * Note: scsiglue.a essentially sets init() as this program module's 
 * entry point.
 */
 
/*
 * Copyright 1988, 1989, 1990, 1992, 1993, 1995 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	"scsi33c93.h"

extern int exec (procid	*proc, Datmod dmod, Cmdblk cmd, 
				 int scsiflags, int llcmd); /* in exec.c */
extern int term (mod_dev *dd, Datmod dmod); /* in term.c */
extern int irqsvc(); 	/* in irqsvc.a (object code in scsiglue) */
extern void wd_command (int cmd, Lstat lst, Hardware hw); /* in misc.c */
            
           
/*
 * Low-level device driver initialization 
 *
 * The first time this routine is called it creates a data module
 * used by high-level device drivers and by other routines in
 * the low-level device driver.  Elements in the data module are
 * then initialized, the WD33C93 SCSI controller is added to the 
 * system irq polling routine, and the WD33C93 registers are 
 * initialized.
 *
 * Subsequent calls to this routine simply link the data module. 
 *
 *		Call with:
 *			dd, 	pointer to device descriptor 
 *			pdmod, 	pointer to data module pointer
 *		Return:
 *			error code
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int init 
(
	mod_dev		*dd,
	Datmod		*pdmod
)
#else
int init (dd, pdmod)
mod_dev     *dd;    
Datmod      *pdmod; 
#endif
{
	Lstat		lst;		/* low level static storage */
	Datmod		dmod;		/* pointer to data module info */
	int			error;		/* the error number holder */
	int			portaddr;	/* hold adjusted address here */
	u_char     	selfid = SELFID;  /* our SCSI ID */
	Hardware	hw;			/* pointer to the wd33c93 registers */
	void        *dat_hdr;	/* pointer to data module header */
	int         sink;		/* put unused results here */
	int         old_mask;	/* save of 68xxx irq level */
	char		name[12];	/* used to build data mod & event names */

#if defined (USEDMA) && defined (VME147)
	Dmahardware dmahw;		/* pointer to dma registers */
#endif 

	/*
	 * build the data module name:
	 * the name is the device port address "ANDED" with 0xffffff00.
	 * e.g. for a device at 0xfffe4000, the name is "datfffe4000".
	 */	
	portaddr = ((int) rbfstat.v_sysio.v_port) & (0xffffff00); 

#if defined (USEDMA) && defined (VME147)
	/* test the desc address for 147s */
        if (portaddr != WD93Base)
            return EOS_HARDWARE;
#endif

	strcpy (name, "dat");			/* get prefix */
	lxtoa (&name[3], portaddr);		/* build address part */
	
	/* attempt to create data module */
	error = _f_datmod (sizeof (struct datmod), MA_REENT<<8,
						LLMODACCESS, name,
						&sink, &sink, &sink, pdmod, &dat_hdr);

	/*
	 * if no error, this is a "first call" (the data module
	 * was just created) to the scsi port.
	 *
	 * the low level module needs to:
	 *  - initialize data module values in the data module
	 *    (F$DatMod defaults all entries to zero).
	 *  - create data module free/busy event
	 *  - create irq service event
	 *  - add hardware device(s) to IRQ polling system
	 *  - arm hardware device(s) for interrupts
	 *
	 * if error, and error is "known module", then this is
	 * a "non-first call" to the low-level module.  The data
	 * module, event(s) and irq initialization have already
	 * been setup, so the new user needs only to record
	 * itself in the data module.
	 */
	
	
	if (error == 0) {
		/* "first-call" to low-level module */
		dmod = *pdmod;					/* get address of data module info */
		dmod->dm_modhdr = dat_hdr;		/* set the module's header ptr */
		dmod->dm_init = init;			/* set init function pointer */
		dmod->dm_exec = exec;			/* set exec function pointer */
		dmod->dm_term = term;			/* set terminate routine */
		dmod->dm_users++;				/* count new user */
		
		/* get memory for "low-level statics" for this module */
		if (error = _f_srqmem (sizeof (struct lstat), 
									&dmod->dm_statsize, &dmod->dm_statics))
			return error;

		
		/* setup local statics ptr & clear it to zeros */
		lst = dmod->dm_statics;					/* point at statics */
		clearmem (lst, dmod->dm_statsize);		/* zero it */

#ifdef RECORD
		lst->statcount = 0;				/* no stati stored */
		lst->nxtstat = &lst->stats[0];	/* next status in pointer */
#endif

		/*
		 * form irq mask level:
		 * this mask is used by this module to "mask irqs" to
		 * the assigned irq level of the wd33c93 during critical
		 * sections.
		 */
		lst->mask_level = (dd->_mirqlvl << 8);
		
		/*
		 * set pointer for data module in low-level statics, so
		 * that irq routine can access data module.
		 */
		lst->dmptr = dmod;				/* record data module ptr */

		/*
		 * name conventions for events:
		 * event names are built in the form 
		 * "c<portaddress>", where:
		 *
		 * c =  'i' for interrupt service event.
		 * <portaddress> is the name of the device port
		 * address "ANDED" with 0xfffffff0.  Added to the
		 * end is an '_x' where x is the SCSI address of the
		 * target device.
		 *
		 * e.g. for a device at 0xfffe4000, the event
		 * names is:
		 *
		 *          irq event:  "ifffe4000_x"
		 */

#ifdef TARGET
		/* create event for OWNID if nessesary.
		 *	This will come into play if the driver is extended to include
		 *	a target mode of operation.
		 **/
		if( lst->cntb[SELFID].event_id == 0 ) {
			name[0] = 'i';
			lxtoa(&(name[1]), portaddr);
			name[9] = '_';
			name[10] = '0' + SELFID;						/* for us */
			name[11] = 0;
			if( (error = _fev_creat(0, -1, 1, name, &lst->cntb[SELFID].event_id,
			                                                            &sink)))
				return error;
		}
#endif 

		/* add the device to the system irq polling routine */
		if (error = _f_irq(dd->_mvector, dd->_mpriority,
					irqsvc, lst, portaddr))
			return error;

#if defined (USEDMA) && defined (VME147)
		/* init dma hardware ptr */
		dmahw = (Dmahardware)DMA_ADDR;			/* set dma address */
		lst->dmabase = dmahw;					/* save in statics */

#ifdef DMAIRQ		
		/*
		 * add dma device to the system irq polling routine.
		 * NOTE that the vector generated by the PCC for scsi
		 * dma interrupts is 1 greater than the scsi device
		 * itself.
		 */
		if (error = _f_irq(dd->_mvector + 1, dd->_mpriority,
					irqsvc, lst, portaddr))
			return error;
#endif 
#endif 

		/* initialize the wd33c93 */
		hw = (Hardware) portaddr;	/* address of the wd33c93 */
		lst->hdware = hw;
#ifdef VME620
		HW(int_vect) = dd->_mvector;			/* set vector in hardware */
		HW(dma_init) = DMAINIT;					/* init dma device */
		HW(dma_control) = DMA_DMAOFF;			/* ensure dma disabled */
#endif
		/* reset any possible pending interrupts in device */
		if (HW(address_reg) & AUX_IRQ) {
			/* interrupt pending - clear it */
			HW(address_reg) = STATUS;			/* point at status reg */
			sink = HW(reg_file);				/* clear interrupt */
		}
		
		/*
		 * initialization requirements:
		 *  - setup "selfid" in device
		 *  - Advanced features enable bit set to differentiate between chips
		 *  - perform any "one-time" initialization desired
		 */
		HW(address_reg) = OWNID;			/* point to "ownid" register */
		HW(reg_file) = OI_DEFAULT | selfid;	/* set "own id" on scsi bus */
		old_mask = mask_irq(lst->mask_level);	/* mask interrupts */
#ifdef VME620
		/* ensure that VME620 will allow irq to be visible */
		HW(control) = VMEIRQ_DIS;
#endif				
		wd_command(RESET, lst, hw);			/* issue reset command */
		
		/* wait for wd33c93 to acknowledge reset command */
		while ((HW(address_reg) & AUX_IRQ) == 0);	/* wait for interrupt */
		
#ifdef VME620
		/* ensure that VME620 will allow irq to propagate */
		HW(control) = VMEIRQ_DIS;			/* MUST toggle to arm irqs */
#endif
		/* check that wd33c93 reset ok */
		HW(address_reg) = STATUS;			/* read status to clear irq */
		sink = HW(reg_file);
		if( sink == GP0_RESETA )
			lst->apart = TRUE;				/* wd33c93 Advanced Mode Enabled */
		else if( sink == GP0_RESET )
			lst->apart = FALSE;				/* Advanced Mode not enabled */
		else
#if defined (__STDC__) || defined (_ANSI_EXT)
			error = EOS_HARDWARE;			/* can't reset device */
#else
			error = E_HARDWARE;				/* can't reset device */
#endif
		
		mask_irq(old_mask);		/* unmask interrupts */

		/* check if hardware error from device itself */
		if (error != SUCCESS)
			return error;

#ifdef VME147		
		/* enable interrupts from the wd33c93 via the pcc - MVME147 */
		*((char *) SCSI_ICR) = (dd->_mirqlvl & SCSI_ICR_MASK) 
								| SCSI_ICR_ENAB;
#ifdef USEDMA			
		/* enable dma device interrupts */
		old_mask = mask_irq (lst->mask_level);	/* mask irqs */
		dmahw->dmactrl = 0;						/* disable dma device */
		dmahw->dmaicr = DMA_IRQ;				/* clear any pending irqs */
		mask_irq(old_mask);						/* unmask irqs */
		
		/*
		 * setup dma irq level:
		 * this version uses the same irq level for dma
		 * as the wd33c93.
		 */
		dmahw->dmaicr = dd->_mirqlvl;
#endif 
#endif 
		lst->busfree = TRUE;				/* bus is not active */

	} else {
		/* error from data module creating, check if module exists */
#if defined (__STDC__) || defined (_ANSI_EXT)
		if (error == EOS_KWNMOD) {
#else		
		if (error == E_KWNMOD) {
#endif
			/* data module already exists, simply link it */
			if( !(error = _f_link(0, name, &sink, &sink, &sink,
			                                  pdmod, &sink)) ) {
				dmod = *pdmod;			/* get address of data module info */
				dmod->dm_users++;		/* count new user */
			}
		}
	}
	return error;
}
