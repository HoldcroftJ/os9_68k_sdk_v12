/*---------------------------------------------------------------------------,
|                                                                            |
|              Copyright 1999 by Microware Systems Corporation.              |
|              All Rights Reserved.   Reproduced Under License.              |
|                                                                            |
|  This software is confidential property of Microware Systems Corporation,  |
|  and is provided under license for internal development purposes only.     |
|  Reproduction, publication, modification, distribution, or creation of     |
|  derivative works in any form to any party other than the Licensee is      |
|  strictly prohibited, unless expressly provided for under the terms of a   |
|  mutually executed written license agreement for this software between     |
|  the end-user and Microware Systems Corporation, or otherwise expressly    |
|  authorized in writing by Microware Systems Corporation.  Licensee shall   |
|  reproduce this copyright notice exactly as provided herein on any copies  |
|  of the software that Licensee is authorized to distribute.                |
|                                                                            |
|----------------------------------------------------------------------------|
|                                                                            |
| Edition History:                                                           |
| #   Date     Comments                                                 By   |
| --- -------- -------------------------------------------------------- ---- |
|   1 95/05/10 Created                                                  rcb  |
|	2 96/05/12 Optional prototypes added                                rry  |
|	3 97/04/08 Prototypes now defined when compiling C++				tag  |
|   4 98/02/20 Added header.                                            nan  |
|   5 99/09/20 Updated header, changed stat() arg to be const.          cdh  |
|              $$                    <RELEASE_INFO>                     $$   |
`---------------------------------------------------------------------------*/

/*
 * OS-9/OS-9000 implementation specific code, enhancements and modifications:
 *
 * Copyright 1991 Microware Systems Corporation
 * All Rights Reserved
 *
 * Use and/or distribution subject to terms and conditions of applicable
 * Microware license agreement(s).
 *
 * Current Version: @(#)stat.h	1.4 1/14/93
 */

#if !defined(_STAT_H)
#define _STAT_H
 
#if !defined(_TIME_H)
#include <time.h>
#endif

/*
 * file control structure
 */

struct stat{
	unsigned short	st_mode;
	unsigned short	st_nlink;
	unsigned short	st_uid;
	unsigned short	st_gid;
	unsigned long	st_size;
	time_t			st_atime;
	time_t			st_mtime;
	time_t			st_ctime;
	long			st_ino;
	short			st_dev;
	short			st_rdev;
};

#if !defined(_MODES_H)
#include <modes.h>
#endif

#if defined(__cplusplus)
extern "C" {
#	define _OPT_PROTOS			/* C++ needs prototypes */
#endif /* __cplusplus */

#if defined(_OPT_PROTOS)

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#else
#define _OP(x) ()
#endif

int fstat _OP((unsigned int, struct stat *));
int stat _OP((const char *, struct stat *));


#undef _OP

#endif /* _OPT_PROTOS */

#if defined(__cplusplus)
}
#	undef _OPT_PROTOS
#endif /* __cplusplus */
#endif /* _STAT_H */
