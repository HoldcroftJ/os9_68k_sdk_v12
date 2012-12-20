/*******************************************************************************
*
* DESCRIPTION:	Functions that allow you to use IFF images in MAUI.
*
* AUTHOR:		Todd Earles
*
* CREATED ON:	06/02/95
*
* COPYRIGHT:	Copyright 1994 Microware Systems Corporation. All Rights
*				Reserved. Use and/or distribution subject to terms and
*				conditions of applicable Microware license agreement(s).
*
* VERSION:		@(#)load.h	1.3 8/3/95
*
*/

#ifndef _LOAD_H_
#define _LOAD_H_

#include <MAUI/maui_mem.h>
#include <MAUI/maui_gfx.h>

extern error_code load_image(GFX_DMAP *ret_dmap, 
			     char *modname, BOOLEAN swap_bytes);

#endif
