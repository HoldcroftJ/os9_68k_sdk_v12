/******************************************************************************
*
* DESCRIPTION:	Aloha demo program for MAUI
*
* CREATED ON:	02/08/96
*
* COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
*				Reserved. Use and/or distribution subject to terms and
*				conditions of applicable Microware license agreement(s).
*
*/
#include <MAUI/mauidemo.h>
#include <MAUI/maui.h>

/*****************************************************************************
 * Color macros
 *****************************************************************************/
#define _TO_RGB555(r,g,b) ((((r)&0xf8)<<7)|(((g)&0xf8)<<2)|(((b)>>3)&0x1f))
#define _TO_RGB565(r,g,b) ((((r)&0xf8)<<8)|(((g)&0xfc)<<3)|(((b)>>3)&0x1f))
#define _TO_RGB655(r,g,b) ((((r)&0xfc)<<8)|(((g)&0xf8)<<2)|(((b)>>3)&0x1f))
#define _TO_RGB556(r,g,b) ((((r)&0xf8)<<8)|(((g)&0xf8)<<3)|(((b)>>2)&0x2f))
#define _GET_R(rgb) (((rgb)>>16) & 0xff)
#define _GET_G(rgb) (((rgb)>>8) & 0xff)
#define _GET_B(rgb) ((rgb) & 0xff)

/*******************************************************************************
* GLOBALS
*******************************************************************************/
char * font_filename = "default.fnt";
BOOLEAN alternate_font = FALSE;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/
static void parse_options(int argc, char **argv);
static void print_usage(char* modname);


void main(int argc, char *argv[])
{
	GFX_DEV_ID gfxdev;              /* Graphics device ID */
	const GFX_DEV_CAP *devcap;		/* Device capabilities */
	GFX_DEV_RES *devres;			/* Default device resolution */
	GFX_DEV_CM *devcm;				/* Default device coding method */
	GFX_DMAP *dmap;					/* Visible drawmap */
	GFX_VPORT_ID vport;             /* Viewport ID */
	MSG_MBOX_ID mbox;				/* Mailbox ID */
	INP_DEV_ID inpdev;				/* Input device ID */
	TXT_FONT *font;					/* Font */
	TXT_CONTEXT_ID txtctx;			/* Text context ID */
	GFX_PIXEL exptbl[] = {8,14};	/* Pixel expansion table */
	size_t txtlen;					/* Maximum length of text string */
	INP_MSG inpmsg;					/* Input message structure */
	error_code ec;
	GFX_PIXEL ctmp;

    parse_options(argc, argv);

	/* Initialize MAUI APIs used by this program */
	maui_set_error_action(MAUI_ERR_DEFAULT, MAUI_ERR_DEFAULT, MAUI_ERR_FATAL);
	cdb_init();
	txt_init();
	inp_init();

	/* Open the graphics device, create the gfx shade and get devcap info */
	demo_open_gfx_dev(&gfxdev);
	demo_create_gfx_shade(gfxdev, DEMO_GFX_SHADE, NULL);
	gfx_get_dev_cap(&devcap, gfxdev);
	devres = &devcap->res_info[0];
	devcm = &devcap->cm_info[0];

	/* Create and setup the destination drawmap */
	gfx_create_dmap(&dmap, MEM_DEF_SHADE);
	gfx_set_dmap_size(dmap, devcm->coding_method,
		devres->disp_width  / devcm->dm2dp_xmul,
		devres->disp_height / devcm->dm2dp_ymul);
	gfx_set_dmap_pixmem(dmap, NULL, DEMO_GFX_SHADE, 0);
	dmap->palette = &demo_palette;

        /* Set expansion table and background color depending on pixel depth */
        switch ( gfx_get_cm_name(devcm->coding_method) ) {
	     case GFX_CM_1BIT:
              demo_clear_dmap(gfxdev, dmap, 0);
              exptbl[0] = 0;
              exptbl[1] = 1;
              break;
         case GFX_CM_2BIT:
              demo_clear_dmap(gfxdev, dmap, 2);
              exptbl[0] = 1;
              exptbl[1] = 3;
              break;
		 case GFX_CM_RGB555:		                /* 16 bit combo's */
			  ctmp = demo_colors[14];
		      exptbl[1] = _TO_RGB555((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
		      ctmp = demo_colors[8];
		      exptbl[0] = _TO_RGB555((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
			  demo_clear_dmap(gfxdev, dmap, exptbl[0]);
		      break;
		 case GFX_CM_RGB655:
			  ctmp = demo_colors[14];
		      exptbl[1] = _TO_RGB655((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
		      ctmp = demo_colors[8];
		      exptbl[0] = _TO_RGB655((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
		      demo_clear_dmap(gfxdev, dmap, exptbl[0]);
		      break;
		 case GFX_CM_RGB565:
		      ctmp = demo_colors[14];
		      exptbl[1] = _TO_RGB565((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
		      ctmp = demo_colors[8];
		      exptbl[0] = _TO_RGB565((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
		      demo_clear_dmap(gfxdev, dmap, exptbl[0]);
		      break;
		 case GFX_CM_RGB556:
		      ctmp = demo_colors[14];
		      exptbl[1] = _TO_RGB556((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
		      ctmp = demo_colors[8];
		      exptbl[0] = _TO_RGB556((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
		  	  demo_clear_dmap(gfxdev, dmap, exptbl[0]);
		  	  break;
         default:        
	         demo_clear_dmap(gfxdev, dmap, 8);
             break;
	}
	
	/* Create the viewport, put the drawmap in it, and display it */
	gfx_create_vport(&vport, gfxdev, 0, 0, devres->disp_width,
		devres->disp_height, GFX_VPORT_FRONT);
	gfx_set_vport_dmap(vport, dmap, 0, 0);
	gfx_set_vport_state(vport, TRUE);
	gfx_update_display(gfxdev, FALSE);
		
	/* Create the mailbox and open the input device */
	msg_create_mbox(&mbox, "aloha_mb", 1, sizeof(INP_MSG), MEM_ANY);
	demo_open_inp_dev(&inpdev, mbox);

	/* Create and setup the text context */
	if ((ec = demo_get_ucm_font(&font, MEM_DEF_SHADE, font_filename)) == SUCCESS) {
		txt_create_context(&txtctx, gfxdev);
		txt_set_context_dst(txtctx, dmap);
		txt_set_context_font(txtctx, font);
		txt_set_context_exptbl(txtctx, 2, exptbl);
	
		/* Draw the text strings */
		txtlen = ULONG_MAX;
		txt_draw_mbs(NULL, txtctx, "Aloha MAUI demo...",  &txtlen, 20, 40, NULL);
		txtlen = ULONG_MAX;
		txt_draw_mbs(NULL, txtctx, "Press a key to exit", &txtlen, 20, 65, NULL);

		/* Wait for user input */
		msg_read(mbox, &inpmsg, MSG_TYPE_ANY, MSG_BLOCK);
	
		/* Shut down */
		txt_destroy_context(txtctx);
		demo_release_ucm_font(font);
	}
	else {
		fprintf (stderr, "%d - cannot link to '%s'\n", ec, font_filename);
	}
	inp_close_dev(inpdev);
 	msg_close_mbox(mbox);
	gfx_set_vport_state(vport, FALSE);
	gfx_update_display(gfxdev, FALSE);
	gfx_destroy_vport(vport);
	mem_sfree(dmap->pixmem_shade, dmap->pixmem);
	gfx_destroy_dmap(dmap);
	mem_destroy_shade(DEMO_GFX_SHADE);
	gfx_close_dev(gfxdev);
	inp_term();
	txt_term();
	cdb_term();

	exit(SUCCESS);
}

static void parse_options(int argc, char **argv)
{
  char *modname = *argv;
  char *ptr;

  /* parse the command line parameters */
  while (--argc) {
    if (*(ptr = *++argv) == '-') {
      while(*++ptr) {
        switch(*ptr|0x20) { /* to lower */

        case 'f':           /* resolution */
          if (*++ptr == '=') ptr++;   /* optional '=' */
          font_filename = ptr ;
          while (*++ptr);
          ptr--;  

          if ( *font_filename != '\0' )
            alternate_font = TRUE;
          break;  

        case 'h':
        case '?':   /* help message */
          print_usage(modname);
          exit(0);

        default:    /* illegal option */
          fprintf (stderr, "\nunknown option: '%c'\n",*ptr);
          print_usage(modname);
          exit(1);
 
        }   /* end of switch */
      }   /* end of while */
    } /* end of if */
	else
	{
		print_usage(modname);
	}
  } /* end of while */
}
 
static void print_usage(char* modname)
{
  fprintf (stderr, "Usage: %s <opts>\n", modname);
  fprintf (stderr, "Description: Demo app that displays text on the screen. \n");
  fprintf (stderr, "<opts>: -h or -?            This help\n");
  fprintf (stderr, "        -f[=]<filename>     filename - select alternate font file.\n");
  exit(0);
}

