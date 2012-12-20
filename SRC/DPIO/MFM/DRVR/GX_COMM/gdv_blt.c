/*******************************************************************************
* DESCRIPTION :
*
*	I/O based Bit-BLT support functions.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  01/08/96  TJE  First version
*       2  10/20/98  AND  H/W BLT layer
*       3  01/12/00  AND  Fix _gdv_blt_src() and _gdv_blt_dst() 
*                         to handle dmap == NULL case correctly
*       4  07/13/00  SRM  Return an error on expand with RWT mode
*       5  07/17/00  AND  Add RWT for expand operation and properly
*                         handle RWT for pixel depths other than 4bpp and 8bpp 
*/
#include <gdv_priv.h>
#include <memory.h>
#include <modes.h>
#include <string.h>

/*******************************************************************************
* PROTOTYPES FOR STATIC FUNCTIONS
*******************************************************************************/
#ifdef GDV_INCLUDE_IOBLT
error_code set_dispatch(BLT_CONTEXT *bc);
static void set_pixword(GFX_CM coding_method, GFX_PIXEL pix, GDV_WORD *pixword);
u_int32 _gdv_get_pixel_depth(GFX_CM coding_method);

#ifdef GDV_IOBLT_SEP_CHROMA
static void _gdv_sysram_read(BLT_CONTEXT *bc, u_char *src_ptr,
	u_char *chroma_ptr, u_char *buf_ptr, u_int32 word_count);
static void _gdv_sysram_write(BLT_CONTEXT *bc, u_char *dst_ptr,
	u_char *chroma_ptr, u_char *buf_ptr, u_int32 word_count);
static void _gdv_sysram_write_pix(BLT_CONTEXT *bc, u_char *dst_ptr,
	u_char *chroma_ptr, GDV_WORD *pix_word, u_int32 word_count);
#else
static void _gdv_sysram_read(BLT_CONTEXT *bc, u_char *src_ptr, u_char *buf_ptr,
	u_int32 word_count);
static void _gdv_sysram_write(BLT_CONTEXT *bc, u_char *dst_ptr, u_char *buf_ptr,
	u_int32 word_count);
static void _gdv_sysram_write_pix(BLT_CONTEXT *bc, u_char *dst_ptr, GDV_WORD
	*pix_word, u_int32 word_count);
#endif
#endif  /**GDV_INCLUDE_IOBLT**/


/*******************************************************************************
* NAME:		_gdv_blt_createbc()
*
* USAGE:	Create attachment to Bit-BLT context
*/
error_code _gdv_blt_createbc(Dev_list dev_entry, GFX_DEV *gfxdev, BLT_CONTEXT
	*bc)
{
	error_code ec;
	u_int32 size;
	GDV_BCATCH *bcatch;

	/* Set pointers to fast entry points for changes to the context object */
	#ifdef GDV_FE_SYSTATE
		bc->set_context_drwmix = _os_ss_blt_drwmix;
		bc->set_context_cpymix = _os_ss_blt_cpymix;
		bc->set_context_expmix = _os_ss_blt_expmix;
		bc->set_context_pix = _os_ss_blt_pix;
		bc->set_context_src = _os_ss_blt_src;
		bc->set_context_exptbl = _os_ss_blt_exptbl;
		bc->set_context_trans = _os_ss_blt_trans;
		bc->set_context_mask = _os_ss_blt_mask;
		bc->set_context_ofs = _os_ss_blt_ofs;
		bc->set_context_dst = _os_ss_blt_dst;
	#else
		bc->set_context_drwmix = _gdv_blt_drwmix;
		bc->set_context_cpymix = _gdv_blt_cpymix;
		bc->set_context_expmix = _gdv_blt_expmix;
		bc->set_context_pix = _gdv_blt_pix;
		bc->set_context_src = _gdv_blt_src;
		bc->set_context_exptbl = _gdv_blt_exptbl;
		bc->set_context_trans = _gdv_blt_trans;
		bc->set_context_mask = _gdv_blt_mask;
		bc->set_context_ofs = _gdv_blt_ofs;
		bc->set_context_dst = _gdv_blt_dst;
	#endif

#if defined(GDV_MEM_TOP_BIT)
     bc->set_context_drwmix = (error_code(*)(BLT_CONTEXT *))adrcvt(bc->set_context_drwmix);
     bc->set_context_cpymix = (error_code(*)(BLT_CONTEXT *))adrcvt(bc->set_context_cpymix);
     bc->set_context_expmix = (error_code(*)(BLT_CONTEXT *))adrcvt(bc->set_context_expmix);
     bc->set_context_pix = (error_code(*)(BLT_CONTEXT *))adrcvt(bc->set_context_pix);
     bc->set_context_src = (error_code(*)(BLT_CONTEXT *))adrcvt(bc->set_context_src);
     bc->set_context_exptbl = (error_code(*)(BLT_CONTEXT *))adrcvt(bc->set_context_exptbl);
     bc->set_context_trans = (error_code(*)(BLT_CONTEXT *))adrcvt(bc->set_context_trans);
     bc->set_context_mask = (error_code(*)(BLT_CONTEXT *))adrcvt(bc->set_context_mask);
     bc->set_context_ofs = (error_code(*)(BLT_CONTEXT *))adrcvt(bc->set_context_ofs);
     bc->set_context_dst = (error_code(*)(BLT_CONTEXT *))adrcvt(bc->set_context_dst);
#endif

	/* Allocate space for the driver attachment and permit access to it */
	size = sizeof(GDV_BCATCH);
	if ((ec = _os_srqmem(&size, (void *)&bcatch, 0)) != SUCCESS) {
		return ec;
	}
	#if defined(GDV_MEM_TOP_BIT)
	bcatch = (GDV_BCATCH *)adrcvt(bcatch);	/* SH3 */
	#endif
	_os_permit(bcatch, sizeof(GDV_BCATCH), S_IREAD | S_IWRITE, gfxdev->pid);
	bc->gdv_attachment = bcatch;

#ifdef GDV_BCATCH_SPECIFICS
        GDV_INIT_BCATCH(bc);
#endif
	return SUCCESS;
}

/*******************************************************************************
* NAME:		_gdv_blt_destroybc()
*
* USAGE:	Destroy attachment to Bit-BLT context
*/
error_code _gdv_blt_destroybc(Dev_list dev_entry, GFX_DEV *gfxdev, BLT_CONTEXT
	*bc)
{
	error_code ec, result = SUCCESS;
	GDV_BCATCH *bcatch = bc->gdv_attachment;

	/* Protect and deallocate the Bit-BLT context attachment */
	if (bc->gdv_attachment != NULL) {
		_os_protect(bcatch, sizeof(GDV_BCATCH), S_IREAD | S_IWRITE,
			gfxdev->pid);
	      #if defined(GDV_MEM_TOP_BIT)
	      bcatch = (GDV_BCATCH *)unadrcvt(bcatch);	/* SH3 */
	      #endif
		if ((ec = _os_srtmem(sizeof(GDV_BCATCH), bcatch)) != SUCCESS) {
			result = ec;
		}
	}

#ifdef GDV_BCATCH_SPECIFICS
        GDV_TERM_BCATCH(bc);
#endif
	return result;
}

/*******************************************************************************
* NAME:		_gdv_blt_drwmix()
*
* USAGE:	Set mixing mode for draw operations
*/
error_code _gdv_blt_drwmix(BLT_CONTEXT *bc)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

#ifdef GDV_INCLUDE_IOBLT
	ec = set_dispatch(bc);
#endif
#ifdef GDV_INCLUDE_HWBLT
      ec = GDV_HWBLT_DRWMIX(bc);
#endif

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}

/*******************************************************************************
* NAME:		_gdv_blt_cpymix()
*
* USAGE:	Set mixing mode for copy operations
*/
error_code _gdv_blt_cpymix(BLT_CONTEXT *bc)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

#ifdef GDV_INCLUDE_IOBLT
	ec = set_dispatch(bc);
#endif
#ifdef GDV_INCLUDE_HWBLT
      ec = GDV_HWBLT_CPYMIX(bc);
#endif

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}

/*******************************************************************************
* NAME:		_gdv_blt_expmix()
*
* USAGE:	Set mixing mode for expand operations
*/
error_code _gdv_blt_expmix(BLT_CONTEXT *bc)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

#ifdef GDV_INCLUDE_IOBLT
	ec = set_dispatch(bc);
#endif
#ifdef GDV_INCLUDE_HWBLT
      ec = GDV_HWBLT_EXPMIX(bc);
#endif

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}

/*******************************************************************************
* NAME:		_gdv_blt_pix()
*
* USAGE:	Set drawing pixel value
*/
error_code _gdv_blt_pix(BLT_CONTEXT *bc)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif
#ifdef GDV_INCLUDE_IOBLT
	GDV_BCATCH *bcatch = bc->gdv_attachment;
#endif

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

#ifdef GDV_INCLUDE_IOBLT
	/* Re-compute the current GDV_WORD for drawing with this pixel value */
	if (bc->dstdmap != NULL) {
		set_pixword(bc->dstdmap->coding_method, bc->drwpixel,
			&bcatch->curpix_word);
	}

	/* Set dispatch variables */
	ec = set_dispatch(bc);
#endif
#ifdef GDV_INCLUDE_HWBLT
      ec = GDV_HWBLT_PIX(bc);
#endif

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}

/*******************************************************************************
* NAME:		_gdv_blt_src()
*
* USAGE:	Set source drawmap
*/
error_code _gdv_blt_src(BLT_CONTEXT *bc)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif
#ifdef GDV_INCLUDE_IOBLT
	GDV_BCATCH *bcatch = bc->gdv_attachment;
#endif

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

#ifdef GDV_INCLUDE_IOBLT
	/* Set variables in the template section of the driver attachment */
	if (bc->srcdmap == NULL) {
	        bcatch->src_gfxram = FALSE;
	        bcatch->src_line_size = 0;
	} else {
	        bcatch->src_gfxram = GDV_IOBLT_GFXRAM(bc, bc->srcdmap->pixmem);
	        bcatch->src_line_size = bc->srcdmap->line_size;
	}
	if (bc->mask_dmap == NULL) {
		bcatch->msk_gfxram = FALSE;
	} else {
		bcatch->msk_gfxram = GDV_IOBLT_GFXRAM(bc, bc->mask_dmap->pixmem);
	}
	if (bc->srcdmap != NULL) {
		switch (_gdv_get_pixel_depth(bc->srcdmap->coding_method)) {
		case 1: /*GFX_CM_1BIT:*/
            bcatch->srcppw = GDV_IOBLT_WORDSIZ * 8;
            bcatch->srcppw_shift = GDV_IOBLT_WORDSFT + 3;
            bcatch->srcbpp_shift = 0;
            bcatch->srcbpp_mask = 0x00;
            break;
		case 2: /*GFX_CM_2BIT:*/
            bcatch->srcppw = GDV_IOBLT_WORDSIZ * 4;
            bcatch->srcppw_shift = GDV_IOBLT_WORDSFT + 2;
            bcatch->srcbpp_shift = 1;
            bcatch->srcbpp_mask = 0x01;
            break;
		case 4: /* GFX_CM_3BIT: GFX_CM_4BIT: GFX_CM_CDI_RL3:*/
            bcatch->srcppw = GDV_IOBLT_WORDSIZ * 2;
            bcatch->srcppw_shift = GDV_IOBLT_WORDSFT + 1;
            bcatch->srcbpp_shift = 2;
            bcatch->srcbpp_mask = 0x03;
            break;
		case 8: /*GFX_CM_5BIT:GFX_CM_6BIT: GFX_CM_7BIT: GFX_CM_8BIT:GFX_CM_CDI_RL7:GFX_CM_CDI_DYUV:*/
            bcatch->srcppw = GDV_IOBLT_WORDSIZ;
            bcatch->srcppw_shift = GDV_IOBLT_WORDSFT;
            bcatch->srcbpp_shift = 3;
            bcatch->srcbpp_mask = 0x07;
            break;
		case 16: /* GFX_CM_RGB555: GFX_CM_1A7_8BIT: GFX_CM_A1_RGB555:GFX_CM_YCBCR422: GFX_CM_YCRCB422:GFX_CM_RGB565:GFX_CM_RGB655:GFX_CM_RGB556 */
            bcatch->srcppw = GDV_IOBLT_WORDSIZ / 2;
            bcatch->srcppw_shift = GDV_IOBLT_WORDSFT - 1;
            bcatch->srcbpp_shift = 4;
            bcatch->srcbpp_mask = 0x0f;
            break;
#ifdef GDV_IOBLT_SEP_CHROMA
        case 12: /*GFX_CM_YCBCR420:GFX_CM_YCRCB420*/
            bcatch->srcppw = GDV_IOBLT_WORDSIZ / 2;
            bcatch->srcppw_shift = GDV_IOBLT_WORDSFT - 1;
            bcatch->srcbpp_shift = 4;
            bcatch->srcbpp_mask = 0x0f;
            bcatch->src_chroma_dmap_ptr = (u_char *)(bc->srcdmap->pixmem) +
                (bc->srcdmap->height * bc->srcdmap->line_size);
            break;
#endif
		case 32:/* GFX_CM_RGB888:*/
            bcatch->srcppw = GDV_IOBLT_WORDSIZ / 4;
            bcatch->srcppw_shift = GDV_IOBLT_WORDSFT - 2;
            bcatch->srcbpp_shift = 5;
            bcatch->srcbpp_mask = 0x1f;
            break;
        default:
#ifndef GDV_FE_SYSTATE
            change_static(app_static);
#endif
            return EOS_MAUI_BADCODEMETH;
		}
	}
    
	/* Set dispatch variables */
	ec = set_dispatch(bc);

	/* Set IOBLT offsets for separate odd/even lines. If not graphics RAM */
	/* then just set them both to zero. */
	#ifdef GDV_IOBLT_OFFSETS
	if (bcatch->src_gfxram == TRUE) {
		GDV_IOBLT_OFFSETS(bc->srcdmap, &bcatch->src_odd_ofs,
			&bcatch->src_even_ofs);
	} else {
		bcatch->src_odd_ofs = bcatch->src_even_ofs = 0;
	}
	if (bcatch->msk_gfxram == TRUE) {
		if (bc->mask_dmap != NULL) {
			GDV_IOBLT_OFFSETS(bc->mask_dmap, &bcatch->msk_odd_ofs,
				&bcatch->msk_even_ofs);
		}
	} else {
		bcatch->msk_odd_ofs = bcatch->msk_even_ofs = 0;
	}
	#endif
#endif /**GDV_INCLUDE_IOBLT**/
#ifdef GDV_INCLUDE_HWBLT
      ec = GDV_HWBLT_SRC(bc);
#endif

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}

/*******************************************************************************
* NAME:		_gdv_blt_exptbl()
*
* USAGE:	Set pixel expansion table
*/
error_code _gdv_blt_exptbl(BLT_CONTEXT *bc)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

#ifdef GDV_INCLUDE_IOBLT
	ec = set_dispatch(bc);
#endif
#ifdef GDV_INCLUDE_HWBLT
      ec = GDV_HWBLT_EXPTBL(bc);
#endif

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}

/*******************************************************************************
* NAME:		_gdv_blt_trans()
*
* USAGE:	Set transparent pixel value
*/
error_code _gdv_blt_trans(BLT_CONTEXT *bc)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif
#ifdef GDV_INCLUDE_IOBLT
	GDV_BCATCH *bcatch = bc->gdv_attachment;
#endif

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

#ifdef GDV_INCLUDE_IOBLT
	/* Re-compute the current GDV_WORD for drawing with this pixel value */
	if (bc->dstdmap != NULL) {
		set_pixword(bc->dstdmap->coding_method, bc->transpixel,
			&bcatch->transpix_word);
	}

	ec = set_dispatch(bc);
#endif
#ifdef GDV_INCLUDE_HWBLT
      ec = GDV_HWBLT_TRANS(bc);
#endif

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}

/*******************************************************************************
* NAME:		_gdv_blt_mask()
*
* USAGE:	Set mask drawmap
*/
error_code _gdv_blt_mask(BLT_CONTEXT *bc)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif
#ifdef GDV_INCLUDE_IOBLT
	GDV_BCATCH *bcatch = bc->gdv_attachment;
#endif

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

#ifdef GDV_INCLUDE_IOBLT
	/* Set variables in the template section of the driver attachment */
	if (bc->mask_dmap == NULL) {
		bcatch->msk_gfxram = FALSE;
#ifdef GDV_IOBLT_OFFSETS
		bcatch->msk_odd_ofs = bcatch->msk_even_ofs = 0;
#endif
	} else {
		bcatch->msk_gfxram = GDV_IOBLT_GFXRAM(bc, bc->mask_dmap->pixmem);
#ifdef GDV_IOBLT_OFFSETS
		if (bcatch->msk_gfxram == TRUE) {
			GDV_IOBLT_OFFSETS(bc->mask_dmap, &bcatch->msk_odd_ofs,
				&bcatch->msk_even_ofs);
		}
#endif
	}

	ec = set_dispatch(bc);
#endif
#ifdef GDV_INCLUDE_HWBLT
      ec = GDV_HWBLT_MASK(bc);
#endif

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}

/*******************************************************************************
* NAME:		_gdv_blt_ofs()
*
* USAGE:	Set offset pixel value
*/
error_code _gdv_blt_ofs(BLT_CONTEXT *bc)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif
#ifdef GDV_INCLUDE_IOBLT
	GDV_BCATCH *bcatch = bc->gdv_attachment;
#endif

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

#ifdef GDV_INCLUDE_IOBLT
	/* Re-compute the current GDV_WORD for drawing with this offset value */
	if (bc->dstdmap != NULL) {
		set_pixword(bc->dstdmap->coding_method, bc->ofspixel,
			&bcatch->ofspix_word);
	}

	ec = set_dispatch(bc);
#endif
#ifdef GDV_INCLUDE_HWBLT
      ec = GDV_HWBLT_OFS(bc);
#endif

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}

/*******************************************************************************
* NAME:		_gdv_blt_dst()
*
* USAGE:	Set destination drawmap
*/
error_code _gdv_blt_dst(BLT_CONTEXT *bc)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif
#ifdef GDV_INCLUDE_IOBLT
	GDV_BCATCH *bcatch = bc->gdv_attachment;
#endif

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

#ifdef GDV_INCLUDE_IOBLT
	/* Set variables in the template section of the driver attachment */
	if (bc->dstdmap == NULL) {
	        bcatch->dst_gfxram = FALSE;
	        bcatch->dst_line_size = 0;
	} else {
	        bcatch->dst_gfxram = GDV_IOBLT_GFXRAM(bc, bc->dstdmap->pixmem);
	        bcatch->dst_line_size = bc->dstdmap->line_size;
	}
	if (bc->dstdmap != NULL) {
		switch (_gdv_get_pixel_depth(bc->dstdmap->coding_method)) {
        case 1: /*GFX_CM_1BIT:*/
            bcatch->dstppw = GDV_IOBLT_WORDSIZ * 8;
            bcatch->dstppw_shift = GDV_IOBLT_WORDSFT + 3;
            bcatch->dstbpp_shift = 0;
            bcatch->dstbpp_mask = 0x00;
            break;
        case 2: /*GFX_CM_2BIT:*/
            bcatch->dstppw = GDV_IOBLT_WORDSIZ * 4;
            bcatch->dstppw_shift = GDV_IOBLT_WORDSFT + 2;
            bcatch->dstbpp_shift = 1;
            bcatch->dstbpp_mask = 0x01;
            bcatch->expd_src = expd_src_1to2;
            break;
        case 4: /*GFX_CM_3BIT:GFX_CM_4BIT:GFX_CM_CDI_RL3:*/
            bcatch->dstppw = GDV_IOBLT_WORDSIZ * 2;
            bcatch->dstppw_shift = GDV_IOBLT_WORDSFT + 1;
            bcatch->dstbpp_shift = 2;
            bcatch->dstbpp_mask = 0x03;
            bcatch->expd_src = expd_src_1to4;
            break;
        case 8 : /*GFX_CM_5BIT:GFX_CM_6BIT:GFX_CM_7BIT:GFX_CM_8BIT:GFX_CM_CDI_RL7:GFX_CM_CDI_DYUV:*/
            bcatch->dstppw = GDV_IOBLT_WORDSIZ;
            bcatch->dstppw_shift = GDV_IOBLT_WORDSFT;
            bcatch->dstbpp_shift = 3;
            bcatch->dstbpp_mask = 0x07;
            bcatch->expd_src = expd_src_1to8;
            break;
        case 16:/*GFX_CM_RGB555:GFX_CM_1A7_8BIT:GFX_CM_A1_RGB555:GFX_CM_YCBCR422:GFX_CM_YCRCB422:GFX_CM_RGB565:GFX_CM_RGB655:GFX_CM_RGB556 */
            bcatch->dstppw = GDV_IOBLT_WORDSIZ / 2;
            bcatch->dstppw_shift = GDV_IOBLT_WORDSFT - 1;
            bcatch->dstbpp_shift = 4;
            bcatch->dstbpp_mask = 0x0f;
            bcatch->expd_src = expd_src_1to16;
            break;
        case 32: /*GFX_CM_RGB888:*/
            bcatch->dstppw = GDV_IOBLT_WORDSIZ / 4;
            bcatch->dstppw_shift = GDV_IOBLT_WORDSFT - 2;
            bcatch->dstbpp_shift = 5;
            bcatch->dstbpp_mask = 0x1f;
            bcatch->expd_src = expd_src_1to32;
            break;
#ifdef GDV_IOBLT_SEP_CHROMA
        case 12: /*GFX_CM_YCBCR420: GFX_CM_YCRCB420:*/
            bcatch->dstppw = GDV_IOBLT_WORDSIZ / 2;
            bcatch->dstppw_shift = GDV_IOBLT_WORDSFT - 1;
            bcatch->dstbpp_shift = 4;
            bcatch->dstbpp_mask = 0x0f;
            bcatch->expd_src = expd_src_1to16_420;
            bcatch->dst_chroma_dmap_ptr = (u_char *)(bc->dstdmap->pixmem) +
                (bc->dstdmap->height * bc->dstdmap->line_size);
            break;
#endif
        default:
#ifndef GDV_FE_SYSTATE
            change_static(app_static);
#endif
            return EOS_MAUI_BADCODEMETH;
		}
	}
    
	/* Re-compute values affected by the the destination coding method */
	if (bc->dstdmap != NULL) {
		set_pixword(bc->dstdmap->coding_method, bc->drwpixel,
			&bcatch->curpix_word);
		set_pixword(bc->dstdmap->coding_method, bc->ofspixel,
			&bcatch->ofspix_word);
		set_pixword(bc->dstdmap->coding_method, bc->transpixel,
			&bcatch->transpix_word);
	}

	/* Set dispatch variables */
	ec = set_dispatch(bc);

	/* Set IOBLT offsets for separate odd/even lines. If not graphics RAM */
	/* then just set them both to zero. */
	#ifdef GDV_IOBLT_OFFSETS
	if (bcatch->dst_gfxram == TRUE) {
		GDV_IOBLT_OFFSETS(bc->dstdmap, &bcatch->dst_odd_ofs,
			&bcatch->dst_even_ofs);
	} else {
		bcatch->dst_odd_ofs = bcatch->dst_even_ofs = 0;
	}
	#endif
#endif  /**GDV_INCLUDE_IOBLT**/
#ifdef GDV_INCLUDE_HWBLT
      ec = GDV_HWBLT_DST(bc);
#endif

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}


/*******************************************************************************/
/*               Functions only used for I/O BLT layer                         */
/*******************************************************************************/
#ifdef GDV_INCLUDE_IOBLT

/*******************************************************************************
* NAME:		set_pixword()
*
* USAGE:	Set the specified GDV_WORD with the specified pixel value. The pixel
*			value is repeated throughout the GDV_WORD.
*/
static void set_pixword(GFX_CM coding_method, GFX_PIXEL pix, GDV_WORD *pixword)
{
	u_char *pixword_ptr = (u_char *)pixword;
	u_int32 b, p;

	/* Create a packed GDV_WORD of the specified pixel value based on the */
	/* destination drawmap coding method. */
	switch (_gdv_get_pixel_depth(coding_method)) {
		case 1: /*GFX_CM_1BIT:*/
			for (b = GDV_IOBLT_WORDSIZ; b; b--) {
				for (p = 8; p; p--) {
					*pixword_ptr = (*pixword_ptr << 1) | (pix & 0x01);
				}
				pixword_ptr++;
			}
			break;
		case 2:/* GFX_CM_2BIT:*/
			for (b = GDV_IOBLT_WORDSIZ; b; b--) {
				for (p = 4; p; p--) {
					*pixword_ptr = (*pixword_ptr << 2) | (pix & 0x03);
				}
				pixword_ptr++;
			}
			break;
		case 4: /*GFX_CM_3BIT:GFX_CM_4BIT:GFX_CM_CDI_RL3:*/
			for (b = GDV_IOBLT_WORDSIZ; b; b--) {
				for (p = 2; p; p--) {
					*pixword_ptr = (*pixword_ptr << 4) | (pix & 0x0f);
				}
				pixword_ptr++;
			}
			break;
    case 8: /*GFX_CM_5BIT:GFX_CM_6BIT:GFX_CM_7BIT:GFX_CM_8BIT:GFX_CM_CDI_RL7:GFX_CM_CDI_DYUV:*/
			for (b = GDV_IOBLT_WORDSIZ; b; b--) {
				*pixword_ptr++ = pix & 0xff;
			}
			break;
    case 16: /* GFX_CM_RGB555:GFX_CM_1A7_8BIT:GFX_CM_A1_RGB555:GFX_CM_YCBCR422:GFX_CM_YCRCB422:GFX_CM_RGB565:GFX_CM_RGB655:GFX_CM_RGB556 */
			for (p = GDV_IOBLT_WORDSIZ >> 1; p; p--) {
				*pixword_ptr++ = (pix >> 8) & 0xff;
				*pixword_ptr++ = pix & 0xff;
			}
			break;
    case 32: /*GFX_CM_RGB888:*/
			for (p = GDV_IOBLT_WORDSIZ >> 2; p; p--) {
				*pixword_ptr++ = (pix >> 24) & 0xff;
				*pixword_ptr++ = (pix >> 16) & 0xff;
				*pixword_ptr++ = (pix >>  8) & 0xff;
				*pixword_ptr++ = pix & 0xff;
			}
			break;
#ifdef GDV_IOBLT_SEP_CHROMA
    case 12:/* GFX_CM_YCBCR420: GFX_CM_YCRCB420:*/
        for (p = GDV_IOBLT_WORDSIZ / 4; p; p--) {
            *pixword_ptr++ = (pix >> 16) & 0xff;
            *pixword_ptr++ = (pix >> 8) & 0xff;
            *pixword_ptr++ = (pix >> 16) & 0xff;
            *pixword_ptr++ = pix & 0xff;
        }
        break;
#endif
	}
}

/*******************************************************************************
* NAME:		set_dispatch()
*
* USAGE:	Set pointers to fast entry points for Bit-BLT operations.
*/
error_code set_dispatch(BLT_CONTEXT *bc)
{
	error_code result = SUCCESS;
	GDV_BCATCH *bcatch = bc->gdv_attachment;

	/* Set variables based on source and mask drawmaps */
	if ((bc->srcdmap != NULL) && (bc->mask_dmap != NULL)) {
	if (((bcatch->src_gfxram == TRUE) || (bcatch->msk_gfxram == TRUE)) &&
		(bc->cpymix == BLT_MIX_RWM)) {
		#ifdef GDV_FE_SYSTATE
			bc->copy_block = _os_ss_ioblt_copyblk;
			bc->copy_next_block = _os_ss_ioblt_copynblk;
		#else
			bc->copy_block = _gdv_ioblt_copyblk;
			bc->copy_next_block = _gdv_ioblt_copynblk;
		#endif
#ifdef GDV_MEM_TOP_BIT
                  bc->copy_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_POS, GFX_POS, GFX_DIMEN, GFX_DIMEN))adrcvt(bc->copy_block);
                  bc->copy_next_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN))adrcvt(bc->copy_next_block);
#endif
	}
	}

	/* Set variables based on source and destination drawmaps */
	if ((bc->srcdmap != NULL) && (bc->dstdmap != NULL)) {
	if ((bcatch->src_gfxram == TRUE) || (bcatch->dst_gfxram == TRUE)) {
		#ifdef GDV_FE_SYSTATE
			bc->copy_block = _os_ss_ioblt_copyblk;
			bc->copy_next_block = _os_ss_ioblt_copynblk;
			bc->expd_block = _os_ss_ioblt_expdblk;
			bc->expd_next_block = _os_ss_ioblt_expdnblk;
		#else
			bc->copy_block = _gdv_ioblt_copyblk;
			bc->copy_next_block = _gdv_ioblt_copynblk;
			bc->expd_block = _gdv_ioblt_expdblk;
			bc->expd_next_block = _gdv_ioblt_expdnblk;
		#endif
#ifdef GDV_MEM_TOP_BIT
                  bc->copy_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_POS, GFX_POS, GFX_DIMEN, GFX_DIMEN))adrcvt(bc->copy_block);
                  bc->copy_next_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN))adrcvt(bc->copy_next_block);
                  bc->expd_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_POS, GFX_POS, GFX_DIMEN, GFX_DIMEN))adrcvt(bc->expd_block);
                  bc->expd_next_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN))adrcvt(bc->expd_next_block);
#endif
	}
	}

	/* Set variables based on source drawmap */
	if (bcatch->src_gfxram == TRUE) {
	if (bc->srcdmap != NULL) {
		#ifdef GDV_FE_SYSTATE
			bc->get_pixel = _os_gs_ioblt_getpixel;
		#else
			bc->get_pixel = _gdv_ioblt_getpixel;
		#endif
#ifdef GDV_MEM_TOP_BIT
                  bc->get_pixel = (error_code(*)(GFX_PIXEL *, BLT_CONTEXT *, GFX_POS, GFX_POS))adrcvt(bc->get_pixel);
#endif
	}
#ifdef GDV_IOBLT_READ
		bcatch->ioblt_read_src = GDV_IOBLT_READ;
#else
		bcatch->ioblt_read_src = GDV_IOBLT_READ_SRC;
#endif
	} else {
		bcatch->ioblt_read_src = _gdv_sysram_read;
	}

	/* Set variables based on mask drawmap */
	if (bcatch->msk_gfxram == TRUE) {
#ifdef GDV_IOBLT_READ
		bcatch->ioblt_read_msk = GDV_IOBLT_READ;
#else
		bcatch->ioblt_read_msk = GDV_IOBLT_READ_SRC;
#endif
	} else {
		bcatch->ioblt_read_msk = _gdv_sysram_read;
	}

	/* Set variables based on destination drawmap */
	if (bcatch->dst_gfxram == TRUE) {
	if (bc->dstdmap != NULL) {
		#ifdef GDV_FE_SYSTATE
			bc->draw_block = _os_ss_ioblt_drawblk;
			bc->draw_hline = _os_ss_ioblt_drawhline;
			bc->draw_vline = _os_ss_ioblt_drawvline;
			bc->draw_pixel = _os_ss_ioblt_drawpixel;
		#else
			bc->draw_block = _gdv_ioblt_drawblk;
			bc->draw_hline = _gdv_ioblt_drawhline;
			bc->draw_vline = _gdv_ioblt_drawvline;
			bc->draw_pixel = _gdv_ioblt_drawpixel;
		#endif
#ifdef GDV_MEM_TOP_BIT
                  bc->draw_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN, GFX_DIMEN))adrcvt(bc->draw_block);
                  bc->draw_hline = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN))adrcvt(bc->draw_hline);
                  bc->draw_vline = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN))adrcvt(bc->draw_vline);
                  bc->draw_pixel = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS))adrcvt(bc->draw_pixel);
#endif
	}
#ifdef GDV_IOBLT_READ
		bcatch->ioblt_read_dst = GDV_IOBLT_READ;
#else
		bcatch->ioblt_read_dst = GDV_IOBLT_READ_DST;
#endif
		bcatch->ioblt_write_dst = GDV_IOBLT_WRITE;
		bcatch->ioblt_write_dstpix = GDV_IOBLT_WRITE_PIX;
	} else {
		bcatch->ioblt_read_dst = _gdv_sysram_read;
		bcatch->ioblt_write_dst = _gdv_sysram_write;
		bcatch->ioblt_write_dstpix = _gdv_sysram_write_pix;
	}

	/* Set variables based on mixing modes */
	switch (bc->drwmix) {
		case BLT_MIX_SXD:
			bcatch->ioblt_drwmix = _gdv_ioblt_mix_sxd;
			break;
		case BLT_MIX_N_SXD:
			bcatch->ioblt_drwmix = _gdv_ioblt_mix_n_sxd;
			break;
		case BLT_MIX_SOD:
			bcatch->ioblt_drwmix = _gdv_ioblt_mix_sod;
			break;
		case BLT_MIX_N_SOD:
			bcatch->ioblt_drwmix = _gdv_ioblt_mix_n_sod;
			break;
		case BLT_MIX_NS_AD:
			bcatch->ioblt_drwmix = _gdv_ioblt_mix_ns_ad;
			break;
		case BLT_MIX_SO_ND:
			bcatch->ioblt_drwmix = _gdv_ioblt_mix_so_nd;
			break;
		case BLT_MIX_SPD:
			bcatch->ioblt_drwmix = _gdv_ioblt_mix_spd;
			break;
		case BLT_MIX_DMS:
			bcatch->ioblt_drwmix = _gdv_ioblt_mix_dms;
			break;
		case BLT_MIX_SPO:
			bcatch->ioblt_drwmix = _gdv_ioblt_mix_spo;
			break;
	}
	switch (bc->cpymix) {
		case BLT_MIX_SXD:
			bcatch->ioblt_cpymix = _gdv_ioblt_mix_sxd;
			break;
		case BLT_MIX_N_SXD:
			bcatch->ioblt_cpymix = _gdv_ioblt_mix_n_sxd;
			break;
		case BLT_MIX_SOD:
			bcatch->ioblt_cpymix = _gdv_ioblt_mix_sod;
			break;
		case BLT_MIX_N_SOD:
			bcatch->ioblt_cpymix = _gdv_ioblt_mix_n_sod;
			break;
		case BLT_MIX_NS_AD:
			bcatch->ioblt_cpymix = _gdv_ioblt_mix_ns_ad;
			break;
		case BLT_MIX_SO_ND:
			bcatch->ioblt_cpymix = _gdv_ioblt_mix_so_nd;
			break;
		case BLT_MIX_SPD:
			bcatch->ioblt_cpymix = _gdv_ioblt_mix_spd;
			break;
		case BLT_MIX_DMS:
			bcatch->ioblt_cpymix = _gdv_ioblt_mix_dms;
			break;
		case BLT_MIX_SPO:
			bcatch->ioblt_cpymix = _gdv_ioblt_mix_spo;
			break;
		case BLT_MIX_RWT:
		  if (bcatch->srcppw == GDV_IOBLT_WORDSIZ * 2) {
			bcatch->ioblt_cpymix = _gdv_ioblt_mix_rwt4;
		  } else if (bcatch->srcppw == GDV_IOBLT_WORDSIZ) {
			bcatch->ioblt_cpymix = _gdv_ioblt_mix_rwt8;
		  } else {
		  /* IOBLT supports copy with transparency for 4bpp and 8bpp */
		  /* only therefore force it back to default REPLACE mix mode */
		  /* and report an error */
			bc->cpymix = BLT_MIX_REPLACE;
			result = EOS_MAUI_NOHWSUPPORT;
		  }
			break;
		case BLT_MIX_RWM:
			bcatch->ioblt_cpymix = _gdv_ioblt_mix_rwm;
			break;
	}
	switch (bc->expmix) {
		case BLT_MIX_SXD:
			bcatch->ioblt_expmix = _gdv_ioblt_mix_sxd;
			break;
		case BLT_MIX_N_SXD:
			bcatch->ioblt_expmix = _gdv_ioblt_mix_n_sxd;
			break;
		case BLT_MIX_SOD:
			bcatch->ioblt_expmix = _gdv_ioblt_mix_sod;
			break;
		case BLT_MIX_N_SOD:
			bcatch->ioblt_expmix = _gdv_ioblt_mix_n_sod;
			break;
		case BLT_MIX_NS_AD:
			bcatch->ioblt_expmix = _gdv_ioblt_mix_ns_ad;
			break;
		case BLT_MIX_SO_ND:
			bcatch->ioblt_expmix = _gdv_ioblt_mix_so_nd;
			break;
		case BLT_MIX_SPD:
			bcatch->ioblt_expmix = _gdv_ioblt_mix_spd;
			break;
		case BLT_MIX_DMS:
			bcatch->ioblt_expmix = _gdv_ioblt_mix_dms;
			break;
		case BLT_MIX_SPO:
			bcatch->ioblt_expmix = _gdv_ioblt_mix_spo;
			break;
		case BLT_MIX_RWT:
		  if (bcatch->dstppw == GDV_IOBLT_WORDSIZ * 2) {
			bcatch->ioblt_expmix = _gdv_ioblt_mix_rwt4;
		  } else if (bcatch->dstppw == GDV_IOBLT_WORDSIZ) {
			bcatch->ioblt_expmix = _gdv_ioblt_mix_rwt8;
		  } else {
		  /* IOBLT supports expand with transparency for 4bpp and 8bpp */
		  /* only therefore force it back to default REPLACE mix mode */
		  /* and report an error */
			bc->expmix = BLT_MIX_REPLACE;
			result = EOS_MAUI_NOHWSUPPORT;
		  }
			break;
		case BLT_MIX_RWM:
		  /* IOBLT does not currently support expand with mask */
		  /* therefore force it back to default REPLACE mix */
		  /* mode and report an error */
			bc->expmix = BLT_MIX_REPLACE;
			result = EOS_MAUI_NOHWSUPPORT;
			break;
	}

	return result;
}

/*******************************************************************************
* NAME:		_gdv_sysram_read()
*
* USAGE:	Read the number of GDV_WORDs specified by "word_count" from system
*			memory pointed to by "src_ptr" into the line buffer pointed to by
*			"buf_ptr".
*/
#ifdef GDV_IOBLT_SEP_CHROMA
static void _gdv_sysram_read(BLT_CONTEXT *bc, u_char *src_ptr,
	u_char *chroma_ptr, u_char *buf_ptr, u_int32 word_count)
#else
static void _gdv_sysram_read(BLT_CONTEXT *bc, u_char *src_ptr, u_char *buf_ptr,
	u_int32 word_count)
#endif
{
	u_int32 count = word_count * GDV_IOBLT_WORDSIZ;

	memcpy(buf_ptr,src_ptr,count);
}

/*******************************************************************************
* NAME:		_gdv_sysram_write()
*
* USAGE:	Write the number of GDV_WORDs specified by "word_count" from the
*			line buffer pointed to by "buf_ptr" into system memory pointed to by
*			"dst_ptr".
*/
#ifdef GDV_IOBLT_SEP_CHROMA
static void _gdv_sysram_write(BLT_CONTEXT *bc, u_char *dst_ptr,
	u_char *chroma_ptr, u_char *buf_ptr, u_int32 word_count)
#else
static void _gdv_sysram_write(BLT_CONTEXT *bc, u_char *dst_ptr, u_char *buf_ptr,
	u_int32 word_count)
#endif
{
	u_int32 count = word_count * GDV_IOBLT_WORDSIZ;

	memcpy(dst_ptr,buf_ptr,count);
}

/*******************************************************************************
* NAME:		_gdv_sysram_write_pix()
*
* USAGE:	Write the number of GDV_WORDs specified by "word_count" of the solid
*			color pointed to by "pix_word" into system memory pointed to by
*			"dst_ptr".
* NOTE:         This function has been optimized for the case where GDV_IOBLT_WORDSIZ
*               is equal to 4. (Most common case)  This assumes that dst_ptr is
*               aligned on a GDV_IOBLT_WORDSIZ byte boundary (4 byte boundary).
*/
#ifdef GDV_IOBLT_SEP_CHROMA
static void _gdv_sysram_write_pix(BLT_CONTEXT *bc, u_char *dst_ptr,
	u_char *chroma_ptr, GDV_WORD *pix_word, u_int32 word_count)
#else
static void _gdv_sysram_write_pix(BLT_CONTEXT *bc, u_char *dst_ptr, GDV_WORD
	*pix_word, u_int32 word_count)
#endif
{
#if (GDV_IOBLT_WORDSIZ == 4)
        u_int32 *dst4=(u_int32*)dst_ptr;
	u_int32 *tmp = (u_int32*)(&pix_word->bm[0]);
	u_int32 src4 = *tmp;
#else
	u_int32 b;
#endif

	while (word_count--) {
#if (GDV_IOBLT_WORDSIZ == 4)
                *dst4++ = src4;
#else
		for (b = GDV_IOBLT_WORDSIZ; b; b--) {
			dst_ptr[b] = pix_word->bm[b];
		}
		dst_ptr += GDV_IOBLT_WORDSIZ;
#endif
	}
}


/*******************************************************************************
* NAME:		_gdv_get_pixel_depth()
*
* USAGE:	Gets the pixel depth encoded in the coding method
*/

u_int32 _gdv_get_pixel_depth(GFX_CM coding_method)
{
    u_int32 pixel_depth;
	if((pixel_depth = gfx_get_cm_depth(coding_method))==0)
	{
        switch (gfx_get_cm_name(coding_method)) {
        case GFX_CM_1BIT: 
		  pixel_depth = 1; 
		  break;
        case GFX_CM_2BIT: 
		  pixel_depth = 2; 
		  break;
        case GFX_CM_3BIT:
        case GFX_CM_4BIT:
        case GFX_CM_CDI_RL3: 
		  pixel_depth = 4; 
		  break;
        case GFX_CM_5BIT:
        case GFX_CM_6BIT:
        case GFX_CM_7BIT:
        case GFX_CM_8BIT:
        case GFX_CM_CDI_RL7:
        case GFX_CM_CDI_DYUV: 

		  pixel_depth = 8; 
		  break;
        case GFX_CM_RGB555:
        case GFX_CM_1A7_8BIT:
        case GFX_CM_A1_RGB555: 
		case GFX_CM_RGB565:
		case GFX_CM_RGB655:
		case GFX_CM_RGB556:
		  pixel_depth = 16; 
		  break;
#ifdef GDV_IOBLT_SEP_CHROMA
        case GFX_CM_YCBCR420:
        case GFX_CM_YCRCB420:
            pixel_depth = 12; 
		  break;
#endif
        case GFX_CM_RGB888: 
		  pixel_depth = 32; 
		  break;
        default: pixel_depth = 0;
		  break;
        }
    }
	else /* The pixel depth is encoded in coding method */
	{

	  /* The following is the quotation from maui_gfx.h *********/
/*******************************************************************************
* CODING METHOD
*
* Range of Bits		Description
* 31				Byte endianess (0 = Big, 1 = Little)
* 30				Bit endianess (0 = Big, 1 = Little)
* 27 - 29			Pixel bit depth
* 24 - 26			Line alignment
* 10 - 23			Reserved by Microware
* 0 - 9				Coding Method Name
*
* If pixel bit depth is 0 then you must determine it from the coding method
* name. Otherwise it is 1=1bpp, 2=2bpp, 3=4bpp, 4=8bpp, 5=16bpp, 6=32bpp,
* 7 is reserved by Microware.
*
* The line alignment is 0=4bytes (default), 1=8, 2=16, 3=32, 4=64, 5 through
* 7 are reserved by Microware.
*
* The coding method name is divided into the following numeric ranges.
*
* Range				Description
* 0 - 255			Standard coding methods
* 256 - 511			Standard coding methods that require driver for Bit-BLT
* 512 - 767			Reserved by Microware
* 768 - 1023		Defined by OEMs
* 
******************************************************************************/

        switch (pixel_depth)
        {
        case 1:
        case 2:
            break;
        case 3:
            pixel_depth = 4;
            break;
        case 4:
            pixel_depth = 8;
            break;
        case 5:
            pixel_depth = 16;
            break;
        case 6:
            pixel_depth = 32;
            break;
        default:
            pixel_depth = 0;
            break;
        }
	}
    return pixel_depth;
}
#endif  /**GDV_INCLUDE_IOBLT**/
