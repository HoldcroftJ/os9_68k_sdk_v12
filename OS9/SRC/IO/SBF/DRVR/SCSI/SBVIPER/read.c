/*
 * read - read routine for the high-level sbviper driver.
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
	if (fstatus == TAPE_EOF) {
		fstatus &= ~TAPE_EOF;			/* clear eof */
		return (E_EOF * -1);			/* return eof error */
	}
	
	if( buf_size % PHYSIZE )
		return(E_PARAM * -1);			/* no partial reads allowed */

	/*
	 * do the read block command
	 */
	
	if (error = execute(SC_READ, buf_size / PHYSIZE, CMDOPTS, FBS,
						buf_addr, INPUT, FALSE))
		return (error * -1);			/* convert error to negative # */
	
	if (residual) {
		/* read successful, BUT file mark encountered */
		
		residual *= PHYSIZE;			/* convert to byte count */
		if (residual == buf_size) {
			/* no data read - treat as end of file */
			fstatus &= ~TAPE_EOF;		/* clear pending eof */
			residual = 0;				/* invalidate residual */
			return (E_EOF * -1);		/* return eof error */
		} else {
			buf_size -= residual;			/* return actual size read */
			residual = 0;					/* mark residual invalid */
		}
	}	
	return(buf_size);					/* return size read */
}
