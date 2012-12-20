/*------------------------------------------------------------------!
! Tape Header Structures											!
!------------------------------------------------------------------*/

#if !defined(_TAPEHEAD_H) && !defined(TAPEHEAD)
#define _TAPEHEAD_H
#define TAPEHEAD /* Do NOT use the wrapper identifier in new software */

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

#include <time.h>

#if !defined(_TYPES_H)
#include <types.h>
#endif

#define VERSION		1
#define SYNCCODE	0xCA7FECE5

typedef struct tapefentry {
	u_int32		fe_posn;		/* file position on the tape		*/
	u_int32		fe_size;		/* file size (in bytes)				*/
	u_int32		fe_crc;			/* file CRC							*/
	u_int32		fe_rsvd[5];		/* reserved for possible future use	*/
	char		fe_name[32];	/* file name string					*/
} fentry, *Fentry;

typedef struct tape_header {
	u_int32		th_sync;		/* sync header: 0xCA7FECE5			*/
	u_int32		th_vers;		/* tapeboot version id				*/
	struct sgtbuf th_time;		/* system time stamp				*/
	u_char		th_rsvd[2];		/* reserved							*/
	char		th_volid[32];	/* volume name						*/
	char		th_targid[32];	/* target system id name string		*/
								/* if tb_idataddsiz = 0, non disk image	*/
	fentry		th_bootentry;	/* bootstrap file entry				*/
	fentry		th_idataentry;	/* initialized data file entry		*/
	u_int32		th_idataddsiz;	/* DD_TOT field of data file		*/
	u_int32		th_fentries;	/* # of additional file entries to	*/
								/* follow immediately after header	*/
	u_int32		th_padded[10];	/* reserved							*/
} tape_header, *Tape_header;

#endif /* _TAPEHEAD_H */
