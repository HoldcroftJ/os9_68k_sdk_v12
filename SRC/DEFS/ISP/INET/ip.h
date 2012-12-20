/****************************************************************************
 *  $Workfile:   ip.h  $
 *  $Revision:   200.10  $
 *  $Modtime:   17 Feb 1998 10:31:00  $
 *  $Date:   17 Feb 1998 10:31:20  $
 ****************************************************************************
 *	Definitions for Internet IP protocol module.							*
 ****************************************************************************
 *	Copyright 1989 by Microware Systems Corporation							*
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
 *      09/16/93	<***** ISP 2.0 Release ***********************>			*
 *	  1 02/08/94	Combine OS-9/OS9000 ip.h.                          ylee *
 *                  OS-9000 inetmod.h moved in.                             *
 *    2 05/12/94    More merging of OS-9/OS-9000 sources                bat *
 *                  OS-9:ip_header now without bit-fields                   *
 *                    -- now using OS-9000 version with masks               *
 *    3 06/28/94    IPMCB structure now the same for OS-9/OS-9000           *
 *                  Added more icmp fields in ipstats structure             *
 *                  Took out OS-9 mh_com in inetmod                     bat *
 *    4 07/07/94    Added ICMP statistics to IPMCB                      bat *
 *      01/17/96	<***** ISP 2.1 Release ***********************>			*
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release ********>         *
 *      02/13/98    Specified typedef name for declaration, to remove   sr  *
 *                  warnings.                                               *
 *                  ---- OS-9000/Brutus Board Support V1.0 Released ----    *
 ****************************************************************************/
	
#ifndef _IP_H
#define _IP_H


#include <types.h>

#if defined(_OSK)
	#include <module.h>
#endif

#include <INET/inet.h>
#include <INET/mbuf.h>
#include <INET/ifman.h>
#include <INET/sockman.h>


/*
**	Layout of inet data module
*/

#if defined(_OSK)
	#define INET_OFFSET	u_int16
#elif defined(_OS9000)
	#define INET_OFFSET	u_int32
#endif

typedef struct inetmod {
	u_int32		inet_flags;		/* internet system flags */
	u_int32		inet_default;	/* default internet destination */
	INET_OFFSET	inet_hostroute;	/* offset to host-specific routing table */
	INET_OFFSET	inet_netroute;	/* offset to network routing table */
} INETMOD, *Inetmod;

/* flag bit definitions */
#define INET_GATEWAY	0x00000001

typedef struct iroute {
	u_int32	ir_target;	/* the target network or address */
	u_int32	ir_dst;		/* the place to send the packet to */
}  IROUTE, *Iroute;

/*
**	These structures are used by ip to manage fragmented datagrams as they
**	come in.
*/

typedef struct frag FRAG;
typedef FRAG* Frag;

typedef struct frag {
	Frag		fr_next;		/* next fragment in datagram */
	Frag		fr_prev;		/* previous .. */
	u_int32		fr_offset;		/* offset of fragment in datagram */
	u_int32		fr_length;		/* size of fragment */
	u_int32		fr_end;			/* end of data in fragment */
	Mbuf		fr_data;		/* mbuf containing data */
	u_int32		fr_size;		/* size from _srqmem */
} frag;

typedef struct fdgrm FDGRM;
typedef FDGRM* Fdgrm;

typedef struct fdgrm {
	Fdgrm		fd_next;		/* next fragmented datagram in list */
	Fdgrm		fd_prev;		/* previous ... */
	u_int32		fd_timer;
	u_int32		fd_src;			/* source address of this datagram */
	u_int32		fd_dst;			/* destination */
	u_int16 	fd_proto;		/* protocol */
	u_int16 	fd_id;			/* identification */
	int32		fd_length;		/* length of datagram */
	Ifdev		fd_ifdev;		/* device this datagram came from */
	Frag		fd_data;		/* data in packet */
	Frag		fd_back;		/* reverse order */
	u_int32		fd_size;		/* size from _srqmem */
} fdgrm;

#define FRAGTIME	30  /* this is about 15 seconds */

/*	Structure for routing table entries */

typedef struct rtab RTAB;
typedef RTAB* Rtab;

typedef struct rtab {
	u_int32		rt_target;		/* final destination address */
	Rtab		rt_next;		/* next routing entry */
	u_int32		rt_dst;			/* intermediate destination address */
	Ifdev		rt_ifdev;		/* pointer to ifdev structure */
	u_int32		rt_size;		/* size of this structure from _srqmem */
	int			rt_hopcount;	/* hop count of route */
	int			rt_pa;			/* 1 for passive, 0 for active */
	int			rt_delflag;		/* 1 means routed will del this entry within 60 sec */
	long		rt_timestamp;	/* time stamp of route */
} rtab;

#define RT_NONE			0
#define RT_LOCAL		1
#define RT_REMOTE		2

/*
**	IP statistics block
*/

typedef struct {
    u_int32  InMsgs;				/* ICMP msgs received */
    u_int32  InErrors;				/* ICMP msgs received with errors */
    u_int32  InDestUnreachs;
    u_int32  InTimeExcds;
    u_int32  InParmProbs;
    u_int32  InSrcQuenchs;
    u_int32  InRedirects;
    u_int32  InEchos;
    u_int32  InEchoReps;
    u_int32  InTimestamps;
    u_int32  InTimestampReps;
    u_int32  InAddrMasks;
    u_int32  InAddrMaskReps;
    u_int32  InInforms;
    u_int32  InInformReps;
    u_int32  OutMsgs;
    u_int32  OutErrors;
    u_int32  OutDestUnreachs;
    u_int32  OutTimeExcds;
    u_int32  OutParmProbs;
    u_int32  OutSrcQuenchs;
    u_int32  OutRedirects;
    u_int32  OutEchos;
    u_int32  OutEchoReps;
    u_int32  OutTimestamps;
    u_int32  OutTimestampReps;
    u_int32  OutAddrMasks;
    u_int32  OutAddrMaskReps;
    u_int32  OutInforms;
    u_int32  OutInformReps;
} icmp_stats, *Icmp_stats;

typedef struct ip_stats {
	u_int32		ipstt_inpackets;	/* number of packets received */
	u_int32		ipstt_outpackets;	/* number of packets sent */
	u_int32		ipstt_complete;		/* complete datagrams received */
	u_int32		ipstt_badcheck;		/* datagrams received with bad checksum */
	u_int32		ipstt_runt;			/* tiny datagrams that were tossed */
	u_int32		ipstt_version;		/* unacceptable version number */
	u_int32		ipstt_hlen;			/* header length too short */
	u_int32		ipstt_orphan;		/* datagrams that no one wanted */
	u_int32		ipstt_route;		/* datagrams routed through this machine */
	u_int32 	ipstt_qfull;		/* datagrams dropped because recv queue was full */
	u_int32		ipstt_raw;			/* datagrams that went to raw sockets */
	u_int32		ipstt_ulp;			/* datagrams that went to ulp's */
	u_int32		ipstt_icmp;			/* icmp datagrams */
	u_int32		ipstt_nomem;		/* no memory for processing packet */
	icmp_stats	ipstt_icmpstat;		/* detailed icmp statistics */
} IP_STATS, *Ip_stats;

/*
**	IP global static storage structure
*/
typedef struct {
	MCB			ipm_mcb;			/* ip module control block */
	int32		ipm_errno;			/* handy errno for input() */
	Sockmod		ipm_inetsm;			/* pointer to inet */
	Sockmod		ipm_udpsm;			/* pointer to udp */
	Sockmod 	ipm_tcpsm;			/* pointer to tcp */
	Inetmod		ipm_inetmod;		/* pointer to inet data module */
	Ifdev		ipm_ifdev;			/* pointer to the ifdev list */
	Ifdev		ipm_default;		/* pointer to default device for routing */
	Rtab		ipm_hostroute;		/* pointer to host-specific routing list */
	Rtab		ipm_netroute;		/* pointer to network routing list */	
	INETSCB		ipm_inetscb;		/* head of list of ip socket */
	Fdgrm		ipm_fraglist;		/* list of datagram fragments */
	Fdgrm		ipm_fragback;		/* list in reverse order */
	u_int16 	ipm_identifier;		/* ip packet identification */
	u_int16 	ipm_icmpsequence;	/* icmp sequence */
	u_int32		ipm_icmprdflag;		/* icmp redirect flag */
	u_int32		ipm_gwflag;			/* set to 1 if host is gateway */
	IP_STATS	ipm_stats;			/* ip statistics */
	Mh_com		ipm_inetcfg;		/* pointer to top of inet data module */
#if 0 /* moved OS9000 ipm_icmprdflag/ipm_gwflag to OS9's place above */
#ifdef _OS9000
	int			ipm_icmprdflag;		/* icmp redirect flag */
	int			ipm_gwflag;			/* set to 1 if host is gateway */
#endif
#endif
	/* more icmp stats stuff here or in ip_stats??? */
} IPMCB, *Ipmcb;	

/*	easy access to mcb */
#define ipm_scbnext	ipm_mcb.mcb_scbnext
#define ipm_scbprev ipm_mcb.mcb_scbprev
#define ipm_smod	ipm_mcb.mcb_smod

/* make it easy to access stats block */
#define	ipm_inpackets	ipm_stats.ipstt_inpackets
#define	ipm_outpackets	ipm_stats.ipstt_outpackets
#define	ipm_complete	ipm_stats.ipstt_complete
#define	ipm_badcheck	ipm_stats.ipstt_badcheck
#define	ipm_runt		ipm_stats.ipstt_runt
#define	ipm_version		ipm_stats.ipstt_version
#define	ipm_hlen		ipm_stats.ipstt_hlen
#define	ipm_orphan		ipm_stats.ipstt_orphan
#define	ipm_route		ipm_stats.ipstt_route
#define ipm_qfull		ipm_stats.ipstt_qfull
#define	ipm_raw			ipm_stats.ipstt_raw
#define	ipm_ulp			ipm_stats.ipstt_ulp
#define	ipm_icmp		ipm_stats.ipstt_icmp
#define ipm_nomem		ipm_stats.ipstt_nomem

/*
**	IP socket-specific storage structure
*/
typedef struct {
	SCB			ips_scb;	/* scb */
	Inetscb		ips_inetscb;  /* scb for inet module */
	int16		ips_proto;	/* protocol used by this socket */
	int16		pad;
	Mbuf		ips_icmprecv;	/* icmp message receive queue */
} IPSCB, *Ipscb;

/* easy access to scb */
#define ips_snd		ips_scb.scb_snd
#define ips_rcv		ips_scb.scb_rcv
#define ips_smod	ips_scb.scb_smod
#define ips_socket	ips_scb.scb_socket
#define ips_chainnext	ips_scb.scb_chainnext
#define ips_chainprev	ips_scb.scb_chainprev
#define ips_state	ips_scb.scb_state

/*
**	Now we get to the real meaty IP stuff, right out of the DDN manuals.
**	These structures and defines relate to RFC 791.
*/

/*
**	ip datagram header structure.  Note that this does not include any
**	options section, which must be allocated along with this structure if
**	there are any options to send.
*/

typedef struct ip_header {
	u_char			vers_ihl;
	u_int8			tos;
	u_int16			length;
	u_int16			ident;
	u_int16			fl_fr_off;
	u_int8			ttl;
	u_int8			protocol;
	u_int16			checksum;
	u_int32			source_addr;
	u_int32			dest_addr;
} IP_HEADER, *Ip_header;

/* the maximum size, in bytes of an IP header.  RFC 791, pp13, p3 */
#define MAXIPHEAD		60
#define IPHEADSIZE		20
#define IPVERSION		4
#define IP_MAXPACKET	65535

/* ip header flags */
#define IP_FRAG_MASK	0x1FFF
#define	IP_VERS_MASK	0xf0
#define	IP_IHL_MASK		0x0f
#define MORE_FRAGS		0x2000
#define DONT_FRAG		0x4000

/*
	These are the definitions of the flag bits for the flag parameter to
	ip_route.
*/
#define NET_INPUT		0x0001
#define BROADCAST		0x0002

/* some definitions for ip options */
#define IPOPT_END		0x00
#define IPOPT_NOP		0x01
#define IPOPT_SECURITY	0x02
#define IPOPT_LOOSERT	0x03
#define IPOPT_STRICTRT	0x09
#define IPOPT_RECORDRT	0x07
#define IPOPT_STREAMID	0x08
#define IPOPT_TIMESTAMP	0x44

#define IPOPT_COPY		0x80

/* Protocol Information Request Types */
#define NET_RT			1
#define HOST_RT 		2
#define DEFAULT_RT		3
#define IP_STAT			4

#define ACTIVERTENTRY	0	/* active route */
#define PASSIVERTENTRY	1	/* passive route */


#endif	/* _IP_H */
