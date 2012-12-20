/****************************************************************************
 *  $Workfile:   in.h  $
 *  $Revision:   1.15  $
 *  $Modtime:   17 Feb 1998 10:22:00  $
 *  $Date:   17 Feb 1998 10:26:02  $
 ****************************************************************************
 *	Constants and structures defined by the internet system,				*
 *	Per RFC 790, September 1981.											*
 ****************************************************************************
 * Copyright (c) 1982, 1986 Regents of the University of California.		*
 * All rights reserved.														*
 *																			*
 * Redistribution and use in source and binary forms are permitted			*
 * provided that the above copyright notice and this paragraph are			*
 * duplicated in all such forms and that any documentation,					*
 * advertising materials, and other materials related to such				*
 * distribution and use acknowledge that the software was developed			*
 * by the University of California, Berkeley.  The name of the				*
 * University may not be used to endorse or promote products derived		*
 * from this software without specific prior written permission.			*
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR			*
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED			*
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.		*
 *																			*
 *	@(#)in.h	7.6 (Berkeley) 6/29/88										*
 ****************************************************************************
 *  Copyright 1993 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date       Comments                                           By   *
 *  --- --------    ----------------------------------------------- ------- *
 *    0 ??/??/??    Birth                                               ???	*
 *      09/16/93    <***** ISP 2.0 Release ***********************>			*
 *    1 06/22/94    Added MAKE_INADDR(a,b,c,d) macro                    bat *
 *    2 09/01/95    Added C++ functionality                             pjw *
 *      01/17/96    <***** ISP 2.1 Release ***********************>			*
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release ***********>      *
 *      04/29/97    <***** OS-9000 V2.2 Release (Low Level ROM)***********> *
 *                  ---- OS-9000/Brutus Board Support V1.0 Released ----    *
 ****************************************************************************/


#ifndef _IN_H
#define _IN_H


#ifndef _TYPES_H
#include <types.h>						/* Get additional type definitions */
#endif


/*
 * Protocols
 */
#define	IPPROTO_IP			0		/* dummy for IP */
#define	IPPROTO_ICMP		1		/* control message protocol */
#define	IPPROTO_GGP			3		/* gateway^2 (deprecated) */
#define	IPPROTO_TCP			6		/* tcp */
#define	IPPROTO_EGP			8		/* exterior gateway protocol */
#define	IPPROTO_PUP			12		/* pup */
#define	IPPROTO_UDP			17		/* user datagram protocol */
#define	IPPROTO_IDP			22		/* xns idp */
#define	IPPROTO_RAW			255		/* raw IP packet */
#define	IPPROTO_MAX			256


/*
 * Ports < IPPORT_RESERVED are reserved for privileged processes (e.g. root).
 * Ports > IPPORT_USERRESERVED are reserved for servers, not necessarily
 *         privileged.
 */
#define	IPPORT_RESERVED		1024
#define	IPPORT_USERRESERVED	5000

/*
 * Link numbers
 */
#define	IMPLINK_IP			155
#define	IMPLINK_LOWEXPER	156
#define	IMPLINK_HIGHEXPER	158

/*
 * Internet address (a structure for historical reasons)
 */
struct in_addr {
	u_long			s_addr;
};

#if defined(_BIG_END)
#define MAKE_INADDR(a,b,c,d)   ( (a<<24) | (b<<16) | (c<<8) | (d) )
#else /* defined(_LIL_END) */
#define MAKE_INADDR(a,b,c,d)   ( (d<<24) | (c<<16) | (b<<8) | (a) )
#endif

/*
 * Definitions of bits in internet address integers.
 * On subnets, the decomposition of addresses to host and net parts
 * is done according to subnet mask, not the masks here.
 */
#define	IN_CLASSA(i)		(((long)(i) & 0x80000000) == 0)
#define	IN_CLASSA_NET		0xff000000
#define	IN_CLASSA_NSHIFT	24
#define	IN_CLASSA_HOST		0x00ffffff
#define	IN_CLASSA_MAX		128

#define	IN_CLASSB(i)		(((long)(i) & 0xc0000000) == 0x80000000)
#define	IN_CLASSB_NET		0xffff0000
#define	IN_CLASSB_NSHIFT	16
#define	IN_CLASSB_HOST		0x0000ffff
#define	IN_CLASSB_MAX		65536

#define	IN_CLASSC(i)		(((long)(i) & 0xe0000000) == 0xc0000000)
#define	IN_CLASSC_NET		0xffffff00
#define	IN_CLASSC_NSHIFT	8
#define	IN_CLASSC_HOST		0x000000ff

#define	IN_CLASSD(i)		(((long)(i) & 0xf0000000) == 0xe0000000)
#define	IN_MULTICAST(i)		IN_CLASSD(i)

#define	IN_EXPERIMENTAL(i)	(((long)(i) & 0xe0000000) == 0xe0000000)
#define	IN_BADCLASS(i)		(((long)(i) & 0xf0000000) == 0xf0000000)

#define	INADDR_ANY			(u_long)0x00000000
#define	INADDR_BROADCAST	(u_long)0xffffffff	/* must be masked */
#ifndef KERNEL
#define	INADDR_NONE			0xffffffff		/* -1 return */
#endif

#define	IN_LOOPBACKNET		127			/* official! */

/*
 * Socket address, internet style.
 */
struct sockaddr_in {
	short			sin_family;
	u_short			sin_port;
	struct in_addr	sin_addr;
	char			sin_zero[8];
};

/*
 * Options for use with [gs]etsockopt at the IP level.
 */
#define	IP_OPTIONS			1		/* set/get IP per-packet options */

/*
 * Function prototypes
 */
#if (defined(_ANSI_EXT) || defined(__STDC__)) || defined(__cplusplus)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
u_long htonl(u_long hostlong);
u_short htons(u_short hostshort);
u_long ntohl(u_long netlong);
u_short ntohs(u_short netshort);
#if defined(__cplusplus)
}
#endif /* __cplusplus*/
#else
u_long htonl();
u_short htons();
u_long ntohl();
u_short ntohs();
#endif

/*
 * Macros for number representation conversion.  List the byte-swapped
 * machines in the condition above and provide library functions to do
 * the actual swap.
 */
#if defined(_BIG_END)	/* no byte-swapping needed -- use macros for speed */
#define	ntohl(hostlong)		(hostlong)
#define	ntohs(hostshort)	(hostshort)
#define	htonl(netlong)		(netlong)
#define	htons(netshort)		(netshort)
#endif


#endif	/* _IN_H */
