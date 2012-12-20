/*******************************************************************************
* FUNCTION : jpg_decompress()
*
* DESCRIPTION :
*
*	Decompinate use of the draw functions.
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

#ifdef _VGA
#define WORD_BYTE_SWAP(x)    \
    if(big_end == 0) \
    *x = (u_int32)((((*x) & (u_int32)0xff00ff00) >> 8) | (((*x) & (u_int32)0x00ff00ff) << 8))
#else
#define WORD_BYTE_SWAP(x) 
#endif

static error_code _jpg_decompress_clut(JPG_CONTEXT * context, GFX_DIMEN x, GFX_DIMEN y,
										   int * scan_num, GFX_DIMEN max_width, GFX_DIMEN max_height, 
										   BOOLEAN * has_multi_scans);

static error_code _jpg_decompress_rgb(JPG_CONTEXT * context, GFX_DIMEN x, GFX_DIMEN y,
										   int * scan_num, GFX_DIMEN max_width, GFX_DIMEN max_height, 
										   BOOLEAN * has_multi_scans,u_int32 codingmethod);

static void write_4_scanline(JPG_CONTEXT *context, u_int8 *dst_line, u_int8 *src_line);

static u_char *tmp;
static u_char *t;
static u_int16 grc;



error_code jpg_decompress(JPG_CONTEXT * context, GFX_DIMEN x, GFX_DIMEN y,
						  int * scan_num, 
						  GFX_DIMEN max_width, GFX_DIMEN max_height, 
						  BOOLEAN * has_multi_scans)
{
    error_code err;
    _JPG_LOCK();

	switch(gfx_get_cm_name(context->dstdmap->coding_method))
	{
	case GFX_CM_8BIT:
	case GFX_CM_4BIT:
	case GFX_CM_2BIT:
        err = _jpg_decompress_clut(context, 
								   x,y,
								   scan_num, 
								   max_width, max_height, 
								   has_multi_scans);
        break;
		
	case GFX_CM_RGB555:
	err = _jpg_decompress_rgb(context, 
								   x, y,
								   scan_num, 
								   max_width, max_height, 
								   has_multi_scans,555);
	break;

	case GFX_CM_RGB565:
	err = _jpg_decompress_rgb(context, 
								   x, y,
								   scan_num, 
								   max_width, max_height, 
								   has_multi_scans,565);
        break;

	default:
		fprintf(stderr,"Coding method 0x%x not supported\n",
           context->dstdmap->coding_method );
		err = _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_BADVALUE, 
                         JPG_DECOMPRESS);
        break;
	}

    _JPG_UNLOCK();
    return err;
}

#define READ_LOOP_RGB555(cm)  while (context->cinfo.output_scanline < context->cinfo.output_height) \
		{\
             jpeg_read_scanlines(&context->cinfo, (JSAMPARRAY)&tmp, 1); \
             op=(u_int16*)out_ptr; \
			 if(context->cinfo.out_color_space == JCS_GRAYSCALE) \
		     { \
 	     	     for(count=0,t=tmp; count<context->cinfo.output_width; \
					    count++,t++) \
		         { \
				     *op= 0; \
					 grc=(*t >> 3);\
				     *op = *op | grc;\
				     *op = (*op <<5) | grc;\
				     *op = (*op <<5) + grc;\
					 WORD_BYTE_SWAP(op); \
				     op++;\
		         } \
	         } \
			 else \
	         { \
 	     	     for(count=0,t=tmp; count <context->cinfo.output_width; \
 	     	     	count++,t+=3) \
		         { \
				     *op= 0; \
				     *op = *op | (*t >> 3);\
				     *op = (*op <<5) | (*(t+1) >> 3);\
				     *op = (*op <<5) + (*(t+2) >> 3);\
				     WORD_BYTE_SWAP(op); \
				     	op++;\
		         } \
	         } \
		     out_ptr += line_size; \
     	 } 



#define SCAN_LOOP_RGB555(sss,cm)	jpeg_start_output(&context->cinfo, sss);\
					 READ_LOOP_RGB555(cm);\
				jpeg_finish_output(&context->cinfo);


/* don't really know if grayscale needs 6 bits of grc in second grc orr operation */
#define READ_LOOP_RGB565(cm)  while (context->cinfo.output_scanline < context->cinfo.output_height) \
		{\
             jpeg_read_scanlines(&context->cinfo, (JSAMPARRAY)&tmp, 1); \
             	op=(u_int16*)out_ptr; \
			 if(context->cinfo.out_color_space == JCS_GRAYSCALE) \
		     { \
 	     	     for(count=0,t=tmp; count <context->cinfo.output_width; \
					    count++,t++) \
		         { \
				     *op= 0; \
					 grc=(*t >> 3);\
				     *op = *op | grc;\
				     *op = (*op <<6) | grc;\
				     *op = (*op <<5) + grc;\
					 WORD_BYTE_SWAP(op); \
				     op++;\
		         } \
	         } \
			 else \
	         { \
 	     	     for(count=0,t=tmp; count <context->cinfo.output_width; \
 	     	     	count++,t+=3) \
		         { \
				     *op= 0; \
				     *op = *op | (*t >> 3);\
				     *op = (*op <<6) | (*(t+1) >> 2);\
				     *op = (*op <<5) + (*(t+2) >> 3);\
					 WORD_BYTE_SWAP(op); \
					 op++;\
		         } \
	         } \
		     out_ptr += line_size; \
     	 } 

#define SCAN_LOOP_RGB565(sss,cm)	jpeg_start_output(&context->cinfo, sss);\
					 READ_LOOP_RGB565(cm);\
				jpeg_finish_output(&context->cinfo);


static error_code 
_jpg_decompress_rgb(JPG_CONTEXT * context, GFX_DIMEN x, GFX_DIMEN y,
						  int * scan_num, 
						  GFX_DIMEN max_width, GFX_DIMEN max_height, 
						  BOOLEAN * has_multi_scans, u_int32 codingmethod)
{
	
	u_char * out_ptr;
	u_int32 line_size = context->dstdmap->line_size;
	error_code ec;
	int count=0;
	/*	BOOLEAN no_input;*/
	GFX_DMAP * dmap =context->dstdmap;
	u_int16 *op;
#ifdef _VGA
	u_int32 big_end=0;
#endif
	u_int32 x_bytes = x<<1;
	u_int32 cm;

	/* Make sure jpg_init() has been called */
	if (_jpg_initcnt == 0) {
		return _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_NOINIT, JPG_DECOMPRESS);
	}

#ifdef _VGA
	big_end = gfx_get_cm_byte_order(dmap->coding_method);
#endif

	cm = gfx_get_cm_name(dmap->coding_method);

	if((cm != GFX_CM_RGB555) && (cm != GFX_CM_RGB565))
		return _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_BADVALUE, 
						  JPG_DECOMPRESS);

	ec=mem_malloc(&tmp,MEM_DEF_SHADE,line_size*3);
	if(ec != 0)
	{
		fprintf(stderr,"Cannot allocate temp.memory\n");
		return _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_BADID, JPG_DECOMPRESS);
	}


	out_ptr = (u_char*)dmap->pixmem;
	out_ptr +=(x_bytes + (y * dmap->line_size));

	context->cinfo.quantize_colors=FALSE;
	jpeg_start_decompress(&context->cinfo);
	
	*has_multi_scans=jpeg_has_multiple_scans(&context->cinfo);

	/* Clipping part */
	if(dmap->width <= x)
		return SUCCESS;
	if (context->cinfo.output_width + x > dmap->width)
		context->cinfo.output_width=dmap->width - x;
	
	if(dmap->height <= y)
		return SUCCESS;
	if (context->cinfo.output_height + y > dmap->height)
		context->cinfo.output_height=dmap->height -y;    

	if(max_width != 0)
		if (context->cinfo.output_width > max_width)
			context->cinfo.output_width=max_width;

	if(max_height != 0)
		if (context->cinfo.output_height > max_height)
			context->cinfo.output_height=max_height;


	if(*has_multi_scans && context->cinfo.buffered_image)
	{
		if(*scan_num==0)
		{
			while(!jpeg_input_complete(&context->cinfo))
			{
			   if(codingmethod == 565)
			   {
				SCAN_LOOP_RGB565(context->cinfo.input_scan_number,codingmethod);
			   }
			   else	
			   {	
				SCAN_LOOP_RGB555(context->cinfo.input_scan_number,);
			   }
			}
			*scan_num=context->cinfo.input_scan_number;
		}
		else
		{
				if(codingmethod == 565)
				{
				 SCAN_LOOP_RGB565(*scan_num,codingmethod);
				}
				else
				{
				 SCAN_LOOP_RGB555(*scan_num,codingmethod);
				}
		}
	}
	else
	{
		if(codingmethod == 565)
		{
		 READ_LOOP_RGB565(codingmethod);
		}
		else
		{
		 READ_LOOP_RGB555(codingmethod);
		}  
	}
	jpeg_finish_decompress(&context->cinfo);
	mem_free(tmp);
	
	return SUCCESS;
}





#define READ_LOOP 	while (context->cinfo.output_scanline < context->cinfo.output_height) \
				    {\
                       num_scanlines=jpeg_read_scanlines(&context->cinfo,\
					     (JSAMPARRAY)&out_ptr,\
					     context->cinfo.rec_outbuf_height); \
					   out_ptr += num_scanlines*line_size; \
     				 }


#define SCAN_LOOP(sss)	jpeg_start_output(&context->cinfo, sss);\
					 READ_LOOP;\
				jpeg_finish_output(&context->cinfo);



#define READ_LOOP_4 	while (context->cinfo.output_scanline < context->cinfo.output_height) \
				     {\
						 jpeg_read_scanlines(&context->cinfo,(JSAMPARRAY)&tmp,1); \
						 op8=(u_int8*)out_ptr; \
						 write_4_scanline(context,op8,tmp);\
						 out_ptr += line_size; \
     				 }


#define SCAN_LOOP_4(sss)	jpeg_start_output(&context->cinfo, sss);\
					 READ_LOOP_4;\
				jpeg_finish_output(&context->cinfo);


#define READ_LOOP_2 	while (context->cinfo.output_scanline < context->cinfo.output_height) \
				     {\
						 num_scanlines=jpeg_read_scanlines(&context->cinfo,(JSAMPARRAY)&tmp,1); \
						 op8=(u_int8*)out_ptr; \
                         for(count=0,t=tmp; count <context->cinfo.output_width; count+=4,t+=4) \
						 { \
							   *op8= *t & 0xf; \
							   *op8 = (*op8 <<2) | *(t+1);\
							   *op8 = (*op8 <<2) | *(t+2);\
							   *op8 = (*op8 <<2) | *(t+3);\
							   op8++;\
						 } \
						 out_ptr += line_size; \
     				 }


#define SCAN_LOOP_2(sss)	jpeg_start_output(&context->cinfo, sss);\
					 READ_LOOP_2;\
				jpeg_finish_output(&context->cinfo);


static error_code _jpg_decompress_clut(JPG_CONTEXT * context, 
									  GFX_DIMEN x, GFX_DIMEN y,
									  int * scan_num, 
									  GFX_DIMEN max_width, 
									  GFX_DIMEN max_height, 
									  BOOLEAN * has_multi_scans)
{
	
	u_char * out_ptr;
	GFX_PALETTE * plt = context->dstdmap->palette;
	u_int32 line_size = context->dstdmap->line_size;
	error_code ec;
	JDIMENSION num_scanlines;
	int count=0;
	/*	BOOLEAN no_input;*/
	GFX_DMAP * dmap =context->dstdmap;
	u_int8 * op8;
	u_int32 x_bytes;
	
	/* Make sure jpg_init() has been called */
	if (_jpg_initcnt == 0) {
		return _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_NOINIT, JPG_DECOMPRESS);
	}
	if(plt == NULL)
		return _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_BADPTR, JPG_DECOMPRESS);
	

	switch(gfx_get_cm_name(dmap->coding_method))
	{
	case GFX_CM_4BIT:
	    x_bytes  = x>>1;
		ec=mem_malloc(&tmp,MEM_DEF_SHADE,line_size*2);
		break;
	case GFX_CM_2BIT:
	    x_bytes = x>>2;
		ec=mem_malloc(&tmp,MEM_DEF_SHADE,line_size*4);
		break;
	default:
	   x_bytes=x;
		ec=mem_malloc(&tmp,MEM_DEF_SHADE,line_size);
		break;
	}
	if(ec != 0)
	{
		fprintf(stderr,"Cannot allocate temp.memory\n");
		return _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_BADID, JPG_DECOMPRESS);
	}
	
	out_ptr = (u_char*)dmap->pixmem;
	out_ptr +=(x_bytes + (y * dmap->line_size));
	context->cinfo.quantize_colors=TRUE;
	jpeg_start_decompress(&context->cinfo);
	
	*has_multi_scans=jpeg_has_multiple_scans(&context->cinfo);
	
	/***************** CLUT part *******************/
	if(context->cinfo.out_color_space == JCS_GRAYSCALE)
		for (count = 0; count <context->cinfo.actual_number_of_colors; count++)
			plt->colors.rgb[count] = (context->cinfo.colormap[0][count]<<16) | 
				(context->cinfo.colormap[0][count]<<8) | 
					(context->cinfo.colormap[0][count]);
	else
		for (count = 0; count <context->cinfo.actual_number_of_colors; count++)
			plt->colors.rgb[count] = (context->cinfo.colormap[0][count]<<16) | 
				(context->cinfo.colormap[1][count]<<8) | 
					(context->cinfo.colormap[2][count]);
	/***************************************************************/
	
	
	/* Clipping part */
	if(dmap->width <= x)
		return SUCCESS;
	if (context->cinfo.output_width + x > dmap->width)
		context->cinfo.output_width=dmap->width - x;
	
	if(dmap->height <= y)
		return SUCCESS;
	if (context->cinfo.output_height + y > dmap->height)
		context->cinfo.output_height=dmap->height -y;    
	/************************/
	
	if(max_width != 0)
		if (context->cinfo.output_width > max_width)
			context->cinfo.output_width=max_width;
	
	if(max_height != 0)
		if (context->cinfo.output_height > max_height)
			context->cinfo.output_height=max_height;
	
	switch(gfx_get_cm_name(context->dstdmap->coding_method))
	{
	case GFX_CM_8BIT:
		if(*has_multi_scans && context->cinfo.buffered_image)
		{
			if(*scan_num==0)
			{
				while(!jpeg_input_complete(&context->cinfo))
				{
					SCAN_LOOP(context->cinfo.input_scan_number);
				}
				*scan_num=context->cinfo.input_scan_number;
			}
			else
			{
				SCAN_LOOP(*scan_num);
			}
		}
		else
		{
/*			READ_LOOP; */
			while (context->cinfo.output_scanline < context->cinfo.output_height) 
			{
				num_scanlines=jpeg_read_scanlines(&context->cinfo,
												  (JSAMPARRAY)&out_ptr,
												  context->cinfo.rec_outbuf_height); 
				out_ptr += num_scanlines*line_size; 
			}

		}
		break;
		
	case GFX_CM_4BIT:
		if(*has_multi_scans && context->cinfo.buffered_image)
		{
			if(*scan_num==0)
			{
				while(!jpeg_input_complete(&context->cinfo))
				{
					SCAN_LOOP_4(context->cinfo.input_scan_number);
				}
				*scan_num=context->cinfo.input_scan_number;
			}
			else
			{
				SCAN_LOOP_4(*scan_num);
			}
		}
		else
		{
			READ_LOOP_4;
		}
		break;
		
	case GFX_CM_2BIT:
		if(*has_multi_scans && context->cinfo.buffered_image)
		{
			if(*scan_num==0)
			{
				while(!jpeg_input_complete(&context->cinfo))
				{
					SCAN_LOOP_2(context->cinfo.input_scan_number);
				}
				*scan_num=context->cinfo.input_scan_number;
			}
			else
			{
				SCAN_LOOP_2(*scan_num);
			}
		}
		else
		{
			READ_LOOP_2; 
		}
		break;
		
	default:
		fprintf(stderr,"CLUT - Coding method %d not supported\n",context->dstdmap->coding_method);
		return _jpg_error(MAUI_ERR_NON_FATAL, EOS_MAUI_BADVALUE, 
						  JPG_DECOMPRESS);
	}
	
	jpeg_finish_decompress(&context->cinfo);
	mem_free(tmp);
	return SUCCESS;
}




static void write_4_scanline(JPG_CONTEXT *context, u_int8 *dst_line, u_int8 *src_line)
{
  int count = 0;
  u_int8 *ptr;
  int output_width = context->cinfo.output_width;
  u_int8 val;
  u_int8 p1,p2;
  u_int32 bit_swap;

  bit_swap = gfx_get_cm_bit_order(context->dstdmap->coding_method);

  for(count=0,ptr=src_line; count < output_width; count+=2, ptr += 2) {
	p1 = *ptr;
	p2 = *(ptr+1);
	if (bit_swap)
	  val = (p2 << 4) | (p1 & 0x0f);
	else
	  val = (p1 << 4) | (p2 & 0x0f);
	*dst_line = val;
	dst_line++;
  }
}
