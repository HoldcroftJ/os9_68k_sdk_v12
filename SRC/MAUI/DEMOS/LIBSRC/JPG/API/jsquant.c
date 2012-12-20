/*******************************************************************************
* FUNCTION : jpg_set_context_quantization()
*
* DESCRIPTION :
*
*	Set quantization mode for JPEG API.
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

error_code jpg_set_context_quantization(JPG_CONTEXT * context, JPG_QUANT_LEVEL qt)
{
  error_code err = SUCCESS;

    _JPG_LOCK();

	/* Just return if the init has already been done */
	if (_jpg_initcnt == 0) {
        err = _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_NOINIT, 
                         JPG_SET_CONTEXT_QUANTIZATION);
        _JPG_UNLOCK();
        return err;
	}

	switch(qt)
	{
	case JPG_NO_QUANT:
		context->cinfo.quantize_colors=FALSE;
		context->cinfo.two_pass_quantize=FALSE;
		context->cinfo.enable_1pass_quant=FALSE;
		context->cinfo.enable_2pass_quant=FALSE;
		break;
	case JPG_1PASS:
		context->cinfo.quantize_colors=TRUE;
		context->cinfo.two_pass_quantize=FALSE;
		context->cinfo.enable_1pass_quant=TRUE;
		context->cinfo.enable_2pass_quant=FALSE;
		break;
	case JPG_2PASS:
		context->cinfo.quantize_colors=TRUE;
		context->cinfo.two_pass_quantize=TRUE;
		context->cinfo.enable_1pass_quant=FALSE;
		context->cinfo.enable_2pass_quant=TRUE;
		break;
	default:
        err = EOS_MAUI_BADVALUE;
	}

    _JPG_UNLOCK();

	return err;
}
