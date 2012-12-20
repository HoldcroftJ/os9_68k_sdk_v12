/* structure for manipulating OS-9 directories in a (more)
	transportable manner.  The method and names work like
	the BSD4.2 directory calls
*/
#if !defined(_DIR_H)
#define _DIR_H

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
|  02 99/07/06 CF2710 - no void return-type on dir access functions    ajk  |
|              in back-compat mode.                                         |
|  03 01/03/05 Add const at appropriate								   mgh  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

#define MAXNAMLEN 32
#define DIRBLKSIZ 256

struct direct {
	long	d_addr;				/* fildes address (no inodes on OS-9) */
	short	d_reclen;			/* (not used) */
	short	d_namlen;			/* (not used) */
	char	d_name[MAXNAMLEN+1];/* directory entry name */
};

typedef struct _dirdesc {
	int		dd_fd;				/* fd for open directory */
	long	dd_loc;				/* (not used) */
	long	dd_size;			/* (not used) */
	char	dd_buf[DIRBLKSIZ];	/* buffer for directory entry */
} DIR;

#if defined(__cplusplus) || defined(_ANSI_EXT) || defined(__STDC__)
#define VOID void
#define _CONST const
#else
#define VOID 
#define _CONST
#endif

#if defined(__cplusplus) || ((defined(_ANSI_EXT) || defined(__STDC__)) && defined(_OPT_PROTOS))
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

VOID closedir _OP((DIR *));
DIR *opendir _OP((_CONST char *));
struct direct *readdir _OP((DIR *));
VOID rewinddir _OP((DIR *));
VOID seekdir _OP((DIR *, long));
long telldir _OP((DIR *));

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _DIR_H */
