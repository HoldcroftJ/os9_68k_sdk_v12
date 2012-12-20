#if !defined(_MODES_H)
#define _MODES_H

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
|  02 01/03/02 Add const where appropriate                             mgh  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

/* file modes */
#define S_IFMT		0xff	/* mask for type of file */
#define S_IFDIR		0x80	/* directory */
#define S_ISIZE		0x20	/* set initial file size */
#define	S_IAPPEND	0x10	/* open in append mode */

/* permissions */
#define  S_IPRM		0xff		/* mask for permission bits */
#define  S_IREAD	0x01	 	/* owner read */
#define  S_IWRITE	0x02		/* owner write */
#define  S_IEXEC	0x04		/* owner execute */
#define  S_IOREAD	0x08		/* public read */
#define  S_IOWRITE	0x10		/* public write */
#define  S_IOEXEC	0x20		/* public execute */
#define  S_ISHARE	0x40		/* non-sharable */
#define	 S_IEXCL	0x100		/* don't recreate if file exists */


/* Same bit values as above, but hopefully easier to understand */

/* File Access Mode - mode */
#define	FAM_READ		0x01	/* read mode */
#define	FAM_WRITE		0x02	/* write mode */
#define	FAM_EXEC		0x04	/* execute mode, look in execution dir for file */
#define FAM_APPEND		0x10	/* append mode, all write go to end of file */
#define FAM_SIZE		0x20	/* initial size passed mode */
#define FAM_NONSHARE	0x40	/* non-sharable mode */
#define FAM_DIR			0x80	/* directory mode, allow an open on a directory */
#define FAM_NOCREATE	0x100	/* error returned on attempt to create existing file */

/* File Access Permission - perm */
#define	FAP_READ		0x01	/* file created with owner read attribute */
#define FAP_WRITE		0x02	/* file created with onwer write attribute */
#define FAP_EXEC		0x04	/* file created with owner execute attribute */
#define FAP_PREAD		0x08	/* file created with public read attribute */
#define FAP_PWRITE		0x10	/* file created with public write attribute */
#define FAP_PEXEC		0x20	/* file created with public execute attr */
#define FAP_NONSHARE	0x40	/* file created with non-sharable attribute */

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(_OPT_PROTOS) && !defined(_IO_H)
#include <io.h>
#endif

#if (defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus))
#define _OP(x) x
#define _CONST const
#else
#define _OP(x) ()
#define _CONST
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

error_code _os_chdir _OP((_CONST char *, u_int32));
error_code _os_close _OP((path_id));
error_code _os_create _OP((_CONST char *, u_int32, path_id *, u_int32, ...));
error_code _os_delete _OP((_CONST char *, u_int32));
error_code _os_dup _OP((path_id, path_id *));
error_code _os_makdir _OP((_CONST char *, u_int32, u_int32, ...));
error_code _os_open _OP((_CONST char *, u_int32, path_id *));
error_code _os_read _OP((path_id, void *, u_int32 *));
error_code _os_readln _OP((path_id, void *, u_int32 *));
error_code _os_seek _OP((path_id, u_int32));
error_code _os_write _OP((path_id, _CONST void *, u_int32 *));
error_code _os_writeln _OP((path_id, _CONST void *, u_int32 *));
error_code _os9_create _OP((_CONST char *, u_int32, path_id *, u_int32, ...));
error_code _os9_ss_open _OP((path_id));
error_code _os9_ss_close _OP((path_id));

#if defined(_OPT_PROTOS)
int access _OP((_CONST char *, int));
dev_list *attach _OP((_CONST char *, int));
int chdir _OP((_CONST char *));
int chmod _OP((_CONST char *, int));
int chown _OP((_CONST char *, int));
int chxdir _OP((_CONST char *));
int close _OP((int));
int creat _OP((_CONST char *, int));
int create _OP((_CONST char *, int, int, int));
int detach _OP((dev_list *));
int dup _OP((int));
long lseek _OP((int, long, int));
int makdir _OP((_CONST char *, int, int, int));
int mknod _OP((_CONST char *, int));
int open _OP((_CONST char *, int));
int read _OP((int, char *, unsigned int));
int readln _OP((int, char *, unsigned int));
int unlink _OP((_CONST char *));
int unlinkx _OP((_CONST char *, int));
int write _OP((int, _CONST char *, unsigned int));
int writeln _OP((int, _CONST char *, unsigned int));
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _MODES_H */
