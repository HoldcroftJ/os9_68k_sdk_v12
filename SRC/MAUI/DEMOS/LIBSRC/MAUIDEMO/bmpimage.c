#include <stdio.h>
#include <stdlib.h>

#include <types.h>
#include <modes.h>
#include <sg_codes.h>

#include <MAUI/maui.h>

#include <MAUI/mauidemo.h>
#include <MAUI/bmp.h>
#include <string.h>
#include <module.h>

/* some structure padding macros, to ensure we don't cause any alignment
 * problems for different processor types.
 */

#define PADVAL			8
#define PADSTRUCT(it) 	((sizeof(it)+(PADVAL-1)) & ~(PADVAL-1) )

/*
 *
 * Routines to do big/little endian swapping
 *
 */

/*
 * convert a 16 bit value
 */
static u_int16 convert16( u_int16 input )
{
	u_int16	temp;
	
	temp = ( input << 8 )| ( input >> 8 );
	
	return temp;
}

/*
 * convert a 32 bit value
 */
static u_int32 convert32( u_int32 input )
{
	u_int32 temp;

	temp = ( convert16( input & 0x0000FFFF ) << 16 ) |	\
			(convert16( (input & 0xFFFF0000)>>16) );

	return temp;
}


/*
 *
 * Routines to return values based on byte streams...
 *
 */
static u_int8	get8( char **ptr )
{
	u_int8	retval;

	retval = **ptr;

	(*ptr)++;

	return retval;
}

static u_int16	get16( char **ptr )
{
	u_int16	retval;

	retval = get8( ptr ) << 8;

	retval = retval | get8( ptr );

	return retval;
}

static u_int32	get32( char **ptr )
{
	u_int32	retval;

	retval = get16( ptr ) << 16;

	retval = retval | get16( ptr );

	return retval;
}

static error_code bmp_to_dmap( char *ptr, GFX_DMAP *dmap, u_int32 pixmem_shade )
{
  u_int32		i;
  u_int32		count;
  u_int32		bmp_line_size;			/* row length, in bytes */
  u_int32		colors;
  BMPHEADER	bmp;
  RGB			rgb;
  char		*temp;
  char		convert;
  
  bmp.type = get16( &ptr );
  
  /* verify it looks like a bmp file */
  
  switch( bmp.type ) 
	{
    case 0x424d:	/* BMP file, little endian */
      convert = TRUE;
      break;			
      
    case 0x4d42:	/* BMP file, big endian */
      convert = FALSE;
      break;
      
    default:		/* Not a BMP file */
      fprintf( stderr, "bmp_to_dmap:  not BMP format.\n" );
      return EOS_MAUI_BADVALUE;
	}
  
  /* pull more header values from the image */
  
  bmp.filesize 	= get32( &ptr );
  bmp.reserved1 	= get16( &ptr );
  bmp.reserved2	= get16( &ptr );
  bmp.offset		= get32( &ptr );
  bmp.infosize	= get32( &ptr );
  bmp.width		= get32( &ptr );
  bmp.height		= get32( &ptr );
  bmp.planes		= get16( &ptr );
  bmp.bitcount	= get16( &ptr );
  bmp.compression	= get32( &ptr );
  bmp.imagesize	= get32( &ptr );
  bmp.xpixels		= get32( &ptr );
  bmp.ypixels		= get32( &ptr );
  bmp.colors		= get32( &ptr );
  bmp.impcolors	= get32( &ptr );
  
  /* convert that big endian/little endian stuff (if necessary) */
  
  if ( convert ) 
	{
      bmp.type		= convert16( bmp.type );
      bmp.filesize	= convert32( bmp.filesize );
      bmp.offset 		= convert32( bmp.offset );
      bmp.infosize 	= convert16( bmp.infosize );
      bmp.width 		= convert32( bmp.width );
      bmp.height 		= convert32( bmp.height );
      bmp.planes 		= convert16( bmp.planes );
      bmp.bitcount 	= convert16( bmp.bitcount );
      bmp.compression = convert32( bmp.compression );
      bmp.imagesize	= convert32( bmp.imagesize );
      bmp.xpixels 	= convert32( bmp.xpixels );
      bmp.ypixels		= convert32( bmp.ypixels );
      bmp.colors		= convert32( bmp.colors );
      bmp.impcolors	= convert32( bmp.impcolors );
	}
  
  /* parse bmp header info */
  
  if ( bmp.colors == 0 ) {		/* 0 = means all colors used, so set */
    colors = 1<<bmp.bitcount;	/* that from the bitcount */
  } else {
    colors = bmp.colors;		/* else just use the color value */
  }
  
  /* check to see if it's something we can deal with */
  
  if ( bmp.planes != 1 ) 
	{
      fprintf( stderr, "bmp_to_dmap:  bitmap planes not supported.\n" );
      return EOS_MAUI_BADVALUE;
	}
  if ( bmp.compression != 0 ) 
	{
      fprintf( stderr, "bmp_to_dmap:  compressed BMP images not supported.\n" );
      return EOS_MAUI_BADCODEMETH;
	}
  
  /* now we allocate some storage for all of this */
  
  /* figure out how much storage is required for the drawmap structure, the
   * palette structure, and the palette color table, then allocate it.
   *
   * *dmap          = +-------------+  (passed in by user)
   *                  |  GFX_DMAP	  |
   *                  +-------------+
   * *dmap->pixmem  = +-------------+  (allocated here)
   *                  | Pixel       |
   *                  |      Memory |
   * *dmap->palette = +-------------+
   *                  | GFX_PALETTE |
   * ..->palette->    |  structure  |
   *     colors.rgb = |-------------|
   *                  | Palette     |
   *                  |       Table |
   *                  +-------------+
   */
  
  /* now we fill out some drawmap structure items */
  
  switch( bmp.bitcount ) 
	{
    case 1:							/* 1 bit per pixel (black and white) */
      
      gfx_set_dmap_size( dmap, GFX_CM_1BIT, bmp.width, bmp.height );
      break;
      
    case 4:							/* 4 bits per pixel (16 colors) */
      
      gfx_set_dmap_size( dmap, GFX_CM_4BIT, bmp.width, bmp.height );
      break;
      
    case 8:							/* 8 bits per pixel (256 colors) */
      
      gfx_set_dmap_size( dmap, GFX_CM_8BIT, bmp.width, bmp.height );
      break;
      
    default:
      fprintf( stderr, "bmp_to_dmap:  bitcount %d unsupported.\n", bmp.bitcount );
      return EOS_MAUI_BADCODEMETH;
	}
  
  /* Figure out the total size of memory we need to allocate... */
  
#if 0
  bmp_line_size = bmp.imagesize / bmp.height;		/* BMP image bytes per row */
#else
  bmp_line_size = dmap->line_size;
#endif
  
  /* padding needed in here? */
  
  count =	( bmp.height * dmap->line_size ) +
    ( sizeof( GFX_PALETTE ) ) + 
    ( sizeof( GFX_RGB )*colors );
  
  /* make MAUI allocate this space for is, all in pixel memory */
  
  gfx_set_dmap_pixmem( dmap, NULL, pixmem_shade, count );
  
  /* inside the drawmap, set the palette structure pointer to the appropraite
   * location within our allocated memory, then do the same for the palette
   * structure's color table pointer.
   */
  
  dmap->palette = 
    (GFX_PALETTE *)
    ( (u_int32)(dmap->pixmem) + (bmp.height * dmap->line_size) );
  
  /* set some palette structure elements */
  
  dmap->palette->start_entry = 0;
  dmap->palette->color_type = GFX_COLOR_RGB;	/* BMPs always this format */
  dmap->palette->num_colors = colors;
  
  /* and tell it where the color table can be found */
  
  dmap->palette->colors.rgb =
    (GFX_RGB *)
    ((u_int32)(dmap->palette) + sizeof( GFX_PALETTE ) );
  
  /* build MAUI's color table */
  
  for ( i=0 ; i<colors ; i++ ) 
	{
      rgb.blue 	= get8( &ptr );
      rgb.green 	= get8( &ptr );
      rgb.red 	= get8( &ptr );
      rgb.nada	= get8( &ptr );
      
      count = (rgb.red<<16) | (rgb.green<<8) | rgb.blue;
      
      dmap->palette->colors.rgb[i] = (GFX_PIXEL)count;
	}
  
  /* upon entry, "ptr" points to the start of the BMP image data */
  /* we make "temp" point to the last row of pixel memory */
  
  temp = (char*)dmap->pixmem + ( dmap->line_size * ( dmap->height-1 ) );
  
  
  for ( i=0 ; i < bmp.height ; i++ ) 
	{
      memcpy( temp, ptr, dmap->line_size );
      ptr = ptr + bmp_line_size;			/* move to next BMP row */
      temp = temp - bmp_line_size;		/* move up to previous pixmem line */
	}
  return EXIT_SUCCESS;
}

#if 0
/*
 * Misc. diagnostic function, not currently used...
 */

void bmp_show_info( BMPHEADER bmp )
{
  printf( "* File type  : %2x\n", bmp.type );
  printf( "* File size  : %d\n", bmp.filesize );
  printf( "* Offset     : %d\n", bmp.offset );
  printf( "* Info size  : %d\n", bmp.infosize );
  printf( "* Width      : %d\n", bmp.width );
  printf( "* Height     : %d\n", bmp.height );
  printf( "* Planes     : %d\n", bmp.planes );
  printf( "* Trains     : 0\n" );
  printf( "* Bit count  : %d (%d colors)\n", bmp.bitcount, 1<<bmp.bitcount );
  printf( "* Compression: %d\n", bmp.compression );
  printf( "* Image size : %d\n", bmp.imagesize );
  printf( "* X ratio    : %d\n", bmp.xpixels );
  printf( "* Y ratio    : %d\n", bmp.ypixels );
  printf( "* Colors used: %d", bmp.colors );
  if ( bmp.colors == 0 ) printf( " (all colors used)" );
  printf( "\n* Colors imp.: %d\n", bmp.impcolors );
  
}
#endif

/*******************************************************************************
 * NAME:        load_image_module()
 *
 * USAGE:   Load an image from an IFF module in memory. A new drawmap object
 *          is created by this function. Destroy it with gfx_destroy_dmap()
 *          when you no longer need it.
 */
static error_code load_image_module(GFX_DMAP *dmap, 
                                    u_int32 pixmem_shade,
                                    char *module_name)
{
  error_code ec;
  mh_com *mod_head;
  char *modname;
  void *mod_exec;
  u_int16 type_lang = 0;
  u_int16 attr_rev = 0;
  
  /* strip off any leading path */
  if ((modname = strrchr(module_name, '/')) == NULL) {
    modname = module_name;        
  } else {
    modname++;
  }
  
  if ((ec = _os_link(&modname, &mod_head, &mod_exec, &type_lang,
                     &attr_rev)) != SUCCESS) {
    return ec;
  }
  
  ec = bmp_to_dmap( mod_exec, dmap, pixmem_shade);

  _os_unlink(mod_head);

  return ec;

}


/*******************************************************************************
 * NAME:        load_image_file()
 *
 * USAGE:   Load an image from an IFF file. A new drawmap object is created by
 *          this function. Destroy it with gfx_destroy_dmap() when you no
 *          longer need it.
 */
static error_code load_image_file(GFX_DMAP *dmap,
                                  u_int32 pixmem_shade,
                                  char *image_filename)
{
  error_code ec = SUCCESS;
  path_id path;
  char *inbuf;
  u_int32 size;
  
  /* Open image file */
  if ((ec = _os_open(image_filename, S_IREAD, &path)) != SUCCESS)
    {
      fprintf(stderr, "Error opening %s\n", image_filename);
      return ec;
    }
  
  /* allocate space to read in the whole bmp file */
  if (((ec = _os_gs_size(path, &size)) != SUCCESS) ||
      ((ec = mem_calloc(&inbuf, MEM_DEF_SHADE, size, 1)) != SUCCESS)) {
    _os_close(path);
    return ec;
  }
  
  /* read the image in */
  if ((ec = _os_read(path, inbuf, &size)) != SUCCESS) {
    mem_free(inbuf);
    _os_close(path);
    return ec;
  }
  
  /* convert to drawmap */
  ec = bmp_to_dmap( inbuf, dmap, pixmem_shade);
  
  mem_free(inbuf);
  _os_close(path);
  return ec;
}
  

/*******************************************************************************
 * NAME:	demo_load_bmpimage()
 *
 * USAGE:   Load an image from an BMP module or file. A new drawmap object is
 *          created by this function. Destroy it with gfx_destroy_dmap() when
 *          you no longer need it.
 */
error_code demo_load_bmpimage(GFX_DMAP **ret_dmap, u_int32 dmap_shade,
                              u_int32 pixmem_shade, char *image_name)
{
  GFX_DMAP *dmap;
  error_code ec;
  
  /* Allocate memory for the drawmap object and its palette */
  {
    /* allocate enough for either a max GFX_RGB or single GFX_YUV */
    size_t size = sizeof(GFX_DMAP) + sizeof(GFX_PALETTE) +
      sizeof(GFX_RGB) * 256;
    
    if ((ec = mem_calloc(&dmap, dmap_shade, size, 1)) != SUCCESS) {
      return ec;
    }
  }
  
  dmap->palette = (GFX_PALETTE *)((u_int32)(dmap) + sizeof(GFX_DMAP));
  
  /* Try to link to a module with the specified name */
  ec = load_image_module(dmap, pixmem_shade, image_name);
  
  /* If the link failed because the module was not found then try to open */
  /* a file with the specified name */
  if (ec == EOS_MNF) {
    ec = load_image_file(dmap, pixmem_shade, image_name);
  }

  if (ec != SUCCESS)
    mem_free(dmap);
  else
    *ret_dmap = dmap;
  
  return ec;
}



