/****************************************************************************
 *  ID:		%A%
 *  Date:	%G%
 ****************************************************************************
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
 *  01 04/22/96 Initial Version                                        billw
 *  16 03/26/97 Changed to use BSD header files.                       sr
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#ifndef _compress_h_
#define _compress_h_


#ifndef CS_MAX_STATES
	#define CS_MAX_STATES		16			/* 2 < CS_MAX_STATES < 255	*/
#endif /* CS_MAX_STATES */

#define CS_MAX_HDR				128			/* Max tcp/ip header	*/

/* Packet types */

#define TYPE_IP					0x40
#define TYPE_UNCOMPRESSED_TCP	0x70
#define TYPE_COMPRESSED_TCP		0x80
#define TYPE_ERROR				0x00	/* Rx uses this for rcv error flag	*/

typedef struct tcpiphdr tcp_ip_header, *Tcp_ip_header, TCP_IP_HEADER;
typedef struct ip ip_header, *Ip_header, IP_HEADER;
typedef struct tcphdr tcp_header, *Tcp_header, TCP_HEADER;
/*
 * Connection state structure entry for each active
 * TCP conversation on the line.
 */

typedef struct cstate	cstate, *Cstate;

struct cstate
{
	Cstate			cs_next;		/* Most recently used cstate next ptr	*/
	u_short			cs_hlen;		/* Size of header (rcv direction only)	*/
	u_char			cs_id,			/* The connection number				*/
					cs_rsv;			/* Long word boundary					*/
	union
	{
		char			csu_hdr[CS_MAX_HDR];
		TCP_IP_HEADER	csu_ip;			/* TCP/IP pkt from most recent pkt	*/
	} slcs_u;
};

/*
 * State data table structure for a serial line.
 */

typedef struct slcompress	slcompress, *Slcompress;

struct slcompress
{
	Cstate			last_cs;
	u_char			last_recv,
					last_xmit;
	u_short			flags;
	cstate			tstate[CS_MAX_STATES],		/* TX connection states	*/
					rstate[CS_MAX_STATES];		/* RX connection states	*/
};


#ifdef _compress_c_		/* Definitions used only in compress.c */

	#define	T_DOFF_SHR				4


	/* Bits in the first octet of a compressed packet */

	#define NEW_C					0x40
	#define NEW_I					0x20
	#define TCP_PUSH_BIT			0x10

	#define NEW_S					0x08
	#define NEW_A					0x04
	#define NEW_W					0x02
	#define NEW_U					0x01

	/* The special case values */

	#define SPECIAL_I (NEW_S | NEW_W | NEW_U)			/* SWU	*/
	#define SPECIAL_D (NEW_S | NEW_A | NEW_W | NEW_U)	/* SAWU	*/
	#define SPECIALS_MASK (NEW_S | NEW_A | NEW_W | NEW_U)

	/*
	 * segment reassembly queue structure
	 * It is vitally important that this structure does not
	 * kill the ip length field, so we copy it into the structure.
	 * Also note that reassembly header cannot be bigger than ip header
	 * because the tcp header must remain intact.
	 */

	typedef struct mbuf				ppp_mbuf, *Ppp_mbuf;

	#define CS_IP	slcs_u.csu_ip.ti_i
	#define CS_HDR	slcs_u.csu_hdr
	#define CS_TCP	slcs_u.csu_ip.ti_t

	/* Flag values for struct slcompress */

	#define SLF_TOSS	1			/* Toss rx frames due to input err	*/

	/* The following macros are used to encode and decode the header
	 * values being compressed. "cp" points to a buffer where the next
	 * byte encoded/decoded is to be stored/retreived. Since the decode
	 * routines do arithmetic, they have to convert from/to network
	 * byte order.
	 */

	/* ENCODE: encode a number that is known to be non-zero. ENCODEZ checks
	 * for zero and encodes it in the long, 3 byte form.
	 */

	#define ENCODE(n) \
			{ \
				if ((u_short)(n) >= 256) \
				{ \
					*cp++ = 0; \
					cp[1] = (n); \
					cp[0] = (n) >> 8; \
					cp += 2; \
				} \
				else \
				{ \
					*cp++ = (n); \
				} \
			}

	#define ENCODEZ(n) \
			{ \
				if (((u_short)(n) >= 256) || ((u_short)(n) == 0)) \
				{ \
					*cp++ = 0; \
					cp[1] = (n); \
					cp[0] = (n) >> 8; \
					cp += 2; \
				} \
				else \
				{ \
					*cp++ = (n); \
				} \
			}

	/* DECODEL takes the (compressed) change at byte cp and adds it to the
	 * current value of packet field "f" (which must be a 4 byte integer
	 * in network byte order. DECODES does the same for a 2 byte field.
	 * DECODEU takes the change at cp and stuffs it into field "f". "cp" is
	 * updated to point to the next field in the compressed header.
	 */

	#define DECODEL(f) \
			{ \
				if (*cp == 0) \
				{ \
					(f) = htonl(ntohl(f) + ((cp[1] << 8) | cp[2])); \
					cp += 3; \
				} \
				else \
				{ \
					(f) = htonl(ntohl(f) + (u_int32)*cp++); \
				} \
			}

	#define DECODES(f) \
			{ \
				if (*cp == 0) \
				{ \
					(f) = htons(ntohs(f) + ((cp[1] << 8) | cp[2])); \
					cp += 3; \
				} \
				else \
				{ \
					(f) = htons(ntohs(f) + (u_int32)*cp++); \
				} \
			}

	#define DECODEU(f) \
			{ \
				if (*cp == 0) \
				{ \
					(f) = htons((cp[1] << 8) | cp[2]); \
					cp += 3; \
				} \
				else \
				{ \
					(f) = htons((u_int32)*cp++); \
				} \
			}

#endif /* _compress_c_ */

#endif /* _compress_h_ */
