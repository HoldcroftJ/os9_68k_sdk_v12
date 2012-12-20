/*
 * definitions for ncr53c94 scsi interface device
 *
 * Copyright 1991 by Microware Systems Corporation
 * Reproduced Under License
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
 * 00 91/12/12 history starts.                                 AM
 *				---- OS-9/68K V3.0 Release ----
 *				---- OS-9/68K V3.1 Release ----
 */

/*----------------------------------------------------------------------+
|	Register map for the ncr53c94										|
+----------------------------------------------------------------------*/

typedef struct hardware {
	u_char	xfr_cnt_l;				/* Transfer count LSB */
	unsigned :8;
	u_char	xfr_cnt_u;				/* Transfer count MSB */
	unsigned :8;
	u_char	fifo;					/* Data FIFO */
	unsigned :8;
	u_char	command;				/* Command register */
	unsigned :8;
	u_char	status;					/* Status register (Read) */
#define destbusid status			/* Destination bus ID (Write) */
	unsigned :8;
	u_char	interrupt;				/* Interrupt register (Read) */
#define timeout interrupt			/* Select/reselect timeout (Write) */
	unsigned :8;
	u_char	seqstep;				/* Sequence step (Read) */
#define syncperiod seqstep			/* Synchronous period (Write) */
	unsigned :8;
	u_char	fifoflags;				/* FIFO flags/Sequence step 2 (Read) */
#define syncoffset fifoflags		/* Synchronous offset (Write) */
	unsigned :8;
	u_char	config1;				/* Configuration reg 1 */
	unsigned :8;
	u_char	clkconv;				/* Clock conversion factor */
	unsigned :8;
	u_char	testmode;				/* Test mode register */
	unsigned :8;
	u_char	config2;				/* Configuration reg 2 */
	unsigned :8;
	u_char	config3;				/* Configuration reg 3 */
	unsigned :8;
	u_char	resvfifo;				/* Reserve FIFO byte (config 2) */
	unsigned :8;
} *Hardware;

#ifndef MAXDMA
#define MAXDMA (1<<16)
#else
#if MAXDMA > (1<<16)
#undef MAXDMA
#define MAXDMA (1<<16)
#endif
#endif

/*----------------------------------------------------------------------+
|	Commands for the NCR53c94 interface chip							|
+----------------------------------------------------------------------*/

/* miscellaneous group */
#define NOP				0x00		/* No operation */
#define FLUSH_FIFO		0x01		/* Flush FIFO */
#define RESET_CHIP		0x02		/* Reset NCR 53c94 device */
#define RESET_SCSI		0x03		/* Reset SCSI bus */

/* disconnected state group */
#define RESELECT		0x40		/* Reselect sequence */
#define SEL_NOATN		0x41		/* Select without ATN sequence */
#define SEL_ATN			0x42		/* Select with ATN sequence */
#define SEL_ATN_STOP	0x43		/* Select with ATN and stop sequence */
#define ENABLE_SEL		0x44		/* Enable selection/reselection */
#define DISABLE_SEL		0x45		/* Disable selection/reselection */
#define SEL_ATN3		0x46		/* Select with ATN3 */

/* target state group */
#define SEND_MSG		0x20		/* Send message */
#define SEND_STATUS		0x21		/* Send status */
#define SEND_DATA		0x22		/* Send data */
#define DISCONN_SEQ		0x23		/* Disconnect sequence */
#define TERMINATE		0x24		/* Terminate sequence */
#define T_COMPLETE		0x25		/* Target command complete sequence */
#define DISCONNECT		0x27		/* Disconnect */
#define RECV_MSG		0x28		/* Receive message sequence */
#define RECV_COMMAND	0x29		/* Receive command */
#define RECV_DATA		0x2A		/* Receive data */
#define RECV_CMD_SEQ	0x2B		/* Receive command sequence */
#define ABORT_DMA		0x04		/* Target abort DMA */

/* initiator state group */
#define XFER_INFO		0x10		/* Transfer information */
#define I_COMPLETE		0x11		/* Initiator command complete sequence */
#define MSG_ACCEPT		0x12		/* Message accepted */
#define XFER_PAD		0x18		/* Transfer PAD */
#define SET_ATN			0x1A		/* Set ATN */
#define RESET_ATN		0x1B		/* Reset ATN */

/* DMA flag */
#define DMA				0x80		/* Use DMA for this transfer */

/*----------------------------------------------------------------------+
|	Status register														|
+----------------------------------------------------------------------*/

#define SR_IRQ			(1<<7)			/* Interrupt generated */
#define SR_GERR			(1<<6)			/* Gross error */
#define SR_PERR			(1<<5)			/* Parity error */
#define SR_TCNT			(1<<4)			/* Terminal count */
#define SR_VGC			(1<<3)			/* Valid group code */
#define SR_PHASE		(7<<0)			/* Phase code */
#define PH_MSG			(1<<2)			/* Message bit */
#define PH_CD			(1<<1)			/* Command/Data bit */
#define PH_IO			(1<<0)			/* Input/Output bit */
#define PH_DATAOUT		(0<<0)			/* Data out phase */
#define PH_DATAIN		(1<<0)			/* Data in phase */
#define PH_COMMAND		(2<<0)			/* Command phase */
#define PH_STATUS		(3<<0)			/* Status phase */
#define PH_MSGOUT		(6<<0)			/* Message out phase */
#define PH_MSGIN		(7<<0)			/* Message in phase */

/*----------------------------------------------------------------------+
|	Interrupt register													|
+----------------------------------------------------------------------*/

#define IR_SCSIRST		(1<<7)			/* SCSI reset detected */
#define IR_ILLCMD		(1<<6)			/* Illegal command */
#define IR_DISCONN		(1<<5)			/* Disconnect received */
#define IR_BUSSVC		(1<<4)			/* Bus service requested */
#define IR_FUNCMPLT		(1<<3)			/* Function complete */
#define IR_RESELECT		(1<<2)			/* Reselect received */
#define IR_SELATN		(1<<1)			/* Select with ATN received */
#define IR_SELECT		(1<<0)			/* Select without ATN received */

/*----------------------------------------------------------------------+
|	Timeout register													|
+----------------------------------------------------------------------*/

#define TIMEOUT			122				/* .25sec timeout @ 16 MHz */

/*----------------------------------------------------------------------+
|	Sequence register													|
+----------------------------------------------------------------------*/

#define SQ_SOM			(1<<3)			/* Synchronous offset max */
#define SQ_STEP			(7<<0)			/* Sequence step */

/*----------------------------------------------------------------------+
|	Synchronous transfer period register								|
+----------------------------------------------------------------------*/

/* for clocks/byte (cb) from 5 to 35, write (cb & 31) */

/*----------------------------------------------------------------------+
|	FIFO flags register													|
+----------------------------------------------------------------------*/

#define FIFO_SEQ		(7<<5)			/* Sequence step (copy of) */
#define FIFO_CNT		(31<<0)			/* FIFO data count */

/*----------------------------------------------------------------------+
|	Synchronous offset register											|
+----------------------------------------------------------------------*/

#define SO_ASYNC		0				/* Use asynchronous mode */

/*----------------------------------------------------------------------+
|	Configuration register 1											|
+----------------------------------------------------------------------*/

#define CR1_SLOWCBL		(1<<7)			/* Slow cable mode */
#define CR1_SRID		(1<<6)			/* SCSI reset interrupt disable */
#define CR1_PARTEST		(1<<5)			/* Parity test mode */
#define CR1_PARENABL	(1<<4)			/* Enable parity checking */
#define CR1_TESTENABL	(1<<3)			/* Enable chip test mode */
#define CR1_MYBUSID		(7<<0)			/* My SCSI bus ID */

/*----------------------------------------------------------------------+
|	Clock conversion register											|
+----------------------------------------------------------------------*/

#define CLK_10			2				/* Up to 10 MHz */
#define CLK_15			3				/* 10 - 15 MHz */
#define CLK_20			4				/* 15 - 20 MHz */
#define CLK_25			5				/* 20 - 25 MHz */

/*----------------------------------------------------------------------+
|	Test register														|
+----------------------------------------------------------------------*/

#define TR_HIZ			(1<<2)			/* All outputs to Hi Impedance */
#define TR_INIT			(1<<1)			/* Force initiator mode */
#define TR_TARG			(1<<0)			/* Force target mode */

/*----------------------------------------------------------------------+
|	Configuration register 2											|
+----------------------------------------------------------------------*/

#define CR2_RFB			(1<<7)			/* Reserve FIFO byte */
#define CR2_EPL			(1<<6)			/* Enable phase latch */
#define CR2_EBC			(1<<5)			/* Enable byte control */
#define CR2_DREQHIZ		(1<<4)			/* Set DREQ to Hi-Z */
#define CR2_SCSI2		(1<<3)			/* Enable SCSI-2 mode */
#define CR2_TBPA		(1<<2)			/* Target bad parity abort */
#define CR2_RPE			(1<<1)			/* Register parity enable */
#define CR2_DMAPE		(1<<0)			/* DMA parity enable */

/*----------------------------------------------------------------------+
|	Configuration register 3											|
+----------------------------------------------------------------------*/

#define CR3_SRB			(1<<2)			/* Save residual byte */
#define CR3_ALTDMA		(1<<1)			/* Alternate DMA mode */
#define CR3_THRESH8		(1<<0)			/* Set data threshold to 8 */

/* end of file */
