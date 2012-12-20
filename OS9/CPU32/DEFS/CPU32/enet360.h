/* enet360.h */

/***********************************************************************
* NOTICE: This file was originally suppied by Motola Inc. as ethernet.h.
* The file name has been changed to enet360.h to remove a name collision.
***********************************************************************/

/*********************************************************************** 
* NOTICE
* All files contained on this disk are subject to the licensing conditions
* issued by MOTOROLA Inc.
*
* All files are copyright 1993 by MOTOROLA Inc. 
************************************************************************/

/*
 * ethernet.h
 * definitions for the ETHERNET controllers
 */

/* note some of these bits are also defined elsewhere */
/*
 * transmit BD's
 */
#ifndef T_R
#define	T_R	0x8000		/* ready bit */
#endif
#define	T_PAD	0x4000		/* short frame padding */
#ifndef T_W
#define	T_W	0x2000		/* wrap bit */
#define	T_I	0x1000		/* interrupt on completion */
#define	T_L	0x0800		/* last in frame */
#endif
#define	T_C	0x0400		/* transmit CRC (when last) */

#define	ETHERNET_T_ERROR	0x00c2
#define	T_DEF	0x0200		/* defer indication */
#define	T_HB	0x0100		/* heartbeat */
#define	T_LC	0x0080		/* error: late collision */
#define	T_RL	0x0040		/* error: retransmission limit */
#define	T_RC	0x003c		/* retry count */
#define	T_UN	0x0002		/* error: underrun */
#define	T_CSL	0x0001		/* carier sense lost */

/*
 * receive BD's
 */
#ifndef R_E
#define	R_E	0x8000		/* buffer empty */
#define	R_W	0x2000		/* wrap bit */
#define	R_I	0x1000		/* interrupt on reception */
#endif
#define	R_L	0x0800		/* last BD in frame */
#define	R_F	0x0400		/* first BD in frame */

#define	R_ERROR	0x00ff
#define	R_LG	0x0020		/* frame too long */
#define	R_NO	0x0010		/* non-octet aligned */
#define	R_SH	0x0008		/* short frame */
#define	R_CR	0x0004		/* receive CRC error */
#define	R_OV	0x0002		/* receive overrun */
#define	R_CL	0x0001		/* collision */


/*
 * ethernet interrupts
 */
#define	ETHERNET_GRA	0x0080	/* graceful stop complete */
#define	ETHERNET_TXE	0x0010	/* transmit error */
#define	ETHERNET_RXF	0x0008	/* receive frame */
#define	ETHERNET_BSY	0x0004	/* busy condition */
#define	ETHERNET_TXB	0x0002	/* transmit buffer */
#define	ETHERNET_RXB	0x0001	/* receive buffer */

/*
 * ethernet address structure
 */
typedef struct enet_addr {
	unsigned long	high_addr;
	unsigned short	low_addr;
} ENET_ADDR;

typedef struct enet_frame {
	struct enet_addr	dst_addr;
	struct enet_addr	src_addr;
	short			type_or_len;
	char			*data;
} ENET_FRAME;

#define ETHERENT_GET_ADDR(addr, buf)	\
{ \
	 int	i; \
	for( i = 0; i < 6; i++) { \
		((unsigned char *)(addr))[i] = ((unsigned char *)(buf))[i]; \
	} \
}

#define	GROUP_ADDR	0x80000000	/* group address */

#define BROADCAST_ADDR(addr)	(((addr)->high_addr == 0xffffffff) && \
					((addr)->low_addr == 0xffff))

/*
 * Types of ENET_CLR_ADRS action request
 */
#define CLR_GRP_ADDRS	0x1
#define CLR_IND_ADDRS	0x2
#define CLR_ALL_ADDRS	( CLR_GRP_ADDRS | CLR_IND_ADDRS )

#define EN_MIN_PACKET 	64
#define EN_MAX_PACKET 	1518
