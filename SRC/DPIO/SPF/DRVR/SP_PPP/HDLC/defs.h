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
 *  01 03/06/96 Initial                                                billw
 *  16 04/17/97 First cut at threading.                                sr
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  22 10/07/97 If zeroed _OS_ macros.  Use dpio functions.            sr
 *  24 01/19/98 Fixed to avoid compiler warning messages               tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#ifndef _defs_h_
#define _defs_h_

#ifdef _OSK

	#include <defconv.h>
#if 0
	/* Macros for OS9 specific system calls. */

	#define _OS_EV_WAIT(id, vp, sp, lo, hi)		_os9_ev_wait(id, vp, lo, hi)
	#define _OS_SRQMEM(sizp, memp, color)		_os9_srqmem(sizp, memp)
	#define _OS_SLEEP(tickp, sigp)				_os9_sleep(tickp)
#endif
#else

	#if defined(_OS9000)
		#define MAX_SIGS 16

		#if defined(_MPF386)
			#include <reg386.h>
		#elif defined(_MPF68020)
			#include <reg68k.h>
		#elif defined(_MPFPOWERPC)
			#include <regppc.h>
		#endif
	#endif

	/* Macros for OS9000 specific system calls. */

 	#define _OS_EV_WAIT(id, vp, sp, lo, hi)		_os_ev_wait(id, vp, sp, lo, hi)
	#define _OS_SRQMEM(sizp, memp, color)		_os_srqmem(sizp, memp, color)
	#define _OS_SLEEP(tickp, sigp)				_os_sleep(tickp, sigp)

#endif



#include <const.h>
#include <errno.h>
#include <events.h>
#include <io.h>
#include <memory.h>
#include <modes.h>
#include <module.h>
#include <process.h>
#include <scf.h>
#include <sg_codes.h>

#if defined(_OSK)
	#include <sgstat.h>
#endif	/* _OSK */

#include <signal.h>
#include <stddef.h>
#include <string.h>
#include <sysglob.h>
#include <types.h>

#include <SPF/mbuf.h>
/* #include <INET/sysdefs.h> */

#include <SPF/spf.h>
#include <SPF/ppp.h>
#include <SPF/ppp_scf.h>

#if defined(DRIVER)
	#include "dbg_defs.h"
	#include "notify.h"
	#include "lib_func.h"
	#include "nxt_drvr.h"
#endif	/* DRIVER */


#if 0
#if defined(_OSK)
	typedef struct scf_opt	scf_opt;
#endif

/************************************/
/* Structure for SCF device options */
/************************************/

typedef struct device_opts	device_opts, *Device_opts;

struct device_opts
{
	path_id			pd;
	u_char			dev_type;

	u_int8			baud_rate;			/* (1) Serial port baud rate	*/
	u_int8			parity;				/* (2) Serial port parity		*/
	u_int8			word_size;			/* (3) Serial port word size	*/
	u_int8			stop_bits;			/* (4) Serial port stop bits	*/
	u_int8			rts_enable;			/* (5) Serial port RTS control	*/

#if defined(_OSK)

	scf_opt			path_opts;

#elif defined(_OS9000)

	scf_path_opts	path_opts;
	scf_lu_opts		lu_opts;

#endif
};
#endif




/* Default values for HDLC descriptors: */

#ifndef RX_DEVICE_NAME
	#define RX_DEVICE_NAME			"/t1"
#endif

#ifndef TX_DEVICE_NAME
	#define TX_DEVICE_NAME			RX_DEVICE_NAME
#endif

#ifndef BAUD_RATE
	#define BAUD_RATE				BAUDRATE_38400
#endif

#ifndef PARITY
	#define PARITY					NO_PARITY
#endif

#ifndef WORD_SIZE
	#define WORD_SIZE				WORD_SIZE_8
#endif

#ifndef STOP_BITS
	#define STOP_BITS				STOP_BITS_1
#endif

#ifndef RTS_ENABLE
	#define RTS_ENABLE				RTS_ENABLED
#endif

#ifndef TX_BLOCKSIZE
	#define TX_BLOCKSIZE			0
#endif

#ifndef RECEIVE_BUF_SIZE
	#define RECEIVE_BUF_SIZE		100
#endif

#ifndef PPP_MODE
	#define PPP_MODE				WAIT_FOR_MODEM
#endif

#ifndef SENDSIG_DELAY
	#define SENDSIG_DELAY			0
#endif

#ifndef DEBUG_MOD_SIZE
	#define DEBUG_MOD_SIZE			0x2000
#endif

#ifndef HACK_O_RAMA
#define HACK_O_RAMA 0
#endif



/* Signal codes */

#define RX_INPUT_READY		10001
#define DCDL_TRUE			10002
#define DCDL_FALSE			10003
#define THREAD_DONE			10004
#define HDLC_WAKEUP			10005


/* Names of Driver, threads, and startup events */

#define DRV_NAME		"sphdlc"

#define RX_IN_PRIOR		128
#define TX_IN_PRIOR		128


/* Receive states -- Do *NOT* reorder these! */

#define RX_NOT_STARTED				0
#define RX_MODEM_SETUP				1
#define RX_NEED_START_FLAG			2
#define RX_NEED_ADDR_FLAG			3
#define RX_NEED_CTRL_FLAG			4
#define RX_NEED_STOP_FLAG			5
#define RX_FRAME_COMPLETE			6
#define RX_FIRST_DATABYTE			7	/* For IP */

/* Transmit states -- Do *NOT* reorder these! */

#define TX_IDLE						0
#define TX_MODEM_SETUP				1
#define TX_BUSY						2

/* Other defines */

#define XMIT_PORT 1
#define RCV_PORT  0
#define  THREADISDEAD 555
/**** Run-time parameters for logical unit static storage ****/

typedef struct
{
	/********* Rx Thread Flags ********/
	u_int32		rx_acfc				: 1;
	u_int32		reserved_rx			: 4;
	u_int32		start_rxthread		: 1;
	u_int32		stop_rxthread		: 1;
	u_int32		kill_rxthread		: 1;
	/********* Tx Thread Flags ********/
	u_int32		tx_acfc				: 1;
	u_int32		tx_link_msg			: 1;
	u_int32		tx_msg_started		: 1;
	u_int32		reserved_tx			: 2;
	u_int32		start_txthread		: 1;
	u_int32		stop_txthread		: 1;
	u_int32		kill_txthread		: 1;

} hdlc_flags, *Hdlc_flags;

typedef struct hdlc_lustat		hdlc_lustat, *Hdlc_lustat;

struct hdlc_lustat
{
	/************** Driver Parent Parameters ********************************/
	u_int32		lu_id;				/* ID number for this lustat			*/
	process_id	drv_pid;			/* Driver process ID					*/
	Notify_entry notify_up;			/* Clients to notify when I/O enabled.	*/
	Notify_entry notify_down;		/* Clients to notify when I/O disabled.	*/
	/*************************** Thread Flags *******************************/
	hdlc_flags	flags;
	/************** Receive Thread Parameters *******************************/
	mh_com*		rx_drv_mod;			/* Driver module						*/
	path_id		rx_path;			/* Input device path					*/
	process_id	rx_pid;				/* Receive thread process ID			*/
	event_id	rx_ev_id;			/* Receive thread event ID				*/
	signal_code	rx_signal;			/* Receive thread kill signal			*/
	u_int8		rx_last_char;		/* Holds last byte processed			*/
	u_int32		rx_raw_ctr;			/* Bytes currently in buffer 			*/
	u_int32		rx_raw_size;		/* Size of the rxt_raw_buf buffer 		*/
	u_int32		rx_accm;			/* ACCM bits							*/
	u_int32		rx_frame_size;		/* Size of rxt_frame					*/
	Mbuf		rx_frame;			/* Incoming frame buffer				*/
	u_char*		rx_framep;			/* Pointer into frame buffer			*/
	u_char*		rx_raw_buf;			/* Incoming raw data buffer				*/
	u_char*		rx_raw_ptr;			/* Pointer into raw data				*/
	u_int16		rx_fcs;				/* Current FCS value.					*/
	u_int16		rx_state;			/* Current receive state.				*/
	u_int16		rx_mru;				/* Max receive unit.					*/
	device_opts	rx_dev_opts;		/* SCF device options					*/
	/************** Transmit Thread Parameters ******************************/
	mh_com*		tx_drv_mod;			/* Driver module						*/
	path_id		tx_path;			/* Output device path					*/
	process_id	tx_pid;				/* Transmit thread process ID			*/
	event_id	tx_ev_id;			/* Transmit thread event ID				*/
	u_int16		tx_fcs;				/* Current FCS value.					*/
	u_int16		tx_mru;				/* Max receive unit.					*/
	u_int16		tx_state;			/* Current transmit state.				*/
	Mbuf		tx_qhead;			/* Outgoing data queue -- head			*/
	Mbuf		tx_qtail;			/* Outgoing data queue -- tail			*/
	u_int32		tx_accm[TX_ACCM_SIZE];	/* ACCM bits						*/
	u_char*		tx_frame;			/* Outgoing frame buffer				*/
	u_char*		tx_framep;			/* Pointer into frame buffer			*/
	u_int32		tx_frame_len;		/* Byte count of frame buffer			*/
	u_int32		tx_frame_size;		/* Allocation size of frame buffer		*/
	u_int32		tx_frame_alloc;		/* Actual size of frame buffer			*/
	u_int32		tx_info_bytes;		/* Number of info bytes in frame buffer.*/
	u_int32		tx_pkt_ct;			/* Number of mbuf packets processed.	*/
	device_opts	tx_dev_opts;		/* SCF device options					*/
	/******************* Debugging Parameters *******************************/
#ifdef DEBUG_RX_ERRS
	#define D_BUF_LEN	1000
	u_int8		d_buf[D_BUF_LEN];
	u_int8*		d_ptr;
	u_int32		d_len;
	path_id		d_fp;
	path_id		d_ifp;
#endif
	void*		rx_dbg;				/* debug module pointer 				*/
	void*		tx_dbg;				/* debug module pointer 				*/
};

#define DBG_PTR			(lustat->dbg)
#define DBG_TX			(lustat->rtp->tx_dbg)
#define DBG_RX			(lustat->rtp->rx_dbg)
#define IP_DBG			(lustat->dbg_ip)

#define SPF_DRSTAT \
	char		drv_name[MAX_NAME_LEN];	/* Driver name						*/\
	u_int16*	fcs_table;				/* Lookup table for FCS calculations*/\
	u_int32		fcs_table_size;			/* Size of lookup table.			*/

#define SPF_DRSTAT_INIT \
	DRV_NAME,							/* drv_name							*/\
	NULL,								/* fcs_table						*/\
	0									/* fcs_table_size					*/

#define SPF_LUSTAT \
	/************** Debugging Parameters ************************************/\
	u_int32		dbg_mod_size;				/* (4) debug module size 		*/\
	void*		dbg;						/* (5) debugging pointer 		*/\
	void*		dbg_ip;						/* (6) debugging pointer 		*/\
	/*************** Input Device Parameters ********************************/\
	char		rx_dev_name[MAX_NAME_LEN];	/* (1) Input device name		*/\
	char		rx_ev_name[MAX_NAME_LEN];	/* Name of rcv event			*/\
	u_int32		rx_prior;			/* Receive thread priority				*/\
	Pr_desc		rx_proc;			/* Receive thread process desc			*/\
	/*************** Output Device Parameters *******************************/\
	char		tx_dev_name[MAX_NAME_LEN];	/* (1) Output device name		*/\
	char		tx_ev_name[MAX_NAME_LEN];	/* Name of xmit event			*/\
	u_int32		tx_prior;			/* Transmit thread priority				*/\
	Pr_desc		tx_proc;			/* Transmit thread process desc			*/\
	/**************** Serial Port Parameters ********************************/\
	u_int8		baud_rate;					/* (1) Serial port baud rate	*/\
	u_int8		parity;						/* (2) Serial port parity		*/\
	u_int8		word_size;					/* (3) Serial port word size	*/\
	u_int8		stop_bits;					/* (4) Serial port stop bits	*/\
	u_int8		rts_enable;					/* (5) Serial port RTS control	*/\
	/************** Receive Thread Parameters *******************************/\
	u_int32		rxt_raw_size;				/* (1) Size of rx buffer 		*/\
	/************** Transmit Thread Parameters ******************************/\
	u_int32		tx_blocksize;				/* (1) Max chars to transmit	*/\
	/************** HDLC Link Parameters ************************************/\
	int			sendsig_delay;				/* See rxthread.c				*/\
	u_int32		ppp_mode;					/* (1) Operating mode flags.	*/\
	Spf_ppstat	pp_list;					/* (2) List of opened paths		*/\
	Hdlc_lustat	rtp;						/* (3) Run-time parameters.		*/


#define SPF_LUSTAT_INIT \
	/************** Debugging Parameters ************************************/\
	DEBUG_MOD_SIZE,					/* (4) dbg_mod_size		 				*/\
	NULL,							/* (5) dbg				 				*/\
	NULL,							/* (6) dbg_ip			 				*/\
	/*************** Input Device Parameters ********************************/\
	RX_DEVICE_NAME,					/* (1) rx_dev_name						*/\
	{0},							/* Name of rcv event            		*/\
	RX_IN_PRIOR,					/* Receive thread priority				*/\
	NULL,							/* Receive thread process desc			*/\
	/*************** Output Device Parameters *******************************/\
	TX_DEVICE_NAME,					/* (1) tx_dev_name						*/\
	{0},							/* Name of xmit event					*/\
	TX_IN_PRIOR,					/* Transmit thread priority				*/\
	NULL,							/* Transmit thread process desc			*/\
	/**************** Serial Port Parameters ********************************/\
	BAUD_RATE,						/* (1) baud_rate						*/\
	PARITY,							/* (2) parity							*/\
	WORD_SIZE,						/* (3) word_size						*/\
	STOP_BITS,						/* (4) stop_bits						*/\
	RTS_ENABLE,						/* (5) rts_enable						*/\
	/************** Receive Thread Parameters *******************************/\
	RECEIVE_BUF_SIZE,				/* (1) rxt_raw_size						*/\
	/************** Transmit Thread Parameters ******************************/\
	TX_BLOCKSIZE,					/* (1) tx_blocksize						*/\
	/************** HDLC Link Parameters ************************************/\
	SENDSIG_DELAY,					/* sendsig_delay						*/\
	PPP_MODE,						/* (1) ppp_mode							*/\
	NULL,							/* (2) pp_list							*/\
	NULL							/* (3) rtp								*/

/* SPF definitions */

#define SPF_DRVR		/* to include driver specific portions in spf.h */

#if defined(DRIVER)
	#include <SP_PPP/DEFS/per_path.h>
#endif	/* DRIVER */

#include <SPF/spf.h>
#include <SP_PPP/HDLC/history.h>

#if defined(DRIVER)
	#include <SP_PPP/HDLC/proto.h>
#endif	/* DRIVER */

#undef SMCALL
error_code
SMCALL(Dev_list, Dev_list, error_code (*)(), void *);

#endif /* _defs_h_ */
