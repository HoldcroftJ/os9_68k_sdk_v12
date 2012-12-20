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
**   2 08/17/96 added graphics descriptor command line arg        rws       **
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

#include <MAUI/maui_mem.h>
#include <MAUI/maui_gfx.h>
#include <MAUI/maui_blt.h>
#include <MAUI/maui_drw.h>

/*
** Local Definitions
*/

/* colored memory shade ID definitions */
#define SHADE_GRAPHICS	3

char *GRAPHICS_DEV;

#if defined (ELMO_TEST)
#define BRIGHTNESS_CMD		0x81
#define CONTRAST_CMD		0x91
#define PIC_RESPONSE		0x06
#define DIGITIZER_OFF_CMD	0x13
#define DIGITIZER_ON_CMD	0x14
#define GPIO_STATUS			0x20
path_id t2_path;
char buff[5];
char inp;
u_int32 count;
u_int32 brightness, contrast;
#else
char buff[1];
char inp;
#endif

/*
** Global Variables
*/

GFX_DEV_ID gfxdev = 0;			/* Graphics device ID */
GFX_DMAP *srcdmap = NULL;		/* Source Drawmap pointer, used for erasing dstdmap  */
GFX_DMAP *dstdmap = NULL;		/* destination Drawmap pointer, always viewed in vport */
#if defined (MPC821)	
GFX_PIXEL *sdmap_pixmem_org;	/* for use in mem_free() */
GFX_PIXEL *ddmap_pixmem_org;
#endif
GFX_VPORT_ID vport = 0;			/* Viewport ID */ 
BLT_CONTEXT_ID blt_context = 0; /* BLT context ID */
DRW_CONTEXT_ID drw_ctx = 0;		/* Draw context ID */
const GFX_DEV_CAP *dev_cap = NULL; 	/* GFX Device capabilities */
u_int32 xsize, ysize;	/* size of didplay vars */
u_int32 MAXPIX;			/* inferred from command line argument */

/*
** Function Prototypes
*/

void main(int argc, char *argv[]);
error_code initialize(void);
void sighand(int sigcode);
void terminate(error_code err);
void display_image(char *image_name);

extern int _errmsg(int nerr, char *msg, ...);
extern void intercept (void (*sighand)());
extern error_code _os_getstat (path_id path, u_int32 code, void *pb);
extern error_code _os_setstat (path_id path, u_int32 code, void *pb);
extern int sleep(int);


/*
** Program Entry Point
**
**	command line call:
**
**		draw <gfx_descriptor_name> N
**
** where N = 1, 2, or 4 for #bits/pixel
*/
void main(int argc, char *argv[])
{
	error_code ec;

	if (argc != 3)
	{
usage:
		printf ("\n\nUsage:\n\n");
		printf ("draw <gfx_descriptor_name> N\n\n");
		printf ("where N = #bits per pixel (1, 2, or 4)\n");
		printf ("e.g.:    draw /gfx 2\n\n");
		exit (1);
	}
	else
	{
		GRAPHICS_DEV = argv[1];
	}

	if (strncmp (argv[2], "1", 2) != 0 &&
		strncmp (argv[2], "2", 2) != 0 &&
		strncmp (argv[2], "4", 2) != 0)
	{
		goto usage;
	}

	switch (atoi(argv[2]))
	{
		case(1):
			MAXPIX = 1;
			break;
		case(2):
			MAXPIX = 3;
			break;
		case(4):
			MAXPIX = 15;
			break;
		default:
			goto usage;
	}

#if defined (DEBUG)
	printf ("MAXPIX = %d\n\n", MAXPIX);
#endif

#if defined (ELMO_TEST)

    /* open path to PIC16LC65 for control of LCD's Brightness and Contrast PWMs */
   	if ((ec = _os_open ("/t2", FAM_READ | FAM_WRITE, &t2_path)) != SUCCESS)
		terminate(_errmsg(ec, "Can't open t2_path\n"));

#if defined (DEBUG)
	printf ("t2_path = %d\n", t2_path);
#endif

	/* turn off digitizer */
	buff[0] = DIGITIZER_OFF_CMD;
	count = 1;
	if ((ec = _os_write (t2_path, buff, &count)) != SUCCESS)
		terminate(_errmsg(ec, "Can't turn off digitizer\n"));
	if (count != 1) 
		terminate(_errmsg(count, "1 byte not written to t2\n"));

#if defined (DEBUG)
	printf("DIGITIZER_OFF_CMD sent, now waiting for PIC_RESPONSE\n");
#endif

	/* get response */
	count = 1;
	if ((ec = _os_read (t2_path, buff, &count)) != SUCCESS)
		terminate(_errmsg(ec, "Can't read t2\n"));
	if (buff[0] != PIC_RESPONSE)
		terminate(_errmsg(buff[0], "incorrect PIC response\n"));

#if defined (DEBUG)
	printf ("PIC_REPSONSE received, Digitizer OFF\n\n");
#endif

#if defined (DEBUG)

	/* get GPIO status */
	buff[0] = GPIO_STATUS;
	count = 1;
	if ((ec = _os_write (t2_path, buff, &count)) != SUCCESS)
		terminate(_errmsg(ec, "Can't get GPIO Status\n"));
	if (count != 1) 
		terminate(_errmsg(count, "1 byte not written to t2\n"));

	printf("GPIO_STATUS sent, now waiting for PIC_RESPONSE + Status\n");

	/* get response */
	count = 4;
	if ((ec = _os_read (t2_path, buff, &count)) != SUCCESS)
		terminate(_errmsg(ec, "Can't read t2\n"));
	if (buff[0] != PIC_RESPONSE)
		terminate(_errmsg(buff[0], "incorrect PIC response\n"));

	printf ("PIC_REPSONSE received\nGPIO Status:");
	printf ("Identifier = 0x%02x\nPM_GPIO_0 = 0x%02x\nPM_GPIO_1 = 0x%02x\n\n",
		buff[1] & 0xFF, buff[2] & 0xFF, buff[3] & 0xFF);

#endif	/* DEBUG */

#endif	/* ELMO_TEST */

	/*
	 * Setup a signal handler function so that we can clean up properly
	 * if we receive a signal.
	 */ 
	intercept(sighand);
   
	if ((ec = initialize()) != SUCCESS)
		terminate(ec);

#if defined (ELMO_TEST)

	/* turn off digitizer (dr_init_hw() turned it back on) */
	buff[0] = DIGITIZER_OFF_CMD;
	count = 1;
	if ((ec = _os_write (t2_path, buff, &count)) != SUCCESS)
		terminate(_errmsg(ec, "Can't turn off digitizer\n"));
	if (count != 1) 
		terminate(_errmsg(count, "1 byte not written to t2\n"));

#if defined (DEBUG)
	printf("DIGITIZER_OFF_CMD sent, now waiting for PIC_RESPONSE\n");
#endif

	/* get response */
	count = 1;
	if ((ec = _os_read (t2_path, buff, &count)) != SUCCESS)
		terminate(_errmsg(ec, "Can't read t2\n"));
	if (buff[0] != PIC_RESPONSE)
		terminate(_errmsg(buff[0], "incorrect PIC response\n"));

#if defined (DEBUG)
	printf ("PIC_REPSONSE received, Digitizer OFF\n\n");
#endif

#endif	/* ELMO_TEST */

	/* erase screen */
/*	if ((ec = blt_copy_block (blt_context, 0, 0, 0, 0, xsize, ysize)) != SUCCESS)
		terminate(_errmsg(ec, "cannot erase screen\n"));
*/
   
	/*
	 * Draw box just inside border
	 */
    drw_set_context_pix(drw_ctx,MAXPIX);
	if ((ec = drw_set_context_fm(drw_ctx, DRW_FM_OUTLINE)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set drw context fill mode\n"));
/*	if ((ec = drw_rectangle (drw_ctx, 0, 0, xsize, ysize)) != SUCCESS) */
	if ((ec = drw_rectangle (drw_ctx, 5, 5, xsize-10, ysize-10)) != SUCCESS) 
		terminate(_errmsg(ec, "Can't draw border\n"));

#if defined DEBUG
	printf("Border drawn\n");
	printf("Press <return> to continue\n");
	scanf ("%c", &inp);
#endif

	/*
	 * draw first bullseye diagram at coordinates (30, 30).
	 */
    drw_set_context_pix(drw_ctx,MAXPIX);
	if ((ec = drw_set_context_fm(drw_ctx, DRW_FM_SOLID)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set drw context fill mode\n"));
	if ((ec = drw_circle (drw_ctx, 30, 30, 3)) != SUCCESS)
		terminate(_errmsg(ec, "Can't draw circle (30, 30, 3)\n"));
	if ((ec = drw_set_context_fm(drw_ctx, DRW_FM_OUTLINE)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set drw context fill mode\n"));
	if ((ec = drw_circle (drw_ctx, 30, 30, 20)) != SUCCESS)
		terminate(_errmsg(ec, "Can't draw circle (30, 30, 20)\n"));

#if defined DEBUG
	printf("Upper left bullseye drawn\n");
	printf("Press <return> to continue\n");
	scanf ("%c", &inp);
#endif

	/*
	 * draw second bullseye diagram at coordinates (xsize-60, ysize-60).
	 */
	if ((ec = drw_set_context_fm(drw_ctx, DRW_FM_SOLID)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set drw context fill mode\n"));
	if ((ec = drw_circle (drw_ctx, xsize-60, ysize-60, 3)) != SUCCESS)
		terminate(_errmsg(ec, "Can't draw circle (xsize-60, ysize-60, 3)\n"));
	if ((ec = drw_set_context_fm(drw_ctx, DRW_FM_OUTLINE)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set drw context fill mode\n"));
	if ((ec = drw_circle (drw_ctx, xsize-60, ysize-60, 40)) != SUCCESS)
		terminate(_errmsg(ec, "Can't draw circle (xsize-60-1, ysize-60-1, 40)\n"));

#if defined DEBUG
	printf("lower right bullseye drawn\n");
	printf("Press <return> to continue\n");
	scanf ("%c", &inp);
#endif

	/* draw some shaded boxes */
	if ((ec = drw_set_context_fm(drw_ctx, DRW_FM_SOLID)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set drw context fill mode\n"));

    drw_set_context_pix(drw_ctx,0);	/* should make blank screen area */
	if ((ec = drw_rectangle (drw_ctx, 0, 0, xsize/4, ysize/4)) != SUCCESS)
		terminate(_errmsg(ec, "Can't draw border\n"));

	drw_set_context_pix(drw_ctx,1);
	drw_rectangle (drw_ctx, xsize/4, 0, xsize/2, ysize/4);

	if (MAXPIX == 3 || MAXPIX == 15)
	{
		drw_set_context_pix(drw_ctx,2);
		drw_rectangle (drw_ctx, xsize/2, 0, 3*xsize/4, ysize/4);

		drw_set_context_pix(drw_ctx,3);
		drw_rectangle (drw_ctx, 3*xsize/4, 0, xsize-1, ysize/4);
	}

	if (MAXPIX == 15)
	{
		drw_set_context_pix(drw_ctx,4);
		drw_rectangle (drw_ctx, 0, ysize/4, xsize/4, ysize/2);

		drw_set_context_pix(drw_ctx,5);
		drw_rectangle (drw_ctx, xsize/4, ysize/4, xsize/2, ysize/2);

		drw_set_context_pix(drw_ctx,6);
		drw_rectangle (drw_ctx, xsize/2, ysize/4, 3*xsize/4, ysize/2);

		drw_set_context_pix(drw_ctx,7);
		drw_rectangle (drw_ctx, 3*xsize/4, ysize/4, xsize-1, ysize/2);

		drw_set_context_pix(drw_ctx,8);
		drw_rectangle (drw_ctx, 0, ysize/2, xsize/4, 3*ysize/4);

		drw_set_context_pix(drw_ctx,9);
		drw_rectangle (drw_ctx, xsize/4, ysize/2, xsize/2, 3*ysize/4);

		drw_set_context_pix(drw_ctx,10);
		drw_rectangle (drw_ctx, xsize/2, ysize/2, 3*xsize/4, 3*ysize/4);

		drw_set_context_pix(drw_ctx,11);
		drw_rectangle (drw_ctx, 3*xsize/4, ysize/2, xsize-1, 3*ysize/4);

		drw_set_context_pix(drw_ctx,12);
		drw_rectangle (drw_ctx, 0, 3*ysize/4, xsize/4, ysize-1);

		drw_set_context_pix(drw_ctx,13);
		drw_rectangle (drw_ctx, xsize/4, 3*ysize/4, xsize/2, ysize-1);

		drw_set_context_pix(drw_ctx,14);
		drw_rectangle (drw_ctx, xsize/2, 3*ysize/4, 3*xsize/4, ysize-1);

		drw_set_context_pix(drw_ctx,15);
		drw_rectangle (drw_ctx, 3*xsize/4, 3*ysize/4, xsize-1, ysize-1);
	}

#if defined (ELMO_TEST)

	/* brightness/contrast # lower => greater brightness/contrast */
	brightness = 0x28;	/* 100% => dimmest value */
	contrast = 0x0b;	

#if defined (DEBUG)

	/* get GPIO status */
	buff[0] = GPIO_STATUS;
	count = 1;
	if ((ec = _os_write (t2_path, buff, &count)) != SUCCESS)
		terminate(_errmsg(ec, "Can't get GPIO Status\n"));
	if (count != 1) 
		terminate(_errmsg(count, "1 byte not written to t2\n"));

	printf("GPIO_STATUS sent, now waiting for PIC_RESPONSE + Status\n");

	/* get response */
	count = 5;	/* this is AFU!! */
	if ((ec = _os_read (t2_path, buff, &count)) != SUCCESS)
		terminate(_errmsg(ec, "Can't read t2\n"));
	if (buff[0] != PIC_RESPONSE)
		terminate(_errmsg(buff[0], "incorrect PIC response\n"));

	printf ("PIC_RESPONSE received\nGPIO Status:");
	printf ("Identifier = 0x%02x\nPM_GPIO_0 = 0x%02x\nPM_GPIO_1 = 0x%02x\n\n",
		buff[2] & 0xFF, buff[3] & 0xFF, buff[4] & 0xFF);

#endif	/* DEBUG */

	while (1)
	{
		printf ("Brightness = 0x%2x, Contrast = 0x%2x\n", brightness, contrast);
		printf ("Type + to incr brightness, - to decr brightness\n");
		printf ("Type c to incr contrast, d to decr contrast\n");
		printf ("Type q to quit\n\n");
		scanf ("%c%c", &inp, &buff[0]);	/* buff[0] will equal \n */
		switch (inp)
		{
			case ('+'):
				if (brightness > 0)
					brightness--;
				buff[0] = BRIGHTNESS_CMD;
				buff[1] = brightness;
				break;

			case ('-'):
				if (brightness < 0x28)
					brightness++;
				buff[0] = BRIGHTNESS_CMD;
				buff[1] = brightness;
				break;

			case ('c'):
				if (contrast > 0)
					contrast--;
				buff[0] = CONTRAST_CMD;
				buff[1] = contrast;
				break;

			case ('d'):
				if (contrast < 0x28)
					contrast++;
				buff[0] = CONTRAST_CMD;
				buff[1] = contrast;
				break;

			case ('q'):
				terminate(SUCCESS);

			default:
				printf ("HamHands!\n");
				continue;
		}

		/* prevent fall thru to _os_write() */
		if (inp == '+' || inp == '-' ||
			inp == 'c' || inp == 'd')
		{
			count = 2;
			if ((ec = _os_write (t2_path, buff, &count)) != SUCCESS)
				terminate(_errmsg(ec, "cannot write to t2\n"));
			if (count != 2)
				terminate(_errmsg(count, "2 bytes not written to t2\n"));
	
			/* get response */
			count = 1;
			if ((ec = _os_read (t2_path, buff, &count)) != SUCCESS)
				terminate(_errmsg(ec, "cannot read from t2\n"));
			if (buff[0] != PIC_RESPONSE)
				terminate(_errmsg(buff[0], "invalid PIC response\n"));

			if (gfx_update_display(gfxdev, FALSE) != SUCCESS)
				terminate(_errmsg(ec, "Can't update display\n"));
		}
	}

#else	/* not ELMO_TEST */

	printf ("Press <return> to quit\n\n");
	scanf ("%c", &inp);	
	terminate(SUCCESS);

#endif	/* ELMO_TEST/not ELMO_TEST */
	
/* this code is currently not used */

	/* wait for 5 seconds */
	sleep(5);

	/* erase screen */
	if ((ec = blt_copy_block (blt_context, 0, 0, 0, 0, xsize, ysize)) != SUCCESS)
		terminate(_errmsg(ec, "cannot erase screen\n"));

	/* draw a few more shapes */
    drw_set_context_pix(drw_ctx,MAXPIX);
    drw_set_context_mix(drw_ctx,BLT_MIX_SXD);
    drw_set_context_fm(drw_ctx,DRW_FM_SOLID);

    drw_circle(drw_ctx,100,100,50);

    drw_set_context_mix(drw_ctx,BLT_MIX_SOD);
    drw_set_context_fm(drw_ctx,DRW_FM_OUTLINE);

    drw_circle(drw_ctx,100,100,50);

    drw_set_context_pix(drw_ctx,2);
    drw_set_context_mix(drw_ctx,BLT_MIX_SXD);
    drw_set_context_fm(drw_ctx,DRW_FM_SOLID);

    drw_rectangle(drw_ctx,40,60,50,50);

    drw_set_context_pix(drw_ctx,1);

    drw_rectangle(drw_ctx,80,90,50,50);
   
	/* wait for 5 seconds */
	sleep(5);

#if 0
	/* swap through some pictures via BitBLT transfers */
	display_image("mainmenu.ras");
	sleep(5);
	display_image("game.ras");
	sleep(5);
	display_image("email.ras");
	sleep(5);
	display_image("textslide.ras");
	sleep(5);
#endif

	/* exit */
	terminate(SUCCESS);
}

error_code initialize(void)
{
	error_code ec;

	/* initialize memory api */
	{
	 	/* Create needed memory shades */
		if ((ec = mem_init()) != SUCCESS)
			return(_errmsg(ec, "Can't initialize memory api\n"));
		if ((ec = mem_create_shade(SHADE_GRAPHICS, MEM_SHADE_NORMAL, MEM_ANY, 
						153600, 4096, MEM_OV_ATTACHED, FALSE)) != SUCCESS) {
			return(_errmsg(ec, "Can't create graphics shade\n"));
		}
	}

	/* initialize graphics api */
	{
		if ((ec = gfx_init()) != SUCCESS)
			return(_errmsg(ec, "Can't initialize graphics api\n"));

		/* Open the graphics device */
		if ((ec = gfx_open_dev(&gfxdev, GRAPHICS_DEV)) != SUCCESS)
			return(_errmsg(ec, "Can't open graphics device\n"));

		/* get Device Capabilities */
		if ((ec = gfx_get_dev_cap (&dev_cap, gfxdev)) != SUCCESS)
			return(_errmsg(ec, "Can't get device capabilities\n"));

		xsize = dev_cap->res_info->disp_width;
		ysize = dev_cap->res_info->disp_height;

		/* Create and initialize GFX_VPORT structure --
		 * must be same size as display and start at display location (0,0) */
		if ((ec = gfx_create_vport(&vport, gfxdev, 0, 0, xsize, ysize,
											GFX_VPORT_FRONT)) != SUCCESS)
			return(_errmsg(ec, "Can't create viewport\n"));

		/* Create & initialize GFX_DMAP structures */
		if ((ec = gfx_create_dmap(&srcdmap, SHADE_GRAPHICS)) != SUCCESS)
			return(_errmsg(ec, "Can't create source drawmap\n"));
		if ((ec = gfx_create_dmap(&dstdmap, SHADE_GRAPHICS)) != SUCCESS)
			return(_errmsg(ec, "Can't create destination drawmap\n"));

		/* initialize drawmap elements to this application */
		switch (MAXPIX)
		{
			case (1):
				if ((ec = gfx_set_dmap_size(srcdmap, GFX_CM_1BIT, xsize, ysize)) != SUCCESS)
					return(_errmsg(ec, "Can't set source drawmap size\n"));
				if ((ec = gfx_set_dmap_size(dstdmap, GFX_CM_1BIT, xsize, ysize)) != SUCCESS)
					return(_errmsg(ec, "Can't set destination drawmap size\n"));
				break;

			case (3):
				if ((ec = gfx_set_dmap_size(srcdmap, GFX_CM_2BIT, xsize, ysize)) != SUCCESS)
					return(_errmsg(ec, "Can't set source drawmap size\n"));
				if ((ec = gfx_set_dmap_size(dstdmap, GFX_CM_2BIT, xsize, ysize)) != SUCCESS)
					return(_errmsg(ec, "Can't set destination drawmap size\n"));
				break;

			case (15):
				if ((ec = gfx_set_dmap_size(srcdmap, GFX_CM_4BIT, xsize, ysize)) != SUCCESS)
					return(_errmsg(ec, "Can't set source drawmap size\n"));
				if ((ec = gfx_set_dmap_size(dstdmap, GFX_CM_4BIT, xsize, ysize)) != SUCCESS)
					return(_errmsg(ec, "Can't set destination drawmap size\n"));
				break;

			default:
				return(_errmsg(MAXPIX, "Bad MAXPIX value\n"));
		}

		/* allocate drawmap's pixel memory */
		if ((ec = gfx_set_dmap_pixmem(srcdmap, NULL, SHADE_GRAPHICS, 0)) != SUCCESS)
			return(_errmsg(ec, "Can't allocate pixel memory for source drawmap\n"));
#if defined (MPC821)
		sdmap_pixmem_org = srcdmap->pixmem;	/* save for mfree() */
		{
			/* ensure pixmem is on a 16-byte boundary */
			u_int32 adjustment, mask;
			adjustment = (u_int32) (srcdmap->pixmem) % 16;
			if (adjustment != 0)
			{
				mask = (u_int32)srcdmap->pixmem;
				mask = (mask & 0xFFFFFFF0) + 0x10;
				srcdmap->pixmem = (u_int32 *)mask;
			}
		}
#endif
		if ((ec = gfx_set_dmap_pixmem(dstdmap, NULL, SHADE_GRAPHICS, 0)) != SUCCESS)
			return(_errmsg(ec, "Can't allocate pixel memory for dest drawmap\n"));
#if defined (MPC821)
		ddmap_pixmem_org = dstdmap->pixmem;	/* save for mfree() */
		{
			/* ensure pixmem is on a 16-byte boundary */
			u_int32 adjustment, mask;
			adjustment = (u_int32) (dstdmap->pixmem) % 16;
			if (adjustment != 0)
			{
				mask = (u_int32)dstdmap->pixmem;
				mask = (mask & 0xFFFFFFF0) + 0x10;
				dstdmap->pixmem = (u_int32 *)mask;
			}
		}
#endif

		/* no color pallettes (color not supported) */
		srcdmap->palette = dstdmap->palette = NULL;

		/* associate dstdmap to vport and set initial placement on display */
		if ((ec = gfx_set_vport_dmap(vport, dstdmap, 0, 0)) != SUCCESS)
			return(_errmsg(ec, "Can't set viewport dmap\n"));

		/* Set display parameters and show the viewport */
		if ((ec = gfx_set_vport_state(vport, TRUE)) != SUCCESS)
			return(_errmsg(ec, "Can't set viewport state\n"));
		if ((ec = gfx_update_display(gfxdev, FALSE)) != SUCCESS)
			return(_errmsg(ec, "Can't update display\n"));
	}

	/* initialize BLT api */
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
		if ((ec = blt_set_context_pix (blt_context, 0)) != SUCCESS)
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
		if ((ec = drw_set_context_pix(drw_ctx, 0)) != SUCCESS)
			return(_errmsg(ec, "Can't set drw context pixel value\n"));
		if ((ec = drw_set_context_mix(drw_ctx, BLT_MIX_REPLACE)) != SUCCESS)
			return(_errmsg(ec, "Can't set drw context mixing mode\n"));
		if ((ec = drw_set_context_fm(drw_ctx, DRW_FM_SOLID)) != SUCCESS)
			return(_errmsg(ec, "Can't set drw context fill mode\n"));
	}

	return(SUCCESS);

}

void terminate(error_code err)
{
	/* Destroy everything, terminate each API, and exit */
#if defined (ELMO_TEST)
	/* turn digitizer back on */
#if defined (DEBUG)
	printf ("Preparing to turn digitizer ON\n");
#endif
	buff[0] = DIGITIZER_ON_CMD;
	count = 1;
	_os_write (t2_path, buff, &count);
#if defined (DEBUG)
	printf ("Digitizer ON command sent\n");
#endif
	_os_close (t2_path);
#endif	/* ELMO_TEST */

	/* deinitialize drawing api */
	drw_destroy_context(drw_ctx);
	drw_term();

	/* deinitialize blt api */
	blt_destroy_context(blt_context);
	blt_term();

	/* deinitialize graphics api and destroy drawmaps */
	gfx_set_vport_state(vport,FALSE);
	gfx_update_display(gfxdev,FALSE);
#if defined (MPC821)
	srcdmap->pixmem = sdmap_pixmem_org;
	dstdmap->pixmem = ddmap_pixmem_org;
#endif
	mem_free(srcdmap->pixmem);
	mem_free(dstdmap->pixmem);
	gfx_destroy_vport(vport);
	gfx_destroy_dmap(srcdmap);
	gfx_destroy_dmap(dstdmap);
	gfx_close_dev(gfxdev);
	gfx_term();

	/* deinitialize shaded memory api and destroy shades */
	mem_destroy_shade(SHADE_GRAPHICS);
	mem_term();

	exit(err);
}

void sighand (int sigcode)
{
	switch (sigcode) {

		case (2):	/* <control>E keyboard abort signal */
			/* abort program */
			terminate (_errmsg(sigcode, "^E signal received\n"));
			break;

		case (3):	/* <control>C keyboard interrupt signal */
			/* abort program */
			terminate (_errmsg(sigcode, "^C signal received\n"));
			break;

#if 0
			/* erase screen */
			if ((ec = blt_copy_block (blt_context, 0, 0, 0, 0, xsize, ysize)) != SUCCESS)
				terminate(_errmsg(ec, "cannot erase screen\n"));
			break;
#endif

		default:
			terminate (_errmsg(sigcode, "Unexpected signal received\n"));
			break;
	}
	return;
}

