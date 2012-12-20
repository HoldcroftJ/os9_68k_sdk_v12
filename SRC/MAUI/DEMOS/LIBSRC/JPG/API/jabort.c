/*******************************************************************************
* FUNCTION : jpg_abort()
*
* DESCRIPTION :
*
*	Abort JPEG decompression JPEG API.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  01/17/96  YAP  First version
*       2  05/08/00  SRM  Add simple thread support via mutex
*/
#include <jpgcom.h>

error_code jpg_abort(JPG_CONTEXT * context)
{
   _JPG_LOCK();

	/* Just return if the init has already been done */
	if (_jpg_initcnt == 0) {
        error_code err;
		err = _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_NOINIT, JPG_ABORT);
        _JPG_UNLOCK();
        return err;
	}
	jpeg_abort_decompress(&context->cinfo);

    _JPG_UNLOCK();
	return SUCCESS;
}

