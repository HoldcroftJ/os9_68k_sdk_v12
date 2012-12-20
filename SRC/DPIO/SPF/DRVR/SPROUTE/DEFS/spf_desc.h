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

#ifdef route0
/***************************************************************************/
/****              Device Descriptor for SPF sproute driver             ****/
/****                                                                   ****/
/****   This section contains the configurable parameters for sproute   ****/
/***************************************************************************/

/*
** This value is only used if a protocol is pushed on top of sproute
**
** SPF will initiate flow control on a path when its read queue exceeds
** this threshold. Once flow control is enabled sproute will buffer data
** up to the paths RECVBUFFER size. When the application reads data
** and the queue falls below this value all buffered data in sproute is
** delivered to SPF.
*/
#define READSZ 4096

/***************************************************************************/
/****            Nothing below this line should be changed              ****/
/***************************************************************************/
#define DRV_NAME	"sproute"		/* device driver name */

/* Macros that set the logical unit generic sections in spf.h */
#define TXSIZE		0xFFFF		/* transmit packet max size */
#define TXOFFSET	0			/* ROUTE header size */
#define PROTTYPE	SPF_PR_IP

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
#include <SPROUTE/defs.h>

#endif /* route ***********************************************************/
