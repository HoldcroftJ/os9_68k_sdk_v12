/*
 * definitions for SCSI direct command.
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
 * history:
 *
 * ed.  date   reason.                                         who.
 * -- -------  ----------------------------------------------  ---
 * 00 90/04/04 history starts.                                 Rwb
 *				---- OS-9/68K V3.0 Release ----
 *				---- OS-9/68K V3.1 Release ----
 *
 *
 *  Direct command for Microware SCSI drivers.
 *
 *		To use the direct command facilities, the user must
 *			1) open a path to the device
 *          2) fill in the command block structure (cmdblk below).
 *          3) fill in the SCSI packet to be sent  ( scsipkt below ).
 *          4) call the driver via  _gs_dcmd for input, _ss_dcmd for
 *             output.
 *
 * 		The user may fill in the error pointer and error length fields
 *		of the command block.  In the case that the command results in
 *      a check condition, the results will be returned to the user at
 *      the location described in the command block.  For the best
 *      results is is best to consult the manual for the device to be
 *      used and define a structure that reflects the "request sense"
 *      data from the device.
 *		
 *		Warning:  Not all errors will return sense information!
 *      
 *		The use of direct command is restricted to the "super group"
 *  user.  A further restriction is that the direct command be issued
 *  to the path that is opened.  This will prevent easy access to devices
 *  that are otherwise restricted.
 */

#ifndef _types
#include <types.h>
#endif

#include	<scsicmds.h>
#include    <dcmd.h>

/*
 * general definitions
 */

#define		TRUE		1		/* for boolean use */
#define		FALSE		0

/*
 * scsi command packet types
 */
#define		CDB_STD		6		/* std cdb size */
#define		CDB_EXT		10		/* extended cdb size */

/*
 * scsi command block definitions.
 * this block is the common communication block used to
 * communicate between the high-level driver and the
 * low-level module.
 */ 
typedef struct cmdblk {
	void	*_cb_cmdptr;		/* pointer to the scsi command */
	void	*_cb_datptr;		/* pointer to the data area */
	void	*_cb_errptr;		/* pointer to the error data area */
	u_int	_cb_cmdlen;			/* length of the command block */
	u_int	_cb_datlen;			/* expected data length */
	u_int	_cb_errlen;			/* expected error length */
	u_int	_cb_scsi_id;		/* the target id  (filled in by driver) */
	u_char	_cb_pd_lun;			/* the target lun ( also filled in by driver )*/
	u_char	_cb_xfer;			/* data input/output flag */
#define		INPUT		0
#define		OUTPUT		1
} *Cmdblk;

/*
 * scsi command packet for low-level module
 */
union scsipkt {
	struct std_str {
		u_char		sp_opcode;			/* the opcode */
		unsigned	sp_lun		: 3;	/* logical unit (filled in by driver)*/
		unsigned	sp_lbamsbs	: 5;	/* the block address (msbs) */
		char		sp_lba[2];			/* (the rest of) the block address */
		char		sp_blkcnt;			/* the block count */
		u_char		sp_options;			/* command options */
	} std;
	struct ext_str {
		u_char		sp_opcode;			/* the opcode */
		unsigned	sp_lun		: 3;	/* logical unit (filled in by driver)*/
		unsigned	sp_zero		: 5;	/* zero */
		u_int		sp_lba;				/* logical block address */
		u_char		sp_zero2;			/* zero */
		u_char		sp_count[2];		/* transfer count */
		u_char		sp_options;			/* command options */
	} ext;
};
