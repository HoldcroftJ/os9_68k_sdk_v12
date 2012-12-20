/*
 * hw53c710.c - NCR 53C710 SCSI SCRIPT and SIOP Hardware specific routines
 */
 
/*--------------------------------------------------------------------------,
|                                                                           |
|           Copyright 1995-1997 by Microware Systems Corporation            |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, distribution, or creation of derivative works |
|  in any form to any party other than the licensee is strictly prohibited, |
|  unless expressly authorized in writing by Microware Systems Corporation. |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
| 01 96/08/03 Added edition history.                                  rak   |
|             ---- OS-9000/x86 V2.1 Released ----                           |
| 02 96/09/20 Added Cache Flush support for OS9000.                   GbG   |
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
| 03 97/04/21 Added SCSI_SLOW option.                                 GbG   |
|             ---- OS-9000/PPC V2.1.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
| 04 97/07/18 Changed save_state to not use dnad register.            GbG   |
| 05 97/11/17 Added address translation (LCLMEM), tracing                   |
|             (TRACE_SCRIPT), and no acknowledge (NOACK) options.     gdb   |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- SCSI Common Sub-Component v1.0 Released ----             |
|             $$                    <RELEASE_INFO>                     $$   |
`--------------------------------------------------------------------------*/
 
#define _HW53C710_C_

#include <scsisiop.h>
#include <v53c710.c>
#include <ncrsiop.h>

#if defined(OS_CACHE_FLUSH) && defined(_OS9000)
#include	<cache.h>
void cache_flush(Lstat lst, u_int32 *addr, u_int32 size);
#endif

#if defined (__STDC__) || defined (_ANSI_EXT)
Lthread find_lth( Lstat lst );
#else 
Lthread find_lth();
#endif

/*
 *	Function:
 *		relocate - patch script elements as required
 *
 *  Synopsis:
 *		int relocate( Lstat lst );
 *
 *	Description:
 *		Patches script. The script file "v53c710.c" is produced with
 *		the NCR script file compiler
 *
 */

#if defined (__STDC__) || defined (_ANSI_EXT)
error_code relocate( Lstat lst)
#else
error_code relocate(lst)
Lstat lst;
#endif
{
	Lthread lth;
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */
	int i;
	unsigned long  *script;         /* pointer to SCSI SCRIPTs */
	int loop;
	unsigned long *dest,*src;

	/* if the script has grown too large, complain! */	
	if( (loop = sizeof(SCRIPT)) > MAXSCRIPT )
		return ~SUCCESS;

	loop /=4;								/* bytes to longwords */

	src = (unsigned long *) SCRIPT;			/* set source and destination */
	dest = (unsigned long *) &lst->act_script[0];
	while( --loop >= 0 )
		*dest++ = *src++;					/* move it */

	script = (unsigned long *) &lst->act_script[0];
	
    /* Patch script to update external identifers */
    for (i = 0; i < sizeof(E_armsg_mpe_Used) / 4; i++) {
	script[E_armsg_mpe_Used[i]] =
	  SCRIPT_ADDRESS((unsigned long) &lst->armsg.msg.mpe);
    }
    for (i = 0; i < sizeof(E_armsg_reject_Used) / 4; i++) {
	script[E_armsg_reject_Used[i]] =
	  SCRIPT_ADDRESS((unsigned long) &lst->armsg.msg.reject);
    }
    for (i = 0; i < sizeof(E_armsg_abort_Used) / 4; i++) {
	script[E_armsg_abort_Used[i]] = 
	  SCRIPT_ADDRESS((unsigned long) &lst->armsg.msg.abort);
    }
    for (i = 0; i < sizeof(E_bitbucket_Used) / 4; i++) {
	script[E_bitbucket_Used[i]] =
	  SCRIPT_ADDRESS((unsigned long) &lst->selflg.sipflag[0]);
    }
    for (i = 0; i < sizeof(E_statadr_Used) / 4; i++) {
	script[E_statadr_Used[i]] =
	  SCRIPT_ADDRESS((unsigned long) &lst->selflg.sipflag[0]);
    }
    for (i = 0; i < sizeof(E_extcntptr1_Used) / 4; i++) {
	script[E_extcntptr1_Used[i]] =
	  SCRIPT_ADDRESS((unsigned long) &lst->extcnt.mcnt.cnt[2]);
    }
    for (i = 0; i < sizeof(E_extcntptr_Used) / 4; i++) {
	script[E_extcntptr_Used[i]] =
	  SCRIPT_ADDRESS((unsigned long) &lst->extcnt.mcnt.cnt[3]);
    }
    for (i = 0; i < sizeof(E_extmframeptr_Used) / 4; i++) {
	script[E_extmframeptr_Used[i]] =
	  SCRIPT_ADDRESS((unsigned long) &lst->extmsgfram);
    }
    for (i = 0; i < sizeof(E_lcrcid_idmsg_Used) / 4; i++) {
	script[E_lcrcid_idmsg_Used[i]] =
	  SCRIPT_ADDRESS((unsigned long) &lst->lcrcs.lcrcid.idmsg);
    }
    for (i = 0; i < sizeof(E_lcrcid_tid_Used) / 4; i++) {
	script[E_lcrcid_tid_Used[i]] =
	  SCRIPT_ADDRESS((unsigned long) &lst->lcrcs.lcrcid.tid);
    }
#if !defined(LCLMEM)
    for (i = 0; i < sizeof(E_phythread_Used) / 4; i++) {
	script[E_phythread_Used[i]] = (unsigned long) &lst->phythread;
    }
#else
    lst->phythread_local = (u_int32)SCRIPT_ADDRESS(lst->phythread);
    for (i = 0; i < sizeof(E_phythread_Used) / 4; i++) {
      script[E_phythread_Used[i]] =
	SCRIPT_ADDRESS((unsigned long) &lst->phythread_local);
    }
#endif

	/* fill in NCR addresses */
	
    for (i = 0; i < sizeof(E_ncrdsa_Used) / 4; i++) {
	script[E_ncrdsa_Used[i]] =
	  SCRIPT_ADDRESS((unsigned long) &siop->dsa);
    }

    for (i = 0; i < sizeof(E_ncrlcrc_Used) / 4; i++) {
	script[E_ncrlcrc_Used[i]] =
	  SCRIPT_ADDRESS(((unsigned long) &siop->lcrc));
    }

    for (i = 0; i < sizeof(E_ncrscratch0_Used) / 4; i++) {
	script[E_ncrscratch0_Used[i]] =
	  SCRIPT_ADDRESS((unsigned long) &siop->scratch0);
    }

    for (i = 0; i < sizeof(E_ncrscratch1_Used) / 4; i++) {
	script[E_ncrscratch1_Used[i]] = 
	  SCRIPT_ADDRESS((unsigned long) &siop->scratch1);
    }

    for (i = 0; i < sizeof(E_ncrscratch3_Used) / 4; i++) {
	script[E_ncrscratch3_Used[i]] =
	  SCRIPT_ADDRESS((unsigned long) &siop->scratch3);
    }
   
	coherent();					/* insure memory update */
	
	/* and finally,  cache the addresses of the various select entry points
	 *  into the script to streamline testing within the driver 
	 */
	lst->snatnaddr = (int)&lst->act_script[Ent_SELECTNOATN];
	lst->swatnaddr = (int)&lst->act_script[Ent_SELECTATN];
	
	return SUCCESS;							/* no problems! */
}


/*
 * hw53c710.c - NCR 53C710 SIOP specific subroutines.
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void act_scsi_select( Lstat lst, Lthread const lth )
#else
void act_scsi_select( lst, lth )
Lstat lst;						/* low level static storage */
Lthread lth;					/* logical thread to activate */
#endif
{
	
	/* condition script entry and identify message based on ATN flag */		
	if( lth->xferflags & SCSI_ATN )
		lth->scriptstart = &lst->act_script[Ent_SELECTATN];
	else
		lth->scriptstart = &lst->act_script[Ent_SELECTNOATN];
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void scsi_rejectmsg( Lstat lst, Lthread const lth )
#else
void scsi_rejectmsg( lst, lth )
Lstat lst;						/* low level static storage */
Lthread lth;					/* logical thread to activate */
#endif
{
	rethread( lst, lth, (int32)&lst->act_script[Ent_REJECTMESSAGE] );
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void act_scsi_rejectmsg( Lstat lst, Lthread const lth )
#else
void act_scsi_rejectmsg( lst, lth )
Lstat lst;						/* low level static storage */
Lthread lth;					/* logical thread to activate */
#endif
{
	lth->scriptstart = &lst->act_script[Ent_REJECTMESSAGE];
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void scsi_idle( Lstat lst, Lthread const lth )
#else
void scsi_idle( lst, lth )
Lstat lst;						/* low level static storage */
Lthread lth;					/* logical thread to activate */
#endif
{
	rethread( lst, lth, (int32) &lst->act_script[Ent_WHICHPHASE]);
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void act_scsi_idle( Lstat lst, Lthread const lth )
#else
void act_scsi_idle( lst, lth )
Lstat lst;						/* low level static storage */
Lthread lth;					/* logical thread to activate */
#endif
{
	lth->scriptstart = &lst->act_script[Ent_WHICHPHASE];
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
 *		Place the SCSI BUS in BUS FREE state.
 *		This routine runs in asynchronous mode.
 *		Self-thread entry is not allowed and
 *		is re-routed to reschedule.
 *
 */

#if defined (__STDC__) || defined (_ANSI_EXT)
void scsi_free( Lstat lst, Lthread const lth )
#else
void scsi_free( lst, lth )
Lstat lst;						/* low level static storage */
Lthread lth;					/* logical thread to activate */
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */

	if( lth == &lst->logthread[lst->selfid] ) {
		lth->threadstate = TH_EMPTY;
		reschedule( lst );	
	} else 	{

		coherent();					/* insure memory update */

		lst->sbclmaster = USEDCNTL;	
		outc(&siop->sbcl, lst->sbclmaster);	/* use dcntl */

		/* disable parity checking */
		outc(&siop->scntl0, inc(&siop->scntl0) & ~B_EPC);	
		outc(&siop->sdid, lth->devfield.devsync.scsidev);		/* set ID */

		outc(&siop->sxfer, B_DHP);	/* async only */
		outl(&siop->dsa, SCRIPT_ADDRESS(lth));	/* data structure address */

#if defined(OS_CACHE_FLUSH) && defined(_OS9000)

	/* Flush thread */
	cache_flush( lst, (u_int32 *)lst, (u_int32) sizeof (struct lstat));

#endif
		/* start script at this place! */
		outl(&siop->dsp, SCRIPT_ADDRESS(&lst->act_script[Ent_FREEIT]));
	}
#if defined(TRACE_SCRIPT)
	/* start trace script */
	outc( &siop->dcntl, inc( &siop->dcntl ) | B_ESTD);    
#endif
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void act_scsi_free( Lstat lst, Lthread const lth )
#else
void act_scsi_free( lst, lth )
Lstat lst;						/* low level static storage */
Lthread lth;					/* logical thread to activate */
#endif
{
	lth->scriptstart = &lst->act_script[Ent_FREEIT];
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void scsi_pemsg( Lstat lst, Lthread const lth )
#else
void scsi_pemsg( lst, lth )
Lstat lst;						/* low level static storage */
Lthread lth;					/* logical thread to activate */
#endif
{
	rethread( lst, lth, (int32) &lst->act_script[Ent_PEMESSAGE]);
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void scsi_smsg( Lstat lst, Lthread const lth )
#else
void scsi_smsg( lst, lth )
Lstat lst;						/* low level static storage */
Lthread lth;					/* logical thread to activate */
#endif
{
	rethread(lst, lth, (int32)&lst->act_script[Ent_SENDMESSAGE]);
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void act_scsi_waitrsel( Lstat lst, Lthread const lth )
#else
void act_scsi_waitrsel( lst, lth )
Lstat lst;						/* low level static storage */
Lthread lth;					/* logical thread to activate */
#endif
{
	lth->scriptstart = &lst->act_script[Ent_WAITFORRESELECT];	
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void nthread( Lstat lst, Lthread const lth )
#else
void nthread( lst, lth )
Lstat lst;						/* low level static storage */
Lthread lth;					/* logical thread to activate */
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */
	rethread( lst, lth, inl(&siop->dsp) );
}


/*
 * hw53c710.c - NCR 53C710 SIOP specific subroutines.
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

#if defined (__STDC__) || defined (_ANSI_EXT)
error_code ncr_reset( Lstat lst )
#else
error_code ncr_reset( lst )
Lstat lst;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */	
	int32 clk,temp;
	
	/*  At power-up, the 53c710 is not conditioned to generate ACK when
	 *  accessed.  Until we set the bit to enable ACK, the registers
	 *  cannot be accessed.  This must be a WRITE operation.
	 */

#ifdef NOACK
    outc( &siop->dcntl, 0);     /* Enable ACK: set up as slave device */
#else
    outc( &siop->dcntl, B_EA );     /* Enable ACK: set up as slave device */
#endif

	/* check DMA FIFO EMPTY FLAGS to see if hardware is working */
	
	if ( (inc(&siop->dstat) != 0x80) || (inc(&siop->ctest1) != 0xf0))
#if defined (__STDC__) 
		return EOS_HARDWARE;		/* Not NCR 710 Device */
#else
		return E_HARDWARE;			/* Not NCR 710 Device */
#endif

#ifdef RSTCODE
	/* under normal circumstances,  durring boot a SCSI Reset has been
	 *  performed by the "sysinit" code.  If there is an add-on board
	 *  that contains this chip nad no "init extension" that covers the
	 *  performs the reset,  then this should probably be enabled by 
	 *  defining RSTCODE in the makefile for the booter.
	 */
	SCSI_Reset( lst );			/*  This is a SCSI Bus Reset */
#endif /* RSTCODE */
	
	/* perform a soft reset of the siop  Note: NOT a SCSI BUS RESET! */
 	outc( &siop->istat, inc( &siop->istat ) | B_SRST ); /* set... */

	outc( &siop->istat, 0 ); 	                /* ..and clear it */
	outc( &siop->scid, (1 << lst->selfid) );    /* set SCSI ID of the SIOP */

	/* ensure 710 mode!!! */
    outc( &siop->dcntl, inc( &siop->dcntl ) | B_COM);

    outc( &siop->scntl0, B_EPG);				/* generate parity */

	/* don't halt on parrity error */
    outc( &siop->sxfer, inc( &siop->sxfer ) | B_DHP );
     	
	/*  Enable SIOP interrupts on:
	 *      Phase Mismatch             SCSI Reset
	 *      SCSI Gross Error           SCSI Bus Select Timeout
	 *      Unexpected Disconnect
	 *      Parity Error
	 */
	outc( &siop->sien, (B_PM | B_SGE | B_UDC | B_RRST | B_STO | B_PAR) );
	
	/*  Enable DMA interrupts on:
	 *      Bus Fault
	 *      Illegal Instruction Detected
     *		Script Interrupt Instruction
	 */
#if defined(TRACE_SCRIPT)
	outc( &siop->dien, (B_EBF | B_ESIR | B_EILI | B_SSI) );
#else
	outc( &siop->dien, (B_EBF | B_ESIR | B_EILI) );
#endif
	outc( &siop->scntl1, B_ESR);            /* Enable reselect */

	temp = SCLK_FREQ;	/* get the core-clock speed for the SIOP */
	if( (temp > 66)	|| (temp < 16) )
#if defined (__STDC__) 
		return EOS_PARAM;			/* clock speed is not rational */
#else
		return E_PARAM;				/* clock speed is not rational */
#endif

	/* arrange Divisor based on clock speed and inform chip */		
	if( temp > 50 ) {
		lst->coreclock = temp / 3;
		clk = B_CF0 | B_CF1;
	} else if (temp > 37 ) {
		lst->coreclock = temp / 2;
		clk = 0;
	} else if (temp > 25 ) {
		lst->coreclock = temp * 2 / 3;
		clk = B_CF0;
	} else {
		lst->coreclock = temp;
		clk = B_CF1;
	}

	/* merge the clock setting with the previously
	 * set bits.
	 */
	outc( &siop->dcntl, inc(&siop->dcntl) | clk);
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

#if defined (__STDC__) || defined (_ANSI_EXT)
void ncr_start( Lstat lst )
#else
void ncr_start( lst )
Lstat lst;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */			
	Lthread lth;
	
	/* start executing the script at the IDLE entry point */
	lth = &lst->logthread[lst->selfid];		/* address of the IDLE thread */
	lth->threadstate = TH_EMPTY;
	lst->phythread = lth;

#if defined(LCLMEM)
    lst->phythread_local = (u_int32)SCRIPT_ADDRESS(lst->phythread);
#endif
    outl( &siop->dsa, SCRIPT_ADDRESS(lth));

#if defined(OS_CACHE_FLUSH) && defined(_OS9000)

	/* Flush thread */
	cache_flush(lst, (u_int32 *)lst, (u_int32) sizeof (struct lstat));

#endif

    outl( &siop->dsp, SCRIPT_ADDRESS(lth->scriptstart));
#if defined(TRACE_SCRIPT)
    /* start trace script */
    outc( &siop->dcntl, inc( &siop->dcntl ) | B_COM | B_ESTD);
#endif
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

#if defined (__STDC__) || defined (_ANSI_EXT)
void ncr_stop( Lstat lst )
#else
void ncr_stop( lst )
Lstat lst;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */	
	
	/* perform a soft reset of the siop */

	/* set reset....  */
	outc( &siop->istat, inc( &siop->istat  ) | B_SRST );

	/* ..and clear it */
	outc( &siop->istat, inc( &siop->istat ) & ~B_SRST );
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

#if defined (__STDC__) || defined (_ANSI_EXT)
void ncr_SCSI_reset( Lstat lst )
#else
void ncr_SCSI_reset( lst )
Lstat lst;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */	
	u_int32 bitbucket;				/* garbage holder */
	u_int32 sleepval;				/* holder for sleep value */
	signal_code signal;				/* holder for signal code */
		
	/* Assert SCSI Reset */
	outc( &siop->scntl1, inc( &siop->scntl1 ) | B_RST );

#if defined(_OSK)
	_f_sleep( ONE_TICK );							/* delay */
#else
	sleepval = ONE_TICK;
	_os_sleep( &sleepval, &signal);		
#endif

	/* Deassert SCSI Reset */
	outc( &siop->scntl1, inc( &siop->scntl1 ) & ~B_RST );

#if defined(_OSK)
	_f_sleep( 0x80000000 | 1280 );					/* 5 second delay */
#else
	sleepval = FIVE_SECONDS;
	_os_sleep( &sleepval, &signal);		
#endif

	bitbucket= inl( &siop->sstat2 );
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void ncr_kick( Lstat lst )
#else
void ncr_kick( lst )
Lstat lst;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */		

	outc( &siop->istat, inc(&siop->istat) | B_SSIGP );
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
 *		rethread() [script.c]
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void save_state( Lstat lst )
#else
void save_state( lst )
Lstat lst;				/* low level static storage */
#endif
{
	Siop_p const	siop = (Siop_p) lst->hdware; /* set hardware pointer */		
	Lthread			lth;			/* logical thread  */
	u_int8			*cdpt;			/* image of the data pointer register */
	u_int32			fresidual = 0;	/* number of residual bytes (calculated) */
	u_int32			ccnt;			/* image of the data byte count register */
	u_int32			dataptr, datacount;
		
	lth = lst->phythread;			/* set pointer to physical thread */
#if defined(LCLMEM)
    lst->phythread_local = (u_int32)SCRIPT_ADDRESS(lst->phythread);
#endif

	lth->phaseflg = inl(&siop->scratch3 );	/* preserve phase flag */

	/* save the script pointer for reference (reentry:= dsp - 8) */
	lth->scriptstart = 
	  (u_int8 *) SCRIPT_MADDRESS((u_int8 *)(inl(&siop->dsp) - 8));

	ccnt = inl(&siop->dcmd) & DBC_MSK;      /* DMA count register */

	datacount = lth->datacount;	
	dataptr = (u_int32)lth->dataptr;
	dataptr += datacount-ccnt;

	/* must perform check for residual data in the various FIFOs.
	 *   This is direction dependent and transfer mode dependent.
	 */

	if( ncr_ph_odata(lst) )  {
		/* determine the amount of residual data in the DMA FIFO
		 *  here according to the algorithm provided by NCR.
		 */

		fresidual = ((inc(&siop->dfifo) & DFIFO_MSK) -
										(ccnt & DFIFO_MSK) ) & RESID_OP;

		/* for both sync and async - check output data register for data */
		fresidual += ( (inc(&siop->sstat1)) & B_ORF) ? 1 : 0;

		/* check output data latch as well */
		fresidual += ( (inc(&siop->sstat1)) & B_OLF) ? 1 : 0;

		/* update the logical thread data information for resumption */

		dataptr -= fresidual;
		datacount = ccnt + fresidual;

		lth->dataptr = (u_int8 *)SCRIPT_ADDRESS(dataptr);
		lth->datacount = datacount;

	} else {

		cdpt = (u_int8 *)dataptr;
		
		if( (inc(&siop->sstat1)) & B_ILF ) {
			*cdpt = inc(&siop->sidl);			/* read sidl */
			fresidual++;
		}

		dataptr += fresidual;
		datacount = ccnt - fresidual;

		lth->dataptr = (u_int8 *)dataptr;
		lth->datacount = datacount;

	}

	/* clear the SCSI FIFO */

	outc(&siop->ctest8, inc(&siop->ctest8) | B_CLF);
	while(inc(&siop->ctest8) & B_CLF);

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


#if defined (__STDC__) || defined (_ANSI_EXT)
error_code ncr_irq_sort( Lstat lst )
#else
error_code ncr_irq_sort( lst )
Lstat lst;				/* low level static storage */
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */			
	volatile u_int32 intstat;			/* interrupt status */
	volatile u_int8 istat;				/* copy of the istat register */
	volatile u_int8 dstat;				/* copy of DMA status register */
	volatile u_int8 sstat;				/* copy of ISR status register */
	
	istat =inc(&siop->istat);
	
	if( (istat & (B_SSIP | B_SDIP))  == 0 ) {
		#ifdef STATS
			dblog( lst, NOTOURINT );
		#endif

		#if defined (_OS9000)
		#if defined (__STDC__) 
			return EOS_NOTME;
		#else
			return E_NOTME;
		#endif
		#else
			return 1;
		#endif

	}
	
	do {	 /* do all interrupts */

		#ifdef STATS	
			dblog(lst,istat);
		#endif

		/* process an SIOP interrupt */

		#ifdef STATS
			lst->ninterr++;	/* keep track of the number of interrupts */
		#endif

		intstat = inl(&siop->sstat2);	/* read entire status as long */

		if (istat & B_SSIP){ /* SCSI INTERRUPT PENDING */	

			sstat = (intstat >> 8) & 0xFF;

			if (sstat & B_PAR){ /* parity error detected */
			#ifdef STATS
				dblog( lst, SPARITY );
			#endif
				siop_par(lst);
			}

			if (sstat &	B_RRST){ /* SCSI Bus Reset received */
				#ifdef STATS
					dblog( lst, SRESET );
				#endif
					siop_rrst(lst);
			}

			if (sstat & B_UDC){ /* Unexpected disconnect */
				#ifdef STATS
					dblog( lst, SUDC );
				#endif
				siop_udc(lst);
			}

			if (sstat & B_SGE){ /* SCSI Gross Error */
				#ifdef STATS
					dblog( lst, SGE );
				#endif
				siop_sge(lst);
			}

			if (sstat & B_STO){ /* selection timeout */
				#ifdef STATS
					dblog( lst, SELTO );
				#endif
				outc(&siop->ctest7, inc(&siop->ctest7) | B_DISST );	
				siop_sto(lst);
			}
			if (sstat & B_PM){ /* phase mismatch */
				siop_pm( lst);
			}
		}

		else if (istat & B_SDIP){ /* DMA INTERRUPT PENDING */

			dstat = intstat & 0xFF;	/* low byte is DMA status */
	
			if (dstat & B_IID){		/* Illegal instruction */
				#ifdef STATS
					dblog( lst, ILLIS );
				#endif
					dma_illscript( lst );
			}

			if (dstat & B_WTD){		/* DMA watchdog timer */
				#ifdef STATS
					dblog( lst, DMAWD );
				#endif
					dma_watchdog( lst );
				}

			if (dstat & B_SIR){		/* SCSI SCRIPT int instruction seen */
				routeon_dsp( lst );
			}

			if (dstat & B_BF){		/* DMA bus error */
				#ifdef STATS
					dblog( lst, DMABE );
				#endif
				dma_buserror( lst );	/* dma bus error detected */
			}
		}

	} while ((istat = (inc(&siop->istat) & (B_SSIP | B_SDIP))));

	/* save last interrupt stats */
	lst->phythread->lthresv = intstat;

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
#if defined (__STDC__) || defined (_ANSI_EXT) 
void routeon_dsp( Lstat lst )
#else 
void routeon_dsp( lst )
Lstat lst;
#endif
{
	u_int32 dspvalue;
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */					
	Lthread lth;					/* logical thread  */

	lth = lst->phythread;
	dspvalue = (u_int32) inl(&siop->dsps);
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

		case  A_UNKNPH:			/* unidentified next phase! */
			bomblog( lst, lth, A_UNKNPH );
	        lth->chiperror = BUSY;              /* say BUSY so retry will happen */
			scsi_free(lst, lth);
			break;

		case  A_NOTMSGI:						
			lst->phythread = lth = find_lth(lst);
#if defined(LCLMEM)
			lst->phythread_local = (u_int32)SCRIPT_ADDRESS(lst->phythread);
#endif
			rethread( lst, lth, (int32)&lst->act_script[Ent_ACKANYMSG] );
			break;

		case A_BADMSG:			/* unidentified message in received */
			scripti_badmsg( lst, lth );
			break;
	}
}

#if defined (__STDC__) || defined (_ANSI_EXT)
Lthread find_lth( Lstat lst )
#else 
Lthread find_lth( lst )
Lstat lst;
#endif
{
	u_int8 shifter;					/* gets the SCSI ID bit */
	int32 id;						/* accumulates to SCSI ID of reselector */
	shifter = lst->lcrcs.lcrcid.tid ^ (1 << lst->selfid);
	id = 0;							/* ls bit is ID 0 */
	while( !(shifter & 0x1) ) {
			shifter >>= 1;				/* slide down one */
			id++;
	}
	return ( (Lthread)&lst->logthread[id]);		/* pointer to current */
}

/* Function:
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
#if defined (__STDC__) || defined (_ANSI_EXT)
void check_synchmsg( Lstat lst, Lthread lth )
#else 
void check_synchmsg( lst, lth )
Lstat lst;				/* static storage */
Lthread lth;
#endif
{
	int32 tcp;					/*  1 / core clock (ns) */
	int32 tgp;					/*  target period (ns) */
	int32 temp;
	Siop_p const siop = (Siop_p) lst->hdware; /* pointer to chip */
	int32 tos;					/* target synchronus offset value */
	
	tgp = lst->extmsgbuffer[IN_SYN_PERIOD];		/* target period */
	tos = lst->extmsgbuffer[IN_SYN_OFFSET];		/* target offset */

#ifdef STATS
	lst->syn_period = tgp;	/* record target period */
	lst->syn_offset = tos;	/* record target offset */
#endif	
	ncr_clear_sxfr_mask( lth );		/* clear period/offset */

	if( (tgp == 0) || (tos == SYN_ASYNC ) ) {
		nthread( lst, lth );					/* remain async */
		return;
	}
		
	tgp *= 4;		/* get the targets transfer period in nsec */
		
	/*
	 *  The SCSI II specification (which is unapproved) states clearly
	 *  that the following is the case  ( section 4.8 ):
	 *      if the synchronus period is >= 200 ns the device uses
	 *          SCSI I synchronus timing.
	 *      if the synchronus period is 100ns < period < 200 ns then
	 *          the device uses SCSI II synchronus timing.
	 *  Due to this very frank and clear statement, we will adhear to
	 *  this specification.  If a drive fails,  it's SEP.
	 */

	 if( tgp >= 200 ) {
	 	/* assume SCSI I timings, and program the chip for that rate */
	 	tcp = 100000 / (lst->coreclock * 100 );
		lth->sbclmask = USEDCNTL;			/* use dcntl */	 	
#ifdef STATS
		lst->scsi_timing = 1;	/* SCSI timing */
#endif	

	} else {
		/* set up for SCSI 2 (Fast) synchronus operation */
#ifdef STATS
		lst->scsi_timing = 2;	/* SCSI timing */
#endif	

		if (SCLK_FREQ > 50){
			lth->sbclmask = M_DIV15;			/* sbclk to divide by 1.5 */
			tcp = 150000 / (SCLK_FREQ * 100 );	/* core period in nsec */
		}
		else {
			lth->sbclmask = M_DIV1;				/* sbclk to divide by 1 */
			tcp = 100000 / (SCLK_FREQ * 100 );	/* core period in nsec */
		}
	}

	/* 
	 *  The SCSI specification states that the synchronus transfer
	 *  must be honnored by both parties.  The transfer may proceed
	 *  slower than the slowest device,  but never faster.  Here we
	 *  will ensure that we do not violate the specification.
	 */
	if( tgp % tcp )
		tgp += tcp;		/* rounds up for integral clocks */
			

	/* Calculate the number clocks nessesary and validate the targets offset.
	 *    If we can program a resonable number of clocks and the targets
	 *    offset does not exceed ours, then we will pass on.  If either
	 *    of these tests fail, then we send reject and become asynchronus.
	 */
	if( ((temp = (tgp / tcp) - 4) > MAX_XFERP) || ( tos > MAX_SYNCOFFSET) ) {
		/* far to slow..... tell him to go asynchronus */

#ifdef STATS
		lst->scsi_timing = 0;	/* SCSI timing */
#endif	

		lth->sbclmask = USEDCNTL;			/* use dcntl */

		act_scsi_rejectmsg( lst, lth );

		rethread( lst, lth, (int32)lth->scriptstart );
		return;
	}

	/*  save the bytes where the SIOP can read them out at select time */
	lth->devfield.devsync.sxfr_mask |= (temp << BN_TP0) | (tos & SXFR_OMSK);
	nthread( lst, lth );
}


/*
 * hw53c710.c - NCR 53C710 SIOP specific subroutines.
 *
 * The following routines allow access to information in the SIOP
 * registers.
 *
 * Register "scratch0" for example contains phase information.
 *
 */

/*
 *	Function:
 *		ncr_ph_messd - Was script in Message In Phase
 *
 *  Synopsis:
 *		error_code ncr_ph_messd( Lstat lst );
 *
 *	Description:
 *	    Was script in message in phase?
 *
 */


#if defined (__STDC__) || defined (_ANSI_EXT)
error_code ncr_ph_messd( Lstat lst )
#else
error_code ncr_ph_messd( lst )
Lstat lst;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */				

	return( inc(&siop->scratch0) == A_PH_MESSD ) ;
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


#if defined (__STDC__) || defined (_ANSI_EXT)
error_code ncr_ph_select( Lstat lst )
#else
error_code ncr_ph_select( lst )
Lstat lst;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */				

	return( inc(&siop->scratch0) == A_PH_SELECT ) ;
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


#if defined (__STDC__) || defined (_ANSI_EXT)
error_code ncr_ph_idata( Lstat lst )
#else
error_code ncr_ph_idata( lst )
Lstat lst;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */					
	return( inc(&siop->scratch0) == A_PH_IDATA ) ;
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

#if defined (__STDC__) || defined (_ANSI_EXT)
error_code ncr_ph_odata(  Lstat lst )
#else
error_code ncr_ph_odata( lst )
Lstat lst;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */						
	return( inc(&siop->scratch0) == A_PH_ODATA ) ;
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


#if defined (__STDC__) || defined (_ANSI_EXT)
error_code ncr_ph_mess(  Lstat lst )
#else
error_code ncr_ph_mess( lst )
Lstat lst;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */						
	return( (inc(&siop->scratch3) & PH_MASK) == A_PH_MESS ) ;
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void ncr_flush_fifo(  Lstat lst )
#else
void ncr_flush_fifo( lst )
Lstat lst;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */						
	outc(&siop->ctest8, inc(&siop->ctest8) | B_CLF);	
	while(inc(&siop->ctest8) & B_CLF);
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void ncr_scsi_reset(  Lstat lst )
#else
void ncr_scsi_reset(  Lstat lst )
Lstat lst;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */						
	outc(&siop->scntl1, inc(&siop->scntl1) | B_RST); /* start SCSI Bus Reset */	
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void ncr_setpar(  Lstat lst, Lthread const lth )
#else
void ncr_setpar( lst, lth )
Lstat lst;
Lthread lth;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */					
		
	if( lth->xferflags  & SCSI_PARITY )
		/* enable parity checking */
		outc(&siop->scntl0, inc(&siop->scntl0) | B_EPC);
	else
		/* disable parity checking */
		outc(&siop->scntl0, inc(&siop->scntl0) & ~B_EPC);
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

#if defined (__STDC__) || defined (_ANSI_EXT)
void ncr_setsync( Lstat lst,  Lthread const lth )
#else
void ncr_setsync( lst,  lth )
Lstat lst;
Lthread lth;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */					
	
	if( lth->devfield.devsync.sxfr_mask & SXFR_OMSK ) {
		/* if running synchronus */
		if( lth->sbclmask != lst->sbclmaster ) {
			/* set for proper sync clock select */
			lst->sbclmaster = lth->sbclmask;
			outc(&siop->sbcl, lth->sbclmask);
		}
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

#if defined (__STDC__) || defined (_ANSI_EXT)
void ncr_rethread(  Lstat lst, Lthread const lth, int32 scriptent )
#else
void ncr_rethread( lst,  lth,  scriptent  )
Lstat lst;
Lthread lth;
int32 scriptent;
#endif
{
	Siop_p const siop = (Siop_p) lst->hdware; /* set hardware pointer */

	outc(&siop->sxfer, lth->devfield.devsync.sxfr_mask);
	outc(&siop->sbcl, lth->sbclmask);
	outc(&siop->sdid, lth->devfield.devsync.scsidev);		/* set ID */
	outl(&siop->dsa, SCRIPT_ADDRESS(lth));					/* data structure address */

	/* if no threads are active return to selfid */

	if( lth == &lst->logthread[lst->selfid] ) {
		lth->threadstate = TH_EMPTY;

#if defined(OS_CACHE_FLUSH) && defined(_OS9000)

	/* Flush thread */
	cache_flush( lst, (u_int32 *)lst, (u_int32) sizeof (struct lstat));

#endif

		outl(&siop->dsp,
		     SCRIPT_ADDRESS(&lst->act_script[Ent_WAITFORRESELECT]));

#if defined(TRACE_SCRIPT)
		/* start trace script */
		outc( &siop->dcntl, inc( &siop->dcntl ) | B_ESTD);
#endif

	} else 	{

#if defined(OS_CACHE_FLUSH) && defined(_OS9000)

	/* Flush thread */
	cache_flush( lst, (u_int32 *)lst, (u_int32) sizeof (struct lstat));

#endif

#if defined(LCLMEM)
		lst->phythread_local = (u_int32)SCRIPT_ADDRESS(lst->phythread);
#endif

		/* start script at this place! */
		outl(&siop->dsp, SCRIPT_ADDRESS(scriptent));

#if defined(TRACE_SCRIPT)
		/* start trace script */
		outc( &siop->dcntl, inc( &siop->dcntl ) | B_ESTD);
#endif

	}
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
	

#if defined (__STDC__) || defined (_ANSI_EXT)
void ncr_clear_sxfr_mask( Lthread const lth )
#else
void ncr_clear_sxfr_mask( lth )
Lthread lth;
#endif
{
	lth->devfield.devsync.sxfr_mask &= B_DHP;	/* remove sync per/off */
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

#if defined (__STDC__) || defined (_ANSI_EXT)
void ncr_init_sxfr_mask( Lthread const lth )
#else
void ncr_init_sxfr_mask( lth )
Lthread lth;
#endif
{
	lth->devfield.devsync.sxfr_mask = B_DHP;	/* disable halt on pe */
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


#if defined (__STDC__) || defined (_ANSI_EXT)
void ncr_async_msg( Lstat lst, Lthread const lth )
#else
void ncr_async_msg( lst, lth )
Lstat lst;						/* low level static storage */
Lthread lth;					/* logical thread to activate */
#endif
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
 *  This relates back to the 53C710 via:
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
 *		This routine does not take into account the FAST or WIDE operation
 *	of the bus.  See NCR publication: SCSI I/O Processor Data Manual
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
error_code set_msg( Lstat lst, Lthread lth, u_int32 scsiflags )
#else 
error_code set_msg( lst, lth, scsiflags )
Lstat lst;
Lthread lth;
u_int32 scsiflags;
#endif
{
	volatile u_int8 *msg;
	int32 tcp;
	
	msg = (u_int8 *)&lth->msgobuf[0];		/* set a pointer to the buffer */
	
	lth->msgoutlen = 1;			/* preliminarilly, set to 1 byte */
	*msg = (scsiflags & SCSI_ATN) ? IDENTIFY_DISCON : IDENTIFY_NODISCON;
	/* lun from command block */
	*msg++ |=
	  (((u_int8 *)SCRIPT_MADDRESS(lth->cdbptr))[1]) >> 5;
	
	if( (scsiflags & SCSI_SYNCHR) && (lth->synctried == 0 ) ) {
		/* first time the synchronus flag is seen, set to negotiate */
		lth->msgoutlen += 5;			/* add in size of sync message */
		*msg++ = M_EXTENDED;   			/* extended message flag */
		*msg++ = SYNCREQ_SIZE;			/* size of the rest of the message */
		*msg++ = SYNC_REQUEST;			/* synchronus message request */

		/* 
		 * Allow SCSI I and SCSI II periods based on
		 * descriptor values.
		*/

		/* SCSI I Timing desired? */
		
		if( lth->xferflags & SCSI_SLOW ){		

			*msg++ = 50;				/* minimum period (200 ns, by spec) */

		} 

		/* FAST SCSI II Timing? */		

		else 
		{

			*msg++ = MIN_PERIOD;		/* minimum period (100 ns, by spec) */
		}

 		*msg++ = MAX_SYNCOFFSET;		/* max that siop will allow */

		lth->synctried |= NEGOTIATE | SYNCTRIED;
		return TRUE;
	} else {
		/* not time to negotiate */
		lth->synctried &= ~NEGOTIATE;		/* insure not stale */
		return FALSE;
	}
}



