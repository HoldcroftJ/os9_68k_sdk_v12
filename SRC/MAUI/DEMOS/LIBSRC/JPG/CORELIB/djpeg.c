/*
 * djpeg.c
 *
 * This program uses the JPEG libraries of the Independent JPEG Group's software.
 *
 * This file contains a command-line user interface for the JPEG decompressor.
 */

#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include "jversion.h"		/* for version message */
#include <MAUI/maui.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>		/* to declare isprint() */


/*
 * Argument-parsing code.
 * The switch parser is designed to be useful with DOS-style command line
 * syntax, ie, intermixed switches and file names, where only the switches
 * to the left of a given file name affect processing of that file.
 * The main program in this file doesn't actually use this capability...
 */


LOCAL void
usage (void)
/* complain about bad command line */
{
  fprintf(stderr, "[inputfile]\n");

  fprintf(stderr, "Switches (names may be abbreviated):\n");
  fprintf(stderr, "  -colors N      Reduce image to no more than N colors\n");
  fprintf(stderr, "  -fast          Fast, low-quality processing\n");
  fprintf(stderr, "  -grayscale     Force grayscale output\n");
#ifdef IDCT_SCALING_SUPPORTED
  fprintf(stderr, "  -scale M/N     Scale output image by fraction M/N, eg, 1/8\n");
#endif
  fprintf(stderr, "Switches for advanced users:\n");
#ifdef DCT_ISLOW_SUPPORTED
  fprintf(stderr, "  -dct int       Use integer DCT method%s\n",
	  (JDCT_DEFAULT == JDCT_ISLOW ? " (default)" : ""));
#endif
#ifdef DCT_IFAST_SUPPORTED
  fprintf(stderr, "  -dct fast      Use fast integer DCT (less accurate)%s\n",
	  (JDCT_DEFAULT == JDCT_IFAST ? " (default)" : ""));
#endif
#ifdef DCT_FLOAT_SUPPORTED
  fprintf(stderr, "  -dct float     Use floating-point DCT method%s\n",
	  (JDCT_DEFAULT == JDCT_FLOAT ? " (default)" : ""));
#endif
  fprintf(stderr, "  -dither fs     Use F-S dithering (default)\n");
  fprintf(stderr, "  -dither none   Don't use dithering in quantization\n");
  fprintf(stderr, "  -dither ordered  Use ordered dither (medium speed, quality)\n");
#ifdef QUANT_2PASS_SUPPORTED
  fprintf(stderr, "  -map FILE      Map to colors used in named image file\n");
#endif
  fprintf(stderr, "  -nosmooth      Don't use high-quality upsampling\n");
#ifdef QUANT_1PASS_SUPPORTED
  fprintf(stderr, "  -onepass       Use 1-pass quantization (fast, low quality)\n");
#endif
  fprintf(stderr, "  -maxmemory N   Maximum memory to use (in kbytes)\n");
  fprintf(stderr, "  -outfile name  Specify name for output file\n");
  fprintf(stderr, "  -verbose  or  -debug   Emit debug output\n");
  exit(EXIT_FAILURE);
}


LOCAL int
parse_switches (j_decompress_ptr cinfo, int argc, char **argv,
		int last_file_arg_seen, boolean for_real)
/* Parse optional switches.
 * Returns argv[] index of first file-name argument (== argc if none).
 * Any file names with indexes <= last_file_arg_seen are ignored;
 * they have presumably been processed in a previous iteration.
 * (Pass 0 for last_file_arg_seen on the first or only iteration.)
 * for_real is FALSE on the first (dummy) pass; we may skip any expensive
 * processing.
 */
{
  int argn;
  char * arg;

  /* Set up default JPEG parameters. */
  cinfo->err->trace_level = 0;

  /* Scan command line options, adjust parameters */

  for (argn = 1; argn < argc; argn++) {
    arg = argv[argn];
    arg++;			/* advance past switch marker character */

    if (keymatch(arg, "colors", 1) || keymatch(arg, "colours", 1) ||
	       keymatch(arg, "quantize", 1) || keymatch(arg, "quantise", 1)) {
      /* Do color quantization. */
      int val;

      if (++argn >= argc)	/* advance to next argument */
	usage();
      if (sscanf(argv[argn], "%d", &val) != 1)
	usage();
      cinfo->desired_number_of_colors = val;
      cinfo->quantize_colors = TRUE;

    } else if (keymatch(arg, "dct", 2)) {
      /* Select IDCT algorithm. */
      if (++argn >= argc)	/* advance to next argument */
	usage();
      if (keymatch(argv[argn], "int", 1)) {
	cinfo->dct_method = JDCT_ISLOW;
      } else if (keymatch(argv[argn], "fast", 2)) {
	cinfo->dct_method = JDCT_IFAST;
      } else if (keymatch(argv[argn], "float", 2)) {
	cinfo->dct_method = JDCT_FLOAT;
      } else
	usage();

    } else if (keymatch(arg, "dither", 2)) {
      /* Select dithering algorithm. */
      if (++argn >= argc)	/* advance to next argument */
	usage();
      if (keymatch(argv[argn], "fs", 2)) {
	cinfo->dither_mode = JDITHER_FS;
      } else if (keymatch(argv[argn], "none", 2)) {
	cinfo->dither_mode = JDITHER_NONE;
      } else if (keymatch(argv[argn], "ordered", 2)) {
	cinfo->dither_mode = JDITHER_ORDERED;
      } else
	usage();

    } else if (keymatch(arg, "debug", 1) || keymatch(arg, "verbose", 1)) {
      /* Enable debug printouts. */
      /* On first -d, print version identification */
      static boolean printed_version = FALSE;

      if (! printed_version) {
	fprintf(stderr, "Independent JPEG Group's DJPEG, version %s\n%s\n",
		JVERSION, JCOPYRIGHT);
	printed_version = TRUE;
      }
      cinfo->err->trace_level++;

    } else if (keymatch(arg, "fast", 1)) {
      /* Select recommended processing options for quick-and-dirty output. */
      cinfo->two_pass_quantize = FALSE;
      cinfo->dither_mode = JDITHER_NONE;
      if (! cinfo->quantize_colors) /* don't override an earlier -colors */
	cinfo->desired_number_of_colors = 216;
      cinfo->dct_method = JDCT_FASTEST;
      cinfo->do_fancy_upsampling = FALSE;

    } else if (keymatch(arg, "grayscale", 2) || keymatch(arg, "greyscale",2)) {
      /* Force monochrome output. */
      cinfo->out_color_space = JCS_GRAYSCALE;

    } else if (keymatch(arg, "map", 3)) {
      /* Quantize to a color map taken from an input file. */
      if (++argn >= argc)	/* advance to next argument */
	usage();
      if (for_real) {		/* too expensive to do twice! */
	ERREXIT(cinfo, JERR_NOT_COMPILED);
      }

    } else if (keymatch(arg, "maxmemory", 3)) {
      /* Maximum memory in Kb (or Mb with 'm'). */
      long lval;
      char ch = 'x';

      if (++argn >= argc)	/* advance to next argument */
	usage();
      if (sscanf(argv[argn], "%ld%c", &lval, &ch) < 1)
	usage();
      if (ch == 'm' || ch == 'M')
	lval *= 1000L;
      cinfo->mem->max_memory_to_use = lval * 1000L;

    } else if (keymatch(arg, "nosmooth", 3)) {
      /* Suppress fancy upsampling */
      cinfo->do_fancy_upsampling = FALSE;

    } else if (keymatch(arg, "onepass", 3)) {
      /* Use fast one-pass quantization. */
      cinfo->two_pass_quantize = FALSE;

    } else if (keymatch(arg, "outfile", 4)) {
      /* Set output file name. */
      if (++argn >= argc)	/* advance to next argument */
	usage();

    } else if (keymatch(arg, "scale", 1)) {
      /* Scale the output image by a fraction M/N. */
      if (++argn >= argc)	/* advance to next argument */
	usage();
      if (sscanf(argv[argn], "%d/%d",
		 &cinfo->scale_num, &cinfo->scale_denom) != 2)
	usage();
    }
  }

  return argn;			/* return index of next arg (file name) */
}


/*
 * Marker processor for COM markers.
 * This replaces the library's built-in processor, which just skips the marker.
 * We want to print out the marker as text, if possible.
 * Note this code relies on a non-suspending data source.
 */

LOCAL unsigned int
jpeg_getc (j_decompress_ptr cinfo)
/* Read next byte */
{
  struct jpeg_source_mgr * datasrc = cinfo->src;

  if (datasrc->bytes_in_buffer == 0) {
    if (! (*datasrc->fill_input_buffer) (cinfo))
      ERREXIT(cinfo, JERR_CANT_SUSPEND);
  }
  datasrc->bytes_in_buffer--;
  return GETJOCTET(*datasrc->next_input_byte++);
}


METHODDEF boolean
COM_handler (j_decompress_ptr cinfo)
{
  boolean traceit = (cinfo->err->trace_level >= 1);
  INT32 length;
  unsigned int ch;
  unsigned int lastch = 0;

  length = jpeg_getc(cinfo) << 8;
  length += jpeg_getc(cinfo);
  length -= 2;			/* discount the length word itself */

  if (traceit)
    fprintf(stderr, "Comment, length %ld:\n", (long) length);

  while (--length >= 0) {
    ch = jpeg_getc(cinfo);
    if (traceit) {
      /* Emit the character in a readable form.
       * Nonprintables are converted to \nnn form,
       * while \ is converted to \\.
       * Newlines in CR, CR/LF, or LF form will be printed as one newline.
       */
      if (ch == '\r') {
	fprintf(stderr, "\n");
      } else if (ch == '\n') {
	if (lastch != '\r')
	  fprintf(stderr, "\n");
      } else if (ch == '\\') {
	fprintf(stderr, "\\\\");
      } else if (isprint(ch)) {
	putc(ch, stderr);
      } else {
	fprintf(stderr, "\\%03o", ch);
      }
      lastch = ch;
    }
  }

  if (traceit)
    fprintf(stderr, "\n");

  return TRUE;
}

/*
 * The main program.
 */

GLOBAL int
main (int argc, char **argv)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
#ifdef PROGRESS_REPORT
  struct cdjpeg_progress_mgr progress;
#endif
  int file_index;
  FILE * input_file;
  JDIMENSION num_scanlines;
  GFX_DMAP *dmap;
  GFX_RGB *color;
  error_code err;
  u_int32 size;
  GFX_DEV_ID gfxdev;
  GFX_VPORT_ID vport; 
  u_int32 count;
  int jmem_size,filenum;
  u_char *pixmem,*jsrcmem;
  clock_t start_ticks,stop_ticks;
 

  gfx_init();
  
  gfx_open_dev(&gfxdev,"/gfx");

  /*allocate memory for draw map */
  mem_create_shade(1,MEM_SHADE_NORMAL,0x80,256,256,MEM_OV_ATTACHED,TRUE);
  gfx_create_dmap(&dmap,1);
  size = sizeof(GFX_PALETTE) + sizeof(GFX_RGB) * 256;
  /*allocate memory for the palette and colors */
  if (err = mem_calloc(&dmap->palette, 1, size, 1)) return(err);
  dmap->palette->num_colors = 256;
  dmap->palette->color_type = GFX_COLOR_RGB;
  dmap->palette->start_entry = 0;
  dmap->palette->colors.rgb = (GFX_RGB *)((u_int32)(dmap->palette) + sizeof(GFX_PALETTE));

  /* Initialize the JPEG decompression object with default error handling. */
  cinfo.err = jpeg_std_error(&jerr);

  jpeg_create_decompress(&cinfo);

  /* Insert custom COM marker processor. */
  jpeg_set_marker_processor(&cinfo, JPEG_COM, COM_handler);
 
  /* Open the input file. */

    if ((input_file = fopen(argv[1], READ_BINARY)) == NULL) {
      fprintf(stderr, "APP ERROR: can't open %s\n",argv[1]);
      exit(EXIT_FAILURE);
    }
 
  /* get size of file */
  filenum=fileno(input_file);
  jmem_size=_gs_size(filenum);
  /* read source file into memory */
  jsrcmem=(u_char *)malloc(jmem_size);
  
  fread(jsrcmem,jmem_size,1,input_file);
  fclose(input_file);
  /* Specify data source for decompression */
  jpeg_stdio_src(&cinfo, jsrcmem,jmem_size);

  /* Read file header, set default decompression parameters */
  (void) jpeg_read_header(&cinfo, TRUE); 
 
  /*parse through command line to set options */
  parse_switches(&cinfo,argc,argv,0,TRUE);

  /*quantize colors set TRUE to take 24 bit image down to 8 bit image */
  cinfo.quantize_colors=TRUE;
  cinfo.desired_number_of_colors=256;
  cinfo.two_pass_quantize=FALSE;

  /* Start decompressor */
  (void) jpeg_start_decompress(&cinfo);

  /*do not clip if image does not need to be displayed on screen */
  /*make sure output width and height does not exceed screen res 
    */
if (cinfo.output_width > 360)
    cinfo.output_width=360;
  if (cinfo.output_height > 240)
    cinfo.output_height=240;    

  
   /* setup image attributes */
  gfx_set_dmap_size(dmap,GFX_CM_8BIT,cinfo.output_width,cinfo.output_height);
  gfx_set_dmap_pixmem(dmap,NULL,1,0);

/*used just to test on power pc */
  /*allocate memory for buffer instead of using dmap
  pixmem=(u_char *)malloc(cinfo.output_width*cinfo.output_height);
 */
 
  pixmem=(u_char *)(dmap->pixmem);

  start_ticks=clock();

   /* Process data */
  /* read jpeg data into the MAUI pixmem area */
  while (cinfo.output_scanline < cinfo.output_height) {
    num_scanlines=jpeg_read_scanlines(&cinfo,
									  (JSAMPARRAY)&pixmem,
									  cinfo.rec_outbuf_height);
    pixmem += num_scanlines*dmap->line_size;

    /* for power pc test
    pixmem+=cinfo.output_width*cinfo.output_components;
    */
} 

  stop_ticks=clock();

/* DO NOT NEED FOR TESTING PURPOSES - NO NEED TO DISPLAY ON THE SCREEN */
  for (count = 0; count < cinfo.actual_number_of_colors; count++)
    dmap->palette->colors.rgb[count] = (cinfo.colormap[0][count]<<16) | (cinfo.colormap[1][count]<<8) | (cinfo.colormap[2][count]);

  gfx_create_vport(&vport,gfxdev,0,0,cinfo.output_width*2,cinfo.output_height,GFX_VPORT_FRONT);
  gfx_set_vport_dmap(vport,dmap,0,0);
  
  gfx_set_vport_state(vport,TRUE);
 
  gfx_update_display(gfxdev,FALSE);


  sleep(10);

  /* print to standard error a brief report of image */
  fprintf(stderr,"IMAGE : %s\n",argv[1]);
  fprintf(stderr,"*************************\n");
  fprintf(stderr,"Actual image size : %d x %d\n",cinfo.image_width,cinfo.image_height);
  fprintf(stderr,"Output image size : %d x %d\n",cinfo.output_width,cinfo.output_height);
  fprintf(stderr,"Compressed Image Memory allocated : %d bytes\n",jmem_size);
  fprintf(stderr,"Decompressed Image Mem allocated  : %d bytes\n",cinfo.output_width * cinfo.output_height);
  switch(cinfo.dct_method)
    {
    case JDCT_ISLOW:
      fprintf(stderr,"Integer DCT method used\n");
      break;

    case JDCT_IFAST:
      fprintf(stderr,"Fast Integer DCT method used\n");
      break;

    case JDCT_FLOAT:
      fprintf(stderr,"Floating-point DCT method used\n");
      break;
    }

   stop_ticks-=start_ticks;
  fprintf(stderr,"Decompression time: %d ticks or %d secs\n",stop_ticks,stop_ticks/CLOCKS_PER_SEC);
  fprintf(stderr,"*************************\n");


  /* Finish decompression and release memory.
   * I must do it in this order because output module has allocated memory
   * of lifespan JPOOL_IMAGE; it needs to finish before releasing memory.
   */
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

#ifdef PROGRESS_REPORT
  end_progress_monitor((j_common_ptr) &cinfo);
#endif
/*need to terminate all maui objects */
  
/*do not need if not displaying to screen */
  gfx_set_vport_state(vport,FALSE);
  gfx_update_display(gfxdev,TRUE);
  mem_free(dmap->pixmem);
  mem_free(dmap->palette);
  free(jsrcmem);

/*do not need if not displaying on the screen */  
  gfx_destroy_vport(vport);
  gfx_destroy_dmap(dmap);
  gfx_close_dev(gfxdev);
  mem_destroy_shade(1);

  gfx_term();
  /* All done. */
  exit(jerr.num_warnings ? EXIT_WARNING : EXIT_SUCCESS);
  return 0;			/* suppress no-return-value warnings */
}






