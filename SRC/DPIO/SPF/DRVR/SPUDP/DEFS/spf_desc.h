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

#ifdef udp0
/***************************************************************************/
/****               Device Descriptor for SPF spudp driver              ****/
/****                                                                   ****/
/****    This section contains the configurable parameters for spudp    ****/
/***************************************************************************/

/*
** If this value is 1 udp checksums are calculated on all transmitted
** datagrams. A value of 0 will disable checksums. This has no effect on
** received datagrams as their checksums, if present, are always checked.
*/
#define CHECKSUM 1

/*
** The spudp driver will buffer this much incoming data before it starts
** silently dropping packets. This may also be set on a per socket basis
** using the SO_RCVBUF socket option. Valid settings range from 1 to
** MAXSOCKBUF.  MAXSOCKBUF is defined in the spip descriptor and defaults
** to 256K.
*/
#define RECVBUFFER 0x8000

/*
** This value controls the maximum amount of data that can be sent in a
** single UDP datagram. This may also be set on a per socket basis using
** the SO_SNDBUF socket option. Valid settings range from 1 to MAXSOCKBUF.
** MAXSOCKBUF is defined in the spip descriptor and defaults to 256K.
*/
#define SENDBUFFER 9216

/*
** This value is only used if a protocol is pushed on top of spudp
**
** SPF will initiate flow control on a path when its read queue exceeds
** this threshold. Once flow control is enabled spudp will buffer data
** up to the paths RECVBUFFER size. When the application reads data
** and the queue falls below this value all buffered data in spudp is
** delivered to SPF.
*/
#define READSZ 4096

/***************************************************************************/
/****            Nothing below this line should be changed              ****/
/***************************************************************************/
#define DRV_NAME	"spudp"		/* device driver name */

/* Macros that set the logical unit generic sections in spf.h */
#define TXSIZE		0xFFFF		/* transmit packet max size */
#define TXOFFSET	8			/* UDP header size */
#define PROTTYPE	SPF_PR_UDP

/* Macros that initialize device descriptor common fields */
#define PORTADDR	0								/* HW Port addr */
#define LUN			0x00							/* logical unit number */
#define MODE		S_ISIZE | S_IREAD | S_IWRITE	/* device mode */
#define LUFLAGS		LU_UIOREAD

/* Macros that initialize dd_popts structure of the device descriptor */
#define ASYNCFLAG	IO_SYNC
#define BLOCKTIME	0
#define PKTFLAG		IO_PACKET_TRUNC
#define WRITESZ		0

/*
** Define the ITEM default values. All possible values to initialize these
** macros to can be found in item.h in the device_type and addr_type
** structures
*/
#define ITE_NETYPE_IN		ITE_NET_CTL
#define ITE_NETYPE_OUT		ITE_NET_CTL
#define ITE_CALL_STATE		ITE_CS_CONNLESS
#define OURADDR_CLASS		ITE_ADCL_INET
#define OURADDR_SUBCLASS	ITE_ADSUB_NONE
#define OURADDR_SIZE		16				/* sizeof(sockaddr_in) */
#define OURADDR				0
#define THEIRADDR_CLASS		ITE_ADCL_INET
#define THEIRADDR_SUBCLASS	ITE_ADSUB_NONE
#define THEIRADDR_SIZE		16				/* sizeof(sockaddr_in) */
#define THEIRADDR			0

/* Macros that set logical units specific information in SPPROTO/defs.h */
#include <SPUDP/defs.h>

#endif /* sudp *************************************************************/
