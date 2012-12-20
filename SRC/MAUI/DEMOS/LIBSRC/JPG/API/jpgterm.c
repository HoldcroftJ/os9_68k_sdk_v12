/*******************************************************************************
* FUNCTION : jpg_term()
*
* DESCRIPTION :
*
*	Terminate use of the draw functions.
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

error_code jpg_term(void)
{
	error_code ec;

    _JPG_LOCK()

	/* Make sure jpg_init() has been called */
	if (_jpg_initcnt == 0) {
      ec = _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_NOINIT, JPG_TERM);
      _JPG_UNLOCK();
      return ec;
	}

    /* Just return if not the last terminate */
	if (_jpg_initcnt > 1) {
		_jpg_initcnt--;
        _JPG_UNLOCK();
		return SUCCESS;
	}

	/* Terminate APIs this one depends on */
	ec = mem_term();

	/* Return error code or SUCCESS */
	if (ec != SUCCESS) {
        ec = _jpg_error(MAUI_ERR_NON_FATAL, ec, JPG_TERM);
	} else {
		_jpg_initcnt--;
    }

    _JPG_UNLOCK();
    return ec;
}
