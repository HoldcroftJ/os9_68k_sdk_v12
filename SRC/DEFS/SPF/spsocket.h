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
 *  01 07/15/97 Created                                                rds
 *  02 08/24/97 added defines from protocols sp???.h files             rds
 *  03 09/24/97 added support for sproute                              rds
 *  04 01/15/98 changed **ifnet to ***ifnet                            rds
 *              changed **mask_rnhead to ***mask_rnhead                rds
 *              added syscopy to ip_funcs                              rds
 *              added copyin and copyout macros                        rds
 *              added forward definition for struct uio                rds
 *  05 02/17/98 Added more forward definitions                         rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  06 07/13/98 Added functions required for multicasting              rds
 *  07 06/28/99 Added functions for zero copy TCP                      rds
 *  08 08/24/00 Added soreceive                                        rds
 *  09 02/13/01 Added _lancomm_version what string                     rds
 ****************************************************************************/
#ifndef _SPSOCKET_H
#define _SPSOCKET_H

#define SS_SOCKET_BASE (SPF_PR_SOCKET << 16)

#define SOCKET_SS_REG (SS_SOCKET_BASE + 1)
#define SOCKET_SS_CREATE (SS_SOCKET_BASE + 2)

#define SOCKET_GS_TYPE (SS_SOCKET_BASE + 128)
#define REG_DOMAIN 1
#define REG_PROTO 2

struct socket_args {
	int domain;
	int type;
	int protocol;
};

/*
** This string is used by the core LAN Communications Pak modules. The
** ifdefs are to ensure the what string is only included in those modules,
** and that each one only includes it once.
*/
#ifdef SYSEDIT
  #if defined(SPIP) || defined(SPTCP) || defined(SPUDP) || \
      defined(SPRAW) || defined(SPROUTE)
  char _lancomm_version[] =
      "@(#)LAN Communications Pak version 3.5.0 (" __DATE__ ")";
  #endif
#endif /* SYSEDIT */


/*
** Forward declarations to avoid compiler warnings in prototypes
*/
struct uio;
struct inpcb;
struct in_addr;
struct ip_moptions;
struct bsd_mbuf;
struct socket;
struct sockbuf;
struct route;
struct ifnet;
struct selinfo;

struct ip_funcs {
	int num_funcs;

	int (**in_cksum)(struct bsd_mbuf *, int);
	int (**in_pcballoc)(struct socket *, struct inpcb *);
	int (**in_pcbbind)(struct inpcb *, struct bsd_mbuf *);
	int (**in_pcbconnect)(struct inpcb *, struct bsd_mbuf *);
	void (**in_pcbdisconnect)(struct inpcb *);
	void (**in_pcbdetach)(struct inpcb *);
	struct inpcb *(**in_pcblookup)(struct inpcb *, struct in_addr, u_int,
				struct in_addr, u_int, int);
	int (**soisconnecting)(struct socket *);
	int (**soisconnected)(struct socket *);
	int (**sosend)(struct socket *, struct bsd_mbuf *, struct uio *,
				struct bsd_mbuf *, struct bsd_mbuf *, int);
	void (**in_losing)(struct inpcb *);
	int (**sowakeup)(struct socket *, struct sockbuf *);
	int (**soreserve)(struct socket *, u_long, u_long);
	int (**soisdisconnecting)(struct socket *);
	int (**soisdisconnected)(struct socket *);
	int (**socantsendmore)(struct socket *);
	int (**socantrcvmore)(struct socket *);
	int (**soabort)(struct socket *);
	int (**soclose)(struct socket *);
	int (**sohasoutofband)(struct socket *);
	struct socket *(**sonewconn1)(struct socket *, int);
	void (**sbappend)(struct sockbuf *, struct bsd_mbuf *);
	int (**sbdrop)(struct sockbuf *, int);
	int (**sbflush)(struct sockbuf *);
	int (**sbappendaddr)(struct sockbuf *, struct sockaddr *,
				struct bsd_mbuf *, struct bsd_mbuf *);
	int (**ip_output)(struct bsd_mbuf *, struct bsd_mbuf *, struct route *,
				int, struct ip_moptions *);
	void (**rtalloc)(struct route *);
	int (**sbreserve)(struct sockbuf *, u_long);
	int (**in_broadcast)(struct in_addr, struct ifnet *);
	int **ip_defttl;
	void (**ip_stripoptions)(struct bsd_mbuf *, struct bsd_mbuf *);
	int **sb_max;
	int **max_linkhdr;
	struct ifnet ***ifnet;
	u_short **ip_id;
	struct ipstat **ipstat;
	void (**in_pcbnotify)(struct inpcb *, struct sockaddr *, u_int, u_int,
				struct in_addr, int, void (*)());
	int (*(*inetctlerrmap))[];
	struct ifaddr *(**ifa_ifwithaddr)(struct sockaddr *);
	void (**icmp_error)(struct bsd_mbuf *, int, int, u_long, struct ifnet *);
	struct ifaddr *(**ifa_ifwithnet)(struct sockaddr *);
	struct ifaddr *(**ifa_ifwithroute)(int, struct sockaddr *, struct sockaddr *);
	struct ifaddr *(**ifaof_ifpforaddr)(struct sockaddr *, struct ifnet *);
	struct radix_node_head *(*(*rt_tables))[];
	void (**ifafree)(struct ifaddr *);
	struct rtentry *(**rtalloc1)(struct sockaddr *, int);
	int (**rt_setgate)(struct rtentry *, struct sockaddr *, struct sockaddr *);
	int (**rtrequest)(int, struct sockaddr *, struct sockaddr *, struct sockaddr *,
				int, struct rtentry **);
	void (**rtfree)(struct rtentry *);
	struct radix_node *(**rn_search)(void *, struct radix_node *);
	struct radix_node *(**rn_addmask)(void *, int, int);
	struct radix_node_head ***mask_rnhead;
	int (**sofree)(struct socket *);
	struct routefuncs **rtfuncs;
	int (**syscopy)(void *, void *, size_t, u_int32, void *);
	struct protosw (*(*inetsw))[];
	struct socket **ip_mrouter;
	int (*(**_ip_mrouter_set))(int, struct socket *, struct bsd_mbuf *);
	int (*(**_ip_mrouter_get))(int, struct socket *, struct bsd_mbuf **);
	int (*(**_ip_mrouter_done))(void);
	int (**sbwait)(struct sockbuf *);
	int (**sock_sleep)(int, struct selinfo *);
	int (**in_localaddr)(struct in_addr);
	int (**soreceive)(struct socket *, void *);
};

struct routefuncs {
	void *globs;
	void (*rt_missmsg)();
	void (*rt_ifmsg)();
	void (*rt_newaddrmsg)();
};

struct net_register {
	int code;			/* register protocol (REG_PROTO) or domain (REG_DOMAIN) */
	void *param;		/* pointer to protosw or domain structure. */
	void *funcs;		/* requested functions for a particular domain */
	void *globs;		/* IP returns it globals so we can set them */
};

#define bcopy(s, d, n) memmove(d, s, n)
#define bzero(a, n) memset(a, 0, n)
#define bcmp(x, y, z) memcmp(x, y, z)

#define copyin(u, k, l) syscopy(u, k, l, S_IREAD, u)
#define copyout(k, u, l) syscopy(k, u, l, S_IWRITE, u)

#define DEBUG_DATA(a,b,c)

#define BSDCALL(s, a) { \
	void *old_static = (void *)change_static((s)); \
	a; \
	change_static(old_static); \
}

#define ADD_ENTRY(new, old) { \
	((Pp_hdr)new)->next = ((Pp_hdr)old)->next; \
	((Pp_hdr)new)->prev = (Pp_hdr)old; \
	((Pp_hdr)old)->next = (Pp_hdr)new; \
	((Pp_hdr)new)->next->prev = (Pp_hdr)new; \
}

#define DROP_ENTRY(x_list) { \
	Pp_hdr xx_list = ((Pp_hdr)x_list); \
	xx_list->next->prev = xx_list->prev; \
	xx_list->prev->next = xx_list->next; \
}

#define INIT_ENTRY(x_list) { \
	((Pp_hdr)x_list)->next = ((Pp_hdr)x_list)->prev = (Pp_hdr)x_list; \
}

#undef insque
#define insque(node, prev) ADD_ENTRY(node, prev)

#undef remque
#define remque(node) DROP_ENTRY(node)

#undef DTOM
#define DTOM(n) (*((mbuf **)(((char *)n) - 4)))

#if 0
	void splx(int s);
	int splnet(void);
	int splimp(void);
	#define NETLVL (6<<8)
	#define IMPLVL (6<<8)
#else
	#define MWIRQ(a)
	#define splx(x)
#endif /* 0 */

/*
** MCLBYTES in BSD is the maximum size of an mbuf cluster. It is used to
** round down MSS's to the nearest multiple of itself. Since we don't use
** clusters that code has been commented out in tcp_input.c. It is still
** used as the low water mark in the send socket buffer (soreserve) and
** to check the size of the receive socket buffer (sbreserve).
*/
#define MCLBYTES 2048

#define m_copydata(mb, offset, cnt, buf) \
	m_move((Mbuf)mb, (u_int32)offset, (u_int32)cnt, (char *)buf)

#endif /* _SPSOCKET.H */
