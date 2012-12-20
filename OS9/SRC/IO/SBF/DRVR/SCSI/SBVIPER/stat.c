/*
 * stat - stat routines for the high-level sbviper driver.
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
|  02 99/05/12 Fixed EOF clearing in SS_Feed.                          rkw  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

#include "sbviper.h"					/* include all the defs */


/*
 * getstat - get device status.
 */
getstat(stat_code, arg)
register u_short		stat_code;		/* status code */
register int			arg;			/* passed argument */
{
	register u_int		error;				/* error holder */
	extern int          direct_command();	/* direct command handler */

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
	extern int          direct_command();	/* direct command handler */

	/* conditional options */
#ifdef EXABYTE
	action = 0;							/* wait for commands to return */
	retenopt = LOAD;					/* LOAD on retension */
#else
	action = IMMED;						/* return immediately */
	retenopt = RETEN + LOAD;			/* RETEN and LOAD on retension */
#endif
	
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
			fstatus &= ~(TAPE_EOM | TAPE_EOF);
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
			error = execnoxfer(SC_ERASE, 0, CMDOPTS, action);
			fstatus &= ~TAPE_EOF;			/* clear eof status */
			break;
		
		/* sequence down tape (put it off-line) */
		case SS_SQD:
			
			/* QUESTION - SHOULD WE ALWAYS RETENSION OR NOT?? */
			/* FOR NOW, LETS NOT */
			
			if (error = initdrv())			/* initialize drive */
				return error;				/* abort if failed */
			error = execnoxfer(SC_LOAD, 0, CMDOPTS, action);
			online = FALSE;					/* mark drive off-line */
			/* clear eom and eof status */
			fstatus &= ~(TAPE_EOM | TAPE_EOF);
			break;
		
		/* retension tape */
		case SS_Reten:
			if (error = initdrv())			/* initialize drive */
				return error;				/* abort if failed */
			error = execnoxfer(SC_LOAD, retenopt, CMDOPTS, action);
			/* clear eom and eof status */
			fstatus &= ~(TAPE_EOM | TAPE_EOF);
			break;
		
		/* write file-mark(s) */
		case SS_WFM:
			if (error = initdrv())			/* initialize drive */
				return error;				/* abort if failed */
			error = execnoxfer(SC_WRITEMARK, arg, CMDOPTS, 0);
			fstatus &= ~TAPE_EOF;			/* clear eof status */
			break;
		
		/* read past file mark(s) */
		case SS_RFM:
			if (error = initdrv())			/* initialize drive */
				return error;				/* abort if failed */
			error = execnoxfer(SC_SPACE, arg, CMDOPTS, SPACE_FM);
			fstatus &= ~TAPE_EOF;			/* clear eof status */
			break;
		
		/* skip block(s) */
		case SS_Skip:
			if (error = initdrv())			/* initialize drive */
				return error;				/* abort if failed */
			error = execnoxfer(SC_SPACE, (arg * lbsize), CMDOPTS,
								SPACE_BLKS);
			fstatus &= ~TAPE_EOF;			/* clear eof status */
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

	data_access = error_access = errflg = 0;
 	if (error = initdrv ())			/* init drive */
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
	    
	/* good luck, well see you when you get back....... */
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

