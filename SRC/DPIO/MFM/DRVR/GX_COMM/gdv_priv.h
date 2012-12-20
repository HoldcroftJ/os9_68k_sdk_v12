/*******************************************************************************
*
* DESCRIPTION :
*
*	This file contains definitions private to the template code used for MAUI
*	graphics drivers.
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
*       2  10/20/98  AND  H/W BLT defines
*       3  01/13/00  AND  Add _gdv_ioblt_mix_rwt8() prototype
*		   04/12/00  JPH  Add _gdv_read_hw() and _gdv_write_hw() prototypes
*/
#ifndef _GDV_PRIV_H
#define _GDV_PRIV_H

#include <defs.h>

/*******************************************************************************
* LINE BUFFERS
*
* We add a GDV_WORD to each end of the line buffer to ensure that we can
* logically shift the buffer simply by moving a pointer instead of physically
* moving each byte.
*
* Do not use "lbuf". It is used only so that we can hide the fact that we are
* adding a GDV_WORD to the beginning of the line buffer. This is necessary
* since the shifting described above may cause us to point to memory before
* the line buffer.
*
* In addition to the extra GDV_WORD at the beginning and end of the buffer, we
* add an extra GDV_WORD to the end because expand shifts the destination to the
* right one GDV_WORD to avoid overlap when it does the expansion in place.
*******************************************************************************/


#ifdef GDV_INCLUDE_IOBLT
#ifdef GDV_MAIN
u_char lbuf[3][GDV_IOBLT_LINESIZ + (GDV_IOBLT_WORDSIZ * 3)] = {"", ""};
u_char *src_buffer = lbuf[0] + GDV_IOBLT_WORDSIZ;
u_char *dst_buffer = lbuf[1] + GDV_IOBLT_WORDSIZ;
u_char *msk_buffer = lbuf[2] + GDV_IOBLT_WORDSIZ;
#else
extern u_char *src_buffer;
extern u_char *dst_buffer;
extern u_char *msk_buffer;
#endif
#endif

/*******************************************************************************
* TYPE DEFINITIONS for BIT-BLT OPERATIONS
*******************************************************************************/

typedef enum {
	GDV_SHIFT_NONE,					/* No shift */
	GDV_SHIFT_RIGHT,				/* Shift right */
	GDV_SHIFT_LEFT					/* Shift left */
} GDV_SHIFT_TYPE;

/*******************************************************************************
* ATTACHMENT FOR BIT_CONTEXT STRUCTURE FOR I/O AND H/W BASED BIT-BLT OPERATIONS
*******************************************************************************/

#ifdef GDV_INCLUDE_IOBLT
#define _GDV_IOBLT_MIX_PARAMS struct _GDV_BCATCH *bcatch, u_char *src_buf, u_int32 \
	src_inc, u_char *dst_buf, GDV_WORD *beg_mask, u_int32 midcount, GDV_WORD \
	*end_mask
#endif

#if defined(GDV_INCLUDE_IOBLT) || defined(GDV_INCLUDE_HWBLT)
typedef struct _GDV_BCATCH {
#ifdef GDV_INCLUDE_IOBLT
	/* The following members are setup by the functions that modify the */
	/* Bit-BLT context object. */

	BOOLEAN src_gfxram;				/* Source is graphics RAM if TRUE */
	BOOLEAN msk_gfxram;				/* Mask is graphics RAM if TRUE */
	BOOLEAN dst_gfxram;				/* Destination is graphics RAM if TRUE */
	void (*ioblt_read_src)(_GDV_IOBLT_READ_P);
									/* Function to read source pixels */
	void (*ioblt_read_msk)(_GDV_IOBLT_READ_P);
									/* Function to read mask pixels */
	void (*ioblt_read_dst)(_GDV_IOBLT_READ_P);
									/* Function to read source pixels */
	void (*ioblt_write_dst)(_GDV_IOBLT_WRITE_P);
									/* Function to write destination pixels */
	void (*ioblt_write_dstpix)(_GDV_IOBLT_WRITE_PIX_P);
									/* Function to write solid dst pixels */
	void (*ioblt_drwmix)(_GDV_IOBLT_MIX_PARAMS);
									/* Function to mix pixels for draw ops */
	void (*ioblt_cpymix)(_GDV_IOBLT_MIX_PARAMS);
									/* Function to mix pixels for copy ops */
	void (*ioblt_expmix)(_GDV_IOBLT_MIX_PARAMS);
									/* Function to mix pixels for expand ops */
	u_int32 srcppw;					/* Source pixels per GDV_WORD */
	u_int32 dstppw;					/* Destination pixels per GDV_WORD */
	u_int32 srcppw_shift;			/* Shift value to mul/div by src ppw */
	u_int32 dstppw_shift;			/* Shift value to mul/div by dst ppw */
	u_int32 srcbpp_shift;			/* Shift value to mul/div by src bpp */
	u_int32 dstbpp_shift;			/* Shift value to mul/div by dst bpp */
	u_int32 srcbpp_mask;			/* Mask value for multiple of src bpp */
	u_int32 dstbpp_mask;			/* Mask value for multiple of dst bpp */
	u_int32 src_line_size;			/* Source line size in bytes */
	u_int32 dst_line_size;			/* Destination line size in bytes */
	GDV_WORD curpix_word;			/* GDV_WORD packed with drwpixel value */
	GDV_WORD ofspix_word;			/* GDV_WORD packed with offset pix value */
	GDV_WORD transpix_word;			/* GDV_WORD packed with trans pix value */

	/* The following members are setup and used by each of the actual Bit-BLT */
	/* draw, copy, and expand functions that require them. */
	u_char *src_line_ptr;			/* Pointer to beg source line */
	u_char *msk_line_ptr;			/* Pointer to beg mask line */
	u_char *dst_line_ptr;			/* Pointer to beg destination line */
	u_int32 shift_process_cnt;		/* Number of bytes to process (shift) */
	u_int32 width;					/* Temp used to hold the previous width */

	/* The following members are setup by set_src_members() */
	u_char *src_ptr;				/* Pointer to beg GDV_WORD of source */
	u_char *msk_ptr;				/* Pointer to beg GDV_WORD of mask */
	u_int32 src_count;				/* Number of GDV_WORDs in the source line */

	/* The following members are setup by set_dst_members() */
	u_char *dst_ptr;				/* Pointer to beg GDV_WORD of destination */
	u_int32 dst_count;				/* Number of GDV_WORDs in a dst line */
	u_int32 dst_mid_count;			/* Number of middle GDV_WORDs in dst line */
	GDV_WORD *dst_beg_maskp;		/* Ptr to dst_beg_mask or NULL if no mask */
	GDV_WORD *dst_end_maskp;		/* Ptr to dst_end_mask or NULL if no mask */
	GDV_WORD dst_beg_mask;			/* Mask for beg GDV_WORD in dst line */
	GDV_WORD dst_end_mask;			/* Mask for end GDV_WORD in dst line */

	/* The following members are setup by set_sft_members() */
	GDV_SHIFT_TYPE shift_direction;	/* Direction to shift the source */
	u_int32 shift_process_ofs;		/* Offset to bytes to process (shift) */
	u_int8 shift_up;				/* Num of bits in each byte to shift up */
	u_int8 shift_down;				/* Num of bits in each byte to shift down */
	u_int8 shift_mask;				/* Mask for bits remaining in same byte */
	int8 shift_byte_ofs;			/* Num of bytes to logically shift */

	/* The following members are setup by set_expd_members() */
	u_char *expd_src_ptr;			/* Ptr to source for exp (end of line) */
	u_char *expd_dst_ptr;			/* Ptr to destination for expansion */
	u_int32 expd_count;				/* Number of byte to expand */
        u_int32 expd_shift_size;                /* Shift size for 1-to-2 bit expantion */
	void (*expd_src)(BLT_CONTEXT *, struct _GDV_BCATCH *);
									/* Function used to expand source */

	/* The following members are used to track whether we are processing an */
	/* odd or an even line. The first line in the drawmap has a Y coordinate */
	/* of 0, but in TV terms it is thought of as line 1, so it is the odd */
	/* line. */
#if defined(GDV_IOBLT_OFFSETS) || defined (GDV_IOBLT_SEP_CHROMA)
	BOOLEAN src_odd;				/* True if the current src is odd */
	BOOLEAN dst_odd;				/* True if the current dst is odd */
#endif

	/* The following members are used to deal with hardware that requires */
	/* the odd and even lines to be separated into two components. This */
	/* is refered to as an interlaced format. */
#ifdef GDV_IOBLT_OFFSETS
	int32 src_odd_ofs;				/* Offset to odd lines in src dmap */
	int32 src_even_ofs;				/* Offset to even lines in src dmap */
	int32 msk_odd_ofs;				/* Offset to odd lines in mask dmap */
	int32 msk_even_ofs;				/* Offset to even lines in mask dmap */
	int32 dst_odd_ofs;				/* Offset to odd lines in dst dmap */
	int32 dst_even_ofs;				/* Offset to even lines in dst dmap */
#endif

	/* The following members are used to deal with coding methods that */
	/* require separate luma (Y) and chroma (CrCb) sections. */
#ifdef GDV_IOBLT_SEP_CHROMA
	u_char *src_chroma_dmap_ptr;	/* Ptr to chroma section of src dmap */
	u_char *dst_chroma_dmap_ptr;	/* Ptr to chroma section of dst dmap */
	u_char *src_chroma_line_ptr;	/* Ptr to chroma src line for BLT */
	u_char *dst_chroma_line_ptr;	/* Ptr to chroma dst line for BLT */
	u_char *src_chroma_ptr;			/* Ptr to current src chroma for BLT */
	u_char *dst_chroma_ptr;			/* Ptr to current dst chroma for BLT */
#endif

	/* The following member is used only for RWM mode. It is stored here */
	/* to avoid changing the prototype to the mixing functions since only RWM */
	/* needs it. */
	u_char *msk_buf;				/* Mask buffer pointer */

#endif /**GDV_INCLUDE_IOBLT**/

#if defined(GDV_INCLUDE_HWBLT) && defined(GDV_BCATCH_SPECIFICS)
      GDV_BCATCH_SPECIFICS
#endif
} GDV_BCATCH;

#endif

/*******************************************************************************
* FUNCTION CODES AND PARAMETER BLOCKS FOR FUNCTIONS THAT MUST RUN IN SYSTEM
* STATE.
*******************************************************************************/

#ifdef GDV_FE_SYSTATE

#define FC_GFX_UPDATEDPY	32768	/* Update the graphics display */
#define FC_GFX_VPPOS		32769	/* Set viewport position */
#define FC_GFX_VPSIZE		32770	/* Set viewport size */
#define FC_GFX_VPSTATE		32771	/* Set viewport state */
#define FC_GFX_VPINTEN		32772	/* Set viewport intensity */
#define FC_GFX_VPDMAP		32773	/* Set viewport drawmap */
#define FC_GFX_VPDMPOS		32774	/* Set drawmap position in viewport */
#define FC_GFX_RESTACKVP	32775	/* Re-stack a viewport */
#define FC_BLT_SETCPYMIX	32776	/* Set mixing mode for copy operations */
#define FC_BLT_SETEXPMIX	32777	/* Set mixing mode for expand operations */
#define FC_BLT_SETDRWMIX	32778	/* Set mixing mode for draw operations */
#define FC_BLT_SETPIX		32779	/* Set drawing pixel value */
#define FC_BLT_SETSRC		32780	/* Set source drawmap */
#define FC_BLT_SETEXPTBL	32781	/* Set pixel expansion table */
#define FC_BLT_SETTRANS		32782	/* Set transparent pixel value */
#define FC_BLT_SETMASK		32783	/* Set mask drawmap */
#define FC_BLT_SETOFS		32784	/* Set offset pixel value */
#define FC_BLT_SETDST		32785	/* Set destination drawmap */
#define FC_IOBLT_COPYBLK	32786	/* I/O copy rectangular block of pixels */
#define FC_IOBLT_COPYNBLK	32787	/* I/O copy next rectangular block of pixels */
#define FC_IOBLT_EXPDBLK	32788	/* I/O expand rectangular block of pixels */
#define FC_IOBLT_EXPDNBLK	32789	/* I/O expand next rect block of pixels */
#define FC_IOBLT_DRAWBLK	32790	/* I/O draw block of pixels */
#define FC_IOBLT_DRAWHLINE	32791	/* I/O draw horizontal line of pixels */
#define FC_IOBLT_DRAWVLINE	32792	/* I/O draw vertical line of pixels */
#define FC_IOBLT_DRAWPIXEL	32793	/* I/O draw pixel */
#define FC_IOBLT_GETPIXEL	32794	/* I/O get pixel */
#define FC_GFX_VPCOLORS		32795	/* Set viewport colors */
#define FC_GFX_SETCURSOR      32796 /* Set cursor */
#define FC_GFX_SETCURSORPOS   32797 /* Set cursor position */
#define FC_HWBLT_COPYBLK	32798	/* H/W copy rectangular block of pixels */
#define FC_HWBLT_COPYNBLK	32799	/* H/W copy next rectangular block of pixels */
#define FC_HWBLT_EXPDBLK	32800	/* H/W expand rectangular block of pixels */
#define FC_HWBLT_EXPDNBLK	32801	/* H/W expand next rect block of pixels */
#define FC_HWBLT_DRAWBLK	32802	/* H/W draw block of pixels */
#define FC_HWBLT_DRAWHLINE	32803	/* H/W draw horizontal line of pixels */
#define FC_HWBLT_DRAWVLINE	32804	/* H/W draw vertical line of pixels */
#define FC_HWBLT_DRAWPIXEL	32805	/* H/W draw pixel */
#define FC_HWBLT_GETPIXEL	32806	/* H/W get pixel */

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_UPDATEDPY */
	GFX_DEV *gfxdev;			/* Graphics device */
	BOOLEAN sync;				/* Synchronize with retrace if TRUE */
} ss_gfx_updatedpy_pb, *Ss_gfx_updatedpy_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_VPPOS */
	GFX_VPORT *vport;			/* Viewport */
	GFX_POS x;					/* X position for viewport */
	GFX_POS y;					/* Y position for viewport */
} ss_gfx_vppos_pb, *Ss_gfx_vppos_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_VPSIZE */
	GFX_VPORT *vport;			/* Viewport */
	GFX_DIMEN width;			/* Width of viewport */
	GFX_DIMEN height;			/* Height of viewport */
} ss_gfx_vpsize_pb, *Ss_gfx_vpsize_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_VPSTATE */
	GFX_VPORT *vport;			/* Viewport */
	BOOLEAN active;				/* Make viewport active if TRUE */
} ss_gfx_vpstate_pb, *Ss_gfx_vpstate_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_VPINTEN */
	GFX_VPORT *vport;			/* Viewport */
	u_int8 intensity;			/* Viewport intensity */
} ss_gfx_vpinten_pb, *Ss_gfx_vpinten_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_VPCOLORS */
	GFX_VPORT *vport;			/* Viewport */
	u_int16 start_entry;		/* Start entry */
	u_int16 num_colors;			/* Number of colors */
	GFX_COLOR_TYPE color_type;	/* Type of encoding for "colors" */
	void *colors;				/* Array of colors */
} ss_gfx_vpcolors_pb, *Ss_gfx_vpcolors_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_VPDMAP */
	GFX_VPORT *vport;			/* Viewport */
	const GFX_DMAP *dmap;		/* Drawmap to be placed in the viewport */
	GFX_POS x;					/* X coord in drawmap for 1st pixel in vport */
	GFX_POS y;					/* Y coord in drawmap for 1st pixel in vport */
} ss_gfx_vpdmap_pb, *Ss_gfx_vpdmap_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_VPDMPOS */
	GFX_VPORT *vport;			/* Viewport */
	GFX_POS x;					/* X coord in drawmap for 1st pixel in vport */
	GFX_POS y;					/* Y coord in drawmap for 1st pixel in vport */
} ss_gfx_vpdmpos_pb, *Ss_gfx_vpdmpos_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_RESTACKVP */
	GFX_VPORT *vport;			/* Viewport */
	GFX_VPORT_PLACEMENT placement;
								/* Placement in stack of viewports */
	GFX_VPORT *ref_vport;		/* Reference vieport (put behind this one) */
} ss_gfx_restackvp_pb, *Ss_gfx_restackvp_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_SETCURSOR */
	GFX_DEV *gfxdev;                        /* Graphics device */
	GFX_CURSOR *cursor;			/* Cursor */
} ss_gfx_setcursor_pb, *Ss_gfx_setcursor_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_GFX_SETCURSORPOS */
	GFX_DEV *gfxdev;			/* Graphics device */
	GFX_POS x, y;				/* position */
} ss_gfx_setcursorpos_pb, *Ss_gfx_setcursorpos_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_SETCPYMIX */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
} ss_blt_setcpymix_pb, *Ss_blt_setcpymix_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_SETEXPMIX */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
} ss_blt_setexpmix_pb, *Ss_blt_setexpmix_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_SETDRWMIX */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
} ss_blt_setdrwmix_pb, *Ss_blt_setdrwmix_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_SETPIX */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
} ss_blt_setpix_pb, *Ss_blt_setpix_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_SETSRC */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
} ss_blt_setsrc_pb, *Ss_blt_setsrc_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_SETEXPTBL */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
} ss_blt_setexptbl_pb, *Ss_blt_setexptbl_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_SETTRANS */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
} ss_blt_settrans_pb, *Ss_blt_settrans_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_SETMASK */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
} ss_blt_setmask_pb, *Ss_blt_setmask_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_SETOFS */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
} ss_blt_setofs_pb, *Ss_blt_setofs_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_SETDST */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
} ss_blt_setdst_pb, *Ss_blt_setdst_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_COPYBLK */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
	GFX_POS dstx;				/* Destination X coordinate */
	GFX_POS dsty;				/* Destination Y coordinate */
	GFX_POS srcx;				/* Source X coordinate */
	GFX_POS srcy;				/* Source Y coordinate */
	GFX_DIMEN width;			/* Width */
	GFX_DIMEN height;			/* Height */
} ss_blt_copyblk_pb, *Ss_blt_copyblk_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_COPYNBLK */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
	GFX_POS dstx;				/* Destination X coordinate */
	GFX_POS srcx;				/* Source X coordinate */
	GFX_DIMEN height;			/* Height */
} ss_blt_copynblk_pb, *Ss_blt_copynblk_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_EXPDBLK */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
	GFX_POS dstx;				/* Destination X coordinate */
	GFX_POS dsty;				/* Destination Y coordinate */
	GFX_POS srcx;				/* Source X coordinate */
	GFX_POS srcy;				/* Source Y coordinate */
	GFX_DIMEN width;			/* Width */
	GFX_DIMEN height;			/* Height */
} ss_blt_expdblk_pb, *Ss_blt_expdblk_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_EXPDNBLK */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
	GFX_POS dstx;				/* Destination X coordinate */
	GFX_POS srcx;				/* Source X coordinate */
	GFX_DIMEN height;			/* Height */
} ss_blt_expdnblk_pb, *Ss_blt_expdnblk_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_DRAWBLK */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
	GFX_POS dstx;				/* Destination X coordinate */
	GFX_POS dsty;				/* Destination Y coordinate */
	GFX_DIMEN width;			/* Width */
	GFX_DIMEN height;			/* Height */
} ss_blt_drawblk_pb, *Ss_blt_drawblk_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_DRAWHLINE */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
	GFX_POS dstx;				/* Destination X coordinate */
	GFX_POS dsty;				/* Destination Y coordinate */
	GFX_DIMEN width;			/* Width */
} ss_blt_drawhline_pb, *Ss_blt_drawhline_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_DRAWVLINE */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
	GFX_POS dstx;				/* Destination X coordinate */
	GFX_POS dsty;				/* Destination Y coordinate */
	GFX_DIMEN height;			/* Height */
} ss_blt_drawvline_pb, *Ss_blt_drawvline_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_DRAWPIXEL */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
	GFX_POS dstx;				/* Destination X coordinate */
	GFX_POS dsty;				/* Destination Y coordinate */
} ss_blt_drawpixel_pb, *Ss_blt_drawpixel_pb;

typedef struct {
	u_int16 func_code;			/* Must be FC_BLT_GETPIXEL */
	GFX_PIXEL *ret_pixel;		/* Return value for pixel */
	BLT_CONTEXT *bc;			/* Bit-BLT context */
	GFX_POS srcx;				/* Source X coordinate */
	GFX_POS srcy;				/* Source Y coordinate */
} gs_blt_getpixel_pb, *Gs_blt_getpixel_pb;

#endif

/*******************************************************************************
* PROTOTYPES - MAIN ENTRY POINTS
*******************************************************************************/

error_code dr_iniz(Dev_list dev_entry);
error_code dr_term(Dev_list dev_entry);
error_code dr_getstat(Dev_list dev_entry, gs_mod_pb *gs_pb);
error_code dr_setstat(Dev_list dev_entry, ss_mod_pb *ss_pb);
error_code dr_read_hw(Dev_list dev_entry, i_read_pb *rpb);
error_code dr_write_hw(Dev_list dev_entry, i_write_pb *wpb);

/*******************************************************************************
* PROTOTYPES FOR IOBLT AND HWBLT FUNCTIONS
*******************************************************************************/

#if defined(GDV_INCLUDE_IOBLT) || defined(GDV_INCLUDE_HWBLT)
error_code _gdv_blt_createbc(Dev_list dev_entry, GFX_DEV *gfxdev, BLT_CONTEXT
	*bc);
error_code _gdv_blt_destroybc(Dev_list dev_entry, GFX_DEV *gfxdev, BLT_CONTEXT
	*bc);
error_code _gdv_blt_drwmix(BLT_CONTEXT *bc);
error_code _gdv_blt_cpymix(BLT_CONTEXT *bc);
error_code _gdv_blt_expmix(BLT_CONTEXT *bc);
error_code _gdv_blt_pix(BLT_CONTEXT *bc);
error_code _gdv_blt_src(BLT_CONTEXT *bc);
error_code _gdv_blt_exptbl(BLT_CONTEXT *bc);
error_code _gdv_blt_trans(BLT_CONTEXT *bc);
error_code _gdv_blt_mask(BLT_CONTEXT *bc);
error_code _gdv_blt_ofs(BLT_CONTEXT *bc);
error_code _gdv_blt_dst(BLT_CONTEXT *bc);

#ifdef GDV_INCLUDE_IOBLT
void set_src_members(GDV_BCATCH *bcatch, GFX_POS srcx, GFX_DIMEN width);
void set_dst_members(GDV_BCATCH *bcatch, GFX_POS dstx, GFX_DIMEN width);
void compute_word_mask(GDV_WORD *mask, u_int32 start_bit, u_int32 end_bit);
void set_sft_members(GDV_BCATCH *bcatch, GFX_POS srcx, GFX_POS dstx);
void shift_src_buffer(GDV_BCATCH *bcatch, u_char *srcbuf);
void mix_word_replace(u_char *src_word, u_char *dst_word, GDV_WORD
	*mask_word);
void _gdv_ioblt_mix_sxd(_GDV_IOBLT_MIX_PARAMS);
void _gdv_ioblt_mix_n_sxd(_GDV_IOBLT_MIX_PARAMS);
void _gdv_ioblt_mix_sod(_GDV_IOBLT_MIX_PARAMS);
void _gdv_ioblt_mix_n_sod(_GDV_IOBLT_MIX_PARAMS);
void _gdv_ioblt_mix_ns_ad(_GDV_IOBLT_MIX_PARAMS);
void _gdv_ioblt_mix_so_nd(_GDV_IOBLT_MIX_PARAMS);
void _gdv_ioblt_mix_spd(_GDV_IOBLT_MIX_PARAMS);
void _gdv_ioblt_mix_dms(_GDV_IOBLT_MIX_PARAMS);
void _gdv_ioblt_mix_spo(_GDV_IOBLT_MIX_PARAMS);
void _gdv_ioblt_mix_rwt4(_GDV_IOBLT_MIX_PARAMS);
void _gdv_ioblt_mix_rwt8(_GDV_IOBLT_MIX_PARAMS);
void _gdv_ioblt_mix_rwm(_GDV_IOBLT_MIX_PARAMS);
error_code _gdv_ioblt_copyblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
error_code _gdv_ioblt_copynblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx,
	GFX_DIMEN height);
error_code _gdv_ioblt_expdblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
error_code _gdv_ioblt_expdnblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx,
	GFX_DIMEN height);
error_code _gdv_ioblt_drawblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN width, GFX_DIMEN height);
error_code _gdv_ioblt_drawhline(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN width);
error_code _gdv_ioblt_drawvline(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN height);
error_code _gdv_ioblt_drawpixel(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty);
error_code _gdv_ioblt_getpixel(GFX_PIXEL *ret_pixel, BLT_CONTEXT *bc, GFX_POS
	srcx, GFX_POS srcy);
void expd_src_1to2(BLT_CONTEXT *bc, GDV_BCATCH *bcatch);
void expd_src_1to4(BLT_CONTEXT *bc, GDV_BCATCH *bcatch);
void expd_src_1to8(BLT_CONTEXT *bc, GDV_BCATCH *bcatch);
void expd_src_1to16(BLT_CONTEXT *bc, GDV_BCATCH *bcatch);
void expd_src_1to32(BLT_CONTEXT *bc, GDV_BCATCH *bcatch);
void expd_src_1to16_420(BLT_CONTEXT *bc, GDV_BCATCH *bcatch);
#endif  /**GDV_INCLUDE_IOBLT**/

#endif  /**GDV_INCLUDE_IOBLT||GDV_INCLUDE_HWBLT**/

/*******************************************************************************
* PROTOTYPES FOR FAST ENTRY POINTS THAT MUST RUN IN SYSTEM STATE
*******************************************************************************/

#ifdef GDV_FE_SYSTATE

error_code _os_ss_gfx_set_cursor(GFX_DEV *gfxdev, GFX_CURSOR *cursor);
error_code _os_ss_gfx_set_cursor_pos(GFX_DEV *gfxdev, GFX_POS x, GFX_POS y);
error_code _os_ss_gfx_updatedpy(GFX_DEV *gfxdev, BOOLEAN sync);
error_code _os_ss_gfx_vppos(GFX_VPORT *vport, GFX_POS x, GFX_POS y);
error_code _os_ss_gfx_vpsize(GFX_VPORT *vport, GFX_DIMEN width, GFX_DIMEN height);
error_code _os_ss_gfx_vpstate(GFX_VPORT *vport, BOOLEAN active);
#ifdef GDV_SET_VPINTEN
error_code _os_ss_gfx_vpinten(GFX_VPORT *vport, u_int8 intensity);
#endif
error_code _os_ss_gfx_vpcolors(GFX_VPORT *vport, u_int16 start_entry, u_int16
	num_colors, GFX_COLOR_TYPE color_type, void *colors);
error_code _os_ss_gfx_vpdmap(GFX_VPORT *vport, const GFX_DMAP *dmap, GFX_POS x,
	GFX_POS y);
error_code _os_ss_gfx_vpdmpos(GFX_VPORT *vport, GFX_POS x, GFX_POS y);
error_code _os_ss_gfx_restackvp(GFX_VPORT *vport, GFX_VPORT_PLACEMENT placement,
	GFX_VPORT *ref_vport);

#if defined(GDV_INCLUDE_IOBLT) || defined(GDV_INCLUDE_HWBLT)
error_code _os_ss_blt_cpymix(BLT_CONTEXT *bc);
error_code _os_ss_blt_expmix(BLT_CONTEXT *bc);
error_code _os_ss_blt_drwmix(BLT_CONTEXT *bc);
error_code _os_ss_blt_pix(BLT_CONTEXT *bc);
error_code _os_ss_blt_src(BLT_CONTEXT *bc);
error_code _os_ss_blt_exptbl(BLT_CONTEXT *bc);
error_code _os_ss_blt_trans(BLT_CONTEXT *bc);
error_code _os_ss_blt_mask(BLT_CONTEXT *bc);
error_code _os_ss_blt_ofs(BLT_CONTEXT *bc);
error_code _os_ss_blt_dst(BLT_CONTEXT *bc);

#ifdef GDV_INCLUDE_IOBLT
error_code _os_ss_ioblt_copyblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
error_code _os_ss_ioblt_copynblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx,
	GFX_DIMEN height);
error_code _os_ss_ioblt_expdblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
error_code _os_ss_ioblt_expdnblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx,
	GFX_DIMEN height);
error_code _os_ss_ioblt_drawblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN width, GFX_DIMEN height);
error_code _os_ss_ioblt_drawhline(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN width);
error_code _os_ss_ioblt_drawvline(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN height);
error_code _os_ss_ioblt_drawpixel(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty);
error_code _os_gs_ioblt_getpixel(GFX_PIXEL *ret_pixel, BLT_CONTEXT *bc, GFX_POS
	srcx, GFX_POS srcy);
#endif  /**GDV_INCLUDE_IOBLT**/

#ifdef GDV_INCLUDE_HWBLT
error_code _os_ss_hwblt_copyblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
error_code _os_ss_hwblt_copynblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx,
	GFX_DIMEN height);
error_code _os_ss_hwblt_expdblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height);
error_code _os_ss_hwblt_expdnblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx,
	GFX_DIMEN height);
error_code _os_ss_hwblt_drawblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN width, GFX_DIMEN height);
error_code _os_ss_hwblt_drawhline(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN width);
error_code _os_ss_hwblt_drawvline(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN height);
error_code _os_ss_hwblt_drawpixel(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty);
error_code _os_gs_hwblt_getpixel(GFX_PIXEL *ret_pixel, BLT_CONTEXT *bc, GFX_POS
	srcx, GFX_POS srcy);
#endif  /**GDV_INCLUDE_HWBLT**/

#endif  /**GDV_INCLUDE_IOBLT||GDV_INCLUDE_HWBLT**/

#endif  /**GDV_FE_SYSTATE**/



/*******************************************************************************
* MACROS FOR DRIVER-DEFINED HARDWARE BLIT FAST ENTRY POINTS
*******************************************************************************/
#ifdef GDV_INCLUDE_HWBLT

/* Macro for draw_block */
#ifdef GDV_MEM_TOP_BIT
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_draw_block(bc,func) \
          bc->draw_block = _os_ss_hwblt_drawblk; \
          bc->draw_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN, GFX_DIMEN))adrcvt(bc->draw_block);
#else
#define _hwblt_set_draw_block(bc,func) \
          bc->draw_block = func; \
          bc->draw_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN, GFX_DIMEN))adrcvt(bc->draw_block);
#endif
#else
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_draw_block(bc,func) \
          bc->draw_block = _os_ss_hwblt_drawblk;
#else
#define _hwblt_set_draw_block(bc,func) \
          bc->draw_block = func;
#endif
#endif

/* Macro for draw_vline */
#ifdef GDV_MEM_TOP_BIT
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_draw_vline(bc,func) \
          bc->draw_vline = _os_ss_hwblt_drawvline; \
          bc->draw_vline = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN))adrcvt(bc->draw_vline);
#else
#define _hwblt_set_draw_vline(bc,func) \
          bc->draw_vline = func; \
          bc->draw_vline = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN))adrcvt(bc->draw_vline);
#endif
#else
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_draw_vline(bc,func) \
          bc->draw_vline = _os_ss_hwblt_drawvline;
#else
#define _hwblt_set_draw_vline(bc,func) \
          bc->draw_vline = func;
#endif
#endif

/* Macro for draw_hline */
#ifdef GDV_MEM_TOP_BIT
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_draw_hline(bc,func) \
          bc->draw_hline = _os_ss_hwblt_drawhline; \
          bc->draw_hline = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN))adrcvt(bc->draw_hline);
#else
#define _hwblt_set_draw_hline(bc,func) \
          bc->draw_hline = func; \
          bc->draw_hline = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN))adrcvt(bc->draw_hline);
#endif
#else
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_draw_hline(bc,func) \
          bc->draw_hline = _os_ss_hwblt_drawhline;
#else
#define _hwblt_set_draw_hline(bc,func) \
          bc->draw_hline = func;
#endif
#endif

/* Macro for draw_pixel */
#ifdef GDV_MEM_TOP_BIT
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_draw_pixel(bc,func) \
          bc->draw_pixel = _os_ss_hwblt_drawpixel; \
          bc->draw_pixel = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS))adrcvt(bc->draw_pixel);
#else
#define _hwblt_set_draw_pixel(bc,func) \
          bc->draw_pixel = func; \
          bc->draw_pixel = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS))adrcvt(bc->draw_pixel);
#endif
#else
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_draw_pixel(bc,func) \
          bc->draw_pixel = _os_ss_hwblt_drawpixel;
#else
#define _hwblt_set_draw_pixel(bc,func) \
          bc->draw_pixel = func;
#endif
#endif

/* Macro for copy_block */
#ifdef GDV_MEM_TOP_BIT
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_copy_block(bc,func) \
          bc->copy_block = _os_ss_hwblt_copyblk; \
          bc->copy_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_POS, GFX_POS, GFX_DIMEN, GFX_DIMEN))adrcvt(bc->copy_block);
#else
#define _hwblt_set_copy_block(bc,func) \
          bc->copy_block = func; \
          bc->copy_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_POS, GFX_POS, GFX_DIMEN, GFX_DIMEN))adrcvt(bc->copy_block);
#endif
#else
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_copy_block(bc,func) \
          bc->copy_block = _os_ss_hwblt_copyblk;
#else
#define _hwblt_set_copy_block(bc,func) \
          bc->copy_block = func;
#endif
#endif

/* Macro for copy_next_block */
#ifdef GDV_MEM_TOP_BIT
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_copy_next_block(bc,func) \
          bc->copy_next_block = _os_ss_hwblt_copynblk; \
          bc->copy_next_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN))adrcvt(bc->copy_next_block);
#else
#define _hwblt_set_copy_next_block(bc,func) \
          bc->copy_next_block = func; \
          bc->copy_next_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN))adrcvt(bc->copy_next_block);
#endif
#else
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_copy_next_block(bc,func) \
          bc->copy_next_block = _os_ss_hwblt_copynblk;
#else
#define _hwblt_set_copy_block(bc,func) \
          bc->copy_next_block = func;
#endif
#endif

/* Macro for expd_block */
#ifdef GDV_MEM_TOP_BIT
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_expd_block(bc,func) \
          bc->expd_block = _os_ss_hwblt_expdblk; \
          bc->expd_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_POS, GFX_POS, GFX_DIMEN, GFX_DIMEN))adrcvt(bc->expd_block);
#else
#define _hwblt_set_expd_block(bc,func) \
          bc->expd_block = func; \
          bc->expd_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_POS, GFX_POS, GFX_DIMEN, GFX_DIMEN))adrcvt(bc->expd_block);
#endif
#else
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_expd_block(bc,func) \
          bc->expd_block = _os_ss_hwblt_expdblk;
#else
#define _hwblt_set_expd_block(bc,func) \
          bc->expd_block = func;
#endif
#endif

/* Macro for expd_next_block */
#ifdef GDV_MEM_TOP_BIT
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_expd_next_block(bc,func) \
          bc->expd_next_block = _os_ss_hwblt_expdnblk; \
          bc->expd_next_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN))adrcvt(bc->expd_next_block);
#else
#define _hwblt_set_expd_next_block(bc,func) \
          bc->expd_next_block = func; \
          bc->expd_next_block = (error_code(*)(BLT_CONTEXT *, GFX_POS, GFX_POS, GFX_DIMEN))adrcvt(bc->expd_next_block);
#endif
#else
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_expd_next_block(bc,func) \
          bc->expd_next_block = _os_ss_hwblt_expdnblk;
#else
#define _hwblt_set_expd_next_block(bc,func) \
          bc->expd_next_block = func;
#endif
#endif

/* Macro for get_pixel */
#ifdef GDV_MEM_TOP_BIT
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_get_pixel(bc,func) \
          bc->get_pixel = _os_gs_hwblt_getpixel; \
          bc->get_pixel = (error_code(*)(GFX_PIXEL *, BLT_CONTEXT *, GFX_POS, GFX_POS))adrcvt(bc->get_pixel);
#else
#define _hwblt_set_get_pixel(bc,func) \
          bc->get_pixel = func; \
          bc->get_pixel = (error_code(*)(GFX_PIXEL *, BLT_CONTEXT *, GFX_POS, GFX_POS))adrcvt(bc->get_pixel);
#endif
#else
#ifdef GDV_FE_SYSTATE
#define _hwblt_set_get_pixel(bc,func) \
          bc->get_pixel = _os_gs_hwblt_getpixel;
#else
#define _hwblt_set_get_pixel(bc,func) \
          bc->get_pixel = func;
#endif
#endif
         
#endif  /**GDV_INCLUDE_HWBLT**/



/*******************************************************************************
* SYSTEM PROTOTYPES
*******************************************************************************/

#ifdef _OS9000
void *change_static(void *dest_stat);
#endif

/*******************************************************************************
* OTHER PROTOTYPES
*******************************************************************************/

error_code _gdv_opendev(Dev_list dev_entry, GFX_DEV **ret_gfxdev,
	Mfm_path_desc path_desc);
error_code _gdv_clonedev(Dev_list dev_entry, GFX_DEV **ret_gfxdev, GFX_DEV
	*gfxdev, Mfm_path_desc path_desc);
error_code _gdv_closedev(Dev_list dev_entry, GFX_DEV *gfxdev, Mfm_path_desc
	path_desc);
error_code _gdv_restackdev(Dev_list dev_entry, GFX_DEV *gfxdev,
	GFX_DEV_PLACEMENT placement, GFX_DEV *ref_gfxdev);
error_code _gdv_devres(Dev_list dev_entry, GFX_DEV *gfxdev, GFX_DIMEN
	disp_width, GFX_DIMEN disp_height, GFX_INTL_MODE intl_mode, u_int16
	refresh_rate);
error_code _gdv_createvp(Dev_list dev_entry, GFX_DEV *gfxdev, GFX_VPORT
	**ret_vport, GFX_POS x, GFX_POS y, GFX_DIMEN width, GFX_DIMEN height,
	GFX_VPORT_PLACEMENT placement, GFX_VPORT *ref_vport, Mfm_path_desc
	path_desc);
error_code _gdv_clonevp(Dev_list dev_entry, GFX_VPORT **ret_vport, GFX_VPORT
	*vport, Mfm_path_desc path_desc);
error_code _gdv_destroyvp(Dev_list dev_entry, GFX_DEV *gfxdev, GFX_VPORT
	*vport);
error_code _gdv_alloc_mem(Dev_list dev_entry, GFX_DEV *gfxdev, size_t *size,
	void **mem_ptr, u_int32 color);
error_code _gdv_dealloc_mem(Dev_list dev_entry, GFX_DEV *gfxdev, size_t size,
	void *mem_ptr, u_int32 color);

#endif /* _GDV_PRIV_H */
