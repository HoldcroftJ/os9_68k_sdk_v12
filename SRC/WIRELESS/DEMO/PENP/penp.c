/*****************************************************************************
** Demo program to show how to output text to a drawmap in MAUI and         **
** wait for user input.                                                     **
******************************************************************************
** Copyright 1995 by Microware Systems Corporation                          **
** Reproduced Under License                                                 **
**                                                                          **
** This source code is the proprietary confidential property of             **
** Microware Systems Corporation, and is provided to licensee               **
** solely for documentation and educational purposes. Reproduction,         **
** publication, or distribution in any form to any party other than         **
** the licensee is strictly prohibited.                                     **
******************************************************************************
** Edition History:                                                         **
** #   Date     Comments                                          By        **
** --- -------- ------------------------------------------------- --------- **
**   1 11/29/95 creation                                          bat       **
**   2 03/14/96 modified to use tpad DevCon info (X_PIXELS,       rws       **
**              Y_PIXELS, CYC_ALM_PERIOD, and SAMPLES_PER_POINT)            **
*****************************************************************************/

/*
** Header Files
*/

#include <types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <memory.h>
#include <modes.h>
#include <sg_codes.h>
#include "mc328ads.h"
#include "load.h"

#include <MAUI/maui_mem.h>
#include <MAUI/maui_gfx.h>
#include <MAUI/maui_blt.h>
#include <MAUI/maui_drw.h>
#include <MAUI/maui_txt.h>
#include <MAUI/maui_msg.h>
#include <MAUI/maui_inp.h>


/*
** Local Definitions
*/

/* colored memory shade ID definitions */
#define SHADE_MISC		2
#define SHADE_GRAPHICS	3

char gfx_device[20];

/* #define FONTNAME1 "FONT8X8" */
#define FONTNAME2 "default.fnt"

#define MAILBOX_NAME	"penp_mbox"
char mailbox_dev[20];

#ifndef SUCCESS
#define SUCCESS (0)
#endif

u_int32 xsize, ysize;		/* display resolution */

typedef union _MESSAGE {			/* Union of message types we care about */
	MSG_PTR ptr;
	MSG_KEY key;
} MESSAGE;


/*
** Global Variables
*/

GFX_DEV_ID gfxdev = 0;			/* Graphics device ID */
const GFX_DEV_CAP *ret_dev_cap;		/* device capability pointer */
GFX_DMAP *srcdmap = NULL;		/* Source Drawmap pointer  */
GFX_DMAP *imgsrcdmap = NULL;	/* Source Drawmap pointer  */
GFX_DMAP *dstdmap = NULL;		/* destination Drawmap (in VRAM) pointer */
GFX_VPORT_ID vport = 0;			/* Viewport ID */ 
BLT_CONTEXT_ID blt_context = 0; /* BLT context ID */
DRW_CONTEXT_ID drw_ctx;			/* Draw context ID */
TXT_CONTEXT_ID  txtctx;			/* Text API Context */
/* TXT_FONT *font1; */
TXT_FONT *font2;
MSG_MBOX_ID mbox = 0;			/* Mailbox ID */
INP_DEV_ID inpdev = 0;			/* Input device ID */
dcmd_struct tpad_cmd;			/* _os_setstat() direct command structure */
path_id drv_path;				/* Touch-pad path ID */
char *string;					/* Pointer to text string */
size_t len;						/* Length of text string */
GFX_DIMEN pwidth;				/* Pixel width of a text string write */



/*
** Function Prototypes
*/

void main(int argc, char *argv[]);
error_code initialize(void);
void sighand(int sigcode);
void terminate(error_code err);
extern int _errmsg(int nerr, char *msg, ...);
extern void intercept (void (*sighand)());
extern error_code _os_getstat (path_id path, u_int32 code, void *pb);
extern error_code _os_setstat (path_id path, u_int32 code, void *pb);
extern error_code get_ucm_font (TXT_FONT *font, u_int32 shade, char *fontname);
extern error_code release_ucm_font (TXT_FONT *font);
void display_image(char *image_name);


/*
** Program Entry Point
**
** Call:
**		penp <gfx_descriptor_name> <tpad_descriptor_name>
*/
void main (int argc, char *argv[])
{
	error_code ec;
	MESSAGE message;
	GFX_POS oldx = 0;
	GFX_POS oldy = 0;
#if defined (TXT_TEST)
	u_int32 loop_var;
#endif

	/* variables used for _os_link() */
	char tpad_name[20];
	char *link_name = &tpad_name[0];
	mod_dev *mod_head = NULL;
	void *trash;
	u_int16 type_lang = 0;

	if (argc != 3)
	{
		fprintf (stderr, "\n\nUsage:\n\n");
		fprintf (stderr, "penp <gfx_descriptor_name> <touchpad_descriptor_name>\n");
		fprintf (stderr, "e.g.:   penp gfx tpad \n\n");
		exit (1);
	}

#if defined (DEBUG)
	fprintf (stderr, "argv[0]=%s, argv[1]=%s, argv[2]=%s\n",
		argv[0], argv[1], argv[2]);
#endif

	gfx_device[0] = '/';
	strcpy (&gfx_device[1],argv[1]);

#if defined (DEBUG)
	fprintf (stderr, "gfx_device = %s\n", gfx_device);
#endif

	strcpy (tpad_name,argv[2]);

#if defined (DEBUG)
	fprintf (stderr, "tpad_name = %s\n", tpad_name);
#endif
	
	mailbox_dev[0] = '/';
	strcpy (&mailbox_dev[1], argv[2]);
	strcat (mailbox_dev, "/ads_mppm");

#if defined (DEBUG)
	fprintf (stderr, "mailbox_dev = %s\n", mailbox_dev);
#endif

	/*
	 * Setup a signal handler function so that we can clean up properly
	 * if we receive a signal.
	 */ 
	intercept(sighand);
   
	/* link to the touchpad descriptor and get xsize and ysize info */
	if ((ec = _os_link (&link_name, (mh_com **)&mod_head, (void **)&trash, 
				&type_lang, (u_int16 *)&trash)) != SUCCESS) {
		_errmsg (0, "Can't link to %s (error 0x%2x)\n", tpad_name, ec);
		exit (1);
	}
	xsize = *(u_int32 *)((char *)mod_head + mod_head->_mdevcon);
	ysize = *(u_int32 *)((char *)mod_head + mod_head->_mdevcon + 4);

#if defined (DEBUG)
	fprintf (stderr, "xsize = %d, ysize = %d\n", xsize, ysize);
#endif

	/*
	 * Connect to the touchpad driver.
	 */
	tpad_name[0] = '/';
	strcpy (&tpad_name[1], argv[2]);
	if ((ec = _os_open (tpad_name, FAM_READ+FAM_WRITE, &drv_path)) != SUCCESS) {
		_errmsg (0, "Can't open path to %s (error 0x%2x)\n", tpad_name, ec);
		exit (1);
	}

	if ((ec = initialize()) != SUCCESS)
		terminate(ec);

#if defined (BLT_TEST)

	fprintf(stderr, "BLT Test: Touch Touchpad to Continue...\n");

	display_image("pcss.img"); 
/*	display_image("dilbert1.img"); */

	/* wait for touchscreen to be released */
	while ((ec = msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK)) == SUCCESS)
		if (message.ptr.com.type == MSG_TYPE_PTR &&
			message.ptr.subtype == INP_PTR_UP)
				break;
				
	if (ec != SUCCESS)
		terminate(_errmsg(ec, "msg_read() error during BLT test\n"));

	/* erase screen */
	if ((ec = blt_copy_block (blt_context, 0, 0, 0, 0, xsize, ysize)) != SUCCESS)
		terminate(_errmsg(ec, "cannot erase screen\n"));

#endif

#if defined (TXT_TEST)

/*	if ((ec = txt_set_context_font(txtctx, font1)) != SUCCESS) 
		terminate(_errmsg(ec, "Can't set text context font1\n"));
*/
	if ((ec = txt_set_context_font(txtctx, font2)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set text context font2\n"));

	for (loop_var = 0; loop_var < 8; loop_var++)
	{
		if (loop_var == 7) {
			string = "Touch to End...";
			len = strlen(string);
		} else {
			string = "Touch to Continue...";
   			len = strlen(string);
		}
		
		fprintf(stderr, "Text Test: Line %d at (%d,%d).  Touch Touchpad to %s...\n",
			loop_var, loop_var+8, loop_var+8, (loop_var == 7) ? "End" : "Continue");
		
		if ((ec = txt_draw_mbs (&pwidth, txtctx, string, &len,
								loop_var+8, loop_var+8, NULL)) != SUCCESS)
			terminate(_errmsg(ec, "Can't draw text test text\n"));

		/* wait for touchscreen to be released */
		while ((ec = msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK)) == SUCCESS)
			if (message.ptr.com.type == MSG_TYPE_PTR &&
				message.ptr.subtype == INP_PTR_UP)
					break;
				
		if (ec != SUCCESS)
			terminate(_errmsg(ec, "msg_read() error during text test\n"));

		/* erase screen */
		if ((ec = blt_copy_block (blt_context, 0, 0, 0, 0, xsize, ysize)) != SUCCESS)
			terminate(_errmsg(ec, "cannot erase screen\n"));
	}

#endif	

	/* Touchpanel Calibration */

    /* read calibration values -- should be DEFAULT values */
    tpad_cmd.command = CALIBRATE;
    if ( (ec = _os_getstat (drv_path, SS_DCmd, (void *) &tpad_cmd)) != SUCCESS) {
        _errmsg (0, "Error on getstat(CALIBRATE) ((error 0x%2x)\n", ec);
        terminate (ec);
    }
    else
    {
        printf ("Default Calibration values:\n");
        printf ("%s = 0x%04x\n%s = 0x%04x\n%s = 0x%04x\n%s = 0x%04x\n",
                "tpad_cmd.ss_specific.cal_data.ss_X_min",
                tpad_cmd.ss_specific.cal_data.ss_X_min,
                "tpad_cmd.ss_specific.cal_data.ss_Y_min",
                tpad_cmd.ss_specific.cal_data.ss_Y_min,
                "tpad_cmd.ss_specific.cal_data.ss_X_max",
                tpad_cmd.ss_specific.cal_data.ss_X_max,
                "tpad_cmd.ss_specific.cal_data.ss_Y_max",
                tpad_cmd.ss_specific.cal_data.ss_Y_max);
    }

/*	if ((ec = txt_set_context_font(txtctx, font1)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set text context font1\n"));
*/
	if ((ec = txt_set_context_font(txtctx, font2)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set text context font2\n"));
	string = "CALIBRATING:";
	len = strlen(string);
	if ((ec = txt_draw_mbs (&pwidth, txtctx, string, &len, 72, 68, NULL)) != SUCCESS)
		terminate(_errmsg(ec, "Can't draw text\n"));

	string = "TOUCH BULLSEYE";
	len = strlen(string);
	if ((ec = txt_draw_mbs (&pwidth, txtctx, string, &len, 64, 84, NULL)) != SUCCESS)
		terminate(_errmsg(ec, "Can't draw text\n"));

	/*
	 * draw first bullseye diagram for touch calibration at coordinates 
	 * (30, 30).
	 * We actually read the message from the mailbox so we will block
	 * until the point is read.  We then throw the message away.
	 */
	/* DRW_FM_SOLID set in initialize() */
	if ((ec = drw_circle (drw_ctx, 30, 30, 3)) != SUCCESS)
		terminate(_errmsg(ec, "Can't draw circle (30, 30, 3)\n"));
	if ((ec = drw_set_context_fm(drw_ctx, DRW_FM_OUTLINE)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set drw context fill mode\n"));
	if ((ec = drw_circle (drw_ctx, 30, 30, 20)) != SUCCESS)
		terminate(_errmsg(ec, "Can't draw circle (30, 30, 20)\n"));
	if ((ec = drw_set_context_fm(drw_ctx, DRW_FM_SOLID)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set drw context fill mode\n"));
	
	tpad_cmd.command = GET_POINT;
	tpad_cmd.ss_specific.gfx_point.x_pos = 30;
	tpad_cmd.ss_specific.gfx_point.y_pos = 30;
	if ((ec = _os_setstat (drv_path, SS_DCmd, &tpad_cmd)) != SUCCESS) 
	{
		_os_close (drv_path);
		terminate(_errmsg(ec, "_os_setstat(SS_DCmd) failed\n"));
	}    

	fprintf(stderr, "Calibrating to position %d, %d\n",
		tpad_cmd.ss_specific.gfx_point.x_pos,
		tpad_cmd.ss_specific.gfx_point.y_pos);

	/* wait for touchscreen to be released */
	while ((ec = msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK)) == SUCCESS)
		if (message.ptr.com.type == MSG_TYPE_PTR &&
			message.ptr.subtype == INP_PTR_UP)
				break;
				
	if (ec != SUCCESS)
		terminate(_errmsg(ec, "cannot calibrate touch pad\n"));

	/* erase screen */
	if ((ec = blt_copy_block (blt_context, 0, 0, 0, 0, xsize, ysize)) != SUCCESS)
		terminate(_errmsg(ec, "cannot erase screen\n"));
   
	if ((ec = txt_set_context_font(txtctx, font2)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set text context font2\n"));
	string = "CALIBRATING:";
	len = strlen(string);
	if ((ec = txt_draw_mbs (&pwidth, txtctx, string, &len, 42, 68, NULL)) != SUCCESS)
		terminate(_errmsg(ec, "Can't draw text\n"));

	string = "TOUCH BULLSEYE";
	len = strlen(string);
	if ((ec = txt_draw_mbs (&pwidth, txtctx, string, &len, 34, 84, NULL)) != SUCCESS)
		terminate(_errmsg(ec, "Can't draw text\n"));

	/*
	 * draw second bullseye diagram for touch calibration at coordinates 
	 * (xsize-30, ysize-30).
	 * We actually read the message from the mailbox so we will block
	 * until the point is read.  We then throw the message away.
	 */
	/* DRW_FM_SOLID set above */
	if ((ec = drw_circle (drw_ctx, xsize-30, ysize-30, 3)) != SUCCESS)
		terminate(_errmsg(ec, "Can't draw circle (xsize-30, ysize-30, 3)\n"));
	if ((ec = drw_set_context_fm(drw_ctx, DRW_FM_OUTLINE)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set drw context fill mode\n"));
	if ((ec = drw_circle (drw_ctx, xsize-30, ysize-30, 20)) != SUCCESS)
		terminate(_errmsg(ec, "Can't draw circle (xsize-30-1, ysize-30-1, 20)\n"));
	if ((ec = drw_set_context_fm(drw_ctx, DRW_FM_SOLID)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set drw context fill mode\n"));
	
	tpad_cmd.command = GET_POINT;
	tpad_cmd.ss_specific.gfx_point.x_pos = xsize-30;
	tpad_cmd.ss_specific.gfx_point.y_pos = ysize-30;
	if ((ec = _os_setstat (drv_path, SS_DCmd, &tpad_cmd)) != SUCCESS) 
	{
		_os_close (drv_path);
		terminate(_errmsg(ec, "_os_setstat(SS_DCmd) failed\n"));
	}    

	fprintf(stderr, "Calibrating to position %d, %d\n\n",
		tpad_cmd.ss_specific.gfx_point.x_pos,
		tpad_cmd.ss_specific.gfx_point.y_pos);

	/* wait for touchscreen to be released */
	while ((ec = msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK)) == SUCCESS)
		if (message.ptr.com.type == MSG_TYPE_PTR &&
			message.ptr.subtype == INP_PTR_UP)
				break;
				
	if (ec != SUCCESS)
		terminate(_errmsg(ec, "cannot calibrate touch pad\n"));

    /* read new calibration values */
    tpad_cmd.command = CALIBRATE;
    if ( (ec = _os_getstat (drv_path, SS_DCmd, (void *) &tpad_cmd)) != SUCCESS) {
        _errmsg (0, "Error on getstat(CALIBRATE) ((error 0x%2x)\n", ec);
        terminate(ec);
    }
    else
    {
        printf ("Default Calibration values:\n");
        printf ("%s = 0x%04x\n%s = 0x%04x\n%s = 0x%04x\n%s = 0x%04x\n",
                "tpad_cmd.ss_specific.cal_data.ss_X_min",
                tpad_cmd.ss_specific.cal_data.ss_X_min,
                "tpad_cmd.ss_specific.cal_data.ss_Y_min",
                tpad_cmd.ss_specific.cal_data.ss_Y_min,
                "tpad_cmd.ss_specific.cal_data.ss_X_max",
                tpad_cmd.ss_specific.cal_data.ss_X_max,
                "tpad_cmd.ss_specific.cal_data.ss_Y_max",
                tpad_cmd.ss_specific.cal_data.ss_Y_max);
    }

	/* erase screen */
	if ((ec = blt_copy_block (blt_context, 0, 0, 0, 0, xsize, ysize)) != SUCCESS)
		terminate(_errmsg(ec, "cannot erase screen\n"));
   
	fprintf(stderr, "Begin Etch-a-Sketch Mode\n");
	fprintf(stderr, "Press <Control>E to exit program\n");
	fprintf(stderr, "Press <Control>C to clear display\n");

loop:

	/* Wait for the user to press the touch-screen */
	while ((ec = msg_read(mbox, &message, MSG_TYPE_ANY, MSG_BLOCK)) == SUCCESS) 
	{
		/* is this a pointer event */
		if (message.ptr.com.type == MSG_TYPE_PTR) {

			/* is this a pen-down event */
			if (message.ptr.subtype == INP_PTR_DOWN) {
				if ((ec = drw_point(drw_ctx,message.ptr.position.x,
										message.ptr.position.y)) != SUCCESS)
					terminate(_errmsg(ec,"cannot draw line\n"));
				oldx = message.ptr.position.x;
				oldy = message.ptr.position.y;
			}
		
			/* is this a pen-movement event */
			else if (message.ptr.subtype == INP_PTR_MOVE) {
				if ((ec = drw_line(drw_ctx,oldx,oldy,message.ptr.position.x,
										message.ptr.position.y)) != SUCCESS)
					terminate(_errmsg(ec,"cannot draw line\n"));
				oldx = message.ptr.position.x;
				oldy = message.ptr.position.y;
			}
		}

		else {
			terminate(_errmsg(EOS_PARAM, "unknown message type\n"));
		}
	}

	/* see if msg_read() aborted due to ^C being pressed */
	if (ec == EOS_MAUI_SIGNAL)	
		goto loop;

	terminate(_errmsg(ec, "message read error\n"));
}

error_code initialize(void)
{
	error_code ec;

	/* initialize memory api */
	{
	 	/* Create needed memory shades */
		if ((ec = mem_init()) != SUCCESS)
			return(_errmsg(ec, "Can't initialize memory api\n"));
		if ((ec = mem_create_shade(SHADE_MISC, MEM_SHADE_NORMAL, MEM_ANY, 
						4096, 4096, MEM_OV_ATTACHED, FALSE)) != SUCCESS) {
			return(_errmsg(ec, "Can't create miscellaneous shade\n"));
		}
		if ((ec = mem_create_shade(SHADE_GRAPHICS, MEM_SHADE_NORMAL, MEM_ANY, 
						8192, 4096, MEM_OV_ATTACHED, FALSE)) != SUCCESS) {
			return(_errmsg(ec, "Can't create graphics shade\n"));
		}
	}

	/* initialize graphics api */
	{
		if ((ec = gfx_init()) != SUCCESS)
			return(_errmsg(ec, "Can't initialize graphics api\n"));

		/* Open the graphics device */
		if ((ec = gfx_open_dev(&gfxdev, gfx_device)) != SUCCESS)
			return(_errmsg(ec, "Can't open graphics device %s\n", gfx_device));

		/* Create & initialize GFX_DMAP structures */
		if ((ec = gfx_create_dmap(&srcdmap, SHADE_GRAPHICS)) != SUCCESS)
			return(_errmsg(ec, "Can't create source drawmap\n"));
		if ((ec = gfx_create_dmap(&dstdmap, SHADE_GRAPHICS)) != SUCCESS)
			return(_errmsg(ec, "Can't create destination drawmap\n"));
		if ((ec = gfx_create_dmap(&imgsrcdmap, SHADE_GRAPHICS)) != SUCCESS)
			return(_errmsg(ec, "Can't create image drawmap\n"));

		/* initialize drawmap elements to this application */
		if ((ec = gfx_set_dmap_size(srcdmap, GFX_CM_1BIT, xsize, ysize)) != SUCCESS)
			return(_errmsg(ec, "Can't set source drawmap size\n"));
		if ((ec = gfx_set_dmap_size(dstdmap, GFX_CM_1BIT, xsize, ysize)) != SUCCESS)
			return(_errmsg(ec, "Can't set destination drawmap size\n"));
		if ((ec = gfx_set_dmap_size(imgsrcdmap, GFX_CM_1BIT, xsize, ysize)) != SUCCESS)
			return(_errmsg(ec, "Can't set image drawmap size\n"));

		/* allocate drawmap's pixel memory */
		if ((ec = gfx_set_dmap_pixmem(srcdmap, NULL, SHADE_GRAPHICS, 0)) != SUCCESS)
			return(_errmsg(ec, "Can't allocate pixel memory for source drawmap\n"));
		if ((ec = gfx_set_dmap_pixmem(dstdmap, NULL, SHADE_GRAPHICS, 0)) != SUCCESS)
			return(_errmsg(ec, "Can't allocate pixel memory for dest drawmap\n"));
		if ((ec = gfx_set_dmap_pixmem(imgsrcdmap, NULL, SHADE_GRAPHICS, 0)) != SUCCESS)
			return(_errmsg(ec, "Can't allocate pixel memory for image drawmap\n"));

		/* no color pallettes (color not supported) */
		srcdmap->palette = dstdmap->palette = imgsrcdmap->palette = NULL;

		/* get device capabilities */
		if ((ec = gfx_get_dev_cap(&ret_dev_cap, gfxdev)) != SUCCESS)
			return(_errmsg(ec, "Can't get device capabilities\n"));

		fprintf (stderr, "disp_width = %d, disp_height = %d\n",
			ret_dev_cap->res_info->disp_width,
			ret_dev_cap->res_info->disp_height);

		/* Create and initialize GFX_VPORT structure --
		 * must be same size as display and start at display location (0,0) */
		if ((ec = gfx_create_vport(&vport, gfxdev, 0, 0, xsize, ysize,
											GFX_VPORT_FRONT)) != SUCCESS)
			return(_errmsg(ec, "Can't create viewport\n"));

		/* associate dstdmap to vport and set initial placement on display */
		if ((ec = gfx_set_vport_dmap(vport, dstdmap, 0, 0)) != SUCCESS)
			return(_errmsg(ec, "Can't set viewport dmap\n"));

		/* Set display parameters and show the viewport */
		if ((ec = gfx_set_vport_state(vport, TRUE)) != SUCCESS)
			return(_errmsg(ec, "Can't set viewport state\n"));
		if ((ec = gfx_update_display(gfxdev, FALSE)) != SUCCESS)
			return(_errmsg(ec, "Can't update display\n"));
	}

	/* initialize BLTing api */
	{
		if ((ec = blt_init()) != SUCCESS)
			return(_errmsg(ec, "Can't initialize BLT api\n"));

		/* create BLT context */
		if ((ec = blt_create_context (&blt_context, gfxdev)) != SUCCESS)
			return(_errmsg(ec, "Can't create BLT context\n"));

		/* Set BLT context parameters */
		if ((ec = blt_set_context_cpymix (blt_context, BLT_MIX_REPLACE)) != SUCCESS)
			return(_errmsg(ec, "Can't set blt_context cpymix\n"));
		if ((ec = blt_set_context_drwmix (blt_context, BLT_MIX_SOD)) != SUCCESS)
			return(_errmsg(ec, "Can't set blt_context drwmix\n"));
		if ((ec = blt_set_context_dst (blt_context, dstdmap)) != SUCCESS)
			return(_errmsg(ec, "Can't set blt_context dst\n"));
		if ((ec = blt_set_context_pix (blt_context, 1)) != SUCCESS)
			return(_errmsg(ec, "Can't set blt_context pix\n"));
		if ((ec = blt_set_context_src (blt_context, srcdmap)) != SUCCESS)
			return(_errmsg(ec, "Can't set blt_context src\n"));
	}

	/* initialize drawing api */
	{
		/* Create drw context */
		if ((ec = drw_init()) != SUCCESS)
			return(_errmsg(ec, "Can't initialize drawing api\n"));
		if ((ec = drw_create_context(&drw_ctx, gfxdev)) != SUCCESS)
			return(_errmsg(ec, "Can't create drw context\n"));
		if ((ec = drw_set_context_dst(drw_ctx, dstdmap)) != SUCCESS)
			return(_errmsg(ec, "Can't set drw context dst\n"));
		if ((ec = drw_set_context_pix(drw_ctx, 1)) != SUCCESS)
			return(_errmsg(ec, "Can't set drw context pixel value\n"));
		if ((ec = drw_set_context_mix(drw_ctx, BLT_MIX_REPLACE)) != SUCCESS)
			return(_errmsg(ec, "Can't set drw context mixing mode\n"));
		if ((ec = drw_set_context_fm(drw_ctx, DRW_FM_SOLID)) != SUCCESS)
			return(_errmsg(ec, "Can't set drw context fill mode\n"));
	}

	/* initialize text api */
	{
		/* initialize API */
		if ((ec = txt_init ()) != SUCCESS)
			return(_errmsg(ec, "Can't initialize text api\n"));

		/* create font object and initialize structure elements */
/*		if ((ec = txt_create_font(&font1, SHADE_GRAPHICS, 1)) !=SUCCESS)
			return(_errmsg(ec, "Can't create font1 object\n"));
*/
		if ((ec = txt_create_font(&font2, SHADE_GRAPHICS, 1)) !=SUCCESS)
			return(_errmsg(ec, "Can't create font2 object\n"));

	    /* initialize TXT_FONT data structure members and load font */
/*		if ((ec = get_ucm_font(font1, SHADE_GRAPHICS, FONTNAME1)) != SUCCESS) 
			return(_errmsg(ec, "Can't load UCM font1\n"));
*/
		if ((ec = get_ucm_font(font2, SHADE_GRAPHICS, FONTNAME2)) != SUCCESS) 
			return(_errmsg(ec, "Can't load UCM font2\n"));

		/* create TXT data structures */
		if ((ec = txt_create_context(&txtctx, gfxdev)) != SUCCESS)
			return(_errmsg(ec, "Can't create txt context\n"));

		/* initialize TXT_CONTEXT_PARAMS data structure members */
		if ((ec = txt_set_context_mix(txtctx, BLT_MIX_REPLACE)) != SUCCESS)
			return(_errmsg(ec, "Can't set gfx context copy mixing mode\n"));
		if ((ec = txt_set_context_dst(txtctx, dstdmap)) != SUCCESS)
			return(_errmsg(ec, "Can't set text context dst\n"));
	}
		           

	/* initialize message api */
	{
		/* Create a mailbox */
		if ((ec = msg_init()) != SUCCESS)
			return(_errmsg(ec, "Can't initialize message api\n"));
		if ((ec = msg_create_mbox(&mbox, MAILBOX_NAME, 20, sizeof(MESSAGE),
													MEM_ANY)) != SUCCESS)
			return(_errmsg(ec, "Can't create mailbox\n"));
	}

	/* initialize input api */
	{
		/* open the input device */
		if ((ec = inp_init()) != SUCCESS)
			return(_errmsg(ec, "Can't initialize input api\n"));
		if ((ec = inp_open_dev(&inpdev, mbox, mailbox_dev)) != SUCCESS) {
			msg_close_mbox(mbox);
			return(_errmsg(ec, "Can't open input device\n"));
		}
	}

	return(SUCCESS);

}

void terminate(error_code err)
{
	/* Destroy everything, terminate each API, and exit */

	/* deinitialize input & message api */
	_os_close(drv_path);
	inp_close_dev(inpdev);
	inp_term();
	msg_close_mbox(mbox);
	msg_term();

#if defined (TXT_TEST)
	/* deinitialize text api and unload font */
/*	release_ucm_font(font1); */
/*	txt_destroy_font(font1); */
	release_ucm_font(font2);
	txt_destroy_font(font2);
	txt_destroy_context(txtctx);
	txt_term();
#endif /* TXT_TEST */

	/* deinitialize drawing api */
	drw_destroy_context(drw_ctx);
	drw_term();

	/* deinitialize graphics api and destroy drawmaps */
	gfx_set_vport_state(vport,FALSE);
	gfx_update_display(gfxdev,FALSE);
	mem_free(srcdmap->pixmem);
	mem_free(dstdmap->pixmem);
	mem_free(imgsrcdmap->pixmem);
	gfx_destroy_vport(vport);
	gfx_destroy_dmap(srcdmap);
	gfx_destroy_dmap(dstdmap);
	gfx_destroy_dmap(imgsrcdmap);
	gfx_close_dev(gfxdev);
	gfx_term();

	/* deinitialize blt api */
	blt_destroy_context(blt_context);
	blt_term();

	/* deinitialize shaded memory api and destroy shades */
	mem_destroy_shade(SHADE_GRAPHICS);
	mem_destroy_shade(SHADE_MISC);
	mem_term();

	exit(err);
}

/*
 * function that loads a raster image in memory (which is in an OS-9
 * data module) into imgsrcdmap, and then BLTs imgsrcdmap into the 
 * destination DMAP
 */
void display_image (char *image_name)
{
	error_code ec;

	/* clear screen */
	gfx_set_vport_state(vport, FALSE);
	gfx_update_display(gfxdev,FALSE);

	/* load called for raster image into imgsrcdmap drawmap */
	if ((ec = load_image (imgsrcdmap, image_name, FALSE)) != SUCCESS)
		terminate (_errmsg(ec, "Can't load image into image drawmap\n"));

	/* copy imgsrcdmap DMAP into destination DMAP */
	if ((ec = blt_set_context_src (blt_context, imgsrcdmap)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set blt_context src\n"));
	if ((ec = blt_copy_block (blt_context, 0, 0, 0, 0, xsize, ysize)) != SUCCESS)
		terminate (_errmsg(ec, "Can't blt image into dest drawmap\n"));
	if ((ec = blt_set_context_src (blt_context, srcdmap)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set blt_context src\n"));

	/* turn display on */
	gfx_set_vport_state(vport, TRUE);
	gfx_update_display(gfxdev,FALSE);
}

void sighand (int sigcode)
{
	error_code ec;

	switch (sigcode) {

		case (2):	/* <control>E keyboard abort signal */
			/* abort program */
			terminate (_errmsg(sigcode, "^E signal received\n"));
			break;

		case (3):	/* <control>C keyboard interrupt signal */
			/* erase screen */
			if ((ec = blt_copy_block (blt_context, 0, 0, 0, 0, xsize, ysize)) != SUCCESS)
				terminate(_errmsg(ec, "cannot erase screen\n"));
			break;

		default:
			terminate (_errmsg(sigcode, "Unexpected signal received\n"));
			break;
	}
	return;
}

