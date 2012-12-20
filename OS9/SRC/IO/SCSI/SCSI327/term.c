/*
 * term - terminate use of the vme327 low level module
 */

/*
 * Copyright 1990 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include    "scsi327.h"
#include	<sbf.h>
#include    <sg_codes.h>

extern void makeevname();			/* routine to make an event name */

/*
 * term () - terminate use of low level driver
 */
 
int term (dd, dmod)
mod_dev					*dd;		/* device descriptor */
Datmod					dmod;		/* data module */
{
	register int error;		/* the error code */
	register Lstat lst;		/* low level statics */
	register int portaddr;	/* temporary holder for port address */
	int evid,				/* event id of target */
	    sink,				/* put junk here */
	    i;					/* loop counter */
	char name[16];			/* build event names here */
	struct rbf_opt *rbopt;		/* device descriptor options for RBF */
	struct sbf_opt *sbopt;		/* device descriptor options for SBF */
	

	/* get pointer to statistics and exit if none present */
	if( (lst = dmod->dm_statics) == NULL )
		return 0;

	portaddr = (int)lst->hdware & ~0xf;	/* port address */
	error = 0;							/* default no error */
	evid = 0;							/* insure that next switch works */
	
	/* unlink the IRQ event for the caller's device */
	switch ( dd->_mdtype ) {

		case DT_RBF:		
			rbopt = (struct rbf_opt *)&dd->_mdtype;
			evid = lst->sc_events[(i = rbopt->pd_ctrlrid)];
			break;
		
		case DT_SBF:
			sbopt = (struct sbf_opt *)&dd->_mdtype;
			evid = lst->sc_events[(i = sbopt->pd_scsiid)];
			break;
		
		default:
#ifdef __STDC__
			error = EOS_PARAM;
#else
			error = E_PARAM;
#endif
			break;
	}

	if( evid ) { 	/* if the event exists, delete it */
		makeevname( name, portaddr, i);
		_fev_unlnk (evid);		/* unlink the event */
		_fev_delet (name, &sink);		/* now remove the event */
		lst->sc_events[i] = 0;			/* remove record of existance */
	}
	
	/* if last user of data module, clean up */
	if (--dmod->dm_users == 0) {

		if( lst->sc_chanel.valid_flag ) {
			/* delete the vme327 channel for the SCSI devices */
			docmd( portaddr, DELETE_CHAN, lst, &lst->sc_chanel);
		}

		/* remove the device from the irq polling system */
		_f_irq (dd->_mvector, dd->_mpriority, 0, lst, 0);

  		/* return low-level statics */
		error = _f_srtmem (dmod->dm_statsize, lst);
		dmod->dm_statics = 0;			/* only return the memory once */
	}

	/* unlink the data module if one is present  */
	if( dmod->dm_modhdr ) {
		if (sink = _f_unlink (dmod->dm_modhdr))
			return sink;
	}
	else
		return error;							/* return any/latest error */
}
