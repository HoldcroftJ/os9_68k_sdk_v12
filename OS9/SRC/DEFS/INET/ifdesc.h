/****************************************************************************
 *	ifdesc.h																*
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
 *	  1 ??/??/??	Birth											    ???	*
 *      09/16/93	<***** ISP 2.0 Release ***********************>			*
 ****************************************************************************/

#ifndef _IFDESC_H
#define _IFDESC_H

/*
	Device descriptor layout for an if device
*/
typedef struct ifdevdd {
	mod_dev		dds;			/* standard device descriptor */
	int32		dd_name;		/* (offset to) interface (descriptor) name */
	int16		dd_mtu;			/* maximum transmission unit */
	u_int16		dd_flags;		/* interface flags */
	u_int16		dd_addr;		/* (offset to) struct sockaddr logical address */
	u_int32		dd_subnet;		/* subnet mask */
	int16		dd_bdaddr;		/* (offset to) broadcast/destination address */
	int16		dd_linkaddr;	/* (offset to) link-level address */
	int32		dd_spare[5];	/* reserved */
} IFDEVDD, *Ifdevdd;

/*
	ifloop device specific section
	used to pass tunable parameters to ifman
*/
typedef struct ifloopdevdd {
	u_int32		if_priority;	/* priority for ifman's net_in process */
} IFLOOPDEVDD, *Ifloopdevdd;


#endif	/* _IFDESC_H */
