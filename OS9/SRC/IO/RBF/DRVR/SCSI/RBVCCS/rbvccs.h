/*
 * definitions for Common Command Set scsi disk controller
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
 *
 * history:
 *
 * ed.  date.   reason.                                           who.
 * -- --------  --------------------------------------------      ---
 * 00 88/09/26  created                                           wwb
 * 01 88/10/20  fixed inquiry detail struct problem.              wwb
 * 02 88/11/22  added drive capacity definitions.                 wwb
 * 03 88/11/27  added "non-int" return declarations.              wwb
 * 04 89/03/22  added mode sense definitions (1st "sccs" cut)     srw
 * 05 89/04/21  fixed mode sense structure for page three         srw
 * 06 89/07/14  removed E_HDWARE (use errno.h value).             wwb
 * 07 89/07/20  added "started" flags for SC_LOAD cmds.           wwb
 * 08 89/08/04  changed "not-ready" timeout from 100 to 10.       wwb
 * 09 90/04/02  added PF_MASK - page format = scsi II             Rwb
 * 10 90/04/02  taken from file of same name, modified for
 *              single drive/incarnation and variable sector
 *              size RBF.  Requires 1:1 physical/logical sector
 *              size.                                             Rwb
 * 11 90/04/06  Added direct command to stat.c                    Rwb
 * 12 90/04/06  made all static storage structures be long-word   wwb
 *              aligned. This is an issue for dma-driven low-
 *              level modules:  some systems can not handle
 *              non-aligned transfers, so the high-level module
 *              needs to ensure that its internal structures
 *              are safely aligned (JIC: just in case!).
 *              Removed enquiry structure storage (not used).     wwb
 *              Removed cache block definition (not used).        wwb
 * 13 90/06/15  added CONTR_SUPPORT qualification to pd_scsiopts. wwb
 * 14 93/05/03  Modified for UCC. Collected all driver statics    ats
 *              into a single record for known ordering.
 *				---- OS-9/68K V3.0 Release ----
 * 15 94/07/18  Removed CONTR_SUPPORT (ed13) - checks are now     wwb
 *              normally performed by low-level.
 *				---- OS-9/68K V3.0.1 Release ----
 * 16 95/06/08  Corrected pathname case of reg.h.                 ats
 *				---- OS-9/68K V3.0.2 Release ----
 * 17 98/07/08  reworked static storage naming for UCC2.X.        wwb
 * 18 98/09/08  Added new copyright info.                         wwb
 *				---- OS-9/68K V3.1 Release ----
 *    $$                    <RELEASE_INFO>                     $$  |
 */

#define		RBF_MAXDRIVE	1			/* max number of drives */

#include	<rbf.h>
#include	<MACHINE/reg.h>
#include	<procid.h>
#include	<path.h>
#include	<errno.h>
#include	<sg_codes.h>
#include	<scsidmod.h> 
#include	<cmdblk.h>
#include	<scsicmds.h>
#include    <modes.h>
#include    <dcmd.h>

/*
 * non-int function declarations.
 */
u_int addoffsets ();

/*
 * scsi command packet for low-level module
 */
 
union scsipkt {
	struct std_str {
		u_char		sp_opcode;			/* the opcode */
		unsigned	sp_lun		: 3;	/* the logical unit number */
		unsigned	sp_lbamsbs	: 5;	/* the block address (msbs) */
		char		sp_lba[2];			/* (the rest of) the block address */
		char		sp_blkcnt;			/* the block count */
		u_char		sp_options;			/* command options */
	} std;
	struct ext_str {
		u_char		sp_opcode;			/* the opcode */
		unsigned	sp_lun		: 3;	/* the logical unit number */
		unsigned	sp_zero		: 5;	/* zero */
		u_int		sp_lba;				/* logical block address */
		u_char		sp_zero2;			/* zero */
		u_char		sp_count[2];		/* transfer count */
		u_char		sp_options;			/* command options */
	} ext;
};

typedef union scsipkt *Scsipkt;			/* pointer to a packet */


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
	u_char			ed_info[4],			/* info byte [lba] */
					ed_senslen;			/* additional sense info length */
	u_int			ed_zero3;			/* always zero */
	u_char			ed_code,			/* error code */
					ed_zero4,			/* always 0 */
					ed_fru;				/* fru code */
	unsigned		ed_fpv		: 1,	/* field pointer valid */
					ed_cd		: 1,	/* command/data bit */
					ed_zero5	: 2,	/* always zero */
					ed_bpv		: 1,	/* block pointer valid */
					ed_bitptr	: 3;	/* bit pointer */
	u_char			ed_fptr[2];			/* field pointer */
} errdet;

typedef errdet *Errdet;			/* pointer to error detail packet */

/*
 * inquiry data structure
 */

typedef struct inqdet_str {
	u_char			iq_type;			/* device type */
	unsigned		iq_rmb		: 1,	/* always 0 */
					iq_typqual	: 7,	/* device type qualifier */
					iq_iso		: 2,	/* ISO version */
					iq_ecma		: 4,	/* ECMA version */
					iq_ansi		: 2,	/* ANSI version */
					iq_zero		: 4,	/* always 0 */
					iq_datfmt	: 4;	/* response data format */
	char			iq_addlen,			/* additional length */
					iq_datlen,			/* request sense data length */
					iq_cdcres[2],		/* CDC reserved */
					iq_vendid[8],		/* vendor id */
					iq_prodid[16],		/* product id */
					iq_revlev[4];		/* revision level */
} inqdet;

typedef inqdet *Inqdet;			/* pointer to inquiry data packet */

/*
 * mode select data structure
 */
typedef struct modesel_str {
	u_char			ms_res1;			/* reserved: (0) */
	u_char			ms_medtype;			/* medium type: (0) */
	u_char			ms_res2;			/* reserved: (0) */
	u_char			ms_bdlen;			/* block descriptor length: (8) */
	u_char			ms_denscode;		/* density code: (0) */
	u_char			ms_numblks[3];		/* number of blocks: (0) */
	u_char			ms_res3;			/* reserved: (0) */
	u_char			ms_blklen[3];		/* block length */
} modesel;

typedef modesel *Modesel;

/*
 * mode sense data structure
 */
typedef struct modesen_str {
	/* parameter list header */
	u_char			msn_datlen;			/* length of sense info */
	u_char			msn_medtype;		/* medium type */
	unsigned		msn_wp : 1;			/* write protect 1=WP ON */
	unsigned		msn_res1 : 7;		/* unused */
	u_char			msn_bdlen;			/* block descriptor length */
	/* block descriptor */
	u_char			msn_denscode;		/* density code  */
	u_char			msn_numblks[3];		/* number of blocks */
	u_char			msn_res2;			/* reserved */
	u_char			msn_blklen[3];		/* block length */
	/* Parameter Page 3 */				/* format paramerers page */
	u_char			msn_p3code;			/* page three header */
	u_char			msn_p3plen;			/* page three length (0x16) */
	u_char			msn_p3trk_zone[2];	/* tracks per zone */
	u_char			msn_p3asct_zone[2];	/* alternate sectors per zone */
	u_char			msn_p3atrk_zone[2];	/* alternate tracks per zone */
	u_char			msn_p3atrk_vol[2];	/* alternate tracks per volume */
	u_char			msn_p3sect_trk[2];	/* sectors per track */
	u_char			msn_p3byte_sect[2];	/* bytes per physical sector */
	u_char			msn_p3interlv[2];	/* interleave value */
	u_char			msn_p3trk_skew[2];	/* track skew */
	u_char			msn_p3cyl_skew[2];	/* cylinder skew */
	u_char			msn_p3res[4];		/* reserved */
} modesense;

typedef modesense	*Modesense;

/*
 * drive capacity structure
 */
typedef struct dsize_str {
	u_int			ds_maxlsn;			/* highest lsn on device */
	u_int			ds_blksize;			/* physical block size */
} dsize;

typedef dsize *Dsize;

/*
 * rbf driver static storage definitions
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
	struct rbfstatic	ds_rbfstat;	/* RBF required static */
	short				ds_padder;		/* long-word align */
	void				*ds_sys_stat;	/* system globals ptr */
	Rbfstatic			ds_stat;		/* driver static storage ptr */
	Datmod				ds_data_mod;	/* low-level data module */
	void				*ds_drvr_mod;	/* low-level driver module */
	Pathdesc			ds_pathdesc;	/* path descriptor ptr */
	procid				*ds_procdesc;	/* process descriptor ptr */
	dsize				ds_drvsize;	/* READ CAP. results */
	u_int				ds_started;	/* LOAD cmd sent flags */
	struct cmdblk		ds_cmdblk;		/* the command block */
	u_char				ds_jicpad1[2];	/* make next long-align */
	union scsipkt		ds_scsicmd;	/* SCSI command packet */
	u_char				ds_jicpad2[2];	/* make next long-align */
	errdet				ds_err_det;	/* error detail block */
	u_char				ds_jicpad3[2];	/* make next long-align */
	modesel				ds_mode_sel;	/* mode select block */
	modesense			ds_mode_sense;	/* mode sense block */
} vccsstatics, *VccsStatics;

driver_static	vccsstatics		driver_statics;

#ifndef		NO_STATICS_SHORTCUTS
#define		rbfstat		driver_statics.ds_rbfstat
#define		padder		driver_statics.ds_padder
#define		sys_stat	driver_statics.ds_sys_stat
#define		stat		driver_statics.ds_stat
#define		data_mod	driver_statics.ds_data_mod
#define		drvr_mod	driver_statics.ds_drvr_mod
#define		pathdesc	driver_statics.ds_pathdesc
#define		procdesc	driver_statics.ds_procdesc
#define		drvsize		driver_statics.ds_drvsize
#define		started		driver_statics.ds_started
#define		cmdblk		driver_statics.ds_cmdblk
#define		jicpad1		driver_statics.ds_jicpad1
#define		scsicmd		driver_statics.ds_scsicmd
#define		jicpad2		driver_statics.ds_jicpad2
#define		err_det		driver_statics.ds_err_det
#define		jicpad3		driver_statics.ds_jicpad3
#define		mode_sel	driver_statics.ds_mode_sel
#define		mode_sense	driver_statics.ds_mode_sense
#endif		NO_STATICS_SHORTCUTS

#else

driver_static	struct rbfstatic	rbfstat;	/* RBF required static */
driver_static	short				padder;		/* long-word align */
driver_static	void				*sys_stat;	/* system globals ptr */
driver_static	Rbfstatic			stat;		/* driver static storage ptr */
driver_static	Datmod				data_mod;	/* low-level data module */
driver_static	void				*drvr_mod;	/* low-level driver module */
driver_static	Pathdesc			pathdesc;	/* path descriptor ptr */
driver_static	procid				*procdesc;	/* process descriptor ptr */
driver_static	dsize				drvsize;	/* READ CAP. results */
driver_static	u_int				started;	/* LOAD cmd sent flags */
driver_static	struct cmdblk		cmdblk;		/* the command block */
driver_static	u_char				jicpad1[2];	/* make next long-align */
driver_static	union scsipkt		scsicmd;	/* SCSI command packet */
driver_static	u_char				jicpad2[2];	/* make next long-align */
driver_static	errdet				err_det;	/* error detail block */
driver_static	u_char				jicpad3[2];	/* make next long-align */
driver_static	modesel				mode_sel;	/* mode select block */
driver_static	modesense			mode_sense;	/* mode sense block */

#endif

/*
 * general definitions
 */

#ifdef		NULL
#undef		NULL
#endif

#define		NULL		0		/* pointer to nowhere */
#define		TRUE		1		/* for boolean use */
#define		FALSE		0

#define		PARK		TRUE	/* park function flags */
#define		NOPARK		FALSE

/*
 * unit limit checks
 */
#define		MAXLUN		0x0		/* only LUN supported */
#define		MAXSCSI		0x7		/* maximum SCSI ID */

/*
 * command options byte for SCCS drives
 */
#define		CCSOPTS	0x0		/* cdb options byte always 0 */

/*
 * scsi command packet types
 */
#define		CDB_STD		6		/* std cdb size */
#define		CDB_EXT		10		/* extended cdb size */

/*
 * time-out values
 */
#define		NOTRDY_TO	10		/* "not-ready" time-out value (initdrv) */

/*
 * mode sense defines
 */
#define		PCF_CURRENT	0x00	/* current drive parameters */
#define		PCF_CHANGE	0x01	/* changeable drive parameters */
#define		PCF_DEFAULT	0x02	/* default drive parameters */
#define		PCF_SAVED	0x03	/* saved drive parameters */
#define		PAGE_3		0x03	/* page three code */
#define		ALL_PAGES	0x3f	/* page code for all pages */
#define		PCODE_MASK	0x3f	/* mask for changing mode sense to mode select */
#define     PF_MASK     0x400000 /* for mode select command, in blockaddr field
                                  * indicates that pages conform with SCSI II.
                                  */

/*
 * definitions of Common Command Set sense keys
 * These are the sense values given in byte 2 (bits 3-0)
 * of the extended sense data.
 */

#define		MAIN_NOSENSE	0x00		/* no sense */
#define		MAIN_RECOVERED	0x01		/* recovered error */
#define		MAIN_NOTRDY		0x02		/* not ready */
#define		MAIN_MEDIUM		0x03		/* medium error */
#define		MAIN_HARDWARE	0x04		/* hardware error */
#define		MAIN_ILGL		0x05		/* illegal request */
#define		MAIN_UNITATN	0x06		/* unit attention */
#define		MAIN_ABORT		0x0b		/* aborted command */
#define		MAIN_OVRFL		0x0d		/* volume overflow */
#define		MAIN_DATACMP	0x0e		/* data miscompare */

/*
 * definitions of Common Command Set extended sense error codes
 * These are the sense values given in byte 12
 * of the extended sense data.
 */
 
#define		SENSE_RECOVER	0x00		/* no error or no sense info */
#define		SENSE_NOINDEX	0x01		/* no index from drive */
#define		SENSE_NOSEEK	0x02		/* no seek complete */
#define		SENSE_WRITEFLT	0x03		/* write fault received from drive */
#define		SENSE_NOTRDY	0x04		/* drive not ready */
#define		SENSE_LUNFAIL	0x08		/* LUN communication failure */
#define		SENSE_TRKFOL	0x09		/* track following error */
#define		SENSE_ID		0x10		/* ID CRC or ECC error */
#define		SENSE_DATAERR	0x11		/* uncorrectable data error */
#define		SENSE_NOADDRID	0x12		/* no address mark in ID field */
#define		SENSE_NOADDR	0x13		/* no address mark in data field */
#define		SENSE_NOREC		0x14		/* no record found */
#define		SENSE_SEEK		0x15		/* seek error */
#define		SENSE_RETRYOK	0x17		/* recovered data by retries */
#define		SENSE_ECCOK		0x18		/* recovered data by ecc correction */
#define		SENSE_DLERR		0x19		/* defect list error */
#define		SENSE_DLMISS	0x1c		/* primary defect list missing */
#define		SENSE_CMPERR	0x1d		/* compare error */
#define		SENSE_ILLREQ	0x20		/* invalid command code */
#define		SENSE_ILLLBA	0x21		/* illegal logical block addr */
#define		SENSE_ILLFUNC	0x22		/* illegal function for drive type */
#define		SENSE_ILLCDB	0x24		/* illegal field in CDB */
#define		SENSE_ILLLUN	0x25		/* invalid LUN */
#define		SENSE_ILLFLD	0x26		/* invalid field in param list */
#define		SENSE_RESET		0x29		/* power-on or reset or bus-reset */
#define		SENSE_MODE		0x2a		/* mode select parameters changed */
#define		SENSE_MEDIUM	0x31		/* medium format corrupted */
#define		SENSE_NOSPARE	0x32		/* no defect spare available */
#define		SENSE_DIAGRAM	0x40		/* ram failure */
#define		SENSE_DIAGDATA	0x41		/* data path diagnostic failure */
#define		SENSE_DIAGPON	0x42		/* power-on diagnostic failure */
#define		SENSE_REJECT	0x43		/* message reject error */
#define		SENSE_CONTR		0x44		/* internal controller error */
#define		SENSE_RESEL		0x45		/* select/reselect failed */
#define		SENSE_SCSIPAR	0x47		/* scsi interface parity error */
#define		SENSE_INITIATR	0x48		/* initiator detected error */
#define		SENSE_INAPPROP	0x49		/* inappropriate/illegal message */

/* end of file (rbvccs.h) */

