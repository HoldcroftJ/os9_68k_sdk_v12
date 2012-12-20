#if !defined(_SBF_H)
#define _SBF_H

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
|                                                                           |
| Edition History:                                                          |
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
|  01 98/07/29 Added edition history.                                  wwb  |
|				---- OS-9/68K V3.1 Release ----								|
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/


#if !defined(_SYSIO_H)
#include <sysio.h>
#endif /* _SYSIO_H */

/*
	SBF drive table static storage.  One for each drive.
*/

typedef	struct	sbfdrv {
	u_short		sbf_dflg,		/* drive flag */
				sbf_nbuf;		/* number of buffers */
	u_long		sbf_ibh,		/* pointer to top of input buffer list */
				sbf_ibt,		/* pointer to end of input buffer list */
				sbf_obh,		/* pointer to top of output buffer list */
				sbf_obt,		/* pointer to end of output buffer list */
				sbf_wait,		/* pointer to waiting process */
				sbf_serr,		/* number of recoverable errors */
				sbf_herr,		/* number of non-recoverable errors */
				sbf_rsv3[8];	/* 8 unused long words */
} *Sbfdrv;

/*
	SBF static storage.  Required in every SBF driver.
*/
typedef struct sbfstatic {
	sysioStatic	sbf_sysio;		/* kernel static storage variables	*/
	u_short		sbf_rsv0;		/* reserved 						*/
	u_char		sbf_ndrv,		/* number of drives					*/
				sbf_rsv1;		/* reserved							*/
	u_short		sbf_flag;		/* driver flags						*/
	u_char		*sbf_drvr,		/* driver module pointer			*/
				*sbf_dprc,		/* driver process pointer			*/
				*sbf_iprc;		/* interrupt process pointer		*/
	u_char		sbf_rsv2[64];	/* reserved 						*/
#if !defined(SBF_MAXDRIVE)
#define	SBF_MAXDRIVE	1
#endif /* SBF_MAXDRIVE */
	struct sbfdrv	drv[SBF_MAXDRIVE];	/* SBF drive table(s) */
} sbfstatic, *Sbfstatic;

/*
	Sequential block file manager PD definitions
*/
struct sbf_opt {
	u_char		pd_dtp;			/* device type						*/
	u_char		pd_tdrv;		/* logical drive number				*/
	u_char		pd_sbf;			/* reserved 						*/
	u_char		pd_numblk;		/* maximum # of blocks to allocate	*/
	u_int		pd_blksiz;		/* size of blocks allocated			*/
	u_short		pd_prior;		/* driver process priority			*/
	u_short		pd_flags;		/* SBF/driver capability flags		*/
#if !defined(_SBF_DEV_FLGS)
/* bits 0 - 7 are free for driver usage */
#define		DEV_REWIND_FLG	(1<<8)	/* rewind device on close */
#define		DEV_OFFLINE_FLG	(1<<9)	/* take device offline on close */
#define		DEV_ERASE_FLG	(1<<10)	/* erase to EOT after writing */
#define		_SBF_DEV_FLGS
#endif	/* _SBF_DEV_FLGS) */
	u_short		pd_dmamode;		/* DMA type/usage					*/
	u_char		pd_scsiid;		/* SCSI controller ID				*/
	u_char		pd_scsilun;		/* SCSI controller drive LUN		*/
	u_int		pd_scsiopt;		/* SCSI options flags */
#if !defined(_SCSI_OPTS)
#define		SCSI_ATN		(1<<0)	/* assert ATN supported				*/
#define		SCSI_TARGET		(1<<1)	/* target mode supported			*/
#define		SCSI_SYNCHR		(1<<2)	/* synchronous transfers supported	*/
#define		SCSI_PARITY		(1<<3)	/* enable SCSI parity checking		*/
#define		SCSI_WIDE		(1<<4)	/* enable SCSI wide operation		*/
#define		SCSI_ULTRA		(1<<5)	/* enable SCSI ultra support        */
#define		SCSI_SLOW		(1<<6)	/* force max synchronous to 5MB     */
#define		_SCSI_OPTS
#endif /* _SCSI_OPTS */
};

/*
 * dummy structure to make path.h happy
 */
 
struct sbf_pvt {
	char		unused;
};

#define SBFMAN	1
#define sbfopt	path.fm_opt.sbf

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
error_code _os_ss_erase(path_id, u_int32);
error_code _os_ss_rfm(path_id, int32);
error_code _os_ss_skip(path_id, int32);
error_code _os_ss_wfm(path_id, u_int32);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#else
error_code _os_ss_erase();
error_code _os_ss_rfm();
error_code _os_ss_skip();
error_code _os_ss_wfm();
#endif

#endif /* _SBF_H */
