/* _______________________________ srq.c _______________________________
 *
 *  This is the service requested interrupt handler module containing the
 * followinf functions:
 *
 * save_pointers() saves the devices registers and pointers for the
 *     current command.
 * restartcmd() prepares for a DMA transfer
 * statcode() decodes the status
 * group4() services group 8 interrupts
 */

/*
 * Copyright 1988, 1989, 1990, 1995 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	"scsi33c93.h"


extern void dma_off (Hardware  hw, Lstat lst); /* in irqsup.c */ 
        

/*
 *  Function to save the devices registers and pointers for the
 *  current command.  This is executed when the device requests
 *  a disconnect and has more to do.
 *
 *		Call with:
 *			hw, the hardware registers
 *			cnt, current command block
 *			lst, low level static storage
 *		Return:
 *			Nothing
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void save_pointers
(
	Hardware hw,
	CntrlBlk cnt,
	Lstat lst
)
#else
void save_pointers (hw, cnt, lst)
Hardware hw;
CntrlBlk cnt;
Lstat lst;
#endif
{ 
	int temp,temp1;			/* used for calculations below */

#if defined (VME147) && defined (USEDMA)
	u_char *cpt;				/* pointer to data space */
	u_char dma_statr;					/* dma status register */
	u_int dma_dat;						/* dma data register */
	Dmahardware dmahw = lst->dmabase;	/* will need the DMA hardware */	
#endif	

	if( !cnt->cntreg_valid )
		return;									/* no DMA prepared */
		
#if defined (VME147) && defined (USEDMA)
	/* we must preserve the condition of the PCC DMA registers.
	 *   If the buffer that was the source of the DMA was not long word
	 *   aligned then we may have residual data in the PCC if the DMA was a
	 *   read from SCSI.  If the direction was a write to the SCSI bus then
	 *   the pointers and data will be ok because the pointers are based on
	 *   the number of bytes transfered.   This must be done before the
	 *   the DMA turn off since the registers are cleared when DMA is 
	 *   disabled.
	 */
	 dma_statr = dmahw->dmastat;		/* status register (bytes valid) */
	 dma_dat = dmahw->datahold;			/* data still in PCC */
#endif

#ifdef USEDMA
	dma_off( hw, lst );					/* turn off the DMA */
#endif	
		
	HW(address_reg) = CMDPHASE;
	cnt->spcmd_phase = HW(reg_file);			/* preserve command phase */
	HW(address_reg) = CONTROL;
	HW(reg_file) = cnt->sp_xtrol & ~CR_DMA;		/* turn off chip DMA */
	
	cnt->cntreg_valid = FALSE;					/* mark it saved */

	temp = 0;						/* sweep for count collection */
	HW(address_reg) = COUNT_MSB;	/* read out the transfer count register */
	temp = HW(reg_file) << 16;		/* msb */
	temp |= HW(reg_file) << 8;		/* isb */
	temp |= HW(reg_file);			/* lsb */
		
	/* temp is now the number of bytes remaining to be transfered */
	if( temp < (temp1 = cnt->curr_cnt) ) {
		/* partial data transfer complete, adjust pointers */
		cnt->curr_cnt = temp;					/* less to be done */
		cnt->curr_addr += (temp1 - temp);		/* update data pointer */

#if defined (VME147) && defined (USEDMA)
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
#endif
	}
}



/*
 *   function that prepares for a DMA transfer
 *		Call with:
 *			hw, the hardware registers
 *			cnt, current command block
 *			lst, low level static storage
 *		Return:
 *			Nothing
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void restartcmd
(
	Hardware hw,
	CntrlBlk cnt,
	Lstat lst
)
#else
restartcmd (hw, cnt, lst)
Hardware hw;
CntrlBlk cnt;
Lstat lst;
#endif
{
 	u_char *ptr;		/* pointer to the SCSI command */
	int i;				/* loop counter */
	Cmdblk cmd;   		/* pointer to the SCSI command block */
	int zero = 0;		/* 32 bits of zero */
	int temp;					/* twiddle variable */
	
#if defined (USEDMA) && defined (VME147)
	Dmahardware dmahw = lst->dmabase;			/* dma hardware ptr */

	dma_off( hw, lst );
#endif

	i = cnt->curr_cnt;						/* number of bytes to transfer */
	ptr = (u_char *)cnt->curr_addr;			/* get data pointer */
	cmd = (Cmdblk)&cnt->cmdblock;           /* scsi command information */
	
	if( lst->apart ) {
		/* load CDB size into OWNID for "A" version part */
		HW(address_reg) = OWNID;
		HW(reg_file) = cmd->_cb_cmdlen;
	} else	/* for the non-"A" part.... */
		HW(address_reg) = CONTROL;				/* start at control register */
	HW(reg_file) = cnt->sp_xtrol;				/* set control register */
	HW(reg_file) = MAXTIME;						/* set timeout period */

	HW(address_reg) = TARGLUN;					/* set to target LUN register */
	HW(reg_file) = (u_char) cmd->_cb_pd_lun;	/* send the lun */
	HW(reg_file) = cnt->spcmd_phase;			/* set current phase */
	HW(reg_file) = cnt->spsyc_register;	 		/* synchronous register */
	
	/* write transfer count */
	HW(reg_file) = (u_char) ((i & 0xff0000l) >> 16);
	HW(reg_file) = (u_char) ((i & 0x00ff00l) >> 8);
	HW(reg_file) = (u_char) ( i & 0x0000ffl);

	/* set the desitination ID, "A" part wants data phase direction
	 *    as well.
	 */
	temp = lst->apart ? ((cmd->_cb_xfer == OUTPUT) ? 0 : DI_DPD) : 0;
	HW(reg_file) = (u_char) cmd->_cb_scsi_id | temp;	/* set dest scsi id */
	HW(reg_file) = SI_ER;						/* insure reselect set */

#ifdef USEDMA		
	if( i ) {
		cnt->cntreg_valid = TRUE;		/* flag count register valid */
		/* if any DMA to perform */
#ifdef VME147
		dmahw->tabladdr = NULL;			/* no table walking */
		dmahw->dataaddr = ptr;			/* set dma address */
		dmahw->link = zero;				/* clear link bit */
		dmahw->dfc = DMA_DFC;			/* set function codes */
		dmahw->datacount = i;			/* set data count */
#endif
#ifdef VME620
		HW(dma_init) = DMAINIT;			/* reset the DMA chip */
		HW(dma_channel) = SCSI_CHAN;	/* use scsi channel on the chip */
		HW(dma_control) = DMA_DMAOFF;	/* disable dma chip */
	
		/* set transfer count */
		HW(dma_cntlsb) = (u_char)(i & 0xff);
		HW(dma_cntmsb) = (u_char)((i & 0xff00) >> 8);

		/* set transfer address */
		HW(dma_addrlsb) = ((unsigned) ptr & 0xff);
		HW(dma_addrnsb) = ((unsigned) ptr & 0xff00) >> 8;
		HW(dma_addrmsb) = ((unsigned) ptr & 0xff0000) >> 16;

		/* set dma transfer direction */
		HW(dma_mode) = (cmd->_cb_xfer & OUTPUT) ? DMA_WRITE : DMA_READ;	
		HW(dma_mask) = 0x07;			/* mask all channels */
		HW(dma_control2) = zero; 			/* clear secondary control register */
#endif
	}
#endif

	HW(address_reg) = COMMAND;			/* restart the command */
 	HW(reg_file) = cnt->ll_cmd;

#ifdef USEDMA
	if( i ) {
#ifdef VME147
		/* only enable if DMA to be done */ 	
		dmahw->dmactrl = (cmd->_cb_xfer & OUTPUT) ? DMA_WRITE : DMA_READ;
		dmahw->dmaicr |= DMA_IRQ_ENAB;	/* enable DMA irqs */
#endif
#ifdef VME620
		HW(dma_control) = DMA_DMAON;	/* re-enable dma chip */
#endif
	}
#endif
}

/*
 *  function that decodes the status
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void statcode
( 
	CntrlBlk cnt,
	u_char status
)
#else
statcode( cnt, status )
CntrlBlk cnt; 
u_char status;
#endif
{
	cnt->cmd_status = status & OMTI_KLUDGE;
	cnt->cbk_status |= CBK_STATIN;
	if( status == 0x8 ) {		/* device busy */
		cnt->cmd_cmpcode = BUS_RERUN;
	} else {					/* ok */
		cnt->cmd_cmpcode = BUS_DONE;
	}
}
		
/* 
 * WD33C93 SERVICE REQUIRED INTERRUPTS decoding function 
 *		Call with:
 *			hw, the harware registers 
 *			cnt, current command block 
 *			status, status word that got us here (between 0x80 and 0x8F)
 *			lst, static storage area
 *		Return:
 *			TRUE to wake up caller (i.e. indicates event completed)
 *			FALSE to keep caller asleep (i.e. indicates event not completed)
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int group4
(
	Hardware hw, 
	CntrlBlk cnt,
	int status,           
	Lstat lst
)
#else
int group4( hw, cnt, status, lst )
Hardware hw;     
CntrlBlk cnt;    
int status;               
Lstat lst;
#endif
{
	Cmdblk cmd;			/* pointer to the SCSI command block */
	int error;			/* temporary storage for error condition */
	u_char statbyte;	/* incomming status ( for STAT_IN ) */
	u_int zero = 0;

	switch ( status ) {

		case RESELECTED:					/* reselected interrupt */
		case RESELECTEDADF:
			lst->busfree = FALSE;			/* mark bus busy */

			do {     						/* wait for valid source ID */
				HW(address_reg) = SRCID;
				error = (int)HW(reg_file);	/* get id of reselector */
			} while( !(error & SI_VALID) );

			/*  If a command has just been programmed into the 
			 *    33C93 concurrent with the reselection it is
			 *    possible that the reselection will be the only
			 *    visable interrupt.   The LCI bit is not always
			 *    present.
			 */
			if( lst->freshcmd ) {
				cnt = lst->curr_cmd;			/* get current command */
				lst->freshcmd = FALSE;			/* clear the flag */
				cnt->cbk_status &= ~CBK_SUBMIT; /* must be re-submitted */
			}
#ifdef RECORD
			if( lst->statcount++  < STATMAX ) 
					*(lst->nxtstat++) = error;	/* save cause of IRQ */
#endif
			/* source id valid, look for disconnected control block */
			cnt = (CntrlBlk)&lst->cntb[error & 0x7];
			if( cnt->cbk_status & CBK_DISC ) {
				/* we disconnected with this target, reactivate */
				cnt->cbk_status &= ~CBK_DISC;	/* mark selected */
				lst->curr_cmd = cnt;
				cmd = (Cmdblk)&cnt->cmdblock;  /* scsi command information */
				if( !lst->apart ) { 	/* non-"A" part operation */
					HW(address_reg) = CMDPHASE;
					cnt->spcmd_phase =  HW(reg_file) = PH_OTARGCT;
					HW(address_reg) = STATUS;				/* set to status reg */

					/*
					 *  Now hang out until the message phase request arrives,
					 *    when it does, we will restart the command.
					 */
					do {
						delay();
					} while( !(HW(address_reg) & AUX_IRQ) );	/* wait for message.. */

					delay();
					statbyte = HW(reg_file);   /* clear interrupt */
				} else { 	/* "A" part operation */
					HW(address_reg) = DATA;   /* go get the identify message */
					statbyte = HW(reg_file);
					cnt->spcmd_phase = PH_IDMOK;
				}					
				restartcmd(hw, cnt, lst);				/* resume */
				return FALSE;
			} else {
				/* real nasty problem, reconnection from a target that
				 *   is not active as far as we know! 
				 */
				cnt->cmd_cmpcode = BUS_FATAL;
#if defined (__STDC__) || defined (_ANSI_EXT)
				cnt->cmd_status = EOS_HARDWARE;		/* illegal!!!! */
#else
				cnt->cmd_status = E_HARDWARE;		/* illegal!!!! */
#endif
				return TRUE;		/* command not submitted yet */
			}
			break;

		case TERM_DISCON:
			/* this is a disconnect interrupt may or may not terminate cmd */
			lst->busfree = TRUE;					/* mark bus not busy */
			
			if( cnt->cbk_status & CBK_RESET )
				return TRUE;						/* was a reset device */
			HW(address_reg) = SRCID;				/* re-enable reselect */
			HW(reg_file) = SI_ER;
			
			if (cnt->nxt_phase == TERM_DISCON ) {
				if( !(cnt->cbk_status & CBK_STATIN) ) { /* get the status */
				 	HW(address_reg) = TARGLUN;
			 		cnt->cmd_status = HW(reg_file) & OMTI_KLUDGE;
			 		statcode( cnt, cnt->cmd_status );
				}

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
			break;
			
		case DATA_IN:			/* receive data from the target */
		case DATA_OUT:			/* send the data blocks to the target */
			cnt->spcmd_phase = (cnt->curr_cnt == 0) ?  PH_DXFRDN : PH_IDMOK;
			restartcmd( hw, cnt, lst );
			return FALSE;
			break;

		case CMD_OUT:			/* send the command block to the target */
			cnt->spcmd_phase = PH_CDBSTXF;		/* start of cmdblock xfer */
			cnt->ll_cmd = SEL_ATN_XFR;			/* set for correct command */
			restartcmd( hw, cnt, lst );			/* restart it */
			return FALSE;
			break;
			
		case STAT_IN:
#ifdef USEDMA
			dma_off( hw, lst ); 				/* turn off DMA chip */
#endif
			HW(address_reg) = CMDPHASE;
			HW(reg_file) = PH_DXFRDN;			
			xfern( hw, 0 );						/* single byte xfer */
			HW(address_reg) = DATA;
			while(!(HW(address_reg) & AUX_RDY) );	/* wait for ready */
			statcode(cnt, HW(reg_file) );
			HW(address_reg) = CMDPHASE;
			HW(reg_file) = PH_STRECV;			/* tell chip status received */
			cnt->cbk_status |= CBK_STATIN;		/* record it in the cmd block */
			cnt->nxt_phase = MESSAGE_IN;
			return FALSE;						/* target will disconnect */
			break;
			
		case MESSAGE_IN:
			/* message in phase */
			return message_in( hw, cnt, lst );
			break;
			
		case MESSAGE_OUT:
			/* message out phase */
			return message_out( hw, cnt, lst );
			break;
			
		default:
			HW(address_reg) = COMMAND;
			HW(reg_file) = ABORT;
			cnt->cmd_cmpcode = BUS_FATAL;		/* fatal, sorry! */
			break;
	}
	return TRUE;			/* always wakes him up, not much to be done */
}
