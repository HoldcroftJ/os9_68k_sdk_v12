/****************************************************************************
 *  $Workfile:   ie.h  $
 *  $Revision:   1.0  $
 *  $Modtime:   31 Mar 1997 10:48:00  $
 *  $Date:   31 Mar 1997 13:46:46  $
 ****************************************************************************
 *	Copyright 1993 by Microware Systems Corporation							*
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
 *	  1	08/10/93	changed includes for ie162.h and 1e167.h which have 	*
 *					moved to mwos/os9/src/defs/inet and are one file 		*
 *					called ie162167.h									gkm	*
 *	  2	08/11/93	ie.h from the booter and the driver were merged.		*
 *					-dCBOOT is used to conditionalize the code.				*
 *					The revision checked in underneath this one is the 		*
 *					original booter ie.h and the one underneath that 		*
 *					one is the original driver ie.h.					gkm	*
 *	  3	09/09/93	Added recv_giant, licr_mask, v_isV4d            kk/ylee	*
 *	  4 10/18/93	Added mizar's SYSBUS_EBE				 bat/arb(mizar) *
 *		10/18/93	<***** ISP 2.0 Release **************************>		*
 *	  5 11/24/93	added iestat.recv_unused							gkm
 *	  6 01/31/94	added iestat.rbd_headlost for when v_rbd_head !=	gkm
 *					rfd_rbd && v_rbd_head == 0, and we decide to keep
 *					going (in recv_irq) instead of calling f_sysdbg(0x2222).
 *    7 06/24/94    Updated includes for release 2.1                    bat
 *      03/31/97    added support for mvme172                           gdw
 ****************************************************************************/


#include <INET/if.h> 
#include <INET/arp.h>
#include <INET/ether.h>
#include <signal.h>


/*#if !defined(CBOOT)*/				/* for driver only */
	#undef ETHERMIN
	#define ETHERMIN (64-14)	/* (am7990) bug in ether.h */
/*#endif*/

#define ETHER_MIN	(ETHERMIN + sizeof(struct ether_header))
#define ETHER_MAX	(ETHERMTU + sizeof(struct ether_header))


/* The driver calls it PVTRAM and the booter calls it LCLMEM (I call it a pain).
 */
#if defined(PVTRAM)
/*
 * i82596 cannot access CPU's memory: it uses a private ram area.
 * Driver must copy data to/from controller ram.  Always uses
 * i82596 simplified mode for both recv and xmit. xmit data is
 * copied from mbuf into TCB, no TBD list is used.  recv data is
 * copied from RFD into mbuf, no RBD list is used.
 */
#else
/*
 * i82596 shares CPU's memory: direct access to CPU data is possible.
 * The i82596 supports a "flexible" data structure mode that allows
 * pointers to data rather than requiring copy operations.  Unfortunately,
 * this mode is riddled with bugs [the riddle is for *you* to figure them
 * out!]
 */
#endif
 
#include <INET/i82596.h>


/* get hardware-specific definitions */
#if defined(ie8554)
#include <ie8554/mz8554.h>
#elif defined(ie7140)
#include <ie7140/ie7140.h>
#include <acmlib.h>
#elif defined(hkvf82596)
#include <HKVF/hkvf.h>
#elif defined(if167) || defined(if177)
#include <INET/if162167.h>
#elif defined(if162) || defined(if172)
#include <INET/if162167.h>
#endif


/* This is where the shit really starts to hit the fan.  The driver and booter
 * use different structures for ie_desc, ie_stat and ifs.
 */

/*
 *	ie device descriptor specific info
 */
struct ie_desc {
	#if defined(CBOOT)
		u_int32		port;	/* base address of chip */
	#endif
	u_int32		ramaddr;	/* base address for i82596 shared ram (PVTRAM) */
	u_int32		ramoffset;	/* offset in window for i82596 */
	u_int32		ramadjust;	/* ramaddr-ramadjust = i82596 ram address */
	u_int32		ramsiz;		/* size of ram for i82596 */
	u_int32		idpromaddr;	/* address of ID prom */
	#if defined(CBOOT)			/* BOOTER only */
		u_int16		max_rfd;	/* max. receive frame descriptors */
		u_int16		max_cbl;	/* max. xmit commands to queue */
		u_int16		port_off;	/* offset (from device addr) to port access  */
		u_int16		ca_off;		/* offset (from device addr) to channel attention */
		u_int16		irq_off;	/* offset (from device addr) to clear IRQ */
		u_int8		ramwidth;	/* 2=D16 (word) ram, 4=D32 (longword) ram */
		u_int8		portwidth;	/* i82596 port access width */
		u_int8		ca_width;	/* i82596 channel access width */
		u_int8		irq_width;	/* i82596 clear IRQ width */
	#else						/* DRIVER only */
		u_int32		ca_offset;	/* offset to channel attn address */
		u_int16		max_rfd;	/* max. receive frame descriptors */
		u_int16		max_cbl;	/* max. command block descriptors */
		u_int16		t_on;		/* i82596 t-on  bus throttle value */
		u_int16		t_off;		/* i82596 t-off bus throttle value */
		u_int8		chipmode;	/* 0=auto config, 1=flex_xmit, 2=simp_xmit */
	#endif
	u_int8		sysbus;		/* i82596 SCP sysbus value */
#if defined(hkvf82596)
	u_int8		licr_mask;	/* licr OR mask for vic */
#endif
};

struct ie_stat {
	u_int	unk_irq;			/* unknown irqs */
	u_int	dev_irq;			/* device irq */
	u_int	recv_irq;			/* receive interrupts */
	u_int	xmit_irq;			/* transmit interrupts */
	u_int	cmd_complete;		/* commands completed (by irq) */
	#if !defined(CBOOT)			/* for DRIVER only */
		u_int	cmd_busy;			/* commands busy (debug) */
		u_int	cmd_cu_active;
		u_int	ru_nores;			/* recv unit ran out of RFD/RBD resources */
		u_int	ru_norbd;			/* recv unit ran out of RBDs */
		u_int	rbd_enqueued;		/* number of RBDs enqueued */
		u_int	rbd_dequeued;		/* number of RBDs dequeued */
		u_int	rbd_tailadds;		/* number of RBDs added to RBL tail */
		u_int	rbd_listadd;		/* number of successful RBD lists at NORES */
		u_int	rbd_headlost;		/* times v_rbd_head != rfd_rbd && == 0 */
	#endif
	u_int	cmd_issued;			/* commands issued to CU */
	u_int	scb_busy_loops;		/* max. scb busy loops */
	u_int	cmd_dropped;		/* cmds dropped because CB list full */
	u_int	cu_starts;			/* number of cu starts */
	u_int	cu_idle;			/* number of cu idles */
	u_int	cmd_links;			/* number of cb links to active chain */
	u_int	xmit_scoops;		/* mbuf scoops required for SIMP_XMIT mode */
	u_int	needxirqstart;		/* number of cb chains left for xirq start */
	u_int	xirqstarts;			/* number of cb chains started by xirq */
	u_int	abort_irq;			/* hkvf abort IRQ count */
	u_int	recv_count;			/* number of successful receives */
	#if defined(CBOOT)			/* for BOOTER only */
		u_int recv_miss;		/* recv ready but no mbuf available */
		u_int recv_loops;		/* max number of consec. loops in recv irq */
		u_int recv_non_dma;		/* number of non-dma packets received */
		u_int recv_dma;			/* number of non-dma packets received */
	#else						/* for DRIVER only */
		u_int recv_nobuf;		/* recv ready but no mbuf available */
		u_int recv_rustart;		/* recv no resources from i82596 */
		u_int recv_loops;		/* max number of consec. loops in recv irq */
		u_int recv_giant;		/* giant packets received and tossed */
		u_int recv_unused;		/* received packets which were marked unused */
	#endif
	u_int	xmit_count;			/* number of successful transmits */
	u_int	xmit_trys;			/* number of xmit attempts */
	u_int	xmit_lcol;			/* late collision error */
	u_int	xmit_lcar;			/* lost carrier on xmit */
	u_int	xmit_lcts;			/* lost CTS on xmit */
	u_int	xmit_udma;			/* DMA underrun */
	u_int	xmit_beat;			/* heartbeat indicator */
	u_int	xmit_mtry;			/* multiple retries on xmit */
	u_int	xmit_defr;			/* deferral required for xmit */
	u_int	xmit_coll;			/* number of xmit collisions */
	#if defined(CBOOT)			/* for BOOTER only */
		u_int	err_xenq;		/* xmit requests enqueued */
		u_int	no_busd;		/* no bus_d's available for xmit DMA */
		u_int	stloops;		/* selftest loops */
		STR		str;			/* selftest result */
		#if defined(NHIST)
			u_int	cmdmax;		/* max wait spinloops */
		#endif
	#else						/* for DRIVER only */
		u_int	stloops;		/* selftest loops */
		STR		str;			/* selftest result */
		u_int8	chip_step;		/* i82596 stepping byte */
	#endif
	u_int	recv_norustart;		/* rustart called/didn't restart chip */
};

typedef struct ifs {
	sysioStatic		v_sysio;		/* kernel static storage variables */
	u_int16			v_padd;			/* longword alignment */
	struct mbuf		*v_inq;			/* input mbuf queue */
	u_int8			*v_addr;		/* base address for device */	
	u_int8			*v_shram;		/* base of shared CPU/i82596 ram */
	#if defined(CBOOT)				/* for BOOTER only */
		u_int8		*v_zpad;		/* zero padding for short packets */
	#endif
	void 			*v_port;		/* port access address */
	void			*v_ca;			/* channel attention access address  */
	#if defined(CBOOT)				/* for BOOTER only */
		void		*v_irq;			/* clear chip IRQ */
	#endif
	SCP				*v_scp;			/* ptr to chip SCP structure */
	ISCP			*v_iscp;		/* ptr to chip ISCP structure */
	SCB				*v_scb;			/* ptr to chip SCB structure */

	#if defined(CBOOT)				/* for BOOTER only */
		CB			*v_cb;			/* ptr to chip CB structure (list) */
		CB			*v_cbtail;		/* ptr to last CB in chip list to process */
		CB			*v_cbhead;		/* ptr to first CB in chip list to process */
		CB			*v_cbnext;		/* ptr to next CB entry to use */
	#else							/* for DRIVER only */
		CB			*v_cb_base;		/* ptr to original CB list */
		CB			*v_cb_head;		/* head of currently executing CB list */
#if 1	/* Mizar's fix */
		CB			*v_cb_next;		/* next CB list */
#else
		CB			*v_cb_tail;		/* tail of currently executing CB list */
#endif
		CB			*v_cb_free;		/* list of free CB entries */
	#endif
	
	#if defined(CBOOT)				/* for BOOTER only */
		RFD			*v_rfd;			/* ptr to chip first RFD structure */
	#else							/* for DRIVER only */
		RFD			*v_rfd_base;	/* ptr to original RFD list */
	#endif
	RFD				*v_rfd_head;	/* ptr to next RFD to receive from */

	#if defined(CBOOT)				/* for BOOTER only */
		#ifdef LCLMEM
			TBD		*v_xdhead;		/* ptr to xmit buffer pool */
			TBD		*v_xdlast;		/* ptr to last xmit buffer */
			TBD		*v_xdnext;		/* ptr to next xmit buffer to use */
		#endif
	#else							/* for DRIVER only */
		#ifdef PVTRAM
			struct mbuf	*v_recvfree;	/* free list of mbufs for recv */
		#else
			RBD			*v_rbd_head;	/* ptr to first unused RBD */
			RBD			*v_rbd_tail;	/* ptr to last unused RBD */
			RBD			*v_rbd_free;	/* list of free recv RBDs (inside mbufs) */
			RBD			*v_rbd_fend;	/* new tail ptr for new RBD list */
		#endif
	#endif /* CBOOT */

	u_int32			v_ramadjust;	/* v_shram-v_ramadjust = i82596 DMA address */
	u_int32			v_shramsize;	/* size of shared ram (if we allocated) */
	struct ie_stat	v_iestat;		/* driver local statistics */
	u_int16			v_needrecvfree;	/* number of mbufs needed for recvfree list */
	u_int16			v_running;		/* driver initialized and running */
	u_int16			v_irqlvl;		/* irq mask level */
	u_int16			v_nrfd;			/* number of RFDs in use */
	u_int16			v_ncbl;			/* number of CB entries in use */
	u_int16			v_sigproc;		/* signal process for init IRQ sequence */
	#if !defined(CBOOT)				/* for DRIVER only */
		u_int16			v_chipmode;		/* chip operating mode */
			#define CM_XMIT_FLEX	0x0001
			#define CM_XMIT_SIMP	0x0002
			#define CM_RECV_FLEX	0x0004
			#define CM_RECV_SIMP	0x0008
	#endif
#if defined(hkvf82596)
	u_int16			v_isV4d;		/* we is a V4d, else Corebus */
#endif
	struct ie_desc	v_desc;			/* copy of device descriptor goodies */
	u_int8 			v_idprom[32];	/* copy of ID prom */
	SCB				v_scbsnap;		/* snapshot of SCB */
	#if !defined(CBOOT)				/* for DRIVER only */
		#ifdef TIMER
			#define NTIMER		50
			u_int32			v_timer[NTIMER];
			u_int16			v_tidx;
		#endif
	#endif
} *Ifs;

#if !defined(CBOOT)				/* for DRIVER only */
	#define MAX_LOOP_CNT	10
#endif
