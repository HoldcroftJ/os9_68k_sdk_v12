/*--------------------------------------------------------------------------!
! if_xmit_qe: driver for quads if_xmit	    								!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 93/11/20 Cloned from other bootp stuff							arb	!
!---------------------------------------------------------------------------!
!                                                                           !
!         Copyright 1993 Microware Systems Corporation       				!
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/
#include <sysboot.h>
#include "qedvr.h"

/* if_send_ie - set up ethernet header then transmit packet
 */
 
if_send_qe(ifd, buf, mlen, ipdst)
struct ifdev *ifd;
char *buf;
int mlen;
int ipdst;
{
	struct ether_header *eh;

	eh = (struct ether_header *)(buf - sizeof(struct ether_header));
	
	eh->ether_type = ETHERTYPE_IP;
	memcpy(eh->ether_shost, ifd->if_paddr, 6);	/* ethernet source addr */
	memcpy(eh->ether_dhost, arp_resolve(ifd, ipdst), 6);

	if_xmit_qe(ifd, eh, mlen + sizeof(struct ether_header));
}

/* if_xmit_qe - transmit ready made packet
 */
 
if_xmit_qe(ifd, eh, mlen)
struct ifdev *ifd;
struct ether_header *eh;
int mlen;
{
	register Ifs ifstat = (Ifs) ifd->if_static;

	qe_xmit(eh,ifstat,mlen);	
	return 0;
}
