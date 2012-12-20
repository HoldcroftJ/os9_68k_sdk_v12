/*******************************************************************************
*
* DESCRIPTION :
*
*   THIS FILE IS REQUIRED BY THE TEMPLATE.
*
*   This file contains global definitions for this driver.
*
* COPYRIGHT:
*
*   This source code is the proprietary confidential property of Microware
*   Systems Corporation, and is provided to licensee solely for documentation
*   and educational purposes. Reproduction, publication, or distribution in
*   form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  12/22/94  TJE  First version
*       2  12/18/00  SRM  CF2120 Add extended device capabilities
*/
#ifndef _GLOBAL_H
#define _GLOBAL_H

/*******************************************************************************
* DEVICE CAPABILITIES
*
* This set of data structures define the capabilities for the graphics device.
* The template requires that you define these structures.
*******************************************************************************/

#ifdef GDV_MAIN

*?*?* MODIFY THE INITIALIZER FOR THIS STRUCTURE TO INCLUDE AN ENTRY FOR EACH
*?*?* RESOLUTION SUPPORTED BY YOUR DRIVER. THE ENTRIES ALREADY IN THE TABLE
*?*?* ARE ONLY EXAMPLES. THEY SHOULD BE REPLACED. THE DEFAULT RESOLUTION SHOULD
*?*?* BE THE FIRST ENTRY IN THE TABLE.

GFX_DEV_RES gdv_res_info[] = {
    {640, 480, 60, GFX_INTL_OFF, 1, 1},     /* Default resolution */
    {360, 480, 60, GFX_INTL_OFF, 1, 1}
};

*?*?* MODIFY THE INITIALIZER FOR THIS STRUCTURE TO INCLUDE AN ENTRY FOR EACH
*?*?* CODING METHOD SUPPORTED BY YOUR DRIVER. THE ENTRIES ALREADY IN THE TABLE
*?*?* ARE ONLY EXAMPLES. THEY SHOULD BE REPLACED. THE DEFAULT CODING METHOD
*?*?* SHOULD BE THE FIRST ENTRY IN THE TABLE.

GFX_DEV_CM gdv_cm_info[] = {
    {GFX_CM_4BIT, TRUE, 1, 1, GDV_NUMCOLORS, gdv_valid_colors}, /* Default CM */
    {GFX_CM_8BIT, TRUE, 1, 1, GDV_NUMCOLORS, gdv_valid_colors}
};

*?*?* MODIFY THE INITIALIZER FOR THIS STRUCTURE TO MATCH THE CAPABILITIES OF
*?*?* YOUR DRIVER. THE ENTRIES ALREADY IN THE TABLE ARE ONLY EXAMPLES. THEY
*?*?* SHOULD BE REPLACED.

GFX_DEV_CAP gdv_dev_cap = {
    "VGA",                      /* Hardware type */
    NULL,                       /* Hardware sub-type name (filled in later) */
    FALSE,                      /* Supports viewport mixing */
    FALSE,                      /* Supports external video */
    FALSE,                      /* Supports backdrop color */
    FALSE,                      /* Supports viewport transparency */
    FALSE,                      /* Supports vport intensity */
    TRUE,                       /* Supports retrace synchronization */
    sizeof(gdv_res_info)/sizeof(*gdv_res_info), /* Num res_info */
    gdv_res_info,               /* Pointer to display resolution information */
    6,                          /* Depth of DAC in bits */
    sizeof(gdv_cm_info)/sizeof(*gdv_cm_info), /* Num cm_info */
    gdv_cm_info,                /* Pointer to coding method information */
    FALSE                       /* Supports video decoding into a drawmap */
};

*?*?* OPTIONALY MODIFY THE INITIALIZER FOR THIS STRUCTURE TO MATCH THE EXTENDED CAPABILITIES OF
*?*?* YOUR DRIVER. THE ENTRIES ALREADY IN THE TABLE ARE ONLY EXAMPLES. THEY
*?*?* SHOULD BE REPLACED. FOR THIS EXAMPLE, ALL RESOLUTIONS SUPPORT ALL 

#ifdef GDV_DEVCAPEXTEN
GFX_DEV_MODES gdv_dev_modes[] = {
  {0, 0, "640x480x4"},
  {0, 1, "640x480x8"},
  {1, 0, "360x480x4"},
  {1, 1, "360x480x8"}
};

const GFX_DEV_CAPEXTEN gdv_dev_capexten = {
    sizeof(GFX_DEV_CAPEXTEN),   /* Size/Version of structure, NEVER CHANGE */
    sizeof(gdv_dev_modes)/sizeof(*gdv_dev_modes),/* Number of modes */
    gdv_dev_modes,              /* Mode info */
    GFX_VPC_ONE_EXACT,          /* Supports only one viewport the
                                   exact size of the display */
    GFX_VPDMC_LARGER            /* Can display sub-drawmaps */
};
#endif

*?*?* MODIFY THE INITIALIZER FOR THIS STRUCTURE TO INCLUDE AN ENTRY FOR EACH
*?*?* HARDWARE CURSOR SUPPORTED BY YOUR DRIVER. THE ENTRIES ALREADY IN THE TABLE
*?*?* ARE ONLY EXAMPLES. THEY SHOULD BE REPLACED. THE DEFAULT HARDWARE CURSOR
*?*?* SHOULD BE THE FIRST ENTRY IN THE TABLE.

GFX_CURSOR_INFO gdv_cursor_info[] = {
  { 32,                         /* Width in pixels */
    32,                         /* Height in pixels */
    2,                          /* Number of colors */
    GFX_CM_8BIT,                /* Bitmap coding method */
    GFX_CM_8BIT,                /* Mask coding method */
    32                          /* Max. number of cursors the driver
                                   or hardware will support */
  }
};
#define GDV_NUM_CURCAPS (sizeof(gdv_cursor_info)/sizeof(*gdv_cursor_info))

*?*?* MODIFY THE INITIALIZER FOR THIS STRUCTURE TO MATCH THE CAPABILITIES OF
*?*?* THE HARDWARE CURSORS SUPPORTED BY YOUR DRIVER. THE ENTRIES ALREADY IN THE 
*?*?* TABLE ARE ONLY EXAMPLES. THEY SHOULD BE REPLACED.

GFX_CURSOR_CAP gdv_cursor_cap = {
    GDV_NUM_CURCAPS,            /* Number of cursor formats */
    gdv_cursor_info             /* Pointer to array of cursor formats */
};

#endif

/*******************************************************************************
* PROTOTYPES
*
* Prototypes for driver specific functions that must be seen by more than one
* file. These functions are not referenced by the template.
*******************************************************************************/

*?*?* PUT PROTOTYPES FOR GLOBAL FUNCTIONS YOU DEFINED IN YOUR DRIVER HERE.

#endif /* _GLOBAL_H */
