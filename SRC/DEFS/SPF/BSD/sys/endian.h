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
 *  01 03/27/97 Creation
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  02 06/30/97 Added new copyright header                             apw
 *  03 01/15/98 Changed NTOHx macros to be nothing on big endian       rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/
/*
 * Copyright (c) 1987, 1991, 1993
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
 *	@(#)endian.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _SYS_ENDIAN_H_
#define	_SYS_ENDIAN_H_

/*
 * Define the order of 32-bit words in 64-bit words.
 */
#define _QUAD_HIGHWORD 0
#define _QUAD_LOWWORD 1

/* make sure we use this - TMD */
#undef _POSIX_SOURCE

#ifndef _POSIX_SOURCE
/*
 * Definitions for byte order, according to byte significance from low
 * address to high.
 */
#define	LITTLE_ENDIAN	1234	/* LSB first: i386, vax */
#define	BIG_ENDIAN	4321	/* MSB first: 68000, ibm, net */
#define	PDP_ENDIAN	3412	/* LSB first in word, MSW first in long */

#if defined(_BIG_END)
#define	BYTE_ORDER	BIG_ENDIAN
#else
#define	BYTE_ORDER	LITTLE_ENDIAN
#endif /* _BIG_END */

#include <sys/cdefs.h>

__BEGIN_DECLS
unsigned long	htonl __P((unsigned long));
unsigned short	htons __P((unsigned short));
unsigned long	ntohl __P((unsigned long));
unsigned short	ntohs __P((unsigned short));
__END_DECLS

/*
 * Macros for network/external number representation conversion.
 */
#if BYTE_ORDER == BIG_ENDIAN && !defined(lint)
#define	ntohl(x)	(x)
#define	ntohs(x)	(x)
#define	htonl(x)	(x)
#define	htons(x)	(x)

#define	NTOHL(x)
#define	NTOHS(x)
#define	HTONL(x)
#define	HTONS(x)

#else

#define	NTOHL(x)	(x) = ntohl((u_long)x)
#define	NTOHS(x)	(x) = ntohs((u_short)x)
#define	HTONL(x)	(x) = htonl((u_long)x)
#define	HTONS(x)	(x) = htons((u_short)x)
#endif
#endif /* !_POSIX_SOURCE */
#endif /* !_SYS_ENDIAN_H_ */
