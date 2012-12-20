/****************************************************************************
 *	File:	@(#) spsin.h 1.3@(#)
 *	ID:		5/30/96
 ****************************************************************************
 * Copyright (c) 1995 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of Microware 
 * Systems Corporation, and is provided to licensee solely for documentation 
 * and educational purposes. Reproduction, publication, or distribution in
 * any form to any party other than the licensee is strictly prohibited.
 *
 ****************************************************************************
 *
 * ed#   date    changes                                          	by
 * --- -------- -------------------------------------------------	---
 *  1  08/27/97 Created                                             tmd
 ****************************************************************************/

#ifndef _SPSNMP_H
#define _SPSNMP_H

/*  Driver Setstat Codes     */

#define SS_SPSNMP_BASE  (SPF_PR_SNMP_MIBII << 16) 

/*--------------------------------------------------*/
/* IP Protocol Driver Codes                         */
/*--------------------------------------------------*/

#define SNMP_GET_DATA (SS_SPSNMP_BASE + 1)
#define SNMP_PUT_DATA (SS_SPSNMP_BASE + 2)

typedef struct {
	const void *src;     /* where to copy data from */
	void *dst;     /* where to copy data to */
	u_int32 size;  /* how much data to copy */
} snmp_pb;

#endif /* _SPSNMP_H */
