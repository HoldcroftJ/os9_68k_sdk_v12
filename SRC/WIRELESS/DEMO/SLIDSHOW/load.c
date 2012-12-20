
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
* VERSION:		@(#)load.c	1.3 8/3/95
**************************************************************************
*  Edition History:
*   #   Date       Comments                                           By
*  --- --------    ----------------------------------------------- -------
*   1  95/08/02    Modified load_image() to load a SUNRAST image     rws
*                  in flash into a called for DMAP's pixel memory
*                  (before it would create a DMAP and load it in,
*                  but now main() is appropriately handling that)
*   2  96/03/19    Program modified to handle only two bit images    pbt
*   3  96/03/25    Modified so that dmap not needed as input         rws
*                  parameter
*   4  96/03/28    Modified for PPC821ADS such that image is loaded  rws
*                  at 16 byte boundary in DMAP's pixmem
*   5  96/04/22    Since mauidrvr was modified, have now removed     rws
*                  edition #4 changes
*	6	96/05/01	 Modified to handle drawmaps that are larger       pbt
*                  than the images being loaded into pixmem
**************************************************************************
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
#include <MAUI/maui_gfx.h>


extern GFX_DMAP *dmap;	/* in demo.c */
extern GFX_DEV_ID gfxdev;

/*******************************************************************************
* NAME:		load_image()
*
* USAGE:	Load an image from a SUNRAS module in memory into a drawmap
*           The coding method of this drawmap must be GFX_CM_2BIT.
*/
error_code load_image
(
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

	/* verify parameters */
	/* !!!! modify the next revision of load.c to handle all combinations
	 *      of drawmap dimensions and image dimensions. Also use the
	 *		BLT COPY function to handle this.
	 */
	if (raster->ras_magic != RAS_MAGIC ||  /* ras_magic (sync code) */
		raster->ras_length == 0 ||
		raster->ras_depth != 2 ||
		raster->ras_width > dmap->width ||
		raster->ras_height> dmap->height)
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

		bytes_per_row = raster->ras_width * raster->ras_depth / 8;

		/* Loop over all image rows */
		for (loop_y = 0; loop_y < raster->ras_height ; loop_y++)     
		{
			/* Copy the actual bitmap data */
		   for (loop_x = 0; loop_x < bytes_per_row; loop_x++)  
		      *src_pixmem++ = *ras_pixmem++;
		   
		   /*
		    * Pad to a multiple of GFX_LINE_PAD bytes.
		    */
		   for (; loop_x % GFX_LINE_PAD != 0; loop_x++)
		      *src_pixmem++ = 0;

           /* loop some more to pad upto the line_size */
           for (; loop_x < dmap->line_size; loop_x++)
           	*src_pixmem++ = 0;
		}

   		gfx_update_display(gfxdev, FALSE);

        /* Additional looping to pad remaining dmap rows
           for images that are smaller than the dmap size   */
        for(loop_y = raster->ras_height; loop_y < dmap->height; loop_y++)
        {
            /*
             * Pad to line_size in bytes of the draw map.
             */
              for (loop_x = 0; loop_x < dmap->line_size; loop_x++)
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
