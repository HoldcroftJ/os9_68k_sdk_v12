/*
 * hw53c810.c - NCR 53C810 SCSI SCRIPT and SIOP Hardware specific routines
 */
 
/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1995-2001 by Microware Systems Corporation         |
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
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
| 01 96/08/03 Added edition history.                                  rak   |
|             ---- OS-9000/x86 V2.1 Released ----                           |
| 02 96/09/02 Read SCSI status as word to prevent 12 CLK delay.		  GbG   |
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
| 03 97/04/20 Added Ultra and Wide Support.                                 |
|             SCRIPTS in RAM and PCI_IO/MEMORY modes added.           GbG   |
| 04 97/06/01 Fixed problem in save_state, datacount not adjusted.    GbG   |
|             ---- OS-9000/PPC V2.1.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
| 05 98/01/29 Fixed compiler warnings.                                GbG   |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- SCSI Common Sub-Component v1.0 Released ----             |
| 06 99/07/02 Added relocate() for a check which PCI space is used.   KeiT  |
| 71 01/08/09 change debug code to use dbglog.                        gdw   |
`--------------------------------------------------------------------------*/
 
#define _HW53C810_C_

#include <scsisiop.h>
#include <v53c810.c>
#include <ncrsiop.h>

#if defined(OS_CACHE_FLUSH) && defined(_OS9000)
#include	<cache.h>
void cache_flush(Lstat lst, u_int32 *addr, u_int32 size);
#endif

#define	MAXPMSG			0xFA		/* max sdtr period msg (1 Mb/s) */
#define	SI_BADPMSG		0xFF		/* value sent greater than MAXPMSG   */
#define	NORMALIZE		1000000000	/* normalization value for clock calc */


/*
 * Define local functions.
*/

u_int32 sdtrmsg(Lstat lst, u_int32 periodmsg, u_int32 clk);
Lthread find_lth( Lstat lst );
void ncr_dma_buserror( Lstat lst );
void ncr_dma_illscript( Lstat lst );

/*
 *	Function:
 *		relocate - patch script elements as required
 *
 *  Synopsis:
 *		int relocate( Lstat lst );
 *
 *	Description:
 *		Patches script. The script file "v53c810.c" is produced with
 *		the NCR script file compiler
 *
 *		SCRIPTS RAM OPTION -dSCRIPTS_RAM
 *
 *		The -dSCRIPTS_RAM option must be in effect to use the 
 *		special SCRIPTS RAM option. Some system platforms such
 *		as PowerPC may have problems with this feature.
 *      On Symbios controllers that support SCRIPTS RAM feature
 *      the PCI configuration base[2] address will be used for
 *		scripts. 
 *
 *      
 *
 */

error_code relocate( Lstat lst)
{
	Siop_p 			siop_mb = (Siop_p) lst->pci_memory_base;
	error_code		error;		/* Error code holder */
	int				i;			/* local index */
	int				loop;		/* local index */
	u_int32			*script;	/* temp pointer to SCSI SCRIPTs */
	u_int32			*dest,*src;	/* temp pointers to SCSI SCRIPTs */
	u_int32			tmp;		/* temp for swap usage */

	/*
	 * Check if siop_mb is valid, or if we must use the PCI IO Space.
	 * Some SCSI Hardware is only configured to use PCI IO Space. For that
	 * reason we need to check if the PCI MEM Space is 0. Then use the
	 * hdware (pci_io_base + PCI_IO_BASE) address. If both are zero
	 * KeiT 07/02/99
	 */

	if (!siop_mb)
	{
		if (!lst->pci_io_base)
		{
			/* the pci_io_base is also 0. This is not good! */
#if defined (__STDC__)
			return EOS_PARAM;
#else	
			return E_PARAM;
#endif 
		}	
		siop_mb = (Siop_p) (lst->pci_io_base | PCI_IO_BASE);
	}

	/*
	 * Get script size
	*/
	
	lst->script_sz = loop = sizeof(SCRIPT);

	loop /=4;								/* bytes to longwords */

	src = (u_int32 *) SCRIPT;				/* set source and destination */
	
	/*
	 * For NCR53c825A and NCR53c875 SCRIPTS RAM
	 * may be located on the chip itself.
	 * Init will probe for the SCRIPTS RAM and 
	 * if found will save the address in pci_ram_base.
	 * If pci_ram_base is zero or the script is larger
	 * then 4k then we must allocate RAM for the scripts.
	*/

	if ( lst->pci_ram_base && (loop <= MAX_SRAM_SZ) ){	
		lst->act_script = lst->pci_ram_base;
	} else {
		if ((error =
			_os_srqmem( (u_int32 *)&lst->script_sz, 
				(void **)&lst->act_script,MEM_ANY)) != SUCCESS)
			return error;
			lst->pci_ram_base = 0;
	}

	script = dest = (u_int32 *) (lst->act_script);
	
	while( --loop >= 0 )
	{
		tmp = *src++;
		*dest++ = SWAPLONG(tmp);					/* move it */
	}

    /* Patch script to update external identifers */

	tmp = (u_int32) &lst->armsg.msg.mpe;
	tmp = SCRIPT_ADDRESS(SWAPLONG(tmp));
		
    for (i = 0; i < sizeof(E_armsg_mpe_Used) / 4; i++) {
		script[E_armsg_mpe_Used[i]] = tmp;
    }

	tmp = (u_int32) &lst->armsg.msg.reject;
	tmp = SCRIPT_ADDRESS(SWAPLONG(tmp));

    for (i = 0; i < sizeof(E_armsg_reject_Used) / 4; i++) {
		script[E_armsg_reject_Used[i]] = tmp;
    }

	tmp = (u_int32) &lst->armsg.msg.abort;
	tmp = SCRIPT_ADDRESS(SWAPLONG(tmp));

    for (i = 0; i < sizeof(E_armsg_abort_Used) / 4; i++) {
		script[E_armsg_abort_Used[i]] = tmp;
    }

	tmp = (u_int32) &lst->selflg.sipflag[0];
	tmp = SCRIPT_ADDRESS(SWAPLONG(tmp));

    for (i = 0; i < sizeof(E_bitbucket_Used) / 4; i++) {
		script[E_bitbucket_Used[i]] = tmp;
    }

	tmp = (u_int32) &lst->selflg.sipflag[3];
	tmp = SCRIPT_ADDRESS(SWAPLONG(tmp));

    for (i = 0; i < sizeof(E_statadr_Used) / 4; i++) {
		script[E_statadr_Used[i]] = tmp;
    }

	tmp = (u_int32) &lst->extcnt.mcnt.cnt[1];
	tmp = SCRIPT_ADDRESS(SWAPLONG(tmp));

    for (i = 0; i < sizeof(E_extcntptr1_Used) / 4; i++) {
	script[E_extcntptr1_Used[i]] = tmp;
    }

	tmp = (u_int32) &lst->extcnt.mcnt.cnt[0];
	tmp = SCRIPT_ADDRESS(SWAPLONG(tmp));

    for (i = 0; i < sizeof(E_extcntptr_Used) / 4; i++) {
		script[E_extcntptr_Used[i]] = tmp;
    }

	tmp = (u_int32) &lst->extmsgfram;
	tmp = SCRIPT_ADDRESS(SWAPLONG(tmp));

    for (i = 0; i < sizeof(E_extmframeptr_Used) / 4; i++) {
		script[E_extmframeptr_Used[i]] = tmp;
    }

	tmp = (u_int32) &lst->lcrcs.lcrcid.idmsg;
	tmp = SCRIPT_ADDRESS(SWAPLONG(tmp));

    for (i = 0; i < sizeof(E_lcrcid_idmsg_Used) / 4; i++) {
		script[E_lcrcid_idmsg_Used[i]] = tmp;
    }

	tmp = (u_int32) &lst->lcrcs.lcrcid.tid;
	tmp = SCRIPT_ADDRESS(SWAPLONG(tmp));

    for (i = 0; i < sizeof(E_lcrcid_tid_Used) / 4; i++) {
		script[E_lcrcid_tid_Used[i]] = tmp;
    }

	tmp = (u_int32) &lst->phythread_swap;
	tmp = SCRIPT_ADDRESS(SWAPLONG(tmp));

    for (i = 0; i < sizeof(E_phythread_Used) / 4; i++) {
		script[E_phythread_Used[i]] = tmp;
    }

	/* fill in NCR addresses */
	tmp = (u_int32) &siop_mb->dsa;
	tmp = SWAPLONG(tmp);
    for (i = 0; i < sizeof(E_ncrdsa_Used) / 4; i++) {
		script[E_ncrdsa_Used[i]] = tmp;
    }

	tmp = (u_int32) &siop_mb->slpar;
	tmp = SWAPLONG(tmp);
    for (i = 0; i < sizeof(E_ncrlcrc_Used) / 4; i++) {
		script[E_ncrlcrc_Used[i]] = tmp;
    }

	tmp = (u_int32) &siop_mb->scratch0;
	tmp = SWAPLONG(tmp);
    for (i = 0; i < sizeof(E_ncrscratch0_Used) / 4; i++) {
		script[E_ncrscratch0_Used[i]] = tmp;
    }

	tmp = (u_int32) &siop_mb->scratch1;
	tmp = SWAPLONG(tmp);	
    for (i = 0; i < sizeof(E_ncrscratch1_Used) / 4; i++) {
		script[E_ncrscratch1_Used[i]] = tmp;
    }
    
	tmp = (u_int32) &siop_mb->scratch3;
	tmp = SWAPLONG(tmp);
    for (i = 0; i < sizeof(E_ncrscratch3_Used) / 4; i++) {
		script[E_ncrscratch3_Used[i]] = tmp;
    }
   
	coherent();					/* insure memory update */
	
	/* and finally,  cache the addresses of the various select entry points
	 *  into the script to streamline testing within the driver 
	 */
	lst->snatnaddr = lst->act_script+Ent_SELECTNOATN;
	lst->swatnaddr = lst->act_script+Ent_SELECTATN;
	
	return SUCCESS;							/* no problems! */
}


/*
 * hw53c810.c - NCR 53C810 SIOP specific subroutines.
 *
 * The following routines either prepare for or start the
 * script running at a specific script entry routine.
 * The script entry points all begin with "Ent_".
 *
*/

/*
 *	Function:
 *		act_scsi_select - prepare for SCSI select with or without ATN 
 *
 *  Synopsis:
 *		void act_scsi_select( lst, lth );
 *
 *	Description:
 *		Point lth-scriptstart to script select entry.
 *		The select entry is based on ATN from the SCSI_OPTS
 *		This routine only sets the entry. The user will do
 *	    a "rethread" or set "dsp" directly to start script.
 *
*/


void act_scsi_select( Lstat lst, Lthread const lth )
{
	u_int32		tmp;

	/* condition script entry and identify message based on ATN flag */		

	if( lth->xferflags & SCSI_ATN )
		tmp = lst->act_script+Ent_SELECTATN;
	else
		tmp = lst->act_script+Ent_SELECTNOATN;

	lth->scriptstart = (u_int8 *)SCRIPT_ADDRESS(SWAPLONG(tmp));
}


/*
 *	Function:
 *		scsi_rejectmsg - Reject SCSI message 
 *
 *  Synopsis:
 *		void scsi_rejectmsg( lst, lth );
 *
 *	Description:
 *		Start script at "Ent_REJECTMESSAGE"
 *
 */


void scsi_rejectmsg( Lstat lst, Lthread const lth )
{
	u_int32		tmp;

    tmp = lst->act_script+Ent_REJECTMESSAGE;

	rethread( lst, lth, SCRIPT_ADDRESS(SWAPLONG(tmp)));
}

/*
 *	Function:
 *		act_scsi_rejectmsg - Prepare to reject SCSI message 
 *
 *  Synopsis:
 *		void act_scsi_rejectmsg( lst, lth );
 *
 *	Description:
 *		Point lth-scriptstart to script rejectmsg entry.
 *		This routine only sets the entry. The user will do
 *	    a "rethread" or set "dsp" directly to start script.
 *
*/


void act_scsi_rejectmsg( Lstat lst, Lthread const lth )
{
	u_int32		tmp;

	tmp = lst->act_script+Ent_REJECTMESSAGE;

	lth->scriptstart = (u_int8 *)SCRIPT_ADDRESS(SWAPLONG(tmp));
}



/*
 *	Function:
 *		scsi_idle -  Enter SCSI idle state 
 *
 *  Synopsis:
 *		void scsi_idle( lst, lth );
 *
 *	Description:
 *		Start script at "Ent_WHICHPHASE"
 *
*/


void scsi_idle( Lstat lst, Lthread const lth )
{
	u_int32		tmp;

	tmp = lst->act_script+Ent_WHICHPHASE;

	rethread( lst, lth, (int32) SCRIPT_ADDRESS(SWAPLONG(tmp)));

}


/*
 *	Function:
 *		act_scsi_idle -  Prepare to enter SCSI idle state 
 *
 *  Synopsis:
 *		void act_scsi_idle( lst, lth );
 *
 *	Description:
 *		Point lth-scriptstart to script idle entry.
 *		This routine only sets the entry. The user will do
 *	    a "rethread" or set "dsp" directly to start script.
 *
 */


void act_scsi_idle( Lstat lst, Lthread const lth )
{
	u_int32		tmp;

	tmp = lst->act_script+Ent_WHICHPHASE;

	lth->scriptstart = (u_int8 *)SCRIPT_ADDRESS(SWAPLONG(tmp));

}


/*
 *	Function:
 *		scsi_free -  Enter SCSI free state 
 *
 *  Synopsis:
 *		void scsi_free( lst, lth );
 *
 *	Description:
 *		Start script at "Ent_FREEIT"
 *
 */

void scsi_free( Lstat lst, Lthread const lth )
{
	Siop_p const	siop = (Siop_p) lst->hdware; /* set hardware pointer */
	u_int32			tmp;
	u_int8			val;
	
	if( lth == &lst->logthread[lst->selfid] ) {
		lth->threadstate = TH_EMPTY;
		reschedule( lst );	
	} else 	{

		lth->chiperror = BUSY;		/* say BUSY so retry will happen */

		coherent();					/* insure memory update */

		tmp = SCRIPT_ADDRESS(SWAPLONG(((u_int32)lth)));

		SCSI_OUTL(&siop->dsa, tmp);					/* data structure address */
		lst->phythread_swap = tmp;

#if defined(OS_CACHE_FLUSH) && defined(_OS9000)

		/* Flush thread */
		cache_flush( lst, (u_int32 *)lst, (u_int32) sizeof (struct lstat));

#endif

		/*
		 * Disable and force synchronous re-negotiation if required
		 * Wide negotiation will force asynchronous.
		*/
	
		val = SCSI_INC(&siop->scntl3)&SCNTL3_CCF_MASK;
		lth->devfield.devsync.scntl3 = val | (val << 4);
	
		lth->synctried = 0;
		lth->widetried = 0;		
		lth->devfield.devsync.sxfr_mask = 0;
		lth->sbclmask = 0;

		SCSI_OUTC(&siop->sdid, lth->devfield.devsync.scsidev);		/* set ID */
		SCSI_OUTC(&siop->sxfer, lth->devfield.devsync.sxfr_mask);
		SCSI_OUTC(&siop->scntl3, lth->devfield.devsync.scntl3);

		/* start script at this place! */

		SCSI_OUTL(&siop->dsp, 
			SCRIPT_ADDRESS(SWAPLONG(lst->act_script+Ent_FREEIT)));	
	}
}


/*
 *	Function:
 *		act_scsi_free -  Prepare to enter SCSI free state 
 *
 *  Synopsis:
 *		void act_scsi_free( lst, lth );
 *
 *	Description:
 *		Point lth-scriptstart to script scsi_free entry.
 *		This routine only sets the entry. The user will do
 *	    a "rethread" or set "dsp" directly to start script.
 *
*/


void act_scsi_free( Lstat lst, Lthread const lth )
{
	u_int32		tmp;

	tmp = lst->act_script+Ent_FREEIT;

	lth->scriptstart = (u_int8 *)SCRIPT_ADDRESS(SWAPLONG(tmp));	

}


/*
 *	Function:
 *		scsi_pemsg - Send SCSI parity message 
 *
 *  Synopsis:
 *		void scsi_pemsg( lst, lth );
 *
 *	Description:
 *		Start script at "Ent_PEMESSAGE"
 *
*/

void scsi_pemsg( Lstat lst, Lthread const lth )
{
	u_int32		tmp;

    tmp = lst->act_script+Ent_PEMESSAGE;

	rethread( lst, lth, SCRIPT_ADDRESS(SWAPLONG(tmp)));	
}


/*
 *	Function:
 *		scsi_smsg - Send SCSI message
 *
 *  Synopsis:
 *		void scsi_smsg( lst, lth );
 *
 *	Description:
 *		Start script at "Ent_SENDMESSAGE"
 *
*/


void scsi_smsg( Lstat lst, Lthread const lth )
{
	u_int32		tmp;

    tmp = lst->act_script+Ent_SENDMESSAGE;

	rethread( lst, lth, SCRIPT_ADDRESS(SWAPLONG(tmp)));	
}


/*
 *	Function:
 *		act_scsi_waitrsel - prepare for SCSI WAIT FOR RE-SELECT 
 *
 *  Synopsis:
 *		void act_scsi_waitrsel( lst, lth );
 *
 *	Description:
 *		Point lth-scriptstart to script wait for reselect entry.
 *		This routine only sets the entry. The user will do
 *	    a "rethread" or set "dsp" directly to start script.
 *
*/

void act_scsi_waitrsel( Lstat lst, Lthread const lth )
{
	u_int32		tmp;

	tmp =  lst->act_script+Ent_WAITFORRESELECT;	

	lth->scriptstart = (u_int8 *)SCRIPT_ADDRESS(SWAPLONG(tmp));	
}


/*
 *	Function:
 *		nthread - restart script at current dsp 
 *
 *  Synopsis:
 *		void nthread( lst, lth );
 *
 *	Description:
 * 		restart script at current dsp 
 *
*/

void nthread( Lstat lst, Lthread const lth )
{
	Siop_p const	siop = (Siop_p) lst->hdware;

	rethread( lst, lth, SCSI_INL(&siop->dsp));	/* remain async */
}


/*
 * hw53c810.c - NCR 53C810 SIOP specific subroutines.
 *
 * The following routines are support level setup for the SIOP.
 * See "Dev???.c" files for more information.
 *
 */


/*
 * Function: 
 *		ncr_reset - reset SIOP and reset parameters
 *
 * Synopsis:
 * 		int ncr_reset( Lstat lst )
 *
 * Description:
 *		This routine will perform a software reset of the SIOP and
 * reload the parameters needed for the chips proper function.
 *
*/

error_code ncr_reset( Lstat lst )
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */	

	SCSI_OUTC( &siop->sien0, 0);	/* disable interrupts */
	SCSI_OUTC( &siop->sien1, 0);	/* disable interrupts */

	/* perform a soft reset of the siop  Note: NOT a SCSI BUS RESET! */

 	/* set.. */

 	SCSI_OUTC( &siop->istat, SCSI_INC( &siop->istat ) | B_ISTAT_SRST ); 

	/* ..and clear it */

	SCSI_OUTC( &siop->istat, 0 ); 	                

	/* check DMA FIFO EMPTY FLAGS to see if hardware is working */
	
	if ( (SCSI_INC(&siop->dstat) != 0x80) || (SCSI_INC(&siop->ctest1) != 0xf0))
#if defined (__STDC__) 
		return EOS_HARDWARE;		/* Not NCR 8xx Device */
#else
		return E_HARDWARE;			/* Not NCR 8xx Device */
#endif

	SCSI_OUTC( &siop->scid, lst->selfid);    /* set SCSI ID of the SIOP */
	SCSI_OUTC( &siop->respid, (1<<(lst->selfid)));
	/* ensure 810 mode!!! */
    SCSI_OUTC( &siop->dcntl, SCSI_INC( &siop->dcntl ) | B_DCNTL_COM);
	
	/* Set STO Timer Register (Use 819.2 msec) and disable HTH*/
	SCSI_OUTC( &siop->stime0, STIME0_820m|(STIME0_DISABLE<<4));

#ifdef RSTCODE
	/* under normal circumstances,  durring boot a SCSI Reset has been
	 *  performed by the "sysinit" code.  If there is an add-on board
	 *  that contains this chip nad no "init extension" that covers the
	 *  performs the reset,  then this should probably be enabled by 
	 *  defining RSTCODE in the makefile for the booter.
	 */
	SCSI_Reset( lst );			/*  This is a SCSI Bus Reset */
#endif /* RSTCODE */

	/*  Enable SIOP interrupts on:
	 *      Phase Mismatch             SCSI Reset
	 *      SCSI Gross Error           SCSI Bus Select Timeout
	 *      Unexpected Disconnect
	 *      Parity Error 			   Handshake-to-Handshake
	 */
	SCSI_OUTC( &siop->sien0, ( B_SIEN0_MA | B_SIEN0_SGE | 
					   /* B_SIEN0_SEL | B_SIEN0_RESEL | */
						  B_SIEN0_UDC | B_SIEN0_RST | B_SIEN0_PAR) );
	SCSI_OUTC( &siop->sien1, ( B_SIEN1_STO | B_SIEN1_HTH ) ); 
						 
	
	/*  Enable DMA interrupts on:
	 *      Bus Fault
	 *      Illegal Instruction Detected
     *		Script Interrupt Instruction
	 */

	SCSI_OUTC( &siop->dien, (B_DIEN_BF | B_DIEN_SIR | B_DIEN_IID) );

	/* Response to selection and reselection */

	SCSI_OUTC( &siop->scid, SCSI_INC(&siop->scid)|B_SCID_RRE|B_SCID_SRE);

	return SUCCESS;
}


/*
 *	Function:
 *		ncr_start - Start SIOP at IDLE entry
 *
 *  Synopsis:
 *		ncr_start( lst );
 *
 *	Description:
 *  	Start SIOP at IDLE entry
*/

void ncr_start( Lstat lst )
{
	Siop_p const	siop = (Siop_p) lst->hdware; /* set hardware pointer */
	Lthread			lth;
	u_int32			tmp;
	
	/* start executing the script at the IDLE entry point */
	lth = &lst->logthread[lst->selfid];		/* address of the IDLE thread */
	lst->phythread = lth;

	tmp = (u_int32)lth;
	tmp = SCRIPT_ADDRESS(SWAPLONG(tmp));
	lst->phythread_swap = tmp;

    SCSI_OUTL( &siop->dsa, tmp);
	
#if defined(OS_CACHE_FLUSH) && defined(_OS9000)

	/* Flush thread */
	cache_flush(lst, (u_int32 *)lst, (u_int32) sizeof (struct lstat));

#endif

    SCSI_OUTL( &siop->dsp, (u_int32)lth->scriptstart);

}


/*
 *	Function:
 *		ncr_stop - Stop SIOP 
 *
 *  Synopsis:
 *		ncr_stop( lst);
 *
 *	Description:
 *  	Stop SIOP
 */

void ncr_stop( Lstat lst )
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */	
	
	/* disable all interrupt */
	SCSI_OUTC( &siop->sien0, 0);
	SCSI_OUTC( &siop->sien1, 0); 
	SCSI_OUTC( &siop->dien, 0); 

	/* perform a soft reset of the siop */
	
	/* set reset....  */
	SCSI_OUTC( &siop->istat, SCSI_INC( &siop->istat  ) | B_ISTAT_SRST );

	/* ..and clear it */
	SCSI_OUTC( &siop->istat, SCSI_INC( &siop->istat ) & ~B_ISTAT_SRST );
}


/*
 *	Function:
 *		ncr_SCSI_reset - Start SCSI Reset
 *
 *  Synopsis:
 *		ncr_SCSI_reset( lst );
 *
 *	Description:
 *  	Start SCSI Reset
 */

void ncr_SCSI_reset( Lstat lst )
{
	Siop_p const		siop = (Siop_p) lst->hdware; /* set hardware pointer */	
	volatile u_int32	bitbucket;			/* garbage holder */
	u_int32				sleepval;			/* holder for sleep value */
	signal_code			signal;				/* holder for signal code */

	/* Assert SCSI Reset */
	SCSI_OUTC( &siop->scntl1, SCSI_INC( &siop->scntl1 ) | B_SCNTL1_RST );

	sleepval = ONE_TICK;
	_os_sleep( &sleepval, &signal);		

	/* Deassert SCSI Reset */
	SCSI_OUTC( &siop->scntl1, SCSI_INC( &siop->scntl1 ) & ~B_SCNTL1_RST );

	sleepval = ONE_TICK+ONE_TICK;
	_os_sleep( &sleepval, &signal);		

	/* Clear Interrrupt Status */
	sleepval = ONE_TICK;
	_os_sleep( &sleepval, &signal);		
	bitbucket= SCSI_INC( &siop->dstat );
	sleepval = ONE_TICK;
	_os_sleep( &sleepval, &signal);		
	bitbucket= SCSI_INC( &siop->sist0 );
	sleepval = ONE_TICK;
	_os_sleep( &sleepval, &signal);		
	bitbucket= SCSI_INC( &siop->sist1 );
}


/*
 *	Function:
 *		ncr_kick - inform running script of new SCSI thread to service
 *
 *  Synopsis:
 *		void ncr_kick( Lstat lst );
 *
 *	Description:
 * 		inform running script of new SCSI thread to service
 *  	
*/


void ncr_kick( Lstat lst )
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */		

	SCSI_OUTC( &siop->istat, SCSI_INC(&siop->istat) | B_ISTAT_SIGP );
}



/*
 *	Function:
 *		save_state - save the chip's pointers and state 
 *
 *  Synopsis:
 *      void save_pointers( Lstat lst );
 *
 *  Description:
 *		This function will save the state of the SIOP and the current
 *	PHYSICAL thread. The nature of the command that is being performed
 *  governs the operation of the subroutine.  If the command calls for
 *  data transfer this routine has to insure the integrity of the FIFO
 *  and data pointers.  This routine is dependent to some degree on 
 *  whether the data transfer is synchronous or not.  Note that the
 *  script resumtion of the thread will not nessesarily be the point
 *  at which the save was invoked. (Determined by other routines.)
 *		During DATA_OUT operations some amount of data is likly to have
 *  been DMAed prior to the interrupt.  As such, we need to bring
 *  the pointers up to date for later rethread of the command.
 *		During DATA-IN operations the registers in the SIOP will be 
 *  correct.  The manual states that the FIFO's are allowed to complete
 *  data transfer at phase change prior to the generation of an interrupt.
 *  In the case of LTO (bus error - non-fatal), the script will simply be
 *  restarted by the interrupt handler and the residuals will  be transfered.
 *
 *  See Also:
 *		rethread() 
 */

void save_state( Lstat lst )
{
	Siop_p const	siop = (Siop_p) lst->hdware; /* set hardware pointer */		
	Lthread			lth;				/* logical thread  */
	u_int32			fresidual = 0;		/* number of residual bytes (calculated) */
	u_int32			ccnt;				/* image of the data byte count register */
	u_int32			tmp;				
	u_int32			dataptr, datacount;
	u_int32			dfifo_val;
	volatile		u_int8 phase;
	
	fresidual = 0;	/* clear regidual counter */

	lth = lst->phythread;			/* set pointer to physical thread */

	if( lth == &lst->logthread[lst->selfid] ) {

		/* 
		 * pathalogical case, somebody has mismanaged the bus...
		*/

		scsi_free( lst, lth );
		return;

	}

	lth->phaseflg = SCSI_INL((u_int32 *)&siop->scratch0);	/* preserve phase flage */
	
	tmp = SCSI_INL((u_int32 *)&siop->dbc[0]);
	tmp = SWAPLONG(tmp);
	ccnt =  tmp & DBC_MSK;      /* DMA count register */

	datacount = SWAPLONG(lth->datacount);	
	dataptr = SCRIPT_MADDRESS(SWAPLONG((u_int32)lth->dataptr));
	dataptr += datacount-ccnt;

	act_scsi_idle( lst, lth );			

	/* 
	 *   must perform check for residual data in the various FIFOs.
	 *   This is direction dependent and transfer mode dependent.
	*/

	phase =  SCSI_INC( &siop->socl) & 0x07;

	switch (phase){

		case DATAOUT:
			/* 
			*  determine the amount of residual data in the DMA FIFO
			*  here according to the algorithm provided by NCR.
			*/
 
			dfifo_val = ((SCSI_INC(&siop->ctest5) & 
				(B_CTEST5_B08|B_CTEST5_B09)) << 8) | (SCSI_INC(&siop->dfifo));

			if( (SCSI_INC(&siop->ctest5)) & B_CTEST5_DFS )  /* Big FIFO */
				fresidual = ((dfifo_val & DLFIFO_MSK) - 
						(ccnt & DLFIFO_MSK)) & DLFIFO_MSK;

			else /* Default FIFO */
				fresidual = ((dfifo_val & DFIFO_MSK) - 
					(ccnt & DFIFO_MSK)) & DFIFO_MSK;

			if( (SCSI_INC(&siop->sstat0)) & B_SST0_OLF )  /* SODL LSB FULL */
				fresidual++;

			if( (SCSI_INC(&siop->sstat2)) & B_SST2_OLF )  /* SODL MSB FULL */
				fresidual++;

			if( (SCSI_INC(&siop->sstat0)) & B_SST0_ORF )  /* SODR LSB FULL */
				fresidual++;

			if( (SCSI_INC(&siop->sstat2)) & B_SST2_ORF )  /* SODR MSR FULL */
				fresidual++;

			/* clear the SCSI FIFO */
			SCSI_OUTC(&siop->ctest3, SCSI_INC(&siop->ctest3) | B_CTEST3_CLF);

			dataptr -= fresidual;
			datacount = ccnt + fresidual;

			lth->dataptr = (u_int8 *)SCRIPT_ADDRESS(SWAPLONG(dataptr));
			lth->datacount = SWAPLONG(datacount);
			lth->threadstate = TH_LOGICAL;	/* mark this one disconnected */
			break;
				
		case DATAIN:

			lth->dataptr = (u_int8 *)SCRIPT_ADDRESS(SWAPLONG(dataptr));
			lth->datacount = SWAPLONG(ccnt);

			if( (SCSI_INC(&siop->scntl2)) & B_SCNTL2_WIDE_RECV )  /* SWIDE */		
			{
				u_int32  *script = (u_int32 *)lst->act_script;
				script[E_RW_Last_di_byte_buf_Used[0]] = (u_int32)lth->dataptr;
			    tmp = lst->act_script+Ent_MOVE_SWIDE_BYTE;
				lth->scriptstart = (u_int8 *)SCRIPT_ADDRESS(SWAPLONG(tmp));
				DPUTS(DBIT_HW, ("\nSave_state SWIDE"));
			}
			lth->threadstate = TH_LOGICAL;	/* mark this one disconnected */
			break;

		case MSGIN:
			DPRINTF(DBIT_HW, ("\nSave_state MSGIN = %2x", lth->msgibuf[0]));

			lth->chiperror = BUSY;		/* say BUSY so retry will happen */

			/*
			 * Clear All fifo's
			*/

 			SCSI_OUTC( &siop->ctest3, SCSI_INC( &siop->ctest3 ) | B_CTEST3_CLF ); 
		 	SCSI_OUTC( &siop->stest3, SCSI_INC( &siop->stest3 ) | B_STEST3_CSF );  	
			
			/*
			 * Check to see if disconnect has occured 
			*/
			
			if (lth->msgibuf[0] == A_M_DISCONNECT){
				lth->threadstate = TH_LOGICAL;		
			} 

			rethread( lst, lth, 
				SCRIPT_ADDRESS(SWAPLONG(lst->act_script+Ent_ACKANYMSG)) );

			return;

		case MSGOUT:
			/*
			 * On some SCSI-I drives that do not support sync
			 * this is were we will be if the sync negotiation
			 * fails. Since this will occur during select we
			 * need only mark the command done and allow it
			 * to happed again. By setting BUSY the command
			 * will be re-issued. 
			*/

			DPUTS(DBIT_HW, ("\nSave_state MSGOUT"));

			lth->chiperror = BUSY;		/* say BUSY so retry will happen */		

			lth->threadstate = TH_DONE;						/* mark it complete */
			lst->wakequeue[lst->wakes++] = lth->processid;	/* queue wakeup */
			reschedule( lst );								/* restart SIOP */

			return;
						
		default: 

			/*
			 * Clear All fifo's
			*/

 			SCSI_OUTC( &siop->ctest3, SCSI_INC( &siop->ctest3 ) | B_CTEST3_CLF ); 
		 	SCSI_OUTC( &siop->stest3, SCSI_INC( &siop->stest3 ) | B_STEST3_CSF );  	

			DPRINTF(DBIT_HW, ("\nSave_state phase = %2x", phase));

			lth->chiperror = BUSY;		/* say BUSY so retry will happen */
			scsi_free( lst, lth );
			return;

	}

	rethread( lst, lth, (int32) lth->scriptstart );

}	/* end save_state */



/*
 *	Function:
 *		ncr_irq_sort - SIOP/DMA interrupt sort
 *
 *  Synopsis:
 *		error_code ncr_irq_sort( lst );
 *
 *	Description:
 *	    Deal with SIOP interrupt. Called from IRQ.
 *
*/

error_code ncr_irq_sort( Lstat lst )
{
	Siop_p const		siop = (Siop_p) lst->hdware; 
	volatile u_int8		istat;			/* copy of the istat register */
	volatile u_int8		dstat;			/* copy of DMA status register */
	volatile u_int16	sist;			/* ISR0 and ISR1 combined image */
	volatile u_int8 	sist0;			/* copy of ISR0 status register */
	volatile u_int8 	sist1;			/* copy of ISR1 status register */

	istat =SCSI_INC(&siop->istat);
	
	if( (istat & (B_ISTAT_SIP | B_ISTAT_DIP))  == 0 ) {
		#ifdef STATS
			dblog( lst, NOTOURINT );
		#endif

	#if defined (_OS9000)
		return EOS_NOTME;
	#else
		return 1;
	#endif

	}


	do {	/* do all interrupts */

		#ifdef STATS	
			dblog(lst,istat);
		#endif

		/* process an SIOP interrupt */

		#ifdef STATS
			lst->ninterr++;			/* keep track of the number of interrupts */
		#endif

		if (istat & B_ISTAT_SIP){ /* SCSI INTERRUPT PENDING */	

			/* Read SIST0 and SIST1 as word to prevent 12 CLK period delay */
			
			sist = SCSI_INW(&siop->sist0);

#if defined(SIOP_LIL_END)
			sist0 = (sist >> 8 ) & 0xff;
			sist1 = sist & 0xff;
#else
			sist1 = (sist >> 8 ) & 0xff;
			sist0 = sist & 0xff;
#endif						
		
			if (sist0 & B_SIST0_PAR){ /* parity error detected */
				#ifdef STATS
					dblog( lst, SPARITY );
				#endif
				siop_par(lst);
			}
			if (sist0 &	B_SIST0_RST){ /* SCSI Bus Reset received */
				#ifdef STATS
					dblog( lst, SRESET );
				#endif
				siop_rrst(lst);
			}
			if (sist0 & B_SIST0_UDC){ /* Unexpected disconnect */
				#ifdef STATS
					dblog( lst, SUDC );
				#endif
				siop_udc(lst);
			}
			if (sist0 & B_SIST0_SGE){ /* SCSI Gross Error */
				#ifdef STATS
					dblog( lst, SGE );
				#endif
				siop_sge(lst);
			}

			if (sist1 & B_SIST1_STO){ /* selection timeout */
				#ifdef STATS
					dblog( lst, SELTO );
				#endif
				siop_sto(lst);
			}
			if (sist0 & B_SIST0_MA){ /* phase mismatch */
				save_state( lst);
			}
		}

		if (istat & B_ISTAT_DIP){ /* DMA INTERRUPT PENDING */
			dstat = SCSI_INC(&siop->dstat);	/* read entire status as long */	
		
			if (dstat & B_DSTAT_IID){		/* Illegal instruction */
				#ifdef STATS
					dblog( lst, ILLIS );
				#endif
                DPUTS(DBIT_HW, ("\nCalling Save_state from dma_illscript"));
				dma_illscript( lst );
			}

			if (dstat & B_DSTAT_SIR){		/* SCSI SCRIPT int instruction seen */
				routeon_dsp( lst );
			}

			if (dstat & B_DSTAT_BF){		/* DMA bus error */
				#ifdef STATS
					dblog( lst, DMABE );
				#endif
                DPUTS(DBIT_HW, ("\nCalling Save_state from dma_buserror"));
				dma_buserror( lst );	/* dma bus error detected */
			}
		}

	} while ((istat = (SCSI_INC(&siop->istat) & ( B_ISTAT_SIP | B_ISTAT_DIP ))));

	/* save last interrupt stats */
	lst->phythread->lthresv = (dstat << 16) | (sist1 << 8) | sist0;

#if defined(_MPF386) && defined(PIC_CISR)
	SCSI_OUTC(0xa0, 0x20);
	SCSI_OUTC(0x20, 0x20);
#endif

	return SUCCESS;
}


/*
 *	Function:
 *		routeon_dsp - route to appropriate script interrupt handler
 *
 *  Synopsis:
 *		void routeon_dsp( Lstat lst )
 *
 *  Description:
 *		This provides routing for the Script generated interrupts.
 *
 */

void routeon_dsp( Lstat lst )
{
	u_int32			dspvalue;
	Siop_p const	siop = (Siop_p) lst->hdware; 
	Lthread			lth = lst->phythread;

	dspvalue = SCSI_INL(&siop->dsps);
	dspvalue = (u_int32) SCRIPT_MADDRESS(SWAPLONG(dspvalue));

#ifdef STATS
	dblog( lst, dspvalue );
#endif	

	switch ( dspvalue & SIOP_INT_MSK ) {
		case  A_SC_DONE:
			scripti_done( lst, lth );
			break;

		case  A_SC_SIGP:		/* SIGP -- SCSI Bus in free state */
			scripti_sigp( lst );
			break;
			
		case A_SC_RESEL:
			scripti_resel( lst, lth );
			break;
			
		case A_SC_IDISC:
			 scripti_idisc( lst, lth );
			break;
			
		case A_SC_MSGREJ:
			 scripti_reject( lst, lth );
			break;

		case A_SC_EXMSG:
			scripti_msgi( lst, lth );
			break;

		case A_UNKNPH:			/* unidentified next phase! */
			bomblog( lst, lth, A_UNKNPH );
			scsi_free(lst, lth);
			break;
			
		case  A_NOTMSGI:						
			lst->phythread = lth = find_lth(lst);
			rethread( lst, lth, 
				SCRIPT_ADDRESS(SWAPLONG(lst->act_script+Ent_ACKANYMSG)) );
			break;

		case A_BADMSG:			/* unidentified message in received */
			scripti_badmsg( lst, lth );
			break;

		case A_SC_SWIDEB:		/* Wide receive byte */
			lth->dataptr += SWAPLONG(1);
			lth->datacount -= SWAPLONG(1);
			act_scsi_idle( lst, lth );			
			rethread( lst, lth, (int32) lth->scriptstart );
			break;
	}
}


/*
 * Return current thread 
*/

Lthread find_lth( Lstat lst )
{
	/* pointer to current */
	return ( (Lthread)&lst->logthread[lst->lcrcs.lcrcid.tid]);
}


/*
 * Function:
 *		check_synchmsg - validate targets synchronous capability.
 * Synopsis:
 *		void check_synchmsg( Lstat lst, char *msg )
 * Description:
 *		This routine will validate the synchronous message returned
 * from the target device.  If the message returns indicates an offset
 * of 0 then the device will only run asynchronous.  If the targets 
 * response indicates a period slower or offset less than ours proposed
 * values then match his requested period as close as possible but always
 * insuring that our values are slower if exact match not possible. 
 *
 *   The calculations used to determine this are:
 *
 *     (PERIOD / TCP) - 4 = XFERP  ( value to program into SXFER (TP2-TP0)
 *
 *
 *   where:  PERIOD = minimum synchronous transfer period (byte 3 of message)
 *           TCP    = 1 / SCSI core clock frequency (calculated)
 *                    (determined by; DCNTL CF(0,1) and  SBCL SSCF(1:0))
 *                    SCNTL1(7) is programmed 0 and does not enter into the
 *                    equation.  (There is a problem with the extra clock
 *                    bit.... )
 *
 *  See Also:
 *		set_msg() [misc.c]
 */

void check_synchmsg( Lstat lst, Lthread lth )
{
	int32			tgp;		/*  target period (ns) */
	Siop_p const	siop = (Siop_p) lst->hdware; /* pointer to chip */
	int32			tos;		/* target synchronus offset value */
	u_int32			work4;
	volatile u_int8 *msg;
	volatile u_int32 tmp;
	
	msg = (u_int8 *)&lth->msgobuf[0];		/* set a pointer to the buffer */
	
	tgp = lst->extmsgbuffer[IN_SYN_PERIOD];		/* target period */
	tos = lst->extmsgbuffer[IN_SYN_OFFSET];		/* target offset */

	lth->tgp_rec = tgp;
	lth->tos_rec = tos;

	/* if no period or no sync offset - remain async */

	if( (tgp == 0) || (tos == SYN_ASYNC ) ) {
		nthread( lst, lth );					/* remain async */
		return;
	}

	/* get sync values to use */
	
	work4 = sdtrmsg(lst, tgp, lst->ncr_clock );		

	/* if period is to small or sync offset is to large, reject message */

	if ( (work4 == SI_BADPMSG) || ( tos > lst->ncr_max_syncoffset) ){
		act_scsi_rejectmsg( lst, lth );
		rethread( lst, lth, (int32)lth->scriptstart );
		return;
	}
		
	/* set message out pointer with new sync period and offset values */

	lth->msgoutlen = 1;			/* preliminarilly, set to 1 byte */
	*msg = (lth->xferflags & SCSI_ATN) ? IDENTIFY_DISCON : IDENTIFY_NODISCON;
	
	/* cdbptr was already swaped in initthread */
	tmp  = (u_int32)lth->cdbptr;
	tmp  = SCRIPT_MADDRESS(SWAPLONG(tmp));
	*msg++ |= ((u_int8 *)tmp)[1] >> 5;		/* lun from command block */

	lth->msgoutlen += 5;			/* add in size of sync message */
	*msg++ = M_EXTENDED;   			/* extended message flag */
	*msg++ = SYNCREQ_SIZE;			/* size of the rest of the message */
	*msg++ = SYNC_REQUEST;			/* synchronus message request */
	*msg++ = work4&0xff;			/* period */
	*msg++ = tos;					/* sync offset */

	lth->tgp_final = work4&0xff;
	lth->tos_final = tos;

	lth->msgoutlen = SWAPLONG(lth->msgoutlen);
	
	/* set SCSI clock % rate */

	switch( (work4 >> 16)&0xff){
		case 1: 
			lth->sbclmask |= B_SCNTL3_SCF0;			/* sbclk to divide by 1 */
			break;
		case 2:
			lth->sbclmask |= B_SCNTL3_SCF1;			/* sbclk to divide by 1.5 */
			break;
		case 3:
			lth->sbclmask |= B_SCNTL3_SCF0 | B_SCNTL3_SCF1;	/* sbclk to divide by 2 */
			break;
	}

	/*
	 * Ultra? Fast20/40 
	 *
	 * If the period < 100ns then ULTRA is active
	 *
	*/
	
	if (lth->tgp_final < MIN_PERIOD){
		lth->sbclmask |= B_SCNTL3_ULTRA;

	}

	/*  save the bytes where the SIOP can read them out at select time */

	lth->devfield.devsync.sxfr_mask = 
		( ((work4 >> 12)&0xf) << BN_SXFER_TP) | (tos & SXFER_MO_MASK);

	lth->devfield.devsync.scntl3 = (SCSI_INC(&siop->scntl3)&SCNTL3_CCF_MASK)|lth->sbclmask;
	nthread( lst, lth );
}


#if defined(WIDE_SUPPORT)

/* Function:
 *		check_widemsg - validate targets wide capability.
 * Synopsis:
 *		void check_widemsg( Lstat lst, char *msg )
 * Description:
 *		This routine will validate the wide message returned
 * from the target device.  
 *
 *  See Also:
 *		set_msg() [misc.c]
 */

void check_widemsg( Lstat lst, Lthread lth )
{
	Siop_p const	siop = (Siop_p) lst->hdware; /* pointer to chip */

	u_int8			tmp;

	tmp = SCSI_INC(&siop->scntl3)&SCNTL3_CCF_MASK;
	lth->devfield.devsync.scntl3 = tmp | (tmp << 4);

	/*
	 * Disable and force synchronous re-negotiation if required
	 * Wide negotiation will force asynchronous.
	*/
	
	lth->synctried = 0;
	lth->devfield.devsync.sxfr_mask = 0;
	lth->sbclmask = 0;
	lth->devfield.devsync.scntl3 &= ~B_SCNTL3_ULTRA;
	
	/* Disable wide until we have a match */
	
	lth->sbclmask &= ~B_SCNTL3_WIDE;
	lth->devfield.devsync.scntl3 &= ~B_SCNTL3_WIDE;

	DPRINTF(DBIT_HW, ("\ncheck_widemsg 0x%8x", lst->extmsgbuffer[1]));

	switch(lst->extmsgbuffer[1])
	{
		case WIDE_SIZE_8BIT:
			nthread( lst, lth );
			break;
			
		case WIDE_SIZE_16BIT:	
			/* enable wide */
			lth->sbclmask |= B_SCNTL3_WIDE;
			lth->devfield.devsync.scntl3 |= B_SCNTL3_WIDE;
			nthread( lst, lth );
			break;
			
		case WIDE_SIZE_32BIT:
		default:
			act_scsi_rejectmsg( lst, lth );
			rethread( lst, lth, (int32)lth->scriptstart );

	}		
}

#endif

/*
 * hw53c810.c - NCR 53C810 SIOP specific subroutines.
 *
 * The following routines allow access to information in the SIOP
 * registers.
 *
 * Register "scratch0" for example contains phase information.
 *
*/

 /*
 *  Function:
 *      ncr_scntl3 - Get scntl3
 *
 *  Synopsis:   
 *      u_int8 ncr_scntl3( Lstat lst );
 *      
 *  Description:
 *      Get scntl3 
 *
*/
 
u_int8 ncr_scntl3( Lstat lst )
{
    Siop_p const	siop = (Siop_p) lst->hdware; /* set hardware pointer */
	u_int8			tmp;

	tmp = SCSI_INC(&siop->scntl3)&SCNTL3_CCF_MASK;

	return( tmp | (tmp << 4));

}


/*
 *  Function:
 *      ncr_ph_messd - Was script in Message In Phase
 *
 *  Synopsis:   
 *      error_code ncr_ph_messd( Lstat lst );
 *      
 *  Description:
 *      Was script in message in phase?
 *
*/
  
error_code ncr_ph_messd( Lstat lst )
{

    Siop_p const	siop = (Siop_p) lst->hdware; /* set hardware pointer */
 
    return( SCSI_INC(&siop->scratch0) == A_PH_MESSD ) ;

}
 

/*
 *	Function:
 *		ncr_ph_select - Was script in select phase
 *
 *  Synopsis:
 *		error_code ncr_ph_select( Lstat lst );
 *
 *	Description:
 *	    Was script in select phase?
 *
*/


error_code ncr_ph_select( Lstat lst )
{

	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */

	return( SCSI_INC(&siop->scratch0) == A_PH_SELECT ) ;

}


/*
 *	Function:
 *		ncr_ph_idata - Was script in data in phase
 *
 *  Synopsis:
 *		error_code ncr_idata( ( Lstat lst );
 *
 *	Description:
 *	    Was script in data in phase?
 *
*/


error_code ncr_ph_idata( Lstat lst )
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */
	return( SCSI_INC(&siop->scratch0) == A_PH_IDATA ) ;
}


/*
 *	Function:
 *		ncr_ph_odata - Was script in data out phase
 *
 *  Synopsis:
 *		error_code ncr_odata( Lstat lst  );
 *
 *	Description:
 *	    Was script in data out phase?
 *
*/

error_code ncr_ph_odata(  Lstat lst )
{
	
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */
	return( SCSI_INC(&siop->scratch0) == A_PH_ODATA ) ;

}


/*
 *	Function:
 *		ncr_ph_mess - Was script in message in phase
 *
 *  Synopsis:
 *		error_code ncr_ph_mess(  Lstat lst );
 *
 *	Description:
 *	    Was script in message in phase?
 *
*/

error_code ncr_ph_mess(  Lstat lst )
{

	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */
	
	return( (SCSI_INC(&siop->scratch3) & PH_MASK) == A_PH_MESS ) ;

}


/*
 *	Function:
 *		ncr_flush_fifo - Flush SCSI Fifo's
 *
 *  Synopsis:
 *		void ncr_flush_fifo(  Lstat lst );
 *
 *	Description:
 *	    Flush SCSI Fifo's
 *
*/

void ncr_flush_fifo(  Lstat lst )
{

	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */
	
	SCSI_OUTC(&siop->ctest3, SCSI_INC(&siop->ctest3) | B_CTEST3_CLF);	
	while(SCSI_INC(&siop->ctest3) & B_CTEST3_CLF);

}

/*
 *	Function:
 *		ncr_scsi_reset - start SCSI reset
 *
 *  Synopsis:
 *		void ncr_scsi_reset(  Lstat lst );
 *
 *	Description:
 *	    Start SCSI reset
 *
*/


void ncr_scsi_reset(  Lstat lst )
{

	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */

	/* start SCSI Bus Reset */	

	SCSI_OUTC(&siop->scntl1, SCSI_INC(&siop->scntl1) | B_SCNTL1_RST); 


}


/*
 *	Function:
 *		ncr_setpar - set parity checking
 *
 *  Synopsis:
 *		void ncr_setpar(   Lstat lst , Lthread const lth );
 *
 *	Description:
 *	    Set parity checking
 *
*/


void ncr_setpar(  Lstat lst, Lthread const lth )
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */
		
	if( lth->xferflags  & SCSI_PARITY )
		/* enable parity checking */
		SCSI_OUTC(&siop->scntl0, SCSI_INC(&siop->scntl0) | B_SCNTL0_EPC);
	else
		/* disable parity checking */
		SCSI_OUTC(&siop->scntl0, SCSI_INC(&siop->scntl0) & ~B_SCNTL0_EPC);
}


/*
 *	Function:
 *		ncr_setsync - set synchronus mode if required
 *
 *  Synopsis:
 *		void ncr_setsync( Lstat lst, Lthread const lth );
 *
 *	Description:
 *	    Set synchronus mode if required
 *
*/

void ncr_setsync( Lstat lst,  Lthread const lth )
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */
	
	if( lth->devfield.devsync.sxfr_mask & SXFER_MO_MASK ) {
		/* if running synchronus */
		if( lth->sbclmask != lst->sbclmaster ) {
			/* set for proper sync clock select */
			lst->sbclmaster = lth->sbclmask;
			lth->devfield.devsync.scntl3 =
				 (SCSI_INC(&siop->scntl3)&SCNTL3_CCF_MASK)|lth->sbclmask;
		}
	}
	else
	{
		u_int8 tmp;

		tmp = SCSI_INC(&siop->scntl3)&SCNTL3_CCF_MASK;
		lth->devfield.devsync.scntl3 = tmp | (tmp << 4);
	}
}


/*
 *	Function:
 *		ncr_rethread - Start actual rethread operation
 *
 *  Synopsis:
 *		void ncr_rethread( Lstat lst, Lthread const lth );
 *
 *	Description:
 *	    Start actual rethread operation, start scripts running
 *
*/

void ncr_rethread(  Lstat lst, Lthread const lth, int32 scriptent )
{
	u_int32			tmp;

	Siop_p const	siop = (Siop_p) lst->hdware;
		
	SCSI_OUTC(&siop->sdid, lth->devfield.devsync.scsidev);	/* set ID */
	SCSI_OUTC(&siop->sxfer, lth->devfield.devsync.sxfr_mask);
	SCSI_OUTC(&siop->scntl3, lth->devfield.devsync.scntl3);

	/* if no threads are active return to selfid */

	if( lth == &lst->logthread[lst->selfid] ) {
		lth->threadstate = TH_EMPTY;

		scriptent = \
			SCRIPT_ADDRESS(SWAPLONG((lst->act_script+Ent_WAITFORRESELECT)));

	} 

	tmp = SCRIPT_ADDRESS(SWAPLONG(((u_int32)lth)));
	SCSI_OUTL(&siop->dsa, tmp);			/* data structure address */
	lst->phythread_swap = tmp;

#if defined(OS_CACHE_FLUSH) && defined(_OS9000)

	/* Flush thread */
	cache_flush( lst, (u_int32 *)lst, (u_int32) sizeof (struct lstat));

#endif

	SCSI_OUTL(&siop->dsp, scriptent);	/* start script at this place! */
}


/*
 *	Function:
 *		void ncr_clear_sxfr_mask - Clear sync mode from sxfr mask
 *
 *  Synopsis:
 *		void ncr_clear_sxfr_mask( lth );
 *
 *	Description:
 *	    Clear sync mode from sxfr mask
 *
*/
	

void ncr_clear_sxfr_mask( Lthread const lth )
{

	/* remove sync per/off */	

	lth->devfield.devsync.sxfr_mask = CLEAR_SYNC_VALUE;	

}

/*
 *	Function:
 *		void ncr_init_sxfr_mask - Set sxfr mask to default
 *
 *  Synopsis:
 *		void ncr_init_sxfr_mask( lth );
 *
 *	Description:
 *	    Set sxfr mask to default
 *
*/

void ncr_init_sxfr_mask( Lthread const lth )
{

	/* disable halt on pe */

	lth->devfield.devsync.sxfr_mask = CLEAR_SYNC_VALUE;	

}


/*
 *	Function:
 *		ncr_async_msg - send asynchronus message.
 *
 *  Synopsis:
 *		void ncr_async_msg( lst, lth );
 *
 *	Description:
 * 		send asynchronus message.
 *
*/


void ncr_async_msg( Lstat lst, Lthread const lth )
{
	setasyn( lst, lth, MIN_PERIOD, SYN_ASYNC );
}


/*
 * Function:
 *		set_msg - set message out buffer for this command
 * Synopsis:
 *      int  set_msg( Lstat lst, Lthread lth, int scsiflags )
 * Description:
 *		Subroutine to set the message buffer for the command ahead.
 *  When it finds the synchronus flag set it will set up the buffer
 *  for synchronus negotiations.  This is done only once for a given device.
 *  It also uses the atn flag to set the identify flag.
 *
 *  The return value of this routine is true if the synchronus message is
 *  installed.  This allows the driver to 'pencil' in the select with atn
 *  flag, thus enabling the message out phase.  If SCSI_SYNCHR is requested
 *  but SCSI_ATN is not, then the ID message will be not contain disconnect.
 *
 *		This function will use the core frequency to determine the
 *	correct message bytes to send durring the synchronous negotiations.
 *  Note that this routine is critical to the correct function of the
 *  chip in synchronous operations. 
 *       The synchronous message is composed of:
 *
 *   Byte:   Value:    Description:
 *  -------  -------   --------------
 *     0       0x1     Extended Message Identifier
 *     1       0x3     Extended Message Length
 *     2       0x1     Synchronous Data Transfer Request Code
 *     3        m      Transfer period ( m times 4 nano-seconds )
 *     4        x      REQ/ACK offset
 *
 *  This relates back to the 53C810 via:
 *
 *     PERIOD = TCP * (4 + XFERP)
 *
 *   where:  PERIOD = minimum synchronous transfer period
 *           TCP    = 1 / SCSI core clock frequency
 *                    (determined by; DCNTL CF(0,1) and  SBCL SSCF(1:0))
 *
 *  See Also:
 *		check_syncmess()
 *
 *  Caveats:
 *		This routine does takes into account the FAST and WIDE operation
 *	of the bus.  See NCR publication: SCSI I/O Processor Data Manual
*/

error_code set_msg( Lstat lst, Lthread lth, u_int32 scsiflags )
{
	volatile u_int8		*msg;
	volatile u_int32	tmp;

	msg = (u_int8 *)&lth->msgobuf[0];		/* set a pointer to the buffer */
	
	lth->msgoutlen = 1;			/* preliminarilly, set to 1 byte */
	*msg = (scsiflags & SCSI_ATN) ? IDENTIFY_DISCON : IDENTIFY_NODISCON;

	/* cdbptr was already swaped in initthread */
	tmp  = (u_int32)lth->cdbptr;
	tmp  = SCRIPT_MADDRESS(SWAPLONG(tmp));
	*msg++ |= ((u_int8 *)tmp)[1] >> 5;		/* lun from command block */

	/*
	 * We must first try wide then sync.
	 *
	*/

#if defined(WIDE_SUPPORT)
	
	if( (scsiflags & SCSI_WIDE) && (lth->widetried == 0 ) && lst->ncr_wide ) {

	DPUTS(DBIT_HW, ("\nset msg wide\n"));

		/* first time the wide flag is seen, set to negotiate */
		lth->msgoutlen += 4;			/* add in size of wide message */
		*msg++ = M_EXTENDED;   			/* extended message flag */
		*msg++ = WIDEREQ_SIZE;			/* size of the rest of the message */
		*msg++ = WIDE_REQUEST;			/* wide message request */
		*msg++ = WIDE_SIZE_16BIT;		/* Wide size = 16Bit */

		lth->widetried |= NEGOTIATE | WIDETRIED;
 		lth->msgoutlen = SWAPLONG(lth->msgoutlen);
		return FALSE; /* we allways return FALSE */

	} 

#endif	

	/*
	 * Try SYNC
	*/

	if( (scsiflags & SCSI_SYNCHR) && (lth->synctried == 0 ) ) {

    	DPUTS(DBIT_HW, ("\nset msg sync\n"));

		/* first time the synchronus flag is seen, set to negotiate */
		lth->msgoutlen += 5;			/* add in size of sync message */
		*msg++ = M_EXTENDED;   			/* extended message flag */
		*msg++ = SYNCREQ_SIZE;			/* size of the rest of the message */
		*msg++ = SYNC_REQUEST;			/* synchronus message request */

		/* Use Ultra Support? */

		if( lth->xferflags & SCSI_ULTRA ){

    	DPUTS(DBIT_HW, ("\nset msg ultra\n"));

			*msg++ = lst->ncr_min_period;	/* minimum period */
	 		*msg++ = lst->ncr_max_syncoffset; /* max that siop will allow */
		} else

		/* SCSI I Timing desired? */
		
		if( lth->xferflags & SCSI_SLOW ){		

			*msg++ = 50;					/* minimum period (200 ns, by spec) */
	 		*msg++ = MAX_SYNCOFFSET;		/* max that siop will allow */

		}

		/* FAST SCSI II Timing? */
		else
		{		
			*msg++ = MIN_PERIOD;			/* minimum period (100 ns, by spec) */
	 		*msg++ = MAX_SYNCOFFSET;		/* max that siop will allow */

		}
			*msg++ = lst->ncr_min_period;	/* minimum period */
	 		*msg++ = lst->ncr_max_syncoffset; /* max that siop will allow */

		lth->synctried |= NEGOTIATE | SYNCTRIED;
 		lth->msgoutlen = SWAPLONG(lth->msgoutlen);
		return TRUE;

	} else {
		/* not time to negotiate */
		lth->synctried &= ~NEGOTIATE;		/* insure not stale */
		lth->msgoutlen = SWAPLONG(lth->msgoutlen);
		return FALSE;
	}
}

	

/*                                                                           
 *    Function:
 *		sdtrmsg
 *    This routine is called to generate a period message to respond to a    
 *    'synchronous data transfer request'  message.   Additionally,  this    
 *    routine will generate the period value to be installed in the SXFER    
 *    register and the clock divider to be installed in the SBCL register    
 *    to achieve the transfer period for the period message generated.       
 *                                                                           
 *    Entry: periodmsg  = transfer period this code will try to match        
 *           clk        = SCSI clock frequency                               
 *                                                                           
 *    Exit:  response = bits 31-21 -reserved-                                
 *                      bits 23-16 -clock divider code-                      
 *                      bits 15-08 -SXFER period code-                       
 *                      bits 07-00 -sdtr msg response period value-          
 *                                                                           
 * 
 *                   PERIOD = TCP * (4 + XFERP)                              
 *                   Pmsg = PERIOD / (4 * .000000001)                        
 *                   TCP = 1 / clockspeed                                    
 *                   Pmsg * .000000004 = (1 / clockspeed)(4 + XFERP)         
 *                   Pmsg = (1 / clockspeed)(4 + XFERP) / .000000004         
 *                   Pmsg = (1000000000 / clockspeed)(4 + XFERP) / 4         
 *                                                                           
 *     where-   PERIOD      is the minimum synchronous transfer period       
 *                          period when sending or receiving SCSI data.      
 *              TCP         is the total clock period of the SCSI core       
 *                          clock.                                           
 *              XFERP       is the sync tranfer period code used by the      
 *                          NCR53C810/825 (range of 0-7).                    
 *              Pmsg        is the period byte in the sdtr message.          
 *              clockspeed  is the SCSI core clock frequency which           
 *                          may be bus clock divided by 1, 1.5, or 2.        
 *                                                                            
 *    NOTE2: this function can be used to determine the clock code values    
 *           needed for an arbitration agreement initiated by us.            
 *                                                                           
*/

u_int32 sdtrmsg(Lstat lst, u_int32 periodmsg, u_int32 clk)
{
	u_int32		clkspeed =  clk * 1000000;
	u_int32		coreclock;          /* SCSI core clock speed */
	u_int32		divider;            /* clock divider code (1-3) */
	u_int32		xferp;              /* right justified sxfer code (0-7) */
	u_int32		pmsg;               /* current period message */
	u_int32		response;           /* return msg, divider, period code */

	/*
	 * MAX bounds of the input value cannot exceed 0xFA (1Mb/s)
	 * due to programming limitations of the SIOP. MIN bounds
	 * of the input value cannot be less than 0x19 (10Mb/s)
	 * because of the SCSI specification limitation.
	 * a value of 0xfa is equivilent to 1Mbyte/sec; this value 
	 * is much slower than asynchronous data transfers (~2Mb/s).
	 */


	if ((periodmsg > MAXPMSG) || (periodmsg < lst->ncr_min_period))
		return (SI_BADPMSG);

	/*
	 * response is now used as a continuation flag. since we got here
	 * the input message value was within the valid range. the first
	 * message generated which is equal to or greater than the input
	 * message value will become the LSB value of response. each subsequent
	 * message byte generated which falls between the input value and
	 * the LSB of response will replace response. the init value is 0xFF.
	 */

	response = SI_BADPMSG;

	/*
	 * loop through clock divider values. this value will be returned
	 * in the same bit positions as it would be used in the SBCL reg.
	 *
	 * clock divider code to determine SCSI code clock speed-
	 *    1 - data transfer clk = (local bus clk speed) / 1.0
	 *              clkspeed * (2/2) = clkspeed
	 *    2 - data transfer clk = (local bus clk speed) / 1.5
	 *              clkspeed * (2/3) = clkspeed / 1.5
	 *    3 - data transfer clk = (local bus clk speed) / 2.0
	 *              clkspeed * (2/4) = clkspeed / 2
	 */

	for ( divider = 1; divider < 4; divider++)
	{
		coreclock = (clkspeed * 2) / (divider + 1);
		/*
		 * loop through sync transfer period code values. find first
		 * sdtr period byte value which is greater than the input
		 * sdtr period byte. compare with previous value and save
		 * if it is better. xferp is saved in the bit positions as
		 * it would be in the SXFER register.
		 */
		for ( xferp = 1; xferp < 8; xferp++)
		{
			pmsg = ((NORMALIZE / coreclock) * (4 + xferp)) / 4;
			if (pmsg >= periodmsg)
			{
			   if ((response & 0xFF) > pmsg)
			   {
			      response = divider << 16;
			      response |= xferp << 12;
			      response |= pmsg;
			   }
			   break;
			}
		}
	}

	return (response);
}

/*
 * DMA bus error.  This routine seems broken because it does not restart
 * the last operation that died.  Thus, we now just use dma_buserror().
*/

void ncr_dma_buserror( Lstat lst )
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */		
	Lthread lth = lst->phythread;	/* pointer to current physical thread */

	save_state(lst);					/* need to preserve state here */
	lth->chiperror = BUSY;				/* say BUSY so retry will happen */		

	/*
	 * Clear All fifo's
	*/

 	SCSI_OUTC( &siop->ctest3, SCSI_INC( &siop->ctest3 ) | B_CTEST3_CLF ); 
 	SCSI_OUTC( &siop->stest3, SCSI_INC( &siop->stest3 ) | B_STEST3_CSF );  	

}

/*
 * DMA ILLEGAL SCRIPT.  this routine seems broken because it does not
 * restart the last operation that died.  We now just use dma_illscript().
*/

void ncr_dma_illscript( Lstat lst )
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */		

	Lthread lth = lst->phythread;	/* pointer to current physical thread */

	save_state(lst);				/* need to preserve state here */

	lth->chiperror = BUSY;				/* say BUSY so retry will happen */


	/*
	 * Clear All fifo's
	*/

 	SCSI_OUTC( &siop->ctest3, SCSI_INC( &siop->ctest3 ) | B_CTEST3_CLF ); 
 	SCSI_OUTC( &siop->stest3, SCSI_INC( &siop->stest3 ) | B_STEST3_CSF );  	

}


