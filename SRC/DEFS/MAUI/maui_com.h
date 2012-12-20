/*******************************************************************************
* FILENAME : maui_com.h
*
* DESCRIPTION :
*
*	This file contains definitions common to all MAUI API's.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* ED#  DATE      WHO  DETAIL
*   1  02/01/95  TJE  First version
*   2  01/11/00  srm  Moved error codes to errno.h
*   3  03/16/00  srm  Threads (MAUI 3.0 COMPAT_LEVEL 100)
*   4  12/18/00  srm  Added Extended GFX Device Capabilities (COMPAT_LEVEL 101)
*   5  02/12/01  srm  Moved MSG and CDB support to mauidrvr, full build
*                     all processors (COMPAT_LEVEL 102)
*   6  10/09/01  srm  Update v3.2 COMPAT_LEVEL 103
*/
#ifndef _MAUI_COM_H
#define _MAUI_COM_H

#include <errno.h>

/*******************************************************************************
* MAUI COMPATIBILITY LEVEL
*******************************************************************************/

#define MAUI_VERSION		"3.2"
#define MAUI_COMPAT_LEVEL	103

/*******************************************************************************
* MAUI TYPE DEFINITIONS
*******************************************************************************/

#ifndef _MAUI_DISABLE_BOOLEAN
#undef BOOLEAN
#undef FALSE
#undef TRUE
#undef OFF
#undef ON
typedef int BOOLEAN;
#define FALSE 0
#define TRUE 1
#define OFF 0
#define ON 1
#endif

#define MSBFIRST 0				/* MSB First - Big endian */
#define LSBFIRST 1				/* LSB First - Little endian */

/*******************************************************************************
* MAUI ERROR LEVELS - The level of severity increases with each error type.
* Therefore NON_FATAL is more severe than WARNING.
*******************************************************************************/

typedef enum {
	MAUI_ERR_AS_IS,				/* Don't change current setting */
	MAUI_ERR_ANY,				/* Any error */
	MAUI_ERR_NOTICE,			/* Notice: No error code */
	MAUI_ERR_WARNING,			/* Warning: Operation completed anyway */
	MAUI_ERR_NON_FATAL,			/* Non-Fatal: Does not cause cascade failure */
	MAUI_ERR_FATAL,				/* Fatal: Causes cascade failure */
	MAUI_ERR_NONE,				/* No error */
	MAUI_ERR_DEFAULT			/* Restore default value */
} MAUI_ERR_LEVEL;

/*******************************************************************************
* MAUI ERROR CODES
*******************************************************************************/

/* The error codes have moved to SRC/DEFS/errno.h */

#endif /* _MAUI_COM_H */
