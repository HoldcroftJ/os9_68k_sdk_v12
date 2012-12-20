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
 * 08  10/11/96 Changed Protocol address fields in arp_msg struct to   sr
 *              u_int8 to avoid alignment/padding by complier for ppc
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *     05/20/97 <--------- SPF LAN Comm Pack v3.1.1 Release -------->
 *  32 04/01/98 Cleaned up code.                                       sr
 *     04/01/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  37 11/22/99 Added arptab, arpstat, and in_addr structures          rds
 *              Removed unused defines, added default timer defines
 ****************************************************************************/

#ifndef _ARP_H
#define _ARP_H

/*************************/
/* Constant Declarations */
/*************************/

/* ARP message hardware type */
#define ETHERHARDTYPE   0x01

/* ARP message protocol type */
#define IPPROTTYPE      0x0800

/* ARP message protocol address size */
#define IPPROTSIZE      0x04

/* ARP message OP field values */
#define ARP_REQUEST     0x01
#define ARP_REPLY       0x02
#define RARP_REQUEST    0x03
#define RARP_REPLY      0x04

/* default timer values */
#define DEF_TIMER_INT   60  /* Timer interval value in seconds */
#define DEF_KILL_C      20  /* kill completed entry after 20 timer intervals */
#define DEF_KILL_I      3   /* kill incomplete entry after 3 tiemr intervals */

#define ETHERHARDSIZE   6
#define ETHERHEADSIZE   sizeof(struct ether_header)

/*
** The in_addr and arptab structures are needed when making both
** descriptors and the driver. The driver is no problem because it
** includes all the required BSD header files. Rather than adding
** all the BSD header files to defs.h when making descriptors,
** I put these two structures here. This is probably a bad idea.
*/
#ifndef _NETINET_IN_H_
struct in_addr {
	u_long s_addr;
};
#endif

#ifndef _NETINET_IF_ETHER_H
typedef struct arptab {
	struct	arptab *next, *prev;	/* chain pointers */
	struct	mbuf *at_hold;			/* last packet until resolved/timeout */
	int32	at_grant;				/* srqmem grant size */
	struct	in_addr at_iaddr;		/* internet address */
	u_char	at_enaddr[6];			/* ethernet address */
	u_char	at_timer;				/* minutes since last reference */
	u_char	at_flags;				/* flags */
	u_int32	at_asked;				/* timestamp when last request sent */
	u_int32	reserved[2];			/* reserved fields */
} arptab, *Arptab;
#endif

typedef struct arpstat {
	arptab		arproot;
	u_int32		*timer_sec;
	u_int8		busy_flag;
	u_int8		reserved;
	int16		busy_count;
} arpstat, *Arpstat;

#endif /* _ARP_H */
