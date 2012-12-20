/*
 * definitions for SCSI Driver System Data Module.
 *
 * Copyright 1988, 1989, 1995 by Microware Systems Corporation
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
 * ed.  date.   reason.                                       who.
 * -- --------  --------------------------------------------  ---
 * 00 88/10/24  history started, added STATS.                 wwb
 * 01 88/11/03  updated calling convention for "execute",     wwb
 *              added low-level command defs.
 * 02 88/11/10  redid stats info.                             wwb
 * 03 89/05/11  commented FLG_ usage as archaic.              wwb
 *				---- OS-9/68K V3.0 Release ----
 * 04 95/03/11  Added OS9000 support                          GbG
 * 05 95/07/11  Deleted OS9000 support (use OS9000 version)   GbG
 *				---- OS-9/68K V3.0.2 Release ----
 *				---- OS-9/68K V3.1 Release ----
 */

/*
 * defs for data module to be used by scsi
 */
 
typedef struct datmod {
	int		dm_pid;			/* the process id that has the module */
	int		(*dm_init)();	/* initialize the scsi low level driver */
	int		(*dm_exec)();	/* execute function */
	int		(*dm_term)();	/* terminate the scsi low level driver */
	void	*dm_statics;	/* static storage for low level driver */
	int		dm_statsize;	/* size of low level static storage */
	void	*dm_modhdr;		/* pointer to this module's header */
	int		dm_devent;		/* event number for device */
	int		dm_irqevt;		/* event number for irq */
	int		dm_users;		/* number of user's of low level driver */

	/* statistical information:  irq records for low-level beasty.
	 * Any module needing to record statistical information about
	 * Scsi bus phases and Irqs can record the information here.
	 * Typically this will be done by a "low-level" module.
	 */
	int		dummy[2];		/* eye-alignment (stat_xxx offset = $70) */
	int		stat_wdirq;		/* total wd33c93 irqs seen */
	int		stat_dmairq;	/* total dma irqs seen */
	int		stat_noirq;		/* total "not my irq" calls */
	int		stat_res1;		/* reserved */
	
	int		stat_group1;	/* total successful completion status */
	int		stat_group2;	/* total pause or aborted status */
	int		stat_group3;	/* total terminated status */
	int		stat_group4;	/* total service required status */
	
	int		stat_g3unex;	/* group 3 unexpected info phase */
	int		stat_g4unex;	/* group 4 unexpected REQ */
	int		stat_g1succ;	/* group 1 select & xfer completed ok (0x16) */
	int		stat_g4disc;	/* group 4 disconnect (0x85) */
	
	int		stat_restart;	/* total restarts (intermediate status) */
	int		stat_devbusy;	/* scsi bus busy/fatal */
	
	int		stat_reserv[2];	/* 2 more longs as spare */
} *Datmod;

/*
 * parameters for low level driver functions
 *
 * int (*dm_init) (dd, pdmod)
 * mod_dev	*dd;		pointer to device descriptor
 * Datmod	*pdmod;		pointer to pointer to data module info
 *
 * Returns 0 for no error or an OS-9 error code otherwise
 */
 
/*
 * int (*dm_exec) (proc, dmod, cmd, atn, llcmd)
 * procid	*proc;		process descriptor pointer
 * Datmod	dmod;		pointer to data module info
 * Cmdblk	cmd;		pointer to command block
 * int		atn;		assert ATN flag
 * int		llcmd;		low-level scsi bus command
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
 *
 * NOTE:  these definitions are now OBSOLETE - the usage of
 *        ATN, etc is defined via the "scsiopts" field in
 *        the device descriptor.  These defs are retained
 *        only for compiling old version sources.
 */
#define FLG_NOATN	0		/* do not assert ATN */
#define FLG_ATN		1		/* assert ATN with command */

/*
 * int (*dm_term) (dd, dmod)
 * mod_dev	*dd;		pointer to device descriptor module 
 * Datmod	dmod;		pointer to data module info
 *
 * Returns 0 for no error or an OS-9 error code otherwise
 */


