/*******************************************************************************
* FILENAME : maui_jpg.h
*
* DESCRIPTION :
*
*	This file contains definitions for applications that use the MAUI JPEG
*	API.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
*/

#ifndef _MAUI_JPG_H
#define _MAUI_JPG_H

#include <stddef.h>
#include <types.h>
#include <const.h>
#include <stdio.h>
#include <stdlib.h>
#include <MAUI/maui_com.h>
#include <MAUI/maui_mem.h>
#include <MAUI/maui_gfx.h>
/*******************************************************************************
* DEFINE ID TYPES
*******************************************************************************/

#ifdef _JPG_ID_STRUCTS_EXPOSED
#define JPG_CONTEXT_ID JPG_CONTEXT *
#include <cdjpeg.h>
#else
typedef void * JPG_CONTEXT_ID;

/* Known color spaces. */

typedef enum {
	JCS_UNKNOWN,		/* error/unspecified */
	JCS_GRAYSCALE,		/* monochrome */
	JCS_RGB,		/* red/green/blue */
	JCS_YCbCr,		/* Y/Cb/Cr (also known as YUV) */
	JCS_CMYK,		/* C/M/Y/K */
	JCS_YCCK		/* Y/Cb/Cr/K */
} J_COLOR_SPACE;


/* DCT/IDCT algorithm options. */

typedef enum {
	JDCT_ISLOW,		/* slow but accurate integer algorithm */
	JDCT_IFAST,		/* faster, less accurate integer method */
	JDCT_FLOAT		/* floating-point: accurate, fast on fast HW */
} J_DCT_METHOD;


#ifndef JDCT_DEFAULT		/* may be overridden in jconfig.h */
#define JDCT_DEFAULT  JDCT_ISLOW
#endif
#ifndef JDCT_FASTEST		/* may be overridden in jconfig.h */
#define JDCT_FASTEST  JDCT_IFAST
#endif

/* Dithering options for decompression. */

typedef enum {
	JDITHER_NONE,		/* no dithering */
	JDITHER_ORDERED,	/* simple ordered dither */
	JDITHER_FS		/* Floyd-Steinberg error diffusion dither */
} J_DITHER_MODE;



#endif




typedef struct _JPG_CONTEXT_PARAMS
{
	GFX_DIMEN image_width;
	GFX_DIMEN image_height;
	int num_components;
	J_COLOR_SPACE out_color_space;
	u_int32 scale_num;
	u_int32 scale_denom;
	BOOLEAN buffered_image;
	J_DCT_METHOD dct_method;
	BOOLEAN do_fancy_upsampling;
	BOOLEAN do_block_smoothing;
	BOOLEAN quantize_colors;
	J_DITHER_MODE dither_mode;
	BOOLEAN two_pass_quantize;
	int desired_number_of_colors;
	BOOLEAN enable_1pass_quant;
	BOOLEAN enable_2pass_quant;
	GFX_DIMEN output_width;
	GFX_DIMEN output_height;
	int out_color_components;
} JPG_CONTEXT_PARAMS;

typedef enum 
{
	JPG_NO_QUANT,JPG_1PASS, JPG_2PASS
} JPG_QUANT_LEVEL;

typedef enum 
{
	JPG_NO_BUFF, JPG_BUFF
} JPG_BUFF_LEVEL;

typedef enum 
{
	JPG_LOW_QUAL, JPG_MED_QUAL, JPG_HIGH_QUAL
} JPG_QUALITY_LEVEL;

/*******************************************************************************
 * PROTOTYPES
 *******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
	error_code      jpg_init (void);
	error_code 		jpg_term (void);
	error_code 		jpg_abort(JPG_CONTEXT_ID context);
	error_code      jpg_set_error_action(MAUI_ERR_LEVEL debug_level, MAUI_ERR_LEVEL
									passback_level, MAUI_ERR_LEVEL exit_level);
	error_code 		jpg_create_context(JPG_CONTEXT_ID *ret_context);
	error_code 		jpg_destroy_context(JPG_CONTEXT_ID context);
	error_code      jpg_set_context_source(JPG_CONTEXT_ID context, 
										   void * ptr, u_int32 size);
	error_code      jpg_set_context_out_colorspace(JPG_CONTEXT_ID context, 
												   J_COLOR_SPACE colorspace);
	error_code      jpg_set_context_quantization(JPG_CONTEXT_ID context,
												  JPG_QUANT_LEVEL quant_level);
	error_code      jpg_set_context_color_number(JPG_CONTEXT_ID context,
												 u_int16 col_number);
	error_code      jpg_set_context_out_size(JPG_CONTEXT_ID context,
											  GFX_DIMEN width, GFX_DIMEN height);
	error_code      jpg_set_context_buffering(JPG_CONTEXT_ID context,
											  JPG_BUFF_LEVEL buff_level);
	error_code 		jpg_set_context_dct_method(JPG_CONTEXT_ID context,
											   J_DCT_METHOD dct_method);
	error_code 		jpg_set_context_scale(JPG_CONTEXT_ID context,
										  u_int32 numerator, u_int32 denominator);
	error_code 		jpg_set_context_dither(JPG_CONTEXT_ID context,
										   J_DITHER_MODE dither_mode);
	error_code 		jpg_set_context_dst(JPG_CONTEXT_ID context,
										   GFX_DMAP * dstdmap);
	error_code 		jpg_get_context_params(JPG_CONTEXT_PARAMS *params,
										   JPG_CONTEXT_ID context);
	error_code      jpg_read_header(JPG_CONTEXT_ID context,
									BOOLEAN req_image);

	error_code      jpg_set_context_quality(JPG_CONTEXT_ID context,
											JPG_QUALITY_LEVEL quality);
	error_code 		jpg_decompress(JPG_CONTEXT_ID context, GFX_DIMEN x, GFX_DIMEN y,
								   int * scan_num, GFX_DIMEN max_width, GFX_DIMEN max_height, 
								   BOOLEAN * has_multi_scans);
	
#ifdef __cplusplus
}
#endif

#endif /* _MAUI_JPG_H */
