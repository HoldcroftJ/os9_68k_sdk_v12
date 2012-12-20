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
	error_code ec;

	*?*?* YOUR DRIVER MUST SUPPORT THIS FUNCTION. THE TEMPLATE CHECKS FOR
	*?*?* EOS_MAUI_BADPOS AND EOS_MAUI_INUSE SO YOU DO NOT NEED TO CHECK
	*?*?* FOR THEM HERE. THE TEMPLATE ALSO HANDLES CLEARING THE VPORT MEMBERS
	*?*?* IF THE DRAWMAP IS NULL.

	*?*?* CALL THE FUNCTION THAT SETS THE COLORS FOR THE VIEWPORT. FOLLOWING
	*?*?* IS AN EXAMPLE OF HOW IT SHOULD BE CALLED.

	/* Set the CLUT using the palette */
	if (dmap->palette == NULL) {
		return EOS_MAUI_NOPALETTE;
	}
	if ((ec = fe_set_vpcolors(vport, dmap->palette->start_entry,
		dmap->palette->num_colors, dmap->palette->color_type,
		(void *)(dmap->palette->colors.rgb))) != SUCCESS) {
		return ec;
	}

	return SUCCESS;
}

/*******************************************************************************
* NAME:		fe_set_vpcolors()
*
* USAGE:	Set the colors in a viewport.
*/
error_code fe_set_vpcolors(GFX_VPORT *vport, u_int16 start_entry, u_int16
	num_colors, GFX_COLOR_TYPE color_type, void *colors)
{
	error_code ec;
	u_int16 i, clut_num;
	GFX_RGB rgb_color;
	GDV_VPATCH *vpatch = vport->shared->gdv_attachment;

	*?*?* THE TEMPLATE PROVIDES FUNCTIONS TO CONVERT SOURCE COLORS INTO THE
	*?*?* TYPE REQUIRED BY THE DRIVER. FOLLOWING IS AN EXAMPLE OF HOW TO USE
	*?*?* THESE FUNCTIONS TO SETUP THE CLUT TABLE USING THE VALUES IN THE
	*?*?* COLOR ARRAY. THIS EXAMPLE ASSUMES AN RGB HARDWARE CLUT, BUT SIMILAR
	*?*?* FUNCTIONS ARE PROVIDED FOR EACH COLOR TYPE. THE VALUES FOR THE
	*?*?* HARDWARE CLUT ARE COMPUTED BUT THE HARDWARE IS NOT UPDATED YET.
	*?*?* THE MOST LIKELY PLACE TO STORE THE RESULTS IS IN THE VIEWPORT
	*?*?* ATTACHMENT.

	/* Save the CLUT values in the viewport attachment */
	if ((ec = gdv_cvt2_rgb_setup(color_type, colors)) != SUCCESS) {
		return ec;
	}
	clut_num = start_entry;
	for (i = 0; i < num_colors; i++) {
		rgb_color = gdv_cvt2_rgb(i);
		vpatch->clut[clut_num++] = rgb_color;
	}

	return SUCCESS;
}
