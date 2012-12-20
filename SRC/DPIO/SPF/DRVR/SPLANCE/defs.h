/****************************************************************************
 *  $Workfile:   defs.h  $
 *  $Revision:   1.1  $
 *  $Modtime:   15 Mar 1994 15:12:00  $
 *  $Date:   15 Mar 1994 15:12:16  $
 ****************************************************************************
 *  LANCE Device Driver includes		                            *
 ****************************************************************************
 *  Copyright 1994 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date       Comments                                           By   *
 *  --- --------    ----------------------------------------------- ------- *
 *    0 01/11/94    Creation                                            bat *
 *    1 01/11/94    More additions                                      bat *
 *    3 01/27/94    Added wait-for-connection ability                   bat *
 *    4 01/28/94    Moved hw definitions to ports directory             bat *
 *      03/15/94    <***************** SPF v1.0 ******************>         *
 *	05/03/95    modified for MACE                                   amg *
 *	27FEB96	    modified for LANCE               Bill West Inc.     bw  *
 *    4 09/03/94    pop and push bug fix                                    *
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release*********>         *
 *      05/05/97    <***** SPF LAN Comm Pack v3.1 Release*********>         *
 *   10 10/23/97    Added fields for statistics                         tmd *
 *   11 01/08/98    Added #include <memory.h>                            jk *
 *     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              *
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
#include <memory.h>
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
#endif
#include <nlist.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_types.h>

#ifdef DEBUG
#include <dbg_mod.h>
#endif

#include <SPF/mbuf.h>

typedef struct if_data * If_data;

#define ETHERMTU        1500

/********************************************************/
/*    			Structure Definitions					*/
/********************************************************/

#define SPF_DRSTAT \
	char		dr_demarc[8];		/* Demarc for driver specifics portion	*/

#include <SPLANCE/am7990.h>	/* LANCE hardware definitions */

/* logical unit specific static storage */
#define SPF_LUSTAT  \
    struct ifs	ifstat;			/* ugly static storage from ISP	*/\
    u_char		ether_addr[6]; 	/* Ethernet card h/w address	*/\
    u_int16		mpeg_udp_port; 	/* UDP port receiving MPEG	*/\
    void		*scl_chain; 	/* SCL chain for MPEG	*/\
    u_int32		lu_irqmask,     /* Interrupt mask level (shifted) */\
				lu_irqlvl,		/* Interrupt level */\
    			lu_vector,      /* IRQ vector number */\
    			lu_priority;    /* IRQ polling priority */\
    void*		lu_dbg; 					\
    u_int16		ib_mode;		/* initialization block mode */\
    u_char		le_hiaddr;		/* high address byte of local mem */\
    u_char		recv_ring_cnt;	/* receive ring length */\
    u_char		xmit_ring_cnt;	/* transmit ring length */\
    u_char		le_csr3;		/* lance csr3 initialization */\
    u_int32		le_cpu_base;	/* address of Lance LOCAL MEM */\
    u_int32		le_ram_size;	/* size of Lance LOCAL MEM */\
    u_int32		le_lan_adj;		/* addr trans for LANCE LOCAL MEM */\
    u_int32		bbram_lanid;	/* addr of Ethernet addr in bbram */\
	Dev_list	lu_ether_drvr;	/* Ethernet driver Device entry point */ \
	addr_type	lu_item_addr;	/* Generic ITEM address structure */ \
	char        lu_ifdesc[32];  /* Interface description */ \
	struct if_data if_data;		/* BSD interface stats */ \
	char        symbol1[16];    /* name of the first symbol */ \
	char        symbol2[16];    /* name of the second symbol */ \
	char        symbol3[16];    /* name of the third symbol */ \
	char        symbol4[16];    /* name of the fourth symbol */ \
	struct nlist lance_symbols[5]; /* symbol returned via spf_gs_symbols */

    
#define SPF_LUSTAT_INIT \
    {0},			/* ugly static storage from ISP */\
    {EA0, EA1, EA2, EA3, EA4, EA5},		/* Ethernet card h/w address    */\
    0,				/* UDP port receiving MPEG  */\
    NULL,			/* SCL chain for MPEG   */\
    IRQMASK,		/* Interrupt mask level (shifted) */\
    IRQLEVEL,		/* Interrupt level */\
    VECTOR,			/* IRQ vector number */\
    PRIORITY,		/* IRQ polling priority */\
    NULL,			/* Debug data module pointer	*/\
    IB_MODE,		/* Lance Mode byte */\
    HIADDR,			/* High address byte */\
    RX_RING_CNT,	/* Number of buffers in LANCE rcv ring */\
    TX_RING_CNT,	/* Number of buffers in LANCE xmt ring */\
    CSR3_INIT,		/* CSR3 register init value */\
    LCLMEM_BASE,	/* address of Lance LOCAL MEM */\
    LCLMEM_SIZE,	/* size of Lance LOCAL MEM */\
    LCLMEM_TRANS,	/* addr trans for LOCAL MEM */\
    BBRAM_LANID,	/* addr of Ethernet addr in bbram */\
    NULL,			/* ethernet driver device entry point */ \
	{				/* ITEM addr_type structure */ \
		ITE_ADCL_ENET,	/* addr_class */ \
		ITE_ADSUB_NONE,	/* addr_subclass */ \
		0,				/* addr_rsv1 */ \
		6,				/* addr_size */ \
		{0}				/* addr */ \
	}, \
	{"Lance 7990 Ethernet"}, /* lu_ifdesc */ \
	{				/* BSD if_data stucture */ \
		IFT_ETHER,  /* Interface type */ \
		6,          /* Interface address length */ \
		14,         /* Interface header length */ \
		ETHERMTU,   /* Ethernet MTU */ \
		0,          /* routing metric */ \
		10000000,   /* Interface speed, 10Mbps  */ \
		0,0,0,0,    /* In pkt, In err, Out pkt, Out err */ \
		0,0,0,0,0,  /* Coll, In byt, Out byt, In Mul, Out Mul */ \
		0,0,        /* In drop, No Proto */ \
		{0}         /* LastChange up time when operational */ \
	},\
	{"bsd_if_data"},            /* symbol1 string */\
	{"item_addr"},              /* symbol2 string */\
	{"if_stat"},                /* symbol3 string */\
	{"le_stat"},                /* symbol4 string */\
	{\
		{&my_lu.lucom.symbol1[0],   /* address of name of symbol 1*/ \
		(unsigned long) &my_lu.lucom.if_data, /* address of stats */ \
		N_ABS},                     /* indicate absolute address */ \
		{&my_lu.lucom.symbol2[0],   /* address of name of symbol 2*/ \
		(unsigned long) &my_lu.lucom.lu_item_addr, /* address of stats */ \
		N_ABS},                     /* indicate absolute address */ \
		{&my_lu.lucom.symbol3[0],   /* address of name of symbol 3*/ \
		(unsigned long) &my_lu.lucom.ifstat, /* address of ifstat */ \
		N_ABS},                     /* indicate absolute address */ \
		{&my_lu.lucom.symbol4[0],   /* address of name of symbol 4*/ \
		(unsigned long) &my_lu.lucom.ifstat.v_lestat, /* address of lestat */ \
		N_ABS},                     /* indicate absolute address */ \
		{0, 0, 0},                  /* null nlist termination */ \
	}

/* SPF definitions */
#ifndef SPF_DRVR
#define SPF_DRVR        /* Make sure to include this!!! */
#endif
#include <SPF/spf.h>

/*
 *	Device Driver Specific Definitions
 */
#define ETHERMTU        1500
#define ETHERMIN        (60-14)

/*
 * Structure of a 10Mb/s Ethernet header.
 */
typedef struct  ether_header {
        u_char  ether_dhost[6];
        u_char  ether_shost[6];
        u_short ether_type;
} ETH_HEADER, *Eth_header;

#include <SPLANCE/history.h>
#include <SPLANCE/proto.h>

/*
 *	Macro Definitions
 */
/************************************************************************
 *									*
 * Definitions for using the setstats of the LANCE SPF driver		*
 *									*
 ************************************************************************/


#define SS_SPFDRVR		0xFFAA
#define SS_LANCE_BASE	(SPF_PR_MACE << 16)
#define SS_READSTREAM	(SS_LANCE_BASE + 1)


typedef struct	ss_MPEG_pb	
{
	void   	*scl_chain;		/* pointer to the SCL chain to use */
	u_int16 mpeg_udp_port;	/* The UDP port that the data will be received from	*/

} *Ss_MPEG_pb;

/* Because we sometimes pre-allocate mbufs for the receive interrupt,
it is possible that the updrvr won't be defined when an mbuf is allocated,
but will be defined when a packet is received.  Therefore, we need to force
get_mbuf() to leave space to fill in the address of the updrvr. */

#define DUMMY_UPDRVR ((Dev_list)1)	/* force get_mbuf() to allocate space for uplink */
