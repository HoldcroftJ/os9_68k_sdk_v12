/*******************************************************************************
*
* DESCRIPTION :
*
*	I/O based Bit-BLT functions.
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
#include <defs.h>

/*******************************************************************************
* NAME:		fe_ioblt_gfxram()
*
* USAGE:	Return TRUE if the specified "pixmem" is in graphics RAM. Otherwise
*			return FALSE.
*/
BOOLEAN fe_ioblt_gfxram(BLT_CONTEXT *bc, GFX_PIXEL *pixmem)
{
	*?*?* CHECK THE PIXMEM POINTER TO SEE IF IT IS IN GRAPHICS RAM OR NOT.
}

/*******************************************************************************
* NAME:		fe_ioblt_offsets()
*
* USAGE:	Compute offsets to odd and even sections of the drawmap and return
*			them in "odd_offset" and "even_offset". Lines are numbered starting
*			with line 1. Therefore the first line is odd.
*/
void GDV_IOBLT_OFFSETS(const GFX_DMAP *dmap, int32 *odd_offset, int32
	*even_offset)
{
	*?*?* COMPUTE OFFSETS.
}

/*******************************************************************************
* NAME:		fe_ioblt_read()
*
* USAGE:	Read "word_count" GDV_WORDs from the source pointed to by "src_ptr"
*			into the destination buffer pointed to by "buf_ptr".
*/
void fe_ioblt_read(BLT_CONTEXT *bc, u_char *src_ptr, u_char *buf_ptr, u_int32
	word_count)
{
	*?*?* COPY PIXELS FROM THE SOURCE INTO THE LINE BUFFER.
}

/*******************************************************************************
* NAME:		fe_ioblt_write()
*
* USAGE:	Write "word_count" GDV_WORDs from the source buffer "buf_ptr" to the
*			destination pointed to by "dst_ptr".
*/
void fe_ioblt_write(BLT_CONTEXT *bc, u_char *dst_ptr, u_char *buf_ptr, u_int32
	word_count)
{
	*?*?* COPY PIXELS FROM THE LINE BUFFER TO THE DESTINATION.
}

/*******************************************************************************
* NAME:		fe_ioblt_write_pix()
*
* USAGE:	Write "word_count" GDV_WORDs of a solid pixel value from "pix_word"
*			to the destination pointed to by "dst_ptr".
*/
void fe_ioblt_write_pix(BLT_CONTEXT *bc, u_char *dst_ptr, GDV_WORD *pix_word,
	u_int32 word_count)
{
	*?*?* WRITE PIXELS OF A SOLID COLOR TO THE DESTINATION.
}
