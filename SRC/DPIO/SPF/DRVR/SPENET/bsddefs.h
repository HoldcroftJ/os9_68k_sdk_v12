/****************************************************************************
 *           Copyright 1989-1997 by Microware Systems Corporation           *
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
 *  01 04/28/97 Created                                                rds
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *     05/20/97 <--------- SPF LAN Comm Pack v3.1.1 Release -------->
 *  29 12/10/97 Commented our arptab definition                        tmd
 *  30 12/10/97 Added sysglob.h & stddef.h for check_access & is_super tmd
 *  31 02/03/98 Added netinet/in_systm.h, netinet/ip.h, and types.h    rds
 *  32 04/01/98 Cleaned up code.                                       sr
 *     04/01/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  33 09/25/98 Added SPENET define for use in BSD header files        rds
 *  37 11/22/99 Moved arpstat structure to arp.h                       rds
 *  40 10/31/00 Added MAXCOPY and COPYSIZE                             rds
 ****************************************************************************/

#define SPENET

/* DPIO definitions */
#ifdef _OSK
#include <defconv.h>
#endif

/*
** System Header Files
*/
#include <const.h>
#include <errno.h>
#include <signal.h>
#include <events.h>
#include <memory.h>
#include <string.h>
#include <sg_codes.h>
#include <modes.h>
#include <module.h>
#include <process.h>
#include <sysglob.h>
#include <stddef.h>

#ifdef DEBUG
#include <dbg_mod.h>
#endif

#include <SPF/mbuf.h>

/*
** BSD header files.
*/
struct bsd_mbuf;
struct sockaddr;
struct rtentry;
struct ifnet;
struct ether_header;
struct proc;
struct socket;
#include <types.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/if_ether.h>


/*
** Additional header files needed everywhere
*/
#include "defs.h"
#include <SPF/enet.h>
#include "proto.h"
#include "arp.h"

/*
** Structure used by driver
*/
typedef struct addr_list {
	u_int32        in_useflag; /* To indicate whether the proto_addr is valid*/
	struct in_addr proto_addr; /* Protocol address */
} addr_list, *Addr_list;

typedef struct ether_header *Ether_header;

#ifdef DEBUG
  #define DEBUG_DATA debug_data
  #define DEBUG_STRING debug_string
  #define ALGN_DBG_DATA debug_string
#else
  #define DEBUG_DATA(a,b,c)
  #define DEBUG_STRING(a,b,c)
  #define ALGN_DBG_DATA(a,b,c,d)
#endif

#if defined(_MPFMIPS) || defined(_MPFARM) || defined(_MPFSH) || defined(_MPFSPARC)
#define ALIGN_MBUF 
#define MAXCOPY 256 /* Copy entire packet if smaller than this size */
#define COPYSIZE 80 /* Number of bytes to copy when packet bigger than MAXCOPY */
#endif 
