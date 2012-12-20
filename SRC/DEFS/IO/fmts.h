/*
 * fmts.h - disk booters format table 
 */

/*--------------------------------------------------------------------------,
|                                                                           |
|           Copyright 1996-1998 by Microware Systems Corporation            |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, distribution, or creation of derivative works |
|  in any form to any party other than the licensee is strictly prohibited, |
|  unless expressly authorized in writing by Microware Systems Corporation. |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:													        |	
| #   Date    Comments												  By    |
| -- -------- ------------------------------------------------------- ---   |
| 01 95/12/27 Created.												  GbG   |
|             ---- OS9000/PPC V2.0 Release ----                             |
| 02 96/06/06 1200k (5.25) rate changed to RATE_500 from 300.		  GbG   |
|             ---- OS-9000/PPC V2.0.1 Released ----                         |
|             ---- OS-9000/x86 V2.1 Released ----                           |
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
| 03 97/03/26 Removed extra const.                                    GbG   |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
| 04 98/01/32 Fixed nested comments.                                  GbG   |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- Modular Rom v1.1 Sub-component Released ----             |
|             ---- Modular ROM v1.2 Sub-component Released ----             |
|             $$                    <RELEASE_INFO>                     $$   |
|--------------------------------------------------------------------------*/

#define		BLOCK_SIZE	furmat->bsize
#define		SPT_0		furmat->sectors_t0
#define		SPT			furmat->sectors_t0
#define		BLK_OFFSET	furmat->soffs
#define		CYLS		furmat->cyls
#define		TRK_OFFSET	furmat->toffs
#define		LSN_OFFSET	furmat->lsnoffs 
#define		SIDES		furmat->sides
#define		XRATE		furmat->xrate

/*--------------------------------------!
! Boot format identification structure  !
!--------------------------------------*/
typedef const struct fmatid {
	char	const *fcode;			/* format code (i.e. "5803")			*/
	u_int8	const cyls;				/* total cylinders						*/
	u_int8	const sectors;			/* spt									*/
	u_int8	const sectors_t0;		/* spt on track 0						*/
	u_int8	const toffs;			/* track offset							*/
	u_int8	const soffs;			/* sector offset						*/
	u_int8	const sides;			/* sides 								*/
	u_int16	const xrate;			/* transfer rate 						*/
	u_int16 const bsize;			/* block size							*/
	u_int32	const lsnoffs;			/* logical sector offset 				*/
} fmat_id, *Fmat_id;

#ifndef RATE_250
#define RATE_250 250
#endif

#ifndef RATE_300
#define RATE_300 300
#endif

#ifndef RATE_500
#define RATE_500 500
#endif

#ifndef RATE_1000
#define RATE_1000 1000
#endif

#ifndef RATE_2000
#define RATE_2000 2000
#endif

#if defined(_OSK)

#define FORMATS 1
 
fmat_id format[] = {

    { "OS9 Universal 720k", 79, 16, 16, 1, 1, 2, RATE_250, 256, 0 }
};


#elif defined (_OS9000)

/* 
 * The following table contains all of the floppy disk formats 
 * used in booting OS9000.
*/

#define FORMATS 4
 
fmat_id format[] = {
	{ "OS9000 Universal 2880k (3.5)", 80, 36, 36, 0, 1, 2, 
		RATE_1000, 512, 1 },

	{ "OS9000 Universal 1440k (3.5)", 80, 18, 18, 0, 1, 2, 
		RATE_500, 512, 1 },

	{ "OS9000 Universal 720k", 80, 9, 9, 0, 1, 2, 
		RATE_250, 512, 1 },

	{ "OS9000 Universal 1200k (5.25)", 80, 15, 15, 0, 1, 2, 
		RATE_500, 512, 1 }
};

#endif
