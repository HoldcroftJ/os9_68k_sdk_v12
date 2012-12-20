/****************************************************************************
 *  $Workfile:   arp.h  $
 *  $Revision:   1.13  $
 *  $Modtime:   29 Apr 1997 10:13:00  $
 *  $Date:   29 Apr 1997 10:16:20  $
 ****************************************************************************
 *	Address Resolution Protocol.											*
 *																			*
 *	See RFC 826 for protocol description.  ARP packets are variable			*
 *	in size; the arphdr structure defines the fixed-length portion.			*
 *	Protocol type values are the same as those for 10 Mb/s Ethernet.		*
 *	It is followed by the variable-sized fields ar_sha, arp_spa,			*
 *	arp_tha and arp_tpa in that order, according to the lengths				*
 *	specified.  Field names used correspond to RFC 826.						*
 ****************************************************************************
 * Copyright (c) 1986 Regents of the University of California.				*
 * All rights reserved.														*
 *																			*
 * Redistribution and use in source and binary forms are permitted			*
 * provided that this notice is preserved and that due credit is given		*
 * to the University of California at Berkeley. The name of the University	*
 * may not be used to endorse or promote products derived from this			*
 * software without specific prior written permission. This software		*
 * is provided ``as is'' without express or implied warranty.				*
 *																			*
 *	@(#)if_arp.h	7.2 (Berkeley) 12/30/87									*
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
 *    2 06/21/94    Added stuff from <if_arp.h> 						bat	*
 *      01/17/96    <***** ISP 2.1 Release **************************>      *
 *    3 09/19/96    Added busy and count flag to arpstat for ifloop     pjw *
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release ***********>      *
 *      04/29/97    <***** OS-9000 V2.2 Release (Low Level ROM)***********> * 
 ****************************************************************************/

#ifndef _ARP_H
#define _ARP_H


#include <types.h>


struct	arphdr {
	u_int16	ar_hrd;		/* format of hardware address */
		#define ARPHRD_ETHER 	1	/* ethernet hardware address */
	u_int16	ar_pro;		/* format of protocol address */
	u_int8	ar_hln;		/* length of hardware address */
	u_int8	ar_pln;		/* length of protocol address */
	u_int16	ar_op;		/* one of: */
		#define	ARPOP_REQUEST	1	/* request to resolve address */
		#define	ARPOP_REPLY	2	/* response to previous request */

	/* remaining fields are variable in size, according to the sizes above */
	#if 0	/* not used by MW */
		u_int8	ar_sha[];	/* sender hardware address */
		u_int8	ar_spa[];	/* sender protocol address */
		u_int8	ar_tha[];	/* target hardware address */
		u_int8	ar_tpa[];	/* target protocol address */
	#endif
};

/*
**	ARP ioctl request
*/
struct arpreq {
	struct	sockaddr	arp_pa;				/* protocol address */
	struct	sockaddr	arp_ha;				/* hardware address */
	u_int32				arp_flags;			/* flags */
};

/*  arp_flags and at_flags field values */
#define	ATF_INUSE		0x01	/* entry in use */
#define ATF_COM			0x02	/* completed entry (enaddr valid) */
#define	ATF_PERM		0x04	/* permanent entry */
#define	ATF_PUBL		0x08	/* publish entry (respond for other host) */
#define	ATF_USETRAILERS	0x10	/* has requested trailers */
#define ATF_FOUND		0x80	/* unsolicited ARP request */


/*
**	Static values for if_arp.c
*/

#include <INET/ether.h>

/* arp hash table size */
#define MAXTARP		19

struct arpstat {
	struct arptab	arproot;
	u_int32			grant_size;
	alarm_id		arp_alarm;
	u_int8          busy_flag;
	int16           busy_count;
};


#endif	/* _ARP_H */
