/*****************************************************************************
** Definitions file for making MAUI device descriptors                      **
******************************************************************************
** Copyright 1995 by Microware Systems Corporation                          **
** Reproduced Under License                                                 **
**                                                                          **
** This source code is the proprietary confidential property of             **
** Microware Systems Corporation, and is provided to licensee               **
** solely for documentation and educational purposes. Reproduction,         **
** publication, or distribution in any form to any party other than         **
** the licensee is strictly prohibited.                                     **
******************************************************************************
** Edition History:                                                         **
** #   Date     Comments                                          By        **
** --- -------- ------------------------------------------------- --------- **
**   1 10/13/95 Creation                                            bat     **
**   2 10/22/96 Added PIXEL_POLARITY definition                     rws     **
*****************************************************************************/

#ifdef MFM_DESC     /* for use in MWOS/SRC/DPIO/MAUI/DESC/stat.c */

/*****************************************************************************
**	Device Descriptor for MAUI driver: /gfx									*/
#if defined(gfx)
#define LUN			1					/* logical unit number */
#define SHARE		TRUE				/* allow multiple opens */
#define PORTADDR	0xFFFFF000			/* Base address of hardware */
#define GDV_IRQ_NUM         0           /* IRQ vector number */
#define GDV_IRQ_PRIORITY    0           /* IRQ priority (before RAVE driver) */
#define GDV_IRQ_EVNAME      NULL        /* IRQ event name (NULL if none) */
#define MODE		S_IREAD | S_IWRITE	/* device mode */
#define DRV_NAME	"mauidrvr"			/* device driver name */
/*
 * for use in LU_SPECIFICS -- see MFM/DESC/stat.c, MFM/DRVR/GX_COMM/defs.h,
 * and MFM/DRVR/MPC821/static.c to see how it gets in descriptor's
 * lu_stat area.
 * 
 * DEFAULT_RESOLUTION follows the GFX_DEV_RES structure:
 * u_int32 disp_width, u_int32 disp_height, u_int16 refresh_rate,
 * enum interlace_mode, u_int16 aspect_x, u_int16 aspect_y
 *
 * HIGH/LOW defs in hardware.h
 */
#define DEFAULT_RESOLUTION {240,160,60,GFX_INTL_OFF,1,1}
#define PIXEL_POLARITY HIGH

/*
 * for following GDV_HW_SUBTYPE definitions, see MFM/DRVR/MC68328/static.h's
 * typedef enum { ... } HW_SUBTYPE;
 */
#define GDV_HW_SUBTYPE  MC68328_ALPS	/* Hardware sub-type */
#define GDV_HW_SUBNAME  "MC68328ADS with Alps LCD"	/* Hardware sub-type name */
#endif

/*****************************************************************************
**	Device Descriptor for MAUI driver: /gfx1								*/
#if defined(gfx1)
#define LUN			1					/* logical unit number */
#define SHARE		TRUE				/* allow multiple opens */
#define PORTADDR	0xFFFFF000			/* Base address of hardware */
#define GDV_IRQ_NUM         0           /* IRQ vector number */
#define GDV_IRQ_PRIORITY    0           /* IRQ priority (before RAVE driver) */
#define GDV_IRQ_EVNAME      NULL        /* IRQ event name (NULL if none) */
#define MODE		S_IREAD | S_IWRITE	/* device mode */
#define DRV_NAME	"mauidrvr"			/* device driver name */
#define DEFAULT_RESOLUTION {192,192,60,GFX_INTL_OFF,1,1}
#define PIXEL_POLARITY HIGH
/*
 * for following GDV_HW_SUBTYPE definitions, see MFM/DRVR/MC68328/static.h's
 * typedef enum { ... } HW_SUBTYPE;
 */
#define GDV_HW_SUBTYPE  MC68328_SQUARE	/* Hardware sub-type */
#define GDV_HW_SUBNAME  "MC68328ADS with Square LCD"	/* Hardware sub-type name */
#endif


/*****************************************************************************
**	Device Descriptor for MAUI driver: /gfx2								*/
#if defined(gfx2)
#define LUN			1					/* logical unit number */
#define SHARE		TRUE				/* allow multiple opens */
#define PORTADDR	0xFFFFF000			/* Base address of hardware */
#define GDV_IRQ_NUM         0           /* IRQ vector number */
#define GDV_IRQ_PRIORITY    0           /* IRQ priority (before RAVE driver) */
#define GDV_IRQ_EVNAME      NULL        /* IRQ event name (NULL if none) */
#define MODE		S_IREAD | S_IWRITE	/* device mode */
#define DRV_NAME	"mauidrvr"			/* device driver name */
#define DEFAULT_RESOLUTION {320,240,60,GFX_INTL_OFF,1,1}
#define PIXEL_POLARITY HIGH
/*
 * for following GDV_HW_SUBTYPE definitions, see MFM/DRVR/MC68328/static.h's
 * typedef enum { ... } HW_SUBTYPE;
 */
#define GDV_HW_SUBTYPE  MC68328_HALF_VGA	/* Hardware sub-type */
#define GDV_HW_SUBNAME  "MC68328ADS with 1/2 VGA LCD"	/* Hardware sub-type name */
#endif

/*****************************************************************************
**	Device Descriptor for MAUI driver: /gfx_sir								*/
#if defined(gfx_sir)
#define LUN			1					/* logical unit number */
#define SHARE		TRUE				/* allow multiple opens */
#define PORTADDR	0xFFFFF000			/* Base address of hardware */
#define GDV_IRQ_NUM         0           /* IRQ vector number */
#define GDV_IRQ_PRIORITY    0           /* IRQ priority (before RAVE driver) */
#define GDV_IRQ_EVNAME      NULL        /* IRQ event name (NULL if none) */
#define MODE		S_IREAD | S_IWRITE	/* device mode */
#define DRV_NAME	"mauidrvr"			/* device driver name */
#define DEFAULT_RESOLUTION {320,240,60,GFX_INTL_OFF,1,1}
#define PIXEL_POLARITY LOW
/*
 * for following GDV_HW_SUBTYPE definitions, see MFM/DRVR/MC68328/static.h's
 * typedef enum { ... } HW_SUBTYPE;
 */
#define GDV_HW_SUBTYPE  MC68328_QUARTER_VGA	/* Hardware sub-type */
#define GDV_HW_SUBNAME  "MC68328ADS with 1/4 VGA LCD"	/* Hardware sub-type name */
#endif



#endif  /* MFM_DESC */

