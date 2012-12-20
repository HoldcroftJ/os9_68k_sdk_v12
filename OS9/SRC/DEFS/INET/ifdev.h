/****************************************************************************
 *	ifdev.h																	*
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
 *   #   Date   	Comments                                          	By	*
 *  --- --------	-----------------------------------------------	------- *
 *	  1 ??/??/??	Birth										   		???	*
 *      09/16/93	<***** ISP 2.0 Release ***********************>			*
 *    2 ??/??/??    Added wrappers                                          *
 ****************************************************************************/

#ifndef _IFDEV_H
#define _IFDEV_H

/*
	Interface description structure
*/

typedef struct ifdev {
	struct ifdev	*if_next,		/* next known interface */
					*if_prev;		/* previous interface */
	struct devicetbl *if_devtbl;	/* device table ptr for device */
	sysioStatic		*if_static;		/* static storage ptr for device */
	struct ifmanstat *if_man;		/* ptr to ifman's static storage */
	char			*if_name;		/* ptr to interface name */
	u_int32			if_size;		/* size of this structure */
	int16			if_mtu;			/* maximum transmission unit */
	u_int16			if_flags;		/* interface flags */
	struct sockaddr	if_addr;		/* interface address (usu. sockaddr) */
	char			if_paddr[16];	/* interface physical address */
	u_int32			if_subnet;		/* subnet mask */
	union {
		struct sockaddr ifu_broadcast;	/* broadcast address filter */
		struct sockaddr ifu_dstaddr;	/* point-to-point conn. dest. addr. */
	} if_ifu;
	struct ifqueue	*if_outq;		/* output queue */
	int32		(*if_init)();		/* attach device */
	int32		(*if_term)();		/* detach device */
	int32		(*if_send)();		/* send a datagram */
	int32		(*if_xmit)();		/* transmit raw packet */
	int32		(*if_recv)();		/* input data siphon function */
	int32		(*if_stat)();		/* determine status of interface */
	u_int32		if_ipackets;		/* num. input packets received */
	u_int32		if_opackets;		/* num. output packets sent */
	u_int32		if_ierrors;			/* num. input errors */
	u_int32		if_oerrors;			/* num. output errors */
	u_int32		if_collisions;		/* num. collisions on csma devices */
/* for status utilities */
	struct ifdev	*if_this;		/* next known interface */
	char 		if_devnam[32];		/* copied device name string */
	char 		if_dvrnam[32];		/* copied driver name string */
} *Ifdev;

#define IFF_UP			0x1		/* interface is up */
#define IFF_BROADCAST	0x2		/* interface can broadcast */
#define IFF_DEBUG		0x4		/* debugging mode */
#define IFF_ROUTE		0x8		/* routing entry installed */
#define IFF_PT_TO_PT	0x10	/* point to point link */
#define IFF_NOTRAILERS	0x20	/* don't use trailers */
#define IFF_RUNNING		0x40	/* online.. */
#define IFF_TAKE_NFWD	0x80	/* accept "no forward" packets */
#define IFF_NOARP		0x100	/* no address resolution protocol */
#define IFF_NOTETHER	0x200	/* not an ethernet interface (Kludge) */

#define	IA_SIN(ia) ((struct sockaddr_in *)(&(ia)->if_addr))
#define ID_SIN(ia) ((struct sockaddr_in *)(&(ia)->if_ifu.ifu_dstaddr))


#endif	/* _IFDEV_H */
