/*
 * term - terminate routine for the rbteac high level driver
 */

/*
 * Copyright 1990, 1991, 1995, 1996 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 *
 *	Edition History:														
 *	#   Date    Comments												By
 *	-- -------- ------------------------------------------------------- ---
 *   00 95/07/12 Added Edition History for this file.                   GbG
 *   01 95/07/12 ANSI Version.                                          GbG
 *               ---- OS-9/68K V3.0.3 Released ----
 *               ---- OS-9/68K V3.1 Released ----
 */

#include    "rbteac.h"
#include	"rbproto.h"

/*
 * int term () - terminate the device 
 */

#if defined (__STDC__) || defined (_ANSI_EXT) 
int term (mod_dev *dd)
#else
int term (dd)
mod_dev     *dd;
#endif
{
	Rbfdrive   			drvptr;		/* drive table pointer */
	struct rbf_opt 		*opt;		/* drive options table */
    int        			error;      /* error holder */
    int        			i;          /* secondary error holder */
    
	
    /* call low-level module to inform it that this driver is finished */
    error = 0;
    if( (long)data_mod != NULL )
	    if (data_mod->dm_term != NULL) {
        	error = (*data_mod->dm_term) (dd, data_mod); /* llvl terminate */
		    _f_unlink (drvr_mod);		    			/* unlink data module */
   		}

	/* if there is an allocated sector 0 buffer, must get rid of it */
	for( i =  0; i < RBF_MAXDRIVE; i++ ) {
		opt = (struct rbf_opt *)&dd->_mdtype;
   		drvptr = &rbfstat.drv[i];
	   	if( drvptr->v_sczero )
			_f_srtmem( scztime[i].s0bsize, drvptr->v_sczero );
	}

    return error;
}
