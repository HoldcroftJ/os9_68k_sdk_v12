/*
 * init - initialization code for the Common Command Set high level driver
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


#include	"rbvccs.h"			/* includes all necessary .h files */

/*
 * init(dd) - initialize the device
 */
 
int	init(dd)
register mod_dev		*dd;			/* device descriptor ptr */
{
	register Rbfdrive	drvptr;			/* pointer to drive table */
	register int		error;			/* used to keep track of errors */
	auto int			(*drvrinit)();	/* point to driver's init routine */
	auto int			sink;			/* put unused info here */
	char				*ldrvrnam;		/* low level driver name */

	/*
	 * set maximum logical drive number.
	 * this setup is required so the 'initdriv' routine can
	 * validate the pd_drv field of the path descriptor.
	 * if out of range, then the pd_drvtbl pointer provided
	 * by RBF will be out of range
	 */
	
	rbfstat.v_ndrv = RBF_MAXDRIVE;			/* max number of drives */
	
	/* set default drive table info:
	 * NOTE: for this driver, only ONE logical unit is supported
	 */

	drvptr = &rbfstat.drv[0];			/* get address of drive table */
	drvptr->v_0.dd_tot[0] = 0xff;		/* make media look large */
	drvptr->v_0.dd_tot[1] = 0xff;		/* make media look large */
	drvptr->v_trak = 0xff;				/* set current track to 0xff */

	/* locate the name of the low-level module from the DevCon
	 * section of the Device Descriptor.  Link to that module and
	 * call its init routine
	 */

	if (dd->_mdevcon == NULL)				/* DevCon ptr setup ? */
		return(E_MNF);						/* no - return module not found */
	 
	ldrvrnam = (char *) ((int) dd + (int) dd->_mdevcon);	/* name offset */

	if ((error = _f_link (0, ldrvrnam, &sink, &sink, &sink,
				&drvrinit, &drvr_mod)) == 0)
		error = (*drvrinit) (dd, &data_mod);	/* call low level init */
	return error;
}
