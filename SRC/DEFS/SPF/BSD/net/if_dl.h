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
/*
 * Copyright (c) 1990, 1993
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
 *	@(#)if_dl.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _NET_IF_DL_H
#define _NET_IF_DL_H

/* 
 * A Link-Level Sockaddr may specify the interface in one of two
 * ways: either by means of a system-provided index number (computed
 * anew and possibly differently on every reboot), or by a human-readable
 * string such as "il0" (for managerial convenience).
 * 
 * Census taking actions, such as something akin to SIOCGCONF would return
 * both the index and the human name.
 * 
 * High volume transactions (such as giving a link-level ``from'' address
 * in a recvfrom or recvmsg call) may be likely only to provide the indexed
 * form, (which requires fewer copy operations and less space).
 * 
 * The form and interpretation  of the link-level address is purely a matter
 * of convention between the device driver and its consumers; however, it is
 * expected that all drivers for an interface of a given if_type will agree.
 */

/*
 * Structure of a Link-Level sockaddr:
 */
struct sockaddr_dl {
	u_char	sdl_len;	/* Total length of sockaddr */
	u_char	sdl_family;	/* AF_DLI */
	u_short	sdl_index;	/* if != 0, system given index for interface */
	u_char	sdl_type;	/* interface type */
	u_char	sdl_nlen;	/* interface name length, no trailing 0 reqd. */
	u_char	sdl_alen;	/* link level address length */
	u_char	sdl_slen;	/* link layer selector length */
	char	sdl_data[12];	/* minimum work area, can be larger;
				   contains both if name and ll address */
};

#define LLADDR(s) ((caddr_t)((s)->sdl_data + (s)->sdl_nlen))

#ifndef KERNEL

#include <sys/cdefs.h>

__BEGIN_DECLS
void	link_addr __P((const char *, struct sockaddr_dl *));
char	*link_ntoa __P((const struct sockaddr_dl *));
__END_DECLS

#endif /* !KERNEL */
#endif /*_NET_IF_DL_H*/
