/****************************************************************************
 * Stacked Protocol Manager interface & data structure definitions.         *
 ****************************************************************************
 * Copyright 1995 by Microware Systems Corporation                          *
 * Reproduced Under License                                                 *
 *                                                                          *
 * This source code is the proprietary confidential property of             *
 * Microware Systems Corporation, and is provided to licensee               *
 * solely for documentation and educational purposes. Reproduction,         *
 * publication, or distribution in any form to any party other than         *
 * the licensee is strictly prohibited.                                     *
 ****************************************************************************
 * Edition History:                                                         *
 * #   Date     Comments                                          By        *
 * --- -------- ------------------------------------------------- --------- *
 *   1 03/06/95 Creation (from SPF version 1.0.2)                    bat    *
 *     06/30/95 <---------------- SPF v2.0(pre) ---------------->    cas    *
 *   2 10/16/95 Changed lu_txoffset to txoffset&trailer.  Adjusted   dwj    *
 *              spf_update_pb size correctly                                *
 *   3 10/17/95 Changed txoffset to txoffset/txtrailer in popts      dwj    *
 *              Added TXTRAILER definition.                                 *
 *     10/16/95 <------------------- SPF v2.0 ------------------>    dwj    *
 *                                                                          *
 *   4 03/22/96 added return_mbuf macro                              udayn  *
 *     09/12/96 Added PSI driver error codes                         jmh    *
 *   5 96/10/22 <---------------- SPF v2.1.1 ------------------->    cas    *
 *   6 01/15/97 Added BSD 4.4 getstats/setstats                      rds    *
 *   7 02/11/97 Added Bench Mark                                     taka   *
 *     02/19/97 Added per-path storage fields in lustat              jmh    *
 * 225 02/27/97 rearrange the spf_topdvr structure                   taka   *
 *     03/25/97 added SPF_PR_IRDA                                    taka   *
 * 228 03/27/97 IO_ASYNC path option change                          taka   *
 * 230 04/08/97 Added IO_PACKET_TRUNC in pd_iopacket option          taka   *
 * 231 04/17/97 <---------------- SPF v2.2 (beta)---------------->   taka   *
 * 232 04/21/97 txsize change from u_int32 to u_int16                taka   *
 *     04/28/97 Added SPF_SS_ADDARP, SPF_SS_DELARP, SPF_GS_ARPENT    taka   *
 *              SPF_GS_ARPTBL                                               *
 *     05/01/97 Added EOS_NODNDRVR and EOS_GENDRVR_BASE error codes  cas    *
 * 234 05/01/97 <---------------- SPF V2.2 (pre)----------------->   taka   *
 * 235 05/23/97 added SNMP definition                                taka   *
 *     06/17/97 added SPF_GS_SYMBOLS                                 taka   *
 * 236 06/26/97 <---------------- SPF V2.2 ---------------------->   taka   *
 *     07/18/97 Added SPF_PR_SOCKET definition to the Microware      taka   *
 *              protocol type.                                              *
 * 241 10/03/97 Added spf_pdstat.pd_flag element to support UDP      taka   *
 *              sendto().                                                   *
 * 246 11/11/97 IO_ASYNC path option bug fix: changed IO_ASYNC = 1   taka   *
 *     11/12/97 Added SPF_PR_USB definition                          taka   *
 * 250 01/09/97 moved and made spf_luopts.lu_prottype to be u_int16  taka   *
 * 253 04/01/98 Removed net_status_blk and _os_gs_netstatus          taka   *
 *              <---------------- SPF v2.2.1 -------------------->   taka   *
 * 255 08/19/98 <---------------- SPF v2.2.2 -------------------->   taka   *
 * 257 01/19/99 Added stk_int_flags to spf_update_pb                 rds    *
 * 258 01/29/99 Change IO_XXX_ASYNC to individual user values        mgh    *
 * 259 03/17/99 Added fm_rxrunning, moved timer macros to FM/defs.h  rds    *
 * 263 06/28/99 Added lu_flags to spf_lustat.                        rds    *
 *     07/01/99 Reserved all get/setstats through SPF_PR_RSVD+255    rds    *
 *     10/15/99 Added SPF_PR_IEEE1394 definition                     sr     *
 *     10/26/99 Added Base Error Code for IEEE 1394.                 sr     *
 * 266 01/08/00 Moved error codes into /mwos/SRC/DEFS/errno.h        rds    *
 * 267 11/03/00 Added protocol ID's for UAC Technology's PPP stack   rds    *
 * 268 10/24/00 Added SPF_SS_REATTIF                                 rds    *
 ****************************************************************************/

#if !defined(_SPF_H)
#define _SPF_H

#include <sg_codes.h>

#ifdef _OS9000
#if !defined(SS_SPF)
#define SS_SPF SS_PMOD
#endif
#endif

/*
** Header Files
*/

#include <types.h>

/*
** Macro Definitions
*/

#define SPF_VERSION		255				/* current SPF version number */
#define SPF_FMNAME		"spf"			/* file manager name */
#define SPF_RXNAME		"spf_rx"		/* receive-thread name */

#define SPF_DEVNAMELEN	127				/* max device name length */
#define SPF_INITPARMLEN	127				/* max device init parameter length */

/* SPF I/O system generic base addresses */
#define SPF_PR_RSVD		0x0000			/* 0x0000-0x07FF reserved for SPF */
#define SPF_PR_ITEM		0x0800			/* 0x0800-0xFFFF reserved for ITEM */

/* Microware reserves protocol types (0x0000 - 0x08ff) */

/* SPF getstat/setstat subcodes */
#define SPF_SS_OPEN		(SPF_PR_RSVD+1)		/* open path			*/
#define SPF_SS_CLOSE	(SPF_PR_RSVD+2)		/* close path			*/
#define SPF_SS_UPDATE	(SPF_PR_RSVD+3)		/* update info			*/
#define SPF_GS_UPDATE	(SPF_PR_RSVD+3)		/* Same value, but it's getstat	*/
#define SPF_GS_DEVENTRY	(SPF_PR_RSVD+4)		/* return device entry		*/
#define SPF_SS_FLOWON	(SPF_PR_RSVD+5)		/* flow control on		*/
#define SPF_SS_FLOWOFF	(SPF_PR_RSVD+6)		/* flow control off		*/
#define SPF_SS_PUSH		(SPF_PR_RSVD+7)		/* Push a protocol on the top	*/
#define SPF_SS_POP		(SPF_PR_RSVD+8)		/* Pop a protocol from the top	*/
#define SPF_SS_CLRSIGS	(SPF_PR_RSVD+9)		/* clear all path sigs		*/
#define SPF_SS_MOVE		(SPF_PR_RSVD+10)	/* move data in system state	*/
#define SPF_SS_PULL		(SPF_PR_RSVD+11)	/* Pull on protocol from bottom	*/
#define SPF_SS_DROP		(SPF_PR_RSVD+12)	/* drop protocol off bottom	*/
#define SPF_SS_HWEN		(SPF_PR_RSVD+13)	/* Enable hardware driver	*/
#define SPF_SS_HWDIS	(SPF_PR_RSVD+14)	/* Disable hardware driver	*/
#define SPF_SS_BOOTP	(SPF_PR_RSVD+15)	/* Put driver in BOOTP Mode	*/
#define SPF_SS_APPCLOSE	(SPF_PR_RSVD+16)	/* Application hold-on-close	*/
#define SPF_SS_BRIDGEON	(SPF_PR_RSVD+17)	/* Bridge mode operation on	*/
#define SPF_SS_BRIDGEOFF (SPF_PR_RSVD+18)	/* Bridge mode operation off	*/
#define SPF_SS_RXOFFSET	(SPF_PR_RSVD+19)	/* Set RX offset to HW driver	*/
#define SPF_SS_SETADDR	(SPF_PR_RSVD+20)	/* Set a protocol address		*/
#define SPF_SS_DELADDR	(SPF_PR_RSVD+21)	/* Delete a protocol address	*/
#define SPF_SS_ATTIF	(SPF_PR_RSVD+22)	/* Attach an interface			*/
#define SPF_SS_DETIF	(SPF_PR_RSVD+23)	/* Detach an interface			*/
#define SPF_SS_ROUTE	(SPF_PR_RSVD+24)	/* Add or delete a route		*/
#define SPF_GS_ROUTE	(SPF_PR_RSVD+24)	/* Find route					*/
#define SPF_SS_ADDARP 	(SPF_PR_RSVD+25)	/* Add ARP entry		*/
#define SPF_SS_DELARP 	(SPF_PR_RSVD+26)	/* Delet ARP entry		*/
#define SPF_GS_PROTID	(SPF_PR_RSVD+100)	/* Get protocol ID from driver	*/
#define SPF_GS_ETHADDR	(SPF_PR_RSVD+101)	/* Get ethernet addr from drvr	*/
#define SPF_GS_PATHDESC	(SPF_PR_RSVD+102)	/* Get path descriptor from drvr*/
#define SPF_GS_PATHSTK	(SPF_PR_RSVD+103)	/* Get ASCII path stack list	*/
#define SPF_GS_HWSTAT	(SPF_PR_RSVD+104)	/* Get HW driver statistics	*/
#define SPF_GS_SRCADDR	(SPF_PR_RSVD+105)	/* Get source address			*/
#define SPF_GS_GIFCONF	(SPF_PR_RSVD+106)	/* Get interface table config	*/
#define SPF_GS_RTABLE	(SPF_PR_RSVD+107)	/* Get routing table			*/
/*
** SPF_PR_RSVD+108 through SPF_PR_RSVD+122 are reserved - These codes are
** already defined by the socket API and cannot conflict with any other
** API supported by SPF. (See sg_codes.h for the definitions)
*/
#define SPF_GS_ARPENT 	(SPF_PR_RSVD+123)	/* Get ARP entry		*/
#define SPF_GS_ARPTBL 	(SPF_PR_RSVD+124)	/* Get ARP table		*/
#define SPF_GS_SYMBOLS 	(SPF_PR_RSVD+125)	/* Get "Kernel" Symbols (nlist) */
/*
** To avoid conflicts with system defined setstats all future setstats
** and getstats will be allocated starting at SPF_PR_RSVD+256
*/
/*#define SPF_SS_LASTRES (SPF_PR_RSVD+255)	Last reserved subcode */
#define SPF_SS_REATTIF	(SPF_PR_RSVD+256)	/* Reattach an interface		*/



/* Protocol types: Note that when creating protocol specific setstats and
 * getstats, the protocol type + setstat/getstat number should be
 * used to generate the numbering for these codes
 *
 * NOTE AGAIN: These codes are used to identify a particular protocol
 * and should be used as is for that purpose. However, getstats and setstats
 * particular to a given protocol should use that protocol number << 16
 * as the base for all their setstat codes.
 *
 * For example, X.25 is identified by 0x2. All the setstat and getstat
 * codes specific to X.25 start at a base address of (0x2 << 16) or
 * 0x00020000.
 */

/* Protocol Identifier numbers. We don't want to collide with the DoD IP
 * layer protocol types, so for the ones that don't have definitions there,
 * we'll use a value higher than 256 because the protocol field is only a
 * 1 byte value.
 */

/* DoD IP consistant protocol identifiers (0-255) */
#define SPF_PR_IP		255		/* IP protocol drvr (= IPPROTO_RAW)	*/
#define SPF_PR_ICMP		1		/* ICMP Protocol Driver 		*/
#define SPF_PR_TCP		6		/* TCP protocol driver			*/
#define SPF_PR_UDP		17		/* UDP protocol driver			*/

/* Microware reference or test protocols of some type (0x100-0x13F) */
#define SPF_PR_PATH		0x0100		/* ID Of driver in path descriptor	*/
#define SPF_PR_SPPROTO	0x0101		/* Template (transparent) protocol 	*/
#define SPF_PR_TEST		0x0102		/* Test protocol driver				*/
#define SPF_PR_LOOP		0x0103		/* LoopBack device driver			*/
#define SPF_PR_SCF		0x0104		/* driver to/from SCF devices		*/
#define SPF_PR_RC4		0x0105		/* RC4 security protocol			*/
#define SPF_PR_SOCKET	0x0106		/* socket							*/

/* Connection-oriented protocols (0x140-0x1FF) */
#define SPF_PR_LAPB		0x0140		/* LAP-B protocol driver			*/
#define SPF_PR_X25		0x0141		/* X.25 protocol driver				*/
#define SPF_PR_IRLAP	0x0142		/* IrDA LAP layer					*/
#define SPF_PR_IRLMP	0x0143		/* IrDA LMP layer					*/
#define SPF_PR_IRTINYTP	0x0144		/* IrDA TinyTP layer				*/
#define SPF_PR_IRCOMM	0x0145		/* IrDA Comm-Port layer				*/
#define SPF_PR_IROBEX	0x0146		/* IrDA Object Exchange layer		*/
#define SPF_PR_IRDA		0x0147		/* IrDA driver (Generic)			*/

/* B-ISDN Protocols (0x200-0x2FF) */
#define SPF_PR_Q2110	0x0211		/* Q.2110 Conn Oriented Protocol	*/
#define SPF_PR_Q2130	0x0213		/* Q.2130 Svc Spcfic Conn Func		*/
#define SPF_PR_Q2931	0x0231		/* Q.2931 protocol driver			*/

/* ISDN Protocols (0x300-0x3FF) */
#define SPF_PR_Q921		0x0321		/* Q.921 protocol driver			*/
#define SPF_PR_Q931		0x0331		/* Q.931 protocol driver			*/

/* Hardware protocol type definitions (0x400-0x4FF) */
#define SPF_PR_HDLC		0x0400		/* Generic HDLC device driver ID	*/
#define SPF_PR_Z8530	0x0400		/* Z-8530 device driver				*/
#define SPF_PR_ATMHW	0x0401		/* Generic ATM hardware driver ID	*/
#define SPF_PR_N98401	0x0401		/* NEC ATM SAR device driver		*/
#define SPF_PR_ETHER	0x0402		/* Generic ethernet driver ID		*/
#define SPF_PR_MACE		0x0402		/* MACE PCMCIA card driver			*/
#define SPF_PR_QAM		0x0403		/* QAM receiver driver				*/
#define SPF_PR_TUNER	0x0403		/* Generic tuner receiver driver	*/
#define SPF_PR_IRPHYS	0x0404		/* IrDA physical layer				*/
#define SPF_PR_CA		0x0405		/* Conditional Access driver 		*/
#define SPF_PR_BRI		0x0406		/* Basic Rate ISDN Interface		*/
#define SPF_PR_PRI		0x0407		/* Primary Rate ISDN Interface		*/
#define SPF_PR_UART		0x0408		/* UART SPF driver					*/
#define SPF_PR_USB		0x0450		/*Universal Serial Bus (USB) protocol driver*/
#define SPF_PR_IEEE1394  0x0451 /* IEEE 1394 Protocol */

/* DAVID (and DSM-cc) protocols (0x500-0x5FF) */
#define SPF_PR_DSMHDR	0x0500		/* DSM-cc message header			*/
#define SPF_PR_UNCFG	0x0501		/* Network configuration message	*/
#define SPF_PR_DSMCC_UN	0x0502		/* DSM-cc User to Network messaging	*/
#define SPF_PR_UUCFG	0x0503		/* User-User config message			*/
#define SPF_PR_DSMCC_UU	0x0504		/* DSM-cc User-User messaging		*/
						/* 0x05-0x7f = ISO/IEC 13818-6 Reserved 		*/
/* DVB defined protocols */
#define SPF_PR_DVBRES	0x0510
#define SPF_PR_DVBSESS	0x0511
#define SPF_PR_DVBTRANS	0x0512
#define SPF_PR_DVBHW	0x0513
						/* 0x80-0xff = User defined message type		*/
#define SPF_PR_IP_ON_MPEG	0x0588	/* IP over MPEG driver				*/
#define SPF_PR_RT_DRVR		0x0589	/* Real Time driver					*/
#define SPF_PR_CHMGR		0x0590	/* Channel Mgmt protocol driver		*/
#define SPF_PR_SVCMGMT		0x0590	/* NYNEX Service Mgmt 				*/
#define SPF_PR_EVENTMGMT	0x0591	/* NYNEX Event Mgmt 				*/
#define SPF_PR_L1BDCAST		0x0592	/* NYNEX L1 broadcast				*/
#define SPF_PR_SVCASSURE	0x0593	/* NYNEX Service assurance (PING)	*/
#define SPF_PR_APPDNLD		0x059F	/* Application download driver		*/
#define SPF_PR_SPSDMX		0x05A0	/* Software MPEG-2 Transport Stream Demultiplexer */

/* Connectionless protocol type definitions (0x600-0x64F) */
#define SPF_PR_ENET				0x0600	/* SPENET protocol type				*/
#define SPF_PR_LLCSNAP			0x0601	/* LLC/SNAP (IP over ATM			*/
#define SPF_PR_PPP_IPCP			0x0602	/* IP control protocol for PPP		*/
#define SPF_PR_PPP_LCP			0x0603	/* Link control protocol for PPP	*/
#define SPF_PR_SLIP				0x0604	/* Seral Line IP protocol			*/
#define	SPF_PR_IP_UPPP			0x0606	/* UAC Technology: PPP				*/
#define	SPF_PR_UPPP_HDLC_ASYNC	0x0607	/* UAC Technology: PPP				*/
#define	SPF_PR_UPPP_PPTP		0x0608	/* UAC Technology: PPP				*/
#define	SPF_PR_UPPP_ETHER		0x0609	/* UAC Technology: PPP				*/

/* OSI protocol type definitions */
#define SPF_PR_OSI		0x0650		/* OSI Protocol stack			*/

/* SNMP definitions */
#define	SPF_PR_SNMP_MIBII	0x0700		/* SNMP macro space			*/

/* Client side protocols of network equipment manufacturers */
#define SPF_PR_BBT			0x0800		/* Broadband Technologies ATM UNI client */

/* user-defined protocol types (0x0900 - 0xffff) */
#include <SPF/prot_ids.h>			/* Defines the user defined IDs */

#include <errno.h>

/*
** Type Definitions
*/
typedef struct spf_luopts spf_luopts;
typedef struct spf_luopts* Spf_luopts;
typedef struct spf_popts spf_popts;
typedef struct spf_popts* Spf_popts;
typedef struct spf_ss_pb spf_ss_pb;
typedef struct spf_ss_pb* Spf_ss_pb;
typedef struct spf_update_pb spf_update_pb;
typedef struct spf_update_pb* Spf_update_pb;
typedef struct spf_move_pb spf_move_pb;
typedef struct spf_move_pb* Spf_move_pb;

/* generic getstat/setstat parameter block */
struct spf_ss_pb {
	u_int32		code;			/* setstat module code				*/
	u_int32		size;			/* size of mod_param				*/
	void*		param;			/* module parameter block			*/
	u_int8		updir;			/* gs/ss going up the stak flag 	*/
		#define SPB_GOINGUP	1	/* Param blk is going up stack		*/
		#define SPB_GOINGDWN 0	/* Param blk going down stack		*/
	u_int8		rsv[3];			/* RESERVED FOR FUTURE USE!			*/
};

/* Parameter block for doing the protocol stack updates */
struct spf_update_pb {
	spf_ss_pb	spb;				/* SPF parameter block					*/
	u_int8		stk_ioenabled;		/* I/O is enabled for the stack			*/
	u_int16		stk_txsize;			/* Smallest MTU of the protocol stack	*/
	u_int16		stk_txoffset;		/* TX mbuf Offset SPF will leave		*/
	u_int8		stk_hold_on_close,	/* # protocols on stk that msg on close */
				stk_reliable;		/* Reliable indicator flag				*/
		#define STK_UNRELIABLE	0
		#define STK_RELIABLE	1
	u_int16		stk_txtrailer;		/* Trailer bytes for stack				*/
	u_int16		stk_rxoffset,		/* RX mbuf offset HW drvr will leave	*/
				stk_rsv1;			/* Long word alignment					*/
	u_int32		stk_int_flags;		/* Interface flags (see BSD/net/if.h)	*/
	u_int32		rsv2[2];			/* reserved fields						*/
};

struct spf_move_pb {
	spf_ss_pb	spb;
	void 		*src,
			*dst;
	u_int32		count;
};

/* Logical Unit Options */
struct spf_luopts {
	#if defined(_OSK)
		u_int8		lu_devtype;	/* device type (DT_SPF) 		*/
		u_int8		lu_buf1;
	#elif defined(_OS9000)
		u_int16		lu_devtype;	/* device type (DT_SPF) 		*/
	#endif
	u_int16			lu_devclass;	/* device class (DC_SEQ) 		*/
	u_int8			lu_version;	/* current SPF revision 		*/
	u_int8			lu_rsv2;	/* used to be lu_prottype 		*/
	u_int16			lu_optsize;	/* size of logical unit options 	*/
	u_int16			lu_prottype;	/* protocol type 			*/
	u_int8			lu_rsv1[6];	/* RESERVED FOR FUTURE USE! 		*/
};

/* Path Descriptor Options */
struct spf_popts {
	#if defined(_OSK)
		u_int8		pd_devtype;			/* device type (DT_SPF) */
		u_int8		pd_buf1;
	#elif defined(_OS9000)
		u_int16		pd_devtype;			/* device type (DT_SPF) */
	#endif
	u_int16			pd_devclass;		/* device class (DC_SEQ) */
	u_int8			pd_version;			/* current SPF revision */
	u_int8			pd_ioenabled;		/* I/O enabled flag */
	u_int8			pd_ioasync;			/* asynchronous I/O flag */
		#define IO_SYNC			0	
		#define	IO_ASYNC		1	
		#define IO_WRITE_ASYNC	2		/* Set Write ONLY async operation */
		#define IO_READ_ASYNC	4		/* Set Read  ONLY async operation */
	u_int8			pd_iopacket;		/* packet-oriented I/O flag */
		#define IO_CHAR			0		/* Character oriented I/O */
		#define IO_PACKET		1		/* Pkt oriented */
		#define IO_DGRAM_TOSS	2		/* Pkt oriented, toss extra */
		#define IO_NEXTPKT_ONLY	4		/* Return w/next packet only */
		#define IO_PACKET_TRUNC 6		/* toss extra in a current packet */
	u_int32			pd_optsize;			/* path options size */
	u_int32			pd_iotime;			/* blocking I/O time */
	u_int32			pd_readsz;			/* read flow control size */
	u_int32			pd_writesz;			/* Write flow control size */
	u_int16			pd_rsvd; 			/* RESERVED FOR FUTURE USE */
	u_int16			pd_txsize;			/* Smallest MTU in the stack */
	u_int16			pd_txoffset;		/* Total # bytes for stack header */
	u_int16			pd_txtrailer;		/* Transmit number of trailer bytes	*/
	u_int16			pd_txmsgtype;		/* Transit msg type (oob drivers) */
		#define TXMSG_CONF	0x8000		/* When set, confirmed TX operation */
	u_int8			pd_reliable;		/* Path provides reliable transport	*/
	u_int8			pd_rsv2[93];		/* RESERVED FOR FUTURE USE! */
};

#endif /* _SPF_H */


/*
** Driver/File Manager Definitions
*/
#if !defined(_SPF_H_DRVR) && (defined(SPF_DRVR)||defined(SPF_FM))
#define _SPF_H_DRVR

/*
** Header Files
*/

#include <srvcb.h>
#include <io.h>

#include <SPF/timer.h>

#include <SPF/ppstat.h>

#include <SPF/mbuf.h>

/* Include ITEM definitions for support of the ITEM interface. Since
 * item_pvt.h includes spf.h, this time with SPF_FM defined when making
 * the file manager, we'll check and undefine SPF_FM temporarily.
 */
#ifdef SPF_FM
#undef SPF_FM
#include <SPF/item_pvt.h>
#define SPF_FM
#else
#include <SPF/item_pvt.h>
#endif

/*
** Device Descriptor Macro Definitions
*/

/* DRV_NAME must !ALWAYS! be defined when making descriptors */

#ifndef MODE
#define MODE (S_ISIZE|S_IREAD|S_IWRITE)
#endif

#ifndef LUFLAGS
#define LUFLAGS (0)
#endif

#ifndef PORTADDR
#define PORTADDR (0)
#endif

#ifndef DESCNUM
#define DESCNUM (0)
#endif

#ifndef VECTOR
#define VECTOR (0)
#endif

#ifndef IRQLEVEL
#define IRQLEVEL (0)
#endif

#ifndef PRIORITY
#define PRIORITY (0)
#endif

#ifndef BLOCKTIME
#define BLOCKTIME (0)
#endif

#ifndef ASYNCFLAG
#define ASYNCFLAG IO_SYNC
#endif

#ifndef PKTFLAG
#define PKTFLAG IO_CHAR
#endif

#ifndef READSZ
#define READSZ (0)
#endif

#ifndef WRITESZ
#define WRITESZ (0)
#endif

#ifndef TXSIZE
#define TXSIZE (0xFFFF)
#endif

#ifndef TXOFFSET
#define TXOFFSET (0)
#endif

#ifndef TXTRAILER
#define TXTRAILER (0)
#endif

#ifndef PATH_HOLDONCLOSE
#define PATH_HOLDONCLOSE PATH_NOHOLD
#endif

#ifndef CTL_FLAGS
#define CTL_FLAGS (0)
#endif

#ifndef PROTTYPE
#define PROTTYPE (0)
#endif

#ifndef ITE_NETYPE_IN
#define ITE_NETYPE_IN ITE_NET_ANY
#endif

#ifndef ITE_NETYPE_OUT
#define ITE_NETYPE_OUT ITE_NET_ANY
#endif

#ifndef ITE_CALL_STATE
#define ITE_CALL_STATE ITE_CS_CONNLESS
#endif

#ifndef OURADDR_CLASS
#define OURADDR_CLASS ITE_ADCL_NONE
#endif

#ifndef OURADDR_SUBCLASS
#define OURADDR_SUBCLASS ITE_ADSUB_NONE
#endif

#ifndef OURADDR_SIZE
#define OURADDR_SIZE 0
#endif

#ifndef OURADDR
#define OURADDR ""
#endif

#ifndef THEIRADDR_CLASS
#define THEIRADDR_CLASS ITE_ADCL_NONE
#endif

#ifndef THEIRADDR_SUBCLASS
#define THEIRADDR_SUBCLASS ITE_ADSUB_NONE
#endif

#ifndef THEIRADDR_SIZE
#define THEIRADDR_SIZE 0
#endif

#ifndef THEIRADDR
#define THEIRADDR ""
#endif

/*
** Type Definitions
*/

typedef struct spf_ss_open_pb spf_ss_open_pb;	/* SPF open parameter block	*/
typedef struct spf_ss_open_pb* Spf_ss_open_pb;
typedef struct spf_drstat spf_drstat;			/* SPF Driver static		*/
typedef struct spf_drstat* Spf_drstat;
typedef struct spf_lustat spf_lustat;			/* SPF Logical unit static	*/
typedef struct spf_lustat* Spf_lustat;
typedef struct spf_topdvr spf_topdvr;			/* Top driver struct in pd	*/
typedef struct spf_topdvr* Spf_topdvr;
typedef struct spf_pdstat spf_pdstat;			/* SPF path descriptor		*/
typedef struct spf_pdstat* Spf_pdstat;
typedef struct spf_desc spf_desc;				/* SPF Device descriptor	*/
typedef struct spf_desc* Spf_desc;

/* SS_OPEN parameter block */
struct spf_ss_open_pb {
	u_int32			code;				/* setstat module code */
	char*			init;				/* open initialization string */
	void*			param;				/* upper driver device entry */
	u_int8			rsv[4];				/* RESERVED FOR FUTURE USE! */
};

/* Device Driver Static Storage */
struct spf_drstat {

	u_int32			dr_version;			/* SPF version number */

	/* file-manager callup entry point */
	error_code		(*dr_fmcallup)(u_int32 code, void* param1, void* param2);

		/* packet for receive-thread to process */
		#define FMCALLUP_PKT			0x04
		#define FMCALLUP_CLOSE			0x05
		#define FMCALLUP_NTFY			0x06
		#define FMCALLUP_UPDATE			0x07
		#define FMCALLUP_TIMER_START	0x08
		#define FMCALLUP_TIMER_RESTART	0x09
		#define FMCALLUP_TIMER_STOP		0x0A
		#define FMCALLUP_TIMER_ENVIRON	0x0B

		#define DR_FMCALLUP_PKT(my_deventry,dst_deventry,mb) \
			(((Spf_drstat)(my_deventry->v_dr_stat))->dr_fmcallup)(FMCALLUP_PKT,dst_deventry,mb)
		#define DR_FMCALLUP_CLOSE(my_deventry,pathdesc) \
			(((Spf_drstat)(my_deventry->v_dr_stat))->dr_fmcallup)(FMCALLUP_CLOSE,pathdesc,NULL)
		#define DR_FMCALLUP_NTFY(my_deventry, npb, param2) \
			(((Spf_drstat)(my_deventry->v_dr_stat))->dr_fmcallup)(FMCALLUP_NTFY,npb,param2)
		#define DR_FMCALLUP_UPDATE(my_deventry, pathdesc) \
			(((Spf_drstat)(my_deventry->v_dr_stat))->dr_fmcallup)(FMCALLUP_UPDATE,pathdesc,NULL)
		#define DR_FMCALLUP_TMRSTART(my_deventry, tpb) \
			(((Spf_drstat)(my_deventry->v_dr_stat))->dr_fmcallup)(FMCALLUP_TIMER_START,my_deventry,tpb)
		#define DR_FMCALLUP_TMRRESTART(my_deventry, tpb) \
			(((Spf_drstat)(my_deventry->v_dr_stat))->dr_fmcallup)(FMCALLUP_TIMER_RESTART,my_deventry,tpb)
		#define DR_FMCALLUP_TMRSTOP(my_deventry, tpb) \
			(((Spf_drstat)(my_deventry->v_dr_stat))->dr_fmcallup)(FMCALLUP_TIMER_STOP,my_deventry,tpb)
		#define DR_FMCALLUP_TMRENVIRON(my_deventry, t_env) \
			(((Spf_drstat)(my_deventry->v_dr_stat))->dr_fmcallup)(FMCALLUP_TIMER_ENVIRON,my_deventry,t_env)
	/* driver initialization entry point */
	error_code		(*dr_iniz)(Dev_list deventry);

	/* driver termination entry point */
	error_code		(*dr_term)(Dev_list deventry);

	/* driver get-stat entry point */
	error_code		(*dr_getstat)(Dev_list deventry, Spf_ss_pb gs_pb);

	/* driver set-stat entry point */
	error_code		(*dr_setstat)(Dev_list deventry, Spf_ss_pb ss_pb);

	/* driver down-data entry point */
	error_code		(*dr_downdata)(Dev_list deventry, Mbuf mb);

	/* driver up-data entry point */
	error_code		(*dr_updata)(Dev_list deventry, Mbuf mb);

	u_int32			dr_att_cnt;		/* # of attaches to this drvr		*/
	Spf_lustat		dr_lulist;		/* LU list (only when dr_lumode=1	*/
	u_int8			dr_lumode;		/* Logical unit mode flag			*/
		#define	DR_ALLOC_LU_PERPORT	0	/* Alloc LU per normal OS rules	*/
		#define DR_ALLOC_LU_PERPATH	1	/* Alloc LU per open path		*/
	u_int8			dr_rsv1[11];	/* Long word alignment				*/
	u_int32			dr_use_cnt;		/* # open paths to driver			*/

	#ifdef SPF_DRSTAT
		SPF_DRSTAT						/* device-specific drvr static */
	#endif
};

/* Logical Unit Static Storage */
struct spf_lustat {

	/* general logical unit fields */
	u_int32			lu_att_cnt;			/* # of attaches to this lu */
	u_int32			lu_use_cnt;			/* path use count */
	void*			lu_port;			/* device port address */
	u_int16			lu_num;				/* logical unit number */

	/* SPF specific fields */
	u_int8			lu_ioenabled;		/* I/O enabled flag */
		#define DRVR_IOEN	1
		#define DRVR_IODIS	0
	u_int8			lu_reliable;		/* Reliable transport flag */
	u_int16			lu_rsvd;			/* reserved for future use */
	u_int16			lu_txsize;			/* max transmit packet size */
	u_int16			lu_txoffset;		/* transmit packet offset */
	u_int16			lu_txtrailer;		/* transmit packet trailer */
	void*			lu_attachptr;		/* attach() device entry ptr */
	Dev_list		lu_updrvr;			/* upper driver device entry */
	Dev_list		lu_dndrvr;			/* lower driver device entry */
	Spf_pdstat		lu_pathdesc;		/* current path descriptor */
		#define LU_CURRPATH		lu_pathdesc->pd_common.pd_id
		#define LU_CURRPROC		lu_pathdesc->pd_common.pd_cproc
		#define LU_CURRIOTIME	lu_pathdesc->pd_opts.pd_iotime
		#define LU_CURRIOASYN	lu_pathdesc->pd_opts.pd_ioasync
		#define LU_ITEMPATH		lu_pathdesc->pd_item
		#define LU_ITEMDEV		lu_pathdesc->pd_item.conn_info
		#define LU_OURNUM		lu_pathdesc->pd_item.conn_info.dev_ournum
		#define LU_THEIRNUM		lu_pathdesc->pd_item.conn_info.dev_theirnum
	#define LU_NPB_LNKDWN	lu_pathdesc->pd_item.notify_list[ITE_ON_LINKDOWN]
	#define LU_NPB_INCALL	lu_pathdesc->pd_item.notify_list[ITE_ON_INCALL]
	#define LU_NPB_CONN		lu_pathdesc->pd_item.notify_list[ITE_ON_CONN]
	#define LU_NPB_DATAVAIL	lu_pathdesc->pd_item.notify_list[ITE_ON_DATAVAIL]
	#define LU_NPB_ENDPGM	lu_pathdesc->pd_item.notify_list[ITE_ON_ENDPGM]
	#define LU_NPB_FEHANGUP	lu_pathdesc->pd_item.notify_list[ITE_ON_FEHANGUP]
	#define LU_NPB_DNLDONE	lu_pathdesc->pd_item.notify_list[ITE_ON_DNLDONE]
	#define LU_NPB_MSGCONF	lu_pathdesc->pd_item.notify_list[ITE_ON_MSGCONF]
	#define LU_NPB_RESADD	lu_pathdesc->pd_item.notify_list[ITE_ON_RESADD]

	Spf_lustat		lu_next;			/* To make logical unit lists */

	u_int8			lu_hold_on_close;	/* Hold on close indicator */
		#define PATH_NOHOLD	0			/* No holding the path on close	*/
		#define PATH_HOLD	1			/* Hold path on close-interactive msging */
	u_int8			lu_flags;			/* Flags */
		#define LU_UIOWRITE	0x01		/* FM doesn't copy write data */
		#define LU_UIOREAD	0x02		/* FM doesn't copy read data */
	u_int16			lu_pps_size;		/* Per-path storage size */
	Spf_ppstat		lu_pps_list;		/* Per-path storage list */
	void			*lu_pps_idata;		/* Initialized per-path data */

	/* Logical Unit Options Fields */
	spf_luopts		lu_luopts;			/* logical unit options section */

	#ifdef SPF_LUSTAT
		SPF_LUSTAT						/* device-specific lu static */
	#endif
};

/* Top-Level Protocol "Driver" */
struct spf_topdvr {
	Mbuf			rdq;				/* incoming data queue */
	u_int32			rsv2[4];			/* RESERVED FOR FUTURE USE! */
	u_int32			readsz;				/* read queue byte count */
	u_int8			rsv1[8];			/* RESERVED FOR FUTURE USE! */
	dev_list		deventry;			/* "device entry" */
	spf_drstat		drstat;				/* "driver static" */
	spf_lustat		lustat;				/* "logical unit static" */
};

/* Protocol driver per-path logical unit storage structure */
typedef struct path_stack {
	Dev_list	pdstk_deventry;	/* The device entry for the protocol driver	*/
	void		*pdstk_ppstg;	/* Pointer to driver allocated per path stg	*/
	void 		*pdstk_rsv;		/* Reserved for future use					*/
} path_stack, *Path_stack;

/* Maximum of 6 incrementing protocol drivers per stack */
#define STACK_SIZE 6

/* Path Descriptor Static Storage */
struct spf_pdstat {

	/* common path desc section */
	struct pathcom	pd_common;

	/* path descriptor options */
	spf_popts		pd_opts;

	/* ITEM support structure */
	path_type		pd_item;

	/* spf specific fields */
	spf_topdvr		pd_drvr;			/* top protocol/device driver */

	char			pd_protlist[60];	/* This path's protocol stack list	*/

	alarm_id		pd_hoc_aid;		/* Hold on close alarm ID				*/

	u_char			pd_flag;		/* Sendto type							*/
	u_char			pd_rsv2[2];		/* Long word alignment					*/
	u_char			pd_numdrvrs;	/* # of drivers stacked on this path	*/

	/* This array stores pointers to pertinant information the protocol
	 * needs on a per path basis. SPF also uses this information on close
	 * so it correctly calls the close routines of the drivers stacked
	 * on this path.
	 */
	path_stack		pd_stack[STACK_SIZE];
};

/* Device Descriptor */
struct spf_desc {

	/* common device descriptor section */
	dd_com			dd_desccom;

	/* path options */
	spf_popts		dd_popts;

	/* ITEM device type information */
	device_type		dd_item;

	/* SPF-specific section */
	u_int32			dd_pmstak;			/* offset to protocol string */
	u_int8			dd_rsv1[16];		/* RESERVED FOR FUTURE USE! */
};


/*
**	Function Prototypes
*/

/* driver call-up functions */
#define SMCALL(mydeventry,deventry,entrypoint,param)						\
	(																		\
		/* update destination driver fields */								\
			(((Spf_lustat)((deventry)->v_lu_stat))->lu_pathdesc = 		\
				((Spf_lustat)((mydeventry)->v_lu_stat))->lu_pathdesc),	\
		/* call destination driver entry point */							\
			(*entrypoint)(deventry,param)									\
	)
#define SMCALL_DNDATA(mydeventry,deventry,mb)								\
	(SMCALL(mydeventry,deventry,											\
		((Spf_drstat)(deventry->v_dr_stat))->dr_downdata,mb))
#define SMCALL_UPDATA(mydeventry,deventry,mb)								\
	(SMCALL(mydeventry,deventry,											\
		((Spf_drstat)(deventry->v_dr_stat))->dr_updata,mb))
#define SMCALL_GS(mydeventry,deventry,pb)									\
	(SMCALL(mydeventry,deventry,											\
		((Spf_drstat)(deventry->v_dr_stat))->dr_getstat,pb))
#define SMCALL_SS(mydeventry,deventry,pb)									\
	(SMCALL(mydeventry,deventry,											\
		((Spf_drstat)(deventry->v_dr_stat))->dr_setstat,pb))

/* misc driver help functions */

#define return_mbuf(mb) (m_free_q(&mb))

#ifdef __cplusplus
	extern "C" {
#endif

/* DRVR/misc.c */
extern error_code get_mbuf(Dev_list dev_entry, u_int32 size, Mbuf *mb);

/* DRVR/pull.c */
extern error_code pull(Dev_list, char*,Dev_list*,Dev_list*);
extern error_code drop(Dev_list dev_entry, Dev_list dndrvr);

/* Function prototypes for ppstat.l */
extern error_code pps_add_entry(Dev_list deventry, Spf_ppstat *ret_pps);
extern error_code pps_find_entry(Spf_lustat lustat, Spf_pdstat pd,
													Spf_ppstat *ret_pps);
extern error_code pps_find_entry_by_offset(Spf_lustat lustat,
		u_int16 offset, u_int16 size, void *cmp_val, Spf_ppstat *ret_pps);
extern error_code pps_del_entry(Spf_lustat lustat, Spf_pdstat pd);
extern error_code pps_chg_updrvr(Spf_lustat lustat, Spf_pdstat pd,
													Dev_list new_updrvr);

#ifdef __cplusplus
	}
#endif

#endif /* _SPF_H_DRVR *^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


/*
** File Manager Definitions
*/
#if !defined(_SPF_H_FM) && defined(SPF_FM) /*vvvvvvvvvvvvvvvvvvvvvvv*/
#define _SPF_H_FM

/*
** Type Definitions
*/

typedef const struct spf_fmconst spf_fmconst;
typedef const struct spf_fmconst* Spf_fmconst;
typedef struct spf_fmstat spf_fmstat;
typedef struct spf_fmstat* Spf_fmstat;

/* File Manager Constants */
struct spf_fmconst {
	const char		fm_string_start[16];	/* start string */
	const u_int32	fm_rx_priority;			/* priority of rx-thread */
		#define SPF_RX_PRIORITY 0			/* rx-thread inherits priority */
	const u_int32	fm_rx_stack;			/* rx-thread stack space */
		#define SPF_RX_STACK 0				/* additional rx-thread stack */
	const u_int8	fm_rsv1[8];				/* RESERVED FOR FUTURE USE! */
	const char		fm_string_end[16];		/* end string */
};

/* File Manager Static Storage */
struct spf_fmstat {

	u_int32		fm_entrycnt;			/* # of OS entry points (16) */
			#define FM_ENTRYCNT		16

	/* Address of "attach" function */
	error_code	(*fm_attach)(i_attach_pb *pb, Dev_list dev_entry);

	/* Address of "chgdir" function: (i_chdir_pb *pb, Spf_pdstat pd) */
	error_code	(*fm_chdir)(void);

	/* Address of "close" function */
	error_code	(*fm_close)(i_close_pb *pb, Spf_pdstat pd);

	/* Address of "create" function (i_create_pb *pb, Spf_pdstat pd) */
	error_code	(*fm_create)(i_open_pb *pb, Spf_pdstat pd);

	/* Address of "delete" function (i_delete_pb *pb, Spf_pdstat pd) */
	error_code	(*fm_delete)(void);

	/* Address of "detach" function */
	error_code	(*fm_detach)(i_detach_pb *pb, Dev_list dev_entry);

	/* Address of "dup" function */
	error_code	(*fm_dup)(i_dup_pb *pb, Spf_pdstat pd);

	/* Address of "getstat" function */
	error_code	(*fm_getstat)(i_getstat_pb *pb, Spf_pdstat pd);

	/* Address of "makdir" function (i_makdir_pb *pb, Spf_pdstat pd) */
	error_code	(*fm_makdir)(void);

	/* Address of "open" function */
	error_code	(*fm_open)(i_open_pb *pb, Spf_pdstat pd);

	/* Address of "read" function */
	error_code	(*fm_read)(i_read_pb *pb, Spf_pdstat pd);

	/* Address of "readln" function (i_readln_pb *pb, Spf_pdstat pd) */
	error_code	(*fm_readln)(void);

	/* Address of "seek" function (i_seek_pb *pb, Spf_pdstat pd) */
	error_code	(*fm_seek)(void);

	/* Address of "setstat" function */
	error_code	(*fm_setstat)(i_setstat_pb *pb, Spf_pdstat pd);

	/* Address of "write" function */
	error_code	(*fm_write)(i_write_pb *pb, Spf_pdstat pd);

	/* Address of "writeln" function (i_writeln_pb *pb, Spf_pdstat pd) */
	error_code	(*fm_writeln)(void);

	/* Address of receive thread function */
	error_code	(*fm_rx)(Mbuf mb, Spf_fmstat fmstat);

	u_int32		fm_att_cnt;				/* # of attaches to file manager */
	u_int32		fm_rx_cnt;				/* # of attaches to rx-thread */
	u_int32		fm_rx_priority;			/* Priority of rx-thread */
	u_int32		fm_rx_stack;			/* Additional stack for rx-thread */

	Mbuf		fm_mbhead;				/* Incoming data queue -- head */
	Mbuf		fm_mbtail;				/* Incoming data queue -- tail */

	process_id	fm_rx_proc;				/* Receive thread process ID */

	event_id	fm_evid;				/* Receive thread startup event ID */
	event_id	fm_rx_evid;				/* Receive thread event ID */

	u_int8*		fm_rx_death;			/* Receive thread death flag ptr */

	alarm_id	fm_talarm_id;			/* Timer alarm ID */

	Timer_pb	fm_timerq_head,			/* Head of the timer queue */
				fm_timerq_tail;			/* Tail of the timer queue */

	u_int16		fm_timerid,				/* Timer ID granting service */
				fm_timer_flags;			/* Timer flags */
		#define TIMER_OFF	0x0001		/* Indicates timer has gone off */
		#define TIMER_CLR	0xFFFE		/* Clear timer off flag macro */
	u_int32		fm_active_timers,		/* # active timers in list */
				*fm_currtick_ptr;		/* Ptr to systm glob. current tick */

	u_int16		fm_ticks_sec,			/* Stored number of ticks/second */
				fm_min_alarm_int;		/* Minimum alarm interval in ticks */

	/* Address of timer service function */
	error_code	(*fm_timer_svc)(Spf_fmstat fmstat);

	void*		fm_benchmk;				/* bench mark data module pointer */
	u_int32		fm_rxrunning;			/* rx thread is executing */
};

#endif /* _SPF_H_FM *^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
