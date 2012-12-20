/*******************************************************************************
*
* DESCRIPTION :
*
*	H/W accelerated Bit-BLT functions.
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
#include <gdv_priv.h>

/* Local dispatch function */
static error_code _set_dispatch(BLT_CONTEXT *bc);

/*******************************************************************************
* NAME:         _gdv_hwblt_drwmix()
*
* USAGE:        Set mixing mode for draw operations
*/
error_code _gdv_hwblt_drwmix(BLT_CONTEXT *bc)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* STORE DRAW MIXING MODE INTO "BCATCH" STRUCTURE.

      /* Reset pointers to H/W accelerated operations */
      return _set_dispatch(bc);
}

/*******************************************************************************
* NAME:         _gdv_hwblt_cpymix()
*
* USAGE:        Set mixing mode for copy operations
*/
error_code _gdv_hwblt_cpymix(BLT_CONTEXT *bc)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* STORE COPY MIXING MODE INTO "BCATCH" STRUCTURE.

      /* Reset pointers to H/W accelerated operations */
      return _set_dispatch(bc);
}

/*******************************************************************************
* NAME:         _gdv_hwblt_expmix()
*
* USAGE:        Set mixing mode for expand operations
*/
error_code _gdv_hwblt_expmix(BLT_CONTEXT *bc)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* STORE EXPAND MIXING MODE INTO "BCATCH" STRUCTURE.

      /* Reset pointers to H/W accelerated operations */
      return _set_dispatch(bc);
}

/*******************************************************************************
* NAME:         _gdv_hwblt_pix()
*
* USAGE:        Set drawing pixel value
*/
error_code _gdv_hwblt_pix(BLT_CONTEXT *bc)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* STORE DRAWING PIXEL VALUE INTO "BCATCH" STRUCTURE.

      /* Reset pointers to H/W accelerated operations */
      return _set_dispatch(bc);
}

/*******************************************************************************
* NAME:         _gdv_hwblt_src()
*
* USAGE:        Set source drawmap
*/
error_code _gdv_hwblt_src(BLT_CONTEXT *bc)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* STORE SOURCE DRAWMAP SETTINGS INTO "BCATCH" STRUCTURE.

      /* Reset pointers to H/W accelerated operations */
      return _set_dispatch(bc);
}

/*******************************************************************************
* NAME:         _gdv_hwblt_exptbl()
*
* USAGE:        Set pixel expansion table
*/
error_code _gdv_hwblt_exptbl(BLT_CONTEXT *bc)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* STORE PIXEL EXPANSION TABLE INTO "BCATCH" STRUCTURE.

      /* Reset pointers to H/W accelerated operations */
      return _set_dispatch(bc);
}

/*******************************************************************************
* NAME:         _gdv_hwblt_trans()
*
* USAGE:        Set transparent pixel value
*/
error_code _gdv_hwblt_trans(BLT_CONTEXT *bc)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* STORE TRANSPARENT PIXEL VALUE INTO "BCATCH" STRUCTURE.

      /* Reset pointers to H/W accelerated operations */
      return _set_dispatch(bc);
}

/*******************************************************************************
* NAME:		_gdv_hwblt_mask()
*
* USAGE:	Set mask drawmap
*/
error_code _gdv_hwblt_mask(BLT_CONTEXT *bc)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* STORE MASK DRAWMAP SETTINGS INTO "BCATCH" STRUCTURE.

      /* Reset pointers to H/W accelerated operations */
      return _set_dispatch(bc);
}

/*******************************************************************************
* NAME:		_gdv_hwblt_ofs()
*
* USAGE:	Set offset pixel value
*/
error_code _gdv_hwblt_ofs(BLT_CONTEXT *bc)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* STORE OFFSET PIXEL VALUE INTO "BCATCH" STRUCTURE.

      /* Reset pointers to H/W accelerated operations */
      return _set_dispatch(bc);
}

/*******************************************************************************
* NAME:         _gdv_hwblt_dst()
*
* USAGE:        Set destination drawmap
*/
error_code _gdv_hwblt_dst(BLT_CONTEXT *bc)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* STORE DESTINATION DRAWMAP SETTINGS INTO "BCATCH" STRUCTURE.

      /* Set pointers to H/W accelerated operations */
      return _set_dispatch(bc);
}

/*******************************************************************************
* NAME:         _gdv_hwblt_drawblk
*
* USAGE:        Draw a block of pixels.
*/
error_code _gdv_hwblt_drawblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
        GFX_DIMEN width, GFX_DIMEN height)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* IMPLEMENT H/W ACCELERATED "DRAW_BLOCK" FUNCTION HERE. 
      *?*?* THIS FUNCTION IS CALLED DIRECTLY FROM BLT API "blt_draw_block".

      return SUCCESS;
}

/*******************************************************************************
* NAME:         _gdv_hwblt_drawhline
*
* USAGE:        Draw a horizontal line of pixels.
*/
error_code _gdv_hwblt_drawhline(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
        GFX_DIMEN width)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* IMPLEMENT H/W ACCELERATED "DRAW_HLINE" FUNCTION HERE. 
      *?*?* THIS FUNCTION IS CALLED DIRECTLY FROM BLT API "blt_draw_hline".

      return SUCCESS;
}

/*******************************************************************************
* NAME:         _gdv_hwblt_drawvline
*
* USAGE:        Draw a vertical line of pixels.
*/
error_code _gdv_hwblt_drawvline(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
        GFX_DIMEN height)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* IMPLEMENT H/W ACCELERATED "DRAW_VLINE" FUNCTION HERE. 
      *?*?* THIS FUNCTION IS CALLED DIRECTLY FROM BLT API "blt_draw_vline".

      return SUCCESS;
}

/*******************************************************************************
* NAME:         _gdv_hwblt_drawpixel
*
* USAGE:        Draw a pixel.
*/
error_code _gdv_hwblt_drawpixel(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* IMPLEMENT H/W ACCELERATED "DRAW_PIXEL" FUNCTION HERE. 
      *?*?* THIS FUNCTION IS CALLED DIRECTLY FROM BLT API "blt_draw_pixel".

      return SUCCESS;
}

/*******************************************************************************
* NAME:         _gdv_hwblt_copyblk
*
* USAGE:        Copy a block of pixels.
*/
error_code _gdv_hwblt_copyblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
        GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* IMPLEMENT H/W ACCELERATED "COPY_BLOCK" FUNCTION HERE. 
      *?*?* THIS FUNCTION IS CALLED DIRECTLY FROM BLT API "blt_copy_block".

      return SUCCESS;
}

/*******************************************************************************
* NAME:         _gdv_hwblt_copynblk
*
* USAGE:        Copy a next block of pixels.
*/
error_code _gdv_hwblt_copynblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx, 
	GFX_DIMEN height)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* IMPLEMENT H/W ACCELERATED "COPY_NEXT_BLOCK" FUNCTION HERE. 
      *?*?* THIS FUNCTION IS CALLED DIRECTLY FROM BLT API "blt_copy_next_block".

      return SUCCESS;
}

/*******************************************************************************
* NAME:         _gdv_hwblt_expdblk
*
* USAGE:        Expand a block of pixels.
*/
error_code _gdv_hwblt_expdblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
        GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* IMPLEMENT H/W ACCELERATED "EXPD_BLOCK" FUNCTION HERE. 
      *?*?* THIS FUNCTION IS CALLED DIRECTLY FROM BLT API "blt_expd_block".

      return SUCCESS;
}

/*******************************************************************************
* NAME:         _gdv_hwblt_expdnblk
*
* USAGE:        Expand a next block of pixels.
*/
error_code _gdv_hwblt_expdnblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx, 
        GFX_DIMEN height)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* IMPLEMENT H/W ACCELERATED "EXPD_NETX_BLOCK" FUNCTION HERE. 
      *?*?* THIS FUNCTION IS CALLED DIRECTLY FROM BLT API "blt_expd_next_block".

      return SUCCESS;
}

/*******************************************************************************
* NAME:         _gdv_hwblt_getpixel
*
* USAGE:        Get a pixel.
*/
error_code _gdv_hwblt_getpixel(GFX_PIXEL *ret_pixel, BLT_CONTEXT *bc, GFX_POS srcx, 
        GFX_POS srcy)
{
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?* IMPLEMENT H/W ACCELERATED "GET_PIXEL" FUNCTION HERE. 
      *?*?* THIS FUNCTION IS CALLED DIRECTLY FROM BLT API "blt_get_pixel".

      return SUCCESS;
}

/*******************************************************************************
* NAME:         _set_dispatch()
*
* USAGE:        Set pointers to H/W accelerated Bit-BLT operations.
*/
error_code _set_dispatch(BLT_CONTEXT *bc)
{	
      GDV_BCATCH *bcatch = bc->gdv_attachment;

      *?*?*  SET BIT-BLT CONTEXT FUNCTION POINTERS TO H/W ACCELERATED FUNCTIONS
      *?*?*  DEPENDING ON THE CONTEXT SETTINGS (EITHER SOURCE OR DESTINATION DRAWMAP 
      *?*?*  MEMORY IS ALLOCATED FROM VIDEO MEMORY SHADE AND HAS THE "DISPLAYABLE" SIZE 
      *?*?*  AND PIXEL FORMAT, OPERATIONS CAN BE SUPPORTED WITH THE MIXING MODE SET, ETC)
      if (...) {


            *!*!* CALL MACROS _hwblt_set_* TO SET CONTEXT POINTERS 
            *!*!* THEY WILL CHOOSE THE RIGHT FUNCTION POINTERS DEPENDING 
            *!*!* ON THE STATE OF "GDV_MEM_TOP_BIT" AND "GDV_FE_SYSTATE" FLAGS !!!
            *!*!* REFER TO "gdv_priv.h" FILE FOR THE MACROS' DEFINITION 


      *?*?*  IF BIT-BLT CONTEXT SETTINGS ARE APPROPRIATE TO SUPPORT "DRAW_BLOCK" AND 
      *?*?*  H/W ACCELERATION ENGINE CAN PERFORM THIS OPERATION FASTER THAN API (IOBLT),
      *?*?*  SET "DRAW_BLOCK" FIELD OF BIT-BLT CONTEXT STRUCTURE. 
	  if (...) {
             _hwblt_set_draw_block(bc, GDV_HWBLT_DRAWBLK);
	  }

      *?*?*  IF BIT-BLT CONTEXT SETTINGS ARE APPROPRIATE TO SUPPORT "DRAW_HLINE" AND 
      *?*?*  H/W ACCELERATION ENGINE CAN PERFORM THIS OPERATION FASTER THAN API (IOBLT),
      *?*?*  SET "DRAW_HLINE" FIELD OF BIT-BLT CONTEXT STRUCTURE. 
	  if (...) {
             _hwblt_set_draw_hline(bc, GDV_HWBLT_DRAWHLINE);
	  }

      *?*?*  IF BIT-BLT CONTEXT SETTINGS ARE APPROPRIATE TO SUPPORT "DRAW_VLINE" AND 
      *?*?*  H/W ACCELERATION ENGINE CAN PERFORM THIS OPERATION FASTER THAN API (IOBLT),
      *?*?*  SET "DRAW_VLINE" FIELD OF BIT-BLT CONTEXT STRUCTURE. 
	  if (...) {
             _hwblt_set_draw_vline(bc, GDV_HWBLT_DRAWVLINE);
	  }

      *?*?*  IF BIT-BLT CONTEXT SETTINGS ARE APPROPRIATE TO SUPPORT "DRAW_PIXEL" AND 
      *?*?*  H/W ACCELERATION ENGINE CAN PERFORM THIS OPERATION FASTER THAN API (IOBLT),
      *?*?*  SET "DRAW_PIXEL" FIELD OF BIT-BLT CONTEXT STRUCTURE. 
	  if (...) {
             _hwblt_set_draw_pixel(bc, GDV_HWBLT_DRAWPIXEL);
	  }

      *?*?*  IF BIT-BLT CONTEXT SETTINGS ARE APPROPRIATE TO SUPPORT "COPY_BLOCK" AND 
      *?*?*  H/W ACCELERATION ENGINE CAN PERFORM THIS OPERATION FASTER THAN API (IOBLT),
      *?*?*  SET "COPY_BLOCK" FIELD OF BIT-BLT CONTEXT STRUCTURE. 
	  if (...) {
             _hwblt_set_copy_block(bc, GDV_HWBLT_COPYBLK);
	  }

      *?*?*  IF BIT-BLT CONTEXT SETTINGS ARE APPROPRIATE TO SUPPORT "COPY_NEXT_BLOCK" 
      *?*?*  AND H/W ACCELERATION ENGINE CAN PERFORM THIS OPERATION FASTER THAN API 
      *?*?*  (IOBLT), SET "COPY_NEXT_BLOCK" FIELD OF BIT-BLT CONTEXT STRUCTURE. 
	  if (...) {
             _hwblt_set_copy_next_block(bc, GDV_HWBLT_COPYNBLK);
	  }

      *?*?*  IF BIT-BLT CONTEXT SETTINGS ARE APPROPRIATE TO SUPPORT "EXPD_BLOCK" AND 
      *?*?*  H/W ACCELERATION ENGINE CAN PERFORM THIS OPERATION FASTER THAN API (IOBLT),
      *?*?*  SET "EXPD_BLOCK" FIELD OF BIT-BLT CONTEXT STRUCTURE. 
	  if (...) {
             _hwblt_set_expd_block(bc, GDV_HWBLT_EXPDBLK);
	  }

      *?*?*  IF BIT-BLT CONTEXT SETTINGS ARE APPROPRIATE TO SUPPORT "EXPD_NEXT_BLOCK"  
      *?*?*  AND H/W ACCELERATION ENGINE CAN PERFORM THIS OPERATION FASTER THAN API 
      *?*?*  (IOBLT), SET "EXPD_NEXT_BLOCK" FIELD OF BIT-BLT CONTEXT STRUCTURE. 
	  if (...) {
             _hwblt_set_expd_next_block(bc, GDV_HWBLT_EXPDNBLK);
	  }

      *?*?*  IF BIT-BLT CONTEXT SETTINGS ARE APPROPRIATE TO SUPPORT "GET_PIXEL" AND 
      *?*?*  H/W ACCELERATION ENGINE CAN PERFORM THIS OPERATION FASTER THAN API (IOBLT),
      *?*?*  SET "GET_PIXEL" FIELD OF BIT-BLT CONTEXT STRUCTURE. 
	  if (...) {
             _hwblt_set_get_pixel(bc, GDV_HWBLT_GETPIXEL);
	  }

      }

        return SUCCESS;
}

