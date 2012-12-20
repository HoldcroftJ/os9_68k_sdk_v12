/*
 * write - write routine for Common Command Set high level driver
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

#include	"rbvccs.h"				/* gets all the defs */

/*
 * int write (numsects, blkaddr) - write the blocks 
 */
int write (numsects, blkaddr)
register int numsects;			/* number of sectors to transfer */
register u_int	blkaddr;		/* starting block address for transfer */
{
	register Rbfdrive	drvptr;		/* drive table pointer */
	register struct rbf_opt *opt;	/* path descriptor options ptr */
	register int error;				/* keep any error codes here */
	int firstblk,retry;				/* first lsn and retry flag on ID change */

	
	/*---------------------!
	! general init & setup !
	!---------------------*/
	drvptr = pathdesc->rbfpvt.pd_dtb;	/* get drive table ptr */
	opt = &pathdesc->rbfopt;			/* point at pd options */
	firstblk = blkaddr;					/* save first accessed block */
	
	/* init drive if required */
	if (error = initdrv (drvptr, NOPARK))
		return error;				/* abort if error */
	
	/* check if writing sector 0.
	 * if so, check if write is allowed and then unbuffer sector.
	 */
	if (blkaddr == 0) {
		/* check if device format protected */
		if (opt->pd_cntl & CNTL_NOFMT)
			return E_FORMAT;				/* can't write sector 0 */
			
		/* insure that the logical:physical sector size is 1:1 */
		if( ((Sector0)pathdesc->path.pd_buf)->dd_lsnsize != drvsize.ds_blksize)
			return E_SECTSIZE;

		/* mark sector 0 unbuffered */
		drvptr->v_zerord = FALSE;
	}
	
	/* add any blkaddr offsets */
	blkaddr = addoffsets (blkaddr);
	
	/*---------------------------------------!
	! write the integral blocks              !
	!---------------------------------------*/
	if( numsects != 0 ) {
		retry = FALSE;
		do {
			if (error =  execute(SC_WRITEXTD, blkaddr, numsects * opt->pd_ssize,
	                     CCSOPTS, pathdesc->path.pd_buf, OUTPUT, CDB_EXT)) {
				/* retry once and only once if E_DIDC */
				if( (error == E_DIDC) && (!firstblk) && (!retry) )  {
					if( error = initdrv(drvptr, NOPARK) )
						return error;
					retry = TRUE;				/* mark for retry */
				} else
					return error;
			} else
				retry = FALSE;
		} while( retry );						/* redo only once */

		/* check if write verify required and do it if so */
		if (opt->pd_vfy == NULL)
			error = execnoxfer(SC_DAVERIFY, blkaddr, numsects,
			                   CCSOPTS, CDB_EXT);
	}
	return error;
}
