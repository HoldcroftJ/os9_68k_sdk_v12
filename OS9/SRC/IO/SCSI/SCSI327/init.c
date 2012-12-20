/*
 * init - init routine for the mVme327 low level scsi module
 */
 
/*
 * Copyright 1990, 1991, 1993 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 * history:
 *
 * ed.   date    reason                                           who.
 * --  --------  ---------------------------------------------    ---
 * 01  90/01/25  history starts                                   Rwb
 *               ---- OS-9/68K V2.4 released ----
 * 02  91/01/30  Added debug initialization code.                 Rwb
 * 03  93/02/10  reworked mask_level for MSP support.             wwb
 *               ---- OS-9/68K V3.0 released ----
 * 04  94/04/14  Adopted new error codes for ANSIfication.        ats
 *               ---- OS-9/68K V3.1 released ----
 */

#include "scsi327.h"		/* device specific definitions */
#include <sbf.h>
#include <sg_codes.h>

/* 
 * declarations of exec, term, and irqsvc routines 
 */
 
int	exec(), term(), irqsvc(), dmasvc();
extern char *_f_trans();

/*
 * int init () - initialize low level driver 
 */
 
int init (dd, pdmod)
mod_dev		*dd;			/* path descriptor */
Datmod		*pdmod;			/* pointer to pointer to data module */
{
	register Lstat		lst;		/* low level static storage */
	register int		error;		/* the error number holder */
	register int   		portaddr;	/* hold adjusted address here */
	register CmdChan	ccpt;		/* the command channel pointer */
	register Envelope	ept,tpt;	/* for envelope link up */
	Datmod				dmod;		/* pointer to data module info */
	char                *ptr;		/* general purpose address storage */
	char     			*dat_hdr;	/* pointer to data module header */
	int        			sink,		/* put unused results here */
	        			old_mask;	/* save of 68xxx sr */
	char				name[16];	/* used to build data mod & event names */
	extern void			envlink();	/* routine to link envelope arrays */

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
						LLMODACCESS, name,
						&sink, &sink, &sink, pdmod, &dat_hdr);

	/*
	 * if no error, this is a "first call" (the data module
	 * was just created) to the scsi port.
	 *
	 * the low level module needs to:
	 *  - initialize data module values in the data module
	 *    (F$DatMod defaults all entries to zero).
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
		
		
		/* get memory for "low-level statics" for this module */
		if (error = _f_srqmem (sizeof (struct lstat), 
									&dmod->dm_statsize, &dmod->dm_statics))
			return error;
		
		/* setup local statics ptr & clear it to zeros */
		lst = dmod->dm_statics;					/* point at statics */
		clearmem (lst, dmod->dm_statsize);		/* zero it */
		
		/* data module may not be in local space, and as such we will
		 *   create a relocation constant that will be applied to any
		 *   addresses within the scope of our static storage.
		 */
		if( (long)( ptr = _f_trans( lst, sizeof(struct lstat), TRANS_LOCAL)) == -1 )
#ifdef __STDC__
			return EOS_IBA;		/* could not translate */
#else
			return E_IBA;		/* could not translate */
#endif
		else
			lst->reloc = (long)ptr - (long)lst;   /* the relocation constant */

		/*
		 * form (68xxx) status register image:
		 * this image is used by this module to "mask irqs" to
		 * the assigned irq level of the board during critical
		 * sections.
		 */
		lst->mask_level = (dd->_mirqlvl << 8);

		/* set global level/vector information */
		lst->hdware = (struct hware *)portaddr;
		lst->irqlevel = dd->_mirqlvl;
		lst->irqvector = dd->_mvector;
		
		/* copy of dma_mode from descriptor. */
		switch( dd->_mdtype ) {
		case DT_RBF:		/* rbf descriptor */
			lst->dma_mode = ((struct rbf_opt *)(&dd->_mdtype))->pd_tfm;
			break;
		case DT_SBF:		/* sbf descriptor */
			lst->dma_mode = ((struct sbf_opt *)(&dd->_mdtype))->pd_dmamode;
			break;
		default:	/* unknown type */
#ifdef __STDC__
			return EOS_PARAM;
#else
			return E_PARAM;
#endif
		}
	
		/* must initialize the buffered pipe so that the interrupt service
		 *  will be able to do it's thing
		 */	
		ccpt = &lst->sc_chanel;				/* pointer to the channel */
		if( !ccpt->valid_flag ) {  /* first time for SCSI devices */
			tpt =  &lst->sc_env[0];		/* set pointers */
			ccpt->cmd_head = ccpt->cmd_tail =
			                            (Envelope)((int)tpt++ + lst->reloc );
			ccpt->stat_head = ccpt->stat_tail =
			                            (Envelope)((int)tpt++ + lst->reloc );
			lst->freptr = tpt;		/* set pointer to next free */

			/* now link up the rest of the free blocks */
			error = NEVL - 2;
			while( --error > 0 ) {
				ept = tpt++;			/* set up links within pool */
				ept->link_ptr = tpt;
				ept->env_valid = 0;
			}
			tpt->link_ptr = 0;			/* absolute end is a null packet */
			tpt->env_valid = 0;

			ccpt->chan_prior = 0x01;			/* assign as highest priority */
			ccpt->int_level = lst->irqlevel;	/* set the level and vector */
			ccpt->int_vector = lst->irqvector;
			ccpt->data_width = lst->dma_mode ? DMA_16 : DMA_32;

			/* address modifier for mailbox access */
			ccpt->addr_mod =
			    (((long)lst + lst->reloc) & EXTMSK) ? VME_EXSUPDAT : VME_SUPDAT;
			
			if( docmd( lst->hdware, CREATE_CHAN, lst, ccpt ) < 0 )
#ifdef __STDC__
				return EOS_HARDWARE;			/* cannot create the channel */
#else
				return E_HARDWARE;				/* cannot create the channel */
#endif
		}

		/*
		 * set pointer for data module in low-level statics, so
		 * that irq routine can access data module.
		 */
		lst->dmptr = dmod;				/* record data module ptr */

		/* add the device to the system irq polling routine, abort if error */
		if (error = _f_irq(dd->_mvector, dd->_mpriority,
					irqsvc, lst, rbfstat.v_sysio.v_port))
			return error;

	} else {
		/* error from data module creating, check if module exists */
#ifdef __STDC__
		if (error == EOS_KWNMOD)
#else
		if (error == E_KWNMOD)
#endif
			/* data module already exists, simply link it */
			error = _f_link(0, name, &sink, &sink, &sink, pdmod, &sink);
		else
			return error;			/* could not creat/link to the module */
				
	} 

	dmod = *pdmod;				/* address of primary data module */
	dmod->dm_users++;			/* success means one more user */
	
#ifdef DEBUGDRVR
	lst = dmod->dm_statics;		/* make sure it's set if module already exists */
	lst->ndbgpak = 0;			/* set index into command storage */
#endif
	return 0;					/* all ok! */
}
