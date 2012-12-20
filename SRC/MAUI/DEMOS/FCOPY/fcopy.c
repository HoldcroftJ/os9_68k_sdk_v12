/******************************************************************************
*
* DESCRIPTION:	Fast copy demo program for MAUI
*
* CREATED ON:	02/08/96
*
* COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
*				Reserved. Use and/or distribution subject to terms and
*				conditions of applicable Microware license agreement(s).
*/
#include <MAUI/mauidemo.h>
#include <MAUI/maui.h>

#define NUM_IMAGES 3

char **image_name;
char *image_name_c4[] = {
	"fun.c4", "mwlogo.c4", "travel.c4"
};
char *image_name_c8[] = {
	"fun.c8", "mwlogo.c8", "travel.c8"
};

void main(int argc, char *argv[])
{
	error_code ec;					/* Error code */
	GFX_DEV_ID gfxdev;              /* Graphics device ID */
	const GFX_DEV_CAP *devcap;		/* Device capabilities */
	GFX_DEV_RES *devres;			/* Default device resolution */
	GFX_DEV_CM *devcm;				/* Default device coding method */
	GFX_DMAP *image[NUM_IMAGES];	/* Image drawmaps */
	GFX_DMAP *dmap;					/* Visible drawmap */
	GFX_VPORT_ID vport;             /* Viewport ID */
	BLT_CONTEXT_ID bltctx;          /* Bit_BLT context */
	u_int32 i;						/* Loop counter */
    u_int32 box_cnt = 0;
    u_int32 timeout = 10;           /* default time to run example */

    if (argc >= 2) {
	  int i = 1;
	  char *ptr;

	  while (i < argc) {
		ptr = argv[i++];
        if (!strncmp(ptr,"-t",2)) {
          ptr +=2;
		  if (*ptr == '=')
			ptr++;
		  timeout = atoi(ptr);
		  printf("Using timeout of %d seconds\n", timeout);
		} else {
		  fprintf(stderr, "Usage: %s [-t[=]timeout]\n", argv[0]);
		  exit(0);
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
	devres = &devcap->res_info[0];
	devcm = &devcap->cm_info[0];

	/* Make sure we have images for the default coding method */
	switch (gfx_get_cm_name( devcm->coding_method )) {
		case GFX_CM_4BIT:
			image_name = image_name_c4;
			break;
		case GFX_CM_8BIT:
			image_name = image_name_c8;
			break;
		default:
			exit (_errmsg(0, "Default coding method %d is not supported.\n",
				devcm->coding_method));
	}


	/* Load iff images into source drawmaps */
	for(i = 0; i < NUM_IMAGES; i++) {
		if ((ec = demo_load_image(&image[i], MEM_DEF_SHADE, MEM_DEF_SHADE,
			image_name[i])) != SUCCESS) {
			exit(ec);
		}
	}
		
	/* Create and setup the destination drawmap */
	gfx_create_dmap(&dmap, MEM_DEF_SHADE);
	gfx_set_dmap_size(dmap, devcm->coding_method,
		devres->disp_width  / devcm->dm2dp_xmul,
		devres->disp_height / devcm->dm2dp_ymul);
	gfx_set_dmap_pixmem(dmap, NULL, DEMO_GFX_SHADE, 0);
	dmap->palette = image[0]->palette;
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

	/* Copy source to random locations on destination */
    {
      GET_DEMO_TLDATA();
      while (demo_notdone) {
        u_int32 n = demo_random(0, NUM_IMAGES - 1);
        GFX_POS x = demo_random(0, dmap->width - image[n]->width);
        GFX_POS y = demo_random(0, dmap->height - image[n]->height);
        blt_set_context_src(bltctx, image[n]);
        blt_copy_block(bltctx, x, y, 0, 0, image[n]->width, image[n]->height);
        box_cnt++;
        if (!box_cnt) printf("Counter wrap-around\n");
      }
    }
    
    printf("%u random images in %u seconds\n", box_cnt, timeout);

	/* Shut down */
	blt_destroy_context(bltctx);
	gfx_set_vport_state(vport, FALSE);
	gfx_update_display(gfxdev, FALSE);
	gfx_destroy_vport(vport);
	mem_sfree(dmap->pixmem_shade, dmap->pixmem);
	gfx_destroy_dmap(dmap);
	for(i = 0; i < NUM_IMAGES; i++){
		mem_sfree(image[i]->pixmem_shade, image[i]->pixmem);
		gfx_destroy_dmap(image[i]);
	}
	mem_destroy_shade(DEMO_GFX_SHADE);
	gfx_close_dev(gfxdev);
	blt_term();
	cdb_term();

	exit(SUCCESS);
}
