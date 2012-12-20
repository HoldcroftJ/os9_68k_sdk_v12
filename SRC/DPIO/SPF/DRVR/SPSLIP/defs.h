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
 * 203 ??/??/?? Snarfed from SPSLIP from MWUK.                         sr
 * 205 12/26/96 Added more fields to lustat, for input and output      sr
 *              buffer pointers.
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 * 206 05/11/97 Added BSD statistics                                   tmd
 * 207 11/18/97 Use COMPRESS_FLAG to set lu_compress field.            sr
 * 209 02/04/98 Replaced cslip.h with compress.h                       rds
 * 210 02/28/98 Added output packet queueing                           rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 * 218 08/02/01	Set the event value of TERMINATE smaller       	       pengf
 ****************************************************************************/

/*
**	Header Files
*/

/* DPIO definitions */
#ifdef _OSK
#include <defconv.h>
#endif

/* System definitions */

#include <const.h>
#include <errno.h>
#include <events.h>
#include <semaphore.h>
#include <memory.h>
#include <string.h>
#include <types.h>

#include <nlist.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_types.h>
#include <netinet/in_systm.h>
#define EXTERN extern
#include <netinet/tcp.h>
#include <netinet/tcp_seq.h>
#include <netinet/ip.h>
#include <netinet/ip_var.h>
#include <netinet/tcpip.h>


#ifdef DEBUG
#include <dbg_mod.h>
#endif

#ifndef TRUE
#define TRUE					1
#endif
#ifndef FALSE
#define	FALSE					0
#endif

#define	TX_THREAD_TERMINATE		32000
#define RX_THREAD_TERMINATE		32001
#define	THREADISDEAD			32002

#define NTRY        8

#ifndef COMPRESS_FLAG
#define COMPRESS_FLAG	(1)	/* Compression On by default */
#endif

#ifndef MAX_OUTQLEN
#define MAX_OUTQLEN 50
#endif

#ifdef _CSLIP
#define RSVD_FOR_CMPR	128
#else
#define RSVD_FOR_CMPR	0
#endif

/*
 * Slip status structure : Compatible with the ISP
 */
#ifdef STATISTICS
typedef struct slipstat {
	u_int32		inp_bytes,		/* number of bytes read 					*/
				out_bytes,		/* number of bytes sent 					*/
				inp_packets,	/* number of IP frames received 			*/
				out_packets,	/* number of IP frames sent 				*/
				inp_errors,		/* input errors 							*/
				out_errors,		/* output errors 							*/
				waiting,		/* # times needed char but none avail 		*/
				sigsleep,		/* # sleeping on signals 					*/
				inp_mbufs,		/* slip_inp mbuf allocate failures 			*/
				inp_reopens,	/* input path reopens						*/
				out_reopens,	/* output path reopens 						*/
				runts,			/* packets received < sizeof(ip header) 	*/
				comphdr_rcvd,	/* # compressed header packets received		*/
				comphdr_sent,	/* # compressed header packets sent 		*/
				uncomphdr_rcvd,	/* # uncompressed header packets received	*/
				uncomphdr_sent;	/* # uncompressed header packets sent		*/
	u_int16		ip_inpmin,		/* smallest input IP datagram 				*/
				ip_inpmax,		/* largest input IP datagram 				*/
				ip_outmin,		/* smallest output IP datagram 				*/
				ip_outmax;		/* largest output IP datagram 				*/
} slipstat, *Slipstat;

	/* Macro to define statistics fields in the device static storage */
#define			DR_STATISTICS		slipstat	dr_slipstat; \

#define			DR_STATISTICS_INIT	{0}

#else	/* STATISTICS is undefined */
	/* No logical unit static storage required for statistics gathering		 */
#define			DR_STATISTICS

#define			DR_STATISTICS_INIT

#endif /* STATISTICS */

/* Are we being compiled for CSLIP support? */
#ifdef _CSLIP
#include "compress.h"
#define IP_MINSIZE  3			/* IP packet minimum size   */

#define DR_CSLIP	\
				slcompress	dr_cstable;		/* Compression table			*/\
				u_int16		dr_cstable_state;	/* TRUE == Initialised		*/\

#define DR_CSLIP_INIT	\
				{ 0 },						/* dr_cstable					*/\
				0,							/* dr_cstable_state				*/\

#else /* _CSLIP undefined, no TCP header compression required */
#define IP_MINSIZE  20			/* IP packet minimum size   */
#define DR_CSLIP
#define DR_CSLIP_INIT
#endif
/* spslip-Specific Driver Definitions */
#define SPF_DRSTAT \
	char		dr_demarc[8];	/* Demarc for driver specifics portion		*/\
	semaphore	dr_enqsem;		/* Semaphore for enqueue/dequeue of mbufs	*/\
	int			dr_outqlen;		/* Output queue length						*/\
	DR_CSLIP					/* CSLIP specific fields 					*/\
	DR_STATISTICS				/* statistics gathering fields 				*/\

/* Device-specific logical unit static fields
 * Must be defined before spf.h is included
 */
#if !defined(_LLSLIP)
#define SPF_LUSTAT \
	Dev_list	lu_sl_updrvr;		/* Updrvr for this instatnce of SLIP */\
	Spf_pdstat	lu_sl_uppath;		/* Path descriptor to THE updrvr */\
	u_int32		lu_path_cnt;		/* Count of open paths on this LU */\
	u_int8		lu_par_bits; 		/* parity/stopbits				*/\
	u_int8		lu_baud_rate;		/* baud rate					*/\
	u_int32		lu_inp_prior;		/* input process priority	*/\
	u_int32		lu_out_prior;		/* output process priority 	*/\
	Pr_desc		lu_inp_proc,		/* slip input thread 	*/\
				lu_out_proc;		/* slip output thread 	*/\
	char		lu_idevname[32],	/* serial device namestring 	*/\
				lu_odevname[32];	/* serial device namestring 	*/\
	path_id		lu_inp_path,		/* input path					*/\
			 	lu_out_path;		/* output path					*/\
	Mbuf		lu_outq;			/* outgoing mbuf queue 	*/\
	void		*lu_rx_spare;		/* transmit buffer tail pointer */\
	void		*lu_tx_spare;		/* transmit buffer head pointer */\
	event_id	lu_out_evid;		/* lu_outq event handle		*/\
	event_id	lu_inp_evid;		/* lu_outq event handle		*/\
	int			lu_out_death;		/* output thread death flag */\
	int			lu_inp_death;		/* input thread death flag	*/\
	char		lu_ev_inp_name[32];	/* input event name 		*/\
	char		lu_ev_out_name[32];	/* output event name 		*/\
	u_int32		lu_outbufsiz,		/* output buffer size		*/\
				lu_inbufsiz;		/* input buffer size	*/\
	u_char		*lu_rcvbuff,		/* ptr to raw byte rcv buffer 	*/\
				*lu_rcvbend,		/* End ptr to raw byte rcv buffer*/\
				*lu_rcvtail,		/* receive buffer tail pointer	*/\
				*lu_rcvhead;		/* receive buffer head pointer	*/\
	u_int32		lu_rcvbufsiz;		/* size of receive buffer 		*/\
	void*		lu_dbg;				/* debugging pointer */\
	char		lu_dbg_name[16];	/* Name of debug module */\
	char		lu_compress,		/* Header compression flag (1 = on)	*/\
				lu_rsv[3];			/* Long word boundary				*/\
	u_char      *lu_inbuf,          /* Input buffer                     */\
				*lu_outbuf;         /* Output buffer                    */\
	struct		if_data if_data;    /* BSD interface stats */ \
	char        symbol1[16];        /* name of the first symbol */ \
	struct nlist slip_symbols[2];	/* symbol returned via spf_gs_symbols */\
	int			lu_maxoutqlen;		/* number of output packets to queue */


#else /* _LLSLIP is defined */
#define SPF_LUSTAT \
		void		*v_rinfo;			/* ROM jump table pointer			*/
		void		*v_cons;			/* Low level console structure used	*/
		u_char		*v_outbuf,			/* scratchpad output buffer			*/
					*v_inbuf,			/* Scratchpad input buffer			*/
					*v_slipglobs;		/* Pointer to llslip globals		*/
		#ifdef DEBUG
			Dbg_stat dbg_stg;			/* pointer to debug storage area	*/
		#endif
		u_char		*v_cmnInbuf;		/* llisr buf ptr for cslip & slip	*/
		u_char		*v_rcvmbuf;			/* pointer to rcv mbuf				*/
		u_short		v_llisrRcvNdx;		/* index into mbuf during LLISR		*/
#endif /* _LLSLIP */


#ifndef DEBUG_NAME
#define DEBUG_NAME	"dbg_slip"
#endif

#if !defined(_LLSLIP)
#define SPF_LUSTAT_INIT \
	NULL,				/* lu_sl_updrvr */\
	NULL,				/* lu_sl_uppath */\
	0,					/* lu_use_cnt */\
	SL_PAR_BITS,		/* lu_par_bits */\
	SL_BAUD_RATE,		/* lu_baud_rate */\
	SL_IN_PRIOR,		/* lu_inp_prior */\
	SL_OUT_PRIOR,		/* lu_out_prior */\
	0,					/* lu_inp_proc */\
	0,					/* lu_out_proc */\
	I_DEV_NAME,			/* lu_idevname */\
	O_DEV_NAME,			/* lu_odevname */\
	0,					/* lu_inppath */\
	0,					/* lu_outpath */\
	NULL,				/* lu_outq */\
	NULL,				/* lu_tx_tail */\
	NULL,				/* lu_tx_head */\
 	(event_id)-1,		/* lu_out_evid */\
 	(event_id)-1,		/* lu_inp_evid */\
	0,					/* lu_out_death */\
	0,					/* lu_inp_death */\
	{0},				/* lu_ev_inp_name */\
	{0},				/* lu_ev_out_name */\
	OUTBUFSIZE,			/* lu_outbufsiz */\
	INBUFSIZE,			/* lu_inbufsiz */\
	NULL,				/* lu_rcvbuff */\
	NULL, 				/* lu_rcvbend */\
	NULL,				/* lu_rcvtail */\
	NULL,				/* lu_rcvhead */\
	SL_RCV_BUF_SIZ,		/* lu_rcvbufsiz */\
	NULL,				/* lu_dbg */\
	{DEBUG_NAME},		/* lu_dbg_name */\
	COMPRESS_FLAG,		/* lu_compress */\
	{0},				/* lu_rsv */\
	NULL,				/* lu_inbuf */\
	NULL,				/* lu_outbuf */\
	{                   /* BSD if_data stucture */ \
		IFT_SLIP,       /* Interface type */ \
		0,              /* Interface address length */ \
		0,              /* Interface header length */ \
		0,              /* Ethernet MTU */ \
		0,              /* routing metric */ \
		0,              /* Interface speed, 10Mbps  */ \
		0,0,0,0,        /* In pkt, In err, Out pkt, Out err */ \
		0,0,0,0,0,      /* Coll, In byt, Out byt, In Mul, Out Mul */ \
		0,0,            /* In drop, No Proto */ \
		{0}             /* LastChange up time when operational */ \
	},\
	{"bsd_if_data"},    /* symbol1 string */\
	{ /* nlist array */ \
		{&my_lu.lucom.symbol1[0],   /* address of name of symbol 1*/ \
		(unsigned long) &my_lu.lucom.if_data, /* address of stats */ \
		N_ABS},                     /* indicate absolute address */ \
		{0, 0, 0},                  /* null nlist termination */ \
	},\
	MAX_OUTQLEN,		/* Maximum output queue length (lu_maxoutqlen) */
#endif

/* SPF definitions */

#define SPF_DRVR		/* to include driver specific portions in spf.h */
#include <SPF/spf.h>

#include <SPSLIP/history.h>


/* end of file */

