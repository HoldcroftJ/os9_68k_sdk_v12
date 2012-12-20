/* _________________________ irqsup.c _________________________
 * 
 *  This module contains the following interrupt service support
 * functions:
 *
 * delay() performs  peripheral access delays
 * route_irq() routes an interrupt based on chip status
 * xfern() sets the chip up to transfer bytes
 * getone() waits for a character to become available at a data port
 *   and returns it to the caller
 * message_rejected() is called from the message_out() function
 *   when an unexpected message_in phase occures during its transfer 
 *   to the target
 * message_out() sends a message to the target
 * message_in() recieves a message from the target
 * group1() services group 1 interrupts
 * group2() services group 2 interrupts
 * group3() services group 4 interrupts
 * wd_wait() waits for a WD33C93 command to finish
 * dma_off() turns off the DMA controller
 * flush_dcache() from irq147.a/irq620.a is included below using the use
 *		assembly directive, flushes the data cache for the MVME147/MVME620
 */ 

/*
 * Copyright 1990, 1993, 1995 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	"scsi33c93.h"

/* the following functions are in srq.c */
extern void save_pointers (register Hardware hw, register CntrlBlk cnt, 
						   register Lstat lst);
extern int group4 (register Hardware hw, register CntrlBlk cnt,
                   int status, Lstat lst);
                   
/* the following functions are in this module and are used by other 
 * functions in this module 
 */
void delay (void);
int route_irq (register Hardware hw, register CntrlBlk cnt, Lstat lst, 
			   int devstat);
void xfern (register Hardware hw, int  count);
u_char getone (register Hardware hw);	
int message_rejected (register Hardware hw, register CntrlBlk cnt, 
					  Lstat lst);
int message_out	(register Hardware hw, register CntrlBlk cnt, Lstat lst);
int group1 (register Hardware hw, register CntrlBlk cnt, int status, 
			register Lstat lst);
int group2 (register Hardware hw, register CntrlBlk cnt, int status, 
			register Lstat lst);
int group3 (register Hardware hw, register CntrlBlk cnt, int status, 
			Lstat lst);


/*
 * peripheral access delayer function, required primarily for > 20MHz cpus.
 */
void delay (void)
{
	/* delay counter (made volatile so function won't be optimized away) */
	volatile int delay_var;
	
	for (delay_var = 1; delay_var < PCC_DELAY; delay_var++);
}

/*
 * function that routes an interrupt based on chip status.
 * Group 1 - 3 interrupts are in this file, for group4 interrupts see srq.c
 *
 *	Call with:
 *		hw, WD33C93 hardware pointer
 *		cnt, low level static storage
 *      lst, low level static storage            
 *		devstat, wd33c93 device status (contains its SCSI Status Register contents)
 *	Return:
 *		TRUE to wake up caller (i.e. indicates event completed)
 *		FALSE to keep caller asleep (i.e. indicates event not completed)
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int route_irq
(
	register Hardware hw,
	register CntrlBlk cnt,
	Lstat lst,  	
	int devstat
)
#else
int route_irq( hw, cnt, lst, devstat)
register Hardware hw;
register CntrlBlk cnt;
Lstat lst;
int devstat;
#endif
{
	register int retval;

#ifdef RECORD
	if( lst->statcount++  < STATMAX ) {
		*(lst->nxtstat++) = devstat;		/* record status */
	}
#endif
			
	switch (devstat & GPX_MASK) {
		case GROUP1: 
			/* successful command completion */
			retval = group1( hw, cnt, devstat, lst );
			break;

		case GROUP2:
			/* pause/aborted command interrupt */
			retval = group2( hw, cnt, devstat,lst );	/* process it */
			break;

		case GROUP3:
			/* command terminated with error */
			retval = group3( hw, cnt, devstat, lst);
			break;
	
		case GROUP4:
			/* bus device service requests (see srq.c) */
			retval = group4( hw, cnt, devstat, lst);
			break;
	
		/*  If we arrive here, there is a serious driver problem */
		default:
			retval= TRUE;					/* activate waiting process */
			break;
	} /* end switch */
	return retval;			/* return status of the phase */
}

/*
 * function to set the chip up to transfer bytes.
 *		Call with:
 *			hw, wd33c93 hardware pointer
 *			count, number of bytes to transfer
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void xfern
(
	register Hardware hw,	
	int  count				
)
#else
void xfern( hw, count)
register Hardware hw;
int  count;
#endif
{
	register char *cpt;
	
	cpt = (char *)&count;	/* pointer to the count */
	cpt++;					/* only  24 bits allowed */

	HW(address_reg) = COUNT_MSB;	/* set xfer count */
	HW(reg_file) = *cpt++;
	HW(reg_file) = *cpt++;
	HW(reg_file) = *cpt++;

	/* send the command to transfer from the target */
	HW(address_reg) = COMMAND;		/* tell chip to go */
	HW(reg_file) = XFR_INFO;
	delay();
}

/*
 *  getone - wait for a character to become available at the data port
 *           and return it to the caller 
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
u_char getone
(
	register Hardware hw
)
#else
u_char getone( hw )
register Hardware hw;
#endif
{
	while(!(HW(address_reg) & AUX_RDY) );		/* wait for data */
	delay();
	HW(address_reg) = DATA;
	return( HW(reg_file) );
}

/*
 *  message_rejected - This call is made from the message_out subroutine
 *                     in the case that an unexpected message_in phase
 *                     occures durring it's transfer to the target.
 *
 *		Call with:
 *			hw, wd33c93 hardware pointer
 *			cnt, low level static storage
 *          lst, low level static storage            
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int message_rejected
(
	register Hardware hw, 
	register CntrlBlk cnt,
	Lstat lst
)
#else
int message_rejected( hw, cnt, lst )
register Hardware hw;     
register CntrlBlk cnt;    
Lstat lst;	
#endif
{
	register int retreg, zero = 0;		
	
	/* the next message to send is just an identify, this message
	 *  will not fail.
	 */	
	cnt->mess_out[0] = 1;					/* just identify again */
	cnt->mess_out[1] = IDENTIFY_MESSAGE;

	/* read in the message rejected byte from the target */
	xfern( hw, 0 );
	retreg = getone( hw);
	delay();
	
	/* this interrupt will tell us that ack  is still asserted. (0x20)  */
	while( !(HW(address_reg) & AUX_IRQ) )
		delay();							/* back off chip */

	delay();								/* insure that status is latched */
	HW(address_reg) = STATUS;
	retreg = HW(reg_file);					/* get type of interrupt */
	delay();

	/* release ack to allow bus to continue */
	if( retreg == 0x20 ) {
		/* paused with ack asserted,  must negate ACK */
		HW(address_reg) = COMMAND;
		HW(reg_file) = NOACK;
		delay();
	} else
		return route_irq( hw, cnt, lst, retreg );

	/* this interrupt will return us to the message in state */
	while( !(HW(address_reg) & AUX_IRQ) )
		delay();
	
	delay();									/* prevent bad status */
	HW(address_reg) = STATUS;
	retreg = HW(reg_file);						/* clear out interrupt */

	return message_out(hw, cnt, lst );		/* send the identify message */
}

/* 
 *  message_out - send a message to the target
 *		Call with:
 *			hw, wd33c93 hardware pointer
 *			cnt, low level static storage area
 *          lst, low level static storage            
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int message_out
(
	register Hardware hw,
	register CntrlBlk cnt,
	Lstat lst
)
#else
int message_out( hw, cnt, lst )
register Hardware hw;     
register CntrlBlk cnt;    
Lstat lst;	
#endif
{
	register u_char *cpt;
	register u_char retreg;
	register u_int zero = 0;
	register int count;
	
	save_pointers(hw, cnt, lst);		/* save data count registers */

	/* set the transfer count register */
	count = (int)(cnt->mess_out[0] & 0xff);	/* count is stored in message */
	cpt = (u_char *)&cnt->mess_out[1];		/* pointer to the message */

	HW(address_reg) = CONTROL;
	HW(reg_file) = cnt->sp_xtrol & ~CR_DMA;	/* insure in byte mode, not DMA */
	xfern( hw, count );						/* set for transfer */

	/* this is the data transfer loop */
	HW(address_reg) = DATA;
	while( --count >= 0 ) {
		do {
			retreg = HW(address_reg);
			if( (retreg	& AUX_IRQ ) ) {
				delay();					/* in case bit just set! */
				HW(address_reg) = STATUS;
				retreg = HW(reg_file);		/* get interrupt cause */
				if( retreg == 0x4f )  		/* unexpected message in phase */
					return message_rejected( hw, cnt, lst );
				else
					return route_irq( hw, cnt, lst, retreg );
				break;
			}
		} while( !(retreg & AUX_RDY) );		/* wait for ready or interrupt */

		HW(reg_file) = *cpt++;			/* write one message byte */
	}
	cnt->nxt_phase = MESSAGE_IN;		/* flag for Group 1 handler */
	return 0;
}

/*
 *  message_in() - receive a message from the target 
 *		Call with:
 *			hw, wd33c93 hardware pointer
 *			cnt, low-level static storage
 *          lst, low level static storage            
 *		Return:
 *			TRUE to wake up caller (i.e. indicates event completed)
 *			FALSE to keep caller asleep (i.e. indicates event not completed)
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int message_in
(
	Hardware hw,
	CntrlBlk cnt,
	Lstat lst
)
#else
int message_in( hw, cnt, lst )
Hardware hw;   
CntrlBlk cnt;  
Lstat lst;
#endif
{
	u_char ch;
	int retval = FALSE;			/* value returned from parse */
	u_char devstat;				/* device status after each byte */
	int extended_mess = 0;		/* # bytes in incoming message */
	int regval;
	u_int zero = 0;						/* a way to avoid clr instr. */
	
	save_pointers(hw, cnt, lst);		/* save data count registers */

#ifdef RECORD
	if( lst->statcount++  < STATMAX ) {
		*(lst->nxtstat++) = 0xE5;		/* start of message */
	}
#endif

	do {
		xfern( hw, 1 );			/* tell it to transfer 1 byte */
		ch = getone( hw );		/* get a byte */
		delay();
		
#ifdef RECORD
	if( lst->statcount++  < STATMAX ) {
		*(lst->nxtstat++) = ch;	/* message byte */
	}
#endif

		while( ((retval = HW(address_reg)) & AUX_IRQ) == 0 );
		
		delay();					/* prevents false status latching */
		HW(address_reg) = STATUS;
		retval = HW(reg_file);			/* clears out paused with ack */
		delay();
		
		/* the status at this point is supposed to be paused with
		 *   ACK asserted.  If it is not, then there is the possibility
		 *   that the target released the bus before the ack!
		 */
		if( retval != 0x20 )
			return route_irq( hw, cnt, lst, retval );
		
		HW(address_reg) = COMMAND;		/* negate the ack! */
		HW(reg_file) = NOACK;
		delay();
		
		if( !extended_mess ) {
			switch (ch) {
				case 0:				/* command complete */
					/* the entire message has been read, this command is done */
					cnt->nxt_phase = TERM_DISCON;
					break;
	
				case 0x1:			/* extended message in */
					extended_mess++;	/* byte 1 is the extended message */
					break;				/*   indication */

				case 0x3:				/* restore pointers */
					break;
			
				case 0x2:				/* save data pointers */
					cnt->nxt_phase = SUSPEND;	/* non-terminal disconnect */
					break;

				case 0x4:				/* target requests disconnect */
					cnt->cbk_status |= CBK_DISC;		/* set disconnected */
					cnt->nxt_phase = SUSPEND;		/* non-terminal disconnect */
					break;

				case 0x7:				/* message reject  */
					HW(address_reg) = COMMAND;	
					HW(reg_file) = ABORT;
					delay();
					return TRUE;		/* we don't send bad messages, some */
					break;				/*  some other serious problems...  */
				
				case 0x9:			/* message parity error seen */
					cnt->nxt_phase = MESSAGE_OUT;		/* note to ourself */
					HW(address_reg) = COMMAND;
					HW(reg_file) = ATN;					/* resend message */
					delay();
					break;

				case 0x8:
				default:			/* identify type messages. */
					cnt->spcmd_phase = PH_IDMOK;	/* set identify received */
					break;
			}  /* end switch */
		} else {
			/* this is the handling of the extended message */
			extended_mess++;
			switch( extended_mess ) {
				case 2:			/* just the number of bytes to follow */
					break;
	
				case 3:					/* should be the sync xfer reqeust */
					if( ch != SYNC_XFER_REQ ) {
						/* assert attention to reject */
						HW(address_reg) = COMMAND;
						HW(reg_file) = ATN;
						cnt->nxt_phase = MESSAGE_OUT;
						delay();
						return FALSE;
					}
					break;
					
				case 4:
					/* this byte is the bus cycle time / 4 for the
					 *  req/ack sequence.  Convert it to the value for
					 *  of thes sync register.
					 *     NOTE: refer to transfer period table 33c93 manual
					 */
					if( ch < MINPERIOD )
						regval = ch = MINPERIOD;		/* set to our period */
					else 
						regval = (ch & 0x70) + 0x10;	/* round up */
						
					if( regval > 0x60 )
						regval = 0;			/* maximum clock period */
					break;
					
				case 5:
					/* this byte is the offset that they can tolerate.
					 *   we have advertised a value base on the chip that
					 *   we are using , if they cannot do at least that 
					 *   many, they will have told us.  If the drive is not
					 *   sychronous, they will have responded with a 0 here.
					 *   In that case, the transfers are asynchronous.
					 */
					if( ch == 0 )
						cnt->spsyc_register = 0;
					else
						cnt->spsyc_register =  ch | regval;

					cnt->syc_tried = TRUE;			/* we at least tried */
					break;
					
				default:			/* too many bytes!!!! */
					HW(address_reg) = COMMAND;
					HW(reg_file) = ATN;			/* assert attention to reject */
					cnt->nxt_phase = MESSAGE_OUT;
					delay();
					break;
			}  /* end switch */
		}  /* end else */

		while( !(HW(address_reg) & AUX_IRQ) );  /* wait for next phase */
		delay();						/* prevents false status latch */
		HW(address_reg) = STATUS;		/* status will determine the phase */
		devstat = lst->auxstatus = HW(reg_file);
		delay();	
		
		if( devstat == MESSAGE_IN )
			continue;
		else {
			/* return with results of next phase */
#ifdef RECORD
			if( lst->statcount++  < STATMAX ) {
				*(lst->nxtstat++) = 0xEE;	/* end of message */
			}
#endif
			return route_irq( hw, cnt, lst, devstat );
		}

	} while( TRUE );	/* until done */
	return FALSE;
}

/*
 * WD33C93 SUCCESSFUL COMPLETION INTERRUPTS decoding function 
 *		Call with:
 *			hw, the hardware registers
 *			cnt, current command block
 *			status, status word that got us here (between 0x10 and 0x1F)
 *			lst, low level static storage
 *		Return:
 *			TRUE to wake up caller (i.e. indicates event completed)
 *			FALSE to keep caller asleep (i.e. indicates event not completed)
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int group1
(
	register Hardware hw,
	register CntrlBlk cnt,
	int status,
	register Lstat lst
)
#else
int group1( hw, cnt, status, lst )
register Hardware hw; 
register CntrlBlk cnt;
int status;           
register Lstat lst;   
#endif
{
	/* for the most part, we will just need to send back the status */
	switch( status ) {
		case 0x10:
			/* Reselect command completed successfully */
			cnt->nxt_phase = MESSAGE_IN;
			break;

		case 0x16:
			/* a SELECT-and-TRANSFER command completed successfully (expect disconnect) */
			cnt->nxt_phase = TERM_DISCON;
			if( lst->apart )
				return group4( hw, cnt, TERM_DISCON, lst );
			break;
			
		case 0x11:
			/* a SELECT command has completed successfully (expect message out phase) */
			cnt->nxt_phase = MESSAGE_OUT;
			break;
			
		case 0x1A:
			/* some drives cause this interrupt after a message reject */
			cnt->nxt_phase = CMD_OUT;
			return group4( hw, cnt, CMD_OUT, lst );
			
		case 0x1B:
			/* at the end of the CDB transfer will get one of these */
			return group4( hw, cnt, STAT_IN, lst );
			break;
			
		case 0x1f:
			/* at the end of message_out phase, will get one of these
			 *  indicating that the XFR_INFO has completed with sucess
			 */
			if( cnt->nxt_phase == MESSAGE_IN )
				return group4( hw, cnt, MESSAGE_IN, lst );
			break;
		
		default:
			/* some form of transfer has ( not message in ) has completed */
			break;
	}
	return FALSE;		/* don't wake up caller */
}

/*
 * WD33C93 PAUSED/ABORTED INTERRUPTS decoding function 
 *
 *   In the case that a message in phase is in effect, an interrupt 
 *     informing the driver of the ACK line being held is generated.
 *     ( 0x20 ).  We handle this by negating ACK.  It is important to
 *     note that if we had an error or are rejecting the message in,
 *     we will have reasserted attention (see message_in) to signal
 *     the target that we will require him to resend the message.
 *
 *		Call with:
 *			hw, the hardware registers
 *			cnt, current command block
 *			status, status word that got us here (between 0x20 and 0x2F)
 *			lst, low level static storage
 *		Return:
 *			TRUE to wake up caller (i.e. indicates event completed)
 *			FALSE to keep caller asleep (i.e. indicates event not completed)
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int group2
(
	register Hardware hw,
	register CntrlBlk cnt,       
	int status,       
	register Lstat lst
)
#else
int group2( hw, cnt, status, lst )
register Hardware hw; 
register CntrlBlk cnt;
int status;           
register Lstat lst;   
#endif
{
	if( status == 0x20 ) {		/* message_in complete ACK is asserted */
		if( cnt->ll_cmd == SEL_ATN ) {
			HW(address_reg) = COMMAND;
			HW(reg_file) = NOACK;		/* negate ACK */
			return FALSE;
		}
	} else if( status == 0x21 ) {  		/* A Save Data Pointers message was */
		save_pointers(hw, cnt, lst);	/* received during a SELECT-and-TRANSFER */
		HW(address_reg) = COMMAND;		/* command (unexpected disconnect) */
		HW(reg_file) = NOACK;
		return FALSE;
	}

	/* covers all others in this group */
	cnt->cmd_cmpcode = BUS_FATAL;				/* mark that we are dead */
	cnt->cbk_status &= ~(CBK_VALID | CBK_SUBMIT);	/* mark block done */
	return TRUE;			/* always wakes him up, not much can be done */
}

/* 
 *  WD33C93 TERMINATED INTERRUPTS decoding function
 *   (termination due to error)
 *
 *    NOTE:   The target disconnected interrupt here will be
 *      run in the case of a select w/ATN and transfer command 
 *      that has been terminated by a target disconnect.  It is 
 *      perfectly ok that this happens.  This ISR is aware and
 *      will restart the command when the target reconnects.
 *
 *		Call with:
 *			hw, the hardware registers
 *			cnt, current command block
 *			status, status word that got us here (between 0x40 and 0x4F)
 *          lst, low level static storage
 *		Return:
 *			TRUE to wake up caller (i.e. indicates event completed)
 *			FALSE to keep caller asleep (i.e. indicates event not completed)
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int group3
(
	register Hardware hw,
	register CntrlBlk cnt,
	int status,
	Lstat lst
)
#else
int group3( hw, cnt, status, lst )
register Hardware hw; 
register CntrlBlk cnt;
int status;           
Lstat lst;
#endif
{
	int error;		/* temporary storage for error condition */
	register u_int zero = 0;

	switch( status ) {
		case 0x41:      /* target unexpectedly disconnected */
			lst->busfree = TRUE;					/* mark bus not busy */
			cnt->cntreg_valid = FALSE;
			HW(address_reg) = SRCID;
			HW(reg_file) = SI_ER;			/* enable reselect */
			if( (cnt->nxt_phase != TERM_DISCON) && 
			    !(cnt->cbk_status & CBK_STATIN) )	{
				cnt->cbk_status |= CBK_DISC;	/* mark disconnected */
				cnt->nxt_phase = SUSPEND;		/* mark suspended */
				return FALSE;
			}
			cnt->cbk_status &= ~(CBK_VALID | CBK_SUBMIT);
			return TRUE;
			break;
				
		case 0x42: 		/* timeout during select or reselect */
#if defined (__STDC__) || defined (_ANSI_EXT)
			error = EOS_UNIT;			/* device must not be on the bus */
#else
			error = E_UNIT;				/* device must not be on the bus */
#endif
			break;

		case 0x43:		/* command failed due to parity error (ATN is not asserted) */
		case 0x44:		/* command failed due to parity error (ATN is asserted) */
#if defined (__STDC__) || defined (_ANSI_EXT)
            error =  EOS_HARDWARE;      /* return "hardware error" */
#else
			error =  E_HARDWARE;		/* return "hardware error" */
#endif
			break;
			
		case 0x46:		/* wrong target reselected */
			if( lst->freshcmd ) {
				if( cnt ) {
					cnt->cbk_status |= CBK_DISC;	/* mark current as disc. */
					cnt->spcmd_phase = PH_OTARGCT;	/* target reselected */
					cnt->cntreg_valid = FALSE;		/* don't save pointers */
				}
			}
			return group4( hw, cnt, RESELECTED, lst);	/* handle reselect */
			break;

		case 0x48:		/* unexpected data out phase request */
			return( group4( hw, cnt, DATA_OUT, lst ) );
			break;

		case 0x49:		/* unexpected data in phase request */
			return( group4( hw, cnt, DATA_IN, lst ) );
			break;

		case 0x4a:		/* unexpectd cmd out phase request */
			return( group4( hw, cnt, CMD_OUT, lst ) );
			break;

		case 0x4b:		/* unexpected status out phase */
			return ( group4( hw, cnt, STAT_IN, lst ) );
			break;


		case 0x4c:		/*  transfer phase failed */
			HW(address_reg) = CMDPHASE;		/* point at cmd phase reg */
			HW(reg_file) = PH_IDSENT;		/* send something to it */
			error = BUS_RERUN;				/* can try again if desires */
			break;

		case 0x4e:			/* unexpected phase -- message_out request */
			HW(address_reg) = COMMAND;
			HW(reg_file) = ATN;						/* assert attention */
			message_out( hw, cnt, lst );
			break;
			
		case 0x4f:		/* unexpected message in phase requested */
			return( group4(hw, cnt, MESSAGE_IN, lst ) );
			break;

		default:		/* non-specific but fatal none the less */
			error = BUS_FATAL;
			break;
	}

	cnt->cmd_cmpcode = BUS_FATAL;				    /* mark that we are dead */
	cnt->cmd_status = error;
	cnt->cbk_status &= ~(CBK_VALID | CBK_SUBMIT);	/* mark block done */
	lst->busfree = TRUE;							/* mark us clear */
	return TRUE;			/* always wakes him up, not much to be done */
}

/*
 * u_char wd_wait (lst, hw) - wait for wd33c93 command to finish
 *		Call with:
 *			lst, low-level static storage
 *			hw, wd33c93 hardware pointer
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
u_char wd_wait
(
	register Lstat      lst,            
	register Hardware   hw            
)
#else
u_char wd_wait(lst, hw)
register Lstat		lst;
register Hardware 	hw;	
#endif
{
	/* poll aux status register for command complete */
	while ((HW(address_reg) & AUX_IRQ) == 0)
		delay();							/* delay for fast cpus */
	
	/* clear pending interrupt */
	HW(address_reg) = STATUS;				/* point at status register */
	return( HW(reg_file) );					/* return status */
}


#ifdef USEDMA

/*
 *  dma_off - turn off the DMA controller
 *		Call with:
 *			hw, wd33c93 hardware pointer
 *			lst, low-level static storage
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void dma_off
(
	Hardware	hw,
	Lstat 		lst
)
#else
void dma_off( hw, lst )
Hardware 	hw;			
Lstat		lst;		
#endif
{
#ifdef VME147
	register Dmahardware dmahw;			/* dma hardware ptr */
#endif
	
#ifdef VME620
		HW(dma_control) = DMA_DMAOFF;		/* disable dma controller */
#endif
#ifdef VME147
		dmahw = lst->dmabase;			/* dma h/w ptr (for later) */
		dmahw->dmactrl = 0;				/* disable DMA controller */
		dmahw->dmaicr &= ~DMA_IRQ_ENAB;			/* set new status */
#endif
}

#ifdef VME147
#ifdef _UCC
_asm(" use ./irq147.a");
#else
@ use ./irq147.a
#endif
#endif

#ifdef VME620
#ifdef _UCC
_asm(" use ./irq620.a");
#else
@ use ./irq620.a
#endif
#endif

#endif

