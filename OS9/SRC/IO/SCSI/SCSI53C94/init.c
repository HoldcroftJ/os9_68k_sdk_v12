/*
 * init - init routine for ncr53c94 low level scsi module
 */
 
/*
 * Copyright 1992 by Microware Systems (UK) Limited
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	"scsi53c94.h"

/* 
 * declarations of exec, term, and irqsvc routines 
 */
 
int	exec(), term(), irqsvc(), dmasvc();

/*
 * int init () - initialize low level driver 
 */
 
int init (dd, pdmod)
mod_dev		*dd;			/* path descriptor */
Datmod		*pdmod;			/* pointer to pointer to data module */
{
	register Lstat		lst;		/* low level static storage */
	register Datmod		dmod;		/* pointer to data module info */
	register int		error;		/* the error number holder */
	register int		portaddr;	/* hold adjusted address here */
	register u_char     selfid = SELFID;  /* our SCSI ID */
	register Hardware	hw;			/* pointer to the ncr53c94 registers */
	void                *dat_hdr;	/* pointer to data module header */
	int                 sink;		/* put unused results here */
	int                 old_mask;	/* save of 68xxx sr */
	char				name[12];	/* used to build data mod & event names */
#ifdef USEDMA
	Dmahardware         dmahw;		/* pointer to dma registers */
#endif USEDMA


	/*
	 * build the data module name:
	 * the name is the device port address "ANDED" with 0xfffffff0.
	 * e.g. for a device at 0xfffe4000, the name is "datfffe4000".
	 */	

	portaddr = ((int) rbfstat.v_sysio.v_port) & (~0xf);
	strcpy (name, "dat");			/* get prefix */
	lxtoa (&name[3], portaddr);		/* build address part */
	
	/* attempt to create data module */
	error = _f_datmod (sizeof (struct datmod), MA_REENT<<8,
						MP_OWNER_READ+MP_OWNER_WRITE, name,
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
		 * form (68xxx) status register image:
		 * this image is used by this module to "mask irqs" to
		 * the assigned irq level of the ncr53c94 during critical
		 * sections.
		 */
		lst->mask_level = (dd->_mirqlvl << 8) | SUPSTAT;
		
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
#endif TARGET

		/* add the device to the system irq polling routine */
		if (error = _f_irq(dd->_mvector, dd->_mpriority,
					irqsvc, lst, portaddr))
			return error;

#ifdef USEDMA
		/* init dma hardware ptr */
		dmahw = (Dmahardware)DMA340;			/* set dma address */
		lst->dmabase = dmahw;					/* save in statics */

#ifdef DMAIRQ		
		/*
		 * add dma device to the system irq polling routine.
		 * NOTE that vector is defined in the local dma/device
		 * .h files, whilst the priority is taken from the
		 * device descriptor of the calling device.
		 */
		if (error = _f_irq (DMA340VECT, dd->_mpriority,
					irqsvc, lst, portaddr))
			return error;
#endif DMAIRQ
#endif USEDMA

		/* initialize the ncr53c94 */
		hw = (Hardware) portaddr;	/* address of the ncr53c94 */
		lst->hdware = hw;
		
		/*
		 * initialization requirements:
		 *  - setup "selfid" in device
		 *  - perform any "one-time" initialization desired
		 */
		HW(command) = RESET_CHIP;			/* issue reset command */
		delay();
		HW(command) = NOP;					/* terminate reset command */
		delay();
/*		HW(config1) = selfid;				/* set "own id" on scsi bus */
		HW(config1) = selfid + CR1_SRID;	/* set "own id" on scsi bus */
		delay();
		HW(config2) = CR2_EPL;				/* enable parity latch */
		delay();
		HW(config3) = CR3_THRESH8;			/* enable DMA threshold 8 */
		delay();
		HW(clkconv) = CLK_20;				/* 16 MHz clock */
		delay();
#ifdef SCSIRESET
		HW(command) = RESET_SCSI;			/* reset SCSI bus *!*!*!*/
#else
		HW(command) = NOP;					/* nothing */
#endif
		delay();
		
		/* check if hardware error from device itself */
		if (error != SUCCESS)
			return error;

#ifdef USEDMA			
		/* enable dma device interrupts */

		dmahw->dh_intv = DMA340VECT;
		
		/*
		 * setup dma irq level:
		 * this version uses the same irq level for dma
		 * as the ncr53c94.
		 */
		dmahw->dh_intl = dd->_mirqlvl;
#endif USEDMA
	 	lst->busfree = TRUE;				/* bus is not active */

	} else {
		/* error from data module creating, check if module exists */
		if (error == E_KWNMOD) {
			/* data module already exists, simply link it */
			if( !(error = _f_link(0, name, &sink, &sink, &sink,
			                                             pdmod, &sink)) ) {
				dmod = *pdmod;				/* get address of data module info */
				dmod->dm_users++;			/* count new user */
			}
		}
	}
	return error;
}
