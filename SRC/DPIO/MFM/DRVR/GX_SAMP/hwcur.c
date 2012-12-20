/*******************************************************************************
*
* DESCRIPTION :
*
*	H/W cursor support functions.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  11/16/98  AND  First version
*/
#include <defs.h>

/*******************************************************************************
* NAME:		dr_cursor_create()
*
* USAGE:	Create an instance of H/W cursor with the characteristics, 
*                       specified by GFX_CURSOR_SPEC parameter. The new cursor 
*                       is associated with the opened path to the graphics device 
*                       and should become "active" (visible) only if it's logical 
*                       graphics device is visible and the cursor itself has been set
*                       (by calling fe_cursor_set() entry point).
*			
*/
error_code dr_cursor_create(GFX_DEV *gfxdev, GFX_CURSOR *new_cursor, 
	const GFX_CURSOR_SPEC *cursor)
{
	*?*?* CREATE A NEW H/W CURSOR BY STORING IT'S SIZE, PATTERN AND PALETTE.
        return SUCCESS;
}

/*******************************************************************************
* NAME:		dr_cursor_destroy()
*
* USAGE:	Destroy an instance of H/W cursor. The cursor should not be active 
*                       at the time this function is called. It is an application 
*                       responsibility to de-activate the H/W cursor before destroying. 
*
*/
error_code dr_cursor_destroy(GFX_CURSOR *cursor)
{
	*?*?* DESTROY AN EXISTING CURSOR PATTERN.
        return SUCCESS;
}

/*******************************************************************************
* NAME:		fe_cursor_set()
*
* USAGE:	Set "cursor" to be an "active" H/W cursor. This function directly sets 
*                       the hardware, therefore it should immediately affect the shape 
*                       and the position of the cursor on the screen. 
*
*/
void fe_cursor_set(GFX_DEV *gfxdev, GFX_CURSOR *cursor)
{
	*?*?* SET AN INSTANCE OF H/W CURSOR TO BE ACTIVE (VISIBLE).
        return SUCCESS;
}

/*******************************************************************************
* NAME:		fe_cursor_set_pos()
*
* USAGE:	Set a position of the current "active" cursor. This function directly 
*                       sets the hardware, therefore it should immediately affect the 
*                       position of the cursor on the screen. 
*
*/
void fe_cursor_set_pos(GFX_DEV *gfxdev, GFX_POS x, GFX_POS y)
{
	*?*?* SET A POSITION OF THE ACTIVE H/W CURSOR.
        return SUCCESS;
}

