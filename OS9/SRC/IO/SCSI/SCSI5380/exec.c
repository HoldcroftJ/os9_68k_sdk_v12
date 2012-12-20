/*
 * exec: execute scsi command for the ncr5380 low level module.
 */
 
/*
 * Copyright 1988, 1989, 1990, 1991, 1994 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee solely
 * for documentation and educational purposes. Reproduction, publication
 * or distribution in any form to any party other than the licensee is
 * strictly prohibited.
 */

#include	"scsi5380.h"

#ifdef USEDMA
#ifdef JNMS
/*  IO routines specific to then JNMS player */
#include "io_jnms.c"
#else
#ifdef PCB1782
/*  IO routines specific to then PCB1782 */
#include "io_1782.c"
#endif
#endif
#else
/* polled IO routines (No DMA support) */
#include "io_poll.c"
#endif

/*
 * exec: execute a SCSI command
 */
int	exec (proc, dmod, cmd, atn, llcmd)
procid		*proc;			/* process descriptor pointer */
Datmod		dmod;			/* data module pointer	 */
Cmdblk		cmd;			/* command block pointer */
int			atn;			/* assert ATN flag		 */
int			llcmd;			/* low-level scsi bus command */
{
	register Hardware		hw;			/* pointer to the hardware */
	register Lstat			lst;		/* low level statics */
	register error_code		error;		/* local error number */
	auto int 				sink, sink2;/* junk holder and counters */
	auto int				old_mask;	/* save of 68xxx sr	 */
	u_char					bus_phase;	/* SCSI bus phase	 */

	/*
	 * integrity check: check that passed 'llcmd' is valid for this version
	 * low level module: current implementation only does "select and transfer"
	 * commands.  Therefore 'llcmd' must be set to DEFAULT.
	 */
	if ((llcmd != DEFAULT) || (atn != FLG_NOATN))
		return E_PARAM;						/* signal parameter problem */

	/* ensure that the SCSI ID makes sense for this host adapter */
	if (cmd->_cb_scsi_id > MAXSCSI) return E_UNIT;
	
	/* get control of the SCSI device */
	while ((error = _fev_wait(dmod->dm_devent, 1, 1, &sink)) == 0 && sink != 1);
	if (error != SUCCESS)
		return error;						/* abort on error */

	lst = (Lstat) dmod->dm_statics;			/* get low level statics */

#ifdef USEDMA
#ifdef JNMS
	/* In order to support the possibility of a SCSI Bus Reset occuring...
	 */
	old_mask = mask_irq(lst->v_mask_level);	/* put a hold on things */
	_fev_set(lst->v_irqevt, 0, 0);			/* set the value back to zero */
	lst->v_CmdActive = TRUE;				/* mark chip busy */
	lst->v_SCSIReset = FALSE;				/* clear any SCSI Reset status */
	mask_irq(old_mask);						/* let it flow..... */
#endif JNMS
#endif USEDMA

	/* copy useful parameters */
	lst->v_dir = cmd->_cb_xfer;				/* set data direction flag */
	lst->v_datalen = cmd->_cb_datlen;		/* get amount of data to xfer */
	lst->v_cmdlen = cmd->_cb_cmdlen;		/* get length of command block */
	lst->v_buffer = (u_char *) cmd->_cb_datptr;	/* get ptr to data buffer */

	/* setup ncr5380 device for command */
	/* get address of hardware and mask off the embedded ID  (ed.7)*/
	hw = (Hardware)((long)rbfstat.v_sysio.v_port & SCSIIDMASK);
	HW(TARGCMND) = 0;						/* clear target cmd register */
	
	/* execute the command and return status or error to the driver */
	error = transact( hw, lst, dmod, cmd );
#ifdef USEDMA
#ifdef JNMS	
	lst->v_CmdActive = FALSE;		/* all done with command */
#endif JNMS
#endif USEDMA
	if ( !error ) {
		freedmod(lst->v_statbyte);	/* return status */
	} else {
		freedmod(error);			/* return error */
	}
}
