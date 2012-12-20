/*
 * term - terminate use of the ncr53c94 low level module
 */

/*
 * Copyright 1992 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	"scsi53c94.h"
#include	<sbf.h>
#include    <sg_codes.h>

/*
 * term () - terminate use of low level driver
 */
 
int term (dd, dmod)
mod_dev					*dd;		/* device descriptor */
Datmod					dmod;		/* data module */
{
	register int		error;		/* the error code */
	register Lstat		lst;		/* low level statics */
	register int		portaddr;	/* temporary holder for port address */
	CntrlBlk            cnt;		/* command/control block */
	int					sink;		/* put junk here */
	int                 i;          /* loop counter */
#ifdef USEDMA
	Dmahardware			dmahw;		/* ptr to dma hardware */
#endif USEDMA
	char				name[12];	/* build event names here */
	extern void         reset_device();

	portaddr = ((int)rbfstat.v_sysio.v_port) & ~0xf;
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
#ifdef SYNCHRO		
		/* if any devices were synchronous, reset them */
		for( i = 0; i < 8; i++ ) {
			if( i == SELFID )
				continue;		/* skip our own ID */
			
			cnt = &lst->cntb[i];						/* control block     */
			if( cnt->syc_off )				/* if synchronus.... */
				reset_device( portaddr, cnt, lst, i );	/* .... reset it     */
		}
#endif SYNCHRO
		/* disable hardware devices */
#ifdef USEDMA
		if (lst->dmabase != NULL) {
			dmahw = lst->dmabase;				/* set dma ptr */
			dmahw->dh_ccr = CCR_STOP;			/* ensure dma off */
		}
#endif USEDMA
		/* remove the device from the irq polling system */
		if (sink = _f_irq (dd->_mvector, dd->_mpriority, 0, lst, 0))
			error = sink;					/* keep the error */
#ifdef USEDMA
#ifdef DMAIRQ
		/* remove the dma device from the irq polling system */
		if (sink = _f_irq (DMA340VECT, dd->_mpriority, 0, lst, 0))
			error = sink;					/* keep the error */
#endif DMAIRQ
#endif USEDMA
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

#ifdef SYNCHRO
/*
 *  reset_device() - send a bus device reset to the device.
 */
void reset_device( hw, cnt, lst, scsiid )
register Hardware hw;
register struct cntrlblk *cnt;
register Lstat lst;
register int scsiid;
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
	
	cnt->ll_cmd = SEL_ATN_STOP;			/* save command being sent */
	HW(destbusid) = scsiid;			/* set SCSI ID of target */
	delay();
	HW(fifo) = cnt->mess_out[1];
	delay();
#ifdef RECORD
	record(cnt->ll_cmd, lst);
#endif RECORD
	HW(command) = cnt->ll_cmd;
	delay();
	
	/* wait for done */
	sink = 0;
	while( sink != CBLOCKWAKE )
		_fev_wait(evid, CBLOCKWAKE, CBLOCKWAKE, &sink );
	
	/* free the block */
	cnt->cbk_status = CBK_BUSY;
	_fev_set( evid, CBLOCKFREE, FALSE );
	
	return;
}
#endif SYNCHRO
