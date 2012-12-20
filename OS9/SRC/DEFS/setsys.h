#if !defined(_SETSYS_H)
#define _SETSYS_H

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
| 01 96/08/02 History finally started.									wwb	|
| 02 98/07/29 Updated for new copyright info.							wwb	|
|				---- OS-9/68K V3.1 Release ----								|
|				$$			<RELEASE_INFO>			$$						|
`--------------------------------------------------------------------------*/

/*
	System global offsets for _setsys() and _getsys() functions.
*/

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#else
extern int _setsys(), _getsys();
#endif

#define D_ID		0x0000		/* modsync code after coldstart */
#define	D_NoSleep	0x0002		/* non-zero: sysproc won't sleep */
#define	D_Init		0x0020		/* init module pointer */
#define D_Clock		0x0024		/* pointer to system tick routine */
#define D_TckSec	0x0028		/* clock tickrate */
#define D_Year		0x002a		/* year */
#define D_Month		0x002c		/* month */
#define D_Day		0x002d		/* day */
#define D_Compat	0x002e		/* version compatibility byte #1 */
#define D_68881		0x002f		/* FPU Type (68020/030/040/060 systems) */
								/* 0=no FPU, 1=68881, 2=68882, 40=68040, 60=68060 */
#define D_Julian	0x0030		/* julian day */
#define D_Second	0x0034		/* seconds until midnight */
#define D_SysConf	0x0038		/* system configuration control */
#define D_IRQFlag	0x003a		/* IRQ flag */
#define D_UnkIRQ	0x003b		/* unknown IRQ count */
#define D_ModDir	0x003c		/* system module directory pointers */
#define D_PrcDBT	0x0044		/* process descriptor block table pointer */
#define D_PthDBT	0x0048		/* path descriptor block table pointer */
#define D_Proc		0x004c		/* ptr to current process descriptor */
#define D_SysPrc	0x0050		/* ptr to system process descriptor */
#define D_Ticks		0x0054		/* system heartbeat (current tick counter) */
#define D_FProc		0x0058		/* process context in FPU registers */
#define D_AbtStk	0x005c		/* system state panic stack ptr */
#define D_SysStk	0x0060		/* system state IRQ ptr */
#define D_SysROM	0x0064		/* bootstrap ROM entry point */
#define D_ExcJmp	0x0068		/* exception jump table ptr */
#define D_TotRAM	0x006c		/* total RAM available at startup */
#define D_MinBlk	0x0070		/* process minimun allocatable block size */
#define D_FreMem	0x0074		/* system free memory list head */
#define D_BlkSiz	0x007c		/* system minimum allocatable block size */
#define D_DevTbl	0x0080		/* I/O device table ptr */
#define	D_SpurIRQ	0x0084		/* spurious IRQ counter */
#define D_AutIRQ2	0x0088		/* 68070 on-chip Autovector table heads */
#define D_VctIRQ	0x00a4		/* Vectored IRQ device table ptrs */
#define D_SysDis	0x03a4		/* system service dispatch table ptr */
#define D_UsrDis	0x03a8		/* user service dispatch table ptr */
#define D_ActivQ	0x03ac		/* active process queue headnode */
#define D_SleepQ	0x03b4		/* sleeping process queue headnode */
#define D_WaitQ		0x03bc		/* waiting process queue headnode */
#define D_ActAge	0x03c4		/* active process queue age delta value */
#define D_MPUType	0x03c8		/* mpu type (68000/010/020/030/3XX/040/060/070) */
#define D_MPUTyp	0x03c8		/* mpu type (68000/010/020/030/3XX/040/060/070) */
#define D_EvTbl		0x03cc		/* system event table pointers */
#define D_EvID		0x03d4		/* next event ID number */
#define D_SPUMem	0x03d8		/* ptr to SPU global variables */
#define D_AddrLim	0x03dc		/* highest address found during startup */
#define D_Compat2	0x03e0		/* version compatibility byte #2 */
#define D_SnoopD    0x03e1		/* all d-caches are snoopy/coherent if non-zero */
#define D_ProcSz	0x03e2		/* size of a process descriptor */
#define D_PolTbl	0x03e4		/* I/O AutoVector Polling table heads */
#define D_FreeMem	0x0404		/* head of system free memory color node list */
#define D_IPID		0x040c		/* interprocessor identification number */
#define D_CPUs		0x0410		/* ptr to array of cpu descriptor list heads */
#define D_IPCmd 	0x0414		/* head of interprocessor command queue */
#define D_CachMode	0x0764		/* 68020/68030/68040 CACR mode */
#define D_DisInst	0x0768		/* instruction cache disable depth */
#define D_DisData	0x076c		/* data cache disable depth */
#define D_ClkMem	0x0770		/* ptr to clock tick thief's static storage */
#define D_Tick		0x0774		/* tick of second */
#define D_TSlice	0x0776		/* ticks per timeslice */
#define D_Slice		0x0778		/* ticks remaining in timeslice */
#define D_Elapse	0x077c		/* time (ticks) until system process wakes */
#define D_Thread	0x0780		/* system thread queue head (absolute) */
#define D_AlarTh	0x0788		/* system thread queue head (relative) */
#define D_SStkLm	0x0790		/* System IRQ stack low bound */
#define D_Forks 	0x0794		/* Number of actively forked processes */
#define	D_BootRAM	0x0798		/* ram found during bootrom search */
#define	D_FPUSize	0x079c		/* FPU (max) state frame size */
#define	D_FPUMem	0x07a0		/* FPU Emulator global data */
#define D_IOGlob	0x07a4		/* system hardware dependent I/O flags */
#define	D_DevSiz	0x08a4		/* device table entry size */
#define D_MinPty	0x08a6		/* system minimum priority */
#define D_MaxAge	0x08a8		/* system maximum natural age */
#define D_Sieze		0x08aa		/* ID of only process allowed to run */
#define D_Cigar		0x08ac		/* internal data structure format numbers */
#define	D_MMinLim	0x08b0		/* former min. mem. address allocatable */
#define	D_MMaxLim	0x08b4		/* former max. mem. address allocatable (+1) */
#define D_MoveMin	0x08b8		/* minimum count for DMA/move16 options */
#define D_Preempt	0x08bc		/* 0 = system state preemption allowed */
#define D_FDispQ	0x08c0		/* fast dispatch queue pointer */
#define D_FDisp		0x08c4		/* insert routine for the fast dispatch queue */
#define	D_ProfMem	0x08c8		/* profiler (FasTrak) memory ptr */

#define	D_SysLst	0x08e4		/* last user changable global */

#define D_FIRQVct	0x08e4		/* FIRQ routine/data pointer */
#define D_VctJmp	0x08e8		/* FIRQ vector/jump-target table */
#define D_SysDbg	0x08ec		/* system debugger entry point */
#define D_DbgMem	0x08f0		/* system debugger memory ptr */
#define D_DbgFlg	0x08f4		/* system debugger enabled flag */
#define D_AllocType 0x08f5		/* memory allocator type */
#define D_DevCnt	0x08f6		/* system device count */
#define D_Cache		0x08f8		/* disk block cache ptr */
#define D_NumSigs	0x08fc		/* default maximum number of queued signals */
#define D_PrcDescStack 0x08fe	/* default process descriptor system stack size */
#define D_FDispSys	0x0900		/* system' fast dispatch block */
#define D_KerTyp	0x0930		/* kernel type (developement/atomic) */
#define D_InIRQ		0x0931		/* IRQ level counter (0=not in IRQ context) */
#define	D_FIRQOff	0x0932		/* FIRQ - offset to stack frame */
#define	D_IRQOff	0x0933		/* IRQ - offset to stack frame */
#define	D_IRQSPOff	0x0934		/* IRQ - offset to (isp) stack ptr */
#define D_MBAR		0x0938		/* CPU32 MBAR value */
#define	D_Idle		0x0940		/* idle loop call out routine */
#define	D_IdleData	0x0944		/* idle loop call out data ptr */
#define	D_Switches	0x0948		/* context switch ctr for idle check */
#define	D_IRQHeads	0x0960		/* IRQ recognition heads (non-MSP) */

extern int _sysglob;			/* system global pointer */
#define sysglob(type,glob)		(*(type*)(_sysglob+(glob)))

#endif /* _SETSYS_H */
