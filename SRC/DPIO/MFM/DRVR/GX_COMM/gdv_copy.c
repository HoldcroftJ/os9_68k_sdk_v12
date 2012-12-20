/*******************************************************************************
* DESCRIPTION :
*
*	I/O based Bit-BLT functions for copy operations.
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

/*******************************************************************************
* NAME:		_gdv_ioblt_copyblk()
*
* USAGE:	Copy a block of pixels from the source drawmap to the destination
*			drawmap.
*/
error_code _gdv_ioblt_copyblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
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
        /* the next potential call to blt_next_copy_block(). Return if */
        /* either of them is zero. This is the first place validation */
        /* happens when driver is handling copy Blit-BLT operations */
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
	bcatch->src_odd = (srcy & 1) ? FALSE : TRUE;
	bcatch->dst_odd = (dsty & 1) ? FALSE : TRUE;
	bcatch->src_chroma_line_ptr = bcatch->src_chroma_dmap_ptr +
		((srcy / 2) * (bcatch->src_line_size));
	bcatch->dst_chroma_line_ptr = bcatch->dst_chroma_dmap_ptr +
		((dsty / 2) * (bcatch->dst_line_size));
	#endif

	/* Additional setup for RWM mixing mode. */
	if (bc->cpymix == BLT_MIX_RWM) {
		#ifdef GDV_IOBLT_OFFSETS
		if (bcatch->msk_odd_ofs != bcatch->msk_even_ofs) {
			bcatch->msk_line_ptr = (u_char *)(bc->mask_dmap->pixmem) +
				((srcy / 2) * (bcatch->src_line_size)) +
				((bcatch->src_odd) ? bcatch->msk_odd_ofs : bcatch->msk_even_ofs);
		} else {
			bcatch->msk_line_ptr = (u_char *)(bc->mask_dmap->pixmem) +
				(srcy * (bcatch->src_line_size));
		}
		#else
		bcatch->msk_line_ptr = (u_char *)(bc->mask_dmap->pixmem) +
			(srcy * (bcatch->src_line_size));
		#endif
	}

	/* Call the continuation function */
	ec = _gdv_ioblt_copynblk(bc, dstx, srcx, height);

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}

/*******************************************************************************
* NAME:		_gdv_ioblt_copynblk()
*
* USAGE:	Copy the next block of pixels from the source drawmap to the desti-
*			nation drawmap.
*/
error_code _gdv_ioblt_copynblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx,
	GFX_DIMEN height)
{
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif
	GDV_BCATCH *bcatch = bc->gdv_attachment;
	u_char *src_buf;
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
        /* copy Blit-BLT operations */
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
	set_sft_members(bcatch, srcx, dstx);

	/* Additional setup for RWM mode. */
	if (bc->cpymix == BLT_MIX_RWM) {
		bcatch->msk_ptr = bcatch->msk_line_ptr +
			(srcx >> bcatch->srcppw_shift) * GDV_IOBLT_WORDSIZ;
	}

	/* Process each line in the block */
	while (height-- != 0) {

		/* Read all source pixels into the source buffer and shift them so */
		/* that they line-up with where they will be written to in the */
		/* destination. */
		#ifdef GDV_IOBLT_SEP_CHROMA
		bcatch->ioblt_read_src(bc, bcatch->src_ptr, bcatch->src_chroma_ptr,
			src_buffer, bcatch->src_count);
		#else
		bcatch->ioblt_read_src(bc, bcatch->src_ptr, src_buffer,
			bcatch->src_count);
		#endif
		if (bcatch->shift_direction != GDV_SHIFT_NONE) {
			shift_src_buffer(bcatch, src_buffer);
		}
		src_buf = src_buffer + bcatch->shift_byte_ofs;

		/* Mix the source and destination pixels using the proper mixing */
		/* function, then write the results to the destination. */
		if (bc->cpymix == BLT_MIX_REPLACE) {

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
				mix_word_replace(src_buf, dst_buffer, bcatch->dst_beg_maskp);
			}
			if (bcatch->dst_end_maskp) {
				#ifdef GDV_IOBLT_SEP_CHROMA
				bcatch->ioblt_read_dst(bc, bcatch->dst_ptr + ofs,
					bcatch->dst_chroma_ptr, dst_buffer, 1);
				#else
				bcatch->ioblt_read_dst(bc, bcatch->dst_ptr + ofs, dst_buffer,
					1);
				#endif
				mix_word_replace(src_buf + ofs, dst_buffer,
					bcatch->dst_end_maskp);
			}
			#ifdef GDV_IOBLT_SEP_CHROMA
			bcatch->ioblt_write_dst(bc, bcatch->dst_ptr, bcatch->dst_chroma_ptr,
				src_buf, bcatch->dst_count);
			#else
			bcatch->ioblt_write_dst(bc, bcatch->dst_ptr, src_buf,
				bcatch->dst_count);
			#endif

		} else {

			/* Additional setup for RWM. Read the mask data into the mask */
			/* buffer. */
			if (bc->cpymix == BLT_MIX_RWM) {
				bcatch->ioblt_read_msk(bc, bcatch->msk_ptr, msk_buffer,
					bcatch->src_count);
				if (bcatch->shift_direction != GDV_SHIFT_NONE) {
					shift_src_buffer(bcatch, msk_buffer);
				}
				bcatch->msk_buf = msk_buffer + bcatch->shift_byte_ofs;
			}

			/* All modes except replace mode. Read the destination pixels */
			/* into the destination buffer, then mix the buffers and write */
			/* the results. */
			#ifdef GDV_IOBLT_SEP_CHROMA
			bcatch->ioblt_read_dst(bc, bcatch->dst_ptr, bcatch->dst_chroma_ptr,
				dst_buffer, bcatch->dst_count);
			bcatch->ioblt_cpymix(bcatch, src_buf, GDV_IOBLT_WORDSIZ, dst_buffer,
				bcatch->dst_beg_maskp, bcatch->dst_mid_count,
				bcatch->dst_end_maskp);
			bcatch->ioblt_write_dst(bc, bcatch->dst_ptr, bcatch->dst_chroma_ptr,
				dst_buffer, bcatch->dst_count);
			#else
			bcatch->ioblt_read_dst(bc, bcatch->dst_ptr, dst_buffer,
				bcatch->dst_count);
			bcatch->ioblt_cpymix(bcatch, src_buf, GDV_IOBLT_WORDSIZ, dst_buffer,
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
		if (bcatch->msk_odd_ofs != bcatch->msk_even_ofs) {
			if (src_odd == FALSE) {
				bcatch->msk_ptr += bcatch->msk_even_ofs - bcatch->msk_odd_ofs;
			} else {
				bcatch->msk_ptr += bcatch->msk_odd_ofs - bcatch->msk_even_ofs +
					bcatch->src_line_size;
			}
		} else {
			bcatch->msk_ptr += bcatch->src_line_size;
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
		bcatch->msk_ptr += bcatch->src_line_size;
		bcatch->dst_ptr += bcatch->dst_line_size;
		#endif
		#ifdef GDV_IOBLT_SEP_CHROMA
		if (src_odd == TRUE) {
			src_odd = FALSE;
		} else {
			src_odd = TRUE;
			bcatch->src_chroma_ptr += bcatch->src_line_size;
		}
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
* NAME:		set_src_members()
*
* USAGE:	Set the members of the Bit-BLT context attachment that are affected
*			by the source variables for the operation.
*/
void set_src_members(GDV_BCATCH *bcatch, GFX_POS srcx, GFX_DIMEN width)
{
	GFX_POS src_ex;					/* End X coordinate for source */
	u_int32 src_beg_ofs;			/* Offset to beg src GDV_WORD in bytes */
	u_int32 src_end_ofs;			/* Offset to end src GDV_WORD in bytes */

	/* Set all members affected by source variables */
	src_ex = srcx + width - 1;
	src_beg_ofs = (srcx >> bcatch->srcppw_shift) * GDV_IOBLT_WORDSIZ;
	src_end_ofs = (src_ex >> bcatch->srcppw_shift) * GDV_IOBLT_WORDSIZ;
	#ifdef GDV_IOBLT_SEP_CHROMA
	bcatch->src_ptr = bcatch->src_line_ptr + (src_beg_ofs / 2);
	#else
	bcatch->src_ptr = bcatch->src_line_ptr + src_beg_ofs;
	#endif
	bcatch->src_count = ((src_end_ofs - src_beg_ofs) >> GDV_IOBLT_WORDSFT) + 1;

	/* Compute variables for separate luma/chroma */
	#ifdef GDV_IOBLT_SEP_CHROMA
	bcatch->src_chroma_ptr = bcatch->src_chroma_line_ptr + (src_beg_ofs / 2);
	#endif
}

/*******************************************************************************
* NAME:		set_dst_members()
*
* USAGE:	Set the members of the Bit-BLT context attachment that are affected
*			by the destination variables for the operation.
*/
void set_dst_members(GDV_BCATCH *bcatch, GFX_POS dstx, GFX_DIMEN width)
{
	GFX_POS dst_ex;					/* End X coordinate for destination */
	u_int32 dst_beg_ofs;			/* Offset to beg dst GDV_WORD in bytes */
	u_int32 dst_end_ofs;			/* Offset to end dst GDV_WORD in bytes */
	u_int32 dst_beg_idx;			/* Index of beg dst pixel in GDV_WORD */
	u_int32 dst_end_idx;			/* Index of end dst pixel in GDV_WORD */

	/* Set all members affected by destination variables */
	dst_ex = dstx + width - 1;
	dst_beg_ofs = (dstx >> bcatch->dstppw_shift) * GDV_IOBLT_WORDSIZ;
	dst_end_ofs = (dst_ex >> bcatch->dstppw_shift) * GDV_IOBLT_WORDSIZ;
	dst_beg_idx = dstx & (bcatch->dstppw - 1);
	dst_end_idx = dst_ex & (bcatch->dstppw - 1);
	#ifdef GDV_IOBLT_SEP_CHROMA
	bcatch->dst_ptr = bcatch->dst_line_ptr + (dst_beg_ofs / 2);
	#else
	bcatch->dst_ptr = bcatch->dst_line_ptr + dst_beg_ofs;
	#endif
	bcatch->dst_count = ((dst_end_ofs - dst_beg_ofs) >> GDV_IOBLT_WORDSFT) + 1;

	/* Compute mask for beginning and ending GDV_WORDs and number of middle */
	/* GDV_WORDs. */
	bcatch->dst_mid_count = bcatch->dst_count;
	if ((dst_beg_idx == 0) && (bcatch->dst_mid_count != 1)) {
		bcatch->dst_beg_maskp = NULL;
	} else {
		bcatch->dst_beg_maskp = &(bcatch->dst_beg_mask);
		if (--bcatch->dst_mid_count == 0) {
			compute_word_mask(bcatch->dst_beg_maskp,
				dst_beg_idx << bcatch->dstbpp_shift,
				((dst_end_idx + 1) << bcatch->dstbpp_shift) - 1);
		} else {
			compute_word_mask(bcatch->dst_beg_maskp, dst_beg_idx <<
				bcatch->dstbpp_shift, (GDV_IOBLT_WORDSIZ << 3) - 1);
		}
	}
	if (((((dst_end_idx + 1) << bcatch->dstbpp_shift) >> 3) ==
		GDV_IOBLT_WORDSIZ) || (bcatch->dst_mid_count == 0)) {
		bcatch->dst_end_maskp = NULL;
	} else {
		bcatch->dst_mid_count--;
		bcatch->dst_end_maskp = &(bcatch->dst_end_mask);
		compute_word_mask(bcatch->dst_end_maskp, 0,
			((dst_end_idx + 1) << bcatch->dstbpp_shift) - 1);
	}

	/* Compute variables for separate luma/chroma */
	#ifdef GDV_IOBLT_SEP_CHROMA
	bcatch->dst_chroma_ptr = bcatch->dst_chroma_line_ptr + (dst_beg_ofs / 2);
	#endif
}

/*******************************************************************************
* NAME:		compute_word_mask()
*
* USAGE:	Compute the mask for a GDV_WORD. All bits from "start_bit" to
*			"end_bit" inclusive are set to 1. All others are 0. The maximum
*			bit number is ((GDV_IOBLT_WORDSIZ * 8) - 1). This maximum must
*			be observed, but is not enforced here (for the sake of speed).
*
*			Bits are numbered from 0 to ((GDV_IOBLT_WORDSIZ * 8) - 1) where
*			bit 0 is the left-most bit in the first byte.
*/
void compute_word_mask(GDV_WORD *mask, u_int32 start_bit, u_int32 end_bit)
{
	u_int8 *ptr = (u_int8 *)mask;
	u_int32 start_count, middle_count, end_count;

	/* Compute size of start, middle, and end areas in bytes. */
	start_count = start_bit >> 3;
	end_count = ((GDV_IOBLT_WORDSIZ * 8) - end_bit - 1) >> 3;
	middle_count = GDV_IOBLT_WORDSIZ - start_count - end_count;

	/* Set start bytes to zeros */
	while (start_count--) {
		*ptr++ = 0;							/* Start bytes (all zeros) */
	}

	/* Set middle bytes */
	if (middle_count == 1) {
		/* Combined start/end partial byte */
		*ptr++ = (0xff >> (start_bit & 0x7)) & (0xff << (7 - (end_bit & 0x7)));
	} else {
		*ptr++ = 0xff >> (start_bit & 0x7);		/* Start partial byte */
		middle_count -= 2;
		while (middle_count--) {
			*ptr++ = 0xff;						/* Middle bytes (all ones) */
		}
		*ptr++ = 0xff << (7 - (end_bit & 0x7));	/* End partial byte */
	}

	/* Set end bytes to zeros */
	while (end_count--) {
		*ptr++ = 0;							/* End bytes (all zeros) */
	}
}

/*******************************************************************************
* NAME:		set_sft_members()
*
* USAGE:	Set the members of the Bit-BLT context attachment that are used
*			to shift source pixels to line them up with the destination.
*/
void set_sft_members(GDV_BCATCH *bcatch, GFX_POS srcx, GFX_POS dstx)
{
	u_int32 src_idx;				/* Index into source */
	u_int32 dst_idx;				/* Index into destination */

	/* Compute variables used to logically shift the source by whole bytes */
	/* to line it up with the destination. This value is added to the */
	/* address of the source buffer to shift it without physically moving */
	/* the bytes. */
	/* The src_idx and dst_idx values here are byte indicies into the */
	/* GDV_WORD that contain the start pixel. */
	src_idx = ((srcx & (bcatch->srcppw - 1)) << bcatch->srcbpp_shift) / 8;
	dst_idx = ((dstx & (bcatch->dstppw - 1)) << bcatch->dstbpp_shift) / 8;
	bcatch->shift_byte_ofs = src_idx - dst_idx;

	/* Compute variables used to shift the source within each byte to line */
	/* it up with the destination. This is only done if source pixels are */
	/* smaller than a byte. */
	/* The src_idx and dst_idx values here are bit indicies into the */
	/* byte that contain the start pixel. */
	bcatch->shift_direction = GDV_SHIFT_NONE;
	if (bcatch->srcbpp_shift < 3) {
		bcatch->shift_process_ofs = src_idx;
		bcatch->shift_process_cnt =
			(((bcatch->width << bcatch->dstbpp_shift) + 7) / 8) + 1;
		src_idx = (srcx << bcatch->srcbpp_shift) & 0x07;
		dst_idx = (dstx << bcatch->dstbpp_shift) & 0x07;
		if (src_idx != dst_idx) {
			if (src_idx < dst_idx) {
				bcatch->shift_direction = GDV_SHIFT_RIGHT;
				bcatch->shift_down = dst_idx - src_idx;
				bcatch->shift_up = 8 - bcatch->shift_down;
				bcatch->shift_mask = 0xff >> bcatch->shift_down;
			} else {
				bcatch->shift_direction = GDV_SHIFT_LEFT;
				bcatch->shift_up = src_idx - dst_idx;
				bcatch->shift_down = 8 - bcatch->shift_up;
				bcatch->shift_mask = 0xff << bcatch->shift_up;
			}
		}
	}
}

/*******************************************************************************
* NAME:		shift_src_buffer()
*
* USAGE:	Shift the source pixels in the source buffer so that they line up
*			with where they are going to be written to in the destination.
*
*			The source is always read into the source buffer starting at the
*			first byte. To align bytes we simply point to the correct byte
*			within the first GDV_WORD of the buffer when it is written to the
*			destination. Therefore, all we need to do in this function is shift
*			the bits within each byte so that the source becomes bit-aligned
*			with the destination.
*
*			Do not call this function if the source does not need to be shifted.
*/
void shift_src_buffer(GDV_BCATCH *bcatch, u_char *srcbuf)
{
	u_char *p, *ep, nxt, tmp = 0;

	/* Switch based on the direction that we need to shift */
	switch (bcatch->shift_direction) {

		case GDV_SHIFT_RIGHT:	/* Shift to the right */
		p = srcbuf + bcatch->shift_process_ofs;
			ep = p + bcatch->shift_process_cnt;
			while (p != ep) {
				nxt = *p;
				*p = (((tmp << bcatch->shift_up)   & ~(bcatch->shift_mask)) |
					  ((*p  >> bcatch->shift_down) &  bcatch->shift_mask));
				tmp = nxt;
				p++;
			}
			break;

		case GDV_SHIFT_LEFT:	/* Shift to the left */
			p = srcbuf + bcatch->shift_process_ofs;
			ep = p + bcatch->shift_process_cnt;
			while (p != ep) {
				*p = (((*p     << bcatch->shift_up)   & bcatch->shift_mask) |
					  ((*(p+1) >> bcatch->shift_down) & ~(bcatch->shift_mask)));
				p++;
			}
			break;
	}
}

/*******************************************************************************
* NAME:		mix_word_replace()
*
* USAGE:	Mix the pixels in the specified source GDV_WORD with those in the
*			specified destintion GDV_WORD and leave the results in the source
*			GDV_WORD.
*/
void mix_word_replace(u_char *src_word, u_char *dst_word, GDV_WORD
	*mask_word)
{
	u_int32 j = 0;

	_asm(-1000,"");		/* Force this function to be inlined */

	/* Process each byte in the GDV_WORD */
	for (j = 0 ; j < GDV_IOBLT_WORDSIZ ; j++) {
		src_word[j] = (dst_word[j] & ~mask_word->bm[j]) |
			(src_word[j] & mask_word->bm[j]);
	}
}

/*******************************************************************************
* NAME:		MIX
*
* USAGE:	This is a macro that expands to a function that handles mixing
*			pixels in the source buffer with those in the destination buffer.
*			The results are placed in the destination buffer. The macro
*			references will invoke this macro once for each supported mixing
*			mode.
*
*			The "src_inc" is a source increment (in GDV_WORDs) and can be used
*			to control how much source you need to provide. When set to 0 only
*			one GDV_WORD of source is required. When set to GDV_IOBLT_WORDSIZ
*			you should provide a source buffer that is the same size as the
*			destination buffer. Do not use any values other than 0 or
*			GDV_IOBLT_WORDSIZ.
*/
#define MIX(MIXMODE, MIXOP1, MIXOP2, MIXOP3, NEXTOP)						\
void _gdv_ioblt_mix_##MIXMODE(_GDV_IOBLT_MIX_PARAMS)						\
{																			\
	u_int32 j = 0;															\
																			\
	/* Mix beginning GDV_WORD */											\
	if (beg_mask) {															\
		for (j = 0 ; j < GDV_IOBLT_WORDSIZ ; j++) {							\
			dst_buf[j] = (dst_buf[j] & ~beg_mask->bm[j]) |					\
				((MIXOP1) & beg_mask->bm[j]);								\
		}																	\
		dst_buf += GDV_IOBLT_WORDSIZ;										\
		src_buf += src_inc;													\
		NEXTOP																\
	}																		\
																			\
	/* Mix middle GDV_WORDs */												\
	while (midcount--) {													\
		for (j = 0 ; j < GDV_IOBLT_WORDSIZ ; j++) {							\
			dst_buf[j] = MIXOP2;											\
		}																	\
		dst_buf += GDV_IOBLT_WORDSIZ;										\
		src_buf += src_inc;													\
		NEXTOP																\
	}																		\
																			\
	/* Mix ending GDV_WORD */												\
	if (end_mask) {															\
		for (j = 0 ; j < GDV_IOBLT_WORDSIZ ; j++) {							\
			dst_buf[j] = (dst_buf[j] & ~end_mask->bm[j]) |					\
				((MIXOP3) & end_mask->bm[j]);								\
		}																	\
	}																		\
}

#define SRC (src_buf[j])
#define SRC_BEG (src_buf[j] & beg_mask->bm[j])
#define SRC_END (src_buf[j] & end_mask->bm[j])
#define OFS (bcatch->ofspix_word.bm[j])
#define MSK (bcatch->msk_buf[j])
#define DST (dst_buf[j])
#define TRAN (bcatch->transpix_word.bm[j])

#define TRANS4																\
	(((SRC & 0xf0) == (TRAN & 0xf0)) ? (DST & 0xf0) : (SRC & 0xf0)) +		\
	(((SRC & 0x0f) == (TRAN & 0x0f)) ? (DST & 0x0f) : (SRC & 0x0f))

#define TRANS8																\
	((SRC == TRAN) ? DST : SRC)

#define NOP	;
#define MSK_NEXTOP bcatch->msk_buf += GDV_IOBLT_WORDSIZ;

#define MIX1(MIXMODE, MIXOP)												\
	MIX(MIXMODE, MIXOP, MIXOP, MIXOP, NOP)
#define MIX2(MIXMODE, MIXOP1, MIXOP2, MIXOP3)								\
	MIX(MIXMODE, MIXOP1, MIXOP2, MIXOP3, NOP)
#define MIX3(MIXMODE, MIXOP, NEXTOP)										\
	MIX(MIXMODE, MIXOP, MIXOP, MIXOP, NEXTOP)

MIX1(sxd, SRC ^ DST)
MIX1(n_sxd, ~(SRC ^ DST))
MIX1(sod, SRC | DST)
MIX1(n_sod, ~(SRC | DST))
MIX1(ns_ad, ~SRC & DST)
MIX1(so_nd, SRC | ~DST)
MIX2(spd, SRC_BEG + DST, SRC + DST, SRC_END + DST)
MIX2(dms, DST - SRC_BEG, DST - SRC, DST - SRC_END)
MIX2(spo, SRC_BEG + OFS, SRC + OFS, SRC_END + OFS)
MIX1(rwt4, TRANS4)
MIX1(rwt8, TRANS8)
MIX3(rwm, (SRC & MSK) | (DST & ~MSK), MSK_NEXTOP)

