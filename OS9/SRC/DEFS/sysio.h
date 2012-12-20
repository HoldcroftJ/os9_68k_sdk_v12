/* sysio.h */

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


/*
	System I/O static storage definitions
*/
#if !defined(_SYSIO_H)
#define _SYSIO_H

#if !defined(_TYPES_H)
#include <types.h>
#endif /* _TYPES_H */

#if !defined(_MODULE_H)
#include <module.h>
#endif /* _MODULE_H */

/* System device table entry */
typedef struct devicetbl {
	struct modhcom 	*V_driv;		/* device driver module ptr */
	u_char			*V_stat;		/* device driver static storage base */
	mod_dev		 	*V_desc;		/* device descriptor module ptr */
	struct modhcom	*V_fmgr;		/* file manager module ptr */
	u_short			V_usrs,			/* use count */
					V_res1;			/* reserved */
	u_int			V_res2[3];		/* reserved */
} *Devicetbl;

/* The is required by the kernel in static storage of all devices */
typedef struct sysiost {
	u_char		*v_port;		/* device base port address */
	u_short		v_lprc;			/* last active process ID */
	u_short		v_busy;			/* current process ID (0=not busy) */
	u_short		v_wake;			/* active process ID for driver to wake */
	union pathdesc *v_paths;	/* list of open paths on device */
	u_int		v_dumm1[8];		/* reserved */
} sysioStatic;
#define _sysio	1

/* device driver entry offsets */

#define D_INIT	0			/* initialize */
#define D_READ	2			/* read */
#define D_WRIT	4			/* write */
#define D_GSTA	6			/* getstat */
#define D_PSTA	8			/* putstat */
#define D_TERM	10			/* terminate */
#define D_TRAP	12			/* trap */

/* file open mode and access permission values */
#define READ_	1
#define WRITE_	2
#define EXEC_	4
#define UPDAT_	(READ_|WRITE_)
#define PREAD_	8
#define PWRIT_	16
#define PEXEC_	32
#define PUPDAT_	(PREAD_|PWRIT_)
#define ISIZE_	32
#define SHARE_	64
#define DIR_	128

#endif /* _SYSIO_H */
