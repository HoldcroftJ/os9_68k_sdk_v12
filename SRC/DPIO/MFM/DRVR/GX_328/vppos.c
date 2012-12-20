/*******************************************************************************
*
* DESCRIPTION :
*
*	Set the position of the specified viewport.
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

error_code fe_set_vppos(GFX_VPORT *vport, GFX_POS x, GFX_POS y)
{
  /*    GFX_DEV *gfxdev = vport->gfxdev;*/
	GFX_VPORT_SHARED *vport_shared = vport->shared;

    /* If the viewport is not active then just update the structure */
    if (vport_shared->active != TRUE) {
        vport_shared->sx = x;
        vport_shared->ex = x + vport_shared->width - 1;
        vport_shared->sy = y;
        vport_shared->ey = y + vport_shared->height - 1;
        return SUCCESS;
    }

    /* Deactivate the viewport, move it, then reactivate it. */
    deactivate_vport(vport);
    vport_shared->sx = x;
    vport_shared->ex = x + vport_shared->width - 1;
    vport_shared->sy = y;
    vport_shared->ey = y + vport_shared->height - 1;
    activate_vport(vport);

	return SUCCESS;
}
