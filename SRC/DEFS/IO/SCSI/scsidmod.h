/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1998 by Microware Systems Corporation              |
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
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
| 00  88/10/24 history started, added STATS.                           wwb  |
| 01  88/11/03 updated calling convention for "execute",               wwb  |
|               added low-level command defs.                               |
| 02  88/11/10 redid stats info.                                       wwb  |
| 03  89/02/01 modified for use with os-9000.                          djl  |
|              ---- SCSI Common Sub-Component v1.0 Released ----            |
| 04  99/11/18 Moved dm_port to reserved area to maintain 68k binary   mgh  |
|              compatibility with previous releases (V3.1 was broken)       |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

/*
 * defs for data module to be used by scsi
 */
 
typedef struct datmod {
	process_id	dm_pid;			/* the process id that has the module */
#if !defined(_OSK)
	u_char		*dm_port;		/* the port address */
#endif
	error_code	(*dm_init)();	/* initialize the scsi low level driver */
	error_code	(*dm_exec)();	/* execute function */
	error_code	(*dm_term)();	/* terminate the scsi low level driver */
	void		*dm_statics;	/* static storage for low level driver */
	u_int32		dm_statsize;	/* size of low level static storage */
	void		*dm_modhdr;		/* pointer to this module's header */
	u_int32		dm_devent;		/* event number for device */
	u_int32		dm_irqevt;		/* event number for irq */
	u_int32		dm_users;		/* number of user's of low level driver */

	/* statistical information:  irq records for low-level beasty.
	 * Any module needing to record statistical information about
	 * Scsi bus phases and Irqs can record the information here.
	 * Typically this will be done by a "low-level" module.
	 */
#if defined(_OSK)
	/* FOR 68k we provide dm_port but had to put it here for compatibility with */
	/* binaries Proprior to 3.1 (which broke compatibility) */
	u_char		*dm_port;		/* the port address */
	u_int32		dummy[1];		/* eye-alignment (stat_xxx offset = $70) */
#else
	u_int32		dummy[2];		/* eye-alignment (stat_xxx offset = $70) */
#endif
	u_int32		stat_wdirq;		/* total wd33c93 irqs seen */
	u_int32		stat_dmairq;	/* total dma irqs seen */
	u_int32		stat_noirq;		/* total "not my irq" calls */
	u_int32		stat_res1;		/* reserved */
	
	u_int32		stat_group1;	/* total successful completion status */
	u_int32		stat_group2;	/* total pause or aborted status */
	u_int32		stat_group3;	/* total terminated status */
	u_int32		stat_group4;	/* total service required status */
	
	u_int32		stat_g3unex;	/* group 3 unexpected info phase */
	u_int32		stat_g4unex;	/* group 4 unexpected REQ */
	u_int32		stat_g1succ;	/* group 1 select & xfer completed ok (0x16) */
	u_int32		stat_g4disc;	/* group 4 disconnect (0x85) */
	
	u_int32		stat_restart;	/* total restarts (u_int32ermediate status) */
	u_int32		stat_devbusy;	/* scsi bus busy/fatal */
	
	u_int32		stat_reserv[2];	/* 2 more longs as spare */
} *Datmod;

/*
 * parameters for low level driver functions
 *
 * u_int32 (*dm_init) (pd, pdmod)
 * Dev_list	dlp;		device list pointer
 * Datmod	*pdmod;		pointer to pointer to data module info
 *
 * Returns 0 for no error or an OS-9000 error code otherwise
 */
 
/*
 * error_code (*dm_exec) (dmod, cmd, atn, llcmd)
 * Datmod	dmod;		pointer to data module info
 * Cmdblk	cmd;		pointer to command block
 * u_int32		atn;		assert ATN flag
 * u_int32		llcmd;		low-level scsi bus command
 * 
 * Returns 0 for no error or an OS-9 error code otherwise.
 */

/*
 * Low-level SCSI Commands for Exec
 *
 * These commands are the SCSI function to perform, NOT
 * the (CDB1) command code
 */
#define	DEFAULT		-1		/* perform select & transfer (default) */

/*
 * ATN assertion flag
 */
#define FLG_NOATN	0		/* do not assert ATN */
#define FLG_ATN		1		/* assert ATN with command */

/*
 * error_code (*dm_term) (dlp, dmod)
 * Dev_list	dlp;		device list pointer
 * Datmod	dmod;		pointer to data module info
 *
 * Returns 0 for no error or an OS-9000 error code otherwise
 */


