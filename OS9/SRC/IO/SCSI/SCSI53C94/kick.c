 /*
  * kick - subroutine that will start up an SCSI command 
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

int kick ( lst, cnt )
Lstat lst;						/* static storage */
register CntrlBlk cnt;			/* command control block to use */
{
	register Hardware hw;		/* pointer to the hardware */
	register u_char *ptr;		/* pointer to the SCSI command */
	u_int zero = 0;
	Cmdblk cmd;          		/* pointer to the SCSI command block */
	register int i;				/* loop counter */
	int temp,					/* used to build status register word */
	    old_mask;				/* sr temporary storage */
#ifdef USEDMA
	Dmahardware dmahw;			/* dma hardware ptr */
#endif USEDMA	

	hw = lst->hdware;                       /* hardware pointer */
	cmd = (Cmdblk)&cnt->cmdblock;           /* scsi command information */
	
	/* now set up some internals before masking and checking */

#ifdef USEDMA
	dmahw = lst->dmabase;				/* dma h/w ptr (for later) */
#endif
	temp = SELFID;

	/* if parity is enabled, halt on parity error */
    if ((cnt->scsi_flags & SCSI_PARITY) == SCSI_PARITY)
    	temp |= CR1_PARENABL;

    ptr = cmd->_cb_cmdptr;				/* pointer to the SCSI command */
    cnt->sp_xtrol = temp;	/* set control mask register */

	/* mask interrupts when writing ncr53c94 registers */
	old_mask = mask_irq(lst->mask_level);

	if( lst->busfree ) {		/* try to kick off the command */
		do {
#ifdef RECORD
			record(0xC5, lst);	/* command start */
			record(cmd->_cb_scsi_id, lst);	/* record the ID of the target */
#endif RECORD
			lst->busfree = FALSE;				/* bus is now busy */
			HW(config1) = temp;					/* control register */
			delay();
			HW(timeout) = MAXTIME;				/* set timeout period */
			delay();

			if (cnt->ll_cmd == SEL_ATN) {
				temp = IDENTIFY_DISCONN + cmd->_cb_pd_lun;
#ifdef RECORD
				record(temp, lst);		/* record message byte */
#endif RECORD
				HW(fifo) = temp;		/* send message byte */
				delay();
			}
			else if (cnt->ll_cmd == SEL_ATN_STOP) {
#ifdef RECORD
				record(cnt->mess_out[1], lst);	/* record message byte */
#endif RECORD
				HW(fifo) = cnt->mess_out[1];	/* send message byte */
				delay();
			}
			
			if (cnt->ll_cmd != SEL_ATN_STOP) {
				/* Send the command block to the 53c94 chip and  
				 *  check for passed packet less than maximum packet size.
				 *  if so, then fill remaining registers with 0 and
				 *  increment register file ptr to TARGLUN register
				 */
				i = cmd->_cb_cmdlen;
				while( --i >= 0 ) {
#ifdef RECORD
					record(*ptr, lst);
#endif RECORD
					HW(fifo) = *ptr++;			/* write passed cdb bytes */
					delay();
				}
			}
					
#ifdef SYNCHRO
			HW(syncoffset) = cnt->spsyc_register;	/* synchronous scsi bus */
			delay();
#endif SYNCHRO
			HW(destbusid) = (u_char) cmd->_cb_scsi_id;	/* set target ID */
			delay();

			/* start off the command */
#ifdef RECORD
			record(cnt->ll_cmd, lst);
#endif RECORD
			HW(command) = cnt->ll_cmd;
			delay();

 			/* mark this command as current and in progress */
			lst->freshcmd = TRUE;				/*    fresh command in */
			cnt->cbk_status |= CBK_SUBMIT;		/* command is submitted */
			cnt->nxt_phase = PH_COMMAND;		/* command phase */
			cnt->ext_msg = 0;			/* start with fresh message */
			lst->curr_cmd = cnt;
#ifdef RECORD
			record(0xCE, lst);			/* command end */
#endif RECORD

		} while(FALSE); 				/* this removes need for goto */

	}		/* end if bus free */

	cnt->cbk_status |= CBK_VALID;	/* command block is valid in any case */
	mask_irq(old_mask);				/* restore interrupt mask level  */ 

	return 0;				/* always returns all ok */
}
