/*
 * Copyright 1990, 1991, 1992, 1993 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 * history:
 *
 * ed.   date    reason                                           who.
 * --  --------  ---------------------------------------------    ---
 * 01  90/01/25  history starts                                   Rwb
 *               ---- OS-9/68K V2.4 released ----
 * 02  91/07/23  changed the docmd() method of accessing the      Rwb
 *               boards address register.
 * 03  92/04/03  repaired cacheflush to ensure valid a6.          wwb
 * 04  93/02/10  reworked mask_irq for MSP support.               wwb
 * 05  93/05/26  Modified for UCC compatibility mode. Extracted   ats
 *               assembly code into separate file.
 *               ---- OS-9/68K V3.0 released ----
 * 06  94/04/14  Adopted new error codes for ANSIfication. Added  ats
 *               SERIALIZE_M() calls for future superscalars.
 *               ---- OS-9/68K V3.1 released ----
 */

#include "scsi327.h"
#include <scsicmds.h>

#define BDTIMEOUT 0x500000		/* timeout period for the board */



/*********************************************************************
***   perform a CSR type command
*********************************************************************/
int docmd( hw, cmd, lst, ccpt )
register Hardware hw;			/* the device itself */
int cmd;						/* CSR command to perform */
Lstat  lst;						/* low level static storage */
register CmdChan ccpt;			/* command channel header pointer */
{
	int timeout = BDTIMEOUT;		/* nice busy loop */
	register int lreloc;
	register short *sp;						/* coerce the hardware pointer */
	register long value;
	extern int tas();
	
	
	lreloc = lst->reloc;			/* set local relocation constant */
	value = (long)ccpt + lreloc;	/* address of the command header */
	
	while( --timeout ) {
		/* wait for un_busy and gain control */
		if( !(hw->xtrl_reg & CSR_BUSY) )
			if( !tas( &hw->tas_reg ) )
				break;
	}

	if( !timeout )
#ifdef __STDC__
		return EOS_DEVBSY;		/* couldn't get control */
#else
		return E_DEVBSY;		/* couldn't get control */
#endif
		
	hw->addr_mod = ( value & EXTMSK ) ? VME_EXSUPDAT : VME_SUPDAT;
	hw->buss_wth = DMA_WIDTH;				/* specify size of DMA */
/*	hw->madd_reg = (char *)ccpt + lreloc;	/* address of the command header */

	/* new code to talk to the address register */
	sp = (short *)&hw->madd_reg;			/* pointer to address register */
	*sp++ = (value >> 16);					/* high word */
	*sp = (value);							/* low word */
	/* end new code */
	
	SERIALIZE_M();
	hw->tas_reg = TAS_BIT | TAS_VCM | cmd;	/* tell board what to do */
	SERIALIZE_M();
	hw->xtrl_reg |= CSR_ATTEN;				/* ok... so do it! */
	SERIALIZE_M();

	timeout = BDTIMEOUT;	/* now wait for valid status to arrive */
	while( --timeout ) {
		if( hw->tas_reg & TAS_STV )
			break;
	}
	
	cacheflush();

	if( !timeout )
#ifdef __STDC__
		return EOS_HARDWARE;		/* board went under! */
#else
		return E_HARDWARE;			/* board went under! */
#endif
		
	timeout = hw->stat_reg;			/* get the status for return */
	
	SERIALIZE_M();
	hw->tas_reg |= TAS_CCP;			/* set command complete */
	SERIALIZE_M();
    hw->xtrl_reg |= CSR_ATTEN;		/* tell board we are done */
	SERIALIZE_M();

	return timeout;					/* return the status */
}
	

/**********************************************************
**  This function will return the script table associated
**  with the function to be performed.  
**
**  The arrangement of the scripts (index) determines which script
**  is passed back.  The non-data-transfer script is index 0 and the
**  special message phase script is script 3.  This routine will
**  set a flag indicating if the special message script has been returned.
**/
u_char *get_script( function, flags, retflag )
register int function;
int flags;
int *retflag;
{
	register int index;
	extern void script_table();
	
	*retflag = 0;			/* assume that will not be message script...  */
	             			/*  .... may be changed below.                */

	switch( function ) {
		/* first, no data transfer commands */
		case SC_TESTREADY:						/* test unit ready */
		case SC_RESTORE:						/* restore (SC_REWIND) */
		case SC_FORMAT:							/* format the drive */
		case SC_SEEK:							/* seek */
		case SC_SEEKEXTD:						/* seek extended */
		case SC_REMOVAL:						/* prevent media removal */
		case SC_RETENTION:						/* retention a tape */
		case SC_SEEKBLOCK:						/* seek block */
		case SC_WRITEMARK:						/* write filemark */
		case SC_SPACE:							/* space the tape */
		case SC_ERASE:							/* erase a tape */
		case SC_DEFINE:							/* omti define floppy */
		case SC_FORMATRK:						/* omti format track */
		case SC_LOAD:							/* load/unload unit */
/*			index = 0;      */
			*retflag = index = (flags & SCSI_SYNCHR) ?  3 : 0;
			break;
			
		/* these are the data in commands */
		case SC_REQSENSE:						/* request sense */
		case SC_READ:							/* read data from device */
		case SC_INQUIRY:						/* find out about device */
		case SC_MODESENSE:						/* mode sense */
		case SC_RECDIAG:						/* receive diagnostic */
		case SC_READCAP:						/* read capacity */
		case SC_READXTD:						/* read extended */
		case SC_RDDEFECT:						/* read defect data */
		case SC_READDB:							/* read data buffer */
		case SC_READLIMITS:						/* read block limits */
		case SC_RECOVER:						/* recover buffered data */
			index = 1;
			break;
			
		/* these are the data out type commands */		
		case SC_REASSIGN:						/* reassign blocks */
		case SC_WRITE:							/* write */
		case SC_RSRVUNIT:						/* reserve unit */
		case SC_MODESELECT:						/* mode select */
		case SC_RELEASE:						/* release unit */
		case SC_SENDDIAG:						/* send diagnostic */
		case SC_WRITEXTD:						/* write extended */
		case SC_WRITEVFY:						/* write and verify */
		case SC_VERIFY:							/* verify - Sequential access */
		case SC_DAVERIFY:						/* verify - Direct Access */
	    case SC_WRITEDB:						/* write data buffer */
	    case SC_COPY:							/* copy */
	    case SC_ASSIGN:							/* omti assign winch params */
			index = 2;
			break;

		default:
			return 0;						/* not valid command */

	}	/* end switch */

	return( ((u_char *) ((void *)script_table)) + ( index * SCRIPT_SIZE ) );
}

#ifdef _UCC
_asm(" use spptasm.a");
#else
@ use spptasm.a
#endif

