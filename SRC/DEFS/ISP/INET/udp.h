/****************************************************************************
 *  $Workfile:   udp.h  $
 *  $Revision:   1.11  $
 *  $Modtime:   17 Jan 1996 14:22:00  $
 *  $Date:   17 Jan 1996 14:23:34  $
 ****************************************************************************
 *	Definitions for Internet UDP protocol module.							*
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
 *	  1	10/19/93	Merged OS-9/OS-9000 sources							bat	*
 *    2 05/17/94    Added UDP_STATS & inetsudps                         bat *
 *    3 07/06/94    Added more packet statistics                        bat *
 *      01/17/96	<***** ISP 2.1 Release ***********************>			*
 ****************************************************************************/

#ifndef _UDP_H
#define _UDP_H


/*
 * UDP statistics.
 * Many of these should be kept per connection,
 * but that's inconvenient at the moment.
 */
typedef struct	udp_stats {
	u_int32		udpstt_connattempt;			/* connections initiated */
	u_int32		udpstt_in_pkts;				/* in packets (no errs) */
	u_int32		udpstt_in_noports;			/* in packets (no port listener) */
	u_int32		udpstt_in_errs;				/* in packets (errors) */
	u_int32		udpstt_out_pkts;			/* out packets */
	/*** more stats to come! ***/
} UDP_STATS, *Ucp_stats;

/*
	UDP module control block
*/
typedef struct {
	MCB			udpm_mcb;			/* mcb header */
	INETSCB		udpm_inetscb;		/* head of udp socket list */
	u_int32		udpm_ipin;			/* installed with ip flag */
	Sockmod		udpm_inetsm;		/* pointer to inet */
	Sockmod		udpm_ipsm;			/* pointer to ip */
	UDP_STATS	udpm_stats;			/* udp statistics block */
} UDPMCB, *Udpmcb;

/*	easy access to mcb */
#define udpm_scbnext	udpm_mcb.mcb_scbnext
#define udpm_scbprev udpm_mcb.mcb_scbprev
#define udpm_smod	udpm_mcb.mcb_smod

/*
	Udp Scb
*/
typedef struct {
	SCB		udps_scb;		/* scb header */
	Inetscb	udps_inetscb;	/* inet scb for this socket */
} UDPSCB, *Udpscb;

/* easy access to scb */
#define udps_snd		udps_scb.scb_snd
#define udps_rcv		udps_scb.scb_rcv
#define udps_smod	udps_scb.scb_smod
#define udps_socket	udps_scb.scb_socket
#define udps_chainnext	udps_scb.scb_chainnext
#define udps_chainprev	udps_scb.scb_chainprev
#define udps_state	udps_scb.scb_state



/*
		Now we get to the real meaty UDP stuff, right out of the DDN manuals.
*/

/*
		udp datagram header structure.
*/
typedef struct udp_header {
	u_int16		source_port;
	u_int16		dest_port;
	u_int16		length;
	u_int16		checksum;
} UDP_HEADER, *Udp_header;

typedef struct udp_ip_header {
	IP_HEADER ipheader;
	UDP_HEADER udpheader;
} UDP_IP_HEADER, *Udp_ip_header;

/*
	udp pseudo header structure
*/
typedef struct udp_pseudohead {
	u_int32	source_addr;
	u_int32	dest_addr;
	u_int8	zero;
	u_int8	proto;
	u_int16	length;
} UDP_PSEUDOHEAD, *Udp_pseudohead;

/* the size, in bytes of a UDP header. */
#define UDPHEADSIZE	sizeof(UDP_HEADER)



/* following structure is for inetstat <-a|-n> which will call udp */
struct inetsudps {
	u_short pd_pd;				/* path number */
	struct in_addr inets_laddr; /* local internet address of socket */
	struct in_addr inets_paddr;	/* internet address of connected peer */
	u_int16 inets_lport;		/* local port number */
	u_int16 inets_pport;		/* peer port number */
	u_int16 state;				/* for compatability only! */
	u_int32 so_rcv_sb_cc;		/* characters available in buffer */
	u_int16 pd_pid;				/* process ID for signals */
};


#endif	/* _UDP_H */
