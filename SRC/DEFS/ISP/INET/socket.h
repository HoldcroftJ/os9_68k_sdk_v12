/****************************************************************************
 *  $Workfile:   socket.h  $
 *  $Revision:   1.17  $
 *  $Modtime:   17 Feb 1998 10:23:00  $
 *  $Date:   17 Feb 1998 10:26:30  $
 ****************************************************************************
 *	Definitions related to sockets: types, address families, options.		*
 ****************************************************************************
 * Copyright (c) 1982, 1985, 1986 Regents of the University of California.	*
 * All rights reserved.														*
 *																			*
 * Redistribution and use in source and binary forms are permitted			*
 * provided that this notice is preserved and that due credit is given		*
 * to the University of California at Berkeley. The name of the University	*
 * may not be used to endorse or promote products derived from this			*
 * software without specific prior written permission. This software		*
 * is provided ``as is'' without express or implied warranty.				*
 *																			*
 *	@(#)socket.h	7.2 (Berkeley) 12/30/87									*
 ****************************************************************************
 *  Copyright 1988 by Microware Systems Corporation                         *
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
 *	  1	11/10/88	Ported												aln	*
 *	  2	08/09/93	ASNI-ized                                           bat	*
 *      09/16/93    <***** ISP 2.0 Release ***********************>			*
 *	  3 05/03/94	Added microware spcific option SO_NOBLOCK			gkm *
 *    4 05/16/94    Added _gs_pcmd()/_ss_pcmd()/_gs_dcmd()/_ss_dcmd()       *
 *                    prototypes										bat *
 *    5 09/13/94    Added PF_ETHER protocol family type 				bat *
 *	  6	01/19/95	uncomment SO_NOBLOCK								gkm *
 *	  7	00/01/95    Added C++ Functionality                             pjw *
 *      01/17/96    <***** ISP 2.1 Release ***********************>			*
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release ********>         *
 *      04/29/97    <***** OS-9000 V2.2 Release (Low Level ROM)***********> * 
 *                  ---- OS-9000/Brutus Board Support V1.0 Released ----    *
 ****************************************************************************/


#ifndef _SOCKET_H
#define _SOCKET_H


#ifndef _TYPES_H
#include <types.h>					/* Get additional type definitions */
#endif

#ifndef _IN_H
#include <INET/in.h>				/* Get additional InterNet definitions */
#endif


/*
 * Types
 */
#define	SOCK_STREAM		1			/* stream socket */
#define	SOCK_DGRAM		2			/* datagram socket */
#define	SOCK_RAW		3			/* raw-protocol interface */
#define	SOCK_RDM		4			/* reliably-delivered message */
#define	SOCK_SEQPACKET	5			/* sequenced packet stream */

/*
 * Option flags per-socket.
 */
#define	SO_DEBUG		0x0001		/* turn on debugging info recording */
#define	SO_ACCEPTCONN	0x0002		/* socket has had listen() */
#define	SO_REUSEADDR	0x0004		/* allow local address reuse */
#define	SO_KEEPALIVE	0x0008		/* keep connections alive */
#define	SO_DONTROUTE	0x0010		/* just use interface addresses */
#define	SO_BROADCAST	0x0020		/* permit sending of broadcast msgs */
#define	SO_USELOOPBACK	0x0040		/* bypass hardware when possible */
#define	SO_LINGER		0x0080		/* linger on close if data present */
#define	SO_OOBINLINE	0x0100		/* leave received OOB data in line */

/*
 * Additional options, not kept in so_options.
 */
#define SO_SNDBUF		0x1001		/* send buffer size */
#define SO_RCVBUF		0x1002		/* receive buffer size */
#define SO_SNDLOWAT		0x1003		/* send low-water mark */
#define SO_RCVLOWAT		0x1004		/* receive low-water mark */
#define SO_SNDTIMEO		0x1005		/* send timeout */
#define SO_RCVTIMEO		0x1006		/* receive timeout */
#define	SO_ERROR		0x1007		/* get error status and clear */
#define	SO_TYPE			0x1008		/* get socket type */

/*
 * Microware specific options
 */
#define SO_NOBLOCK		0x8001		/* set socket as non blocking */


/*
 * Structure used for manipulating linger option.
 */
struct	linger {
	int				l_onoff;		/* option on/off */
	int				l_linger;		/* linger time */
};

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define	SOL_SOCKET		0xffff		/* options for socket level */

/*
 * Address families.
 */
#define	AF_UNSPEC		0			/* unspecified */
#define	AF_UNIX			1			/* local to host (pipes, portals) */
#define	AF_INET			2			/* internetwork: UDP, TCP, etc. */
#define	AF_IMPLINK		3			/* arpanet imp addresses */
#define	AF_PUP			4			/* pup protocols: e.g. BSP */
#define	AF_CHAOS		5			/* mit CHAOS protocols */
#define	AF_NS			6			/* XEROX NS protocols */
#define	AF_NBS			7			/* nbs protocols */
#define	AF_ECMA			8			/* european computer manufacturers */
#define	AF_DATAKIT		9			/* datakit protocols */
#define	AF_CCITT		10			/* CCITT protocols, X.25 etc */
#define	AF_SNA			11			/* IBM SNA */
#define AF_ETHER		12			/* Ethernet 2.0 */
#define AF_DLI			13			/* Direct data link interface */
#define AF_LAT			14			/* LAT */
#define	AF_HYLINK		15			/* NSC Hyperchannel */
#define	AF_APPLETALK	16			/* Apple Talk */
#define	AF_NIT      	17			/* Network Interface Tap */
#define	AF_802      	18			/* IEEE 802.2, also ISO 8802 */
#define	AF_OSI			19			/* umbrella for all OSI families */
#define	AF_X25			20			/* X.25 */
#define	AF_MAX			21

/*
 * Structure used by kernel to store most
 * addresses.
 */
struct sockaddr {
	u_short			sa_family;		/* address family */
	char			sa_data[14];	/* up to 14 bytes of direct address */
};

/*
 * Structure used by kernel to pass protocol
 * information in raw sockets.
 */
struct sockproto {
	u_short			sp_family;		/* address family */
	u_short			sp_protocol;	/* protocol */
};

/*
 * Protocol families, same as address families for now.
 */
#define	PF_UNSPEC		AF_UNSPEC
#define	PF_UNIX			AF_UNIX
#define	PF_INET			AF_INET
#define	PF_IMPLINK		AF_IMPLINK
#define	PF_PUP			AF_PUP
#define	PF_CHAOS		AF_CHAOS
#define	PF_NS			AF_NS
#define	PF_NBS			AF_NBS
#define	PF_ECMA			AF_ECMA
#define	PF_DATAKIT		AF_DATAKIT
#define	PF_CCITT		AF_CCITT
#define	PF_SNA			AF_SNA
#define PF_ETHER		AF_ETHER
#define PF_DECnet		AF_DECnet
#define PF_DLI			AF_DLI
#define PF_LAT			AF_LAT
#define	PF_HYLINK		AF_HYLINK
#define	PF_APPLETALK	AF_APPLETALK
#define	PF_MAX			AF_MAX

/*
 * Maximum queue length specifiable by listen.
 */
#define	SOMAXCONN		5

/*
 * Message header for recvmsg and sendmsg calls.
 */
struct msghdr {
	char			*msg_name;		/* optional address */
	int				msg_namelen;	/* size of address */
	struct iovec	*msg_iov;		/* scatter/gather array */
	int				msg_iovlen;		/* # elements in msg_iov */
	char			*msg_accrights;	/* access rights sent/received */
	int				msg_accrightslen;
};

#define	MSG_OOB			0x1			/* process out-of-band data */
#define	MSG_PEEK		0x2			/* peek at incoming message */
#define	MSG_DONTROUTE	0x4			/* send without using routing tables */

#define	MSG_MAXIOVLEN	16

/*
 * Socket function prototypes
 */
#if (defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus))
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/
int accept(int s, struct sockaddr* addr, int* addrlen);
int bind(int s, struct sockaddr* name, int namelen);
int connect(int s, struct sockaddr* to, int namelen);
int gethostname(char* name, int namelen);
int getpeername(int s, struct sockaddr* name, int* namelen);
int getsockname(int s, struct sockaddr* name, int* namelen);
int getsockopt(int s, int level, int optname, char* optval, int* optlen);
int listen(int s, int backlog);
int pcmd(u_int16 pcode, void* buffer, u_int32 timestamp, u_int32 type, u_int32 patype);
int recv(int s, char* msg, int len, int flags);
int recvfrom(int s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen);
int send(int s, char* msg, int len, int flags);
int sendto(int s, char* msg, int len, int flags, struct sockaddr* to, int tolen);
int sethostname(char* name, int namelen);
int setsockopt(int s, int level, int optname, char* optval, int optlen);
int shutdown(int s, int how);
int socket(int af, int type, int protocol);
int _gs_pcmd(int s, void* pb);
int _gs_dcmd(int s, u_int32 code, void* buf, u_int32 buflen);
int _ss_pcmd(int s, void* pb);
int _ss_dcmd(int s, u_int32 code, void* buf, u_int32 buflen);
int _ss_sevent(int s, int io_event);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#else
int accept();
int bind();
int connect();
int gethostname();
int getpeername();
int getsockname();
int getsockopt();
int listen();
int pcmd();
int recv();
int recvfrom();
int send();
int sendto();
int sethostname();
int setsockopt();
int shutdown();
int socket();
int _gs_pcmd();
int _gs_dcmd();
int _ss_pcmd();
int _ss_dcmd();
int _ss_sevent();
#endif


#endif	/* _SOCKET_H */
