/*******************************************************************************
* FUNCTION : jpg_set_context_buffering()
*
* DESCRIPTION :
*
*	Set buffering mode for JPEG API.
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

error_code jpg_set_context_buffering(JPG_CONTEXT * context, JPG_BUFF_LEVEL buff)
{
    error_code err = SUCCESS;
    _JPG_LOCK();

	/* Just return if the init has already been done */
	if (_jpg_initcnt == 0) {
		err = _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_NOINIT, 
                         JPG_SET_CONTEXT_BUFFERING);
        _JPG_UNLOCK();
        return err;
	}

	switch(buff)
	{
	case JPG_NO_BUFF:
		context->cinfo.buffered_image=FALSE;
		break;
	case JPG_BUFF:
		context->cinfo.buffered_image=TRUE;
		break;
	default:
		err = EOS_MAUI_BADVALUE;
	}

    _JPG_UNLOCK();
	return err;
}
