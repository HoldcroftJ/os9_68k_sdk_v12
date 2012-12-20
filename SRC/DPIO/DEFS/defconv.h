/****************************************************************************
 *           Copyright 1989-1998 by Microware Systems Corporation           *
 *                           All Rights Reserved                            *
 *                         Reproduced Under License                         *
 *                                                                          *
 * This software is confidential property of Microware Systems Corporation, *
 * and is provided under license for internal development purposes only.    *
 * Reproduction, publication, distribution, or creation of derivative works *
 * in any form to any party other than the licensee is strictly prohibited, *
 * unless expressly authorized in writing by Microware Systems Corporation. *
 *                                                                          *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *  01 ??/??/?? Created                                                ???
 *  02 04/22/99 Added fatalsignal() macro and function prototypes      rds
 *  03 04/26/99 Changed prototype for _os_initdata, include module.h   rds
 *  04 10/15/01 fatalsignal() is now defined in signal.h for 68k       rds
 *  05 11/12/01 Okay, now fatalsignal() is in signal.h				   rry
 ****************************************************************************/

/* OS-9/68k DPIO conversion definitions */

#if !defined(_DEFCON_H)
#define _DEFCON_H


#ifdef _OSK


/*** Module Definitions from OS-9000 <module.h> ***/

/* structure mh_exec (mod_exec) field conversions */
#define	m_sync		_mh._msync		/* sync bytes ($4afc) 					*/
#define	m_sysrev	_mh._msysrev	/* system revision check value 			*/
#define	m_owner		_mh._mowner		/* owner id 							*/
#define	m_name		_mh._mname		/* offset to module name 				*/
#define	m_access	_mh._maccess	/* access permission 					*/
#define	m_tylan		_mh._mtylan		/* type/lang 							*/
#define	m_attrev	_mh._mattrev	/* rev/attr 							*/
#define	m_edit		_mh._medit		/* edition 								*/
#define	m_usage		_mh._musage		/* comment string offset 				*/
#define	m_symbol	_mh._msymbol	/* symbol table offset 					*/
#define	m_parity	_mh._mparity	/* header parity 						*/
#define	m_exec		_mexec			/* offset to execution entry point 		*/
#define	m_excpt		_mexcpt			/* offset to exception entry point 		*/
#define	m_data		_mdata			/* data storage requirement 			*/
#define	m_stack		_mstack			/* stack size 							*/
#define	m_idata		_midata			/* offset to initialized data 			*/
#define	m_idref		_midref			/* offset to data reference lists 		*/

/*** WARNING:  THIS CONFLICTS WITH <mbuf.h> Mbuf->m_size DEFINITION ***/
#if 0
#define	m_size		_mh._msize		/* module size 							*/
#endif


/*** Process definitions from OS9000 process.h */

/* structure procid field conversions */
#define	p_id		_id				/* process id 							*/
#define	p_pid		_pid			/* parent's id 							*/
#define	p_sp		_sp				/* system stack ptr 					*/
#define	p_group		_group			/* group number 						*/
#define	p_user		_user			/* user number 							*/
#define	p_prior		_prior			/* priority 							*/
#define	p_age		_age			/* age 									*/
#define	p_state		_state			/* status 								*/
#define	p_queueid	_queueid		/* current queue id 					*/
#define	p_pmodul	_pmodul			/* primary module 						*/
#define	p_memimg	_memimg			/* allocated memory block ptrs 			*/
#define	p_sigflg	_sigflg			/* signal flag 							*/
#define p_siglst	_signal			/* value of last received signal		*/
#define p_sigcnt	_sigcnt			/* number of signals in queue			*/
#define p_preempt	_preempt		/* System state preemption flag			*/

#define CONDEMN		PS_CONDEMN		/* Define the condemn bit				*/
#define DEAD		PS_DEAD			/* Define the dead bit					*/
#define TIMOUT		PS_TIMOUT		/* Define the time slice expired bit	*/

#define B_WAKEUP	0x80			/* wakeup is pending when set 			*/

#define	Q_DEAD		'-'				/* no queue: dead process				*/
#define	P_BLKSIZ_0	_blksiz[0]
#define	PROC_STATE	0x8000

#define SYSSTATE	PS_SYSSTATE
#define MEMIMGSZ	((u_char*)(MEMBLKS*sizeof(u_char*)))

#define TH_SPOWN	0x00000002		/* System process owns the thread		*/

/*** Mode definitions from OS9000 modes.h */

#define  S_IGREAD   S_IREAD     /* group read (same as user) */
#define  S_IGWRITE  S_IWRITE    /* group write (same as user) */
#define  S_IGEXEC   S_IEXEC     /* group execute (same as user) */
#define  S_ISEARCH  S_IEXEC     /* search permission */
#define  S_IGSEARCH S_IGEXEC    /* group search */
#define  S_IOSEARCH S_IOEXEC    /* public search */



/*** Error Definitions from OS-9000 <errno.h> ***/

#if !defined(__STDC__)
#define E_NOTME		0xb0		/* OS-9000 interrupt service routine error	*/
#endif
#define EOS_NOTME	0xb0		/* OS-9000 interrupt service routine error	*/


/*** Signal Definitions from OS-9000 <signal.h> ***/

#define S_KILL		SIGKILL		/* DPIO kill process signal */
#define S_WAKE		SIGWAKE		/* DPIO wakeup process signal */
#define S_QUIT		SIGQUIT		/* DPIO keyboard abort */
#define S_INT		SIGINT		/* DPIO keyboard interrupt */
#define S_HANGUP	SIGHUP		/* DPIO hang up process signal */
#include <signal.h>				/* get fatalsignal macro */



/*** Setstat/Getstat Definitions from OS-9000 <sg_codes.h> ***/

#define SS_PATHOPT	SS_Opt		/* read/write PD options					*/
#define	SS_READY	SS_Ready	/* check for device ready					*/
#define	SS_SIZE		SS_Size		/* read/Write file size						*/
#define	SS_RESET	SS_Reset	/* device restore							*/
#define	SS_WTRACK	SS_WTrk		/* device write track						*/
#define	SS_POS		SS_Pos		/* get file current position				*/
#define	SS_LINK		SS_Link		/* link to Status routines					*/
#define	SS_ULINK	SS_ULink	/* unlink Status routines					*/
#define	SS_FEED		SS_Feed		/* destructive forward skip (form feed)		*/
#define	SS_FREEZE	SS_Frz		/* freeze DD_ information					*/
#define	SS_DCMD		SS_DCmd		/* send direct command to device			*/
#define	SS_DEVNAME	SS_DevNm	/* return device name						*/
#define	SS_TICKS	SS_Ticks	/* set lockout honor duration				*/
#define	SS_LOCK		SS_Lock		/* lock/release record						*/
#define	SS_DSTAT	SS_DStat	/* return display status					*/
#define	SS_JOY		SS_Joy		/* return joystick value					*/
#define	SS_BLKRD	SS_BlkRd	/* block read								*/
#define	SS_BLKWR	SS_BlkWr	/* block write								*/
#define	SS_RETEN	SS_Reten	/* retension cycle							*/
#define	SS_ELOG		SS_ELog		/* read error log							*/
#define	SS_SENDSIG	SS_SSig		/* send signal on data ready				*/
#define	SS_RELEASE	SS_Relea	/* release device							*/
#define	SS_ATTR		SS_Attr		/* set file attributes						*/
#define	SS_BREAK	SS_Break	/* send break out serial device				*/
#define	SS_RSBIT	SS_RsBit	/* reserve bitmap sector (for disk reorg)	*/
#define	SS_FDINFO	SS_FDInf	/* get FD info for specified FD sector		*/
#define	SS_ENRTS	SS_EnRTS	/* enable RTS (modem control)				*/
#define	SS_DSRTS	SS_DsRTS	/* disable RTS (modem control)				*/
#define	SS_DCON		SS_DCOn		/* send signal DCD TRUE						*/
#define	SS_DCOFF	SS_DCOff	/* send signal DCD FALSE					*/
#define	SS_SKIP		SS_Skip		/* skip block(s)							*/
#define	SS_MODE		SS_Mode		/* set RBF access mode						*/
#define	SS_OPEN		SS_Open		/* notification of new path opened			*/
#define	SS_CLOSE	SS_Close	/* notification of path being closed		*/


/*** Include OS-9000 register definitions header file */
#include <reg68k.h>

#if 1

#define _change_static change_static 
#define _get_static get_static
void *change_static(void *);
void *get_static(void);

#define change_static(x) \
	((void *)((char *)((change_static)((void *) (((char *)(x)) + 0x8000))) - 0x8000))

#define get_static()  ((void *)((char *)((get_static)()) - 0x8000))

#endif /* 1 */

#include <module.h>
/*** Prototypes for functions in conv_lib.l ***/
#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#else
#define _OP(x) ()
#endif
error_code _os_sleep _OP((u_int32 *, signal_code *));
error_code _os_ev_wait _OP((event_id, int32 *, signal_code *, int32, int32));
error_code _os_initdata _OP((mh_com *, void *));
error_code _os_getstat _OP((path_id, u_int32, void *));
error_code _os_setstat _OP((path_id, u_int32, void *));
error_code _os_irq _OP((u_int32, u_int32, void *, void *));
error_code _os_alarm_cycle _OP((alarm_id *, signal_code, u_int32));
error_code _os_alarm_set _OP((alarm_id *, signal_code, u_int32));
error_code _os_ss_luopt _OP((path_id, u_int32, void *));
error_code _os_gs_luopt _OP((path_id, u_int32 *, void *));

#endif /* _OSK */

#endif	/* _DEFCON_H */

