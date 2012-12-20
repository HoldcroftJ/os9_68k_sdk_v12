/*******************************************************************************
*
* DESCRIPTION :
*
*	This file contains definitions common to all JPEG functions.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  03/08/95  YAP  First version
*       2  05/08/00  SRM  Add simple thread support via mutex
*/
#ifndef _JPGCOM_H
#define _JPGCOM_H

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <const.h>
#include <errno.h>

#define _JPG_ID_STRUCTS_EXPOSED
#include <cdjpeg.h>		/* Common decls for cjpeg/djpeg applications */
#include <jversion.h>		/* for version message */

#include <MAUI/maui_gfx.h>
#include <MAUI/maui_mem.h>

typedef enum {
        JPG_INIT,
		JPG_TERM,
		JPG_SET_ERROR_ACTION,
		JPG_CREATE_CONTEXT,
		JPG_DESTROY_CONTEXT,
        JPG_SET_CONTEXT_SOURCE,
        JPG_SET_CONTEXT_OUT_COLORSPACE,
        JPG_SET_CONTEXT_QUANTIZATION,
        JPG_SET_CONTEXT_COLOR_NUMBER,
        JPG_SET_CONTEXT_OUT_SIZE,
        JPG_SET_CONTEXT_BUFFERING,
        JPG_SET_CONTEXT_QUALITY,
		JPG_SET_CONTEXT_DCT_METHOD,
		JPG_SET_CONTEXT_SCALE,
		JPG_SET_CONTEXT_DITHER,
		JPG_SET_CONTEXT_DST,
		JPG_GET_CONTEXT_PARAMS,
        JPG_READ_HEADER,
		JPG_DECOMPRESS,
		JPG_ABORT
} JPG_FUNC;

#ifdef JPG_MAIN
const char * const _jpg_func_names[] = {
        "jpg_init",
		"jpg_term",
		"jpg_set_error_action",
		"jpg_create_context",
		"jpg_destroy_context",
        "jpg_set_context_source",
        "jpg_set_context_out_colorspace",
        "jpg_set_context_quantization",
        "jpg_set_context_color_number",
        "jpg_set_context_out_size",
        "jpg_set_context_buffering",
        "jpg_set_context_quality",
		"jpg_set_context_dct_method",
		"jpg_set_context_scale",
		"jpg_set_context_dither",
		"jpg_set_context_dst",
		"jpg_get_context_params",
        "jpg_read_header",
		"jpg_decompress",
		"jpg_abort"
};
#else
extern const char * const _jpg_func_names[];
#endif

/*******************************************************************************
* JPEG CONTEXT OBJECT
*******************************************************************************/


#define _JPG_CONTEXT_SYNC 0x44435458
typedef struct _JPG_CONTEXT
{
	u_int32 sync_code;				/* Sync code - must be _JPG_CONTEXT_SYNC */

/* This section of the structure contains members that are specified by */
/* the user of this API. They are initialized in jpg_create_context(), */
/* set by jpg_set_context*() and retrieved by jpg_get_context(). */

	struct jpeg_decompress_struct cinfo;
	GFX_DMAP *dstdmap;
} JPG_CONTEXT;

#include <MAUI/maui_jpg.h>

/*******************************************************************************
* GLOBALS
*******************************************************************************/

#ifdef JPG_MAIN
#define global
u_int32 _jpg_initcnt = 0;
MAUI_ERR_LEVEL _jpg_debug_level = MAUI_ERR_ANY;
MAUI_ERR_LEVEL _jpg_passback_level = MAUI_ERR_NON_FATAL;
MAUI_ERR_LEVEL _jpg_exit_level = MAUI_ERR_NONE;
#else
#define global extern
extern u_int32 _jpg_initcnt;
extern MAUI_ERR_LEVEL _jpg_debug_level;		/* debug level */
extern MAUI_ERR_LEVEL _jpg_passback_level;	/* passback level */
extern MAUI_ERR_LEVEL _jpg_exit_level;		/* exit level */
#endif

#if defined(_OS9THREAD)
typedef enum {
	SUSPEND,						/* allow suspension while semaphore/monitor locked */
	UNSUSPEND						/* make unsuspendable before acquisition */
} susp_type;
typedef struct {
	semaphore		sem;			/* ownership semaphore					*/
	process_id		owner;			/* owner id (0 = none)					*/
	u_int32			link;			/* link count (number of locks)			*/
} monitor;
global monitor _jpg_mon; /* used to lock the JPG API */
void _jpg_monitor_lock(monitor *m, susp_type type);
void _jpg_monitor_unlock(monitor *m, susp_type type);
#define _JPG_LOCK() _jpg_monitor_lock(&_jpg_mon, SUSPEND);
#define _JPG_UNLOCK() _jpg_monitor_unlock(&_jpg_mon, SUSPEND);
#else
#define _JPG_LOCK()
#define _JPG_UNLOCK()
#endif

/*******************************************************************************
* PROTOTYPES FOR INTERNAL FUNCTIONS
*******************************************************************************/

global struct jpeg_error_mgr jerr;
error_code _jpg_error(MAUI_ERR_LEVEL errlevel, error_code ec, JPG_FUNC func);

#endif /* _JPGCOM_H */
