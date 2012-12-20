/*******************************************************************************
*
* DESCRIPTION:	Common functions for demo programs. The caller can use demo_init()
*               and demo_term() before calling functions in this file, or else they are 
*		responsible for calling the necessary individual *_init() functions 
*		needed before calling functions in this file.
*
* AUTHOR:       Todd Earles
*
* CREATED ON:   02/06/96
*
* COPYRIGHT:    Copyright 1994 Microware Systems Corporation. All Rights
*               Reserved. Use and/or distribution subject to terms and
*               conditions of applicable Microware license agreement(s).
*
*/
#define DEMO_GFX
#include "democom.h"

/* Swap macros */
static u_int32 swap32(u_int32 val);
static u_int32 swap16(u_int32 val);



/*******************************************************************************
* NAME:     demo_open_gfx_dev()
*
* USAGE:    Open the primary graphics device.
*/
error_code demo_open_gfx_dev(GFX_DEV_ID *ret_gfxdev)
{
    error_code ec;
    char devname[CDB_MAX_DNAME];

    /* Clear device ID in case we get an error */
    *ret_gfxdev = NULL;

    /* Get the name of the graphics device from the CDB */
    cdb_init(); /* Make sure we can make a cdb call. This returns fast
                   if already inited */
    if ((ec = cdb_get_ddr(CDB_TYPE_GRAPHIC, 1, devname, NULL)) != SUCCESS) {
        cdb_term();
        return demo_error(ec, "Failed to find the graphic device name in the CDB\n");
    }
    cdb_term();

    /* Open the device */
    if ((ec = gfx_open_dev(ret_gfxdev, devname)) != SUCCESS) {
        return demo_error(ec, "Failed to open graphic device \"%s\"\n", devname);
    }

    return SUCCESS;
}

/*******************************************************************************
* NAME:     demo_create_gfx_shade()
*
* USAGE:    Create a shade of memory whose request are satisfied from the
*           default color for the primary graphics device. The shade type is
*           returned in "ret_shade_type" if it is not NULL.
*
*           If no graphics memory is found, then EOS_NORAM is returned.
*/
error_code demo_create_gfx_shade(GFX_DEV_ID gfxdev, u_int32 shade_id,
    MEM_SHADE_TYPE *ret_shade_type)
{
    error_code ec;
    char params[CDB_MAX_PARAM], *ptr = NULL;
    u_int32 color;
    BOOLEAN gfxmem = FALSE;
    MEM_SHADE_TYPE shade_type = MEM_SHADE_NORMAL;
    
    cdb_init(); /* Make sure we can make a cdb call. This returns fast
                   if already inited */

    /* Search for graphics RAM in the SYSTEM entry in the CDB */
    if ((ec = cdb_get_ddr(CDB_TYPE_SYSTEM, 1, NULL, params)) != SUCCESS) {
        cdb_term();
        return ec;
    }
    if ((ptr = strstr(params, "GR#")) != NULL) {
        shade_type = MEM_SHADE_NORMAL;
    }

    /* If no graphics RAM found in the SYSTEM entry, then look in the */
    /* GRAPHIC entry. */
    if (ptr == NULL) {
        if ((ec = cdb_get_ddr(CDB_TYPE_GRAPHIC, 1, NULL, params)) != SUCCESS) {
            cdb_term();
            return ec;
        }
        if ((ptr = strstr(params, "GR#")) != NULL) {
            gfxmem = TRUE;
            shade_type = MEM_SHADE_NORMAL;
        } else {
            if ((ptr = strstr(params, "PR#")) != NULL) {
                gfxmem = TRUE;
                shade_type = MEM_SHADE_PSEUDO;
            } else {
                cdb_term();
                return EOS_NORAM;
            }
        }
    }

    cdb_term();

    /* Create the shade */
    color = atoi(strstr(ptr, ",") + 1);
    if ((ec = mem_create_shade(shade_id, shade_type, color, 0, 1,
        MEM_OV_SEPARATE, FALSE)) != SUCCESS) {
        return ec;
    }

    /* If graphics memory, then setup allocator and deallocator */
    if (gfxmem == TRUE) {
        if ((ec = mem_set_alloc(shade_id, gfx_alloc_mem, gfxdev, 0))
            != SUCCESS) {
            mem_destroy_shade(shade_id);
            return ec;
        }
        if ((ec = mem_set_dealloc(shade_id, gfx_dealloc_mem, gfxdev))
            != SUCCESS) {
            mem_destroy_shade(shade_id);
            return ec;
        }
    }

    /* Return shade type if it was asked for */
    if (ret_shade_type != NULL) {
        *ret_shade_type = shade_type;
    }
    return SUCCESS;
}

/*******************************************************************************
* NAME:     demo_clear_dmap()
*
* USAGE:    Clear the drawmap to the specified pixel value.
*/
error_code demo_clear_dmap(GFX_DEV_ID gfxdev, GFX_DMAP *dmap, GFX_PIXEL pixval)
{
    error_code ec;
    BLT_CONTEXT_ID bltctx;

    /* Create a Bit-BLT context */
    if ((ec = blt_create_context(&bltctx, gfxdev)) != SUCCESS) {
        return ec;
    }

    /* Clear the drawmap */
    blt_set_context_dst(bltctx, dmap);
    blt_set_context_pix(bltctx, pixval);
    blt_draw_block(bltctx, 0, 0, dmap->width, dmap->height);

    /* Destroy the Bit-BLT context */
    blt_destroy_context(bltctx);

    return SUCCESS;
}

/*******************************************************************************
* NAME:     demo_color_pixval()
*
* USAGE:    Lookup the specified "color_index" color from the "demo_colors"
*           array and convert it to a pixel value valid for the specified
*           "coding_method". The pixel value is returned in "ret_pixval".
*/
error_code demo_color_pixval(u_int16 color_index, GFX_CM coding_method,
    GFX_PIXEL *ret_pixval)
{
    GFX_PIXEL pixval;
    GFX_RGB rgb;
    int32 r, g, b, y;
    int8 cb, cr;
    BOOLEAN is16bit=FALSE;
    BOOLEAN is32bit=FALSE;

    switch (gfx_get_cm_name(coding_method)) {
        case GFX_CM_1BIT:
        case GFX_CM_2BIT:
        case GFX_CM_3BIT:
        case GFX_CM_4BIT:
        case GFX_CM_5BIT:
        case GFX_CM_6BIT:
        case GFX_CM_7BIT:
        case GFX_CM_8BIT:
        case GFX_CM_1A7_8BIT:
            *ret_pixval = color_index;
            return SUCCESS;
        case GFX_CM_RGB555:
        case GFX_CM_A1_RGB555:
            rgb = demo_colors[color_index];
            pixval = ((rgb >> 9) & 0x7c00) + ((rgb >> 6) & 0x03e0) +
                ((rgb>>3) & 0x001f);
			is16bit = TRUE;
            break;
        case GFX_CM_RGB888:
            pixval = demo_colors[color_index];
            is32bit = TRUE;
            break;
        case GFX_CM_YCBCR422:
        case GFX_CM_YCRCB422:
        case GFX_CM_YCBCR420:
        case GFX_CM_YCRCB420:
            rgb = demo_colors[color_index];
            r = (rgb >> 16) & 0xff;
            g = (rgb >> 8) & 0xff;
            b = rgb & 0xff;
            y  = (r *  77 / 256) + (g * 150 / 256) + (b *  29 / 256);
            cb = (r * -43 / 256) - (g *  85 / 256) + (b * 128 / 256);
            cr = (r * 128 / 256) - (g * 107 / 256) - (b *  21 / 256);
            if (y  <    0)  y =    0;
            if (cb < -128) cb = -128;
            if (cr < -128) cr = -128;
            if (y  >  255)  y =  255;
            if (cb >  127) cb =  127;
            if (cr >  127) cr =  127;
            if ((coding_method == GFX_CM_YCBCR422) || (coding_method ==
                GFX_CM_YCBCR422)) {
                pixval = ((y & 0xff) << 16) | ((cb & 0xff) << 8) | (cr & 0xff);
            } else {
                pixval = ((y & 0xff) << 16) | ((cr & 0xff) << 8) | (cb & 0xff);
            }
            break;

        case GFX_CM_RGB565:
            rgb = demo_colors[color_index];
            pixval = 
              ((rgb & 0x00f80000) >> (8-5) >> 16 << 11) | 
              ((rgb & 0x0000fc00) >> (8-6) >>  8 <<  5) |
              ((rgb & 0x000000f8) >> (8-5) >>  0 <<  0);
			is16bit=1;
            break;
        case GFX_CM_RGB655:
            rgb = demo_colors[color_index];
            pixval = 
              ((rgb & 0x00fc0000) >> (8-6) >> 16 << 11) | 
              ((rgb & 0x0000f800) >> (8-5) >>  8 <<  5) |
              ((rgb & 0x000000f8) >> (8-5) >>  0 <<  0);
			is16bit=1;
            break;
        case GFX_CM_RGB556:
            rgb = demo_colors[color_index];
            pixval = 
              ((rgb & 0x00f80000) >> (8-5) >> 16 << 11) | 
              ((rgb & 0x0000f800) >> (8-5) >>  8 <<  5) |
              ((rgb & 0x000000fc) >> (8-6) >>  0 <<  0);
			is16bit=1;
            break;

        case GFX_CM_CDI_RL3:
        case GFX_CM_CDI_RL7:
        case GFX_CM_CDI_DYUV:
        default:
          return demo_error(EOS_MAUI_BADCODEMETH, "Can't set color for coding method %x\n", 
                            gfx_get_cm_name(coding_method));
    }

    /* determine if "true color" pixel value must be swapped for hw */
    if (is16bit && gfx_get_cm_tc_swap(coding_method))
      *ret_pixval = swap16(pixval);
    else if (is32bit && gfx_get_cm_tc_swap(coding_method))
      *ret_pixval = swap32(pixval);
    else
      *ret_pixval = pixval;
    return SUCCESS;
}


/*******************************************************************************
* NAME:     demo_random_rect()
*
* USAGE:    Compute a random rectangle within the specified rectangle. Make
*           sure the width and height match or exceed the specified minimums.
*/
void demo_random_rect(GFX_RECT *ret_rect, GFX_POS x, GFX_POS y, GFX_DIMEN
    min_width, GFX_DIMEN min_height, GFX_DIMEN width, GFX_DIMEN height)
{
	GFX_POS	x1 = demo_random(0, width - min_width);
	GFX_POS	y1 = demo_random(0, height - min_height);
	GFX_POS	x2 = demo_random(x1 + min_width - 1, width - 1);
	GFX_POS	y2 = demo_random(y1 + min_height - 1, height - 1);

	ret_rect->x = x1;
	ret_rect->y = y1;
	ret_rect->width = x2 - x1 + 1;
	ret_rect->height = y2 - y1 + 1;
}

/*******************************************************************************
* NAME:     demo_random_point()
*
* USAGE:    Compute a random point within the given rectangle.
*/
void demo_random_point(GFX_POINT *ret_point, GFX_POS x, GFX_POS y, GFX_DIMEN
    width, GFX_DIMEN height)
{
    ret_point->x = demo_random(0, width - 1);
    ret_point->y = demo_random(0, height - 1);

    return;
}



static u_int32 swap32(u_int32 val)
{
	return  (val >> 24) | ((val>>8)&0xff00) |
			((val&0xff00)<<8) | (val<<24);
}

static u_int32 swap16(u_int32 val)
{ 
	return ((val>>8)&0x00ff) | ((val & 0x00ff) << 8);
}
