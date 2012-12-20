/*****************************************************************************
** Demo program to show how to output text to a drawmap in MAUI and         **
** wait for user input.                                                     **
**                                                                          **
** Here are the #ifdef's for this demo:                                     **
**   TEST_2BIT_DRAW                                                         **
**   TEST_2BIT_IMG                                                          **
**   TEST_1BIT_DRAW                                                         **
**   TEST_1BIT_IMG                                                          **
**   TEST_1BIT_TEXT                                                         **
**   TEST_1BIT_ALL                                                          **
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
**   1 03/15/96 creation                                          bat       **
**   2 06/10/96 modified to get device xsize & ysize during init  pbt       **
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

/*#include "mc328ads.h"*/
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

/* #define FONTNAME1 "FONT8X8" */
#define FONTNAME2 "default.fnt"

#ifndef SUCCESS
#define SUCCESS (0)
#endif

typedef union _MESSAGE {			/* Union of message types we care about */
	MSG_PTR ptr;
	MSG_KEY key;
} MESSAGE;


/*
** Global Variables
*/

char* GRAPHICS_DEV = "/gfx";
u_int32 xsize, ysize;           /* size of display vars */
GFX_DEV_ID gfxdev = 0;			/* Graphics device ID */
GFX_DMAP *srcdmap = NULL;		/* Source Drawmap pointer  */
GFX_DMAP *imgsrcdmap = NULL;	/* Source Drawmap pointer  */
GFX_DMAP *dstdmap = NULL;		/* destination Drawmap (in VRAM) pointer */
#if defined (MPC821)	
GFX_PIXEL *sdmap_pixmem_org;	/* for use in mem_free() */
GFX_PIXEL *idmap_pixmem_org;
GFX_PIXEL *ddmap_pixmem_org;
#endif
GFX_VPORT_ID vport = 0;			/* Viewport ID */ 
BLT_CONTEXT_ID blt_ctx = 0;		/* BLT context ID */
DRW_CONTEXT_ID drw_ctx;			/* Draw context ID */
TXT_CONTEXT_ID  txt_ctx;		/* Text API Context */
/* TXT_FONT *font1; */
TXT_FONT *font2;
char *string;					/* Pointer to text string */
size_t len;						/* Length of text string */
GFX_DIMEN pwidth;				/* Pixel width of a text string write */



/*
** Function Prototypes
*/

error_code erasescreen(void);
error_code setdmap(GFX_CM method);
int main(int argc, char *argv[]);
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


/* erase screen */
error_code erasescreen(void)
{
	/*
	** draw a big white rectangle to "erase" anything on the screen
	*/
	drw_set_context_pix(drw_ctx,0);
	drw_set_context_mix(drw_ctx,BLT_MIX_REPLACE);
	drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
	drw_set_context_fm(drw_ctx,DRW_FM_SOLID);
	drw_rectangle(drw_ctx,0,0,xsize,ysize);

	return(SUCCESS);
}

/* set drawmap depth */
error_code setdmap(GFX_CM method)
{
	error_code ec;
	
	/* set display to indicated mode */

	if ((ec = gfx_set_dmap_size(srcdmap, method, xsize, ysize)) != SUCCESS)
		return(_errmsg(ec, "Can't set source drawmap size\n"));
	if ((ec = gfx_set_dmap_size(dstdmap, method, xsize, ysize)) != SUCCESS)
		return(_errmsg(ec, "Can't set destination drawmap size\n"));
	if ((ec = gfx_set_dmap_size(imgsrcdmap, method, xsize, ysize)) != SUCCESS)
		return(_errmsg(ec, "Can't set image drawmap size\n"));
	if ((ec = gfx_set_vport_size(vport, xsize, ysize)) != SUCCESS)
		return(_errmsg(ec, "Can't set viewport size\n"));
	if ((ec = gfx_set_vport_dmap(vport, dstdmap, 0, 0)) != SUCCESS)
		return(_errmsg(ec, "Can't set viewport dmap\n"));
	if ((ec = gfx_update_display(gfxdev, FALSE)) != SUCCESS)
		return(_errmsg(ec, "Can't update display\n"));

	if ((ec = drw_set_context_dst(drw_ctx, dstdmap)) != SUCCESS)
		return(_errmsg(ec, "Can't set drw context dst\n"));

	if ((ec = txt_set_context_dst(txt_ctx, dstdmap)) != SUCCESS)
		return(_errmsg(ec, "Can't set text context dst\n"));

	if ((ec = blt_set_context_dst (blt_ctx, dstdmap)) != SUCCESS)
		return(_errmsg(ec, "Can't set blt_ctx dst\n"));
	if ((ec = blt_set_context_src (blt_ctx, srcdmap)) != SUCCESS)
		return(_errmsg(ec, "Can't set blt_ctx src\n"));

	return(SUCCESS);
}

/*
** Program Entry Point
*/
int main(int argc, char *argv[])
{
	error_code ec;

	/* Setup signal handler so we can clean up properly upon deadly signals */
	intercept(sighand);

	/* parse command line options */
	if (argc != 2) {
		printf("Syntax:   mauidemo <graphics-device>\n");   
		printf("Function: MAUI demo program for 1 & 2 bit graphics\n");
		terminate(EOS_PARAM);
	} else {
		GRAPHICS_DEV = argv[1];
	}

	if ((ec = initialize()) != SUCCESS)
		terminate(ec);

	printf("MAUIDEMO: [%s] (%d x %d)\n",GRAPHICS_DEV,xsize,ysize);

#if defined(TEST_2BIT_DRAW)
	printf("draw some pretty 2-bit geometric shapes...\n");
	{
		/* set display to 2-bit mode */
		setdmap(GFX_CM_2BIT);

		printf("  draw some empty circles (press [return] to continue)...\n");
		drw_set_context_pix(drw_ctx,3);
		drw_set_context_mix(drw_ctx,BLT_MIX_REPLACE);
		drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
		drw_set_context_fm(drw_ctx,DRW_FM_OUTLINE);
		drw_circle(drw_ctx, 30, 30, 20);
		drw_circle(drw_ctx, xsize-31, 30, 20);
		drw_circle(drw_ctx, 30, ysize-31, 20);
		drw_circle(drw_ctx, xsize-31, ysize-31, 20);
		getchar();

		printf("  draw some lighter dased circles (press [return] to continue)...\n");
		drw_set_context_pix(drw_ctx,2);
		drw_set_context_mix(drw_ctx,BLT_MIX_REPLACE);
		drw_set_context_ls(drw_ctx,DRW_LS_DASHED);
		drw_set_context_fm(drw_ctx,DRW_FM_OUTLINE);
		drw_circle(drw_ctx, 30, 30, 15);
		drw_circle(drw_ctx, xsize-31, 30, 15);
		drw_circle(drw_ctx, 30, ysize-31, 15);
		drw_circle(drw_ctx, xsize-31, ysize-31, 15);
		getchar();

		printf("  draw some faint solid circles (press [return] to continue)...\n");
		drw_set_context_pix(drw_ctx,1);
		drw_set_context_mix(drw_ctx,BLT_MIX_REPLACE);
		drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
		drw_set_context_fm(drw_ctx,DRW_FM_SOLID);
		drw_circle(drw_ctx, 30, 30, 10);
		drw_circle(drw_ctx, xsize-31, 30, 10);
		drw_circle(drw_ctx, 30, ysize-31, 10);
		drw_circle(drw_ctx, xsize-31, ysize-31, 10);
		getchar();

		printf("  draw some white points (press [return] to continue)...\n");
		drw_set_context_pix(drw_ctx,0);
		drw_set_context_mix(drw_ctx,BLT_MIX_REPLACE);
		drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
		drw_set_context_fm(drw_ctx,DRW_FM_SOLID);
		drw_point(drw_ctx, 30, 30);
		drw_point(drw_ctx, xsize-31, 30);
		drw_point(drw_ctx, 30, ysize-31);
		drw_point(drw_ctx, xsize-31, ysize-31);
		getchar();

		printf("  draw some X-ORed squares (press [return] to continue)...\n");
		drw_set_context_pix(drw_ctx,3);
		drw_set_context_mix(drw_ctx,BLT_MIX_SXD);
		drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
		drw_set_context_fm(drw_ctx,DRW_FM_SOLID);
		drw_rectangle(drw_ctx, 0, 0, 60, 60);
		drw_rectangle(drw_ctx, xsize-61, 0, xsize-1, 60);
		drw_rectangle(drw_ctx, 0, ysize-61, 60, ysize-1);
		drw_rectangle(drw_ctx, xsize-61, ysize-61, xsize-1, ysize-1);
		getchar();

		printf("  draw some X-ORed squares (press [return] to continue)...\n");
		drw_set_context_pix(drw_ctx,3);
		drw_set_context_mix(drw_ctx,BLT_MIX_SXD);
		drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
		drw_set_context_fm(drw_ctx,DRW_FM_SOLID);
		drw_rectangle(drw_ctx, 0, 0, 60, 60);
		drw_rectangle(drw_ctx, xsize-61, 0, xsize-1, 60);
		drw_rectangle(drw_ctx, 0, ysize-61, 60, ysize-1);
		drw_rectangle(drw_ctx, xsize-61, ysize-61, xsize-1, ysize-1);
		getchar();

		printf("  draw some solid lines (press [return] to continue)...\n");
		drw_set_context_pix(drw_ctx,3);
		drw_set_context_mix(drw_ctx,BLT_MIX_REPLACE);
		drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
		drw_set_context_fm(drw_ctx,DRW_FM_SOLID);
		drw_line(drw_ctx, 30, 30, xsize-31, 30);
		drw_line(drw_ctx, xsize-31, 30, 30, ysize-31);
		drw_line(drw_ctx, 30, ysize-31, xsize-31, ysize-31);
		drw_line(drw_ctx, xsize-31, ysize-31, 30, 30);
		getchar();

		/* erase the screen */
		erasescreen();
	}
#endif /* TEST_2BIT_DRAW */	

#if defined(TEST_2BIT_IMG)
	printf("display some 2-bit images...\n");
	{
		/* set display to 2-bit mode */
		setdmap(GFX_CM_2BIT);

		printf("  display the images (press [return] to continue)...\n");
#if 0
		display_image("mainmenu.img");
		getchar();
#endif
		display_image("email.img");
		getchar();
		display_image("game.img");
		getchar();
		display_image("mail.img");
		getchar();

		/* erase the screen */
		erasescreen();
	}
#endif /* TEST_2BIT_IMG */

#if defined(TEST_1BIT_DRAW)
	printf("draw some pretty 1-bit geometric shapes...\n");
	{
		/* set display to 1-bit mode */
		setdmap(GFX_CM_1BIT);

		printf("  draw some empty circles (press [return] to continue)...\n");
		drw_set_context_pix(drw_ctx,1);
		drw_set_context_mix(drw_ctx,BLT_MIX_REPLACE);
		drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
		drw_set_context_fm(drw_ctx,DRW_FM_OUTLINE);
		drw_circle(drw_ctx, 30, 30, 20);
		drw_circle(drw_ctx, xsize-31, 30, 20);
		drw_circle(drw_ctx, 30, ysize-31, 20);
		drw_circle(drw_ctx, xsize-31, ysize-31, 20);
		getchar();

		printf("  draw some dased circles (press [return] to continue)...\n");
		drw_set_context_pix(drw_ctx,1);
		drw_set_context_mix(drw_ctx,BLT_MIX_REPLACE);
		drw_set_context_ls(drw_ctx,DRW_LS_DASHED);
		drw_set_context_fm(drw_ctx,DRW_FM_OUTLINE);
		drw_circle(drw_ctx, 30, 30, 15);
		drw_circle(drw_ctx, xsize-31, 30, 15);
		drw_circle(drw_ctx, 30, ysize-31, 15);
		drw_circle(drw_ctx, xsize-31, ysize-31, 15);
		getchar();

		printf("  draw some solid circles (press [return] to continue)...\n");
		drw_set_context_pix(drw_ctx,1);
		drw_set_context_mix(drw_ctx,BLT_MIX_REPLACE);
		drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
		drw_set_context_fm(drw_ctx,DRW_FM_SOLID);
		drw_circle(drw_ctx, 30, 30, 10);
		drw_circle(drw_ctx, xsize-31, 30, 10);
		drw_circle(drw_ctx, 30, ysize-31, 10);
		drw_circle(drw_ctx, xsize-31, ysize-31, 10);
		getchar();

		printf("  draw some white points (press [return] to continue)...\n");
		drw_set_context_pix(drw_ctx,0);
		drw_set_context_mix(drw_ctx,BLT_MIX_REPLACE);
		drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
		drw_set_context_fm(drw_ctx,DRW_FM_SOLID);
		drw_point(drw_ctx, 30, 30);
		drw_point(drw_ctx, xsize-31, 30);
		drw_point(drw_ctx, 30, ysize-31);
		drw_point(drw_ctx, xsize-31, ysize-31);
		getchar();

		printf("  draw some X-ORed squares (press [return] to continue)...\n");
		drw_set_context_pix(drw_ctx,1);
		drw_set_context_mix(drw_ctx,BLT_MIX_SXD);
		drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
		drw_set_context_fm(drw_ctx,DRW_FM_SOLID);
		drw_rectangle(drw_ctx, 0, 0, 60, 60);
		drw_rectangle(drw_ctx, xsize-61, 0, xsize-1, 60);
		drw_rectangle(drw_ctx, 0, ysize-61, 60, ysize-1);
		drw_rectangle(drw_ctx, xsize-61, ysize-61, xsize-1, ysize-1);
		getchar();

		printf("  draw some X-ORed squares (press [return] to continue)...\n");
		drw_set_context_pix(drw_ctx,1);
		drw_set_context_mix(drw_ctx,BLT_MIX_SXD);
		drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
		drw_set_context_fm(drw_ctx,DRW_FM_SOLID);
		drw_rectangle(drw_ctx, 0, 0, 60, 60);
		drw_rectangle(drw_ctx, xsize-61, 0, xsize-1, 60);
		drw_rectangle(drw_ctx, 0, ysize-61, 60, ysize-1);
		drw_rectangle(drw_ctx, xsize-61, ysize-61, xsize-1, ysize-1);
		getchar();

		printf("  draw some solid lines (press [return] to continue)...\n");
		drw_set_context_pix(drw_ctx,1);
		drw_set_context_mix(drw_ctx,BLT_MIX_REPLACE);
		drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
		drw_set_context_fm(drw_ctx,DRW_FM_SOLID);
		drw_line(drw_ctx, 30, 30, xsize-31, 30);
		drw_line(drw_ctx, xsize-31, 30, 30, ysize-31);
		drw_line(drw_ctx, 30, ysize-31, xsize-31, ysize-31);
		drw_line(drw_ctx, xsize-31, ysize-31, 30, 30);
		getchar();

		/* erase the screen */
		erasescreen();
	}
#endif /* TEST_1BIT_DRAW */

#if defined(TEST_1BIT_IMG)
	printf("display some 1-bit images...\n");
	{
		/* set display to 1-bit mode */
		setdmap(GFX_CM_1BIT);

		printf("  display the images (press [return] to continue)...\n");
		display_image("pcss.img");
		getchar();

		/* erase the screen */
		erasescreen();
	}
#endif /* TEST_1BIT_IMG */

#if defined(TEST_1BIT_TEXT)
#if 0
	printf("display some small printed 1-bit text...\n");
	{
		/* set display to 1-bit mode */
		setdmap(GFX_CM_1BIT);

		printf("  print something useless in a small font (press [return to continue])...\n");
		if ((ec = txt_set_context_font(txt_ctx, font1)) != SUCCESS)
			terminate(_errmsg(ec, "Can't set text context font1\n"));
		txt_set_context_mix(txt_ctx,BLT_MIX_REPLACE);

		string = "Hello Dude!";
		len = strlen(string);
		txt_draw_mbs(&pwidth, txt_ctx, string, &len, 30, 30, NULL);
		getchar();

		string = "You can call me Al";
		len = strlen(string);
		txt_draw_mbs(&pwidth, txt_ctx, string, &len, 30, 70, NULL);
		getchar();

		string = "Ba-Bye!!!";
		len = strlen(string);
		txt_draw_mbs(&pwidth, txt_ctx, string, &len, 50, 150, NULL);
		getchar();

		/* erase the screen */
		erasescreen();
	}
#endif
	printf("display some large printed 1-bit text...\n");
	{
		/* set display to 1-bit mode */
		setdmap(GFX_CM_1BIT);

		printf("  print something useless in a large font (press [return to continue])...\n");
		if ((ec = txt_set_context_font(txt_ctx, font2)) != SUCCESS)
			terminate(_errmsg(ec, "Can't set text context font2\n"));
		txt_set_context_mix(txt_ctx,BLT_MIX_REPLACE);

		string = "Hello Dude!";
		len = strlen(string);
		txt_draw_mbs(&pwidth, txt_ctx, string, &len, 30, 30, NULL);
		getchar();

		string = "Call me Al";
		len = strlen(string);
		txt_draw_mbs(&pwidth, txt_ctx, string, &len, 30, 70, NULL);
		getchar();

		string = "Ba-Bye!!!";
		len = strlen(string);
		txt_draw_mbs(&pwidth, txt_ctx, string, &len, 50, 150, NULL);
		getchar();

		/* erase the screen */
		erasescreen();
	}
#endif /* TEST_1BIT_TEXT */

#if defined(TEST_1BIT_ALL)
	printf("now, put it all together for 1-bit graphics (press [return] to continue)...\n");
	{
		/* set display to 1-bit mode */
		setdmap(GFX_CM_1BIT);

		display_image("pcss.img");
		getchar();

		drw_set_context_pix(drw_ctx,1);
		drw_set_context_mix(drw_ctx,BLT_MIX_REPLACE);
		drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
		drw_set_context_fm(drw_ctx,DRW_FM_SOLID);
		drw_line(drw_ctx, 0,0,xsize-1,ysize-1);
		drw_line(drw_ctx, xsize-1,0,0,ysize-1);
		getchar();

		drw_set_context_pix(drw_ctx,1);
		drw_set_context_mix(drw_ctx,BLT_MIX_SXD);
		drw_set_context_ls(drw_ctx,DRW_LS_SOLID);
		drw_set_context_fm(drw_ctx,DRW_FM_SOLID);
		drw_circle(drw_ctx, 50, 50, 40);
		drw_circle(drw_ctx, 150, 50, 40);
		drw_circle(drw_ctx, 100, 90, 40);
		getchar();

		if ((ec = txt_set_context_font(txt_ctx, font2)) != SUCCESS)
			terminate(_errmsg(ec, "Can't set text context font2\n"));
		txt_set_context_mix(txt_ctx,BLT_MIX_SXD);
		string = "Sha-ZAM!!!";
		len = strlen(string);
		txt_draw_mbs(&pwidth, txt_ctx, string, &len, 20, 80, NULL);
		getchar();

		/* erase the screen */
		erasescreen();
	}
#endif /* TEST_1BIT_ALL */

	/* exit */
	printf("All Done!\n");
	terminate(0);
}

error_code initialize(void)
{
	error_code ec;
	const GFX_DEV_CAP* devcap;

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
						(((xsize*ysize/8)*2)*3), 4096, MEM_OV_ATTACHED, FALSE)) != SUCCESS) {
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

		/* Get graphics device capabilities */
		if ((ec = gfx_get_dev_cap(&devcap,gfxdev)) != SUCCESS)
			return(_errmsg(ec, "Can't create source drawmap\n"));
		xsize = devcap->res_info[0].disp_width;
		ysize = devcap->res_info[0].disp_height;

		/* Create & initialize GFX_DMAP structures */
		if ((ec = gfx_create_dmap(&srcdmap, SHADE_GRAPHICS)) != SUCCESS)
			return(_errmsg(ec, "Can't create source drawmap\n"));
		if ((ec = gfx_create_dmap(&dstdmap, SHADE_GRAPHICS)) != SUCCESS)
			return(_errmsg(ec, "Can't create destination drawmap\n"));
		if ((ec = gfx_create_dmap(&imgsrcdmap, SHADE_GRAPHICS)) != SUCCESS)
			return(_errmsg(ec, "Can't create image drawmap\n"));

		/* initialize drawmap elements to this application */
		if ((ec = gfx_set_dmap_size(srcdmap, GFX_CM_2BIT, xsize, ysize)) != SUCCESS)
			return(_errmsg(ec, "Can't set source drawmap size\n"));
		if ((ec = gfx_set_dmap_size(dstdmap, GFX_CM_2BIT, xsize, ysize)) != SUCCESS)
			return(_errmsg(ec, "Can't set destination drawmap size\n"));
		if ((ec = gfx_set_dmap_size(imgsrcdmap, GFX_CM_2BIT, xsize, ysize)) != SUCCESS)
			return(_errmsg(ec, "Can't set image drawmap size\n"));

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
		ddmap_pixmem_org = srcdmap->pixmem;	/* save for mfree() */
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
		if ((ec = gfx_set_dmap_pixmem(imgsrcdmap, NULL, SHADE_GRAPHICS, 0)) != SUCCESS)
			return(_errmsg(ec, "Can't allocate pixel memory for image drawmap\n"));
#if defined (MPC821)
		idmap_pixmem_org = imgsrcdmap->pixmem;	/* save for mfree() */
		{
			/* ensure pixmem is on a 16-byte boundary */
			u_int32 adjustment, mask;
			adjustment = (u_int32) (imgsrcdmap->pixmem) % 16;
			if (adjustment != 0)
			{
				mask = (u_int32)imgsrcdmap->pixmem;
				mask = (mask & 0xFFFFFFF0) + 0x10;
				imgsrcdmap->pixmem = (u_int32 *)mask;
			}
		}
#endif

		/* no color pallettes (color not supported) */
		srcdmap->palette = dstdmap->palette = imgsrcdmap->palette = NULL;

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
		if ((ec = blt_create_context (&blt_ctx, gfxdev)) != SUCCESS)
			return(_errmsg(ec, "Can't create BLT context\n"));

		/* Set BLT context parameters */
		if ((ec = blt_set_context_cpymix (blt_ctx, BLT_MIX_REPLACE)) != SUCCESS)
			return(_errmsg(ec, "Can't set blt_ctx cpymix\n"));
		if ((ec = blt_set_context_drwmix (blt_ctx, BLT_MIX_SOD)) != SUCCESS)
			return(_errmsg(ec, "Can't set blt_ctx drwmix\n"));
		if ((ec = blt_set_context_dst (blt_ctx, dstdmap)) != SUCCESS)
			return(_errmsg(ec, "Can't set blt_ctx dst\n"));
		if ((ec = blt_set_context_pix (blt_ctx, 1)) != SUCCESS)
			return(_errmsg(ec, "Can't set blt_ctx pix\n"));
		if ((ec = blt_set_context_src (blt_ctx, srcdmap)) != SUCCESS)
			return(_errmsg(ec, "Can't set blt_ctx src\n"));
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

#if defined (TEST_1BIT_TEXT) || defined (TEST_1BIT_ALL)

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
		if ((ec = txt_create_context(&txt_ctx, gfxdev)) != SUCCESS)
			return(_errmsg(ec, "Can't create txt context\n"));

		/* initialize TXT_CONTEXT_PARAMS data structure members */
		if ((ec = txt_set_context_mix(txt_ctx, BLT_MIX_REPLACE)) != SUCCESS)
			return(_errmsg(ec, "Can't set gfx context copy mixing mode\n"));
		if ((ec = txt_set_context_dst(txt_ctx, dstdmap)) != SUCCESS)
			return(_errmsg(ec, "Can't set text context dst\n"));
	}

#endif	/* TEST_1BIT_TEXT / TEST_1BIT_ALL */
		           
	/* erase screen */
	erasescreen();

	return(SUCCESS);

}

void terminate(error_code err)
{
	/* erase screen */
	erasescreen();

	/* Destroy everything, terminate each API, and exit */

	/* deinitialize text api and unload font */
/*	release_ucm_font(font1);
	txt_destroy_font(font1);
*/
	release_ucm_font(font2);
	txt_destroy_font(font2);
	txt_destroy_context(txt_ctx);
	txt_term();

	/* deinitialize drawing api */
	drw_destroy_context(drw_ctx);
	drw_term();

	/* deinitialize blt api */
	blt_destroy_context(blt_ctx);
	blt_term();

	/* deinitialize graphics api and destroy drawmaps */
	gfx_set_vport_state(vport,FALSE);
	gfx_update_display(gfxdev,FALSE);
#if defined (MPC821)
	srcdmap->pixmem = sdmap_pixmem_org;
	dstdmap->pixmem = ddmap_pixmem_org;
	imgsrcdmap->pixmem = idmap_pixmem_org;
#endif
	mem_free(srcdmap->pixmem);
	mem_free(dstdmap->pixmem);
	mem_free(imgsrcdmap->pixmem);
	gfx_destroy_vport(vport);
	gfx_destroy_dmap(srcdmap);
	gfx_destroy_dmap(dstdmap);
	gfx_destroy_dmap(imgsrcdmap);
	gfx_close_dev(gfxdev);
	gfx_term();

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
	if ((ec = blt_set_context_src (blt_ctx, imgsrcdmap)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set blt_ctx src\n"));
	if ((ec = blt_copy_block (blt_ctx, 0, 0, 0, 0, xsize, ysize)) != SUCCESS)
		terminate (_errmsg(ec, "Can't blt image into dest drawmap\n"));
	if ((ec = blt_set_context_src (blt_ctx, srcdmap)) != SUCCESS)
		terminate(_errmsg(ec, "Can't set blt_ctx src\n"));

	/* turn display on */
	gfx_set_vport_state(vport, TRUE);
	gfx_update_display(gfxdev,FALSE);
}

void sighand (int sigcode)
{
	switch (sigcode) {

		case (SIGQUIT):	/* <control>E keyboard abort signal */
		case (SIGINT):	/* <control>C keyboard interrupt signal */
			/* abort program */
			terminate (_errmsg(sigcode, "^E signal received\n"));
			break;

		default:
			terminate (_errmsg(sigcode, "Unexpected signal received\n"));
			break;
	}
	return;
}
