/*******************************************************************************
*
* DESCRIPTION :
*
*	Set the drawmap area to be displayed in a viewport.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  12/22/94  TJE  First version
*/
#include <defs.h>

error_code fe_set_vpdmap(GFX_VPORT *vport, const GFX_DMAP *dmap, GFX_POS x,
	GFX_POS y)
{
  /*    error_code ec;*/

    BOOLEAN reactivate=FALSE;
	/*   GFX_DIMEN max_vpwidth;*/
	GFX_VPORT_SHARED *vport_shared = vport->shared;

    /* If the drawmap is NULL then handle it and return */
    if (dmap == NULL) {
    	return SUCCESS;
    }

    /* Verify parameters */
    if ((dmap->coding_method != GFX_CM_1BIT)
                                && (dmap->coding_method != GFX_CM_2BIT)) {
        return EOS_MAUI_BADCODEMETH;
    							}
    if ((x >= dmap->width) || (y >= dmap->height)) {
        return EOS_MAUI_BADPOS;
    }
    if ((vport_shared->width > (dmap->width - x))
                              || (vport_shared->height > (dmap->height - y))) {
        return EOS_MAUI_DMAPTOOSMALL;
    							}
	/*    if(dmap->palette != NULL){
      return EOS_MAUI_BADCOLORTYPE;
    }*/

    /* If the viewport is active then deactivate it first. After updating */
    /* it, we will re-activate it. */
    if (vport_shared->active == TRUE) {
        deactivate_vport(vport);
        reactivate = TRUE;
    }

    /* Copy current values from the drawmap object to the viewport */
    vport_shared->dmap  = dmap;
    vport_shared->dmapx = x;
    vport_shared->dmapy = y;
    vport_shared->coding_method = dmap->coding_method;
    vport_shared->dmap_width = dmap->width;
    vport_shared->dmap_height = dmap->height;
    vport_shared->dmap_line_size = dmap->line_size;
    vport_shared->pixmem = dmap->pixmem;

    /* Set multipliers used to convert viewport coordinates to display */
    /* coordinates. */
    switch (vport_shared->coding_method) {
        case GFX_CM_2BIT:
        case GFX_CM_1BIT:
            vport_shared->dm2dp_xmul = 1;
            break;
    }
    vport_shared->dm2dp_ymul = 1;

	/* set palette if in 2 bit mode */
	if (vport_shared->coding_method == GFX_CM_2BIT)
      fe_set_vpcolors(vport,dmap->palette->start_entry, dmap->palette->num_colors, dmap->palette->color_type, dmap->palette->colors.rgb);


    /* Re-activate the viewport if it was deactivated earlier */
    if (reactivate == TRUE) {
        activate_vport(vport);
    }

    return SUCCESS;
}



/* Scale = 0x10000           .299          .587          .114            */
#define RGBTOGRAY(r, g, b) ((19595 * (r) + 38470 * (g) + 7471 * (b)) >> 16)
#define RED(rgb)   (((rgb) >> 16) & 0xff)
#define GREEN(rgb) (((rgb) >> 8) & 0xff)
#define BLUE(rgb)  ((rgb) & 0xff)

/*
** for a given r,g,b value, return the value from the following table that
** matches the closest cooresponding gray color.
** the table is as follows:
** code- gray value 
**   0 -   0
**   1 -  63
**   2 -  73
**   3 - 127
**   4 - 175
**   5 - 191
**   6 - 255
**   7 - 255 
** Post: returns a 3 bit value that represents the gray color density
*/
static u_int8 rgb_to_density(u_int32 rgb)
{
  u_int8 r,g,b;
  u_int16 gray;
  u_int8 retval;

  r = RED(rgb);
  g = GREEN(rgb);
  b = BLUE(rgb);
  gray = RGBTOGRAY(r,g,b);
  
  if (gray <= 32) {
	retval = 0;
  } else if (gray <= 67) {
	retval = 1;
  } else if (gray <= 100) {
	retval = 2;
  } else if (gray <= 151) {
	retval = 3;
  } else if (gray <= 184) {
	retval = 4;
  } else if (gray <= 220) {
	retval = 5;
  } else {
	retval = 7;
  }

  return retval;
}


/*******************************************************************************
* NAME:     fe_set_vpcolors()
*
* USAGE:    Set the colors in a viewport.
* This hardware only allows grayscale selection in 2 bit mode.
*/
error_code fe_set_vpcolors(GFX_VPORT *vport, u_int16 start_entry, u_int16
    num_colors, GFX_COLOR_TYPE color_type, void *colors)
{
  GFX_VPORT_SHARED *vport_shared = vport->shared;
  GFX_DEV_SHARED *gfxdev_shared = vport_shared->gfxdev_shared;
  GDV_DVATCH *dvatch = gfxdev_shared->gdv_attachment;
  Mfm_lu_stat lustat = dvatch->lustat;
  M68328_regs regs = (M68328_regs)(lustat->lu_port);

  /* The code above probably seems a bit convoluted to get the reg. base
  ** pointer.  Going through the vport_shared->gdv_attachment isn't always
  ** setup when this code is called.  Going through the gfxdev_shared is
  ** more reliable. */

  if (colors && (num_colors > 0)) { 
	  if (vport_shared->coding_method == GFX_CM_2BIT) {
		u_int16 glevel, tmp;
		GFX_RGB *rgb = (GFX_RGB*)colors;
		rgb = &rgb[start_entry];

		/* convert to rgb.  Order of 4 pixels in each word is 3,2,0,1 */
		tmp = rgb_to_density(*rgb++);
		glevel = tmp << 8;           
		if (num_colors >= 2)
		  tmp = rgb_to_density(*rgb++);
		glevel |= tmp << 12;
		if (num_colors >= 3)
		  tmp = rgb_to_density(*rgb++);
		glevel |= tmp << 4;
		if (num_colors >= 4)
		  tmp = rgb_to_density(*rgb++);
		glevel |= tmp; 

		/* update gray palette mapping register */
		regs->lcdc_regs.lgpmr = glevel;
	  }
  }

  return SUCCESS;
}
