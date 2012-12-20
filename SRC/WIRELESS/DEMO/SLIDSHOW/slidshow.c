/*****************************************************************************
** Demo program for Wireless Beta Package
** For use with Alps 240x160 display, or Sharp 640X480 display: 2 bit images
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
**   3 03/25/96 modification for embedded systems demo dvlpmnt    rws       **
**              for MPC821 target                                           **
**   4 04/22/96 modified for new mauidrvr -- ensured dmap->pixmem rws       **
**              is on a 16-byte boundary                                    **
**	 5 05/01/96 modified to handle either ALPS or SHARP displays  pbt		**
**	 6 05/08/96 "intro" screen removed from display sequence	  pbt		**
**	7  05/10/96 device capabilities determined with gfx_get_dev   pbt		**
**				_cap function in order to allow program to handle			**
**				a variety of displays										**
**	 8 05/17/96 removed inclusion of mc328ads.h file since demo	  pbt		**
				no longer includes touchpad									**
*****************************************************************************/

/*
** Header Files
*/

#include <signal.h>
#include <types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <memory.h>
#include <modes.h>
#include <sg_codes.h>
#include "load.h"
#include "usr_cmds.h"

#include <MAUI/maui_mem.h>
#include <MAUI/maui_gfx.h>

/*
** Local Definitions
*/

/* colored memory shade ID definitions */
#define SHADE_GRAPHICS	3

#define GRAPHICS_DEV "/gfx"


#define TRUE 1
#define FALSE 0

/*
** Global Variables
*/

GFX_DEV_ID gfxdev = 0;		/* Graphics device ID */
GFX_VPORT_ID vport = 0;		/* Viewport ID */ 
GFX_DMAP *dmap = NULL;		/* Drawmap Pointer */
const GFX_DEV_CAP *dev_cap = NULL;  /* GFX Device capabilities */
u_int32 xsize, ysize;           /* size of display vars */

#if defined(_OSK)
#define _os_sleep(t,s) _os9_sleep(t)
#endif

#if defined (_MPFPPC821)
GFX_PIXEL *dmap_pixmem_org;	/* for mem_free() usage */
#endif

/* Pointers to OS9 data module containing image -- used by load_image() */
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

void main (int argc, char *argv[]);
void initialize (void);

void sighand (int sigcode);
extern void intercept (void (*sighand)());
extern int _errmsg (int nerr, char *msg, ...);
void terminate (error_code err);



/*
** Program Entry Point
*/
void main (int argc, char *argv[])
{
    u_int32  *usr_sel;  /* pointer to user selections */
    u_int32 tk_rate;    /* tick rate */
    u_int32 halt_img;   /* image number to halt on */
    u_int32 ticks[12];  /* array to store tick values for each image */
    u_int32 temp[13];	/* temporary array to store tick values	*/
    u_int32 y;      	/* loop variables */
	u_int16 loop=0;		/* variable to continue looping in while loop */
	error_code ec;

	/* Setup a signal handler function so that when we exit the program
	 * we can clean up properly */ 
	intercept (sighand);

	/*
	 * get user options from command line and call usr_cmds function
	 * to verify
	 */

	usr_sel = usr_cmds(argc, argv[1]);

	/* confirmation of results from usr_cmds */
	if(usr_sel == NULL)
	{
		terminate(_errmsg(EOS_ILLARG, "Incorrect usage occurred\n"));
	}
	else
	{
		/* determine the last image to be displayed and the tk_rate */

		tk_rate = *usr_sel;
		halt_img = *(usr_sel+1);


		/* assign while loop variable to enable loop entry */
		loop = 1;

		/* set the image halting and tick rate accordingly */
	
		if(halt_img==0)	/* recursive display requested */
    	{
			
		/* assign tick rate for each image	*/
	
    		for(y=1; y<=11; y++)
    		{
    			ticks[y] = tk_rate;
    		}
    	}	
		else /* check for image number to halt on */
		{

			/* set the tick rate for all the images to be displayed */

			for(y=1; y<halt_img; y++)
			{
				ticks[y] = tk_rate;
			}	
    		/* assign a tick rate of zero for the last image */
    			ticks[halt_img] = 0;
		}
	}

	/*
	 * initialize MAUI APIs and data structures and load images
	 * into pixel memories
	 */
	initialize();

    /* show the viewport */
    if ((ec = gfx_set_vport_state(vport, TRUE)) != SUCCESS)
	    terminate(_errmsg(ec, "Can't set viewport state\n"));
    if ((ec = gfx_update_display(gfxdev, FALSE)) != SUCCESS)
		terminate(_errmsg(ec, "Can't update display\n"));

	while (loop)
	{
        if ((ec = load_image (image_name_slide, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
		temp[1] = ticks[1]; _os_sleep (&temp[1], &sg_code);

        if ((ec = load_image (image_name_main, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
 		temp[2] = ticks[2]; _os_sleep (&temp[2], &sg_code);

        if ((ec = load_image (image_name_s_1, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
 		temp[3] = ticks[3]; _os_sleep (&temp[3], &sg_code);

        if ((ec = load_image (image_name_s_e1, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
 		temp[4] = ticks[4]; _os_sleep (&temp[4], &sg_code);

        if ((ec = load_image (image_name_s_e2, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
 		temp[5] = ticks[5]; _os_sleep (&temp[5], &sg_code);

        if ((ec = load_image (image_name_s_e3, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
 		temp[6] = ticks[6]; _os_sleep (&temp[6], &sg_code);

        if ((ec = load_image (image_name_s_e4, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
 		temp[7] = ticks[7]; _os_sleep (&temp[7], &sg_code);

        if ((ec = load_image (image_name_a_f, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
 		temp[8] = ticks[8]; _os_sleep (&temp[8], &sg_code);

        if ((ec = load_image (image_name_a_w, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
 		temp[9] = ticks[9]; _os_sleep (&temp[9], &sg_code);

        if ((ec = load_image (image_name_m_1, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
 		temp[10] = ticks[10]; _os_sleep (&temp[10], &sg_code);

        if ((ec = load_image (image_name_m_2, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
 		temp[11] = ticks[11]; _os_sleep (&temp[11], &sg_code);
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
	error_code ec;

	/* initialize MEM API (must be first) */
	{
	 	/* Create needed memory shades */
		if ((ec = mem_init()) != SUCCESS)
			terminate (_errmsg(ec, "Can't initialize memory api\n"));

		if ((ec = mem_create_shade (SHADE_GRAPHICS, MEM_SHADE_NORMAL,
					MEM_ANY,10240, 4096, MEM_OV_ATTACHED, FALSE)) != SUCCESS)
			terminate (_errmsg (ec, "Can't create graphics shade\n"));
	}

	/* initialize GFX API */
	{
		if ((ec = gfx_init()) != SUCCESS)
			terminate(_errmsg(ec, "Can't initialize graphics api\n"));

		/* Open the graphics device */
		if ((ec = gfx_open_dev(&gfxdev, GRAPHICS_DEV)) != SUCCESS)
			terminate(_errmsg(ec, "Can't open graphics device\n"));

		/* get Device Capabilities */
		if ((ec = gfx_get_dev_cap (&dev_cap, gfxdev)) != SUCCESS)
			terminate(_errmsg(ec, "Can't get device capabilities\n"));
		else
		{
			xsize = dev_cap->res_info->disp_width;
			ysize = dev_cap->res_info->disp_height;
		}
			
        /* Create and initialize GFX_VPORT structure --
         * must be same size as display and start at display location (0,0) */
        if ((ec = gfx_create_vport(&vport, gfxdev, 0, 0, xsize, ysize, GFX_VPORT_FRONT)) != SUCCESS)
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

#if defined (_MPFPPC821)
		dmap_pixmem_org = dmap->pixmem;	/* save for mfree() */
		{
			/* ensure pixmem is on a 16-byte boundary */
			u_int32 adjustment, mask;
			adjustment = (u_int32) (dmap->pixmem) % 16;
			if (adjustment != 0)
			{
				mask = (u_int32)dmap->pixmem;
				mask = (mask & 0xFFFFFFF0) + 0x10;
				dmap->pixmem = (u_int32 *)mask;
			}
		}
#endif

        /* no color pallettes (color not supported) */
        dmap->palette = NULL;

	    /* associate called dmap to vport and set initial placement on display */
    	if ((ec = gfx_set_vport_dmap(vport, dmap, 0, 0)) != SUCCESS)
    		terminate(_errmsg(ec, "Can't set viewport dmap\n"));
	}

	return;
}

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
	/* Exit directly if incorrect usage has occurred */
	if(err != EOS_ILLARG)
	{
		/* Destroy everything, terminate each API, and exit */
		/* deinitialize graphics api and destroy drawmaps */
		gfx_set_vport_state(vport,FALSE);
		gfx_update_display(gfxdev,FALSE);

#if defined (_MPFPPC821)
		dmap->pixmem = dmap_pixmem_org;
#endif
		mem_free(dmap->pixmem);

		gfx_destroy_vport(vport);
		gfx_destroy_dmap(dmap);
		gfx_close_dev(gfxdev);
		gfx_term();

		/* deinitialize shaded memory api and destroy shades */
		mem_destroy_shade(SHADE_GRAPHICS);
		mem_term();
	}
	exit(err);
}

