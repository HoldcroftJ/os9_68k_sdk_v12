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
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *   0 ??/??/?? Birth                                                  ???
 *   3 07/15/97 Ported to OS9/68k                                      jpd
 *   4 08/27/97 Bug fix                                                taka
 *   6 11/05/97 Added RFD_ERRS define                                  pjw
 *   8 01/21/98 Removed nested comment (compiler warning)              jk
 *   9 02/06/98 Added (sp162), (sp172), (sp177) to #if defined
 *              for #define SWAPWORDS                                  jk
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  11 06/22/99 Integrated Kei Thomsen's changes, fixed mbuf leak.     jk
 *  12 08/29/99 Cleaned up and removed unused code                     rds
 ****************************************************************************/

#ifndef _I82596_H
#define _I82596_H

#if defined (sp167) || defined (sp162) || defined(sp172) || defined(sp177)
#define SWAPWORDS(a)			(((a)>>16)|((a)<<16))
#endif

#if defined (_MPFSH)
#define MAKE_LOCAL_ADDR(a)		((a)&0x7fffff)
#define MAKE_LOCAL_ADDR_SWAP(a)	((((a)&0x7fffff)>>16)|(((a)&0x7fffff)<<16))
#define MAKE_CPU_ADDR(a)		((a)|0xa4000000)
#define MAKE_CPU_ADDR_SWAP(a)	((((a)>>16)|((a)<<16))|0xa4000000)
#else
#define MAKE_LOCAL_ADDR(a)		((a))
#define MAKE_LOCAL_ADDR_SWAP(a)	(((a)>>16)|((a)<<16))
#define MAKE_CPU_ADDR(a)		((a))
#define MAKE_CPU_ADDR_SWAP(a)	(((a)>>16)|((a)<<16))
#endif	/* _MPFSH */

/*
 * RBD - receive buffer descriptor (stored in mbuf) (NOT USED BY BOOTER)
 */
typedef volatile struct ie_rbd RBD;
struct ie_rbd {
	u_int16	rbd_acount;				/* number of actual bytes in buffer */
		#define RBD_EOF		0x8000	/* last buffer in frame */
		#define RBD_USED	0x4000	/* buffer has been used (and abused) */
	u_int16	rbd_zero1;				/* must be zeroes */
	RBD		*rbd_leNext;			/* next rbd in RFL */
	char	*rbd_leBuffer;			/* pointer to received data */
	u_int16	rbd_size;				/* size of receive buffer */
		#define RBD_SIZEMASK 0x3fff	/* size is 14-bits */
		#define RBD_LAST	 0x8000	/* last rbd in RFL */
		#define RBD_PREFETCH 0x4000	/* i82596 has prefetched this RBD */
	u_int16	rbd_zero2;				/* must be zeroes */
	Mbuf	rbd_mbuf;				/* ptr to mbuf containing this RBD */
	RBD		*rbd_beNext;			/* next rbd in RFL */
	u_int32	rbd_padd[2];
};

#define RMBUF_SIZE	(0x640-16)		/* mballoc multiple - mbuf header */

/*
 * RFD - receive frame descriptor: linear mode, simplified model
 * was identical for the booter and driver except that the driver
 * conditionalizes on PVTRAM.  Since the booter never uses PVTRAM, cool.
 * The driver also defines rfd_padd at the end and the booter does not.
 * *should* be okay (at the end)- gkm.
 */
typedef volatile struct ie_rfd RFD;
struct ie_rfd {
	u_int16	rfd_stat1;					/* first status word */
		#define	RFD_COMPLETE	0x8000	/* reception complete */
		#define RFD_BUSY		0x4000	/* reception in progress */
		#define RFD_OK			0x2000	/* received frame has no errors */
		#define RFD_NORES		0x0200	/* no RBD to receive frame */
		#define RFD_ERRS        0x1fff  /* Check for errors*/
	u_int16	rfd_stat2;					/* second status word */
		#define RFD_LAST		0x8000	/* last rfd in rdl */
		#define RFD_SUSPEND		0x4000	/* suspend after receive */
		#define RFD_FLEXIBLE	0x0008	/* flexible/simple memory model */
	RFD		*rfd_leNext;				/* ptr to next rfd (little-end) */
	RBD		*rfd_leRbd;					/* ptr to receive buffer descriptor */
		#define RFD_RBD_NOTUSED		(void *)-1
	u_int16	rfd_acount;					/* number of actual bytes in buffer */
		#define RFD_EOF			0x8000	/* last buffer in frame */
		#define RFD_USED		0x4000	/* buffer has been (ab)used */
	u_int16	rfd_size;					/* size of buffer */
		#define RFD_SIZEMASK	0x3fff	/* size is 14-bits */
#if defined(PVTRAM)
	/*
	** The driver normally runs in "no source address insertion" mode,
	** except when using private RAM. Therefore, following are only
	** needed for private RAM.
	*/
	u_int8	rfd_da[6];					/* destination address */
	u_int8	rfd_sa[6];					/* source address */
	u_int16	rfd_length;					/* length/type from 802.3 frame */
	u_int8	rfd_data[ETHER_MAX];		/* ethernet received data */
#endif
	RFD		*rfd_beNext;	/* ptr to next rfd (big-end for driver) */
	RFD		*rfd_bePrev;	/* ptr to prev rfd (big-end for driver) */
	u_int32	rfd_padd[1];	/* padd to boundary (not really!) */
};

/*
 * TBD - Transmit buffer descriptor
 */
#define	NTBD	8		/* max. TBD segments/xmit command */
typedef volatile struct ie_tbd TBD;
struct ie_tbd {
	u_int16	tbdcnt;					/* number of bytes to xmit from this TBD */
		#define	TBD_EOF		0x8000	/* last TBD in xmit list */
		#define TBD_CNTMASK	0x3fff	/* 14-bit count field */
	u_int16	zeroes;
	TBD		*tbd_leNext;			/* pointer to next tbd in xmit chain */
	char	*tbd_leData;			/* pointer to data to xmit */
	u_int32	tbd_padd;				/* pad to 16 bytes */
#ifdef LCLMEM						/* LCLMEM usage possible only by booter */
	u_int8	xmitdata[1536];			/* xmit data buffer (lld + alignment) */
#endif
};

typedef volatile struct ie_tcb TCB;
struct ie_tcb {						/* NOTE: merged for booter & driver */
	TBD		*leTbdptr;				/* ptr to transmit buffer descriptor */
		#define TBD_NOT_USED	((void *)-1)
	u_int16	tcbcnt;					/* number of bytes to xmit from TCB */
		#define	TCB_EOF		0x8000	/* entire frame is in TCB */
		#define TCB_CNTMASK	0x3fff	/* 14-bit count field */
	u_int16	zeroes;					/* must be zeroes */
#ifdef PVTRAM
	char	xmitdata[1536];			/* data for simplified mode */
#else
	Mbuf	xmbuf;					/* mbuf used for xmit */
	TBD		tbd[NTBD];				/* num. segments gathered for xmit */
	/*
	** hack to adjust CB size so CB can be placed in mbuf
	*/
	#define	TCB_SIZE_ADJ	(sizeof(Mbuf)+(sizeof(TBD)*NTBD))
	#define MBUF_MARKER		0x4d425546
#endif	/* PVTRAM */
};

/*
 * CB - Command block template
 */
typedef volatile struct ie_cb CB;
struct ie_cb {
	u_int16	cb_status;					/* status word */
		#define	CB_COMPLETE		0x8000	/* 1=command complete */
		#define CB_BUSY			0x4000	/* 1=command in progress */
		#define CB_OK			0x2000	/* 1=completed without error */
		#define CB_ABORT		0x1000	/* 1=command aborted */
		#define XMIT_LCOL		0x0800	/* 1=late collision on xmit */
		#define	XMIT_LCAR		0x0400	/* 1=no carrier sensed */
		#define XMIT_LCTS		0x0200	/* 1=lost clear to send */
		#define XMIT_UDMA		0x0100	/* 1=dma underrun */
		#define XMIT_DEFR		0x0080	/* 1=transmit deferred */
		#define XMIT_BEAT		0x0040	/* 1=CDT (heartbeat) active */
		#define XMIT_MTRY		0x0020	/* 1=exceeded max retries for xmit */
		#define XMIT_COLLMASK	0x000f	/* collision count field */
	u_int16	cb_command;					/* command word */
		#define CB_LAST			0x8000	/* last command in list */
		#define CB_SUSPEND		0x4000	/* suspend after this command */
		#define CB_IRQ			0x2000	/* generate IRQ after command */
		#define XMIT_NC			0x0010	/* 1=no CRC insertion enable */
		#define XMIT_SF			0x0008	/* 1=flexible, 0=simplified (sic) */
		#define CB_CMDMASK		0x0007	/* mask for command code */
		#define CB_CMD_NOP		0		/* NOP command */
		#define CB_CMD_IASETUP	1		/* Intermediate addr setup command */
		#define CB_CMD_CONFIG	2		/* CONFIG command */
		#define CB_CMD_MCSETUP	3		/* Multicast setup command */
		#define CB_CMD_XMIT		4		/* Transmit command */
		#define CB_CMD_TDR		5		/* TDR command */
		#define CB_CMD_DUMP		6		/* Dump internals command */
		#define CB_CMD_DIAG		7		/* Diagnose command */
	CB		*cb_leNext;	/* ptr to next command block (little end) */
	union {
		u_int8	ia[6];		/* ethernet address (for IA command) */
		TCB		tcb;		/* transmit command block */
	} cmd;
#if 0 /*defined (_MPFSH) */
	u_int8 cb_padding[2];
#endif
};

/*
 * SCB - system control block template
 */
typedef volatile struct ie_scb SCB;
struct ie_scb {
	u_int16	scb_status;					/* status word */
		#define SCB_STATUS_CX	0x8000  /* command complete interrupt */
		#define SCB_STATUS_FR	0x4000  /* finished receiving frame interrupt */
		#define SCB_STATUS_CNA	0x2000  /* CU left the active state */
		#define SCB_STATUS_RNR	0x1000  /* RU left the ready state */
		#define SCB_CU_ACTIVE	0x0200  /* CU in active state */
		#define SCB_CU_SUSPEND	0x0100  /* CU is suspended */
		#define SCB_CU_IDLE		0x0000	/* CU is idle */
		#define SCB_RU_NORBD	0x0080	/* RU is in no RBD state */
		#define SCB_RU_READY	0x0040  /* RU is in ready state */
		#define SCB_RU_NORES	0x0020	/* RU is in no resources state */
		#define SCB_RU_SUSPEND	0x0010  /* RU is suspended */
		#define SCB_RU_IDLE		0x0000  /* RU is idle */
		#define SCB_THROTTLE	0x0008	/* throttle timers loaded */
	u_int16	scb_cmd;					/* command word */
		#define SCB_CX_ACK			0x8000	/* ACK that CU completed an action cmd */
		#define SCB_FR_ACK			0x4000	/* ACK that RU received a frame */
		#define SCB_CNA_ACK			0x2000	/* ACK that CU became not active */
		#define SCB_RNR_ACK			0x1000	/* ACK that RU became not ready */
		#define SCB_CU_BUSTHRI		0x0600	/* load immediate new bus thrtl timers*/
		#define SCB_CU_BUSTHR		0x0500	/* load bus th. timers after current*/
		#define SCB_CU_ABORT_CMD	0x0400  /* abort CU cmd immediately */
		#define SCB_CU_SUSPEND_CMD	0x0300  /* suspend CU cmds after current */
		#define SCB_CU_RESUME_CMD	0x0200  /* resume CU cmds */
		#define SCB_CU_START_CMD	0x0100  /* start CU with first cmd in CBL */
		#define SCB_CU_NOP_CMD		0x0000  /* CU nop cmd */
		#define SCB_CHIP_RESET_CMD	0x0080  /* chip reset */
		#define SCB_RU_ABORT_CMD	0x0040  /* abort RU operation immediately */
		#define SCB_RU_SUSPEND_CMD	0x0030  /* suspn RU oper after curr frame */
		#define SCB_RU_RESUME_CMD	0x0020  /* resume RU operation */
		#define SCB_RU_START_CMD	0x0010  /* start RU operation */
		#define SCB_RU_NOP_CMD		0x0000  /* RU nop cmd */
	CB		*leCbl;			/* command block list ptr */
	RFD		*leRfa;			/* receive frame area ptr */
	u_int32	crcErrs;		/* CRC errors */
	u_int32	alignErrs;		/* alignment errors */
	u_int32	resErrs;		/* resource errors */
	u_int32	overrunErrs;	/* overrun errors */
	u_int32	rcvcdtErrs;		/* receive collisions errors */
	u_int32	sfErrs;			/* short frame errors */
	u_int16	toffTimer;		/* throttle t-off timer */
	u_int16	tonTimer;		/* throttle t-on timer */
	u_int8	cb_padd[8];		/* padd to 16-byte bound */
};

/*
 * ISCP - Intermediate System configuration pointer
 */
typedef volatile struct ie_iscp ISCP;
struct ie_iscp {
	u_int8		iscp_unused1;
	u_int8		iscp_busy;			/* busy indicator */
		#define	ISCP_NOT_BUSY	1
	u_int16		iscp_unused2;
	SCB			*iscp_leSCB;		/* (little end) scb pointer */
	SCB			*iscp_beSCB;		/* (big end) scb pointer */
	u_int8		iscp_padd[4];		/* padd to 16-byte bound */
};

/*
 * SCP - System configuration pointer
 */
typedef volatile struct ie_scp SCP;
struct ie_scp {
	u_int32	scp_sysbus;						/* configuration goodies */
		#define SYSBUS_EBE		0x00000080	/* enhanced big endian enable */
		#define SYSBUS_CSW		0x00000040	/* reserved */
		#define SYSBUS_INTLO	0x00000020	/* interrupt polarity */
		#define SYSBUS_INTHI	0x00000020	/* interrupt polarity */
		#define SYSBUS_LOCK		0x00000010	/* lock function */
		#define SYSBUS_TRG		0x00000008	/* bus throttle triggers */
		#define SYSBUS_MODEMASK	0x00000006	/* operation mode */
		#define SYSBUS_LINEAR	0x00000004	/* operation mode (linear) */
	u_int32	scp_unused2;
	ISCP	*scp_leISCP;					/* (little end) iscp pointer */
	ISCP	*scp_beISCP;					/* (big end) iscp pointer */
};

/*
 * template for selftest result
 */
typedef volatile struct str STR;
struct str {
	u_int32		str_romsig;			/* ROM signature */
	u_int32		str_result;			/* selftest result */
};


#endif /* _I82596_H */
