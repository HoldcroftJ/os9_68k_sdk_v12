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
 *  01 02/22/96 Initial                                                billw
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *     06/23/97 Added CHAP With MD5 Algorithm define.                  sr
 *     10/10/97 Added def_route field in ppp_ipcp_p structure.         sr
 *              Also added the flag values.
 *  02 11/28/97 Added defines for TRUE & FALSE                         tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  03 02/25/99 Use scf.h definitions for baud rates, parity, wordsize rds
 *     02/25/99 Added new getstat and setstat codes for API version    rds
 ****************************************************************************/

#ifndef _ppp_h_
#define _ppp_h_


#include <SPF/spf.h>
#include <types.h>
#include <scf.h>


/*** Mode flags - mode is a 16 bit value ***/

#define NO_WAIT_ON_OPEN		0x0001	/* Don't wait for a connection on open. */
#define PASSIVE_OPEN		0x0002	/* Don't initiate a connection, let the	*/
									/* other side contact you.				*/
#define NOTIFY_ON_LINKUP	0x0004	/* Notify upper layers on link up.		*/
#define XPARENT_UPDATA_OK	0x0008	/* OK for HDLC to accept dr_updata.		*/
#define WAIT_FOR_MODEM		0x0010	/* Wait for a modem ready signal before	*/
									/* setting the lu_ioenabled flag.		*/
#define LOOPBACK_MODE		0x0020	/* Run driver in loopback mode.			*/

#define NO_RX_COMPRESS	0x0100	/* Don't allow server to compress.			*/
#define NO_TX_COMPRESS	0x0200	/* Don't allow client to compress.			*/
#define NO_PAP			0x0400	/* Don't allow PAP protocol.				*/
#define NO_CHAP			0x0800	/* Don't allow CHAP protocol.				*/
#define NO_PFC			0x1000	/* Don't allow Protocol Field Compression.	*/
#define NO_ACFC			0x2000	/* Don't allow Address/Control Field Compr.	*/



/*** PPP assigned protocol ids ***/

#define IP				0x21		/* IP protocol							*/
#define COMPRESSED_TCP	0x2d		/* Van Jacobsen compressed tcp			*/
#define TCP				0x2f		/* uncompressed tcp						*/
#define IPCP			0x8021		/* IP control protocol					*/
#define LCP				0xc021		/* Link Control							*/
#define PAP				0xc023		/* Password Authentication				*/
#define LINK_QUALITY	0xc025		/* Link quality							*/
#define CHAP			0xc223		/* Challenge Handshake authentication	*/

/* CHAP Algorithm values */
#define CHAP_MD5_ALGTHM 0x05		/* CHAP with MD5 Algorithm */


/*
** Definitions for baud rate, parity, word size and stop bits. These used
** to be defined here, but now we use the ones in scf.h. Unfortunately the
** names used here were different, so for compatibility we need to map
** the names.
** 
*/

#define BAUDRATE_50			BAUD50
#define BAUDRATE_75			BAUD75
#define BAUDRATE_110		BAUD110
#define BAUDRATE_134_5		BAUD134_5
#define BAUDRATE_150		BAUD150
#define BAUDRATE_300		BAUD300
#define BAUDRATE_600		BAUD600
#define BAUDRATE_1200		BAUD1200
#define BAUDRATE_1800		BAUD1800
#define BAUDRATE_2000		BAUD2000
#define BAUDRATE_2400		BAUD2400
#define BAUDRATE_3600		BAUD3600
#define BAUDRATE_4800		BAUD4800
#define BAUDRATE_7200		BAUD7200
#define BAUDRATE_9600		BAUD9600
#define BAUDRATE_19200		BAUD19200
#define BAUDRATE_31250		BAUD31250
#define BAUDRATE_MIDI		BAUDMIDI
#define BAUDRATE_38400		BAUD38400
#define BAUDRATE_56000		BAUD56000
#define BAUDRATE_64000		BAUD64000
#define BAUDRATE_57600		BAUD57600
#define BAUDRATE_115200		BAUD115200
#define BAUDRATE_230400		BAUD230400
#define BAUDRATE_460800		BAUD460800
#define BAUDRATE_921600		BAUD921600
#define BAUDRATE_76800		BAUD76800
#define BAUDRATE_153600		BAUD153600
#define BAUDRATE_307200		BAUD307200
#define BAUDRATE_614400		BAUD614400
#define BAUDRATE_1228800	BAUD1228800

#define BAUDRATE_EXTERN	EXTERNAL

#define NO_PARITY		NOPARITY
#define ODD_PARITY		ODDPARITY
#define EVEN_PARITY		EVENPARITY
#ifdef _OS9000
#define MARK_PARITY		MARKPARITY
#define SPACE_PARITY	SPACEPARITY
#else
#define MARK_PARITY		4
#define SPACE_PARITY	5
#endif /* _OS9000 */

#define WORD_SIZE_5		WORDSIZE5
#define WORD_SIZE_6		WORDSIZE6
#define WORD_SIZE_7		WORDSIZE7
#define WORD_SIZE_8		WORDSIZE8

#define STOP_BITS_1		ONESTOP
#define STOP_BITS_1P5	ONE_5STOP
#define STOP_BITS_2		TWOSTOP

#ifdef _OS9000
#define RTS_DISABLED	RTSDISABLED
#define RTS_ENABLED		RTSENABLED
#else
#define RTS_DISABLED	0		/* Don't use RTS/CTS control	*/
#define RTS_ENABLED		1		/* Use RTS/CTS control			*/
#endif /* _OS9000 */


/* Length for strings */

#define MAX_NAME_LEN	16

/* Length for temporary text buffers */

#define MAX_LINE_LEN	256

/* Size (# of 32 bit words) of transmit ACCM */

#define TX_ACCM_SIZE	8

/* Default size for Max Receive Unit */

#define MRU_DEFAULT		1500


#ifndef MIN
	#define MIN(a,b)	(a<b ? a : b)
#endif

#ifndef MAX
	#define MAX(a,b)	(a>b ? a : b)
#endif

#ifndef False
	#define False		0
#endif

#ifndef True
	#define True		1
#endif



/*** Structures for PPP specific Getstat / Setstat calls ***/

typedef struct ppp_sysparams_pb		ppp_sysparams_pb,	*Ppp_sysparams_pb;
typedef struct ppp_ss_flg			ppp_ss_flg,			*Ppp_ss_flg;
typedef struct ppp_port_pb			ppp_port_pb,		*Ppp_port_pb;
typedef struct ppp_notify_pb		ppp_notify_pb,		*Ppp_notify_pb;
typedef struct ppp_procs_pb			ppp_procs_pb,		*Ppp_procs_pb;
typedef struct notify_entry			notify_entry,		*Notify_entry;


typedef struct
{
	char		rx_dev_name[MAX_NAME_LEN];
	char		tx_dev_name[MAX_NAME_LEN];

	u_int8		baud_rate;
	u_int8		parity;
	u_int8		word_size;
	u_int8		stop_bits;
	u_int8		rts_enable;

	u_int32		rx_bufsize;
	u_int32		tx_bufsize;

} ppp_modem_p, *Ppp_modem_p;

/*
 * This structure is used to communicate the serial port
 * settings to and from the HDLC layer driver.
 */
typedef struct
{
	spf_ss_pb	spb;
	ppp_modem_p	val;

} ppp_modem_params, *Ppp_modem_params;

typedef struct
{
	u_int8		rx_acfc;	/* Address & control field compression. */
	u_int8		tx_acfc;
	u_int8		rx_pfc;		/* Protocol field compression. */
	u_int8		tx_pfc;
	u_int32		rx_mru;		/* Max Receive Unit. */
	u_int32		tx_mru;
	u_int32		rx_accm;	/* Async Control Character Maps. */
	u_int32		tx_accm[TX_ACCM_SIZE];

} ppp_lcp_p, *Ppp_lcp_p;

/*
 * This structure is used to communicate the LCP layer negotiated
 * settings to and from the LCP layer driver.
 */
typedef struct
{
	spf_ss_pb	spb;
	ppp_lcp_p	val;

} ppp_lcp_params, *Ppp_lcp_params;

typedef struct
{
	char		interface_name[MAX_NAME_LEN];
	u_int16		rx_comp_proto;	/* IP compression protocol (0=None)	*/
	u_int16		tx_comp_proto;
	u_int8		rx_comp_mslot;	/* IP compression max slot used		*/
	u_int8		tx_comp_mslot;
	u_int8		rx_comp_cslot;	/* IP compression comp Slot used	*/
	u_int8		tx_comp_cslot;
	u_int8		def_route;		/* Default route option */
	u_int8		accept_remote;	/* Let peer specify its address */
	u_int8		accept_local;	/* Let peer specify our address */

#define NO_DEFAULT_ROUTE 0x00
#define SET_DEFAULT_ROUTE 0x01
#define ROUTE_EXISTS -1

#if !defined(FALSE)
	#define FALSE 0
#endif

#if !defined(TRUE)
	#define TRUE 1
	/* Should that instead be "#define TRUE (!FALSE)" */
#endif

} ppp_ipcp_p, *Ppp_ipcp_p;

/*
 * This structure is used to communicate the IPCP layer negotiated
 * settings to and from the IPCP layer driver.
 */
typedef struct
{
	spf_ss_pb	spb;
	ppp_ipcp_p	val;

} ppp_ipcp_params, *Ppp_ipcp_params;

typedef struct
{
	u_int32		ppp_mode;	/* Mode flags. See ppp.h for details.	*/
	u_int8		max_scr;	/* Variable defined by RFC 1661.		*/
	u_int8		max_str;	/* Variable defined by RFC 1661.		*/
	u_int32		timeout;	/* Variable defined by RFC 1661.		*/

} ppp_sys_p, *Ppp_sys_p;

/*
 * This structure is used by the PPP drivers to communicate operating
 * parameter values between the layers.
 */
typedef struct
{
	spf_ss_pb	spb;
	ppp_sys_p	val;

} ppp_sys_params, *Ppp_sys_params;


/*
 * This structure is used by application(s) to tell the HDLC driver
 * it may use the serial port, and if there is a modem link up.
 */
typedef struct
{
	spf_ss_pb	spb;
	u_int32		port_ready;

} ppp_port_rdy, *Ppp_port_rdy;



/***************** Obsolete ********************/

struct ppp_ss_flg
{
	u_int32	ipc_proto		: 1;
	u_int32	ipc_mslot		: 1;
	u_int32	ipc_cslot		: 1;
	u_int32	rx_acfc			: 1;
	u_int32	tx_acfc			: 1;
	u_int32	rx_pfc			: 1;
	u_int32	tx_pfc			: 1;
	u_int32	rx_accm			: 1;
	u_int32	tx_accm			: 1;
	u_int32	rx_mru			: 1;
	u_int32	tx_mru			: 1;
	u_int32	mode			: 1;
	u_int32	rx_dev_name		: 1;
	u_int32	tx_dev_name		: 1;
	u_int32	baud_rate		: 1;
	u_int32	parity			: 1;
	u_int32	word_size		: 1;
	u_int32	stop_bits		: 1;
	u_int32	rts_enable		: 1;
	u_int32	rx_bufsize		: 1;
	u_int32	tx_blocksize	: 1;
	u_int32	max_scr			: 1;
	u_int32	max_str			: 1;
	u_int32	timeout			: 1;
};

struct ppp_sysparams_pb
{
	spf_ss_pb	spb;
	ppp_ss_flg	flg;

	/*** TCP/IP Header Compression Variables ***/

	u_int16		ipc_proto;	/* IP compression protocol (0=None)	*/
	u_int8		ipc_mslot;	/* IP compression max slot used		*/
	u_int8		ipc_cslot;	/* IP compression comp Slot used	*/

	/*** LCP & IPCP Compression Variables ***/

	u_int8		rx_acfc;	/* Address & control field compression */
	u_int8		tx_acfc;

	u_int8		rx_pfc;		/* Protocol field compression */
	u_int8		tx_pfc;

	/* Async Control Character Maps ***/

	u_int32		rx_accm;
	u_int32		tx_accm;

	/*** Max Receive Unit ***/

	u_int32		rx_mru;
	u_int32		tx_mru;

	/*** Mode Variable ***/

	u_int8		mode;

	/*** Rx & Tx device names ***/

	char		rx_dev_name[MAX_NAME_LEN];
	char		tx_dev_name[MAX_NAME_LEN];

	/*** Serial Port Settings ***/

	u_int8		baud_rate;
	u_int8		parity;
	u_int8		word_size;
	u_int8		stop_bits;
	u_int8		rts_enable;

	/*** Receive buffer size ***/

	u_int32		rx_bufsize;

	/* Transmit blocksize limit ***/

	u_int32		tx_blocksize;

	/*** Timers and counters ***/

	u_int8		max_scr;
	u_int8		max_str;
	u_int32		timeout;
};

struct ppp_port_pb
{
	spf_ss_pb	spb;
	u_int32		port_ready;
};

struct ppp_notify_pb
{
	spf_ss_pb	spb;
	process_id	proc_id;
	signal_code	signal;
};

#define PPP_MAX_PROCS	10

struct ppp_procs_pb
{
	spf_ss_pb	spb;
	process_id	proc_id[PPP_MAX_PROCS];
	signal_code	signal[PPP_MAX_PROCS];
};

struct notify_entry
{
	Notify_entry	next,
					prev;
	process_id		proc_id;
	signal_code		signal;
};


#define PPP_BASE	(SPF_PR_PPP_IPCP << 16)

/* 0x01 thru 0x80 for Getstat calls */

#define PPP_GS_SYSPARAMS		(PPP_BASE + 0x01)
#define PPP_GS_USRPARAMS		(PPP_BASE + 0x02)
#define PPP_GS_NOTIFY_UP		(PPP_BASE + 0x04)
#define PPP_GS_NOTIFY_DOWN		(PPP_BASE + 0x05)
#define PPP_GS_MODEM_PARAMS		(PPP_BASE + 0x06)
#define PPP_GS_IPCP_PARAMS		(PPP_BASE + 0x07)
#define PPP_GS_LCP_PARAMS		(PPP_BASE + 0x08)
#define PPP_GS_NUM_NCP			(PPP_BASE + 0x09)
#define PPP_GS_PARAMS			(PPP_BASE + 0x0a)
#define PPP_GS_STATS			(PPP_BASE + 0x0b)
#define PPP_GS_OPTIONS			(PPP_BASE + 0x0c)


/* 0x81 thru 0xff for Setstat calls */

#define PPP_SS_SYSPARAMS		(PPP_BASE + 0x81)
#define PPP_SS_USRPARAMS		(PPP_BASE + 0x82)
#define PPP_SS_PORT_SETUP		(PPP_BASE + 0x84)
#define PPP_SS_PORT_DONE		(PPP_BASE + 0x85)

/* Tell lower layer we need it. */
#define PPP_SS_CONNECT			(PPP_BASE + 0x86)

/* Tell lower layer we are done. */
#define PPP_SS_DISCONNECT		(PPP_BASE + 0x87)

#define PPP_SS_NOTIFY_UP		(PPP_BASE + 0x88)
#define PPP_SS_NOTIFY_DOWN		(PPP_BASE + 0x89)
#define PPP_SS_MODEM_PARAMS		(PPP_BASE + 0x8a)
#define PPP_SS_IPCP_PARAMS		(PPP_BASE + 0x8b)
#define PPP_SS_LCP_PARAMS		(PPP_BASE + 0x8c)
#define PPP_SS_HDLC_MODE		(PPP_BASE + 0x8d)
#define PPP_SS_PARAMS			(PPP_BASE + 0x8e)
#define PPP_SS_RESET_STATS		(PPP_BASE + 0x8f)

#define PPP_SS_SET_CONNINFO		(PPP_BASE + 0x90)
#define PPP_SS_START_LCP		(PPP_BASE + 0x91)
#define PPP_SS_STOP_LCP			(PPP_BASE + 0x92)
#define PPP_SS_OPTIONS			(PPP_BASE + 0x93)


/* Mode values for HDLC_MODE setstat... */
#define PPP_HDLC_MODE_LIMBO		0
#define PPP_HDLC_MODE_CHAT		1
#define PPP_HDLC_MODE_FRAME		2


#endif /* _ppp_h_ */
