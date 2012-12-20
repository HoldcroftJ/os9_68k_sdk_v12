/*
 * definitions for Teac SCSI floppy disk controller
 *
 * Copyright 1990, 1991, 1992, 1993, 1994, 1995, 1996 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 *
 * history:
 *
 * ed.  date.   reason.                                           who.
 * -- --------  --------------------------------------------      ---
 * 00 90/01/08  created                                           srw
 * 01 90/04/06  made all static storage structures be long-word   wwb
 *              aligned. This is an issue for dma-driven low-
 *              level modules:  some systems can not handle
 *              non-aligned transfers, so the high-level module
 *              needs to ensure that its internal structures
 *              are safely aligned (JIC: just in case!).
 *              Removed enquiry structure storage (not used).     wwb
 * 02 90/04/11  Added nessesary defs for direct command           Rwb
 * 03 90/04/20  Changed for single controller of 4 logical units  
 *              per incarnation of the driver.                    Rwb
 * 04 90/05/03  Added defs to allow creation of a booter          Rwb
 * 05 90/06/15  added CONTR_SUPPORT qualification to pd_scsiopts. wwb
 * 06 90/07/13  added defs for new descriptors                    Rwb
 * 07 90/07/16  made changes and additions for "ed" format        Rwb
 *              ---- OS-9/68K V2.4 Released ----
 * 08 91/04/01  Changed to not use block descriptors, Uses media  Rwb
 *              codes that are differentiated based on the FC-1
 *              version (US or Japanese).
 *              ---- OS-9/68K V2.4.3 Released ----
 * 09 92/04/08  reworked drive/controller types.                  wwb
 * 10 93/04/15  added missing reserved field in inquiry response. ats
 * 11 93/05/14  Modified for UCC. Collected all driver statics    ats
 *              into a single record for known ordering.
 * 12 93/05/18  Defined NULL for UCC booter.                      ats
 * 13 93/05/27  Changed driver statics variable name.             ats
 *              ---- OS-9/68K V3.0 Released ----
 * 14 94/07/18  Removed CONTR_SUPPORT checks (ed5) - checks are   wwb
 *              now normally performed by low-level.
 * 15 94/08/19  Added FD55 support.                               ats
 *				---- OS-9/68K V3.0.2 Release ----
 * 16 95/06/08  Corrected pathname case of reg.h.                 ats
 * 17 95/07/11  GbG  ANSI Version, "rbproto.h" added.             GbG
 *				---- OS-9/68K V3.0.3 Release ----
 *				---- OS-9/68K V3.1 Release ----
 */

#define     RBF_MAXDRIVE    4           /* max number of drives */

#include <types.h>
#include <const.h>
#ifdef CBOOT
#include <sysboot.h>
#else
#include    <rbf.h>
#include    <setsys.h>
#include    <errno.h>
#include    <sg_codes.h>
#include    <cmdblk.h>
#include    <dcmd.h>
#endif
#include    <MACHINE/reg.h>
#include    <procid.h>
#include    <path.h>
#include    <modes.h>
#include    <scsidmod.h> 
#include	<scsidefs.h>

/*
 *  This structure is used to store the time stamp for sector 0 buffering.
 */
typedef struct sztime {
	struct accblk_str { 		/* time last accessed structure */
		u_int date,				/* julian date */
		      time;				/* time of day */
	} accb;
	u_int  s0bsize;				/* size of the buffer, in case of desc change */
} *Sztime;
		

#define NDRV RBF_MAXDRIVE		/* for definitions use only */

#define	sysstat(type,glob)			(*((type*)((char*)sys_stat+glob)))


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
 * transfers).
 */
#ifdef _UCC
typedef struct {
	struct rbfstatic	rbfstat;	/* RBF required static */
	short				padder;		/* long-word align */
	void                *sys_stat;  /* system globals ptr */
	Rbfstatic           stat;       /* driver static storage ptr */
	Datmod              data_mod;   /* low-level data module */
	void                *drvr_mod;  /* low-level driver module */
	Pathdesc            pathdesc;   /* path descriptor ptr */
	procid              *procdesc;  /* process descriptor ptr */
	long                last_descript[NDRV];	/* last device descriptor */
#ifndef CBOOT
	struct sztime       scztime[NDRV];	/* sector zero buffer time */
	int					drive_kind[RBF_MAXDRIVE];	/* HF/JHF */
	int					drive_checked[RBF_MAXDRIVE]; /* TRUE/FALSE */
/* format device uses these as flags and storage */
	int                 fmt_first[RBF_MAXDRIVE];
	int                 fmt_ddfmt[RBF_MAXDRIVE];
#else
	int					drive_kind;	/* HF/JHF */
	int					drive_checked; /* TRUE/FALSE */
#endif
	struct cmdblk       tcmdblk;    /* the command block */
	u_char				jicpad1[2];	/* make next long-align */
	union scsipkt       scsicmd;    /* SCSI command packet */
	u_char				jicpad2[2];	/* make next long-align */
	errdet              err_det;    /* error detail block */
	teac_fc_modesel     mode_sel;   /* mode select block */
	enqdet              inquir;		/* inquiry data area */

} teacstatics, *TeacStatics;

driver_static	teacstatics		rbteac_statics;

#ifndef		NO_STATICS_SHORTCUTS
#define		rbfstat			rbteac_statics.rbfstat
#define		padder			rbteac_statics.padder
#define		sys_stat		rbteac_statics.sys_stat
#define		stat			rbteac_statics.stat
#define		data_mod		rbteac_statics.data_mod
#define		drvr_mod		rbteac_statics.drvr_mod
#define		pathdesc		rbteac_statics.pathdesc
#define		procdesc		rbteac_statics.procdesc
#define		last_descript	rbteac_statics.last_descript
#define		drive_kind		rbteac_statics.drive_kind
#define		drive_checked	rbteac_statics.drive_checked
#ifndef CBOOT
#define		scztime			rbteac_statics.scztime
#define		fmt_first		rbteac_statics.fmt_first
#define		fmt_ddfmt		rbteac_statics.fmt_ddfmt
#endif	/* CBOOT */
#define		tcmdblk			rbteac_statics.tcmdblk
#define		scsicmd			rbteac_statics.scsicmd
#define		err_det			rbteac_statics.err_det
#define		mode_sel		rbteac_statics.mode_sel
#define		inquir			rbteac_statics.inquir
#endif	/* NO_STATICS_SHORTCUTS */

#else	/* _UCC */

driver_static	struct rbfstatic    rbfstat;    /* RBF required static */
driver_static   short               padder;     /* long-word align */
driver_static   void                *sys_stat;  /* system globals ptr */
driver_static   Rbfstatic           stat;       /* driver static storage ptr */
driver_static   Datmod              data_mod;   /* low-level data module */
driver_static   void                *drvr_mod;  /* low-level driver module */
driver_static   Pathdesc            pathdesc;   /* path descriptor ptr */
driver_static   procid              *procdesc;  /* process descriptor ptr */
driver_static   long                last_descript[NDRV];	/* last device descriptor */

#ifndef CBOOT
driver_static   struct sztime       scztime[NDRV];	/* sector zero buffer time */
driver_static	int					drive_kind[RBF_MAXDRIVE];	/* HF/JHF */
driver_static	int					drive_checked[RBF_MAXDRIVE]; /* TRUE/FALSE */

/* format device uses these as flags and storage */
driver_static   int                 fmt_first[RBF_MAXDRIVE];
driver_static   int                 fmt_ddfmt[RBF_MAXDRIVE];
#else
driver_static	int					drive_kind;	/* HF/JHF */
driver_static	int					drive_checked; /* TRUE/FALSE */
#endif

driver_static   struct cmdblk       tcmdblk;    /* the command block */
driver_static	u_char				jicpad1[2];	/* make next long-align */
driver_static   union scsipkt       scsicmd;    /* SCSI command packet */
driver_static	u_char				jicpad2[2];	/* make next long-align */
driver_static   errdet              err_det;    /* error detail block */
driver_static   teac_fc_modesel     mode_sel;   /* mode select block */
driver_static   enqdet              inquir;		/* inquiry data area */

#endif	/* _UCC */

/*
 * general definitions
 */

#define DISK_ROTATION_RATE  0x0F  /* mask for PD_Rate - yields rotation */
#define DATA_TRANSFER_RATE  0xF0  /* mask for PD_Rate - yields data rate */

#define     PARK        TRUE    /* park function flags */
#define     NOPARK      FALSE

#define     HOLDTIME    2		/* time to hold sector 0 buffer */

/*
 * unit limit checks
 */
#define     MAXLUN      0x4     /* 0-3 supported */
#define     MAXSCSI     0x7     /* maximum SCSI ID */

#define		MAXSSIZE    4096	/* maximum sector size */
#define     MINSSIZE    256		/* minimum sector size */

/* define a means of reaching various nessesary structures */
#ifdef CBOOT
#define PATHOPTIONS &pathopts
#define DRIVENUMBER drivenum
#define PATHBUFFER  pathbuf
#define DRIVETABLE  &drivetable
#else
#define PATHOPTIONS &pathdesc->rbfopt
#define DRIVENUMBER rbfstat.v_ndrv
#define PATHBUFFER  pathdesc->path.pd_buf
#define DRIVETABLE  pathdesc->rbfpvt.pd_dtb
#endif

/* end of file (rbteac.h) */

