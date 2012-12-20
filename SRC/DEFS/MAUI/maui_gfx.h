/*******************************************************************************
* FILENAME : maui_gfx.h
*
* DESCRIPTION :
*
*   This file contains definitions for applications that use the MAUI graphics
*   device API.
*
* COPYRIGHT:
*
*   This source code is the proprietary confidential property of Microware
*   Systems Corporation, and is provided to licensee solely for documentation
*   and educational purposes. Reproduction, publication, or distribution in
*   form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  08/02/94  TJE  First version
*       2  12/18/00  SRM  CF2120 Add extended device capabilities
*       3  06/11/01  SRM  Added new A8_RGB coding method
*/
#ifndef _MAUI_GFX_H
#define _MAUI_GFX_H

#include <stddef.h>
#include <limits.h>
#include <types.h>
#include <const.h>
#include <MAUI/maui_com.h>
#include <MAUI/maui_mem.h>

/*******************************************************************************
* DEFINE ID TYPES
*******************************************************************************/

#ifdef _GFX_ID_STRUCTS_EXPOSED
typedef struct _GFX_DEV GFX_DEV;
#define GFX_DEV_ID GFX_DEV *
typedef struct _GFX_VPORT GFX_VPORT;
#define GFX_VPORT_ID GFX_VPORT *
typedef struct _GFX_CURSOR GFX_CURSOR;
#define GFX_CURSOR_ID GFX_CURSOR *
#else
typedef void * GFX_DEV_ID;
typedef void * GFX_VPORT_ID;
typedef void * GFX_CURSOR_ID;
#endif

/*******************************************************************************
* DEFINITIONS FOR INIT FUNCTION
*******************************************************************************/

typedef enum {
    GFX_INTL_OFF,               /* Interlace mode is off */
    GFX_INTL_ON,                /* Interlace mode is on */
    GFX_INTL_REPEAT             /* Interlace mode is on in field repeat mode */
} GFX_INTL_MODE;

/*******************************************************************************
* DEFINITIONS FOR VIEWPORT PLACEMENT (STACKING ORDER)
*******************************************************************************/

typedef enum {
    GFX_VPORT_FRONT,            /* In front */
    GFX_VPORT_BACK,             /* In back */
    GFX_VPORT_FRONT_OF,         /* In front of the specified viewport */
    GFX_VPORT_BACK_OF           /* In back of the specified viewport */
} GFX_VPORT_PLACEMENT;

/*******************************************************************************
* DEFINITIONS FOR DEVICE PLACEMENT (STACKING ORDER)
*******************************************************************************/

typedef enum {
    GFX_DEV_FRONT,              /* In front */
    GFX_DEV_BACK,               /* In back */
    GFX_DEV_FRONT_OF,           /* In front of the specified device */
    GFX_DEV_BACK_OF             /* In back of the specified device */
} GFX_DEV_PLACEMENT;

/*******************************************************************************
* GFX TYPE DEFINITIONS
*******************************************************************************/

#define GFX_MAX_DEV_NAME (80+1) /* Maximum length of device name */

#define GFX_POS_MIN INT_MIN     /* Minimum value for a GFX_POS type */
#define GFX_POS_MAX INT_MAX     /* Maximum value for a GFX_POS type */
#define GFX_DIMEN_MIN 0         /* Minimum value for a GFX_DIMEN type */
#define GFX_DIMEN_MAX UINT_MAX  /* Maximum value for a GFX_DIMEN type */
#define GFX_OFFSET_MIN INT_MIN  /* Minimum value for a GFX_OFFSET type */
#define GFX_OFFSET_MAX INT_MAX  /* Maximum value for a GFX_OFFSET type */

#define GFX_DIMEN_LIMIT(value) ((int32)(value & 0x7fffffff))
                                /* Convert dimen to a signed value and */
                                /* limit its range to be positive. */

typedef int32 GFX_POS;          /* X or Y coordinate in pixels */
typedef u_int32 GFX_DIMEN;      /* Width or Height in pixels */
typedef int32 GFX_OFFSET;       /* X or Y coordinate in pixels */
typedef u_int32 GFX_PIXEL;      /* Pixel value */
typedef int32 GFX_ANGLE;        /* Angle in 64ths of a degree */

#define GFX_LINE_PAD sizeof(GFX_PIXEL)  /* Line padding for drawmaps */

typedef struct _GFX_POINT {
    GFX_POS x,y;                /* X/Y position */
} GFX_POINT;

typedef struct _GFX_DELTA {
    GFX_OFFSET x,y;             /* X/Y offset */
} GFX_DELTA;

typedef struct _GFX_RECT {
    GFX_POS x,y;                /* Top-left corner of the rectangle */
    GFX_DIMEN width, height;    /* Width and height of the rectangle */
} GFX_RECT;

/*******************************************************************************
* COLOR AND PALETTE DEFINITIONS
*******************************************************************************/

typedef u_int32 GFX_RGB;        /* RGB color */
typedef u_int32 GFX_YUV;        /* YUV color */
typedef u_int32 GFX_A1_RGB;     /* RGB color with alpha flag */
typedef u_int32 GFX_YCBCR;      /* YCbCr color */
typedef u_int32 GFX_1A7_RGB;    /* RGB color with alpha flag & value */
typedef u_int32 GFX_1A7_YCBCR;  /* YCbCr color with alpha flag & value */
typedef u_int32 GFX_A8_RGB;     /* RGB color with 8-bit alpha value */

typedef enum {
    GFX_COLOR_NONE,             /* No color encoding */
    GFX_COLOR_RGB,              /* RGB color(s) */
    GFX_COLOR_YUV,              /* YUV color(s) */
    GFX_COLOR_A1_RGB,           /* RGB color(s) with alpha flag */
    GFX_COLOR_YCBCR,            /* YCbCr color(s) */
    GFX_COLOR_1A7_RGB,          /* RGB color(s) with alpha flag & value */
    GFX_COLOR_1A7_YCBCR,        /* YCbCr color(s) with alpha flag & value */
    GFX_COLOR_A8_RGB            /* RGB color(s) with 8-bit alpha value */
} GFX_COLOR_TYPE;

typedef union _GFX_COLOR_VALUE {
    GFX_RGB rgb;                /* RGB color */
    GFX_YUV yuv;                /* YUV color */
    GFX_A1_RGB a1_rgb;          /* RGB color with alpha flag */
    GFX_YCBCR ycbcr;            /* YCbCr color */
    GFX_1A7_RGB a1_7_rgb;       /* RGB color with alpha flag & value */
    GFX_1A7_YCBCR a1_7_ycbcr;   /* YCbCr color with alpha flag & value */
    GFX_A8_RGB a8_rgb;          /* RGB color with 8-bit alpha value */
    u_int8 reserved[8];         /* Force maximum size to be 8 bytes */
} GFX_COLOR_VALUE;

typedef struct _GFX_COLOR {
    GFX_COLOR_TYPE color_type;  /* Type of color */
    GFX_COLOR_VALUE color;      /* Color value */
} GFX_COLOR;

typedef struct _GFX_PALETTE {
    u_int16 start_entry;        /* Starting entry */
    u_int16 num_colors;         /* Number of colors */
    GFX_COLOR_TYPE color_type;  /* Type of color table */
    union {
        GFX_RGB *rgb;           /* Array of "num_colors" RGB colors */
        GFX_YUV *yuv;           /* Array of "num_colors" YUV colors */
        GFX_A1_RGB *a1_rgb;     /* Array of "num_colors" RGB colors with */
                                /* alpha flag */
        GFX_YCBCR *ycbcr;       /* Array of "num_colors" YCbCr colors */
        GFX_1A7_RGB *a1_7_rgb;  /* Array of "num_colors" RGB colors with */
                                /* alpha flag & value */
        GFX_1A7_YCBCR *a1_7_ycbcr;
                                /* Array of "num_colors" YCbCr colors with
                                   alpha flag & value */
        GFX_A8_RGB *a8_rgb;     /* RGB color with 8-bit alpha value */
    } colors;
} GFX_PALETTE;

/*******************************************************************************
* CODING METHOD
*
* Range of Bits     Description
* 31                Byte endianess (0 = Big, 1 = Little)
* 30                Bit endianess (0 = Big, 1 = Little)
* 27 - 29           Pixel bit depth
* 23,24 - 26        Line alignment (non-consecutive)
* 22                True Color swap (0 = no swap, 1 = swap)
* 10 - 21           Reserved by Microware
* 0 - 9             Coding Method Name
*
* If pixel bit depth is 0 then you must determine it from the coding method
* name. Otherwise it is 1=1bpp, 2=2bpp, 3=4bpp, 4=8bpp, 5=16bpp, 6=32bpp,
* 7 is reserved by Microware.
*
* The line alignment is made up of bits 24 through 26 plus bit
* 23. MAUI v2.0 added bit 23 from the reserved space to extend the
* alignment range past 512. 
* Value Alignement
*   0      4 bytes (default)
*   1      8
*   2     16
*   3     32
*   4     64
*   5    128
*   6    256
*   7    512
*   8   1024
*   9   2048
*  10   4096
*  11   8192
*  12  16384
*  13  32768
*  14  65536
*  15 Reserved by Microware
* In general, the alignement may be calculated as:
*   padsize = 1 << (gfx_get_cm_align(coding_method) + 2);
*
* The true color swap bit is an indication to the application program that
* pixel values (GFX_PIXEL) must be swapped so colors appear correctly for
* a 16 or 24 bit color modes.
*
* The coding method name is divided into the following numeric ranges.
*
* Range             Description
* 0 - 255           Standard coding methods
* 256 - 511         Standard coding methods that require driver for Bit-BLT
* 512 - 767         Reserved by Microware
* 768 - 1023        Defined by OEMs
* 
*******************************************************************************/

typedef u_int32 GFX_CM;

#define gfx_set_cm_byte_order(order)    ((order & 0x1) << 31)
#define gfx_get_cm_byte_order(cm)       ((cm >> 31) & 0x1)
#define gfx_set_cm_bit_order(order)     ((order & 0x1) << 30)
#define gfx_get_cm_bit_order(cm)        ((cm >> 30) & 0x1)
#define gfx_set_cm_depth(depth)         ((depth & 0x7) << 27)
#define gfx_get_cm_depth(cm)            ((cm >> 27) & 0x07)
#define gfx_set_cm_align(align)         (((align & 0x8) << 20) | ((align & 0x7) << 24))
#define gfx_get_cm_align(cm)            (((cm >> 20) & 0x8) | ((cm >> 24) & 0x7))
#define gfx_set_cm_tc_swap(swap)        ((swap & 0x1)<<22)
#define gfx_get_cm_tc_swap(cm)          (((cm)>>22) & 0x1)
#define gfx_set_cm_name(name)           (name & 0x3ff)
#define gfx_get_cm_name(cm)             (cm & 0x3ff)

/* Standard coding methods */
#define GFX_CM_UNKNOWN      0   /* Unknown or not yet determined */
#define GFX_CM_1BIT         1   /* 1 bit per pixel */
#define GFX_CM_2BIT         2   /* 2 bits per pixel */
#define GFX_CM_3BIT         3   /* 3 bits per pixel (padded to 4) */
#define GFX_CM_4BIT         4   /* 4 bits per pixel */
#define GFX_CM_5BIT         5   /* 5 bits per pixel (padded to 8) */
#define GFX_CM_6BIT         6   /* 6 bits per pixel (padded to 8) */
#define GFX_CM_7BIT         7   /* 7 bits per pixel (padded to 8) */
#define GFX_CM_8BIT         8   /* 8 bits per pixel */
#define GFX_CM_RGB555       9   /* 16 bit RGB */
#define GFX_CM_RGB888       10  /* 24 bit RGB (padded to 32) */
#define GFX_CM_CDI_RL3      11  /* CDi format 3 bit run-length */
#define GFX_CM_CDI_RL7      12  /* CDi format 7 bit run-length */
#define GFX_CM_CDI_DYUV     13  /* CDi format DYUV */
#define GFX_CM_1A7_8BIT     14  /* 8 bpp with alpha flag plus 7 bits alpha */
#define GFX_CM_A1_RGB555    15  /* 16 bit RGB with 1 bit of alpha */
#define GFX_CM_YCBCR422     16  /* 16 bpp YCbCr 4:2:2 */
#define GFX_CM_YCRCB422     17  /* 16 bpp YCrCb 4:2:2 */
#define GFX_CM_RGB565       18  /* 16 bpp RGB 5:6:5 */
#define GFX_CM_RGB655       19  /* 16 bpp RGB 6:5:5 */
#define GFX_CM_RGB556       20  /* 16 bpp RGB 5:5:6 */
#define GFX_CM_A8_RGB888    21  /* 24 bit RGB with 8 bits of alpha
                                   0xFF = opaque, 0x00 = transparent */

/* Standard coding methods that require driver for Bit-BLT */
#define GFX_CM_YCBCR420     256 /* 12 bpp YCbCr 4:2:0 (BLT in driver only) */
#define GFX_CM_YCRCB420     257 /* 12 bpp YCrCb 4:2:0 (BLT in driver only) */

/*******************************************************************************
* DRAWMAP (DMAP) OBJECT
*******************************************************************************/

typedef struct _GFX_DMAP {
    GFX_CM coding_method;       /* Coding method */
    GFX_DIMEN width;            /* Width in pixels */
    GFX_DIMEN height;           /* Height in pixels */
    size_t line_size;           /* Line size in bytes (mul of GFX_LINE_PAD) */
    GFX_PIXEL *pixmem;          /* Pointer to pixel memory */
    u_int32 pixmem_shade;       /* Shade used to allocate pixmem */
    size_t pixmem_size;         /* Size of pixmem in bytes */
    GFX_PALETTE *palette;       /* Pointer to color palette if any */
} GFX_DMAP;

/*******************************************************************************
* DEVICE DEFINITIONS
*******************************************************************************/

typedef struct _GFX_DEV_RES {
    GFX_DIMEN disp_width;       /* Display width */
    GFX_DIMEN disp_height;      /* Display height */
    u_int16 refresh_rate;       /* Refresh rate in hertz */
    GFX_INTL_MODE intl_mode;    /* Interlace mode */
    u_int16 aspect_x;           /* X dimension of aspect ratio */
    u_int16 aspect_y;           /* Y dimension of aspect ratio */
} GFX_DEV_RES;

typedef struct _GFX_DEV_STATUS {
    GFX_DEV_RES devres;         /* Device resolution */
    BOOLEAN vpmix;              /* Viewport mixing on/off */
    BOOLEAN extvid;             /* External video on/off */
    GFX_COLOR bkcol;            /* Backdrop color */
    GFX_COLOR transcol;         /* Transparent color */
    const GFX_DMAP *decode_dmap;/* Drawmap for video decoding */
} GFX_DEV_STATUS;

/*******************************************************************************
* VIEWPORT DEFINITIONS
*******************************************************************************/

typedef struct _GFX_VPORT_STATUS {
    GFX_DEV_ID gfxdev;          /* Graphics device */
    u_int8 intensity;           /* Viewport intensity */
    GFX_RECT area;              /* Viewport area on the display */
    BOOLEAN active;             /* Viewport is active if TRUE */
    const GFX_DMAP *dmap;       /* Current drawmap in viewport */
    GFX_POS dmapx;              /* X coordinate (offset) in drawmap */
    GFX_POS dmapy;              /* X coordinate (offset) in drawmap */
} GFX_VPORT_STATUS;

/*******************************************************************************
* DEVICE CAPABILITY INFORMATION
*******************************************************************************/

typedef struct _GFX_DEV_CM {
    GFX_CM coding_method;       /* Coding method */
    BOOLEAN clut_based;         /* TRUE if CLUT-based, otherwise FALSE */
    u_int16 dm2dp_xmul;         /* Multiplier to convert X coordinates */
    u_int16 dm2dp_ymul;         /* Multiplier to convert Y coordinates */
    u_int8 num_color_types;     /* Number of color types */
    GFX_COLOR_TYPE *color_types;/* Array of color types */
} GFX_DEV_CM;

typedef struct _GFX_DEV_CAP {
    char *hw_type;              /* Hardware type */
    char *hw_subtype;           /* Hardware subtype */
    BOOLEAN sup_vpmix;          /* Supports viewport mixing */
    BOOLEAN sup_extvid;         /* Supports external video */
    BOOLEAN sup_bkcol;          /* Supports backdrop color */
    BOOLEAN sup_vptrans;        /* Supports viewport transparency */
    BOOLEAN sup_vpinten;        /* Supports vport intensity */
    BOOLEAN sup_sync;           /* Supports retrace synchronization */
    u_int8 num_res;             /* Number of display resolutions supported */
    GFX_DEV_RES *res_info;      /* Array of display resolutions supported */
    u_int8 dac_depth;           /* Depth of the DAC in bits */
    u_int8 num_cm;              /* Number of coding methods supported */
    GFX_DEV_CM *cm_info;        /* Array of coding methods supported */
    BOOLEAN sup_decode;         /* Supports video decoding */
} GFX_DEV_CAP;

/*******************************************************************************
* EXTENDED DEVICE CAPABILITY INFORMATION
*******************************************************************************/

/* Viewport Complexity - see GFX_DEV_CAPEXTEN */
typedef enum {
  GFX_VPC_UNKNOWN,              /* Unknown or not yet determined */
  GFX_VPC_OTHER,                /* Does not fix an existing category
                                   or is undefined */
  GFX_VPC_ONE_EXACT,            /* One viewport, same size as the display */
  GFX_VPC_ONE_ANY,              /* One viewport, any size */
  GFX_VPC_ANY_FULL,             /* Multiple full width viewports */
  GFX_VPC_ANY_NOSHARE,          /* Multiple viewports on separate lines */
  GFX_VPC_ANY_SEPARATE,         /* Multiple non-overlapping viewports */
  GFX_VPC_ANY_COMPLEX,          /* Multiple overlapping viewports */
  GFX_VPC_DEVSPECIFIC = 0x7fff0000 /* Types between here and MAX_INT32 
                                      are device/OEM specific */
} GFX_VPC;

/* Viewport Drawmap Complexity - see GFX_DEV_CAPEXTEN */
typedef enum {
  GFX_VPDMC_UNKNOWN,            /* Unknown or not yet determined */
  GFX_VPDMC_OTHER,              /* Does not fix an existing category
                                   or is undefined */
  GFX_VPDMC_EXACT,              /* Drawmaps same size as viewport */
  GFX_VPDMC_TALLER,             /* Drawmaps taller than viewport */
  GFX_VPDMC_WIDER,              /* Drawmaps wider than viewport */
  GFX_VPDMC_LARGER,             /* Drawmaps larger than viewport */
  GFX_VPDMC_DEVSPECIFIC = 0x7fff0000 /* Types between here and MAX_INT32 
                                      are device/OEM specific */
} GFX_VPDMC;

/* Device Resolution/Coding Method Pairs - see GFX_DEV_CAPEXTEN */
typedef struct _GFX_DEV_MODES {
  u_int16 res_idx;              /* GFX_DEV_CAP.res_info index */
  u_int16 cm_idx;               /* GFX_DEV_CAP.cm_info index */
  char *desc;                   /* Optional text description of mode */
} GFX_DEV_MODES;

/* Extended device capabilities structure - may not be supported by all drivers */
typedef struct _GFX_DEV_CAPEXTEN {
  /* MAUI 3.1 fields */
  u_int16 version;              /* == sizeof(GFX_DEV_CAPEXTEN) used to
                                   determine revision of struct */
  u_int16 num_modes;            /* Number of modes in mode_info */
  GFX_DEV_MODES *mode_info;     /* Array of supported modes (maybe subset) */
  GFX_VPC vp_complexity;        /* Hint regarding supported viewport
                                   complexity */
  GFX_VPDMC vpdm_complexity;    /* Hint regarding supported drawmap
                                   viewport complexity */
  /* add future fields here */
} GFX_DEV_CAPEXTEN;

/* Use this macro to validate existence of fields in GFX_DEV_CAPEXTEN */
#define GFX_DEV_CAPEXTEN_VALIDATE(ptr,mem_desig) \
  ((ptr)->version >= offsetof(GFX_DEV_CAPEXTEN,mem_desig)+sizeof((ptr)->mem_desig))

/*******************************************************************************
* DEVICE ATTRIBUTES
*******************************************************************************/

typedef enum {
    GFX_ATTR_BRIGHTNESS,        /* Brightness control */
    GFX_ATTR_CONTRAST,          /* Contrast control */
    GFX_ATTR_HUE,               /* Hue control */
    GFX_ATTR_SATURATION,        /* Saturation control */
    GFX_ATTR_SHARPNESS,         /* Sharpness control */
    GFX_ATTR_GAMMA,             /* Gamma control */
    GFX_ATTR_WHITEBALANCE,      /* White Balance control */
    GFX_ATTR_DEVSPECIFIC = 0x7fff0000 /* Types between here and MAX_INT32 
                                         are device/OEM specific and should be
                                         defined contiguously to allow 
                                         searching */
} GFX_ATTR_TYPE;

typedef enum {
  GFX_ATTR_RESET,               /* Reset attribute to the default */
  GFX_ATTR_ABSOLUTE,            /* Set attribute to specified value */
  GFX_ATTR_RELATIVE             /* Set attribute relative to the current value */
} GFX_ATTR_MODE;

typedef struct _GFX_DEV_ATTR {
    int32 curvalue;             /* Current value */
    int32 defvalue;             /* Default value */
    int32 minvalue;             /* Minimum value */
    int32 maxvalue;             /* Maximum value */
    int32 stepsize;             /* Step size */
    const char *description;    /* Pointer to attribute description */
} GFX_DEV_ATTR;

/*******************************************************************************
* CURSOR CAPABILITY INFORMATION
*******************************************************************************/

typedef struct _GFX_CURSOR_INFO {
    GFX_DIMEN width;        /* Width in pixels */
    GFX_DIMEN height;       /* Height in pixels */
    u_int16 num_colors;     /* Number of colors */
    GFX_CM bitmap_cm;       /* Bitmap coding method */
    GFX_CM mask_cm;         /* Mask coding method */
    u_int16 num_cursors;        /* Max. number of cursors the driver
                    ** or hardware will support */
} GFX_CURSOR_INFO;

typedef struct _GFX_CURSOR_CAP {
    u_int8 num_info;        /* Number of cursor formats */
    GFX_CURSOR_INFO *info;      /* Pointer to array of cursor formats */
} GFX_CURSOR_CAP;

typedef struct _GFX_CURSOR_SPEC {
    GFX_POINT hit_point;        /* Hit point */
    GFX_DMAP *bitmap;       /* Cursor bitmap */
    GFX_DMAP *mask;         /* mask for bitmap */
} GFX_CURSOR_SPEC;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code gfx_alloc_mem(GFX_DEV_ID gfxdev, size_t *size, void **mem_ptr,
    u_int32 color);
error_code gfx_calc_pixmem_size(size_t *ret_size, GFX_CM coding_method,
    GFX_DIMEN width, GFX_DIMEN height);
error_code gfx_clone_dev(GFX_DEV_ID *ret_gfxdev, GFX_DEV_ID gfxdev);
error_code gfx_clone_vport(GFX_VPORT_ID *ret_vport, GFX_DEV_ID gfxdev, GFX_VPORT_ID vport);
error_code gfx_close_dev(GFX_DEV_ID gfxdev);
error_code gfx_create_dmap(GFX_DMAP **ret_dmap, u_int32 dmap_shade);
error_code gfx_create_cursor(GFX_CURSOR_ID *ret_cursor_id,
    GFX_DEV_ID gfxdev, const GFX_CURSOR_SPEC *cursor);
error_code gfx_create_vport(GFX_VPORT_ID *ret_vport, GFX_DEV_ID gfxdev, GFX_POS
    x, GFX_POS y, GFX_DIMEN width, GFX_DIMEN height, GFX_VPORT_PLACEMENT
    placement, ...);
error_code gfx_cvt_dmpos_dppos(GFX_POS *ret_dpx, GFX_POS *ret_dpy,
    GFX_VPORT_ID vport, GFX_POS dmx, GFX_POS dmy);
error_code gfx_cvt_dppos_dmpos(GFX_POS *ret_dmx, GFX_POS *ret_dmy,
    GFX_VPORT_ID vport, GFX_POS dpx, GFX_POS dpy);
error_code gfx_dealloc_mem(GFX_DEV_ID gfxdev, size_t size, void *mem_ptr,
    u_int32 color);
error_code gfx_destroy_cursor(GFX_CURSOR_ID cursor_id);
error_code gfx_destroy_dmap(GFX_DMAP *dmap);
error_code gfx_destroy_vport(GFX_VPORT_ID vport);
error_code gfx_find_vport(GFX_VPORT_ID *ret_vport, GFX_DEV_ID gfxdev, GFX_POS
    dpx, GFX_POS dpy);
error_code gfx_get_cursor_cap(const GFX_CURSOR_CAP **ret_cursor_cap,
    GFX_DEV_ID gfxdev);
error_code gfx_get_dev_attribute(GFX_DEV_ATTR *ret_dev_attr, GFX_DEV_ID gfxdev, 
                                 GFX_ATTR_TYPE attr_type);
error_code gfx_get_dev_cap(const GFX_DEV_CAP **ret_dev_cap, GFX_DEV_ID gfxdev);
error_code gfx_get_dev_capexten(const GFX_DEV_CAPEXTEN **ret_dev_capexten, 
    GFX_DEV_ID gfxdev);
error_code gfx_get_dev_status(GFX_DEV_STATUS *ret_dev_status, GFX_DEV_ID
    gfxdev);
error_code gfx_get_vport_status(GFX_VPORT_STATUS *ret_vport_status, GFX_VPORT_ID
    vport);
error_code gfx_init(void);
error_code gfx_open_dev(GFX_DEV_ID *ret_gfxdev, const char *device_name);
error_code gfx_restack_dev(GFX_DEV_ID gfxdev, GFX_DEV_PLACEMENT placement, ...);
error_code gfx_restack_vport(GFX_VPORT_ID vport, GFX_VPORT_PLACEMENT
    placement, ...);
error_code gfx_set_cursor(GFX_DEV_ID gfxdev, GFX_CURSOR_ID cursor_id);
error_code gfx_set_cursor_pos(GFX_DEV_ID gfxdev, GFX_POS x, GFX_POS y);
error_code gfx_set_decode_dst(GFX_DEV_ID gfxdev, const GFX_DMAP *decode_dmap);
error_code gfx_set_dev_attribute(GFX_DEV_ID gfxdev, GFX_ATTR_TYPE attr_type, 
                                 GFX_ATTR_MODE mode, int32 value);
error_code gfx_set_display_bkcol(GFX_DEV_ID gfxdev, const GFX_COLOR *bkcol);
error_code gfx_set_display_extvid(GFX_DEV_ID gfxdev, BOOLEAN extvid);
error_code gfx_set_display_size(GFX_DEV_ID gfxdev, GFX_DIMEN width, GFX_DIMEN
    height, GFX_INTL_MODE intl_mode, u_int16 refresh_rate);
error_code gfx_set_display_transcol(GFX_DEV_ID gfxdev, const GFX_COLOR
    *transcol);
error_code gfx_set_display_vpmix(GFX_DEV_ID gfxdev, BOOLEAN vpmix);
error_code gfx_set_dmap_pixmem(GFX_DMAP *dmap, void *pixmem, u_int32 shade,
    size_t size);
error_code gfx_set_dmap_size(GFX_DMAP *dmap, GFX_CM coding_method, GFX_DIMEN 
    width, GFX_DIMEN height);
error_code gfx_set_error_action(MAUI_ERR_LEVEL debug_level, MAUI_ERR_LEVEL
    passback_level, MAUI_ERR_LEVEL exit_level);
error_code gfx_set_vport_dmap(GFX_VPORT_ID vport, const GFX_DMAP *dmap, GFX_POS
    x, GFX_POS y);
error_code gfx_set_vport_dmpos(GFX_VPORT_ID vport, GFX_POS x, GFX_POS y);
error_code gfx_set_vport_intensity(GFX_VPORT_ID vport, u_int8 intensity);
error_code gfx_set_vport_colors(GFX_VPORT_ID vport, u_int16 start_entry,
    u_int16 num_colors, GFX_COLOR_TYPE color_type, void *colors);
error_code gfx_set_vport_position(GFX_VPORT_ID vport, GFX_POS x, GFX_POS y);
error_code gfx_set_vport_size(GFX_VPORT_ID vport, GFX_DIMEN width, GFX_DIMEN
    height);
error_code gfx_set_vport_state(GFX_VPORT_ID vport, BOOLEAN active);
error_code gfx_sync_retrace(GFX_DEV_ID gfxdev);
error_code gfx_term(void);
error_code gfx_update_display(GFX_DEV_ID gfxdev, BOOLEAN sync);

#ifdef __cplusplus
}
#endif

#endif /* _MAUI_GFX_H */
