/*******************************************************************************
* FILENAME : maui_mem.h
*
* DESCRIPTION :
*
*	This file contains definitions for applications that use the MAUI shaded
*	memory API.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  07/12/94  TJE  First version
*/
#ifndef _MAUI_MEM_H
#define _MAUI_MEM_H

#include <stddef.h>
#include <types.h>
#include <const.h>
#include <MAUI/maui_com.h>

/*******************************************************************************
* CONSTANTS
*******************************************************************************/

#define MEM_MIN_ALLOC 4			/* Minimum allocation */
#define MEM_DEF_SHADE 1000		/* Default shade (automatically created) */

/*******************************************************************************
* SHADE DEFINITIONS
*******************************************************************************/

typedef enum {					/* Where to keep overhead */
	MEM_OV_ATTACHED,			/* Attached to the allocated segments */
	MEM_OV_SEPARATE				/* In memory separate from the segments */
} MEM_OVTYPE;

typedef enum {
	MEM_GROW_LARGER,			/* Grow by whichever size is larger */
	MEM_GROW_MULTIPLE			/* Grow by multiple of grow size */
} MEM_GROW;

typedef enum {
	MEM_SHADE_PSEUDO,			/* Pseudo shade */
	MEM_SHADE_NORMAL			/* Normal shade */
} MEM_SHADE_TYPE;

typedef struct _MEM_SHADE_STATUS {
	MEM_SHADE_TYPE type;		/* Shade type */
	MEM_OVTYPE ovtype;			/* Overhead type */
	BOOLEAN overflow_detect;	/* Overflow detection ON/OFF */
	u_int32 color;				/* Color */
	size_t initial_size;		/* Initial size */
	MEM_GROW grow_method;		/* Grow method */
	size_t grow_size;			/* Grow size */
	error_code (*alloc_func)(void *, size_t *, void **, u_int32);
								/* Function to allocate memory */
	void *alloc_data;			/* Passed to alloc_func() */
	error_code (*dealloc_func)(void *, size_t, void *, u_int32);
								/* Function to de-allocate memory */
	void *dealloc_data;			/* Passed to dealloc_func() */
} MEM_SHADE_STATUS;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code mem_calloc(void *ret_ptr, u_int32 shade_id, size_t num_entries,
	size_t entry_size);
error_code mem_create_shade(u_int32 shade_id, MEM_SHADE_TYPE shade_type,
	u_int32 color, size_t initial_size, size_t grow_size, MEM_OVTYPE ovtype,
	BOOLEAN overflow_detect);
error_code mem_destroy_shade(u_int32 shade_id);
error_code mem_free(void *ptr);
error_code mem_get_shade_status(MEM_SHADE_STATUS *ret_shade_status,
	u_int32 shade_id);
error_code mem_init(void);
error_code mem_list_overflows(void);
error_code mem_list_segments(void);
error_code mem_list_tables(void);
error_code mem_malloc(void *ret_ptr, u_int32 shade_id, size_t size);
error_code mem_realloc(void *ret_ptr, size_t size);
error_code mem_set_alloc(u_int32 shade_id, error_code (*alloc_func)(void
	*alloc_data, size_t *size, void **mem_ptr, u_int32 color), void *alloc_data,
	size_t initial_size);
error_code mem_set_alloc_bndry(u_int32 shade_id, size_t boundary_size);
error_code mem_set_dealloc(u_int32 shade_id, error_code (*dealloc_func)(void
	*dealloc_data, size_t size, void *mem_ptr, u_int32 color), void
	*dealloc_data);
error_code mem_set_error_action(MAUI_ERR_LEVEL debug_level, MAUI_ERR_LEVEL
	passback_level, MAUI_ERR_LEVEL exit_level);
error_code mem_set_grow_method(u_int32 shade_id, MEM_GROW grow_method);
error_code mem_sfree(u_int32 shade_id, void *ptr);
error_code mem_sfree_all(u_int32 shade_id);
error_code mem_term(void);

#ifdef __cplusplus
}
#endif

#endif /* _MAUI_MEM_H */
