/****************************************************************************
 *  ID:		%A%
 *  Date:	%G%
 ****************************************************************************
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
 *  01 10/07/96 Creation
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *  02 03/27/97 Changed to include endian.h                            tmd
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  03 06/04/97 Updated lseek to grab OSK/OS9000 parameters            pjw
 *  04 06/30/97 Added new copyright header                             apw
 *  05 07/29/97 Cleaned up UNIX/DOS conflicts and lseek                pjw
 *  06 11/04/97 Remove everything except what was needed by networking tmd
 *  07 02/05/98 Removed even more stuff we don't use                   rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_

#include <stddef.h>

/* Machine type dependent parameters. */
#include <sys/endian.h>

typedef unsigned int    uint;       /* Sys V compatibility */
#define ssize_t int

#ifndef DOS
#include <ansi_c.h>
#include <types.h>
#include <UNIX/os9types.h>


typedef	process_id		pid_t;		/* process id */
#endif

#if	!defined(_CADDR_T)
#	define	_CADDR_T
typedef	char *		caddr_t;	/* core address */
#endif

#define	NBBY	8		/* number of bits in a byte */

#if defined(__STDC__) && defined(KERNEL)
/*
 * Forward structure declarations for function prototypes.  We include the
 * common structures that cross subsystem boundaries here; others are mostly
 * used in the same place that the structure is defined.
 */
struct	proc;
struct	pgrp;
struct	ucred;
struct	rusage;
struct	file;
struct	buf;
struct	tty;
struct	uio;
#endif

#endif /* !_SYS_TYPES_H_ */
