/*******************************************************************************
*
* DESCRIPTION :
*
*	THIS FILE IS REQUIRED BY THE TEMPLATE.
*
*	This file contains global definitions for this driver.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  12/22/94  TJE  First version
*/
#ifndef _GLOBAL_H
#define _GLOBAL_H

/*******************************************************************************
* DEVICE CAPABILITIES
*
* This set of data structures define the capabilities for the graphics device.
* The template requires that you define these structures.
*******************************************************************************/

#define GDV_NUM_RESS 1
#define GDV_NUM_COLORS 1
#define GDV_NUM_CMS 2

#ifdef GDV_MAIN

GFX_DEV_RES gdv_res_info[GDV_NUM_RESS] = {
    /*
     * Default resolution (width/height) to be filled in by dr_init_hw()
     * from lu_stat's default_res
     */
	{0, 0, 60, GFX_INTL_OFF, 1, 1}
};

GFX_COLOR_TYPE gdv_colors[GDV_NUMCOLORS] = {
        GFX_COLOR_NONE,
};

GFX_DEV_CM gdv_cm_info[GDV_NUM_CMS] = {
    {GFX_CM_1BIT, TRUE, 1, 1, GDV_NUM_COLORS, gdv_colors},  /* Default CM */
    {GFX_CM_2BIT, TRUE, 1, 1, GDV_NUM_COLORS, gdv_colors},
};

GFX_DEV_CAP gdv_dev_cap = {
	"MC68328(LCD)",				/* Hardware type */
	NULL,						/* Hardware sub-type name (filled in later) */
	FALSE,						/* Supports viewport mixing */
	FALSE,						/* Supports external video */
	FALSE,						/* Supports backdrop color */
	FALSE,						/* Supports viewport transparency */
	FALSE,						/* Supports vport intensity */
	TRUE,						/* Supports retrace synchronization */
	GDV_NUM_RESS,				/* Number of display resolutions supported */
	gdv_res_info,				/* Pointer to display resolution information */
	2,							/* Depth of DAC in bits */
	GDV_NUM_CMS,				/* Number of coding methods supported */
	gdv_cm_info,				/* Pointer to coding method information */
	FALSE,						/* Supports video decoding into a drawmap */
};

#else

extern GFX_DEV_RES gdv_res_info[GDV_NUM_RESS]; 
extern GFX_COLOR_TYPE gdv_colors[GDV_NUMCOLORS];
extern GFX_DEV_CM gdv_cm_info[GDV_NUM_CMS];
extern GFX_DEV_CAP gdv_dev_cap;


#endif

/*******************************************************************************
* PROTOTYPES
*
* Prototypes for driver specific functions that must be seen by more than one
* file. These functions are not referenced by the template.
*******************************************************************************/

void activate_vport(GFX_VPORT *act_vport);
void deactivate_vport(GFX_VPORT *act_vport);

#endif /* _GLOBAL_H */
