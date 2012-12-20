/*******************************************************************************
 *
 * DESCRIPTION:	MAUI demo to display a graphic image
 *
 * COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
 *				Reserved. Use and/or distribution subject to terms and
 *				conditions of applicable Microware license agreement(s).
 *
 *
 */

#include <MAUI/mauidemo.h>
#include <MAUI/maui.h>
#include <stdio.h>
#include <stdlib.h>

int sleep(unsigned seconds);

void main(int argc, char *argv[])
{
	error_code ec;					/* Error code */
	GFX_DEV_ID gfxdev;              /* Graphics device ID */
	const GFX_DEV_CAP *devcap;		/* Device capabilities */
	GFX_DEV_RES *devres;			/* Default device resolution */
	GFX_DEV_CM *devcm;				/* Default device coding method */
	GFX_DMAP *dst_dmap;					/* Visible drawmap */
	GFX_DMAP *src_dmap;				/* Image drawmap */
	GFX_VPORT_ID vport;             /* Viewport ID */
	BLT_CONTEXT_ID bltctx;          /* Bit_BLT context */

    /* Simple (perhaps too simple) command line parsing */
	if (argc != 2 || *argv[1] == '-') {
      fprintf (stderr, "Usage: %s [filename]\n", argv[0]);
      exit (0);
    }
	
	/* Initialize MAUI APIs used by this program */
	maui_set_error_action(MAUI_ERR_DEFAULT, MAUI_ERR_DEFAULT, MAUI_ERR_FATAL);
	cdb_init();
	blt_init();

	/* Load iff images into source drawmaps */
    if ((ec = demo_load_image(&src_dmap, MEM_DEF_SHADE, MEM_DEF_SHADE,
                              argv[1])) != SUCCESS) {
      mem_destroy_shade(DEMO_GFX_SHADE);
      blt_term();
      cdb_term();
      exit(ec);
    }

	/* Create and setup the destination drawmap */
	gfx_create_dmap(&dst_dmap, MEM_DEF_SHADE);
    
	/* Open the graphics device, create the gfx shade and get devcap info */
	demo_open_gfx_dev(&gfxdev);
	demo_create_gfx_shade(gfxdev, DEMO_GFX_SHADE, NULL);
	gfx_get_dev_cap(&devcap, gfxdev);

    { /* Try to find the coding method */
      u_int8 num_cm = devcap->num_cm;

      devcm = &devcap->cm_info[0];
      while (gfx_get_cm_name(devcm->coding_method) != src_dmap->coding_method) {
        if (--num_cm == 0) {
          fprintf (stderr, "Cannot find coding method %d\n",
                   src_dmap->coding_method);
          switch (src_dmap->coding_method) {
          case GFX_CM_3BIT:
            src_dmap->coding_method = GFX_CM_4BIT;
            break;
          case GFX_CM_5BIT:
          case GFX_CM_6BIT:
          case GFX_CM_7BIT:
            src_dmap->coding_method = GFX_CM_8BIT;
            break;
          default:
            mem_free(src_dmap->pixmem);
            gfx_destroy_dmap(src_dmap);
            mem_destroy_shade(DEMO_GFX_SHADE);
            gfx_close_dev(gfxdev);
            blt_term();
            cdb_term();
            exit (EOS_MAUI_BADCODEMETH);
          }
          fprintf (stderr, "trying coding method %d\n",
                   src_dmap->coding_method);
          /* restart search from the top */
          devcm = &devcap->cm_info[0];
        } else {
          devcm++;
        }
      }
    }

    { /* Look for a resolution */
      u_int8 num_res = devcap->num_res;

      devres = &devcap->res_info[0];
      while ((ec = gfx_set_dmap_size(dst_dmap, devcm->coding_method,
                                     devres->disp_width  / devcm->dm2dp_xmul,
                                     devres->disp_height / devcm->dm2dp_ymul))
             != SUCCESS) {
        if (--num_res == 0) {
          fprintf (stderr,
                   "Cannot find display resolution for coding method %d\n",
                   devcm->coding_method);
          mem_free(src_dmap->pixmem);
          gfx_destroy_dmap(src_dmap);
          mem_destroy_shade(DEMO_GFX_SHADE);
          gfx_close_dev(gfxdev);
          blt_term();
          cdb_term();
          exit (ec);
        }
        devres++;
      }
    }

    /* Init the visable drawmap */
	gfx_set_dmap_pixmem(dst_dmap, NULL, DEMO_GFX_SHADE, 0);
	dst_dmap->palette = src_dmap->palette;
	demo_clear_dmap(gfxdev, dst_dmap, 0);
	
	/* Create the viewport, put the drawmap in it, and display it */
	gfx_create_vport(&vport, gfxdev, 0, 0, devres->disp_width,
		devres->disp_height, GFX_VPORT_FRONT);
	gfx_set_vport_dmap(vport, dst_dmap, 0, 0);
	gfx_set_vport_state(vport, TRUE);
	gfx_update_display(gfxdev, FALSE);
		
	/* Create and setup the Bit-BLT context */
	blt_create_context(&bltctx, gfxdev);
	blt_set_context_dst(bltctx, dst_dmap);
    blt_set_context_src(bltctx, src_dmap);

    { /* Try to center the image */
      GFX_POS sx, sy, dx, dy;
      GFX_DIMEN w, h;

      /* Pick an X */
      if (src_dmap->width < dst_dmap->width) {
        sx = 0;
        dx = (dst_dmap->width - src_dmap->width)/2;
        w = src_dmap->width;
      } else {
        sx = (src_dmap->width - dst_dmap->width)/2;
        dx = 0;
        w = dst_dmap->width;
      }
      /* Pick a Y */
      if (src_dmap->height < dst_dmap->height) {
        sy = 0;
        dy = (dst_dmap->height - src_dmap->height)/2;
        h = src_dmap->height;
      } else {
        sy = (src_dmap->height - dst_dmap->height)/2;
        dy = 0;
        h = dst_dmap->height;
      }
      blt_copy_block(bltctx, dx, dy, sx, sy, w, h);
	}

    /* wait a bit */
	sleep(10);

	/* Shut down */
	blt_destroy_context(bltctx);
	gfx_set_vport_state(vport, FALSE);
	gfx_update_display(gfxdev, FALSE);
	gfx_destroy_vport(vport);
	mem_free(dst_dmap->pixmem);
	gfx_destroy_dmap(dst_dmap);
    mem_free(src_dmap->pixmem);
    gfx_destroy_dmap(src_dmap);
	mem_destroy_shade(DEMO_GFX_SHADE);
	gfx_close_dev(gfxdev);
	blt_term();
	cdb_term();

	exit(SUCCESS);
}
