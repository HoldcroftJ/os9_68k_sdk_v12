/****************************************************************************
 *           Copyright 1989-2001 by Microware Systems Corporation           *
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
 *  01 10/08/97 Creation
 *  02 01/29/98 Added rpcent structure                                 tmd
 *              Non-OS9 versions of inet_lnaof & inet_netof return     tmd
 *              ints to match netdb.c in idbgen directory              tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  03 01/19/99 Added mw_flags to n_ifnet structure                    rds
 *  04 07/05/01 Added support to HOST.CONF. The hostconfent structure  vd
 *              and the put,get,end,del functions were also added.
 *  05 08/01/01 CF11923 - Added some more function prototypes here:    mdu
 *                        putnetent, putprotoent, putservent
 *  06 09/10/01 Added delservbyname() prototype                        zw   
 ****************************************************************************/
/*-
 * Copyright (c) 1980, 1983, 1988, 1993
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
 *      @(#)netdb.h	8.1 (Berkeley) 6/2/93
 *	$Id: netdb.h,v 4.9.1.2 1993/05/17 09:59:01 vixie Exp $
 * -
 * Portions Copyright (c) 1993 by Digital Equipment Corporation.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies, and that
 * the name of Digital Equipment Corporation not be used in advertising or
 * publicity pertaining to distribution of the document or software without
 * specific, written prior permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND DIGITAL EQUIPMENT CORP. DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL DIGITAL EQUIPMENT
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 * -
 * --Copyright--
 */

#ifndef _NETDB_H_
#define _NETDB_H_
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define	_PATH_HEQUIV	"/etc/hosts.equiv"
#define	_PATH_HOSTS	"/etc/hosts"
#define	_PATH_NETWORKS	"/etc/networks"
#define	_PATH_PROTOCOLS	"/etc/protocols"
#define	_PATH_SERVICES	"/etc/services"

/*
 * Structures returned by network data base library. All addresses are
 * supplied in host order, and returned in network order (suitable for
 * use in system calls).
 */
struct	hostent {
	char	*h_name;		/* official name of host */
	char	**h_aliases;	/* alias list */
	int	h_addrtype;			/* host address type */
	int	h_length;			/* length of address */
	char	**h_addr_list;	/* list of addresses from name server */
#define	h_addr	h_addr_list[0]	/* address, for backward compatiblity */
};

/*
 * Assumption here is that a network number
 * fits in an unsigned long -- probably a poor one.
 */
struct	netent {
	char	*n_name;		/* official name of net */
	char	**n_aliases;	/* alias list */
	int		n_addrtype;		/* net address type */
	unsigned long	n_net;	/* network # */
};

struct	servent {
	char	*s_name;	/* official service name */
	char	**s_aliases;	/* alias list */
	int	s_port;		/* port # */
	char	*s_proto;	/* protocol to use */
};

struct	protoent {
	char	*p_name;		/* official protocol name */
	char	**p_aliases;	/* alias list */
	int	p_proto;			/* protocol # */
};

struct 	hostconfent {
	char 	*key;
	char 	*value;
};

/*Inetd entry structure for idbgen*/
typedef struct inetdent {
	char*		service_name;	/* service name */
	int			socket_type;	/* socket type */
	char*		protocol;		/* socket protocol type*/
	int			wait_status;	/* child wait status*/
	owner_id	uid;			/* user ID to set child*/
	char**		argv;			/* child / child argument list*/
} inetdent, *Inetdent;

struct	rpcent {
	char	*r_name;		/* name of server for this rpc program */
	char	**r_aliases;	/* alias list */
	int		r_number;		/* rpc program number */
};


/*
** The following structures are used by the OS-9/OS-9000 utilites idbgen,
** ndbmod, ifconfig to add/remove interface and route entries to the
** OS-9/OS-9000 BSDv4.4 IP.
*/
/* BSD 4.4 add interface structure */
typedef struct n_ifnet {
	char	if_name[16]; /* Taken from IFNAMSIZ */
	char	if_stack_name[30];
	short	if_flags;
	struct n_if_data {
		u_long	ifi_type;
		u_long	ifi_addrlen;
		u_long	ifi_mtu;
		u_long	ifi_metric;
	} if_data;
	u_long	mw_flags;	/* Microware defined interface flags */
/*
** The low 16 bits are used as "not" versions of the net/if.h defined
** interface flags. These are used to override driver defaults and must
** match the value of the corresponding flag defined in net/if.h. Currently
** only broadcast, multicast, and pointopoint are defined. The high 16 bits
** are available to be used for any future requirements.
*/
#define IFF_UNDEF01		0x1
#define IFF_NOBROADCAST	0x2
#define IFF_UNDEF02		0x4
#define IFF_UNDEF03		0x8
#define IFF_NOPOINTOPOINT 0x10
#define IFF_UNDEF04		0x20
#define IFF_UNDEF05		0x40
#define IFF_UNDEF06		0x80
#define IFF_UNDEF07		0x100
#define IFF_UNDEF08		0x200
#define IFF_UNDEF09		0x400
#define IFF_UNDEF10		0x800
#define IFF_UNDEF11		0x1000
#define IFF_UNDEF12		0x2000
#define IFF_UNDEF13		0x4000
#define IFF_NOMULTICAST	0x8000
	u_long	extra[5];	/* for future use */
} n_ifnet;

/* BSD 4.4 add/remove interface address structure */
typedef struct n_ifaliasreq {
	char	ifra_name[16]; /* Taken from IFNAMSIZ */
	struct	sockaddr ifra_addr;
	struct	sockaddr ifra_broadaddr;
#define ifra_dstaddr ifra_broadaddr
	struct	sockaddr ifra_mask;
} n_ifaliasreq;

/* BSD 4.4 add/remove route structure */

struct rtreq {
	int req;
	int flags;
	struct sockaddr dst;
	struct sockaddr gateway;
	struct sockaddr netmask;
};


/*
** Error return codes from gethostbyname() and gethostbyaddr()
** (left in extern int h_errno).
*/

#define	HOST_NOT_FOUND	1 /* Authoritative Answer Host not found */
#define	TRY_AGAIN		2 /* Non-Authoritive Host not found, or SERVERFAIL */
#define	NO_RECOVERY		3 /* Non recoverable errors, FORMERR, REFUSED, NOTIMP */
#define	NO_DATA			4 /* Valid name, no data record of requested type */
#define	NO_ADDRESS	NO_DATA	/* no address, look for MX record */

/*
** NetDB Trap Handler Function Codes
*/
#define NETDB_TRAP_NAME				"netdb"
#define NETDB_TRAP_NUMBER			9
#define NETDB_FC_INIT				0
#define NETDB_FC_INET_ADDR			1
#define NETDB_FC_INET_LNAOF			2
#define NETDB_FC_INET_MAKEADDR		3
#define NETDB_FC_INET_NETOF			4
#define NETDB_FC_INET_NETWORK		5
#define NETDB_FC_INET_NTOA			6
#define NETDB_FC_ENDHOSTENT			7
#define NETDB_FC_GETHOSTBYADDR		8
#define NETDB_FC_GETHOSTBYNAME		9
#define NETDB_FC_GETHOSTENT			10
#define NETDB_FC_SETHOSTENT			11
#define NETDB_FC_ENDNETENT			12
#define NETDB_FC_GETNETBYADDR		13
#define NETDB_FC_GETNETBYNAME		14
#define NETDB_FC_GETNETENT			15
#define NETDB_FC_SETNETENT			16
#define NETDB_FC_ENDPROTOENT		17
#define NETDB_FC_GETPROTOBYNAME		18
#define NETDB_FC_GETPROTOBYNUMBER	19
#define NETDB_FC_GETPROTOENT		20
#define NETDB_FC_SETPROTOENT		21
#define NETDB_FC_ENDSERVENT			22
#define NETDB_FC_GETSERVBYNAME		23
#define NETDB_FC_GETSERVBYPORT		24
#define NETDB_FC_GETSERVENT			25
#define NETDB_FC_SETSERVENT			26
#define NETDB_FC_ENDINETDENT		27
#define NETDB_FC_GETINETD			28
#define NETDB_FC_GETINETDENT		29
#define NETDB_FC_SETINETDENT		30
#define NETDB_FC_GETRESOLVENT		31
#define NETDB_FC_PUTHOSTENT			32
#define NETDB_FC_PUTRESOLVENT		33
#define NETDB_FC_GETINTENT			34
#define NETDB_FC_PUTINTENT			35
#define NETDB_FC_DELHOSTBYNAME		36
#define NETDB_FC_DELINTBYNAME		37
#define NETDB_FC_DELRESOLVENT		38
#define NETDB_FC_ENDINTENT			39
#define NETDB_FC_ENDRESOLVENT		40
#define NETDB_FC_PUTROUTENT			41
#define NETDB_FC_GETROUTENT			42
#define NETDB_FC_DELROUTENT			43 /* not used right now */
#define NETDB_FC_ENDROUTENT			44
#define NETDB_FC_RES_CANCEL			45
#define NETDB_FC_NUMBER				46

#include <sys/cdefs.h>


__BEGIN_DECLS
#if (defined(_OSK)||defined(_OS9000))
int		endhostent __P((void));
int		endnetent __P((void));
int		endprotoent __P((void));
int		endservent __P((void));
int		sethostent __P((int));
int		setnetent __P((int));
int		setprotoent __P((int));
int		setservent __P((int));
#else
void		endhostent __P((void));
void		endnetent __P((void));
void		endprotoent __P((void));
void		endservent __P((void));
void		sethostent __P((int));
void		setnetent __P((int));
void		setprotoent __P((int));
void		setservent __P((int));

#endif
struct hostent	*gethostbyaddr __P((const char *, int, int));
struct hostent	*gethostbyname __P((const char *));
struct hostent	*gethostent __P((void));
struct netent	*getnetbyaddr __P((long, int)); /* u_long? */
struct netent	*getnetbyname __P((const char *));
struct netent	*getnetent __P((void));
struct protoent	*getprotobyname __P((const char *));
#if (defined(_OSK)||defined(_OS9000))
struct protoent	*getprotobynumber __P((long));
struct servent	*getservbyport __P((long, const char *));
#else
struct protoent	*getprotobynumber __P((int));
struct servent	*getservbyport __P((int, const char *));
#endif
struct protoent	*getprotoent __P((void));
struct servent	*getservbyname __P((const char *, const char *));
struct servent	*getservent __P((void));
void		herror __P((const char *));
char		*hstrerror __P((int));

/* void		sethostfile __P((const char *)); */

/* functions normally in inet.h*/
#if (defined(_OSK)||defined(_OS9000))
char			*inet_ntoa __P((struct in_addr in));
unsigned long	inet_addr __P((const char *));
int				inet_aton __P((const char *, struct in_addr *));
int				inet_lnaof __P((struct in_addr));
struct in_addr	inet_makeaddr __P((int, int));
int				inet_netof __P((struct in_addr));
unsigned long	inet_network __P((const char *));
char			*inet_ntoa __P((struct in_addr));
#else
char			*inet_ntoa __P((struct in_addr in));
unsigned long	inet_addr __P((const char *));
int				inet_aton __P((const char *, struct in_addr *));
int				inet_lnaof __P((struct in_addr));
struct in_addr	inet_makeaddr __P((u_long , u_long));
int				inet_netof __P((struct in_addr));
unsigned long	inet_network __P((const char *));
char			*inet_ntoa __P((struct in_addr));
#endif

/*Microware defined functions*/
error_code		puthostent __P((const struct hostent *));
char			*getintent __P((void));
error_code		putintent __P((n_ifnet *, n_ifaliasreq *, u_int32));
error_code		delhostbyname __P((char *));
error_code      delservbyname __P((char* ));
error_code		delintbyname __P((char *));
error_code		endintent __P((void));
error_code		netdb_link __P((int));
error_code		putroutent __P((struct rtreq *));
struct rtreq	*getroutent __P((void));
error_code		delroutent __P((const struct rtreq *));
error_code		endroutent __P((void));
struct hostconfent	*gethostconfent __P((void));
error_code		puthostconfent __P((struct hostconfent *));
error_code		delhostconfent __P((char *));
error_code		endhostconfent __P((void));
void			res_cancel __P((void));
struct inetdent* getinetd __P((struct inetdent* ient));
struct inetdent* getinetdent __P((void));
error_code		putnetent __P((const struct netent *));
error_code		putprotoent __P((const struct protoent *));
error_code		putservent __P((const struct servent *));


/* not trap functions */
error_code		netdb_unlink __P((int));
__END_DECLS


#endif /* !_NETDB_H_ */
