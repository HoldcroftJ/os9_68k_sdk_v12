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
* EDITION  DATE      WHO  DETAIL
*       1  11/20/95  TJE  First version
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
*     mfm_lu_stat                                      GFX_DEV & GFX_DEV_SHARED
* +------------------+                                 +----------------------+
* | MFM              |            (1) <----vport_head--| MFM                  |
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
* GFX_VPORT & GFX_VPORT_SHARED                       | | GDV_DVATCH_SPECIFICS |
* +----------------------+                           | +----------------------+
* | MFM                  |<--- (1)                   |
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

*?*?* DEFINE THE HARDWARE SUBTYPES SUPPORTED BY YOUR DRIVER HERE. THE ENTRIES
*?*?* ALREADY IN THIS STRUCTURE ARE ONLY EXAMPLES. THEY SHOULD BE REPLACED.

typedef enum {
	ET-4000,					/* Tseng Labs ET-4000 */
	CL-GD542X,					/* Cirrus Logic CL-GD542X */
} HW_SUBTYPE;

/*******************************************************************************
* DRIVER SPECIFIC STATICS FOR LU_STAT
*
* This data structure should contain driver specific definitions for the
* physical device (e.g. /gfx). Data from the descriptor may be used to init-
* ialize the members of this data strucutre.
*******************************************************************************/

#define GDV_LU_SPECIFICS													\
	*?*?* DEFINE YOUR DRIVER SPECIFIC ENTRIES FOR THIS DATA STRUCTURE. THIS	\
	*?*?* IS A MACRO DEFINITION, SO MAKE SURE YOU USE THE CONTINUATION		\
	*?*?* INDICATOR "\" AT THE END OF EACH LINE.							\

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

#define GDV_LU_SPECIFICS_INIT												\
	*?*?* DEFINE THE INITIALIZERS FOR THE "GDV_LU_SPECIFICS" DATA STRUCTURE	\
	*?*?* HERE. YOU SHOULD REFERENCE DEFINITIONS IN THE DESCRIPTOR HEADER	\
	*?*?* FILE (IN PORTS/DRIVERNAME/DEFS) TO SET OR COMPUTE THESE VALUES.	\
	*?*?* 																	\
	*?*?* THIS IS A MACRO DEFINITION, SO MAKE SURE YOU USE THE CONTINUATION	\
	*?*?* INDICATOR "\" AT THE END OF EACH LINE.							\

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

#define GDV_DVATCH_SPECIFICS												\
	*?*?* DEFINE YOUR DRIVER SPECIFIC ENTRIES FOR THIS DATA STRUCTURE. THIS	\
	*?*?* IS A MACRO DEFINITION, SO MAKE SURE YOU USE THE CONTINUATION		\
	*?*?* INDICATOR "\" AT THE END OF EACH LINE.							\

/*******************************************************************************
* DRIVER SPECIFIC STATICS FOR GDV_VPATCH
*
* This data structure should contain driver specific definitions for the
* viewport.
*******************************************************************************/

#define GDV_VPATCH_SPECIFICS												\
	*?*?* DEFINE YOUR DRIVER SPECIFIC ENTRIES FOR THIS DATA STRUCTURE. IF	\
	*?*?* YOUR DRIVER DOES NOT NEED ANY DRIVER SPECIFIC DEFINITIONS HERE,	\
	*?*?* THEN YOU SHOULD DELETE THIS DEFINITION.
	*?*?* 																	\
	*?*?* THIS IS A MACRO DEFINITION, SO MAKE SURE YOU USE THE CONTINUATION	\
	*?*?* INDICATOR "\" AT THE END OF EACH LINE.							\

/*******************************************************************************
* DRIVER SPECIFIC STATICS FOR GDV_BCATCH
*
* This data structure should contain driver specific definitions for the
* Bit-BLT context (if GDV_INCLUDE_HWBLR is defined).
*******************************************************************************/

#define GDV_BCATCH_SPECIFICS												\
	*?*?* DEFINE YOUR DRIVER SPECIFIC ENTRIES FOR THIS DATA STRUCTURE. IF	\
	*?*?* YOUR DRIVER DOES NOT NEED ANY DRIVER SPECIFIC DEFINITIONS HERE,	\
	*?*?* THEN YOU SHOULD DELETE THIS DEFINITION.
	*?*?* 																	\
	*?*?* THIS IS A MACRO DEFINITION, SO MAKE SURE YOU USE THE CONTINUATION	\
	*?*?* INDICATOR "\" AT THE END OF EACH LINE.							\

/*******************************************************************************
* DRIVER SPECIFIC STATICS FOR GDV_CPATCH
*
* This data structure should contain driver specific definitions for the
* H/W cursor.
*******************************************************************************/

#define GDV_CPATCH_SPECIFICS												\
	*?*?* DEFINE YOUR DRIVER SPECIFIC ENTRIES FOR THIS DATA STRUCTURE. IF	\
	*?*?* YOUR DRIVER DOES NOT NEED ANY DRIVER SPECIFIC DEFINITIONS HERE,	\
	*?*?* THEN YOU SHOULD DELETE THIS DEFINITION.
	*?*?* 																	\
	*?*?* THIS IS A MACRO DEFINITION, SO MAKE SURE YOU USE THE CONTINUATION	\
	*?*?* INDICATOR "\" AT THE END OF EACH LINE.							\


#endif /* _STATIC_H_ */
