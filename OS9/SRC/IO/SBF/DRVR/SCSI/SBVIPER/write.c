/*
 * write - write routine for the high-level sbviper driver.
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
	fstatus &= ~TAPE_EOF;				/* clear eof */
	
	/* check for partial physical block and refuse if it is */
	if( buf_size % PHYSIZE )
		return( E_PARAM * -1);	

	/*
	 * do the write block command
	 */
	
	if (error = execute(SC_WRITE, buf_size / PHYSIZE , CMDOPTS, FBS,
						buf_addr, OUTPUT, FALSE))
		return (error * -1);			/* return error as negative # */

	/* handle early eom warnings */
	if (fstatus & TAPE_EOM) {
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
			if (fstatus & TAPE_FULL) {
				fstatus &= ~TAPE_FULL;
				return (E_FULL * -1);
			}
			else
				return 0;
		}
		else
			return (sink * -1);
	}	
	
	return 0;							/* return good status */
}
