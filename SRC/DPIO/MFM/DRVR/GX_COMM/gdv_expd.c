/*******************************************************************************
* DESCRIPTION :
*
*	I/O based Bit-BLT functions for expand operations.
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
#include <gdv_priv.h>

static void set_exp_members(GDV_BCATCH *bcatch, GFX_POS srcx, GFX_POS dstx,
	GFX_DIMEN width);

/*******************************************************************************
* NAME:		_gdv_ioblt_expdblk
*
* USAGE:	Expand a block of pixels from the source drawmap to the destination
*			drawmap.
*/
error_code _gdv_ioblt_expdblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif
	GDV_BCATCH *bcatch = bc->gdv_attachment;

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

        /* Validate "width" and "height" parameters before processing */
        /* further. Update bcatch->width field to stay compatible with */
        /* the next potential call to blt_next_expd_block(). Return if */
        /* either of them is zero. This is the first place validation */
        /* happens when driver is handling expand Blit-BLT operations */
        if ((width == 0) || (height == 0)) {
               bcatch->width = width;
#ifndef GDV_FE_SYSTATE
               change_static(app_static);
#endif
               return SUCCESS;
        }

        /* Convert line buffer pointers */
	/* from system to user addresses */
	/* for the processors like SuperH and MIPS */
	#if defined(GDV_MEM_TOP_BIT)
	src_buffer = (u_char *)adrcvt(src_buffer);
	dst_buffer = (u_char *)adrcvt(dst_buffer);
	msk_buffer = (u_char *)adrcvt(msk_buffer);
	#endif

	/* Setup the members of bcatch that are not updated by helper functions */
	#ifdef GDV_IOBLT_OFFSETS
	if (bcatch->src_odd_ofs != bcatch->src_even_ofs) {
		bcatch->src_odd = (srcy & 1) ? FALSE : TRUE;
		bcatch->src_line_ptr = (u_char *)(bc->srcdmap->pixmem) +
			((srcy / 2) * (bcatch->src_line_size)) +
			((bcatch->src_odd) ? bcatch->src_odd_ofs : bcatch->src_even_ofs);
	} else {
		bcatch->src_line_ptr = (u_char *)(bc->srcdmap->pixmem) +
			(srcy * (bcatch->src_line_size));
	}
	if (bcatch->dst_odd_ofs != bcatch->dst_even_ofs) {
		bcatch->dst_odd = (dsty & 1) ? FALSE : TRUE;
		bcatch->dst_line_ptr = (u_char *)(bc->dstdmap->pixmem) +
			((dsty / 2) * (bcatch->dst_line_size)) +
			((bcatch->dst_odd) ? bcatch->dst_odd_ofs : bcatch->dst_even_ofs);
	} else {
		bcatch->dst_line_ptr = (u_char *)(bc->dstdmap->pixmem) +
			(dsty * (bcatch->dst_line_size));
	}
	#else
	bcatch->src_line_ptr = (u_char *)(bc->srcdmap->pixmem) +
		(srcy * (bcatch->src_line_size));
	bcatch->dst_line_ptr = (u_char *)(bc->dstdmap->pixmem) +
		(dsty * (bcatch->dst_line_size));
	#endif
	bcatch->width = width;
	#ifdef GDV_IOBLT_SEP_CHROMA
	bcatch->dst_odd = (dsty & 1) ? FALSE : TRUE;
	bcatch->dst_chroma_line_ptr = bcatch->dst_chroma_dmap_ptr +
		((dsty / 2) * (bcatch->dst_line_size));
	#endif

	/* Call the continuation function */
	ec = _gdv_ioblt_expdnblk(bc, dstx, srcx, height);

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}

/*******************************************************************************
* NAME:		_gdv_ioblt_expdnblk
*
* USAGE:	Expand the next block of pixels from the source drawmap to the
*			destination drawmap.
*/
error_code _gdv_ioblt_expdnblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx,
	GFX_DIMEN height)
{
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif
	GDV_BCATCH *bcatch = bc->gdv_attachment;
	u_char *srcbuf;
#ifdef GDV_IOBLT_OFFSETS
	BOOLEAN src_odd = bcatch->src_odd;
	BOOLEAN dst_odd = bcatch->dst_odd;
#endif

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

        /* Validate "bcatch->width" and "height" parameters before */
        /* processing further. Return if either of them is zero. This is */
        /* the second  place validation happens when driver is handling */
        /* expand Blit-BLT operations */
        if ((bcatch->width == 0) || (height == 0)) {
#ifndef GDV_FE_SYSTATE
               change_static(app_static);
#endif
               return SUCCESS;
        }

        /* Convert line buffer pointers */
	/* from system to user addresses */
	/* for the processors like SuperH and MIPS */
	#if defined(GDV_MEM_TOP_BIT)
	src_buffer = (u_char *)adrcvt(src_buffer);
	dst_buffer = (u_char *)adrcvt(dst_buffer);
	msk_buffer = (u_char *)adrcvt(msk_buffer);
	#endif

	/* Set necessary members of the Bit-BLT context attachment */
	set_src_members(bcatch, srcx, bcatch->width);
	set_dst_members(bcatch, dstx, bcatch->width);
	set_exp_members(bcatch, srcx, dstx, bcatch->width);
	srcbuf = src_buffer + GDV_IOBLT_WORDSIZ;

	/* Process each line in the block */
	while (height-- != 0) {

		/* Read all source pixels into the source buffer and expand them */
		#ifdef GDV_IOBLT_SEP_CHROMA
		bcatch->ioblt_read_src(bc, bcatch->src_ptr, NULL, src_buffer,
			bcatch->src_count);
		#else
		bcatch->ioblt_read_src(bc, bcatch->src_ptr, src_buffer,
			bcatch->src_count);
		#endif
		bcatch->expd_src(bc, bcatch);

		/* Mix the source and destination pixels using the proper mixing */
		/* function, then write the results to the destination. */
		if (bc->expmix == BLT_MIX_REPLACE) {

			/* Replace mode. Mix the first and last GDV_WORD of the destina- */
			/* tion into the source buffer, then write the results. */
			#ifdef GDV_IOBLT_SEP_CHROMA
			u_int32 ofs = (bcatch->dst_count - 1) * (GDV_IOBLT_WORDSIZ / 2);
			#else
			u_int32 ofs = (bcatch->dst_count - 1) * GDV_IOBLT_WORDSIZ;
			#endif
			if (bcatch->dst_beg_maskp) {
				#ifdef GDV_IOBLT_SEP_CHROMA
				bcatch->ioblt_read_dst(bc, bcatch->dst_ptr,
					bcatch->dst_chroma_ptr, dst_buffer, 1);
				#else
				bcatch->ioblt_read_dst(bc, bcatch->dst_ptr, dst_buffer, 1);
				#endif
				mix_word_replace(srcbuf, dst_buffer, bcatch->dst_beg_maskp);
			}
			if (bcatch->dst_end_maskp) {
				#ifdef GDV_IOBLT_SEP_CHROMA
				bcatch->ioblt_read_dst(bc, bcatch->dst_ptr + ofs,
					bcatch->dst_chroma_ptr, dst_buffer, 1);
				#else
			bcatch->ioblt_read_dst(bc, bcatch->dst_ptr + ofs, dst_buffer,
					1);
				#endif
				mix_word_replace(srcbuf + ofs, dst_buffer,
					bcatch->dst_end_maskp);
			}
			#ifdef GDV_IOBLT_SEP_CHROMA
			bcatch->ioblt_write_dst(bc, bcatch->dst_ptr, bcatch->dst_chroma_ptr,
				srcbuf, bcatch->dst_count);
			#else
			bcatch->ioblt_write_dst(bc, bcatch->dst_ptr, srcbuf,
				bcatch->dst_count);
			#endif

		} else {

			/* All modes except replace mode. Read the destination pixels */
			/* into the destination buffer, then mix the buffers and write */
			/* the results. */
			#ifdef GDV_IOBLT_SEP_CHROMA
			bcatch->ioblt_read_dst(bc, bcatch->dst_ptr, bcatch->dst_chroma_ptr,
				dst_buffer, bcatch->dst_count);
			bcatch->ioblt_expmix(bcatch, srcbuf, GDV_IOBLT_WORDSIZ, dst_buffer,
				bcatch->dst_beg_maskp, bcatch->dst_mid_count,
				bcatch->dst_end_maskp);
			bcatch->ioblt_write_dst(bc, bcatch->dst_ptr, bcatch->dst_chroma_ptr,
				dst_buffer, bcatch->dst_count);
			#else
			bcatch->ioblt_read_dst(bc, bcatch->dst_ptr, dst_buffer,
				bcatch->dst_count);
			bcatch->ioblt_expmix(bcatch, srcbuf, GDV_IOBLT_WORDSIZ, dst_buffer,
				bcatch->dst_beg_maskp, bcatch->dst_mid_count,
				bcatch->dst_end_maskp);
			bcatch->ioblt_write_dst(bc, bcatch->dst_ptr, dst_buffer,
				bcatch->dst_count);
			#endif
		}

		/* Setup for next line */
		#ifdef GDV_IOBLT_OFFSETS
		if (bcatch->src_odd_ofs != bcatch->src_even_ofs) {
			if (src_odd == TRUE) {
				src_odd = FALSE;
				bcatch->src_ptr += bcatch->src_even_ofs - bcatch->src_odd_ofs;
			} else {
				src_odd = TRUE;
				bcatch->src_ptr += bcatch->src_odd_ofs - bcatch->src_even_ofs +
					bcatch->src_line_size;
			}
		} else {
			bcatch->src_ptr += bcatch->src_line_size;
		}
		if (bcatch->dst_odd_ofs != bcatch->dst_even_ofs) {
			if (dst_odd == TRUE) {
				dst_odd = FALSE;
				bcatch->dst_ptr += bcatch->dst_even_ofs - bcatch->dst_odd_ofs;
			} else {
				dst_odd = TRUE;
				bcatch->dst_ptr += bcatch->dst_odd_ofs - bcatch->dst_even_ofs +
					bcatch->dst_line_size;
			}
		} else {
			bcatch->dst_ptr += bcatch->dst_line_size;
		}
		#else
		bcatch->src_ptr += bcatch->src_line_size;
		bcatch->dst_ptr += bcatch->dst_line_size;
		#endif
		#ifdef GDV_IOBLT_SEP_CHROMA
		if (dst_odd == TRUE) {
			dst_odd = FALSE;
		} else {
			dst_odd = TRUE;
			bcatch->dst_chroma_ptr += bcatch->dst_line_size;
		}
		#endif
	}

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return SUCCESS;
}

/*******************************************************************************
* NAME:		set_exp_members()
*
* USAGE:	Set the members of the Bit-BLT context attachment that are used to
*			expand 1 bit source pixels into the proper bit depth and alignment
*			for the destination.
*
*			The destination is logically shifted to the right GDV_IOBLT_WORDSIZ
*			bytes to make sure there is no overlap between source and destina-
*			tion when the expansion takes place. Remember that the expansion
*			is done in place. The src and dst are in the same buffer.
*/
static void set_exp_members(GDV_BCATCH *bcatch, GFX_POS srcx, GFX_POS dstx,
	GFX_DIMEN width)
{
	GFX_POS src_ex, exp_ex;
	GFX_DIMEN exp_width;
	u_int32 exp_ofs;

	/* Compute ending pixel in source (src_ex) and ending pixel to be */
	/* expanded (exp_ex). The latter is necessary because we always expand */
	/* whole bytes of source. */
	src_ex = srcx + width - 1;
	exp_ex = ((src_ex + 8) & ~7) - 1;
	exp_width = exp_ex - srcx + 1;

	/* Number of bytes of source to expand */
	bcatch->expd_count = (exp_width + 7) / 8;

	/* Pointer to last byte of 1 bit source */
	bcatch->expd_src_ptr = src_buffer +
		(((srcx & (bcatch->srcppw - 1)) + exp_width - 1) / 8);

	/* Compute offset in bits to the start of the last pixel of expanded */
	/* source */
	exp_ofs = ((dstx & (bcatch->dstppw - 1)) + exp_width - 1)
		<<  bcatch->dstbpp_shift;

	/* Pointer to last byte of expanded source (aligned with destination) */
	bcatch->expd_dst_ptr = src_buffer + GDV_IOBLT_WORDSIZ + (exp_ofs / 8);

	/* Determine whether the expanded source needs to be shifted to line up */
	/* with the destination. */
	bcatch->shift_direction = GDV_SHIFT_NONE;
	if ((bcatch->dstbpp_shift == 2) && (exp_ofs & 7) == 0) {  /* 4bpp */
		bcatch->shift_direction = GDV_SHIFT_LEFT;
	} else if (bcatch->dstbpp_shift == 1) { /* 2bpp */
	          if ((bcatch->expd_shift_size = ((dstx + exp_width) & 0x3)) != 0)
		    bcatch->shift_direction = GDV_SHIFT_LEFT;
	}
}

/*******************************************************************************
* NAME:		expd_src_1to2()
*
* USAGE:	Expand source pixels from 1 bit to 2 bit per pixel. The processing
*			is done from the end of the line to the beginning.
*
*			The source buffer passed in is a whole number of GDV_WORDs and they
*			start on a valid GDV_WORD boundry in the source. This function
*			takes this buffer and expands it in place to 2 bit pixels.
*
*			The result are already aligned at the byte level with the
*			destination (see computation of "bcatch->expd_dst_ptr" above).
*			However the nibbles within the byte my not be aligned between the
*			expanded source and the destination. In the latter case, we always
*			shift the nibbles to the left.
*/
void expd_src_1to2(BLT_CONTEXT *bc, GDV_BCATCH *bcatch)
{
	u_int32 b, count = bcatch->expd_count;
	u_char srcpix;
	u_char *srcptr = bcatch->expd_src_ptr;
	u_char *dstptr = bcatch->expd_dst_ptr;

	/* Switch based on the sub-byte alignment between the expanded source and */
	/* the destination. */
	switch (bcatch->shift_direction) {

		/* Expanded source is aligned with the destination */
		case GDV_SHIFT_NONE:
		while (count--) {
			srcpix = *srcptr--;
			for (b = 2; b; b--) {
				*dstptr    = (bc->exptbl[(srcpix & 0x01)] & 0x03) << 0;  srcpix >>= 1;
				*dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 2;  srcpix >>= 1;
				*dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 4;  srcpix >>= 1;
				*dstptr-- |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 6;  srcpix >>= 1;
			}
		}
		break;

		/* Expanded source must be shifted to the to be aligned */
		case GDV_SHIFT_LEFT:
		*dstptr = 0;
		while (count--) {
			srcpix = *srcptr--;
			switch (bcatch->expd_shift_size) {
			case 1:
			  *dstptr-- |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 6;	srcpix >>= 1;
			  *dstptr    = (bc->exptbl[(srcpix & 0x01)] & 0x03) << 0;	srcpix >>= 1;
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 2;	srcpix >>= 1;
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 4;	srcpix >>= 1;
			  *dstptr-- |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 6;	srcpix >>= 1;
			  *dstptr    = (bc->exptbl[(srcpix & 0x01)] & 0x03) << 0;	srcpix >>= 1;
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 2;	srcpix >>= 1;
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 4;
			break;
			case 2:
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 4;	srcpix >>= 1;
			  *dstptr-- |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 6;	srcpix >>= 1;
			  *dstptr    = (bc->exptbl[(srcpix & 0x01)] & 0x03) << 0;	srcpix >>= 1;
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 2;	srcpix >>= 1;
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 4;	srcpix >>= 1;
			  *dstptr-- |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 6;	srcpix >>= 1;
			  *dstptr    = (bc->exptbl[(srcpix & 0x01)] & 0x03) << 0;	srcpix >>= 1;
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 2;
			break;
			case 3:
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 2;	srcpix >>= 1;
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 4;	srcpix >>= 1;
			  *dstptr-- |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 6;	srcpix >>= 1;
			  *dstptr    = (bc->exptbl[(srcpix & 0x01)] & 0x03) << 0;	srcpix >>= 1;
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 2;	srcpix >>= 1;
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 4;	srcpix >>= 1;
			  *dstptr-- |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 6;	srcpix >>= 1;
			  *dstptr    = (bc->exptbl[(srcpix & 0x01)] & 0x03) << 0;
			break;
			default:   /* Should not happend */
			  *dstptr    = (bc->exptbl[(srcpix & 0x01)] & 0x03) << 0;	srcpix >>= 1;
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 2;	srcpix >>= 1;
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 4;	srcpix >>= 1;
			  *dstptr-- |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 6;	srcpix >>= 1;
			  *dstptr    = (bc->exptbl[(srcpix & 0x01)] & 0x03) << 0;	srcpix >>= 1;
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 2;	srcpix >>= 1;
			  *dstptr   |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 4;	srcpix >>= 1;
			  *dstptr-- |= (bc->exptbl[(srcpix & 0x01)] & 0x03) << 6;
			break;
			}
		}
		break;

	}
}

/*******************************************************************************
* NAME:		expd_src_1to4()
*
* USAGE:	Expand source pixels from 1 bit to 4 bit per pixel. The processing
*			is done from the end of the line to the beginning.
*
*			The source buffer passed in is a whole number of GDV_WORDs and they
*			start on a valid GDV_WORD boundry in the source. This function
*			takes this buffer and expands it in place to 4 bit pixels.
*
*			The result are already aligned at the byte level with the
*			destination (see computation of "bcatch->expd_dst_ptr" above).
*			However the nibbles within the byte my not be aligned between the
*			expanded source and the destination. In the latter case, we always
*			shift the nibbles to the left.
*/
void expd_src_1to4(BLT_CONTEXT *bc, GDV_BCATCH *bcatch)
{
	u_int32 b, count = bcatch->expd_count;
	u_char srcpix;
	u_char *srcptr = bcatch->expd_src_ptr;
	u_char *dstptr = bcatch->expd_dst_ptr;

	/* Switch based on the sub-byte alignment between the expanded source and */
	/* the destination. */
	switch (bcatch->shift_direction) {

		/* Expanded source is aligned with the destination */
		case GDV_SHIFT_NONE:
		while (count--) {
			srcpix = *srcptr--;
			for (b = 4; b; b--) {
				*dstptr = (bc->exptbl[(srcpix & 0x01)]) & 0x0f;
				srcpix >>= 1;
				*dstptr-- |= (bc->exptbl[(srcpix & 0x01)]) << 4;
				srcpix >>= 1;
			}
		}
		break;

		/* Expanded source must be shifted to the to be aligned */
		case GDV_SHIFT_LEFT:
		*dstptr = 0;
		while (count--) {
			srcpix = *srcptr--;
			*dstptr-- |= (bc->exptbl[(srcpix & 0x01)]) << 4;	srcpix >>= 1;
			*dstptr    = (bc->exptbl[(srcpix & 0x01)]) & 0x0f;	srcpix >>= 1;
			*dstptr-- |= (bc->exptbl[(srcpix & 0x01)]) << 4;	srcpix >>= 1;
			*dstptr    = (bc->exptbl[(srcpix & 0x01)]) & 0x0f;	srcpix >>= 1;
			*dstptr-- |= (bc->exptbl[(srcpix & 0x01)]) << 4;	srcpix >>= 1;
			*dstptr    = (bc->exptbl[(srcpix & 0x01)]) & 0x0f;	srcpix >>= 1;
			*dstptr-- |= (bc->exptbl[(srcpix & 0x01)]) << 4;	srcpix >>= 1;
			*dstptr    = (bc->exptbl[(srcpix & 0x01)]) & 0x0f;
		}
		break;

	}
}

/*******************************************************************************
* NAME:		expd_src_1to8()
*
* USAGE:	Expand source pixels from 1 bit to 8 bit per pixel. The processing
*			is done from the end of the line to the beginning.
*
*			The source buffer passed in is a whole number of GDV_WORDs and they
*			start on a valid GDV_WORD boundry in the source. This function
*			takes this buffer and expands it in place to 8 bit pixels. The
*			result are already aligned with the destination (see computation
*			of "bcatch->expd_dst_ptr" above) so they do not need to be shifted.
*/
void expd_src_1to8(BLT_CONTEXT *bc, GDV_BCATCH *bcatch)
{
	u_int32 b, count = bcatch->expd_count;
	u_char srcpix;
	u_char *srcptr = bcatch->expd_src_ptr;
	u_char *dstptr = bcatch->expd_dst_ptr;

	/* Process each byte to be expanded */
	while (count--) {
		srcpix = *srcptr--;

		/* Expand each 1 bit pixel to an 8 bit pixel */
		for (b = 8; b; b--) {
			*dstptr-- = bc->exptbl[(srcpix & 0x01)];
			srcpix >>= 1;
		}
	}
}

/*******************************************************************************
* NAME:		expd_src_1to16()
*
* USAGE:	Expand source pixels from 1 bit to 16 bit per pixel. The processing
*			is done from the end of the line to the beginning.
*
*			The source buffer passed in is a whole number of GDV_WORDs and they
*			start on a valid GDV_WORD boundry in the source. This function
*			takes this buffer and expands it in place to 16 bit pixels. The
*			result are already aligned with the destination (see computation
*			of "bcatch->expd_dst_ptr" above) so they do not need to be shifted.
*/
void expd_src_1to16(BLT_CONTEXT *bc, GDV_BCATCH *bcatch)
{
	u_int32 b, count = bcatch->expd_count;
	u_char srcpix;
	u_char *srcptr = bcatch->expd_src_ptr;
	u_char *dstptr = bcatch->expd_dst_ptr;

	/* Process each byte to be expanded */
	while (count--) {
		srcpix = *srcptr--;

		/* Expand each 1 bit pixel to a 16 bit pixel */
		for (b = 8; b; b--) {
			*dstptr-- = bc->exptbl[(srcpix & 0x01)];
			*dstptr-- = (bc->exptbl[(srcpix & 0x01)]) >> 8;
			srcpix >>= 1;
		}
	}
}

/*******************************************************************************
* NAME:		expd_src_1to32()
*
* USAGE:	Expand source pixels from 1 bit to 32 bit per pixel. The processing
*			is done from the end of the line to the beginning.
*
*			The source buffer passed in is a whole number of GDV_WORDs and they
*			start on a valid GDV_WORD boundry in the source. This function
*			takes this buffer and expands it in place to 32 bit pixels. The
*			result are already aligned with the destination (see computation
*			of "bcatch->expd_dst_ptr" above) so they do not need to be shifted.
*/
void expd_src_1to32(BLT_CONTEXT *bc, GDV_BCATCH *bcatch)
{
	u_int32 b, count = bcatch->expd_count;
	u_char srcpix;
	u_char *srcptr = bcatch->expd_src_ptr;
	u_char *dstptr = bcatch->expd_dst_ptr;

	/* Process each byte to be expanded */
	while (count--) {
		srcpix = *srcptr--;

		/* Expand each 1 bit pixel to a 32 bit pixel */
		for (b = 8; b; b--) {
			*dstptr-- = bc->exptbl[(srcpix & 0x01)];
			*dstptr-- = (bc->exptbl[(srcpix & 0x01)]) >> 8;
			*dstptr-- = (bc->exptbl[(srcpix & 0x01)]) >> 16;
			*dstptr-- = (bc->exptbl[(srcpix & 0x01)]) >> 24;
			srcpix >>= 1;
		}
	}
}

#ifdef GDV_IOBLT_SEP_CHROMA
/*******************************************************************************
* NAME:		expd_src_1to16_420()
*
* USAGE:	Expand source pixels from 1 bit to 16 bit per pixel. The 16 bit
*			pixel pairs contain interleaved chroma data (e.g. Cr and Cb). The
*			expansion algorithm here gives favor to the chroma of foreground
*			pixels (bits with the value 1) over those of the background (bits
*			with a value of 0) when they are mixed in the same GDV_WORD (pixel
*			pair). The processing is done from the end of the line to the
*			beginning. 
*
*			The source buffer passed in is a whole number of GDV_WORDs and they
*			start on a valid GDV_WORD boundry in the source. This function
*			takes this buffer and expands it in place to 16 bit pixels. The
*			result are already aligned with the destination (see computation
*			of "bcatch->expd_dst_ptr" above) so they do not need to be shifted.
*/
void expd_src_1to16_420(BLT_CONTEXT *bc, GDV_BCATCH *bcatch)
{
	u_int32 b, count = bcatch->expd_count;
	u_char srcpix;
	u_char *srcptr = bcatch->expd_src_ptr;
	u_char *dstptr = bcatch->expd_dst_ptr;
	GFX_PIXEL pix;

	/* Process each byte to be expanded */
	while (count--) {
		srcpix = *srcptr--;

		/* Expand each 2 bit pixel pair to a 32 bit pixel pair */
		for (b = 4; b; b--) {
			pix = bc->exptbl[(srcpix & 0x01)];
			*dstptr-- = pix;					/* Cb */
			*dstptr-- = pix >> 16;				/* Y  */
			*dstptr-- = pix >> 8;				/* Cr */
			srcpix >>= 1;

			pix = bc->exptbl[(srcpix & 0x01)];
			*dstptr-- = pix >> 16;				/* Y  */
			if ((srcpix & 0x01) == 1) {
				*(dstptr + 2) = pix >> 8;		/* Cr */
				*(dstptr + 3) = pix;			/* Cb */
			}
			srcpix >>= 1;
		}
	}
}
#endif
