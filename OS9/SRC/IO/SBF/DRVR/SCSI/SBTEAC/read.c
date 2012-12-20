/*
 * read - read routine for the high-level sbteac driver.
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

read(buf_size, buf_addr)
u_int	buf_size;						/* buffer size */
char	*buf_addr;						/* buffer address */
{
	register u_int		error;			/* error storage */
	
	/*
	 * initialize drive
	 */
	if (error = initdrv())
		return (error * -1);			/* abort if failed */
	
	/* check for eof pending */
	if (sbdata_mod->fstatus == TAPE_EOF) {
		sbdata_mod->fstatus &= ~TAPE_EOF;	/* clear eof */
		sbdata_mod->blknum = 0;			/* no blocks seen yet */
		sbdata_mod->markcnt++;			/* count file mark now */
		return (E_EOF * -1);			/* return eof error */
	}
	
	if( buf_size % PHYSIZE )
		return(E_PARAM * -1);	/* cannot do non-integral block operations */

	/*
	 * do the read block command
	 */
	
	if (error = execute(SC_READ, buf_size / PHYSIZE, CMDOPTS, FBS,
						buf_addr, INPUT, FALSE))
		return (error * -1);			/* convert error to negative # */
	
	if (residual) {
		/* read successful, BUT file mark encountered */
		
		residual *= PHYSIZE;			/* convert to byte count */
		sbdata_mod->markcnt++;			/* count one more mark reached (ed.13)*/
		if (residual == buf_size) {
			/* no data read - treat as end of file */
			sbdata_mod->fstatus &= ~TAPE_EOF;	/* clear pending eof */
			residual = 0;				/* invalidate residual */
			return (E_EOF * -1);		/* return eof error */
		} else {
			buf_size -= residual;			/* return actual size read */
			residual = 0;					/* mark residual invalid */
		}
	}
	sbdata_mod->blknum += (buf_size / PHYSIZE);	/* track blocks read */
	return(buf_size);					/* return size read */
}
