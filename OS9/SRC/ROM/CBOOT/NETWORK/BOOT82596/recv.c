/*--------------------------------------------------------------------------!
! if_recv_ie: driver for i82596 if_recv			    						!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 91/12/13 Cloned from other Cboot stuff							djl	!
!   01 93/06/17 Modified for UCC compatibility mode.                    bat !
!               ---- OS-9 V3.0 Released ----                                !
!	02 98/01/14 Removed add of 5 tick add to etime .. caused the driver mgh !
!               to never timeout.
!---------------------------------------------------------------------------!
!                                                                           !
!         Copyright 1988,1989,1990,1993 by Microware Systems Corporation    !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/
#include <sysboot.h>
#include "ie.h"
#include <inetboot_p.h>

#define MIN_TIME_RETRY	   5	/* (.5 sec) min. time to poll for net input */

if_recv_ie(ifdev, mbuf, timeout)
register struct ifdev *ifdev;
char *mbuf;
int *timeout;
{
	register Ifs 	ifstat = (Ifs) ifdev->if_static;
	register RFD	*rfd, 				/* working rfd */
					*t_rfd;				/* rfd tail */
	register SCB	*scb = ifstat->v_scb;
	register int rlen = 0;
	register u_int32 etime;
					
/* initialize timer values */
/*  etime = bpt_curtick + *timeout + MIN_TIME_RETRY; /*ADD of 5 a problem*/
    etime = bpt_curtick + *timeout;
    
	
	rfd = ifstat->v_rfd_head;		/* start of rfd list */

	/* verify integrity of rfd list */
	if (((t_rfd = rfd->rfd_bePrev)->rfd_stat2 & RFD_LAST) == 0) {
		rprintf("ie: Lost end of rfd list in boot82596 !\n");
		while (1);					/* should never happen */
	}
		
/* wait until data comes in */
	while ((rfd->rfd_stat1 & (RFD_COMPLETE | RFD_OK)) == 0) {
		/*
		 * now let the receiver know that there is at least one free rfd 
		 */
		if (scb->scb_status & SCB_STATUS_RNR) {
			if (msgprint>=2) rprintf("ie: restart RU\n");
		 	scb->scb_cmd = SCB_RU_START_CMD | SCB_RNR_ACK;
			scb->rfa = (RFD *)ram_swapw(rfd);
			I82596_CHANATTN(ifstat);
		}
		if ((*timeout = etime - bpt_curtick) <= 0) return 0;
	}
		
/*
 * ack that we received the frame
 */
	scb->scb_cmd = SCB_FR_ACK;
	I82596_CHANATTN(ifstat);
	
	memcpy(mbuf, rfd->rfd_da, 
		rlen = (rfd->rfd_acount & RFD_SIZEMASK));
	rfd->rfd_stat1 = 0;		/* mark available for controller */
	rfd->rfd_acount = 0;
	rfd->rfd_bePrev->rfd_stat2 &= ~RFD_LAST;	/* move end of list */
	rfd->rfd_stat2 |= RFD_LAST;
	rfd = rfd->rfd_beNext;
	
	ifstat->v_rfd_head = rfd;							/* new head of list */	
	return rlen;
}

