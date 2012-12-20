/*******************************************************************************
* FILENAME : maui_win.h
*
* DESCRIPTION :
*
*   This file contains definitions for applications that use the MAUI
*   windowing API.
*
* COPYRIGHT:
*
*   This source code is the proprietary confidential property of Microware
*   Systems Corporation, and is provided to licensee solely for documentation
*   and educational purposes. Reproduction, publication, or distribution in
*   form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  05/02/96  LJG  First version
*       2  06/28/96  MAS  Modified to reflect design changes
*       3  07/17/96  TJE  Took over responsibility for this file
*/
#ifndef _MAUI_WIN_H
#define _MAUI_WIN_H


#include <stddef.h>
#include <types.h>
#include <const.h>
#include <MAUI/maui_com.h>
#include <MAUI/maui_gfx.h>
#include <MAUI/maui_msg.h>
#include <MAUI/maui_inp.h>
#include <MAUI/maui_drw.h>
#include <MAUI/maui_txt.h>

/*******************************************************************************
* ID TYPES
*******************************************************************************/

#ifdef _WIN_ID_STRUCTS_EXPOSED
typedef struct _WIN WIN;
#define WIN_ID WIN *
typedef struct _WIN_DEV WIN_DEV;
#define WIN_DEV_ID WIN_DEV *
typedef struct _WIN_CMAP WIN_CMAP;
#define WIN_CMAP_ID WIN_CMAP *
#else
typedef void * WIN_ID;
typedef void * WIN_DEV_ID;
typedef void * WIN_CMAP_ID;
#endif

/*******************************************************************************
* DEFINED CONSTANTS
*******************************************************************************/

#define WIN_MAX_DEV_NAME (MSG_MAX_MBOX_NAME+1)	/* Maximum length of device */
												/* name. We always trim 1 */
												/* byte off to make the mbox */
												/* name. */

/*******************************************************************************
* WINDOW STACK PLACEMENT
*******************************************************************************/

typedef enum {
	WIN_FRONT,					/* In front of all siblings */
	WIN_BACK,					/* In back of all siblings */
	WIN_FRONT_OF,				/* In front of another sibling */
	WIN_BACK_OF					/* In back of another sibling */
} WIN_PLACEMENT;


/*******************************************************************************
* WINDOW DEVICE STACK PLACEMENT
*******************************************************************************/

typedef enum {
	WIN_DEV_FRONT,					/* In front of all devices */
	WIN_DEV_BACK					/* In back of all devices */
} WIN_DEV_PLACEMENT;



/*******************************************************************************
* INKING METHOD
*******************************************************************************/

typedef enum {
	WIN_INK_OFF,				/* No inking method */
	WIN_INK_REPLACE				/* Replace method */
} WIN_INK_METHOD;

/*******************************************************************************
* CURSOR
*******************************************************************************/

typedef struct _WIN_CURSOR {
	GFX_POINT hit_point;		/* Hit point */
	GFX_DMAP *bitmap;			/* Cursor bitmap */
	GFX_DMAP *mask;				/* Source mask for Bit-BLT */
	GFX_PIXEL trans_pixel;		/* Transparent pixel value */
} WIN_CURSOR;

/*******************************************************************************
* WINDOW MESSAGE TYPES
*******************************************************************************/

typedef enum {
	WIN_MSG_BUTTON_DOWN,		/* Pointer button pressed */
	WIN_MSG_BUTTON_UP,			/* Pointer button released */
	WIN_MSG_BORDER_ENTER,		/* Pointer entered window */
	WIN_MSG_BORDER_LEAVE,		/* Pointer left window */
	WIN_MSG_CREATE,				/* Child window was created */
	WIN_MSG_DESTROY,			/* Window was destroyed */
	WIN_MSG_EXPOSE,				/* Part of window exposed */
	WIN_MSG_FOCUS_IN,			/* Received keyboard focus */
	WIN_MSG_FOCUS_OUT,			/* Lost keyboard focus */
	WIN_MSG_KEY_DOWN,			/* Key pressed */
	WIN_MSG_KEY_UP,				/* Key released */
	WIN_MSG_MOVE,				/* Window moved */
	WIN_MSG_MOVE_REQ,			/* Window move request */
	WIN_MSG_PTR,				/* Pointer moved */
	WIN_MSG_REPARENT,			/* Window was re-parented */
	WIN_MSG_RESIZE,				/* Window was re-sized */
	WIN_MSG_RESIZE_REQ,			/* Window re-size request */
	WIN_MSG_RESTACK,			/* Window was re-stack */
	WIN_MSG_RESTACK_REQ,		/* Window re-stack request */
	WIN_MSG_STATE,				/* Window state changed */
	WIN_MSG_STATE_REQ,			/* Window state change request */
	WIN_MSG_INK_OFF,			/* Inking turned off */
	WIN_MSG_KEYMOD_DOWN,    	/* Key modifier pressed */
	WIN_MSG_KEYMOD_UP,			/* Key modifier released */
	WIN_MSG_CURPOS,             /* Cursor moved */
	WIN_MSG_CURPOS_REQ          /* Request to move cursor */
} WIN_MSG_TYPE;

/*******************************************************************************
 * New Colormatching Constants
*******************************************************************************/
typedef enum {
    WIN_CMATCH_NONE,    /* Always allocate a new cell*/
    WIN_CMATCH_CLOSEST, /* Never allocate a new cell*/
    WIN_CMATCH_CLOSE,   /* Allocate cell only if no close-enough match*/
    WIN_CMATCH_EXACT    /* Allocate cell only if no exact match */
} WIN_CMATCH;
  
/*******************************************************************************
 * Colormap parameters
*******************************************************************************/
typedef struct _WIN_CELL_PARAMS
{
    BOOLEAN allocated;          /* TRUE if a cell is allocated */
    BOOLEAN shareable;          /* TRUE if a cell is sharable */
    u_int32 link_count;         /* Number of allocations made for this cell,
                                   always 1 for private cells */
    GFX_COLOR_VALUE color_value; /* Color value assigned to the cell */
} WIN_CELL_PARAMS;

typedef struct _WIN_CMAP_PARAMS
{
    u_int32 num_colors;         /* Number of colors in colormap */
    WIN_CMATCH colormatch_method; /* Method currenly used to match colors */
    GFX_COLOR_TYPE color_type;  /* Colormap color type */
} WIN_CMAP_PARAMS;

/*******************************************************************************
* WINDOW MESSAGE MASKS
*******************************************************************************/

typedef int WIN_MSG_MASK;
#define WIN_MASK_NONE			(0)		/* No messages */
#define WIN_MASK_BORDER			(1<< 0)	/* Border messages */
#define WIN_MASK_BUTTON			(1<< 1)	/* Button messages */
#define WIN_MASK_CHILD_CONFIG	(1<< 2)	/* Child configuration messages */
#define WIN_MASK_CHILD_CREATE	(1<< 3)	/* Child create window message */
#define WIN_MASK_CHILD_DESTROY	(1<< 4)	/* Child destroy window message */
#define WIN_MASK_CONFIG			(1<< 5)	/* Configuration messages */
#define WIN_MASK_DESTROY		(1<< 6)	/* Destroy window message */
#define WIN_MASK_EXPOSE			(1<< 7)	/* Expose message */
#define WIN_MASK_FOCUS			(1<< 8)	/* Focus messages */
#define WIN_MASK_KEY			(1<< 9)	/* Key messages */
#define WIN_MASK_REPARENT		(1<<10)	/* Re-parent window message */
#define WIN_MASK_PTR			(1<<11)	/* Pointer move message */
#define WIN_MASK_INK_OFF		(1<<12)	/* Inking turned off message */
#define WIN_MASK_BORDER_GRAB	(1<<13)	/* Border messages while grabbing */
#define WIN_MASK_ANY			(~0)	/* Any message */

/*******************************************************************************
* WINDOW MESSAGES
*******************************************************************************/

typedef struct _MSG_WIN_BORDER {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
	GFX_POINT root_pos;			/* Border position in root */
	GFX_POINT position;			/* Border position in parent */
} MSG_WIN_BORDER;

typedef struct _MSG_WIN_BUTTON {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
	u_int8 button;				/* Button that changed (0-7) */
	u_int8 button_state;		/* State of all ptr buttons */
	GFX_POINT root_pos;			/* Button position in root */
	GFX_POINT position;			/* Button position in parent */
	wchar_t keysym;				/* Original key symbol */
} MSG_WIN_BUTTON;

typedef struct _MSG_WIN_COMMON {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
} MSG_WIN_COMMON;

typedef struct _MSG_WIN_CREATE {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
	WIN_ID create_win;			/* Window created */
	GFX_POINT position;			/* Position in parent */
	GFX_DIMEN width;			/* Width of window */
	GFX_DIMEN height;			/* Height of window */
} MSG_WIN_CREATE;

typedef struct _MSG_WIN_DESTROY {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
	WIN_ID destroy_win;			/* Window destroyed */
} MSG_WIN_DESTROY;

typedef struct _MSG_WIN_EXPOSE {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
	GFX_RECT exposed;			/* Exposed rectangle in parent */
} MSG_WIN_EXPOSE;

typedef struct _MSG_WIN_FOCUS {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
} MSG_WIN_FOCUS;

typedef struct _MSG_WIN_INK_OFF {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
	WIN_ID obscure_win;			/* Obscuring window that caused disable */
} MSG_WIN_INK_OFF;

typedef struct _MSG_WIN_KEY {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
	wchar_t keysym;				/* Key symbol */
	INP_KEYMOD key_modifiers;	/* key modifiers */
} MSG_WIN_KEY;

typedef struct _MSG_WIN_MOVE {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
	WIN_ID move_win;			/* Window moved or requested to be moved */
	GFX_POINT position;			/* New position in parent */
} MSG_WIN_MOVE;

typedef struct _MSG_WIN_PTR {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
	GFX_POINT root_pos;			/* New position in root */
	GFX_POINT position;			/* New position in parent */
	wchar_t keysym;				/* Original key symbol */
} MSG_WIN_PTR;

typedef struct _MSG_WIN_REPARENT {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
	WIN_ID new_parent;			/* New parent window */
	WIN_PLACEMENT placement;	/* New placement */
	WIN_ID ref_win;				/* Reference window for new placement */
	GFX_POINT position;			/* New position in parent */
} MSG_WIN_REPARENT;

typedef struct _MSG_WIN_RESIZE {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
	WIN_ID resize_win;			/* Window resized or requested to be resized */
	GFX_DIMEN width;			/* New width */
	GFX_DIMEN height;			/* New height */
} MSG_WIN_RESIZE;

typedef struct _MSG_WIN_RESTACK {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
	WIN_ID restack_win;			/* Window restacked or requested to be */
								/* restacked */
	WIN_PLACEMENT placement;	/* New placement */
	WIN_ID ref_win;				/* Reference window for new placement */
} MSG_WIN_RESTACK;

typedef struct _MSG_WIN_STATE {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
	WIN_ID state_win;			/* Window whose state was changed or */
								/* requested to be changed. */
	BOOLEAN new_state;			/* New state */
} MSG_WIN_STATE;

typedef struct _MSG_WIN_CURPOS {
	MSG_COMMON com;				/* Common section */
	WIN_MSG_TYPE wtype;			/* Window message type */
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID win;					/* Window message is for */
	WIN_ID base_win;			/* Window which coordinate system is used*/
	GFX_POINT position;			/* New cursor position in window */
} MSG_WIN_CURPOS;

/*******************************************************************************
* UNION OF ALL WINDOW MESSAGE TYPES
*******************************************************************************/

typedef union _WIN_MSG {
	MSG_WIN_BORDER border;		/* Border enter/leave message */
	MSG_WIN_BUTTON button;		/* Button down/up message */
	MSG_WIN_CREATE create;		/* Create child window message */
	MSG_WIN_DESTROY destroy;	/* Destroy window message */
	MSG_WIN_EXPOSE expose;		/* Expose message */
	MSG_WIN_FOCUS focus;		/* Focus in/out message */
	MSG_WIN_KEY winkey;			/* Key down/up message */
	MSG_WIN_MOVE move;			/* Window move message */
	MSG_WIN_PTR winptr;			/* Pointer move message */
	MSG_WIN_REPARENT reparent;	/* Re-parent window message */
	MSG_WIN_RESIZE resize;		/* Window re-sized message */
	MSG_WIN_RESTACK restack;	/* Window Re-stacked message */
	MSG_WIN_STATE state;		/* Window state change message */
	MSG_WIN_INK_OFF inkoff;		/* Inking turned off message */
    MSG_WIN_CURPOS setcurpos; /* Set cursor position */
	MSG_WIN_COMMON any_win;		/* Any window message */
	MSG_COMMON any;				/* Common for all messages */
} WIN_MSG;

/*******************************************************************************
* WINDOW CALLBACK
*******************************************************************************/

typedef void (*WIN_CALLBACK)(const WIN_MSG *msg, void *user_data);

/*******************************************************************************
* WINDOW STATUS
*******************************************************************************/

typedef struct _WIN_STATUS {
	WIN_DEV_ID windev;			/* Windowing device ID */
	WIN_ID parent;				/* Parent window */
	WIN_ID child_front;			/* Child window in front of all others */
	WIN_ID child_back;			/* Child window in back of all others */
	WIN_ID sib_front_of;		/* Sibling in front of window */
	WIN_ID sib_back_of;			/* Sibling in back of window */
	BOOLEAN state;				/* Window state */
	GFX_POINT root_pos;			/* Window position in root */
	GFX_POINT position;			/* Window position in parent */
	GFX_DIMEN width;			/* Window width */
	GFX_DIMEN height;			/* Window height */
	u_int32 cursor_id;			/* Cursor ID (0 if none) */
	WIN_INK_METHOD ink_method;	/* Inking method */
	GFX_PIXEL ink_pixel;		/* Inking pixel value */
	u_int32 msg_mask;			/* Message mask */
	WIN_CALLBACK callback;		/* Callback for messages */
	void *user_data;			/* User data for callback */
	DRW_CONTEXT_ID drwctx;		/* Drawing context */
	TXT_CONTEXT_ID txtctx;		/* Text context */
	WIN_CMAP_ID cmap;			/* Colormap ID (NULL if none) */
	process_id owner_pid;		/* Process ID that owns this window */
} WIN_STATUS;

/*******************************************************************************
* WINDOW DEVICE STATUS
*******************************************************************************/

typedef struct _WIN_DEV_STATUS {
	GFX_DEV_ID gfxdev;			/* Graphics device ID */
	WIN_ID root_win;			/* Root window */
	GFX_DIMEN width;			/* Width of root window */
	GFX_DIMEN height;			/* Height of root window */
	GFX_CM coding_method;		/* Coding method of device */
	WIN_ID focus_win;			/* Window with keyboard focus */
	process_id owner_pid;		/* Process ID that owns this device */
} WIN_DEV_STATUS;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code win_alloc_cmap_cells(GFX_PIXEL ret_pixels[], WIN_CMAP_ID cmap,
	u_int16 num_colors, BOOLEAN contig, const GFX_COLOR_VALUE colors[]);
error_code win_alloc_cmap_color(GFX_PIXEL *ret_pixel, WIN_CMAP_ID cmap,
	GFX_COLOR_VALUE color);
error_code win_close_dev(WIN_DEV_ID windev);
error_code win_close_inpdev(WIN_DEV_ID windev, INP_DEV_ID inpdev);
error_code win_create_cmap(WIN_CMAP_ID *ret_cmap, WIN_DEV_ID windev,
	GFX_COLOR_TYPE color_type);
error_code win_create_cursor(WIN_DEV_ID windev, u_int32 cursor_id, WIN_CURSOR
	*cursor);
error_code win_create_dev(WIN_DEV_ID *ret_windev, GFX_DEV_ID *ret_gfxdev,
	WIN_ID *ret_root, const char *win_devname, const char *gfx_devname,
	u_int8 res_index, u_int8 cm_index, MSG_MBOX_ID mbox);
error_code win_create_win(WIN_ID *ret_win, WIN_ID parent_win, GFX_POS x,
	GFX_POS y, GFX_DIMEN width, GFX_DIMEN height, WIN_MSG_MASK mask,
	WIN_PLACEMENT placement, ...);
error_code win_destroy_cmap(WIN_CMAP_ID cmap);
error_code win_destroy_cursor(WIN_DEV_ID windev, u_int32 cursor_id);
error_code win_destroy_dev(WIN_DEV_ID windev);
error_code win_destroy_win(WIN_ID win);
error_code win_erase_ink(WIN_ID win);
error_code win_free_cmap_cells(WIN_CMAP_ID cmap, GFX_PIXEL start_pix, u_int16
	num_colors);
error_code win_get_cmap_cells(GFX_COLOR_VALUE ret_colors[], WIN_CMAP_ID cmap,
	u_int16 num_colors, GFX_PIXEL pixels[]);
error_code win_get_cmap_free(WIN_CMAP_ID cmap, u_int16 *ret_free_cells, u_int16
	*ret_largest_block, u_int16 *ret_num_blocks);
error_code win_get_dev_status(WIN_DEV_STATUS *ret_dev_status, WIN_DEV_ID
	windev);
error_code win_get_win_status(WIN_STATUS *ret_win_status, WIN_ID win);
error_code win_grab_ptr(WIN_ID win);
error_code win_init(void);
error_code win_lock_region(WIN_ID win, GFX_POS x, GFX_POS y, GFX_DIMEN width,
	GFX_DIMEN height);
error_code win_move_win(WIN_ID win, GFX_POS x, GFX_POS y);
error_code win_open_dev(WIN_DEV_ID *ret_windev, GFX_DEV_ID *ret_gfxdev,
	WIN_ID *ret_root, const char *device_name, MSG_MBOX_ID mbox);
error_code win_open_inpdev(INP_DEV_ID *ret_inpdev, WIN_DEV_ID windev, const
	char *device_name);
error_code win_reparent_win(WIN_ID win, WIN_ID new_parent_win, GFX_POS new_x,
	GFX_POS new_y, WIN_PLACEMENT placement, ...);
error_code win_resize_win(WIN_ID win, GFX_DIMEN width, GFX_DIMEN height);
error_code win_restack_win(WIN_ID win, WIN_PLACEMENT placement, ...);
error_code win_set_callback(WIN_ID win, WIN_CALLBACK callback, void
	*user_data);
error_code win_set_cmap(WIN_ID win, WIN_CMAP_ID cmap);
error_code win_set_cmap_cells(WIN_CMAP_ID cmap, const GFX_PIXEL pixels[],
	u_int16 num_colors, const GFX_COLOR_VALUE colors[]);
error_code win_set_cursor(WIN_ID win, u_int32 cursor_id);
error_code win_set_cursor_pos(WIN_ID win, GFX_POS x, GFX_POS y);
error_code win_set_cursor_state(WIN_DEV_ID windev, BOOLEAN active);
error_code win_set_drw_area(WIN_ID win, GFX_POS x, GFX_POS y, GFX_DIMEN width,
	GFX_DIMEN height);
error_code win_set_drw_context(WIN_ID win, DRW_CONTEXT_ID context);
error_code win_set_error_action(MAUI_ERR_LEVEL debug_level, MAUI_ERR_LEVEL
	passback_level, MAUI_ERR_LEVEL exit_level);
error_code win_set_focus(WIN_ID win);
error_code win_set_ink_method(WIN_ID win, WIN_INK_METHOD method);
error_code win_set_ink_pix(WIN_ID win, GFX_PIXEL ink_pixel);
error_code win_set_msg_mask(WIN_ID win, WIN_MSG_MASK mask);
error_code win_set_state(WIN_ID win, BOOLEAN active);
error_code win_set_txt_context(WIN_ID win, TXT_CONTEXT_ID context);
error_code win_term(void);
error_code win_ungrab_ptr(WIN_ID win);
error_code win_unlock_region(WIN_ID win);
error_code win_copy_block(WIN_ID dstwin, WIN_ID srcwin, 
						  GFX_POS dx, GFX_POS dy, 
						  GFX_POS sx, GFX_POS sy, 
						  GFX_DIMEN width, GFX_DIMEN height);
error_code win_restack_dev(WIN_DEV_ID windev, WIN_DEV_PLACEMENT placement);

/* New colormapping functions */

error_code win_alloc_cmap_colors(GFX_PIXEL ret_pixels[], WIN_CMAP_ID cmap, 
								 u_int16 num_colors,
								 const GFX_COLOR_VALUE colors[]);
error_code win_alloc_cmap_cell(GFX_PIXEL *ret_pixel, WIN_CMAP_ID cmap, 
							   const GFX_COLOR_VALUE color);
error_code win_set_color_match(WIN_CMAP_ID cmap, WIN_CMATCH method);
error_code win_get_cmap_params(WIN_CMAP_PARAMS * params, WIN_CMAP_ID cmap);

error_code win_get_cells_params(WIN_CELL_PARAMS params[], WIN_CMAP_ID cmap,
                               u_int16 num_cells,
                               GFX_PIXEL pixel[]);


#ifdef __cplusplus
}
#endif

#endif /* _MAUI_WIN_H */
