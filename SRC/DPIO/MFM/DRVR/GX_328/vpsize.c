/*******************************************************************************
*
* DESCRIPTION :
*
*	Set the size of the specified viewport.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  12/23/94  TJE  First version
*/
#include <defs.h>

error_code fe_set_vpsize(GFX_VPORT *vport, GFX_DIMEN width, GFX_DIMEN height)
{
    GFX_DEV *gfxdev = vport->gfxdev;
	GFX_DEV_RES *dvres = gfxdev->shared->devres;
	GFX_VPORT_SHARED *vport_shared = vport->shared;

	/* check to see if vport is the size of the screen */
	if ((width != dvres->disp_width) || (height == dvres->disp_height))
	  return EOS_MAUI_BADDIMEN;

    /* If there is a drawmap mapped to the viewport make sure it is not too */
    /* small for the new size. */
    if (vport_shared->pixmem != NULL) {
        if ((width > (vport_shared->dmap_width - vport_shared->dmapx))
                        || (height > (vport_shared->dmap_height - vport_shared->dmapy))) {
            return EOS_MAUI_DMAPTOOSMALL;
		}
    }

    /* If the viewport is not active then just update the structure */
    if (vport_shared->active != TRUE) {
        vport_shared->width = width;
        vport_shared->height = height;
        vport_shared->ex = vport_shared->sx + width - 1;
        vport_shared->ey = vport_shared->sy + height - 1;
        return SUCCESS;
    }

    /* Deactivate the viewport, re-size it, then reactivate it. */
    deactivate_vport(vport);
    vport_shared->width = width;
    vport_shared->height = height;
    vport_shared->ex = vport_shared->sx + width - 1;
    vport_shared->ey = vport_shared->sy + height - 1;
    activate_vport(vport);

    return SUCCESS;
}
