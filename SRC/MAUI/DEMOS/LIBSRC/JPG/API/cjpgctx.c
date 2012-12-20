/*******************************************************************************
* FUNCTION : jpg_create_context()
*
* DESCRIPTION :
*
*	Create and initialize a JPEG context object.
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

error_code jpg_create_context(JPG_CONTEXT **ret_context)
{

	error_code ec;
	JPG_CONTEXT *jc;
	
    _JPG_LOCK()

	/* Verify parameters */
	if (_jpg_initcnt == 0) {
      ec = _jpg_error(MAUI_ERR_FATAL, EOS_MAUI_NOINIT, JPG_CREATE_CONTEXT);
      _JPG_UNLOCK();
      return ec;
	}

	/* Allocate (and clear) memory for the context object */
	if ((ec = mem_calloc(&jc, MEM_DEF_SHADE, sizeof(JPG_CONTEXT), 1)) != SUCCESS) 
	{
      ec =  _jpg_error(MAUI_ERR_FATAL, ec, JPG_CREATE_CONTEXT);
      _JPG_UNLOCK();
      return ec;
	}

	/* Initialize members of the context object to their default values */
	jc->sync_code = _JPG_CONTEXT_SYNC;
	
	jpeg_create_decompress(&(jc->cinfo));

	jc->cinfo.image_width=jc->cinfo.image_height=0;
	jc->cinfo.num_components=3;
	jc->cinfo.out_color_space=JCS_UNKNOWN;
	jc->cinfo.scale_num=jc->cinfo.scale_denom=1;
	jc->cinfo.buffered_image=FALSE;
	jc->cinfo.dct_method=JDCT_IFAST;
	jc->cinfo.do_fancy_upsampling=TRUE;
	jc->cinfo.do_block_smoothing=TRUE;
	jc->cinfo.quantize_colors=TRUE;
	jc->cinfo.dither_mode=JDITHER_FS;
	jc->cinfo.two_pass_quantize=FALSE;
	jc->cinfo.desired_number_of_colors=256;
	jc->cinfo.enable_1pass_quant=TRUE;
	jc->cinfo.output_width=jc->cinfo.output_height=0;
	jc->cinfo.out_color_components=3;

	jc->cinfo.err = jpeg_std_error(&jerr);
	/* Return the pointer as an ID */
	*ret_context = jc;

    _JPG_UNLOCK();
	return SUCCESS;
}
