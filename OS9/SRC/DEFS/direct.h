#if !defined(_DIRECT_H)
#define _DIRECT_H

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


struct dirent {
	char	dir_name[28];	/* directory name */
	long	dir_addr;		/* file descriptor address */
};

struct fildes {
	char	fd_att,			/* file attributes */
			fd_own[2],		/* file owner */
			fd_date[5],		/* date last modified */
			fd_link,		/* link count */
			fd_fsize[4],	/* file size */
			fd_dcr[3];		/* date created */
	struct {
			char	addr[3],	/* segment begin LSN */
					size[2];	/* segment size (sectors) */
	} fdseg[48];
};

struct ddsect {
	char	dd_tot[3];		/* total number of sectors on media */
	char	dd_tks;			/* track size in sectors */
	short	dd_map;			/* number of bytes in allocation map */
	short	dd_bit;			/* disk cluster size (sectors/bit */
	char	dd_dir[3];		/* lsn of root directory */
	char	dd_own[2];		/* disk owner ID */
	char	dd_att;			/* disk attributes */
	short	dd_dsk;			/* disk ID */
	char	dd_fmt;			/* disk format (density/sides) */
	char	dd_spt[2];		/* sectors/track */
	char	dd_res[2];		/* reserved */
	char	dd_bt[3];		/* system bootstrap LSN */
	short	dd_bsz;			/* size of system bootstrap (bytes) */
	char	dd_date[5];		/* creation date */
	char	dd_name[32];	/* volume name */
	char	dd_opt[32];		/* PD options */
	unsigned char	
			dd_res2;		/* reserved */
	unsigned char
			dd_sync[4];		/* media integrity code */
	unsigned
			dd_maplsn;		/* bitmap starting lsn (valid if non-zero) */
	unsigned short
			dd_lsnsize;		/* logical sector size (0 = 256 bytes)	*/
	char	dd_null[150];	/* reserved */
};

#endif /* _DIRECT_H */
