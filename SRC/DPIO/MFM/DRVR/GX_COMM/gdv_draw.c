/*******************************************************************************
* DESCRIPTION :
*
*	I/O based Bit-BLT functions for draw operations.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  01/09/96  TJE  First version
*/
#include <gdv_priv.h>

/*******************************************************************************
* NAME:		_gdv_ioblt_drawblk
*
* USAGE:	Draw a block of pixels.
*/
error_code _gdv_ioblt_drawblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN width, GFX_DIMEN height)
{
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif
	GDV_BCATCH *bcatch = bc->gdv_attachment;
	u_int32 ofs;
	GDV_WORD mix_word;

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

        /* Validate "width" and "height" parameters before processing */
        /* further. Return immediately if either of them is zero. This is */
        /* the only place validation happens when driver is handling draw */
        /* Blit-BLT operations */
        if ((width == 0) || (height == 0)) {
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
	#ifdef GDV_IOBLT_OFFSETS
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
	bcatch->dst_line_ptr = (u_char *)(bc->dstdmap->pixmem) +
		(dsty * (bcatch->dst_line_size));
	#endif
	#ifdef GDV_IOBLT_SEP_CHROMA
	bcatch->dst_odd = (dsty & 1) ? FALSE : TRUE;
	bcatch->dst_chroma_line_ptr = bcatch->dst_chroma_dmap_ptr +
		((dsty / 2) * (bcatch->dst_line_size));
	#endif
	set_dst_members(bcatch, dstx, width);

	/* Process each line in the block */
	while (height-- != 0) {

		/* Mix the the solid color with the destination pixels using the */
		/* proper mixing function. */
		if (bc->drwmix == BLT_MIX_REPLACE) {

			/* Replace mode. Mix the first GDV_WORD of the destination with */
			/* a GDV_WORD of the solid color and write the results. */
			if (bcatch->dst_beg_maskp) {
				mix_word = bcatch->curpix_word;
				#ifdef GDV_IOBLT_SEP_CHROMA
                bcatch->ioblt_read_dst(bc, bcatch->dst_ptr,
					bcatch->dst_chroma_ptr, dst_buffer, 1);
                mix_word_replace((u_char *)(&mix_word), dst_buffer,
                    bcatch->dst_beg_maskp);
                bcatch->ioblt_write_dst(bc, bcatch->dst_ptr,
					bcatch->dst_chroma_ptr, (u_char *)(&mix_word), 1);
                ofs = GDV_IOBLT_WORDSIZ / 2;
                #else
                bcatch->ioblt_read_dst(bc, bcatch->dst_ptr, dst_buffer, 1);
                mix_word_replace((u_char *)(&mix_word), dst_buffer,
                    bcatch->dst_beg_maskp);
                bcatch->ioblt_write_dst(bc, bcatch->dst_ptr,
                    (u_char *)(&mix_word), 1);
                ofs = GDV_IOBLT_WORDSIZ;
                #endif
            } else {
                ofs = 0;
            }

            /* Write the middle GDV_WORDs with the solid color */
            if (bcatch->dst_mid_count) {
                #ifdef GDV_IOBLT_SEP_CHROMA
                bcatch->ioblt_write_dstpix(bc, bcatch->dst_ptr + ofs,
					bcatch->dst_chroma_ptr, &bcatch->curpix_word,
                    bcatch->dst_mid_count);
                #else
                bcatch->ioblt_write_dstpix(bc, bcatch->dst_ptr + ofs,
                    &bcatch->curpix_word, bcatch->dst_mid_count);
                #endif
            }

            /* Mix the last GDV_WORD of the destination with a GDV_WORD of */
            /* the solid color and write the results. */
            if (bcatch->dst_end_maskp) {
                mix_word = bcatch->curpix_word;
                #ifdef GDV_IOBLT_SEP_CHROMA
                ofs = (bcatch->dst_count - 1) * (GDV_IOBLT_WORDSIZ / 2);
                bcatch->ioblt_read_dst(bc, bcatch->dst_ptr + ofs,
                    bcatch->dst_chroma_ptr, dst_buffer, 1);
                mix_word_replace((u_char *)(&mix_word), dst_buffer,
                    bcatch->dst_end_maskp);
                bcatch->ioblt_write_dst(bc, bcatch->dst_ptr + ofs,
                    bcatch->dst_chroma_ptr, (u_char *)(&mix_word), 1);
				#else
                ofs = (bcatch->dst_count - 1) * GDV_IOBLT_WORDSIZ;
				bcatch->ioblt_read_dst(bc, bcatch->dst_ptr + ofs, dst_buffer,
					1);
				mix_word_replace((u_char *)(&mix_word), dst_buffer,
					bcatch->dst_end_maskp);
				bcatch->ioblt_write_dst(bc, bcatch->dst_ptr + ofs,
					(u_char *)(&mix_word), 1);
				#endif
			}

		} else {

			/* All modes except replace mode. Read the destination pixels */
			/* into the destination buffer, then mix the solid color into */
			/* the destination buffer and write the results. */
			#ifdef GDV_IOBLT_SEP_CHROMA
			bcatch->ioblt_read_dst(bc, bcatch->dst_ptr, bcatch->dst_chroma_ptr,
				dst_buffer, bcatch->dst_count);
			bcatch->ioblt_drwmix(bcatch, (u_char *)(&bcatch->curpix_word), 0,
				dst_buffer, bcatch->dst_beg_maskp, bcatch->dst_mid_count,
				bcatch->dst_end_maskp);
			bcatch->ioblt_write_dst(bc, bcatch->dst_ptr, bcatch->dst_chroma_ptr,
				dst_buffer, bcatch->dst_count);
			#else
			bcatch->ioblt_read_dst(bc, bcatch->dst_ptr, dst_buffer,
				bcatch->dst_count);
			bcatch->ioblt_drwmix(bcatch, (u_char *)(&bcatch->curpix_word), 0,
				dst_buffer, bcatch->dst_beg_maskp, bcatch->dst_mid_count,
				bcatch->dst_end_maskp);
			bcatch->ioblt_write_dst(bc, bcatch->dst_ptr, dst_buffer,
				bcatch->dst_count);
			#endif
		}

		/* Setup for next line */
		#ifdef GDV_IOBLT_OFFSETS
		if (bcatch->dst_odd_ofs != bcatch->dst_even_ofs) {
			if (bcatch->dst_odd == TRUE) {
				bcatch->dst_odd = FALSE;
				bcatch->dst_ptr += bcatch->dst_even_ofs - bcatch->dst_odd_ofs;
			} else {
				bcatch->dst_odd = TRUE;
				bcatch->dst_ptr += bcatch->dst_odd_ofs - bcatch->dst_even_ofs +
					bcatch->dst_line_size;
			}
		} else {
			bcatch->dst_ptr += bcatch->dst_line_size;
		}
		#else
		bcatch->dst_ptr += bcatch->dst_line_size;
		#endif
		#ifdef GDV_IOBLT_SEP_CHROMA
		if (bcatch->dst_odd == TRUE) {
			bcatch->dst_odd = FALSE;
		} else {
			bcatch->dst_odd = TRUE;
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
* NAME:		_gdv_ioblt_drawhline
*
* USAGE:	Draw a horizontal line of pixels.
*/
error_code _gdv_ioblt_drawhline(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN width)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif
	ec = _gdv_ioblt_drawblk(bc, dstx, dsty, width, 1);

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}

/*******************************************************************************
* NAME:		_gdv_ioblt_drawvline
*
* USAGE:	Draw a vertical line of pixels.
*/
error_code _gdv_ioblt_drawvline(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN height)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif
	ec = _gdv_ioblt_drawblk(bc, dstx, dsty, 1, height);

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}

/*******************************************************************************
* NAME:		_gdv_ioblt_drawpixel
*
* USAGE:	Draw a pixel.
*/
error_code _gdv_ioblt_drawpixel(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty)
{
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif
	GDV_BCATCH *bcatch = bc->gdv_attachment;
	GDV_WORD mix_word;

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

        /* Convert line buffer pointers */
	/* from system to user addresses */
	/* for the processors like SuperH and MIPS */
	#if defined(GDV_MEM_TOP_BIT)
	src_buffer = (u_char *)adrcvt(src_buffer);
	dst_buffer = (u_char *)adrcvt(dst_buffer);
	msk_buffer = (u_char *)adrcvt(msk_buffer);
	#endif

	/* Set necessary members of the Bit-BLT context attachment */
#ifdef GDV_IOBLT_OFFSETS
	if (bcatch->dst_odd_ofs != bcatch->dst_even_ofs) {
		bcatch->dst_odd = (dsty & 1) ? FALSE : TRUE;
		bcatch->dst_line_ptr = (u_char *)(bc->dstdmap->pixmem) +
			((dsty / 2) * (bcatch->dst_line_size)) +
				((bcatch->dst_odd) ? 
				 bcatch->dst_odd_ofs : bcatch->dst_even_ofs);
	} else {
		bcatch->dst_line_ptr = (u_char *)(bc->dstdmap->pixmem) +
			(dsty * (bcatch->dst_line_size));
	}
#else
	bcatch->dst_line_ptr = (u_char *)(bc->dstdmap->pixmem) +
		(dsty * (bcatch->dst_line_size));
#endif
#ifdef GDV_IOBLT_SEP_CHROMA
	bcatch->dst_chroma_line_ptr = bcatch->dst_chroma_dmap_ptr +
		((dsty / 2) * (bcatch->dst_line_size));
#endif
	set_dst_members(bcatch, dstx, 1);

	/* Mix the the solid color with the destination pixel using the */
	/* proper mixing function and write it. */
	if (bc->drwmix == BLT_MIX_REPLACE) {

		/* Replace mode */
		mix_word = bcatch->curpix_word;
		#ifdef GDV_IOBLT_SEP_CHROMA
		bcatch->ioblt_read_dst(bc, bcatch->dst_ptr, bcatch->dst_chroma_ptr,
			dst_buffer, 1);
		mix_word_replace((u_char *)(&mix_word), dst_buffer,
			bcatch->dst_beg_maskp);
		bcatch->ioblt_write_dst(bc, bcatch->dst_ptr, bcatch->dst_chroma_ptr,
			(u_char *)(&mix_word), 1);
		#else
		bcatch->ioblt_read_dst(bc, bcatch->dst_ptr, dst_buffer, 1);
		mix_word_replace((u_char *)(&mix_word), dst_buffer,
			bcatch->dst_beg_maskp);
		bcatch->ioblt_write_dst(bc, bcatch->dst_ptr, (u_char *)(&mix_word), 1);
		#endif

	} else {

		/* All modes except replace mode. */
		#ifdef GDV_IOBLT_SEP_CHROMA
		bcatch->ioblt_read_dst(bc, bcatch->dst_ptr, bcatch->dst_chroma_ptr,
			dst_buffer, 1);
		bcatch->ioblt_drwmix((bcatch, u_char *)(&bcatch->curpix_word), 0,
			dst_buffer, bcatch->dst_beg_maskp, 0, NULL);
		bcatch->ioblt_write_dst(bc, bcatch->dst_ptr, bcatch->dst_chroma_ptr,
			dst_buffer, 1);
		#else
		bcatch->ioblt_read_dst(bc, bcatch->dst_ptr, dst_buffer, 1);
		bcatch->ioblt_drwmix(bcatch, (u_char *)(&bcatch->curpix_word), 0,
			dst_buffer, bcatch->dst_beg_maskp, 0, NULL);
		bcatch->ioblt_write_dst(bc, bcatch->dst_ptr, dst_buffer, 1);
		#endif
	}

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return SUCCESS;
}

/*******************************************************************************
* NAME:		_gdv_ioblt_getpixel
*
* USAGE:	Get a pixel.
*/
error_code _gdv_ioblt_getpixel(GFX_PIXEL *ret_pixel, BLT_CONTEXT *bc, GFX_POS
	srcx, GFX_POS srcy)
{
#ifndef GDV_FE_SYSTATE
	GFX_DEV *gfxdev = bc->gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif
	GDV_BCATCH *bcatch = bc->gdv_attachment;
	u_int32 ofs;

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif
	/* Compute variables needed to extract the pixel */
#ifdef GDV_IOBLT_OFFSETS
	if (bcatch->src_odd_ofs != bcatch->src_even_ofs) {
		bcatch->src_odd = (srcy & 1) ? FALSE : TRUE;
		bcatch->src_line_ptr = (u_char *)(bc->srcdmap->pixmem) +
            ((srcy / 2) * (bcatch->src_line_size)) +
				((bcatch->src_odd) ? 
				 bcatch->src_odd_ofs : bcatch->src_even_ofs);
	} else {
		bcatch->src_line_ptr = (u_char *)(bc->srcdmap->pixmem) +
			(srcy * (bcatch->src_line_size));
	}
#else
	bcatch->src_line_ptr = (u_char *)(bc->srcdmap->pixmem) +
		(srcy * (bcatch->src_line_size));
#endif
	ofs = (srcx >> bcatch->srcppw_shift) * GDV_IOBLT_WORDSIZ;
	bcatch->src_ptr = bcatch->src_line_ptr + ofs;

	/* Get the GDV_WORD that contains the pixel and extract the pixel */
	bcatch->ioblt_read_src(bc, bcatch->src_ptr, src_buffer, 1);
	switch (bcatch->srcbpp_shift) {
		case 0:		/* 1 bpp */
			ofs = (srcx & (bcatch->srcppw - 1)) >> 3;
			*ret_pixel = (src_buffer[ofs] >> (7 - (srcx & 7))) & 0x1;
			break;
		case 1:		/* 2 bpp */
			ofs = (srcx & (bcatch->srcppw - 1)) >> 2;
			*ret_pixel = (src_buffer[ofs] >> (6 - ((srcx & 3) * 2))) & 0x3;
			break;
		case 2:		/* 4 bpp */
			ofs = (srcx & (bcatch->srcppw - 1)) >> 1;
			if ((srcx & 1) == 0) {
				*ret_pixel = src_buffer[ofs] >> 4;
			} else {
				*ret_pixel = src_buffer[ofs] & 0x0f;
			}
			break;
		case 3:		/* 8 bpp */
			*ret_pixel = src_buffer[srcx & (bcatch->srcppw - 1)];
			break;
		case 4:		/* 16 bpp */
			ofs = (srcx & (bcatch->srcppw - 1)) << 1;
			*ret_pixel = (src_buffer[ofs] << 8) + src_buffer[ofs + 1];
			break;
		case 5:		/* 32 bpp */
			ofs = (srcx & (bcatch->srcppw - 1)) << 2;
			*ret_pixel = (src_buffer[ofs] << 24) + (src_buffer[ofs + 1] << 16)
				+ (src_buffer[ofs + 2] << 8) + src_buffer[ofs + 3];
			break;
	}

#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return SUCCESS;
}

