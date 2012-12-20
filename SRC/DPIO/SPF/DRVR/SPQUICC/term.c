/****************************************************************************
 *  File:	@(#) term.c 1.7@(#)
 *	Date:	10/22/97
 ***************************************************************************
 *  (Old PVCS Header)
 *  $Workfile:   term.c  $
 *  $Revision:   18.1  $
 *  $Modtime:   23 Sep 1993 11:40:00  $
 *  $Date:   23 Sep 1993 11:40:56  $
 *  $Author:   brentt  
 *****************************************************************************
 * QUICC Terminate routine.                                                  *
 *****************************************************************************
 *            Copyright 1989-1996 by Microware Systems Corporation           *
 *                            All Rights Reserved                            *
 *                          Reproduced Under License                         *
 *                                                                           *
 *  This software is confidential property of Microware Systems Corporation, *
 *  and is provided under license for internal development purposes only.    *
 *  Reproduction, publication, distribution, or creation of derivative works *
 *  in any form to any party other than the licensee is strictly prohibited, *
 *  unless expressly authorized in writing by Microware Systems Corporation. *
 *****************************************************************************
 *  Edition History:                                                         *
 *   #   Date       Comments                                           By    *
 *  --- --------    ----------------------------------------------- -------- *
 *  01 10/07/96 Creation                                                     *
 *  03 03/05/97 Free up all the recv desc mbufs.                       sr    *
 *     02/19/97 <***** SPF LAN Comm Pack v3.0 Release*********>              *
 *     05/05/97 <***** SPF LAN Comm Pack v3.1 Release*********>              *
 *  09 08/11/97 Added SCC channel dependent code                        taka *
 *     10/22/97 <***** OS-9000 MBX BSP Release v1.0 **********>              *
 *     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              *
 *     02/11/00 Added CHECK_LOCK option                                 kimk *
 *              $$                    <RELEASE_INFO>                     $$  *
 ****************************************************************************/


#include <SPQUICC/defs.h>

error_code term_hdw(Dev_list deventry)
{
	Spf_lustat 	lustat = (Spf_lustat)(deventry->v_lu_stat);
	Mbuf        mb;
	QUICC_BD    *bd;
	u_int32     channel = lustat->lu_channel;
	int         i;

/* if we have already been called, forget it! */
	if (!lustat->lu_running)
		return E_SEEK;		/* (sic) */
			
/* stop things... */
	lustat->lu_running = 0;
	lustat->qp->pio_pcpar &= 0xfffe;	/* disable TENA */
	lustat->qp->scc_regs[channel].scc_gsmra = 0x1088000c;		/* disable RX & TX */
	lustat->qp->intr_cimr &= ~(INTR_SCC1>>channel);	/* disable irq */
	lustat->qp->scc_regs[channel].scc_sccm = 0;	/*  turn off all interrupts  */
	lustat->qp->scc_regs[channel].scc_scce = 0xffff;	/* Clear any events */

	bd = lustat->base_rx_bd;
	for(i=0 ; i < lustat->lu_nrdesc; i++) {
		/* Free up all the recv desc mbufs. */
		mb = (Mbuf)((unsigned)bd->buf - (unsigned)sizeof(mbuf)
	                                                 - (2*sizeof(Dev_list)));
		m_free(mb);
		bd++;
	}
#ifdef CHECK_LOCK
	return clrLock(lustat, channel);
#else

	return(SUCCESS);
#endif
}
