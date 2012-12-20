/*******************************************************************************
* FUNCTION : jpg_set_context_quality()
*
* DESCRIPTION :
*
*	Set quality level for JPEG API.
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

error_code jpg_set_context_quality(JPG_CONTEXT * context, JPG_QUALITY_LEVEL qual)
{
    error_code err = SUCCESS;

    _JPG_LOCK()

	/* Just return if the init has already been done */
	if (_jpg_initcnt == 0) {
      err = _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_NOINIT, 
                       JPG_SET_CONTEXT_QUALITY);
      _JPG_UNLOCK();
      return err;
	}

	switch(qual)
	{
	case JPG_LOW_QUAL:
		jpg_set_context_dct_method(context,JDCT_IFAST);
		jpg_set_context_dither(context,JDITHER_NONE);
		jpg_set_context_quantization(context,JPG_1PASS);
		context->cinfo.do_fancy_upsampling=TRUE;
		if (! context->cinfo.quantize_colors) 
			context->cinfo.desired_number_of_colors = 216;
		break;
	case JPG_MED_QUAL:
		jpg_set_context_dct_method(context,JDCT_IFAST);
		jpg_set_context_quantization(context,JPG_1PASS);
		jpg_set_context_dither(context,JDITHER_FS);
		context->cinfo.do_fancy_upsampling=TRUE;
		break;
	case JPG_HIGH_QUAL:
		jpg_set_context_dct_method(context,JDCT_ISLOW);
		jpg_set_context_quantization(context,JPG_2PASS);
		context->cinfo.do_fancy_upsampling=TRUE;
		jpg_set_context_dither(context,JDITHER_FS);
		break;
	default:
        err = EOS_MAUI_BADVALUE;
	}

    _JPG_UNLOCK();
	return SUCCESS;
}
