/*
 * misc - miscellaneous functions for the mVme327 floppy driver 
 */

/*
 * Copyright 1990 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include "scsi327.h"

/*
 *  errstat_b327( error, cmd ) - convert VME327 error to OS9 error
 */
 
int errstat( error, cmd )
register int error;
register Cmdblk cmd;
{
	switch( error ) {
		case FEC_BADDES:		/* bad descriptor */
		case FEC_BFPARM:		/* bad format parameters */
		case FEC_NOALTS:		/* not enough alternates */
		case FEC_BSMIS:			/* block size mismatch */
		case FEC_IVCYL:			/* invalid cylinder number in defect list */
		case FEC_IVHD:			/* invalid head number in defect list */
		case FEC_NFBLKM:		/* non-fatal block size mismatch */
		case FEC_IVSN:			/* invalid sector number  */
		case FEC_ILBS:			/* illegal logical block size */
		case FEC_STMISM:		/* sectors/track mismatch */
		case FEC_NHMM:			/* Number of heads mismatch */
		case FEC_NCMM:			/* Number of cylinders mismatch */
		case FEC_IFPAR:			/* invalid floppy parameters */
#ifdef __STDC__
			error = EOS_PARAM;
#else
			error = E_PARAM;
#endif
			break;


		case FEC_BADCMD:		/* bad command */
		case FEC_UNICMD:		/* unimplimented command */
		case FEC_UNIDEV:		/* unimplemented device */
		case FEC_RELTRK:		/* relocated track error */
		case FEC_BADSGT:		/* bad scatter/gather table */
		case FEC_INENAI:		/* Indeterminate media error, no additional */
		case FEC_UCT:			/* unsupported controller type */
		case FEC_UPT:			/* unsupported peripheral device type */
#ifdef __STDC__
			error = EOS_BTYP;	
#else
			error = E_BTYP;	
#endif
			break;

	
		case FEC_BADDRV:		/* bad drive */
		case FEC_NOTINT:		/* unit not initialized */
#ifdef __STDC__
			error = EOS_UNIT;
#else
			error = E_UNIT;
#endif
			break;	

			
		case FEC_NOID:			/* no ID found on track */
		case FEC_BADID:			/* Record not found, bad ID */
		case FEC_RNF:			/* record not found */
		case FEC_LOGADR:		/* bad logical address */
		case FEC_SEEK:			/* seek error */
#ifdef __STDC__
			error = EOS_SEEK;
#else
			error = E_SEEK;
#endif
			break;


		case FEC_BADSNC:		/* data sync fault */
		case FEC_BADECC:		/* ECC error */
		case FEC_MEDIA:			/* media error */
		case FEC_ICDAT:			/* incomplete data transfer */
#ifdef __STDC__
			error = (cmd->_cb_xfer) ? EOS_WRITE : EOS_READ;
#else
			error = (cmd->_cb_xfer) ? E_WRITE : E_READ;
#endif
			break;


		case FEC_WRPROT:		/* Write protected media */
#ifdef __STDC__
			error = EOS_WP;
#else
			error = E_WP;
#endif
			break;


		case FEC_DOOR:			/* door open */
#ifdef __STDC__
			error = EOS_NOTRDY;
#else
			error = E_NOTRDY;
#endif
			break;

			
		case FEC_DRVBSY:		/* drive busy */
#ifdef __STDC__
			error = EOS_DEVBSY;
#else
			error = E_DEVBSY;
#endif
			break;


		case FEC_FMTERR:		/* format failed */
		case FEC_VERERR:		/* verify failed */
		case FEC_CFBS:			/* can't fix bad spot */
#ifdef __STDC__
			error = EOS_WRITE;
#else
			error = E_WRITE;
#endif
			break;


		default:
#ifdef __STDC__
			error = EOS_HARDWARE;
#else
			error = E_HARDWARE;
#endif
			break;
	}
	return error;
}

