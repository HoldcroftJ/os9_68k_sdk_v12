/******************************************************************************
*
* DESCRIPTION:	Fast draw demo program for MAUI
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

void main(int argc, char *argv[])
{
	GFX_DEV_ID gfxdev;              /* Graphics device ID */
	const GFX_DEV_CAP *devcap;		/* Device capabilities */
	GFX_DEV_RES *devres;			/* Default device resolution */
	GFX_DEV_CM *devcm;				/* Default device coding method */
	GFX_DMAP *dmap;					/* Visible drawmap */
	GFX_VPORT_ID vport;             /* Viewport ID */
	BLT_CONTEXT_ID bltctx;          /* Bit_BLT context */
	GFX_PIXEL pixval;				/* Pixel value for drawing */
    u_int32 resindex=0;             /* Resolution index */
	u_int32 cmindex=0;              /* Coding Method index */
    u_int32 box_cnt = 0;
    u_int32 timeout = 10;           /* default time to run example */

    if (argc >= 2) {
	  int i = 1;
	  char *ptr;

	  while (i < argc) {
		ptr = argv[i++];
		if (!strncmp(ptr,"-cm",3)) {
		  ptr += 3;
		  if (*ptr == '=')
			ptr++;
		  cmindex = atoi(ptr);
		  printf("Using coding method index = %d\n", cmindex);
		} else if (!strncmp(ptr,"-t",2)) {
          ptr +=2;
		  if (*ptr == '=')
			ptr++;
		  timeout = atoi(ptr);
		  printf("Using timeout of %d seconds\n", timeout);
		} else if (*ptr == '-') {
		  fprintf(stderr, "Usage: %s [res index] [-t[=]timeout] [-cm[=]coding meth. index]\n", argv[0]);
		  exit(0);
		} else {
		  resindex = atoi(ptr);
		  printf("Using resolution index = %d\n",resindex);
		}
	  }
	}

	/* Initialize MAUI APIs used by this program */
	maui_set_error_action(MAUI_ERR_DEFAULT, MAUI_ERR_DEFAULT, MAUI_ERR_FATAL);
	cdb_init();
	blt_init();

	/* Open the graphics device, create the gfx shade and get devcap info */
	demo_open_gfx_dev(&gfxdev);
	demo_create_gfx_shade(gfxdev, DEMO_GFX_SHADE, NULL);
	gfx_get_dev_cap(&devcap, gfxdev);
    if (resindex >= devcap->num_res) {
      printf ("Max res index is %d. Reseting to 0\n", devcap->num_res);
      resindex = 0;
    }
	if (cmindex >= devcap->num_cm) {
      printf ("Max cm index is %d. Reseting to 0\n", devcap->num_cm);
      cmindex = 0;
	}
	devres = &devcap->res_info[resindex];
	devcm = &devcap->cm_info[cmindex];
    if (resindex!=0)
      gfx_set_display_size(gfxdev, 
                           devres->disp_width, devres->disp_height, 
                           devres->intl_mode, devres->refresh_rate);
    
	/* Create and setup the destination drawmap */
	gfx_create_dmap(&dmap, MEM_DEF_SHADE);
	gfx_set_dmap_size(dmap, devcm->coding_method,
		devres->disp_width  / devcm->dm2dp_xmul,
		devres->disp_height / devcm->dm2dp_ymul);
	gfx_set_dmap_pixmem(dmap, NULL, DEMO_GFX_SHADE, 0);
	dmap->palette = &demo_palette;
	demo_clear_dmap(gfxdev, dmap, 0);
	
	/* Create the viewport, put the drawmap in it, and display it */
	gfx_create_vport(&vport, gfxdev, 0, 0, devres->disp_width,
		devres->disp_height, GFX_VPORT_FRONT);
	gfx_set_vport_dmap(vport, dmap, 0, 0);
	gfx_set_vport_state(vport, TRUE);
	gfx_update_display(gfxdev, FALSE);
		
	/* Create and setup the Bit-BLT context */
	blt_create_context(&bltctx, gfxdev);
	blt_set_context_dst(bltctx, dmap);

    /* Set time out in "timeout" seconds */
    demo_set_timeout(timeout);

    /* Set number of colors depending on pixel depth */
    { 
      u_int32 Demo_Num_Colors;
      
      switch ( gfx_get_cm_name(devcm->coding_method) ) {
      case GFX_CM_1BIT:
        Demo_Num_Colors = 2;
        break;
      case GFX_CM_2BIT:
        Demo_Num_Colors = 4;
        break;
      default:        
        Demo_Num_Colors = 14;
        break;
      }
      
      /* Randomly draw blocks to the destination */
      {
        GET_DEMO_TLDATA();
        while (demo_notdone) {
          GFX_POS x1 = demo_random(0, dmap->width - 1);
          GFX_POS y1 = demo_random(0, dmap->height - 1);
          GFX_POS x2 = demo_random(0, dmap->width - 1);
          GFX_POS y2 = demo_random(0, dmap->height - 1);
          if (x1 > x2) {
            GFX_POS temp = x2; x2 = x1; x1 = temp;
          }
          if (y1 > y2) {
            GFX_POS temp = y2; y2 = y1; y1 = temp;
          }
          demo_color_pixval(demo_random(1, Demo_Num_Colors),
                            dmap->coding_method, &pixval);
          blt_set_context_pix(bltctx, pixval);
          blt_draw_block(bltctx, x1, y1, x2 - x1 + 1, y2 - y1 + 1);
          box_cnt++;
          if (!box_cnt) printf("Counter wrap-around\n");
        }
      }
    }

    printf("%u boxes of random size and color in %u seconds\n", box_cnt, timeout);

	/* Shut down */

	blt_destroy_context(bltctx);
	gfx_set_vport_state(vport, FALSE);
	gfx_update_display(gfxdev, FALSE);
	gfx_destroy_vport(vport);
	mem_sfree(DEMO_GFX_SHADE, dmap->pixmem);
	gfx_destroy_dmap(dmap);
	mem_destroy_shade(DEMO_GFX_SHADE);
	gfx_close_dev(gfxdev);
	blt_term();
	cdb_term();
	exit(SUCCESS);
}
