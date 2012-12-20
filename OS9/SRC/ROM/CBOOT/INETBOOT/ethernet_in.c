/*--------------------------------------------------------------------------!
! ethernet_in.c: handle link-level input for ethernet						!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!               ---- OS-9/68K V2.4 released ----                            !
!	01 90/08/08 Cloned from diskboot									kk	!
!	02 90/10/25 Returned when ICMP message received						kk	!
!	03 91/09/23 now returns residual timeout value						kk	!
!	04 93/08/09 replaced use of inetboot.h with explicit includes (4)	gkm !
!               ---- OS-9 V3.0 Released ----                                !
!   05 96/06/21 changed ifdev variable to ifd for ISP 2.1 defs compat   mgh !
!               ---- OS-9 V3.0.3 Released ----                              !
!   06 98/01/22 initialize rtype .. exiting do/while by accident        mgh !
!   12 98/02/17 Matched edition number to inetboot.l printing edition   mgh !
!   13 00/02/04 Added timestamp display									kimk!
!---------------------------------------------------------------------------!
!                                                                           !
!              Copyright 1990,1993 by Microware Systems Corporation         !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include <sysboot.h>
/*#include "if.h"*/
#include "lip.h"
#include "inetboot_p.h"
#include "bootp.h"

/* handle network input: remove link-level headers */
char *ethernet_in(ifd, timeout, ptype)
Ifdev ifd;
int *timeout;
int ptype;
{
	register struct ether_header *eh = (struct ether_header *)net_inpbuf;
	int rlen, rtype;
	static int rcount = 0;
	
	do {
/*		memset(net_inpbuf, 0x88, sizeof net_inpbuf); */
		rlen = (*ifd->if_recv)(ifd, net_inpbuf, timeout);
		if (rlen == 0) return 0;

		switch(eh->ether_type) {
		case ETHERTYPE_ARP:
			if (msgprint>=4) TS(), rprintf("ETHER_IN: type=%04X (ARP)\n", eh->ether_type); 
			rtype = arp_input(ifd, net_inpbuf);
			break;
		case ETHERTYPE_IP:
			if (msgprint>=4) TS(), rprintf("ETHER_IN: type=%04X (IP)\n", eh->ether_type); 
			rtype = ip_inp(ifd, net_inpbuf + sizeof(struct ether_header),
					rlen - sizeof(struct ether_header));
			break;
		default:
			if (msgprint>=4) TS(), rprintf("ETHER_IN: %04X <---unknown ETHER type!\n", eh->ether_type); 
			break;
		}
	} while (ptype != rtype && rtype != IPPROTO_ICMP);
/* ICMP means something is probably wrong */

	return net_inpbuf + sizeof(struct ether_header);
}


