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
 ****************************************************************************/

#include <SPF/item.h>

#ifdef tcp0
/***************************************************************************/
/****               Device Descriptor for SPF sptcp driver              ****/
/****                                                                   ****/
/****    This section contains the configurable parameters for sptcp    ****/
/***************************************************************************/

/*
** By default all TCP sockets will use this a the window size. This may
** also be set on a per socket basis using the SO_RCVBUF socket option.
** Valid settings range from 1 to 64K.
*/
#define TCPWINDOW 0x4000

/*
** By default all TCP sockets will buffer up to this much data waiting
** to be ACK'ed. This may also be set on a per socket basis using the
** SO_SNDBUF socket option. Valid settings range from 1 to MAXSOCKBUF.
** MAXSOCKBUF is defined in the spip descriptor and defaults to 256K.
*/
#define SENDBUFFER 0x5000

/*
** Normally sptcp will use the MTU of the outgoing interface as the
** maximum segment size. In unusual circumstances where this can not
** be determined this value is used.
*/
#define DFLT_MSS 512

/*
** RFC 1323 defines two methods for increasing throughput, particularly
** on networks with large bandwidth-delay products. These TCP options
** are window scaling and timestamps. If the DO_RFC1323 option is
** enabled (default) these options are requested when initiating any
** TCP connection. If the other side does not understand the options
** they are not used even if DO_RFC1323 is enabled. Valid settings are
** 1 to enable, and 0 to disable.
*/
#define DO_RFC1323 1

/*
** This value is only used if a protocol is pushed on top of sptcp
**
** The maximum size of an mbuf allocated by SPF is controlled with this
** variable. A single write of more than this amount of data will be
** delivered to sptcp in multiple mbuf's. Valid settings range from
** 1 to 0xFFFF.
*/
#define TXSIZE 4380

/*
** This value is only used if a protocol is pushed on top of sptcp
**
** SPF will initiate flow control on a path when its read queue exceeds
** this threshold. Once flow control is enabled sptcp will buffer data
** up to the paths tcp window size. When the application reads data
** and the queue falls below this value all buffered data in sptcp is
** delivered to SPF.
*/
#define READSZ 4096

/***************************************************************************/
/****            Nothing below this line should be changed              ****/
/***************************************************************************/
#define DRV_NAME	"sptcp"		/* device driver name */

/* Macros that initialize device descriptor common fields */
#define PORTADDR	0			/* Base address of hardware */
#define LUN			0x00		/* logical unit number */
#define MODE		S_ISIZE | S_IREAD | S_IWRITE	/* device mode */
#define LUFLAGS		LU_UIOWRITE | LU_UIOREAD

/* Macros for hardware drivers */
#define VECTOR		0
#define IRQLEVEL	0
#define PRIORITY	0

/* Macros that initialize dd_popts structure of the device descriptor */
#define BLOCKTIME	0
#define ASYNCFLAG	0
#define PKTFLAG		IO_PACKET
#define WRITESZ		0

/* Macros that set the logical unit generic sections in spf.h */
#define TXOFFSET	20			     	/* TCP header size */
#define TXTRAILER	0
#define PATH_HOLDONCLOSE	PATH_HOLD

#define CTL_FLAGS	0
#define PROTTYPE	SPF_PR_TCP

/*
** Define the ITEM default values. All possible values to initialize these
** macros to can be found in item.h in the device_type and addr_type
** structures
*/
#define ITE_NETYPE_IN		ITE_NET_CTL
#define ITE_NETYPE_OUT		ITE_NET_CTL
#define ITE_CALL_STATE		ITE_CS_IDLE
#define OURADDR_CLASS		ITE_ADCL_INET
#define OURADDR_SUBCLASS	ITE_ADSUB_NONE
#define OURADDR_SIZE		16				/* sizeof(sockaddr_in) */
#define OURADDR				0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
#define THEIRADDR_CLASS		ITE_ADCL_INET
#define THEIRADDR_SUBCLASS	ITE_ADSUB_NONE
#define THEIRADDR_SIZE		16				/* sizeof(sockaddr_in) */
#define THEIRADDR			0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

/* Macros that set logical units specific information in SPTCP/defs.h     */

#include <SPTCP/defs.h>

#endif /* tcp0 *************************************************************/
