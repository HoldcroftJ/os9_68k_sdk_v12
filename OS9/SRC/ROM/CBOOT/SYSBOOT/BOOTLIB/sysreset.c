/*
 * Copyright 1990, 1993 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include <sysboot.h>

/*--------------------------------------------------------------!
! sysreset: Restart the system from dead start initialization	!
!--------------------------------------------------------------*/
void sysreset()
{
	
	( (void (*)()) ((void *) &(bootjmp->sysreset)) )();
}
