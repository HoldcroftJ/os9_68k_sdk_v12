/*
 * definitions for Archive 2150S/2060S Viper scsi Tape controller
 */

/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1998 by Microware Systems Corporation              |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, modification, distribution, or creation of    |
|  derivative works in any form to any party other than the Licensee is     |
|  strictly prohibited, unless expressly provided for under the terms of a  |
|  mutually executed written license agreement for this software between    |
|  the end-user and Microware Systems Corporation, or otherwise expressly   |
|  authorized in writing by Microware Systems Corporation.  Licensee shall  |
|  reproduce this copyright notice exactly as provided herein on any copies |
|  of the software that Licensee is authorized to distribute.               |
|---------------------------------------------------------------------------|
 * history:
 *
 * ed.   date    reason.                                           who.
 * --  --------  ------------------------------------------------  ---
 * 00  88/10/20  today we write a tape driver...                   wwb
 * 01  88/11/03  removed file-mark/block counters.                 wwb
 * 02  88/11/06  fixed error detail structure.                     wwb
 * 03  88/11/07  added residual length valid flag to static.       wwb
 * 04  88/12/03  added eom status to fstatus.                      wwb
 * 05  89/05/12  added optflags for pd_scsiopt.                    wwb
 * 06  89/08/10  added SLEEP_TIME value, removed E_HDWARE and      wwb
 *               replaced with errno.h's E_HARDWARE.
 * 07  90/04/06  made all static storage structures be long-word   wwb
 *               aligned. This is an issue for dma-driven low-
 *               level modules:  some systems can not handle
 *               non-aligned transfers, so the high-level module
 *               needs to ensure that its internal structures
 *               are safely aligned (JIC: just in case!).
 *               Removed enquiry structure storage (not used).     wwb
 * 09  90/04/10  Added definitions for direct command interface    Rwb
 * 10  90/06/06  Added sense code defs                             Rwb
 * 11  90/06/15  added CONTR_SUPPORT qualification to pd_scsiopt.  wwb
 * 12  91/03/04  added vender specific fields for Exabyte          Rwb
 * 13  93/05/19  Modified for UCC compatibility mode. Collected    ats
 *               all driver statics into a single record.
 *				 ---- OS-9/68K V3.0 released ----
 * 14  94/07/18  Removed CONTR_SUPPORT checks (ed11) - checks are  wwb
 *               now normally performed by low-level.
 *               ---- OS-9/68K V3.0.1 Release ----
 * 15  95/06/08  Corrected pathname case of reg.h.                 ats
 *               ---- OS-9/68K V3.0.2 Release ----
 * 16  98/07/08  reworked static storage naming for UCC2.X         wwb
 * 17  98/09/08  Added new copyright info.                         wwb
 *               ---- OS-9/68K V3.1 Release ----
 *     $$                    <RELEASE_INFO>                     $$  |
 */

/*
 * driver limitations:
 * this version of the driver can only handle one "logical"
 * drive per driver incarnation.  If it is desired to have
 * a scsi bus with more than one "sbviper" Tape device
 * connected, it is suggested that the port address of the
 * device descriptors be adjusted so that each tape device
 * descriptor has a unique address:
 * e.g. (PortAddr & 0xfffffff0) | scsi_id
 */
#define		SBF_MAXDRIVE	1		/* only one controller per "driver" */

#include	<types.h>
#include	<sbf.h>
#include	<MACHINE/reg.h>
#include	<procid.h>
#include	<path.h>
#include	<errno.h>
#include	<sg_codes.h>
#include	<cmdblk.h>
#include	<scsidmod.h> 
#include    <dcmd.h>
#include    <modes.h>
#include	<scsicmds.h>


/*
 * scsi 6 byte command packet for tape controller
 */
typedef struct scsipkt_str {
	u_char		sp_opcode;			/* the opcode */
	unsigned	sp_lun		:3,		/* the logical unit number */
				sp_action	:5;		/* command dependant */
	char		sp_blkcnt[3];		/* the block address */
	u_char		sp_options;			/* command options */
} scsipkt;

typedef scsipkt *Scsipkt;			/* pointer to a packet */

/*
 * extended error details structure
 */
 
typedef struct errdet_str {
	unsigned		ed_valid	: 1,	/* error is valid */
					ed_class	: 3,	/* error class = 7 */
					ed_zero		: 4;	/* always 0 */
	u_char			ed_seg;				/* segment number always 0 */
	unsigned		ed_filemrk	: 1,	/* filemark */
					ed_eom		: 1,	/* end of medium */
					ed_ili		: 1,	/* incorrect length indicator */
					ed_zero2	: 1,	/* always zero */
					ed_main		: 4;	/* main sense key */
	u_char			ed_info[4];			/* info byte [lba] */
	u_char			ed_senslen;			/* additional sense info length */
	u_char			ed_cpsrc;			/* COPY source sense data ptr */
	u_char			ed_cpdest;			/* COPY destination sense data ptr */
	u_char			ed_zero3[2];		/* always zero */
	u_char			ed_recov[2];		/* # recoverable errors */
	u_char			ed_cptstat;			/* COPY target status */
	u_char			ed_cptsens[8];		/* COPY target sense data (0-7) */
} errdet;

typedef errdet *Errdet;			/* pointer to error detail packet */

/*
 * inquiry data structure
 */

typedef struct inqdet_str {
	u_char			iq_type;			/* device type */
	unsigned		iq_rmb		: 1,	/* removable media = 1*/
					iq_typqual	: 7,	/* device type qualifier */
					iq_iso		: 2,	/* ISO version */
					iq_ecma		: 3,	/* ECMA version */
					iq_ansi		: 3;	/* ANSI version */
	u_char			iq_zero;			/* always 0 */
	char			iq_addlen,			/* additional length */
					iq_zero2[3],		/* always zero */
					iq_vendid[8],		/* vendor id */
					iq_prodid[16],		/* product id */
					iq_revlev[4];		/* revision level */
} inqdet;

typedef inqdet *Inqdet;			/* pointer to inquiry data packet */

/*
 * mode select/sense data structure
 */
typedef struct modesel_str {
	u_char			ms_res1;			/* reserved: (0) */
	u_char			ms_res2;			/* reserved: (0) */
	unsigned		ms_res3		: 1,	/* reserved: (0) */
					ms_buffmode	: 3,	/* buffer mode */
					ms_speed	: 4;	/* speed = 0 */
	u_char			ms_bdlen;			/* block descriptor length: (8) */
	u_char			ms_denscode;		/* density code */
	u_char			ms_numblks[3];		/* number of blocks: (0) */
	u_char			ms_res4;			/* reserved: (0) */
	u_char			ms_blklen[3];		/* block length */
#ifdef EXABYTE
	unsigned		ms_ct		:1,		/* Cassette type */
	        		ms_res5		:4,		/* reserved (0) */
#define EVEN_BYTE_DISCONNECT TRUE
#define EVEN_ODD_DISCONNECT  FALSE
	        		ms_ebd		:1,		/* even/odd byte boundry disconnect */
	        		ms_pce		:1,		/* parity check enable */
	        		ms_nal		:1;		/* auto cassette load disable */
	u_char 			ms_res6;			/* yet another reserved field */
#define DEFAULT_MOTION 0xF0
	u_char 			ms_mtnthr;			/* motion threshhold */
#define DEFAULT_RECON  0x40
	u_char			ms_rctthr;			/* reconnect threshold */
#endif EXABYTE
} modesel;

typedef modesel *Modesel;

/*
 * sbf driver static storage definitions
 */
#ifndef driver_static
#define driver_static extern
#endif

/* caring instructions:  long-word alignment of pointers, etc.
 * results in optimum performance.  Also, any "internal" structures
 * should be padded to start on a long-word boundary.  This ensures
 * that this high-level driver will work with ANY DMA-driven low-level
 * module (some hardware implementations cannot handle un-aligned
 * transfers.
 */
#ifdef _UCC
typedef struct {
	struct sbfstatic	ds_sbfstat;	/* SBF required static */
	Pathdesc            ds_pathdesc;	/* path descriptor (stat only)*/
	void				*ds_sys_stat;	/* system globals ptr */
	Sbfstatic			ds_stat;		/* driver static storage ptr */
	Datmod				ds_data_mod;	/* low level data module */
	void				*ds_drvr_mod;	/* low level driver module */
	procid				*ds_procdesc;	/* process descriptor ptr */
	int					ds_blk_size;	/* Block size */
	int					ds_lbsize;		/* Logical block size */
	int					ds_residual;	/* residual length valid */
	int					ds_fstatus;	/* file status */
	u_int				ds_optflags;	/* scsi options flags */
	struct cmdblk		ds_cmdblk;		/* command block */
	u_char				ds_scsiid;		/* target controller id */
	u_char				ds_scsilun;	/* target lun */
	scsipkt				ds_scsicmd;	/* SCSI command packet */
	u_char				ds_jicpad1[2];	/* make next long-align */
	errdet				ds_err_det;	/* error details block */
	u_char				ds_online;		/* drive online status */
	modesel				ds_mode_sel;	/* mode select block */
} viperstatics, *ViperStatics;

driver_static	viperstatics	driver_statics;

#ifndef		NO_STATICS_SHORTCUTS
#define		sbfstat		driver_statics.ds_sbfstat
#define		pathdesc	driver_statics.ds_pathdesc
#define		sys_stat	driver_statics.ds_sys_stat
#define		stat		driver_statics.ds_stat
#define		data_mod	driver_statics.ds_data_mod
#define		drvr_mod	driver_statics.ds_drvr_mod
#define		procdesc	driver_statics.ds_procdesc
#define		blk_size	driver_statics.ds_blk_size
#define		lbsize		driver_statics.ds_lbsize
#define		residual	driver_statics.ds_residual
#define		fstatus		driver_statics.ds_fstatus
#define		optflags	driver_statics.ds_optflags
#define		cmdblk		driver_statics.ds_cmdblk
#define		scsiid		driver_statics.ds_scsiid
#define		scsilun		driver_statics.ds_scsilun
#define		scsicmd		driver_statics.ds_scsicmd
#define		err_det		driver_statics.ds_err_det
#define		online		driver_statics.ds_online
#define		mode_sel	driver_statics.ds_mode_sel
#endif		NO_STATICS_SHORTCUTS

#else

driver_static	struct sbfstatic	sbfstat;	/* SBF required static */
/* the above definition allocates the SBF global static storage
 * AND the SBF drive tables:  it must be the first definition.
 * 'sbf.h' holds the declarations.
 */
driver_static   Pathdesc            pathdesc;	/* path descriptor (stat only)*/
driver_static	void				*sys_stat;	/* system globals ptr */
driver_static	Sbfstatic			stat;		/* driver static storage ptr */
driver_static	Datmod				data_mod;	/* low level data module */
driver_static	void				*drvr_mod;	/* low level driver module */
driver_static	procid				*procdesc;	/* process descriptor ptr */
driver_static	int					blk_size;	/* Block size */
driver_static	int					lbsize;		/* Logical block size */
driver_static	int					residual;	/* residual length valid */
driver_static	int					fstatus;	/* file status */
driver_static	u_int				optflags;	/* scsi options flags */
driver_static	struct cmdblk		cmdblk;		/* command block */
driver_static	u_char				scsiid;		/* target controller id */
driver_static	u_char				scsilun;	/* target lun */
driver_static	scsipkt				scsicmd;	/* SCSI command packet */
driver_static	u_char				jicpad1[2];	/* make next long-align */
driver_static	errdet				err_det;	/* error details block */
driver_static	u_char				online;		/* drive online status */
driver_static	modesel				mode_sel;	/* mode select block */

#endif

/*
 * general definitions
 */
 
#ifdef		NULL
#undef		NULL
#endif

#define 	ERROR			-1			/* general error value */
#define		NULL			0			/* pointer to nowhere */

#define		TRUE			1			/* for boolean use */
#define		FALSE			0

#define		PHYSIZE			512			/* size of physical block */
#define		TAPE_LUN		0			/* Viper tape MUST use LUN=0 */
#define		MAXSCSI			7			/* maximum scsi id value */

#define		START_DELAY		((1<<31)|(2*256))	/* 2 second delay */

#define		SLEEP_TIME		((1<<31)|(128))		/* 1/2 second delay */

/*
 * tape state flags
 */
#define		TAPE_EOF		(1<<0)		/* end of file pending */
#define		TAPE_EOM		(1<<1)		/* early eom pending */
#define		TAPE_BOM		(1<<2)		/* tape at beginning of media */
#define		TAPE_FULL		(1<<3)		/* issue E$Full error flag */

/*
 * mode select/sense options
 */
#define		MS_UNBUFF		0			/* ms_buffmode:  unbuffered */
#define		MS_BUFF			1			/* ms_buffmode:  buffered */
#define		MS_AUTODENS		0			/* ms_denscode:  default auto */
#define		MS_QIC24		5			/* ms_denscode:  QIC-24 */
#define		MS_QIC120		0x0f		/* ms_denscode:  QIC-120 */
#define		MS_QIC150		0x10		/* ms_denscode:  QIC-150 */

#define		BUFF_MODE		MS_BUFF		/* default buffer mode */

/*
 * scsi command packet options and tweaks
 */
#define		CMDOPTS			0x00		/* sp_options: normal mode*/

#define		FBS				1			/* sp_action: fixed block size */
#define		IMMED			1			/* sp_action: immediate status */
#define		SPACE_BLKS		0			/* sp_action: space blocks */
#define		SPACE_FM		1			/* sp_action: space file marks */
#define		SPACE_SFM		2			/* sp_action: space sequential FM */
#define		SPACE_EOD		3			/* sp_action: space to EOD */

#define		LOAD			1			/* sp_blkcnt: load ON flag */
#define		RETEN			2			/* sp_blkcnt: retension ON flag */

/*
 * definitions of scsi error codes for Archive 2150
 */
#define		SENSE_RECOVER	0x00		/* no error or no sense info */
#define		SENSE_RECOVOK	0x01		/* completed ok by recovery action */
#define		SENSE_NOTRDY	0x02		/* drive not ready */
#define		SENSE_DATAERR	0x03		/* uncorrectable error */
#define		SENSE_HARDWARE	0x04		/* hardware error */
#define		SENSE_ILGL		0x05		/* illegal request */
#define		SENSE_UNITATN	0x06		/* media change or drive reset */
#define		SENSE_WRTPRT	0x07		/* write protected */
#define		SENSE_BLANK		0x08		/* no data recorded */
#define		SENSE_ABORT		0x0b		/* command aborted */
#define		SENSE_OVRFLW	0x0d		/* volume overflow */

/* end of file (sbviper.h) */

