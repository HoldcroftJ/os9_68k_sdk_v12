/****************************************************************************
 *           Copyright 1989-1998 by Microware Systems Corporation           *
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
 *  01 10/07/96 Creation
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *  02 03/07/97 Hacked up bit fields for littlen endian machines       tmd
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  03 06/30/97 Added new copyright header                             apw
 *  04 08/11/97 Added iphlen parm to rip_input                         tmd
 *              Added imo_multicast_vif to ip_moptions structure       tmd
 *  05 08/28/97 Added SPF_DRVR, SPIP, and SPRAW ifdefs                 rds
 *     10/13/97 Changed ip_len and ip_off from short to u_short        rds
 *  06 10/13/97 Added ips_toolong to ipstat                            rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  07 10/23/98 Changed ipopt_list from char to u_char                 rds
 ****************************************************************************/
/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)ip_var.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _NETINET_IP_VAR_H
#define _NETINET_IP_VAR_H

/* Since ANSI/ucc doesn't allow bit fields with a character, we replace
   u_char to be bitfld in all bit field definitions, and define bitfld
   appropriately. (TMD) */

#if (defined(_OSK)||defined(_OS9000))
#define bitfld unsigned int
#else
#define bitfld u_char
#endif /* OS9 */

/*
 * Overlay for ip header used by other protocols (tcp, udp).
 */
struct ipovly {
	caddr_t	ih_next, ih_prev;	/* for protocol sequence q's */
	u_char	ih_x1;			/* (unused) */
	u_char	ih_pr;			/* protocol */
	short	ih_len;			/* protocol length */
	struct	in_addr ih_src;		/* source internet address */
	struct	in_addr ih_dst;		/* destination internet address */
};

/*
 * Ip reassembly queue structure.  Each fragment
 * being reassembled is attached to one of these structures.
 * They are timed out after ipq_ttl drops to 0, and may also
 * be reclaimed if memory becomes tight.
 */
struct ipq {
	struct	ipq *next,*prev;	/* to other reass headers */
	u_char	ipq_ttl;		/* time for reass q to live */
	u_char	ipq_p;			/* protocol of this fragment */
	u_short	ipq_id;			/* sequence id for reassembly */
	struct	ipasfrag *ipq_next,*ipq_prev;
					/* to ip headers of fragments */
	struct	in_addr ipq_src,ipq_dst;
};

/*
 * Ip header, when holding a fragment.
 *
 * Note: ipf_next must be at same offset as ipq_next above
 */
struct	ipasfrag {
#if BYTE_ORDER == LITTLE_ENDIAN
	bitfld	ip_hl:4,
		ip_v:4;
#endif
#if BYTE_ORDER == BIG_ENDIAN
	bitfld	ip_v:4,
		ip_hl:4;
#endif
	u_char	ipf_mff;		/* XXX overlays ip_tos: use low bit
					 * to avoid destroying tos;
					 * copied from (ip_off&IP_MF) */
	u_short	ip_len;
	u_short	ip_id;
	u_short	ip_off;
	u_char	ip_ttl;
	u_char	ip_p;
	u_short	ip_sum;
	struct	ipasfrag *ipf_next;	/* next fragment */
	struct	ipasfrag *ipf_prev;	/* previous fragment */
};

/*
 * Structure stored in mbuf in inpcb.ip_options
 * and passed to ip_output when ip options are in use.
 * The actual length of the options (including ipopt_dst)
 * is in m_len.
 */
#define MAX_IPOPTLEN	40

struct ipoption {
	struct	in_addr ipopt_dst;	/* first-hop dst if source routed */
	u_char	ipopt_list[MAX_IPOPTLEN];	/* options proper */
};

/*
 * Structure attached to inpcb.ip_moptions and
 * passed to ip_output when IP multicast options are in use.
 */
struct ip_moptions {
	struct	ifnet *imo_multicast_ifp; /* ifp for outgoing multicasts */
	u_char	imo_multicast_ttl;	/* TTL for outgoing multicasts */
	u_char	imo_multicast_loop;	/* 1 => hear sends if a member */
	u_short	imo_num_memberships;	/* no. memberships this socket */
	struct	in_multi *imo_membership[IP_MAX_MEMBERSHIPS];
	u_long  imo_multicast_vif;  /* vif num outgoing multicasts */
};

struct	ipstat {
	u_long	ips_total;			/* total packets received */
	u_long	ips_badsum;			/* checksum bad */
	u_long	ips_tooshort;		/* packet too short */
	u_long	ips_toosmall;		/* not enough data */
	u_long	ips_badhlen;		/* ip header length < data size */
	u_long	ips_badlen;			/* ip length < ip header length */
	u_long	ips_fragments;		/* fragments received */
	u_long	ips_fragdropped;	/* frags dropped (dups, out of space) */
	u_long	ips_fragtimeout;	/* fragments timed out */
	u_long	ips_forward;		/* packets forwarded */
	u_long	ips_cantforward;	/* packets rcvd for unreachable dest */
	u_long	ips_redirectsent;	/* packets forwarded on same net */
	u_long	ips_noproto;		/* unknown or unsupported protocol */
	u_long	ips_delivered;		/* datagrams delivered to upper level*/
	u_long	ips_localout;		/* total ip packets generated here */
	u_long	ips_odropped;		/* lost packets due to nobufs, etc. */
	u_long	ips_reassembled;	/* total packets reassembled ok */
	u_long	ips_fragmented;		/* datagrams sucessfully fragmented */
	u_long	ips_ofragments;		/* output fragments created */
	u_long	ips_cantfrag;		/* don't fragment flag was set, etc. */
	u_long	ips_badoptions;		/* error in option processing */
	u_long	ips_noroute;		/* packets discarded due to no route */
	u_long	ips_badvers;		/* ip version != 4 */
	u_long	ips_rawout;			/* total raw ip packets generated */
	u_long	ips_toolong;		/* ip length > max ip packet size */
};

#ifdef SPF_DRVR

/* flags passed to ip_output as last parameter */
#define	IP_FORWARDING		0x1		/* most of ip header exists */
#define	IP_RAWOUTPUT		0x2		/* raw ip header exists */
#define	IP_ROUTETOIF		SO_DONTROUTE	/* bypass routing tables */
#define	IP_ALLOWBROADCAST	SO_BROADCAST	/* can send broadcast packets */

#ifdef SPIP
EXTERN struct	ipstat	ipstat;
EXTERN struct	ipq	ipq;			/* ip reass. queue */
EXTERN u_short	ip_id;				/* ip packet ctr, for ids */
EXTERN int		ip_defttl;			/* default IP ttl */

int	 in_control __P((struct socket *, int, caddr_t, struct ifnet *));
int	 ip_ctloutput __P((int, struct socket *, int, int, struct bsd_mbuf **));
void	 ip_deq __P((struct ipasfrag *));
int	 ip_dooptions __P((struct bsd_mbuf *));
void	 ip_drain __P((void));
void	 ip_enq __P((struct ipasfrag *, struct ipasfrag *));
void	 ip_forward __P((struct bsd_mbuf *, int));
void	 ip_freef __P((struct ipq *));
void	 ip_freemoptions __P((struct ip_moptions *));
int	 ip_getmoptions __P((int, struct ip_moptions *, struct bsd_mbuf **));
void	 ip_init __P((void));
int	 ip_mforward __P((struct bsd_mbuf *, struct ifnet *));
int	 ip_optcopy __P((struct ip *, struct ip *));
int	 ip_output __P((struct bsd_mbuf *,
	    struct bsd_mbuf *, struct route *, int, struct ip_moptions *));
int	 ip_pcbopts __P((struct bsd_mbuf **, struct bsd_mbuf *));
struct ip *
	 ip_reass __P((struct ipasfrag *, struct ipq *));
struct in_ifaddr *
	 ip_rtaddr __P((struct in_addr));
int	 ip_setmoptions __P((int, struct ip_moptions **, struct bsd_mbuf *));
void	 ip_slowtimo __P((void));
struct bsd_mbuf *
	 ip_srcroute __P((void));
void	 ip_stripoptions __P((struct bsd_mbuf *, struct bsd_mbuf *));
int	 ip_sysctl __P((int *, u_int, void *, size_t *, void *, size_t));
void	 ipintr __P((struct bsd_mbuf *, struct ifnet *));
#endif /* SPIP */

#ifdef SPRAW
int	 rip_ctloutput __P((int, struct socket *, int, int, struct bsd_mbuf **));
void	 rip_init __P((void));
void	 rip_input __P((struct bsd_mbuf *, int));
int	 rip_output __P((struct bsd_mbuf *, struct socket *, u_long));
int	 rip_usrreq __P((struct socket *,
	    int, struct bsd_mbuf *, struct bsd_mbuf *, struct bsd_mbuf *));
#endif /* SPRAW */

#endif /* SPF_DRVR */

#endif /*_NETINET_IP_VAR_H*/
