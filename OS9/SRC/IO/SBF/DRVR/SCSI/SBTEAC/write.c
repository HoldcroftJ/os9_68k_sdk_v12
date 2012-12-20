/*
 * write - write routine for the high-level sbteac driver.
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

write(buf_size, buf_addr)
u_int	buf_size;						/* size of buffer */
char	*buf_addr;						/* buffer address */
{
	register u_int		error;			/* error holder */
	register u_int		sink;			/* disposal site */

	/*
	 * initialize drive
	 */
	if (error = initdrv())
		return (error * -1);			/* abort if failed */

	/* clear any pending eof condition */
	sbdata_mod->fstatus &= ~TAPE_EOF;	/* clear eof */
	
	/* check for partial physical block and refuse if so */
	if ((buf_size % PHYSIZE) != 0)
		return( E_PARAM * -1 );

	/*
	 * do the write block command
	 */
	
	if (error = execute(SC_WRITE, buf_size / PHYSIZE , CMDOPTS, FBS,
						buf_addr, OUTPUT, FALSE))
		return (error * -1);			/* return error as negative # */

	/* handle early eom warnings */
	if (sbdata_mod->fstatus & TAPE_EOM) {
		/*
		 * early eom detected:
		 * check for residual write required, if so
		 * write the remaining data.
		 */
		if (residual) {
			/* residual data remaining */
			error = execute (SC_WRITE, residual, CMDOPTS, FBS,
							(buf_addr + buf_size) - (residual * PHYSIZE),
							OUTPUT, FALSE);
		}
		sink = execnoxfer(SC_WRITEMARK, 0, CMDOPTS, 0);	/* flush buffer */
		if (sink == 0) {
			sbdata_mod->blknum += (buf_size / PHYSIZE);	/* track blocks written */
			if (sbdata_mod->fstatus & TAPE_FULL) {
				sbdata_mod->fstatus &= ~TAPE_FULL;
				return (E_FULL * -1);
			}
			else
				return 0;
		}
		else
			return (sink * -1);
	}
	
	sbdata_mod->blknum += (buf_size / PHYSIZE);		/* track blocks written */
	return 0;										/* return good status */
}
