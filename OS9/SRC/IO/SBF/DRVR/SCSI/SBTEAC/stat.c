/*
 * stat - stat routines for the high-level sbteac driver.
 */
 
/*
 * Copyright 1989 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include "sbteac.h"					/* include all the defs */

/*
 * getstat - get device status.
 */
getstat(stat_code, arg)
register u_short		stat_code;		/* status code */
register int			arg;			/* passed argument */
{
	register u_int		error;			/* error holder */
	extern int          direct_command();	/* direct command interface */

	switch (stat_code) {

		/* Direct Device command */
		case SS_DCmd:
			/* if you got here, you are on your own!! */
			error = direct_command( pathdesc->path.pd_rgs );
			break;

		/* get device options */
		case SS_Opt:
			error = 0;			/* return no error even if nothing done */
			break;

		/* test device ready */
		case SS_Ready:
			error = 0;			/* always ready */
			break;
		
		/* test for end-of-file */
		case SS_EOF:
			error = 0;			/* never EOF */
			break;
		
		/* all other codes */
		default:
			error = E_UNKSVC;
			break;
	}
	return (error);				/* return error status */
}
	

/*
 * putstat - set device status.
 */
putstat(stat_code, arg)
u_short		stat_code;					/* status code */
int			arg;						/* passed argument */
{
	register u_int		error;			/* error holder */
	register int		sink;			/* junk holder */
	auto int			action;			/* immediate return if 1 */
	auto int			retenopt;		/* retension method */
	extern int          direct_command();	/* direct command interface */

	/* conditional options */
	action = IMMED;						/* immediate return on commands */
	retenopt = RETEN + LOAD;			/* RETEN and LOAD on retension */
	
	switch (stat_code) {

		/* Direct Device command */
		case SS_DCmd:
			/* if you got here, you are on your own!! */
			error = direct_command( pathdesc->path.pd_rgs );
			break;

		/* set device options */
		case SS_Opt:
			error = 0;						/* return no error */
			break;
		
		/* rewind tape */
		case SS_Reset:
			if (error = initdrv())			/* initialize drive */
				return error;				/* abort if failed */
			error = execnoxfer (SC_RESTORE, 0, CMDOPTS, action);
			/* clear eom and eof status */
			sbdata_mod->fstatus &= ~(TAPE_EOM | TAPE_EOF);
			sbdata_mod->blknum = 0;			/* at block 0 */
			sbdata_mod->markcnt = 0; 		/* seen no marks yet */
			break;
		
		/* write/erase tape:
		 * if <arg> is positive, then write <arg> blocks.
		 * if <arg> = -1, then erase tape from current
		 * position to EOM.
		 */
		case SS_Feed:
		
		/* WARNING:  THIS ROUTINE WILL NOT WORK - SC_ERASE WILL
		 ONLY ERASE IF AT BOT.
		 */
			if (error = initdrv())			/* initialize drive */
				return error;				/* abort if failed */

			/* Teac has an "Immed" bit at bit 1 an requires bit 0 set */
			error = execnoxfer(SC_ERASE, 0, CMDOPTS, ((action<<1)|1) );

			sbdata_mod->fstatus &= TAPE_EOF;/* clear eof status */
			sbdata_mod->blknum = 0;			/* at block 0 */
			sbdata_mod->markcnt = 0; 		/* seen no marks yet */
			break;
		
		/* sequence down tape (put it off-line) */
		case SS_SQD:
			
			/* QUESTION - SHOULD WE ALWAYS RETENSION OR NOT?? */
			/* FOR NOW, LETS NOT */
			
			if (error = initdrv())			/* initialize drive */
				return error;				/* abort if failed */
			error = execnoxfer(SC_REMOVAL, 0, CMDOPTS, 0);
			sbdata_mod->online = FALSE;		/* mark drive off-line */
			/* clear eom and eof status */
			sbdata_mod->fstatus &= ~(TAPE_EOM | TAPE_EOF);
			sbdata_mod->blknum = 0;			/* at block 0 */
			sbdata_mod->markcnt = 0; 		/* no marks seen yet */
			break;
		
		/* retension tape */
		case SS_Reten:
			if (error = initdrv())			/* initialize drive */
				return error;				/* abort if failed */
			error = execnoxfer(SC_LOAD, retenopt, CMDOPTS, action);
			/* clear eom and eof status */
			sbdata_mod->fstatus &= ~(TAPE_EOM | TAPE_EOF);
			sbdata_mod->blknum = 0;			/* at block 0 */
			sbdata_mod->markcnt = 0; 		/* no marks seen yet */
			break;
		
		/* write file-mark(s) */
		case SS_WFM:
			if (error = initdrv())			/* initialize drive */
				return error;				/* abort if failed */
			error = execnoxfer(SC_WRITEMARK, arg, CMDOPTS, 1);
			sbdata_mod->fstatus &= ~TAPE_EOF;	/* clear eof status */
			sbdata_mod->blknum = 0;			/* at block 0 */
			sbdata_mod->markcnt += arg; 	/* increment mark count */
			break;
		
		/* read past file mark(s) */
		case SS_RFM:
			if (error = initdrv())			/* initialize drive */
				return error;				/* abort if failed */
			if (arg < 0) {
				/* negative argument count implies reverse search,
				 * so for TEAC, we have to rewind and then
				 * space forward.
				 */
				/* do the rewind */
				error = execnoxfer(SC_RESTORE, 0, CMDOPTS, action);
				
				if (((int)sbdata_mod->markcnt + arg) > 0)
					error = execnoxfer(SC_SPACE, sbdata_mod->markcnt + arg, 
										CMDOPTS, SPACE_FM);
				else {
					sbdata_mod->markcnt = 0;		/* tried to pass BOT */
					arg = 0;						/* reset for update */
				}
			}
			else
				error = execnoxfer(SC_SPACE, arg, CMDOPTS, SPACE_FM);
			sbdata_mod->markcnt += arg;				/* set current file mark */
			sbdata_mod->blknum = 0;					/* no blocks seen yet */
			sbdata_mod->fstatus &= ~TAPE_EOF;		/* clear eof status */
			break;
		
		/* skip block(s) */
		case SS_Skip:
			if (error = initdrv())			/* initialize drive */
				return error;				/* abort if failed */
			if (arg < 0) {
				/* negative argument count implies reverse search,
				 * so for TEAC, we have to rewind and then
				 * space forward.
				 */
				/* do the rewind */
				error = execnoxfer(SC_RESTORE, 0, CMDOPTS, action);
				
				/* skip forward to current file mark */
				if (sbdata_mod->markcnt > 0)
					error = execnoxfer(SC_SPACE, sbdata_mod->markcnt, 
										CMDOPTS, SPACE_FM);
				
				/* convert logical (sbf) blocks to physical blocks */
				sink = sbdata_mod->blknum + (arg * lbsize);
				
				/* space forward to current block */
				if (sink > 0) {
					error = execnoxfer(SC_SPACE, sink, CMDOPTS, SPACE_BLKS);
				}
				else {
					/* attempting to skip to a negative block number */
					sbdata_mod->blknum = 0;			/* no blocks seen yet */
					arg = 0;						/* reset arg */
				}
			}
			else {
				/* positive argument:
				 * skip forward 'arg' sbf blocks
				 */
				error = execnoxfer(SC_SPACE, (arg * lbsize), CMDOPTS, SPACE_BLKS);
			}
			sbdata_mod->blknum += (arg * lbsize);	/* set current block */
			sbdata_mod->fstatus &= ~TAPE_EOF;		/* clear eof status */
			break;

		/* unknown service requests here */
		default:
			return E_UNKSVC;
			break;
	}
	return(error);
}


/*
 *  This is the direct device command interface.
 *       There is not much that we can do to validate what is passed
 *       down.  There are some dangers involved with useing this command.
 *
 *   Incomming Parameters:
 *        regs->a[0]   Cmdblk  (See dcmd.h & scsidc.h)
 */

int direct_command( regs )
register REGISTERS *regs;
{
	register Cmdblk cmd;			/* high level command block */
	register Dcmd dcmd;				/* direct command structure */
	register u_char bit_bucket;		/* place to store results */
	register int error;				/* error returns from functions */
	struct sbf_opt *opt;	
	struct scsipkt_str *scp;		/* scsi packet pointer */
	int permit;						/* permissions needed for command to work */
	int data_access;				/* permission result for command data */
	int error_access,errflg;		/* error information */
	
	opt = (struct sbf_opt *)&pathdesc->path.fm_opt;
	
	data_access = error_access = errflg = 0;	/* init vars */
 	if (error = initdrv ())						/* init drive */
		return error;
		
	dcmd = regs->a[0];					/* direct command block */
	cmd = (Cmdblk)dcmd->dcmdblk;		/* command block */
	permit = (S_IREAD | S_IWRITE );		/* permissions nessesary */
	
	/* validate the call, as best as possible */
	if( (dcmd->manager != opt->pd_dtp) ||
	    (dcmd->device != SCSIdevice ) ||
	    (dcmd->dcmdsync != DCMDSYNC ) )
		return E_PARAM;

	/* check data space access permissions 
	 *   in order for the command to proceed, all data spaces must be
	 *   accessable to the caller, and the caller must be super group
	 *   ( 0.n ).
	 */
	if( cmd->_cb_datlen )		/* if data to be transfered, do the test */
		data_access = _f_chkmem( cmd->_cb_datlen, permit, cmd->_cb_datptr );
	
	if( cmd->_cb_errlen ) {		/* if there is an error block specified */
		error_access = _f_chkmem( cmd->_cb_errlen, permit, cmd->_cb_errptr );
		errflg = TRUE;
	}
		
	if( (procdesc->_group != 0 ) || data_access || error_access ||
	     _f_chkmem( sizeof(struct d_cmd), permit, dcmd)  ||
	     _f_chkmem( cmd->_cb_cmdlen, permit, &cmd->_cb_cmdptr) )
	 	return E_PERMIT;   

	/* only allow access to device as passed by the caller */
	cmd->_cb_pd_lun = scsilun;
	cmd->_cb_scsi_id = scsiid;
	scp = cmd->_cb_cmdptr;
	scp->sp_lun = scsilun;
	    
	/* good luck, we'll see you when you get back....... */
	if( (error = (*data_mod->dm_exec)(procdesc, data_mod, cmd, 
	                                             optflags, DEFAULT))) {

		/*  
		 * clear out these, in case scsistat does not perform
		 * a "request sense" i.e. error was not a cmd error
		 */
		err_det.ed_valid = FALSE;
		err_det.ed_main = 0;	
		
		if( errflg ) {
			error = scsistat( error );			/* will do request sense etc. */
			_f_move( cmd->_cb_errptr, &err_det, cmd->_cb_errlen );
		}
	}
	return error;
}

