/****************************************************************************
 *      Copyright 1993 by Microware Systems Corporation                     *
 *      Reproduced Under License                                            *
 *                                                                          *
 *      This source code is the proprietary confidential property of        *
 *      Microware Systems Corporation, and is provided to licensee          *
 *      solely for documentation and educational purposes. Reproduction,    *
 *      publication, or distribution in any form to any party other than    *
 *      the licensee is strictly prohibited.                                *
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date     Comments                                              By  *
 *  --- --------  -----------------------------------------------   ------- *
 *   0 ??/??/??   Birth                                                 ??? *
 *   1  05/20/93  Folded in diffs from ver 1.5 into this here               *
 *                version 2.0 (defines for SUN380)                      gkm *
 *   2  08/05/93  file moved from MWOS/OS9/SRC/IO/INET/DRVR/AM7990          *
 *                to MWOS/OS9/SRC/DEFS.  Changed include for if.h.      gkm *
 *   3  08/06/93  added conditionalized defn for BBRAM_LANID for            *
 *                VME374.                                                   *
 *                conditionalized defn of ifs since VME374 defines          *
 *                it differently in mwos/os9/io/inet/drvr/vme374/me.h   gkm *
 *      09/16/93  <***** ISP 2.0 Release **************************>        *
 *   4  02/27/95  modified for use with SPF - Bill West Inc.                *
 *      02/19/97  <***** SPF LAN Comm Pack v3.0 Release*********>           *
 *      05/05/97  <***** SPF LAN Comm Pack v3.1 Release*********>           *
 *  10  10/30/97  Fixed TMD3 flags: made to check high byte             tmd *
 *     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              *
 ****************************************************************************/

/*
	Definitions for Am7990 LANCE interface
*/

#ifndef VOLATILE
#define VOLATILE volatile
#endif

#if defined(IPAK)
struct leregs {
	VOLATILE u_int16 rdp;		/* register data port */
	VOLATILE u_int16 rap;		/* register address port */
	u_char ipakdum1[0x3d];
	VOLATILE u_char ipakvctr;	/* IRQ vector register */
	u_char ipakdum2;
	VOLATILE u_char ipaksts;	/* status (read) and control (write) register */
};
#define IRQ_ENABLE 1
#define IRQ_DISABLE 0
#define IRQ_FORCE 2
#define MAU_MASK 0x0C
#define COAX_MAU 0
#define SPR_MAU 0x04
#define TP_MAU 0x08
#define NO_MAU 0x0C
#define IPAKMEMSIZE 0x4000		/* 16k local RAM on IPAK */

#elif defined(TP32V)
struct leregs {
	u_int16 rdp;			/* register data port */
	u_int16 tp32dum1;
	u_int16 rap;			/* register address port */
	u_int16 tp32dum2;
};
#else
struct leregs {
	u_int16 rdp;			/* register data port */
	u_int16 rap;			/* register address port */
};
#endif

#define LE_CSR0		0				/* main control/status register */
#define	LE_CSR1		1				/* lsb address of init block */
#define LE_CSR2		2				/* msb address of init block */
#define LE_CSR3		3				/* bus master control register */

#define	LE_ERR		0x8000		/*	error summary bit */
#define	LE_BABL		0x4000		/*  babbling transmitter timeout */
#define	LE_CERR		0x2000		/*  collision error */
#define	LE_MISS		0x1000		/*  missed packet */
#define	LE_MERR		0x0800		/*  memory error */
#define	LE_RINT		0x0400		/*  receiver interrupt */
#define	LE_TINT		0x0200		/*  transmitter interrupt */
#define	LE_IDON		0x0100		/*  initialization done */
#define	LE_INTR		0x0080		/*  interrupt summary bit */
#define	LE_INEA		0x0040		/*  interrupt enable */
#define	LE_RXON		0x0020		/*  receiver on */
#define	LE_TXON		0x0010		/*  transmitter on */
#define	LE_TDMD		0x0008		/*  transmit demand */
#define	LE_TSTOP	0x0004		/*  stop LANCE */
#define	LE_STRT		0x0002		/*  start LANCE */
#define	LE_INIT		0x0001		/*  initialize */

#define LE_BSWP		4			/* byte swap control */
#define LE_ACON		2			/* ALE control */
#define LE_BCON		1			/* byte control */

#define LEM_PROM	0x8000		/* promiscuous mode */
#define LEM_INTL	0x0040		/* internal loopback mode */
#define LEM_DRTY	0x0020		/* disable retry mode */
#define LEM_COLL	0x0010		/* force collision mode */
#define LEM_DTCR	0x0008		/* disable transmit crc mode */
#define LEM_LOOP	0x0004		/* loopback mode */
#define LEM_DTX		0x0002		/* disable transmitter mode */
#define LEM_DRX		0x0001		/* disable receiver mode */

/*
	lance message descriptor
*/
typedef struct le_md {			/* xmit/receive message descriptor */
	u_int32 md_loaddr	:16;	/* lower 16 bits of buffer address */
	u_int32 md_flag		:8;		/* flag byte */
	u_int32 md_hiaddr	:8;		/* higher 8 bits of buffer address */
	u_int32 md_bcnt		:16;	/* buffer byte count */
	u_int32 md_mcnt		:16;	/* recv msg size or xmit status */
#define		md_tmd3		md_mcnt	/* transmit status in xmit md */
} le_md, *Le_md, (*Md_ap)[];

/* common flag bits in xmit/recv descriptors */

#define MD_OWN	0x80	/* owned by LANCE */
#define MD_ERR	0x40	/* error summary */
#define MD_STP	0x02	/* start of packet */
#define MD_ENP	0x01	/* end of packet */

/* receive descriptor flag bits */
#define MD_FRAM	0x20	/* framing error */
#define MD_OFLO	0x10	/* silo overflow */
#define MD_CRC	0x08	/* crc error */
#define MD_RBUF	0x04	/* buffer error */

/* transmit descriptor flag bits */
#define MD_MORE	0x10	/* more than one retry required to transmit */
#define MD_ONE	0x08	/* one retry required to transmit */
#define MD_DEF	0x04	/* deferred transmit */

/* transmit descriptor status bits */
#define MD_XBUF	0x8000	/* xmit buffer error */
#define MD_UFLO	0x4000	/* xmit underflow error */
#define MD_LCOL	0x1000	/* xmit late collision error */
#define MD_LCAR 0x0800	/* loss of carrier */
#define MD_RTRY 0x0400	/* retry count exceeded */

/*
	lance descriptor ring pointer
*/

struct le_drp {
	u_int32 laddr :16;	/* lower 16 bits of descriptor ring address */
	u_int32 rlen  :3;	/* receive ring length */
	u_int32 :5;			/* reserved */
	u_int32 haddr :8;	/* high 8 bits of dr address */
};

struct le_init_block {
	u_int16	ib_mode;		/* init block mode byte */
	u_char  padr[6];		/* physical ethernet address */
	u_char  ladr[8];		/* receive logical address filter */
	struct le_drp recv_drp;	/* receive descriptor ring pointer */
	struct le_drp xmit_drp;	/* transmit descriptor ring pointer */
};	

#define INIT_BLK_SIZE 0x20	/* can't use sizeof because LANCE is funky */
/*
	LANCE statistics
*/

struct lance_stat {
	u_int	unk_irq;		/* unknown irqs */
	u_int	recv_irq;		/* receive interrupts */
	u_int	xmit_irq;		/* transmit interrupts */
	u_int	recv_count;		/* number of successful receives */
	u_int	err_frame;		/* framing error */
	u_int	err_oflo;		/* silo overflow */
	u_int	err_crc;		/* crc error */
	u_int	err_rbuf;		/* no buffer available for chaining */
	u_int	err_missed;		/* no buffer available for receive */
	u_int	xmit_count;		/* number of successful transmits */
	u_int	xmit_trys;		/* number of transmit attempts */
	u_int	err_more;		/* multiple retries required to xmit buffer */
	u_int	err_one;		/* one retry required to xmit buffer */
	u_int	err_defer;		/* xmit deferral required */
	u_int	err_tbuf;		/* xmit buffer error */
	u_int	err_uflo;		/* silo underflow error */
	u_int	err_lcol;		/* collision error */
	u_int	err_lcar;		/* lost carrier */
	u_int	err_retry;		/* too many collisions */
	u_int	err_babble;		/* babbling transmitter */
	u_int	err_xenq;		/* xmit requests enqueued */
	u_int	err_bogus;		/* generally bogus packet received */
	u_int	no_tail_irqs;		/* xmit IRQ with tail == NULL */
	u_int	already_seen;		/* xmit already freed md */
	u_int	no_uplink;		/* recv can't send up data */
};

#if !defined(VME374)	
typedef struct ifs {
	struct leregs		*v_leregs;			/* points to lance csr */
	struct mbuf		*v_inq;				/* input mbuf queue */
	struct mbuf		*v_outq;			/* output mbuf queue */
	Md_ap			v_recv_ring, v_xmit_ring;	/* xmit/recv ring address */
	Le_md			v_recv_rend, v_xmit_rend;	/* ring end pointers */
	Le_md			v_recv_head, v_xmit_head;	/* xmit/recv ring head addr */
	Le_md			v_recv_tail, v_xmit_tail;	/* xmit/recv ring tail addr */
	u_short			v_rpmem, v_tpmem;		/* memory sizes for rings */
	u_short			v_running;			/* driver running flag */
	u_char			v_rpcnt, v_tpcnt;		/* size of rings */
	u_char			dummy;				/* pad */
	u_short			dummy2;
	struct le_init_block	v_ib;				/* lance initialization block */
	u_int32			v_hiaddr;			/* bits 31-24 of lance address */
#ifdef LCLMEM
/*
	v_cpu_base is the address the CPU uses to access the LANCE
	ram.  v_cpu_base - v_lan_adj is the address the LANCE uses
	to access that same shared ram.  Examples:
	
	Shared CPU/LANCE ram size =	256kb (0x00040000)	<- v_ram_size
	CPU memory access address =	0xFFC00000		<- v_cpu_base
	LANCE memory access address =	0x000000
					0xFFC00000		<- v_lan_adj

	Shared CPU/LANCE ram size = 	128kb (0x00020000)	<- v_ram_size
	CPU memory access address = 	0xFF800000		<- v_cpu_base
	LANCE memory access address =   0x800000
				 	0xFF000000		<- v_lan_adj
*/
	u_int32			v_cpu_base;			/* base addr of local lance RAM (CPU access) */
	u_int32			v_ram_size;			/* size of local lance RAM */
	u_int32			v_lan_adj;			/* adjustment value to local lance RAM */
#endif
#ifdef IPAK
	u_int32			v_ipaklowaddr;
	u_int32			v_ipakhiaddr;
	u_int32			v_ipaklowio;
	u_int32			v_ipakhiio;
	u_int32			v_buserrhdlr;			/* previous bus error handler */
	u_int32			v_sysglob;			/* address of system globals */
#endif
	struct lance_stat	v_lestat;			/* driver statistics */
} *Ifs;
#endif		/* #if !defined(VME374) */

#if defined(TP22V)
#	define LANRESET	0x14002		/* TP22V lan reset register */
#elif defined(TP32V)
#	define LANRESET	0x00410048		/* TP32V lan reset register */
#elif defined(VME147)
#	define LANICR		0xfffe1028	/* LAN interrupt control register */
#	define LANICR_IE	0x08		/* LAN interrupt enable */
#	define LANICR_IRQ	0x80		/* LAN interrupt pending */
#elif defined(SUN380)
#	define	SUN380_IDPROM		0xfef047d8	/* address of sun idprom */
#	define	SUN380_IOMAPPER 	0xfef66000	/* io mapper address */
#	define	SUN380_IOMAPSIZE	2048		/* number of entries */
#	define	SUN380_PAGE_SIZE	0x2000		/* size of a page */
#	define	SUN380_PAGEDESC		0x01		/* page descriptor flag */
#endif

#define ETHER_MIN	(ETHERMIN + sizeof(struct ether_header))
#define ETHER_MAX	(ETHERMTU + sizeof(struct ether_header))
#define LANCE_ETHER_MAX	(ETHER_MAX + 4)		/* lance needs room for FCS (CRC) */

#ifdef IPAK
#define	IP_IOSIZE	128						/* should put this elsewhere */
#endif

#ifndef LCLMEM_BASE
/*
These are DEFAULTS if not defined in spf_desc.h.
We only need these definitions to keep the SPF initialized data code
happy if driver is not built with LCLMEM defined.  It seems simpler to
always put these values in the descriptor than to try and conditionalize
the macros that define the logical unit static storage.
*/
#define LCLMEM_BASE	0	/* no local mem */
#define LCLMEM_SIZE	0	/* no local mem */
#define LCLMEM_TRANS	0	/* no local mem */
#endif

#ifndef BBRAM_LANID
#define BBRAM_LANID 0
#endif

#ifndef EA0
/*
Set Ethernet Address in descriptor to zero to use hardware addr
(from BBRAM, PROM, or wherever else get_eaddr() can find it).
*/
#define EA0 0
#define EA1 0
#define EA2 0
#define EA3 0
#define EA4 0
#define EA5 0
#endif
