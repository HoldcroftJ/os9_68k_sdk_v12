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

#include <sysboot.h>

char		barray_count = 0;
char        ibdrvrjicpad[3];
/*----------------------------------------------!
! iniz_boot_driver: Initialize a boot driver	!
!----------------------------------------------*/
error_code iniz_boot_driver(address, name, menuline, idstring)
void					*address;
char					*name;
char					*menuline;
char					*idstring;
{
	register Bdrivdef	bdriver;	/* boot function structure pointer	*/
	register error_code error;
	u_int32				arraysize;
	
	if (bootdriver == NULL) {
		arraysize = bdrivers_qty * sizeof(bdrivdef);
		if ((error = extract(&arraysize, &bootdriver)) != SUCCESS) {
			outstr("Can't allocate memory for \"boot driver array\".\n");
			return error;
		}
		bdriver = bootdriver;
		for (arraysize = arraysize/4; --arraysize > 0;)
			*(((u_int32 *) bdriver)++) = 0;
	}
		
	if (barray_count < bdrivers_qty) {
		bdriver = &bootdriver[barray_count++];
		bdriver->driver_entry = address;
		bdriver->driver_name = name;
		bdriver->menuline = menuline;
		bdriver->idstring = idstring;
	} else return E_PARAM;
	return SUCCESS;
}
