/*
 * io_jnms.c - driver I/O extension routines for JNMS
 */
 
/*
 * Copyright 1988, 1989, 1990, 1991 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee solely
 * for documentation and educational purposes. Reproduction, publication
 * or distribution in any form to any party other than the licensee is
 * strictly prohibited.
 */

#define WHOCARES			1		/* dummy data - doesn't matter what */

/*
 *  transact( Hardware hw, Lstat lst, Datmod dmod, Cmdblk cmd )
 *		where: hw is a pointer to the 5380 chip
 *             lst is a pointer to the low level static storage
 *             dmod is a pointer to the scsi data module
 *             cmd is a pointer to the high level drivers command 
 *   Execute a command.
 */
int transact( hw, lst, dmod, cmd )
register Hardware hw;			/* 5380 chip */
register Lstat lst;				/* static storage for the low level driver */
register Datmod	dmod;			/* data module pointer */
Cmdblk cmd;						/* command block */
{
	register int bus_phase;
	error_code error;
	int sink,sink2;
	
	if( lst->v_SCSIReset )
		return E_DIDC;						/* abort if Reset seen */
		
	/* Arbitrate for the bus... */
	HW(SCSIWDAT) = (1 << lst->v_selfid);	/* set our own I.D. in data reg */

	for (sink2 = SELECT_DELAY; --sink2 > 0; ) {
		HW(MODEREG) = ARB_MD;				/* set arbitration bit on */
		/*
		 * The NCR5380 will wait for the "Bus Free" phase before beginning
		 * arbitration.  Therefore, if the bus is not free (i.e. another
		 * initiator is on it) this loop will time out.
		 */
		for (sink = 25; --sink != 0;) {
			/* check for arbitration in progress */
			if ( (HW(INITCMND) & AIP_BT) != 0)
				break;
			else {
				if ( HW(STATREG) & RST_BT )
					return E_DIDC;				/* reset seen! */
			}
		}
		
		/*
		 * if we timed out waiting for the 5380 to start arbitration,
		 * or we've lost arbitration, or a higher priority device is
		 * present, or we've lost arbitration, then start all over.
		 */
		if ((sink == 0)						    /* did we time out? */
			|| (HW(SCSIRDAT) > (1 << lst->v_selfid)) /* higher priority? */
			|| ((HW(INITCMND) & LA_BT) != 0)) {	 /* did we lose arbitration? */
			/*
			 * This is the failure case.  It clears the arbitration bit
			 * and then pops back up to the top of the loop to try again.
			 */
			HW(MODEREG) = 0;		/* make sure arbitration bit is off */
		} else break; /* otherwise, we won arbitration.  Get on with it. */
	}
	
	/* If we timed out bow out gracefully... */
	if (sink2 == 0) {
		HW(MODEREG) = 0;		/* clear the arbitration bit	 */
 		return E_HARDWARE;	/* free data module and return error */
	}

	/* set SEL and ensure ACK is negated */
	lst->v_initcmd = A_SEL;
	HW(INITCMND) = lst->v_initcmd;
	short_delay();		/* waste time (bus clear & bus settle delays) ed. 40 */
	
	/*
	 * Select the target by setting the bit corresponding to the SCSI ID
	 * number of the target ORed along with our own SCSI ID bit
	 */
	HW(SCSIWDAT) = ((1 << cmd->_cb_scsi_id) | (1 << lst->v_selfid));

	/* enable the data bus and assert BUSY */
	lst->v_initcmd |= A_DBUS | A_BSY;
	HW(INITCMND) = lst->v_initcmd;
	HW(MODEREG) = 0;	/* clear the arbitration bit */
	HW(SELENABL) = 0;	/* clear the select enable register */

	/* Now, clear the busy bit... */
	lst->v_initcmd &= ~A_BSY;
	HW(INITCMND) = lst->v_initcmd;

	/*
	 * loop until the controller is ready or timeout occurs.
	 * NOTE: The SCSI spec says to allow up to 250
	 *       MILLIseconds for the target to assert the BUSY line.
	 */
	for (sink = SELECT_DELAY; --sink != 0;) {
		if ( (HW(STATREG) & BUSY_BT) != 0)
			break;
		if ( (HW(STATREG) & RST_BT) || (lst->v_SCSIReset) )
				return E_DIDC;				/* reset seen! */
	}

	/* release SEL and data bus */
	lst->v_initcmd &= ~A_SEL & ~A_DBUS;
	HW(INITCMND) = lst->v_initcmd;
	
	/* if timeout occurred .... */
	if (sink == 0) {
 		return E_HARDWARE;	/* free data module and return error */
	}

	/* phase detection and implementation */
	error = SUCCESS;	/* initialize error accumulator to success */
	while (((HW(STATREG) & BUSY_BT) != 0) && (error == SUCCESS)) {
		
		bus_phase = HW(STATREG);
		
		if( (bus_phase & RST_BT) || (lst->v_SCSIReset) ) {
			error = E_DIDC;
			continue;
		}

		if ( !(bus_phase & REQ_BT) )		/* REQ true? */
			continue;						/* no, try again */

		switch ( (bus_phase &= PHS_BTS) ) {

			case (COMMAND << 2):			/* Command Phase */
				HW(TARGCMND) = COMMAND;
				error = sendcmd(lst, hw, cmd->_cb_cmdptr);
			 	break;

			case (DATAIN << 2):				/* data I/O transfer phase */
			case (DATAOUT << 2):
				/* set correct bus phase.. */
				HW(TARGCMND) = (bus_phase == (DATAIN << 2)) ? DATAIN : DATAOUT;
				error = data_io(lst, hw);   /* .. and do the I/O */
				break;
				
			case (STATUS << 2):				/* Status Phase */
				HW(TARGCMND) = STATUS;
				/* get status byte and mask out reserved bits: %00011110 */
				lst->v_statbyte = (hs_input(lst, hw, &error) & 0x1E);
				break;

			case (MSGIN << 2):				/* Message IN phase */
				HW(TARGCMND) = MSGIN;
				lst->v_msgin = hs_input(lst, hw, &error);	/* get message byte */
				break;

			default:
		 		error = E_NOTRDY;	/* free data module and return error */
		 		break;
		}
	}
	return error;			/* all done, and all ok */
}

/*
 * hs_input: Hand shake input of status/message byte from drive.
 */
u_char hs_input(lst, hw, erroret)
register Lstat		lst;		/* low level static storage */
register Hardware	hw;			/* scsi controller address */
int *erroret;					/* error return pointer */
{
	register int stat;
	register u_char	bus_data;	/* bus data information */
	
	*erroret = 0;				/* no error as yet */

	/* wait until input byte is ready */
	while ((stat = HW(STATREG) & REQ_BT) == 0) {
		if( (stat & RST_BT) || (lst->v_SCSIReset) )
			return (*erroret = E_DIDC);
	}
			
	bus_data = HW(SCSIDATA);				/* read bus data */
	lst->v_initcmd = HW(INITCMND) | A_ACK;
	HW(INITCMND) = lst->v_initcmd;			/* assert ack */

	/* wait for req deassertion */
	while ( ((stat = HW(STATREG)) & REQ_BT) != 0) {
		if( (stat & RST_BT) || (lst->v_SCSIReset) )
			return ( *erroret = E_DIDC);
	}

	lst->v_initcmd = HW(INITCMND) & ~A_ACK;
	HW(INITCMND) = lst->v_initcmd;			/* deassert ack */
	return bus_data;
}


/*
 * sendcmd: Send command to controller
 */
error_code sendcmd(lst, hw, cmdblock)
register Lstat			lst;		/* low level static storage */
register Hardware		hw;			/* SCSI controller address */
register u_char			*cmdblock;	/* SCSI command block pointer */
{
	register int counter;
	register error_code	error;
	int sink;						/* junk collector */

	
	counter = lst->v_cmdlen - 1;	/* set CDB cmd length counter */
	
	lst->v_initcmd = HW(INITCMND) | A_DBUS;	/* enable data bus */
	HW(INITCMND) = lst->v_initcmd;
	HW(MODEREG) |= DMA_MD;					/* enable phase interrupts */

	/* Handshake command bytes to controller */
	do {
		if (counter == 0) {				/* last cmd byte? */
			sink = HW(RESET);			/* reset interrupts */
		}

		/* wait for request active */
  		while ( ((sink = HW(STATREG)) & REQ_BT) == 0) {
			if( (sink & RST_BT) || (lst->v_SCSIReset) )
				return E_DIDC;
		}
	
		HW(SCSIWDAT) = *cmdblock++;				/* load cmd byte, inc ptr */
		lst->v_initcmd = HW(INITCMND) | A_ACK;	/* set ack true */
		HW(INITCMND) = lst->v_initcmd;

		/* wait for request release */
		while ( ((sink = HW(STATREG)) & REQ_BT) != 0) {
			if( (sink & RST_BT) || (lst->v_SCSIReset) )
				return E_DIDC;
		}
	
		lst->v_initcmd = HW(INITCMND) & ~A_ACK;	/* release ack  */
		HW(INITCMND) = lst->v_initcmd;

	} while (--counter >= 0);	/* loop until all bytes sent */

	/* synchronize with irq */
	while (_fev_wait(lst->v_irqevt, 1, 1, &sink) == 0 && sink == 0) {
		if( lst->v_SCSIReset ) {
			HW(MODEREG) &= ~DMA_MD;		/* disable interrupts */
			return E_DIDC;			/* SCSI Reset seen */
		}
	}
	
	HW(MODEREG) &= ~DMA_MD;		/* disable phase change interrupt */
	return SUCCESS;
}

/*
 * data_io: Transfer data to/from controller using DMA
 */
int data_io(lst, hw)
register Lstat				lst;	/* low level static storage */
register Hardware			hw;		/* scsi controller address */
{
	register Dmahardware	dmahw;	/* DMA controller address */
	int sink;						/* junk collector */
	int error = 0;
	
	dmahw = lst->v_dmabase;					/* get DMA address */
	if( (sink = set_dma(lst, hw)) ) {
		reset_dma( lst, hw );
		return sink;						/* set up DMA */
	}

	lst->v_dmaactive = get_system_ticks();
	HW(MODEREG) |= (DMAMODE);				/* enable DMA */

	/* select direction for shadow mask */
	if (lst->v_dir == INPUT) {
		lst->v_initcmd &= ~A_DBUS;
		HW(INITCMND) = lst->v_initcmd;		/* enable data bus in chip */
		HW(INTDMARX) = WHOCARES;			/* start DMA read */
	} else {
		lst->v_initcmd |= A_DBUS;
		HW(INITCMND) = lst->v_initcmd;		/* enable data bus in chip */
		HW(INTDMATX) = WHOCARES;			/* start DMA write */
	}
	
	/* wait for the SCSI interrupt (EOP) */
	/*  should also take into account that there may be intermediate
	 *   disconnects from the various (non-standard SCSI devices::= VIPER)
	 *   devices on the bus.
	 */
	while( (_fev_wait(lst->v_irqevt, 1, 1, &sink) == 0) && (sink == 0) ) {
		if ( (dmahw->dh_status & ERRORSET) ||
	         (lst->v_SCSIReset) ||
	         (lst->v_monDTO) ) {
	    	break;
	    }
	}
	
	lst->v_dmaactive = 0;			/* clear DMA active flag */
	HW(MODEREG) &= ~(DMAMODE);		/* clear DMA mode on SCSI chip */

	reset_dma(lst);					/* insure DMA disabled! */
	
	/* if SCSI Bus Reset is seen, return Disk ID Change */
	if( lst->v_SCSIReset ) {
		error = E_DIDC;
	} else { 	/* check for DMA problems */
		if( (lst->v_csr & ERRORSET ) || (lst->v_monDTO) ) {
			lst->v_monDTO = FALSE;		/* clear timeout status */
			scsi_Reset(hw, lst);		/* reset the SCSI bus (backs devices off) */
			if( lst->v_cer == ERBUSMEM )
				error =  E_BUSERR;      /* bad dma pointer */
			else
				error = E_HARDWARE;     /* about as specific as possible */
		}
	}
	return error;		/* all is well */
}

/*
 * set_dma: Set up DMA transfer.
 *
 *    Returns:  state of SCSI Reset flag.
 */
int set_dma(lst, hw)
register Lstat				lst;			/* low level static storage */
register Hardware			hw;				/* scsi controller address */
{
	register Dmahardware	dmahw;			/* DMA controller address */
	register u_char			xfer_dir;		/* data direction mask */
	
	dmahw = lst->v_dmabase;					/* get DMA address	 */
	dmahw->dh_chlctrl = OPABORT;			/* make sure nothing running*/
	dmahw->dh_status = 0xFF;				/* reset error bits	 */
	dmahw->dh_devctrl = (STEALMOD + DEVACKRY + DEV8BIT + STATINP);

	if (lst->v_dir == INPUT) {				/* get transfer direction */
		dmahw->dh_devaddr = &HW(SCSIRDAT);	/* set input reg address */
		xfer_dir = DEVTOMEM;				/* set data direction mask */
	} else {
		dmahw->dh_devaddr = &HW(SCSIWDAT);	/* set output reg address */
		xfer_dir = MEMTODEV;				/* set data direction mask */
	}

	dmahw->dh_opsctrl = xfer_dir | (BYTESIZE + REQINIT); /* set byte xfer */
	dmahw->dh_chlpri = 0;				/* set highest priority */
	dmahw->dh_memtfrct = lst->v_datalen;/* set up byte transfer count */
	dmahw->dh_memaddr = lst->v_buffer;	/* set up sector buffer address */
	dmahw->dh_memfuncd = SUPVDATA;		/* set memory access */
	dmahw->dh_devfuncd = SUPVDATA;		/* set device access */
	dmahw->dh_seqctrl = MEMCNTUP;		/* set MAR to count up, DAR no cnt */
	dmahw->dh_chlctrl = STARTOP;		/* start dma controller */
	return lst->v_SCSIReset;			/* if reset occurs, returns TRUE */
}

/*
 * reset_dma: read DMA registers and cleanup DMA
 */
int reset_dma(lst)
register Lstat				lst;			/* low level static storage */
{
	register Dmahardware	dmahw;		/* DMA controller address */
	
	dmahw = lst->v_dmabase;				/* get DMA address */
	lst->v_csr = dmahw->dh_status;		/* pickup channel status */
	lst->v_cer = dmahw->dh_error;		/* and channel error register */
	dmahw->dh_chlctrl = OPABORT;		/* stop channel	 */
	dmahw->dh_chlctrl = 0;				/* clear command */
	dmahw->dh_status = 0xFF;			/* reset status bits */
}


#ifdef USEDMA
#ifdef JNMS
/*
 *  dma_monitor(lst) -  dma monitor 
 *
 *  This code is executed as the service for an alarm.  The purpose is that
 *  if a DMA bus error occurs then  the code can recover without locking the
 *  SCSI bus permanently.  This routine will monitor the length of time
 *  that a DMA is taking and warn the awaken the process waiting on DMA,
 *  which can then take the appropriate action.
 */
dma_monitor( lst )
register Lstat lst;		/* pointer to low level static storage */
{
	if( (lst->v_monda = lst->v_dmaactive) ) {
		/* DMA is active, check a bit further */
		if( lst->v_dmaactive == lst->v_monds ) {
			/* same DMA we saw last time */
			if( ++lst->v_monct == DMA_MAX_TIME ) {
				 /* too long, must be a problem */
				lst->v_DMATOs++;				/* count time-outs */
				lst->v_monDTO++;				/* flag timeout occured */
				_fev_signl(lst->v_irqevt, 0);	/* wake the driver */
			}
		} else {
			/* new incarnation of DMA, init monitor variables */
			lst->v_monds = lst->v_dmaactive;	/* start time */
			lst->v_monct = 0;					/* count */
		} /* end test for same DMA as last entry */
	} /* end if DMA active */
} /* end of dma_monitor() */

#ifdef _UCC
_asm(" use io_jasm.a");
#else
@ use io_jasm.a
#endif

#endif JNMS
#endif USEDMA
