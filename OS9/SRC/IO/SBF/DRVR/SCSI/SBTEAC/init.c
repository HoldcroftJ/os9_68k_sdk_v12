/*
 * init - initialization code for the sbteac high-level driver
 */
 
/*
 * Copyright 1989, 1990, 1994 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include "sbteac.h"					/* include all necessary .h files */

init(dd)
register mod_dev	*dd;				/* device descriptor ptr */
{
	register int		error;			/* used to keep track of errors */
	auto int			(*drvrinit)();	/* point to driver's init routine */
    auto void           *sbdat_hdr;   	/* pointer to data module header */
    auto void			*sbpdmod;		/* pointer to data module body */
    auto int            sink;       	/* put unused results here */
	char				*ldrvrnam,		/* low level driver name */
						name[32];		/* high level driver name */
	int					portaddr;		/* port address */

	/*
	 * Initialize the static storage.
	 * NOTE:  this driver can only support 1 drive at present.
	 */
	
	sbfstat.sbf_ndrv = SBF_MAXDRIVE;		/* set maximum # drives */
	blk_size = PHYSIZE;						/* the physical block size */

	/* 
	 * Attempt to create the data module which contains the
	 * markcnt and blknum variables.
	 */
	portaddr = ((int) sbfstat.sbf_sysio.v_port) & (~0xf);
    strcpy (name, "stat");
    lxtoa (&name[4], portaddr);
    
    /* NOTE: ignore error in case module exists */
	if (_f_datmod (sizeof (struct sbdatmod), MA_REENT<<8,
						LLMODACCESS, name,
					&sink, &sink, &sink, &sbpdmod, &sbdat_hdr) == 0) {
		sbdata_mod = sbpdmod;				/* save module body ptr */
		sbdata_mod->sbmodhdr = sbdat_hdr;	/* save module header in body */
	}

    if (error = _f_link(0, name, &sink, &sink, &sink, &sbpdmod, &sink))
    	return(error);						/* abort can't link */
	else
	   	sbdata_mod = sbpdmod;				/* save ptr to module body */


	/* get the logical block size in terms of physical blocks */

	lbsize = ((struct sbf_opt *) &(dd->_mdtype))->pd_blksiz / blk_size;
	
	/* MAKE SURE the dd->_mdevcon name exists; ie the low level driver name 
	 * is specified before trying to link to it !!!
	 */
	if (dd->_mdevcon == NULL)				/* DevCon set up correctly ? */
		return(E_MNF);						/* no - return module not found */
	
	ldrvrnam = (char *) ((int) dd + (int) dd->_mdevcon);	/* name offset */

	/* link to the low level driver */
	if (error = _f_link(0, ldrvrnam, &sink, &sink, &sink, 
						&drvrinit, &(drvr_mod)))
		return(error);
		
	/* execute the low level init routine */
	if (error = (*drvrinit) (dd, &data_mod))	/* low level init */
		return (error);

	/* initialize target controller/lun values.
	 * NOTE:  this assumes only one drive is supported by
	 * the driver incarnation.  Any path descriptor options
	 * can thus be read directly from the device descriptor
	 * itself.
	 */
	
	/* init target controller's scsi id */
	scsiid = ((struct sbf_opt *) &(dd->_mdtype))->pd_scsiid;
	
	/* init target lun for controller */
	scsilun = ((struct sbf_opt *) &(dd->_mdtype))->pd_scsilun;
	
	/* get the scsi options flags */
	optflags = ((struct sbf_opt *) &(dd->_mdtype))->pd_scsiopt;

	/* apply some basic parameter checks to the
	 * values retrieved from the descriptor.
	 */
	
	/* for the TEAC drive, the target LUN is always 0 */
	if (scsilun != TAPE_LUN)
		return (E_UNIT);						/* signal bad descriptor */
	
	return 0;									/* good exit */
}
