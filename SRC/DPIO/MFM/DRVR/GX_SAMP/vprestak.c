/*******************************************************************************
*
* DESCRIPTION :
*
*	Re-stack a viewport within the stack of viewports.
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

error_code fe_restack_vp(GFX_VPORT *vport, GFX_VPORT_PLACEMENT placement,
	GFX_VPORT *ref_vport)
{

	*?*?* YOUR DRIVER MUST SUPPORT THIS FUNCTION. INCLUDE CODE FOR IT HERE.

	*?*?* THE TEMPLATE PROVIDES FUNCTIONS FOR UNLINKING THE VIEWPORT FROM ITS
	*?*?* CURRENT POSITION AND INSERTING IT IN A NEW POSITION. FOLLOWING IS AN
	*?*?* EXAMPLE OF HOW TO CALL THESE FUNCTIONS.
	
	/* Unlink the viewport from its current location in the stack */
	gdv_unlink_vport(vport);
	
	/* Insert the viewport into its new location in the stack */
	gdv_insert_vport(vport, placement, ref_vport);

	return SUCCESS;
}
