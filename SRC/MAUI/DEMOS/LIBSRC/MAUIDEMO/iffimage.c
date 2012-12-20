/*******************************************************************************
 *
 * DESCRIPTION: Functions that allow you to use IFF images in MAUI.
 *
 * COPYRIGHT:   Copyright 1995 Microware Systems Corporation. All Rights
 *              Reserved. Use and/or distribution subject to terms and
 *              conditions of applicable Microware license agreement(s).
 *
 */
#include <MAUI/mauidemo.h>
#include <MAUI/maui_gfx.h>
#include <MAUI/iff.h>
#include <MAUI/iff_imag.h>
#include <modes.h>
#include <module.h>
#include <stdio.h>
#include <string.h>

#define IFF_ID_CAT  IFF_MAKEID('C','A','T',' ')
#define IFF_ID_FORM IFF_MAKEID('F','O','R','M')

/* function prototypes */
static error_code load_image_module(GFX_DMAP **ret_dmap, u_int32 dmap_shade,
                                    u_int32 pixmem_shade, char *module_name);
static error_code load_image_file(GFX_DMAP **ret_dmap, u_int32 dmap_shade,
                                  u_int32 pixmem_shade, char *image_filename);
static void rgb_plte_copy(GFX_DMAP *dmap, IFF_PLTE *plte, IFF_COLOR *src_color);
static void yuv_plte_copy(GFX_DMAP *dmap, IFF_IHDR *ihdr);


#ifdef _LIL_END
   /* These functions should ideally be replaced with a Macro for speed
      purposes. They also currently support unaligned longs and shorts.
      If these are converted to macros, make sure that this is still
      supported */
    u_int32 swap32(u_int32 *val)
    {
        u_char *c = (u_char*)val;
        return (*c<<24 | *(c+1)<<16 | *(c+2)<<8 | *(c+3));
    }

    u_int16 swap16(u_int16 *val)
    {
        u_char *c = (u_char*)val;
        return (*c<<8 | *(c+1));
    }
#else
    /* Handle unaligned longs */
    #define swap32(x)                              \
            ((((((*((u_char *)(x)) << 8) |         \
                  *(((u_char *)(x)) + 1)) << 8) |  \
                  *(((u_char *)(x)) + 2)) << 8) |  \
                  *(((u_char *)(x)) + 3))

    #define swap16(x)                              \
                ((*((u_char *)(x)) << 8) |         \
                  *(((u_char *)(x)) + 1))
#endif


/*******************************************************************************
 * NAME:	demo_load_image()
 *
 * USAGE:   Load an image from an IFF module or file. A new drawmap object is
 *          created by this function. Destroy it with gfx_destroy_dmap() when
 *          you no longer need it.
 */
error_code demo_load_image(GFX_DMAP **ret_dmap, u_int32 dmap_shade,
                      u_int32 pixmem_shade, char *image_name)
{
    error_code ec;
    
    /* Try to link to a module with the specified name */
    if ((ec = load_image_module(ret_dmap, dmap_shade, pixmem_shade,
                                image_name)) == SUCCESS)
    {
        return SUCCESS;
    }
    
    /* If the link failed because the module was not found then try to open */
    /* a file with the specified name */
    if (ec == EOS_MNF)
    {
		ec = load_image_file(ret_dmap, dmap_shade, pixmem_shade, image_name);
    }
    
    return ec;
}



/*******************************************************************************
 * NAME:        load_image_module()
 *
 * USAGE:   Load an image from an IFF module in memory. A new drawmap object
 *          is created by this function. Destroy it with gfx_destroy_dmap()
 *          when you no longer need it.
 */
static error_code load_image_module(GFX_DMAP **ret_dmap, u_int32
                                    dmap_shade, u_int32 pixmem_shade,
                                    char *module_name)
{
    GFX_DMAP *dmap = NULL;
    IFF_COLOR *plte_data = NULL;
    IFF_IHDR ihdr;
    u_char *ihdr_ptr;
    IFF_PLTE *plte = NULL;
    IFF_U_32 *iff_ptr;
    IFF_U_32 *end_of_form;
    char *pixmem = NULL;
    error_code ec;
    mh_com *mod_head;
    size_t chunk_size;
    size_t pixmem_size;

    /* Link to the module */
    {
        char *modname;
        void *mod_exec;
        u_int16 type_lang = 0;
        u_int16 attr_rev = 0;
        

		/* strip off any leading path */
		if ((modname = strrchr(module_name, '/')) == NULL)
		{
			modname = module_name;        
		}
		else
		{
			modname++;
		}

        if ((ec = _os_link(&modname, &mod_head, &mod_exec, &type_lang,
                           &attr_rev)) != SUCCESS)
        {
            return ec;
        }

        iff_ptr = (IFF_U_32 *)mod_exec;
    }

    /* Skip optional CAT if present */
    if (swap32(iff_ptr) == IFF_ID_CAT) 
    {
        iff_ptr += 3;   /* skip CAT, size, and ID */
    }
    
    /* Verify that it is a FORM */
    if (swap32(iff_ptr) != IFF_ID_FORM)
    {
        _os_unlink(mod_head);
        return EOS_IFF_READER;
    }
	iff_ptr++;

    /* find the end of the FORM */
    end_of_form = (IFF_U_32 *)(((char *)iff_ptr) + swap32(iff_ptr));
    iff_ptr++;

    /* Verify that it is an IMAG FORM */
    if (swap32(iff_ptr) != IFF_ID_IMAG)
    {
        _os_unlink(mod_head);
        return EOS_IFF_READER;
    }
	iff_ptr++;

    /* Extract pointers to the components we need */
    {
        BOOLEAN done = FALSE;
        
        while (done == FALSE)
        {
            if (iff_ptr > end_of_form)
            {
                done = TRUE;
                ec = EOS_IFF_READER;
                fprintf (stderr, "Corrupted IFF file\n");
            }

            switch (swap32(iff_ptr)) 
            {
                /* Extract a pointer to the IFF header */
            case IFF_ID_IHDR:
				iff_ptr++;
                chunk_size = swap32(iff_ptr);  iff_ptr++;
                ihdr_ptr = (u_char*) iff_ptr;
                iff_ptr = (IFF_U_32 *)(((char *)iff_ptr) + chunk_size);
                break;
                
                /* Extract a pointer to the palette data */
            case IFF_ID_PLTE:
				iff_ptr++;
                chunk_size = swap32(iff_ptr); iff_ptr++;
                plte = (IFF_PLTE *)iff_ptr;
                plte_data = (IFF_COLOR *)(iff_ptr + 1);
                iff_ptr = (IFF_U_32 *)(((char *)iff_ptr) + chunk_size);
                break;
                
                /* Extract a pointer to the pixel data */
            case IFF_ID_IDAT:
				iff_ptr++;
                pixmem_size = chunk_size = swap32(iff_ptr);  iff_ptr++;
                pixmem = (char *)iff_ptr;
                iff_ptr = (IFF_U_32 *)(((char *)iff_ptr) + chunk_size);
                /* Quit on the IDAT chunk */
                done = TRUE;
                break;

                /* Skip over chunks we don't know about */
            default:
				iff_ptr++;
                fprintf (stderr, "Ignoring chunk type %08X\n", swap32(iff_ptr-1));
                iff_ptr = (IFF_U_32 *)(((char *)iff_ptr) + chunk_size);
                break;
            }
        }
    }
	/*SWAP the elements of the ihdr structure as well*/
	ihdr.ihdr_width = swap16((u_int16*)ihdr_ptr);
	ihdr.ihdr_line_size = swap16((u_int16*)ihdr_ptr+1);
	ihdr.ihdr_height = swap16((u_int16*)ihdr_ptr+2);
	ihdr.ihdr_model = swap16((u_int16*)ihdr_ptr+3);
	ihdr.ihdr_depth = swap16((u_int16*)ihdr_ptr+4);
	ihdr.ihdr_dyuv_kind = *(ihdr_ptr += sizeof(u_int16)*5);
	ihdr.ihdr_dyuv_start.y = *++ihdr_ptr;
	ihdr.ihdr_dyuv_start.u = *++ihdr_ptr;
	ihdr.ihdr_dyuv_start.v = *++ihdr_ptr;

    /* Allocate memory for the drawmap object and its palette */
    {
		/* allocate enough for either a max GFX_RGB or single GFX_YUV */
        size_t size = sizeof(GFX_DMAP) + sizeof(GFX_PALETTE) +
			sizeof(GFX_RGB) * 256;

        if ((ec = mem_calloc(&dmap, dmap_shade, size, 1)) != SUCCESS) 
        {
            _os_unlink(mod_head);
            return ec;
        }
    }
    
	dmap->palette =
		(GFX_PALETTE *)((u_int32)(dmap) + sizeof(GFX_DMAP));

    /* Set values in drawmap object based on image header information */
	if (ihdr.ihdr_model == IFF_MDL_DYUV)
	{
		/* Copy the IFF DYUV start to the drawmap palette */ 
		if (ihdr.ihdr_dyuv_kind != IFF_DYUV_ONE)
		{
			fprintf(stderr, "Only IFF_DYUV_ONE supported\n");
			_os_unlink(mod_head);
			mem_free(dmap);
			return EOS_MAUI_BADCODEMETH;
		}
		yuv_plte_copy(dmap, &ihdr);
        dmap->coding_method = GFX_CM_CDI_DYUV;

	}
	else
	{
		switch(ihdr.ihdr_model) 
		{
		case IFF_MDL_CLUT4:
			dmap->coding_method = GFX_CM_4BIT;
			break;
		case IFF_MDL_CLUT7:
			dmap->coding_method = GFX_CM_7BIT;
			break;
		case IFF_MDL_CLUT8:
			dmap->coding_method = GFX_CM_8BIT;
			break;
		case IFF_MDL_RL3:
			dmap->coding_method = GFX_CM_CDI_RL3;
			break;
		case IFF_MDL_RL7:
			dmap->coding_method = GFX_CM_CDI_RL7;
			break;
		default:
			fprintf(stderr, "Bad coding method\n");
			_os_unlink(mod_head);
			mem_free(dmap);
			return EOS_MAUI_BADCODEMETH;
		}
		/* there must be a PALETTE if not DYUV */
		if (plte == NULL)
		{
			fprintf(stderr, "Did not find PLTE chunk\n");
			_os_unlink(mod_head);
			mem_free(dmap);
			return EOS_MAUI_BADCODEMETH;
		}

		rgb_plte_copy(dmap, plte, plte_data);
	}

    
    /* Allocate pixel memory for the drawmap and copy IFF pixel data there */
    if ((ec = gfx_set_dmap_pixmem(dmap, NULL, pixmem_shade, pixmem_size))
        != SUCCESS) 
    {
        gfx_destroy_dmap(dmap);
        _os_unlink(mod_head);
        return ec;
    }
    memcpy(dmap->pixmem, pixmem, pixmem_size);
    
    dmap->width = ihdr.ihdr_width;
    dmap->height = ihdr.ihdr_height;
    dmap->line_size = ihdr.ihdr_line_size;
    
    /* Set return value and return to caller */
    *ret_dmap = dmap;
    _os_unlink(mod_head);
    return SUCCESS;
}



/*******************************************************************************
 * NAME:        load_image_file()
 *
 * USAGE:   Load an image from an IFF file. A new drawmap object is created by
 *          this function. Destroy it with gfx_destroy_dmap() when you no
 *          longer need it.
 */
static error_code load_image_file(GFX_DMAP **ret_dmap,
                                  u_int32 dmap_shade,
                                  u_int32 pixmem_shade,
                                  char *image_filename)
{
    GFX_DMAP *dmap = NULL;
    IFF_COLOR plte_data[256];
    IFF_CONTEXT *context = NULL;
    IFF_IMAG imag;
    error_code ec = SUCCESS;
    path_id path;
    size_t pixmem_size;
    
    /* Open image file */
    if ((ec = _os_open(image_filename, S_IREAD, &path)) != SUCCESS)
    {
        fprintf(stderr, "Error opening %s\n", image_filename);
        return ec;
    }

    /* Open IFF context */
    if ((context = iff_open(path, IFF_READ, IFF_ID_IMAG)) == NULL) 
    {
        ec = errno;
        fprintf(stderr, "Error in iff_open\n");
    }
    else
    {
        /* Read first FORM */
        if (iff_read_what_next(context) == -1) 
        {
            ec = errno;
            fprintf(stderr, "Error in iff_read_what_next\n");
        }
        else
        {
            /* Allocate memory for the drawmap object and its palette */
            size_t size = sizeof(GFX_DMAP) + sizeof(GFX_PALETTE) +
                sizeof(GFX_RGB) * 256;
            
            if ((ec = mem_calloc(&dmap, dmap_shade, size, 1)) != SUCCESS) 
            {
                dmap = 0;
                fprintf(stderr, "Error mem_calloc for drawmap\n");
            }
            else
            {
                dmap->palette =
                    (GFX_PALETTE *)((u_int32)(dmap) +
                                    sizeof(GFX_DMAP));
                /* Read the IMAG header */
                imag.plte_data = (u_char *)plte_data;
                imag.yuv_data = 0;
                imag.user_data = 0;
                if ((pixmem_size = iff_read_imag(context, &imag)) == (size_t) -1) 
                {
                    ec = errno;
                    fprintf(stderr, "Error in iff_read_imag\n");
                }
                else
                {
					dmap->palette->colors.rgb =
						(GFX_RGB *)((u_int32)(dmap->palette) + sizeof(GFX_PALETTE));

					switch(imag.ihdr.ihdr_model) 
					{
					case IFF_MDL_CLUT4:
					case IFF_MDL_CLUT7:
					case IFF_MDL_CLUT8:
					case IFF_MDL_RL3:
					case IFF_MDL_RL7:
					{
						rgb_plte_copy(dmap, &imag.plte, plte_data);
						break;
					}
					case IFF_MDL_DYUV:
					{
						/* Copy the IFF DYUV start to the drawmap palette */ 
						if (imag.ihdr.ihdr_dyuv_kind != IFF_DYUV_ONE)
						{
							ec = EOS_MAUI_BADCODEMETH;
							fprintf(stderr, "Only IFF_DYUV_ONE supported\n");
							break;
						}
						yuv_plte_copy(dmap, &imag.ihdr);
						break;
					}
					default:
						ec = EOS_MAUI_BADCODEMETH;
						fprintf(stderr, "Bad coding method\n");
						break;
					}

                    
                    /* Read the pixel data */
                    if ((ec = gfx_set_dmap_pixmem(dmap, NULL,
                                                  pixmem_shade,
                                                  pixmem_size))
                        == SUCCESS)
                    {
                        if (iff_read_data(context,
                                          (char*)dmap->pixmem,
                                          pixmem_size) == -1)
                        {
                            ec = errno;
                            fprintf(stderr, "Error in iff_read_data\n");
                        }
                        else
                        {
                            /* Set values in drawmap object based on image
                               header information */
                            switch(imag.ihdr.ihdr_model) 
                            {
                            case IFF_MDL_CLUT4:
                                dmap->coding_method = GFX_CM_4BIT;
                                break;
                            case IFF_MDL_CLUT7:
                                dmap->coding_method = GFX_CM_7BIT;
								break;
                            case IFF_MDL_CLUT8:
                                dmap->coding_method = GFX_CM_8BIT;
                                break;
                            case IFF_MDL_RL3:
                                dmap->coding_method = GFX_CM_CDI_RL3;
                                break;
                            case IFF_MDL_RL7:
                                dmap->coding_method = GFX_CM_CDI_RL7;
                                break;
                            case IFF_MDL_DYUV:
                                dmap->coding_method = GFX_CM_CDI_DYUV;
                                break;
                            default:
                                ec = EOS_MAUI_BADCODEMETH;
                                fprintf(stderr, "Bad coding method\n");
                                break;
                            }
                            if (ec == SUCCESS) 
                            {
                                dmap->width = imag.ihdr.ihdr_width;
                                dmap->height = imag.ihdr.ihdr_height;
                                dmap->line_size = imag.ihdr.ihdr_line_size;

                                *ret_dmap = dmap;

								/* Close the IFF context and the image file */
								iff_close(context);
								_os_close(path);
								return ec;
                            }
                        }
                    }
                }
                /* The following will destroy the drawmap object along
                   with the palette object and list of colors attached
                   to it. The pixmem will also be returned to the
                   system. */
                gfx_destroy_dmap(dmap);
            }
        }
        /* Close the IFF context and the image file */
        iff_close(context);
    }
    _os_close(path);

    return ec;
}
                    

/* Copy the IFF palette to the drawmap palette */
static void rgb_plte_copy(GFX_DMAP *dmap, IFF_PLTE *plte, IFF_COLOR *src_color)
{
	int num_colors = dmap->palette->num_colors = swap16(&plte->plte_count);
	GFX_RGB *dst_color = dmap->palette->colors.rgb
		= (GFX_RGB *)((u_int32)(dmap->palette) +
					  sizeof(GFX_PALETTE));
	
	dmap->palette->start_entry = swap16(&plte->plte_offset);
	dmap->palette->color_type = GFX_COLOR_RGB;
	while (num_colors-- > 0) 
	{
		*dst_color++ = (src_color->r << 16) | (src_color->g << 8) |
			src_color->b;
		src_color++;
	}

	return;
}

/* Copy the IFF YUV start to the drawmap palette */
static void yuv_plte_copy(GFX_DMAP *dmap, IFF_IHDR *ihdr)
{
	dmap->palette->start_entry = 0;
	dmap->palette->num_colors = 1;
	dmap->palette->color_type = GFX_COLOR_YUV;
	
	dmap->palette->colors.yuv =
		(GFX_YUV *)((u_int32)(dmap->palette) + sizeof(GFX_PALETTE));
	
	*dmap->palette->colors.yuv =
		(ihdr->ihdr_dyuv_start.y<<16) |
			(ihdr->ihdr_dyuv_start.u<<8) |
				(ihdr->ihdr_dyuv_start.v);
	return;
}
