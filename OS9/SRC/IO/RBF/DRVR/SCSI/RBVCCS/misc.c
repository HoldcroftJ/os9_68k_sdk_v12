/*
 * misc - miscellaneous functions for the Common Command Set high level driver
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

#include	"rbvccs.h"					/* gets all the defs */

/*
 * initdrv (drvtbl, park) - initialize drive as needed 
 */
 
initdrv (drvtbl, park)
Rbfdrive		drvtbl;			/* the drive table */
u_char			park;			/* boolean PARK flag */
{
	register struct rbf_opt	*opt;		/* path descriptor options ptr */
	register int			error;		/* error holder */
	int                     flagstemp;	/* preservation of pd_scsiopt */
	int         			notrdyto;	/* not ready time-out */
	int                     sink;		/* bit bucket */

	opt = &pathdesc->rbfopt;		/* point at path descriptor options */
	
	/* check path descriptor for legal parameters */
	if ((opt->pd_drv >= rbfstat.v_ndrv)		/* logical drive # ok ?		*/
		|| (opt->pd_lun > MAXLUN))			/* pd_lun valid ?			*/
		return E_UNIT;	/* bad unit number */

	/* force drive re-initialization if parking heads */
	if (park == PARK)
		drvtbl->v_init = FALSE;
	
	/* send drive parameters to controller if drive not previously
	 * initialized, or if parking the heads.
	 */
	if (drvtbl->v_init == TRUE) {
		if (opt->pd_ssize == 0)
			/* 
			 * if new path open and pd_ssize is 0, init
			 * it from drive's block size.
			*/
			opt->pd_ssize = drvsize.ds_blksize;
		return 0;						/* drive is initialized */
	}
	
	/*
	 * check to see if we have previously sent the start command to
	 * the drive (in case jumper option is implemented).
	 * Repeat the command until successful.
	 * 
	 * NOTE:  some drives always returns "not ready" instead of unit
	 * attention on first command after power-up.
	 */

	if (started == FALSE) {
		/* send "start unit" command */
		/*  We cannot allow the Synchronous operation flag until after
		 *  "load"  since some drives actually load firmware in response
		 *  to the load command.  Preserve the scsi options from the path
		 *  descriptor here, send the load command and restore them
		 *  after the command.
		 */
		flagstemp = opt->pd_scsiopt;
		opt->pd_scsiopt &= ~SCSI_SYNCHR;
		
		notrdyto = NOTRDY_TO;				/* init timeout factor */	
		do {
			/* find out if the device is ready, if so, load is not nessesary */
			if( !(error = execnoxfer( SC_TESTREADY, 0, 0, CCSOPTS, CDB_STD)) )
				break;			/* device is already ready! */

			error = execnoxfer (SC_LOAD, 0, 1, CCSOPTS, CDB_STD);
			if (error == E_NOTRDY)
				notrdyto--;					/* count not-ready state */
		} while (((error == E_NOTRDY) && notrdyto) || (error == E_DIDC) );

		opt->pd_scsiopt = flagstemp;		/* restore options flags */
	
		if (error)
			return error;					/* abort if unsuccessful */

		started = TRUE;						/* flag device started sent */
	}

	/* added ed. 16,  non-data transfer command (possibly with SYNCH set) */
	if( error = execnoxfer( SC_TESTREADY, 0, 0, CCSOPTS, CDB_STD) )
		return error;			/* problems!!! */

	/* get drive capacity */
	if ((error = execute (SC_READCAP, 0, sizeof dsize, CCSOPTS,
						&drvsize, INPUT, CDB_EXT)) != 0)
		return error;					/* abort if failed */
	
	/*  to support variable sector size RBF, but leave open the chance that
	 *  we are about to format the drive, we must validate the users selection
	 *  of pd_ssize.
	 */	
	if( opt->pd_ssize == 0 ) {
		/* fill in the sector size based on the drives current sector size */
		opt->pd_ssize = drvsize.ds_blksize;
	} else {
		if (opt->pd_cntl & CNTL_NOFMT) {
			/* if format protected, and a mismatch occurs, error! */
			if( opt->pd_ssize != drvsize.ds_blksize )
				return E_SECTSIZE;
		} else {
			/* iff not format protected and the sector size needs to be changed
			 * then set drive parameters as requested (chg Ed.14)
			 */
			if( opt->pd_ssize != drvsize.ds_blksize )
				if( (error = sccsmode()) )
					return error;
		}
	}

	/*  Allocate a sector 0 buffer iff needed */
	if( !(drvtbl->v_sczero) )
		error = _f_srqmem(opt->pd_ssize, &sink, &drvtbl->v_sczero);

	/* mark drive initialized if all ok */
	if (error == 0)
		drvtbl->v_init = TRUE;			/* flag drive initialized */
	return error;
}

/*
 * execute () - execute a command involving data transfer.
 *
 */

int execute (opcode, blkaddr, bytcnt, cmdopts, buff, xferflags, cmdtype)
u_int				opcode,			/* the command opcode */
					blkaddr,		/* the disk block address */
					bytcnt,			/* number of bytes to xfer */
					cmdopts;		/* command options byte */
char				*buff;			/* the buffer pointer */
u_int				xferflags;		/* data transfer direction flag */
u_int				cmdtype;		/* cdb command type */
{
	register int	error;			/* keep track of errors */
	register struct rbf_opt	*opt;	/* path descriptor options ptr */
	register u_int  blkcnt;     	/* number of blocks to be read */

	opt = &pathdesc->rbfopt;		/* point at path descriptor options */
	
	if( opt->pd_ssize )
		blkcnt =  bytcnt / opt->pd_ssize; /* for possible use later */
	else
		blkcnt = 0;

	do {
		/*	set up SCSI command packet: command code, logical unit,
		 *  block address, block count, and options
		 */
		
		if (cmdtype == CDB_STD) {					/* setup 6-byte cdb */
			scsicmd.std.sp_opcode = opcode;			/* set cmd byte */
			scsicmd.std.sp_lun = opt->pd_lun;		/* set target lun */
			scsicmd.std.sp_lbamsbs = (blkaddr & 0x1f0000) >> 16;	/* lba */
			scsicmd.std.sp_lba[0] = (blkaddr & 0xff00) >> 8;
			scsicmd.std.sp_lba[1] = (blkaddr & 0x00ff);
			
			/* byte-count field manipulation:  if the count
			 * is < sector size, assume byte-count to be
			 * retained in cdb, else assume cdb count to
			 * be made into a block count.
			 */
			scsicmd.std.sp_blkcnt = (bytcnt < opt->pd_ssize)? bytcnt : blkcnt;
			scsicmd.std.sp_options = cmdopts;				/* cmd options */
		}
		else if (cmdtype == CDB_EXT) {				/* setup 10-byte cdb */
			scsicmd.ext.sp_opcode = opcode;			/* set cmd byte */
			scsicmd.ext.sp_lun = opt->pd_lun;		/* set target lun */
			scsicmd.ext.sp_zero = 0;				/* null field */
			scsicmd.ext.sp_lba = blkaddr;			/* lba */
			scsicmd.ext.sp_zero2 = 0;				/* null field */
			scsicmd.ext.sp_count[0] = (blkcnt & 0xff00) >> 8;
			scsicmd.ext.sp_count[1] = (blkcnt & 0x00ff);	/* xfer count */
			scsicmd.ext.sp_options = cmdopts;		/* cmd options */
		} else {							/* unknown command block size */
			error = E_PARAM;					/* return parameter error */
			break;
		}
		
		/*	set up low-level driver command block: SCSI command packet
		 *  address and size, logical unit, data buffer address and size,
		 *  and SCSI controller id 
		 */
		cmdblk._cb_cmdptr = &scsicmd;				/* set cmd blk ptr */
		cmdblk._cb_cmdlen = cmdtype;				/* set its size */
		cmdblk._cb_pd_lun = opt->pd_lun;			/* set target lun */
		cmdblk._cb_datptr = buff;					/* set buffer ptr */
		cmdblk._cb_datlen = bytcnt;					/* set data count */
		cmdblk._cb_scsi_id = opt->pd_ctrlrid;		/* set scsi id */
		cmdblk._cb_xfer = xferflags;				/* set xfer direction */

		/* call the low-level module */
		if (error = (*data_mod->dm_exec) (procdesc, data_mod, &cmdblk,
					opt->pd_scsiopt, DEFAULT))
			error = scsistat (error);				/* decode scsi status */
	
	/* repeat command if device/bus is busy */
	} while (error == E_DEVBSY);
	
	/* repair "reservation conflict" situation */
	if (error == RESERV_CONFLCT)
		error = E_DEVBSY;							/* say device busy */
	return error;
}

/*
 * execnoxfer () - execute a command without data transfer
 *
 */
 
int execnoxfer (opcode, blkaddr, bytcnt, cmdopts, cmdtype)
u_int				opcode,		/* the opcode */
					blkaddr,	/* the block address */
					bytcnt,		/* block count for command */
					cmdopts,	/* command options byte */
					cmdtype;	/* cdb type */
{
	register int	error;			/* error stash */
	register struct rbf_opt	*opt;	/* path descriptor options ptr */

	opt = &pathdesc->rbfopt;		/* point at path descriptor options */

	do {	

		/* set up SCSI command packet: command code, logical unit,
		 * block address, block count, and options
		 */
		
		if (cmdtype == CDB_STD) {					/* setup 6-byte cdb */
			scsicmd.std.sp_opcode = opcode;			/* set command byte */
			scsicmd.std.sp_lun = opt->pd_lun;		/* set target lun */
			scsicmd.std.sp_lbamsbs = (blkaddr & 0x1f0000) >> 16;	/* lba */
			scsicmd.std.sp_lba[0] = (blkaddr & 0xff00) >> 8;
			scsicmd.std.sp_lba[1] = (blkaddr & 0x00ff);
			scsicmd.std.sp_blkcnt = bytcnt;			/* set byte count */
			scsicmd.std.sp_options = cmdopts;		/* set cmd options */
		}
		else if (cmdtype == CDB_EXT) {				/* setup 10-byte cdb */
			scsicmd.ext.sp_opcode = opcode;			/* set cmd byte */
			scsicmd.ext.sp_lun = opt->pd_lun;		/* set target lun */
			scsicmd.ext.sp_zero = 0;				/* null field */
			scsicmd.ext.sp_lba = blkaddr;			/* lba */
			scsicmd.ext.sp_zero2 = 0;				/* null field */
			scsicmd.ext.sp_count[0] = (bytcnt & 0xff00) >> 8;
			scsicmd.ext.sp_count[1] = (bytcnt & 0x00ff);	/* xfer count */
			scsicmd.ext.sp_options = cmdopts;		/* cmd options */
		} else {							/* unknown command block size */
			error = E_PARAM;					/* return parameter error */
			break;
		}

		/* set up low-level driver command block: SCSI command packet
		 * address and size, logical unit, and SCSI controller id
		 */
		cmdblk._cb_cmdptr = &scsicmd;				/* set cmd ptr */
		cmdblk._cb_cmdlen = cmdtype;				/* set its size */
		cmdblk._cb_pd_lun = opt->pd_lun;			/* set target lun */
		cmdblk._cb_datptr = NULL;					/* no data */
		cmdblk._cb_datlen = 0;						/* set byte count */
		cmdblk._cb_scsi_id = opt->pd_ctrlrid;		/* set scsi id */
		cmdblk._cb_xfer = INPUT;					/* set xfer direction */

		/* call the low-level module */
		if (error = (*data_mod->dm_exec) (procdesc, data_mod, &cmdblk,
					opt->pd_scsiopt, DEFAULT))
			error = scsistat (error);				/* decode scsi status */

	} while (error == E_DEVBSY);

	/* repair "reservation conflict" situation */
	if (error == RESERV_CONFLCT)
		error = E_DEVBSY;							/* say device busy */
	return error;
}

/*
 * int scsistat (error) - decode and interpret scsi status
 */
int scsistat (error)
register int	error;			/* scsi status byte */
{
	/*
	 * scsi status non-zero or OS-9 error returned.
	 * decode the scsi status if applicable.
	 */
	switch (error) {
	case CHKCOND:							/* check condition */
		error = handlerr (); 				/* request sense */
		break;
	case BUSY:								/* device busy */
		error = E_DEVBSY;					/* force retry */
		break;
	case CONDMET:							/* condition met */
		/* used by "search" commands:
		 * a driver using these commands should set
		 * a flag here (e.g. found = TRUE) and then
		 * check the flag in the applicable command
		 * issuer.
		 * /
		error = 0;							/* not an error */
		break;
	case INTERMEDIATE:						/* intermediate status */
		/*
		 * intermediate status on linked command.
		 * this probably should never be returned
		 * to here (the low-level beasty should
		 * return when all commands are finished),
		 * so return no error.
		 */
		error = 0;							/* not an error */
		break;
	case RESERV_CONFLCT:					/* reservation conflict */
		/*
		 * consider this a "device busy" error,
		 * BUT - return error intact so that caller
		 * can repair error code outside of the
		 * command transfer loop
		 */
		break;							/* error setup done OUTSIDE loop */
	default:								/* unknown scsi status */
		/*
		 * unknown scsi status:  assume that error code is
		 * an OS-9 error, therefore return value intact.
		 */
		break;
	}
	return error;							/* return decoded error */
}

/*
 * int handlerr () - get error details and process
 */

int handlerr ()
{
	register int			error;		/* error holder */
	register struct rbf_opt	*opt;		/* path descriptor options ptr */
	register Rbfdrive		drvptr;		/* drive table ptr */
	register int			sink;		/* trash holder */

	opt = &pathdesc->rbfopt;		/* point at path descriptor options */

	/* setup "request sense" cdb */
	scsicmd.std.sp_opcode = SC_REQSENSE;		/* request sense */
	scsicmd.std.sp_lun = opt->pd_lun;			/* set target lun */
	scsicmd.std.sp_lbamsbs = 0;					/* lba = 0 */
	scsicmd.std.sp_lba[0] = 0;
	scsicmd.std.sp_lba[1] = 0;
	scsicmd.std.sp_blkcnt = sizeof errdet;		/* error block size */
	scsicmd.std.sp_options = CCSOPTS;			/* always 0 for */

	/* pass cdb details to low-level module */
	cmdblk._cb_cmdptr = &scsicmd;				/* set cmd ptr */
	cmdblk._cb_cmdlen = CDB_STD;				/* set its size */
	cmdblk._cb_pd_lun = opt->pd_lun;			/* set target lun */
	cmdblk._cb_datptr = &err_det;				/* error details buffer */
	cmdblk._cb_datlen = sizeof errdet;			/* amount of data to transfer */
	cmdblk._cb_xfer = INPUT;					/* set data input flag */
	cmdblk._cb_scsi_id = opt->pd_ctrlrid;		/* set scsi id */

	/* get the command sense bytes */
	if ((error = (*data_mod->dm_exec) (procdesc, data_mod, &cmdblk,
					opt->pd_scsiopt, DEFAULT)) == 0) {

		switch (err_det.ed_main) {
		
		case MAIN_NOSENSE:		/* no sense info */
			error = 0;			/* return no error */
			break;
		
		case MAIN_RECOVERED:	/* recovered error */
		
		/*
		 * "recovered" error philosophy:
		 * These types of error indicate that there is some
		 * kind of "soft" error occuring - they are trapped
		 * separately here so that any special handling can
		 * be easily performed (e.g. reassign blocks cmd).
		 * Unless there is a major problem detected at this
		 * point, a "no error" result should be returned.
		 */
			switch (err_det.ed_code) {
			case SENSE_WRITEFLT:	/* recovered write fault */
			case SENSE_NOTRDY:		/* drive not ready */
			case SENSE_LUNFAIL:		/* lun communication failure */
			case SENSE_ID:			/* ID crc or ecc error */
			case SENSE_DATAERR:		/* unrecovered read error */
			case SENSE_NOADDRID:	/* no AM in ID field */
			case SENSE_NOADDR:		/* no AM in Data field */
			case SENSE_NOREC:		/* no record found */
			case SENSE_SEEK:		/* seek error */
			case SENSE_RETRYOK:		/* recovered data by retries */
			case SENSE_ECCOK:		/* recovered data by ecc */
			case SENSE_CONTR:		/* controller error */
				error = 0;			/* return no error */
				break;
			default:
				/* unknown case: return actual sense/error code */
				error = (err_det.ed_main << 8) | err_det.ed_code;
				break;
			}
			break;
		
		case MAIN_NOTRDY:		/* not ready */
			switch (err_det.ed_code) {
			case SENSE_NOTRDY:		/* drive not ready */
				error = E_NOTRDY;	/* say not ready */
				break;
			case SENSE_CONTR:		/* internal controller error */
				error = E_HARDWARE;	/* say hardware error */
				break;
			default:
				/* unknown case: return actual sense/error code */
				error = (err_det.ed_main << 8) | err_det.ed_code;
				break;
			}
			break;
		
		case MAIN_MEDIUM:		/* medium error */
			switch (err_det.ed_code) {
			case SENSE_ID:			/* ID crc or ecc error */
			case SENSE_NOADDRID:	/* no AM in ID field */
			case SENSE_NOADDR:		/* no AM in data field */
			case SENSE_NOREC:		/* no record found */
			case SENSE_SEEK:		/* seek error */
				error = E_SEEK;		/* return seek error */
				break;
			case SENSE_DATAERR:		/* unrecovered read error */
				error = E_READ;
				break;
			case SENSE_DLERR:		/* defect list error */
			case SENSE_DLMISS:		/* primary defect list missing */
				error = E_READ;		/* return read error */
				break;
			case SENSE_MEDIUM:		/* medium format corrupted */
				error = E_BTYP;		/* say media bad type */
				break;
			case SENSE_NOSPARE:		/* no defect spare available */
				error = E_FULL;		/* say media full */
				break;
            case SENSE_NOTRDY:      /* drive not ready  */
                error = E_NOTRDY;   /* say not ready    */
                break;

			default:
				/* unknown case: return actual sense/error code */
				error = (err_det.ed_main << 8) | err_det.ed_code;
				break;
			}
			break;
		
		case MAIN_HARDWARE:		/* hardware error */
			switch (err_det.ed_code) {
			case SENSE_NOINDEX:		/* no index from drive */
				error = E_NOTRDY;	/* say not ready */
				break;
			case SENSE_NOSEEK:		/* no seek complete */
			case SENSE_ID:			/* ID crc or ecc error */
			case SENSE_SEEK:		/* seek positioning error */
				error = E_SEEK;		/* say seek error */
				break;
			case SENSE_WRITEFLT:	/* write fault */
				error = E_WRITE;	/* say write error */
				break;
			case SENSE_LUNFAIL:		/* lun communication failure */
			case SENSE_TRKFOL:		/* track following error */
			case SENSE_DIAGRAM:		/* ram diagnostic error */
			case SENSE_DIAGDATA:	/* data path diagnostic error */
			case SENSE_DIAGPON:		/* power-on diagnostic error */
			case SENSE_CONTR:		/* controller internal error */
			case SENSE_SCSIPAR:		/* scsi parity error */
				error = E_HARDWARE;	/* say hardware error */
				break;
			default:
				/* unknown case: return actual sense/error code */
				error = (err_det.ed_main << 8) | err_det.ed_code;
				break;
			}
			break;
		
		case MAIN_ILGL:			/* illegal request */
			switch (err_det.ed_code) {
			case SENSE_ILLREQ:		/* invalid cmd code */
			case SENSE_ILLFUNC:		/* illegal function for device type */
				error = E_BTYP;		/* say bad type */
				break;
			case SENSE_ILLLBA:		/* illegal block address */
				error = E_SECT;		/* say illegal sector address */
				break;
			case SENSE_ILLCDB:		/* illegal field in cdb */
			case SENSE_ILLFLD:		/* invalid field in parameter list */
				error = E_PARAM;	/* say parameter error */
				break;
			case SENSE_ILLLUN:		/* illegal lun */
				error = E_UNIT;		/* say unit error */
				break;
			default:
				/* unknown case: return actual sense/error code */
				error = (err_det.ed_main << 8) | err_det.ed_code;
			}
			break;
		
		case MAIN_UNITATN:		/* unit attention */
			/* mark drive uninitialized */
			sink = opt->pd_drv;
			drvptr = &rbfstat.drv[sink];	/* point at drive table */
			drvptr->v_init = FALSE;	/* drive is uninitialized */
			started = FALSE;				/* retest UNIT READY */
			return E_DIDC;					/* always treat as ID change */
			break;
		
		case MAIN_ABORT:		/* aborted command */
			switch (err_det.ed_code) {
			case SENSE_REJECT:		/* message reject error */
			case SENSE_RESEL:		/* select/reselect failed */
			case SENSE_INITIATR:	/* initiator detected error */
			case SENSE_INAPPROP:	/* inappropriate message */
				error = E_DEVBSY;	/* say device busy */
				break;
			case SENSE_SCSIPAR:		/* scsi parity error */
				error = E_HARDWARE;	/* say hardware error */
				break;
			default:
				/* unknown case: return actual sense/error code */
				error = (err_det.ed_main << 8) | err_det.ed_code;
				break;
			}
			break;
		
		case MAIN_OVRFL:		/* volume overflow */
			error = E_FULL;			/* say media full */
			break;
		
		case MAIN_DATACMP:		/* data miscompare */
			error = E_WRITE;		/* say write error */
			break;
		
		default:				/* unknown case:  return scsi details */
			error =(err_det.ed_main << 8) | err_det.ed_code;
			break;
		}

	} else error = E_HARDWARE;	/* can't get sense info */
	
	return error;
}

/*
 * int sccsmode ( ) - do a sccs mode sense & mode select
 */
int sccsmode()
{
	register struct rbf_opt *opt;	/* path descriptor options ptr */
	register u_int		blkaddr;	/* logical block for format track */
	register u_int		blklen;		/* block length for mode select */
	register int		error;		/* error holder */

	opt = &pathdesc->rbfopt;				/* point at pd options */

	/* do mode sense for changeable parameters on all pages */
	blkaddr = (PCF_CHANGE << 14) | (PAGE_3 << 8);
	blklen = sizeof(modesense);
	if ((error = execute(SC_MODESENSE, blkaddr, blklen, CCSOPTS,
						&mode_sense, INPUT, CDB_STD)) != 0)
		return error;
			
	/* check for changeable parameters on page 3 */
	if (*((u_short *) mode_sense.msn_p3byte_sect) != 0) {
		/* do full mode sense */
		blkaddr = (PCF_CURRENT << 14) | (PAGE_3 << 8);
		blklen = sizeof(modesense);
		if ((error = execute(SC_MODESENSE, blkaddr, blklen, CCSOPTS,
							&mode_sense, INPUT, CDB_STD)) != 0)
			return error;
				
		/* set the bytes per physical sector from device descriptor */
		mode_sense.msn_p3byte_sect[0] = (opt->pd_ssize & 0xff00) >> 8;
		mode_sense.msn_p3byte_sect[1] = (opt->pd_ssize & 0xff);
		/* Changed at Ed.14,  use sectors/track field due to some drives
		 *    being incapable or refusing to set sectors per track.
		 */
		mode_sense.msn_p3sect_trk[0] = opt->pd_sct >> 8;
		mode_sense.msn_p3sect_trk[1] = opt->pd_sct;
		mode_sense.msn_p3code &= PCODE_MASK;
				
		/* set block descriptor parameters */
		mode_sense.msn_datlen = 0;			/* byte 0 = 0 */
		mode_sense.msn_medtype = 0;			/* medium type = 0 */
		mode_sense.msn_wp = 0;
		mode_sense.msn_res1 = 0;			/* byte 2 = 0 */
		mode_sense.msn_bdlen = 8;			/* blk descriptor length */
		mode_sense.msn_denscode = 0;		/* density code = 0 */
		mode_sense.msn_numblks[0] = 0;		/* number of blocks = 0 */
		mode_sense.msn_numblks[1] = 0;
		mode_sense.msn_numblks[2] = 0;
		mode_sense.msn_res2 = 0;			/* bd byte 4 = 0 */
		blklen = opt->pd_ssize;				/* get sector size value */
		mode_sense.msn_blklen[0] = (blklen & 0xff0000) >> 16;
		mode_sense.msn_blklen[1] = (blklen & 0xff00) >> 8;
		mode_sense.msn_blklen[2] = blklen & 0xff;

		/* do full mode select with sector size set */
		if ((error = execute (SC_MODESELECT, PF_MASK, sizeof modesense, CCSOPTS,
							&mode_sense, OUTPUT, CDB_STD)) != 0)
			return error;					/* abort if failed */
			
	} else {
		/* set block descriptor parameters */
		mode_sel.ms_res1 = 0;				/* byte 0 = 0 */
		mode_sel.ms_medtype = 0;			/* medium type = 0 */
		mode_sel.ms_res2 = 0;				/* byte 2 = 0 */
		mode_sel.ms_bdlen = 8;				/* blk descriptor length */
		mode_sel.ms_denscode = 0;			/* density code = 0 */
		mode_sel.ms_numblks[0] = 0;			/* number of blocks = 0 */
		mode_sel.ms_numblks[1] = 0;
		mode_sel.ms_numblks[2] = 0;
		mode_sel.ms_res3 = 0;				/* bd byte 4 = 0 */
		blklen = opt->pd_ssize;				/* get sector size value */
		mode_sel.ms_blklen[0] = (blklen & 0xff0000) >> 16;
		mode_sel.ms_blklen[1] = (blklen & 0xff00) >> 8;
		mode_sel.ms_blklen[2] = blklen & 0xff;
	
		/* send mode select command to controller */
		if ((error = execute (SC_MODESELECT, PF_MASK, sizeof modesel, CCSOPTS,
							&mode_sel, OUTPUT, CDB_STD)) != 0)
			return error;					/* abort if failed */
	}

	/* issue a get drive capacity command */
	error = execute (SC_READCAP, 0, sizeof dsize, CCSOPTS, 
	                 &drvsize, INPUT, CDB_EXT);

	return error;		/* return with proper results */
}

/*
 * void lxtoa (stg, num) - convert long to 8 ascii hex digits 
 */
void lxtoa (stg, num)
register char	*stg;
register long	num;
{
	register int	i;
	
	for (i = 7; i >= 0; i--, num >>= 4)
		stg[i] = hexchar(num & 0xf);
	stg[8] = '\0';		/* terminate */
}

/*
 * int hexchar (n) - return ascii hex equivalent of n 
 */
int hexchar (n)
register int	n;
{
	if (n > 9)
		n += 0x27;		/* for range a - f */
	return (n + 0x30);
}

/*
 * void strcpy (s1, s2) - another version of string copy
 */
void strcpy(s1, s2)
register char	*s1, *s2;
{
	while (*s1++ = *s2++);
}

/*
 * int set3l (int3, num) - make num into 3 byte integer
 */
int set3l (int3, num)
register char	*int3;
register int	num;
{
	register int	i;
	
	for (i = 2; i >= 0; i--, num >>= 8)
		int3[i] = num & 0xff;
}

/*
 * int c4tol (stg) - convert four characters to long
 */
int c4tol (stg)
register char	*stg;
{
	register int	rslt = 0, i;
	
	for (i = 0; i < 4; i++) 
		rslt = (rslt * 256) + stg[i];
	return rslt;
}

/*
 * void copysect(dest, src) - copy sector from src to dest
 *
 * this routine assumes that dest and src are aligned to
 * a word boundary.  Better performance will result if
 * long-word alignment is used.
 */
void copysect (dest, src, size)
register long	*dest;
register long	*src;
register int    size;
{
	size >>= 2;				/* byte size to long word size */
		
	while (--size >= 0 )
		*dest++ = *src++;
}

/*
 * u_int	addoffsets (blkaddr) - adjust lba according to
 *                       lsn offsets and track offset info.
 */
u_int addoffsets (blkaddr)
u_int	blkaddr;		/* logical block address */
{
	register struct	rbf_opt	*opt;	/* path descriptor ptr */
	
	opt = &pathdesc->rbfopt;		/* point at the path descriptor */
	
	blkaddr += opt->pd_lsnoffs;		/* add lsn offset value */
	
	/*
	 * add in track offset value:
	 *
	 * WARNING: for "autosize" media, the values for heads/
	 * cylinders/sectors per track are not guaranteed,
	 * thus the track offset value is somewhat irrelevant.
	 * To invoke any offsetting on "autosize" media, use
	 * the pd_lsnoffs field.
	 */
	if ((opt->pd_toffs) && ((opt->pd_cntl & CNTL_AUTOSIZE) == 0)) {
		/* adjust lba according to track offsets */
		
		/* add sectors on track zero, side 0 */
		blkaddr += opt->pd_t0s;
		/* add remaining sides of track zero */
		blkaddr += (opt->pd_sct * (opt->pd_sid - 1));
		/* add non-track zero tracks */
		blkaddr += ((opt->pd_toffs - 1) * opt->pd_sid * opt->pd_sct);
	}
	return blkaddr;
}		

memcpy(dest, src, count)
register char *dest;
register char *src;
register int count;
{
	while( --count >= 0 )
		*dest++ = *src++;
	return 0;
}
