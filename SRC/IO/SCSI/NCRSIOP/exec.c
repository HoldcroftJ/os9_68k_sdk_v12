/*
 * exec - execute scsi command for the scsisiop low level module
 *        (NCRSIOP SCSI I/O Processor)
 */

/*--------------------------------------------------------------------------,
|                                                                           |
|           Copyright 1995-1998 by Microware Systems Corporation            |
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
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
|             ---- OS-9000/PPC V2.1.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
| 02 98/01/29 Fixed compiler warnings.                                GbG   |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- SCSI Common Sub-Component v1.0 Released ----             |
| 03 09/20/01 Improve the fariness of access to disk (CF10781)        peng	|
|             $$                    <RELEASE_INFO>                     $$   |
`--------------------------------------------------------------------------*/

#include	<scsisiop.h>

#if defined(OS_CACHE_FLUSH) && defined(_OS9000)
#include	<cache.h>
void cache_flush(Lstat lst, u_int32 *addr, u_int32 size);
#endif

/*
 * int exec (proc, dmod, cmdblk, scsiflags, llcmd) - execute a SCSI command
 */

#if defined (_OSK)

#ifndef CBOOT
#if defined (__STDC__) || defined (_ANSI_EXT)
int exec (procid *proc, Datmod dmod, Cmdblk cmd, u_int32 scsiflags, u_int32 llcmd)
#else
int exec (proc, dmod, cmd, scsiflags, llcmd)
procid		*proc;			/* process descriptor ptr */
Datmod		dmod;			/* data module ptr */
Cmdblk		cmd;			/* command block ptr */
u_int32		scsiflags;		/* scsi option flags */
u_int32		llcmd;			/* low-level scsi bus command */
#endif /* __STDC__ or _ANSI_EXT */

#else /* _OSK and CBOOT */
/* Cboot entry point */
#if defined (__STDC__) || defined (_ANSI_EXT)
int llexec (Cmdblk cmd, u_int32 atn, u_int32 llcmd )
#else
int llexec ( cmd, atn, llcmd )
Cmdblk cmd;			/* command block ptr */
u_int32 atn;		/* assert attention */
u_int32 llcmd;		/* low-level scsi bus command */
#endif
#endif

/* OS9000 version of BOOTER is not included 
   OS9000 is compiled in ANSI mode only     */

#elif defined (_OS9000) && !defined(BOOTNCR)
error_code exec (Datmod dmod, Cmdblk cmd, u_int32 scsiflags, u_int32 llcmd)
#endif
{
	Lstat lst;						/* low level statics */
	Lthread lth;					/* logical thread */
	error_code error;				/* local error number */
#ifdef _OS9000
	u_int32 sleepval;				/* holder for sleep value */
	signal_code signal;				/* holder for signal code */
#endif

#ifdef CBOOT
	extern struct lstat	sstorage;	/* cboot static storage */
#endif
 
	/*
	 * integrity check:
	 * check that passed 'llcmd' is valid for this version
	 * low level module and that the scsiflags can be supported.
	 */
#ifndef CBOOT
#ifdef NOSYNC
	/* due to reliablility problems and our inability to prove that the
	 *  issue is of a hardware nature,  this line will remove the synchronus
	 *  flag. (synchronus not supported)
	 */
	scsiflags &= ~SCSI_SYNCHR;		/* remove synchronus flag. */
#endif

	if( (llcmd != (u_int32)DEFAULT) || ((scsiflags & ~SCSI_SUPPORT) != 0) )
#else	
	if( llcmd != (u_int32)DEFAULT ) 
#endif	

#if defined (__STDC__)
		return EOS_PARAM;				/* signal parameter problem */
#else
		return E_PARAM;					/* signal parameter problem */
#endif
		
#ifdef CBOOT
	lst = &sstorage;					/* set the static storage pointer */
#else	
	lst = (Lstat) dmod->dm_statics;			/* get static storage */
#endif /* CBOOT */

#ifdef STATS
	lst->grosscmd++;
#endif		
		
	/* ensure that byte-count of dma is reasonable for dma device */
	if (cmd->_cb_datlen >= MAXDMA)

#if defined (__STDC__)
		return EOS_PARAM;		/* return parameter error */
#else
		return E_PARAM;			/* return parameter error */
#endif

	/* ensure that the SCSI ID makes sense for this host adapter */
	if (cmd->_cb_scsi_id > MAXSCSI)

#if defined (__STDC__)
		return EOS_UNIT;		/* return bad unit */
#else
		return E_UNIT;			/* return bad unit */
#endif
	
	lth = &lst->logthread[cmd->_cb_scsi_id];  /* get pointer to ID's thread */
	
#if defined (_OSK)
	while( lst->chipbusy != CHIPFREE )
		_f_sleep( ONE_TICK + ONE_TICK );	/* give up time until reset done */

#elif defined (_OS9000)
	while( lst->chipbusy != CHIPFREE ) {
        sleepval = ONE_TICK+ONE_TICK;
		_os_sleep( &sleepval, &signal);		/* give up time until reset done */
    }
#endif

	/*  Semaphore on the logical thread (SCSI ID) */
	if( (error = grabsem( (u_int8 *) &lth->thread_sem, (u_int32 *) &lth->waiters)) )
		return error;
		
	lth->chiperror = lth->cmdstatus[0] = FALSE;		/* no errors */

	/*  now, condition static storage for the command */
	lth->cbkptr = (Cmdblk)SCRIPT_ADDRESS(SWAPLONG((u_int32)cmd));						/* preserve pointer to command */
	lth->datacount = SWAPLONG(cmd->_cb_datlen);		/* set data length */
	lth->dataptr = (u_int8 *)SCRIPT_ADDRESS(SWAPLONG((u_int32)cmd->_cb_datptr));			/* set pointer to data */
	lth->cdblength = SWAPLONG(cmd->_cb_cmdlen);		/* size of SCSI command */
	lth->cdbptr = (u_int8 *)SCRIPT_ADDRESS(SWAPLONG((u_int32)cmd->_cb_cmdptr));			/* pointer to SCSI command */

	lth->phaseflg = 0;						/* insure that it is not stale */

#ifdef CBOOT
	lth->xferflags = 0;				/* no atn, sync, or parity at boot */
#else

#if defined(_OSK)
	lth->processid = (process_id) proc->_id;		/* who to wake when done */
#elif defined(_OS9000)
	lth->processid = (process_id) dmod->dm_pid;	;	/* who to wake when done */
#endif

	lth->xferflags = scsiflags;		/* copy for rethread */
#endif	
	
#if defined(OS_CACHE_FLUSH) && defined(_OS9000)

	/* Flush data buffer */

	if (cmd->_cb_datlen) 
			cache_flush( lst, (u_int32 *)cmd->_cb_datptr, (u_int32) cmd->_cb_datlen);

	/* Flush SCSI cmd */

	cache_flush( lst, (u_int32 *)cmd->_cb_cmdptr, (u_int32) cmd->_cb_cmdlen);

	/* Flush lstat */

	cache_flush( lst, (u_int32 *)lst, (u_int32) sizeof (struct lstat));

#endif
	
	/* set the message out buffer as needed */
	if( set_msg( lst, lth, lth->xferflags ) )
		lth->xferflags |= SCSI_ATN;			/* 'pencil' in the flag */

	act_scsi_select( lst, lth );			/* set script for SCSI select */
		
	enqueue( lst, lth );					/* enqueue and start */
	
	if( lth->chiperror )
		error = lth->chiperror;
	else
		error = lth->cmdstatus[0];			/* scsi status goes instead */
    	
	lth->thread_sem = 0;					/* free the semaphore (Phase II) */

	return error;							/* results.... */
}

#if defined(OS_CACHE_FLUSH) && defined(_OS9000)

void cache_flush(Lstat lst, u_int32 *addr, u_int32 size)
{
	if (lst->cache_cctl)
		(*lst->cache_cctl)(C_FLDATA|C_ADDR, addr, size);
}
#endif


