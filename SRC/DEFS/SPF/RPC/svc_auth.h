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
 *  01 08/04/94 started prototyping.                                   gkm
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  02 05/19/98 Added #ifdef for multiple include protection           rds
 *  03 09/01/99 Added conditionals for use with C++.                   djl
 ****************************************************************************/

/* @(#)svc_auth.h	1.1 87/11/04 3.9 RPCSRC */
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
/*      @(#)svc_auth.h 1.6 86/07/16 SMI      */

#ifndef _RPC_SVC_AUTH_H
#define _RPC_SVC_AUTH_H

/*
 * svc_auth.h, Service side of rpc authentication.
 * 
 * Copyright (C) 1984, Sun Microsystems, Inc.
 */

#include <RPC/rpc_msg.h>

#	if	defined(__cplusplus)
extern "C" {
#	endif	/* __cplusplus */

/*
 * Server side authenticator
 */
enum auth_stat _authenticate(struct svc_req *rqst, struct rpc_msg *msg);

#	if	defined(__cplusplus)
}
#	endif	/* __cplusplus */


#endif /* _RPC_SVC_AUTH_H */
