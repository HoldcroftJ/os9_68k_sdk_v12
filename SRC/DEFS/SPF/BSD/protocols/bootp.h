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
 *   0 ??/??/?? Birth                                                  ???
 *     09/16/93 <--------- ISP 2.0 Release ------------------------->
 *     01/17/96 <--------- ISP 2.1 Release ------------------------->
 *     05/31/96 Added individual byte defs for rfc1048 magic cookie    mgh
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *     05/07/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  02 04/02/98 Add #ifdef for multiple include protection             rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  03 05/16/00 Updated from current FreeBSD, and our local dhcp copy  rds
 ****************************************************************************/

/*****************************************************************************
*           Copyright 1988, 1991 by Carnegie Mellon University               *
*                                                                            *
*                           All Rights Reserved                              *
*                                                                            *
* Permission to use, copy, modify, and distribute this software and its      *
* documentation for any purpose and without fee is hereby granted, provided  *
* that the above copyright notice appear in all copies and that both that    *
* copyright notice and this permission notice appear in supporting           *
* documentation, and that the name of Carnegie Mellon University not be used *
* in advertising or publicity pertaining to distribution of the software     *
* without specific, written prior permission.                                *
*                                                                            *
* CARNEGIE MELLON UNIVERSITY DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS    *
* SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. *
* IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL *
* DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR      *
* PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS    *
* ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS*
* SOFTWARE.                                                                  *
******************************************************************************/

#ifndef _PROTOCOLS_BOOTP_H
#define _PROTOCOLS_BOOTP_H

#define BP_CHADDR_LEN   16
#define BP_SNAME_LEN    64
#define BP_FILE_LEN    128
#define BP_VEND_LEN     64
#define BP_MINPKTSZ    300 /* to check sizeof(struct bootp) */

struct bootp {
	unsigned char	bp_op;			/* packet opcode type */
	unsigned char	bp_htype;		/* hardware addr type */
	unsigned char	bp_hlen;		/* hardware addr length */
	unsigned char	bp_hops;		/* gateway hops */
	unsigned long	bp_xid;			/* transaction ID */
	unsigned short	bp_secs;		/* seconds since boot began */
	unsigned short	bp_flags;		/* RFC1532 broadcast, etc. */
	struct in_addr	bp_ciaddr;		/* client IP address */
	struct in_addr	bp_yiaddr;		/* 'your' IP address */
	struct in_addr	bp_siaddr;		/* server IP address */
	struct in_addr	bp_giaddr;		/* gateway IP address */
	unsigned char	bp_chaddr[BP_CHADDR_LEN];	/* client hardware address */
	char			bp_sname[BP_SNAME_LEN];		/* server host name */
	char			bp_file[BP_FILE_LEN];		/* boot file name */
	unsigned char	bp_vend[BP_VEND_LEN];		/* vendor-specific area */
	/* note that bp_vend can be longer, extending to end of packet */
};

/*
 * UDP port numbers, server and client.
 */
#define	IPPORT_BOOTPS		67
#define	IPPORT_BOOTPC		68

#define BOOTREPLY		2
#define BOOTREQUEST		1

/*
 * Hardware types from Assigned Numbers RFC.
 */
#define HTYPE_ETHERNET		1
#define HTYPE_EXP_ETHERNET	2
#define HTYPE_AX25			3
#define HTYPE_PRONET		4
#define HTYPE_CHAOS			5
#define HTYPE_IEEE802		6
#define HTYPE_ARCNET		7

/*
 * Vendor magic cookie (v_magic) for CMU
 */
#define VM_CMU		"CMU"

/*
 * Vendor magic cookie (v_magic) for RFC1048
 */
#define VM_RFC1048	{ 99, 130, 83, 99 }
#define VMC_0 99
#define VMC_1 130
#define VMC_2 83
#define VMC_3 99

/*
 * RFC1048 tag values used to specify what information is being supplied in
 * the vendor field of the packet.
 */
#define TAG_END				((unsigned char) 255)
#define TAG_PAD				((unsigned char)   0)
#define TAG_SUBNET_MASK		((unsigned char)   1)
#define TAG_TIME_OFFSET		((unsigned char)   2)
#define TAG_GATEWAY			((unsigned char)   3)
#define TAG_TIME_SERVER		((unsigned char)   4)
#define TAG_NAME_SERVER		((unsigned char)   5)
#define TAG_DOMAIN_SERVER	((unsigned char)   6)
#define TAG_LOG_SERVER		((unsigned char)   7)
#define TAG_COOKIE_SERVER	((unsigned char)   8)
#define TAG_LPR_SERVER		((unsigned char)   9)
#define TAG_IMPRESS_SERVER	((unsigned char)  10)
#define TAG_RLP_SERVER		((unsigned char)  11)
#define TAG_HOSTNAME		((unsigned char)  12) /* Compatability w/bootpd */
#define TAG_HOST_NAME		((unsigned char)  12)
#define TAG_BOOTSIZE		((unsigned char)  13)
#define TAG_DUMP_FILE		((unsigned char)  14)
#define TAG_DOMAIN_NAME		((unsigned char)  15)
#define TAG_SWAP_SERVER		((unsigned char)  16)
#define TAG_ROOT_PATH		((unsigned char)  17)
#define TAG_EXTEN_FILE		((unsigned char)  18)
#define TAG_IP_FORWARDING	((unsigned char)  19)
#define TAG_NL_SRC_RT		((unsigned char)  20)
#define TAG_POLICY_FILTER	((unsigned char)  21)
#define TAG_MAX_DGRAM		((unsigned char)  22)
#define TAG_IP_TTL			((unsigned char)  23)
#define TAG_PATH_MTU_AGING	((unsigned char)  24)
#define TAG_PATH_MTU_TAB	((unsigned char)  25)
#define TAG_INT_MTU			((unsigned char)  26)
#define TAG_SUBNETS_LOCAL	((unsigned char)  27)
#define TAG_BROADCAST_ADDR	((unsigned char)  28)
#define TAG_DO_MASK_DISC	((unsigned char)  29)
#define TAG_MASK_SUPPLIER	((unsigned char)  30)
#define TAG_DO_ROUTE_DISC	((unsigned char)  31)
#define TAG_RT_SOLIC_ADDR	((unsigned char)  32)
#define TAG_STATIC_ROUTE	((unsigned char)  33)
#define TAG_TRAILER_ENCAP	((unsigned char)  34)
#define TAG_ARP_TIMEOUT		((unsigned char)  35)
#define TAG_ETH_ENCAP		((unsigned char)  36)
#define TAG_TCP_TTL			((unsigned char)  37)
#define TAG_TCP_KP_ALIVE	((unsigned char)  38)
#define TAG_TCP_KP_GARBAGE	((unsigned char)  39)
#define TAG_NIS_DOMAIN		((unsigned char)  40)
#define TAG_NIS_SERVER		((unsigned char)  41)
#define TAG_NTP_SERVER		((unsigned char)  42)
#define TAG_VENDOR			((unsigned char)  43)
#define TAG_NBNS_SERVER		((unsigned char)  44)
#define TAG_NODE_TYPE		((unsigned char)  46)
#define TAG_NB_SCOPE		((unsigned char)  47)
/* DHCP maximum message size. */
#define TAG_DHCP_IP			((unsigned char)  50)
#define TAG_DHCP_IPLEASE	((unsigned char)  51)
#define TAG_DHCP_OVL		((unsigned char)  52)
#define TAG_DHCP_MSG		((unsigned char)  53)
#define TAG_DHCP_SERVERID	((unsigned char)  54)
#define TAG_DHCP_REQ		((unsigned char)  55)
#define TAG_MESSAGE			((unsigned char)  56)
#define TAG_MAX_MSGSZ		((unsigned char)  57)
#define TAG_DHCP_RENEW		((unsigned char)  58)
#define TAG_DHCP_REBIND		((unsigned char)  59)
#define TAG_DHCP_CLIID		((unsigned char)  61)
#define TAG_DHCP_BOOTSRV	((unsigned char)  66)
#define TAG_DHCP_BOOTFILE	((unsigned char)  67)
#define TAG_DHCP_USERCLASS	((unsigned char)  77)

/* XXX - Add new tags here */


/*
 * "vendor" data permitted for CMU bootp clients.
 */

struct cmu_vend {
	unsigned char	v_magic[4];	/* magic number */
	unsigned long	v_flags;	/* flags/opcodes, etc. */
	struct in_addr 	v_smask;	/* Subnet mask */
	struct in_addr 	v_dgate;	/* Default gateway */
	struct in_addr	v_dns1, v_dns2; /* Domain name servers */
	struct in_addr	v_ins1, v_ins2; /* IEN-116 name servers */
	struct in_addr	v_ts1, v_ts2;	/* Time servers */
	unsigned char	v_unused[25];	/* currently unused */
};


/* v_flags values */
#define VF_SMASK	1	/* Subnet mask field contains valid data */

#endif /* _PROTOCOLS_BOOTP_H */
