/*--------------------------------------------------------------------------!
! larp.c: low-level implementation of ARP (ethernet) protocol				!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!               ---- OS-9/68K V2.4 released ----                            !
!	01 90/08/08 Cloned from diskboot									kk	!
!   02 93/06/10 Modified for UCC compatibility mode.                    bwt !
!   03 93/08/09 replaced use of inetboot.h with explicit includes (4)   gkm !
!               ---- OS-9 V3.0 Released ----                                !
!   04 96/06/21 changed ifdev variable to ifd for ISP 2.1 defs compat   mgh !
!               ---- OS-9 V3.0.3 Released ----                              !
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

/* one entry ARP cache */
int arp_dest_ip;
char arp_dest_hwaddr[16];

#define ETHER_BROADADDR "\xff\xff\xff\xff\xff\xff"

/*
 * handle incoming ARP packets
 */
arp_input(ifd, mbuf)
register struct ifdev *ifd;
char *mbuf;
{
	register struct ether_header *eh = (struct ether_header *)mbuf;
	struct ether_arp *ea =
		(struct ether_arp *)(mbuf + sizeof(struct ether_header));
	struct in_addr targ_addr;
	int llen, status = 0;
				
#ifdef DEBUG_ARP
	printf(ea->arp_hrd);
	printf("arp (%s) %s received:\n",
		ea->arp_hrd == ARPHRD_ETHER ? "ethernet" : "argh!",
		ea->arp_op == ARPOP_REQUEST ? "REQUEST" :
		ea->arp_op == ARPOP_REPLY ? "REPLY" : "(huh?)");
/* source address */
	printf("\tsha: %s\n", show_ether(ea->arp_sha));
	memcpy(&targ_addr, ea->arp_spa, sizeof targ_addr);
	printf("\tspa: %s\n", inetname(targ_addr, 1));

/* target address */
	printf("\ttha: %s\n", show_ether(ea->arp_tha));
	memcpy(&targ_addr, ea->arp_tpa, sizeof targ_addr);
	printf("\ttpa: %s\n...", inetname(targ_addr, 1));
#else
	memcpy(&targ_addr, ea->arp_tpa, sizeof targ_addr);
#endif

/* someone requesting our IP address? */
	if (ea->arp_op == ARPOP_REQUEST &&
			targ_addr.s_addr == (IA_SIN(ifd)->sin_addr).s_addr) {
/* fill in arp reply */
		memcpy(ea->arp_tha, ea->arp_sha, sizeof(ea->arp_sha));
		memcpy(ea->arp_sha, ifd->if_paddr, sizeof(ea->arp_sha));
		memcpy(ea->arp_tpa, ea->arp_spa, sizeof(ea->arp_spa));
		memcpy(ea->arp_spa, &targ_addr, sizeof(ea->arp_spa));
		ea->arp_op = htons(ARPOP_REPLY); 
/* fill in link-level ethernet header */
		memcpy(eh->ether_dhost, ea->arp_tha, sizeof(eh->ether_dhost));
		memcpy(eh->ether_shost, ea->arp_sha, sizeof(eh->ether_shost));
		eh->ether_type = ETHERTYPE_ARP;
/* pad the short packet to minimum length */
		llen = sizeof(struct ether_header) + sizeof(struct ether_arp);
		memset(mbuf + llen ,0, ETHER_MIN - llen);
		llen = ETHER_MIN;
/* dumpmap(eh, llen, 0); */
		(*ifd->if_xmit)(ifd, 	eh, llen);
#ifdef DEBUG_ARP
		printf("sent response to: %s\n...", inetname(targ_addr, 1));
#endif
	} else if (ea->arp_op == ARPOP_REPLY &&
				memcmp(ea->arp_spa, &arp_dest_ip, sizeof(ea->arp_spa)) == 0) {
/* answer to our prayers! */
		memcpy(arp_dest_hwaddr, ea->arp_sha, sizeof(ea->arp_sha));
#ifdef DEBUG_ARP
		printf("got reply from: %s\n...", inetname(targ_addr, 1));
#endif
		status = ARPOP_REPLY;
	}			
	return status;
}

/*
 * Broadcast an ARP packet, asking who has addr on interface ifdev.
 */
void arp_whohas(ifd, addr)
struct ifdev *ifd;
int addr;
{
	register struct ether_header *eth_pkt;
	register struct ether_arp *arp_pkt;
#define ARPMSG_SIZE		(sizeof(struct ether_header) + sizeof(struct ether_arp))
	char arpoutbuf[ARPMSG_SIZE + 128 /* safety fudge! */ ];
		
	eth_pkt = (struct ether_header *)arpoutbuf;
	arp_pkt = (struct ether_arp *)(eth_pkt + 1);
	memset(eth_pkt, 0, ARPMSG_SIZE);

/*
	set up ethernet broadcast header
	to: broadcast, from: us, type: ARP
*/
	memcpy(eth_pkt->ether_dhost, ETHER_BROADADDR, sizeof(eth_pkt->ether_dhost));
	memcpy(eth_pkt->ether_shost, ifd->if_paddr, sizeof(arp_pkt->arp_sha));
	eth_pkt->ether_type = ETHERTYPE_ARP;		/* if_output will swap */

/*
	fill in arp request
*/
	arp_pkt->arp_hrd = htons(ARPHRD_ETHER);
	arp_pkt->arp_pro = htons(ETHERTYPE_IP);
	arp_pkt->arp_hln = sizeof(arp_pkt->arp_sha);	/* hardware address length */
	arp_pkt->arp_pln = sizeof(arp_pkt->arp_spa);	/* protocol address length */
	arp_pkt->arp_op = htons(ARPOP_REQUEST);
	memcpy(arp_pkt->arp_sha, ifd->if_paddr, sizeof(arp_pkt->arp_sha));
	memcpy(arp_pkt->arp_spa, &IA_SIN(ifd)->sin_addr,sizeof(arp_pkt->arp_spa));
	memcpy(arp_pkt->arp_tpa, &addr, sizeof(arp_pkt->arp_tpa));
	(*ifd->if_xmit)(ifd, arpoutbuf, ARPMSG_SIZE);/* give packet to hardware */
}

/*
 * Use ARP to find hw address for IP address
 */
arp_resolve(ifd, ipdst)
Ifdev ifd;
int ipdst;
{
	auto int timeout = 100;	/* wait 10 seconds (but maybe backoff is better) */
	
	if (ipdst != INADDR_BROADCAST) {
		if (ipdst != arp_dest_ip) {
			arp_dest_ip = ipdst;	/* stamp IP address we're looking for */
			do {
				arp_whohas(ifd, ipdst);
			} while ((*net_in)(ifd, &timeout, ARPOP_REPLY) == 0);
		}
		return (int)arp_dest_hwaddr;
	} else return (int)ETHER_BROADADDR;
}
