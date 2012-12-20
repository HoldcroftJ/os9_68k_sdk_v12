/*
 * definitions for TEAC MT-2ST/N50 scsi Tape controller
 *
 * Copyright 1989, 1990, 1993, 1994, 1995, 1998 by Microware Systems Corporation
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
 * ed.   date    reason.                                           who.
 * --  --------  ------------------------------------------------  ---
 * 00  89/09/12  Modified from sbviper.h for use with sbteac       Rwb
 * 01  89/10/10  Added counters etc. in static storage for neg.    srw
 *               skip using rewind.
 * 02  89/11/07  Redid sbdatmod structure to keep long-alignment.  wwb
 *               Added spare space also.
 * 03  90/04/06  made all static storage structures be long-word   wwb
 *               aligned. This is an issue for dma-driven low-
 *               level modules:  some systems can not handle
 *               non-aligned transfers, so the high-level module
 *               needs to ensure that its internal structures
 *               are safely aligned (JIC: just in case!).
 *               Removed enquiry structure storage (not used).     wwb
 * 04  90/04/10  Added path descriptor to static storage           Rwb
 * 05  90/04/12  Added defs for direct command interface           Rwb
 * 06  90/06/15  added CONTR_SUPPORT qualification to pd_scsiopts. wwb
 *               ---- OS-9/68K V2.4.3 released ----
 * 07  93/05/25  Modified for UCC compatibility mode. Collected    ats
 *               all driver statics into a single record.
 *               ---- OS-9/68K V3.0 released ----
 * 08  94/07/18  Removed CONTR_SUPPORT checks (ed6) - checks are   wwb
 *               now normally performed by low-level.
 *               ---- OS-9/68K V3.0.1 Release ----
 * 09  95/06/08  Corrected pathname case of reg.h.                 ats
 *               ---- OS-9/68K V3.0.2 Release ----
 * 10  98/07/08  reworked static storage naming for UCC2.X
 *               ---- OS-9/68K V3.1 Release ----
 */

/*
 * driver limitations:
 * this version of the driver can only handle one "logical"
 * drive per driver incarnation.  If it is desired to have
 * a scsi bus with more than one "sbteac" Tape device
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
#include    <modes.h>
#include	<sg_codes.h>
#include	<cmdblk.h>
#include	<scsidmod.h> 
#include    <dcmd.h>
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
	u_char			ed_zero3[10];		/* 10 bytes reserved */
	unsigned		ed_zero4	: 4,	/* reserved */
					ed_mfault	: 1,	/* media fault */
					ed_stall	: 1,	/* tape stalled */
					ed_hole		: 1,	/* hole detected */
					ed_parity	: 1;	/* parity error detected */
	u_char			ed_zero5;			/* reserved */
	unsigned		ed_cld		: 1,	/* cassette inserted = 0 */
					ed_wpt		: 1,	/* write protected */
					ed_bom		: 1,	/* BOM */
					ed_run		: 1,	/* tape moving */
					ed_stream	: 1,	/* tape streaming */
					ed_idcode	: 3;	/* device identity code */
	u_char			ed_dataerr[2];		/* data error counter */
	u_char			ed_underrun[2];		/* data underrun counter */
	u_char			ed_blkaddr[4];		/* current block address */
	u_char			ed_errblkcnt[2],	/* error block counter */
					ed_tapepos[2],		/* tape position counter */
					ed_correrr[2];		/* corrected error block counter */
	u_char			ed_trkerr[17];		/* track error counter array */
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
	u_char          ms_TeacRes;         /* TEAC uses this to define 
	                                     *   the directory mode (bit 0 = 1)
	                                     */
} modesel;

typedef modesel *Modesel;

/*
 * Structure for the sbteac high level data module
 */

typedef struct sbdatmod {
	int			fstatus;			/* file status */
	int			markcnt,			/* tape mark counter */
				blknum;				/* tape block number */
    void    	*sbmodhdr; 		    /* pointer to this module's header */
    int			reserved[4];		/* reserved for expansion */
	u_char		online;				/* drive online status */
	u_char		reserved2[15];		/* reserved for expansion */
} *SBDatmod;

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
/* the above definition allocates the SBF global static storage
 * AND the SBF drive tables:  it must be the first definition.
 * 'sbf.h' holds the declarations.
 */
	Pathdesc            ds_pathdesc;	/* path descriptor */
	void				*ds_sys_stat;	/* system globals ptr */
	Sbfstatic			ds_stat;		/* driver static storage ptr */
	Datmod				ds_data_mod;	/* low level data module */
	void				*ds_drvr_mod;	/* low level driver module */
	procid				*ds_procdesc;	/* process descriptor ptr */
	int					ds_blk_size;	/* Block size */
	int					ds_lbsize;		/* Logical block size */
	int					ds_residual;	/* residual length valid */
	u_int				ds_optflags;	/* scsi options flags */
	SBDatmod			ds_sbdata_mod;	/* high level data module */
	struct cmdblk		ds_cmdblk;		/* command block */
	u_char				ds_scsiid;		/* target controller id */
	u_char				ds_scsilun;	/* target lun */
	scsipkt				ds_scsicmd;	/* SCSI command packet */
	u_char				ds_jicpad1[2];	/* keep next long-aligned */
	modesel				ds_mode_sel;	/* mode select block */
	u_char				ds_jicpad2[3];	/* keep next long-aligned */
	errdet				ds_err_det;	/* error details block */
} teacstatics, *TeacStatics;

driver_static	teacstatics		driver_statics;

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
#define		optflags	driver_statics.ds_optflags
#define		sbdata_mod	driver_statics.ds_sbdata_mod
#define		cmdblk		driver_statics.ds_cmdblk
#define		scsiid		driver_statics.ds_scsiid
#define		scsilun		driver_statics.ds_scsilun
#define		scsicmd		driver_statics.ds_scsicmd
#define		mode_sel	driver_statics.ds_mode_sel
#define		err_det		driver_statics.ds_err_det
#endif		NO_STATICS_SHORTCUTS

#else

driver_static	struct sbfstatic	sbfstat;	/* SBF required static */
/* the above definition allocates the SBF global static storage
 * AND the SBF drive tables:  it must be the first definition.
 * 'sbf.h' holds the declarations.
 */
driver_static   Pathdesc            pathdesc;	/* path descriptor */
driver_static	void				*sys_stat;	/* system globals ptr */
driver_static	Sbfstatic			stat;		/* driver static storage ptr */
driver_static	Datmod				data_mod;	/* low level data module */
driver_static	void				*drvr_mod;	/* low level driver module */
driver_static	procid				*procdesc;	/* process descriptor ptr */
driver_static	int					blk_size;	/* Block size */
driver_static	int					lbsize;		/* Logical block size */
driver_static	int					residual;	/* residual length valid */
driver_static	u_int				optflags;	/* scsi options flags */
driver_static	SBDatmod			sbdata_mod;	/* high level data module */

driver_static	struct cmdblk		cmdblk;		/* command block */
driver_static	u_char				scsiid;		/* target controller id */
driver_static	u_char				scsilun;	/* target lun */
driver_static	scsipkt				scsicmd;	/* SCSI command packet */
driver_static	u_char				jicpad1[2];	/* keep next long-aligned */
driver_static	modesel				mode_sel;	/* mode select block */
driver_static	u_char				jicpad2[3];	/* keep next long-aligned */
driver_static	errdet				err_det;	/* error details block */

#endif

/*
 * general definitions
 */
 
#ifdef 	NULL
#undef		NULL
#endif

#define 	ERROR			-1			/* general error value */
#define		NULL			0			/* pointer to nowhere */

#define		TRUE			1			/* for boolean use */
#define		FALSE			0
#define		GOOD			0			/* "GOOD" status return */

#define		PHYSIZE			512			/* size of physical block */
#define		TAPE_LUN		0			/* TEAC tape MUST use LUN=0 */
#define		MAXSCSI			7			/* maximum scsi id value */

#define		START_DELAY		((1<<31)|(2*256))	/* 2 second delay */

#define		SLEEP_TIME		((1<<31)|(128))		/* 1/2 second delay */

#define     READY_TIMEOUT   0x20       /* timeout on unit busy - initdrv() */

#define LLMODACCESS (MP_OWNER_READ|MP_OWNER_WRITE|MP_GROUP_READ|MP_WORLD_READ)

/*
 * tape state flags
 */
#define		TAPE_EOF		(1<<0)		/* end of file pending */
#define		TAPE_EOM		(1<<1)		/* early eom pending */
#define		TAPE_BOM		(1<<2)		/* tape at beginning of media */
#define		TAPE_FULL		(1<<3)		/* return E_FULL flag */

/*
 * mode select/sense options
 */
#define		MS_UNBUFF		0			/* ms_buffmode:  unbuffered */
#define		MS_BUFF			1			/* ms_buffmode:  buffered */
#define		MS_AUTODENS		0			/* ms_denscode:  default auto */
#define		MS_QIC24		5			/* ms_denscode:  QIC-24 */
#define		MS_QIC120		0x0f		/* ms_denscode:  QIC-120 */
#define		MS_QIC150		0x10		/* ms_denscode:  QIC-150 */
#define		MS_TEACDENS		0x88		/* ms_denscode:  required for teac */

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

/* end of file (sbteac.h) */

