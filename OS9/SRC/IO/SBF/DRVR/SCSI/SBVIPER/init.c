/*
 * init - initialization code for the sbviper high-level driver
 */
 
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
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
|  01 98/09/08 Added edition history.                                  wwb  |
|				---- OS-9/68K V3.1 Release ----								|
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

#include "sbviper.h"					/* include all necessary .h files */

init(dd)
register mod_dev	*dd;				/* device descriptor ptr */
{
	register int		error;			/* used to keep track of errors */
	auto int			(*drvrinit)();	/* point to driver's init routine */
	auto int			sink;			/* put unused info here */
	char				*ldrvrnam;		/* low level driver name */

	/*
	 * Initialize the static storage.
	 * NOTE:  this driver can only support 1 drive at present.
	 */
	
	sbfstat.sbf_ndrv = SBF_MAXDRIVE;		/* set maximum # drives */
	blk_size = PHYSIZE;						/* the physical block size */

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
	
	/* for the Archive Viper, the target LUN is always 0 */
	if (scsilun != TAPE_LUN)
		return (E_UNIT);						/* signal bad descriptor */
	
	return 0;									/* good exit */
}
