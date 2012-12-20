/*
 * Copyright 1990 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include <types.h>
#include <gendefs.h>

extern void	*exjmptab;

/*------------------------------------------------------------------!
! setexcpt: Install an exception service routine directly into the	!
!			exception jump table									!
!------------------------------------------------------------------*/
u_int32 setexcpt(vector, irqsvc)
u_char		vector;
u_int32		irqsvc;
{
	register u_int32	*ejt_entry;
	u_int32 			orgaddr;
	
	ejt_entry = (u_int32 *)(((u_int32) exjmptab) + ((vector - 2) * 10) + 6);
	orgaddr = *ejt_entry;
	*ejt_entry = irqsvc;
	return orgaddr;
}
