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
 ****************************************************************************/

#include <SPF/item.h>

#ifdef ip0
/***************************************************************************/
/****               Device Descriptor for SPF spip driver               ****/
/****                                                                   ****/
/****    This section contains the configurable parameters for spip     ****/
/***************************************************************************/

/*
** The system may be configured to act as a gateway and forward IP packets
** received that have other hosts as a final destination. If this feature
** is disabled the packets are simply dropped. Possbile values are 0
** to disable forwarding and 1 to enable forwarding.
*/
#define GATEWAY 0

/*
** This is the value put in the TTL field of outgoing packets. Valid
** settings range from 1 to 255.
*/
#define DEFTTL 64

/*
** If interfaces with large hardware header requirements are added to
** the system dynamically, previously opened paths may not have enough
** header space preallocated. This could cause spip to perform an
** unnecessary data copy. To prevent this, additional buffer space
** beyond what spip normally requires is requested.
*/
#define IPOFFSET 16

/*
** To prevent an application from using too many system resources a
** system wide limit on the size of a socket buffer is imposed. This
** is NOT the amount of space allocated per path by default. The default
** is contolled by settings in each protocol's descriptor (ip0, tcp0,
** etc.). This is a limit on what may be set using the SO_RCVBUF and
** SO_SNDBUF socket options.
*/
#define MAXSOCKBUF (256*1024)

/***************************************************************************/
/****            Nothing below this line should be changed              ****/
/***************************************************************************/
#define DRV_NAME	"spip"		/* device driver name */

/* Macros that initialize device descriptor common fields */
#define PORTADDR	0			/* Base address of hardware */
#define LUN			0x00		/* logical unit number */
#define MODE		S_ISIZE | S_IREAD | S_IWRITE	/* device mode */

/* Macros that initialize dd_popts structure of the device descriptor */
/* #define ASYNCFLAG */
/* #define BLOCKTIME */
#define PKTFLAG		IO_NEXTPKT_ONLY
#define READSZ		0	/* SPF will not do flow control for spip */

/* Macros that set the logical unit generic sections in spf.h */
#define TXSIZE		0xFFFF		/* transmit packet max size */
#define IPHEADSIZE	20
#define BSD_HDRSIZE	8
#define TXOFFSET	IPHEADSIZE + BSD_HDRSIZE + IPOFFSET

#define PROTTYPE	SPF_PR_IP

/* Define the ITEM default values. All possible values to initialize these
 * macros to can be found in item.h in the device_type and addr_type
 * structures
 */
#define ITE_NETYPE_IN		ITE_NET_CTL
#define ITE_NETYPE_OUT		ITE_NET_CTL
#define ITE_CALL_STATE		ITE_CS_CONNLESS
#define OURADDR_CLASS		ITE_ADCL_INET
#define OURADDR_SUBCLASS	ITE_ADSUB_NONE
#define OURADDR_SIZE		16				/* sizeof(sockaddr_in) */
/* Address family AF_INET, Source port 0, IP address 0.0.0.0 */
#define OURADDR				0x00,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
#define THEIRADDR_CLASS		ITE_ADCL_INET
#define THEIRADDR_SUBCLASS	ITE_ADSUB_NONE
#define THEIRADDR_SIZE		16				/* sizeof(sockaddr_in) */
/* Address family AF_INET, Source port 0, IP address 0.0.0.0 */
#define THEIRADDR			0x00,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

/* Define the protocol stack in this descriptor if desired.
 */
/* #define PROTSTAK " " */

/* Macros that set logical units specific information in SPPROTO/defs.h */
#include <SPIP/defs.h>

#endif /* ip0  **********************************************************/
