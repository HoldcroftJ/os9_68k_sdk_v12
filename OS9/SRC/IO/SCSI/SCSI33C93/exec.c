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
| #   yy/mm/dd Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
| 62  01/01/31 Change v_port mask from bottom nibble to bottom byte    mgh  |
|                This edition history added                            mgh  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

/* ___________________________ exec.c ___________________________
 *
 *  This module contains exec(), the WD33C93 low level execute
 * SCSI command function
 */


#include	"scsi33c93.h"

extern int kick (Lstat lst, CntrlBlk cnt); /* in kick.c */
extern int mask_irq();		/* in miscirq.a */
extern void flush_dcache(); /* in irq147/620.a */
	
/*
 * The execute a SCSI command function.  This function is called from the
 * high-level device driver to execute a low-level SCSI command.  exec()
 * first validates that the command and all paramters are valid, and if so
 * creates an event for the operation (for the first time) or waits for a
 * current SCSI event to finish before proceeding.  exec() then calls kick() 
 * to implement the command.
 *
 *		Call with:
 *			proc, process descriptor pointer
 *			dmod, data module pointer
 *			cmd, scsi command block pointer 
 *			scsiflags, scsi option flags 
 *			llcmd, low-level scsi bus command 
 *		Return:
 *			error code			
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int	exec 
(
	procid		*proc,			
	Datmod		dmod,
	Cmdblk		cmd,
	int			scsiflags,
	int			llcmd	
)
#else
int exec (proc, dmod, cmd, scsiflags, llcmd)
procid      *proc;
Datmod      dmod; 
Cmdblk      cmd;  
int         scsiflags; 
int         llcmd;     
#endif
{
	Lstat lst;			/* low level statics */
	int error;			/* local error number */
	CntrlBlk cnt;		/* control block pointer */
	int evid,			/* event id for the control block */
	    tarid,			/* target ID */
	    sink,			/* junk holder */
	    old_mask;		/* save of 68xxx sr */
	char name[16];		/* place to build name for event */

	/*
	 * integrity check:
	 * check that passed 'llcmd' is valid for this version
	 * low level module and that the scsiflags can be supported.
	 */
	if( (llcmd != DEFAULT) || ((scsiflags & ~SCSI_SUPPORT) != 0) )
#if defined (__STDC__) || defined (_ANSI_EXT)
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
#if defined (__STDC__) || defined (_ANSI_EXT)
		return EOS_UNIT;	/* return bad unit */
#else
		return E_UNIT;		/* return bad unit */
#endif
	
	cnt = &lst->cntb[tarid];		/* command/control block for this target */
	cnt->scsi_flags = scsiflags;	/* flag register */

#ifdef USEDMA
	/* ensure that byte-count of dma is reasonable for dma device */
	if (cmd->_cb_datlen >= MAXDMA)
#if defined (__STDC__) || defined (_ANSI_EXT)
		return EOS_PARAM;			/* return parameter error */
#else
		return E_PARAM;			/* return parameter error */
#endif
#endif 
			
	if( cnt->event_id == 0 ) {
		/* first time for this target, must create an event for the block */
		name[0] = 'i';
		lxtoa(&(name[1]), ((u_int)rbfstat.v_sysio.v_port) & 0xFFFFFF00 );
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
				
		} while( sink != CBLOCKFREE );		/* wait until SCSI resource is free */			

	}

	/* block is ours, we mark it busy here */
	old_mask = mask_irq(lst->mask_level);

	/* indicate SCSI resource is taken */
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
	llcmd = ((cnt->scsi_flags & SCSI_ATN) == SCSI_ATN) ? SEL_ATN_XFR : SEL_NOATN_XFR;
#ifdef USEDMA
#ifdef SYNCHRO                                                  
	if( (!cnt->syc_tried) && (cnt->scsi_flags & SCSI_SYNCHR) ) {
			/* will send the first command with attention */
			cnt->ll_cmd = SEL_ATN;				/* force message out phase */
			cnt->mess_out[0] = 6;				/* six bytes to send */
			cnt->mess_out[1] = IDENTIFY_MESSAGE;
			cnt->mess_out[2] = EXTENDED_MESSAGE;
			cnt->mess_out[3] = 3;				/* number of bytes */
			cnt->mess_out[4] = SYNC_XFER_REQ;	/* request sync negotiation */
			cnt->mess_out[5] = MINPERIOD;		/* number of clocks */
			cnt->mess_out[6] = 
				lst->apart ? ACHIPFIFO : CHIPFIFO;	/* max ack offset allowed */
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
#endif /* SYNCHRO */
#else
	/* polled I/O case is the same as non-synchronous DMA case */
	cnt->ll_cmd = llcmd;						/* chip command */
	cnt->mess_out[0] = 1;						/* message size 1 */
	cnt->mess_out[1] = IDENTIFY_MESSAGE;		/* non-spec. identify */
#endif /* USEDMA */
	/* set dynamic registers */

#ifdef VME620
#ifdef USEDMA	
	/* perform address translation for external dma device if needed */
	if( cnt->cmdblock._cb_datlen ) {
		if ((int)( cnt->curr_addr = (char *)_f_trans(cnt->cmdblock._cb_datptr,
		                                     cnt->cmdblock._cb_datlen,
		                                     TRANS_LOCAL)             ) == -1) {
#if defined (__STDC__) || defined (_ANSI_EXT)
					return EOS_IBA;				/* signal "illegal block address */
#else
					return E_IBA;				/* signal "illegal block address */
#endif
		}
	} else {
		cnt->curr_addr = 0;
	}
#else
	/* no address translation as CPU will transfer the data by hand */
	cnt->curr_addr = cnt->cmdblock._cb_datptr;		/* data pointer */
#endif
#else
	/* no address translation as Onboard DMA has 1:1 memory mapping (mVme147)*/
	cnt->curr_addr = cnt->cmdblock._cb_datptr;		/* data pointer */
#endif

	cnt->curr_cnt = cnt->cmdblock._cb_datlen;		/* data length  */
	if( (sink = kick( lst, cnt )))					/* error in xlation */
		return sink;		
	
	/* now wait for command to be completed */
	sink = 0;
	while( sink != CBLOCKWAKE )
		_fev_wait( evid, CBLOCKWAKE, CBLOCKWAKE, &sink );

#ifdef RECORD
	old_mask = mask_irq(lst->mask_level);		/* record order of return */
	if( lst->statcount++  < STATMAX )       	/* with respect to the */
		*(lst->nxtstat++) = 0xec;              	/* disconnect interrupt */
	mask_irq( old_mask );
#endif

	sink = 0;										/* prevent old status */
#if defined (USEDMA) && defined (VME147)
	/* check for dma errors:
	 *   dma errors will be considered higher priority 
	 *   than scsi errors.
	 */
	if( cnt->dmastatus & DMA_ERRMASK ) {
		sink = cnt->dmastatus & DMA_ERRMASK;		/* get dma status */
		cnt->cmd_cmpcode = ((sink & DMA_ERR_BUS) || ( sink & DMA_ERR_TBLBE ))
#if defined (__STDC__) || defined (_ANSI_EXT)
			? EOS_BUSERR : EOS_HARDWARE;
#else
			? E_BUSERR : E_HARDWARE;
#endif
		sink = cnt->cmd_cmpcode;
	}
#endif
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
#endif
	return sink;
}

