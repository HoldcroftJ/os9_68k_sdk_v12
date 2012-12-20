/*******************************************************************************
* FUNCTION : jpg_get_context_params()
*
* DESCRIPTION :
*
*	Get draw context parameters.
*
* COPYRIGHT:
*
* This source code is the proprietary confidential property of Microware
* Systems Corporation, and is provided to licensee solely for documentation
* and educational purposes. Reproduction, publication, or distribution in
* form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  01/16/96  YAP  First version
*       2  05/08/00  SRM  Add simple thread support via mutex
*/
#include <jpgcom.h>


error_code jpg_get_context_params(JPG_CONTEXT_PARAMS *ret_context_params,
    JPG_CONTEXT *context)
{
    error_code err;
    _JPG_LOCK();

	/* Verify parameters */
	if (_jpg_initcnt == 0) {
        err = _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_NOINIT, 
                         JPG_GET_CONTEXT_PARAMS);
        _JPG_UNLOCK();
        return err;
	}

    if ((context == NULL) || (context->sync_code != _JPG_CONTEXT_SYNC)) {
        err = _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_BADID, 
                         JPG_GET_CONTEXT_PARAMS);
        _JPG_UNLOCK();
        return err;
	} 

	/* Copy the draw context parameters to the callers structure */

	(*ret_context_params).image_width= context->cinfo.image_width;
	(*ret_context_params).image_height= context->cinfo.image_height;
	(*ret_context_params).num_components= context->cinfo.num_components;
	(*ret_context_params).out_color_space= context->cinfo.out_color_space;	
	(*ret_context_params).scale_num= context->cinfo.scale_num;
	(*ret_context_params).buffered_image= context->cinfo.buffered_image;
	(*ret_context_params).dct_method= context->cinfo.dct_method;
	(*ret_context_params).do_fancy_upsampling= context->cinfo.do_fancy_upsampling;
	(*ret_context_params).do_block_smoothing= context->cinfo.do_block_smoothing;
	(*ret_context_params).quantize_colors= context->cinfo.quantize_colors;
	(*ret_context_params).dither_mode= context->cinfo.dither_mode;
	(*ret_context_params).two_pass_quantize= context->cinfo.two_pass_quantize;
	(*ret_context_params).desired_number_of_colors= context->cinfo.desired_number_of_colors;
	(*ret_context_params).enable_1pass_quant= context->cinfo.enable_1pass_quant;
	(*ret_context_params).enable_2pass_quant= context->cinfo.enable_2pass_quant;

	if(context->cinfo.output_width == 0 || context->cinfo.output_height == 0)
		jpeg_calc_output_dimensions(&context->cinfo);
	
	(*ret_context_params).output_width= context->cinfo.output_width;
	(*ret_context_params).output_height= context->cinfo.output_height;
	(*ret_context_params).out_color_components= context->cinfo.out_color_components;

    _JPG_UNLOCK();
	return SUCCESS;
}
