/*
 * stat - getstat/putstat routines for the Common Command Set high level driver
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

#include	"rbvccs.h"
 
/*
 * int getstat (code, regs) - do necessary getstats
 */
 
int getstat (code, regs)
u_short				code;
REGISTERS			*regs;
{
	register Rbfdrive drvptr;		/* pointer to the drive table */
	register struct rbf_opt *opt;	/* path descriptor options ptr */
	register int error;				/* error holder */
	Cmdblk cmd;						/* comes in for direct command */

	opt = &pathdesc->rbfopt;				/* point at pd options */
	drvptr = pathdesc->rbfpvt.pd_dtb;		/* point at drive table */
	
	switch (code) {

		/* Direct Device command */
		case SS_DCmd:
			/* if you got here, you are on your own!! */
			error = direct_command( regs, opt, drvptr );
			break;

		/* get drive capacity */
		case SS_DSize:
		/* SS_DSize philosophy:
		 * If the descriptor is "formatable" then we need to supply
		 * the drive capacity based on the descriptor sector size which
		 * may be different from the current drive parameters. To do this
		 * we must issue a mode select (see note on SS_WTrk putstat) to
		 * set the drive for the sector size in the descriptor, and then
		 * do the read capacity command.
		 */
		if (error = initdrv (drvptr, NOPARK))		/* init drive */
				break;								/* abort if failed */

		if (opt->pd_ssize != drvsize.ds_blksize) {	/* are these different? */
			if ((opt->pd_cntl & CNTL_NOFMT) == 0)	/* if format enabled... */
				if (error = sccsmode())				/* .... set drive size */
					return error;
		} else {
			/* added at Ed.14, allways does read capacity */
			error = execute (SC_READCAP, 0, sizeof dsize, CCSOPTS, 
	                         &drvsize, INPUT, CDB_EXT);
	    }

		/* return sector count in users D2 */
		regs->d[2] = drvsize.ds_maxlsn + 1;
		break;
		
		/* check for variable sector size support */
		case SS_VarSect:	
		/* NOTE:
		 * In addition to explicit getstat(SS_VarSect) calls by programs
		 * (e.g. format utility), this call is made by RBF whenever a
		 * new path is opened.  Thus, the Path Descriptor is usually
		 * NOT fully initialized at the time of this call.  The driver
		 * should be aware of this fact, and the following points should
		 * be noted regarding the Path Descriptor:
		 *
		 * - the options section (i.e. the section copied from the options
		 *   section of the device descriptor) is valid.
		 * - the "system-global" section is valid, WITH THE EXCEPTION OF
		 *   pd_buf (BUFFER POINTER).  pd_buf cannot be set by RBF until
		 *   the sector size of the media is known (i.e. this call has
		 *   completed).
		 * - the file-manager section is NOT initialized, WITH THE EXCEPTION
		 *   OF pd_dtb (DRIVE TABLE POINTER).  RBF will have validated the
		 *   logical drive number (PD_DRV) and set up the drive table
		 *   pointer prior to calling this routine.
		 *
		 * When this call is made, the driver will interogate the controller
		 * to find out the current sector size, and take action as follows:
		 *
		 * - if the driver supports device descriptors with a sector size
		 *   (pd_ssize) of 0, then the current sector size of the media is
		 *   returned from the controller and the pd_ssize field set to
		 *    the current media value.
		 *
		 * - if the sector size field is non-zero, and it differs from
		 *   the media's current value, then:
		 *
		 *      a) if the path descriptor states that the media is
		 *         formattable (pd_cntl) then the controller is set
		 *         to the new media sector size.
		 *      b) if the path descriptor states that the media is
		 *         NOT formattable, then an error is returned (there
		 *         is a sector size mismatch).
		 */
			error = initdrv( drvptr, NOPARK );
			break;

		/* Unknown Service Requests here */
		default:
			error = E_UNKSVC;				/* return error */
			break;
	}
	return error;
}

/*
 * int putstat (code, regs) - perform setstat operations
 */
 
int putstat (code, regs)
u_short				code;
register REGISTERS	*regs;
{
	register Rbfdrive	drvptr;		/* pointer to the drive table */
	register struct rbf_opt *opt;	/* path descriptor options ptr */
	register u_int		blkaddr;	/* logical block for format track */
	register u_int		blklen;		/* block length for mode select */
	register int		error;		/* error holder */
	int                 retry;		/* retry flag for retore */
	extern int direct_command();	/* direct command interface */

	opt = &pathdesc->rbfopt;				/* point at pd options */
	drvptr = pathdesc->rbfpvt.pd_dtb;		/* point at drive table */
	
	switch (code) {

	/* Direct Device command */
	case SS_DCmd:
		/* if you got here, you are on your own!! */
		error = direct_command( regs, opt, drvptr );
		break;

	/* RESTORE disk head routine */
	case SS_Reset:
	 	if (error = initdrv (drvptr, NOPARK))		/* init drive */
				break;								/* abort if failed */
		retry = FALSE;								/* set no retry */
		do {
			if(error = execnoxfer (SC_RESTORE, 0, 0, CCSOPTS, CDB_STD)) {
				/* retry once on ID change */
				if( (error == E_DIDC) && (!retry) ) {
					if( error = initdrv(drvptr, NOPARK) )
						return error;
					retry = TRUE;				/* mark for retry */
				} else
					return error;
			} else
				retry = FALSE;
		} while( retry );						/* redo only once */
		break;
	
	/* WRITE-TRACK routine. Used for disk formatting */
	case SS_WTrk:
	
		/*
		 * parameters passed:
		 *	regs->a[0]	= track buffer ptr
		 *	regs->a[1]	= logical interleave table ptr
		 *	regs->d[2]	= physical track/cylinder number
		 *	regs->d[3], bits 15-8	= physical side/head number
		 *	regs->d[3], bits 7-0	= dd_fmt flags:
		 *							bit 0 = side flag (0 or 1)
		 *							bit 1 = density (FM or MFM)
		 *							bit 2 = track density (48tpi or 96tpi)
		 */
		
		/* check if formatting allowed on device */
		if (opt->pd_cntl & CNTL_NOFMT)
			error = E_FORMAT;			/* format protected */
		else if (opt->pd_cntl & CNTL_PARTDSK)
			error = E_FORMAT;			/* no physical format with partition */
		else {
			/* sccs format philosophy :
			 * do mode sense for changeable parameters on all pages
			 * if parameters on page 3 are changeable
					do mode sense for current parameters on all pages
					set appropriate parameters on page 3
					set block descriptor parameters
					do mode select for all pages
			   else
			   		set block descriptor parameters
			   		do "short" mode select (no parameter pages)
			   endif
			 * issue format unit command
			 * mark drive not inized to force new get drive capacity
			 */
			 
			/* check that write-track being performed on
			 * head 0 of base physical cylinder.  If so,
			 * issue "format media" command, else return no error.
			 */
			
			if ((regs->d[2] == (u_int) opt->pd_toffs)
								&& (((regs->d[3] >> 8) & 0xff) == 0))
				blkaddr = 0;			/* set starting block address */
			else {
				/* ignore write-tracks for non cyl 0, head 0 */
				error = 0;					/* return no error */
				break;
			}

			if (error = initdrv (drvptr, NOPARK))	/* init drive */
				break;					/* abort if failed */

			drvptr->v_zerord = FALSE;	/* mark sector zero unbuffered */
			drvptr->v_init = FALSE;		/* drive not inized */

			/* do mode sense & mode select */
			if (error = sccsmode())
				return error;
				
			/* issue format media command */
			error = execnoxfer (SC_FORMAT, 0, regs->d[4], CCSOPTS, CDB_STD);
		}
		break;

	/* Unknown Service Requests here */
	default:
		error = E_UNKSVC;				/* return error */
		break;
	}
	return error;
}

/*
 *  This is the direct device command interface.
 *       There is not much that we can do to validate what is passed
 *       down.  There are some dangers involved with useing this command.
 *
 *   Incomming Parameters:
 *        regs->a[0]   Cmdblk  (See dcmd.h & scsidc.h)
 */

int direct_command( regs, opt, drvptr )			
register REGISTERS *regs;
struct rbf_opt *opt;
Rbfdrive drvptr;
{
	register Cmdblk cmd;			/* high level command block */
	register Dcmd dcmd;				/* direct command structure */
	register u_char bit_bucket;		/* place to store results */
	register int error;				/* error returns from functions */
	union scsipkt *scp;				/* scsi packet pointer */
	int permit;						/* permissions needed for command to work */
	int data_access;				/* permission result for command data */
	int error_access,errflg;		/* error information */
	extern int direct_command();	/* direct command interface */
	
	data_access = error_access = errflg = 0;
	
 	if (error = initdrv (drvptr, NOPARK))		/* init drive */
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
	cmd->_cb_pd_lun = opt->pd_lun;
	cmd->_cb_scsi_id = opt->pd_ctrlrid;
	scp = cmd->_cb_cmdptr;
	scp->std.sp_lun = opt->pd_lun;
	    
	/* good luck, well see you when you get back.......
	 *
	 * call the low-level module:
	 *
	 */
	if( (error = (*data_mod->dm_exec)(procdesc, data_mod, cmd, 
					opt->pd_scsiopt, DEFAULT))) {

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
