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
 *   2 05/22/97 Changed to not include any INET header files           tmd
 *              Followed SPLANCE's lead by putting stuff in defs.h     tmd
 *   3 07/15/97 Ported to OS9/68k                                      jpd
 *   4 08/27/97 Bug fix                                                taka
 *   5 09/08/97 fixed os_irq()                                         taka
 *   7 12/03/97 Added structures to SPF_LUSTAT to support netstat      jk
 *   8 01/21/98 Added prototypes for _os9_irq, _os_irq                 jk
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *   9 11/18/98 changed lu_ncbl from 8 to 20 for performance increase  mgh
 *  11 06/22/99 Integrated Kei Thomsen's changes, fixed mbuf leak.     jk
 *  12 08/29/99 Cleaned up and removed unused stuff                    rds
 *              Changed lu_config_data to enable short packet padding  rds
 ****************************************************************************/

/*
** Header Files
*/
#include <types.h>
#include <errno.h>
#include <const.h>
#include <modes.h>
#include <sg_codes.h>
#include <memory.h>
#include <events.h>
#include <string.h>

#if defined (sp167)
#include <signal.h>
#include <module.h>
#endif

#include <nlist.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_types.h>

#if defined(_OSK)
#include <defconv.h>
#include <regs.h>
#include <setsys.h>

#elif defined(_OS9000)
	#include <io.h>
#endif

#include <SPF/mbuf.h>

#if defined(sp177) || defined(sp172) || defined(sp167) || defined(sp162)
/*
** In theory the 16X's should not need cache flushing because they
** do bus snooping. There is some belief this does not work correctly
** so we will error on the side of caution and turn it on for them.
*/
#ifndef Flushing
#define Flushing
#endif
#endif /* sp177 || sp172 || sp167 || sp162 */

#define ETHERMTU        1500
#define ETHERMIN        (60-14)
/*
** Structure of a 10Mb/s Ethernet header.
*/
typedef struct  ether_header {
		u_char  ether_dhost[6];
		u_char  ether_shost[6];
		u_short ether_type;
} ETH_HEADER, *Eth_header;

#include <process.h>

#include <SP82596/ie.h>

#ifdef DEBUG
#include <dbg_mod.h>
void debug1 (void *q, void *p);
void debug2 (void *q, void *p);
void debug3 (void *q, void *p);
void debug4 (void *q, void *p);
void debug5 (void *q, void *p);
void debug6 (void *q, void *p);
void debug7 (void *q, void *p);

#define DEBUG_DATA(a,b,c) debug_data((a),(b),(c))
#define DEBUG_4DATA(a,b,c,d) debug_4data((a),(b),(c),(d))

#define DEBUG1(a,b) debug1((a),(b))
#define DEBUG2(a,b) debug2((a),(b))
#define DEBUG3(a,b) debug3((a),(b))
#define DEBUG4(a,b) debug4((a),(b))
#define DEBUG5(a,b) debug5((a),(b))
#define DEBUG6(a,b) debug6((a),(b))
#define DEBUG7(a,b) debug7((a),(b))

#define SYSDBG(a,b)     _os_sysdbg((a),(b))

#else

#define DEBUG_DATA(a,b,c)
#define DEBUG_4DATA(a,b,c,d)

#define DEBUG1(a,b)
#define DEBUG2(a,b)
#define DEBUG3(a,b)
#define DEBUG4(a,b)
#define DEBUG5(a,b)
#define DEBUG6(a,b)
#define DEBUG7(a,b)

#define SYSDBG(a,b)
#endif

/*
**  Macro Definitions
*/

#if defined(_OSK)
	#define _OS_IRQ(a,b,c,d,e) _os9_irq(a,b,c,d,e)
	error_code _os9_irq(u_int32, u_int32, void *, void *, void *);
	#define maskirq(a) irq_change(a<<8)
	#define restirq(a) irq_change(a)
#elif defined(_OS9000)
	#define _OS_IRQ(a,b,c,d,e) _os_irq(a,b,c,d)
	error_code _os_irq (u_int32, u_int32, void *, void *);
	#define maskirq(a) irq_maskget(1);
	#define restirq(a) irq_change(a)
#endif

#define nop() {_asm(" nop");}

/* spproto-Specific Driver Definitions */
#define SPF_DRSTAT \
	char dr_demarc[8];	/* Demarc for driver specifics portion */ \
	struct ie_stat dr_iestat;

/* device-specific logical unit static fields.  Must be defined before spf.h
 * is included. */

/*
** Note: Driver versions previous to 12 had two extra symbols incorrectly
** defined in the logical unit statics. They each used the first 7 bytes
** of the 16 allocated to them. I have redefined these as u_int32's and
** the 4 unused ones are now reserved fields (rsv1 and rsv2). The first
** u_int32 of rsv1 has been defined as a pointer to the driver statics
** and is used by the iestat program. The other 3 u_int32's are available.
*/

#define SPF_LUSTAT \
	u_char		lu_ether_addr[6];	/* Ethernet card h/w address        */\
	u_char		lu_reserv[2];		/* alignment                        */\
	Dev_list	lu_ether_drvr;		/* x50 ethernet driver              */\
	u_int32		lu_irqmask;			/* Interrupt mask level (shifted)   */\
	u_int32		lu_irqlvl;			/* Interrupt level                  */\
	u_int32		lu_vector;			/* IRQ vector number                */\
	u_int32		lu_priority;		/* IRQ polling priority             */\
	u_int8		*lu_addr;			/* base address for device          */\
	u_int32		*lu_dga;			/* dga address                      */\
	void		*lu_irqclr;			/* irq clr port                     */\
	void		*lu_ca;				/* channel attention access address */\
	SCP			*lu_scp;			/* ptr to chip SCP structure        */\
	ISCP		*lu_iscp;			/* ptr to chip ISCP structure       */\
	SCB			*lu_scb;			/* ptr to chip SCB structure        */\
	CB			*lu_cb_base;		/* ptr to original CB list          */\
	CB			*lu_cb_head;		/* head of currently executing CB list */\
	CB			*lu_cb_next;		/* next CB list                     */\
	CB			*lu_cb_free;		/* list of free CB entries          */\
	struct mbuf	*lu_recvfree;		/* free list of mbufs for recv      */\
	RFD			*lu_rfd_base;		/* ptr to original RFD list         */\
	RFD			*lu_rfd_head;		/* ptr to next RFD to receive from  */\
	RBD			*lu_rbd_head;		/* ptr to first unused RBD          */\
	RBD			*lu_rbd_tail;		/* ptr to last unused RBD           */\
	RBD			*lu_rbd_free;		/* list of free recv RBDs (inside mbufs) */\
	RBD			*lu_rbd_fend;		/* new tail ptr for new RBD list    */\
	u_int16		lu_needrecvfree;	/* lu_recvfree shortfall            */\
	u_int32		lu_mbufstart;		/* start of mbuf pool .. ptr >= mbfustart */\
	u_int32		lu_mbufend;			/* end of mbuf pool .. ptr < mbufend */\
	u_int32		lu_irq;				/* irq on PICs                      */\
	u_int16		lu_nrfd;			/* number of RFDs in use            */\
	u_int16		lu_ncbl;			/* number of CB entries in use      */\
	u_int8		*lu_shram;			/* base of shared CPU/i82596 ram    */\
	size_t		lu_shramsize;		/* sizeof lu_shram                  */\
	size_t		lu_ramsiz;			/* size of ram for I82596           */\
	size_t		lu_ramoffset;		/* offset in window for I82596      */\
	u_int32		lu_ramadjust;		/* lu_shram-lu_ramadjust = i82596 DMA addr */\
	u_int8		lu_chipmode;		/* 0=auto config, 1=flex_xmit, 2=simp_xmit */\
	u_int8		lu_sysbus;			/* sysbus value                     */\
	u_int16		lu_t_on;			/* i82596 t-on bus throttle value   */\
	u_int16		lu_t_off;			/* i82596 t-off bus throttle value  */\
	process_id	lu_sigproc;			/* process id                       */\
	u_char		lu_config_data[16]; /* i82596 config data               */\
	void*		lu_dbg;				/* debugging pointer                */\
	char		lu_dbg_name[16];	/* Name of debug module             */\
	addr_type	lu_item_addr;		/* ITEM address structrure          */\
	struct if_data if_data;			/* BSD interface stats              */\
	char		symbol1[16];		/* name of the first symbol         */\
	char		symbol2[16];		/* name of the second symbol        */\
	u_int32		lu_junk1[2];		/* no longer used                   */\
	Spf_drstat	lu_drstat;			/* pointer to driver statics        */\
	u_int32		lu_rsv1;			/* reserved field                   */\
	u_int32		lu_junk2[2];		/* no longer used                   */\
	u_int32		lu_rsv2[2];			/* reserved field                   */\
	struct nlist lance_symbols[3];	/* Symbols avail for spf_gs_symbols */

#ifndef SP82596_DBG_NAME
#define SP82596_DBG_NAME	"dbg_spsin"
#endif

#define SPF_LUSTAT_INIT \
	{EA0, EA1, EA2, EA3, EA4, EA5}, /* Descriptor defined Ethernet address */\
	{0,0},					/* aliginment                             */\
	NULL,					/* lu_ether_drvr                          */\
	IRQMASK,				/* Interrupt mask level (shifted)         */\
	IRQLEVEL,				/* Interrupt level                        */\
	VECTOR,					/* IRQ vector number                      */\
	PRIORITY,				/* IRQ polling priority                   */\
	NULL,					/* lu_addr                                */\
	NULL,					/* lu_dga                                 */\
	NULL,					/* lu_irqclr                              */\
	NULL,					/* lu_ca                                  */\
	NULL,					/* lu_scp                                 */\
	NULL,					/* lu_iscp                                */\
	NULL,					/* lu_scb                                 */\
	NULL,NULL,NULL,NULL,	/* lu_cb_*                                */\
	NULL,					/* lu_recvfree                            */\
	NULL,NULL,NULL,NULL,NULL,NULL, /* lu_rbd_*                        */\
	0,						/* lu_needrecvfree                        */\
	0,0,					/* lu_mbuf*                               */\
	0,						/* lu_irq                                 */\
	32,						/* lu_nrfd                                */\
	20,						/* lu_ncbl                                */\
	NULL,					/* lu_shram                               */\
	0,						/* lu_shramsize                           */\
	128*1024,				/* lu_ramsiz                              */\
	0,						/* lu_ramoffset                           */\
	0,						/* lu_ramadjust                           */\
	0,						/* lu_chipmode                            */\
	SYSBUS_CSW|SYSBUS_LOCK|SYSBUS_LINEAR, /* lu_sysbus                */\
	7*25,					/* lu_t_on (mhz)                          */\
	1*25,					/* lu_t_off (mhz)                         */\
	0,						/* lu_sigproc                             */\
	{						/* lu_config_data                         */\
		0x8e,				/*   Byte  0 - 14 valid config bytes      */\
		0xc8,				/*   Byte  1 - Default                    */\
		0x40,				/*   Byte  2 - Default                    */\
		0x2e,				/*   Byte  3 - No auto insert of src addr */\
		0x00,				/*   Byte  4 - Default                    */\
		0x60,				/*   Byte  5 - Default                    */\
		0x00,				/*   Byte  6 - Default                    */\
		0xf2,				/*   Byte  7 - Default                    */\
		0x00,				/*   Byte  8 - Default                    */\
		0x00,				/*   Byte  9 - Default                    */\
		0x40,				/*   Byte 10 - Default                    */\
		0xfd,				/*   Byte 11 - Pad short frames           */\
		0x00,				/*   Byte 12 - Default                    */\
		0x3f,				/*   Byte 13 - Default                    */\
		0x00,				/*   Byte 14 - Not Used                   */\
		0x00				/*   Byte 15 - Not Used                   */\
	},						/* end lu_config_data                     */\
	NULL,					/* lu_dbg                                 */\
	{SP82596_DBG_NAME},		/* lu_dbg_name                            */\
	{						/* ITEM addr_type structure               */\
		ITE_ADCL_ENET,		/* addr_class                             */\
		ITE_ADSUB_NONE,		/* addr_subclass                          */\
		0,					/* addr_rsv1                              */\
		6,					/* addr_size                              */\
		{0}					/* addr[32]                               */\
	}, \
	{						/* BSD if_data stucture                   */\
		IFT_ETHER,			/* Interface type                         */\
		6,					/* Interface address length               */\
		14,					/* Interface header length                */\
		ETHERMTU,			/* Ethernet MTU                           */\
		0,					/* routing metric                         */\
		10000000,			/* Interface speed, 10Mbps                */\
		0,0,0,0,			/* In pkt, In err, Out pkt, Out err       */\
		0,0,0,0,0,			/* Coll, in/out bytes, in/out multicasts  */\
		0,0,				/* In drop, No Proto                      */\
		{0}					/* LastChange up time when operational    */\
	}, \
	{"bsd_if_data"},		/* symbol1 string                         */\
	{"item_addr"},			/* symbol2 string                         */\
	{0},					/* lu_junk1                               */\
	0,						/* lu_drstat                              */\
	0,						/* lu_rsv1                                */\
	{0},					/* lu_junk2                               */\
	{0},					/* lu_rsv2                                */\
	{ \
		{&my_lu.lucom.symbol1[0],	/* address of name of symbol1     */\
		(u_int32) &my_lu.lucom.if_data,		 /* address of symbol1    */\
		N_ABS},						/* symbol1 is an absolute address */\
		{&my_lu.lucom.symbol2[0],	/* address of name of symbol2     */\
		(u_int32) &my_lu.lucom.lu_item_addr, /* address of symbol2    */\
		N_ABS},						/* symbol2 is an absolute address */\
		{0, 0, 0},			/* end of array marker                    */\
	}

/* SPF definitions */

#define SPF_DRVR  /* to include driver specific portions in spf.h */
#include <SPF/spf.h>

#include <SP82596/history.h>
#include <SP82596/proto.h>

#if defined (spdve7708)
#include <DEFS/dve7708.h>
#endif
