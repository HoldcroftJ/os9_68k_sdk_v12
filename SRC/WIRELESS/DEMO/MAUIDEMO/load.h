/*****************************************************************************
** Demo program to show how to output text to a drawmap in MAUI and         **
** wait for user input.                                                     **
******************************************************************************
** Copyright 1996 by Microware Systems Corporation                          **
** Reproduced Under License                                                 **
**                                                                          **
** This source code is the proprietary confidential property of             **
** Microware Systems Corporation, and is provided to licensee               **
** solely for documentation and educational purposes. Reproduction,         **
** publication, or distribution in any form to any party other than         **
** the licensee is strictly prohibited.                                     **
******************************************************************************
** Edition History:                                                         **
** #   Date     Comments                                          By        **
** --- -------- ------------------------------------------------- --------- **
**   1 03/15/96 creation                                          bat       **
*****************************************************************************/

/*
**	DESCRIPTION:	Functions that allow you to use IFF images in MAUI.
*/

#ifndef _LOAD_H_
#define _LOAD_H_

#include <MAUI/maui_mem.h>
#include <MAUI/maui_gfx.h>

extern error_code load_image(GFX_DMAP *ret_dmap, 
			     char *modname, BOOLEAN swap_bytes);

#endif
