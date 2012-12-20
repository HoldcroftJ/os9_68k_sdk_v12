/*
 * misc - miscellaneous functions for the high level sbteac driver
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

#include	"sbteac.h"				/* gets all the defs */

/*
 * initdrv() - do any special drive startup features
 */
initdrv()
{
	register int	error;			/* error holder */
	auto int        timer;			/* time out counter */
	auto int		rdycmd;			/* ready unit cmd code */
	auto int		action;			/* immediate return if 1 */
	auto int		blkcnt;			/* blockcnt field flags */
	

	/* see if we have sent device parameters as yet */
	if (sbdata_mod->online)
		return 0;							/* device already on-line */
	
	/* conditional options */
	rdycmd = SC_TESTREADY;			/* ready unit using TESTREADY */
	action = 0;						/* wait for command to return */
	blkcnt = 0;						/* blockcnt field = 0 */

	/* see if unit ready for service */
	error = 0;
	timer = READY_TIMEOUT;
	do {
		if( error )
			_f_sleep( SLEEP_TIME );			/* wait a bit */
	
		error = execnoxfer (rdycmd, blkcnt, CMDOPTS, action);
		if( !(timer--) )
			break;
	} while ((error != 0) && ((err_det.ed_main == SENSE_UNITATN) ||
	                          (err_det.ed_main == SENSE_NOTRDY))  );
	
	if (error)
		return error;						/* abort if can't start unit */

	/* send "mode select" command */
	mode_sel.ms_res1 = 0;					/* reserved */
	mode_sel.ms_res2 = 0;
	mode_sel.ms_res3 = 0;
	mode_sel.ms_buffmode = MS_BUFF;			/* select buffered operation */
	mode_sel.ms_speed = 0;					/* always 0 */
	mode_sel.ms_bdlen = 8;					/* one block descriptor */
	mode_sel.ms_denscode = MS_TEACDENS;		/* special for teac */
	mode_sel.ms_numblks[0] = 0;				/* block count = 0 */
	mode_sel.ms_numblks[1] = 0;
	mode_sel.ms_numblks[2] = 0;
	mode_sel.ms_res4 = 0;					/* reserved */
	mode_sel.ms_blklen[0] = PHYSIZE >> 16;	/* set physical block size */
	mode_sel.ms_blklen[1] = PHYSIZE >> 8;
	mode_sel.ms_blklen[2] = PHYSIZE & 0xff;
    mode_sel.ms_TeacRes = 0;				/* not directory mode */

	error = 0;
	do {
		if( error )
			_f_sleep( SLEEP_TIME );
			
		error = execute (SC_MODESELECT, sizeof modesel, CMDOPTS, 0,
						&mode_sel, OUTPUT, TRUE);
	} while ((error != 0) && (err_det.ed_main == SENSE_UNITATN));

	if (error)
		return error;					/* abort if can't select mode */

	/* drive initialized, mark it so */
	sbdata_mod->online = TRUE;						/* mark drive on-line */
	return 0;							/* return success */
}

/*
 * execute(opcode, blkcount, opts, action, buff, xferflags, bytemode)
 *
 */

int execute(opcode, blkcount, opts, action, buff, xferflags, bytemode)
u_int				opcode,			/* the command opcode */
					blkcount,		/* the number of blocks to xfer */
					opts,			/* options */
					action;			/* sp_action flags */
char				*buff;			/* the buffer pointer */
u_int				xferflags,		/* data transfer direction flag */
					bytemode;		/* blkcount mode */
{
	register int	error;			/* keep track of errors */
	auto int		sink;			/* junk holder */

	do {
		/* setup scsi command packet */
		scsicmd.sp_opcode = opcode;					/* set opcode */
		scsicmd.sp_lun = scsilun;					/* set LUN */
		scsicmd.sp_action = action;					/* set action flags */
		scsicmd.sp_blkcnt[0] = (blkcount & 0xff0000) >> 16;	/* blk cnt */
		scsicmd.sp_blkcnt[1] = (blkcount & 0xff00) >> 8;
		scsicmd.sp_blkcnt[2] = (blkcount & 0x00ff);
		scsicmd.sp_options = opts;					/* command options */

		/* setup command block for low-level module */
		cmdblk._cb_cmdptr = &scsicmd;				/* ptr to cmd block */
		cmdblk._cb_cmdlen = sizeof scsipkt;			/* its size */
		cmdblk._cb_datptr = (void *) buff;			/* data buffer ptr */
		
		/* figure whether the count is a block or byte count */
		if (bytemode == TRUE)
			cmdblk._cb_datlen = blkcount;				/* byte count */
		else
			cmdblk._cb_datlen = blk_size * blkcount;	/* block count */

		cmdblk._cb_xfer = xferflags;				/* set transfer dir */
		cmdblk._cb_scsi_id = scsiid;				/* set scsi id */
		cmdblk._cb_pd_lun = scsilun;				/* target LUN */

		/* call the low-level module */
		if (error = (*data_mod->dm_exec) (procdesc, data_mod, &cmdblk,
					optflags, DEFAULT))
			error = scsistat (error);				/* decode scsi status */

		/* if device is busy, wait a while (off-line commands
		 * may be in progress).
		 */
		if (error == E_DEVBSY)
			sink = _f_sleep(SLEEP_TIME);			/* delay a while */

	/* repeat command if device/bus is busy */
	} while (error == E_DEVBSY);
	
	/* repair "reservation conflict" situation */
	if (error == RESERV_CONFLCT)
		error = E_DEVBSY;							/* say device busy */
	return error;
}

/*
 * execnoxfer(opcode, blkcount, opts, action)
 *
 * this function executes a command without transfering any data
 */
 
int execnoxfer(opcode, blkcount, opts, action)
u_int				opcode,			/* the opcode */
					blkcount,		/* the block address */
					opts,			/* information for command byte #5 */
					action;			/* sp_action flags */
{
	register int	error;			/* error stash */
	auto int		sink;			/* junk holder */

	do {
		/* setup the scsi cmd block */
		scsicmd.sp_opcode = opcode;					/* cmd opcode */
		scsicmd.sp_lun = scsilun;					/* set target LUN */
		scsicmd.sp_action = action;					/* set action flags */
		scsicmd.sp_blkcnt[0] = (blkcount & 0xff0000) >> 16;	/* blk cnt */
		scsicmd.sp_blkcnt[1] = (blkcount & 0xff00) >> 8;		
		scsicmd.sp_blkcnt[2] = (blkcount & 0x00ff);
		scsicmd.sp_options = opts;					/* cmd options */

		/* setup cmd block for low-level module */
		cmdblk._cb_cmdptr = &scsicmd;				/* scsi cmd ptr */
		cmdblk._cb_cmdlen = sizeof scsipkt;			/* its size */
		cmdblk._cb_datptr = NULL;					/* no data */
		cmdblk._cb_datlen = 0;						/* no data */
		cmdblk._cb_xfer = INPUT;					/* transfer dir */
		cmdblk._cb_scsi_id = scsiid;				/* set scsi id */
		cmdblk._cb_pd_lun = scsilun;				/* target LUN */

		/* call the low-level module */
		if (error = (*data_mod->dm_exec) (procdesc, data_mod, &cmdblk,
					optflags, DEFAULT))
			error = scsistat (error);				/* decode scsi status */
	
		/* if device is busy, wait a while (off-line commands
		 * may be in progress).
		 */
		if (error == E_DEVBSY)
			sink = _f_sleep(SLEEP_TIME);			/* delay a while */

	/* repeat command if device/bus is busy */
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
		/* used by "search commands:
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
 * int handlerr() - get error details and process
 */
 
int handlerr()
{
	register int	error;			/* error holder */
	register int	sink;			/* junk holder */
	register int	oldcmd;			/* cmd that provoked this routine */
	
	oldcmd = scsicmd.sp_opcode;				/* save original command */
	residual = 0;							/* residual length is 0 */

	/* setup request tape sense command block */
	scsicmd.sp_opcode = SC_REQSENSE;		/* request sense */
	scsicmd.sp_lun = scsilun;				/* set target lun */
	scsicmd.sp_action = 0;					/* no special action flags */
	scsicmd.sp_blkcnt[0] = 0;				/* set sense block size */
	scsicmd.sp_blkcnt[1] = 0;
	scsicmd.sp_blkcnt[2] = sizeof errdet;
	scsicmd.sp_options = CMDOPTS;			/* the option byte */

	/* setup command block for low-level module */
	cmdblk._cb_cmdptr = &scsicmd;			/* ptr to scsi cmd */
	cmdblk._cb_cmdlen = sizeof scsicmd;		/* its size */
	cmdblk._cb_datptr = &err_det;			/* error details buffer */
	cmdblk._cb_datlen = sizeof errdet;		/* data count to transfer */
	cmdblk._cb_xfer = INPUT;				/* transfer dir */
	cmdblk._cb_scsi_id = scsiid;			/* set scsi id */
	cmdblk._cb_pd_lun = scsilun;			/* target LUN */

	/* call the low level driver */
	error = 0;
	do {
		if( error )
			_f_sleep( SLEEP_TIME );		/* back of for a bit */
			
		error = (*data_mod->dm_exec) (procdesc, data_mod, &cmdblk,
					optflags, DEFAULT);
	} while (error == BUSY);
	
	if (error == 0) {
		/* sense request succeeded:  decode error details */
		if (err_det.ed_valid)				/* set residual block count */
			residual = (err_det.ed_info[0] << 24) +
						(err_det.ed_info[1] << 16) +
						(err_det.ed_info[2] << 8) +
						(err_det.ed_info[3]);
		
		/* error decoding here */
		
		switch (err_det.ed_main) {
		
		case SENSE_RECOVER:
			if (err_det.ed_filemrk)	{		/* file mark */
				sbdata_mod->fstatus |= TAPE_EOF;	/* mark eof pending */
				error = 0;					/* no error */
				break;
			} else if (err_det.ed_eom) {	/* end of medium */
				if (oldcmd == SC_RESTORE) {	/* rewinding tape *
					error = 0;				/* no error */
					break;
				} else if (oldcmd == SC_WRITE) {
					/* writing, early eom warning */
					if ((sbdata_mod->fstatus & TAPE_EOM) == 0)
						sbdata_mod->fstatus |= TAPE_FULL;
					sbdata_mod->fstatus |= TAPE_EOM;	/* flag eom pending */
					error = 0;				/* no error this write (yet) */
					break;
				} else if (oldcmd == SC_READ) {
					sbdata_mod->fstatus |= TAPE_EOM;	/* flag eom pending */
					error = 0;
					break;
				} else {
					error = E_FULL;			/* assume media full */
					break;
				}
			} else {						/* no sense, no error */
				error = 0;					/* no error */
				break;
			}
		
		case SENSE_RECOVOK: 				/* cmd ok after retries */
			error = 0;
			break;
		
		case SENSE_NOTRDY:					/* device not ready */
			error = E_NOTRDY;
			break;
		
		case SENSE_DATAERR:					/* medium error */
			if ((oldcmd == SC_WRITE) || (oldcmd == SC_WRITEMARK))
				error = E_WRITE;			/* write error */
			else
				error = E_READ;				/* read error */
			break;
		
		case SENSE_HARDWARE:				/* hardware error */
			error = E_HARDWARE;
			break;
		
		case SENSE_ILGL:					/* parameter block problem */
											/* OR offline command in progress */
			/* setup test unit ready command block */
			scsicmd.sp_opcode = SC_TESTREADY;		/* test unit ready */
			scsicmd.sp_lun = scsilun;				/* set target lun */
			scsicmd.sp_action = 0;					/* no special action flags */
			scsicmd.sp_blkcnt[0] = 0;				/* set sense block size */
			scsicmd.sp_blkcnt[1] = 0;
			scsicmd.sp_blkcnt[2] = 0;
			scsicmd.sp_options = CMDOPTS;			/* the option byte */

			/* setup command block for low-level module */
			cmdblk._cb_cmdptr = &scsicmd;			/* ptr to scsi cmd */
			cmdblk._cb_cmdlen = sizeof scsipkt;		/* its size */
			cmdblk._cb_datptr = 0;					/* error details buffer */
			cmdblk._cb_datlen = 0;					/* data count to transfer */
			cmdblk._cb_xfer = INPUT;				/* transfer dir */
			cmdblk._cb_scsi_id = scsiid;			/* set scsi id */
			cmdblk._cb_pd_lun = scsilun;			/* target LUN */

			/* call the low level driver */
			error = 0;
			do {
				if( error )
					_f_sleep( SLEEP_TIME );		/* back off a bit */
					
				error = (*data_mod->dm_exec) (procdesc, data_mod, &cmdblk,
							optflags, DEFAULT);
			} while (error == BUSY);
	
			/*
			 * The TEAC drive will return a non-good status when a command
			 * in the immediate mode is still in progress, therefore if the
			 * result of the test unit ready command is non zero assume the
			 * drive is busy, else the previous command issued is illegal.
			 */
			 
			switch (error) {
				case GOOD :
				case CHKCOND :
					error = E_DEVBSY;
					break;
				default :
					error = E_BTYP;
					break;
			}
			break;
		
		case SENSE_UNITATN:					/* unit attention */
			if ((sbdata_mod->blknum == 0) && (sbdata_mod->markcnt == 0))
				error = E_DEVBSY;			/* force retry on casette change
											   after rewind etc. */
			else {
				sbdata_mod->online = FALSE;		/* mark drive uninitialized */
				error = E_DIDC;
			}
			break;
		
		case SENSE_WRTPRT:					/* data protect */
			error = E_WP;
			break;
		
		case SENSE_BLANK:					/* blank tape */
			sbdata_mod->fstatus |= TAPE_EOF;	/* set eof pending */
			error = E_EOF;
			break;
		
		case SENSE_ABORT:					/* aborted command */
			error = E_DEVBSY;				/* say busy (for retry) */
			break;
		
		case SENSE_OVRFLW:					/* volume overflow */
			/*
			 * Volume overflow strategy: 
			 * The Teac drive will return this sense key when an operation
			 * is attempted past physical EOM. This should be considered
			 * a write error since NO recovery is possible due to SBF
			 * buffering of write requests from the calling process. Even
			 * if this seems like an E$Full situation it is really E$Write.
			 */
			sbdata_mod->fstatus |= TAPE_EOM;	/* set eom pending */
			error = E_WRITE;
			break;
		
		default:							/* unknown error */
			error = (err_det.ed_main << 8) | 0xff;	/* return main sense */
			break;
		}

	} else error = E_HARDWARE;		/* can't get sense status */

#ifdef DEBUG
	if ((error) && (error != E_DEVBSY))
			sink = error;			/* breakpoint location */
#endif
	return(error);
}

/*
 * lxtoa(stg, num) - convert long to 8 ascii hex digits 
 */
 
lxtoa(stg, num)
register char   *stg;
register long   num;
{
    register int    i;
    
    for (i = 7; i >= 0; i--, num >>= 4)
        stg[i] = hexchar(num & 0xf);
    stg[8] = '\0';      /* terminate */
}

/*
 * int hexchar(n) - return ascii hex equivalent of n 
 */

int hexchar(n)
register int    n;
{
    if (n > 9)
        n += 0x27;      /* for range a - f */
    return (n + 0x30);
}

/*
 * strcpy(s1, s2) - another version of string copy
 */
 
strcpy(s1, s2)
register char   *s1, *s2;
{
    while (*s1++ = *s2++);
}

