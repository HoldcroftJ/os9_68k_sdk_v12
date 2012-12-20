
/*******************************************************************************
 * FILENAME : main.c
 *
 *
 * COPYRIGHT:
 *
 *	This source code is the proprietary confidential property of Microware
 *	Systems Corporation, and is provided to licensee solely for documentation
 *	and educational purposes. Reproduction, publication, or distribution in
 *	form to any party other than the licensee is strictly prohibited.
 *
 * EDITION  DATE      WHO  DETAIL
 *       1  11/09/99  gjb,sbh  ported mp_ucb1200 to Hampshire touchscreen
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MAUI/maui.h>
#include <MAUI/maui_gfx.h>
#include <MAUI/maui_drw.h>
#include <MAUI/maui_win.h>
#include <MP_HAMP/_key.h>
#include <signal.h>
#include <MAUI/mauidemo.h>

/* defines */
#define TARGET_WIDTH 15
#define TARGET_HEIGHT 19
#define NUM_POINTS 50     /* number of sample points to use */
#define CALIBRATION_PTS 2 /* number of calibration points */
#define CURSOR_ID 2345
#define CURSOR_WIDTH 16
#define CURSOR_HEIGHT 16

/* enough memory for a 16x16 cursor at 32 bit res. */
#define CURSOR_SIZE (CURSOR_WIDTH*CURSOR_HEIGHT*4)

#define M_RGB(r,g,b) (((r)<<16)|((g)<<8)|(b))
#define M_TO_RGB565(r,g,b) ((((r)&0xf8)<<8)|(((g)&0xfc)<<3)|(((b)>>3)&0x1f))
#define M_TO_RGB655(r,g,b) ((((r)&0xfc)<<8)|(((g)&0xf8)<<2)|(((b)>>3)&0x1f))
#define M_TO_RGB556(r,g,b) ((((r)&0xf8)<<8)|(((g)&0xf8)<<3)|(((b)>>2)&0x2f))
#define M_SEC(sec) (0x80000000 | (sec * 256))
#define M_MSEC(sec) (0x80000000 | ((sec * 256) / 1000))

/* array indicies of colors we use... */
#define NUM_COLORS 3
#define BACKGROUND 0
#define FOREGROUND 1
#define WHITE      2


/* structure definitions */
typedef struct {
  WIN_CURSOR win_cursor;
  GFX_DMAP bitmap;
  GFX_DMAP mask;
  u_int8 bitmap_pixmem[CURSOR_SIZE];
  u_int8 mask_pixmem[CURSOR_SIZE];
  GFX_PALETTE palette;
  GFX_RGB palette_colors[2];
} CURSOR_STRUCT;

/* function prototypes */
void usage(char *prog);
void parse_args(int argc, char *argv[],char **filename,int *check,
				char **font_module);
error_code run_calibration(char *output_file,char *font_module);
void draw_screen(WIN_ID,DRW_CONTEXT_ID,const GFX_RECT *expose_rect,
				 GFX_PIXEL,GFX_PIXEL,GFX_PIXEL,const GFX_POINT*,
				 TXT_CONTEXT_ID txtctx, int text_height);
void point_analysis(const GFX_POINT *points,
					u_int32 num_points,GFX_POINT *cal_pts);
void write_cal_pts(char *module_name,GFX_POINT cal_pts[CALIBRATION_PTS],
				   const char *default_filename,GFX_DIMEN screen_w,
				   GFX_DIMEN screen_h);
INP_DEV_ID open_touch_device(MSG_MBOX_ID mbox);
void wait_eat_msgs(MSG_MBOX_ID mbox);
error_code event_loop(WIN_ID win, MSG_MBOX_ID mbox,
					  GFX_POINT cal_pts[CALIBRATION_PTS],
					  GFX_CM coding_method,GFX_DEV_ID gfxdev,
					  WIN_CMAP_ID cmap, DRW_CONTEXT_ID,
					  TXT_CONTEXT_ID,int text_height);
void my_sleep(u_int32 ticks);
error_code open_windev(WIN_DEV_ID *windev, GFX_DEV_ID *gfxdev, WIN_ID *root,
					   MSG_MBOX_ID mbox);

void main(int argc, char *argv[])
{
  char *default_filename=NULL;
  char *font_module="default.fnt";
  int check;
  int run_cal=TRUE;
  error_code err=SUCCESS;

  parse_args(argc,argv,&default_filename,&check,&font_module);

  /* determine if we need to run the calibration */
  if (check && default_filename) {
	FILE *f;
	
	/* if the file exists, then do not run the calibration */
	f = fopen(default_filename,"r");
	if (f) {
	  fclose(f);
	  run_cal = FALSE;
	  fprintf(stderr,"File '%s' exists, skipping calibration.\n",default_filename);
	}
  }

  if (run_cal)
	err = run_calibration(default_filename,font_module);

  if (err != SUCCESS)
	fprintf(stderr,"%s\n",strerror(err));

  exit(0);
}


/*
** writes calibration points to the calibration module, and possibly a file.
** Pre:  module_name -- valid OS-9 module name
**       output_file -- valid file, or NULL
*/
void write_cal_pts(char *module_name,GFX_POINT cal_pts[CALIBRATION_PTS],
				   const char *output_file,GFX_DIMEN screen_width,
				   GFX_DIMEN screen_height)
{
  mh_com *head=NULL;
  ModData *data=NULL;
  u_int16 attr_rev=0,type_lang=0;
  u_int32 perm= MP_OWNER_READ | MP_OWNER_WRITE | MP_GROUP_READ |
	  MP_GROUP_WRITE | MP_WORLD_READ | MP_WORLD_WRITE;
  BOOLEAN our_mod = FALSE;
  error_code ec;
  FILE *out_f;

  /* link to calibration data module, if there, otherwise create it */
  ec = _os_link(&module_name,&head,(void**)&data,
					 &type_lang,&attr_rev);
  if (ec == E_MNF)
	{
	  /* module not there */
	  type_lang = 0x0400; /* data module */
	  attr_rev = 0x8000;  /* revision 0 */
	  if ((ec = _os_datmod(module_name,sizeof(ModData),&attr_rev,
						   &type_lang,perm,(void**)&data,
						   &head)) == SUCCESS)
		{
		  our_mod = TRUE;
		  data->sync = HAMPSHIRE_SYNC;
		}
	}

  /* write data to calibration module */
  if (data && (data->sync == HAMPSHIRE_SYNC))
	{
	  memset(data->xpts,0,NUM_CAL_PTS * sizeof(data->xpts));
	  memset(data->ypts,0,NUM_CAL_PTS * sizeof(data->ypts));
	  data->x = cal_pts[0].x;
	  data->y = cal_pts[0].y;
	  if (cal_pts[1].x < cal_pts[0].x)
		data->width = -((int16)cal_pts[0].x - (int16)cal_pts[1].x);
	  else
		data->width = (int16)cal_pts[1].x - (int16)cal_pts[0].x;
	  if (cal_pts[1].y < cal_pts[0].y)
		data->height = -((int16)cal_pts[0].y - (int16)cal_pts[1].y);
	  else
		data->height = (int16)cal_pts[1].y - (int16)cal_pts[0].y;
	  data->screen_width = screen_width;
	  data->screen_height = screen_height;

	  _os_setcrc(head);

	  /* save data module to persistant storage */
	  if (output_file) {
		out_f = fopen(output_file,"wb");
		if (out_f)
		  {
			if (fwrite(head,MODSIZE(head),1,out_f) != 1)
			  fprintf(stderr,"Could not write data to file '%s'\n",output_file);
			fclose(out_f);
		  }
		else
		  fprintf(stderr,"Could not open file '%s' for write\n",output_file);
	  }
	}

  /* leave module in memory if we created it, otherwise unlink */
  if (head && !our_mod)
	_os_unlink(head);
}


/*
** allocates a cursor that is "invisible".
*/
CURSOR_STRUCT *allocate_empty_cursor(GFX_CM coding_method)
{
  CURSOR_STRUCT *cs;

  coding_method = gfx_get_cm_name(coding_method);

  /* allocate all structures for cursor */
  cs = (CURSOR_STRUCT*)malloc(sizeof(CURSOR_STRUCT));

  if (cs) {
	/* clear everything */
	memset(cs,0,sizeof(CURSOR_STRUCT));
	memset(cs->bitmap_pixmem,0xff,CURSOR_SIZE);

	/* setup cursor fields */
	cs->win_cursor.bitmap = &cs->bitmap;
	cs->win_cursor.mask = &cs->mask;

	/* setup bitmap fields */
	cs->bitmap.coding_method = coding_method;
	cs->bitmap.width = CURSOR_WIDTH;
	cs->bitmap.height = CURSOR_HEIGHT;
	cs->bitmap.line_size = CURSOR_SIZE/CURSOR_HEIGHT;
	cs->bitmap.pixmem = (GFX_PIXEL*)cs->bitmap_pixmem;
	cs->bitmap.palette = &cs->palette;

	/* setup palette fields */
	cs->palette.start_entry = 254;
	cs->palette.num_colors = 2;
	cs->palette.color_type = GFX_COLOR_RGB;
	cs->palette.colors.rgb = cs->palette_colors;

	/* setup palette entrys */
	cs->palette_colors[0] = 0xfe;
	cs->palette_colors[1] = 0xff;
	
	/* setup mask fields */
	cs->mask.coding_method = coding_method;
	cs->mask.width = CURSOR_WIDTH;
	cs->mask.height = CURSOR_HEIGHT;
	cs->mask.line_size = CURSOR_SIZE/CURSOR_HEIGHT;
	cs->mask.pixmem = (GFX_PIXEL*)cs->mask_pixmem;
  }

  return cs;
}


/* function that will properly allocate colors based on the
** display coding method. */
GFX_PIXEL allocate_color(GFX_COLOR_VALUE color_val, GFX_CM coding_method,
						 WIN_CMAP_ID cmap)
{
  GFX_PIXEL pix;
  u_int32 r=(color_val.rgb) >> 16, g=(color_val.rgb) >> 8, b=(color_val.rgb)&0xff;

  coding_method = gfx_get_cm_name(coding_method);

  if (coding_method <= GFX_CM_8BIT) {
	win_alloc_cmap_color(&pix,cmap,color_val);
  } else if (coding_method == GFX_CM_RGB565) {
	pix = M_TO_RGB565(r,g,b);
  } else if (coding_method == GFX_CM_RGB655) {
	pix = M_TO_RGB655(r,g,b);
  } else if (coding_method == GFX_CM_RGB556) {
	pix = M_TO_RGB556(r,g,b);
  }

  return pix;
}


void free_color(GFX_PIXEL pix, GFX_CM coding_method, WIN_CMAP_ID cmap)
{
  coding_method = gfx_get_cm_name(coding_method);
  if (coding_method <= GFX_CM_8BIT) {
	win_free_cmap_cells(cmap,pix,1);
  }
}


/*
** Runs the calibration, and saves the information to the data module
** as well as the output file.
*/
error_code run_calibration(char *output_file, char *font_module)
{
  error_code err=SUCCESS;
  WIN_DEV_ID windev=NULL;
  GFX_DEV_ID gfxdev=NULL;
  INP_DEV_ID touch_dev=NULL;
  WIN_ID root=NULL, win=NULL;
  MSG_MBOX_ID mbox=NULL;
  char *mbox_name="tscal.mbox";
  WIN_STATUS win_status;
  int32 cal_w;
  int32 cal_h;
  GFX_POINT cal_pts[CALIBRATION_PTS];
  GFX_DIMEN screen_width, screen_height;
  CURSOR_STRUCT *empty_cursor=NULL;
  WIN_DEV_STATUS windev_status;
  TXT_FONT *font=NULL;
  TXT_CONTEXT_ID txtctx=NULL;
  DRW_CONTEXT_ID drwctx=NULL;

  /* initialize maui */
  cdb_init();
  msg_init();
  drw_init();
  inp_init();
  txt_init();
  win_init();

  /* create input mailbox */
  err = msg_create_mbox(&mbox,mbox_name,50,sizeof(MAUI_MSG), 0);
  if (err) {
	fprintf(stderr,"Could not create mailbox\n");
	goto cleanup;
  }

  /* open windowing device */
  err = open_windev(&windev, &gfxdev, &root, mbox);
  if (err) goto cleanup;

  err = win_get_dev_status(&windev_status,windev);
  if (err) goto cleanup;

  err = win_get_win_status(&win_status,root);
  if (err) goto cleanup;

  /* open touch screen device */
  touch_dev = open_touch_device(mbox);
  if (!touch_dev) goto cleanup;
	
  /* create full-screen window that we can draw in */
  err = win_create_win(&win,root,0,0,win_status.width,win_status.height,
					   WIN_MASK_EXPOSE, WIN_FRONT);
  if (err) goto cleanup;

  /* make window visible */
  win_set_state(win,TRUE);
  
  /* allocate empty cursor for window */
  empty_cursor = allocate_empty_cursor(windev_status.coding_method);
  if (!empty_cursor) {
	err = EOS_NORAM;
	goto cleanup;
  }

  err = win_create_cursor(windev,CURSOR_ID,&empty_cursor->win_cursor);
  if (err) goto cleanup;

  err = win_set_cursor(win,CURSOR_ID);
  if (err) goto cleanup;

  /* create/setup draw context */
  err = drw_create_context(&drwctx,gfxdev);
  if (err) goto cleanup;
  drw_set_context_fm(drwctx,DRW_FM_SOLID);

  /* setup font */
  err = demo_get_ucm_font(&font, MEM_DEF_SHADE, font_module);
  if (err) {
	fprintf(stderr,"Could not link to ucm font '%s'\n",font_module);
	goto cleanup;
  }
  err = txt_create_context(&txtctx, gfxdev);
  if (err) goto cleanup;
  err = txt_set_context_font(txtctx, font);
  if (err) goto cleanup;

  event_loop(win,mbox,cal_pts,windev_status.coding_method,
			 gfxdev,win_status.cmap,
			 drwctx,txtctx,font->height);

  /* we need to adjust the calibration points slightly, because
  ** the user was prompted to press a point on the screen inside
  ** the screen boundaries. */
  screen_width = win_status.width;
  cal_w = ((cal_pts[1].x - cal_pts[0].x) * (int32)TARGET_WIDTH)/
	(int32)((screen_width - TARGET_WIDTH) << 1);

  screen_height = win_status.height;
  cal_h = ((cal_pts[1].y - cal_pts[0].y) * (int32)TARGET_HEIGHT)/
	(int32)((screen_height - TARGET_HEIGHT) << 1);

  cal_pts[0].x -= cal_w;
  cal_pts[0].y -= cal_h; 
  cal_pts[1].x += cal_w;
  cal_pts[1].y += cal_h;

  /* write calibration points to outpout file and module.  Do this
  ** before setting the sim_method back to INP_SIM_NATIVE */
  write_cal_pts(CALIBRATION_MOD_NAME, cal_pts, output_file,
				screen_width, screen_height);

  /* reset input input device before windowing device is destroyed */
  inp_set_sim_meth(touch_dev,0,NULL,INP_SIM_NATIVE);

cleanup:
  /* free resources in reverse order of allocation */

  if (win)
	win_destroy_win(win);

  if (txtctx)
	txt_destroy_context(txtctx);

  if (drwctx)
	drw_destroy_context(drwctx);

  if (empty_cursor && windev) {
	win_destroy_cursor(windev,CURSOR_ID);
	free(empty_cursor);
  }

  if (touch_dev)
	inp_close_dev(touch_dev);

  if (windev)
	win_close_dev(windev);

  if (font)
	demo_release_ucm_font(font);

  if (mbox)
	msg_close_mbox(mbox);

  win_term();
  txt_term();
  inp_term();
  drw_term();
  msg_term();
  cdb_term();

  return err;
}


error_code event_loop(WIN_ID win, MSG_MBOX_ID mbox,
					  GFX_POINT cal_pts[CALIBRATION_PTS],
					  GFX_CM coding_method, GFX_DEV_ID gfxdev,
					  WIN_CMAP_ID cmap, DRW_CONTEXT_ID drwctx,
					  TXT_CONTEXT_ID txtctx, int text_height)

{
  WIN_STATUS win_status;
  GFX_COLOR_VALUE color;
  GFX_PIXEL pix[NUM_COLORS];
  GFX_PIXEL bkgd, target_fg, target_bg;
  GFX_RECT tmp_rect;
  GFX_POINT target;
  GFX_POINT points[NUM_POINTS];
  u_int32 num_points=0,i=0;
  int recorded_pt=0;
  MAUI_MSG msg;
  error_code err;
  GFX_PIXEL exptbl[2];

  /* get status for win */
  (void) win_get_win_status(&win_status,win);

  /* allocate colors */
  color.rgb = M_RGB(25,25,112);
  pix[BACKGROUND] = target_bg = bkgd = allocate_color(color,coding_method,cmap);
  color.rgb = M_RGB(255,0,0);
  pix[FOREGROUND] = target_fg = allocate_color(color,coding_method,cmap);
  color.rgb = M_RGB(240,240,240);
  pix[WHITE] = allocate_color(color,coding_method,cmap);
  exptbl[0] = target_bg;
  exptbl[1] = pix[WHITE];

  /* set contexts into window */
  (void) win_set_drw_context(win,drwctx);
  (void) win_set_txt_context(win,txtctx);
  txt_set_context_exptbl(txtctx, 2, exptbl);

  /* initially change screen color */
  tmp_rect.x = tmp_rect.y = 0;
  tmp_rect.width = win_status.width;
  tmp_rect.height = win_status.height;
  target.x = target.y = 0;
  draw_screen(win,drwctx,&tmp_rect,bkgd,target_fg,target_bg,&target,txtctx,
			  text_height);

  /* event loop */
  do {
	msg.any.type = MSG_TYPE_NONE;
	err = msg_read(mbox,&msg,MSG_TYPE_ANY,MSG_BLOCK);
	if (err == SUCCESS)
	  {
		/* process message */
		
		if (msg.any.type == MSG_TYPE_WIN)
		  {
			/* windowing message */
			WIN_MSG *winmsg = (WIN_MSG*)&msg;
			
			/* expose message, draw the sreen */
			if (winmsg->any_win.wtype == WIN_MSG_EXPOSE)
			  draw_screen(win,drwctx, &(winmsg->expose.exposed),
						  bkgd,target_fg,target_bg,&target,txtctx,
						  text_height);
		  }
		else if (msg.any.type == MSG_TYPE_PTR)
		  {
			MSG_PTR *ptrmsg = (MSG_PTR*)&msg;
			
			/* received a raw touch screen pointer message */
			
			/* store point in array, update i and num_points */
			points[i++] = ptrmsg->position;
			if (i == NUM_POINTS)
			  i = 0; /* wrap around... */
			else if (num_points < NUM_POINTS)
			  num_points ++;

			/* invert target if we have received enough points */
			if (num_points == 5) {
			  target_fg = pix[BACKGROUND];
			  target_bg = pix[FOREGROUND];
			  draw_screen(win,drwctx, &tmp_rect, bkgd, target_fg,
						  target_bg, &target, txtctx, text_height);
			}
			
			if (ptrmsg->subtype == INP_PTR_UP)
			  {
				/* recieved pointer up message.  Average gathered points
				** and put in cal_pts array. */
				point_analysis(points,num_points,&cal_pts[recorded_pt++]);
				
				if (recorded_pt < CALIBRATION_PTS) {
				  /* draw over target... */
				  target_fg = target_bg = pix[BACKGROUND];
				  draw_screen(win,drwctx,&tmp_rect,bkgd,target_fg,
							  target_bg,&target,txtctx,
							  text_height);

				  /* before continuting onward, wait x seconds-for visual effect */
				  wait_eat_msgs(mbox);
				}
				
				if (recorded_pt == 1)
				  {
					/* reset target for second point */
					target_fg = pix[FOREGROUND];
					target_bg = pix[BACKGROUND];
					num_points = i = 0;
					target.x = win_status.width - TARGET_WIDTH - 1;
					target.y = win_status.height - TARGET_HEIGHT - 1;
					draw_screen(win,drwctx,&tmp_rect,bkgd,
								target_fg,target_bg,&target,txtctx,
								text_height);

				  }
				
			  }
		  }
	  }
  } while (recorded_pt < CALIBRATION_PTS);

  /* shutdown */
  win_set_drw_context(win,NULL);
  win_set_txt_context(win,NULL);

  free_color(pix[BACKGROUND],coding_method,cmap);
  free_color(pix[FOREGROUND],coding_method,cmap);
  free_color(pix[WHITE],coding_method,cmap);

  return err;
}


void wait_eat_msgs(MSG_MBOX_ID mbox)
{
  MAUI_MSG msg;
  error_code err;
  int button_state = 0;
  
  /* sleep for x seconds */
  my_sleep(M_MSEC(1000));

  /* now eat all messages in the mailbox.  If another button down was
  ** detected, wait until we get the cooresponding up. */
  do {
	msg.any.type = MSG_TYPE_NONE;
	err = msg_read(mbox,&msg,MSG_TYPE_ANY,MSG_NOBLOCK);
	if (err == SUCCESS) {

	  if (msg.any.type == MSG_TYPE_PTR) {
		/* track pointer up/down messages */

		MSG_PTR *ptrmsg = (MSG_PTR*)&msg;
		
		if (ptrmsg->subtype == INP_PTR_DOWN)
		  button_state ++;
		if (ptrmsg->subtype == INP_PTR_UP)
		  button_state --;
	  }
	}
  } while ((msg.any.type != MSG_TYPE_NONE) || (button_state > 0));
}





INP_DEV_ID open_touch_device(MSG_MBOX_ID mbox)
{
  error_code err;
  char name[CDB_MAX_DNAME], param[CDB_MAX_PARAM];
  u_int32 i=1;
  INP_DEV_ID touch_dev=NULL;
  INP_DEV_CAP dev_cap;
  BOOLEAN found_touch_device=FALSE;

  /* find the first remote device that is a touchscreen.  Open it, set the
  ** simulation mode to RAW, and return it. */
  err = cdb_get_ddr(CDB_TYPE_REMOTE,i++,name,param);
  while (err == SUCCESS)
	{
	  err = inp_open_dev(&touch_dev,mbox,name);
	  if (err == SUCCESS)
		{
		  err = inp_get_dev_cap(&dev_cap, touch_dev);
		  if (err == SUCCESS)
			{
			  if (dev_cap.ptr_type == INP_CLASS_TOUCHSCR)
				{
				  found_touch_device = TRUE;
				  err = inp_set_sim_meth(touch_dev,0,NULL,(INP_SIM_METH)RAW_MODE);
				  if (err == SUCCESS)
					{
					  fprintf(stderr,"Found touch screen device '%s'\n",name);
					  return touch_dev;
					}

				  fprintf(stderr,"Could not set simulation method of touch screen device \'%s\': \'%s\'\n",name,strerror(err));
				}
			}
		  
		  /* close device, and get next one's name */
		  inp_close_dev(touch_dev);
		  err = cdb_get_ddr(CDB_TYPE_REMOTE,i++,name,param);
		}
	}

  if (!found_touch_device)
	fprintf(stderr,"Did not find touch screen device in CDB\n");

  return NULL;
}


int point_compare(const void *e1, const void *e2)
{
  GFX_POINT *p1 = (GFX_POINT*)e1, *p2 = (GFX_POINT*)e2;
  u_int32 d1, d2;
  int result;

  d1 = p1->x * p1->x + p1->y * p1->y;
  d2 = p2->x * p2->x + p2->y * p2->y;

  if (d1 < d2)
    result = -1;
  else if (d1 > d2)
    result = 1;
  else
    result = 0;

  return result;
}


void point_analysis(const GFX_POINT *points, u_int32 num_points,
					GFX_POINT *cal_pts)
{
  u_int32 x_average=0, y_average=0;
  const GFX_POINT *end_pt = points + num_points;
  const GFX_POINT *pt = end_pt - num_points;

  while (pt < end_pt)
	{
	  x_average += pt->x;
	  y_average += pt->y;
	  pt++;
	}
  x_average /= num_points;
  y_average /= num_points;

  cal_pts->x = x_average;
  cal_pts->y = y_average;
}


void draw_screen(WIN_ID win,DRW_CONTEXT_ID drwctx,const GFX_RECT *draw_rect,
				 GFX_PIXEL bg, GFX_PIXEL fg, GFX_PIXEL target_bg,
				 const GFX_POINT *target, TXT_CONTEXT_ID txtctx,
				 int text_height)
{
  GFX_POS x=draw_rect->x,y=draw_rect->y;
  GFX_DIMEN width=draw_rect->width,height=draw_rect->height;
  GFX_POS x2,y2;
  size_t txtlen;

  win_set_drw_area(win,x,y,width,height);
  win_lock_region(win,x,y,width,height);

  /* fill background */
  drw_set_context_pix(drwctx,bg);
  drw_rectangle(drwctx,draw_rect->x,draw_rect->y,
				draw_rect->width,draw_rect->height);

  /* draw "target" */

  /* first background */
  drw_set_context_pix(drwctx,target_bg);
  drw_rectangle(drwctx,target->x,target->y,TARGET_WIDTH,TARGET_HEIGHT);

  /* next target */
  drw_set_context_pix(drwctx,fg);
  
  x = target->x;
  y = target->y;
  x2 = x + (TARGET_WIDTH -1);
  y2 = y + TARGET_HEIGHT;
  drw_point(drwctx,x2,y2);
  drw_line(drwctx, x++, y, x2++, y2);
  drw_point(drwctx,x2,y2);
  drw_line(drwctx, x++, y, x2++, y2);
  
  x = target->x + TARGET_WIDTH;
  x2 = target->x + 1;
  drw_point(drwctx,x2, y2);
  drw_line(drwctx, x--, y, x2--, y2);
  drw_point(drwctx,x2, y2);
  drw_line(drwctx, x, y, x2, y2);

  /* draw text */
  x = 10;
  y = TARGET_HEIGHT + (text_height * 3);
  txtlen = ULONG_MAX;
  txt_draw_mbs(NULL, txtctx, "Press the \"x\" targets", &txtlen, x, y, NULL);
  y += text_height;
  txtlen = ULONG_MAX;
  txt_draw_mbs(NULL, txtctx, "to calibrate the", &txtlen, x, y, NULL);
  y += text_height;
  txtlen = ULONG_MAX;
  txt_draw_mbs(NULL, txtctx, "touchscreen.", &txtlen, x, y, NULL);

  win_unlock_region(win);
}


/*
** Opens the windowing device.  The device name from the cdb is used.
** Post: returns SUCCESS -- side effect of setting windev, gfxdev, and root
*/
error_code open_windev(WIN_DEV_ID *windev, GFX_DEV_ID *gfxdev, WIN_ID *root,
					   MSG_MBOX_ID mbox)
{
  char win_devname[CDB_MAX_DNAME];
  error_code ec;

  /* get windowing device name from cdb */
  if ((ec = cdb_get_ddr(CDB_TYPE_WIN, 1, win_devname, NULL)) != SUCCESS) {
	fprintf(stderr,"Could not find windowing device name in cdb\n");
	return ec;
  }

  /* now, open the windowing device */
  ec = win_open_dev(windev,gfxdev,root,win_devname,mbox);
  if (ec) {
	fprintf(stderr,"Couldn't open windowing device.\n");
	fprintf(stderr,"Make sure the window manager is running.\n");
  }

  return ec;
}


void usage(char *prog)
{
  printf("%s <options> -- touchscreen calibration program\n",prog);
  printf("\t-f[=]<name>        Output filename\n");
  printf("\t-c                 Only run calibration if output filename\n");
  printf("\t                   does not exist\n");
  printf("\t-m[=]<font_module> Use given UCM font module to display text\n");
  printf("\n");
}


void parse_args(int argc, char *argv[],char **filename,int *check,
				char **font_module)
{
  int i;
  char *ptr;

  *filename = NULL;
  *check = FALSE;

  i = 1;
  while (i < argc) {

	if (!strncmp(argv[i],"-f",2)) {
	  /* output filename */
	  ptr = argv[i];
	  ptr += 2;
	  if (*ptr == '=')
		ptr++;

	  if (*ptr != '\0') {
		*filename = ptr;
	  }
	} else if (!strcmp(argv[i],"-c")) {
	  /* check option */
	  *check = TRUE;
	} else if (!strncmp(argv[i],"-m",2)) {
	  /* font module option */
	  ptr = argv[i];
	  ptr += 2;
	  if (*ptr == '=')
		ptr ++;

	  if (*ptr != '\0') {
		*font_module = ptr;
	  }
	} else {
	  usage(argv[0]);
	  exit(0);
	}

	i++;
  }
}


void my_sleep(u_int32 ticks)
{
  u_int32 t = ticks;

#ifdef _OS9000
  signal_code sig;
  (void) _os_sleep(&t,&sig);
#endif
#ifdef _OSK
  (void) _os9_sleep(&t);
#endif
}
