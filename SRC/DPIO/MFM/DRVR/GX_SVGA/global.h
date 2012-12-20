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
*       2  12/22/00  SRM  CF2120 Add extended device capabilities
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

#ifdef DEFRES800 
GFX_DEV_RES gdv_res_info[] = {
	{800, 600, 60, GFX_INTL_OFF, 1, 1},		/* Default resolution 800x600 */
	{640, 480, 60, GFX_INTL_OFF, 1, 1},
	{1024, 768, 60, GFX_INTL_OFF, 1, 1}
};
enum { /* maintain in the same order as the data above */
  RES800X600,
  RES640X480,
  RES1024X768
};
#else /* #ifdef DEFRES800 */
#ifdef DEFRES1024 
GFX_DEV_RES gdv_res_info[] = {
	{1024, 768, 60, GFX_INTL_OFF, 1, 1},   /* Default resolution 1024x768 */
	{640, 480, 60, GFX_INTL_OFF, 1, 1},
	{800, 600, 60, GFX_INTL_OFF, 1, 1}
};
enum { /* maintain in the same order as the data above */
  RES1024X768,
  RES640X480,
  RES800X600
};
#else /* #ifdef DEFRES1024 */
GFX_DEV_RES gdv_res_info[] = {
	{640, 480, 60, GFX_INTL_OFF, 1, 1},	     /* Default resolution 640x480*/
	{800, 600, 60, GFX_INTL_OFF, 1, 1},
	{1024, 768, 60, GFX_INTL_OFF, 1, 1}
};
enum { /* maintain in the same order as the data above */
  RES640X480,
  RES800X600,
  RES1024X768
};
#endif /* #ifdef DEFRES1024 */
#endif /* #ifdef DEFRES800 */

/* TC_SWAP determines if the colors in 16 and 24 bit mode need to be swapped on
** a given board. */
#if defined(MOTRAVEN)
#define TC_SWAP 1
#else
#define TC_SWAP 0
#endif

GFX_DEV_CM gdv_cm_info[] = {
  {GFX_CM_8BIT | gfx_set_cm_depth(4), TRUE, 1, 1, GDV_NUMCOLORS, gdv_valid_colors}
  /* Default CM */
#ifdef HIGH_COLOR
  , {(GFX_CM_RGB555 | gfx_set_cm_tc_swap(TC_SWAP)) | gfx_set_cm_depth(5), FALSE, 1, 1, 0, NULL}
#endif
#ifdef TRUE_COLOR
  , {GFX_CM_RGB888 | gfx_set_cm_tc_swap(TC_SWAP) | gfx_set_cm_depth(6), FALSE, 1, 1, 0, NULL}
#endif
};
enum { /* maintain in the same order as the data above */
  CM8BIT
#ifdef HIGH_COLOR
  , CMRGB555
#endif
#ifdef TRUE_COLOR
  , CMRGB888
#endif
};

GFX_DEV_CAP gdv_dev_cap = {
	"SVGA",                     /* Hardware type */
	NULL,                       /* Hardware sub-type name (filled in later) */
	FALSE,                      /* Supports viewport mixing */
	FALSE,                      /* Supports external video */
	FALSE,                      /* Supports backdrop color */
	FALSE,                      /* Supports viewport transparency */
	FALSE,                      /* Supports vport intensity */
	FALSE,                      /* Supports retrace synchronization */
    sizeof(gdv_res_info)/sizeof(*gdv_res_info), /* Num res_info */
	gdv_res_info,               /* Pointer to display resolution information */
	6,                          /* Depth of DAC in bits */
    sizeof(gdv_cm_info)/sizeof(*gdv_cm_info), /* Num cm_info */
	gdv_cm_info,                /* Pointer to coding method information */
	FALSE                       /* Supports video decoding into a drawmap */
};

#ifdef GDV_DEVCAPEXTEN
GFX_DEV_MODES gdv_dev_modes[] = {
  {RES640X480,   CM8BIT, "640x480x8"},
  {RES800X600,   CM8BIT, "800x600x8"},
  {RES1024X768,  CM8BIT, "1024x768x8"}
#ifdef HIGH_COLOR
  ,
  {RES640X480,   CMRGB555, "640x480x555"},
  {RES800X600,   CMRGB555, "800x600x555"},
  {RES1024X768,  CMRGB555, "1024x768x555"}
#endif
#ifdef TRUE_COLOR
  ,
  {RES640X480,   CMRGB888, "640x480x888"},
  {RES800X600,   CMRGB888, "800x600x888"},
  {RES1024X768,  CMRGB888, "1024x768x888"}
#endif
};

const GFX_DEV_CAPEXTEN gdv_dev_capexten = {
    sizeof(GFX_DEV_CAPEXTEN),   /* Size/Version of structure, NEVER CHANGE */
    sizeof(gdv_dev_modes)/sizeof(*gdv_dev_modes), /* Number of modes */
    gdv_dev_modes,              /* Mode info */
    GFX_VPC_ONE_EXACT,          /* Supports only one viewport the
                                   exact size of the display */
    GFX_VPDMC_LARGER            /* Can display sub-drawmaps */
};
#endif /* #ifdef GDV_DEVCAPEXTEN */

#endif /* #ifdef GDV_MAIN */

/*******************************************************************************
* PROTOTYPES
*
* Prototypes for driver specific functions that must be seen by more than one
* file. These functions are not referenced by the template.
*******************************************************************************/

/* svga.c */
void initGfxPart(Mfm_lu_stat lustat);
void termGfxPart(Mfm_lu_stat lustat);
void set_vga_mode(Mfm_lu_stat lustat, int mode);
void switchBank(Mfm_lu_stat lustat, u_int32 bank);
void setScreenAddr(Mfm_lu_stat lustat, u_int32 addr);

/* access.c */
u_char read_ar(Mfm_lu_stat lustat, u_int32);
void write_ar(Mfm_lu_stat lustat, u_int32, u_int32);
u_char read_gr(Mfm_lu_stat lustat, u_int32);
void write_gr(Mfm_lu_stat lustat, u_int32, u_int32);
u_char read_cr(Mfm_lu_stat lustat, u_int32);
void write_cr(Mfm_lu_stat lustat, u_int32, u_int32);
u_char read_sr(Mfm_lu_stat lustat, u_int32);
void write_sr(Mfm_lu_stat lustat, u_int32, u_int32);
u_char read_gen(Mfm_lu_stat lustat, u_int32);
void write_gen(Mfm_lu_stat lustat, u_int32, u_int32);

/* dac.c */
void set_external_palette(Mfm_lu_stat lustat, u_int32 start_entry, u_int32 num_colors, GFX_RGB *color);
void set_color(Mfm_lu_stat lustat, u_int32 index, u_int32 red, u_int32 green ,u_int32 blue);

#endif /* _GLOBAL_H */








