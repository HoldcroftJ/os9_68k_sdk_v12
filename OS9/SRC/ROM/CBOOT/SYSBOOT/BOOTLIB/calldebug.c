#include <sysboot.h>

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

/*--------------------------------------------------!
! calldebug: invoke system level debugger			!
!---------------------------------------------------!
! NOTE: if no system level debugger this function	!
!		will quickstart the system					!
!--------------------------------------------------*/
void calldebug()
{
	( (void (*)()) ((void *) &(bootjmp->rstdebug)) )();
}

