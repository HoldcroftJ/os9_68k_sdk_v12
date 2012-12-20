/*******************************************************************************
*
* DESCRIPTION :
*
*	Set the position of the drawmap in the viewport.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  08/10/95  TJE  First version
*/
#include <defs.h>

error_code fe_set_vpdmpos(GFX_VPORT *vport, GFX_POS x, GFX_POS y)
{
  /*    BOOLEAN reactivate=FALSE;*/
	GFX_VPORT_SHARED *vport_shared = vport->shared;
    const GFX_DMAP *dmap = vport_shared->dmap;

    /* Verify parameters */
    if (vport_shared->dmap == NULL) {
        return EOS_MAUI_NODMAP;
    }
    if ((x >= dmap->width) || (y >= dmap->height)) {
        return EOS_MAUI_BADPOS;
    }
    if ((vport_shared->width > (dmap->width - x))
                                || (vport_shared->height > (dmap->height - y))) {
        return EOS_MAUI_DMAPTOOSMALL;
    							}
	/* set the position */
    vport_shared->dmapx = x;
    vport_shared->dmapy = y;

    return SUCCESS;
}
