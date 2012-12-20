/*
 * stat - getstat/putstat routines for the rbteac high level driver
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
 *
 *	Edition History:														
 *	#   Date    Comments												By
 *	-- -------- ------------------------------------------------------- ---
 *   00 95/07/12 Added Edition History for this file.                   GbG
 *   01 95/07/12 ANSI Version.                                          GbG
 *               ---- OS-9/68K V3.0.3 Released ----
 *               ---- OS-9/68K V3.1 Released ----
 */

#include    "rbteac.h"
#include	"rbproto.h"

/*
 * int getstat (code, regs) - do necessary getstats
 */
 
#if defined (__STDC__) || defined (_ANSI_EXT)
int getstat (u_short code, REGISTERS *regs)
#else
int getstat (code, regs)
u_short             code;
REGISTERS           *regs;
#endif
{
    Rbfdrive   		drvptr;     /* pointer to the drive table */
    struct rbf_opt 	*opt;   	/* path descriptor options ptr */
    u_int      		blkaddr;    /* logical block for format track */
    int        		error;      /* error holder */

    opt = PATHOPTIONS;				/* point at pd options */
    drvptr = DRIVETABLE;			/* pointer to the drive table */
    
    switch (code) {
    
		/* Direct Device command */
		case SS_DCmd:
			/* if you got here, you are on your own!! */
			error = direct_command( regs, opt, drvptr );
			break;

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
		 	/* validate sector size */
		 	if( (opt->pd_ssize < MINSSIZE) || (opt->pd_ssize > MAXSSIZE) )

#if defined (__STDC__) || defined (_ANSI_EXT)
		 		error = EOS_SECTSIZE;
#else
		 		error = E_SECTSIZE;
#endif
		 	else 
				error = 0;  /* return without error indicates supported */
			break;

        /* Unknown Service Requests here */
        default:

#if defined (__STDC__) || defined (_ANSI_EXT)
            error = EOS_UNKSVC;             /* return error */
#else
            error = E_UNKSVC;               /* return error */
#endif
            break;
    }
    return error;
}

/*
 * int putstat (code, regs) - perform setstat operations
 */

#if defined (__STDC__) || defined (_ANSI_EXT) 
int putstat (u_short code, REGISTERS *regs)
#else
int putstat (code, regs)
u_short             code;
register REGISTERS  *regs;
#endif
{
    Rbfdrive   		drvptr;     /* pointer to the drive table */
    struct rbf_opt 	*opt;   	/* path descriptor options ptr */
    u_int      		blkaddr;    /* logical block for format track */
    int        		error;      /* error holder */
    int        		drvn;		/* drive number */

    opt = PATHOPTIONS; 							/* point at pd options */
    drvptr = DRIVETABLE;			 			/* point at drive table */
    drvn = opt->pd_drv;							/* drive number */

    switch (code) {
		/* Direct Device command */
		case SS_DCmd:
			/* if you got here, you are on your own!! */
			error = direct_command( regs, opt, drvptr );
			break;

	    /* RESTORE disk head routine */
	    case SS_Reset:
        	if (error = ftinitdrv(drvptr, 0))       /* init drive */
                	break;                              /* abort if failed */

	        error = execnoxfer (SC_RESTORE, 0, 0, OPTS, CDB_STD, FALSE);
        	break;
    
	    /* WRITE-TRACK routine.  Used for disk formatting */
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
		 *  regs->d[4], interleave factor
		 */

        /* check if formatting allowed on device */
        if (opt->pd_cntl & CNTL_NOFMT) {

#if defined (__STDC__) || defined (_ANSI_EXT) 
            error = EOS_FORMAT;          /* format protected */
#else
            error = E_FORMAT;           /* format protected */
#endif
            
            break;
		} else {
			if ((regs->d[2] == (u_int32) opt->pd_toffs)) {
				/* first accessable track, record sides */
				if( !(regs->d[3] & 0x1) ) {
					/* if this is side 0 */
					fmt_first[drvn] = TRUE;    		/* record the first track */
					fmt_ddfmt[drvn] = regs->d[3];	/* record the bits */
				} else
					fmt_ddfmt[drvn] |= regs->d[3];	/* record the bits */
				return 0;					/* say it's ok */
				
			} else {
				/* have moved on beyond the first track */
				if( fmt_first[drvn] ) {
					/* we now have enough info to format the drive, do it */
					drvptr->v_0.dd_fmt = fmt_ddfmt[drvn];  /* set new dd_fmt */
					fmt_first[drvn] = FALSE;
					fmt_ddfmt[drvn] = 0;
				} else 	{			
					error = 0;		/* don't format more than once */
					break;
				}
			}

        	drvptr->v_zerord = FALSE;					/* mark dirty */
        	drvptr->v_init = FALSE;						/* not inited */
            if (error = ftinitdrv(drvptr, 0))			/* init drive */
                break;									/* abort if failed */

            error = execnoxfer (SC_FORMAT, 0, regs->d[4] & 0xff,
                                                         OPTS, CDB_STD, FALSE);
        }
        break;

	    /* Unknown Service Requests here */
	    default:

#if defined (__STDC__) || defined (_ANSI_EXT) 
        	error = EOS_UNKSVC;              /* return error */
#else
        	error = E_UNKSVC;               /* return error */
#endif        	
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

#if defined (__STDC__) || defined (_ANSI_EXT) 
int direct_command( REGISTERS *regs, struct rbf_opt *opt, Rbfdrive drvptr )			
#else
int direct_command( regs, opt, drvptr )			
register REGISTERS *regs;
struct rbf_opt *opt;
Rbfdrive drvptr;
#endif
{
	Cmdblk 			cmd;			/* high level command block */
	Dcmd 			dcmd;			/* direct command structure */
	u_char 			bit_bucket;		/* place to store results */
	int 			error;			/* error returns from functions */
	union scsipkt 	*scp;			/* scsi packet pointer */
	int 			permit;			/* permissions needed for command to work */
	int 			data_access;	/* permission result for command data */
	int error_access,errflg;		/* error information */
	
	data_access = error_access = errflg = 0;	/* init vars */
 	if (error = ftinitdrv(drvptr, 0))			/* init drive */
		return error;
		
	dcmd = regs->a[0];					/* direct command block */
	cmd = (Cmdblk)dcmd->dcmdblk;		/* command block */
	permit = (S_IREAD | S_IWRITE );		/* permissions nessesary */
	
	/* validate the call, as best as possible */
	if( (dcmd->manager != opt->pd_dtp) ||
	    (dcmd->device != SCSIdevice ) ||
	    (dcmd->dcmdsync != DCMDSYNC ) )

#if defined (__STDC__) || defined (_ANSI_EXT) 
		return EOS_PARAM;
#else
		return E_PARAM;		
#endif

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

#if defined (__STDC__) || defined (_ANSI_EXT) 
	 	return EOS_PERMIT;   
#else
	 	return E_PERMIT;   
#endif	 	

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

		err_det.ed_main = 0;	
		
		if( errflg ) {
			error = scsistat( error );			/* will do request sense etc. */
			_f_move( cmd->_cb_errptr, &err_det, cmd->_cb_errlen );
		}
	}
	return error;
}

