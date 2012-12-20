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
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/
/*-
 * Copyright (c) 1982, 1986, 1990, 1993, 1994
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
 *	@(#)ioccom.h	8.2 (Berkeley) 3/28/94
 */

#ifndef	_SYS_IOCCOM_H_
#define	_SYS_IOCCOM_H_

/*
 * Ioctl's have the command encoded in the lower word, and the size of
 * any in or out parameters in the upper word.  The high 3 bits of the
 * upper word are used to encode the in/out status of the parameter.
 */
#define	IOCPARM_MASK	0x1fff		/* parameter length, at most 13 bits */
#define	IOCPARM_LEN(x)	(((x) >> 16) & IOCPARM_MASK)
#define	IOCBASECMD(x)	((x) & ~(IOCPARM_MASK << 16))
#define	IOCGROUP(x)	(((x) >> 8) & 0xff)

#define	IOCPARM_MAX	NBPG		/* max size of ioctl, mult. of NBPG */
#define	IOC_VOID	0x20000000	/* no parameters */
#define	IOC_OUT		0x40000000	/* copy out parameters */
#define	IOC_IN		0x80000000	/* copy in parameters */
#define	IOC_INOUT	(IOC_IN|IOC_OUT)
#define	IOC_DIRMASK	0xe0000000	/* mask for IN/OUT/VOID */

#define	_IOC(inout,group,num,len) \
	(inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num))
#define	_IO(g,n)	_IOC(IOC_VOID,	(g), (n), 0)
#define	_IOR(g,n,t)	_IOC(IOC_OUT,	(g), (n), sizeof(t))
#define	_IOW(g,n,t)	_IOC(IOC_IN,	(g), (n), sizeof(t))
/* this should be _IORW, but stdio got there first */
#define	_IOWR(g,n,t)	_IOC(IOC_INOUT,	(g), (n), sizeof(t))

#endif /* !_SYS_IOCCOM_H_ */
