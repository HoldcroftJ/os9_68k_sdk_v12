/*******************************************************************************
* FUNCTION : jpg_init()
*
* DESCRIPTION :
*
*	Initialize JPEG API.
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
#include <jpgcom.h>

error_code jpg_init(void)
{
	error_code ec;

    _JPG_LOCK()

	/* Just return if the init has already been done */
	if (_jpg_initcnt++ != 0) {
        _JPG_UNLOCK();
		return SUCCESS;
	}

	/* Initialize APIs this one depends on */
	if ((ec = mem_init()) != SUCCESS) {
		_jpg_initcnt--;
		ec = _jpg_error(MAUI_ERR_FATAL, ec, JPG_INIT);
    }

    _JPG_UNLOCK()
    return ec;
}
