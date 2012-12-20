/*
 * read - read routine for the rbteac high level driver
 */
 
/*
 * Copyright 1990, 1991, 1995, 1996 by Microware Systems Corporation
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

#include    "rbteac.h"                        /* includes all the defs */
#include	"rbproto.h"

/*
 * int read (numsects, blkaddr) - read sectors 
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
#ifdef CBOOT
error_code read_fteac( int numsects, u_int blkaddr )
#else 
int read (int numsects, u_int blkaddr)
#endif

#else /* K&R */
#ifdef CBOOT
error_code read_fteac( numsects, blkaddr )
#else 
int read (numsects, blkaddr)
#endif
int     numsects;       /* number of sectors to transfer */
u_int   blkaddr;        /* logical block address of disk */
#endif
{
    Rbfdrive   			drvptr;     /* pointer to the drive table */
    struct rbf_opt 		*opt;	    /* path descriptor options ptr */
    u_char     			*buffptr;   /* pointer to rbf buffer */
    int        			error;      /* the error code */
    u_int      			firstblk;   /* the first block read (sect 0 check) */
    int                 clockon;	/* clock status */
    int                 drvn;		/* drive number */

	drvptr = DRIVETABLE;

	/* initialize drive if required */
    if (error = ftinitdrv (drvptr, 0))
        return error;                       /* abort if error */

    buffptr = PATHBUFFER;					/* get the buffer address */
    opt = PATHOPTIONS;						/* point at pd options */
    drvn = opt->pd_drv;
    firstblk = blkaddr;                     /* remember where we started */
    
#ifndef CBOOT
    /*
     * check for buffered sector 0.
     *     secotor 0 is normally buffered for FIXED MEDIA ONLY.
     *     This driver impliments a time stamp to allow limited buffering
     *     of sector zero even though we know that the media is removeable
     */
    if( (firstblk == 0) && drvptr->v_zerord && numsects ) {
    	if( chkfdtime(drvptr, drvn) ) {
    		/* use the sector zero copy instead of the disk's */
    		copyblock(buffptr, (u_char *)drvptr->v_sczero, opt->pd_ssize );
    		buffptr += opt->pd_ssize;				/* update pointer */
    		blkaddr++;								/* update lsn */
    		numsects--;								/* update number to read */
    	}
    }
#endif

	/* Validate the requested sector range. */
    if ( (blkaddr + numsects) > (*((int *)&drvptr->v_0.dd_tot[0]) >> 8) )

#if defined (__STDC__) || defined (_ANSI_EXT)
		return EOS_SECT;
#else
		return E_SECT;
#endif

	/* adjust for track/lba offset */
	blkaddr = fttrkoffs(blkaddr, drvptr->v_0.dd_fmt);

    /* read the data from the disk if required */
    if ( numsects ) {
        /* call low-level module to read sector(s) */
        if (error = execute (SC_READXTD, blkaddr, numsects * opt->pd_ssize,
                    OPTS, buffptr, INPUT, CDB_EXT, 0))
            return error;
    }
    
#ifndef CBOOT
   	clockon = setfdtime(drvn);		/* refresh access time */
#endif

    /*   If sector zero was read from the disk, then update
     * the drive table for that drive.  If the system clock is running
     * we can time stamp and cache sector 0 as well.
     */
    if ( (firstblk == 0) && (drvptr->v_zerord == FALSE) ) {
        /* update drive table */
        copyblock( (u_char *)drvptr, PATHBUFFER, sizeof(struct sect0std) );
        
		drvptr->v_init = FALSE;		/* in case new info has arrived */
        
       	if(( opt->pd_sid == 1) && (drvptr->v_0.dd_fmt & FMT_DS))
       		/* can't read d/s media in s/s drive */

#if defined (__STDC__) || defined (_ANSI_EXT)
       		return EOS_BTYP;			/* bad type!!! */
#else
       		return E_BTYP;				/* bad type!!! */
#endif
       		
		if(((opt->pd_dns & DNS_DT) == 0 ) && (drvptr->v_0.dd_fmt & FMT_DT))

#if defined (__STDC__) || defined (_ANSI_EXT)
			return EOS_BTYP;			/* bad type!!! */
#else
			return E_BTYP;				/* bad type!!! */
#endif
            
#ifndef CBOOT
		/* if clock is running, can buffer sector 0 */
		if( clockon ) {
			copyblock( (u_char *)drvptr->v_sczero, PATHBUFFER, opt->pd_ssize );
			drvptr->v_zerord = TRUE;
		}
#endif
    }
    return 0;
}
