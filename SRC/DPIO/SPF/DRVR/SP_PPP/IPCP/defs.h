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
 *     03/19/97 Got rid of spinlib structures and header file.         sr
 *  16 03/26/97 Replaced INET Def files with BSD def files.            sr
 *  17 04/18/97 Added default_route flag to lustat.                    sr
 *  20 04/30/97 Modified to cleanup set_default_route code.            sr
 *     05/05/97 Created defs9.h, moved include files.                  sr
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  23 10/08/97 Adopted changes Bill W. made to Beta PPP code          tmd
 *  24 10/10/97 Moved default route defines to ppp.h.                  sr
 *     10/17/97 Default PPP_MODE is Active Open.                       sr
 *  25 11/04/97 Added BSD statistics                                   tmd
 *  26 11/28/97 Added ipcp_accept_remote & ipcp_accept_local flags.    tmd
 *              Added fields to save original IP addresses.            tmd
 *  29 02/16/98 Added sys/ioctl.h header for bsdioctl struct           tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#ifndef _defs_h_
#define _defs_h_

/** System definitions and include files **/

#include <events.h>
#include <io.h>
#include <module.h>
#include <rbf.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

#include <netinet/in.h>
#include <net/if.h>
#include <net/if_types.h>
#include <netinet/in_systm.h>
#define EXTERN extern
#include <netinet/tcp.h>
#include <netinet/tcp_seq.h>
#include <netinet/tcp_fsm.h>
#include <netinet/tcp_timer.h>
#include <netinet/ip.h>
#include <netinet/ip_var.h>
#include <netinet/tcpip.h>
#include <SPF/ppp.h>
#include <SPF/ppp_auth.h>
#include <SPF/spf.h>
#include <SPF/timer.h>

#include <net/route.h>
#include <nlist.h>
#include <stddef.h>

#include <SP_PPP/IPCP/compress.h>
#ifdef DRIVER
	#include "alt_dbg.h"
	#include "auth_if.h"
	#include "msg_list.h"
	#include "inetd_if.h"
	#include "lib_func.h"
	#include "notify.h"
	#include "nxt_drvr.h"
#endif /* DRIVER */

#include <SP_PPP/DEFS/msg_list.h>

#define SIG_FATAL( s )		( (s > 1 && s < 32) || s == 228 ? 1 : 0 )
#define MAX_OPT_SIZE    14      /* change this if we add more features */

/******************** IPCP State Machine Definitions ********************/

/*** States for IPCP as per RFC 1661 ***/

#define STATE_0_INIT		0
#define STATE_1_STARTING	1
#define STATE_2_CLOSED		2
#define STATE_3_STOPPED		3
#define STATE_4_CLOSING		4
#define STATE_5_STOPPING	5
#define STATE_6_REQ_SENT	6
#define STATE_7_ACK_RCVD	7
#define STATE_8_ACK_SENT	8
#define STATE_9_OPENED		9

#define MAX_IPCP_STATES		10

/*** Events for IPCP as per RFC 1661 ***/

#define NO_EVENT			-1
#define EVENT_UP			0	/* lower layer is up						*/
#define EVENT_DOWN			1	/* lower layer is down						*/
#define EVENT_OPEN			2	/* this layer is opened						*/
#define EVENT_CLOSE			3	/* this layer is closed						*/
#define EVENT_TM0			4	/* Timeout with counter value = 0			*/
#define EVENT_TM1			5	/* Timeout with counter value = 1			*/
#define EVENT_RCR_GOOD		6	/* recvd a good configure request message	*/
#define EVENT_RCR_BAD		7	/* recvd a unacceptable config request msg	*/
#define EVENT_RCA			8	/* receive configure ack					*/
#define EVENT_RCN			9	/* receive configure nak					*/
#define EVENT_RTR			10	/* receive terminate request				*/
#define EVENT_RTA			11	/* receive terminate ack					*/
#define EVENT_RUC			12	/* receive unknown code						*/
#define EVENT_RXJ_GOOD		13	/* receive code reject (permitted)			*/
#define EVENT_RXR			14	/* receive echo request						*/

#define MAX_EVENTS			15

/*** PPP assigned types ***/

#define CONFIGURE_REQUEST	1
#define CONFIGURE_ACK		2
#define CONFIGURE_NAK		3
#define CONFIGURE_REJECT	4
#define TERMINATE_REQUEST	5
#define TERMINATE_ACK		6
#define CODE_REJECT			7
#define PROTOCOL_REJECT		8
#define ECHO_REQUEST		9
#define ECHO_REPLY			10
#define DISCARD_REQUEST		11

/*** IPCP assigned configuration types ***/

#define IP_ADDRESSES		1	/* IP Addresses					*/
#define IP_COMPR_PROT		2	/* IP Compression protocol		*/
#define IP_ADDRESS			3	/* IP Address					*/


/*** Structure defining an entry in the state machine array ***/

typedef error_code (*Action)(Dev_list);

#define MAX_ACTIONS		3

typedef struct
{
	u_int8	next_state;
	Action	action[MAX_ACTIONS];

} ipcp_entry, *Ipcp_entry;

typedef struct
{
	ipcp_entry   entry[MAX_EVENTS];

} ipcp_state, *Ipcp_state;

/**************** Driver Static Storage Definitions ***************/

#define SPF_DRSTAT \
	void*           std_state_tbl;  \
	void*           alt_state_tbl;  \
	auth_handle     auth_module;        /* Authentication module        */\
	void*           ip_db_module;       /* Pointer to IP database module*/\
	u_int16         dr_cstable_iniz;    /* TRUE == Initialized          */\
	slcompress      dr_cstable;         /* Compression table            */

#define SPF_DRSTAT_INIT \
	NULL,                               /* std_state_tbl            */\
	NULL,                               /* alt_state_tbl            */\
	NULL,                               /* auth_module              */\
	NULL,                               /* ip_db_module             */\
	0,                                  /* dr_cstable_iniz          */\
	{ 0 }                               /* dr_cstable               */


/**************** Logical Unit Static Storage Definitions ***************/


/* IPCP assigned configuration defaults */

#ifndef DESC_NAME
	#define DESC_NAME				"ipcp0"
#endif

#ifndef INTERFACE_NAME
	#define INTERFACE_NAME			"ppp0"
#endif

#ifndef LOWER_LAYER_PATH
	#define LOWER_LAYER_PATH		""
#endif

#ifndef PPP_MODE
	#define PPP_MODE				NO_WAIT_ON_OPEN
#endif

#ifndef MAX_SCR
	#define MAX_SCR					10
#endif

#ifndef MAX_STR
	#define MAX_STR					2
#endif

#ifndef TIMEOUT
	#define TIMEOUT					3000
#endif

#ifndef TCP_COMPRESSION
	#define TCP_COMPRESSION			COMPRESSED_TCP
#endif

#ifndef MAX_SLOT_ID
	#define MAX_SLOT_ID				15
#endif

#ifndef COMP_SLOT_ID
	#define COMP_SLOT_ID			1
#endif

#ifndef DEFAULT_ROUTE
	#define DEFAULT_ROUTE           NO_DEFAULT_ROUTE
#endif

#ifndef IPCP_ACCEPT_REMOTE
	#define IPCP_ACCEPT_REMOTE		TRUE
#endif

#ifndef IPCP_ACCEPT_LOCAL
	#define IPCP_ACCEPT_LOCAL		TRUE
#endif

typedef struct
{
	u_int32		rx_cslot : 1;	/* Compress Slot ID (RX). */
	u_int32		tx_cslot : 1;	/* Compress Slot ID (TX). */
	u_int32		rx_pfc : 1;		/* Protocol field compression (RX). */
	u_int32		tx_pfc : 1;		/* Protocol field compression (TX). */

	u_int32		cfg_req_chg : 1;	/* Cfg req message params changed.	*/
	u_int32		timer_on : 1;		/* Timer is running.				*/
	u_int32	    max_timeouts : 1;	/* We've used up all our timeouts.	*/

} ipcp_flags, *Ipcp_flags;



typedef struct ipcp_lustatic	ipcp_lustat, *Ipcp_lustat;

struct ipcp_lustatic
{
	/*************************** Driver Info ****************************/
	char		lock_name[MAX_NAME_LEN];	/* Locking event name.		*/
	event_id	lock_id;			/* ID of event used for locking.	*/
	u_int32		down_ctr;			/* Number of packets sent down.		*/
	u_int32		up_ctr;				/* Number of packets sent up.		*/
	Notify_entry notify_up;		/* Clients to notify when I/O enabled.	*/
	Notify_entry notify_down;	/* Clients to notify when I/O disabled.	*/
	ipcp_flags	flags;				/* See above.						*/
	void*		dn_drvr;			/* Saved-off down driver.			*/
	/************************* IP Address Info **************************/
	u_int32		loc_ip_addr;		/* Local IP Address used			*/
	u_int32		rem_ip_addr;		/* Remote IP Address used			*/
	u_int32		orig_loc_ip_addr;	/* Original Local IP Address used	*/
	u_int32		orig_rem_ip_addr;	/* Original Remote IP Address used	*/
	/************************ Compression Info **************************/
	u_int16		rx_cproto;			/* Current IP Compression Protocol	*/
	u_int16		tx_cproto;			/* Current IP Compression Protocol	*/
	u_int8		rx_mslot;			/* Current IP Compression Max Slot	*/
	u_int8		tx_mslot;			/* Current IP Compression Max Slot	*/
	u_int8		rx_cslot;			/* Current IP Compression Comp Slot	*/
	u_int8		tx_cslot;			/* Current IP Compression Comp Slot	*/
	u_int8		rx_pfc;				/* Protocol field compression flag.	*/
	u_int8		tx_pfc;				/* Protocol field compression flag.	*/
	/************************ Lower Layer Info **************************/
	void*		pulled_drvr;		/* Path entry of driver we pulled.	*/
	u_int16		dn_txoffset;		/* txoffset of all lower drivers	*/
	u_int16		dn_txtrailer;		/* txtrailer of all lower drivers 	*/
	/*********************** Track the State Machine ********************/
	event_id	ev_curr_state;		/* Value of event is current state	*/
	event_id	ev_last_event;		/* Value of event is last event		*/
	/*********************** State Machine Info *************************/
	u_int32		curr_state;			/* Current state					*/
	process_id	wake_pr;			/* process id to be woken up		*/
	mbufList    tx_msg_list;        /* List of transmitted messages.    */
	Mbuf        rx_msg;             /* Last received mbuf chain         */
	Mbuf        resp_mb;            /* mbuf to be sent for response     */
	u_int8		unique_id;			/* unique id for packets			*/
	u_int8		counter;			/* counter for retransmissions		*/
	timer_pb	timer;				/* Timer as defined in RFC 1661		*/
	Ipcp_state	smt;				/* State Machine Table				*/
};


#define DBG_PTR					(lustat->lu_dbg)
#define IP_DBG					(lustat->dbg_ip)



/****************************************************************************/
/*  Logical Unit Specific Static Storage									*/
/****************************************************************************/

#define SPF_LUSTAT \
	/************************ Character Strings *************************/\
	char		desc_name[MAX_NAME_LEN];	/* Name of descriptor		*/\
	char		interface_name[MAX_NAME_LEN];	/* Name of interface	*/\
	char		ll_pathdesc[MAX_NAME_LEN];	/* Lower layer path string	*/\
	/************************ Debug Mod Pointer *************************/\
	void*		lu_dbg;				/* debugging pointer				*/\
	void*		dbg_ip;				/* debugging pointer				*/\
	/************************ Driver Parameters *************************/\
	u_int32		ppp_mode;			/* Mode of operation flags			*/\
	u_int32		timeout;			/* timeout value in milliseconds	*/\
	u_char		scr_max;			/* Max retransmit for config msg	*/\
	u_char		str_max;			/* Max retransmit for term msg		*/\
	u_int16		comp_protocol;		/* Desired IP Compression Protocol	*/\
	u_int8		max_slot_id;		/* Desired IP Compression Max Slot	*/\
	u_int8		comp_slot_id;		/* Desired IP Compression Comp Slot	*/\
	u_int8		ipcp_accept_remote; /* Let peer specify its address  	*/\
	u_int8		ipcp_accept_local;  /* Let peer specify our address  	*/\
	Spf_ppstat	pp_list;			/* Per-path storage list			*/\
	Ipcp_lustat	rtp;				/* Run-time parameters				*/\
	int32       def_route;			/* Default Route Flag				*/\
	struct		if_data if_data;	/* BSD interface stats              */ \
	char		symbol1[16];		/* name of the first symbol         */ \
	struct nlist ppp_symbols[2];	/* symbol returned via spf_gs_symbols */

/* The initialization values for the logical unit storage */

#define SPF_LUSTAT_INIT \
	DESC_NAME,						/* desc_name						*/\
	INTERFACE_NAME,					/* interface_name					*/\
	LOWER_LAYER_PATH,				/* ll_pathdesc						*/\
	NULL,							/* lu_dbg							*/\
	NULL,							/* dbg_ip							*/\
	PPP_MODE,						/* mode								*/\
	TIMEOUT,						/* timeout							*/\
	MAX_SCR,						/* scr_max							*/\
	MAX_STR,						/* str_max							*/\
	TCP_COMPRESSION,				/* comp_protocol					*/\
	MAX_SLOT_ID,					/* max_slot_id						*/\
	COMP_SLOT_ID,					/* comp_slot_id						*/\
	IPCP_ACCEPT_REMOTE,				/* ipcp_accept_remote				*/\
	IPCP_ACCEPT_LOCAL,				/* ipcp_accept_local				*/\
	NULL,							/* pp_list							*/\
	NULL,							/* rt_params						*/\
	DEFAULT_ROUTE,					/* Default Route Flag               */\
	{               /* BSD if_data stucture */ \
		IFT_PPP,    /* Interface type */ \
		0,          /* Interface address length */ \
		0,          /* Interface header length */ \
		0,          /* MTU */ \
		0,          /* routing metric */ \
		0,          /* Interface speed */ \
		0,0,0,0,    /* In pkt, In err, Out pkt, Out err */ \
		0,0,0,0,0,  /* Coll, In byt, Out byt, In Mul, Out Mul */ \
		0,0,        /* In drop, No Proto */ \
		{0}         /* LastChange up time when operational */ \
	},\
	{"bsd_if_data"},            /* symbol1 string */\
	{\
		{&my_lu.lucom.symbol1[0],   /* address of name of symbol 1*/ \
		(unsigned long) &my_lu.lucom.if_data, /* address of stats */ \
		N_ABS},                     /* indicate absolute address */ \
		{0, 0, 0}                   /* null nlist termination */ \
	}

#define SPF_DRVR	/* to include driver specific portions in spf.h */

#if defined(DRIVER)
	#include <SP_PPP/DEFS/per_path.h>
#endif /* DRIVER */

#include <SPF/spf.h>

#include <SP_PPP/IPCP/history.h>

/* SPF sucks in that it doesn't restore the pathdesc!!! */
#undef SMCALL

#if defined(DRIVER)
	#include <SP_PPP/IPCP/proto.h>
#endif /* DRIVER */

#endif /* _defs_h_ */
