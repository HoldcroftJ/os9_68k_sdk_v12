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

	*?*?* YOUR DRIVER MUST SUPPORT THIS FUNCTION. THE TEMPLATE CHECKS FOR
	*?*?* EOS_MAUI_NODMAP AND EOS_MAUI_BADDIMEN SO YOU DO NOT NEED TO CHECK
	*?*?* FOR THEM HERE.

	return SUCCESS;
}
