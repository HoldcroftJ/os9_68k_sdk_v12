/*******************************************************************************
* FILENAME : mfm_gfx.h
*
* DESCRIPTION :
*
*	This file contains definitions for the interface between the graphics device
*	API and the graphics driver. This file should not be used directly by app-
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
*       2  12/18/00  SRM  CF2120 Add extended device capabilities
*/
#ifndef _MFM_GFX_H
#define _MFM_GFX_H

#include <MAUI/maui_com.h>
#define _GFX_ID_STRUCTS_EXPOSED
#include <MAUI/maui_gfx.h>
#include <MAUI/sema4.h>

/*******************************************************************************
* GRAPHICS DEVICE OBJECT
*******************************************************************************/

#define _GFX_DEV_SYNC 0x47444556 /* "GDEV" */

struct _GFX_DEV_SHARED {
	sema4 modify_lock;			/* Modify lock */
	void *gdv_attachment;		/* Driver allocated attachment */
	u_int16 link_count;			/* Link count */

	/* Application defined parameters */
	GFX_DEV_RES *devres;		/* Device resolution */
	BOOLEAN vpmix;				/* Viewport mixing on/off */
	BOOLEAN extvid;				/* External video on/off */
	GFX_COLOR bkcol;			/* Backdrop color */
	GFX_COLOR transcol;			/* Transparent color */

	/* Fast entry points for handling changes to the graphics device. */
	/* These must be set up by the driver. */
        error_code (*set_vport_pos)(GFX_VPORT *vport, GFX_POS x, GFX_POS y);
        error_code (*set_vport_size)(GFX_VPORT *vport, GFX_DIMEN width, GFX_DIMEN height);
        error_code (*set_vport_state)(GFX_VPORT *vport, BOOLEAN active);
        error_code (*set_vport_intensity)(GFX_VPORT *vport, u_int8 intensity);
        error_code (*set_vport_dmap)(GFX_VPORT *vport, const GFX_DMAP *dmap,
        GFX_POS x, GFX_POS y);
        error_code (*set_vport_dmpos)(GFX_VPORT *vport, GFX_POS x, GFX_POS y);
        error_code (*restack_vport)(GFX_VPORT *vport, GFX_VPORT_PLACEMENT placement,
                                    GFX_VPORT *ref_vport);
        error_code (*update_display)(GFX_DEV *gfxdev, BOOLEAN sync);
        error_code (*set_vport_colors)(GFX_VPORT *vport, u_int16 start_entry,
		u_int16 num_colors, GFX_COLOR_TYPE color_type, void *colors);

	BOOLEAN visible;			/* TRUE if update display has been called */
	char device_name[GFX_MAX_DEV_NAME+1];
								/* Device name */
	char retrace_evname[11+1];	/* Vertical retrace event name */
	GFX_VPORT *vport_head;		/* Head of viewport stack */
	GFX_VPORT *vport_tail;		/* Tail of viewport stack */
	const GFX_DMAP *decode_dmap;/* Destination drawmap for video decoding */

	/* The following fields were added for MAUI2.2 (compat level 25) */
	GFX_CURSOR *cursor_head;	                /* Head of cursor list */
	error_code (*set_cursor)(GFX_DEV *gfxdev,       /* fast entrypoint to set the */
                                 GFX_CURSOR *cursor);   /* current hw cursor */
	error_code (*set_cursor_pos)(GFX_DEV *gfxdev,	/* fast entrypoint to set */
				GFX_POS x, GFX_POS y);	/* the cursor position */
	GFX_POINT cursor_pos;		                /* Cursor position */
};


typedef struct _GFX_DEV_SHARED GFX_DEV_SHARED;

struct _GFX_DEV {
	u_int32 api_compat_level;	/* API compatibility level */
	u_int32 gdv_compat_level;	/* Driver compatibility level */
	u_int32 sync_code;			/* Sync code - must be _GFX_DEV_SYNC */
	path_id usr_path;			/* User state path to device */
	event_id usr_retrace_evid;	/* User retrace event ID */
	GFX_DEV_SHARED *shared;		/* Shared logical device */
	process_id pid;				/* Process ID */
};



/*******************************************************************************
* VIEWPORT OBJECT
*******************************************************************************/

#define _GFX_VPORT_SYNC 0x56505254 /* "VPRT" */

struct _GFX_VPORT_SHARED {
	void *gdv_attachment;		/* Driver allocated attachment */
	u_int16 link_count;			/* Link count */
	GFX_DEV_SHARED *gfxdev_shared;
								/* Graphics device */
	GFX_VPORT *prev;			/* Pointer to previous viewport in stack */
	GFX_VPORT *next;			/* Pointer to next viewport in stack */
	BOOLEAN active;				/* Viewport is active if TRUE */
	const GFX_DMAP *dmap;		/* Pointer to current dmap in viewport */
	GFX_POS dmapx;				/* X coordinate (offset) in drawmap */
	GFX_POS dmapy;				/* Y coordinate (offset) in drawmap */
	GFX_POS sx;					/* Start X coordinate in display */
	GFX_POS ex;					/* End X coordinate in display */
	GFX_POS sy;					/* Start Y coordinate in display */
	GFX_POS ey;					/* End Y coordinate in display */
	GFX_DIMEN width;			/* Width of viewport in pixels */
	GFX_DIMEN height;			/* Height of viewport in lines */
	u_int8 intensity;			/* Intensity (0-100) as a percentage */
	GFX_CM coding_method;		/* Drawmap coding method */
	GFX_DIMEN dmap_width;		/* Width of drawmap in pixels */
	GFX_DIMEN dmap_height;		/* Height of drawmap in lines */
	size_t dmap_line_size;		/* Size of drawmap line in bytes */
	void *pixmem;				/* Pointer to pixel memory for drawmap */
	u_int16 dm2dp_xmul;			/* Multiplier to convert X coordinates */
	u_int16 dm2dp_ymul;			/* Multiplier to convert Y coordinates */
};

typedef struct _GFX_VPORT_SHARED GFX_VPORT_SHARED;

struct _GFX_VPORT {
	u_int32 sync_code;			/* Sync code - must be _GFX_VPORT_SYNC */
	GFX_DEV *gfxdev;			/* Graphics device this viewport belongs to */
	GFX_VPORT_SHARED *shared;	/* Shared viewport object */
	process_id pid;				/* Process ID */
};


/*******************************************************************************
* CURSOR OBJECT
*******************************************************************************/

#define _GFX_CURSOR_SYNC 0x47435552 /* 'GCUR' */

struct _GFX_CURSOR {
	u_int32 sync_code;			/* Sync code - must be _GFX_CURSOR_SYNC */
	void *gdv_cursor_attach;		/* Driver allocated attachment */
	GFX_DEV *gfxdev;			/* Graphics device */
	struct _GFX_CURSOR * next;		/* Pointer to next sibling cursor */
	struct _GFX_CURSOR * prev;		/* Pointer to prev sibling cursor */
	process_id pid;				/* Process ID */
};

/*******************************************************************************
* GETSTAT PARAMETER BLOCKS
*******************************************************************************/

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_COMPAT */
	u_int32 *ret_gdv_compat;	/* Driver compatibility level */
	u_int32 api_compat;			/* API compatibility level */
} gs_gfx_compat_pb, *Gs_gfx_compat_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_DEVCAP */
	const GFX_DEV *gfxdev;		/* Graphics device */
	const GFX_DEV_CAP **ret_dev_cap;
								/* Pointer to returned information block */
} gs_gfx_devcap_pb, *Gs_gfx_devcap_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_DEVCAPEXTEN */
	const GFX_DEV *gfxdev;		/* Graphics device */
	const GFX_DEV_CAPEXTEN **ret_dev_capexten;
								/* Pointer to returned information block */
} gs_gfx_devcapexten_pb, *Gs_gfx_devcapexten_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_CURCAP */
	const GFX_DEV *gfxdev;      /* Graphics device */
	const GFX_CURSOR_CAP **ret_cursor_cap;	/* Pointer to returned information block */
} gs_gfx_curcap_pb, *Gs_gfx_curcap_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_ATTRIBUTE */
    const GFX_DEV *gfxdev;      /* Graphics device */
    GFX_DEV_ATTR *ret_dev_attr; /* Pointer to return attribute block */
    GFX_ATTR_TYPE attr_type;    /* Attribute type specifier */
} gs_gfx_attribute_pb, *Gs_gfx_attribute_pb;

/*******************************************************************************
* SETSTAT PARAMETER BLOCKS
*******************************************************************************/

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_ALLOC */
	GFX_DEV *gfxdev;			/* Graphics device */
	size_t *size;				/* Pointer to size to allocate */
	void **mem_ptr;				/* Pointer to return value for allocation */
	u_int32 color;				/* Color to allocate from */
} ss_gfx_allocmem_pb, *Ss_gfx_allocmem_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_BKCOL */
	GFX_DEV *gfxdev;			/* Graphics device */
	const GFX_COLOR *bkcol;		/* Backdrop color */
} ss_gfx_bkcol_pb, *Ss_gfx_bkcol_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_CLONEDEV */
	GFX_DEV **ret_gfxdev;		/* Return pointer to new GFX_DEV */
	GFX_DEV *gfxdev;			/* Device to be cloned */
} ss_gfx_clonedev_pb, *Ss_gfx_clonedev_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_CLONEVP */
	GFX_VPORT **ret_vport;		/* Return pointer to new GFX_VPORT */
	GFX_VPORT *vport;			/* Viewport to be cloned */
} ss_gfx_clonevp_pb, *Ss_gfx_clonevp_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_CLOSEDEV */
	GFX_DEV *gfxdev;			/* Pointer to device structure */
} ss_gfx_closedev_pb, *Ss_gfx_closedev_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_CREATEVP */
	GFX_DEV *gfxdev;			/* Graphics device */
	GFX_VPORT **ret_vport;		/* Return pointer to viewport object */
	GFX_POS x, y;				/* Viewport position */
	GFX_DIMEN width;
    GFX_DIMEN height;	/* Viewport size */
	GFX_VPORT_PLACEMENT placement;	/* New position for viewport */
	GFX_VPORT *ref_vport;		/* Reference viewport */
} ss_gfx_createvp_pb, *Ss_gfx_createvp_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_DEALLOC */
	GFX_DEV *gfxdev;			/* Graphics device */
	size_t size;				/* Size originally allocated */
	void *mem_ptr;				/* Memory to deallocated */
	u_int32 color;				/* Color originally allocated from */
} ss_gfx_deallocmem_pb, *Ss_gfx_deallocmem_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_BKCOL */
	GFX_DEV *gfxdev;			/* Graphics device */
	const GFX_DMAP *decode_dmap;/* Destination drawmap for video decoding */
} ss_gfx_decodedst_pb, *Ss_gfx_decodedst_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_DESTROYVP */
	GFX_DEV *gfxdev;			/* Graphics device */
	GFX_VPORT *vport;			/* Pointer to viewport object */
} ss_gfx_destroyvp_pb, *Ss_gfx_destroyvp_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_DEVRES */
	GFX_DEV *gfxdev;			/* Graphics device */
	GFX_DIMEN disp_width;		/* Display width */
	GFX_DIMEN disp_height;		/* Display height */
	GFX_INTL_MODE intl_mode;	/* Interlace mode */
	u_int16 refresh_rate;		/* Refresh rate */
} ss_gfx_devres_pb, *Ss_gfx_devres_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_EXTVID */
	GFX_DEV *gfxdev;			/* Graphics device */
	BOOLEAN extvid;				/* External video on/off */
} ss_gfx_extvid_pb, *Ss_gfx_extvid_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_OPENDEV */
	GFX_DEV **ret_gfxdev;		/* Return pointer to device structure */
} ss_gfx_opendev_pb, *Ss_gfx_opendev_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_RESTACKDEV */
	GFX_DEV *gfxdev;			/* Graphics device */
	GFX_DEV_PLACEMENT placement;
								/* New position for device */
	GFX_DEV *ref_gfxdev;		/* Reference device */
} ss_gfx_restackdev_pb, *Ss_gfx_restackdev_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_TRANSCOL */
	GFX_DEV *gfxdev;			/* Graphics device */
	const GFX_COLOR *transcol;	/* Transparent color */
} ss_gfx_transcol_pb, *Ss_gfx_transcol_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_VPMIX */
	GFX_DEV *gfxdev;			/* Graphics device */
	BOOLEAN vpmix;				/* Viewport mixing on/off */
} ss_gfx_vpmix_pb, *Ss_gfx_vpmix_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_CURCREATE */
	GFX_DEV *gfxdev;			/* Graphics device */
	GFX_CURSOR **cursor;        /* Return pointer to cursor object */
	const GFX_CURSOR_SPEC *cursor_spec;	/* User's cursor definition */
} ss_gfx_curcreate_pb, *Ss_gfx_curcreate_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_CURDESTROY */
	GFX_CURSOR *cursor;			/* Cursor to destroy */
} ss_gfx_curdestroy_pb, *Ss_gfx_curdestroy_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_ATTRIBUTE */
    const GFX_DEV *gfxdev;      /* Graphics device */
    GFX_ATTR_TYPE attr_type;    /* Attribute type specifier */
    GFX_ATTR_MODE mode;         /* Absolute or relative */
    int32 value;                /* Value to set attribute */
} ss_gfx_attribute_pb, *Ss_gfx_attribute_pb;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code _fe_cursor_set(GFX_DEV *gfxdev, GFX_CURSOR *cursor);
error_code _fe_cursor_set_pos(GFX_DEV *gfxdev, GFX_POS x, GFX_POS y);
error_code _fe_restack_vp(GFX_VPORT *vport, GFX_VPORT_PLACEMENT placement, GFX_VPORT *ref_vport);
error_code _fe_update_dpy(GFX_DEV *gfxdev, BOOLEAN sync);
error_code _fe_set_vpcolors(GFX_VPORT *vport, u_int16 start_entry, u_int16 num_colors, GFX_COLOR_TYPE color_type, void *colors);
error_code _fe_set_vpdmap(GFX_VPORT *vport, const GFX_DMAP *dmap, GFX_POS x, GFX_POS y);
error_code _fe_set_vpdmpos(GFX_VPORT *vport, GFX_POS x, GFX_POS y);
error_code _fe_set_vpinten(GFX_VPORT *vport, u_int8 intensity);
error_code _fe_set_vppos(GFX_VPORT *vport, GFX_POS x, GFX_POS y);
error_code _fe_set_vpsize(GFX_VPORT *vport, GFX_DIMEN width, GFX_DIMEN height);
error_code _fe_set_vpstate(GFX_VPORT *vport, BOOLEAN active);
error_code _os_gs_gfx_attribute(path_id path, GFX_DEV *gfxdev, GFX_DEV_ATTR *ret_dev_attr, GFX_ATTR_TYPE attr_type);
error_code _os_gs_gfx_compat(path_id path, u_int32 *ret_gdv_compat, u_int32 api_compat);
error_code _os_gs_gfx_cursor_cap(path_id path, GFX_DEV *gfxdev, const GFX_CURSOR_CAP **ret_cursor_cap);
error_code _os_gs_gfx_devcap(path_id path, const GFX_DEV *gfxdev, const GFX_DEV_CAP **ret_dev_cap);
error_code _os_gs_gfx_devcapexten(path_id path, const GFX_DEV *gfxdev, const GFX_DEV_CAPEXTEN **ret_dev_capexten);
error_code _os_ss_gfx_allocmem(path_id path, GFX_DEV *gfxdev, size_t *size, void **mem_ptr, u_int32 color);
error_code _os_ss_gfx_bkcol(path_id path, GFX_DEV *gfxdev, const GFX_COLOR *bkcol);
error_code _os_ss_gfx_attribute(path_id path, GFX_DEV *gfxdev, GFX_ATTR_TYPE attr_type, GFX_ATTR_MODE mode, int32 value);
error_code _os_ss_gfx_clonedev(path_id path, GFX_DEV **ret_gfxdev, GFX_DEV *gfxdev);
error_code _os_ss_gfx_clonevp(path_id path, GFX_VPORT **ret_vport, GFX_VPORT *vport);
error_code _os_ss_gfx_closedev(path_id path, GFX_DEV *gfxdev);
error_code _os_ss_gfx_createvp(path_id path, GFX_DEV *gfxdev, GFX_VPORT **ret_vport, GFX_POS x, GFX_POS y, GFX_DIMEN width, GFX_DIMEN height, GFX_VPORT_PLACEMENT placement, GFX_VPORT *ref_vport);
error_code _os_ss_gfx_cursor_create(path_id path, GFX_DEV *gfxdev, GFX_CURSOR **cursor, const GFX_CURSOR_SPEC *cursor_spec);
error_code _os_ss_gfx_cursor_destroy(path_id path, GFX_CURSOR *cursor);
error_code _os_ss_gfx_deallocmem(path_id path, GFX_DEV *gfxdev, size_t size, void *mem_ptr, u_int32 color);
error_code _os_ss_gfx_decodedst(path_id path, GFX_DEV *gfxdev, const GFX_DMAP *decode_dmap);
error_code _os_ss_gfx_destroyvp(path_id path, GFX_DEV *gfxdev, GFX_VPORT *vport);
error_code _os_ss_gfx_devres(path_id path, GFX_DEV *gfxdev, GFX_DIMEN disp_width, GFX_DIMEN disp_height, GFX_INTL_MODE intl_mode, u_int16 refresh_rate);
error_code _os_ss_gfx_extvid(path_id path, GFX_DEV *gfxdev, BOOLEAN extvid);
error_code _os_ss_gfx_opendev(path_id path, GFX_DEV **ret_gfxdev);
error_code _os_ss_gfx_restackdev(path_id path, GFX_DEV *gfxdev, GFX_DEV_PLACEMENT placement, GFX_DEV *ref_gfxdev);
error_code _os_ss_gfx_transcol(path_id path, GFX_DEV *gfxdev, const GFX_COLOR *transcol);
error_code _os_ss_gfx_vpmix(path_id path, GFX_DEV *gfxdev, BOOLEAN vpmix);

#ifdef __cplusplus
}
#endif

#endif /* _MFM_GFX_H */
