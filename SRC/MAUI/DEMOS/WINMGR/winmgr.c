/******************************************************************************
*
* DESCRIPTION:	Window manager demo
*
* CREATED ON:	07/30/96
*
* COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
*				Reserved. Use and/or distribution subject to terms and
*				conditions of applicable Microware license agreement(s).
*
*/
#define _OPT_PROTOS
#include <MAUI/mauidemo.h>
#include <MAUI/maui.h>
#include <cglob.h>
#include <process.h>
#include <alarm.h>
#include <signal.h>

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
* COLORS
*******************************************************************************/

#define COLOR_BLACK	0
#define COLOR_WHITE	1
#define COLOR_MGREY	2
#define COLOR_DGREY	3
#define COLOR_DBLUE 4

#define QUIT_SIGNAL 500

#define RGB(r,g,b) (((r)<<16)|((g)<<8)|((b)))

#define NUM_COLORS	5				/* Total number of colors */
GFX_PIXEL pix[NUM_COLORS];			/* Pixel values for colors */
volatile u_int32 done=0;            /* done flag */
volatile u_int32 quit_sig=0;        /* quit alarm flag */



/*******************************************************************************
* STIPPLE DRAWMAP
*******************************************************************************/

GFX_PIXEL stipple_pixmem[] = {
	0x11111111, 0x44444444, 0x22222222, 0x88888888,
	0x11111111, 0x44444444, 0x22222222, 0x88888888,
	0x11111111, 0x44444444, 0x22222222, 0x88888888,
	0x11111111, 0x44444444, 0x22222222, 0x88888888,
	0x11111111, 0x44444444, 0x22222222, 0x88888888,
	0x11111111, 0x44444444, 0x22222222, 0x88888888,
	0x11111111, 0x44444444, 0x22222222, 0x88888888,
	0x11111111, 0x44444444, 0x22222222, 0x88888888,
};
GFX_DMAP stipple = {GFX_CM_1BIT, 32, 32, 4, stipple_pixmem, 0, 128, NULL};

/*******************************************************************************
* CURSOR DEFINITIONS
*******************************************************************************/
#define ARROW_WIDTH 12   
#define ARROW_HEIGHT 16  
#define ARROW_LINESIZE 4 
#define ARROW_PIXMEM_SIZE (ARROW_LINESIZE * ARROW_HEIGHT)
#define ARROW_BLACK 1
#define ARROW_WHITE 0

#if defined (_BIG_END)
  #define ARROW_CM (GFX_CM_1BIT | gfx_set_cm_byte_order(0))
#else
  #define ARROW_CM (GFX_CM_1BIT | gfx_set_cm_byte_order(1))
#endif



GFX_PIXEL demo_arrow_1bit_bitmap[ARROW_HEIGHT] = {
  0x00000000,
  0x40000000,
  0x60000000,
  0x70000000,
  0x78000000,
  0x7c000000,
  0x7e000000,
  0x7f000000,
  0x7f800000,
  0x7c000000,
  0x6c000000,
  0x46000000,
  0x06000000,
  0x03000000,
  0x03000000,
  0x00000000
};


GFX_PIXEL demo_arrow_1bit_mask[ARROW_HEIGHT] = {
  0xc0000000,
  0xe0000000,
  0xf0000000,
  0xf8000000,
  0xfc000000,
  0xfe000000,
  0xff000000,
  0xff800000,
  0xffc00000,
  0xffc00000,
  0xfe000000,
  0xef000000,
  0xcf000000,
  0x07800000,
  0x07800000,
  0x03000000
};

GFX_RGB demo_arrow_colors[] = {
	0x101010,					/* Black - cursor pixels */
	0xebebeb,					/* White - shadow pixels */
};

GFX_PALETTE demo_arrow_palette = {
    0,						    /* Starting entry */
	2,							/* Number of colors */
	GFX_COLOR_RGB,				/* Color type */
	demo_arrow_colors			/* RGB colors */
};

GFX_DMAP demo_arrow_bitmap = {
	ARROW_CM,   				/* Coding method */
	ARROW_WIDTH,                /* width in pixels */
	ARROW_HEIGHT,		     	/* height in pixels */
	ARROW_LINESIZE,		        /* Line size in bytes */
	demo_arrow_1bit_bitmap,     /* Pixel memory */
	0,							/* Pixel memory shade */
	ARROW_PIXMEM_SIZE,          /* Pixel memory size (line size * num lines) */
	&demo_arrow_palette			/* Palette */
};

GFX_DMAP demo_arrow_mask = {
	ARROW_CM,   				/* Coding method */
	ARROW_WIDTH,                /* width in pixels */
	ARROW_HEIGHT,    			/* height in pixels */
	ARROW_LINESIZE,     		/* Line size in bytes */
	demo_arrow_1bit_mask,       /* Pixel memory */
	0,							/* Pixel memory shade */
	ARROW_PIXMEM_SIZE,          /* Pixel memory size (line size * num lines) */
	NULL						/* Palette (not used) */
};

WIN_CURSOR demo_arrow_cursor = {
	0, 0,						/* Hit point */
	&demo_arrow_bitmap,			/* Bitmap */
	&demo_arrow_mask,			/* Transparency mask */
	0							/* Transparent pixel value (not used) */
};

error_code expand_cursor(const WIN_CURSOR *src, WIN_CURSOR **dst, GFX_CM coding_method);

/*******************************************************************************
* GLOBALS
*******************************************************************************/

GFX_DEV_ID gfxdev = NULL;			/* Graphics device ID */
WIN_ID root = NULL;					/* Root window */
DRW_CONTEXT_ID root_drwctx = NULL;	/* Drawing context ID for the root */
GFX_PIXEL root_exptbl[2];			/* Expansion table */
BOOLEAN verbose = FALSE;
u_int32 res_index = 0;
u_int32 cm_index = 0;
char *gfx_name_override = 0;
BOOLEAN draw_stipple = FALSE;
int     bkgd_color = 4;             /* default to blue */

GFX_POINT drag_anchor;
GFX_POINT drag_win_pos = {0,0};
WIN_ID	  drag_win = NULL;
u_int32	  drag_last_tick = 0;
WIN_STATUS dragstat;				/* drag Window status */
WIN_STATUS rootstat;				/* rppt Window status */
/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

void process_rootmsg(const WIN_MSG *msg, void *user_data);
void create_child(const MSG_WIN_CREATE *winmsg, void *user_data);
void draw_border(WIN_ID win, GFX_POS x, GFX_POS y, GFX_DIMEN width, GFX_DIMEN
	height);
void change_child_state(const MSG_WIN_STATE *winmsg, void *user_data);
void draw_root(GFX_POS x, GFX_POS y, GFX_DIMEN width, GFX_DIMEN height);
void process_winmsg(const WIN_MSG *msg, void *user_data);
void drag_window(GFX_POS x, GFX_POS y);
void drag_draw(GFX_POS x, GFX_POS y, GFX_POS w, GFX_POS h);
static void parse_options(int argc, char **argv);
static void print_usage(char* modname);
void signal_handler(int sig);
int kill_children(void);

void showMsg(WIN_MSG * msg)
{
  MSG_COMMON *		comMsg	= & msg->any;
  MSG_WIN_COMMON *	winMsg	= & msg->any_win;
  WIN_MSG_TYPE		wtype   = winMsg->wtype;

  /* Show information about common part. */

  /* Show message-specific part. */
  printf("MSG: win=0x%x  time=%-8d  wtype=%3d ",
	 winMsg->win, comMsg->time_queued, wtype);
  switch (wtype)
    {
    case WIN_MSG_EXPOSE:
      {
	MSG_WIN_EXPOSE * exposeMsg = & msg->expose;
	GFX_RECT *	 exposed   = & exposeMsg->exposed;
	printf("(EXP): x,y=%4d,%-4d w,h=%4d,%-4d\n",
	       exposed->x, exposed->y, exposed->width, exposed->height);
      }
      break;

    case WIN_MSG_PTR:
      printf("(PTR)\n");
      break;

    case WIN_MSG_BUTTON_DOWN:
      printf("(BDN)\n");
      break;

    case WIN_MSG_BUTTON_UP:
      printf("(BUP)\n");
      break;

    case WIN_MSG_CREATE:
      printf("(CRT)\n");
      break;

    default:
      printf("\n");
      break;
    }
}

/*******************************************************************************
* NAME:		main()
*
* USAGE:	Window manager "main" function.
*/
void main(int argc, char *argv[])
{
	error_code ec;					/* Error code */
	MSG_MBOX_ID mbox;				/* Mailbox ID */
	WIN_DEV_ID windev;				/* Windowing device ID */
	WIN_CMAP_ID cmap;				/* Default colormap */
	GFX_COLOR_VALUE color;			/* Color value */
	INP_DEV_ID inpdev[10]={0,0,0,0,0,0,0,0,0,0};    /* Input device ID */
	WIN_MSG winmsg;					/* Window message */
	char gfxname[CDB_MAX_DNAME];	/* Graphics device name */
	char winname[CDB_MAX_DNAME];	/* Windowing device name */
	char inpname[CDB_MAX_DNAME];	/* Input device name */
	int i=1;
	int num_children;               /* for shutdown */
	alarm_id alarmid;               /* shutdown alarm */
	const GFX_DEV_CAP *dev_cap;
	WIN_CURSOR *cur;

	/* setup signal handler */
	signal(SIGQUIT,signal_handler);
	signal(SIGINT,signal_handler);
	signal(QUIT_SIGNAL,signal_handler);

	parse_options(argc, argv);

	/* Initialize MAUI APIs used by this program */
	/* maui_set_error_action(MAUI_ERR_DEFAULT, MAUI_ERR_DEFAULT,
			      MAUI_ERR_NON_FATAL); */

	cdb_init();
	msg_init();
	win_init();
	drw_init();

	/* Create the application mailbox */
	msg_create_mbox(&mbox, "winmgr_mb", 50, sizeof(MAUI_MSG), MEM_ANY);

	/* Create the windowing device and setup callback info for root window */
    if (!gfx_name_override) {
      gfx_name_override = gfxname;
      cdb_get_ddr(CDB_TYPE_GRAPHIC, 1, gfxname, NULL);
    }
	cdb_get_ddr(CDB_TYPE_WIN, 1, winname, NULL);

	ec = win_create_dev(&windev, &gfxdev, &root, winname, gfxname, res_index,
         cm_index, mbox);
	if (ec != SUCCESS) {
	    msg_close_mbox(mbox);
	    win_term();
		drw_term();
		msg_term();
		cdb_init();
		_os_exit(ec);
	}

	if (verbose)
	  printf("DEV Create: root_id=0x%x\n", root);
	win_set_callback(root, process_rootmsg, NULL);
 	win_set_msg_mask(root, (WIN_MASK_CHILD_CONFIG|WIN_MASK_CHILD_CREATE
                            |WIN_MASK_EXPOSE));

	gfx_get_dev_cap(&dev_cap,gfxdev);

	/* Create the default colormap, add colors, and assign it to the root */
	/* window. */
	win_create_cmap(&cmap, windev, GFX_COLOR_RGB);
	color.rgb = RGB(16,16,16); win_alloc_cmap_color(&pix[COLOR_BLACK], cmap, color);
	color.rgb = RGB(235,235,235); win_alloc_cmap_color(&pix[COLOR_WHITE], cmap, color);
	color.rgb = RGB(189,189,189); win_alloc_cmap_color(&pix[COLOR_MGREY], cmap, color);
	color.rgb = RGB(128,128,128); win_alloc_cmap_color(&pix[COLOR_DGREY], cmap, color);
	color.rgb = RGB(25,25,112); win_alloc_cmap_color(&pix[COLOR_DBLUE], cmap, color);
	win_set_cmap(root, cmap);

	/* Create and configure the drawing context for the root */
	drw_create_context(&root_drwctx, gfxdev);
	root_exptbl[0] = pix[COLOR_BLACK];
	root_exptbl[1] = pix[COLOR_WHITE];
	drw_set_context_exptbl(root_drwctx, 2, root_exptbl);
	drw_set_context_src(root_drwctx, &stipple);
	win_set_drw_context(root, root_drwctx);

	/* Clear the root window background */
	win_get_win_status(&rootstat, root);
	draw_root(0, 0, rootstat.width, rootstat.height);

	/* Create the default cursor and assign it to the root window */
	expand_cursor(&demo_arrow_cursor, &cur, dev_cap->cm_info[cm_index].coding_method);

	win_create_cursor(windev, DEMO_ARROW_CURSOR, cur);
	win_set_cursor(root, DEMO_ARROW_CURSOR);

#if 0
	maui_set_error_action(MAUI_ERR_NONE, MAUI_ERR_DEFAULT, 
						  /*MAUI_ERR_NON_FATAL*/MAUI_ERR_DEFAULT);
#endif

	/* Open all input devices */
	i = 1;
	while (cdb_get_ddr(CDB_TYPE_REMOTE, i, inpname, NULL) == SUCCESS) {
	  if (verbose)
	    printf("opening remote device \'%s\'\n",inpname);
	  win_open_inpdev(&inpdev[i-1], windev, inpname);
	  i++;
	}
/*	maui_set_error_action(MAUI_ERR_DEFAULT, MAUI_ERR_DEFAULT,
		MAUI_ERR_DEFAULT); */

	/* Process messages */
	while (!done) {
		ec = msg_read(mbox, &winmsg, MSG_TYPE_ANY, MSG_BLOCK);
		if (verbose)
		  showMsg(&winmsg);
		if (ec == SUCCESS)
		  msg_dispatch(&winmsg);
	}

	/* kill children */
	num_children = kill_children();

	/* set alarm so we won't wait for children forever... */
	if (num_children > 0)
	  _os_alarm_set(&alarmid, SIGQUIT, 0x80000000 | (num_children * 256));

	/* process only WIN_DESTROY messages */
	while (num_children && !quit_sig) {
		ec = msg_read(mbox, &winmsg, MSG_TYPE_ANY, MSG_BLOCK);
		if (verbose)
		  showMsg(&winmsg);
		if ((ec == SUCCESS) && (winmsg.any_win.wtype == WIN_MSG_DESTROY)
			|| (winmsg.any_win.wtype == WIN_MSG_EXPOSE)) {
		  msg_dispatch(&winmsg);
		  num_children--;
		}
	}

	if (!quit_sig)
	  _os_alarm_delete(alarmid);

	/* sleep a half second to let all apps. close */
	tsleep(0x80000000 | 128);
	
	/* Shut down */
	mem_free(cur);
	win_set_drw_context(root, NULL);
	drw_destroy_context(root_drwctx);
	msg_close_mbox(mbox);
	win_destroy_cursor(windev, DEMO_ARROW_CURSOR);
	win_destroy_cmap(cmap);
	win_destroy_dev(windev);
	win_term();
	drw_term();
	msg_term();
	cdb_term();

	_os_exit(SUCCESS);
}

/*******************************************************************************
* NAME:		process_rootmsg()
*
* USAGE:	Process messages for the root window.
*/
void process_rootmsg(const WIN_MSG *winmsg, void *user_data)
{
	/* Switch on the message type */
	switch (winmsg->any_win.wtype) {
		case WIN_MSG_CREATE:
			create_child(&winmsg->create, user_data);
			break;
		case WIN_MSG_MOVE_REQ:
			printf("Received move request message\n");
			break;
		case WIN_MSG_RESIZE_REQ:
			printf("Received resize request message\n");
			break;
		case WIN_MSG_RESTACK_REQ:
			printf("Received restack request message\n");
			break;
		case WIN_MSG_STATE_REQ:
			change_child_state(&winmsg->state, user_data);
			break;
		case WIN_MSG_EXPOSE:
			draw_root(winmsg->expose.exposed.x, winmsg->expose.exposed.y,
				winmsg->expose.exposed.width, winmsg->expose.exposed.height);
			break;
		default:
			printf("Received unknown message %d\n", winmsg->any_win.wtype);
			break;
	}
}


/* margin values for window manager frame */
#define FRAME_BORDER 5     /* left, right, and bottom border pixel size*/
#define FRAME_TITLE_BAR 25 /* top title bar height */

/*******************************************************************************
* NAME:		create_child()
*
* USAGE:	Someone is opening a top-level window (one on the root). Therefore,
*			we need to create the window-manager window and re-parent the apps
*			window to be a child of it.
*/
void create_child(const MSG_WIN_CREATE *winmsg, void *user_data)
{
	WIN_ID win;						/* Window manager window */

	/* Compute position and size of window manager window */
	GFX_POS   x = winmsg->position.x - FRAME_BORDER;
	GFX_POS   y = winmsg->position.y - FRAME_TITLE_BAR;
	GFX_DIMEN w = winmsg->width + (FRAME_BORDER << 1);
	GFX_DIMEN h = winmsg->height + (FRAME_TITLE_BAR + FRAME_BORDER);

	/* Create window-manager window */
	win_create_win(&win, root, x, y, w, h, 
                   (WIN_MASK_CHILD_CONFIG | WIN_MASK_BUTTON | WIN_MASK_PTR |
                    WIN_MASK_EXPOSE | WIN_MASK_CHILD_DESTROY), WIN_BACK);
	if (verbose)
	  printf("WIN Create: id=0x%x\n", win);
	win_set_callback(win, process_winmsg, NULL);
	win_set_state(win, TRUE);

	/* Re-parent the applications window */
	win_reparent_win(winmsg->create_win, win, FRAME_BORDER, FRAME_TITLE_BAR,
					 WIN_FRONT);

	/* Draw the border */
	draw_border(win, 0, 0, w, h);
}

/*******************************************************************************
* NAME:		draw_border()
*
* USAGE:	Draw the border for a top-level window.
*/
void draw_border(WIN_ID win, GFX_POS x, GFX_POS y, GFX_DIMEN width, GFX_DIMEN
	height)
{
	DRW_CONTEXT_ID drwctx;			/* Drawing context ID */
	WIN_STATUS win_status;
	error_code err;

	err = win_get_win_status(&win_status, win);
	if (err == SUCCESS) {

	  /* Create the drawing context and assign it to the new window */
	  drw_create_context(&drwctx, gfxdev);
	  
	  /* Setup for drawing */
	  win_set_drw_context(win, drwctx);
	  win_set_drw_area(win, 0, 0, win_status.width, win_status.height);
	  win_lock_region(win, x, y, width, height);

	  /* draw window... */
	  drw_set_context_pix(drwctx, pix[COLOR_BLACK]);
	  drw_set_context_fm(drwctx, DRW_FM_OUTLINE);
	  drw_set_context_ls(drwctx, DRW_LS_SOLID);
	  drw_set_context_mix(drwctx,BLT_MIX_REPLACE);
	  x = y = 0;
	  width = win_status.width;
	  height = win_status.height;

	  /* black outlines */
	  drw_rectangle(drwctx, x, y, width, height);
	  drw_rectangle(drwctx, x+4, y+4, width-8, height-8);
	  drw_line(drwctx,x+5,y+24,width-5,y+24);

	  /* white shadow */
	  drw_set_context_pix(drwctx, pix[COLOR_WHITE]);
	  drw_line(drwctx, x+1, height-2, x+1, y+1);
	  drw_line(drwctx, x+1, y+1, width-2, y+1);
	  drw_line(drwctx, x+3, height-4, width -3,height-4);
	  drw_line(drwctx, width-4, height-4, width-4, y+2);

	  /* dark grey shadow */
	  drw_set_context_pix(drwctx, pix[COLOR_DGREY]);
	  drw_line(drwctx, x+3, y+3, width-4, y+3);
	  drw_line(drwctx, x+3, y+3, x+3, height-4);
	  drw_line(drwctx, width-2,y+1,width-2,height-2);
	  drw_line(drwctx, x+1,height-2,width-1,height-2);

	  /* medium grey shadow and titlebar background */
	  drw_set_context_pix(drwctx, pix[COLOR_MGREY]);	
	  drw_rectangle(drwctx, x+2, y+2, width -4, height -4);
	  drw_set_context_fm(drwctx, DRW_FM_SOLID);
	  drw_rectangle(drwctx, x+5, y+5, width -10, 19);


	  /* finish drawing */
	  win_unlock_region(win);
	  win_set_drw_context(win, NULL);
	  drw_destroy_context(drwctx);
	}
}

/*******************************************************************************
* NAME:		change_child_state()
*
* USAGE:	Another process is attempting to change the state of its top-level
*			window.
*/
void change_child_state(const MSG_WIN_STATE *winmsg, void *user_data)
{
	WIN_ID parent;					/* Parent of application window */
	WIN_STATUS winstat;				/* Window status */

	/* Get the ID of the parent window */
	win_get_win_status(&winstat, winmsg->state_win);
	parent = winstat.parent;

	/* We need to change the state of both the application and the window */
	/* manager windows, but the order we do it in depends on whether the */
	/* application window is being mapped or unmapped. */
	if (winmsg->new_state == TRUE) {

		/* Map both windows */
		win_set_state(winmsg->state_win, TRUE);
		if (parent != root) {
			win_set_state(parent, TRUE);
		}

	} else {

		/* Unmap both windows */
		if (parent != root) {
			win_set_state(parent, FALSE);
		}
		win_set_state(winmsg->state_win, FALSE);

	}
}

/*******************************************************************************
* NAME:		draw_root()
*
* USAGE:	Draw an area of the root window. This causes a stipple pattern
*			to be drawn in the specified area.
*/
void draw_root(GFX_POS x, GFX_POS y, GFX_DIMEN width, GFX_DIMEN height)
{
	int32 h, v;

	win_set_drw_area(root, x, y, width, height);
	win_lock_region(root, x, y, width, height);
	if(draw_stipple)
	{

	  u_int32 x_diff = x%32; /* 32 is the tile size */
	  u_int32 y_diff = y%32;

	  width +=  x_diff;
	  height += y_diff;
	  x -= x_diff;
      y -= y_diff;

	  /* Draw the stipple background */
	  for (h = 0; (h * 32) < width; h++) {
		for (v = 0; (v * 32) < height; v++) {
		  drw_expd_block(root_drwctx, x + h * 32, y+ v * 32, 0, 0, 32, 32);
		}
	  }
	}
	else
	{
	  drw_set_context_fm(root_drwctx, DRW_FM_SOLID);
	  drw_set_context_pix(root_drwctx, pix[bkgd_color]);
	  drw_rectangle(root_drwctx, x, y, width, height);
	}
	win_unlock_region(root);
}
 
/*******************************************************************************
* NAME:		process_winmsg()
*
* USAGE:	Process messages for a top-level (child of root) window.
*/
void process_winmsg(const WIN_MSG *winmsg, void *user_data)
{
	/* Switch on the message type */
	switch (winmsg->any_win.wtype)
	  {
	  case WIN_MSG_BUTTON_DOWN: /* begin drag */
	    {
	      drag_win       = winmsg->button.win;
	      win_get_win_status(&dragstat, drag_win);
	      drag_win_pos   = dragstat.position;
	      drag_anchor    = winmsg->button.root_pos;
	      drag_last_tick = winmsg->any.time_queued;

		  /* move window to top of stacking order, if not already there */
		  if (dragstat.sib_front_of != NULL)
			win_restack_win(drag_win, WIN_FRONT);
	      break;
	    }

	  case WIN_MSG_PTR:
			if (drag_win)
			{
				u_int32 tick    = winmsg->any.time_queued;
				u_int32 elapsed = tick - drag_last_tick;
				if (elapsed > 20)
				{
					drag_last_tick = tick;
					drag_window(winmsg->winptr.root_pos.x,
								winmsg->winptr.root_pos.y);
				}
			}
			break;

	  case WIN_MSG_BUTTON_UP:
	    if (drag_win)
	      {
		drag_window(winmsg->button.root_pos.x,
			    winmsg->button.root_pos.y);
		win_move_win(drag_win, drag_win_pos.x, drag_win_pos.y);
		drag_win = NULL;
	      }
	    break;

	  case WIN_MSG_EXPOSE:
	    draw_border(winmsg->expose.win, winmsg->expose.exposed.x,
			winmsg->expose.exposed.y, winmsg->expose.exposed.width,
			winmsg->expose.exposed.height);
	    break;

	  case WIN_MSG_STATE_REQ:
	    win_set_state(winmsg->state.state_win, winmsg->state.new_state);
	    break;

	  case WIN_MSG_DESTROY:
		if (winmsg->destroy.win != winmsg->destroy.destroy_win)
		  {
			/* child window has been destroyed, destroy frame */
			win_destroy_win(winmsg->destroy.win);
		  }
		break;

	  default:
	    printf("Received unknown message %d\n", winmsg->any_win.wtype);
	    break;
	  }
}

/*******************************************************************************
* NAME:		drag_window()
*
* USAGE:	Drag a top-level window to a new location
*/
void drag_window(GFX_POS x, GFX_POS y)
{
  GFX_POS deltaX, deltaY;
  
  /* Compute delta x,y, and reset anchor pos */
  deltaX = x - drag_anchor.x;
  deltaY = y - drag_anchor.y;

  drw_set_context_fm(root_drwctx, DRW_FM_OUTLINE);
  drw_set_context_pix(root_drwctx, pix[COLOR_WHITE]);
  drw_set_context_mix(root_drwctx, BLT_MIX_SXD);

  drag_draw(drag_win_pos.x, drag_win_pos.y, dragstat.width, dragstat.height);

  drag_anchor.x = x;
  drag_anchor.y = y;

  /* Move the window to its new location */
  drag_win_pos.x += deltaX;
  drag_win_pos.y += deltaY;
   
  drag_draw(drag_win_pos.x, drag_win_pos.y, dragstat.width, dragstat.height);
  drw_set_context_mix(root_drwctx, BLT_MIX_REPLACE);
}


/**********************************************************
***  drag_draw - draws the window outline while dragging
***********************************************************/

void drag_draw(GFX_POS x, GFX_POS y, GFX_POS w, GFX_POS h)
{
  win_set_drw_area(root,0,0,rootstat.width,rootstat.height);
  win_lock_region(root, x, y, w, h);
  
  drw_line(root_drwctx, x, y, x+w-2, y);
  drw_line(root_drwctx, x + w - 1, y, x + w - 1, y + h - 2);
  drw_line(root_drwctx, x+1, y+h-1, x+w-1, y+h-1);
  drw_line(root_drwctx, x, y+1, x, y+h-1);
  
  win_unlock_region(root);
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
		  
		case 'b':     /* Not to draw stipple */
		  draw_stipple=FALSE;
		  
		  if (*++ptr == '=') ptr++; /* optional = */
		  if (*ptr != '\0') {
			bkgd_color = atoi(ptr);
			if ((bkgd_color < 0) || (bkgd_color > 4))
			  bkgd_color = 4; /* on error, default to blue */
		  }
		  break;

        case 'c':           /* Coding method */
          if (*++ptr == '=') ptr++;   /* optional '=' */
          cm_index = atoi(ptr) ;
          while (*ptr >= '0' && *ptr <= '9') ptr++;
          ptr--;
          break;

        case 'g':           /* override name of gfx device */
          if (*++ptr == '=') ptr++;   /* optional '=' */
          gfx_name_override = ptr;
          break;

        case 'r':           /* resolution */
          if (*++ptr == '=') ptr++;   /* optional '=' */
          res_index = atoi(ptr) ;
          while (*ptr >= '0' && *ptr <= '9') ptr++;
          ptr--;
          break;

		case 's':     /* draw stipple */
		  draw_stipple=TRUE;
		  break;

        case 'v':           /* verbose */
		  verbose = TRUE;
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
  fprintf (stderr, "Description: Window managing routine. \n");
  fprintf (stderr, "<opts>: -h or -?     This help\n");
  fprintf (stderr, "        -b[=]<n>     blank - not to draw background pattern (default)\n");
  fprintf (stderr, "                     value for <n>:  0 = black, 1 = white, 2 = medium grey,\n");
  fprintf (stderr, "                                     3 = dark grey, 4 = dark blue\n");
  fprintf (stderr, "        -c[=]<idx>   coding method - coding method index for graphics device\n");
  fprintf (stderr, "        -g[=]<name>  graphic dev name - overrides CDB's name\n");
  fprintf (stderr, "        -r[=]<idx>   resolution - resolution index for graphics device\n");
  fprintf (stderr, "        -s           stipple - draw stipple background pattern\n");
  fprintf (stderr, "        -v           verbose - prints more info\n");
}


void signal_handler(int sig)
{
  if ((sig == SIGINT) ||
	  (sig == SIGQUIT))
	done = TRUE;

  if (sig == QUIT_SIGNAL)
	quit_sig = TRUE;
}


int kill_children(void)
{
  WIN_ID child;
  error_code err;
  WIN_STATUS win_status;
  int num_children=0;

  win_status.child_front = NULL;
  win_get_win_status(&win_status,root);
  child = win_status.child_front;

  while (child) {

	/* get the window status */
	win_get_win_status(&win_status,child);
	
	/* assign new child here, as we will be reusing tmp_status below */
	child = win_status.sib_back_of;
	
	/* careful!  this one may not be ours... */
	if ((win_status.owner_pid == _procid) && (win_status.child_front)) {
	  /* get window status for child, which is definitely not ours */
	  err = win_get_win_status(&win_status,win_status.child_front);
	  if (err == SUCCESS) {
		_os_send(win_status.owner_pid, SIGQUIT);
		num_children ++;
	  }
	}
  }

  return num_children;
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
	mem_free(dst);
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
