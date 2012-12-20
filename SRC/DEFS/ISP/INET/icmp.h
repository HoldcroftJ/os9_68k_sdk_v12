/****************************************************************************
 *  $Workfile:   icmp.h  $
 *  $Revision:   1.11  $
 *  $Modtime:   23 Feb 1998 14:03:00  $
 *  $Date:   23 Feb 1998 14:03:32  $
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
 *	  0 ??/??/??	Birth												???	*
 *      09/16/93	<***** ISP 2.0 Release **************************>		*
 *	  1	10/19/93	Merged OS-9/OS-9000 sources							bat	*
 *      01/17/96	<***** ISP 2.1 Release **************************>		*
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release ***********>      *
 *                  ---- OS-9000/Brutus Board Support V1.0 Released ----    *
 ****************************************************************************/

#ifndef _ICMP_H
#define _ICMP_H

/*
	defs for icmp protocol handling in ip
*/
typedef struct icmp_header {
	u_int8	icmp_type;		/* icmp message type */
	u_int8	icmp_code;		/* icmp type code */
	u_int16	icmp_checksum;	/* icmp message checksum */
} ICMP_HEADER, *Icmp_header;

/* for echo request and other icmp messages */
typedef struct icmp_identseq {
	u_int16	icmp_identifier;
	u_int16	icmp_sequence;
} ICMP_IDENTSEQ, *Icmp_identseq;

typedef struct icmp_gwiphd {
	u_long		icmp_gwaddr;
	IP_HEADER	icmp_iphd;
}ICMP_GWIPHD, *Icmp_gwiphd;

/* ICMP message types */
#define ICMP_ECHOREP		0
#define ICMP_DESTUNREACH	3
#define ICMP_SOURCEQUENCH	4
#define ICMP_REDIRECT		5
#define ICMP_ECHOREQ		8
#define ICMP_TIMEEXCEEDED	11
#define ICMP_PARMPROBLEM	12
#define ICMP_TIMEREQ		13
#define ICMP_TIMEREP		14
#define ICMP_INFORMREQ		15
#define ICMP_INFORMREP		16
#define ICMP_ADDRMASKREQ	17
#define ICMP_ADDRMASKREP	18

/*	destination unreachable codes */
#define NET_UNREACH		0
#define HOST_UNREACH	1
#define PROTO_UNREACH	2
#define PORT_UNREACH	3
#define FRAG_NEEDED		4
#define SROUTE_FAIL		5

/*	time exceeded codes */
#define TIME_TO_LIVE	0
#define TIME_FRAGMENT	1

/* parameter problem error codes */
#define POINTER			0

/* Redirect Message Code */
#define ICMP_REDIRECT_NET		0
#define ICMP_REDIRECT_HOST		1
#define ICMP_REDIRECT_TOSNET	2
#define ICMP_REDIRECT_TOSHOST	3


#endif	/* _ICMP_H */
