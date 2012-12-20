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
 *  03 02/14/98 Moved MSIZE to mbuf.h                                  rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/
/*
 * Copyright (c) 1988 University of Utah.
 * Copyright (c) 1992 OMRON Corporation.
 * Copyright (c) 1982, 1986, 1990, 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * the Systems Programming Group of the University of Utah Computer
 * Science Department.
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
 * from: Utah $Hdr: machparam.h 1.11 89/08/14$
 * from: hp300/include/param.h	7.13 (Berkeley) 12/27/92
 *
 *	@(#)param.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _SYS_MINFUNCS_H
#define _SYS_MINFUNCS_H

#include <types.h>

static inline int
 imax(int a, int b)
{
	return (a > b ? a : b);
}
static inline int
 imin(int a, int b)
{
	return (a < b ? a : b);
}
static inline long
 lmax(long a, long b)
{
	return (a > b ? a : b);
}
static inline long
 lmin(long a, long b)
{
	return (a < b ? a : b);
}
static inline u_int
 max(u_int a, u_int b)
{
	return (a > b ? a : b);
}
static inline u_int
 min(u_int a, u_int b)
{
	return (a < b ? a : b);
}
static inline u_long
 ulmax(u_long a, u_long b)
{
	return (a > b ? a : b);
}
static inline u_long
 ulmin(u_long a, u_long b)
{
	return (a < b ? a : b);
}

#endif /*_SYS_MINFUNCS_H*/
