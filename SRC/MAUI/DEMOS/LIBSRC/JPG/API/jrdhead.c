/*******************************************************************************
* FUNCTION : jpg_read_header()
*
* DESCRIPTION :
*
*	Read JPEG file header.
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

error_code jpg_read_header(JPG_CONTEXT * context, BOOLEAN image_req)
{
    _JPG_LOCK();

	/* Just return if the init has already been done */
	if (_jpg_initcnt == 0) {
        error_code err;
		err = _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_NOINIT, JPG_READ_HEADER);
        _JPG_UNLOCK();
        return err;
	}
	jpeg_abort((j_common_ptr)&context->cinfo);
	jpeg_read_header(&context->cinfo,image_req);
	jpeg_calc_output_dimensions(&context->cinfo);

    _JPG_UNLOCK();
	return SUCCESS;
}

