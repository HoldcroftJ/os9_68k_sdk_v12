/*--------------------------------------------------------------------------!
! if_xmit_ie: driver for I82596 if_xmit	    								!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 91/12/13 Cloned from other VME374 stuff							djl	!
!   01 93/06/17 Modified for UCC compatibility mode.                    bat !
!               ---- OS-9 V3.0 Released ----                                !
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

/* if_send_ie - set up ethernet header then transmit packet
 */
 
if_send_ie(ifdev, mbuf, mlen, ipdst)
struct ifdev *ifdev;
char *mbuf;
int mlen;
int ipdst;
{
	struct ether_header *eh;

	eh = (struct ether_header *)(mbuf - sizeof(struct ether_header));
	
	eh->ether_type = ETHERTYPE_IP;
	memcpy(eh->ether_shost, ifdev->if_paddr, 6);	/* ethernet source addr */
	memcpy(eh->ether_dhost, arp_resolve(ifdev, ipdst), 6);

	if_xmit_ie(ifdev, eh, mlen + sizeof(struct ether_header));
}

/* if_xmit_ie - transmit ready made packet
 */
 
if_xmit_ie(ifdev, mbuf, mlen)
struct ifdev *ifdev;
char *mbuf;
int mlen;
{
	register Ifs					ifstat = (Ifs) ifdev->if_static;
	register struct ether_header	*eh;
	register CB						*cb;	/* command block */
	
	if (mlen > ETHER_MAX) {
		if (msgprint>=3) rprintf("ie: Excessively large packet for boot82596.\n");
		return 0;
	} else if (mlen < ETHER_MIN)
		mlen = ETHER_MIN;					/* force o.k. size */
	
	cb = ifstat->v_cbnext;					/* get command block pointer */
	while ((cb->cb_status & CB_BUSY) != 0);	/* shouldn't happen */
	
	eh = (struct ether_header *) mbuf;		/* get access to target address */
	memcpy(cb->cmd.tcb.da, eh->ether_dhost, 6);	/* copy target address */
	memcpy(cb->cmd.tcb.xmitdata, mbuf + sizeof (struct ether_header), 
		mlen - sizeof(struct ether_header));	
	cb->cmd.tcb.tbdptr = (TBD *) -1l;		/* set to all ones */
	cb->cmd.tcb.tcbcnt = mlen | TCB_EOF;	/* set the length */
	cb->cmd.tcb.zeroes = 0;					/* clear zero area */
	cb->cmd.tcb.tcblen = eh->ether_type;	/* get packet type ? */
	if (i82596CMDExec(ifstat, CB_CMD_XMIT, cb)) /* send the command */
		if (msgprint>=3) rprintf("ie: i82596 transmit status: %04x\n", cb->cb_status);
		
	
	return 0;
}
