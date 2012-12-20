/*******************************************************************************
*
* DESCRIPTION :
*
*	Set the state (active or not) of the specified viewport.
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

error_code fe_set_vpstate(GFX_VPORT *vport, BOOLEAN active)
{
    GFX_VPORT *vp;

    /* Activate or deactivate the viewport */
    if (active == TRUE) 
	{
	  if(vport->shared->active == FALSE)
	  {
		for (vp = vport->gfxdev->shared->vport_head; vp != NULL; 
			 vp = vp->shared->next) 
		{
		  if (vp->shared->active == TRUE) 
			return EOS_MAUI_TOOCOMPLEX; /*There can be just one active vport */
		}
		/* Activate the viewport */
		activate_vport(vport);
	  }
	} else {
	  
	  /* Deactivate the viewport */
	  deactivate_vport(vport);
	}
	return SUCCESS;
}

void activate_vport(GFX_VPORT *act_vport)
{
    act_vport->shared->active = TRUE;
}

void deactivate_vport(GFX_VPORT *act_vport)
{
    act_vport->shared->active = FALSE;
}
