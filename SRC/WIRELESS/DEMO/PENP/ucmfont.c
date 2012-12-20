/*******************************************************************************
*
* DESCRIPTION:	Functions that allow you to use UCM fonts in MAUI.
*
* AUTHOR:		Todd Earles
*
* CREATED ON:	06/02/95
*
* COPYRIGHT:	Copyright 1994 Microware Systems Corporation. All Rights
*				Reserved. Use and/or distribution subject to terms and
*				conditions of applicable Microware license agreement(s).
*
* VERSION:		@(#)ucmfont.c	1.3 6/28/95
*
******************************************************************************
** Edition History:                                                         **
** #   Date     Comments                                          By        **
** --- -------- ------------------------------------------------- --------- **
** 1   08/03/95 Modified get_ucm_font() to use an already          rws
**              created TXT_FONT
*/
#include <stdlib.h>
#include <module.h>
#include <MAUI/maui_mem.h>
#include <MAUI/maui_gfx.h>
#include <MAUI/maui_txt.h>

/*******************************************************************************
* DEFINITIONS FOR UCM FONT MODULES
*******************************************************************************/

#define FNT_1BIT 	0		/* 1-bit/pixel font */
#define FNT_2BIT 	1		/* 2-bits/pixel font */
#define FNT_4BIT 	2		/* 4-bits/pixel font */
#define FNT_CLUT4 	3		/* CLUT4 font */
#define FNT_CLUT7 	4		/* CLUT7 font */
#define FNT_CLUT8 	5		/* CLUT8 font */
#define FNT_RGB 	9		/* RGB font */

#define FNT_PROP 	0x8000	/* Proportionally-spaced font */
#define FNT_MONO	0		/* Mono-spaced font */

/* Font data header definition. */
/* note: the offset variables are offsets from the beginning of the font */
/* header and NOT from the module header. */
typedef struct FontData  {
	u_int16
		fnt_type,					/* Font type and flags */
		fnt_width,					/* Maximum Glyph cell width */
		fnt_height,					/* Glyph cell height (ascent+descnt) */
		fnt_ascent,					/* Ascent of character cell above baseline */
		fnt_descent,				/* Descent of character cell below baseline */
		fnt_pxlsz,					/* Pixel size in bits */
		fnt_frstch,					/* First character value of font */
		fnt_lastch;					/* Last character value of font */
	u_int32
		fnt_lnlen,					/* Line length of font bitmap in bytes */
		fnt_offstbl,				/* Offset to glyph offset table */
		fnt_datatbl,				/* Offset to glyph data table */
		fnt_map1off,				/* Offset to first bitmap */
		fnt_map2off;				/* Offset to second bitmap (RGB only) */
} FONTDATA;								

/*******************************************************************************
* NAME:		get_ucm_font()
*
* USAGE:	Create a new font object and load a UCM font into it. The UCM font
*			module must be in memory before calling this function. Destroy the
*			font object with release_ucm_font() when you no longer need it.
*/
error_code get_ucm_font
(
	TXT_FONT *font, 
	u_int32 shade, 
	char *fontname
)
{
	error_code ec = SUCCESS;
	mh_com *mod_head = NULL;
	FONTDATA *ucmfont = NULL;
	GFX_DMAP *bitmap = NULL;
	int16 *ucm_otbl;
	GFX_OFFSET *offset_tbl = NULL, *maui_otbl;
	GFX_PIXEL *pixmem;
	size_t pixmem_size, num_chars;
	int loop_var;

	/* Link to the UCM font */
	{
		u_int16 type_lang = 0, attr_rev = 0;
		if ((ec = _os_link (&fontname, &mod_head, (void**)&ucmfont, 
					&type_lang, &attr_rev)) != SUCCESS)
		{
			return ec;
		}
	}

	/* Extract information from the UCM font */
	if ( ((ucmfont->fnt_type) & 0xff) != FNT_1BIT)
	{
		_os_unlink(mod_head);
		return (EOS_MAUI_BADCODEMETH);
	}

	/* Create drawmap for text object and initialize structure members */
	if ((ec = gfx_create_dmap (&bitmap, shade)) != SUCCESS)
	{
		_os_unlink(mod_head);
		return (ec);
	}

	if ((ec = gfx_set_dmap_size (bitmap, GFX_CM_1BIT, 
			ucmfont->fnt_lnlen * 8, ucmfont->fnt_height)) != SUCCESS) 
	{
		gfx_destroy_dmap (bitmap);
		_os_unlink(mod_head);
		return (ec);
	}

	pixmem = (GFX_PIXEL *)((char *)ucmfont + ucmfont->fnt_map1off);
	pixmem_size = ucmfont->fnt_lnlen * ucmfont->fnt_height;
	if ((ec = gfx_set_dmap_pixmem (bitmap, pixmem, 0, pixmem_size)) != SUCCESS)
	{
		gfx_destroy_dmap (bitmap);
		_os_unlink(mod_head);
		return (ec);
	}

	/* Create the offset table */
	num_chars = ucmfont->fnt_lastch - ucmfont->fnt_frstch + 1;
	if ((ec = mem_calloc (&offset_tbl, shade, num_chars, sizeof(GFX_OFFSET)))
			!= SUCCESS)
	{
		gfx_destroy_dmap (bitmap);
		_os_unlink(mod_head);
		return (ec);
	}

	/* Copy offset from UCM table to MAUI table */
	loop_var = num_chars;
	maui_otbl = offset_tbl;
	ucm_otbl = (int16 *)((char *)ucmfont + ucmfont->fnt_offstbl);
	while (loop_var > 0) {
		if (*ucm_otbl == 0xffff) {
			*maui_otbl++ = TXT_NOGLYPH;
		} else {
			*maui_otbl++ = *ucm_otbl;
		}
		ucm_otbl++;
		loop_var--;
	}

	/* Set members of the TXT_FONT structure */
	if ((ucmfont->fnt_type & FNT_PROP) == 0) {
		font->font_type = TXT_FONTYPE_MONO;
	} else {
		font->font_type = TXT_FONTYPE_PROP;
	}
	font->width = ucmfont->fnt_width;
	font->height = ucmfont->fnt_height;
	font->ascent = ucmfont->fnt_ascent;
	font->descent = ucmfont->fnt_descent;
	font->default_char = ucmfont->fnt_frstch;

	/* Set members of the TXT_RANGTBL structure */
	font->range_tbl->first_char = ucmfont->fnt_frstch;
	font->range_tbl->last_char = ucmfont->fnt_lastch;
	font->range_tbl->offset_tbl = offset_tbl;
	font->range_tbl->width_tbl = NULL;
	font->range_tbl->bitmap = bitmap;

	/* Unlink the UCM font module */
	if (ucmfont != NULL) {
		_os_unlink(mod_head);
	}

	return SUCCESS;
}

/*******************************************************************************
* NAME:		release_ucm_font()
*
* USAGE:	Deallocate any memory allocated by get_ucm_font().
*/
error_code release_ucm_font(TXT_FONT *font)
{
	error_code ec, result = SUCCESS;
	
	if ((ec = mem_free(font->range_tbl->offset_tbl)) != SUCCESS)
		result = ec;
	if ((ec = gfx_destroy_dmap(font->range_tbl->bitmap)) != SUCCESS)
		result = ec;

	return result;
}
