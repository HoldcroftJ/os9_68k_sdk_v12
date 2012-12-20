/*
 * term - terminate routine for the Common Command Set high level driver
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

#include	"rbvccs.h"
 
/*
 * int term () - terminate the device 
 */
 
int term (dd)
mod_dev		*dd;
{
	register Rbfdrive drvptr;		/* pointer to drive table */
	register struct rbf_opt *opt;	/* drive options */
	register int error,				/* error holder */
	             ierror;			/* secondary error holder */
	
	drvptr = &rbfstat.drv[0];		/* only one drive table */

	/* call low-level module to inform it that this driver is finished */
	if ((data_mod != NULL) && (data_mod->dm_term != NULL))
		error = (*data_mod->dm_term) (dd, data_mod);
	
	/* unlink low-level module, if error occurs, will report this one
	 *   otherwise, any errors from low level term will be reported
	 *   instead.
	 */
	if (drvr_mod != NULL)
		if( (ierror = _f_unlink (drvr_mod)) )
			error = ierror;

	/* return sector zero buffer if one is allocated */
	if( drvptr->v_sczero != NULL )
		_f_srtmem(drvsize.ds_blksize, drvptr->v_sczero );

	return error;
}
