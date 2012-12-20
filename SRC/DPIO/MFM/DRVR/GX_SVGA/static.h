/*******************************************************************************
*
* DESCRIPTION :
*
*	THIS FILE IS REQUIRED BY THE TEMPLATE.
*
*	This file contains the definitions for static storage areas available to
*	the driver. These definitions are used by the template.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* SCCS VERSION: @(#)static.h	1.1 1/4/96
*
* EDITION  DATE      WHO  DETAIL
*       1  11/20/95  TJE  First version
*       2  04/09/98  AND  Adopt for SVGA template
*/
#ifndef _STATIC_H
#define _STATIC_H

/*******************************************************************************
* DATA STRUCTURES
*
* The following diagram shows the primary data structures which are used by the
* driver. The definitions in this file are used to allow the driver to customize
* these data structures to meet the drivers needs.
*
*     mfm_lu_stat                                            GFX_DEV
* +------------------+                                 +----------------------+
* | MFM              |            (1) <----vport_head--| API                  |
* | Defined          |                    +----------->| Defined              |
* |------------------|                    |            +----------------------+
* | Template         |--dev_head----------+                |
* | Defined          |                                     | gdv_attachment
* |------------------|    +-----------------------+        v
* | Driver           |    | Device                |    +----------------------+
* | Defined          |    | Descriptor            |  +-| Template             |
* |                  |<===|                       |  | | Defined              |
* | GDV_LU_SPECIFICS |    | GDV_LU_SPECIFICS_INIT |  | |----------------------|
* +------------------+    +-----------------------+  | | Driver               |
*                                                    | | Defined              |
*                                                    | |                      |
*     GFX_VPORT                                      | | GDV_DVATCH_SPECIFICS |
* +----------------------+                           | +----------------------+
* | API                  |<--- (1)                   |
* | Defined              |-------next---> ...        +----dev_next---> ...
* +----------------------+
*     |
*     | gdv_attachment
*     v
* +----------------------+
* | Template             |
* | Defined              |
* |----------------------|
* | Driver               |
* | Defined              |
* |                      |
* | GDV_VPATCH_SPECIFICS |
* +----------------------+
*
* The characteristics of a physical device (e.g. /gfx) is represented by the
* device descriptor. When the driver is initialized, the data in this descrip-
* tor is copied to the logical unit static storage (mfm_lu_stat) for the
* device. There is only one copy of this descriptor/mfm_lu_stat per physical
* device regardless of how many paths are open.
*
* When a path to the physical device is opened, a logical device is created.
* This is represented by the GFX_DEV structure. This data structure is allocated
* by the API and used by both the API and the driver. The driver template
* attaches a driver specific data structure to GFX_DEV using the structure
* member "gdv_attachment". A linked list of these logical devices is kept for
* each physical device.
*
* When a viewport is opened, the API allocates a GFX_VPORT data structure. This
* data structure is used by both the API and the driver. The driver template
* attaches a driver specific data structure to GFX_VPORT using the structure
* member "gdv_attachment". A linked list of these viewports is kept for each
* logical device.
*
* Below you will find definitions for the driver specific parts of these data
* structures. The header (comment) before each data structure defines the
* intended purpose of the data structure and the type of members that shoule
* be included.
*
*******************************************************************************/

/*******************************************************************************
* HARDWARE SUB-TYPES
*
* This defines the valid hardware sub-types for this driver. This must be
* defined because it is used by the template.
*******************************************************************************/

typedef enum {
	GENERIC_SVGA
} HW_SUBTYPE;

/*******************************************************************************
* DRIVER SPECIFIC STATICS FOR LU_STAT
*
* This data structure should contain driver specific definitions for the
* physical device (e.g. /gfx). Data from the descriptor may be used to init-
* ialize the members of this data strucutre.
*******************************************************************************/
#define GDV_LU_SPECIFICS\
	u_int32 mem_color;\
        u_int8 int_line;\
        u_int8 maska;\
        u_int8 maskb;\
        u_int8 interrupt_enabled;\
	u_int32 video_mode;\
        u_int32 videomem_size;\
        u_int32 videomem_base;\
	u_int32 bus;\
	u_int32 ioport;\
        GFX_DEV *lastdev;

/*******************************************************************************
* INITIALIZER FOR GDV_LU_SPECIFICS
*
* This is the initialized data for the GDV_LU_SPECIFICS area of the logical unit
* static storage. It is compiled into both the driver and the descriptor. When
* the physical device is initialized, this data is copied from the descriptor
* to the data area of the driver by the MAUI File Manager (MFM).
*
* The initializers here must match (order and total number) the members defined
* in the GDV_LU_SPECIFICS data structure above.
*******************************************************************************/
#define GDV_LU_SPECIFICS_INIT\
	MEM_COLOR,\
        INT_LINE,\
        MASKA,\
        MASKB,\
        INTERRUPT_ENABLED,\
 	MODE_101,\
        MEM_SIZE,\
        MEM_BASE_ADDRESS,\
 	BUS_TYPE,\
	PORTADDR,\
        NULL,\

/*******************************************************************************
* DRIVER SPECIFIC STATICS FOR GDV_DVATCH
*
* This data structure should contain driver specific definitions for the
* logical device (each open of a physical device). Only the top logical device
* is visible on the physical device. Therefore you must include enough infor-
* mation here to totally describe the current state of the logical device.
*
* You must also include members here that keep track of the queued-up changes
* to the logical device. They are stored here until the next time
* _os_fe_gfx_updatedpy() is called.
*******************************************************************************/

#define GDV_DVATCH_SPECIFICS\
        u_int16 hw_start_entry;\
        u_int16 hw_num_colors;\
	GFX_VPORT *topview;\
        GFX_RGB hw_palette[256];\
        BOOLEAN hw_palette_changed;

/*******************************************************************************
* DRIVER SPECIFIC STATICS FOR GDV_VPATCH
*
* This data structure should contain driver specific definitions for the
* viewport.
*******************************************************************************/

#define GDV_VPATCH_SPECIFICS\
	GFX_CM hw_coding_method;\
        GFX_DIMEN hw_vport_width;\
        GFX_DIMEN hw_vport_height;\
	u_int32 hw_pixel_mem;\
        BOOLEAN hw_active;\
        u_int32 hw_video_mode;\
        u_int8  hw_interrupt_enabled;\
        BOOLEAN hw_init_done;\
        u_int16 vp_start_entry;\
        u_int16 vp_num_colors;\
        GFX_RGB vp_palette[256];\
        BOOLEAN vp_palette_changed;

#endif /* _STATIC_H_ */









