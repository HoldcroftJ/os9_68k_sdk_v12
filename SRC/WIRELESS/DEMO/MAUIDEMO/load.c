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

#include <stdlib.h>
#include <module.h>
#include <stdio.h>
#include <errno.h>
#include <modes.h>
#include <string.h>
#include <MAUI/maui_mem.h>
#include <MAUI/maui_gfx.h>
#include <sunrast.h>

/*******************************************************************************
* NAME:		load_image()
*
* USAGE:	Load an image from a SUNRAS module in memory into a drawmap
*           The coding method of this drawmap must be GFX_CM_2BIT.
*/
error_code load_image
(
	GFX_DMAP *dmap, 		/* DMAP to put inmage in */
	char *modname, 
	BOOLEAN swap_bytes
)
{
	Rasterfile raster;
	mh_com *mod_head;
	error_code ec = SUCCESS;
	char *ras_pixmem;
	char *src_pixmem;

	src_pixmem = (char *)dmap->pixmem;

	/* Link to the module */
	{
		u_int16 type_lang = 0, attr_rev;
		if ((ec = _os_link (&modname, &mod_head, (void**)&raster, 
					&type_lang, &attr_rev)) != SUCCESS) 
		{
			return ec;
		}
	}

	/* 1 bit image => ras_depth == 1 */

	/* verify parameters */
	if (raster->ras_magic != RAS_MAGIC ||  /* ras_magic (sync code) */
		raster->ras_length == 0 ||
		raster->ras_depth != 1 ||
		raster->ras_width != dmap->width ||
		raster->ras_height != dmap->height)
	{
		_os_unlink(mod_head);
		return EOS_PARAM;
	}

	/*
	 * get pointer to pixel data 
	 */
	ras_pixmem = (char*)(raster + 1);
	if (raster->ras_maplength != 0)	/* ras_maplength (# colormap bytes) */
	   ras_pixmem += raster->ras_maplength;

	/* copy pixel data into drawmap */
	{
        int loop_x, loop_y, bytes_per_row;

		bytes_per_row = dmap->width * raster->ras_depth / 8;

		/* Loop over all rows */
		for (loop_y = 0; loop_y < dmap->height; loop_y++)     
		{
			/* Copy the actual bitmap data */
		   for (loop_x = 0; loop_x < bytes_per_row; loop_x++)  
		      *src_pixmem++ = *ras_pixmem++;
		   
		   /*
		    * Pad to a multiple of GFX_LINE_PAD bytes.
		    */
		   for (; loop_x % GFX_LINE_PAD != 0; loop_x++)
		      *src_pixmem++ = 0;
		}

		/*
		 * NOTE: because of the different endianess of the MC68341
		 * and the LCD Controller, consecutive bytes of the image
		 * must be swapped if going between system RAM and Video RAM
		 */
        if (swap_bytes)
		{
		   int loop_var;
		   char tmp;
		   for (loop_var = 0; loop_var < dmap->pixmem_size; loop_var+=2) {
		      tmp = src_pixmem[0];
		      src_pixmem[0] = src_pixmem[1];
		      src_pixmem[1] = tmp;
		      src_pixmem += 2;
		   }
		}
	}

	/* Set return value and return to caller */
	_os_unlink(mod_head);
	return ec;
}
