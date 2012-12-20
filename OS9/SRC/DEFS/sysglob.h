#if !defined(_SYSGLOB_H)
#define _SYSGLOB_H

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
|																			|
| Edition History:															|
|  #   date		comments												by	|
| -- -------- --------------------------------------------------------- ---	|
|  1 02-23-87 Created													rg	|
|  2 05-01-88 Converted for use with OS-9.								rfd	|
|              ---- OS-9/68K V2.3 released ----								|
|              ---- CD-RTOS V1.0 released ----								|
|  3 08-23-89 Added d_nosleep.											wwb	|
|              ---- OS-9/68K V2.3.1 released ----							|
|  4 10-26-89 Update d_mputype comment for 68300 family.				wwb	|
|  5 03-09-90 Added T_AUTIRQ2, d_compat2, d_snoopd, d_snoopi.			wwb	|
|              ---- CD-RTOS V1.1 released ----								|
|              ---- OS-9/68K V2.4 released ----								|
|  6 12-10-92 Added d_firqvct, d_vctjmp, d_numsigs, d_sysconf,				|
|			  d_prcdescstack, d_movemin, d_alloctype, d_devcnt,				|
|			  d_preempt, d_fdispq, and d_inhouse variables.  Also,			|
|			  added MA_STD and MA_BUDDY definitions.					afh |
|              ---- OS-9/68K V2.5 (Discovery Pak) released ----				|
|  7 02-01-93 Added fdisp structure.									wwb	|
|             Also updated for std kernel defs.							wwb	|
|  8 02-22-93 Added d_kertyp defs.										wwb	|
|  9 04-20-93 Added d_inirq.											wwb	|
| 10 05-04-93 Added d_firqoff, d_irqoff, d_irqspoff, d_profmem.			wwb	|
|              (for FasTrak profiler).										|
| 11 06-22-93 Added d_devsiz.											wwb	|
| 12 07-08-93 Made sysdata be "set" (not equ) - ICode link issue.		wwb	|
| 13 09-16-93 Added d_mbar.												wwb	|
|              ---- OS-9/68K V3.0 released ----								|
| 14 12-13-93 Fixed close comments problem for d_rev15.					wwb	|
|              ---- OS-9/68K V3.0.1 released ----							|
|              ---- OS-9/68K V3.0.2 released ----							|
| 15 01-23-96 Added PwrMan information.									wwb	|
| 16 03-27-96 sysdata definition not available under C++.				wwb	|
| 17 04-05-96 repaired error introduced in ed.15.						wwb	|
|              ---- OS-9/68K V3.0.3 PwrMan Beta released ----				|
| 18 05-13-96 added optional prototypes									rry |
| 19 06-11-96 added d_irqheads (for non-MSP kernels)					wwb	|
|              ---- OS-9/68K V3.0.3 released ----							|
| 20 09-02-97 added 'fastboot' definitions.								wwb	|
| 21 02-27-98 made OFFSET definition more correct (UCC2.0)				wwb	|
| 22 06-19-98 Conditionalized fastboot definitions to avoid             ats |
|             duplicate definitions with MWOS/SRC/DEFS/ROM/rom.h.           |
| 23 07-15-98 added SysConf word definitions.							wwb	|
| 24 98/07/29 Updated for new copyright info.							wwb	|
|				---- OS-9/68K V3.1 Release ----								|
| 25 99/05/07 Added memory defs to match sysglob.a for .c file use.     rkw |
|				$$			<RELEASE_INFO>			$$						|
`--------------------------------------------------------------------------*/

#if !defined(_TYPES_H)
#include <types.h>
#endif /* _TYPES_H */

#if !defined(_PR_DESC)
#define _PR_DESC
typedef struct pdsc *Pr_desc, pr_desc;
#endif /* _PR_DESC */

#if !defined(_MH_CONFIG)
#define _MH_CONFIG
typedef struct mod_config *Mh_config, mh_config;
#endif /* _MH_CONFIG */

#if !defined(__cplusplus)
#ifdef	_UCC
_asm ("sysdata set 0");				/* offset of sysglobs in data area */
#else
@sysdata set 0						/* offset of sysglobs in data area		*/
#endif
#endif

#define GLOB(var)	(sysdata.var)	/* access to system global variable		*/

/* fake queue head definitions												*/
#define OFFSET(Struct, field)		((u_int32)((char *)&((Struct)0)->field - (char *)0))
#define FAKEHD(Struc, head, lnk)	((Struc)((char*)&head - OFFSET(Struc, lnk)))

/* process queue heads														*/
#define glob_activhd	FAKEHD(procid*, GLOB(d_activq[0]), _queueN)
#define glob_waithd		FAKEHD(procid*, GLOB(d_waitq[0]), _queueN)
#define glob_sleephd	FAKEHD(procid*, GLOB(d_sleepq[0]), _queueN)

/* system execution thread queue heads										*/
#define glob_alarmhd	FAKEHD(Thread, GLOB(d_alarm[0]), t_next)
#define glob_threadhd	FAKEHD(Thread, GLOB(d_thread[0]), t_next)
#define glob_ipcmdhd	FAKEHD(Ip_cmd, GLOB(d_ipcmd[0]), nxt)

/* memory list queue heads													*/
#define glob_memhd		FAKEHD(Mem_color, GLOB(d_freemem[0]), nxt)
#define sys_frags		FAKEHD(Mem_color, GLOB(d_sysprc)->_cfrag[0], nxt)
#define proc_frags		FAKEHD(Mem_color, GLOB(d_proc)->_cfrag[0], nxt)

/* exception jump table format												*/
typedef struct {
	u_int32			pea;			/* pea.l (XXX).w  instruction			*/
	u_int16			jmp;			/* jmp (xxxx).l   instruction			*/
	void			(*destin)();	/* absolute address (xxxx)				*/
} excpt_jmp, *Excpt_jmp;


/* Fast Dispatch definitions */
typedef	struct	fdisp {
	struct fdisp	*fstp_next;			/* next element in queue 			*/
	void			(*fstp_routine)();	/* ptr to dispatched routine		*/
	void			*fstp_data;			/* static data ptr					*/
	u_int32			fstp_status;		/* status information				*/
#define	fstp_inuse		(1<<0)		/* fast dispatch routine being executed	*/
#define	fstp_queued		(1<<1)		/* fast dispatch routine in dispatch queue	*/
} fdisp, *FDisp;


typedef struct {
	u_int16			d_id,				/* sync code 						*/
					d_nosleep;			/* sysproc can't sleep if non-zero	*/
	u_int32			d_rev1[7];			/* reserved 						*/
	Mh_config		d_init;				/* ptr to init module				*/
	void			(*d_clock)();		/* ptr to of system tick routine	*/
	u_int16			d_tcksec,			/* clock tickrate 					*/
					d_year;				/* year 							*/
	u_char			d_month,			/* month 							*/
					d_day,				/* day 								*/
					d_compat,			/* compatibility flags #1			*/
					d_68881;			/* non-zero if 6888x exists			*/
	u_int32			d_julian,			/* julian day 						*/
					d_second;			/* seconds until midnight 			*/
	u_int16			d_sysconf;			/* system configuration control		*/
	char			d_irqflag;			/* in IRQ stack if >= 0				*/
	u_char 			d_unkirq;			/* unknown IRQ count 				*/
	void			*d_moddirh,			/* module directory start ptr		*/
					*d_moddire,			/* module directory end ptr 		*/
					*d_prcdbt,			/* process descriptor block tbl ptr */
					*d_pthdbt;			/* path descriptor block table ptr	*/
	Pr_desc			d_proc,				/* ptr to current process desc		*/
					d_sysprc;			/* ptr to system process descriptor	*/
	u_int32			d_ticks;			/* system heartbeat (tick counter) 	*/
	Pr_desc			d_fproc;			/* process context in FPU registers */
	u_int32			*d_abtstk,			/* system state panic stack ptr 	*/
					*d_sysstk;			/* system state IRQ ptr 			*/
	void			(*d_sysrom)();		/* Bootstrap ROM jmp tbl entry point*/
	excpt_jmp		*d_excjmp;			/* exception jump table ptr			*/
	u_int32			d_totram,			/* total RAM available at startup 	*/
					d_minblk;			/* process min allocatable blk size */
	void			*d_fremem[2];		/* system free memory list head 	*/
	u_int32			d_blksiz,			/* system min allocatable blk size 	*/
					*d_devtbl,			/* I/O device table ptr 			*/
					d_spurirq,			/* spurious IRQ counter             */
					*d_autirq2[7],		/* 68070 on-chip AutoVect poll tbl	*/
					*d_vctirq[192]; 	/* Vectored IRQ device table ptrs 	*/
	void			*d_sysdis,			/* system svc dispatch table ptr	*/
					*d_usrdis;			/* user service dispatch table ptr	*/
	Pr_desc			d_activq[2],		/* active process queue headnode	*/
					d_sleepq[2],		/* sleeping process queue headnode	*/
					d_waitq[2];			/* waiting process queue headnode 	*/
	u_int32			d_actage,			/* active process queue age value	*/
					d_mputyp;			/* mpu type 68xxx					*/
	void			*d_evtbl[2];		/* event table head & tail ptrs		*/
	u_int32			d_evid;				/* next event ID number 			*/
	char			*d_spumem,			/* ptr to SPU global variables		*/
					*d_addrlim;			/* highest addr found during startup*/
	u_char			d_compat2;			/* cache config/compatibility		*/
	u_char			d_snoopd;			/* all data caches are snoopy		*/
	u_int16			d_procsz;			/* size of a process descriptor 	*/
	u_int32			*d_poltbl[8];		/* I/O autovector polling tbl heads */
	void			*d_freemem[2];		/* head of system free memory list	*/
	void			*d_ip[4];			/* reserved							*/
	u_int32			d_rev13[210];		/* reserved (old sys proc stub)		*/
	u_int32			d_cachmode,			/* 68020/68030/68040 CACR mode		*/
					d_disinst,			/* instruction cache disable depth	*/
					d_disdata;			/* data cache disable depth			*/
	char			*d_clkmem;			/* data ptr for sys tick routine	*/
	u_int16			d_tick,				/* current tick 					*/
					d_tslice,			/* ticks per slice 					*/
					d_slice,			/* current time slice remaining		*/
					d_rev5;				/* reserved 						*/
	u_int32			d_elapse;			/* delta time to awaken sys proc	*/
	void			*d_thread[2],		/* system thread queue head node	*/
					*d_alarm[2];		/* timed alarm thread head node 	*/
	u_int32			*d_sstklm,			/* System IRQ stack low bound 		*/
					d_forks,			/* number of actively forked procs	*/
					d_bootram,			/* ram found by bootrom search		*/
					d_fpusize,			/* FPU (max) state frame size 		*/
					d_fpumem;			/* FPU Emulator global data 		*/
	char			d_ioglob[256];		/* sys dependent hardware I/O flags	*/
	u_int16			d_devsiz,			/* device table entry size (IOMan)	*/
					d_minpty,			/* system minimum priority			*/
					d_maxage,			/* system maximum natural age		*/
					d_sieze;			/* ID of only proc allowed to run 	*/
	u_int32			d_cigar;			/* OS-9 data structure format code	*/
	char			*d_mminlim,			/* minimum memory addr allocatable	*/
					*d_mmaxlim;			/* maximum memory addr allocatable	*/
	u_int32			d_movemin,			/* min count for DMA/move16 opts.	*/
					d_preempt;			/* 0=system state preemption allowed*/
	FDisp			d_fdispq;			/* fast dispatch queue pointer		*/
	void			*(*d_fdisp)();		/* insert routine for fast dispatch */
	void			*d_profmem;			/* profiler (FasTrak) memory ptr	*/
	u_int32			d_rev8[6];			/* reserved 						*/
	u_int32			d_firqvct,			/* FIRQ routine/data pointer table  */
					d_vctjmp;			/* FIRQ vector/jump-target table	*/
	void			(*d_sysdbg)();		/* system debugger entry pt address	*/
	u_int32			*d_dbgmem;			/* system debugger memory ptr 		*/
	char 			d_dbgflg,			/* system debugger enabled flag 	*/
					d_alloctype;		/* memory allocator type			*/
	u_int16			d_devcnt;			/* system device count				*/
	u_int32			*d_cache;			/* disk block cache ptr 			*/
	u_int16			d_numsigs,			/* max number of queued signals		*/
					d_prcdescstack;		/* process descriptor stack size	*/
	struct fdisp	d_fdispsys;			/* system's fast dispatch block		*/
	u_int32			d_inhouse[8];		/* internal debugging space			*/
	char			d_kertyp,			/* kernel type (development/atomic)	*/
					d_inirq,			/* in irq context					*/
					d_firqoff,			/* FIRQ - offset to stack frame		*/
					d_irqoff,			/* IRQ - offset to stack frame		*/
					d_irqspoff,			/* IRQ - offset to (isp) stack ptr	*/
					d_rev16[3];			/* reserved							*/
	u_int32			d_mbar,				/* CPU32 MBAR value.				*/
					d_crystal;			/* system boot method flags			*/
	void			(*d_idle)(),		/* idle loop call out routine		*/
					*d_idledata;		/* idle loop call out data ptr		*/
	u_int32			d_switches,			/* context switch ctr for idle chk	*/
					d_rev17[5];			/* reserved							*/
	void			*d_irqheads[8];		/* irq recognition heads (non-MSP)	*/
	char			d_rev15[1678];		/* reserved							*/
	u_int32			d_last;				/* top of system abort stack		*/
} sysglobs, *Sysglobs;

typedef union glob_buff {
	u_char	byt;		/* 8-bit value */
	u_int16 wrd;		/* 16-bit value */
	u_int32 lng;		/* 32-bit value */
} glob_buff;

/* System memory allocator type (d_alloctype) definitions. */
#define MA_STD		1	/* standard allocator */
#define MA_BUDDY	2	/* buddy (binary) allocator */

/* memory type access bit definitions */
#define B_USER			(1<<0)	/* memory allocatable by user procs */ 
#define B_PARITY		(1<<1)	/* parity memory; must be initialized */
#define B_ROM			(1<<2)	/* read-only memory; searched for modules */
#define B_NVRAM	    	(1<<3)	/* non-volatile RAM; searched for modules */
#define B_SHARE	    	(1<<4)	/* shared memory */

/* Kernel type (d_kertyp) definitions */
#define	KT_DEV		1	/* Development kernel */
#define	KT_ATOM		2	/* Atomic kernel */

/* System configuration (d_sysconf) definitions */
#define	NOTBLEXP		(1<<0)	/* system table expansion disabled */
#define	CRCDIS			(1<<2)	/* disable CRC check for F$VModul (Atomic kernel ONLY) */
#define	SYSTSDIS		(1<<3)	/* disable system-state time-slicing */
#define	SSM_NOPROT		(1<<4)	/* single user-state page-table (no protection) */
#define	SSM_SYSPT		(1<<5)	/* enable system-state page tables */
#define	SSM_CMODE		(1<<6)	/* system-state cache mode: copy-back if set */
#define	SSM_PAGE8K		(1<<7)	/* use 8K page tables:  68040/68060 only */

/* System startup/boot method (d_crystal) flags */
/* Bits 0 - 23 are reserved for Microware definition */
/* Bits 24 - 31 are free for OEM boot method definition */

#if !defined(OSK_FASTBOOT_DEFS)
#define OSK_FASTBOOT_DEFS

#define	B_QUIKVAL		(1<<0)	/* just validate module headers in ROM at startup */
#define	B_OKRAM			(1<<1)	/* accept RAM definitions without probing */
#define	B_OKROM			(1<<2)	/* accept ROM definitions without probing */
#define	B_1STINIT		(1<<3)	/* accept first Init module found */
#define	B_NOIRQMASK		(1<<4)	/* don't mask interrupts during system startup */
#define	B_NODEBUGCALL	(1<<5)	/* don't call/init debugger during boot */
#define	B_SKIPPARITY	(1<<6)	/* skip memory parity init during startup */
#define	B_QUIET			(1<<7)	/* skip progress/info messages during boot */

#endif	/* !defined(OSK_FASTBOOT_DEFS) */

extern sysglobs sysdata;

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

error_code _os_getsys _OP((u_int32, u_int32, glob_buff *));
error_code _os_setsys _OP((u_int32, u_int32, glob_buff));

#if defined(_OPT_PROTOS)
int _getsys _OP((int, int));
int _setsys  _OP((int, int, int));
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _SYSGLOB_H */
