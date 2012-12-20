
/*
 * irq - interrupt routine for the mVme327 low level driver
 */
 
/*
 * Copyright 1990 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	"scsi327.h"

extern void cacheflush();

/*
 * irq(lst, hw) - notify mainline that irq has occured
 *     Note that because F$IRQ allows us to set up what we
 *       wish in the registers when we reach the interrupt service,
 *       we have chosen the low level static storage.  Note well
 *       that the declaration of variables within irq() must not
 *       change.   'lst' and 'hw' are in the proper registers and need
 *       not be initialized before use.
 */

int irq (lst, hw)
Lstat lst;		/* low-level static storage */
Hardware hw;	/* vme327 hardware ptr */
{
	register int lreloc;	/* local relocation constant */
	Envelope ept;			/* envelope pointer */
	SCSIspec spt;			/* pointer to the command status info */
	Spcmd sp;				/* command block */
	
	cacheflush();				/* clear caches if nessesary */
	lreloc = lst->reloc;		/* local copy of the relocation constant */

	/* status envelope pointer */
	ept = (Envelope)((int)lst->sc_chanel.stat_head - lreloc);
	
	/* if not valid the first time, not our interrupt */
	if( !(ept->env_valid) )
		return -1;
		
	/* seems to have been our interrupt, process it */
	do {
		/* get the address of the command/status */
		sp = (Spcmd)((int)ept->cmd_stat - lreloc);
		
		/* then the address of the SCSI specific packet. */
		spt = (SCSIspec)((int)sp->cp_packt - lreloc);
		
		lst->sc_chanel.stat_head = ept->link_ptr;	/* point to next */
		_fev_set (spt->ssp_ievent, CBWAKE, FALSE );	/* tell process */

		/* return envelope to pool */
		ept->link_ptr = lst->freptr;				/* set link */
		lst->freptr = ept;							/* set our pool ptr */
		ept->env_valid = FALSE;						/* insure null */

		/* see if there are more ready to be processed */
		ept = (Envelope)((int)lst->sc_chanel.stat_head - lreloc);
		if( !(ept->env_valid) )
			return 0;								/* all done for now */
	} while( TRUE );		/* until all processed */
}

