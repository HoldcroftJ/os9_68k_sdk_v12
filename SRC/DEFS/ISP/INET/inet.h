/****************************************************************************
 *  $Workfile:   inet.h  $
 *  $Revision:   1.13  $
 *  $Modtime:   23 Feb 1998 18:10:00  $
 *  $Date:   23 Feb 1998 18:10:20  $
 ****************************************************************************
 *	Definitions for internet domain module.									*
 ****************************************************************************
 *	Copyright 1993 by Microware Systems Corporation							*
 *	Reproduced Under License												*
 *																			*
 *	This source code is the proprietary confidential property of			*
 *	Microware Systems Corporation, and is provided to licensee				*
 *	solely for documentation and educational purposes. Reproduction,		*
 *	publication, or distribution in any form to any party other than		*
 *	the licensee is strictly prohibited.									*
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date   	Comments                                       	   By	*
 *  --- --------	-----------------------------------------------	------- *
 *	  0 ??/??/??	Birth											    ???	*
 *      09/16/93	<***** ISP 2.0 Release ***********************>			*
 *	  1	10/21/93	Merged OS-9/OS-9000 sources							bat	*
 *    2 05/03/94    OS-9000 Ifm_dev_list -> Ifdev 						bat	*
 *      01/17/96	<***** ISP 2.0 Release ***********************>			*
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release ***********>      *
 *                  ---- OS-9000/Brutus Board Support V1.0 Released ----    *
 ****************************************************************************/

#ifndef _INET_H
#define _INET_H


#include <types.h>

#include <INET/sockman.h>


/*
	internet socket-specific storage structure
*/
typedef struct inetscb {
	struct inetscb	*inets_socknext;	/* list of internet sockets */
	struct inetscb	*inets_sockprev;	/* reverse */
	struct inetscb	*inets_sockhead;	/* pointer to head of list */
	Scb			inets_scb;		/* back pointer to protocol scb */
	Sockmod		inets_smod;		/* pointer to socket module entry */
	Ifdev		inets_ifdev;	/* default route if device */
	u_int32		inets_dst;		/* destination address */
	int			inets_rttype;	/* local or remote route */
	u_int32		inets_flags;	/* some flags */
	struct in_addr	inets_laddr;	/* local internet address of socket */
	struct in_addr	inets_paddr;	/* internet address of connected peer */
	u_int16		inets_lport;		/* local port number */
	u_int16		inets_pport;		/* peer port number */
	int32		inets_size;		/* size from srqmem */
} INETSCB, *Inetscb;

#define INTERNET_SETLOCAL 0x01		/* domain set the local address */

/* Extra services provided by the internet module */
#define PRC_FINDSOCK	0x00010001	/* find an internet socket */
#define PRC_CLEAR		0x00010002	/* clear the socket list */
#define PRC_INITHEAD	0x00010003	/* init the head of the socket list */


#endif	/* _INET_H */
