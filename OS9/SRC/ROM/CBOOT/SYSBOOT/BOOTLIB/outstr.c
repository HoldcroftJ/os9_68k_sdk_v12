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

#include <gendefs.h>

/*------------------------------------------!
! outstr: Print a null terminated string	!
!		  using boot.a's "OutChar" routine	!
!------------------------------------------*/
error_code outstr(msgptr)
register char	*msgptr;
{
	while (*msgptr != 0)
		OutChar(*msgptr++);
	return SUCCESS;
}
