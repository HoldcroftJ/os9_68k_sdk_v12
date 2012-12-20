/****************************************************************************
 *           Copyright 1989-1998 by Microware Systems Corporation           *
 *                           All Rights Reserved                            *
 *                         Reproduced Under License                         *
 *                                                                          *
 * This software is confidential property of Microware Systems Corporation, *
 * and is provided under license for internal development purposes only.    *
 * Reproduction, publication, distribution, or creation of derivative works *
 * in any form to any party other than the licensee is strictly prohibited, *
 * unless expressly authorized in writing by Microware Systems Corporation. *
 *                                                                          *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *  01 10/21/93 <----------- OS-9 NFS 1.3 Released --------------->    gkm
 *  02 06/23/94 changed "#  define OSK 1" to "#define OSK 1" for unix  gkm
 *  03 08/09/94 include string.h to prototype mem* functions           gkm
 *  04 08/12/94 Remove perror & fputc defines for the new compiler     gkm
 *              They are declared in stdio.h in the new compiler.
 *             >comment out define for gethostname, use original function.
 *  05 03/07/95 include rbf.h for OS9000                               gkm
 *  06 11/06/95 #define EINTR EALREADY changed from 0x02               pjw
 *  07 05/12/97 Change caddr_t from int to char *                      rds
 *  08 06/03/97 Updated to remove INET references                      pjw
 *  09 12/08/97 Removed timeval, timezone, EACCES, and EINTR           rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  10 05/19/98 Added #ifdef for multiple include protection           rds
 *  11 09/01/99 Added conditionals for C++ usage.                      djl
 *  12 05/03/00 Added support for threads                              rds
 ****************************************************************************/

/* take care of some misc. defs */

#ifndef _RPC_OS9_H
#define _RPC_OS9_H

#include <string.h>
#include <sys/types.h>
#include <sys/errno.h>

#ifdef _OS9000
	#include <rbf.h>
#endif

#ifndef OSK
#define OSK 1
#endif
#ifndef mc68000
#define mc68000
#endif

#define RPCDB "rpcdb"

/* take care of UNIX to OS-9 names */

#define bcopy(b1,b2,l) memcpy(b2,b1,l)
#define bcmp(b1,b2,l) memcmp(b1,b2,l)
#define bzero(b,l) memset(b,0,l)

#if !defined(_UCC) && !defined(DOS)
	#define perror(s) _errmsg(0,s)
	#define fputc putc
#endif

#	if		defined(__cplusplus)
extern "C" {
#	endif	/* __cplusplus */

int getuid(void);

#	if		defined(__cplusplus)
}
#	endif

#define geteuid() (getuid() & 0xffff)
#define getegid() ((getuid() & 0xffff0000)>>16)
#define getgroups(n,g) 0
#define OS9_dtablesize 32

/* take care of OS-9 to OS-9000 names */

#ifdef _OS9000
	#define MODEXEC(mod) ((u_char*)((u_char*)mod + ((Mh_com)mod)->m_exec))
	#define _f_gregor _gregorian
	#define fildes fd_stats
	/* changed for error */
	/*	#define _time_buf sgtbuf*/
	#define environ _environ
	#define sgbuf rbf_path_opts
	#define SECTSIZ(sg) sg.pd_bsize
#else
	#define MODEXEC(mod) ((u_char*)((u_char*)mod + ((mod_exec *)mod)->_mexec))
	/*** #define gethostname(n,l) os9_gethostname(n,l) ***/
	#define SECTSIZ(sg) sg.sg_sctsiz
#endif

#endif /* _RPC_OS9_H */
