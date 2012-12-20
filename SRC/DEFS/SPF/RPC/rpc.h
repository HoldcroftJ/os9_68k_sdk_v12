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
 *  01 ??/??/?? Created
 *  02 08/04/94 changed includes to upper case for unix                gkm
 *  03 12/11/97 use BSD header files                                   rds
 *     01/08/98 include pmap_clnt.h                                    rds
 *     01/29/98 include rpc/inetdb.h prototypes here                   rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  04 09/01/99 Added conditionals for use with C++.                   djl
 ****************************************************************************/

/* @(#)rpc.h	1.2 87/11/13 3.9 RPCSRC */
/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 * 
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 * 
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 * 
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 * 
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */
/*	@(#)rpc.h 1.8 87/07/24 SMI	*/

/*
 * rpc.h, Just includes the billions of rpc header files necessary to
 * do remote procedure calling.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 */

#ifndef __RPC_HEADER__
#define __RPC_HEADER__

#include <RPC/types.h>		/* some typedefs */
#include <netinet/in.h>

/* external data representation interfaces */
#include <RPC/xdr.h>		/* generic (de)serializer */

/* Client side only authentication */
#include <RPC/auth.h>		/* generic authenticator (client side) */

/* Client side (mostly) remote procedure call */
#include <RPC/clnt.h>		/* generic rpc stuff */
#include <RPC/pmap_clnt.h>	/* Portmapper client */

/* semi-private protocol headers */
#include <RPC/rpc_msg.h>	/* protocol for rpc messages */
#include <RPC/auth_unix.h>	/* protocol for unix style cred */

/* Server side only remote procedure callee */
#include <RPC/svc.h>		/* service manager and multiplexer */
#include <RPC/svc_auth.h>	/* service side authenticator */

#	if		defined(__cplusplus)
extern "C" {
#	endif	/* __cplusplus */

/* routines for parsing /etc/rpc */
struct rpcent *getrpcbyname(char *name);
struct rpcent *getrpcbynumber(int number);
struct rpcent *getrpcent(void);

#	if		defined(__cplusplus)
	}
#	endif

#endif /* ndef __RPC_HEADER__ */
