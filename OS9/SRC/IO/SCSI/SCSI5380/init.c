/*
 * init - init routine for ncr5380 low level scsi module.
 */

/*
 * Copyright 1988, 1989, 1990, 1991, 1993 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee solely
 * for documentation and educational purposes. Reproduction, publication
 * or distribution in any form to any party other than the licensee is
 * strictly prohibited.
 */

#include	"scsi5380.h"

extern	Self_ID(),
		bname();
 
/*
 * init: initialize low level driver.
 */
int init(dd, pdmod)
mod_dev		*dd;					/* device descriptor pointer*/
Datmod		*pdmod;					/* pointer to pointer to data module*/
{
	register Lstat		lst;		/* low level static storage	*/
	register Datmod		dmod;		/* pointer to data module info */
	register int		error;		/* the error number holder	*/
	register int		portaddr;	/* hold adjusted address here */
	register Sdata		sdmod;		/* self data module structure ptr */
	Hardware			hw;			/* pointer to the ncr5380 registers	*/
	auto void			*dat_hdr;	/* pointer to data module header */
	auto int			sink;		/* put unused results here	*/
	char				name[12];	/* used to build data mod & event names */
	char				sdname[32];	/* used to self data module name */
#ifdef USEDMA
#if defined(JNMS) || defined(PCB1782)
	auto Dmahardware	dmahw;		/* pointer to dma registers */
#endif
#if defined(JNMS)
	REGISTERS regs;					/* register set for _f_set_alarm */
#endif
#endif

	/*
	 * build the data module name:
	 * the name is the device port address "ANDED" with 0xfffffff0.
	 * e.g. for a device at 0xfffe4000, the name is "datfffe4000".
	 */	
	portaddr = ((long)rbfstat.v_sysio.v_port) & SCSIIDMASK;  /* (*ed.7) */
	strcpy(name, "dat");			/* get prefix */
	lxtoa(&name[3], portaddr);		/* build address part */
	
	/* attempt to create data module */
	error = _f_datmod(sizeof(struct datmod), MA_REENT<<8,
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
		if (error = _f_srqmem(sizeof(struct lstat), 
									&dmod->dm_statsize, &dmod->dm_statics))
			return error;	/* abort if no memory */
		
		/* setup local statics ptr & clear it to zeros */
		lst = dmod->dm_statics;					/* point at statics */
		clearmem(lst, dmod->dm_statsize);		/* zero it */

		/*
		 * form (68xxx) status register image:
		 * this image is used by this module to "mask irqs" to
		 * the assigned irq level of the ncr5380 during critical
		 * sections.
		 */
		lst->v_mask_level = (dd->_mirqlvl << 8);
		
		/* set pointer for data module in low-level statics, so
		 * that irq routine can access data module.
		 */
		lst->v_dmptr = dmod;		/* record data module pointer */

		/*
		 * name conventions for events:
		 * event names are built in the form 
		 * "nam<portaddress>", where:
		 *
		 * nam = 'evt' for data module access event.
		 *       'irq' for interrupt service event.
		 * <portaddress> is the name of the device port
		 * address "ANDED" with 0xfffffff0.
		 *
		 * e.g. for a device at 0xfffe4000, the event
		 * names are:
		 *
		 * data module access:  "evtfffe4000"
		 *        irq service:  "irqfffe4000"
		 *        dma service:  "dmafffe4000"
		 */

		/* create event for data module "free/busy" */
		strcpy(name, "evt");
		lxtoa(&(name[3]), portaddr);
		if (error = _fev_creat(1, -1, 1, name, &dmod->dm_devent, &sink))
			return error;	/* abort if error */

#if defined(JNMS) || defined(PCB1782)
		/* create event for scsi irq occurred */
		strcpy(name, "irq");
		lxtoa(&(name[3]), portaddr);
		if (error = _fev_creat(0, -1, 1, name, &dmod->dm_irqevt, &sink))
			return error;	/* abort if error */
		
		/* 
		 * copy irq event id from data module to low-level static
		 * so that irq routine can access it.
		 */
		lst->v_irqevt = dmod->dm_irqevt;
		
#ifndef JNMS
		/* create event for dma irq occurred */
		strcpy(name, "dma");
		lxtoa(&(name[3]), portaddr);
		if (error = _fev_creat(0, -1, 1, name, &lst->v_dmaevt, &sink))
			return error;		/* abort if error */
#endif
#endif
		/*
		 * Attempt to link to the "self data module" to get the SCSI ID for
		 * this system (SELFID).  If no data module exists, the SCSI SELF ID
		 * defaults to the value found in the "Self_ID" variable. (7)
		 */
		strcpy(sdname, bname);
		strcpy(&sdname[strlen(bname)], ".data");
		if (_f_link(0, sdname, &sink, &sink, &sink, sdmod, &sink) != SUCCESS)
			lst->v_selfid = *((u_char *) ((void *)Self_ID));
		else lst->v_selfid = sdmod->sd_selfid;
		
		/* get base address of hardware and mask off the ID bits (ed.7) */
		hw = (Hardware)((long)rbfstat.v_sysio.v_port & SCSIIDMASK);

		/* add the device to the system irq polling routine */
		if (error = _f_irq(dd->_mvector, dd->_mpriority, irqsvc, lst, hw))
			return error;	/* abort if error */

/*
 * HARDWARE AND DMA INITIALIZATION 
 */
#ifdef JNMS
		/* initialize vector registers for JNMS player */
		*((u_int16 *) HDDINTSET) = ((0xF8 | dd->_mirqlvl) << 8) | dd->_mvector;
#endif		
#ifdef USEDMA
#ifdef JNMS
		/* initialize DMA hardware pointer */
		lst->v_dmabase = (Dmahardware) DMA_DEVICE;	/* save dma address */

		clearmem( &regs, sizeof(regs) );		
		if( (lst->v_monAID = _f_set_alarm(HALF_SECOND, &regs, lst )) < 0 )
			return -(lst->v_monAID);	/* error setting alarm */
#endif
#ifdef PCB1782
		/* initialize DMA hardware pointer */
		lst->v_dmabase = (Dmahardware) (DMA_DEVICE + (CHNLSIZE * DMACHNL));

		/* add the DMA device to the system irq polling routine */
		if (error = _f_irq(DMA_VECT, dd->_mpriority, dmasvc, lst, hw))
			return error;	/* abort if error */
#endif
#endif
		/* reset the ncr5380 and SCSI bus */
		scsi_Reset( hw, lst );

	/*
	 * if a "known module" error occurred when creating the data module,
	 * then the data module already exists.  Simply link to it.
	 */
	} else {
		if (error == E_KWNMOD)  {
			/* module exists, attempt to link */
			if( !(error = _f_link(0, name, &sink, &sink,
			                                      &sink, pdmod, &sink)))  {
				dmod = *pdmod;		/* get address of data module info */
				dmod->dm_users++;	/* count new user */
			}
		}
	}
	return error;
}


/*
 *  Time waster, for when chip needs time to act
 */
delay(delval)
register int	delval;
{
	while (--delval >= 0)
		short_delay();			/* must do something */
}

/*
 *  scsi_Reset() - send a SCSI RESET to the Bus and recondition the 5380.
 *      (created from inline code above Ed.40)
 */
int scsi_Reset( hw, lst )
register Hardware hw;
register Lstat lst;
{
	register int old_mask;
	int sink;
	
	old_mask = mask_irq(lst->v_mask_level);	/* mask interrupts */
	HW(MODEREG) = 0;	/* clear mode reg, dsbl parity checking & irqs */
	sink = HW(RESET);	/* clear IRQs, reset parity and busy error bits */
	HW(INITCMND) = 0;	/* negate RST,TM,DE,ACK,BSY,SEL,ATN*/
	HW(SCSIWDAT) = 0;	/* clear ID bits (none of them set)*/
	HW(MODEREG) = 0;	/* clear mode register, disable parity */
						/* checking & interrupt		 */
	HW(TARGCMND) = 0;	/* clear target command register*/
						/* (Bus Free Phase)			 */
	HW(SELENABL) = 0;			/* clear Select Enable Register */
	HW(INITCMND) |= RESET_BT;	/* assert reset line */
	delay(-1);					/* delay a very short time*/
	HW(INITCMND) &= ~RESET_BT;	/* negate reset line */
	delay(RESDELAY);			/* Wait for half a second*/
	HW(INITCMND) |= A_DBUS;		/* assert data bus	 */
	sink = HW(RESET);			/* clear 5380 interrupts*/
	mask_irq(old_mask);			/* unmask interrupts */
	return 0;
}

#ifndef JNMS
#ifdef _UCC
_asm("Self_ID: dc.b 7,0");
#else
@Self_ID: dc.b 7,0	/* Default SCSI self ID (plus alignment nullbyte) */
#endif
#else JNMS
#ifdef _UCC
_asm("Self_ID: dc.b 5,0");
#else
@Self_ID: dc.b 5,0	/* Default SCSI self ID (plus alignment nullbyte) */
#endif
#endif JNMS

