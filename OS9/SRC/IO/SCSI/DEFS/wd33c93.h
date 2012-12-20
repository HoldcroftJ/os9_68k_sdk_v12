/*
 * definitions for wd33c93 scsi interface device
 *
 * Copyright 1988, 1989, 1990, 1991, 1995 by Microware Systems 
 * Corporation.  Reproduced Under License.
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 * history:
 *
 * ed.  date   reason.                                         who.
 * -- -------  ----------------------------------------------  ---
 * 00 88/10/29 history starts.                                 wwb
 * 01 88/10/31 updated somewhat.                               wwb
 * 02 88/11/10 removed dma definitions to "dma.h".             wwb
 * 03 88/12/20 added vme620 defs.                              wwb
 * 03 89/02/17 Fixed size of "res2" & "res3" in VME620 defs    jal
 * 04 89/05/15 added synchronous transfer register defs.       wwb
 * 05 89/12/15 added corrected definition of command code 0x3  Rwb
 * 06 90/05/31 added phase register definitions                Rwb
 *             ---- OS-9/68k V2.4 Release ----
 * 07 91/03/21 added definitions for the wd33c93A part         Rwb
 *             ---- OS-9/68k V2.4.3 Release ----
 *             ---- OS-9/68K V3.0 Release ----
 * 08 95/02/14 added volatile declaration for ANSI mode        rws
 *             ---- OS-9/68K V3.0.2 Release ----
 *             ---- OS-9/68K V3.1 Release ----
 */
 
/*  NOTE:  The definitions that are highlighted with an "(A)" are
 *          relevant to the wd33c93a only.  These bits/functions
 *          were undefined or unavailable with the earlier parts.
 */


#if !defined  (__STDC__) && !defined (volatile)
#define volatile /* the old compiler doesn't understand 'volatile' */
#endif


#ifdef VME147
/* register map for the wd33c93 indirect addressing */ 
typedef volatile struct hardware {
	u_char		address_reg,		/* address/auxstatus register */
				reg_file;			/* register file access */
} *Hardware;
#endif

#ifdef VME620
typedef volatile struct hardware {
	u_char res1;					/* first regster at 0x01 */
	u_char int_vect;				/* interrupt vector */
	u_char res2[0x3F];				/* more blank space */
	u_char control;					/* control/status register */
	u_char res3[0x3E];				/* even more blank space */
	u_char dma_init;				/* DMA - initialization register (w/o) */
	u_char dma_channel;				/* DMA - channel register (R/W) */
	u_char dma_cntlsb;				/* DMA - byte count (lsb) */
	u_char dma_cntmsb;				/* DMA - byte count (msb) */
	u_char dma_addrlsb;				/* DMA - address register (lsb) */
	u_char dma_addrnsb;				/* DMA - address register middle */
	u_char dma_addrmsb;				/* DMA - address register (msb) */
	u_char res4;					/* space byte (unused by us) */
	u_char dma_control;				/* DMA - primary control register */
	u_char dma_control2;			/* DMA - control register */
	u_char dma_mode;				/* DMA - mode register */
	u_char dma_status;				/* DMA - status register */
	u_char res5[0x2];				/* two space bytes (unused by us) */
	u_char dma_request;				/* DMA - request register */
	u_char dma_mask;				/* DMA - mask register */
	u_char res6[0x30];				/* yet even more "dead" space */
	u_char address_reg;				/* SCSI - address register */
	u_char reg_file;				/* SCSI - register file access byte */
} *Hardware;
#endif

/*
 * indirect register addresses for the wd33c93.
 * to access a register in the indirect mode, the address of the
 * register is written to the "address_reg", and then the target
 * register is read/written via the "reg_file".
 */

#define OWNID 			0x00		/* wd33c93 id on scsi bus */
#define CONTROL	 		0x01		/* control register */
#define TIMEOUT			0x02		/* timeout period register */
#define CDB1			0x03		/* command descriptor block, byte 1 */
#define CDB2			0x04		/* command descriptor block, byte 2 */
#define CDB3			0x05		/* command descriptor block, byte 3 */
#define CDB4			0x06		/* command descriptor block, byte 4 */
#define CDB5			0x07		/* command descriptor block, byte 5 */
#define CDB6			0x08		/* command descriptor block, byte 6 */
#define CDB7			0x09		/* command descriptor block, byte 7 */
#define CDB8			0x0a		/* command descriptor block, byte 8 */
#define CDB9			0x0b		/* command descriptor block, byte 9 */
#define CDB10			0x0c		/* command descriptor block, byte 10 */
#define CDB11			0x0d		/* command descriptor block, byte 11 */
#define CDB12			0x0e		/* command descriptor block, byte 12 */
#define TARGLUN			0x0f		/* target lun/target status */
#define CMDPHASE		0x10		/* command phase */
#define SYNCXFER		0x11		/* synchronous transfer control */
#define COUNT_MSB		0x12		/* transfer count msb */
#define COUNT_NSB		0x13		/* transfer count */
#define COUNT_LSB		0x14		/* transfer count lsb */
#define DESTID			0x15		/* target scsi id */
#define SRCID			0x16		/* source id */
#define STATUS			0x17		/* scsi status */
#define COMMAND			0x18		/* command register */
#define DATA			0x19		/* data register */

/* commands for the WD 33c93 interface chip */

#define RESET 			0x00		/* reset wd33c93 device */
#define	ABORT			0x01		/* abort current command */
#define	ATN				0x02		/* assert ATN */
#define NOACK           0x03        /* negate ACK */
#define DISCONNECT		0x04		/* disconnect */
#define	RESELECT		0x05		/* reselect */
#define	SEL_ATN			0x06		/* select with ATN */
#define	SEL_NOATN		0x07		/* select without ATN */
#define	SEL_ATN_XFR		0x08		/* select with ATN and transfer */
#define SEL_NOATN_XFR	0x09		/* select without ATN and transfer */
#define	RESEL_RX		0x0a		/* reselect and receive data */
#define RESEL_TX		0x0b		/* reselect and send data */
#define WAIT_SEL_RX		0x0c		/* wait for select and receive */
#define	RX_CMD			0x10		/* receive command */
#define RX_DATA			0x11		/* receive data */
#define	RX_MSGOUT		0x12		/* receive message out */
#define RX_UNSPEC		0x13		/* receive unspecified info out */
#define	TX_STATUS		0x14		/* send status */
#define	TX_DATA			0x15		/* send data */
#define	TX_MSGIN		0x16		/* send message in */
#define	TX_UNSPEC		0x17		/* send unspecified message in */
#define	TRANS_ADDR		0x18		/* translate address */
#define	XFR_INFO		0x20		/* transfer info */
#define	XFR_PAD			0x21		/* transfer pad */

/* auxiliary status register flags */

#define AUX_IRQ			(1<<7)		/* interrupt request pending */
#define	AUX_LCI			(1<<6)		/* last command ignored */
#define AUX_BUSY		(1<<5)		/* level II command in progress */
#define AUX_CIP			(1<<4)		/* command in progress */
#define AUX_PE			(1<<1)		/* scsi parity error */
#define AUX_RDY			(1<<0)		/* data buffer ready */


/*  vvvvvv  (A)  version part only vvvvvv  */
/* own ID/CDB size register */
/*  in the pre-33c93a chip, this is own ID only.  In the A version part
 *  this register has several important bits that activate the enhanced 
 *  features of the 'a' version part.
 */
#define OI_IDM		0x7		/* mask for the ID bits in the register */
#define OI_FSM		0xC0	/* mask for frequency select divisor */

/* this bit enables the advanced features of the chip.  If set at the
 *  time that a "RESET" command is issued, the return status will be 
 *  0x1 rather than  0x0 thus identifying the part as version 'a'.
 */
#define OI_EAF      (1<<3)	/* Enables advanced features */
#define OI_EHP		(1<<4)	/* Enable Host Parity (host bus parity) */

/* NOTE:  clock below refers to WD33C93 input clock, not the CPU clock! */
#define OI_FREQ8	(0x0)		/* selects divisor 2 for 8-10 MHZ clock */
#define OI_FREQ12	(1<<6)		/* selects divisor 3 for 12-15 MHz clock */
#define OI_FREQ16	(2<<6)		/* selects divisor 4 for 16 MHz clock */

#ifdef VME620
/* The VME620 has an 8 MHz SCSI clock for an input */
#define OI_DEFAULT  OI_FREQ8 | OI_EAF
#else
#ifdef VME147
/* the mVme147 card has a 10MHz SCSI clock */
#define OI_DEFAULT  OI_FREQ8 | OI_EAF
#endif
#endif

/*  ^^^^^^  (A)  version part only ^^^^^^  */

/* control register flags */

#define	CR_DMA			(1<<7)		/* dma select on */
#define	CR_WDB			(1<<6)		/* WD-bus select */
#define CR_BURST		(1<<5)		/* enables burst for demand-mode DMA (A) */
#define CR_HHP          (1<<4)		/* halt on host parity error (A) */
#define CR_EDI			(1<<3)		/* if TRUE, holds 0x16 int off  (A) */
#define CR_IDI			(1<<2)		/* intermediate disconnect interrupt */	
#define	CR_HA			(1<<1)		/* halt on attention */
#define CR_HPE			(1<<0)		/* halt on parity error */

#define CR_MODE			0			/* default control reg mode */

/* synchronous transfer register flags */
/*  Notes concerning offset field of this register:
 *     The (A) version of this part expands the on-chip fifo from 5 to 
 *     12 bytes.  The early versions of the chip had a bug which decreased
 *     this fifo to an effective 4 bytes.  The (A) version of the part has
 *     an additonal bit to describe the offset register value.
 */
#define CHIPFIFO        4           /* really 5, but silicon bug sets limit */
#define ACHIPFIFO       12          /* should work just fine */
#define	ST_TPX			(7<<3)		/* transfer period mask */
#define	ST_OFX			(7<<0)		/* offset value */
#define ST_OFXA			(0xF<<0)	/* offset mask for (A) version part */

#define	ST_ASYNC		0			/* asynchronous mode */

/*
 * synchronous mode parameters:
 *   transfer period = 6
 *   offset = 4
 */
#define	ST_SYNC			((6<<3)|(4))	/* synchronous mode */

/* destination id register */
#define DI_SCC			(1<<7)		/* Select Command Chain (A) */
#define DI_DPD			(1<<6)		/* Data Phase direction, TRUE = in (A) */
#define DI_DMSK			(0x7)		/* destination ID mask bits */

/* source id register flags */
#define	SI_ER			(1<<7)		/* enable reselection */
#define	SI_ES			(1<<6)		/* enable selection */
#define	SI_VALID		(1<<3)		/* source id valid */
#define SI_MASK			0x7			/* source id mask */

/* target LUN register  (A)  */
#define TL_TLV			(1<<7)		/* target LUN valid */
#define TL_DOK			(1<<6)		/* disconnect allowed (target moded) */
#define TL_TLM			(0x7)		/* LUN mask */

/* command register values */

#define	CM_SBT			(1<<7)		/* single byte transfer mode */
#define	CM_CMDMASK		0x7f		/* command code mask */

/* status register values */

#define	GROUP4			(1<<7)		/* another device needs service */
#define	GROUP3			(1<<6)		/* terminated interrupt */
#define	GROUP2			(1<<5)		/* pause or aborted command */
#define	GROUP1			(1<<4)		/* command completed successfully */
#define GPX_MASK		0xf0		/* group 'x' mask */

/* group 4:  another device needs service */

#define	GP4_REQ			(1<<3)		/* REQ asserted: look at bits 2 - 0 */
#define	GP4_DISCON		0x5			/* disconnect occurred */
#define	GP4_ATN			0x4			/* ATN asserted */
#define	GP4_SEL_ATN		0x3			/* selected with ATN */
#define	GP4_SEL_NOATN	0x2			/* selected without ATN */
#define	GP4_RESEL		0			/* reselected */

/* group 3:  terminated interrupt */

#define	GP3_UNEXPECT	(1<<3)		/* unexpected info phase */
#define	GP3_INCORRECT	0x7			/* incorrect message/status/cmd */
#define	GP3_BADTARG		0x6			/* wrong target reselected */
#define	GP3_VOLOF		0x5			/* logical address exceeded disk size */
#define	GP3_PEATN		0x4			/* parity error with ATN */
#define	GP3_PENOATN		0x3			/* parity error without ATN */
#define	GP3_TIMEOUT		0x2			/* timeout during select or reselect */
#define	GP3_DISCON		0x1			/* unexpected disconnect */
#define	GP3_INVAL		0			/* invalid command */

/* group 2:  pause or aborted command */

#define	GP2_XFR_ABORT	(1<<3)		/* transfer cmd aborted */
#define	GP2_XFR_ATN		0x4			/* rx/tx cmd aborted with ATN */
#define	GP2_XFR_NOATN	0x3			/* rx/tx cmd aborted without ATN */
#define	GP2_SEL_ABORT	0x2			/* select/reselect cmd aborted */
#define	GP2_SAVEDP		0x1			/* save data pointers */
#define	GP2_XFRPAU		0			/* transfer cmd paused with ACK */

/* group 1:  successful completion */

#define	GP1_XFR			(1<<3)		/* transfer command completed */
#define	GP1_SELXFR		0x6			/* select and transfer completed */
#define	GP1_TRANS		0x5			/* translate address completed */
#define	GP1_XFR_ATN		0x4			/* receive, send, reselect-and-transfer
									   or wait-for-select-and-transfer
									   completed with ATN */
#define	GP1_XFR_NOATN	0x3			/* receive, send, reselect-and-transfer
									   or wait-for-select-and-transfer
									   completed without ATN */
#define	GP1_SEL			0x2			/* select completed */
#define	GP1_RESEL		0x1			/* reselect completed */

/* group 0:  reset by command or power-up */

#define	GP0_RESET		0			/* reset by command or power-up */
#define	GP0_RESETA		0x1			/* as above for the wd33c93A with EAF */

/* command phase register values:  register set a various times to
 *                                 allow the internal chip state to 
 *                                 continue.
 */
#define PH_SELFAIL 0x0		/* selection failed */
#define PH_SELCOMP 0x10		/* selection complete */
#define PH_IDSENT  0x20		/* ID message sent */
#define PH_CDBSTXF 0x30		/* start of CDB transfer */
#define PH_CDB6XF  0x36		/* 6 CDB transfers made */    /* CDB = cmd blk */
#define PH_CDB10XF 0x3A		/* 10 CDB transfers made */
#define PH_CDB12XF 0x3C		/* 12 CDB transfers made */
#define PH_DCTREC  0x42		/* disconnect message received */
#define PH_TARGDCT 0x43		/* target disconnected */
#define PH_OTARGCT 0x44		/* original target connected */
#define PH_IDMOK   0x45		/* correct ID message received */
#define PH_DXFRDN  0x46		/* data transfer complete */
#define PH_STRECV  0x50		/* status byte recieved */
#define PH_CMDCPLT 0x60		/* command complete message recieved */



