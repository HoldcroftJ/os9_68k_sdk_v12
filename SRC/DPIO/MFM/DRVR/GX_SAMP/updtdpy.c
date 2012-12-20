/*******************************************************************************
*
* DESCRIPTION :
*
*	Update the display with queued up changes. Synchronize with vertical retrace
*	if specified.
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
#include <events.h>

error_code fe_update_dpy(GFX_DEV *gfxdev, BOOLEAN sync)
{
	*?*?* INCLUDE CODE HERE TO UPDATE THE DISPLAY WITH QUEUED UP CHANGES.
	*?*?*
	*?*?* IF "sync" IS "TRUE" THEN YOU NEED TO SYNCHRONIZE THESE CHANGES WITH
	*?*?* THE VERTICAL RETRACE. THE FOLLOWING CODE IS AN EXAMPLE OF HOW TO DO
	*?*?* THIS.

	error_code ec;
	int32 event_val = 0;
	u_int32 signal;

	/* Synchronize with vertical retrace. Note that we use the entry in */
	/* the gfxdev structure. This is the ID seen from the applications */
	/* side and must be used since this is a fast entry point. */
	if (sync == TRUE) {

	  do {
#if defined(_OS9000)
		if ((ec = _os_ev_wait(gfxdev->usr_retrace_evid, &event_val,
			&signal, 1, 1)) != SUCCESS) {
#elif defined(_OSK)
		if ((ec = _os9_ev_wait(gfxdev->usr_retrace_evid, &event_val,
			1, 1)) != SUCCESS) {
#endif
			return ec;
		}
	  } while (event_val != 1);
	}

	return SUCCESS;
}
