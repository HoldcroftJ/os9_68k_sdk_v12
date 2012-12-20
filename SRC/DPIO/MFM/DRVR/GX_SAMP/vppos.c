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

	*?*?* YOUR DRIVER MUST SUPPORT THIS FUNCTION. THE TEMPLATE CHECKS FOR
	*?*?* EOS_MAUI_BADPOS AND EOS_MAUI_BADDIMEN SO YOU DO NOT NEED TO CHECK
	*?*?* FOR THEM HERE.

	return SUCCESS;
}
