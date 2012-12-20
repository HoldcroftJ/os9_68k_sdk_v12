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
 *  01 10/07/96 Creation
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  02 06/30/97 Added new copyright header                             apw
 *  03 10/15/97 Uncommented MAXHOSTNAMELEN and set it to 128           tmd
 *  04 01/19/98 Wrapped MIN & MAX with #if !defined                    tmd
 *  05 04/02/98 Removed include of resource.h                          rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  06 06/08/99 Removed all the unused BSD defines                     rds
 ****************************************************************************/
/*-
 * Copyright (c) 1982, 1986, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)param.h	8.2 (Berkeley) 1/21/94
 */

#ifndef _SYS_PARAM_H
#define _SYS_PARAM_H

#define	BSD	199306		/* System version (year & month). */
#define BSD4_3	1
#define BSD4_4	1

#ifndef NULL
#define	NULL	0
#endif

#ifndef LOCORE
#include <sys/types.h>
#endif

#ifndef NOFILE					/* max open files per process */
/* Also defined in UNIX/os9def.h */
#if defined(_OSK)
#define NOFILE  32
#elif defined(_OS9000)
/*#define NOFILE  256*/
#endif /*_OSK*/
#endif

#define MAXHOSTNAMELEN	128		/* max hostname size */

/* More types and definitions used throughout the kernel. */
#ifdef KERNEL
#include <sys/cdefs.h>
/*#include <sys/errno.h>*/
#include <sys/time.h>
/*#include <sys/resource.h>*/
/*#include <sys/ucred.h>*/
#include <sys/uio.h>
#endif

/* Machine type dependent parameters. */
#include <sys/minfuncs.h>
#include <limits.h>

#ifndef MAXPATHLEN
/* Also defined in UNIX/os9def.h */
#define	MAXPATHLEN	1024
#endif

/* Bit map related macros. */
#define	setbit(a,i)	((a)[(i)/NBBY] |= 1<<((i)%NBBY))
#define	clrbit(a,i)	((a)[(i)/NBBY] &= ~(1<<((i)%NBBY)))
#define	isset(a,i)	((a)[(i)/NBBY] & (1<<((i)%NBBY)))
#define	isclr(a,i)	(((a)[(i)/NBBY] & (1<<((i)%NBBY))) == 0)

/* Macros for counting and rounding. */
#ifndef howmany
#define	howmany(x, y)	(((x)+((y)-1))/(y))
#endif
#define	roundup(x, y)	((((x)+((y)-1))/(y))*(y))
#define powerof2(x)	((((x)-1)&(x))==0)

/* Macros for min/max. */
#ifndef KERNEL
#ifndef MIN
#define	MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define	MAX(a,b) (((a)>(b))?(a):(b))
#endif
#endif /* KERNEL */

#endif /*_SYS_PARAM_H*/
