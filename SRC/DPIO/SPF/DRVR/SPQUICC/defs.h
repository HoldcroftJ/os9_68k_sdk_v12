 /****************************************************************************
 *  QUICC Device Driver includes                                             *
 *****************************************************************************
 *            Copyright 1989-1996 by Microware Systems Corporation           *
 *                            All Rights Reserved                            *
 *                          Reproduced Under License                         *
 *                                                                           *
 *  This software is confidential property of Microware Systems Corporation, *
 *  and is provided under license for internal development purposes only.    *
 *  Reproduction, publication, distribution, or creation of derivative works *
 *  in any form to any party other than the licensee is strictly prohibited, *
 *  unless expressly authorized in writing by Microware Systems Corporation. *
 *****************************************************************************
 *  Edition History:                                                         *
 *   #   Date       Comments                                           By    *
 *  --- --------    ----------------------------------------------- -------- *
 *  01 10/07/96 Creation                                                     *
 *     02/19/97 <***** SPF LAN Comm Pack v3.0 Release*********>              *
 *     05/05/97 <***** SPF LAN Comm Pack v3.1 Release*********>              *
 *  09 08/11/97 Added SCC channel dependent code                        taka *
 *     10/22/97 <***** OS-9000 MBX BSP Release v1.0 **********>              *
 *  12 11/25/97 Added structures to SPF_LUSTAT to support netstat       jk   *
 *  14 01/21/98 Added prototypes debug_mbuf_up, debug_mbuf_down,             *
 *               algn_dbg_data (should be in dbg_mod.h)                 jk   *
 *  15 03/05/98 Added more prototypes, conditionalized for DEBUG.       jk/sr*
 *     04/02/98 <***** SPF LAN Comm Pack v3.2 Release*********>              *
 *  18 12/28/99 Changed unused scl_chain to mc_addrs                    rds  *
 *  19 02/10/00 Added support for limited transmit queue                rds  *
 ****************************************************************************/

/*
 *	Header Files
 */

/* DPIO definitions */
#ifdef _OSK
#include <defconv.h>

#endif

/* System definitions */

#include <const.h>
#include <errno.h>
#include <signal.h>
#include <sg_codes.h>
#include <srvcb.h>
#include <io.h>
#include <types.h>
#include <modes.h>
#include <sgstat.h>
#include <stdio.h>
#include <string.h>
#ifdef _OS9000
#include <regs.h>	/* irq_change in entry.c */
#include <cache.h>
#endif

#include <nlist.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_types.h>
#include <SPF/mbuf.h>
#include <IO/piclib.h>

#ifdef DEBUG
#include <dbg_mod.h>
  /* these prototypes should be in dbg_mod.h */
  extern void debug_mbuf_up(Dbg_stat, Mbuf, u_int32);
  extern void debug_mbuf_down(Dbg_stat, Mbuf, u_int32);
  extern void algn_dbg_data(Dbg_stat, u_char *, u_int32, u_int32);
  #define DEBUG_DATA(a,b,c)		debug_data(a,b,(u_int32)c)
  #define DEBUG_STRING(a,b,c)		debug_string(a,b,(u_int32)c)
  #define DEBUG_MBUF_UP(a,b)		debug_mbuf_up(a,b,32)
  #define DEBUG_MBUF_DOWN(a,b)		debug_mbuf_down(a,b,32)
  #define DEBUG_4DATA(a,b,c,d,e)		debug_4data(a,b,c,d,e)
#else
  #define DEBUG_DATA(a,b,c)
  #define DEBUG_STRING(a,b,c)
  #define DEBUG_MBUF_UP(a,b)
  #define DEBUG_MBUF_DOWN(a,b)
  #define DEBUG_4DATA(a,b,c,d,e)
#endif

/********************************************************/
/*    			Structure Definitions					*/
/********************************************************/

#define SPF_DRSTAT \
	/* Cache control function pointer */ \
	error_code  (*cache_cctl)(u_int32 control, void *addr, u_int32 size); \
	char		dr_demarc[8];		/* Demarc for driver specifics portion */

#include <SPQUICC/qedvr.h>	/* QUICC hardware definitions */

#ifndef MAX_QUEUE
#define MAX_QUEUE 50
#endif

/*
** Note: Descriptor version 13 and earlier defined an if_stat and le_stat
** symbol. These were never used by the spquicc driver and should not
** have even been added. In descriptor edition 14 I have changed these
** from 2 16 byte character arrays to 4 u_int32's. These were further
** divided into reserved and dirty arrays. The reserved fields are ones
** that are guaranteed to be zero, even with old descriptors. The dirty
** fields are available for anything that doesn't require a known starting
** value.
*/

/* logical unit specific static storage */
#define SPF_LUSTAT \
	u_char		ether_addr[6];		/* Ethernet card h/w address		*/\
	u_int16		lu_qlimit;			/* UDP port receiving MPEG			*/\
	void		*mc_addrs;			/* Multicast address list			*/\
	u_int32		lu_irqmask;			/* Interrupt mask level (shifted)	*/\
	u_int32		lu_irqlvl;			/* Interrupt level					*/\
	u_int32		lu_vector;			/* IRQ vector number				*/\
	u_int32		lu_priority;		/* IRQ polling priority				*/\
	u_int32		lu_nrdesc;			/* Number of receive descriptors	*/\
	u_int32		lu_ntdesc;			/* Number of transmit descriptors	*/\
	QUICC_BD	*lu_rebuf_head;		/* Rebuffer receive desc chain		*/\
	void		*lu_dbg;			/* Debug data module pointer		*/\
	char		lu_dbg_name[16];	/* Name of Debug Module				*/\
	void		*lu_cache_static;	/* Cache Static storage pointer		*/\
	Dev_list	lu_ether_drvr;		/* Dev entry to pass packets up		*/\
	u_int32		lu_channel;			/* which SCC Channel number			*/\
	QUICC_BD	*next_rx_bd;		/* next rx bd						*/\
	QUICC_BD	*base_rx_bd;		/* base rx bd						*/\
	QUICC_BD	*next_tx_bd;		/* next tx bd						*/\
	QUICC_BD	*base_tx_bd;		/* base tx bd						*/\
	QUICC_BD	*trans_bd;			/* Last transmitted bd				*/\
	u_int16		recv_scce;			/* last scce value on recv irq		*/\
	u_int16		xmit_scce;			/* last scce value on xmit irq		*/\
	QUICC		*qp;				/* pointer to quicc registers		*/\
	Mbuf		lu_outq;			/* pointer to transmit mbuf queue	*/\
	u_int32		lu_running;			/* driver initialized and running	*/\
	struct qe_stat lu_qestat;		/* driver local statistics			*/\
	addr_type	lu_item_addr;		/* ITEM address structure			*/\
	struct if_data if_data;			/* BSD interface stats				*/\
	char		symbol1[16];		/* name of the first symbol			*/\
	char		symbol2[16];		/* name of the second symbol		*/\
	u_int32		lu_qsize;			/* length of xmit queue				*/\
	u_int32		lu_dirty1;			/* non-zero reserve (see note above)*/\
	u_int32		lu_reserved1[2];	/* reserved space					*/\
	u_int32		lu_dirty2[2];		/* non-zero reserve (see note above)*/\
	u_int32		lu_reserved2[2];	/* reserved space					*/\
	struct nlist my_symbols[3];		/* Symbols for SPF_GS_SYMBOLS		*/


#ifndef DEBUG_NAME
#define DEBUG_NAME	"dbg_spsin"
#endif

#define SPF_LUSTAT_INIT \
	{EA0, EA1, EA2, EA3, EA4, EA5},	/* Ethernet card h/w address		*/\
	MAX_QUEUE,						/* lu_qlimit						*/\
	NULL,							/* Multicast address list			*/\
	IRQMASK,						/* Interrupt mask level (shifted)	*/\
	IRQLEVEL,						/* Interrupt level					*/\
	VECTOR,							/* IRQ vector number				*/\
	PRIORITY,						/* IRQ polling priority				*/\
	RX_RING_CNT,					/* Number of receive descriptors	*/\
	TX_RING_CNT,					/* Number of transmit descriptors	*/\
	NULL,							/* Rebuffer receive desc chain		*/\
	NULL,							/* Debug data module pointer		*/\
	{DEBUG_NAME},					/* Name of Debug Module				*/\
	NULL,							/* Cache static storage pointer		*/\
	NULL,							/* device entry point above us		*/\
	0,								/* which SCC Channel number			*/\
	NULL,							/* next rx bd						*/\
	NULL,							/* base rx bd						*/\
	NULL,							/* next tx bd						*/\
	NULL,							/* base tx bd						*/\
	NULL,							/* Last transmitted bd				*/\
	0,								/* last scce value on recv irq		*/\
	0,								/* last scce value on xmit irq		*/\
	NULL,							/* pointer to quicc registers		*/\
	NULL,							/* pointer to transmit mbuf queue	*/\
	0,								/* driver initialized and running	*/\
	{ 0 },							/* driver local statistics			*/\
	{								/* ITEM addr_type structure			*/\
		ITE_ADCL_ENET,				/* addr_class						*/\
		ITE_ADSUB_NONE,				/* addr_subclass					*/\
		0,							/* addr_rsv1						*/\
		6,							/* addr_size						*/\
		{0}							/* addr[32]							*/\
	},\
	{								/* BSD if_data stucture				*/\
		IFT_ETHER,					/* Interface type					*/\
		6,							/* Interface address length			*/\
		14,							/* Interface header length			*/\
		ETHERMTU,					/* Ethernet MTU						*/\
		0,							/* routing metric					*/\
		10000000,					/* Interface speed, 10Mbps			*/\
		0,0,0,0,					/* In pkt, In err, Out pkt, Out err	*/\
		0,0,0,0,0,					/* Coll, In byt, Out byt			*/\
		0,0,						/* In drop, No Proto				*/\
		{0}							/* Last change time (not used)		*/\
	},\
	{"bsd_if_data"},				/* symbol1 string					*/\
	{"item_addr"},					/* symbol2 string					*/\
	0,								/* lu_qsize							*/\
	0,								/* lu_dirty1						*/\
	{0},							/* lu_reserved1						*/\
	{0},							/* lu_dirty2						*/\
	{0},							/* lu_reserved2						*/\
	{\
		{&my_lu.lucom.symbol1[0],	/* address of name of symbol 1		*/\
		(unsigned long) &my_lu.lucom.if_data, /* address of stats		*/\
		N_ABS},						/* indicate absolute address		*/\
		{&my_lu.lucom.symbol2[0],	/* address of name of symbol 2		*/\
		(unsigned long) &my_lu.lucom.lu_item_addr,	/* address of stats	*/\
		N_ABS},						/* indicate absolute address		*/\
		{0, 0, 0}, \
	}


/* SPF definitions */
#ifndef SPF_DRVR
#define SPF_DRVR        /* Make sure to include this!!! */
#endif
#include <SPF/spf.h>

/*
 * Structure of a 10Mb/s Ethernet header.
 */
typedef struct  ether_header {
        u_char  ether_dhost[6];
        u_char  ether_shost[6];
        u_short ether_type;
} ETH_HEADER, *Eth_header;

/*
 *	Device Driver Specific Definitions
 */
#define ETHERMTU        1500
#define ETHERMIN        (60-14)
#define ETHERMAX        (ETHERMTU + sizeof(struct ether_header))

#include <SPQUICC/history.h>
#include <SPQUICC/proto.h>

/*
 *	Macro Definitions
 */

#ifdef _OSK
	#define maskirq(a) irq_change(a>>8)
	#define restirq(a) irq_change(a)
#else
	#define maskirq(a) irq_maskget(1)
	#define restirq(a) irq_change(a)
#endif
