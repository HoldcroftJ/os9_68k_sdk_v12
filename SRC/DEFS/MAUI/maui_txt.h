/*******************************************************************************
* FILENAME : maui_txt.h
*
* DESCRIPTION :
*
*	This file contains definitions for applications that use the MAUI Text
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
*       1  11/15/94  TJE  First version
*/
#ifndef _MAUI_TXT_H
#define _MAUI_TXT_H

#include <stddef.h>
#include <types.h>
#include <const.h>
#include <MAUI/maui_com.h>
#include <MAUI/maui_mem.h>
#include <MAUI/maui_gfx.h>
#include <MAUI/maui_blt.h>

/*******************************************************************************
* DEFINE ID TYPES
*******************************************************************************/

#ifdef _TXT_ID_STRUCTS_EXPOSED
typedef struct _TXT_CONTEXT TXT_CONTEXT;
#define TXT_CONTEXT_ID TXT_CONTEXT *
#else
typedef void * TXT_CONTEXT_ID;
#endif

/*******************************************************************************
* FONT OBJECT
*******************************************************************************/

typedef enum {
	TXT_FONTYPE_MONO,			/* Mono-spaced font */
	TXT_FONTYPE_PROP			/* Proportional spaced font */
} TXT_FONTYPE;

typedef struct _TXT_RANGTBL {
	wchar_t first_char;			/* First character in font */
	wchar_t last_char;			/* Last character in font */
	GFX_OFFSET *offset_tbl;		/* Pointer to offset table */
	GFX_DIMEN *width_tbl;		/* Pointer to width table */
	GFX_DMAP *bitmap;			/* Pointer to bitmap */
} TXT_RANGTBL;

typedef struct _TXT_FONT {
	TXT_FONTYPE font_type;		/* Font type (mono or proportional) */
	GFX_DIMEN width;			/* Maximum cell width */
	GFX_DIMEN height;			/* Cell height */
	GFX_DIMEN ascent;			/* Ascent above baseline */
	GFX_DIMEN descent;			/* Descent below baseline */
	wchar_t default_char;		/* Default character */
	u_int8 num_ranges;			/* Number of ranges */
	TXT_RANGTBL *range_tbl;		/* Pointer to range table */
} TXT_FONT;

#define TXT_NOGLYPH INT_MAX		/* Marks a glyph unused in the offset table */

/*******************************************************************************
* TEXT CONTEXT DEFINITION
*******************************************************************************/
 
typedef struct _TXT_CONTEXT_PARAMS {
	GFX_DEV_ID gfxdev;			/* Graphics device ID */
	const TXT_FONT *font;		/* Current font */
	int16 cpad;					/* Additional padding between characters */
	BLT_MIX mixmode;			/* Mixing mode for Bit-BLT operations */
	u_int8 exptbl_entries;		/* Number of entries in exptbl */
	const GFX_PIXEL *exptbl;	/* Pixel expansion table */
	GFX_PIXEL transpixel;		/* Transparent pixel (1 pixel only) */
	GFX_PIXEL ofspixel;			/* Offset pixel value (1 pixel only) */
	const GFX_DMAP *dstdmap;	/* Destination drawmap (NULL if none) */
	GFX_POINT origin;			/* Drawing origin */
	GFX_RECT draw_area;			/* Drawing area */
	u_int32 num_clip_areas;		/* Number of clipping areas */
	const GFX_RECT *clip_areas;	/* Array of clipping areas */
} TXT_CONTEXT_PARAMS;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code txt_create_context(TXT_CONTEXT_ID *ret_context, GFX_DEV_ID gfxdev);
error_code txt_create_font(TXT_FONT **ret_font, u_int32 shade, u_int8
	num_ranges);
error_code txt_destroy_context(TXT_CONTEXT_ID context);
error_code txt_destroy_font(TXT_FONT *font);
error_code txt_draw_mbs(GFX_DIMEN *ret_width, TXT_CONTEXT_ID context, const
	char *string, size_t *char_len, GFX_POS dstx, GFX_POS dsty, const int16
	pad_array[]);
error_code txt_draw_wcs(GFX_DIMEN *ret_width, TXT_CONTEXT_ID context, const
	wchar_t *string, size_t *char_len, GFX_POS dstx, GFX_POS dsty, const int16
	pad_array[]);
error_code txt_get_context(TXT_CONTEXT_PARAMS *ret_context_params,
	TXT_CONTEXT_ID context);
error_code txt_get_mbs_width(GFX_DIMEN *ret_swidth, GFX_DIMEN ret_cwidth[],
	TXT_CONTEXT_ID context, const char *string, size_t *max_chars);
error_code txt_get_wcs_width(GFX_DIMEN *ret_swidth, GFX_DIMEN ret_cwidth[],
	TXT_CONTEXT_ID context, const wchar_t *string, size_t *max_chars);
error_code txt_init(void);
error_code txt_set_context_clip(TXT_CONTEXT_ID context, u_int32 num_clip_areas,
	const GFX_RECT clip_areas[]);
error_code txt_set_context_cpad(TXT_CONTEXT_ID context, int16 cpad);
error_code txt_set_context_draw(TXT_CONTEXT_ID context, GFX_POS x, GFX_POS y,
	GFX_DIMEN width, GFX_DIMEN height);
error_code txt_set_context_dst(TXT_CONTEXT_ID context, const GFX_DMAP *dstdmap);
error_code txt_set_context_exptbl(TXT_CONTEXT_ID context, u_int8 num_entries,
	const GFX_PIXEL exptbl[]);
error_code txt_set_context_font(TXT_CONTEXT_ID context, const TXT_FONT *font);
error_code txt_set_context_mix(TXT_CONTEXT_ID context, BLT_MIX mixmode);
error_code txt_set_context_ofs(TXT_CONTEXT_ID context, GFX_PIXEL ofspixel);
error_code txt_set_context_origin(TXT_CONTEXT_ID context, GFX_POS x, GFX_POS y);
error_code txt_set_context_trans(TXT_CONTEXT_ID context, GFX_PIXEL transpixel);
error_code txt_set_error_action(MAUI_ERR_LEVEL debug_level, MAUI_ERR_LEVEL
    passback_level, MAUI_ERR_LEVEL exit_level);
error_code txt_term(void);

#ifdef __cplusplus
}
#endif

#endif /* _MAUI_TXT_H */
