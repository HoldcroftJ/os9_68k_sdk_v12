/*******************************************************************************
* FILENAME : mfm_blt.h
*
* DESCRIPTION :
*
*	This file contains definitions for the interface between the Bit-BLT API
*	and the graphics driver. This file should not be used directly by app-
*	lications.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  12/21/94  TJE  First version
*       2  08/10/00  gdw  Move the msktable out of the context. This will allow
*                         lazy allocate and expansion to 2 bit destination maps.
*/
#ifndef _MFM_BLT_H
#define _MFM_BLT_H

#include <MAUI/maui_com.h>
#define _GFX_ID_STRUCTS_EXPOSED
#include <MAUI/maui_gfx.h>
#define _BLT_ID_STRUCTS_EXPOSED
#include <MAUI/maui_blt.h>

typedef u_int32 (*_blt_swap_fn)(u_int32);

/*******************************************************************************
* BIT-BLT CONTEXT OBJECT
*******************************************************************************/

#define _BLT_CONTEXT_SYNC 0x42435458 /* "BCTX" */ 
struct _BLT_CONTEXT {
	u_int32 sync_code;			/* Sync code - must be _BLT_CONTEXT_SYNC */

	/* PUBLIC SECTION - VISIBLE TO THE API AND GRAPHICS DRIVER */

	/* This section of the structure contains members that are specified by */
	/* the user of this API. They are initialized in blt_create_context(), */
	/* set by blt_set_context*() and retrieved by blt_get_context*(). */

	GFX_DEV *gfxdev;			/* Graphics device this viewport belongs to */
	BLT_MIX drwmix;				/* Mixing mode for draw operations */
	BLT_MIX cpymix;				/* Mixing mode for copy operations */
	BLT_MIX expmix;				/* Mixing mode for expand operations */
	GFX_PIXEL drwpixel;			/* Pixel value for drawing (1 pixel only) */
	const GFX_DMAP *srcdmap;	/* Source drawmap (NULL if none) */
	u_int8 exptbl_entries;		/* Number of entries in exptbl */
	const GFX_PIXEL *exptbl;	/* Pixel expansion table */
	GFX_PIXEL transpixel;		/* Transparent pixel value (1 pixel only) */
	const GFX_DMAP *mask_dmap;	/* Mask drawmap (NULL if none) */
	GFX_PIXEL ofspixel;			/* Offset pixel value (1 pixel only) */
	const GFX_DMAP *dstdmap;	/* Destination drawmap (NULL if none) */

    /* This section contains pointers to the driver functions for handling */
	/* changes to this Bit-BLT context. These pointers must be initialized */
	/* by the driver when its _os_ss_blt_createbc() function is called. */

	error_code (*set_context_drwmix)(BLT_CONTEXT *bc);
	error_code (*set_context_cpymix)(BLT_CONTEXT *bc);
	error_code (*set_context_expmix)(BLT_CONTEXT *bc);
	error_code (*set_context_pix)(BLT_CONTEXT *bc);
	error_code (*set_context_src)(BLT_CONTEXT *bc);
	error_code (*set_context_exptbl)(BLT_CONTEXT *bc);
	error_code (*set_context_trans)(BLT_CONTEXT *bc);
	error_code (*set_context_mask)(BLT_CONTEXT *bc);
	error_code (*set_context_ofs)(BLT_CONTEXT *bc);
	error_code (*set_context_dst)(BLT_CONTEXT *bc);

    /* This section contains dispatch function pointers. They are setup by */
    /* calling blt_set_context*(). This function gives the graphics driver */
    /* a chance to update them when _os_fe_blt_setbc() is called. */

	error_code (*copy_block)(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
		GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
	error_code (*copy_next_block)(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS
		srcx, GFX_DIMEN height);
	error_code (*expd_block)(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
		GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
	error_code (*expd_next_block)(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS
		srcx, GFX_DIMEN height);
	error_code (*draw_block)(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
		GFX_DIMEN width, GFX_DIMEN height);
	error_code (*draw_hline)(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
		GFX_DIMEN width);
	error_code (*draw_vline)(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
		GFX_DIMEN height);
	error_code (*draw_pixel)(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty);
	error_code (*get_pixel)(GFX_PIXEL *ret_pixel, BLT_CONTEXT *context,
		GFX_POS srcx, GFX_POS srcy);

	/* The following member is used to point to a driver defined block of */
	/* data. */

	void *gdv_attachment;		/* Driver allocated attachment */

#ifdef _BLT_API_PRIVATE

	/* PRIVATE SECTION - VISIBLE ONLY TO THE API */

	/* This section of the structure contains members that are defined and */
	/* used internally. They are set by calling blt_set_context*(). */
	        
	GFX_PIXEL drwpixels;		/* drwpixel repeated throughout a longword */
	GFX_PIXEL ofspixels;		/* ofspixel repeated throughout a longword */
	GFX_PIXEL drw_ofspixels;	/* drwpixels + ofspixels */
	GFX_PIXEL transpixels;		/* transpixel repeated throughout a longword */
	GFX_PIXEL *srcpixmem;		/* Source pixel memory */
	GFX_PIXEL *dstpixmem;		/* Destination pixel memory */
	u_int32 srclinesize;		/* Source line size in longwords */
	u_int32 dstlinesize;		/* Destination line size in longwords */
	GFX_CM srccm;				/* Source coding method */
	GFX_CM maskcm;				/* Mask coding method */
	GFX_CM dstcm;				/* Destination coding method */
	u_int32 srcbpp;				/* Source bits per pixel */
	u_int32 srcbpp_shift;		/* Shift value to mul/div by srcbpp */
	u_int32 dstbpp;				/* Destination bits per pixel */
	u_int32 dstbpp_shift;		/* Shift value to mul/div by dstbpp */
	u_int32 srcppl;				/* Source pixels per longword */
	u_int32 srcppl_shift;		/* Shift value to mul/div by srcppl */
	u_int32 dstppl;				/* Destination pixels per longword */
	u_int32 dstppl_shift;		/* Shift value to mul/div by dstppl */
    const MSKFST *srcfst; 		/* Source param table for first longword */
    const MSKLST *srclst;			/* Source param table for last longword */
    const MSKFST *dstfst; 		/* Destination param table for first longword -draw*/
    const MSKLST *dstlst; 		/* Destination param table for last longword -draw*/
	u_int32 expmsk;				/* Mask for source pixel expansion */
	u_int32 *exptable;		/* Lookup table for source pixel expansion */
        u_int32 exptable_size;  /* size of the exptable */
        u_int32 exptable_valid;  /* how the exptable is set up */
	GFX_PIXEL *mask_pixmem;		/* Mask pixel memory */
	u_int32 mask_offset;		/* Offset from source pixmem to mask pixmem */

	/* This section contains dispatch function pointers. They are setup by */
	/* calling blt_set_context*(). */

	void (*copy_block_mix)(_BLT_COPY_BLOCK_MIX_P);
	void (*copy_block_down_mix)(_BLT_COPY_BLOCK_DOWN_MIX_P);
	void (*copy_block_up_mix)(_BLT_COPY_BLOCK_UP_MIX_P);
	void (*expd_block_mix)(_BLT_EXPD_BLOCK_MIX_P);
	void (*expd_block_down_mix)(_BLT_EXPD_BLOCK_DOWN_MIX_P);
	void (*expd_block_up_mix)(_BLT_EXPD_BLOCK_UP_MIX_P);
	void (*draw_block_mix)(_BLT_DRAW_BLOCK_MIX_P);
	void (*draw_hline_mix)(_BLT_DRAW_HLINE_MIX_P);
	void (*draw_vline_mix)(_BLT_DRAW_VLINE_MIX_P);
	void (*draw_pixel_mix)(_BLT_DRAW_PIXEL_MIX_P);

	/* This section of the structure contains members that are initialized by */
	/* drawing functions such as blt_copy_block(). They remain set and are */
	/* then used by continuation functions such as blt_copy_next_block(). */
    
	u_int32 src_cnt_fst;		/* Pixels in the first longword of source */
	u_int32 src_msk_fst;		/* Mask for pixels in first partial longword */
	u_int32 src_cnt_lst;		/* Pixels in the last longword of source */
	u_int32 src_shift_lst;		/* Bits to shift the last longword of source */
								/* in order to put the last group in the */
								/* lower bits */
	int32 shift;				/* Number of bits to shift each longword of */
								/* source to align it with the destination */
	u_int32 dst_ofs;			/* Offset into destination longword */
								/* where the first pixel is located */
	u_int32 dst_cnt_fst;		/* Pixels in first longword of destination */
	u_int32 dst_pcnt_fst;		/* Pixels in first partial longword of dest- */
								/* ination. 0 if first longword is full. */
	u_int32 dst_cnt_mid;		/* Full longwords to process in destination. */
								/* Includes first and/or last longword(s) if */
								/* they are full. */
	u_int32 dst_cnt_lst;		/* Pixels in last longword of destination */
	u_int32 dst_pcnt_lst;		/* Pixels in last partial longword of dest- */
								/* ination. 0 if last longword is full. */
	u_int32 dst_msk_fst;		/* Mask for pixels in first partial longword */
	u_int32 dst_msk_lst;		/* Mask for pixels in last partial longword */
	GFX_DIMEN copy_width;		/* Width of transfer in pixels */
	GFX_PIXEL *src_line_ptr;	/* Ptr to first longword of start Y in source */
	GFX_PIXEL *dst_line_ptr;	/* Ptr to first longword of start Y in dest. */
    _blt_swap_fn src_swap;      /* Swapping function for source */
    _blt_swap_fn dst_swap;      /* Swapping function for destination */
    const u_int32 *msktable;		/* Mask lookup table for source pixel expansion */
    _blt_swap_fn src_exp_swap;  /* Swapping function for source */
    _blt_swap_fn dst_exp_swap;  /* Swapping function for destination */
    GFX_PIXEL exptbl_internal[BLT_EXPTBL_MAX_SIZE]; /* Internal copy of the exp.table */

#endif
};

/*******************************************************************************
* SETSTAT PARAMETER BLOCKS
*******************************************************************************/

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_CREATEBC */
	GFX_DEV *gfxdev;			/* Graphics device */
	BLT_CONTEXT *context;		/* Pointer to Bit-BLT context object */
} ss_blt_createbc_pb, *Ss_blt_createbc_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_DESTROYBC */
	GFX_DEV *gfxdev;			/* Graphics device */
	BLT_CONTEXT *context;		/* Pointer to Bit-BLT context object */
} ss_blt_destroybc_pb, *Ss_blt_destroybc_pb;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code _os_fe_blt_copyblk(BLT_CONTEXT *context, GFX_POS dstx, GFX_POS
	dsty, GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
error_code _os_fe_blt_copynblk(BLT_CONTEXT *context, GFX_POS dstx, GFX_POS
	srcx, GFX_DIMEN height);
error_code _os_fe_blt_drawblk(BLT_CONTEXT *context, GFX_POS dstx, GFX_POS
	dsty, GFX_DIMEN width, GFX_DIMEN height);
error_code _os_fe_blt_drawhline(BLT_CONTEXT *context, GFX_POS dstx,
	GFX_POS dsty, GFX_DIMEN width);
error_code _os_fe_blt_drawpixel(BLT_CONTEXT *context, GFX_POS dstx,
	GFX_POS dsty);
error_code _os_fe_blt_drawvline(BLT_CONTEXT *context, GFX_POS dstx,
	GFX_POS dsty, GFX_DIMEN height);
error_code _os_fe_blt_expdblk(BLT_CONTEXT *context, GFX_POS dstx,
	GFX_POS dsty, GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN
	height);
error_code _os_fe_blt_expdnblk(BLT_CONTEXT *context, GFX_POS dstx,
	GFX_POS srcx, GFX_DIMEN height);
error_code _os_fe_blt_getpixel(GFX_PIXEL *ret_pixel, BLT_CONTEXT *context,
	GFX_POS srcx, GFX_POS srcy);
error_code _os_fe_blt_setcpymix(BLT_CONTEXT *context);
error_code _os_fe_blt_setdrwmix(BLT_CONTEXT *context);
error_code _os_fe_blt_setdst(BLT_CONTEXT *context);
error_code _os_fe_blt_setexpmix(BLT_CONTEXT *context);
error_code _os_fe_blt_setexptbl(BLT_CONTEXT *context);
error_code _os_fe_blt_setmask(BLT_CONTEXT *context);
error_code _os_fe_blt_setofs(BLT_CONTEXT *context);
error_code _os_fe_blt_setpix(BLT_CONTEXT *context);
error_code _os_fe_blt_setsrc(BLT_CONTEXT *context);
error_code _os_fe_blt_settrans(BLT_CONTEXT *context);
error_code _os_ss_blt_createbc(path_id path, GFX_DEV *gfxdev, BLT_CONTEXT
	*context);
error_code _os_ss_blt_destroybc(path_id path, GFX_DEV *gfxdev, BLT_CONTEXT
	*context);

#ifdef __cplusplus
}
#endif

#endif /* _MFM_BLT_H */
