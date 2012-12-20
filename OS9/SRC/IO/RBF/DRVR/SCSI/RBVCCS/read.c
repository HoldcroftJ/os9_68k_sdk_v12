/*
 * read - read routine for the Common Command Set high level driver
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

#include	"rbvccs.h"						/* includes all the defs */

/*
 * int read (numsects, blkaddr) - read sectors 
 */
 
int	read (numsects, blkaddr)
int		numsects;		/* number of sectors to transfer */
u_int	blkaddr;		/* logical block address of disk */
{
	register Rbfdrive	drvptr;		/* pointer to the drive table */
	register struct rbf_opt *opt;	/* path descriptor options ptr */
	register int		error;		/* the error code */
	register u_int		tmpcnt;		/* temp sector counter */
	u_char              *buffptr;	/* temporary storage for buffer pointer */
	u_int   			sink;		/* disposal site */
	u_int    			firstblk;	/* the first block read (sect 0 check) */
	int                 retry;		/* flag for retries on E_DIDC */

	/*---------------------!
	! general init & setup !
	!---------------------*/
	drvptr = pathdesc->rbfpvt.pd_dtb;		/* get the drive table address */
	buffptr = pathdesc->path.pd_buf;		/* get the buffer address */
	opt = &pathdesc->rbfopt;				/* point at pd options */
	firstblk = blkaddr;						/* remember where we started */

	/* initialize drive if required */
	if (error = initdrv (drvptr, NOPARK))
		return error;						/* abort if error */
	
	/* check for buffered sector 0.
	 * sector 0 is normally buffered for fixed media ONLY.
	 */
	if ((blkaddr == 0) && (drvptr->v_zerord == TRUE) && (numsects)) {
		copysect (buffptr, drvptr->v_sczero, opt->pd_ssize); /* copy the block */
		buffptr += opt->pd_ssize;		/* update buffer pointer */
		blkaddr++;						/* update block address */
		numsects--;						/* update count */
	}

	/*------------------------------!
	! add any blkaddr offsetting	!
	!------------------------------*/
	blkaddr = addoffsets(blkaddr);
	
	if( numsects > 0 ) {
		retry = FALSE;
		do {
			if (error =  execute(SC_READXTD, blkaddr, numsects * opt->pd_ssize,
					                     CCSOPTS, buffptr, INPUT, CDB_EXT))   {
				/* retry once and only once if E_DIDC */
				if( (error == E_DIDC) && (!firstblk) && (!retry) ) {
					if( error = initdrv(drvptr, NOPARK) )
						return error;
					retry = TRUE;				/* mark for retry */
				} else
					return error;
			} else
				retry = FALSE;
		} while( retry );						/* redo only once */
	}
	
	/* if sector zero was read from the disk, then update
	 * the drive table for that drive.
	 * if the disk was a fixed media, then buffer the full
	 * sector for subsequent reads.
	 */

	if ((firstblk == 0) && (drvptr->v_zerord == FALSE)) {

		/* for compatablility, insure physical/logical relationship is 1:1 */	
		if( ((Sector0)buffptr)->dd_lsnsize != drvsize.ds_blksize ) {
			if( ((Sector0)buffptr)->dd_lsnsize == 0 ) {
				if( drvsize.ds_blksize != 256 )		/* old device.... */
					return E_SECTSIZE;				/* ...still must be 1:1 */
			} else
				return E_SECTSIZE;
		}

		/* update drive table */
		memcpy((char *) drvptr, buffptr, sizeof(struct sect0std));

		/* buffer sector 0 if hard and fixed media */
		if ((opt->pd_typ & TYP_HARD) && ((opt->pd_typ & TYP_HREMOV) == 0)) {
			copysect (drvptr->v_sczero, buffptr, opt->pd_ssize);
			drvptr->v_zerord = TRUE;		/* flag sector zero cached */
		}
	}
	return 0;
}
