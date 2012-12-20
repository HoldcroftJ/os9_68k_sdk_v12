/* struct for getstat() and setstat() functions */


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
|  02 99/06/02 Added _sgr_partsiz for RBF partitions.                  djl  |
|				---- OS-9/68K V3.1 Release ----								|
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/


#if !defined(_SGSTAT_H)
#define _SGSTAT_H

struct _sgs {
	/* The following are for an SCF type device. See below for
	 * structure member definitions for an RBF device.
	 */
	unsigned char
		_sgs_class,		/* file manager class code */
		_sgs_case,		/* 0 = upper and lower cases, 1 = upper case only */
		_sgs_backsp,	/* 0 = BSE, 1 = BSE-SP-BSE */
		_sgs_delete,	/* delete sequence */
		_sgs_echo,		/* 0 = no echo */
		_sgs_alf,		/* 0 = no auto line feed */
		_sgs_nulls,		/* end of line null count */
		_sgs_pause,		/* 0 = no end of page pause */
		_sgs_page;		/* lines per page */
	char
		_sgs_bspch,		/* backspace character */
		_sgs_dlnch,		/* delete line character */
		_sgs_eorch,		/* end of record character */
		_sgs_eofch,		/* end of file character */
		_sgs_rlnch,		/* reprint line character */
		_sgs_dulnch,	/* duplicate last line character */
		_sgs_psch,		/* pause character */
		_sgs_kbich,		/* keyboard interrupt character */
		_sgs_kbach,		/* keyboard abort character */
		_sgs_bsech,		/* backspace echo character */
		_sgs_bellch;	/* line overflow character (bell) */
	unsigned char
		_sgs_parity,	/* device initialization (parity) */
		_sgs_baud;		/* baud rate */
	unsigned short
		_sgs_d2p;		/* offset to second device name string */
	char
		_sgs_xon,		/* x-on char */
		_sgs_xoff,		/* x-off char */
		_sgs_tabcr;		/* tab character */
	unsigned char
		_sgs_tabsiz;	/* tab size */
	unsigned long
		_sgs_tbl;		/* Device table address (copy) */
	unsigned short
		_sgs_col;		/* Current column number */
	unsigned char
		_sgs_err;		/* most recent error status */
	char _sgs_spare[93];	/* spare bytes - necessary for correct sizing */
};

#define sg_class	_sgm._sgs._sgs_class
#define sg_case 	_sgm._sgs._sgs_case
#define sg_backsp 	_sgm._sgs._sgs_backsp
#define sg_delete	_sgm._sgs._sgs_delete
#define sg_echo		_sgm._sgs._sgs_echo
#define sg_alf		_sgm._sgs._sgs_alf
#define sg_nulls	_sgm._sgs._sgs_nulls
#define sg_pause	_sgm._sgs._sgs_pause
#define sg_page		_sgm._sgs._sgs_page
#define sg_bspch	_sgm._sgs._sgs_bspch
#define sg_dlnch	_sgm._sgs._sgs_dlnch
#define sg_eorch	_sgm._sgs._sgs_eorch
#define sg_eofch	_sgm._sgs._sgs_eofch
#define sg_rlnch	_sgm._sgs._sgs_rlnch
#define sg_dulnch	_sgm._sgs._sgs_dulnch
#define sg_psch		_sgm._sgs._sgs_psch
#define sg_kbich	_sgm._sgs._sgs_kbich
#define sg_kbach	_sgm._sgs._sgs_kbach
#define sg_bsech	_sgm._sgs._sgs_bsech
#define sg_bellch	_sgm._sgs._sgs_bellch
#define sg_parity	_sgm._sgs._sgs_parity
#define sg_baud		_sgm._sgs._sgs_baud
#define sg_d2p		_sgm._sgs._sgs_d2p
#define sg_xon		_sgm._sgs._sgs_xon
#define sg_xoff		_sgm._sgs._sgs_xoff
#define sg_tabcr	_sgm._sgs._sgs_tabcr
#define sg_tabsiz	_sgm._sgs._sgs_tabsiz
#define sg_tbl		_sgm._sgs._sgs_tbl 
#define sg_col		_sgm._sgs._sgs_col
#define sg_err		_sgm._sgs._sgs_err
#define sg_spare	_sgm._sgs._sgs_spare

/* the following is a structure definition to set the names, types
 * and offsets of structure members which are applicable to an RBF
 * type device file.
 */
struct _sgr {
	unsigned char
		_sgr_class,		/* file manager class code */
		_sgr_drive,		/* drive number */
		_sgr_step,		/* step rate */
		_sgr_dtype,		/* device type */
		_sgr_dense,		/* density capability */
		_sgr_fill1;		/* not used */
	unsigned short
		_sgr_cyls;		/* number of cylinders (tracks) */
	unsigned char
		_sgr_sides,		/* number of sides */
		_sgr_verify;	/* 0 = verify on writes */
	unsigned short
		_sgr_spt,		/* default sectors per track */
		_sgr_spt0,		/* ditto track 0 */
		_sgr_salloc;	/* segment allocation size */
	unsigned char
		_sgr_intlv,		/* sector interleave factor */
		_sgr_DMAtfm,	/* DMA transfer mode */
		_sgr_toffs,		/* track offset values */
		_sgr_soffs;		/* Sector offset value */
	unsigned short
		_sgr_ssize,		/* sector size (bytes) */
		_sgr_cntl;		/* device control word */
	unsigned char
		_sgr_trys,		/* number of tries counter (1=no-retry/correction) */
		_sgr_lun;		/* SCSI LUN of device */
	unsigned short
		_sgr_wpc,		/* start write-precompensation cylinder */
		_sgr_rwr,		/* start reduced write current cylinder */
		_sgr_park;		/* parking cylinder */
	unsigned long
		_sgr_lsnoffs;	/* LSN offset for partition */
	unsigned short
		_sgr_totcyls;	/* total cylinders on device */
	unsigned char
		_sgr_ctrlrid;	/* SCSI controller ID */
	char
		_sgr_res2;		/* reserved */
	unsigned
		_sgr_scsiopt,	/* SCSI driver options flags */
		_sgr_maxcnt;	/* maximum byte-count passable to driver */
    unsigned
		_sgr_partsiz;	/* partition size */
	char
		_sgr_rsvd[1];	/* reserved for future use */
	unsigned char
		_sgr_att;		/* file attributes */
	unsigned long
		_sgr_fdpsn,		/* file descriptor PSN */
		_sgr_dipsn,		/* file's directory PSN */
		_sgr_dirptr,	/* directory entry pointer */
		_sgr_dvt;		/* address of device table entry */
	char
		_sgr_resrvd[2];	/* Reserved */
	unsigned long
		_sgr_sctsiz;	/* sector size */
	char
		_sgr_resrvd2[20],	/* Reserved */
		_sgr_fname[32];	/* filename */
};

#define sg_drive	_sgm._sgr._sgr_drive
#define sg_step		_sgm._sgr._sgr_step
#define sg_dtype	_sgm._sgr._sgr_dtype
#define sg_dense	_sgm._sgr._sgr_dense
#define sg_fill1	_sgm._sgr._sgr_fill1
#define sg_cyls		_sgm._sgr._sgr_cyls
#define sg_sides	_sgm._sgr._sgr_sides
#define sg_verify	_sgm._sgr._sgr_verify
#define sg_spt		_sgm._sgr._sgr_spt
#define sg_spt0		_sgm._sgr._sgr_spt0
#define sg_salloc	_sgm._sgr._sgr_salloc
#define sg_intlv	_sgm._sgr._sgr_intlv
#define sg_DMAtfm	_sgm._sgr._sgr_DMAtfm
#define sg_toffs	_sgm._sgr._sgr_toffs
#define sg_soffs	_sgm._sgr._sgr_soffs
#define	sg_ssize	_sgm._sgr._sgr_ssize
#define	sg_cntl		_sgm._sgr._sgr_cntl
#define	sg_trys		_sgm._sgr._sgr_trys
#define	sg_lun		_sgm._sgr._sgr_lun
#define	sg_wpc		_sgm._sgr._sgr_wpc
#define	sg_rwr		_sgm._sgr._sgr_rwr
#define	sg_park		_sgm._sgr._sgr_park
#define	sg_lsnoffs	_sgm._sgr._sgr_lsnoffs
#define	sg_totcyls	_sgm._sgr._sgr_totcyls
#define	sg_ctrlrid	_sgm._sgr._sgr_ctrlrid
#define	sg_res2		_sgm._sgr._sgr_res2
#define	sg_scsiopt	_sgm._sgr._sgr_scsiopt
#define	sg_maxcnt	_sgm._sgr._sgr_maxcnt
#define	sg_partsiz	_sgm._sgr._sgr_partsiz
#define sg_rsvd		_sgm._sgr._sgr_rsvd
#define sg_att		_sgm._sgr._sgr_att
#define sg_fdpsn	_sgm._sgr._sgr_fdpsn
#define sg_dipsn	_sgm._sgr._sgr_dipsn
#define sg_dirptr	_sgm._sgr._sgr_dirptr
#define sg_dvt		_sgm._sgr._sgr_dvt
#define sg_fname	_sgm._sgr._sgr_fname
#define sg_resrvd	_sgm._sgr._sgr_resrvd
#define sg_sctsiz	_sgm._sgr._sgr_sctsiz
#define sg_resrvd2	_sgm._sgr._sgr_resrvd2

/*
 * The following are for an OS-9 socket path
 */
struct _sgsk {
	unsigned char
		_sgsk_class,		/* file manager class code				*/
		_sgsk_noblock,		/* 0 = blocking, 1 = non-blocking io	*/
		_sgsk_stp,			/* socket type, 0 = Enp, 1 = Sockman	*/
		_sgsk_align;		/* alignment byte						*/
	union {
		struct {
			unsigned char
				_sge_devn[32];		/* device name		*/
			char
				_sge_enpres[60],	/* reserved			*/
				_sge_fname[32];		/* socket name		*/
		} _enpsk;
		struct {
			unsigned short
				_sgsk_mbpad;		/* mbuf pad value	*/
			char
				_sgsk_res[90];		/* reserved space	*/
			unsigned char
				_sgsk_devn[32];		/* device name		*/
		} _sock;
	} _sgsk_var;
};

#define sg_noblock		_sgm._sgsk._sgsk_noblock
#define sg_socktype		_sgm._sgsk._sgsk_stp
#define sg_enpdevn		_sgm._sgsk._sgsk_var._enpsk._sge_devn
#define sg_sockdevn		_sgm._sgsk._sgsk_var._sock._sgsk_devn
#define sg_mbpad		_sgm._sgsk._sgsk_var._sock._sgsk_mbpad

struct sgbuf {
	union {
		struct _sgs _sgs;
		struct _sgr _sgr;
		struct _sgsk _sgsk;
	} _sgm;
};

#endif /* _SGSTAT_H */
