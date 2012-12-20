/*******************************************************************************
* FILENAME : maui_anm.h
*
* DESCRIPTION :
*
*	This file contains definitions for applications that use the MAUI animation
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
*       1  02/07/95  TJE  First version
*/
#ifndef _MAUI_ANM_H
#define _MAUI_ANM_H

#include <stddef.h>
#include <types.h>
#include <const.h>
#include <limits.h>
#include <MAUI/maui_com.h>
#include <MAUI/maui_mem.h>
#include <MAUI/maui_gfx.h>
#include <MAUI/maui_blt.h>

/*******************************************************************************
* DEFINE ID TYPES
*******************************************************************************/

#ifdef _ANM_ID_STRUCTS_EXPOSED
typedef struct _ANM_OBJECT ANM_OBJECT;
#define ANM_OBJECT_ID ANM_OBJECT *
typedef struct _ANM_GROUP ANM_GROUP;
#define ANM_GROUP_ID ANM_GROUP *
#else
typedef void * ANM_OBJECT_ID;
typedef void * ANM_GROUP_ID;
#endif

/*******************************************************************************
* SPRITE OBJECT
*******************************************************************************/

typedef struct _ANM_FRAME {
	GFX_RECT src_area;			/* Area of source drawmap that defines frame */
	GFX_RECT bound_area;		/* Bounding area relative to "src_area" */
	GFX_POINT hit_point;		/* Hit point relative to "src_area" */
	void *user_data;			/* User defined data */
} ANM_FRAME;

typedef struct _ANM_SPRITE {
	GFX_DMAP *srcdmap;			/* Source drawmap */
	GFX_PIXEL trans_pixel;		/* Transparent pixel value */
	GFX_DMAP *maskdmap;			/* Transparency mask drawmap */
	u_int16 num_frames;			/* Number of frames */
	ANM_FRAME *frames;			/* Pointer to array of "num_frames" frames */
	void *user_data;			/* User defined data */
} ANM_SPRITE;

/*******************************************************************************
* ANIMATION OBJECT DEFINITIONS
*******************************************************************************/

typedef enum {
	ANM_OBJECT_FRONT,			/* In front */
	ANM_OBJECT_BACK,			/* In back */
	ANM_OBJECT_FRONT_OF,		/* In front of the specified object */
	ANM_OBJECT_BACK_OF			/* In back of the specified object */
} ANM_OBJECT_PLACEMENT;

typedef enum {
	ANM_METH_DRAW,				/* Draw the object */
	ANM_METH_TDRAW,				/* Draw the object with transparency */
	ANM_METH_DRAW_BKG,			/* Draw the object over a background */
	ANM_METH_TDRAW_BKG			/* Draw the object with transparency */
								/* over a background */
} ANM_METHOD;

#define _ANM_BHV_PROTO ANM_OBJECT_ID object, const ANM_OBJECT_PARAMS *params
typedef struct _ANM_OBJECT_PARAMS ANM_OBJECT_PARAMS;
struct _ANM_OBJECT_PARAMS {
	BOOLEAN active;				/* Active if TRUE, inactive if FALSE */
	const ANM_SPRITE *sprite;	/* Pointer to current sprite */
	u_int16 frame;				/* Current frame */
	GFX_POINT position;			/* Current position */
	error_code (*bhv_func)(_ANM_BHV_PROTO);
								/* Behavior function */
	void *bhv_param;			/* Parameter for behavior function */
	ANM_METHOD method;			/* Draw method */
};

/*******************************************************************************
* ANIMATION GROUP PARAMETERS
*******************************************************************************/

typedef struct _ANM_GROUP_PARAMS {
	GFX_DEV_ID gfxdev;			/* Graphics device ID */
	GFX_VPORT_ID vport;			/* Destination viewport */
	const GFX_DMAP *dstdmap;	/* Destination drawmap */
	GFX_POS dstx;				/* X coordinate (offset) in dstdmap */
	GFX_POS dsty;				/* Y coordinate (offset) in dstdmap */
	BOOLEAN dbl_buff;			/* Double buffered if TRUE */
	const GFX_DMAP *bkgdmap;	/* Background drawmap */
	GFX_PIXEL bkgpixel;			/* Background pixel value */
} ANM_GROUP_PARAMS;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code anm_create_group(ANM_GROUP_ID *ret_group, GFX_DEV_ID gfxdev);
error_code anm_create_object(ANM_OBJECT_ID *ret_object, ANM_GROUP_ID group,
	ANM_OBJECT_PLACEMENT placement, ...);
error_code anm_create_sprite(ANM_SPRITE **ret_sprite, u_int16 num_frames,
	u_int32 shade);
error_code anm_destroy_group(ANM_GROUP_ID group);
error_code anm_destroy_object(ANM_OBJECT_ID object);
error_code anm_destroy_sprite(ANM_SPRITE *sprite);
error_code anm_draw_group(ANM_GROUP_ID group);
error_code anm_get_group(ANM_GROUP_PARAMS *ret_group_params, ANM_GROUP_ID
	group);
error_code anm_get_object(ANM_OBJECT_PARAMS *ret_object_params,
	ANM_OBJECT_ID object);
error_code anm_init(void);
error_code anm_process_group(ANM_GROUP_ID group);
error_code anm_restack_object(ANM_OBJECT_ID object, ANM_OBJECT_PLACEMENT
	placement, ...);
error_code anm_set_error_action(MAUI_ERR_LEVEL debug_level, MAUI_ERR_LEVEL
    passback_level, MAUI_ERR_LEVEL exit_level);
error_code anm_set_group_bkg(ANM_GROUP_ID group, const GFX_DMAP *bkgdmap,
	GFX_PIXEL bkgpixel);
error_code anm_set_group_dst(ANM_GROUP_ID group, GFX_VPORT_ID vport, const
	GFX_DMAP *dstdmap, GFX_POS x, GFX_POS y, BOOLEAN dbl_buff);
error_code anm_set_object_bhv(ANM_OBJECT_ID object, error_code (*bhv_func)
	(_ANM_BHV_PROTO), void *bhv_param);
error_code anm_set_object_frame(ANM_OBJECT_ID object, u_int16 frame);
error_code anm_set_object_meth(ANM_OBJECT_ID object, ANM_METHOD method);
error_code anm_set_object_pos(ANM_OBJECT_ID object, GFX_POS x, GFX_POS y);
error_code anm_set_object_sprite(ANM_OBJECT_ID object, const ANM_SPRITE
	*sprite);
error_code anm_set_object_state(ANM_OBJECT_ID object, BOOLEAN active);
error_code anm_term(void);

#ifdef __cplusplus
}
#endif

#endif /* _MAUI_ANM_H */
