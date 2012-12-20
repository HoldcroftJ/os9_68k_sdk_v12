/*******************************************************************************
*
* DESCRIPTION :
*
*	This file contains internal functions used to support the Draw API.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  01/16/96  YAP  First version
*       2  05/08/00  SRM  Add simple thread support via mutex
*/
#define JPG_MAIN
#include <jpgcom.h>
#include <stdio.h>
/*******************************************************************************
* NAME:		_jpg_error()
*
* USAGE:	Error handler for this API
*/

#define MAUI_ERR_BITS 16
#define MAUI_ERR_MASK 0xff

error_code _jpg_error(MAUI_ERR_LEVEL errlevel, error_code ec, JPG_FUNC func)
{
	/* Return SUCCESS if SUCCESS is passed in */
	if (ec == SUCCESS) {
		return SUCCESS;
	}

	/* Print debug message if required */
	if (errlevel >= _jpg_debug_level) {
		switch (errlevel) {
			case MAUI_ERR_NOTICE:
				fprintf (stderr, "MAUI Notice: ");
				break;
			case MAUI_ERR_WARNING:
				fprintf (stderr, "MAUI Warning: ");
				break;
			case MAUI_ERR_NON_FATAL:
				fprintf (stderr, "MAUI Non-Fatal: ");
				break;
			case MAUI_ERR_FATAL:
				fprintf (stderr, "MAUI Fatal: ");
				break;
		}
		fprintf (stderr, "%d:%d detected in %s.\n", ec >> MAUI_ERR_BITS,
			ec & MAUI_ERR_MASK, _jpg_func_names[func]);
	}

	/* Exit if required */
	if (errlevel >= _jpg_exit_level) {
		_jpg_exit_level = MAUI_ERR_NONE;	/* Just in case jpg_term() gets */
											/* a fatal error. */
		jpg_term();
		exit (ec);
	}

	/* Pass back error code if required */
	if (errlevel >= _jpg_passback_level) {
		return ec;
	} else {
		return SUCCESS;
	}
}

