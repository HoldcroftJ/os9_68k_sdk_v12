/*
 * misc - miscellaneous functions for the rbteac high level driver
 */

/*
 * Copyright 1990, 1991, 1992, 1994, 1995, 1996 by Microware Systems Corporation
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
 *   02 95/08/01 Allow for sector numbering to begin at 0 if requested  GbG
 *   03 96/02/03 Teac support via tables added							GbG
 *               ---- OS-9/68K V3.0.3 Released ----
 *               ---- OS-9/68K V3.1 Released ----
 */

#include    "rbteac.h"                    /* gets all the defs */
#include	"rbproto.h"
#include	<teactab.h>	


/*
* ftinitdrv (drvtbl, recflg ) - initialize drive as needed 
 */

#if defined (__STDC__) || defined (_ANSI_EXT)
int ftinitdrv (Rbfdrive drvtbl, int recflg)
#else
int ftinitdrv (drvtbl, recflg)
Rbfdrive        drvtbl;         /* the drive table */
int 			recflg;			/* flag indicating depth from initdrv */
#endif
{
    struct rbf_opt			*opt;       /* path descriptor options ptr */
    Devicetbl               dev;		/* device table pointer */
    int            			error;      /* error holder */
    u_int          			blklen;     /* block length */
    int            			zero = 0;	/* a nuller */
    int						drvn;		/* drive number */
    int                     sink;       /* junk holder */
    int                     rate;		/* drive rotation rate */
	Teac_fmts				teac_table;	/* teac setup table */
    extern int              validate_descriptor(); 

    opt = PATHOPTIONS;				/* point at path descriptor options */
    drvn = opt->pd_drv;				/* drive number */

    /* check path descriptor for legal parameters */
    if ((drvn >= DRIVENUMBER)					/* logical drive # ok ?     */
        || (opt->pd_lun > MAXLUN))				/* pd_lun valid ?           */

#if defined (__STDC__) || defined (_ANSI_EXT)
        return EOS_UNIT;						/* bad unit number!!!!      */
#else
        return E_UNIT;							/* bad unit number!!!!      */
#endif

#ifndef CBOOT

	/* Check for descriptor change.
	 *   Uses the path descriptor pointer to the the device descriptor.
	 *   If a descriptor change is noted, the device must be re-initialized.
	 */
	dev = pathdesc->path.pd_dev;
	if( (long)dev->V_desc != last_descript[drvn] ) {
		last_descript[drvn] = (long)dev->V_desc;		/* set new descriptor */
		/* now build DD_FMT based on the descriptor */
		drvtbl->v_0.dd_fmt = (opt->pd_sid == 1) ? 0 : FMT_DS;	/* ss or ds */
		drvtbl->v_0.dd_fmt |= (opt->pd_dns << 1);				/* sd or dd */
		drvtbl->v_init = FALSE;
		drvtbl->v_zerord = FALSE;
	}

#endif /* CBOOT */

    /* send drive parameters to controller if drive not previously
     * initialized.
     */
    if (drvtbl->v_init == TRUE)
        return zero;                       /* drive is initialized */
    
	if( (rate = validate_descriptor(opt)) < 0 )

#if defined (__STDC__) || defined (_ANSI_EXT)
		return EOS_PARAM;
#else
		return E_PARAM;
#endif

	/* test for device ready, return if other than ID change */

    if( (error = execnoxfer( SC_TESTREADY, zero, zero, zero, 
                                                     CDB_STD, recflg)))

#if defined (__STDC__) || defined (_ANSI_EXT)
   		if( error != EOS_DIDC )
#else
   		if( error != E_DIDC )
#endif
       		return error;				/* bad news! */

#if defined (CBOOT)
	/* now try and determine the type of controller/drive. */
	if( !drive_checked ) {

		if( (error = execute( SC_INQUIRY, zero, sizeof(enqdet), zero,
					(u_char *)&inquir, INPUT, CDB_STD, recflg)) )
			return error;
	
		/* now parse the string and find out what type of drive
		 * and controller it is */
		if((drive_kind =
					get_drive_kind(&inquir.eq_prodid[0])) < 0 )

#if defined (__STDC__) || defined (_ANSI_EXT)
			return EOS_BTYP;			/* bad drive type */
#else
			return E_BTYP;				/* bad drive type */
#endif
		drive_checked = TRUE;
		
	}
	teac_table = (teac_fmts *)&teac_desc[drive_kind]; /* index to table */	
	
#else  /* NOT CBOOT */
	/* now try and determine the type of controller/drive. */
	if( !drive_checked[drvn] ) {

		if( (error = execute( SC_INQUIRY, zero, sizeof(enqdet), zero,
					(u_char *)&inquir, INPUT, CDB_STD, recflg)) )

			return error;
	
		/* now parse the string and find out what type of drive
		 * and controller it is */
		if((drive_kind[drvn] =
					get_drive_kind(&inquir.eq_prodid[0])) < 0 )

#if defined (__STDC__) || defined (_ANSI_EXT)
			return EOS_BTYP;			/* bad drive type */
#else
			return E_BTYP;				/* bad drive type */
#endif
		drive_checked[drvn] = TRUE;
	}

	teac_table = (teac_fmts *)&teac_desc[drive_kind[drvn]]; /* index to table */	

#endif /* CBOOT */

    /* set drive mode */
    mode_sel.ms_res1 = zero;					/* byte 0 = 0 */
    mode_sel.ms_res2 = zero;					/* byte 2 = 0 */
    mode_sel.ms_bdlen = zero;					/* blk descriptor length */
    mode_sel.ms_pgcode = 0x05;					/* page five -- drive params */
    mode_sel.ms_pglen = 0x1e;					/* length of this page */
    mode_sel.ms_trate[0] = (rate >> 8);			/* set data transfer rate */
    mode_sel.ms_trate[1] = rate;
    mode_sel.ms_heads = (drvtbl->v_0.dd_fmt & FMT_DS) ? 2 : 1;	/* # of sides */
    mode_sel.ms_sectrk = opt->pd_sct;					/* sectors per track */
    blklen = opt->pd_ssize;								/* get sector size */
    mode_sel.ms_secsize[0] = (blklen & 0xff00) >> 8;
    mode_sel.ms_secsize[1] = blklen & 0xff;
    mode_sel.ms_cyls[0] = (opt->pd_totcyls & 0xff00) >> 8;	/* # of cylinders */
    mode_sel.ms_cyls[1] = opt->pd_totcyls & 0xff;
    mode_sel.ms_precomp[0] = (opt->pd_wpc & 0xff00) >> 8;	/* precomp */
    mode_sel.ms_precomp[1] = opt->pd_wpc & 0xff;
    mode_sel.ms_redwrite[0] = (opt->pd_rwr & 0xff00) >> 8;
    mode_sel.ms_redwrite[1] = opt->pd_rwr & 0xff;
    mode_sel.ms_step[0] = zero;								/* step rate */
    mode_sel.ms_step[1] = opt->pd_stp * 10;
    mode_sel.ms_pulse = zero;							/* pulse width */
    mode_sel.ms_headdly[0] = zero;						/* head delay */
    mode_sel.ms_headdly[1] = 15;						/* 15 ms head delay */
    mode_sel.ms_motoron = 5;							/* motor on delay */
    mode_sel.ms_motoroff = 100;							/* motor off delay */

	/* allow for sector numbering to begin at 0 if requested */
		
	if( opt->pd_soffs == 0 )					/* sector offset */
		mode_sel.ms_tsm = 0x20;
	else
		mode_sel.ms_tsm = 0x60;

	mode_sel.ms_tsm |= teac_table->trdy;
	
    mode_sel.ms_precomplvl = zero;
    mode_sel.ms_loaddelay = zero;
    mode_sel.ms_unloaddelay = zero;

	if( (drvtbl->v_0.dd_fmt & (FMT_DT | FMT_QT | FMT_OT)) == 0 )
	    mode_sel.ms_pulcyl = 2;			/* double step for 40 track disks */
    else
        mode_sel.ms_pulcyl = 1;			/* single step for all others */
        


 	/*  set the pin2/4 definitions and media type according to the
 	 *  controller origin/firmware-revision and drive type */

	switch (rate) {
		case DR250:
			mode_sel.ms_pin34 = teac_table->pin34_250;
			mode_sel.ms_medtype = teac_table->medtype_250;
			 break;
		case DR500:
			mode_sel.ms_pin34 = teac_table->pin34_500;
			mode_sel.ms_medtype = teac_table->medtype_500;
			break;
		case DR1000:
			mode_sel.ms_pin34 = teac_table->pin34_1000;
			mode_sel.ms_medtype = teac_table->medtype_1000;
			break;
		default:
			return EOS_PARAM;			/* abort: unknown rate */
			break;
		}
	
    mode_sel.ms_pin4 = teac_table->pin4;

    mode_sel.ms_res5[0] = mode_sel.ms_res5[1] = mode_sel.ms_res5[2] =
                                                 mode_sel.ms_res5[3] = zero;
	/* send mode select command to controller */
	error = execute (SC_MODESELECT, zero, sizeof(teac_fc_modesel), OPTS, 
					(u_char *)&mode_sel, OUTPUT, CDB_STD, recflg );

    /* mark drive initialized if all ok */
    if (error == zero)
        drvtbl->v_init = TRUE;          /* flag drive initialized */

#ifndef CBOOT
	if( !error ) {				/* as long as everything is ok so far */
		/* Check existance and size of the sector zero buffer, if there
		 *   is one, and the sector size on the device has changed
		 *   return the old sector zero buffer.
		 */
		if( scztime[drvn].s0bsize != opt->pd_ssize )
			if( drvtbl->v_sczero )  {
				_f_srtmem( scztime[drvn].s0bsize, drvtbl->v_sczero );
				drvtbl->v_sczero = NULL;
			}

		/* allocate a buffer for sector zero if one does not already exist */
		if( drvtbl->v_sczero == NULL )
			error = _f_srqmem( opt->pd_ssize, &scztime[drvn].s0bsize,
			                   &drvtbl->v_sczero);
	}
#endif /* CBOOT */

    return error;
}

/*
 * execute () - execute a command involving data transfer.
 *
 */


#if defined (__STDC__) || defined (_ANSI_EXT)

int execute (u_int opcode, u_int blkaddr, u_int bytcnt, 
				u_int cmdopts, u_char *buff, u_int xferflags, 
				u_int cmdtype, int rcf)

#else
int execute (opcode, blkaddr, bytcnt, cmdopts, buff, xferflags, cmdtype, rcf)
u_int               opcode,         /* the command opcode */
                    blkaddr,        /* the disk block address */
                    bytcnt,         /* number of bytes to xfer */
                    cmdopts;        /* command options byte */
u_char              *buff;          /* the buffer pointer */
u_int               xferflags;      /* data transfer direction flag */
u_int               cmdtype;        /* cdb command type */
int					rcf;			/* depth of call from initdrv routine */
#endif
{
    int    error;          			/* keep track of errors */
    struct rbf_opt *opt;	   		/* path descriptor options ptr */
    int retry;						/* no retry at outset */
    int blkcnt;                     /* number of blocks */

    opt = PATHOPTIONS;				/* point at path descriptor options */
    
	if( opt->pd_ssize )
		blkcnt = bytcnt / opt->pd_ssize;	/* may be needed later */
	else
		blkcnt = 0;
		
    do {
		retry = FALSE;				/* init the retry flag */

        /*  set up SCSI command packet: command code, logical unit,
         *  block address, block count, and options
         */
        if (cmdtype == CDB_STD) {   /* setup 6-byte cdb */
            scsicmd.std.sp_opcode = opcode;         /* set cmd byte */
            scsicmd.std.sp_lun = opt->pd_lun << 5;   /* set target lun */

            scsicmd.std.sp_lun |= (blkaddr & 0x1f0000) >> 16;    /* lba */
            scsicmd.std.sp_lba[0] = (blkaddr & 0xff00) >> 8;
            scsicmd.std.sp_lba[1] = (blkaddr & 0x00ff);
            
            /* byte-count field manipulation:  if the count
             * is < sector size, assume byte-count to be
             * retained in cdb, else assume cdb count to
             * be made into a block count.
             */

            scsicmd.std.sp_blkcnt = (bytcnt < opt->pd_ssize)
                                    ? bytcnt : bytcnt / opt->pd_ssize;
            scsicmd.std.sp_options = cmdopts;               /* cmd options */
        }
        else {                      /* setup 10-byte cdb */
            scsicmd.ext.sp_opcode = opcode;         /* set cmd byte */
            scsicmd.ext.sp_lun = opt->pd_lun << 5;  /* set target lun */
            scsicmd.ext.sp_lba[0] = (blkaddr & 0xff000000) >> 24;
            scsicmd.ext.sp_lba[1] = (blkaddr & 0x00ff0000) >> 16;
            scsicmd.ext.sp_lba[2] = (blkaddr & 0x0000ff00) >> 8;
            scsicmd.ext.sp_lba[3] = (blkaddr & 0x000000ff);

            scsicmd.ext.sp_zero2 = 0;               /* null field */

            scsicmd.ext.sp_count[0] = ((bytcnt / opt->pd_ssize)
                                        & 0xff00) >> 8;
            scsicmd.ext.sp_count[1] = ((bytcnt / opt->pd_ssize)
                                        & 0x00ff);  /* xfer count */

            scsicmd.ext.sp_options = cmdopts;       /* cmd options */
        }
        
        /*  set up low-level driver command block: SCSI command packet
         *  address and size, logical unit, data buffer address and size,
         *  and SCSI controller id 
         */
        tcmdblk._cb_cmdptr = &scsicmd;               /* set cmd blk ptr */
        tcmdblk._cb_cmdlen = cmdtype;                /* set its size */

        tcmdblk._cb_pd_lun = opt->pd_lun;            /* set target lun */

        tcmdblk._cb_datptr = buff;                   /* set buffer ptr */
        tcmdblk._cb_datlen = bytcnt;                 /* set data count */

        tcmdblk._cb_scsi_id = opt->pd_ctrlrid;       /* set scsi id */

        tcmdblk._cb_xfer = xferflags;                /* set xfer direction */
        
        /* call the low-level module */
#ifdef CBOOT
		if( error = llexec( &tcmdblk, 0, DEFAULT ) )
#else        
        if ((error = (*data_mod->dm_exec) (procdesc,
                        data_mod, &tcmdblk, opt->pd_scsiopt, DEFAULT)))

#endif
            error = scsistat (error);
            
#if defined (__STDC__) || defined (_ANSI_EXT)           
      if( (error == EOS_DIDC) && !(rcf) ) {
#else
      if( (error == E_DIDC) && !(rcf) ) {
#endif
        	Rbfdrive drvtbl;
        	drvtbl = DRIVETABLE;

        	retry = TRUE;				/* will want to retry the command */
        	drvtbl->v_init = FALSE;		/* mark drive un-initialized */
	      	if( (error = ftinitdrv( drvtbl, ++rcf )) )
				break;
		}

#if defined (__STDC__) || defined (_ANSI_EXT)           				
    } while ((error == EOS_DEVBSY) || retry );
#else
    } while ((error == E_DEVBSY) || retry );
#endif

    return error;
}

/*
 * execnoxfer () - execute a command without data transfer
 *
 */

#if defined (__STDC__) || defined (_ANSI_EXT)           

int execnoxfer (int opcode, int blkaddr, int bytcnt, 
		int cmdopts, int cmdtype, int rcf)
#else
int execnoxfer (opcode, blkaddr, bytcnt, cmdopts, cmdtype, rcf)
int	opcode,     /* the opcode */
	blkaddr,    /* the block address */
	bytcnt,     /* block count for command */
	cmdopts,    /* command options byte */
	cmdtype,    /* cdb type */
	rcf;		/* flag:  non 0 indicates don't rerun initdrv */
#endif
{
    int    			error;          /* error stash */
    struct rbf_opt *opt;	   		/* path descriptor options ptr */
    int retry;						/* flag:  TRUE = retry the command */

    opt = PATHOPTIONS;				/* point at path descriptor options */

    do {
		retry = FALSE;				/* init the retry flag */

        /* set up SCSI command packet: command code, logical unit,
         * block address, block count, and options
         */
        
        if (cmdtype == CDB_STD) {   /* setup 6-byte cdb */
            scsicmd.std.sp_opcode = opcode;         /* set command byte */
            scsicmd.std.sp_lun = opt->pd_lun << 5;  /* set target lun */
            scsicmd.std.sp_lun |= (blkaddr & 0x1f0000) >> 16;    /* lba */
            scsicmd.std.sp_lba[0] = (blkaddr & 0xff00) >> 8;
            scsicmd.std.sp_lba[1] = (blkaddr & 0x00ff);
            scsicmd.std.sp_blkcnt = bytcnt;         /* set byte count */
            scsicmd.std.sp_options = cmdopts;       /* set cmd options */
        }
        else {                      /* setup 10-byte cdb */
            scsicmd.ext.sp_opcode = opcode;         /* set cmd byte */
            scsicmd.ext.sp_lun = opt->pd_lun << 5;  /* set target lun */

            scsicmd.ext.sp_lba[0] = (blkaddr & 0xff000000) >> 24;
            scsicmd.ext.sp_lba[1] = (blkaddr & 0x00ff0000) >> 16;
            scsicmd.ext.sp_lba[2] = (blkaddr & 0x0000ff00) >> 8;
            scsicmd.ext.sp_lba[3] = (blkaddr & 0x000000ff);

            scsicmd.ext.sp_zero2 = 0;               /* null field */
            scsicmd.ext.sp_count[0] = (bytcnt & 0xff00) >> 8;
            scsicmd.ext.sp_count[1] = (bytcnt & 0x00ff);    /* xfer count */
            scsicmd.ext.sp_options = cmdopts;       /* cmd options */
        }

        /* set up low-level driver command block: SCSI command packet
         * address and size, logical unit, and SCSI controller id
         */
        tcmdblk._cb_cmdptr = &scsicmd;               /* set cmd ptr */
        tcmdblk._cb_cmdlen = cmdtype;                /* set its size */
        tcmdblk._cb_pd_lun = opt->pd_lun;            /* set target lun */
        tcmdblk._cb_datptr = NULL;                   /* no data */
        tcmdblk._cb_datlen = 0;                      /* set byte count */
        tcmdblk._cb_scsi_id = opt->pd_ctrlrid;       /* set scsi id */
        tcmdblk._cb_xfer = INPUT;                    /* set xfer direction */

        /* call the low-level module */
#ifdef CBOOT
		if( error = llexec( &tcmdblk, 0, DEFAULT ) )
#else      

        if ((error = (*data_mod->dm_exec) (procdesc,
        				data_mod, &tcmdblk,  opt->pd_scsiopt, DEFAULT)))

#endif        				
	error = scsistat (error);

#if defined (__STDC__) || defined (_ANSI_EXT)           
		if( (error == EOS_DIDC) && !(rcf) ) {
#else
		if( (error == E_DIDC) && !(rcf) ) {
#endif

        	Rbfdrive drvtbl;
        	drvtbl = DRIVETABLE;

       		retry = TRUE;				/* will want to retry the command */
	       	drvtbl->v_init = FALSE;		/* mark drive un-initialized */
      		if( (error = ftinitdrv( drvtbl, ++rcf )) )
				break;
		}

#if defined (__STDC__) || defined (_ANSI_EXT)           
	} while( (error == EOS_DEVBSY) || retry );
#else
	} while( (error == E_DEVBSY) || retry );
#endif

    return error;
}

/*
 * int scsistat (error) - decode and interpret scsi status
 */
#if defined (__STDC__) || defined (_ANSI_EXT)           
int scsistat (int error)
#else
int scsistat (error)
int	error;					/* scsi status byte */
#endif
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

#if defined (__STDC__) || defined (_ANSI_EXT)           
		error = EOS_DEVBSY;					/* force retry */
#else
		error = E_DEVBSY;					/* force retry */
#endif

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

#if defined (__STDC__) || defined (_ANSI_EXT)           
int handlerr (void)
#else
int handlerr ()
#endif
{
    int    			error; 				/* error holder */
    struct rbf_opt 	*opt;   			/* path descriptor options ptr */
	Rbfdrive		drvtbl;				/* drive table ptr */
	int				sink;				/* trash holder */

    opt = PATHOPTIONS;			/* point at path descriptor options */

    /* setup "request sense" cdb */
    scsicmd.std.sp_opcode = SC_REQSENSE;        /* request sense */
    scsicmd.std.sp_lun = opt->pd_lun << 5;      /* set target lun */
    scsicmd.std.sp_lba[0] = 0;
    scsicmd.std.sp_lba[1] = 0;
    scsicmd.std.sp_blkcnt = sizeof (errdet);    /* error block size */
    scsicmd.std.sp_options = OPTS;          /* always 0 for wren III */

    /* pass cdb details to low-level module */
    tcmdblk._cb_cmdptr = &scsicmd;               /* set cmd ptr */
    tcmdblk._cb_cmdlen = CDB_STD;                /* set its size */
    tcmdblk._cb_pd_lun = opt->pd_lun;            /* set target lun */
    tcmdblk._cb_datptr = &err_det;               /* error details buffer */
    tcmdblk._cb_datlen = sizeof (errdet);        /* amount of data to transfer */
    tcmdblk._cb_xfer = INPUT;                    /* set data input flag */
    tcmdblk._cb_scsi_id = opt->pd_ctrlrid;       /* set scsi id */

    /*
     * get the command sense bytes
     */
#ifdef CBOOT
    if( (error = llexec( &tcmdblk, 0, DEFAULT )) == 0 ) {
#else    	
    if ((error = (*data_mod->dm_exec) (procdesc, data_mod, &tcmdblk,
					opt->pd_scsiopt, DEFAULT)) == 0) {
#endif
    	/* decode error */
		switch (err_det.ed_main & 0xf) {
		
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
			case SENSE_NOTRDY:		/* drive not ready */
			case SENSE_ID:			/* ID crc or ecc error */
			case SENSE_DATAERR:		/* unrecovered read error */
			case SENSE_NOADDRID:	/* no AM in ID field */
			case SENSE_NOADDR:		/* no AM in Data field */
			case SENSE_NOREC:		/* no record found */
			case SENSE_SEEK:		/* seek error */
			case SENSE_RETRYOK:		/* recovered data by retries */
			case SENSE_CONTR:		/* controller error */
				error = 0;			/* return no error */
				break;
			default:
				/* unknown case: return actual sense/error code */
				error = ( (err_det.ed_main & 0xf) << 8) | err_det.ed_code;
				break;
			}
			break;
		
		case MAIN_NOTRDY:		/* not ready */
			switch (err_det.ed_code) {
			case SENSE_NOTRDY:		/* drive not ready */
			case SENSE_NOINDEX:		/* no index pulse detected */

#if defined (__STDC__) || defined (_ANSI_EXT)           
				error = EOS_NOTRDY;	/* say not ready */
#else
				error = E_NOTRDY;	/* say not ready */
#endif

				break;
			default:
				/* unknown case: return actual sense/error code */
				error = ( (err_det.ed_main & 0xf) << 8) | err_det.ed_code;
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

#if defined (__STDC__) || defined (_ANSI_EXT)           
				error = EOS_SEEK;	/* return seek error */
#else
				error = E_SEEK;		/* return seek error */
#endif
				break;
			case SENSE_DATAERR:		/* unrecovered read error */

#if defined (__STDC__) || defined (_ANSI_EXT)           
				error = EOS_READ;
#else
				error = E_READ;
#endif

				break;
			case SENSE_MEDIUM:		/* medium format corrupted */
			case SENSE_INCOMPAT:	/* incompatible media */

#if defined (__STDC__) || defined (_ANSI_EXT)           
				error = EOS_BTYP;	/* say media bad type */
#else
				error = E_BTYP;		/* say media bad type */
#endif

				break;
			default:
				/* unknown case: return actual sense/error code */
				error = ( (err_det.ed_main & 0xf) << 8) | err_det.ed_code;
				break;
			}
			break;
		
		case MAIN_HARDWARE:		/* hardware error */
			switch (err_det.ed_code) {
			case SENSE_NOTRK0:		/* no track zero found */

#if defined (__STDC__) || defined (_ANSI_EXT)           
				error = EOS_SEEK;	/* say seek error */
#else
				error = E_SEEK;		/* say seek error */
#endif
				break;
			case SENSE_FDC:			/* FDC error */
			case SENSE_CONTR:		/* controller internal error */
			case SENSE_SCSIPAR:		/* scsi parity error */

#if defined (__STDC__) || defined (_ANSI_EXT)           
				error = EOS_HARDWARE; /* say hardware error */
#else
				error = E_HARDWARE;	/* say hardware error */
#endif

				break;
			case SENSE_SELFTEST:
			case SENSE_NOTERMPW:
			case SENSE_SWSTRAP:
/*  These could and probably should be E_HARDWARE, but for the time
 *     being we will return the sense/error type codes.  This code
 *     is non-functional, and will fall into the undefined error code
 *     return.
 */

#if defined (__STDC__) || defined (_ANSI_EXT)           
 				error = EOS_HARDWARE;			/* falls through, error changes */
#else
 				error = E_HARDWARE;			/* falls through, error changes */
#endif

/*				break;				/* uncover to revert to hardware error */
			default:
				/* unknown case: return actual sense/error code */
				error = ( (err_det.ed_main & 0xf) << 8) | err_det.ed_code;
				break;
			}
			break;
		
		case MAIN_ILGL:			/* illegal request */
			switch (err_det.ed_code) {
			case SENSE_ILLREQ:		/* invalid cmd code */

#if defined (__STDC__) || defined (_ANSI_EXT)           
				error = EOS_BTYP;	/* say bad type */
#else
				error = E_BTYP;		/* say bad type */
#endif
				break;
			case SENSE_ILLLBA:		/* illegal block address */

#if defined (__STDC__) || defined (_ANSI_EXT)           
				error = EOS_SECT;	/* say illegal sector address */
#else
				error = E_SECT;		/* say illegal sector address */
#endif

				break;
			case SENSE_ILLCDB:		/* illegal field in cdb */
			case SENSE_ILLFLD:		/* invalid field in parameter list */

#if defined (__STDC__) || defined (_ANSI_EXT)           
				error = EOS_PARAM;	/* say parameter error */
#else
				error = E_PARAM;	/* say parameter error */
#endif

				break;
			case SENSE_ILLLUN:		/* illegal lun */

#if defined (__STDC__) || defined (_ANSI_EXT)           
				error = EOS_UNIT;	/* say unit error */
#else
				error = E_UNIT;		/* say unit error */
#endif

				break;
			default:
				/* unknown case: return actual sense/error code */
				error = ( (err_det.ed_main & 0xf) << 8) | err_det.ed_code;
			}
			break;
		
		case MAIN_UNITATN:		/* unit attention */
			/* mark drive uninitialized */
			sink = opt->pd_drv;		/* get logical drive number */
			drvtbl = DRIVETABLE;	/* point at drive table */
			drvtbl->v_init = FALSE;	/* drive is uninitialized */

			switch (err_det.ed_code) {
			case SENSE_MODE:		/* mode select parameters changed */
			case SENSE_RESET:		/* power-on or reset or bus-reset */
			case SENSE_MEDIACHG:	/* media changed */

#if defined (__STDC__) || defined (_ANSI_EXT)           
				error = EOS_DIDC;	/* say device id changed */
#else
				error = E_DIDC;		/* say device id changed */
#endif

				break;
			default:
				/* unknown case: return actual sense/error code */
				error = ( (err_det.ed_main & 0xf) << 8) | err_det.ed_code;
				break;
			}
			break;
		
		case MAIN_WRITEPROT:

#if defined (__STDC__) || defined (_ANSI_EXT)           		
			error = EOS_WP;		/* write protected error */
#else
			error = E_WP;		/* write protected error */
#endif

			break;

		case MAIN_ABORT:		/* aborted command */
			case SENSE_SCSIPAR:		/* scsi parity error */

#if defined (__STDC__) || defined (_ANSI_EXT)           
				error = EOS_HARDWARE; /* say hardware error */
#else
				error = E_HARDWARE;	/* say hardware error */
#endif

				break;

			/* unknown case: return actual sense/error code */
			error = ( (err_det.ed_main & 0xf) << 8) | err_det.ed_code;
			break;
		
		case MAIN_DATACMP:		/* data miscompare */

#if defined (__STDC__) || defined (_ANSI_EXT)           		
			error = EOS_WRITE;		/* say write error */
#else
			error = E_WRITE;		/* say write error */
#endif

			break;
		
		default:				 /* unknown case:  return scsi details */
			error =( (err_det.ed_main & 0xf) << 8) | err_det.ed_code;
			break;
		}

#if defined (__STDC__) || defined (_ANSI_EXT)           		
	} else error = EOS_HARDWARE; /* can't get sense info */
#else
	} else error = E_HARDWARE;	/* can't get sense info */
#endif

    return error;
}

/* 
 *  copyblock( dest, src, size ) - do a bytewise copy of the sector
 */
#if defined (__STDC__) || defined (_ANSI_EXT)           		 
void copyblock( u_char *dest, u_char * src, int size )
#else
void copyblock( dest, src, size )
u_char 		*dest,*src;				/* source and destination */
int 		size;					/* size of sector in bytes */
#endif
{
	while( --size >= 0 )
		*dest++ = *src++;
}

#if defined (__STDC__) || defined (_ANSI_EXT)           		 
u_int fttrkoffs(u_int blkaddr, int ddfmt)
#else
u_int fttrkoffs(blkaddr, ddfmt)
u_int		blkaddr;				/* logical block address	*/
int 		ddfmt;					/* copy of dd_fmt */
#endif
{
	struct rbf_opt	*opt;			/* path descriptor options ptr */
	int 			sides;	
	
	opt = PATHOPTIONS;
	blkaddr += opt->pd_lsnoffs;			/* add lsn offset value	*/
	sides = (ddfmt & FMT_DS) ? 2 : 1;	/* number of sides */
	
	if ((opt->pd_toffs) && ((opt->pd_cntl & CNTL_AUTOSIZE) == 0)) {
		/* adjust lba according to track offsets	*/
		
		/* add sectors on track zero, side 0	*/
		blkaddr += opt->pd_t0s;
		/* add remaining sides of track zero	*/
		blkaddr += (opt->pd_sct * (sides - 1));
		/* add non-track zero tracks	*/
		blkaddr += ((opt->pd_toffs - 1) * sides * opt->pd_sct);
	}
	return blkaddr;
}

#ifndef CBOOT
/*
 * lxtoa(stg, num) - convert long to 8 ascii hex digits 
 */
#if defined (__STDC__) || defined (_ANSI_EXT)           		  
void lxtoa(char *stg, long num)
#else
void lxtoa(stg, num)
char   *stg;
long   num;
#endif
{
    int    i;
    
    for (i = 7; i >= 0; i--, num >>= 4)
        stg[i] = hexchar(num & 0xf);
    stg[8] = '\0';      /* terminate */
}

/*
 * int hexchar(n) - return ascii hex equivalent of n 
 */

#if defined (__STDC__) || defined (_ANSI_EXT)           		  
int hexchar(int n)
#else
int hexchar(n)
int    n;
#endif
{
    if (n > 9)
        n += 0x27;      /* for range a - f */
    return (n + 0x30);
}


/*
 * strcpy(s1, s2) - another version of string copy
 */
 
#if defined (__STDC__) || defined (_ANSI_EXT)           		  
void strcpy(char *s1, char *s2)
#else
void strcpy(s1, s2)
char   *s1, *s2;
#endif
{
    while (*s1++ = *s2++);
}

/*
 * strcpy(s1, s2) - another version of string copy
 */
 
#if defined (__STDC__) || defined (_ANSI_EXT)           		  
int strncmp(char *s1, char *s2, int size)
#else
int strncmp(s1, s2, sz)
char   *s1, *s2;
int size;
#endif
{
	while( --size >= 0 ){
		if (*s1++ != *s2++)
		return 1;
	}
	return 0;
}

/*
 * set3l(int3, num) - make num into 3 byte integer
 */
 
#if defined (__STDC__) || defined (_ANSI_EXT)           		  
int set3l(char *int3, int num)
#else
int set3l(int3, num)
char   *int3;
int    num;
#endif
{
    int    i;
    
    for (i = 2; i >= 0; i--, num >>= 8)
        int3[i] = num & 0xff;
}

/*
 * int c4tol(stg) - convert four characters to long
 */
 
#if defined (__STDC__) || defined (_ANSI_EXT)           		  
int c4tol(char *stg)
#else
int c4tol(stg)
char   *stg;
#endif
{
    int    rslt = 0, i;
    
    for (i = 0; i < 4; i++) 
        rslt = (rslt * 256) + stg[i];
    return rslt;
}

/* 
 *  int chkfdtime( opt, drvtbl ) - check removable media for time-out of
 *                                 buffered sector 0.
 */
#if defined (__STDC__) || defined (_ANSI_EXT)           		  
int chkfdtime( Rbfdrive drvtbl, int drvn )
#else
int chkfdtime( drvtbl, drvn )
Rbfdrive       drvtbl;			/* drive table pointer */
int            drvn;			/* drive number */
#endif
{
	if( scztime[drvn].accb.date ) {
		/* there will be a date if the system clock is running
		 *    if the date is wrong or to much time, then no longer good.
		 */
		if( (scztime[drvn].accb.date != sysstat(int, D_Julian)) ||
		    ((scztime[drvn].accb.time - sysstat(int, D_Second)) > HOLDTIME ) ) {
			drvtbl->v_zerord = FALSE;
			return FALSE;
		} else {
			/* buffer still within limits, return reflects current
			 *    condition of sector 0 buffer
			 */
			return( drvtbl->v_zerord );
		}
	}
	return FALSE;			/* no clock, cannot be buffered */
}

/*
 * int setfdtime() - set removable media access time.
 */
#if defined (__STDC__) || defined (_ANSI_EXT)           		  
int setfdtime(int drvn)
#else
int setfdtime(drvn)
int drvn;				/* drive number */
#endif
{
	if( sysstat( short, D_TckSec) ) {
		/* the clock is running, can set the access time */
		scztime[drvn].accb.date = sysstat( int, D_Julian );		/* date */
		scztime[drvn].accb.time = sysstat( int, D_Second );		/* time */
		return TRUE;
	} else
		return FALSE;			/* clock is not running */
}
#endif /* CBOOT */

/*
 *  validate that the path descriptor is indeed describing a good disk
 */
#if defined (__STDC__) || defined (_ANSI_EXT)           		  
int validate_descriptor(struct rbf_opt *opt)
#else
int validate_descriptor(opt)
struct rbf_opt *opt;
#endif
{
#ifndef CBOOT
	int type,rates;

	type = opt->pd_typ;			/* disk type */
	rates = opt->pd_rate;		/* rotation and data rates */
	
	/* validate and return results of data rate */
	switch( rates & DATA_TRANSFER_RATE ) {
		case RATE_250K:
			return DR250;
			break;
				
		case RATE_500K:
			return DR500;
				
		case RATE_1M:
			return DR1000;
				
		default:
			return -1;			/* not supported */
	}
#else
	u_int32 dsksz;
	int rates;

	/* get disk size */
	dsksz = opt->pd_sct*opt->pd_sid*opt->pd_cyl*opt->pd_ssize;	

	/* determine XRATE based on disk size */
		
	switch((dsksz&0xf00000)>>20){
			
		case 0: rates = DR250;			/* < 1MEG */
				break;
		case 1: rates = DR500;			/* 1 MEG */
				break;
		case 2: rates = DR1000;			/* 2 MEG */
				break;
		case 3: 
#if defined (__STDC__) || defined (_ANSI_EXT)           		  		
				rates = EOS_BTYP;		/* 4 MEG */
#else
				break;
				rates = E_BTYP;			/* 4 MEG */
#endif
#if defined (__STDC__) || defined (_ANSI_EXT)           		  
		default: rates = EOS_BTYP;		/* bad drive type */	
#else	
		default: rates = E_BTYP;		/* bad drive type */	
#endif
	}
		
	return rates;

#endif	/* CBOOT */
}

/*
 *  get_drive_kind -- parse inquiry string to find out if this is one
 *     of the supported controller/drive combinations.
 *  Note - the controller firmware origin/revision is a constant over
 *         all LUNS (drives) connected, thus only one flag is used.
 *       - for the old-style controller, only one drive type (GF/HF/JHF)
 *         is allowed per controller, but the new-style controller
 *         appears to allow mixing of types, thus the drive-type is
 *         maintained on an LUN basis.
 */
int get_drive_kind( char *cpt)
{
	char		*ds;			/* pointer to series field */
	char		*dt;			/* pointer to type field */
	char		*fw;			/* pointer to world field */
	int			kind = -1;		/* drive kind (index) */
	int			i;				/* index */
	Teac_fmts	teac_table;		/* teac setup table */

	ds = (char *)&cpt[0];		/* series field */
	dt = (char *)&cpt[9];		/* type field */
	fw = (char *)&cpt[15];		/* world field */
	
		for (i=0; i<MAX_TEAC_FMTS; i++){

			teac_table = (teac_fmts *)&teac_desc[i];	/* index to table */

			if (strncmp("END ", (char *)teac_table->dseries, 4) == 0)
				break;
			
			if ((strncmp(ds, (char *)teac_table->dseries, 4) == 0) &&
					(strncmp(dt, (char *)teac_table->dtype, 3) == 0) &&
					(*(char *)fw - '0' == teac_table->fc_firm) ) {
						kind = i;
		}
	}
	return kind;
}

