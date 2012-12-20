/*******************************************************************************
* FUNCTION : jpg_destroy_context()
*
* DESCRIPTION :
*
*	Destory the specified draw context object.
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

error_code jpg_destroy_context(JPG_CONTEXT *context)
{
	error_code ec;
    _JPG_LOCK()
	
	/* Validate parameters */
	if (_jpg_initcnt == 0) {
        ec = _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_NOINIT,
                         JPG_DESTROY_CONTEXT);
        _JPG_UNLOCK();
        return ec;
	}

	if ((context == NULL) || (context->sync_code != _JPG_CONTEXT_SYNC)) 
	{
        ec = _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_BADID, 
                         JPG_DESTROY_CONTEXT);
        _JPG_UNLOCK();
        return ec;
	}

	/* Release JPEG decompression object */
	jpeg_destroy_decompress(&context->cinfo);

	/* Return memory for the context object */
	context->sync_code = 0;
	if ((ec = mem_free(context)) != SUCCESS) {
        ec = _jpg_error(MAUI_ERR_NON_FATAL, ec, JPG_DESTROY_CONTEXT);
	}

    _JPG_UNLOCK();
	return ec;
}
