/*******************************************************************************
* FILENAME : maui_blt.h
*
* DESCRIPTION :
*
*	This file contains definitions for applications that use the MAUI Bit-BLT
*	API.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  10/13/94  TJE  First version
*/
#ifndef _MAUI_BLT_H
#define _MAUI_BLT_H

#include <stddef.h>
#include <types.h>
#include <const.h>
#include <MAUI/maui_com.h>
#include <MAUI/maui_mem.h>
#include <MAUI/maui_gfx.h>

/*******************************************************************************
* DEFINE ID TYPES
*******************************************************************************/

#ifdef _BLT_ID_STRUCTS_EXPOSED
typedef struct _BLT_CONTEXT BLT_CONTEXT;
#define BLT_CONTEXT_ID BLT_CONTEXT *
#else
typedef void * BLT_CONTEXT_ID;
#endif

/******************************************************************************
 * Constatn definitions
 ******************************************************************************/
#define BLT_EXPTBL_MAX_SIZE 2 /* We can support only expansion from 1 bpp */

/*******************************************************************************
* BIT-BLT CONTEXT DEFINITION
*******************************************************************************/

typedef enum {
	BLT_MIX_REPLACE,			/* Source */
	BLT_MIX_SXD,				/* Source XOR destination */
	BLT_MIX_N_SXD,				/* NOT (source XOR destination) */
	BLT_MIX_SOD,				/* Source OR destination */
	BLT_MIX_N_SOD,				/* NOT (source OR destination) */
	BLT_MIX_NS_AD,				/* (NOT source) AND destination */
	BLT_MIX_SO_ND,				/* Source OF (NOT destination) */
	BLT_MIX_SPD,				/* Source plus (arithmetic) destination */
	BLT_MIX_DMS,				/* Destination minus (arithmetic) source */
	BLT_MIX_SPO,				/* Source plus offset */
	BLT_MIX_RWT,				/* Replace with transparency checking */
	BLT_MIX_RWM					/* Replace with transparency mask checking */
} BLT_MIX;

typedef struct _BLT_CONTEXT_PARAMS {
	GFX_DEV_ID gfxdev;			/* Graphics device ID */
	BLT_MIX drwmix;				/* Mixing mode for draw operations */
	BLT_MIX cpymix;				/* Mixing mode for copy operations */
	BLT_MIX expmix;				/* Mixing mode for expand operations */
	GFX_PIXEL drwpixel;			/* Pixel value for drawing (1 pixel only) */
	const GFX_DMAP *srcdmap;	/* Source drawmap (NULL if none) */
	u_int8 exptbl_entries;		/* Number of entries in exptbl */
	const GFX_PIXEL *exptbl;	/* Pixel expansion table */
	GFX_PIXEL transpixel;		/* Transparent pixel (1 pixel only) */
	const GFX_DMAP *mask_dmap;	/* Mask drawmap (NULL if none) */
	GFX_PIXEL ofspixel;			/* Offset pixel value (1 pixel only) */
	const GFX_DMAP *dstdmap;	/* Destination drawmap (NULL if none) */
} BLT_CONTEXT_PARAMS;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code blt_copy_block(BLT_CONTEXT_ID context, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
error_code blt_copy_next_block(BLT_CONTEXT_ID context, GFX_POS dstx, GFX_POS
	srcx, GFX_DIMEN height);
error_code blt_copy_oblock(BLT_CONTEXT_ID context, GFX_POS dstx, GFX_POS dsty,
    GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
error_code blt_create_context(BLT_CONTEXT_ID *ret_context, GFX_DEV_ID gfxdev);
error_code blt_destroy_context(BLT_CONTEXT_ID context);
error_code blt_draw_block(BLT_CONTEXT_ID context, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN width, GFX_DIMEN height);
error_code blt_draw_hline(BLT_CONTEXT_ID context, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN width);
error_code blt_draw_pixel(BLT_CONTEXT_ID context, GFX_POS dstx, GFX_POS dsty);
error_code blt_draw_vline(BLT_CONTEXT_ID context, GFX_POS dstx, GFX_POS
	dsty, GFX_DIMEN height);
error_code blt_expd_block(BLT_CONTEXT_ID context, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
error_code blt_expd_next_block(BLT_CONTEXT_ID context, GFX_POS dstx, GFX_POS
	srcx, GFX_DIMEN height);
error_code blt_get_context(BLT_CONTEXT_PARAMS *ret_context_params,
	BLT_CONTEXT_ID context);
error_code blt_get_pixel(GFX_PIXEL *ret_pixel, BLT_CONTEXT_ID context, GFX_POS
	srcx, GFX_POS srcy);
error_code blt_init(void);
error_code blt_set_context_cpymix(BLT_CONTEXT_ID context, BLT_MIX cpymix);
error_code blt_set_context_drwmix(BLT_CONTEXT_ID context, BLT_MIX drwmix);
error_code blt_set_context_dst(BLT_CONTEXT_ID context, const GFX_DMAP
	*dstdmap);
error_code blt_set_context_expmix(BLT_CONTEXT_ID context, BLT_MIX expmix);
error_code blt_set_context_exptbl(BLT_CONTEXT_ID context, u_int8 num_entries,
	const GFX_PIXEL exptbl[]);
error_code blt_set_context_mask(BLT_CONTEXT_ID context, const GFX_DMAP
	*mask_dmap);
error_code blt_set_context_ofs(BLT_CONTEXT_ID context, GFX_PIXEL ofspixel);
error_code blt_set_context_pix(BLT_CONTEXT_ID context, GFX_PIXEL drwpixel);
error_code blt_set_context_src(BLT_CONTEXT_ID context, const GFX_DMAP
	*srcdmap);
error_code blt_set_context_trans(BLT_CONTEXT_ID context, GFX_PIXEL
	transpixel);
error_code blt_set_error_action(MAUI_ERR_LEVEL debug_level, MAUI_ERR_LEVEL
    passback_level, MAUI_ERR_LEVEL exit_level);
error_code blt_term(void);

#ifdef __cplusplus
}
#endif

#endif /* _MAUI_BLT_H */
