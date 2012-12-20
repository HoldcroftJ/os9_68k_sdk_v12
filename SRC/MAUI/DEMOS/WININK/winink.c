/******************************************************************************
*
* DESCRIPTION:	Window inking demo. The windowing device must already be
*				created before you run this program.
*
* CREATED ON:	08/14/96
*
* COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
*				Reserved. Use and/or distribution subject to terms and
*				conditions of applicable Microware license agreement(s).
*
*/
#include <MAUI/mauidemo.h>
#include <MAUI/maui.h>
#include <cglob.h>
#include <signal.h>
#include <process.h>

error_code expand_cursor(const WIN_CURSOR *src, WIN_CURSOR **d, GFX_CM coding_method);

/*******************************************************************************
* Colors
*******************************************************************************/

#define COLOR_BLACK	0
#define COLOR_MGREY	1

#define NUM_COLORS	2				/* Total number of colors */
GFX_PIXEL pix[NUM_COLORS];			/* Pixel values for colors */
volatile u_int32 done=0;            /* done flag */

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
* PENCIL CURSOR
*******************************************************************************/
#define PENCIL_WIDTH 12   
#define PENCIL_HEIGHT 16  
#define PENCIL_LINESIZE 4 
#define PENCIL_PIXMEM_SIZE (PENCIL_LINESIZE * PENCIL_HEIGHT)
#define PENCIL_BLACK 1
#define PENCIL_WHITE 0

#if defined (_BIG_END)
  #define PENCIL_CM (GFX_CM_1BIT | gfx_set_cm_byte_order(0))
#else
  #define PENCIL_CM (GFX_CM_1BIT | gfx_set_cm_byte_order(1))
#endif

GFX_PIXEL demo_pencil_1bit_bitmap[PENCIL_HEIGHT] = {
  0x07000000,
  0x03100000,
  0x03100000,
  0x05200000,
  0x05700000,
  0x08400000,
  0x08800000,
  0x10800000,
  0x11000000,
  0x23000000,
  0x22000000,
  0x44000000,
  0x78000000,
  0x70000000,
  0x60000000,
  0x40000000
};

GFX_PIXEL demo_pencil_1bit_mask[PENCIL_HEIGHT] = {
  0x01f80000,
  0x03f80000,
  0x07f80000,
  0x07f00000,
  0x0fe00000,
  0x0fe00000,
  0x1fc00000,
  0x1f800000,
  0x3f800000,
  0x3f000000,
  0x7f000000,
  0xfe000000,
  0xfc000000,
  0xf8000000,
  0xf0000000,
  0xe0000000
};

GFX_RGB demo_pencil_colors[] = {
	0x101010,					/* Black - cursor pixels */
	0xebebeb					/* White - shadow pixels */
};

GFX_PALETTE demo_pencil_palette = {
	254,						/* Starting entry */
	2,							/* Number of colors */
	GFX_COLOR_RGB,				/* Color type */
	demo_pencil_colors			/* RGB colors */
};

GFX_DMAP demo_pencil_bitmap = {
	PENCIL_CM,   				/* Coding method */
	PENCIL_WIDTH,               /* width in pixels */
	PENCIL_HEIGHT,		     	/* height in pixels */
	PENCIL_LINESIZE,		    /* Line size in bytes */
	demo_pencil_1bit_bitmap,    /* Pixel memory */
	0,							/* Pixel memory shade */
	PENCIL_PIXMEM_SIZE,         /* Pixel memory size (line size * num lines) */
	&demo_pencil_palette		/* Palette */
};

GFX_DMAP demo_pencil_mask = {
	PENCIL_CM,   				/* Coding method */
	PENCIL_WIDTH,               /* width in pixels */
	PENCIL_HEIGHT,    			/* height in pixels */
	PENCIL_LINESIZE,     		/* Line size in bytes */
	demo_pencil_1bit_mask,      /* Pixel memory */
	0,							/* Pixel memory shade */
	PENCIL_PIXMEM_SIZE,         /* Pixel memory size (line size * num lines) */
	NULL						/* Palette (not used) */
};

WIN_CURSOR demo_pencil_cursor = {
	0, 15,						/* Hit point */
	&demo_pencil_bitmap,		/* Bitmap */
	&demo_pencil_mask,			/* Transparency mask */
	0							/* Transparent pixel value (not used) */
};

/*******************************************************************************
* GLOBALS
*******************************************************************************/

GFX_DEV_ID gfxdev;					/* Graphics device ID */
DRW_CONTEXT_ID drwctx;				/* Drawing context */

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

void process_winmsg(const WIN_MSG *msg, void *user_data);
void button_down(const MSG_WIN_BUTTON *winmsg, void *user_data);
void expose_ink_win(const MSG_WIN_EXPOSE *winmsg, void *user_data);
void signal_handler(int sig);

/*******************************************************************************
* NAME:		main()
*
* USAGE:	Inking demo "main" function.
*/
void main(int argc, char *argv[])
{
	MSG_MBOX_ID mbox;				/* Mailbox ID */
	WIN_DEV_ID windev;				/* Windowing device ID */
	WIN_ID root;					/* Root window */
	WIN_ID win;						/* Drawing window */
	WIN_STATUS rootstat;			/* Window status for root window */
	GFX_POS win_x = 0, win_y = 0;	/* Window position */
	GFX_DIMEN win_w = 0, win_h = 0;	/* Window size */
	char *mbox_name = NULL;			/* Mailbox name */
	WIN_CMAP_ID cmap;				/* Default colormap */
	GFX_COLOR_VALUE color;			/* Color value */
	WIN_MSG winmsg;					/* Window message */
	char win_dev[CDB_MAX_DNAME];    /* Name of window device */
	u_int32 cursor_id;              /* cursor id--different for each process */
	int i;
	error_code err;
	WIN_CURSOR *pencil=NULL;
	WIN_DEV_STATUS windev_status;

	/* Check command line parameters */
	if (argc == 6) {
		win_x = atoi(argv[1]);
		win_y = atoi(argv[2]);
		win_w = atoi(argv[3]);
		win_h = atoi(argv[4]);
		mbox_name = argv[5];
	}
	if (argc != 6) {
		fprintf(stderr, "Syntax: %s <x> <y> <w> <h> <mbox>\n", argv[0]);
		_os_exit(1);
	}

	/* setup signal handler */
	signal(SIGQUIT,signal_handler);
	signal(SIGINT,signal_handler);

	/* Initialize MAUI APIs used by this program */
	msg_init();
	win_init();
	drw_init();
	cdb_init();

	/* Create the application mailbox */
	err = msg_create_mbox(&mbox, mbox_name, 50, sizeof(MAUI_MSG), MEM_ANY);
	if (err != SUCCESS) {
	  msg_term();
	  drw_term();
	  cdb_term();
	  win_term();
	  _os_exit(err);
	}

	/* Read CDB for window device name */
	if (cdb_get_ddr (CDB_TYPE_WIN,1,win_dev,NULL) != SUCCESS)
	  strcpy (win_dev,"/win");

	/* Open the windowing device */
	err = win_open_dev(&windev, &gfxdev, &root, win_dev, mbox);
	if (err != SUCCESS) {
	  msg_close_mbox(mbox);
	  msg_term();
	  drw_term();
	  cdb_term();
	  win_term();
	  _os_exit(err);
	}

	/* Allocate colors from the default colormap */
	win_get_win_status(&rootstat, root);
	win_get_dev_status(&windev_status, windev);
	cmap = rootstat.cmap;
	color.rgb = 0x101010; win_alloc_cmap_color(&pix[COLOR_BLACK], cmap, color);
	color.rgb = 0xbdbdbd; win_alloc_cmap_color(&pix[COLOR_MGREY], cmap, color);

	/* Create the inking window and activate it */
	win_create_win(&win, root, win_x, win_y, win_w, win_h,
                   (WIN_MASK_EXPOSE|WIN_MASK_BUTTON|WIN_MASK_PTR), WIN_BACK);
	win_set_callback(win, process_winmsg, NULL);
	win_set_state(win, TRUE);

	/* Create the default cursor and assign it to the root window */
	cursor_id = _procid + DEMO_PENCIL_CURSOR;
	err = expand_cursor(&demo_pencil_cursor, &pencil, windev_status.coding_method);
	if (err == SUCCESS) {
	  win_create_cursor(windev, cursor_id, pencil);
	  win_set_cursor(win, cursor_id);
	}

	/* Setup the inking parameters for the window */
	win_set_ink_method(win, WIN_INK_REPLACE);
	win_set_ink_pix(win, pix[COLOR_BLACK]);

	/* Create the drawing context and associate it with the window */
	drw_create_context(&drwctx, gfxdev);
	drw_set_context_pix(drwctx, pix[COLOR_MGREY]);
	drw_set_context_fm(drwctx, DRW_FM_SOLID);
	win_set_drw_context(win, drwctx);

	maui_set_error_action(MAUI_ERR_DEFAULT, MAUI_ERR_DEFAULT, MAUI_ERR_DEFAULT);

	/* Process messages forever */
	while (!done) {
		err= msg_read(mbox, &winmsg, MSG_TYPE_ANY, MSG_BLOCK);
		if(err == SUCCESS)
			msg_dispatch(&winmsg);
	}

	/* Shut down */
	win_set_drw_context(win, NULL);
	drw_destroy_context(drwctx);
	win_destroy_win(win);
	for (i = 0; i < NUM_COLORS; i++) {
		win_free_cmap_cells(cmap, pix[i], 1);
	}
	win_destroy_cursor(windev,cursor_id);
	if (pencil != NULL)
	  mem_free(pencil);
	win_destroy_dev(windev);
	msg_close_mbox(mbox);
	win_term();
	drw_term();
	msg_term();
	cdb_term();

	_os_exit(SUCCESS);
}

/*******************************************************************************
* NAME:		process_winmsg()
*
* USAGE:	Process window messages.
*/
void process_winmsg(const WIN_MSG *winmsg, void *user_data)
{
	/* Switch on the message type */
	switch (winmsg->any_win.wtype) {
		case WIN_MSG_BUTTON_DOWN:
			button_down(&winmsg->button, user_data);
			break;
		case WIN_MSG_BUTTON_UP:
			break;
		case WIN_MSG_PTR:
			break;
		case WIN_MSG_EXPOSE:
			expose_ink_win(&winmsg->expose, user_data);
			break;
		default:
			printf("Received unknown message %d\n", winmsg->any_win.wtype);
			break;
	}
}

/*******************************************************************************
* NAME:		button_down()
*
* USAGE:	Process button down messages.
*/
void button_down(const MSG_WIN_BUTTON *winmsg, void *user_data)
{
	/* Clear the ink if the 3rd button is pressed */
	if (winmsg->button == 2) {
		win_erase_ink(winmsg->win);
	}
}

/*******************************************************************************
* NAME:		expose_ink_win()
*
* USAGE:	Expose the inking window. This clears the window to its background
*			color.
*/
void expose_ink_win(const MSG_WIN_EXPOSE *winmsg, void *user_data)
{
	WIN_ID win = winmsg->win;
	GFX_POS x = winmsg->exposed.x;
	GFX_POS y = winmsg->exposed.y;
	GFX_DIMEN w = winmsg->exposed.width;
	GFX_DIMEN h = winmsg->exposed.height;

	/* Expose the specified area of the window with the background color */
	win_lock_region(win, x, y, w, h);
	drw_rectangle(drwctx, x, y, w, h);
	win_unlock_region(win);
}


void signal_handler(int sig)
{
  if ((sig == SIGINT) ||
	  (sig == SIGQUIT))
	done = TRUE;
}


/*
** this routine takes a 1 bit cursor definition, and expands it to the given
** coding method.  A new cursor structure is allocated for you.
*/
error_code expand_cursor(const WIN_CURSOR *src, WIN_CURSOR **d, GFX_CM coding_method) 
{
  u_int32 alloc_size;
  size_t pixmem_size;
  u_int8 *ptr;
  error_code err;
  GFX_PIXEL exptbl[2];
  BLT_CONTEXT_ID bltctx;
  WIN_CURSOR *dst;
  GFX_PIXEL maskval = 0xff;
  GFX_PIXEL ctmp;

  if (gfx_get_cm_name(coding_method) <= GFX_CM_2BIT)
	return EOS_MAUI_INCOMPATCM;

  /* set coding method to 4 byte alignment (or 0) */
  coding_method &= ~gfx_get_cm_byte_order(0xffffffff);

  gfx_calc_pixmem_size(&pixmem_size, coding_method, src->bitmap->width,src->bitmap->height);

  /* allocate memory in one big chunk */
  alloc_size = sizeof(WIN_CURSOR) + sizeof(GFX_DMAP)*2 + pixmem_size *2;
  err = mem_malloc((void*)&ptr, MEM_DEF_SHADE, alloc_size);
  if (err)
	return err;

  /* now, assign fields in WIN_CURSOR from allocated memory */
  dst = (WIN_CURSOR*)ptr;
  ptr += sizeof(WIN_CURSOR);
  dst->hit_point = src->hit_point;
  dst->bitmap = (GFX_DMAP*)ptr;
  ptr += sizeof(GFX_DMAP);
  dst->mask = (GFX_DMAP*)ptr;
  ptr += sizeof(GFX_DMAP);
  
  /* now assign values for dst->bitmap */
  gfx_set_dmap_size(dst->bitmap,coding_method,src->bitmap->width,src->bitmap->height);
  gfx_set_dmap_pixmem(dst->bitmap,ptr,0,pixmem_size);
  ptr += pixmem_size;
  dst->bitmap->palette = src->bitmap->palette;
  if (gfx_get_cm_name(coding_method) == GFX_CM_4BIT) {
	dst->bitmap->palette->start_entry = 14;
	exptbl[0] = 15;
	exptbl[1] = 14;
	maskval = 0xf;
  } else if (gfx_get_cm_name(coding_method) == GFX_CM_8BIT) {
	dst->bitmap->palette->start_entry = 254;
	exptbl[0] = 255;
	exptbl[1] = 254;
	maskval = 0xff;
  } else if (gfx_get_cm_name(coding_method) == GFX_CM_RGB565) {
	ctmp = dst->bitmap->palette->colors.rgb[1];
	exptbl[0] = _TO_RGB565((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
	ctmp = dst->bitmap->palette->colors.rgb[0];
	exptbl[1] = _TO_RGB565((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
	maskval = 0xffff;
  } else if (gfx_get_cm_name(coding_method) == GFX_CM_RGB655) {
	ctmp = dst->bitmap->palette->colors.rgb[1];
	exptbl[0] = _TO_RGB655((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
	ctmp = dst->bitmap->palette->colors.rgb[0];
	exptbl[1] = _TO_RGB655((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
	maskval = 0xffff;
  } else if (gfx_get_cm_name(coding_method) == GFX_CM_RGB556) {
	ctmp = dst->bitmap->palette->colors.rgb[1];
	exptbl[0] = _TO_RGB556((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
	ctmp = dst->bitmap->palette->colors.rgb[0];
	exptbl[1] = _TO_RGB556((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
	maskval = 0xffff;
  } else if (gfx_get_cm_name(coding_method) == GFX_CM_RGB555) {
	ctmp = dst->bitmap->palette->colors.rgb[1];
	exptbl[0] = _TO_RGB555((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
	ctmp = dst->bitmap->palette->colors.rgb[0];
	exptbl[1] = _TO_RGB555((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
	maskval = 0xffff;
  } else if (gfx_get_cm_name(coding_method) == GFX_CM_RGB888) {
	ctmp = dst->bitmap->palette->colors.rgb[1];
	exptbl[0] = ctmp;
	ctmp = dst->bitmap->palette->colors.rgb[0];
	exptbl[1] = ctmp;
	maskval = 0xffffffff;
  }

  /* now assign values for dst->mask */
  gfx_set_dmap_size(dst->mask,coding_method,src->bitmap->width,src->bitmap->height);
  gfx_set_dmap_pixmem(dst->mask,ptr,0,pixmem_size);
  dst->mask->palette = NULL;

  /* now, allocate blt context for expansion */
  err = blt_create_context(&bltctx, gfxdev);
  if (err != SUCCESS) {
	mem_free(ptr);
	return err;
  }

  blt_set_context_cpymix(bltctx, BLT_MIX_REPLACE);

  /*
  ** expand cursor
  */
  blt_set_context_exptbl(bltctx, 2, exptbl);
  blt_set_context_src(bltctx, src->bitmap);
  blt_set_context_dst(bltctx, dst->bitmap);
  blt_expd_block(bltctx, 0, 0, 0, 0, src->bitmap->width, src->bitmap->height);

  /*
  ** expand mask
  */
  exptbl[0] = 0;
  exptbl[1] = maskval;
  blt_set_context_exptbl(bltctx, 2, exptbl);
  blt_set_context_src(bltctx, src->mask);
  blt_set_context_dst(bltctx, dst->mask);
  blt_expd_block(bltctx, 0, 0, 0, 0, src->mask->width, src->mask->height);

  blt_destroy_context(bltctx);

  *d = dst;

  return SUCCESS;
}
