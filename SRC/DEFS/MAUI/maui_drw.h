/*******************************************************************************
* FILENAME : maui_drw.h
*
* DESCRIPTION :
*
*	This file contains definitions for applications that use the MAUI Drawing
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
*       1  02/24/95  TJE  First version
*/
#ifndef _MAUI_DRW_H
#define _MAUI_DRW_H

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

#ifdef _DRW_ID_STRUCTS_EXPOSED
typedef struct _DRW_CONTEXT DRW_CONTEXT;
#define DRW_CONTEXT_ID DRW_CONTEXT *
#else
typedef void * DRW_CONTEXT_ID;
#endif

/*******************************************************************************
* DRAWING CONTEXT DEFINITION
*******************************************************************************/

typedef enum {
	DRW_FM_OUTLINE,				/* Draw the outline of the shape */
	DRW_FM_SOLID				/* Draw a solid shape */
} DRW_FM;

typedef enum {
	DRW_LS_SOLID,				/* Draw a solid outline */
	DRW_LS_DASHED				/* Draw a dashed outline */
} DRW_LS;

typedef struct _DRW_CONTEXT_PARAMS {
	GFX_DEV_ID gfxdev;			/* Graphics device ID */
	DRW_FM fill_mode;			/* Fill mode */
	DRW_LS line_style;			/* Line style */
	u_int32 dash_pattern;		/* Dash pattern */
	u_int16 dash_magnify;		/* Magnification for dash pattern */
	BLT_MIX mixmode;			/* Mixing mode for drawing operations */
	GFX_PIXEL drwpixel;			/* Drawing pixel value (1 pixel only) */
	GFX_PIXEL transpixel;		/* Transparent pixel (1 pixel only) */
	GFX_PIXEL ofspixel;			/* Offset pixel value (1 pixel only) */
	const GFX_DMAP *dstdmap;	/* Destination drawmap (NULL if none) */
	GFX_POINT origin;			/* Drawing origin */
	GFX_RECT draw_area;			/* Drawing area */
	u_int32 num_clip_areas;		/* Number of clipping areas */
	const GFX_RECT *clip_areas;	/* Array of clipping areas */
	BLT_MIX cpymix;				/* Mixing mode for copy operations */
	BLT_MIX expmix;				/* Mixing mode for expand operations */
	const GFX_DMAP *srcdmap;	/* Source drawmap (NULL if none) */
	u_int8 exptbl_entries;		/* Number of entries in exptbl */
	const GFX_PIXEL *exptbl;	/* Pixel expansion table */
	const GFX_DMAP *mask_dmap;	/* Mask drawmap (NULL if none) */
} DRW_CONTEXT_PARAMS;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code drw_arc(DRW_CONTEXT_ID context, GFX_POS x, GFX_POS y, GFX_ANGLE
	start_angle, GFX_ANGLE end_angle, GFX_DIMEN radius);
error_code drw_circle(DRW_CONTEXT_ID context, GFX_POS x, GFX_POS y, GFX_DIMEN
	radius);
error_code drw_copy_block(DRW_CONTEXT_ID context, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
error_code drw_copy_oblock(DRW_CONTEXT_ID context, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
error_code drw_create_context(DRW_CONTEXT_ID *ret_context, GFX_DEV_ID gfxdev);
error_code drw_destroy_context(DRW_CONTEXT_ID context);
error_code drw_earc(DRW_CONTEXT_ID context, GFX_POS x, GFX_POS y, GFX_ANGLE
	start_angle, GFX_ANGLE end_angle, GFX_DIMEN xradius, GFX_DIMEN yradius);
error_code drw_ellipse(DRW_CONTEXT_ID context, GFX_POS x, GFX_POS y, GFX_DIMEN
	xradius, GFX_DIMEN yradius);
error_code drw_expd_block(DRW_CONTEXT_ID context, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
error_code drw_get_context(DRW_CONTEXT_PARAMS *ret_context_params,
	DRW_CONTEXT_ID context);
error_code drw_line(DRW_CONTEXT_ID context, GFX_POS sx, GFX_POS sy, GFX_POS ex,
	GFX_POS ey);
error_code drw_init(void);
error_code drw_point(DRW_CONTEXT_ID context, GFX_POS x, GFX_POS y);
error_code drw_polygon(DRW_CONTEXT_ID context, u_int32 num_verticies, const
	GFX_POINT vertex[]);
error_code drw_polyline(DRW_CONTEXT_ID context, u_int32 num_verticies, const
	GFX_POINT vertex[]);
error_code drw_rectangle(DRW_CONTEXT_ID context, GFX_POS x, GFX_POS y, GFX_DIMEN
	width, GFX_DIMEN height);
error_code drw_set_context_cpymix(DRW_CONTEXT_ID context, BLT_MIX cpymix);
error_code drw_set_context_clip(DRW_CONTEXT_ID context, u_int32 num_clip_areas,
	const GFX_RECT clip_areas[]);
error_code drw_set_context_dash(DRW_CONTEXT_ID context, u_int32 dash_pattern,
	u_int16 dash_magnify);
error_code drw_set_context_draw(DRW_CONTEXT_ID context, GFX_POS x, GFX_POS y,
	GFX_DIMEN width, GFX_DIMEN height);
error_code drw_set_context_dst(DRW_CONTEXT_ID context, const GFX_DMAP
	*dstdmap);
error_code drw_set_context_expmix(DRW_CONTEXT_ID context, BLT_MIX expmix);
error_code drw_set_context_exptbl(DRW_CONTEXT_ID context, u_int8 num_entries,
	const GFX_PIXEL exptbl[]);
error_code drw_set_context_fm(DRW_CONTEXT_ID context, DRW_FM fill_mode);
error_code drw_set_context_ls(DRW_CONTEXT_ID context, DRW_LS line_style);
error_code drw_set_context_mask(DRW_CONTEXT_ID context, const GFX_DMAP
	*mask_dmap);
error_code drw_set_context_mix(DRW_CONTEXT_ID context, BLT_MIX mixmode);
error_code drw_set_context_ofs(DRW_CONTEXT_ID context, GFX_PIXEL ofspixel);
error_code drw_set_context_origin(DRW_CONTEXT_ID context, GFX_POS x, GFX_POS y);
error_code drw_oval(DRW_CONTEXT_ID context, GFX_POS x, GFX_POS y, 
					GFX_DIMEN width, GFX_DIMEN height);
error_code drw_oval_arc(DRW_CONTEXT_ID context, GFX_POS x, GFX_POS y, 
						GFX_ANGLE start_angle, GFX_ANGLE end_angle, 
						GFX_DIMEN width, GFX_DIMEN height);
error_code drw_set_context_pix(DRW_CONTEXT_ID context, GFX_PIXEL drwpixel);
error_code drw_set_context_src(DRW_CONTEXT_ID context, const GFX_DMAP
	*srcdmap);
error_code drw_set_context_trans(DRW_CONTEXT_ID context, GFX_PIXEL
	transpixel);
error_code drw_set_error_action(MAUI_ERR_LEVEL debug_level, MAUI_ERR_LEVEL
    passback_level, MAUI_ERR_LEVEL exit_level);
error_code drw_term(void);

#ifdef __cplusplus
}
#endif

#endif /* _MAUI_DRW_H */
