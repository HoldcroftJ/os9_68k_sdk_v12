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
 *  01 04/30/97 Creation                                               rds
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  02 06/30/97 Added new copyright header                             apw
 *  03 10/28/97 Added EISDIR, EAGAIN                                   rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  04 05/25/00 Removed error mappings, now in SRC/DEFS/errno.h        rds
 ****************************************************************************/

#if !defined(_SYS_ERRNO_H) & !defined(DOS)
#define _SYS_ERRNO_H

/*
** This file used to contain some Unix/Posix errors that were mapped to
** various OS-9 errors. These have been moved to /mwos/SRC/DEFS/errno.h.
** This file needs to remain however, because ported applications may
** include <sys/errno.h>. This will then include our errno.h.
*/

#include <errno.h>

#endif /* _SYS_ERRNO_H */
