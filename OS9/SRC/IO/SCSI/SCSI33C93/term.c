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
| #   yy/mm/dd Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
| 62  01/01/31 Change v_port mask from bottom nibble to bottom byte    mgh  |
|                This edition history added                            mgh  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/
/* _______________________________ term.c _______________________________
 *
 *  This module contains the following functions:
 *
 * term(), the low level terminate use of the WD33C93 function.
 * reset_device(), sends a bus device reset to a device.
 */

/*
 * Copyright 1988, 1989, 1990, 1992, 1995 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	"scsi33c93.h"
#include	<sbf.h>
#include    <sg_codes.h>

/*
 * The terminate use of low level driver function
 *		Call with:
 *			dd,  device descriptor pointer
 *			dmod, data module 
 *		Return:
 *			return low-level statics 
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int term 
(
	mod_dev		*dd,
	Datmod		dmod
)
#else
int term (dd, dmod)
mod_dev                 *dd;
Datmod                  dmod;
#endif
{
	register int		error;		/* the error code */
	register Lstat		lst;		/* low level statics */
	register int		portaddr;	/* temporary holder for port address */
	CntrlBlk            cnt;		/* command/control block */
	int					sink;		/* put junk here */
	int                 i;          /* loop counter */

#if defined (USEDMA) && defined (VME147)
	Dmahardware			dmahw;		/* ptr to dma hardware */
#endif

	char				name[12];	/* build event names here */
	void         		reset_device();	/* function in this module */

	portaddr = ((int)rbfstat.v_sysio.v_port) & 0xffffff00;
	lst = dmod->dm_statics;			/* get pointer to stats */
	dmod->dm_users--;				/* decrement user count */
	error = 0;						/* default no error */
	
	/* get scsi controller ID so that we can get the control block */
	/* NOTE: Only RBF and SBF devices are currently supported */
	if( dd->_mdtype == DT_RBF ) {
		struct rbf_opt      *rbopt;		/* device descriptor options for RBF */

		rbopt = (struct rbf_opt *)&dd->_mdtype;
		cnt = (CntrlBlk)&lst->cntb[rbopt->pd_ctrlrid];
	} else if( dd->_mdtype == DT_SBF ) {
		struct sbf_opt      *sbopt;		/* device descriptor options for SBF */

		sbopt = (struct sbf_opt *)&dd->_mdtype;
		cnt = (CntrlBlk)&lst->cntb[sbopt->pd_scsiid];
	}

	/* if last user of data module, clean up */
	if (dmod->dm_users == 0) {
		
		/* if any devices were synchronous, reset them */
		for( i = 0; i < 8; i++ ) {
			if( i == SELFID )
				continue;		/* skip our own ID */
			
			cnt = &lst->cntb[i];						/* control block     */
			if( cnt->spsyc_register & 0xf )				/* if synchronus.... */
				reset_device( portaddr, cnt, lst, i );	/* .... reset it     */
		}
		
		/* disable hardware devices */
#ifdef VME147
		*((char *) SCSI_ICR) = 0;	 /* disable scsi interrupts from PCC */
#endif
#if defined (USEDMA) && defined (VME147)
		if (lst->dmabase != NULL) {
			dmahw = lst->dmabase;				/* set dma ptr */
			dmahw->dmactrl = 0;					/* ensure dma off */
		}
#endif
		/* remove the device from the irq polling system */
		if (sink = _f_irq (dd->_mvector, dd->_mpriority, 0, lst, 0))
			error = sink;					/* keep the error */

#if defined (USEDMA) && defined (DMAIRQ) && defined (VME147)
		/* remove the dma device from the irq polling system */
		if (sink = _f_irq (dd->_mvector + 1, dd->_mpriority, 0, lst, 0))
			error = sink;					/* keep the error */
#endif

		/* delete the IRQ events */
		for( i = 0; i < 8; i++ ) {
			if( lst->cntb[i].event_id != 0 ) {
				/* delete all interrupt events */
				name[0] = 'i';
				lxtoa(&(name[1]), portaddr);
				name[9] = '_';
				name[10] = '0' + i;
				name[11] = 0;
				_fev_unlnk (lst->cntb[i].event_id);		/* unlink the event */
				_fev_delet (name, &sink);		/* now remove the event */
				lst->cntb[i].event_id = 0;		/* remove record of existance */
			}

		}  /* end if no users left */

		/* return low-level statics */
		if (lst) {
			error = _f_srtmem (dmod->dm_statsize, lst);
			dmod->dm_statics = 0;			/* only return the memory once */
		}
	}
	
	/* unlink the data module */
	if (sink = _f_unlink (dmod->dm_modhdr))
		error = sink;						/* keep the error */

	return error;							/* return any/latest error */
}


/*
 *  function to send a bus device reset to a device.
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void reset_device
(
	register Hardware hw,
	register struct cntrlblk *cnt,
	register Lstat lst,
	register int scsiid
)
#else
void reset_device( hw, cnt, lst, scsiid )
register Hardware hw;
register struct cntrlblk *cnt;
register Lstat lst;
register int scsiid;
#endif
{
	register int evid;		/* event id */
	register int old_mask;	/* interrupt mask */
	register int error;		/* error storage */
	int sink;				/* bit bucket */

	evid = cnt->event_id;		/* the event for this device */
	sink = 0;
	if( error = _fev_wait(evid, CBLOCKFREE, CBLOCKFREE, &sink ) )
		return;				/* problems getting event */
	
	old_mask = mask_irq( lst->mask_level );
	error = _fev_set( evid, CBLOCKBUSY, FALSE );
	mask_irq( old_mask );
	
	if( error )
		return;			/* event problems again, just quit */
		
	/* set reset message */
	lst->curr_cmd = cnt;						/* flag this command */
	cnt->cbk_status = CBK_BUSY | CBK_RESET;		/* set block busy */
	cnt->mess_out[0] = 2;						/* send out 2 bytes */
	cnt->mess_out[1] = IDENTIFY_MESSAGE;		/* identify */
	cnt->mess_out[2] = BDRESET_MESSAGE;			/* bus device reset */
	
	cnt->ll_cmd = SEL_ATN;			/* save command being sent */
	HW(address_reg) = TARGLUN;		/* set SCSI LUN to zero */
	sink = 0;
	HW(reg_file) = sink;
	HW(address_reg) = DESTID;		/* set SCSI ID of target */
	HW(reg_file) = scsiid;
	HW(address_reg) = COMMAND;		/* start the command */
	HW(reg_file) = cnt->ll_cmd;
	
	/* wait for done */
	sink = 0;
	while( sink != CBLOCKWAKE )
		_fev_wait(evid, CBLOCKWAKE, CBLOCKWAKE, &sink );
	
	/* free the block */
	cnt->cbk_status = CBK_BUSY;
	_fev_set( evid, CBLOCKFREE, FALSE );
	
	return;
}
	
	

	
