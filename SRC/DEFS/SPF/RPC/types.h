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
 *  01 10/21/93 <----------- OS-9 NFS 1.3 Released --------------->    gkm
 *     08/04/94 changed include to uppercase                           gkm
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 * 	   09/01/99 Added conditionals for use with C++.                   djl
 ****************************************************************************/

/* @(#)types.h	1.2 87/11/07 3.9 RPCSRC */
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
/*      @(#)types.h 1.18 87/07/24 SMI      */

#ifndef __TYPES_RPC_HEADER__
#define __TYPES_RPC_HEADER__

#include <RPC/os9.h>			/* os9 defs */

#define	bool_t	int
#define	enum_t	int
#define	FALSE	(0)
#define	TRUE	(1)
#define __dontcare__	-1
#ifndef NULL
#	define NULL 0
#endif

#if (!defined(_OSK) && !defined(_OS9000))

#	if	defined(__cplusplus)
extern "C" {
#	endif /* __cplusplus */

char *malloc();

#	if	defined(__cplusplus)
	}
#	endif

#endif
#define mem_alloc(bsize)	malloc(bsize)
#define mem_free(ptr, bsize)	free(ptr)

#ifndef makedev /* ie, we haven't already included it */
#include <types.h>
#if (defined(_OSK) || defined(_OS9000))
#define makedev
#endif
#endif

#if (!defined(_OSK) && !defined(_OS9000))
#include <time.h>
#endif

#endif /* ndef __TYPES_RPC_HEADER__ */

