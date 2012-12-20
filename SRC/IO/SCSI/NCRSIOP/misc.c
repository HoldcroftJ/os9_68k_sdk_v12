/*
 * misc - miscellaneous functions for scsisiop low level module
 *       (NCRSIOP SCSI I/O Processor)
 */
 
/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1991-2001 by Microware Systems Corporation         |
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
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
|             ---- OS-9000/PPC V2.1.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
| 02 98/01/29 Fixed compiler warnings.                                GbG   |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- SCSI Common Sub-Component v1.0 Released ----             |
| 03 00/03/06 Fixed problem with initial chdir from system process    GbG   |
| 04 00/08/17 Fixed OSK issue with chdir as well.                     GbG   |
| 05 00/09/27 Fixed OSK CBOOT sleep issue in enqueue.                 GbG   |
| 71 01/08/09 change debug code to use dbglog.                        gdw   |
| 72 09/20/01 Improve the fariness of access to disk (CF10781)        peng	|
`--------------------------------------------------------------------------*/
 
#include <scsisiop.h>

/*
 *	Function:
 *		grabsem - gain control of the semaphore requested, else sleep
 *
 *  Synopsis:
 *		int grabsem( u_char *sem )
 *
 *  Description:
 *		This subroutine accepts the passed address the address of a
 *  semaphore.  It will attempt to gain control of the semaphore on
 *  behalf of the caller returning FALSE if all goes well.  If the
 *  semaphore remains locked after some serious period of time,
 *  It returns E_HARDWARE indicating that something fatal has happened.
 *
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
error_code grabsem( u_int8 *sem, u_int32 *waiters)
#else
error_code grabsem( sem , waiters)
u_int8 *sem;			/* pointer to the semaphore */
u_int32 *waiters;       /*pointer to the waiters*/
#endif
{

#ifdef _OS9000
	u_int32 sleepval;
	signal_code signal;
#endif

	u_int32 irqmask;
	u_int32 waiter=0;

    DENTER("grabsem");
    DPRINTF(DBIT_HW, ("semaphore address %8x value %8x", sem, *sem));

	if (*waiters){	
#if defined (_OSK)
		_f_sleep( ONE_TICK );	
#elif defined (_OS9000)
		sleepval = ONE_TICK;
		_os_sleep( &sleepval, &signal);
#endif
	} 
	
	do {
#if defined (_OSK)
		irqmask = mask_irq(LEVEL7);
#elif defined (_OS9000)
		irqmask = irq_maskget();
#endif
		if (waiter==0){
			++*waiters;
			waiter=1;
		}
		if (*sem == 0){
			*sem = 1;
			--*waiters;

#if defined (_OSK)
			mask_irq(irqmask);
#elif defined (_OS9000)
			irq_restore(irqmask);
#endif
            DLEAVE("grabsem");
			return 0;
		}
#if defined (_OSK)
		mask_irq(irqmask);
#elif defined (_OS9000)
		irq_restore(irqmask);
#endif

#if defined (_OSK)
		_f_sleep( ONE_TICK );	
#elif defined (_OS9000)
		sleepval = ONE_TICK;
		_os_sleep( &sleepval, &signal);	
#endif

	} while( 1 );		/* until free */

}


/*
 *	Function:
 *		initthreads - initialize thread structures  
 *
 *  Synopsis:
 *		void initthreads( Lstat lst )
 *      
 *  Description:
 *		This routine will perform the one-time initialization of the
 *      the thread structures and the abort message area.
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void initthreads( Lstat lst )
#else
void initthreads( lst )
Lstat lst;
#endif
{
	Lthread lth;				/* logical thread */
	u_int32	tmp;
	int8 loop = NTHREADS;

    DENTER("initthreads");
	
	lst->lastthread = 0;
	
	while( --loop >= 0 ) {

		lth = &lst->logthread[loop];		/* pointer to logical thread      */

#if defined (NCR53C8XX)
		lth->devfield.devsync.scsidev = loop;	/* SCSI Target ID  */
		lth->devfield.devsync.scntl3 = ncr_scntl3( lst ); /* SCSI Control Register 3 */
#else
		lth->devfield.devsync.scsidev = (1<<loop); /* SCSI Target ID  */
#endif
		lth->statuscount = SWAPLONG(1);				/* SCSI status always 1 byte      */
		tmp = (u_int32)(&lth->cmdstatus[0]);
		lth->statusptr = (u_int8 *) SCRIPT_ADDRESS(SWAPLONG(tmp));
		
		lth->msgoutlen = SWAPLONG(1);						/* normally just identify */
		tmp = (u_int32)(&lth->msgobuf[0]);
		lth->msgobuf[0] = IDENTIFY_NODISCON;	/* set initial message    */
		lth->msgoptr = (u_int8 *) SCRIPT_ADDRESS(SWAPLONG(tmp));
		
		lth->msginlen = SWAPLONG(1);						/* normally just 1 byte */
		tmp = (u_int32)(&lth->msgibuf[0]);
		lth->msgiptr = (u_int8 *) SCRIPT_ADDRESS(SWAPLONG(tmp));

		ncr_init_sxfr_mask( lth ); 				/* disable halt on pe */
		
		if( loop == lst->selfid ) {
			/* set SELFID thread to point to the "wait" script entry */
			act_scsi_waitrsel( lst, lth );
		}
	}
		
	/* now hook up the extended message buffer pointers */
	tmp = (u_int32)&lst->extcnt.extenmessgcount;
	lst->extmsgfram = (u_int8 *) SCRIPT_ADDRESS(SWAPLONG(tmp));
	tmp = (u_int32)&lst->extmsgbuffer[0];
 	lst->extmsgbufptr = (u_int8 *) SCRIPT_ADDRESS(SWAPLONG(tmp));
		
	/* set the abort and reject messages */
	lst->armsg.msg.abort = M_ABORT;
	lst->armsg.msg.reject = M_REJECT;
	lst->armsg.msg.mpe = M_MPARITYER;
    DLEAVE("initthreads");
}

/*
 *	Function:
 *		enqueue - enqueue and attempt to start a command. 
 *
 *  Synopsis:
 *		void enqueue( Lstat lst, Lthread lth );
 *
 *  Description:
 *		This routine will perform the final  set up of the logical
 *  thread for submition to the SCSI bus.  This includes defining
 *  the SCSI SCRIPT entry point for the command, and informing the
 *  SIOP that the command is ready.
 *
 *      - if there is a current physical thread, mark us as pending
 *        knowing that we will be activated at the next available slot.
 *
 *		- if there is no current physical thread set signal bit to
 *		  activate the SIOP (which will be waiting for reselect!)
 *  
 *  See Also:
 *		reschedule()   [irqsup.c]
 *
 *  Script requirements:
 *		Since this is a single-threaded situation, we are not to
 *  too concerned with being reselected at this point.  It only 
 *  provides us with good script entry point from which an interrupt
 *  can be generated.
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void enqueue( Lstat lst, Lthread lth )
#else
void enqueue( lst, lth )
Lstat   lst;
Lthread lth;
#endif
{
	u_int32 oldmask;
	volatile u_int8 temp;

#ifdef _OS9000
	u_int32 sleepval;
	signal_code signal;
#endif

    DENTER("enqueue");

	/* this operation must not allow irq() to run until after we
	 * get done with this.
	 */

#if defined (_OSK)
	oldmask = mask_irq(LEVEL7);
#elif defined(_OS9000)
	oldmask = irq_maskget();
#endif

	lth->threadstate = TH_PENDING;		/* will start at next busfree */

	ncr_kick( lst );

#if defined (_OSK)

	mask_irq(oldmask);
	coherent();							/* cache = mem */

#ifndef CBOOT
	/* insure that wake-ups other than the driver are ignored! */		
	while ( lth->threadstate != TH_DONE )
		_f_sleep( (lth->processid > 1)?UNTIL_DONE:2 );	/* wait for results */
#else /* CBOOT */
	while ( lth->threadstate != TH_DONE );	/* wait for results */
#endif

#elif defined (_OS9000)

	irq_restore(oldmask);
	coherent();							/* cache = mem */

	/* insure that wake-ups other than the driver are ignored! */
	while ( 1 ) {

		if (lth->threadstate == TH_DONE)
			break;

		/* if system process wait two ticks. Else wait till signal */

		sleepval = (lth->processid > 1)?UNTIL_DONE:2;
		_os_sleep( &sleepval, &signal);     /* wait for results */
	}

#endif
    DLEAVE("enqueue");
}

/*
 *	Function:
 *		 clearmem - clear a number of bytes
 *
 *  Synopsis:
 *		void clearmem( char *mptr, int size )
 * 
 *  Description:
 *		Writes 'size' bytes of zero begining at 'mptr'
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void clearmem( char *mptr, int32 size)
#else 
int clearmem(mptr, size)
char	*mptr;			/* memory block ptr */
int32	size;			/* memory size */
#endif
{
    DENTER("clearmem");

	while( --size >= 0 )
		*mptr++ = 0;
    DLEAVE("clearmem");
}

/* 
 *  Function:
 *		hexchar - get ascii hex equivalent of a number.
 *
 *  Synopsis:
 *		int8 hexchar( int8 n )
 *
 *  Description:
 *		Returns the ascii hex equivalent of 'n'.
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int8 hexchar( int8 n)
#else 
int8 hexchar(n)
int8 n;
#endif
{
    DENTER("hexchar");

	if (n > 9)
		n += 0x27;		/* for range a - f */
    DLEAVE("hexchar");
	return (n + 0x30);
}

/* 
 * Function:
 * 		lxtoa(stg, num) - convert long to 8 ascii hex digits 
 * 
 * Synopsis:
 *		void lxtoa( char *stg, long num )
 *
 * Description:
 *		Convert long value to ascii hex digits.  Usefull in creating
 * debugging code and generating ascii strings.
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void lxtoa(char *stg, int32 num )
#else 
void lxtoa(stg, num)
char *stg;
int32 num;
#endif
{
	int	i;
    DENTER("lxtoa");
	
	for (i = 7; i >= 0; i--, num >>= 4)
		stg[i] = hexchar(num & 0xf);
	stg[8] = '\0';		/* terminate */
    DLEAVE("lxtoa");
}

#ifndef	CBOOT
/*
 * Function:
 *		strcpy - copy NULL terminated string
 *
 * Synopsis:
 *		void strcpy( char *destination, char *source )
 *
 * Description:
 *		This is a primitive string copy subroutine.  A simple
 *  loop is used to transfer from 'source' to 'destination'.
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void strcpy( char *s1, char *s2)
#else 
void strcpy(s1, s2)
char *s1, *s2;
#endif
{
	while (*s1++ = *s2++);
}
#endif

#ifdef STATS
/*
 * Function:
 *		resetdblog - reset the log
 *
 * Synopsis:
 *		void resetdblog( Lstat lst )
 *
 * Description:
 *		This routine will zero the debug log area and reset the 
 * pointers to allow continuation of debugging.
 *
 * Caveats:
 *		This routine is macroed out in scsisiop.h when stats is not
 *  defined,  reducing the number of apparent ifdefs in the code.
 *
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void resetdblog( Lstat lst )
#else 
void resetdblog( lst )
Lstat lst;
#endif
{
	int8 loop,count;
	int32 *dest;
	
	dest = &lst->irqlog[0];
	loop = 0;
	count = DBLOGSZ;
	while( loop++ < count )
		*dest++ = 0;
	lst->nextin = &lst->irqlog[0];		/* reset pointer */
	lst->nentries = 0;
}

/*
 * Function:
 *		dblog - log a value in the buffer
 *
 * Synopsis:
 *		void dblog( Lstat lst, int value )
 *
 * Description:
 *		This is a primitive albeit a useful subroutine to log the
 * incomming interrupt information. This routine will manage the
 * log as a queue and not log information beyond the end of it's
 * defined space.
 *
 * Caveats:
 *		This routine is macroed out in scsisiop.h when stats is not
 *  defined,  reducing the number of apparent ifdefs in the code.
 *
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void dblog( Lstat lst, int32 value )
#else 
void dblog( lst, value )
Lstat lst;
int32 value;
#endif
{
	if( (lst->nentries + 1) >= DBLOGSZ )
		resetdblog(lst);

	lst->nentries += 1;
	*lst->nextin++ = value;
}
#endif /* STATS */
