/* OS-K process descriptor definitions */
#if !defined(_PROCID_H) && !defined(_procid_h)
#define _PROCID_H
#define _procid_h

/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1998 by Microware Systems Corporation              |
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
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
|  01 98/07/29 Added edition history.                                  wwb  |
|  02 98/08/26 Added t_state field for thread block.                   wwb	|
|  03 00/06/11 Added THR_DELIP bit for protecting alarm deletes        mgh  |
|				---- OS-9/68K V3.1 Release ----								|
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/


#if !defined(_MODULE_H)
#include <module.h>
#endif /* _MODULE_H */

#define NUMPATHS	(32)
#define MEMBLKS		(32)
#define DEFIOSIZE	(32)

/* The pointer definitions herein are generally (unsigned char *) */

typedef struct pdsc {
	unsigned short
		_id,				/* process id */
		_pid,				/* parent's id */
		_sid,				/* sibling's id */
		_cid;				/* child's id */
	unsigned char
		*_sp,				/* system stack ptr */
		*_usp,				/* user stack ptr */
		*_pagcnt;			/* memory size */
	unsigned short
		_group,				/* group number */
		_user,				/* user number */
		_prior,				/* priority */
		_age,				/* age */
		_state,				/* status */
/* process state flags */
#define	PS_SYSSTATE		0x8000	/* executing system-state routine 		*/
#define PS_TIMSLEEP		0x4000	/* timed sleep							*/
#define PS_TIMOUT		0x2000	/* time slice has expired				*/
#define	PS_IMGCHG		0x1000	/* spu/mmu protection map has changed	*/
#define	PS_CHILDDEAD	0x0800	/* child process is dead				*/
/*						0x0400	reserved								*/
#define	PS_CONDEMN		0x0200	/* process is condemned					*/
#define	PS_DEAD			0x0100	/* process has terminated				*/
		_task;				/* process task number */
	unsigned char
		_queueid,			/* current queue id */
		_scall,				/* last user state system call executed */
		_baked,				/* non-zero if process created by fork */
		_emustate;			/* emulation status */
/* emulation status flags */
#define	ES_FPINIT040	0x01	/* 68040 fpu emulation initialized */
#define	ES_FPUNDATA040	0x02	/* extension (unsupported data type) iniz'd */
#define	ES_FPUCALM040	0x04	/* 68040 fpu exception calmed */
	unsigned short
		_deadlk,			/* dominant process id if I/O locked */
		_signal;			/* signal code */
	unsigned char
		*_sigvec,			/* signal intercept vector */
		*_sigdat;			/* signal intercept data address */
	struct pdsc
		*_queueN,			/* queue next ptr */
		*_queueP;			/* queue prev ptr */
	mod_exec
		*_pmodul;			/* primary module */
	unsigned char
		*_except[10],		/* program error exception vectors */
		*_exstk[10],		/* program error exception stack frame ptrs */
		*_traps[15],		/* user's trap vector table */
		*_trpmem[15];		/* user's trap static memory block ptrs */
	unsigned int
		_trpsiz[15];		/* trap handler static memory sizes */
	unsigned char
		*_excpsp,			/* system state except. recovery stack */
		*_excppc;			/* system state except. recovery program counter */
	char
		_dio[DEFIOSIZE];	/* default i/o data */
	unsigned short
 		_path[NUMPATHS];	/* i/o path table */
	unsigned char
		*_memimg[MEMBLKS];	/* allocated memory block ptrs */
	unsigned int
		_blksiz[MEMBLKS];	/* size of each allocated memory block */
	unsigned char
		*_dbgreg;			/* debugged process register stack frame */
	struct pdsc
		*_dbgpar;			/* debugged process parent pd */
	unsigned  int
		_dbgins,			/* debug instruction count */
		_uticks,			/* user state ticks elapsed */
		_sticks,			/* system state tick elapsed */
		_datbeg,			/* julian date when forked */
		_timbeg,			/* julian time when forked */
		_fcalls,			/* number of system calls exec */
		_icalls,			/* number of I/O calls execd */
		_rbytes,			/* number of bytes read */
		_wbytes;			/* number of bytes written */
	unsigned short
		_ioqp,				/* I/O queue prev ptr */
		_ioqn;				/* I/O queue next ptr */
	unsigned char
		*_frag, *_fragg;	/* free memory fragment ptrs */
	unsigned int
		_sched;				/* process scheduling constant */
	unsigned char
		*_spuimg;			/* ptr to process SPU image */
	unsigned int
		_bkptcnt;			/* number of breakpoints set */
	unsigned short
		_bpvalue[16];		/* breakpoint save area */
	unsigned int
		_acct[8];			/* reserved for user accounting module */
	unsigned char
		*_data;				/* ptr to process primary data area */
	unsigned int
		_datasz;			/* size of primary data area */
	unsigned char
		*FPUsave;			/* ptr to FPU save area */
	unsigned char
		*FPExcpt[7],		/* FPU exception vectors */
		*FPExStk[7];		/* FPU exception stack frame ptrs */
	unsigned char
		_siglvl,			/* signal interrupt level */
		_sigflg;			/* signal flag */
	unsigned short
		_sigxs;				/* number of excess signal blocks allocated */
	unsigned int
		_sigmask,			/* mask to disable signals 2 - 31 */
		_sigcnt;			/* number of signals pending */
	unsigned char
		*_sigque,			/* ptr to head of signal queue */
		_defsig[16],		/* default initial signal queue element */
		*_thread[2],		/* doubly linked thread queue */
		*_cfrag[2];			/* new-style mem color lists */
	unsigned int
		_mown;				/* original owner of primary module (security) */
	unsigned char
		*_expstk;			/* reserved for more stack space */
	unsigned int
		_expstksz;			/* reserved for more stack space (size) */
	unsigned int
		_preempt;			/* process level system-state preemption flag */
	unsigned short
		_procsiz,			/* size of (complete) process descriptor */
		_stacksiz;			/* size of process stack area */
	unsigned int
		_sigiret;			/* signal intercept recursion counter */
	unsigned int
		prcid_res[18];		/* reserved */
/* NOTE: sizeof(procid) will only return the "basic" part of the process */
/*       descriptor. */
} procid;

/* register image structure for threads [this MUST track with the
   assembly R$__ definitions] */
typedef struct {
    u_int32 r_d[8];				/* data registers */
    u_int32 r_a[8];				/* address registers */
    u_int16 r_sr;				/* status register */
    u_int32 r_pc;				/* status register/program counter */
    u_int16 r_fmt;				/* size of used execption stack */
} thregs, *Thregs;

/* Thread Execution Block */
typedef struct thread {
	u_int16			t_id,		/* reserved */
					t_proc;		/* owner process ID (zero if none) */
	u_int32			t_msiz,		/* thread block memory size */
					t_user;		/* owner's user number */
	struct thread	*t_next,	/* next thread in doubly linked list */
					*t_prev;	/* previous thread in doubly linked list */
	u_int32			t_linkn,	/* next list of assoc threads (owner link) */
					t_linkp,	/* prev list of assoc threads (owner link) */
					t_cycle,	/* wakeup cycle period */
					t_wktime,	/* wakeup time */
					t_wkdate;	/* wakeup date */
	u_char			t_state;	/* thread state */
#define	THR_DELETE	(1<<0)			/* thread deleted */
#define THR_ACTIVE	(1<<1)			/* thread active */
#define	THR_DELIP	(1<<2)			/* thread delete in progress */
	u_char			t_res[3];	/* reserved */
	thregs			t_regs;		/* register image (d0-d7,a0-a7,sr,pc,fmt) */
} thread, *Thread;


#endif /* _PROCID_H */
