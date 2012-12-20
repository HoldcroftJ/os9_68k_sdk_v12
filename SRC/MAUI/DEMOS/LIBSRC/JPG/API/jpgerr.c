/*******************************************************************************
* FUNCTION : jpg_set_error_action()
*
* DESCRIPTION :
*
*	Set actions performed by error handler.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  06/12/95  TJE  First version
*       2  05/08/00  SRM  Add simple thread support via mutex
*/
#include <jpgcom.h>

error_code jpg_set_error_action(MAUI_ERR_LEVEL debug_level, MAUI_ERR_LEVEL
	passback_level, MAUI_ERR_LEVEL exit_level)
{

    _JPG_LOCK();

	/* Set global variables */
	if (debug_level != MAUI_ERR_AS_IS) {
		if (debug_level == MAUI_ERR_DEFAULT) {
			_jpg_debug_level = MAUI_ERR_ANY;
		} else {
			_jpg_debug_level = debug_level;
		}
	}
	if (passback_level != MAUI_ERR_AS_IS) {
		if (passback_level == MAUI_ERR_DEFAULT) {
			_jpg_passback_level = MAUI_ERR_NON_FATAL;
		} else {
			_jpg_passback_level = passback_level;
		}
	}
	if (exit_level != MAUI_ERR_AS_IS) {
		if (exit_level == MAUI_ERR_DEFAULT) {
			_jpg_exit_level = MAUI_ERR_NONE;
		} else {
			_jpg_exit_level = exit_level;
		}
	}
	
    _JPG_UNLOCK();

	return SUCCESS;
}
