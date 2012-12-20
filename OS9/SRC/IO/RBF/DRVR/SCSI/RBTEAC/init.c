/*
 * init - initialization code for the rbteac high level driver
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
 *   02 96/03/07 RBTEAC now uses tables.								GbG
 *               ---- OS-9/68K V3.0.3 Released ----
 *               ---- OS-9/68K V3.1 Released ----
 */

#include    "rbteac.h"            /* includes all necessary .h files */
#include	"rbproto.h"

/*
 * init(dd) - initialize the device
 */
#ifndef CBOOT 
#if defined (__STDC__) || defined (_ANSI_EXT)
int init(mod_dev *dd)
#else /* K&R OSK only */
int init(dd)
mod_dev        			*dd;	         /* device descriptor ptr */
#endif
{
	int        error;          /* used to keep track of errors */
	Rbfdrive   drvptr;         /* pointer to drive table */
	auto int   (*drvrinit)();  /* point to driver's init routine */
	auto u_int32 sink;         /* put unused info here */
	u_int16    type = 0;       /* module type for link */
	u_int16    lang = 0;       /* module lang for link */	
	char      *ldrvrnam;      /* low level driver name */

	/*
	* set maximum logical drive number.
	* this setup is required so the 'initdriv' routine can
	* validate the pd_drv field of the path descriptor.
	* if out of range, then the pd_drvtbl pointer provided
	* by RBF will be out of range
	*/


	DRIVENUMBER = RBF_MAXDRIVE;          /* max number of drives */
    
	/* set default drive table info  */
	for( error = 0; error < RBF_MAXDRIVE; error++ ) {
		drvptr = &rbfstat.drv[error];  			/* get address of drive table */
		/* set a fake media size */
		*((int *)&drvptr->v_0.dd_tot[0]) = 0xffff00;
		drvptr->v_trak = 0xff;             		/* set current track to 0xff */
	}

	/* locate the name of the low-level module from the DevCon
	 * section of the Device Descriptor.  Link to that module and
	 * call its init routine
	 */

	if (dd->_mdevcon == NULL)               /* DevCon ptr setup ? */

#if defined (__STDC__) || defined (_ANSI_EXT)
		return(EOS_MNF);                    /* no - return module not found */
#else
		return(E_MNF);                      /* no - return module not found */
#endif
     
	ldrvrnam = (char *) ((int) dd + (int) dd->_mdevcon);    /* name offset */
	
	/* link to the low level driver and execute it's init routine */
	if ((error = _os_link ( (char **)&ldrvrnam, (mh_com **) &drvr_mod, 
				(void **)&drvrinit, &type, &lang)) == SUCCESS)

		error = (*drvrinit) (dd, &data_mod);
	return error;

} 
    
#else /* OSK CBOOT */

#if defined (__STDC__) || defined (_ANSI_EXT)
error_code init_fteac(void)
#else
error_code init_fteac()
#endif
{
	Rbfdrive  	 drvptr;         /* pointer to drive table */
	error_code error;
	
	DRIVENUMBER = RBF_MAXDRIVE;
	
	drvptr = DRIVETABLE;
	*((int *)&drvptr->v_0.dd_tot[0]) = 0xffff00;		/* fake media size */
	drvptr->v_trak = 0xff;             					/* set current track */
	drvptr->v_0.dd_fmt = (FMT_DS | FMT_DD | FMT_DT);	/* fake fmt */
	
	if( (error = llinit()) )
		return error;
		
	return( ftinitdrv( drvptr, 0 ));
}
#endif
