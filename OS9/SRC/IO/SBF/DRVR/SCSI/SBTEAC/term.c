/*
 * term - term routine for the high-level sbteac driver.
 */
 
/*
 * Copyright 1989, 1990 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include "sbteac.h"					/* include all the defs */

term(dd)
register mod_dev		*dd;			/* device descriptor ptr */
{
	register int	error; 				/* error holder */
	register int	i;					/* more of the same */
	register int	j;					/* "" */
	register int	k;					/* "" */
	
	/* delete the process SBF created if it exists */
	if (sbfstat.sbf_dprc != NULL)
	   error =_f_delprc(((procid *) sbfstat.sbf_dprc)->_id);

	/* run the low-level terminate routine */
	if ((data_mod != NULL) && (data_mod->dm_term != NULL))
		j = (*(data_mod)->dm_term) (dd, data_mod);

	/* unlink from the data module */
	if (drvr_mod != NULL)
		i = _f_unlink(drvr_mod);
		
	/* unlink from the data module */
	if ((sbdata_mod != NULL) && (sbdata_mod->sbmodhdr != NULL))
		k = _f_unlink(sbdata_mod->sbmodhdr);

	/*
	 * return appropriate error (if any)
	 */
	if (error)
	   return(error);			/* return "delprc" problems */
	if (j)
	   return(j);				/* return "terminate" problems */
	if (i)
	   return(i);				/* return low-level module "unlink" problems */
	if (k)
	   return(k);				/* return data module "unlink" problems */

	return 0;					/* default:  no error */
}
