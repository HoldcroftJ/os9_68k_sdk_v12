/*******************************************************************************
 *
 * DESCRIPTION:	Functions that allow you to use UCM fonts in MAUI.
 *
 * COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
 *				Reserved. Use and/or distribution subject to terms and
 *				conditions of applicable Microware license agreement(s).
 *
 */
#include <MAUI/mauidemo.h>
#include <MAUI/maui_txt.h>
#include <module.h>

/*******************************************************************************
 * DEFINITIONS FOR UCM FONT MODULES
 *******************************************************************************/

#define FNT_1BIT 	0		/* 1-bit/pixel font */
#ifdef FUTURE_ENHANCEMENT
  #define FNT_2BIT 	1		/* 2-bits/pixel font */
  #define FNT_4BIT 	2		/* 4-bits/pixel font */
  #define FNT_CLUT4	3		/* CLUT4 font */
  #define FNT_CLUT7	4		/* CLUT7 font */
  #define FNT_CLUT8	5		/* CLUT8 font */
  #define FNT_RGB 	9		/* RGB font */
#endif

#define FNT_PROP 	0x8000	/* Proportionally-spaced font */
#ifdef FUTURE_ENHANCEMENT
  #define FNT_MONO	0		/* Mono-spaced font */
#endif

/* Font data header definition. */
/* note: the offset variables are offsets from the beginning of the font */
/* header and NOT from the module header. */
typedef struct FontData  {
  u_int16 fnt_type;					/* Font type and flags */
  u_int16 fnt_width;					/* Maximum Glyph cell width */
  u_int16 fnt_height;					/* Glyph cell height (ascent+descnt) */
  u_int16 fnt_ascent;					/* Ascent of character cell above baseline */
  u_int16 fnt_descent;				/* Descent of character cell below baseline */
  u_int16 fnt_pxlsz;					/* Pixel size in bits */
  u_int16 fnt_frstch;					/* First character value of font */
  u_int16 fnt_lastch;					/* Last character value of font */
  u_int32 fnt_lnlen;					/* Line length of font bitmap in bytes */
  u_int32 fnt_offstbl;				/* Offset to glyph offset table */
  u_int32 fnt_datatbl;				/* Offset to glyph data table */
  u_int32 fnt_map1off;				/* Offset to first bitmap */
  u_int32 fnt_map2off;				/* Offset to second bitmap (RGB only) */
} FONTDATA;								

/* routines for fixing "wrong endian" data files */
BOOLEAN needswap;
static u_int16 swap16(u_int16 val)
{ 
	return ((val>>8)&0x00ff) | ((val & 0x00ff) << 8);
}
static u_int32 swap32(u_int32 val)
{
	return  (val >> 24) | ((val>>8)&0xff00) |
			((val&0xff00)<<8) | (val<<24);
}


/*******************************************************************************
 * NAME:	demo_get_ucm_font()
 *
 * USAGE:	Create a new font object and load a UCM font into it. The UCM font
 *			module must be in memory before calling this function. Destroy the
 *			font object with release_ucm_font() when you no longer need it.
 */
error_code demo_get_ucm_font(TXT_FONT **ret_font, u_int32 shade, char *fontname)
{
  GFX_OFFSET *offset_tbl = NULL;
  GFX_DIMEN *width_tbl = NULL;
  FONTDATA *ucmfont = NULL;
  GFX_DMAP *bitmap = NULL;
  TXT_FONT *font = NULL;
  error_code ec = SUCCESS;
  mh_com *mod_head = NULL;
  void *mod_exec = NULL;
  FONTDATA swapped_ucmfont;
  
  
  /* Link to the UCM font */
  {
    u_int16 type_lang = 0, attr_rev = 0;
    
    if ((ec = _os_link(&fontname, &mod_head, &mod_exec, &type_lang, &attr_rev)) != SUCCESS)
      return ec;
  }

  /* Extract information from the UCM font */
  {
    GFX_CM cm = 0;
    GFX_DIMEN width = 0;
    GFX_DIMEN height = 0;
    
    if (ec == SUCCESS) {
      ucmfont = mod_exec;

      /* Decide if the data is the same or different endian from the
         processor. We believe pixel sizes in bits should be between 1 and
         32 inclusive. If the value in this u_int16 field is larger than
         32, we assume that the data is the wrong endian. */
      needswap = (ucmfont->fnt_pxlsz > 32);
      if (needswap) {
        /* if we need swapping, swap the all the header information
           right away so we don't have to worry about it later */
        swapped_ucmfont.fnt_type = swap16(ucmfont->fnt_type);
        swapped_ucmfont.fnt_width = swap16(ucmfont->fnt_width);
        swapped_ucmfont.fnt_height = swap16(ucmfont->fnt_height);
        swapped_ucmfont.fnt_ascent = swap16(ucmfont->fnt_ascent);
        swapped_ucmfont.fnt_descent = swap16(ucmfont->fnt_descent);
        /* swapped_ucmfont.fnt_pxlsz = swap16(ucmfont->fnt_pxlsz); not used beyond this point */
        swapped_ucmfont.fnt_frstch = swap16(ucmfont->fnt_frstch);
        swapped_ucmfont.fnt_lastch = swap16(ucmfont->fnt_lastch);
        swapped_ucmfont.fnt_lnlen = swap32(ucmfont->fnt_lnlen);
        swapped_ucmfont.fnt_offstbl = swap32(ucmfont->fnt_offstbl);
        swapped_ucmfont.fnt_datatbl = swap32(ucmfont->fnt_datatbl);
        swapped_ucmfont.fnt_map1off = swap32(ucmfont->fnt_map1off);
        /* swapped_ucmfont.fnt_map2off = swap32(ucmfont->fnt_map2off); not used beyond this point */
        ucmfont = &swapped_ucmfont;
      }

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
          GFX_PIXEL *pixmem = (GFX_PIXEL *)((char *)mod_exec +
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
    
    /* Copy offsets from UCM table to MAUI table */
    if (ec == SUCCESS) {
      GFX_OFFSET *maui_otbl = offset_tbl;
      int16 *ucm_otbl = (int16 *)((char *)mod_exec + ucmfont->fnt_offstbl);
      
      if (needswap) { /* we moved the needswap test out of the loop
                         for speed */
        while(num_chars-- > 0) {
          if (*ucm_otbl == (int16)0xffff) {
            *maui_otbl++ = TXT_NOGLYPH;
          } else {
            *maui_otbl++ = swap16(*ucm_otbl);
          }
          ucm_otbl++;
        }
      } else {
        while(num_chars-- > 0) {
          if (*ucm_otbl == (int16)0xffff) {
            *maui_otbl++ = TXT_NOGLYPH;
          } else {
            *maui_otbl++ = *ucm_otbl;
          }
          ucm_otbl++;
        }
      }
    }
  }
  
  /* Set members of the TXT_FONT structure */
  if (ec == SUCCESS) {
    if ((ucmfont->fnt_type & FNT_PROP) == 0) {
      font->font_type = TXT_FONTYPE_MONO;
    } else {
      size_t num_chars  = ucmfont->fnt_lastch - ucmfont->fnt_frstch + 1;
      
      font->font_type = TXT_FONTYPE_PROP;
      
      /* Create the width table */
      ec = mem_calloc(&width_tbl, shade, num_chars, sizeof(GFX_DIMEN));
      
      /* Copy widths from UCM table to MAUI table */
      if (ec == SUCCESS) {
        GFX_DIMEN *maui_wtbl = width_tbl;
        u_int8 *ucm_wtbl = (u_int8 *)mod_exec + ucmfont->fnt_datatbl;
        
        while(num_chars-- > 0) {
#ifdef _LIL_END  /* csm4730 - This is a work around for some little
                    endian fonts that mistakenly byte swapped the
                    width table. The FFGB Figure VII.27 specifies
                    that the Glyph Data Table consist of a 1 byte
                    character width in pixels plus three reserved
                    bytes. */
          if (*ucm_wtbl) {
#endif
            *maui_wtbl++ = *ucm_wtbl;
#ifdef _LIL_END  /* csm4730 */
          } else {
            *maui_wtbl++ = *(ucm_wtbl+3);
          }
#endif
          ucm_wtbl += 4;
        }
      }
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
    font->range_tbl->width_tbl = width_tbl;
    font->range_tbl->bitmap = bitmap;
  }
  
  /* Cleanup and return */
  if (ec == SUCCESS) {
    *ret_font = font;
    return SUCCESS;
  } else {
    if (width_tbl != NULL) {
      mem_free(width_tbl);
    }
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
 * NAME:	demo_release_ucm_font()
 *
 * USAGE:	Deallocate any memory allocated by get_ucm_font().
 */
error_code demo_release_ucm_font(TXT_FONT *font)
{
  error_code ec, result = SUCCESS;
  
  if (font->range_tbl->width_tbl != NULL &&
      (ec = mem_free(font->range_tbl->width_tbl)) != SUCCESS) {
    result = ec;
  }
  if ((ec = mem_free(font->range_tbl->offset_tbl)) != SUCCESS) {
    result = ec;
  }
  if ((ec = gfx_destroy_dmap(font->range_tbl->bitmap)) != SUCCESS) {
    result = ec;
  }
  if ((ec = txt_destroy_font(font)) != SUCCESS){
    result = ec;
  }
  
  return result;
}

