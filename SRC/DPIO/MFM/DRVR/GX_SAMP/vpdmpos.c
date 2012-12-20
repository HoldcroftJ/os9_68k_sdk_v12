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

	*?*?* YOUR DRIVER MUST SUPPORT THIS FUNCTION. THE TEMPLATE CHECKS FOR
	*?*?* EOS_MAUI_NODMAP AND EOS_MAUI_BADPOS SO YOU DO NOT NEED TO CHECK
	*?*?* FOR THEM HERE.

	return SUCCESS;
}
