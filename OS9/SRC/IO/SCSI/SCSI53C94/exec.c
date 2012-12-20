/* exec - execute scsi command for the ncr53c94 low level module
 */

/*
 * Copyright 1992, 1994 by Microware Systems (UK) Limited
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	"scsi53c94.h"

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
	register int error;				/* local error number */
	register struct cntrlblk *cnt;	/* control block */
	int evid,						/* event id for the control block */
	    tarid,						/* target ID */
	    sink,						/* junk holder */
	    old_mask;					/* save of 68xxx sr */
	char name[16];					/* place to build name for event */

	/*
	 * integrity check:
	 * check that passed 'llcmd' is valid for this version
	 * low level module and that the scsiflags can be supported.
	 */
	if( (llcmd != DEFAULT) || ((scsiflags & ~SCSI_SUPPORT) != 0) )
		return E_PARAM+0x100;	/*!*!*/			/* signal parameter problem */
	
	/*
	 *  if this is the first command for this target, we will need to 
	 *    create an event for the command block.
	 */
	lst = dmod->dm_statics;			/* get static storage */
	tarid = cmd->_cb_scsi_id;		/* target id */
	
	/* ensure that the SCSI ID makes sense for this host adapter */
	
	if (tarid > MAXSCSI) return E_UNIT;   /* return bad unit */
	
	cnt = &lst->cntb[tarid];		/* command/control block for this target */
	cnt->scsi_flags = scsiflags;	/* flag register */

#ifdef USEDMA
	/* ensure that byte-count of dma is reasonable for dma device */
	if (cmd->_cb_datlen >= MAXDMA)
		return E_PARAM+0x200;	/*!*!*/		/* return parameter error */
#endif USEDMA
			
	if( cnt->event_id == 0 ) {
		/* first time for this target, must create an event for the block */
		name[0] = 'i';
		lxtoa(&(name[1]), ((u_int)rbfstat.v_sysio.v_port) & ~0xF );
		name[9] = '_';
		name[10] = '0' + tarid;						/* for us */
		name[11] = 0;
		if( (error = _fev_creat(0, -1, 1, name, &cnt->event_id, &sink)))
			return error;					/* could not create event !!! */
		evid = cnt->event_id;				/* id for the event */
	} else {
		/* this is not the first time in, must wait on the event */
		evid = cnt->event_id;				/* id for the event */
		do {
			if( (error = _fev_wait( evid, CBLOCKFREE,CBLOCKFREE, &sink )))
				return( error );		/* problems while waiting for event */
				
			if( cnt->cbk_status )
				sink = CBLOCKBUSY;			/* prevent overlap */
				
		} while( sink != CBLOCKFREE );		/* wait until it is free */			

	}

	/* block is ours, we mark it busy here */
	old_mask = mask_irq(lst->mask_level);

	if( (error = _fev_set( evid, CBLOCKBUSY, FALSE )) ) {
			mask_irq(old_mask);			/* so not masked out! */
			return error;				/* could not set event value */
	}

	cnt->cbk_status = CBK_BUSY;					/* mark block busy */
	mask_irq( old_mask );						/* can be free for a while */
	cnt->nxt_phase =  cnt->cmd_status = 0;		/* set up shadow registers */
	cnt->cmd_cmpcode = cnt->spcmd_phase = 0;
	qcopy(&cnt->cmdblock, cmd, sizeof(struct cmdblk));	/* copy cmd structure */
		
	/* convert llcmd to default scsi bus cmd */
	llcmd = ((cnt->scsi_flags & SCSI_ATN) == SCSI_ATN) ? SEL_ATN : SEL_NOATN;
#ifdef USEDMA
#ifdef SYNCHRO                                                  
	if( (!cnt->syc_tried) && (cnt->scsi_flags & SCSI_SYNCHR) ) {
			/* will send the first command with attention */
			cnt->ll_cmd = SEL_ATN_STOP;			/* force message out phase */
			cnt->mess_out[0] = 6;				/* six bytes to send */
			cnt->mess_out[1] = IDENTIFY_MESSAGE;
			cnt->mess_out[2] = EXTENDED_MESSAGE;
			cnt->mess_out[3] = 3;				/* number of bytes */
			cnt->mess_out[4] = SYNC_XFER_REQ;	/* request sync negotiation */
			cnt->mess_out[5] = 0x20;			/* number of clocks */
			cnt->mess_out[6] = 4;				/* max ack offset allowed */
			cnt->syc_tried = TRUE;				/* we at least tried */
	} else {
		/* normal command sequence (sync negotiations were attempted) */
		cnt->ll_cmd = llcmd;						/* chip command */
		cnt->mess_out[0] = 1;						/* message size 1 */
		cnt->mess_out[1] = IDENTIFY_MESSAGE;		/* non-spec. identify */
	}
#else
	cnt->ll_cmd = llcmd;						/* chip command */
	cnt->mess_out[0] = 1;						/* message size 1 */
	cnt->mess_out[1] = IDENTIFY_MESSAGE;		/* non-spec. identify */
#endif SYNCHRO
#else
	/* polled I/O case is the same as non-synchronous DMA case */
	cnt->ll_cmd = llcmd;						/* chip command */
	cnt->mess_out[0] = 1;						/* message size 1 */
	cnt->mess_out[1] = IDENTIFY_MESSAGE;		/* non-spec. identify */
#endif USEDMA
	/* set dynamic registers */
	cnt->curr_addr = cnt->cmdblock._cb_datptr;		/* data pointer */

	cnt->curr_cnt = cnt->cmdblock._cb_datlen;		/* data length  */
	if( (sink = kick( lst, cnt )))				/* error in xlation */
		return sink;		
	
	/* now wait for command to be completed */
	sink = 0;
	while( sink != CBLOCKWAKE )
		_fev_wait( evid, CBLOCKWAKE, CBLOCKWAKE, &sink );

#ifdef RECORD
	record(0xEC, lst);		/* End Command */
#endif RECORD

	sink = 0;										/* prevent old status */
#ifdef USEDMA
	/* check for dma errors:
	 *   dma errors will be considered higher priority 
	 *   than scsi errors.
	 */
	if( cnt->dmastatus & CSR_ERRS ) {
		sink = cnt->dmastatus & CSR_ERRS;		/* get dma status */
		cnt->cmd_cmpcode = ((sink & CSR_BES) || ( sink & CSR_BED ))
			? E_BUSERR : E_HARDWARE;
		sink = cnt->cmd_cmpcode;
	}
#endif USEDMA			
	if(!sink)
		sink = cnt->cmd_status;						/* status for return */

	old_mask = mask_irq(lst->mask_level);			/* no interruptions */
	cnt->cbk_status = 0;
	_fev_set( evid, CBLOCKFREE, FALSE );			/* set block free */
	mask_irq( old_mask );
#ifdef USEDMA
	if((cnt->cmdblock._cb_datlen) && (cnt->cmdblock._cb_xfer == INPUT))
		/* maintain cache coherency if writing to memory */
		flush_dcache();
#endif USEDMA
	return sink;
}
