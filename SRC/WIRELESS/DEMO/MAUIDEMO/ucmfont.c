/*******************************************************************************
 *
 * DESCRIPTION: Functions that allow you to use UCM fonts in MAUI.
 *
 * COPYRIGHT:   Copyright 1995 Microware Systems Corporation. All Rights
 *                              Reserved. Use and/or distribution subject to terms and
 *                              conditions of applicable Microware license agreement(s).
 *
 */
#include <stdlib.h>
#include <MAUI/maui_mem.h>
#include <MAUI/maui_gfx.h>
#include <MAUI/maui_txt.h>
#include <module.h>

/*******************************************************************************
 * DEFINITIONS FOR UCM FONT MODULES
 *******************************************************************************/

#define FNT_1BIT        0               /* 1-bit/pixel font */
#define FNT_2BIT      1               /* 2-bits/pixel font */
#define FNT_4BIT      2               /* 4-bits/pixel font */
#define FNT_CLUT4     3               /* CLUT4 font */
#define FNT_CLUT7     4               /* CLUT7 font */
#define FNT_CLUT8     5               /* CLUT8 font */
#define FNT_RGB       9               /* RGB font */

#define FNT_PROP        0x8000  /* Proportionally-spaced font */
#define FNT_MONO      0               /* Mono-spaced font */

/* Font data header definition. */
/* note: the offset variables are offsets from the beginning of the font */
/* header and NOT from the module header. */
typedef struct FontData  {
	u_int16 fnt_type;                                       /* Font type and flags */
	u_int16 fnt_width;                                      /* Maximum Glyph cell width */
	u_int16 fnt_height;                                     /* Glyph cell height (ascent+descnt) */
	u_int16 fnt_ascent;                                     /* Ascent of character cell above baseline */
	u_int16 fnt_descent;                            /* Descent of character cell below baseline */
	u_int16 fnt_pxlsz;                                      /* Pixel size in bits */
	u_int16 fnt_frstch;                                     /* First character value of font */
	u_int16 fnt_lastch;                                     /* Last character value of font */
	u_int32 fnt_lnlen;                                      /* Line length of font bitmap in bytes */
	u_int32 fnt_offstbl;                            /* Offset to glyph offset table */
	u_int32 fnt_datatbl;                            /* Offset to glyph data table */
	u_int32 fnt_map1off;                            /* Offset to first bitmap */
	u_int32 fnt_map2off;                            /* Offset to second bitmap (RGB only) */
} FONTDATA;                                                             

/*******************************************************************************
 * NAME:        demo_get_ucm_font()
 *
 * USAGE:       Create a new font object and load a UCM font into it. The UCM font
 *                      module must be in memory before calling this function. Destroy the
 *                      font object with release_ucm_font() when you no longer need it.
 */
error_code get_ucm_font(TXT_FONT **ret_font, u_int32 shade, char *fontname)
{
	GFX_OFFSET *offset_tbl = NULL;
	FONTDATA *ucmfont = NULL;
	GFX_DMAP *bitmap = NULL;
	TXT_FONT *font = NULL;
	error_code ec = SUCCESS;
	mh_com *mod_head = NULL;
	void *mod_exec = NULL;
	
	
	/* Link to the UCM font */
    {
		u_int16 type_lang = 0, attr_rev = 0;
		
		ec = _os_link(&fontname, &mod_head, &mod_exec, &type_lang, &attr_rev);
	}
	
	/* Extract information from the UCM font */
    {
		GFX_CM cm = 0;
		GFX_DIMEN width = 0;
		GFX_DIMEN height = 0;
		
		if (ec == SUCCESS) {
			ucmfont = mod_exec;
			switch (ucmfont->fnt_type & 0xff) {
			case (FNT_1BIT):
				cm = GFX_CM_1BIT;
				width = ucmfont->fnt_lnlen * 8;
				break;
			default:
				ec = EOS_MAUI_BADCODEMETH;
				break;
			}
			height = ucmfont->fnt_height;
		}
		
		/* Create the drawmap object and set members of the structure */
		if (ec == SUCCESS) {
			if ((ec = gfx_create_dmap(&bitmap, shade)) == SUCCESS) {
				if ((ec = gfx_set_dmap_size(bitmap, cm, width, height)) ==
					SUCCESS) {
					GFX_PIXEL *pixmem = (GFX_PIXEL *)((char *)ucmfont +
													  ucmfont->fnt_map1off);
					size_t pixmem_size = ucmfont->fnt_lnlen * ucmfont->fnt_height;
					
					ec = gfx_set_dmap_pixmem(bitmap, pixmem, 0, pixmem_size);
				}
			}
		}
	}
	
	/* Create the font object */
	if (ec == SUCCESS) {
		ec = txt_create_font(&font, shade, 1);
	}
	
	/* Create the offset table */
	if (ec == SUCCESS) {
		size_t num_chars  = ucmfont->fnt_lastch - ucmfont->fnt_frstch + 1;

		ec = mem_calloc(&offset_tbl, shade, num_chars, sizeof(GFX_OFFSET));

		/* Copy offset from UCM table to MAUI table */
		if (ec == SUCCESS) {
			GFX_OFFSET *maui_otbl = offset_tbl;
			int16 *ucm_otbl = (int16 *)((char *)ucmfont + ucmfont->fnt_offstbl);
			
			while(num_chars-- > 0) {
				if (*ucm_otbl == 0xffff) {
					*maui_otbl++ = TXT_NOGLYPH;
				} else {
					*maui_otbl++ = *ucm_otbl;
				}
				ucm_otbl++;
			}
		}
	}
	
	/* Set members of the TXT_FONT structure */
	if (ec == SUCCESS) {
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
	}
	
	/* Set members of the TXT_RANGTBL structure */
	if (ec == SUCCESS) {
		font->range_tbl->first_char = ucmfont->fnt_frstch;
		font->range_tbl->last_char = ucmfont->fnt_lastch;
		font->range_tbl->offset_tbl = offset_tbl;
		font->range_tbl->width_tbl = NULL;
		font->range_tbl->bitmap = bitmap;
	}
	
	/* Cleanup and return */
	if (ec == SUCCESS) {
		*ret_font = font;
		return SUCCESS;
	} else {
		if (offset_tbl != NULL) {
			mem_free(offset_tbl);
		}
		if (font != NULL) {
			txt_destroy_font(font);
		}
		if (bitmap != NULL) {
			gfx_destroy_dmap(bitmap);
		}
		return ec;
	}
}

/*******************************************************************************
 * NAME:        demo_release_ucm_font()
 *
 * USAGE:       Deallocate any memory allocated by get_ucm_font().
 */
error_code release_ucm_font(TXT_FONT *font)
{
	error_code ec, result = SUCCESS;
	
	if ((ec = mem_free(font->range_tbl->offset_tbl)) != SUCCESS)
	{
		result = ec;
	}
	if ((ec = gfx_destroy_dmap(font->range_tbl->bitmap)) != SUCCESS)
	{
		result = ec;
	}
	if ((ec = txt_destroy_font(font)) != SUCCESS)
	{
		result = ec;
	}
	
	return result;
}
