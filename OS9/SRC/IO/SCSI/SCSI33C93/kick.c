/* ______________________________ kick.c ______________________________
 *
 *  This module contains kick(), the function that starts up a SCSI command 
 */
  
/*
 * Copyright 1990, 1995 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	"scsi33c93.h"

/*
 * Function to start up a SCSI command.  This function is called from exec()
 * and irq().
 *
 *		Call with:
 *			lst, pointer to static storage area 
 *			cnt, pointer to command control block to use 
 *		Return:
 *			will always return all ok 
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int kick 
(
	Lstat lst,
	CntrlBlk cnt
)
#else
int kick ( lst, cnt )
Lstat lst;        
CntrlBlk cnt; 
#endif
{
	Hardware hw;		/* pointer to the hardware */
	u_char *ptr;		/* pointer to the SCSI command */

	/* variable used to clear registers (need to avoid MC68000 
	 * CLR instruction and use MOVE #0,reg instead) */
	volatile u_int zero = 0;

	Cmdblk cmd;         /* pointer to the SCSI command block */
	int loop;			/* loop counter */
	int temp,			/* used to build wd33c93 CONTROL register byte */
	    old_mask;		/* sr temporary storage */

#if defined (USEDMA) && defined (VME147)
	Dmahardware dmahw;	/* dma hardware ptr */
#endif

	hw = lst->hdware;              	/* hardware pointer */
	cmd = (Cmdblk)&cnt->cmdblock;   /* scsi command information */
	
	/* now set up some internals before masking and checking */
#ifdef USEDMA
#ifdef VME147
	dmahw = lst->dmabase;			/* dma h/w ptr (for later) */
#endif
	temp = CR_DMA | CR_MODE;		/* enable dma mode */
#else
	temp = CR_MODE;					/* set DMA control off */
#endif
	if( lst->apart )
		temp |= CR_EDI;				/* set special ending disconnect bit */

	/* if parity is enabled, halt on parity error */
    if ((cnt->scsi_flags & SCSI_PARITY) == SCSI_PARITY)
    	temp |= CR_HPE;

    ptr = cmd->_cb_cmdptr;				/* pointer to the SCSI command */

	/* set control mask register */
#ifdef USEDMA
 	/* set Intermediate Disconnect Interrupt (IDI-Control Register bit 2) */
	cnt->sp_xtrol = (temp |= ADV_MODE);	
#else
    cnt->sp_xtrol = (temp);	
#endif

	/* mask interrupts when writing wd33c93 registers */
	old_mask = mask_irq(lst->mask_level);

	if( lst->busfree ) {		/* try to kick off the command */
		do {
#ifdef RECORD
			/* record the ID of the target */
			if( lst->statcount++  < STATMAX )
				*(lst->nxtstat++) = cmd->_cb_scsi_id;
#endif
			lst->busfree = FALSE;				/* bus is now busy */
			if( lst->apart ) {
				/* load CDB size into OWNID for "A" version part */
				HW(address_reg) = OWNID;
				HW(reg_file) = cmd->_cb_cmdlen;
			} else	/* for the non-"A" part.... */
				HW(address_reg) = CONTROL;		/* start at control register */
			HW(reg_file) = temp;				/* control register */
			HW(reg_file) = MAXTIME;				/* set timeout period */

			/* Send the command block to the 33c93 chip and  
			 *  check for passed packet less than maximum packet size.
			 *  if so, then fill remaining registers with 0 and
			 *  increment register file ptr to TARGLUN register
			 */
			loop = cmd->_cb_cmdlen;
			while( --loop >= 0 )
				HW(reg_file) = *ptr++;			/* write passed cdb bytes */

			loop = (TARGLUN - CDB1) - cmd->_cb_cmdlen;
			while( --loop >= 0 )
				HW(reg_file) = zero;				/* fill unused cdb's */

			HW(reg_file) = (u_char)cmd->_cb_pd_lun; /* send the lun */
			HW(reg_file) = cnt->spcmd_phase;		/* command phase */
			HW(reg_file) = cnt->spsyc_register;		/* synchronous scsi bus */
			
			/* write transfer count */
			loop = cnt->curr_cnt;					/* size of data xfer */
			HW(reg_file) = (u_char) ((loop & 0xff0000l) >> 16);
			HW(reg_file) = (u_char) ((loop & 0x00ff00l) >> 8);
			HW(reg_file) = (u_char) (loop & 0x0000ffl);

			/* set the desitination ID, "A" part wants data phase direction
			 *    as well.
			 */
			temp = lst->apart ? ((cmd->_cb_xfer == OUTPUT) ? 0 : DI_DPD) : 0;
			HW(reg_file) = (u_char) cmd->_cb_scsi_id | temp;

			HW(reg_file) = SI_ER;					/* Source ID register */

			ptr = (u_char *)cnt->curr_addr;			/* get data pointer */
#ifdef USEDMA
			if( loop ) {
				/* if there is a DMA transfer to perform */
				cnt->cntreg_valid = TRUE;			/* flag count register vaild */
#ifdef VME147
				dmahw->dmaicr &= ~DMA_IRQ_ENAB;	/* enable DMA irqs */
				dmahw->tabladdr = NULL;			/* no table walking */
				dmahw->dataaddr = ptr;			/* set dma address */
				dmahw->link = 0;				/* clear link bit */
				dmahw->dfc = DMA_DFC;			/* set function codes */
				dmahw->datacount = loop;		/* set data count */
#endif
#ifdef VME620
				HW(dma_init) = DMAINIT;			/* reset the DMA chip */
				HW(dma_channel) = SCSI_CHAN;	/* use scsi channel on the chip */
				HW(dma_control) = DMA_DMAOFF;	/* disable dma chip */
	
				/* set transfer count */
				HW(dma_cntlsb) = (u_char)(loop & 0xff);
				HW(dma_cntmsb) = (u_char)((loop & 0xff00) >> 8);
	
				/* set transfer address */
				HW(dma_addrlsb) = ((unsigned) ptr & 0xff);
				HW(dma_addrnsb) = ((unsigned) ptr & 0xff00) >> 8;
				HW(dma_addrmsb) = ((unsigned) ptr & 0xff0000) >> 16;

				/* set dma transfer direction */
				HW(dma_mode) = (cmd->_cb_xfer & OUTPUT) ? DMA_WRITE : DMA_READ;	
				HW(dma_mask) = 0x07;				/* mask all channels */
				HW(dma_control2) = 0; 			/* clear secondary control register */
#endif
			}
#endif

#ifndef USEDMA
			/* prevent stale status for polled mode of operation */
			lst->auxstatus = 0;
#endif

#ifdef VME620
			HW(control) = VMEIRQ_DIS;		/* disable VME interrupts */
#endif
			/* start off the command */
			HW(address_reg) = (char)COMMAND;
			HW(reg_file) = cnt->ll_cmd;

#ifdef VME620
			HW(control) = VMEIRQ_EN;
#endif

#ifdef USEDMA
			if( loop ) {
#ifdef VME620			
				HW(dma_control) = DMA_DMAON;	/* re-enable dma chip */
#endif
#ifdef VME147
				/* start dma device if we are not in low-level chip operation */
				dmahw->dmactrl = (cmd->_cb_xfer & OUTPUT) ? DMA_WRITE : DMA_READ;
				dmahw->dmaicr |= DMA_IRQ_ENAB;	/* enable DMA irqs */
#endif
			}
#endif

 			/* mark this command as current and in progress */
			lst->freshcmd = TRUE;				/*    fresh command in */
			cnt->cbk_status |= CBK_SUBMIT;		/* command is submitted */
			lst->curr_cmd = cnt;
#ifdef RECORD
			if( lst->statcount++  < STATMAX )   /*    with respect to the...  */
				*(lst->nxtstat++) = 0xCE;       /*  .. disconnect interrupt   */
#endif

		} while(FALSE); 				/* this removes need for goto */

	}		/* end if bus free */

	cnt->cbk_status |= CBK_VALID;	/* command block is valid in any case */
	mask_irq(old_mask);				/* restore interrupt mask level  */ 

#ifndef USEDMA

	/* this is polled I/O. */
	HW(address_reg) = DATA;				/* point to data register */
	loop = cnt->curr_cnt;				/* data length to xfer */
	if( cmd->_cb_xfer & OUTPUT ) {

		/* HW(address_reg) & AUX_IRQ) == 0 is included below
		 * for the case in which kick() is called from irq().
		 * scsi620_nodma was seen to hang here if this condition
		 * was not put in.
		 */
		 
		while( ((lst->auxstatus & AUX_IRQ) == 0) && 
			   ((HW(address_reg) & AUX_IRQ) == 0) &&
				 loop ) 
		{
			if (HW(address_reg) & AUX_RDY)
			{
				HW(reg_file) = *ptr++;
				loop--;
			}
		}
	} 
	else 
	{
		while( ((lst->auxstatus & AUX_IRQ) == 0) && 
			   ((HW(address_reg) & AUX_IRQ) == 0) &&
				 loop ) 
		{
			if (HW(address_reg) & AUX_RDY)
			{
				*ptr++ = HW(reg_file);
				loop--;
			}
		}
	}
	
#endif

	return 0;				/* always returns all ok */
}
