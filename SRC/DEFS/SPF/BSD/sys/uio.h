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
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  02 06/30/97 Added new copyright header                             apw
 *  03 01/28/98 Changed uio_offset from off_t to u_int32               tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/
/*
 * Copyright (c) 1982, 1986, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
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
 *	@(#)uio.h	8.5 (Berkeley) 2/22/94
 */

#ifndef _SYS_UIO_H_
#define	_SYS_UIO_H_

/*
 * XXX

 */
struct iovec {
	char	*iov_base;	/* Base address. */
	size_t	 iov_len;	/* Length. */
};

enum	uio_rw { UIO_READ, UIO_WRITE };

/* Segment flag values. */
enum uio_seg {
	UIO_USERSPACE,		/* from user data space */
	UIO_SYSSPACE,		/* from system space */
	UIO_USERISPACE		/* from user I space */
};

struct uio {
	struct	iovec *uio_iov;
	int	uio_iovcnt;
#if (defined(_OSK)||defined(_OS9000))
	u_int32	uio_offset;
#else
	off_t	uio_offset;
#endif
	int	uio_resid;
	enum	uio_seg uio_segflg;
	enum	uio_rw uio_rw;
	struct	proc *uio_procp;
};

/*
 * Limits
 */
#define UIO_MAXIOV	1024		/* max 1K of iov's */
#define UIO_SMALLIOV	8		/* 8 on stack, else malloc */

#if !(defined(_OSK)||defined(_OS9000))
#ifndef	KERNEL
#include <sys/cdefs.h>

__BEGIN_DECLS
ssize_t	readv __P((int, const struct iovec *, int));
ssize_t	writev __P((int, const struct iovec *, int));
__END_DECLS
#endif /* !KERNEL */
#endif /* !_OSK||!_OS9000*/

#endif /* !_SYS_UIO_H_ */
