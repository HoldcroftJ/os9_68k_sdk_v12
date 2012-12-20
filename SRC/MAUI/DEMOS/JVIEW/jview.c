/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 2001 by Microware Systems Corporation              |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, modification, distribution, or creation of    |
|  derivative works in any form to any party other than the Licensee is     |
|  strictly prohibited, unless expressly provided for under the terms of a  |
|  mutually executed written license agreement for this software between    |
|  the end-user and Microware Systems Corporation, or otherwise expressly   |
|  authorized in writing by Microware Systems Corporation.  Licensee shall  |
|  reproduce this copyright notice exactly as provided herein on any copies |
|  of the software that Licensee is authorized to distribute.               |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
|  08 01/08/14 CF11103: added support for the use of JPEG image files       | 
|              as data modules.                                        sbh  |
|              Added edition history                                   sbh  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/
/*
 * jview.c
 *
 * Yuri Persion, 04/10/98
 *
 * COPYRIGHT:
 *
 *	This source code is the proprietary confidential property of Microware
 *	Systems Corporation, and is provided to licensee solely for documentation
 *	and educational purposes. Reproduction, publication, or distribution in
 *	form to any party other than the licensee is strictly prohibited.
 *
 * This program uses the MAUI JPEG API to display either single or
 * sequential JPEG images.
 *
 * If the program is used in a single-image mode, its format is as follows:
 *
 * jview <file_name> <options>
 *
 * where <file_name> is the JPEG image file in the current directory or
 * in the module directory.
 *
 * If a slide-show mode is used, the format is:
 * 
 * jview <list_file> S <options>
 *
 * where <list_file> is the file in the current directory, that holds
 *                   the list of JPEG image files, one name per line.
 *
 * For example:
 *
 * earth.jpg
 * edo.jpg
 * ......
 *
 * JPEG files must be in the current directory or in the module directory.
 *
 *
 * Options:
 *
 *	q<value> - quality level, 1 through 3 (default is 3 - the highest);
 *  s<nd>    - scale, n - numerator, d - denominator (1:1, 1:2, 1:4, 1:8
 *             are supported); for example, s12 means scale 1:2.
 *  m - monochrome;
 *  c<value> - number of colors (default is 256);
 *  C<value> - color method number in dev_cap (default is 0);
 *  r - resolution number in dev_cap (default is 0);
 *  S - slide mode; this assumes that list file is used;
 *  T<value> - time to display each slide in sec (default is 3);
 *  F - use fade-in and fade-out effect(default - no fade);
 *      Note that fade effect is only available in CLUT-based coding methods.
 *  V - verbose mode (debug);
 */

#define EDITION "8"

#if defined(_OSK) || defined(_OS9000)
_asm("_sysedit: equ " EDITION);
#else
char _sysedit[] = "_sysedit=" EDITION;
#endif

#include <stdio.h>
#include <MAUI/maui.h>
#include <MAUI/mauidemo.h>
#include <MAUI/maui_jpg.h>
#include <time.h>
#include <ctype.h>		/* to declare isprint() */
#include <sg_codes.h>
#include <types.h>
#include <module.h>

#define msg(x) fprintf(stderr,"%s\n", x ); fflush(stderr);

/*
 * Argument-parsing code.
 * The switch parser is designed to be useful with DOS-style command line
 * syntax, ie, intermixed switches and file names, where only the switches
 * to the left of a given file name affect processing of that file.
 * The main program in this file doesn't actually use this capability...
 */
#if 0
GFX_RGB colors[] = {
#include "pal216.h"
};

GFX_PALETTE my_palette = {
    0,
	216,			/* Number of colors */
	GFX_COLOR_RGB,				/* Color type */
	colors					/* Pointer to list of colors */
};

#endif

JPG_QUALITY_LEVEL qual=JPG_HIGH_QUAL;
u_int32 numer=0, denom=0;
u_int32 numcol=256;
BOOLEAN is_monochrome=FALSE;
BOOLEAN verbose = FALSE;
GFX_DIMEN x=0,y=0;

const GFX_DEV_CAP *devcap;		/* Device capabilities */
JPG_CONTEXT_PARAMS cinfo;
u_int32 res_num=0;
u_int32 cm_num=0;
BOOLEAN slide_mode = FALSE;
BOOLEAN to_fade = FALSE;
u_int32 show_time = 3;


static void
usage (void)
/* complain about bad command line */
{
	fprintf(stderr,"Usage: jview <file or list_file> <option>\n");
	fprintf(stderr,"where options:\n");
	fprintf(stderr,"quality level:  q<quality_level 1 thru 3>\n");
	fprintf(stderr,"scale:          s<numerator><denominator>\n");
	fprintf(stderr,"monochrome:     m\n");
	fprintf(stderr,"color number:   c<number of colors desired>\n");
	fprintf(stderr,"coding method number: C<value>\n");
	fprintf(stderr,"resolution number:   r<value>\n");
	fprintf(stderr,"x offset coordinate:   x<value>\n");
	fprintf(stderr,"y offset coordinate:   y<value>\n");
	fprintf(stderr,"Slide mode (if you use list file):   S\n");
	fprintf(stderr,"Display time: T<value>\n");
	fprintf(stderr,"Fade: F\n");
	fprintf(stderr,"Verbose: V\n");
	fprintf(stderr,"\nIf the program is used in a single-image mode,\nits format is as follows:\n");
	fprintf(stderr," jview <file_name> <options>\n");
	fprintf(stderr,"where <file_name> is the JPEG image file in the current directory\n\n");
	fprintf(stderr,"                  or in the module directory.\n\n");
	fprintf(stderr,"If a slide-show mode is used, the format is:\n\n");
	fprintf(stderr, " jview <list_file> S <options>\n\n");
	fprintf(stderr, "where <list_file> is the file in the current directory, that holds\n");
	fprintf(stderr, "                  the list of JPEG image files, one name per line.\n\n");

	fprintf(stderr, "For example:\n\n");
	fprintf(stderr, " earth.jpg\n edo.jpg\n ......\n\n");
	fprintf(stderr, "JPEG files must be in the current directory or in the module directory.\n");
 	exit(1);
}

void parse_args(int argc, char ** argv)
{
	if(argc == 1)
		usage();

	if(argv[1][0] == '-' || argv[1][0] == '?')
	  usage();
	
	for(;argc > 2; argc --)
	{
	  switch(argv[argc-1][0])
	  {
	  case 'c':
		sscanf(argv[argc-1]+1,"%d",&numcol);
		break;
	  case 'x':
		sscanf(argv[argc-1]+1,"%d",&x);
		break;
	  case 'y':
		sscanf(argv[argc-1]+1,"%d",&y);
		break;
	  case 'm':
		is_monochrome=TRUE;
		break;
	  case 'q':
		switch(argv[argc-1][1])
		{
		case '1':
		  qual= JPG_LOW_QUAL; 
		  break;
		case '2':
		  qual= JPG_MED_QUAL;
		  break;
		case '3':
		  qual= JPG_HIGH_QUAL;
		  break;
		default:
		  fprintf(stderr,"Quality level should be 1,2, or 3\n");
		  usage();
		}
		break;
	  case 's':
		numer=argv[argc-1][1] -'0';
		denom=argv[argc-1][2] -'0';

		break;
	  case 'r':
	  case 'R':
		sscanf(argv[argc-1]+1,"%d",&res_num);
		break; 
	  case 'C':
		sscanf(argv[argc-1]+1,"%d",&cm_num);
		break; 
	  case 't':
	  case 'T':
		sscanf(argv[argc-1]+1,"%d",&show_time);
		break; 
	  case 'S':
		slide_mode = TRUE;
		break;
	  case 'f':
	  case 'F':
		to_fade = TRUE;
		break;
	  case 'v':
	  case 'V':
		verbose = TRUE;
		break;
	  default:
		usage();
	  }
	}
	if(verbose)
	{
	  fprintf(stderr,"Trying %d colors\n",numcol);
	  fprintf(stderr,"Trying x %d \n",x);
	  fprintf(stderr,"Trying y %d\n",y);
	  switch(qual)
	  {
	  case JPG_LOW_QUAL:
		fprintf(stderr, "Trying low quality level\n");
		break;
	  case JPG_MED_QUAL:
		fprintf(stderr, "Trying medium quality level\n");
		break;
	  case JPG_HIGH_QUAL:
		fprintf(stderr, "Trying high quality level\n");
		break;
	  }
	  fprintf(stderr,"Scale %d : %d\n",numer!=0?numer:1,denom!=0?denom:1);
	  fprintf(stderr,"Trying %d resolution\n",res_num);
	  fprintf(stderr,"Trying %d coding method\n",cm_num);
	  fprintf(stderr,"Display time %d sec.\n",show_time);
	  if(slide_mode)
		fprintf(stderr,"Slide mode\n");
		if(to_fade)
		  fprintf(stderr,"Fade mode\n");
	}

}			




#define R(x) (((x) >> 16) & 0xff)
#define G(x) (((x) >> 8) & 0xff)
#define B(x) ((x) & 0xff)

#define REDUCE_COL(r, f_r)    \
            if(r != f_r)                        \
            {                                   \
                was_lit=TRUE;                   \
                if(r > f_r)                     \
                    r--;                        \
                else                            \
                    r ++;                       \
            }

#define RGB(r, g, b) ((b & 0xff) | ( (g<<8) & 0xff00) | ((r<<16) & 0xff0000))

void fade_out(GFX_DEV_ID gfxdev, GFX_VPORT_ID vp, GFX_PALETTE * pal, u_int32 final_color)
{
    u_int32 i;
    u_int8 red, green, blue;
    u_int8 f_red =   R(final_color), 
           f_green = G(final_color), 
           f_blue =  B(final_color);
    BOOLEAN was_lit = TRUE;

    while(was_lit)
    {
        was_lit=FALSE;
        for(i=0; i<pal->num_colors; i++)
        {
            red =   R(pal->colors.rgb[i]);
            green = G(pal->colors.rgb[i]);
            blue =  B(pal->colors.rgb[i]);
            REDUCE_COL(red, f_red);
            REDUCE_COL(blue, f_blue);
            REDUCE_COL(green, f_green);
            pal->colors.rgb[i]=RGB(red, green, blue);
        }
        gfx_set_vport_colors(vp, 0, pal->num_colors,pal->color_type, (void*)pal->colors.rgb);
        gfx_update_display(gfxdev, FALSE);
    }
}   

void fade_in(GFX_DEV_ID gfxdev, GFX_VPORT_ID vp, GFX_PALETTE * pal, GFX_RGB start_color)
{
    u_int32 i;
    u_int8 red, green, blue;
    u_int8 f_red,  f_green, f_blue;
    BOOLEAN was_lit = TRUE;
    
    GFX_RGB color[256];
    
    for(i=0; i<pal->num_colors; i++)
        color[i]=start_color;

    while(was_lit)
    {
        was_lit=FALSE;
        for(i=0; i<pal->num_colors; i++)
        {
            red = R(color[i]);
            green= G(color[i]);
            blue = B(color[i]);

            f_red =   R(pal->colors.rgb[i]);
            f_green = G(pal->colors.rgb[i]);
            f_blue =  B(pal->colors.rgb[i]);

            REDUCE_COL(red, f_red);
            REDUCE_COL(blue, f_blue);
            REDUCE_COL(green, f_green);
            color[i]=RGB(red, green, blue);
        }
        gfx_set_vport_colors(vp, 0, pal->num_colors,pal->color_type, (void*)color);
        gfx_update_display(gfxdev, FALSE);
    }
}   


/*
 * The main program.
 */

int
main (int argc, char **argv)
{
	JPG_CONTEXT_ID ctx;
	BLT_CONTEXT_ID bctx;
	FILE * input_file, *list_file=NULL;
	GFX_DMAP *dmap;
	error_code err;
	u_int32 size;
	GFX_DEV_ID gfxdev;
	GFX_VPORT_ID vport; 
	u_int32 jmem_size;
	path_id filenum;
	u_char *jsrcmem;
	int scan_num=0;
	BOOLEAN ms=FALSE;
	char file_name[80];
	char in_line[80];
	mh_com *mod_head;
	char *mod_exec;
	u_int32 *data_size;
	u_int16	type_lang, attr_rev;
	char *mod_name = file_name;


	GFX_DIMEN max_width=640, max_height=480;
	GFX_DEV_RES *devres;			/* Default device resolution */
	GFX_DEV_CM *devcm;				/* Default device coding method */
	GFX_DIMEN w,h;
	GFX_POS xx, yy;
	GFX_RGB start_color = 0xff0000;
	GFX_RGB end_color = 0x00ff00;

	type_lang = attr_rev = 0;

	parse_args(argc,argv);
	
	
	gfx_init();

	cdb_init();
	blt_init();



	
    if ((err = demo_open_gfx_dev(&gfxdev)) != SUCCESS) return(err);
	demo_create_gfx_shade(gfxdev, DEMO_GFX_SHADE, NULL);
	gfx_get_dev_cap(&devcap, gfxdev);
	devres = &devcap->res_info[res_num];
	devcm = &devcap->cm_info[cm_num];
	gfx_set_display_size(gfxdev, devres->disp_width,devres->disp_height,
						 devres->intl_mode,devres->refresh_rate);
	w = devres->disp_width  / (devcm->dm2dp_xmul);
	h = devres->disp_height / (devcm->dm2dp_ymul);

	max_width =w;
	max_height = h;
	if(verbose)
	  fprintf(stderr,"%d x %d\n", w, h);
	if(to_fade)
	{
	  u_int8 r = rand()%0xff;
	  u_int8 g = rand()%0xff;
	  u_int8 b = rand()%0xff;
	  start_color = RGB(r,g,b);
	  r = rand()%0xff;
	  g = rand()%0xff;
	  b = rand()%0xff;
	  end_color = RGB(r,g,b);
	}

	gfx_create_dmap(&dmap,MEM_DEF_SHADE);

	gfx_set_dmap_size(dmap, devcm->coding_method, w,h);
    if ((err = gfx_set_dmap_pixmem(dmap, NULL, DEMO_GFX_SHADE, 0)) != SUCCESS) return(err);
#if 1
	size = sizeof(GFX_PALETTE) + sizeof(GFX_RGB) * 256;
	if ((err = mem_calloc(&dmap->palette, MEM_DEF_SHADE, size, 1)) != SUCCESS) return(err);
	dmap->palette->num_colors = 256;
	dmap->palette->color_type = GFX_COLOR_RGB;
	dmap->palette->start_entry = 0;
	dmap->palette->colors.rgb = (GFX_RGB *)((u_int32)(dmap->palette) + 
											sizeof(GFX_PALETTE));

#else
	dmap->palette = &my_palette;
#endif



	blt_create_context(&bctx, gfxdev);
	blt_set_context_dst(bctx,dmap);
	blt_set_context_pix(bctx, 0);
	gfx_create_vport(&vport,gfxdev,0,0,w, h, GFX_VPORT_FRONT);
	gfx_set_vport_dmap(vport,dmap,0,0);
	gfx_set_vport_state(vport,TRUE);
	gfx_update_display(gfxdev,FALSE);
	while(1)
	{
	  jpg_init();
	  jpg_create_context(&ctx);
	  if(slide_mode)
	  {
		/* Open the list file. */
		if(verbose)
		  fprintf(stderr, "Opening %s\n",argv[1]);
		if ((list_file = fopen(argv[1], "r")) == NULL) {
		  fprintf(stderr, "jview: can't open %s\n",argv[1]);
		  exit(EXIT_FAILURE);
		}
	  }
	  while(1)
	  {
		if(slide_mode)
		{
		  if( fgets(in_line, 80, list_file)== NULL)
			break;
		  sscanf(in_line,"%s", file_name);
		}
		else
		  sscanf(argv[1],"%s", file_name);
		
		/* Open the input file. */
		if(verbose)
		{
		  fprintf(stderr, "Opening %s\n",file_name);
		  fflush(stderr);
		}
		err = _os_link(&mod_name,&mod_head,(void *)&mod_exec,&type_lang,&attr_rev);
		if(err != SUCCESS)
		{
		  fflush(stderr);
		  if ((input_file = fopen(file_name, "rb")) == NULL) {
		    fprintf(stderr, "jview: can't open %s\n",file_name);
		    exit(EXIT_FAILURE);
		  }
		  filenum=_fileno(input_file);
		  _os_gs_size(filenum, &jmem_size);
		  mem_malloc(&jsrcmem, MEM_DEF_SHADE,jmem_size);
		  fread(jsrcmem,jmem_size,1,input_file);
		  fclose(input_file);
		  jpg_set_context_source(ctx,jsrcmem,jmem_size);
		}
		else
		{
		  data_size = ((u_int32 *)(mod_exec) - 2);
		  jmem_size = *data_size;
		  mem_malloc(&jsrcmem,MEM_DEF_SHADE,jmem_size);
		  memcpy(jsrcmem,mod_exec,jmem_size);
		  err = _os_unlink(mod_head);
		  if (err != SUCCESS)
		  {
		    fprintf(stderr, "Error unlinking from data module \n");
		    fflush(stderr);
		  }
		  jpg_set_context_source(ctx,jsrcmem,jmem_size);
		}
		if(verbose)
		{
		  fprintf(stderr,"Reading header %s\n", file_name);
		  fflush(stderr);
		}
		jpg_read_header(ctx, TRUE);
		jpg_set_context_quality(ctx,qual);
		  
		if(numer >= 1 && denom >=1)
		  jpg_set_context_scale(ctx,numer,denom);
		if(is_monochrome)
		  jpg_set_context_out_colorspace(ctx,JCS_GRAYSCALE);
		jpg_set_context_color_number(ctx,numcol);
		
		jpg_set_context_dst(ctx,dmap);
		blt_draw_block(bctx, 0,0,w,h);
		if(verbose)
		{
		  fprintf(stderr,"Decompressing %s\n", file_name);
		  fflush(stderr);
		}
		if(!to_fade)
		{
		  gfx_set_vport_state(vport,FALSE);
		  gfx_update_display(gfxdev,FALSE);
		}
		xx=0;
		yy=0;
        if(x==0 && y==0)
		{
		  jpg_get_context_params(&cinfo,ctx);
		  if(w > cinfo.output_width)
			xx = ((w - cinfo.output_width)/2);
		  if(h > cinfo.output_height)
			yy = ((h - cinfo.output_height)/2);
		}
		jpg_decompress(ctx,xx,yy,
					   &scan_num, max_width, max_height,  &ms); 
		if(verbose)
		{
		  fprintf(stderr,"Done decompressing %s\n", file_name);
		  fflush(stderr);
		}
		

		if(to_fade)
		{
		  GFX_RGB color[256];
		  
		  u_int32 i;
		  
		  for(i=0; i<256; i++)
			color[i]=start_color;
		  gfx_set_vport_colors(vport, 0, 256,
							   dmap->palette->color_type, (void*)color);
		}
		else
		  gfx_set_vport_colors(vport,0,dmap->palette->num_colors,
							    dmap->palette->color_type, 
							    dmap->palette->colors.rgb);

		gfx_set_vport_state(vport,TRUE);
		gfx_update_display(gfxdev,FALSE);
		if(to_fade)
		  fade_in(gfxdev, vport, dmap->palette, start_color);
		if(show_time != 0)
		  sleep(show_time); 
		if(to_fade)
		  fade_out(gfxdev, vport, dmap->palette, end_color);
		
		mem_free(jsrcmem);

		if(!slide_mode)
		  break;
		if(to_fade)
		{
		  u_int8 r = rand()%0xff;
		  u_int8 g = rand()%0xff;
		  u_int8 b = rand()%0xff;
		  start_color=end_color;
		  end_color = RGB(r,g,b);
		}
		max_width = w;
		max_height = h;

	  }

	  jpg_destroy_context(ctx);
	  jpg_term();

	  if(slide_mode)
		fclose(list_file);
	  else
		break;
	}

	gfx_set_vport_state(vport,FALSE);
	gfx_update_display(gfxdev,TRUE);
	mem_free(dmap->palette);

	/*do not need if not displaying on the screen */  
	gfx_destroy_vport(vport);
	gfx_destroy_dmap(dmap);
	gfx_close_dev(gfxdev);
	blt_term();
	gfx_term();
	cdb_term();
	
	return 0;			/* suppress no-return-value warnings */
}






