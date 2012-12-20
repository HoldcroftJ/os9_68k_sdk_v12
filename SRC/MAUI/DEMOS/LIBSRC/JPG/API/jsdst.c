/*******************************************************************************
* FUNCTION : jpg_set_context_dst()
*
* DESCRIPTION :
*
*	Set output image destination drawmap
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

error_code jpg_set_context_dst(JPG_CONTEXT *context, GFX_DMAP *dmap)
{
    _JPG_LOCK();

	/* Make sure jpg_init() has been called */
	if (_jpg_initcnt == 0) {
        error_code err;
        err = _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_NOINIT, 
                         JPG_SET_CONTEXT_DST);
        _JPG_UNLOCK();
        return err;
	}
	context->dstdmap = dmap;

    _JPG_UNLOCK();
	return SUCCESS;
}

