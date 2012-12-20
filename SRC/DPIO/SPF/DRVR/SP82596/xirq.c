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
 *   1 ??/??/?? Creation ported from ISP                               mil
 *   4 08/27/97 Bug fix                                                taka
 *   7 12/03/97 Added code to support netstat utility                  jk
 *   8 01/21/98 Removed compiler and linker warnings                   jk
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *   9 04/06/98 Fixed a MAKE_LOCAL_ADDR_SWAP cast to u_int32           tmd
 *              (DDTS MHQco03924)
 *  11 06/22/99 Integrated Kei Thomsen's changes, fixed mbuf leak.     jk
 *  15 08/17/01 Count the Collisions.                                  KeiT
 ****************************************************************************/


#include <SP82596/defs.h>

/* xmit_irq - handle transmitter irq.
*/

void xmit_irq(Dev_list deventry)
{
	Spf_drstat	drstat = (Spf_drstat)(deventry->v_dr_stat);
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat);
	CB *cb, 	*ncb;		/* start of current chain */
	u_int16 	status, endchain = 0;
	int             err = 0;

	/* this is really the "CU went idle" routine rather than xmit_irq... */
	drstat->dr_iestat.cu_idle++;

	/*
	** when we first start up we use signals to sequence the driver
	** through the commands.  Some are fast as fast can be, others may
	** be as long as infinity: i.e. thinlan cable not connected.
	*/

	if (lustat->lu_sigproc)
    	_os_send(lustat->lu_sigproc, SIGWAKE);

	/*
	** don't get confused by the CU idle interrupt when
	** the first CA is given to the chip.  It executes an
	** implicit nop command when first started.
	*/
	if ((cb = lustat->lu_cb_head) == NULL)
		return;

	/*
	** now look at the completed command chain
	*/
	while (cb && !endchain) {
		status = cb->cb_status;
		if (status & CB_COMPLETE)
			drstat->dr_iestat.cmd_complete++;

		if (status & CB_BUSY) {
			if (lustat->lu_scb->scb_status & SCB_CU_ACTIVE)
			drstat->dr_iestat.cmd_cu_active++;
			drstat->dr_iestat.cmd_busy++;
		}
		if ((status & CB_COMPLETE) && (cb->cb_command & CB_CMDMASK) == CB_CMD_XMIT) {
			drstat->dr_iestat.xmit_irq++;
#ifndef PVTRAM
		if ((lustat->lu_chipmode & CM_XMIT_FLEX) && cb->cmd.tcb.xmbuf)
	    	m_free_p(cb->cmd.tcb.xmbuf);
#endif
	    /* check xmit status result */
	    status = cb->cb_status;
	    if (status & XMIT_LCOL) { drstat->dr_iestat.xmit_lcol++; err++;}
	    if (status & XMIT_LCAR) { drstat->dr_iestat.xmit_lcar++; err++;}
	    if (status & XMIT_LCTS) { drstat->dr_iestat.xmit_lcts++; err++;}
	    if (status & XMIT_UDMA) { drstat->dr_iestat.xmit_udma++; err++;}
	    if (status & XMIT_DEFR) { drstat->dr_iestat.xmit_defr++; err++;}
	    if (status & XMIT_BEAT) { drstat->dr_iestat.xmit_beat++; err++;}
	    if (status & XMIT_MTRY) {
			drstat->dr_iestat.xmit_mtry++;
			drstat->dr_iestat.xmit_coll += 16;
                        err++;
	    }
	    drstat->dr_iestat.xmit_coll += (status & XMIT_COLLMASK);

	    /* increment if_data counter */
	    lustat->if_oerrors += err;
		lustat->if_collisions += (status & XMIT_COLLMASK);
	}
	endchain = cb->cb_command & CB_LAST;	/* at end of current chain? */
#if defined (_MPFSH) /**/
    ncb = (CB *)SWAPWORDS(get_long ((u_int16 *)&cb->cb_leNext));
#else
    ncb = (CB *)SWAPWORDS((u_int32)cb->cb_leNext);
#endif

    if(ncb == NULL) { /* was this the end of the chain */
		if(lustat->lu_cb_next) {   /* is there a new chain  */
			ncb = lustat->lu_cb_next;    /* yep - start it */
			lustat->lu_cb_next = NULL;   /* zap !!! */
	    }
	} else {
		ncb =(CB *) MAKE_CPU_ADDR((u_int32)ncb);
	}

#ifdef PVTRAM
    cb->cb_leNext 	/* return CB to free list */
		= (CB *)MAKE_LOCAL_ADDR_SWAP ((u_int32)lustat->lu_cb_free);
    lustat->lu_cb_free = cb;
#else
    /*
     ** For XMIT_SIMP, check to see if the CB is really an MBUF,
     ** if not, just return the CB to the free list, otherwise get
     ** the mbuf pointer so it can be freed.
     */
	if (lustat->lu_chipmode & CM_XMIT_SIMP) {
#ifdef _MPFSH
		if (cmp_long ((u_int16 *)&cb->cmd.tcb.leTbdptr, MBUF_MARKER)) {
			copy_long ((u_int16 *)&cb->cb_leNext, MAKE_LOCAL_ADDR_SWAP((u_int32)lustat->lu_cb_free));
#else
			if (cb->cmd.tcb.leTbdptr != (TBD *)MBUF_MARKER) {
				cb->cb_leNext			/* return CB to free list */
					= (CB *)MAKE_LOCAL_ADDR_SWAP((u_int32)lustat->lu_cb_free);
#endif
				lustat->lu_cb_free = cb;
			} else {
	    		register Mbuf mbp;
#ifdef _MPFSH
	    		mbp = (void *)get_long ((u_int16 *)(((Mbuf*)cb)-1));
	    		DEBUG7 ((void *)0x7777, (void *)mbp);
	    		m_free_p(mbp);		/* ...stored at cb - 4 bytes */
#else
	    		mbp = *(((Mbuf*)cb)-1);	/* hack...mbuf ptr is... */
	    		m_free_p(mbp);		/* ...stored at cb - 4 bytes */
#endif
			}
		} else {
			/* return CB to free list */
#ifdef _MPFSH
			copy_long ((u_int16 *)&cb->cb_leNext, MAKE_LOCAL_ADDR_SWAP((u_int32)lustat->lu_cb_free));
#else
	  		cb->cb_leNext = (CB *)MAKE_LOCAL_ADDR_SWAP((u_int32)lustat->lu_cb_free);
#endif
	  		lustat->lu_cb_free = cb;
		}
#endif
    	cb = ncb;
    }

	if (cb) {	/* another chain ready? */
		lustat->lu_cb_head = cb;
		lustat->lu_scb->leCbl = (CB *)MAKE_LOCAL_ADDR_SWAP((u_int32)(cb));
		drstat->dr_iestat.xirqstarts++;
		drstat->dr_iestat.cu_starts++;
		lustat->lu_scb->scb_cmd |= SCB_CU_START_CMD;

		DEBUG2 ((void *)0, (void *)lustat->lu_scb);

		I82596_CHANATTN(lustat);
	} else {
		lustat->lu_scb->leCbl=lustat->lu_cb_head=lustat->lu_cb_next=NULL; /* nope! */
	}
#if defined (Flushing) && !defined(PVTRAM)
	_os_cache(4);               /* Flush data cache */
#endif
}
