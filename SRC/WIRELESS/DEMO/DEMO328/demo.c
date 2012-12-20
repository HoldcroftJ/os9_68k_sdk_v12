/*****************************************************************************
** Demo program for Embedded Systems Conference, Boston, 3/96.
** For use with Alps 240x160 display, 2 bit images.
******************************************************************************
** Copyright 1996 by Microware Systems Corporation                          **
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
**   2 03/19/96 modification for embedded systems demo dvlpmnt    pbt       **
**	 3 04/03/96 modification for single drawmap					  pbt	    **
**   4 06/10/96 modification to get device caps, xsize & ysize	  pbt		**
**				during initialize								  pbt		**
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
#include <signal.h>
#include "mc328ads.h"
#include "load.h"

#include <MAUI/maui_mem.h>
#include <MAUI/maui_gfx.h>
#include <MAUI/maui_msg.h>
#include <MAUI/maui_inp.h>


/*
** Local Definitions
*/

/* colored memory shade ID definitions */
#define SHADE_GRAPHICS	3

#define GRAPHICS_DEV "/gfx"
#define MAILBOX_NAME "penp_mbox"
#define MAILBOX_DEV	 "/tpad/ads_mppm"

typedef union _MESSAGE {	/* Union of message types we care about */
	MSG_PTR ptr;
	MSG_KEY key;
} MESSAGE;


/*
** Global Variables
*/

GFX_DEV_ID gfxdev = 0;		/* Graphics device ID */
GFX_VPORT_ID vport = 0;		/* Viewport ID */
GFX_DMAP *dmap = NULL;      /* Drawmap Pointer */
MSG_MBOX_ID mbox = 0;		/* Mailbox ID */
INP_DEV_ID inpdev = 0;		/* Input device ID */
u_int32 xsize, ysize;           /* size of display vars */

/* Pointers to OS9 data module containing image -- used by load_image() */
char *image_name_intro = "intro.img";	    /* intro image module */
char *image_name_main  = "mainmenu.img";    /* main_menu image module */
char *image_name_slide = "slide.img";       /* slide image module */
char *image_name_a_f   = "firealarm.img";   /* alarms fire image module */
char *image_name_a_w   = "warnalarm.img";   /* alarms warning image module */
char *image_name_s_1   = "secure1.img";     /* security main  image module */
char *image_name_s_e1  = "secure2.img";     /* employee 1 image module */
char *image_name_s_e2  = "secure3.img";     /* employee 2 image module */
char *image_name_s_e3  = "secure4.img";     /* employee 3 image module */
char *image_name_s_e4  = "secure5.img";     /* employee 4 image module */
char *image_name_m_1   = "message1.img";    /* message1 image module */
char *image_name_m_2   = "message2.img";    /* message2 image module */

/*
** Function Prototypes
*/

void main (void);
void initialize (void);

extern void security_menu (void);	/* functions in menu_funcs.c */
extern void alarms_menu (void);
extern void messages_menu (void);

#if defined (DEBUG)
void sighand (int sigcode);
extern void intercept (void (*sighand)());
extern int _errmsg (int nerr, char *msg, ...);
void terminate (error_code err);
#endif



/*
** Program Entry Point
*/
void main (void)
{
	MESSAGE message;

#if defined (DEBUG)
	error_code ec;
#endif

#if defined (DEBUG)
	/* Setup a signal handler function so that when we exit the program
	 * we can clean up properly */ 
	intercept (sighand);
#endif
   
	/*
	 * initialize MAUI APIs and data structures and load images
	 * into pixel memories
	 */
	initialize();

    /* show the viewport */
#if defined (DEBUG)
    if ((ec = gfx_set_vport_state(vport, TRUE)) != SUCCESS)
        terminate(_errmsg(ec, "Can't set viewport state\n"));
    if ((ec = gfx_update_display(gfxdev, FALSE)) != SUCCESS)
        terminate(_errmsg(ec, "Can't update display\n"));
#else
    gfx_set_vport_dmap(vport, dmap, 0, 0);
    gfx_set_vport_state(vport, TRUE);
    gfx_update_display(gfxdev, FALSE);
#endif

	
	while (1)
	{
#if 0
#if defined (DEBUG)
        if ((ec = load_image (image_name_slide, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
#else
        load_image (image_name_slide, FALSE);
#endif
		ticks = 500;
		_os9_sleep (&ticks);

#endif

#if defined (DEBUG)
        if ((ec = load_image (image_name_intro, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
#else
        load_image (image_name_intro, FALSE);
#endif

		/* wait for Pointer Up message from touchscreen */
#if defined (DEBUG)
		while ((ec = msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK)) == SUCCESS)
			if (message.ptr.com.type == MSG_TYPE_PTR &&
				message.ptr.subtype == INP_PTR_UP)
				break;

		if (ec != SUCCESS)
			terminate(_errmsg(ec, "msg_read() error during msg_read()\n"));
#else
		while (1)
		{
			msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK);
			if (message.ptr.com.type == MSG_TYPE_PTR &&
				message.ptr.subtype == INP_PTR_UP)
				break;
		}
#endif

#if defined (DEBUG)
        if ((ec = load_image (image_name_main, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
#else
        load_image (image_name_main, FALSE);
#endif


		/* wait for Pointer Up message from touchscreen */
#if defined (DEBUG)
		while ((ec = msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK)) == SUCCESS)
			if (message.ptr.com.type == MSG_TYPE_PTR &&
				message.ptr.subtype == INP_PTR_UP)
				break;

		if (ec != SUCCESS)
			terminate(_errmsg(ec, "msg_read() error during msg_read()\n"));
#else
		while (1)
		{
			msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK);
			if (message.ptr.com.type == MSG_TYPE_PTR &&
				message.ptr.subtype == INP_PTR_UP)
				break;
		}
#endif
	
		if (message.ptr.position.x < 100)
			security_menu();

		else if (message.ptr.position.y < 100)
			alarms_menu();

		else 
			messages_menu();
	}
}



/*
 * Function that initializes all needed MAUI APIs and data structures and
 * handles the loading of the images into the drawmap pixel memories
 *
 *	Call with:
 *		Nothing
 *	Return:
 *		Nothing
 */
void initialize (void)
{
#if defined (DEBUG)
	error_code ec;
#endif

	/* initialize MEM API (must be first) */
	{
#if defined (DEBUG)
	 	/* Create needed memory shades */
		if ((ec = mem_init()) != SUCCESS)
			terminate (_errmsg(ec, "Can't initialize memory api\n"));

		if ((ec = mem_create_shade (SHADE_GRAPHICS, MEM_SHADE_NORMAL,
					MEM_ANY,10240, 4096, MEM_OV_ATTACHED, FALSE)) != SUCCESS)
			terminate (_errmsg (ec, "Can't create graphics shade\n"));
#else
	mem_init();
	mem_create_shade (SHADE_GRAPHICS, MEM_SHADE_NORMAL, MEM_ANY,
		10240, 4096, MEM_OV_ATTACHED, FALSE);
#endif
	}

	/* initialize GFX API */
	{
#if defined (DEBUG)
		if ((ec = gfx_init()) != SUCCESS)
			terminate(_errmsg(ec, "Can't initialize graphics api\n"));

		/* Open the graphics device */
		if ((ec = gfx_open_dev(&gfxdev, GRAPHICS_DEV)) != SUCCESS)
			terminate(_errmsg(ec, "Can't open graphics device\n"));

		

        /* Create and initialize GFX_VPORT structure --
         * must be same size as display and start at display location (0,0) */
        if ((ec = gfx_create_vport(&vport, gfxdev, 0, 0, xsize, ysize,GFX_VPORT_FRONT)) != SUCCESS)
	        terminate(_errmsg(ec, "Can't create viewport\n"));

        /* Create GFX_DMAP structure */
        if ((ec = gfx_create_dmap(&dmap, SHADE_GRAPHICS)) != SUCCESS)
            terminate(_errmsg(ec, "Can't create image drawmap\n"));

        /* initialize drawmap elements to this application */
        if ((ec = gfx_set_dmap_size(dmap, GFX_CM_2BIT, xsize, ysize)) != SUCCESS)
            terminate(_errmsg(ec, "Can't set image drawmap size\n"));

        /* allocate drawmap's pixel memory */
        if ((ec = gfx_set_dmap_pixmem(dmap, NULL, SHADE_GRAPHICS, 0)) != SUCCESS)
            terminate(_errmsg(ec, "Can't allocate pixel memory for image drawmap\n"));

         /* no color pallettes (color not supported) */
         dmap->palette = NULL;

        /* associate called dmap to vport and set initial placement on display */
		if ((ec = gfx_set_vport_dmap(vport, dmap, 0, 0)) != SUCCESS)
            terminate(_errmsg(ec, "Can't set viewport dmap\n"));
#else
		gfx_init();
		gfx_open_dev(&gfxdev, GRAPHICS_DEV);
		gfx_create_vport(&vport, gfxdev, 0, 0, xsize, ysize, GFX_VPORT_FRONT);
        gfx_create_dmap(&dmap, SHADE_GRAPHICS);
        gfx_set_dmap_size(dmap, GFX_CM_2BIT, xsize, ysize);
        gfx_set_dmap_pixmem(dmap, NULL, SHADE_GRAPHICS, 0);
        dmap->palette = NULL;
        gfx_set_vport_dmap(vport, dmap, 0, 0);
                                
#endif
	}

/********************************************************/

	/* initialize MSG API */
	{
#if defined (DEBUG)
		/* Create a mailbox */
		if ((ec = msg_init()) != SUCCESS)
			terminate(_errmsg(ec, "Can't initialize message api\n"));
		if ((ec = msg_create_mbox(&mbox, MAILBOX_NAME, 20, sizeof(MESSAGE),MEM_ANY)) != SUCCESS)
			terminate(_errmsg(ec, "Can't create mailbox\n"));
#else
		msg_init();
		msg_create_mbox(&mbox, MAILBOX_NAME, 20, sizeof(MESSAGE), MEM_ANY);
#endif
	}

	/* initialize INP API */
	{
#if defined (DEBUG)
		/* open the input device */
		if ((ec = inp_init()) != SUCCESS)
			terminate(_errmsg(ec, "Can't initialize input api\n"));
		if ((ec = inp_open_dev(&inpdev, mbox, MAILBOX_DEV)) != SUCCESS) {
			msg_close_mbox(mbox);
			terminate(_errmsg(ec, "Can't open input device\n"));
		}
#else
		inp_init();
		inp_open_dev(&inpdev, mbox, MAILBOX_DEV);
#endif
	}

	return;
}



#if defined (DEBUG)

void sighand (int sigcode)
{
	switch (sigcode) {

		case (2):	/* <control>E keyboard abort signal */
		case (3):	/* <control>C keyboard interrupt signal */
			/* abort program */
			terminate (_errmsg(sigcode, "kill signal received\n"));
			break;

		default:
			terminate (_errmsg(sigcode, "Unexpected signal received\n"));
			break;
	}
	return;
}

void terminate (error_code err)
{
	/* Destroy everything, terminate each API, and exit */

	/* deinitialize input & message api */
	inp_close_dev(inpdev);
	inp_term();
	msg_close_mbox(mbox);
	msg_term();

	/* deinitialize graphics api and destroy drawmaps */
	gfx_set_vport_state(vport,FALSE);
	gfx_update_display(gfxdev,FALSE);
	mem_free(dmap->pixmem);

	gfx_destroy_vport(vport);
	gfx_destroy_dmap(dmap);
	gfx_close_dev(gfxdev);
	gfx_term();

	/* deinitialize shaded memory api and destroy shades */
	mem_destroy_shade(SHADE_GRAPHICS);
	mem_term();

	exit(err);
}

#endif


