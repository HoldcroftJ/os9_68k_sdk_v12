/****************************************************************************
 *  $Workfile:   i82596.h  $
 *  $Revision:   26.3  $
 *  $Modtime:   18 Oct 1993 16:03:00  $
 *  $Date:   21 Oct 1993 14:13:30  $
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
 *	  1	08/10/93	merged i82596.h from booter and driver. very kludgy 	*
 *					merge since they had drifted quite far apart.  			*
 *					The fact that the booters makefile defines CBOOT 		*
 *					is used to conditionalize this file					gkm	*
 *	  2 09/09/93	Added RFD_NORES                                 kk/ylee	*
 *	  3 10/18/93	Added mizar's SYSBUS_EBE				 bat/arb(mizar) *
 *		10/18/93	<***** ISP 2.0 Release **************************>		*
 ****************************************************************************/

 
/*
 * RBD - receive buffer descriptor (stored in mbuf) (NOT USED BY BOOTER)
 */
typedef struct ie_rbd {
	u_int16			rbd_acount;		/* number of actual bytes in buffer */
#define RBD_EOF			0x8000		/* last buffer in frame */
#define RBD_USED		0x4000		/* buffer has been used (and abused) */
	u_int16			rbd_zero1;		/* must be zeroes */
	struct ie_rbd	*rbd_leNext;	/* next rbd in RFL */
	char			*rbd_leBuffer;	/* pointer to received data */
	u_int16			rbd_size;		/* size of receive buffer */
#define RBD_SIZEMASK	0x3fff		/* size is 14-bits */
#define RBD_LAST		0x8000		/* last rbd in RFL */
#define RBD_PREFETCH	0x4000		/* i82596 has prefetched this RBD */
	u_int16			rbd_zero2;		/* must be zeroes */
	Mbuf			rbd_mbuf;		/* ptr to mbuf containing this RBD */
	struct ie_rbd	*rbd_beNext;	/* next rbd in RFL */
	u_int32			rbd_padd[2];
} RBD;

#define RMBUF_SIZE	(0x640-16)		/* mballoc multiple - mbuf header */

/*
 * RFD - receive frame descriptor: linear mode, simplified model
 * was identical for the booter and driver except that the driver 
 * conditionalizes on PVTRAM.  Since the booter never uses PVTRAM, cool.
 * The driver also defines rfd_padd at the end and the booter does not.  
 * *should* be okay (at the end)- gkm.
 */
typedef struct ie_rfd {
	u_int16			rfd_stat1;		/* first status word */
#define	RFD_COMPLETE	0x8000		/* reception complete */
#define RFD_BUSY		0x4000		/* reception in progress */
#define RFD_OK			0x2000		/* received frame has no errors */
#define RFD_NORES		0x0200		/* no RBD to receive frame */
	u_int16			rfd_stat2;		/* second status word */
#define RFD_LAST		0x8000		/* last rfd in rdl */
#define RFD_SUSPEND		0x4000		/* suspend after receive */
#define RFD_FLEXIBLE	0x0008		/* flexible/simple memory model */
	struct ie_rfd	*rfd_leNext;	/* ptr to next rfd (little-end) */
	struct ie_rbd	*rfd_rbd;		/* ptr to receive buffer descriptor */
#define RFD_RBD_NOTUSED		(void *)-1
	u_int16			rfd_acount;		/* number of actual bytes in buffer */
#define RFD_EOF			0x8000		/* last buffer in frame */
#define RFD_USED		0x4000		/* buffer has been used (and abused) */
	u_int16			rfd_size;		/* size of buffer */
#define RFD_SIZEMASK	0x3fff		/* size is 14-bits */
#if defined(CBOOT) || defined(PVTRAM) /* ONLY for booter or if drvr has PVTRAM*/
	u_int8			rfd_da[6];		/* destination address */
	u_int8			rfd_sa[6];		/* source address */
	u_int16			rfd_length;		/* length/type from 802.3 frame */
	u_int8			rfd_data[ETHER_MAX];	/* ethernet received data */
#endif
	struct ie_rfd	*rfd_beNext;	/* ptr to next rfd (big-end for driver) */
	struct ie_rfd	*rfd_bePrev;	/* ptr to prev rfd (big-end for driver) */
	u_int32			rfd_padd[1];	/* padd to boundary */
} RFD;

/*
 * TBD - Transmit buffer descriptor
 */
#define	NTBD	8		/* max. TBD segments/xmit command */
typedef struct ie_tbd {
	u_int16			tbdcnt;		/* number of bytes to xmit from this TBD */
#define	TBD_EOF		0x8000		/* last TBD in xmit list */
#define TBD_CNTMASK	0x3fff		/* 14-bit count field */
	u_int16			zeroes;
	struct ie_tbd	*tbd_leNext;/* pointer to next tbd in xmit chain */
	char			*tbd_data;	/* pointer to data to xmit */
	u_int32			tbd_padd;	/* pad to 16 bytes */
#ifdef LCLMEM					/*** LCLMEM usage possible only by booter ***/
	u_int8			xmitdata[1536];	/* xmit data buffer (lld + alignment) */
#endif
} TBD;

typedef struct ie_tcb {					/* NOTE: merged for booter & driver */
	struct ie_tbd *tbdptr;				/* ptr to transmit buffer descriptor */
		#define TBD_NOT_USED	((void *)-1)
	u_int16		tcbcnt;					/* number of bytes to xmit from TCB */
		#define	TCB_EOF		0x8000		/* entire frame is in TCB */
		#define TCB_CNTMASK	0x3fff		/* 14-bit count field */
	u_int16		zeroes;					/* must be zeroes */
	#if defined(CBOOT)					/* FOR BOOTER ONLY */
		u_int8		da[6];				/* destination ethernet address */
		u_int16		tcblen;				/* length/type for 802.3 frame */
		u_int8		xmitdata[1536];		/* the transmit data */
	#else								/* !defined(CBOOT) FOR DRIVER ONLY */
		#ifdef PVTRAM
			char		xmitdata[1536];	/* data for simplified mode */
		#else
			Mbuf		xmbuf;			/* mbuf used for xmit */
			TBD			tbd[NTBD];		/* num. segments gathered for xmit */
			/*
			 * hack to adjust CB size so CB can be placed in mbuf
			 */
			#define	TCB_SIZE_ADJ	(sizeof(Mbuf)+(sizeof(TBD)*NTBD))
			#define MBUF_MARKER		0x4d425546
		#endif	/* PVTRAM */
	#endif		/* CBOOT  */
} TCB;

/*
 * CB - Command block template
 */
typedef struct ie_cb {
	u_int16			cb_status;		/* status word */
#define	CB_COMPLETE		0x8000		/* 1=command complete */
#define CB_BUSY			0x4000		/* 1=command in progress */
#define CB_OK			0x2000		/* 1=completed without error */
#define CB_ABORT		0x1000		/* 1=command aborted */
#define XMIT_LCOL		0x0800		/* 1=late collision on xmit */
#define	XMIT_LCAR		0x0400		/* 1=no carrier sensed */
#define XMIT_LCTS		0x0200		/* 1=lost clear to send */
#define XMIT_UDMA		0x0100		/* 1=dma underrun */
#define XMIT_DEFR		0x0080		/* 1=transmit deferred */
#define XMIT_BEAT		0x0040		/* 1=CDT (heartbeat) active */
#define XMIT_MTRY		0x0020		/* 1=exceeded maximum retries for xmit */
#define XMIT_COLLMASK	0x000f		/* collision count field */
	u_int16			cb_command;		/* command word */
#define CB_LAST			0x8000		/* last command in list */
#define CB_SUSPEND		0x4000		/* suspend after this command */
#define CB_IRQ			0x2000		/* generate IRQ after command */
#define XMIT_NC			0x0010		/* 1=no CRC insertion enable */
#define XMIT_SF			0x0008		/* 1=flexible, 0=simplified (sic) */
#define CB_CMDMASK		0x0007		/* mask for command code */
#define CB_CMD_NOP		0			/* NOP command */
#define CB_CMD_IASETUP	1			/* Intermediate address setup command */
#define CB_CMD_CONFIG	2			/* CONFIG command */
#define CB_CMD_MCSETUP	3			/* Multicast setup command */
#define CB_CMD_XMIT		4			/* Transmit command */
#define CB_CMD_TDR		5			/* TDR command */
#define CB_CMD_DUMP		6			/* Dump internals command */
#define CB_CMD_DIAG		7			/* Diagnose command */
	struct ie_cb	*cb_leNext;		/* ptr to next command block (little end) */
	union {
		u_int8		ia[6];			/* ethernet address (for IA command) */
		TCB			tcb;			/* transmit command block */
	} cmd;
#if defined(CBOOT)					/* for BOOTER only */
	struct ie_cb	*cb_beNext;		/* ptr to next cmd block (bigend for driver) */
	struct ie_cb	*cb_bePrev;		/* ptr to prev cmd block (bigend for driver) */
/**	u_int8			cb_padd[4]; **/	/* padd to 16-byte bound */
#ifdef NHIST
	u_int32			loops;
#endif
#else								/* for DRIVER only */
#if 0
	/*
	 * can't use a big-end pointer here because of the stupid Errata 2.0!
	 */
	struct ie_cb	*cb_beNext;		/* ptr to next cmd block (bigend for driver) */
#endif
#endif /* CBOOT */
} CB;

/*
 * SCB - system control block template
 */		
typedef struct ie_scb {
	u_int16		scb_status;		/* status word */
#define SCB_STATUS_CX	0x8000  /* command complete interrupt */
#define SCB_STATUS_FR	0x4000  /* finished receiving frame interrupt */
#define SCB_STATUS_CNA	0x2000  /* CU left in active state */
#define SCB_STATUS_RNR	0x1000  /* RU left in ready state */
#define SCB_CU_ACTIVE	0x0200  /* CU in active state */
#define SCB_CU_SUSPEND	0x0100  /* CU is suspended */
#define SCB_CU_IDLE		0x0000	/* CU is idle */
#define SCB_RU_NORBD	0x0080	/* RU is in no RBD state */
#define SCB_RU_READY	0x0040  /* RU is in ready state */
#define SCB_RU_NORES	0x0020	/* RU is in no resources state */
#define SCB_RU_SUSPEND	0x0010  /* RU is suspended */
#define SCB_RU_IDLE		0x0000  /* RU is idle */
#define SCB_THROTTLE	0x0008	/* throttle timers loaded */
	u_int16		scb_cmd;		/* command word */
#define SCB_CX_ACK		0x8000	/* complete complete int ack */
#define SCB_FR_ACK		0x4000  /* frame reception int ack */
#define SCB_CNA_ACK		0x2000	/* CU not ready int ack */
#define SCB_RNR_ACK		0x1000	/* RU not ready int ack */
#define SCB_CU_BUSTHRI		0x0600	/* load bus throttle timers */
#define SCB_CU_BUSTHR		0x0500	/* load bus throttle timers */
#define SCB_CU_ABORT_CMD	0x0400  /* abort CU cmd */
#define SCB_CU_SUSPEND_CMD	0x0300  /* suspend CU cmd */
#define SCB_CU_RESUME_CMD	0x0200  /* resume CU cmd */
#define SCB_CU_START_CMD	0x0100  /* start CU cmd */
#define SCB_CU_NOP_CMD		0x0000  /* CU nop cmd */
#define SCB_CHIP_RESET_CMD	0x0080  /* chip reset */
#define SCB_RU_ABORT_CMD	0x0040  /* RU abort cmd */
#define SCB_RU_SUSPEND_CMD	0x0030  /* RU suspend cmd */
#define SCB_RU_RESUME_CMD	0x0020  /* RU resume cmd */
#define SCB_RU_START_CMD	0x0010  /* RU start cmd */
#define SCB_RU_NOP_CMD		0x0000  /* RU nop cmd */
	CB			*cbl;			/* command block list ptr */
    RFD			*rfa;			/* receive frame area ptr */
	u_int32		crcErrs;		/* CRC errors */
	u_int32		alignErrs;		/* alignment errors */
	u_int32		resErrs;		/* resource errors */
	u_int32		overrunErrs;	/* overrun errors */
	u_int32		rcvcdtErrs;		/* receive collisions errors */
	u_int32		sfErrs;			/* short frame errors */
	u_int16		toffTimer;		/* throttle t-off timer */ 
	u_int16		tonTimer;		/* throttle t-on timer */ 
	u_int8		cb_padd[8];		/* padd to 16-byte bound */
} SCB;

/*
 * ISCP - Intermediate System configuration pointer
 */
typedef struct ie_iscp {
	u_int8		iscp_unused1;
	u_int8		iscp_busy;			/* busy indicator */
#define	ISCP_NOT_BUSY	1
	u_int16		iscp_unused2;
	SCB			*iscp_leSCB;		/* (little end) scb pointer */
	SCB			*iscp_beSCB;		/* (big end) scb pointer */
	u_int8		iscp_padd[4];		/* padd to 16-byte bound */
} ISCP;

/*
 * SCP - System configuration pointer
 */
typedef struct ie_scp {
	u_int32		scp_sysbus;			/* configuration goodies */
#define SYSBUS_EBE		0x00000080	/* enhanced big endian enable */
#define SYSBUS_CSW		0x00000040	/* reserved */
#define SYSBUS_INTLO	0x00000020	/* interrupt polarity */
#define SYSBUS_INTHI	0x00000020	/* interrupt polarity */
#define SYSBUS_LOCK		0x00000010	/* lock function */
#define SYSBUS_TRG		0x00000008	/* bus throttle triggers */
#define SYSBUS_MODEMASK	0x00000006	/* operation mode */
#define SYSBUS_LINEAR	0x00000004	/* operation mode (linear) */
	u_int32		scp_unused2;
	ISCP		*scp_leISCP;		/* (little end) iscp pointer */
	ISCP		*scp_beISCP;		/* (big end) iscp pointer */
} SCP;

/*
 * template for selftest result
 */
typedef struct {
	u_int32		str_romsig;			/* ROM signature */
	u_int32		str_result;			/* selftest result */
} STR;

