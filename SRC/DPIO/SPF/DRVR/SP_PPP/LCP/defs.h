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
 *   1 01/19/96 Saw the light of day                                   udayn
 *     03/19/97 Removed spinlib structures and header file.            sr
 *  17 04/30/97 Clean up code.                                         sr
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  25 10/08/97 Adopted changes Bill W. made to Beta PPP code          tmd
 *     10/17/97 Default PPP_MODE is Active Open.                       sr
 *  27 11/28/97 Added fields to lcp_lustat for authentication          tmd
 *  30 03/11/98 Changed AUTH_TIMEOUT from 3000 to 3 seconds            tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#ifndef _LCP_DEFS_H
#define _LCP_DEFS_H

/* DPIO definitions */
#ifdef _OSK
#include <defconv.h>
#endif

/****************************************************************************
 * System definitions and include files
 */

#include <const.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <events.h>
#include <memory.h>
#include <string.h>
#include <sg_codes.h>
#include <modes.h>
#include <module.h>
#include <process.h>
#include <io.h>
#include <types.h>
#include <SPF/spf.h>
#include <SPF/timer.h>

#include <SPF/mbuf.h>
#include <SPF/ppp.h>
#include <SPF/ppp_auth.h>

#include "SP_PPP/DEFS/msg_list.h"

#if defined(DRIVER)
	#include "alt_dbg.h"
	#include "lib_func.h"
	#include "notify.h"
	#include "nxt_drvr.h"
	#include "chap_md5.h"
#endif	/* DRIVER */


/****************************************************************************
 * States for LCP as per RFC 1661
 */
#define MAX_STATES			10

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

/****************************************************************************
 * Events for LCP as per RFC 1661
 */
#define MAX_EVENTS		15

#define NO_EVENT		-1
#define EVENT_UP		0		/* lower layer is up 	*/
#define EVENT_DOWN		1		/* lower layer is down  */
#define EVENT_OPEN		2		/* this layer is opened */
#define EVENT_CLOSE		3		/* this layer is closed */
#define EVENT_TM0		4		/* Timeout with counter value = 0 */
#define EVENT_TM1		5		/* Timeout with counter value = 1 */
#define EVENT_RCR_GOOD  6		/* recvd a good configure request message */
#define EVENT_RCR_BAD   7		/* recvd unacceptable configure req message */
#define EVENT_RCA		8		/* receive configure ack */
#define EVENT_RCN		9		/* receive configure nak */
#define EVENT_RTR		10		/* receive terminate request */
#define EVENT_RTA		11		/* receive terminate ack */
#define EVENT_RUC		12		/* receive unknown code */
#define EVENT_RXJ_BAD   13		/* receive code reject (catastrophic) */
#define EVENT_RXR		14		/* receive echo request */


/****************************************************************************
 * LCP assigned types
 */
#define CONFIGURE_REQUEST	1
#define CONFIGURE_ACK		2
#define CONFIGURE_NAK		3
#define CONFIGURE_REJECT	4
#define TERMINATE_REQUEST	5
#define TERMINATE_ACK		6
#define CODE_REJECT         7
#define PROTOCOL_REJECT		8
#define ECHO_REQUEST		9
#define ECHO_REPLY          10
#define DISCARD_REQUEST		11


/****************************************************************************
 * LCP assigned configuration types
 */
#define MRU				1	/* Max receive unit */
#define ACCM			2	/* Async control character map */
#define AUTH_PROTOCOL	3	/* Authentication protocol */
#define QA_PROTOCOL		4	/* Quality protocol */
#define MAGIC_NUMBER	5	/* magic number */
#define PFC				7	/* protocol field compression */
#define ACFC			8	/* address and control field compression */

/****************************************************************************
 * LCP  assigned configuration defaults
 */

#define MRU_DEFAULT		1500
#define ACCM_DEFAULT	0xffffffff
#define AP_DEFAULT		CHAP
#define APA_DEFAULT		5		/* MD5 */
#define MAX_SCR_DEFAULT 10
#define MAX_STR_DEFAULT 2
#define TIMER_DEFAULT   3000 	/* 3000 milliseconds */


/********************************************************************/
/*********************** PAP/CHAP definitions ***********************/

#define MAX_AUTH_ID			MAX_PEER_NAME /* Max length of PAP Peer ID	*/
#define MAX_AUTH_PASSWORD	MAX_SECRET	/* Max length of PAP Peer Password	*/

#define MIN_CHALL_LEN       32  /* minimum length of a challenge */
#define MAX_CHALL_LEN       48  /* maximum length of a challenge */

#define PAP_AUTH_REQ		1	/* Authenticate-Request */
#define PAP_AUTH_ACK		2	/* Authenticate-Ack		*/
#define PAP_AUTH_NAK		3	/* Authenticate-Nak		*/

#define CHAP_CHALLENGE		1
#define CHAP_RESPONSE		2
#define CHAP_SUCCESS		3
#define CHAP_FAILURE    	4

/*
 * PAP/CHAP Client states.
 */

#define CS_NOT_AUTHENT		0x00	/* Not doing authentication			*/

#define PPP_NO_AUTH			0x00	/* Not doing authentication.		*/
#define PPP_PAP				0x10	/* Base code for PAP.				*/
#define PPP_CHAP			0x20	/* Base code for CHAP.				*/

#define CS_PAP_OPEN		(PPP_NO_AUTH)	/* We've received an Ack			*/
#define CS_PAP_INITIAL	(PPP_PAP + 0)	/* Connection down					*/
#define CS_PAP_CLOSED	(PPP_PAP + 1)	/* Conn. up, haven't requested auth */
#define CS_PAP_PENDING	(PPP_PAP + 2)	/* Conn. down, have requested auth	*/
#define CS_PAP_AUTHREQ	(PPP_PAP + 3)	/* We've sent an Auth-Request		*/
#define CS_PAP_BADAUTH	(PPP_PAP + 4)	/* We've received a Nak				*/

#define CS_CHAP_INITIAL	0x20	/* Connection down					*/

/*
 * PAP/CHAP Server states.
 */

#define SS_NOT_AUTHENT		0x00	/* Not doing authentication			*/
#define SS_PAP_OPEN			0x00	/* We've sent an Ack				*/
#define SS_PAP_INITIAL		0x10	/* Connection down					*/
#define SS_PAP_CLOSED		0x11	/* Conn. up, haven't requested auth	*/
#define SS_PAP_PENDING		0x12	/* Conn. down, have requested auth	*/
#define SS_PAP_LISTEN		0x13	/* Listening for an Authenticate	*/
#define SS_PAP_BADAUTH		0x14	/* We've sent a Nak					*/

#define SS_CHAP_INITIAL		0x20	/* Connection down					*/


/********************************/
/* PAP Timeouts (in seconds).	*/
/********************************/

#define PAP_DEF_TIMEOUT	3	/* Timeout for retransmitting request.	*/
#define PAP_DEF_REQTIME	30	/* Time to wait for auth-req from peer.	*/

/********************************************************/
/* The macros below define all the paramters that can	*/
/* be set up on a per LCP descriptor basis.				*/
/********************************************************/

#ifndef RX_MRU
	#define RX_MRU		1500
#endif

#ifndef RX_PFC
	#define RX_PFC		1
#endif

#ifndef RX_ACFC
	#define RX_ACFC		1
#endif

#ifndef RX_ACCM
	#define RX_ACCM		0xffffffff
#endif

#ifndef TX_ACCM
	#define TX_ACCM		{ 0xffffffff, 0, 0, 0, 0, 0, 0, 0 }
#endif

#ifndef MAX_SCR
	#define MAX_SCR		10
#endif

#ifndef MAX_STR
	#define MAX_STR		2
#endif

#ifndef TIMEOUT
	#define TIMEOUT		3000	/* 3 seconds */
#endif

#ifndef PPP_MODE
	#define PPP_MODE	NO_WAIT_ON_OPEN
#endif

#ifndef AUTH_TIMEOUT
	#define AUTH_TIMEOUT	3   /* 3 seconds */
#endif

#ifndef AUTH_MAX_REQ
	#define AUTH_MAX_REQ	10
#endif

/******************** End of PAP/CHAP definitions *******************/
/********************************************************************/

/****************************************************************************
 *  driver specific entries for per path storage
 */

#define SPF_PPSTAT	\
	u_int32		up_protocol; \
	u_int32		dn_protocol; \
	u_int32		ppp_protocol; \
	u_int32		link_count;


#define MAX_ACTIONS 3

typedef error_code (*Action)(Dev_list);

typedef struct
{
	u_int8	next_state;
	Action	action[MAX_ACTIONS];

} lcp_entry, *Lcp_entry;

typedef struct
{
	lcp_entry	entry[MAX_EVENTS];

} lcp_state, *Lcp_state;


/**********************************************/
/*  SPF to PPP protocol id mapping structure. */
/**********************************************/

typedef struct
{
	u_int32		spf_protocol;
	u_int32		ppp_protocol;

} prot_map, *Prot_map;

#define PROTID_SIZE		2

/***********************************************************/
/* Protocol to network control protocol mapping structure. */
/***********************************************************/

typedef struct
{
	u_int32		protocol;
	u_int32		ncp;

} ncp_map, *Ncp_map;

#define NCP_SIZE		3


/*******************************************/
/* Run-time parameters for the LCP driver. */
/*******************************************/

typedef struct
{
	/*************************** LCP Variables **************************/
	u_int32		rx_acfc:1;		/* RX address/control field compress.	*/
	u_int32		tx_acfc:1;		/* TX address/control field comp.		*/
	u_int32		rx_pfc:1;		/* RX protocol field compress.			*/
	u_int32		tx_pfc:1;		/* TX protocol field compression.		*/
	/********************** Other operational flags *********************/
	u_int32		timer_on:1;		/* Timer is running.					*/
	u_int32		send_mru:1;		/* Send default MRU value.				*/
	u_int32		cfg_req_chg:1;	/* Parameters in Cfg Req have changed.	*/
	u_int32		max_timeouts:1;	/* We've used up all our timeouts.		*/

} lcp_flags, *Lcp_flags;


typedef struct
{
	lcp_flags	flags;				/* Binary flags - see above.			*/
	u_int16		dn_txoffset;		/* Space needed by stack below.			*/
	u_int16		rx_mru;				/* RX max receive unit.					*/
	u_int16		tx_mru;				/* TX max receive unit.					*/
	u_int32		rx_accm;			/* RX Async Control Character Map.		*/
	u_int32		tx_accm[TX_ACCM_SIZE];	/* TX Async Control Character Map.	*/
	u_int32		magic;				/* Magic number.						*/
	prot_map	protid[PROTID_SIZE];/* Protocol array mapping.				*/
	ncp_map		ncp[NCP_SIZE];		/* Protocol to ncp mapping.				*/
	process_id	wake;				/* PID of process to be woken up.		*/
	timer_pb	timer;				/* Timer pb for timer in RFC 1661.		*/
	mbufList    tx_msg_list;        /* List of transmitted messages.        */
	Mbuf        rx_msg;             /* Pointer to last received mbuf.       */
	Mbuf        resp_mb;            /* Mbuf to be sent for reject.          */
	u_int8		unique_id;			/* Unique id for packets.				*/
	u_int8		counter;			/* Counter for retransmissions.			*/
	event_id	evid;				/* Event id.							*/
	char		ev_name[16];		/* Event name.							*/
	Lcp_state	smt;				/* State machine table.					*/
	u_int8		curr_state;			/* Current state.						*/
	void*		dn_drvr;			/* Saved-off downdriver.				*/
	/***************************** PAP / CHAP variables *********************/
	Mbuf        auth_mb;                /* auth mbuf sent (used for retrans)*/
	u_int8      auth_id;                /* id of auth pkt sent out          */
	u_int8      challenge[MAX_CHALL_LEN];/* auth challenge sent             */
	u_int8      challenge_len;          /* length of above challenge        */
	u_int8		auth_client_state;		/* Client state.					*/
	u_int8		auth_transmits;			/* Number of auth-reqs sent.		*/
	u_int8		auth_server_algorithm;	/* Server algorithm.				*/
	u_int16		auth_server_protocol;	/* Server protocol.					*/
	Notify_entry notify_up;			/* Clients to notify when I/O enabled.	*/
	Notify_entry notify_down;		/* Clients to notify when I/O disabled.	*/
	/******************** Track the State Machine ***************************/
	event_id    ev_curr_state;	/* Value of event is current state.			*/
	event_id    ev_last_event;	/* Value of event is last event.			*/

} lcp_lustat, *Lcp_lustat;



#define IP_DBG			(lustat->dbg_ip)
#define DBG_PTR			(lustat->dbg)


#define SPF_LUSTAT  \
	void*		dbg;			/* Debugging pointer.						*/\
	void*		dbg_ip;			/* Debugging pointer.						*/\
	/************ LCP Parameters Per RFC 1661 *******************************/\
	u_int32		tx_accm[TX_ACCM_SIZE];	/* TX Async Control Character Map.	*/\
	u_int16		rx_mru;			/* Desired RX max receive unit.				*/\
	u_int32		rx_accm;		/* Async Control Character Map for RX.		*/\
	u_int8		rx_pfc;			/* Desired RX protocol field compression.	*/\
	u_int8		rx_acfc;		/* Desired RX address/control field comp.   */\
	u_int8		scr_max;		/* Maximum configure request retransmits.	*/\
	u_int8		str_max;		/* Maximum terminate request retransmits.	*/\
	u_int32		timeout;		/* Timeout value in milliseconds.			*/\
	u_int8		auth_timeout;			/* Timeout (seconds) for retrans.	*/\
	u_int8		auth_max_transmits;		/* Max. number of auth-reqs to send.*/\
	/************ Additional LCP Operational Parameters *********************/\
	u_int32		ppp_mode;		/* Mode of operation flags.					*/\
	void*		pp_list;			/* Per protocol list.					*/\
	Lcp_lustat	rtp;			/* Run-time parameters.						*/

/* The initialization values for the logical unit storage */

#define SPF_LUSTAT_INIT \
	NULL,			/* dbg					*/\
	NULL,			/* dbg_ip				*/\
	TX_ACCM,		/* tx_accm				*/\
	RX_MRU,			/* rx_mru				*/\
	RX_ACCM,		/* rx_accm				*/\
	RX_PFC,			/* rx_pfc				*/\
	RX_ACFC,		/* rx_acfc				*/\
	MAX_SCR,		/* scr_max				*/\
	MAX_STR,		/* str_max				*/\
	TIMEOUT,		/* timeout				*/\
	AUTH_TIMEOUT,	/* auth_timeout			*/\
	AUTH_MAX_REQ,	/* auth_max_transmits	*/\
	PPP_MODE,		/* ppp_mode				*/\
	NULL,			/* pp_list				*/\
	NULL			/* rtp					*/


#ifndef MD5_BUFFER_SIZE
	#define MD5_BUFFER_SIZE	64
#endif

#define SPF_DRSTAT \
	void*		dbg_chap;\
	void*		std_state_tbl; \
	void*		alt_state_tbl; \
	auth_handle	auth_module; \
	u_int8		md5_padding[MD5_BUFFER_SIZE];

#define SPF_DRSTAT_INIT \
		NULL,   /* dbg_chap */\
	NULL,	/* std_state_tbl */\
	NULL,	/* alt_state_tbl */\
	{0},	/* auth_module */\
{			/* md5_padding - 64 bytes*/\
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  \
}

/****************************************************************************
 *  Device Driver Static Specific Storage   -  none
 */

#define SPF_DRVR	/* to include driver specific portions in spf.h */

#if defined(DRIVER)
	#include "SP_PPP/DEFS/per_path.h"
#endif	/* DRIVER */

#include <SPF/spf.h>

#include <SP_PPP/LCP/history.h>

/* Don't like how SPF handles this, so change it to a function */
#undef SMCALL

#if defined(DRIVER)
	#include "proto.h"

#endif	/* DRIVER */

#endif /* _LCP_DEFS_H */
