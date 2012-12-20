 /*
 * irq - interrupt routine for ncr53c94 low level module
 */
 
/*
 * Copyright 1992 by Microware Systems (UK) Limited
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	"scsi53c94.h"

/* external routine declarations ( see irqsup.c ) */
extern u_char wd_wait(),getone();
extern void delay();
extern int group1(),group2(),group3(),group4(),
           kick(),message_out(),message_in();
extern void save_pointers(), restore_pointers();

/*
 * irq(lst, hw) - notify mainline that irq has occured
 */
 
int irq (lst, hw)
register Lstat lst;      /* low-level static storage */
register Hardware hw;    /* ncr53c94 hardware ptr */
{
	register u_char devstat;      /* ncr53c94 status */
	register CntrlBlk cnt;        /* current command/control block */
	register u_int zero = 0;      /* an interesting constant */
	int	dowake,                   /* wakeup process flag */
	    sink,                     /* junk holder */
	    i;                        /* used as a loop counter */
#ifdef USEDMA
	register u_char dmastat;      /* dma device status */
	register Dmahardware dmahw;   /* dma hardware ptr */
#endif USEDMA

 	/* check to see if irq is ours */
	devstat = HW(status);			/* read ncr53c94 status reg */

#ifdef USEDMA
	dmahw = lst->dmabase;					/* get dma hardware ptr */
	dmastat = dmahw->dh_csr;				/* get dma irq status */
#ifdef DMAIRQ
	/* check for "our" irq */
	if (((devstat & SR_IRQ) == 0) && ((dmastat & CSR_IRQ) == 0))
#else
	/* check for "our" irq */
	if ((devstat & SR_IRQ) == 0)
#endif DMAIRQ
		return 1;						/* return not us */
#else /* non-USEDMA */		

	if ((devstat & SR_IRQ) == 0)
		return 1;						/* return not us */
#endif USEDMA

	/* interrupt from ncr53c94 and/or dma device */
	do {	
		dowake = FALSE;							/* default no wakeup */
		cnt = lst->curr_cmd;					/* current command/control block */
		lst->statusreg = 0;						/* prevent old status */
		
		/* if irq from ncr53c94, get status & clear irq */
		if (devstat & SR_IRQ) {
			/* ncr53c94 had irq pending, clear it */
			lst->statusreg = devstat;			/* save statusreg */
			lst->seqstep = HW(seqstep);			/* save sequence step */
			devstat = HW(interrupt);			/* read out irq cause */
												/* clears IRQ and unlatches */
#ifdef RECORD
@irqtest:
			record(0xD1, lst);
#endif
		}

#ifdef USEDMA
		/* if irq from dma device, get status & clear irq */
		if (dmastat & CSR_IRQ){
			/* dma device had irq pending, clear it */
			dmastat = dmahw->dh_csr;			/* get dma status */
			cnt->dmastatus = dmastat;			/* save dma status */
			dmahw->dh_ccr &= ~CCR_STR;			/* ensure dma stopped */
			dmahw->dh_csr = dmastat;			/* reset the set bits */
		}
#endif USEDMA

	/* service ncr53c94 irq termination */
	if (lst->statusreg & SR_IRQ) {		/* ncr53c94 interrupt occurred */
		dowake = route_irq( hw, cnt, lst, devstat );
		lst->freshcmd = FALSE;				/* served it's purpose */
	}

#ifdef USEDMA
	/* service dma termination */
	if (dmastat & CSR_DONE) {
		/* check for errors */
		if (dmastat & CSR_ERRS) {	/* dma stopped due to error */
			/* stop ncr53c94 */
#ifdef RECORD
			record(RESET_CHIP, lst);
#endif RECORD
			HW(command) = RESET_CHIP;
			delay();
		}
	} else if (dowake) {
		/*
		 * no dma irq, BUT wakeup required.
		 * in this case assume that dma irq was "late" arriving
		 * and thus dma needs to be shutdown
		 */
	}
#endif USEDMA

	/* if nessesary wake up the current command owner */
	if (dowake) {
		_fev_set (cnt->event_id, CBLOCKWAKE, FALSE );
		lst->curr_cmd = &lst->cntb[SELFID];   /* place for messages */
		lst->busfree = TRUE;
	}

	/* if bus is free, start next command off if one ready */
	if( lst->busfree ) {
		i = 8;
		while( --i >= 0 ) {
			if( i == SELFID )
				continue;				/* my own ID is not active */

			cnt =  &lst->cntb[i];		/* set pointer to the control block */

			/* if a command is waiting, start it up */			
			if( ((sink = cnt->cbk_status) & CBK_VALID) == CBK_VALID )
				if( !(sink & CBK_SUBMIT ) ) {
					kick( lst, cnt );			/* start new command */
					break;
				}
		}
	}
#ifdef USEDMA
	dmastat = dmahw->dh_csr;	/* refresh local copy of DMA status */
#endif USEDMA
	devstat = HW(status);	/* new copy of 53c94 status */

#ifdef DMAIRQ
} while( (devstat & SR_IRQ) || (dmastat & CSR_IRQ) );
#else
} while( (devstat & SR_IRQ) );
#endif

	return 0;								/* record that irq serviced */
}

flush_dcache()
{
	return 0;
}

int route_irq( hw, cnt, lst, devstat )
register Hardware hw;      /* wd33c93 hardware ptr */
register CntrlBlk cnt;     /* low level static storage */
Lstat lst;  	
int devstat;
{
	register int status;

	cnt->spcmd_phase = (status = lst->statusreg) & SR_PHASE;

#ifdef RECORD
	record(devstat, lst);
	record(status, lst);
	record(HW(fifoflags), lst);
#endif RECORD

	if (status & (SR_GERR|SR_PERR) || devstat & IR_ILLCMD) {
		cnt->cmd_status = E_HARDWARE;
		/* mark that we are done */
		cnt->cbk_status &= ~(CBK_VALID | CBK_SUBMIT);
		cnt->cntreg_valid = FALSE;
		return TRUE;
	}
	cnt->sp_term = !!(status & SR_TCNT);

	if (devstat & IR_BUSSVC)
		return bus_service(hw, cnt, lst, devstat);
		
	else if (devstat & IR_DISCONN) {
		/* this is a disconnect interrupt may or may not terminate cmd */
		lst->busfree = TRUE;					/* mark bus not busy */
#ifdef RECORD
		record(ENABLE_SEL, lst);
#endif RECORD
		HW(command) = ENABLE_SEL;				/* re-enable reselect */
			
		if( cnt->cbk_status & CBK_RESET )
			return TRUE;						/* was a reset device */
			
		if (cnt->nxt_phase == TERM_DISCON ) {
			if( !(cnt->cbk_status & CBK_STATIN) ) { /* get the status */
				cnt->cmd_status = E_HARDWARE;
			}
			/* mark that we are done */
			cnt->cbk_status &= ~(CBK_VALID | CBK_SUBMIT);
			cnt->cntreg_valid = FALSE;
			return TRUE;
		} else if (lst->freshcmd) {			/* device not there */
			cnt->cmd_status = E_UNIT;		/* invalid unit number */
			/* mark that we are done */
			cnt->cbk_status &= ~(CBK_VALID | CBK_SUBMIT);
			cnt->cntreg_valid = FALSE;
			return TRUE;
		} else {
			save_pointers( hw,cnt,lst);		/* save data pointers */	
			cnt->cbk_status |= CBK_DISC;	/* set disconnected */
			cnt->nxt_phase = SUSPEND;		/* non-terminal disconnect */
			return FALSE;	/* he just let go for a while, he'll be back */
		}
	}
	else if (devstat & IR_RESELECT) {
		u_char busid, msg;
		lst->busfree = FALSE;					/* mark bus busy */

		if (!(devstat & IR_FUNCMPLT) || (HW(fifoflags) & FIFO_CNT) != 2) {
			cnt->cmd_status = E_HARDWARE;
			/* mark that we are done */
			cnt->cbk_status &= ~(CBK_VALID | CBK_SUBMIT);
			cnt->cntreg_valid = FALSE;
			return TRUE;
		}
		busid = HW(fifo);
		delay();
		msg = HW(fifo);
		delay();
#ifdef RECORD
		record(busid, lst);
#endif
		if ((busid & (1<<SELFID)) &&
				(busid &= ~(1<<SELFID)) &&
					(busid & (busid-1)) == 0) {
			register int i;
			for (i=0; busid >>= 1; i++)
				;
			busid = i;
		}
		else {
			cnt->cmd_status = E_HARDWARE;
			/* mark that we are done */
			cnt->cbk_status &= ~(CBK_VALID | CBK_SUBMIT);
			cnt->cntreg_valid = FALSE;
			return TRUE;
		}
		restore_pointers(hw, cnt, lst);

		/* source id valid, look for disconnected control block */
		cnt = (CntrlBlk)&lst->cntb[busid];
		if( cnt->cbk_status & CBK_DISC ) {
			/* we disconnected with this target, reactivate */
			cnt->cbk_status &= ~CBK_DISC;	/* mark selected */
			lst->curr_cmd = cnt;
		} else {
			/* real nasty problem, reconnection from a target that
			 *   is not active as far as we know! 
			 */
			cnt->cmd_cmpcode = BUS_FATAL;
			cnt->cmd_status = E_HARDWARE;		/* illegal!!!! */
		}
		return message_byte(msg, hw, cnt, lst);
	}
	else if (devstat & IR_FUNCMPLT) {
		if (cnt->spcmd_phase == PH_MSGIN) {
			u_char msg;
			if (cnt->nxt_phase != PH_MSGIN) {
				msg = HW(fifo);
#ifdef RECORD
				record(msg, lst);
				record(HW(fifoflags), lst);
#endif RECORD
				return message_byte(msg, hw, cnt, lst);
			} else {

				statcode(cnt, HW(fifo));
				msg = HW(fifo);
#ifdef RECORD
				record(0x55, lst);
				record(cnt->cmd_status, lst);
				record(msg, lst);
				record(0x5E, lst);
				record(HW(fifoflags), lst);
#endif RECORD
				return message_byte(msg, hw, cnt, lst);
			}
		}
		else {
			cnt->cmd_status = E_HARDWARE;
			/* mark that we are done */
			cnt->cbk_status &= ~(CBK_VALID | CBK_SUBMIT);
			cnt->cntreg_valid = FALSE;
			return TRUE;
		}
	}
	else {
		cnt->cmd_status = E_HARDWARE;
		/* mark that we are done */
		cnt->cbk_status &= ~(CBK_VALID | CBK_SUBMIT);
		cnt->cntreg_valid = FALSE;
		return TRUE;
	}
	return 0;
}

void delay()
{
	int i;
/*	for (i=0; i<10; i++);	*/
}

/*
 *  statcode() - decode the status
 */
statcode( cnt, status )
register CntrlBlk cnt; 
register u_char status;
{
	cnt->cmd_status = status & OMTI_KLUDGE;
	cnt->cbk_status |= CBK_STATIN;
	if( status == 0x8 ) {		/* device busy */
		cnt->cmd_cmpcode = BUS_RERUN;
	} else {					/* ok */
		cnt->cmd_cmpcode = BUS_DONE;
	}
}

void save_pointers(hw, cnt, lst)
register Hardware hw;      /* ncr53c94 hardware ptr */
register CntrlBlk cnt;     /* low-level static storage */
Lstat lst;
{
	register int temp,temp1;			/* used for calculations below */
#ifdef USEDMA
	register u_char *cpt;				/* pointer to data space */
	u_char dma_statr;					/* dma status register */
	u_int dma_dat;						/* dma data register */
	Dmahardware dmahw = lst->dmabase;	/* will need the DMA hardware */	
#endif

#ifdef RECORD
	record(0x56, lst);
#endif RECORD

	if( !cnt->cntreg_valid )
		return;									/* no DMA prepared */
		
#ifdef USEDMA
	/* turn off the DMA */
	dmahw->dh_ccr &= ~CCR_STR;
#endif	
	
	cnt->cntreg_valid = FALSE;					/* mark it saved */

	temp = HW(xfr_cnt_u) << 8;		/* msb */
	delay();
	temp |= HW(xfr_cnt_l);			/* lsb */
	delay();

	temp1 = HW(fifoflags) & FIFO_CNT;	/* bytes still in the fifo */

	if (temp1) {
		temp += temp1;
#ifdef RECORD
		record(FLUSH_FIFO, lst);
#endif
		HW(command) = FLUSH_FIFO;		/* throw them away */
		delay();
	}

	/* temp is now the number of bytes remaining to be transfered */
	if( temp < (temp1 = cnt->curr_cnt) ) {
		/* partial data transfer complete, adjust pointers */
		cnt->curr_cnt = temp;					/* less to be done */
		cnt->curr_addr += (temp1 - temp);		/* update data pointer */

#ifdef VME147
#ifdef USEDMA
		if( !(cnt->cmdblock._cb_xfer & OUTPUT) ) {
			/* DMA Read situation, must check for residual */
			if( dma_statr & 0x0f ) {
				/* there is residual data in the PCC, must 
				 *  then hand deliver it to the buffer.
				 */
				cpt = (u_char *)cnt->curr_addr;		/* pointer to data space */
				temp = 4;							/* 4 possible bytes */
				while( --temp >= 0 ) {
					if( dma_statr & 1 )
						*--cpt = dma_dat & 0xff;	/* put it in place */
					dma_statr >>= 1;				/* next byte indicator */
					dma_dat >>= 8;					/* next data byte */
				}
			}
		}
#endif USEDMA
#endif VME147		
	}
}

void restore_pointers(hw, cnt, lst)
register Hardware hw;      /* ncr53c94 hardware ptr */
register CntrlBlk cnt;     /* low-level static storage */
Lstat lst;
{
#ifdef RECORD
	record(0x57, lst);
#endif RECORD
}

int bus_service( hw, cnt, lst, devstat )
register Hardware hw;      /* wd33c93 hardware ptr */
register CntrlBlk cnt;     /* low level static storage */
Lstat lst;  	
int devstat;
{
	register int i;			/* temporary for xfer count */
	register u_char *ptr;	/* temporary data pointer */
	Cmdblk cmd = (Cmdblk)&cnt->cmdblock;	/* pointer to the SCSI command block */
#ifdef USEDMA
	register Dmahardware dmahw = lst->dmabase;	/* dma hardware ptr */
#endif USEDMA	

	i = cnt->curr_cnt;						/* size of data xfer */
	ptr = (u_char *)cnt->curr_addr;			/* get data pointer */

	if (cnt->spcmd_phase == PH_STATUS) {

#ifndef USEDMA
		if (!(cmd->_cb_xfer & OUTPUT) && !(devstat & IR_FUNCMPLT)) {
			if (i && ptr) {
				/* data transfer from target to CPU required */
				*ptr++ = HW(fifo); i--;
				delay();
				cnt->curr_cnt = i;				/* update size of data xfer */
				cnt->curr_addr = ptr;			/* update data pointer */
			}
		}
#endif
		/* data transfer complete - need status and message */

#ifdef RECORD
		record(FLUSH_FIFO, lst);
#endif RECORD
		HW(command) = FLUSH_FIFO;
		delay();

#ifdef RECORD
		record(I_COMPLETE, lst);
#endif RECORD
		HW(command) = I_COMPLETE;
		delay();
		cnt->nxt_phase = PH_MSGIN;
	}
	else if (cnt->spcmd_phase == PH_DATAOUT && (cmd->_cb_xfer & OUTPUT)) {
		/* data transfer from CPU to target required */

		if (i == 0 || ptr == NULL) {	/* Bad - shouldn't request data */
			cnt->cmd_status = E_HARDWARE;
			/* mark that we are done */
			cnt->cbk_status &= ~(CBK_VALID | CBK_SUBMIT);
			cnt->cntreg_valid = FALSE;
			return TRUE;
		}

#ifdef USEDMA

		if (devstat & IR_FUNCMPLT) {
#ifdef RECORD
			record(FLUSH_FIFO, lst);
#endif RECORD
			HW(command) = FLUSH_FIFO;
			delay();
		}
		/* write transfer count */
		HW(xfr_cnt_u) = (u_char) ((i & 0x00ff00l) >> 8);
		delay();
		HW(xfr_cnt_l) = (u_char) (i & 0x0000ffl);
		delay();

		/* if there is a DMA transfer to perform */
		cnt->cntreg_valid = TRUE;			/* flag count register vaild */
		dmahw->dh_ccr = DMA_OUT;
		dmahw->dh_sar = ptr;			/* set memory address */
		dmahw->dh_dar = (u_char *)0x10000000;		/* set device address */
		dmahw->dh_btc = i;				/* set transfer count */
		dmahw->dh_fcr = (SUPVDATA << b_FCR_SRC) |
						(SUPVDATA << b_FCR_DST);
		dmahw->dh_ccr = DMA_OUT + DMA_START;
#ifdef RECORD
		record(XFER_INFO+DMA, lst);
#endif RECORD
		HW(command) = XFER_INFO+DMA;

#else USEDMA

		if (devstat & IR_FUNCMPLT) {

#ifdef RECORD
			record(FLUSH_FIFO, lst);
#endif RECORD
			HW(command) = FLUSH_FIFO;
			delay();

			HW(fifo) = *ptr++; i--;
			delay();
#ifdef RECORD
			record(XFER_INFO, lst);
#endif RECORD
			HW(command) = XFER_INFO;
			delay();

			cnt->curr_cnt = i;					/* update size of data xfer */
			cnt->curr_addr = ptr;				/* update data pointer */
		}
		else {
			HW(fifo) = *ptr++; i--;
			delay();
#ifdef RECORD
			record(XFER_INFO, lst);
#endif RECORD
			HW(command) = XFER_INFO;
			delay();

			cnt->curr_cnt = i;					/* update size of data xfer */
			cnt->curr_addr = ptr;				/* update data pointer */
		}

#endif USEDMA

		cnt->nxt_phase = PH_STATUS;
	}
	else if (cnt->spcmd_phase == PH_DATAIN && !(cmd->_cb_xfer & OUTPUT)) {
		/* data transfer from target to CPU required */

		if (i == 0 || ptr == NULL) {
			cnt->cmd_status = E_HARDWARE;
			/* mark that we are done */
			cnt->cbk_status &= ~(CBK_VALID | CBK_SUBMIT);
			cnt->cntreg_valid = FALSE;
			return TRUE;
		}

#ifdef USEDMA

		if (devstat & IR_FUNCMPLT) {

#ifdef RECORD
			record(FLUSH_FIFO, lst);
#endif RECORD
			HW(command) = FLUSH_FIFO;
			delay();
		}
		/* write transfer count */
		HW(xfr_cnt_u) = (u_char) ((i & 0x00ff00l) >> 8);
		delay();
		HW(xfr_cnt_l) = (u_char) (i & 0x0000ffl);
		delay();

		/* if there is a DMA transfer to perform */
		cnt->cntreg_valid = TRUE;			/* flag count register vaild */
		dmahw->dh_ccr = DMA_IN;
		dmahw->dh_sar = (u_char *)0x10000000;		/* set device address */
		dmahw->dh_dar = ptr;			/* set memory address */
		dmahw->dh_btc = i;				/* set transfer count */
		dmahw->dh_fcr = (SUPVDATA << b_FCR_SRC) |
						(SUPVDATA << b_FCR_DST);
		dmahw->dh_ccr = DMA_IN + DMA_START;
#ifdef RECORD
		record(XFER_INFO+DMA, lst);
#endif RECORD
		HW(command) = XFER_INFO+DMA;
		delay();

#else USEDMA

		if (devstat & IR_FUNCMPLT) {

#ifdef RECORD
			record(FLUSH_FIFO, lst);
#endif RECORD
			HW(command) = FLUSH_FIFO;
			delay();

#ifdef RECORD
			record(XFER_INFO, lst);
#endif RECORD
			HW(command) = XFER_INFO;
			delay();
		}
		else {
			*ptr++ = HW(fifo); i--;
			delay();
#ifdef RECORD
			record(XFER_INFO, lst);
#endif RECORD
			HW(command) = XFER_INFO;
			delay();
			cnt->curr_cnt = i;					/* update size of data xfer */
			cnt->curr_addr = ptr;				/* update data pointer */
		}

#endif USEDMA

		cnt->nxt_phase = PH_STATUS;
	}
	else if (cnt->spcmd_phase == PH_MSGIN) {
		/* message transfer from target to CPU required */
		if (cnt->ext_msg == 0) {			/* start with fresh message */
			if (devstat & IR_FUNCMPLT) {
#ifdef RECORD
				record(FLUSH_FIFO, lst);
#endif RECORD
				HW(command) = FLUSH_FIFO;
				delay();
			}
			save_pointers(hw, cnt, lst);
#ifdef RECORD
			record(XFER_INFO, lst);
#endif RECORD
			HW(command) = XFER_INFO;
			delay();
		}
		else {
			return message_byte(HW(fifo), hw, cnt, lst);
		}
	}
	else {
		cnt->cmd_status = E_HARDWARE;
		/* mark that we are done */
		cnt->cbk_status &= ~(CBK_VALID | CBK_SUBMIT);
		cnt->cntreg_valid = FALSE;
		return TRUE;
	}
	return FALSE;
}

/*
 *	message_byte() - process a received message byte
 */
message_byte(ch, hw, cnt, lst)
register u_char ch;
register Hardware hw;      /* ncr53c94 hardware ptr */
register CntrlBlk cnt;     /* low-level static storage */
Lstat lst;
{
	register int retval = FALSE;		/* value returned from parse */
	int reject = FALSE;					/* set to reject message */
	u_int zero = 0;						/* a way to avoid clr instr. */
	
	if( !cnt->ext_msg ) {
#ifdef RECORD
		record(0xE5, lst);			/* start of message */
		record(ch, lst);			/* message byte */
#endif	RECORD		

		switch (ch) {
			case 0:				/* command complete */
				/* the entire message has been read, this command is done */
				cnt->nxt_phase = TERM_DISCON;
				break;
	
			case 0x1:			/* extended message in */
				cnt->ext_msg++;	/* byte 1 is the extended message */
#ifdef RECORD
				record(XFER_INFO, lst);
#endif RECORD
				HW(command) = XFER_INFO;
				delay();
				return FALSE;

			case 0x3:				/* restore pointers */
				restore_pointers(hw, cnt, lst);
				break;
			
			case 0x2:				/* save data pointers */
				save_pointers(hw, cnt, lst);/* save data count registers */
				cnt->nxt_phase = SUSPEND;	/* non-terminal disconnect */
				break;

			case 0x4:				/* target requests disconnect */
				cnt->cbk_status |= CBK_DISC;		/* set disconnected */
				cnt->nxt_phase = SUSPEND;		/* non-terminal disconnect */
				break;

			case 0x7:				/* message reject  */
				reject = TRUE;
				break;				/*  some other serious problems...  */
			
			case 0x9:			/* message parity error seen */
				cnt->nxt_phase = MESSAGE_OUT;		/* note to ourselfs */
				reject = TRUE;
				break;

			case 0x8:
			default:			/* identify type messages. */
/*				cnt->spcmd_phase = PH_IDMOK;	/* set identify received */
				break;
		}  /* end switch */
#ifdef RECORD
		record(0xEE, lst);		/* end of message */
#endif RECORD
		if (reject) {
#ifdef RECORD
			record(SET_ATN, lst);
#endif RECORD
			HW(command) = SET_ATN;				/* resend message */
			delay();
		}
#ifdef RECORD
		record(MSG_ACCEPT, lst);
#endif RECORD
		HW(command) = MSG_ACCEPT;
		delay();
		cnt->ext_msg = 0;			/* start with fresh message */
	} else {
		/* this is the handling of the extended message */
		cnt->ext_msg++;
		switch( cnt->ext_msg ) {
			case 2:			/* just the number of bytes to follow */
				break;
		
			case 3:					/* should be the sync xfer reqeust */
				if( ch != SYNC_XFER_REQ ) {
					/* assert attention to reject */
#ifdef RECORD
					record(0xEE, lst);		/* end of message */
					record(SET_ATN, lst);
#endif RECORD
					HW(command) = SET_ATN;
					delay();
#ifdef RECORD
					record(MSG_ACCEPT, lst);
#endif RECORD
					HW(command) = MSG_ACCEPT;
					delay();
					cnt->ext_msg = 0;			/* start with fresh message */
					cnt->nxt_phase = MESSAGE_OUT;
					return FALSE;
				}
				break;
				
			case 4:
				/* this byte is the bus cycle time / 4 for the
				 *  req/ack sequence.  Convert it to the value for
				 *  the sync register.
				 *     NOTE: refer to transfer period table 53C94 manual
				 */
				cnt->syc_per = (ch >> 4) & 0x1F;
				break;
				
			case 5:
				/* this byte is the offset that they can tolerate.
				 *   we have advertised 4, if they cannot do at least
				 *   4, they will have told us.  If the drive is not
				 *   sychronous, they will have responded with a 0 here.
				 *   In that case, the transfers are asynchronous.
				 */
				cnt->syc_off = ch;
				cnt->syc_tried = TRUE;			/* we at least tried */
#ifdef RECORD
				record(0xEE, lst);		/* end of message */
				record(MSG_ACCEPT, lst);
#endif RECORD
				HW(command) = MSG_ACCEPT;
				delay();
				cnt->ext_msg = 0;			/* start with fresh message */
				return FALSE;
								
			default:			/* too many bytes!!!! */
#ifdef RECORD
				record(0xEE, lst);		/* end of message */
				record(SET_ATN, lst);
#endif RECORD
				HW(command) = SET_ATN;			/* assert attention to reject */
				delay();
#ifdef RECORD
				record(MSG_ACCEPT, lst);
#endif RECORD
				HW(command) = MSG_ACCEPT;
				delay();
				cnt->ext_msg = 0;			/* start with fresh message */
				cnt->nxt_phase = MESSAGE_OUT;
				return FALSE;
		}  /* end switch */
#ifdef RECORD
		record(XFER_INFO, lst);
#endif RECORD
		HW(command) = XFER_INFO;	/* need more message bytes */
		delay();
	}  /* end else */
	/* return with results of next phase */
	return FALSE;
}
