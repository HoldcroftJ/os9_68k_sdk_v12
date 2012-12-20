/*
 * definitions for default floppy/hard-disk boot descriptor definitions
 */

/*
 * Copyright 1990 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 * history:
 *
 *
 *  History:
 *  Ed.   Date    Who    Reason
 *  --  --------  --- ----------------------------------------------------
 *                    ---- OS-9/68k V2.4 Release ----
 *  ??  90/09/19  Rwb History begins with change that sets total cylinders
 *                      to be cylinders + track offsets
 *                    ---- OS-9/68k V2.4.3 Release ----
 *                    ---- OS-9/68k V3.0 Release ----
 *                    ---- OS-9/68k V3.1 Release ----
 *                    
 */
#include	<rbf.h>
#include	<sg_codes.h>
#include	<gendefs.h>

/*--------------------------------------------------------------------------!
! Default settings:															!
! NOTE: This structure defaults to Universal Format for floppies and a		!
!		Miniscribe 3425 for hard drives unless the following values are		!
!		overridden in the user defined file: "systype.h"					!
!--------------------------------------------------------------------------*/
#ifndef DRIVENUMBER
#define DRIVENUMBER		0		/* drive number				*/
#endif

#ifndef STEPRATE
#ifdef	FLOPPY
#define STEPRATE		6		/* step rate = 6ms			*/
#else
#define STEPRATE		0		/* step rate 				*/
#endif
#endif

#ifndef DISKTYPE
#ifdef	FLOPPY
#define DISKTYPE		0x20	/* 5" floppy - dd/track 0	*/
#else
#define DISKTYPE		TYP_HARD /* hard drive				*/
#endif
#endif

#ifndef DENSITY
#ifdef	FLOPPY
#define DENSITY			3		/* dbl density, dbl trk density	*/
#else
#define DENSITY			0		/* not applicable				*/
#endif
#endif

#ifndef CYLINDERS
#ifdef	FLOPPY
#define CYLINDERS		80		/* default 80 trk for floppies	*/
#else
#define CYLINDERS		612		/* best guess for hard drives	*/
#endif
#endif

#ifndef HEADS
#ifdef	FLOPPY
#define HEADS			2		/* floppies default - dbl sided	*/
#else
#define HEADS			4		/* best guess for hard drives	*/
#endif
#endif

#ifndef NOVERIFY
#define NOVERIFY		OFF		/* normally irrelevant			*/
#endif

#ifndef SECTTRK
#ifdef	FLOPPY
#define SECTTRK			16		/* set floppy sectors per trk	*/
#else
#define SECTTRK			32		/* set hard drive sectors/trk	*/
#endif
#endif

#ifndef SECTTRK0
#ifdef	FLOPPY
#define SECTTRK0		16		/* set sects/track - track 0	*/
#else
#define SECTTRK0		32		/* set sects/track - track 0	*/
#endif
#endif

#ifndef SEGALLOC
#ifdef	FLOPPY
#define SEGALLOC		8		/* normally irrelevant			*/
#else
#define SEGALLOC		32		/* normally irrelevant			*/
#endif
#endif

#ifndef INTERLEAVE
#define INTERLEAVE		2		/* normally irrelevant			*/
#endif

#ifndef DMAMODE
#define DMAMODE			0		/* no dma						*/
#endif

#ifndef TRACKOFFS
#ifdef	FLOPPY
#define TRACKOFFS		1		/* track number offset			*/
#else
#define TRACKOFFS		0		/* track number offset			*/
#endif
#endif

#ifndef SECTOFFS
#ifdef	FLOPPY
#define SECTOFFS		1		/* sector number offset			*/
#else
#define SECTOFFS		0		/* sector number offset			*/
#endif
#endif

#ifndef SECTSIZE
#define SECTSIZE		256		/* sector size in bytes			*/
#endif

#ifndef CONTROL
#ifdef	FLOPPY
#define CONTROL			0x0002	/* control flags				*/
#else
#define CONTROL			0		/* control flags				*/
#endif
#endif

#ifndef TRYS
#ifdef	FLOPPY
#define TRYS			3		/* # of attempts to read sector	*/
#else
#define TRYS			7		/* # of attempts to read sector	*/
#endif
#endif

#ifndef SCSILUN
#define SCSILUN			0		/* SCSI Logical Unit Number		*/
#endif

#ifndef WPRECOMP
#ifdef	FLOPPY
#define WPRECOMP		80		/* normally irrelevant			*/
#else
#define WPRECOMP		128		/* normally irrelevant			*/
#endif
#endif

#ifndef RWC
#ifdef	FLOPPY
#define RWC				80		/* normally irrelevant			*/
#else
#define RWC				0		/* normally irrelevant			*/
#endif
#endif

#ifndef PARKCYL
#ifdef	FLOPPY
#define PARKCYL			0		/* normally irrelevant			*/
#else
#define PARKCYL			656		/* normally irrelevant			*/
#endif
#endif

#ifndef LSNOFFSET
#define LSNOFFSET		0		/* Partition LSN				*/
#endif

#ifndef TOTCYLS
#define TOTCYLS			CYLINDERS+TRACKOFFS  /* total media cylinders		*/
#endif

#ifndef CTRLRID
#define CTRLRID			0		/* SCSI controller ID 			*/
#endif

struct rbf_opt	OPTSNAME = {
	DT_RBF,				/* device type						*/
	DRIVENUMBER,		/* drive number						*/
	STEPRATE,			/* step rate						*/
	DISKTYPE,			/*									*/
	DENSITY,			/* data/track densities				*/
	0,					/* spare							*/
	CYLINDERS-TRACKOFFS,/* Number of cylinders				*/
	HEADS,				/* Number of sides					*/
	NOVERIFY,			/* verify flag						*/
	SECTTRK,			/* default sectors/track			*/
	SECTTRK0,			/* default sectors/track track 0	*/
	SEGALLOC,			/* segment allocation size			*/
	INTERLEAVE,			/* sector interleave factor			*/
	DMAMODE,			/* DMA mode							*/
	TRACKOFFS,			/* track base offset				*/
	SECTOFFS,			/* sector base offset				*/
	SECTSIZE,			/* sector size						*/
	CONTROL,			/* control flags					*/
	TRYS,				/* number of retries				*/
	SCSILUN,			/* SCSI logical unit number			*/
	WPRECOMP,			/* first cyl with write precomp		*/
	RWC,				/* first cyl with low write current	*/
	PARKCYL,			/* cylinder to park over			*/
	LSNOFFSET,			/* LSN offset for partitioning		*/
	TOTCYLS,			/* total media cylinders			*/
	CTRLRID				/* SCSI controller ID				*/
};
