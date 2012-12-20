/*******************************************************************************
*
* DESCRIPTION :
*
*	This file contains driver definitions required by the driver template.
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
*       2  12/18/00  SRM  CF2120 Add extended device capabilities
*       3  01/08/01  SRM  Added missing GDV_HWBLT_* prototypes
*/
#ifndef _DEFS_H
#define _DEFS_H

/*******************************************************************************
* INCLUDE MOST HEADER FILES THAT WE NEED. THE REMAINING ARE INCLUDED AFTER THE
* LOGICAL UNIT MACRO IS DEFINED.
*******************************************************************************/
#include <types.h>
#include <config.h>					/* Supplied by driver writer */

#define _GFX_ID_STRUCTS_EXPOSED
#define _BLT_ID_STRUCTS_EXPOSED
#include <MAUI/mfm.h>
#include <MAUI/mfm_gfx.h>
#include <MAUI/mfm_blt.h>

#include <hardware.h>				/* Supplied by driver writer */
#ifdef MFM_DESC
#include <mfm_desc.h>
#endif
#include <static.h>					/* Supplied by driver writer */

#if defined(GDV_MEM_TOP_BIT)
#define adrcvt(adr)   ((u_int32)(adr) & (u_int32)0x7fffffff)
#define unadrcvt(adr) ((u_int32)(adr) | (u_int32)0x80000000)
#else
#define adrcvt(adr)   ((u_int32)(adr))
#define unadrcvt(adr) ((u_int32)(adr))
#endif

/*******************************************************************************
* CHECK DRIVER SPECIFIC CONFIGURATION PARAMETERS
*******************************************************************************/

#if defined(GDV_IOBLT_OFFSETS) && defined(GDV_IOBLT_SEP_CHROMA)
	fail GDV_IOBLT_OFFSETS and GDV_IOBLT_SEP_CHROMA cannot both be defined
#endif


/*******************************************************************************
* STATICS FOR LU_STAT
*******************************************************************************/

#ifndef GDV_LU_SPECIFICS
#define GDV_LU_SPECIFICS
#endif

#define LU_SPECIFICS														\
	GFX_DEV *dev_head;			/* Head of list of logical devices open */	\
	GFX_DEV_SHARED *dev_head_shared;/* pointer to dev_head's shared struct  \
                                   to allow safe "am I the topdev" test */  \
	GFX_DEV *dev_tail;			/* Tail of list of logical devices open */	\
	GFX_DEV *topdev;			/* Top visible device */					\
	GFX_DEV_SHARED *topdev_shared;/* pointer to topdev's shared struct to   \
                                   allow safe "am I the topdev" test */     \
																			\
	u_int32 irq_num;			/* Retrace IRQ vector number */				\
	u_int32 irq_priority;		/* Retrace IRQ priority */					\
	char *irq_evname;			/* Retrace IRQ event name */				\
	event_id irq_event_id;		/* Retrace IRQ event ID */					\
																			\
	HW_SUBTYPE hw_subtype;		/* Hardware sub-type */						\
	char *hw_subtype_name;		/* Hardware sub-type name */				\
																			\
	GDV_LU_SPECIFICS														\

#ifndef GDV_LU_SPECIFICS_INIT
#define GDV_LU_SPECIFICS_INIT
#endif

#define LU_SPECIFICS_INIT													\
	NULL,						/* dev_head (MUST BE NULL) */				\
	NULL,						/* dev_head_shared (MUST BE NULL) */		\
	NULL,						/* dev_tail (MUST BE NULL) */				\
	NULL,						/* topdev (MUST BE NULL) */					\
	NULL,						/* topdev_shared (MUST BE NULL) */			\
																			\
	GDV_IRQ_NUM,				/* irq_num */								\
	GDV_IRQ_PRIORITY,			/* irq_priority */							\
	GDV_IRQ_EVNAME,				/* irq_evname */							\
	0,							/* irq_event_id (MUST be 0) */				\
																			\
	GDV_HW_SUBTYPE,				/* hw_subtype */							\
	GDV_HW_SUBNAME,				/* hw_subtype_name */						\
																			\
	GDV_LU_SPECIFICS_INIT													\

/*******************************************************************************
* DEVICE CAPABILITIES STRUCTURES
*******************************************************************************/

#ifndef GDV_NUMCOLORS           /* driver headers can pre-define to
                                   allow override of gdv_valid_colors */
#define GDV_NUMCOLORS 3         /* Number of valid color types */

#ifdef GDV_MAIN
GFX_COLOR_TYPE gdv_valid_colors[GDV_NUMCOLORS] = {
	GFX_COLOR_RGB,
	GFX_COLOR_YUV,
	GFX_COLOR_YCBCR
};
#endif
#endif

#ifdef GDV_MAIN

#ifdef GDV_HW_CURSOR
extern GFX_CURSOR_CAP gdv_cursor_cap;
#endif
#else

extern GFX_COLOR_TYPE gdv_valid_colors[];
extern GFX_DEV_CAP gdv_dev_cap;
#ifdef GDV_DEVCAPEXTEN
extern GFX_DEV_CAPEXTEN gdv_dev_capexten;
#endif
#endif

/*******************************************************************************
* INCLUDE REMAINING HEADER FILES.
*******************************************************************************/

#ifdef _OSK
#include <defconv.h>
#endif
#include <drvr.h>
#include <global.h>					/* Supplied by driver writer */

/*******************************************************************************
* ATTACHMENT FOR GFX_DEV STRUCTURE
*******************************************************************************/

typedef struct _GDV_DVATCH {
	Mfm_lu_stat lustat;			/* Logical unit static storage pointer */
	void *gdv_static;			/* Pointer to driver static storage */
	GFX_DEV *dev_prev;			/* Next GFX_DEV in list */
	GFX_DEV *dev_next;			/* Previous GFX_DEV in list */
	u_int8 devres_cindex;		/* Index in gdv_dev_cap.res_info[] for */
								/* current resolution */
	u_int8 devres_nindex;		/* Index in gdv_dev_cap.res_info[] for */
								/* next resolution */

#ifdef GDV_INCLUDE_MEM
	u_int16 shade_hiword;		/* Hiword for memory shades on this GFX_DEV */
#endif

#ifdef GDV_DVATCH_SPECIFICS
	GDV_DVATCH_SPECIFICS
#endif	
} GDV_DVATCH;

/*******************************************************************************
* ATTACHMENT FOR GFX_VPORT STRUCTURE
*******************************************************************************/

#ifdef GDV_VPATCH_SPECIFICS
typedef struct _GDV_VPATCH {
	GDV_VPATCH_SPECIFICS
} GDV_VPATCH;
#endif	

/*******************************************************************************
* ATTACHMENT FOR GFX_CURSOR STRUCTURE
*******************************************************************************/

#ifdef GDV_CPATCH_SPECIFICS
typedef struct _GDV_CPATCH {
	GDV_CPATCH_SPECIFICS
} GDV_CPATCH;
#endif	

/*******************************************************************************
* I/O BIT-BLT DEFINITIONS
*******************************************************************************/

#ifdef GDV_INCLUDE_IOBLT

typedef struct _GDV_WORD {
	u_char bm[GDV_IOBLT_WORDSIZ];	/* Bit-mapped data */
} GDV_WORD;

#endif

/*******************************************************************************
* PROTOTYPES FOR DRIVER SPECIFIC FUNCTIONS KNOWN TO THE TEMPLATE
*******************************************************************************/

error_code GDV_INIT_HW(Dev_list dev_entry);
error_code GDV_TERM_HW(Dev_list dev_entry);
error_code GDV_INIT_IRQS(Dev_list dev_entry);
error_code GDV_TERM_IRQS(Dev_list dev_entry);
void GDV_SHOW_TOPDEV(Mfm_lu_stat lustat);
error_code _fe_set_vppos(GFX_VPORT *vport, GFX_POS x, GFX_POS y);
error_code GDV_SET_VPPOS(GFX_VPORT *vport, GFX_POS x, GFX_POS y);
error_code _fe_set_vpsize(GFX_VPORT *vport, GFX_DIMEN width, GFX_DIMEN height);
error_code GDV_SET_VPSIZE(GFX_VPORT *vport, GFX_DIMEN width, GFX_DIMEN height);
error_code _fe_set_vpstate(GFX_VPORT *vport, BOOLEAN active);
error_code GDV_SET_VPSTATE(GFX_VPORT *vport, BOOLEAN active);
error_code _fe_set_vpcolors(GFX_VPORT *vport, u_int16 start_entry, u_int16 num_colors, GFX_COLOR_TYPE color_type, void *colors);
error_code GDV_SET_VPCOLORS(GFX_VPORT *vport, u_int16 start_entry, u_int16 num_colors, GFX_COLOR_TYPE color_type, void *colors);
error_code _fe_set_vpdmap(GFX_VPORT *vport, const GFX_DMAP *dmap, GFX_POS x, GFX_POS y);
error_code GDV_SET_VPDMAP(GFX_VPORT *vport, const GFX_DMAP *dmap, GFX_POS x, GFX_POS y);
error_code _fe_set_vpdmpos(GFX_VPORT *vport, GFX_POS x, GFX_POS y);
error_code GDV_SET_VPDMPOS(GFX_VPORT *vport, GFX_POS x, GFX_POS y);
error_code _fe_restack_vp(GFX_VPORT *vport, GFX_VPORT_PLACEMENT placement, GFX_VPORT *ref_vport);
error_code GDV_RESTACK_VP(GFX_VPORT *vport, GFX_VPORT_PLACEMENT placement, GFX_VPORT *ref_vport);
error_code _fe_update_dpy(GFX_DEV *gfxdev, BOOLEAN sync);
error_code GDV_UPDATE_DPY(GFX_DEV *gfxdev, BOOLEAN sync);

#ifdef GDV_HW_CURSOR
error_code _fe_cursor_set(GFX_DEV *gfxdev, GFX_CURSOR *cursor);
error_code _fe_cursor_set_pos(GFX_DEV *gfxdev, GFX_POS x, GFX_POS y);
error_code _gdv_curcreate(GFX_DEV *gfxdev, GFX_CURSOR **ret_cursor, const GFX_CURSOR_SPEC *cursor);
error_code _gdv_curdestroy(GFX_CURSOR *cursor);

error_code GDV_CURSOR_CREATE(GFX_DEV *gfxdev, GFX_CURSOR *cursor, const GFX_CURSOR_SPEC *cursor_spec);
error_code GDV_CURSOR_DESTROY(GFX_CURSOR *);
error_code GDV_CURSOR_SET(GFX_DEV*, GFX_CURSOR*);
error_code GDV_CURSOR_SET_POS(GFX_DEV*, GFX_POS x, GFX_POS y);
#endif

#ifdef GDV_DVATCH_SPECIFICS
error_code GDV_INIT_DVATCH(Dev_list dev_entry, GFX_DEV *gfxdev);
error_code GDV_TERM_DVATCH(Dev_list dev_entry, GFX_DEV *gfxdev);
#ifdef GDV_CLONE_DVATCH
error_code GDV_CLONE_DVATCH(Dev_list dev_entry, GFX_DEV *gfxdev);
#endif
#endif
#ifdef GDV_VPATCH_SPECIFICS
error_code GDV_INIT_VPATCH(Dev_list dev_entry, GFX_VPORT *vport);
error_code GDV_TERM_VPATCH(Dev_list dev_entry, GFX_VPORT *vport);
#endif
#ifdef GDV_BCATCH_SPECIFICS
error_code GDV_INIT_BCATCH(BLT_CONTEXT *bc);
error_code GDV_TERM_BCATCH(BLT_CONTEXT *bc);
#endif
#ifdef GDV_SET_VPINTEN
error_code _fe_set_vpinten(GFX_VPORT *vport, u_int8 intensity);
error_code GDV_SET_VPINTEN(GFX_VPORT *vport, u_int8 intensity);
#endif
#ifdef GDV_SET_BKCOL
error_code GDV_SET_BKCOL(GFX_DEV *gfxdev, const GFX_COLOR *bkcol);
#endif
#ifdef GDV_SET_EXTVID
error_code GDV_SET_EXTVID(GFX_DEV *gfxdev, BOOLEAN extvid);
#endif
#ifdef GDV_SET_TRANSCOL
error_code GDV_SET_TRANSCOL(GFX_DEV *gfxdev, const GFX_COLOR *transcol);
#endif
#ifdef GDV_SET_VPMIX
error_code GDV_SET_VPMIX(GFX_DEV *gfxdev, BOOLEAN vpmix);
#endif
#ifdef GDV_DECODE_DST
error_code GDV_DECODE_DST(GFX_DEV *gfxdev, const GFX_DMAP *dmap);
#endif
#ifdef GDV_SET_ATTRIBUTE
error_code GDV_SET_ATTRIBUTE(const GFX_DEV *gfxdev, GFX_ATTR_TYPE attr_type, GFX_ATTR_MODE mode, int32 value);
#endif
#ifdef GDV_GET_ATTRIBUTE
error_code GDV_GET_ATTRIBUTE(const GFX_DEV *gfxdev, GFX_DEV_ATTR *ret_dev_attr, GFX_ATTR_TYPE attr_type);
#endif
#ifdef GDV_INIT_BOARD
error_code GDV_INIT_BOARD(Dev_list dev_entry);
#endif
#ifdef GDV_TERM_BOARD
error_code GDV_TERM_BOARD(Dev_list dev_entry);
#endif

#ifdef GDV_USER_SETSTAT /* User defined setstat function */
error_code GDV_USER_SETSTAT(Dev_list dev_entry, ss_mod_pb *ss_pb);
#endif
#ifdef GDV_USER_GETSTAT /* User defined getstat function */
error_code GDV_USER_GETSTAT(Dev_list dev_entry, gs_mod_pb *gs_pb);
#endif
#ifdef GDV_USER_WRITE /* User defined write function */
error_code GDV_USER_WRITE(Dev_list dev_entry, i_write_pb *wpb);
#endif
#ifdef GDV_USER_READ /* User defined read function */
error_code GDV_USER_READ(Dev_list dev_entry, i_read_pb *rpb);
#endif


/*******************************************************************************
* PROTOTYPES FOR DRIVER SPECIFIC IOBLT FUNCTIONS KNOWN TO THE TEMPLATE
*******************************************************************************/

#ifdef GDV_INCLUDE_IOBLT

#ifdef GDV_IOBLT_SEP_CHROMA
#define _GDV_IOBLT_READ_P \
	BLT_CONTEXT *bc, u_char *src_ptr, u_char *chroma_ptr, u_char *buf_ptr, \
	u_int32 word_count
#define _GDV_IOBLT_WRITE_P \
	BLT_CONTEXT *bc, u_char *dst_ptr, u_char *chroma_ptr, u_char *buf_ptr, \
	u_int32 word_count
#define _GDV_IOBLT_WRITE_PIX_P \
	BLT_CONTEXT *bc, u_char *dst_ptr, u_char *chroma_ptr, GDV_WORD *pix_word, \
	u_int32 word_count
#else
#define _GDV_IOBLT_READ_P \
	BLT_CONTEXT *bc, u_char *src_ptr, u_char *buf_ptr, u_int32 word_count
#define _GDV_IOBLT_WRITE_P \
	BLT_CONTEXT *bc, u_char *dst_ptr, u_char *buf_ptr, u_int32 word_count
#define _GDV_IOBLT_WRITE_PIX_P \
	BLT_CONTEXT *bc, u_char *dst_ptr, GDV_WORD *pix_word, u_int32 word_count
#endif

BOOLEAN GDV_IOBLT_GFXRAM(BLT_CONTEXT *bc, GFX_PIXEL *pixmem);
#ifdef GDV_IOBLT_READ
void GDV_IOBLT_READ(_GDV_IOBLT_READ_P);
#elif defined(GDV_IOBLT_READ_SRC) && defined(GDV_IOBLT_READ_DST)
void GDV_IOBLT_READ_SRC(_GDV_IOBLT_READ_P);
void GDV_IOBLT_READ_DST(_GDV_IOBLT_READ_P);
#else
#error If GDV_INCLUDE_IOBLT is defined, then GDV_IOBLT_READ or (GDV_IOBLT_READ_SRC and GDV_IOBLT_READ_DST) must be defined.
#endif
void GDV_IOBLT_WRITE(_GDV_IOBLT_WRITE_P);
void GDV_IOBLT_WRITE_PIX(_GDV_IOBLT_WRITE_PIX_P);
void GDV_IOBLT_OFFSETS(const GFX_DMAP *dmap, int32 *odd_offset, int32
	*even_offset);
#endif


/*******************************************************************************
* PROTOTYPES FOR DRIVER SPECIFIC HWBLT FUNCTIONS KNOWN TO THE TEMPLATE
*******************************************************************************/

#ifdef GDV_INCLUDE_HWBLT

#ifdef GDV_HWBLT_DRWMIX
error_code GDV_HWBLT_DRWMIX(BLT_CONTEXT *bc);
#endif
#ifdef GDV_HWBLT_CPYMIX
error_code GDV_HWBLT_CPYMIX(BLT_CONTEXT *bc);
#endif
#ifdef GDV_HWBLT_EXPMIX
error_code GDV_HWBLT_EXPMIX(BLT_CONTEXT *bc);
#endif
#ifdef GDV_HWBLT_PIX
error_code GDV_HWBLT_PIX(BLT_CONTEXT *bc);
#endif
#ifdef GDV_HWBLT_SRC
error_code GDV_HWBLT_SRC(BLT_CONTEXT *bc);
#endif
#ifdef GDV_HWBLT_EXPTBL
error_code GDV_HWBLT_EXPTBL(BLT_CONTEXT *bc);
#endif
#ifdef GDV_HWBLT_TRANS
error_code GDV_HWBLT_TRANS(BLT_CONTEXT *bc);
#endif
#ifdef GDV_HWBLT_MASK
error_code GDV_HWBLT_MASK(BLT_CONTEXT *bc);
#endif
#ifdef GDV_HWBLT_OFS
error_code GDV_HWBLT_OFS(BLT_CONTEXT *bc);
#endif
#ifdef GDV_HWBLT_DST
error_code GDV_HWBLT_DST(BLT_CONTEXT *bc);
#endif

#ifdef GDV_HWBLT_GETPIXEL
error_code GDV_HWBLT_GETPIXEL(GFX_PIXEL *ret_pixel, BLT_CONTEXT *bc, GFX_POS srcx, GFX_POS srcy);
#endif
#ifdef GDV_HWBLT_COPYBLK
error_code GDV_HWBLT_COPYBLK(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty, GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
#endif
#ifdef GDV_HWBLT_COPYNBLK
error_code GDV_HWBLT_COPYNBLK(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx, GFX_DIMEN height);
#endif
#ifdef GDV_HWBLT_EXPDBLK
error_code GDV_HWBLT_EXPDBLK(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty, GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
#endif
#ifdef GDV_HWBLT_EXPDNBLK
error_code GDV_HWBLT_EXPDNBLK(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx, GFX_DIMEN height);
#endif
#ifdef GDV_HWBLT_DRAWBLK
error_code GDV_HWBLT_DRAWBLK(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty, GFX_DIMEN width, GFX_DIMEN height);
#endif
#ifdef GDV_HWBLT_DRAWHLINE
error_code GDV_HWBLT_DRAWHLINE(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty, GFX_DIMEN width);
#endif
#ifdef GDV_HWBLT_DRAWVLINE
error_code GDV_HWBLT_DRAWVLINE(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty, GFX_DIMEN height);
#endif
#ifdef GDV_HWBLT_DRAWPIXEL
error_code GDV_HWBLT_DRAWPIXEL(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty);
#endif

#endif

/*******************************************************************************
* PROTOTYPES FOR TEMPLATE FUNCTIONS TO BE CALLED BY DRIVER SPECIFIC CODE
*******************************************************************************/

void gdv_insert_vport(GFX_VPORT *vport, GFX_VPORT_PLACEMENT placement, GFX_VPORT *ref_vport);
void gdv_unlink_vport(GFX_VPORT *vport);
#ifdef GDV_INCLUDE_MEM
error_code gdv_create_mem_color(u_int32 color, MEM_SHADE_TYPE shade_type, void *start_ptr, size_t size);
error_code gdv_destroy_mem_color(u_int32 color);
error_code gdv_create_mem_shade(GFX_DEV *gfxdev, u_int32 shade, MEM_SHADE_TYPE shade_type, u_int32 color, size_t initial_size, size_t grow_size);
error_code gdv_destroy_mem_shade(GFX_DEV *gfxdev, u_int32 shade);
error_code GDV_MEM_INIT(Dev_list dev_entry, char *alloc_ptr, size_t alloc_size, u_int32 value);
#endif
#ifdef GDV_INCLUDE_CVT2_RGB
error_code gdv_cvt2_rgb_setup(GFX_COLOR_TYPE color_type, void *colors);
GFX_RGB gdv_cvt2_rgb(u_int16 color_index);
#endif
#ifdef GDV_INCLUDE_CVT2_YUV
error_code gdv_cvt2_yuv_setup(GFX_COLOR_TYPE color_type, void *colors);
GFX_YUV gdv_cvt2_yuv(u_int16 color_index);
#endif
#ifdef GDV_INCLUDE_CVT2_YCBCR
error_code gdv_cvt2_ycbcr_setup(GFX_COLOR_TYPE color_type, void *colors);
GFX_YCBCR gdv_cvt2_ycbcr(u_int16 color_index);
#endif

#endif /* _DEFS_H_ */

