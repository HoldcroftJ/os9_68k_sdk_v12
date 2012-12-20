/*
 * write - write routine for rbteac high level driver
 */
 
/*
 * Copyright 1990, 1991, 1994, 1995, 1996 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 *	Edition History:														
 *	#   Date    Comments												By
 *	-- -------- ------------------------------------------------------- ---
 *   00 95/07/12 Added Edition History for this file.                   GbG
 *   01 95/07/12 ANSI Version.                                          GbG
 *               ---- OS-9/68K V3.0.3 Released ----
 *               ---- OS-9/68K V3.1 Released ----
 */

#include    "rbteac.h"                /* gets all the defs */
#include	"rbproto.h"

/*
 * int write (numsects, blkaddr) - write the blocks 
 */
#if defined (__STDC__) || defined (_ANSI_EXT) 
int write (int numsects, u_int blkaddr)
#else
int write (numsects, blkaddr)
int     numsects;       /* number of sectors to transfer */
u_int   blkaddr;        /* starting block address for transfer */
#endif
{
    Rbfdrive 			 drvptr;    /* drive table pointer */
    struct rbf_opt			*opt;   /* path descriptor options ptr */
    int					error;      /* keep any error codes here */
    u_int				firstblk;	/* original block number */
    u_char				*buffptr;   /* pointer to rbf buffer */
    u_char				cmdcode;    /* write command opcode */
    long                xfersize;	/* number of bytes to transfer */
    
    drvptr = DRIVETABLE;            /* get drive table ptr */

    /* init drive if required */
    if (error = ftinitdrv (drvptr, 0))
        return error;               /* abort if error */

    opt = PATHOPTIONS;            	/* point at pd options */
    buffptr = PATHBUFFER;		    /* get the buffer address */
	firstblk = blkaddr;				/* save unadjusted block # */

	/* check for all logical sectors to be written are in range */
    if ( (blkaddr + numsects) > (*((int *)&drvptr->v_0.dd_tot[0]) >> 8) )

#if defined (__STDC__) || defined (_ANSI_EXT) 
		return EOS_SECT;					/* bad logical sector number */
#else
		return E_SECT;						/* bad logical sector number */
#endif

	/* adjust for track/lba offset */
	blkaddr = fttrkoffs(blkaddr, drvptr->v_0.dd_fmt);
    
    /* check if writing sector 0.
     * if so, check if write is allowed and then unbuffer sector.
     */
    if (firstblk == 0) {
        /* check if device format protected */
        if (opt->pd_cntl & CNTL_NOFMT)

#if defined (__STDC__) || defined (_ANSI_EXT) 
            return EOS_FORMAT;               /* can't write sector 0 */
#else
            return E_FORMAT;                /* can't write sector 0 */
#endif            

        /* mark sector 0 unbuffered */
        drvptr->v_zerord = FALSE;
    }
    
    /* write the data to the disk */
    xfersize = numsects  * opt->pd_ssize;
    error = execute (SC_WRITEXTD, blkaddr, xfersize, OPTS, buffptr,
                     OUTPUT, CDB_EXT, 0);
    
   	/* if verify requested, do it */
   	/*    NOTE:  The call to verify sets the no retry on Disk ID change. This
   	 *      should prevent the removal and insertion of a disk durring a
   	 *      a verify.  The flag indicates that no retrys should be performed
   	 *      if the ID change occurs.
   	 */
	if( !error && !opt->pd_vfy)
		error = execnoxfer( SC_VERIFYEXT, blkaddr, numsects, 
			OPTS, CDB_EXT, TRUE );
		
    return error;
}
