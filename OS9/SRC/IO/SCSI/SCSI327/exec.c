/*
 *  exec - execute scsi command for the mVme327 card low level driver
 */

/*
 * Copyright 1990, 1994 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	<scsi327.h>

#define SC_LOAD      0x1b		/* define the load command */
#define SC_TESTREADY 0x0		/* define the test unit ready command */

extern int initdrv();			/* drive setup */
extern void tsleep();			/* timed sleep */

/*
 *  This function returns a pointer to the next available command block
 *    for use.   There is no order dependence for these blocks since most
 *    of the commands complete asynchronously.  We have here a general
 *    case of a heap rather than a queue.
 */
Cpack next_scsip( lst )
Lstat lst;						/* low level driver static storage */
{
	register int loop;			/* loop counter */
	register Cpack next;		/* pointer to the next command block */
	register int irmask;		/* preserved value of sr */
	
	do {
		next = (Cpack) &lst->sc_cmst[0];	/* pointer to the heap */
		loop = MAX_SC;				/* the number of blocks available */
		
		irmask = mask_irq( lst->mask_level );  /* insure no interrupts */
		while( --loop >= 0 ) {
			if( next->sp.ssp_cmdlen )
				next++;				/* This block is busy */
			else {
				next->sp.ssp_cmdlen = TRUE;	/* hold for user */
				mask_irq( irmask );			/* restore interrupts */
				return next;				/* use this one... */
			}
		}
		
		mask_irq( irmask );			/* restore interrupts */
		_fc_tsleep( 1 );			/* give up for a time */

	} while( TRUE );			/* only return when one is available */
}

/*
 *  set_script - this command will obtain and transfer the bus
 *     phase script to the SCSI specific command block.
 *
 *  Warning:  This set_script(), exec(), and get_script() work in
 *            lockstep with eachother to select the appropriate
 *            script for the transfer.  We will send a synchronous
 *            negotiation message only on load or test unit ready
 *            and the synchronous bit is set in the scsi options flag.
 *  
 *  Change at edition 12:  Returns TRUE (from get_script() ) if synchronous
 *                         script has been loaded.
 */
int set_script( cmd, scp, flags )
int cmd;						/* command to perform */
register SCSIspec scp;			/* SCSI specific command area */
int flags;						/* scsi operations flags */
{
	register u_char *src,*dp;			/* source and destination poiters */
	register int loop;					/* size of the scripts */
	int retflag;						/* flag from get_script() */
	extern u_char *get_script();		/* returns pointer to the script */
	
	loop = SCRIPT_SIZE;							/* max size of script */
	src = get_script( cmd, flags, &retflag);	/* script pointer */
	dp = (u_char *) &scp->ssp_script[0];		/* put it here */
	while( --loop >= 0 ) {
		if( *src != SCRIPT_PD )
			*dp++ = *src++;					/* copy script to command block */
		else
			*dp++ = 0;						/* pad with 0's */
	}

	/* TRUE if message script has been loaded. */
/*	return( (cmd == SC_LOAD) || (cmd == SC_TESTREADY) ); /* chg ed.12 */
	return retflag;
}

/*
 *  set_sync - set up the synchronous negotiation messages
 */
void set_sync( scp )
register SCSIspec scp;
{
	register u_char *mo,*src;		/* message out buffer and message */
	register int loop;				/* number to transfer */
	extern u_char *get_syncmess();
	
	mo = (u_char *) &scp->ssp_mo[0];	/* pointer to message buffer */
	src = get_syncmess();			/* get the message pointer */

	scp->ssp_misiz = scp->ssp_mosiz = loop = *src++;	/* size of message */
	scp->ssp_moflg = scp->ssp_miflg = 0;	/* messages in SCSIspec */
	
	while( --loop >= 0 )
		*mo++ = *src++;				/* transfer the message */

}

/*
 *  classify - determine from script if this is a data in/out command
 */
int classify( scp )
register SCSIspec scp;		/* pointer to the command packet */
{
	register u_char phase;
	register u_char *script;
	
	script = (u_char *) &scp->ssp_script[0];
	while( (phase = *script++) != SCRIPT_TM ) {
		switch ( phase ) {
			case SCRIPT_DI:
			case SCRIPT_DO:
				scp->ssp_cntrl |= SCT_DMA;
		}
	}
	return 0;
}

/* getenv - get a free envelope */
Envelope getenv(lst)
register Lstat lst;
{
	register Envelope ept;
	register int old_mask;
	
	old_mask = mask_irq(lst->mask_level);	/* no interruptions allowed */
	
	if( !(ept = lst->freptr ) ) {
		mask_irq(old_mask);			/* free interrupts */
		return (Envelope)0;			/* all in use, sorry  */
	}

	lst->freptr = ept->link_ptr;			/* set for next */
	ept->link_ptr = 0;						/* set up envelope for caller */
	ept->env_valid = 0;
	SERIALIZE_M();
	mask_irq(old_mask);						/* free interrupts again */
	return ept;
}

/*
 *  enqueue - put a command on the stack for processing
 */
void enqueue( lst, cp )
register Lstat lst;
Spcmd cp;
{
	register Envelope ept;		/*  envelope pointer */
	register Hardware hw;		/*  hardware pointer */
	register int lreloc;
	u_int freeblock;
	
#ifdef DEBUGDRVR
	if( lst->ndbgpak < BUGGERS ) {
		register int dbcnt = sizeof(struct cpkt);
		register char *dbpt = (char *)&lst->buggers[lst->ndbgpak];
		register char *cmdp = (char *)cp;
		
		lst->ndbgpak++;
		while( --dbcnt >=0 )
			*dbpt++ = *cmdp++;
	}
#endif

	hw = lst->hdware;					/* pointer to the hardware */
	lreloc = lst->reloc;				/* set relocation constant */
	
	/* enqueing a command for the SCSI bus */
	do {
		ept =(Envelope)((int)lst->sc_chanel.cmd_tail - lreloc); /* current end of pipe */
	
		if( !(freeblock = (int)getenv(lst)) ) {
			_fc_tsleep( 1 );		/* sorry, all busy */
			cacheflush();
		} else {
			 /* set the pointer to the null envelope */
			ept->link_ptr = (Envelope)(freeblock + lreloc);
			/* pointer to command to be performed*/
			ept->cmd_stat = (Spcmd)((int)cp + lreloc);
			/* set new end of pipe */
			lst->sc_chanel.cmd_tail = ept->link_ptr;

			SERIALIZE_M();
			ept->env_valid = TRUE;						/* flag it valid */
			SERIALIZE_M();
			hw->xtrl_reg |= CSR_ATTEN;		/* tell board to look */
			SERIALIZE_M();
			return;
		}
	} while( TRUE );			/* until the command is sent */
}

/*
 * int exec (proc, dmod, cmdblk, scsiflags, llcmd) - execute a SCSI command
 */
int	exec (proc, dmod, cmd, scsiflags, llcmd)
procid		*proc;			/* process descriptor ptr */
Datmod		dmod;			/* data module ptr */
Cmdblk		cmd;			/* command block ptr */
int			scsiflags;		/* scsi option flags */
int			llcmd;			/* low-level scsi bus command */
{
	register Lstat lst;				/* low level statics */
	register int temp;				/* temporary storage */
	register Spcmd bc;				/* board level command pointer */
	register SCSIspec scp;			/* low level scsi command block */
	register u_char *sp,*dp;		/* source and destination pointers */
	Cpack cp;						/* board command packet */
	int evid,						/* event id for the control block */
	    tarid,						/* target ID */
	    sink,						/* junk holder */
	    old_mask,					/* save of 68xxx sr */
	    scpreloc,					/* SCSI packet address (translated) */
	    tried,						/* flag: synchronous negotiations */
	    negotiate = FALSE;			/* flag indicating attempt to go sync */

	/*
	 * integrity check:
	 * check that passed 'llcmd' is valid for this version low level module.
	 */
	if (llcmd != DEFAULT)
#ifdef __STDC__
		return EOS_PARAM;				/* signal parameter problem */
#else
		return E_PARAM;					/* signal parameter problem */
#endif
	
	/*
	 *  if this is the first command for this target, we will need to 
	 *    create an event for the command block.
	 */
	lst = dmod->dm_statics;			/* get static storage */
	tarid = cmd->_cb_scsi_id;		/* target id */

	/* ensure that the SCSI ID makes sense for this host adapter */
	
	if (tarid > MAXSCSI)
#ifdef __STDC__
		return(EOS_UNIT);			/* return bad unit */
#else
		return E_UNIT;				/* return bad unit */
#endif
		
	/* establish the target ID for this command */
    do {
	    if( !(evid = lst->sc_events[tarid]) )
	    	if( temp = initdrv( lst, tarid ) )			/* not initialized */
	    		return temp;							/* problem setting up */

	}  while( evid == 0 );
	
	/* wait for the event to become free */
	if( (temp = _fev_wait( evid, CBFREE,CBFREE, &sink )))
		return( temp );
		
	/* block is ours, we mark it busy here */
	old_mask = mask_irq(lst->mask_level);

	if( (temp = _fev_set( evid, CBBUSY, FALSE )) ) {
			mask_irq(old_mask);			/* so not masked out! */
			return temp;				/* could not set event value */
	}
	mask_irq(old_mask);				/* unmask again */

	cp = next_scsip( lst );			/* get the low level command block */
	scp = &cp->sp;					/* SCSI low level command part */
	bc = &cp->sc;					/* board command pointer */
	
	/* first set up board level command for SCSI special packet */
	scpreloc = (int)scp + lst->reloc;		/* relocate the SCSI cmd packet */
	bc->cp_cmd = LLCMDCODE;					/* command code */
	bc->cp_dtp = SCSIDEV;					/* device type */
	bc->cp_unt = (cmd->_cb_scsi_id << 4) | cmd->_cb_pd_lun;
	bc->cp_dbw =  DMA_16;		/* always operate in word moves */
	bc->cp_packt = (SCSIspec)scpreloc; 		/* scsi command pointer */
	bc->cp_sgl = 0;							/* scatter gather non-existant */
	bc->cp_sgct = 0;						/* ... and so then is the count */
	
	/* now set up the SCSI specific packet */
	scp->ssp_link = 0;							/* not a linked command */
	scp->ssp_ievent = lst->sc_events[tarid];	/* mark who's it is */
	
	/* transfer the scsi command block */
	temp = scp->ssp_cmdlen = cmd->_cb_cmdlen;		/* length of the command */
	sp = cmd->_cb_cmdptr;							/* src for xfer */
	dp = (u_char *) &scp->ssp_cdb[0];				/* dest for xfer */
	while( --temp >= 0 )
		*dp++ = *sp++;							/* transfer the command block */
		
	/* set data phase count and pointer to buffer */
	if( (scp->ssp_datlen = cmd->_cb_datlen)) {
		if((long)(scp->ssp_datptr = (char *)_f_trans( cmd->_cb_datptr,
		                                cmd->_cb_datlen, TRANS_LOCAL)) == -1 )
#ifdef __STDC__
			return EOS_IBA;		/* sorry, bad block address */
#else
			return E_IBA;		/* sorry, bad block address */
#endif
	} else
		scp->ssp_datptr = cmd->_cb_datptr;		/* no translation to perform */

	/* set address modifier for the data transfer address
	 *  Note:  If the data pointer is 0 (i.e. no data to transfer) then we
	 *         set the address modifier to be the address modifier needed
	 *         to access the command blocks which should be valid in any
	 *         case.
	 */
	if( scp->ssp_datptr != 0 )
		bc->cp_amf = ((long)scp->ssp_datptr & EXTMSK) ? VME_EXSUPDAT : VME_SUPDAT;
	else
		bc->cp_amf = ((long)(bc) & EXTMSK) ? VME_EXSUPDAT : VME_SUPDAT;

	/* set up message phase */
	scp->ssp_miflg = scp->ssp_moflg = 0;	/* message in & out in block */
	scp->ssp_mosiz = scp->ssp_misiz = 1;	/* size of message expected */

	/* build the message out phase */
	scp->ssp_mo[0] = (scsiflags & FLG_ATN ) ? DISCON : NODISCON;
	scp->ssp_mo[0] |= cmd->_cb_pd_lun;
	
	/* changed at edition 12:
	 *    flags passed to the script code that determine if the sysnchronus
	 *    negotiations will take place on this command or not. This 
	 *    code provides the lockout so that the synchronous negotiations
	 *    once only.
	 */
	tried = lst->sc_opts[tarid] & SCOPT_SYNCTRIED;
	if( set_script( *((u_char *)cmd->_cb_cmdptr),
	                 scp,
	                 ((scsiflags & SCSI_SYNCHR) && !tried )
	                                                 ? scsiflags : 0 )) {
		if( (scsiflags & SCSI_SYNCHR) && !tried ) {
			/* try to go synchronous */
			set_sync( scp );		/* overides standard message phase */
			scp->ssp_mo[0] |= cmd->_cb_pd_lun;  /* add lun */
			negotiate = TRUE;			
			lst->sc_opts[tarid] |= SCOPT_SYNCTRIED;
		}
	}
	
	/* now set up the control word */
	temp = SCT_SCHK;							/* we do sense if nessesary */
	temp |= (bc->cp_dbw == 2) ? SCT_LDAT : 0;		/* 32 bit xfer if possible */
	temp |= (scsiflags & SCSI_PARITY) ? SCT_PAR : 0 ;	/* turn on/off parity */
	temp |= (scsiflags & SCSI_ATN) ? 0 : SCT_NATN;		/* no attention! */

	/* change at ed 17, only set SYNC for negotiation - card remembers result */
	if (negotiate)
		temp |= SCT_SYNC;					/* set for synchronous */
		
	scp->ssp_cntrl = temp;					/* set the control word */
	classify( scp );						/* set remaining flags in control */

	enqueue( lst, (Spcmd) cp );						/* send command to board */
	
	sink = 0;
	while( sink != CBWAKE )
		_fev_wait( evid, CBWAKE, CBWAKE, &sink );	/* wait for complete */
		
	/* check for errors in the command and from the device */
	if( bc->cp_fec )
		temp = errstat( bc->cp_fec, cmd );
	else
		temp = scp->ssp_scsist;					/* return status of command */

	/*  change at ed.7,  moved position, force retry on error */
	if( negotiate ) {
		/* check to see if we became synchronous */
		if( temp ) {
			/* change at ed 17, only clear SYNCTRIED if not a dup negot. error */
			if ((bc->cp_fec != FEC_BADCMD) || (bc->cp_ast != AST_PHSMSM ))
				lst->sc_opts[tarid] &= ~SCOPT_SYNCTRIED;  /* error ! try again! */
		}
	}
	
	scp->ssp_scsist = bc->cp_fec = 0;			/* clear out return values */	
	old_mask = mask_irq( lst->mask_level );	/* mask interrupts */
	scp->ssp_cmdlen = 0;					/* signal block free */
	_fev_set( evid, CBFREE, FALSE );		/* set block free */
	mask_irq( old_mask );					/* restore interrupts */
	return temp;							/* all done */
}
