/*
 * term - term routine for the high-level sbviper driver.
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

#include "sbviper.h"					/* include all the defs */

term(dd)
register mod_dev		*dd;			/* device descriptor ptr */
{
	register int	error; 				/* error holder */
	register int	i;					/* more of the same */
	register int	j;					/* "" */
	
	/* delete the process SBF created if it exists */
	if (sbfstat.sbf_dprc != NULL)
	   error =_f_delprc(((procid *) sbfstat.sbf_dprc)->_id);

	/* run the low-level terminate routine */
	if ((data_mod != NULL) && (data_mod->dm_term != NULL))
		j = (*(data_mod)->dm_term) (dd, data_mod);

	/* unlink from the data module */
	if (drvr_mod != NULL)
		i = _f_unlink(drvr_mod);

	/*
	 * return appropriate error (if any)
	 */
	if (error)
	   return(error);			/* return "delprc" problems */
	if (j)
	   return(j);				/* return "terminate" problems */
	if (i)
	   return(i);				/* return "unlink" problems */

	return 0;					/* default:  no error */
}
