/****************************************************************************
 *  $Workfile:   ether.h  $
 *  $Revision:   1.15  $
 *  $Modtime:   17 Feb 1998 10:24:00  $
 *  $Date:   17 Feb 1998 10:26:48  $
 ****************************************************************************
 * Copyright (c) 1982, 1986 Regents of the University of California.		*
 * All rights reserved.														*
 *																			*
 * Redistribution and use in source and binary forms are permitted			*
 * provided that this notice is preserved and that due credit is given		*
 * to the University of California at Berkeley. The name of the University	*
 * may not be used to endorse or promote products derived from this			*
 * software without specific prior written permission. This software		*
 * is provided ``as is'' without express or implied warranty.				*
 *																			*
 *	@(#)if_ether.h	7.2 (Berkeley) 12/7/87									*
 ****************************************************************************
 *	Copyright 1993 by Microware Systems Corporation							*
 *	Reproduced Under License												*
 *																			*
 *	This source code is the proprietary confidential property of			*
 *	Microware Systems Corporation, and is provided to licensee				*
 *	solely for documentation and educational purposes. Reproduction,		*
 *	publication, or distribution in any form to any party other than		*
 *	the licensee is strictly prohibited.									*
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date   	Comments                                       	   By	*
 *  --- --------	-----------------------------------------------	------- *
 *	  0 ??/??/??	Birth											    ???	*
 *		09/16/93	<***** ISP 2.0 Release **************************>		*
 *	  1	10/21/93	Merged OS-9/OS-9000 sources							bat	*
 *	  2	07/29/94	moved include arp.h outside of ifdef				gkm *
 *		01/17/96	<***** ISP 2.1 Release **************************>		*
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release ***********>      *
 *      04/29/97    <***** OS-9000 V2.2 Release (Low Level ROM)***********> * 
 *                  ---- OS-9000/Brutus Board Support V1.0 Released ----    *
 ****************************************************************************/

#include <INET/arp.h>
 
#ifndef _ETHER_H
#define _ETHER_H

/*
 * Structure of a 10Mb/s Ethernet header.
 */
typedef struct	ether_header {
	u_char	ether_dhost[6];
	u_char	ether_shost[6];
	u_short	ether_type;
} ETH_HEADER, *Eth_header;

#define	ETHERTYPE_PUP	0x0200		/* PUP protocol */
#define	ETHERTYPE_IP	0x0800		/* IP protocol */
#define ETHERTYPE_ARP	0x0806		/* Addr. resolution protocol */
#define ETHERTYPE_RARP	0x0835		/* Reverse ARP */

/*
 * The ETHERTYPE_NTRAILER packet types starting at ETHERTYPE_TRAIL have
 * (type-ETHERTYPE_TRAIL)*512 bytes of data followed
 * by an ETHER type (as given above) and then the (variable-length) header.
 */
#define	ETHERTYPE_TRAIL		0x1000		/* Trailer packet */
#define	ETHERTYPE_NTRAILER	16

#define	ETHERMTU	1500
#define	ETHERMIN	(60-14)

/*
 * Ethernet Address Resolution Protocol.
 *
 * See RFC 826 for protocol description.  Structure below is adapted
 * to resolving internet addresses.  Field names used correspond to 
 * RFC 826.
 */
struct	ether_arp {
	struct	arphdr ea_hdr;	/* fixed-size header */
	u_char	arp_sha[6];	/* sender hardware address */
	u_char	arp_spa[4];	/* sender protocol address */
	u_char	arp_tha[6];	/* target hardware address */
	u_char	arp_tpa[4];	/* target protocol address */
};
#define	arp_hrd	ea_hdr.ar_hrd
#define	arp_pro	ea_hdr.ar_pro
#define	arp_hln	ea_hdr.ar_hln
#define	arp_pln	ea_hdr.ar_pln
#define	arp_op	ea_hdr.ar_op


/*
 * Structure shared between the ethernet driver modules and
 * the address resolution code.  For example, each ec_softc or il_softc
 * begins with this structure.
 */
struct	arpcom {
	struct 	ifdev *ac_if;		/* network-visible interface */
	u_char	ac_enaddr[6];		/* ethernet hardware address */
	struct in_addr ac_ipaddr;	/* copy of ip address- XXX */
};

/*
 * Internet to ethernet address resolution table.
 */
struct	arptab {
	struct	arptab *next, *prev;	/* chain pointers */
	struct	mbuf *at_hold;		/* last packet until resolved/timeout */
	int32	at_grant;			/* srqmem grant size */
	struct	in_addr at_iaddr;	/* internet address */
	u_char	at_enaddr[6];		/* ethernet address */
	u_char	at_timer;		/* minutes since last reference */
	u_char	at_flags;		/* flags */
};


#endif	/* _ETHER_H */
