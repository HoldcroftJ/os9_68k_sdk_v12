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
 *     ??/??/97 Ported to SPF                                          ???
 *   2 05/22/97 Changed to not include any INET header files           tmd
 *   3 07/15/97 Ported to OS9/68k                                      jpd
 *   4 08/27/97 Bug fix                                                taka
 *   6 09/29/97 Added MVME172 support                                  pjw
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  11 06/22/99 Integrated Kei Thomsen's changes, fixed mbuf leak.     jk
 *  12 08/29/99 Cleaned up code that wasn't being used                 rds
 *  15 08/17/01 Changed some pointer to be volatile.                   KeiT
 ****************************************************************************/


#include <signal.h>


#define ETHER_MIN ((ETHERMIN+4) + sizeof(struct ether_header))
#define ETHER_MAX (ETHERMTU + sizeof(struct ether_header))

#if defined(PVTRAM)
/*
** i82596 cannot access CPU's memory: it uses a private ram area.
** Driver must copy data to/from controller ram.  Always uses
** i82596 simplified mode for both recv and xmit. xmit data is
** copied from mbuf into TCB, no TBD list is used.  recv data is
** copied from RFD into mbuf, no RBD list is used.
*/
#else
/*
** i82596 shares CPU's memory: direct access to CPU data is possible.
** The i82596 supports a "flexible" data structure mode that allows
** pointers to data rather than requiring copy operations.  Unfortunately,
** this mode is riddled with bugs [the riddle is for *you* to figure them
** out!]
*/
#endif

#include <SP82596/i82596.h>

/* get hardware-specific definitions */
#if defined(sp167) || defined(sp162) || defined(spgenesis) || defined(sp177) || defined(sp172)

	/* systems limits */
	#define MAX_CBL 1
	#define MAX_RFD 4

	/* chip access macros */
	#define I82596_RESET(xx) doport_command(xx, 0, 0)
	#define I82596_SELFTEST(xx, addr) doport_command(xx, addr, 1)
	#define I82596_ASCP(xx, addr) doport_command(xx, addr, 2)
	#define I82596_DUMP(xx, addr) doport_command(xx, addr, 3)
	#if defined (Flushing)
	#define I82596_CHANATTN(xx)	nop();  *((volatile u_int32 *)xx->lu_ca) = 0; \
		_os_cache(4);               /* Flush data cache */
	#else
    #define I82596_CHANATTN(xx)		nop();  *((volatile u_int32 *)xx->lu_ca) = 0;
	#endif

	/* i82596 DMA adjustment macro */
	#define I82596_DMA_ADJ(ifstat, value) ((void *)(value))

	/* board address offsets */
    #define LANC_ERR_STATUS	((volatile u_int8  *)0xfff42028)
    #define LANC_IRQ_CONTROL	((volatile u_int8  *)0xfff4202a)
    #define LANC_BEIRQ_CONTROL	((volatile u_int8  *)0xfff4202b)
	#define LANC_ICLR		0x08	/* clear LANC interrupt */
	#define LANC_IEN		0x10	/* enable IRQ */
	#define LANC_INT		0x20	/* IRQ asserted */
	#define LANC_EoL		0x40	/* edge=1 level=0 sensitive irq */
	#if defined(sp177) || defined(sp172)
		#define LANC_SNOOP	(1<<7)	/* turn on local bus snooping */
	#else
		#define LANC_SNOOP	(1<<6)	/* turn on local bus snooping */
	#endif
	#define BBRAM_ETHERADDR				((u_int8  *)0xfffc1f2c)

	/* debugging */
	#if defined(DEBUG)
	#define BUSMARK(x) (*((volatile u_int8 *)0xffff8080) = (x))  /* vmebus write */
	#endif


	/* VME162 hardware options */
	#if defined(sp162) || defined(sp172)
		#define	MCOPTIONS	0xfff4202e
		#define	NOLANC		0x08
		#define	nolanc()	((int) (*((u_char *)MCOPTIONS) & NOLANC))
	#endif

#elif defined(ie8554)
	#include <ie8554/mz8554.h>
#elif defined(ie7140)
	#include <ie7140/ie7140.h>
	#include <acmlib.h>
#elif defined(hkvf82596)
	#include <HKVF/hkvf.h>
#elif defined (spdve7708)
	/* chip access macros */
	#define I82596_RESET(xx) doport_command(xx, 0, 0)
	#define I82596_SELFTEST(xx, addr) doport_command(xx, addr, 1)
	#define I82596_ASCP(xx, addr) doport_command(xx, addr, 2)
	#define I82596_DUMP(xx, addr) doport_command(xx, addr, 3)
#if 0
	#if defined (Flushing)
		#define I82596_CHANATTN(xx) nop();  *((u_int32 *)xx->lu_ca) = 0; \
		_os_cache(4);               /* Flush data cache */
	#else
		#define I82596_CHANATTN(xx) nop();  *((u_int32 *)xx->lu_ca) = 0;
	#endif
#endif

#endif

#if defined (_MPFSH)
 #define MAKE_P3_OFFSET(xx) ((u_int32)(xx) | 0xa0000000)
#else
 #define MAKE_P3_OFFSET(xx) ((u_int32)(xx))
#endif

struct ie_stat {
	u_int	unk_irq;			/* unknown irqs */
	u_int	dev_irq;			/* device irq */
	u_int	recv_irq;			/* receive interrupts */
	u_int	xmit_irq;			/* transmit interrupts */
	u_int	cmd_complete;		/* commands completed (by irq) */
	u_int	cmd_busy;			/* commands busy (debug) */
	u_int	cmd_cu_active;
	u_int	ru_nores;			/* recv unit ran out of RFD/RBD resources */
	u_int	ru_norbd;			/* recv unit ran out of RBDs */
	u_int	rbd_enqueued;		/* number of RBDs enqueued */
	u_int	rbd_dequeued;		/* number of RBDs dequeued */
	u_int	rbd_tailadds;		/* number of RBDs added to RBL tail */
	u_int	rbd_listadd;		/* number of successful RBD lists at NORES */
	u_int	rbd_headlost;		/* times v_rbd_head != rfd_rbd && == 0 */
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
	u_int	recv_nobuf;			/* recv ready but no mbuf available */
	u_int	recv_rustart;		/* recv no resources from i82596 */
	u_int	recv_loops;			/* max number of consec. loops in recv irq */
	u_int	recv_giant;			/* giant packets received and tossed */
	u_int	recv_unused;		/* received packets which were marked unused */
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
	u_int	stloops;			/* selftest loops */
	STR		str;				/* selftest result */
	u_int8	chip_step;			/* i82596 stepping byte */
	u_int	recv_norustart;		/* rustart called/didn't restart chip */
	u_int	only_one_rbd;		/* Only one RBD left */
	u_int	rbd_kill_head;		/* Killed buffer */
	u_int	rbd_kill_heads;		/* # Killed buffer */
};

#define CM_XMIT_FLEX	0x0001
#define CM_XMIT_SIMP	0x0002
#define CM_RECV_FLEX	0x0004
#define CM_RECV_SIMP	0x0008

#define MAX_LOOP_CNT	10
