/****************************************************************************
 *           Copyright 1989-2000 by Microware Systems Corporation           *
 *                           All Rights Reserved                            *
 *                         Reproduced Under License                         *
 *                                                                          *
 * This software is confidential property of Microware Systems Corporation, *
 * and is provided under license for internal development purposes only.    *
 * Reproduction, publication, distribution, or creation of derivative works *
 * in any form to any party other than the licensee is strictly prohibited, *
 * unless expressly authorized in writing by Microware Systems Corporation. *
 *                                                                          *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *  01 01/13/00 Created from various INET/XXXX header files            rds
 *  01 09/13/00 Added defines to support _OSK                          rds
 *  02 06/22/01 Added route entry setstat param blocks.                sr    *
 ****************************************************************************/

/*
** The low level drivers used to use the ISP header files that were located
** in /mwos/SRC/DEFS/INET. Since ISP is being phased out, rather than
** converting to use the SPF header files, we will just put the structures
** and defines here.
*/

#ifndef _ROM_LLNET_H
#define _ROM_LLNET_H


#include <const.h>

/*
** Protocols
*/
#define IPPROTO_IP		0	/* dummy for IP */
#define IPPROTO_ICMP	1	/* control message protocol */
#define IPPROTO_GGP		3	/* gateway^2 (deprecated) */
#define IPPROTO_TCP		6	/* tcp */
#define IPPROTO_EGP		8	/* exterior gateway protocol */
#define IPPROTO_PUP		12	/* pup */
#define IPPROTO_UDP		17	/* user datagram protocol */
#define IPPROTO_IDP		22	/* xns idp */
#define IPPROTO_RAW		255	/* raw IP packet */
#define IPPROTO_MAX		256

/*
** Address families. (Only AF_INET is supported)
*/
#define AF_UNSPEC		0		/* unspecified */
#define AF_UNIX			1		/* local to host (pipes, portals) */
#define AF_INET			2		/* internetwork: UDP, TCP, etc. */
#define AF_IMPLINK		3		/* arpanet imp addresses */
#define AF_PUP			4		/* pup protocols: e.g. BSP */
#define AF_CHAOS		5		/* mit CHAOS protocols */
#define AF_NS			6		/* XEROX NS protocols */
#define AF_NBS			7		/* nbs protocols */
#define AF_ECMA			8		/* european computer manufacturers */
#define AF_DATAKIT		9		/* datakit protocols */
#define AF_CCITT		10		/* CCITT protocols, X.25 etc */
#define AF_SNA			11		/* IBM SNA */
#define AF_ETHER		12		/* Ethernet 2.0 */
#define AF_DLI			13		/* Direct data link interface */
#define AF_LAT			14		/* LAT */
#define AF_HYLINK		15		/* NSC Hyperchannel */
#define AF_APPLETALK	16		/* Apple Talk */
#define AF_NIT			17		/* Network Interface Tap */
#define AF_802			18		/* IEEE 802.2, also ISO 8802 */
#define AF_OSI			19		/* umbrella for all OSI families */
#define AF_X25			20		/* X.25 */
#define AF_MAX			21

/*
** Socket types
*/
#define SOCK_STREAM		1	/* stream socket */
#define SOCK_DGRAM		2	/* datagram socket */
#define SOCK_RAW		3	/* raw-protocol interface */

#define PRS_ATTACH		1	/* attach protocol to socket */
#define PRS_DETACH		2	/* detach protocol from socket */
#define PRS_BIND		3	/* bind socket name */
#define PRS_LISTEN		4	/* mark socket as passive */
#define PRS_CONNECT		5	/* initiate a connection */
#define PRS_ACCEPT		6	/* accept a connection */
#define PRS_DISCONNECT	7	/* disconnect from peer */
#define PRS_SHUTDOWN	8	/* shutdown this side of the connection */
#define PRS_RCVD		9	/* data has been read from socket */
#define PRS_SEND		10	/* write data on socket */
#define PRS_ABORT		11	/* abort connection */
#define PRS_CONTROL		12	/* control operations on protocol */
#define PRS_SENSE		13	/* return status */
#define PRS_RCVOOB		14	/* retrieve out of band data */
#define PRS_SENDOOB		15	/* send out of band data */
#define PRS_NAME		16	/* get local connection name */
#define PRS_PEER		17	/* get peer name */
#define PRS_CONNECT2	18	/* connect two sockets */
#define PRS_FASTTIMO	19	/* 200ms timeout */
#define PRS_SLOWTIMO	20	/* 500ms timeout */
#define PRS_PROTORCV	21	/* receive from below */
#define PRS_PROTOSEND	22	/* send to below */
#define PRS_CMP			23	/* compare two names */

#define PRS_NREQ		23
#define PRS_UNREACHABLE	24	/* ICMP DESTUNREACHABLE support */

#define SEQ_LT(a,b) (((a)-(b))&0x80000000)
#define SEQ_LEQ(a,b) (!SEQ_GT(a,b))
#define SEQ_GT(a,b) SEQ_LT(b,a)
#define SEQ_GEQ(a,b) (!SEQ_LT(a,b))


/*
** IP address manipulation macros
*/
#define IN_CLASSA(i)		(((long)(i) & 0x80000000) == 0)
#define IN_CLASSA_NET		0xff000000
#define IN_CLASSA_NSHIFT	24
#define IN_CLASSA_HOST		0x00ffffff
#define IN_CLASSA_MAX		128

#define IN_CLASSB(i)		(((long)(i) & 0xc0000000) == 0x80000000)
#define IN_CLASSB_NET		0xffff0000
#define IN_CLASSB_NSHIFT	16
#define IN_CLASSB_HOST		0x0000ffff
#define IN_CLASSB_MAX		65536

#define IN_CLASSC(i)		(((long)(i) & 0xe0000000) == 0xc0000000)
#define IN_CLASSC_NET		0xffffff00
#define IN_CLASSC_NSHIFT	8
#define IN_CLASSC_HOST		0x000000ff

#define IN_CLASSD(i)		(((long)(i) & 0xf0000000) == 0xe0000000)
#define IN_MULTICAST(i)		IN_CLASSD(i)

#define INADDR_ANY (u_long)0x00000000
#define INADDR_BROADCAST (u_long)0xffffffff


/*
** We will continue to use the old style sockaddr structures. Newer
** implementations have split sin_family into a byte for length and
** a byte for the family.
*/
struct in_addr {
	u_long s_addr;
};
struct sockaddr_in {
	short			sin_family;
	u_short			sin_port;
	struct in_addr	sin_addr;
	char			sin_zero[8];
};
struct sockaddr {
	u_short	sa_family;		/* address family */
	char	sa_data[14];	/* up to 14 bytes of direct address */
};


/*
**************************************************************************
** IP structures and defines
**************************************************************************
*/
typedef struct ip_header {
	u_char		vers_ihl;
	u_int8		tos;
	u_int16		length;
	u_int16		ident;
	u_int16		fl_fr_off;
	u_int8		ttl;
	u_int8		protocol;
	u_int16		checksum;
	u_int32		source_addr;
	u_int32		dest_addr;
} IP_HEADER, *Ip_header;

#define IPHEADSIZE	20
#define IPVERSION	4

#define IP_ADDR u_int32

/* ip header flags */
#define IP_FRAG_MASK	0x1FFF
#define IP_VERS_MASK	0xf0
#define IP_IHL_MASK		0x0f
#define MORE_FRAGS		0x2000
#define DONT_FRAG		0x4000

typedef struct pseudohead {
	u_int32 source_addr;
	u_int32 dest_addr;
	u_int8 zero;
	u_int8 proto;
	u_int16 length;
} PSEUDOHEAD, *pseudohead;

/*
**************************************************************************
** TCP structures and defines
**************************************************************************
*/
typedef struct tcp_header {
	u_int16		source_port;
	u_int16		dest_port;
	u_int32		seq_num;
	u_int32		ack_num;
	u_char		data_off_resvd;
	u_char		flags;
	u_int16		window;
	u_int16		checksum;
	u_int16		urgent;
} TCP_HEADER, *Tcp_header;

/* flag definitions */
#define TH_FIN	0x01
#define TH_SYN	0x02
#define TH_RST	0x04
#define TH_PUSH	0x08
#define TH_ACK	0x10
#define TH_URG	0x20

/* states */
#define TCPS_CLOSED			0	/* closed */
#define TCPS_LISTEN			1	/* listening for connection */
#define TCPS_SYN_SENT		2	/* active, have sent syn */
#define TCPS_SYN_RECEIVED	3	/* have send and received syn */
#define TCPS_ESTABLISHED	4	/* established */
#define TCPS_CLOSE_WAIT		5	/* rcvd fin, waiting for close */
#define TCPS_FIN_WAIT_1		6	/* have closed, sent fin */
#define TCPS_CLOSING		7	/* closed xchd FIN; await FIN ACK */
#define TCPS_LAST_ACK		8	/* had fin and close; await FIN ACK */
#define TCPS_FIN_WAIT_2		9	/* have closed, fin is acked */
#define TCPS_TIME_WAIT		10	/* in 2*msl quiet wait after close */


#define T_DOFF_MASK 0xf0

/*
**************************************************************************
** UDP structures and defines
**************************************************************************
*/
typedef struct udp_header {
	u_int16 source_port;
	u_int16 dest_port;
	u_int16 length;
	u_int16 checksum;
} UDP_HEADER, *Udp_header;

/*
**************************************************************************
** Ethernet structures and defines
**************************************************************************
*/
typedef struct ether_header {
	u_char	ether_dhost[6];
	u_char	ether_shost[6];
	u_short	ether_type;
} ETH_HEADER, *Eth_header;

#define ETHERTYPE_PUP	0x0200	/* PUP protocol */
#define ETHERTYPE_IP	0x0800	/* IP protocol */
#define ETHERTYPE_ARP	0x0806	/* Addr. resolution protocol */
#define ETHERTYPE_RARP	0x0835	/* Reverse ARP */
#define ETHERMIN		(60-14)
#define ETHERMTU 1500

struct arphdr {
	u_int16 ar_hrd;				/* format of hardware address */
		#define ARPHRD_ETHER 1	/* ethernet hardware address */
	u_int16	ar_pro;				/* format of protocol address */
	u_int8	ar_hln;				/* length of hardware address */
	u_int8	ar_pln;				/* length of protocol address */
	u_int16	ar_op;				/* one of: */
		#define ARPOP_REQUEST 1	/* request to resolve address */
		#define ARPOP_REPLY 2	/* response to previous request */
};

struct ether_arp {
	struct arphdr ea_hdr;	/* fixed-size header */
	u_char arp_sha[6];		/* sender hardware address */
	u_char arp_spa[4];		/* sender protocol address */
	u_char arp_tha[6];		/* target hardware address */
	u_char arp_tpa[4];		/* target protocol address */
};
#define arp_hrd ea_hdr.ar_hrd
#define arp_pro ea_hdr.ar_pro
#define arp_hln ea_hdr.ar_hln
#define arp_pln ea_hdr.ar_pln
#define arp_op ea_hdr.ar_op


/*
**************************************************************************
** Control and parameter blocks
**************************************************************************
*/
typedef struct {
	u_int32 pb_fcode;			/* the function code */
	u_int32 pb_status;			/* status 0 = ok, n = error code */
	union {
		struct scb *_scb;		/* pointer to socket scb or socket module */
		struct sockmod *_smod;	/* entry of the called module */
	} pb_cb;
	union {
		void *ptr;				/* parameters to called function */
		int32 val;
	} pb_parm[8];
	u_int32 pb_size;			/* size if it was srqmem'ed */
} PBLOCK, *Pblock;


/* SS_Bind setstat service request parameter block. */
typedef struct {
	struct sockaddr		*name;
	u_int32				length;
} ss_bind_pb, *Ss_bind_pb;

/* SS_RecvFr setstat service request parameter block. */
typedef struct {
	u_char				*buf;
	u_int32				len;
	u_int32				flags;
	struct sockaddr		*name;
	u_int32				*namelen;
} ss_recvfr_pb, *Ss_recvfr_pb;

/* SS_SendTo setstat service request parameter block. */
typedef struct {
	u_char				*buf;
	u_int32				len;
	u_int32				flags;
	struct sockaddr		*name;
	u_int32				namelen;
} ss_sendto_pb, *Ss_sendto_pb;

/* SS_Connect setstat service request parameter block. */
typedef struct {
	struct sockaddr		*name;
	u_int32				namelen;
} ss_connect_pb, *Ss_connect_pb;

/* SS_Listen setstat service request parameter block. */
typedef struct {
	u_int32				qlen;
} ss_listen_pb, *Ss_listen_pb;

/* SS_Accept setstat service request parameter block. */
typedef struct {
	path_id				path;
	struct sockaddr		*name;
	u_int32				*namelen;
} ss_accept_pb, *Ss_accept_pb;

/* SS_SET_RTENT setstat service request parameter block. */
typedef struct {
	struct  in_addr dst_addr; /* Internet Address */
	struct  in_addr rt_gateway; /* Gateway Address */
	u_short rt_flags;
} ss_rtent_pb, *Ss_rtent_pb;

/* The Low level route table entry structure */
typedef struct llrtentry {
	struct  in_addr dst_addr; /* Internet Address */
	struct  in_addr rt_gateway; /* Gateway Address */
	u_short rt_flags;
	u_int32 rsvd[4];
} llrtentry, *Llrtentry;

#define dst_ipaddr dst_addr.s_addr
#define gw_ipaddr rt_gateway.s_addr


/*
**************************************************************************
** Function prototypes
**************************************************************************
*/
#if (defined(_ANSI_EXT) || defined(__STDC__)) || defined(__cplusplus)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
int accept(int, struct sockaddr*, int*);
int bind(int, struct sockaddr*, int);
int connect(int, struct sockaddr*, int);
int listen(int, int);
int recvfrom(int, char*, int, int, struct sockaddr*, int*);
int sendto(int, char*, int, int, struct sockaddr*, int);
int socket(int, int, int);
u_long htonl(u_long);
u_short htons(u_short);
u_long ntohl(u_long);
u_short ntohs(u_short);
#if defined(__cplusplus)
}
#endif /* __cplusplus*/
#else
int accept();
int bind();
int connect();
int listen();
int recvfrom();
int sendto();
int socket();
u_long htonl();
u_short htons();
u_long ntohl();
u_short ntohs();
#endif

#if defined(_BIG_END)
#define ntohl(x) (x)
#define ntohs(x) (x)
#define htonl(x) (x)
#define htons(x) (x)
#endif

/*
** DPIOish cross platform mappings - Should we use "real" DPIO instead?
*/
#if defined(_OS9000)
	#define REGS regs
	#define PR_DESC Pr_desc
	#define P_PRIOR p_prior
	#define P_STATE p_state
	#define P_PMODUL p_pmodul
	#define P_QUEUEID p_queueid
	#define P_SP p_sp
	#define P_ID p_id
	#define PROC_STATE SYSSTATE
	#define STACK Regs
#else
	#define REGS REGISTERS
	#define PR_DESC procid *
	#define P_PRIOR _prior
	#define P_STATE _state
	#define P_PMODUL _pmodul
	#define P_QUEUEID _queueid
	#define P_SP _sp
	#define P_ID _id
	#define PROC_STATE 0x8000
	#define STACK u_char *
#endif

#endif /* _ROM_LLNETCB_H */
