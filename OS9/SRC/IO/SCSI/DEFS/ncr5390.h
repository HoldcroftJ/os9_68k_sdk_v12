/*
 * definitions for NCR 53c90 SCSI Protocol Controller
 *
 * Copyright 1989 by Microware Systems Corporation
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
 * 00 89/11/20 starting at the beginning.					   kse
 *				---- OS-9/68K V3.0 Release ----
 */

/* register map for the 5390 */
#ifdef VPU30				/* fix for buswidth = 2 and start at 1 */
typedef struct hardware {
	u_char		byt0,		/* unused byte */
				tcl,byt2,	/* transfer counter low */
				tch,byt4,	/* transfer counter high */
				fifo,byt6,	/* internal 16 byte fifo (R/W) */
				cmdr,byt8,	/* command */
				stat,byta,	/* status (R) / s/r bus id (W) */
				intst,bytc,	/* interrupt status (R) / s/r timeout (W) */
				seqst,byte,	/* sequence step (R) / sync period (W) */
				fifof,byt10,/* fifo flags (R) / sync offset (W) */
				conf,byt12,	/* configuration */
				clcv		/* clock conversion factor (W) */
} *Hardware;
#else
typedef struct hardware {
	u_char		tcl,		/* transfer counter low */
				tch,		/* transfer counter high */
				fifo,		/* internal 16 byte fifo (R/W) */
				cmdr,		/* command */
				stat,		/* status (R) / s/r bus id (W) */
				intst,		/* interrupt status (R) / s/r timeout (W) */
				seqst,		/* sequence step (R) / sync period (W) */
				fifof,		/* fifo flags (R) / sync offset (W) */
				conf,		/* configuration */
				clcv		/* clock conversion factor (W) */
} *Hardware;
#endif
/* equates for register bits */

/* CMDR register */
#define dma_b	0x80		/* enable dma */

	/* misc commands */
#define nop_b	0x00		/* nop */
#define flfif_b	0x01		/* flush fifo */
#define rchip_b	0x02		/* reset chip */
#define rscsi_b	0x03		/* reset scsi bus */
	/* disconnected commands */
#define reseq_b	0x40		/* reselected sequence */
#define swoat_b 0x41		/* select with out ATN sequence */
	/* initiator commands */
#define trans_b	0x10		/* transfer information */
#define ccsq_b	0x11		/* command complete sequence */
#define macpt_b	0x12		/* message accepted */
#define tpad_b	0x98		/* transfer pad (must use dma) */
#define statn_b	0x1a		/* set atn */

/* STAT register */
#define	grs_b	0x40		/* gross error */
#define par_b	0x20		/* parity error */
#define trcz_b	0x10		/* transfer count zero */
#define trcm_b	0x08		/* transfer complete */

/* phase states based on STAT 2-0 */
#define data_out	0x00	/* data out phase */
#define data_in		0x01	/* data in phase */
#define command		0x02	/* command phase */
#define status		0x03	/* status phase */
#define mesg_out	0x06	/* message out phase */
#define mesg_in		0x07	/* message in phase */

/* INTST register */
#define scrst_b		0x80	/* scsi reset detected */
#define ileg_b		0x40	/* illegal command */
#define disc_b		0x20	/* disconnect */
#define buss_b		0x10	/* bus service */
#define fncm_b		0x08	/* function complete */
#define resl_b		0x04	/* reselected */
#define satn_b		0x02	/* selected with atn */
#define sel_b		0x01	/* selected */

/* CONF register */
#define slcb_b		0x80	/* slow cable mode */
#define srid_b		0x40	/* scsi reset int disable
#define part_b		0x20	/* parity test mode */
#define pare_b		0x10	/* parity enable */
#define chtm_b		0x08	/* chip test mode */
