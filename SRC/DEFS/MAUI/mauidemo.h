/*******************************************************************************
*
* DESCRIPTION:	Common header for MAUI Examples
*
* COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
*				Reserved. Use and/or distribution subject to terms and
*				conditions of applicable Microware license agreement(s).
*
*/
#ifndef _MAUIDEMO_H
#define _MAUIDEMO_H

#include <MAUI/maui.h>
#include <MAUI/bmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

/*******************************************************************************
* COMMON DEFINITIONS
*******************************************************************************/

#define DEMO_GFX_SHADE 1		/* Default graphics shade. Use this when you */
								/* call demo_create_gfx_shade() */

/*******************************************************************************
* THREAD SUPPORT
*******************************************************************************/

#if defined(_OS9THREAD)

#include <pthread.h>
#include <semaphore.h>

/*** LOCKING ***/

typedef enum {
	DEMO_SUSPEND,               /* allow suspension while semaphore/monitor locked */
	DEMO_UNSUSPEND              /* make unsuspendable before acquisition */
} demo_susp_type;

typedef struct {
	semaphore		sem;        /* ownership semaphore					*/
	process_id		owner;      /* owner id (0 = none)					*/
	u_int32			link;       /* link count (number of locks)			*/
} demo_monitor;


/*** THREAD SPECIFIC DATA ***/

/* demo_getdata psects */
/* psects that require thread specific data */
enum {
  DEMO_TD_DEMO,                     /* MAUI demo library data */
  DEMO_TD_SGUI,                     /* SGUI library data */

  DEMO_NUM_THREAD_SPECIFIC_DATAS    /* number of thread specific data's */
};

void *demo_getdata(u_int32 psect, size_t datasize, void (*initdata)(void *), void (*termdata)(void *));

typedef struct tdata {              /* DEMO_TD_DEMO thread specific data */
  volatile BOOLEAN demo_notdone;    /* setup with demo_set_timeout() */
} demo_tdata;

#define demo_notdone (tdata->demo_notdone)
#define GET_DEMO_TLDATA() demo_tdata *tdata = (demo_tdata *)demo_getdata \
                                      (DEMO_TD_DEMO, sizeof(demo_tdata), NULL, NULL);

#else
#define GET_DEMO_TLDATA()
#ifndef DEMO_MAIN
extern 
#endif
volatile BOOLEAN demo_notdone;      /* setup with demo_set_timeout() */
#endif

/*******************************************************************************
* DEFAULT PALETTE - CCIR VALUES
*******************************************************************************/

#define DEMO_NUM_COLORS 15
typedef enum
{
	DEMO_COLOR_BLACK,
	DEMO_COLOR_LTBLUE,
	DEMO_COLOR_LTGREEN,
	DEMO_COLOR_LTCYAN,
	DEMO_COLOR_LTRED,
	DEMO_COLOR_LTMAGENTA,
	DEMO_COLOR_LTYELLOW,
	DEMO_COLOR_LTGREY,
	DEMO_COLOR_DKBLUE,
	DEMO_COLOR_DKGREEN,
	DEMO_COLOR_DKCYAN,
	DEMO_COLOR_DKRED,
	DEMO_COLOR_DKMAGENTA,
	DEMO_COLOR_DKYELLOW,
	DEMO_COLOR_WHITE
} demo_color;


#ifdef DEMO_MAIN

GFX_RGB demo_colors[] = {
	0x101010,					/*  0 Black */
	0x101080,					/*  1 Light blue */
	0x108010,					/*  2 Light green */
	0x108080,					/*  3 Light cyan */
	0x801010,					/*  4 Light red */
	0x801080,					/*  5 Light magenta */
	0x808010,					/*  6 Light yellow */
	0x808080,					/*  7 Light grey */
	0x1010eb,					/*  8 Dark blue */
	0x10eb10,					/*  9 Dark green */
	0x10ebeb,					/* 10 Dark cyan */
	0xeb1010,					/* 11 Dark red */
	0xeb10eb,					/* 12 Dark magenta */
	0xebeb10,					/* 13 Dark yellow */
	0xebebeb,					/* 14 White */
};

GFX_PALETTE demo_palette = {
	0,							/* Start entry */
	DEMO_NUM_COLORS,			/* Number of colors */
	GFX_COLOR_RGB,				/* Color type */
	demo_colors					/* Pointer to list of colors */
};

#else

extern GFX_RGB demo_colors[];
extern GFX_PALETTE demo_palette;

#endif

/*******************************************************************************
* CURSOR IDs
*******************************************************************************/

#define DEMO_ARROW_CURSOR	1
#define DEMO_PENCIL_CURSOR	2

/*******************************************************************************
* FUNCTION PROTOTYPES MISSING FROM STANDARD HEADER FILES
*******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _OPT_PROTOS
int sleep(unsigned seconds);
int _errmsg(int nerr, char *msg, ...);
#endif

/*******************************************************************************
* PROTOTYPES FOR DEMO FUNCTIONS
*******************************************************************************/

error_code demo_open_gfx_dev(GFX_DEV_ID *ret_gfxdev);
error_code demo_open_inp_dev(INP_DEV_ID *ret_inpdev, MSG_MBOX_ID mbox);
error_code demo_create_gfx_shade(GFX_DEV_ID gfxdev, u_int32 shade_id,
	MEM_SHADE_TYPE *ret_shade_type);
error_code demo_clear_dmap(GFX_DEV_ID gfxdev, GFX_DMAP *dmap, GFX_PIXEL pixval);
error_code demo_load_image(GFX_DMAP **ret_dmap, u_int32 dmap_shade, u_int32
	pixmem_shade, char *image_name);
error_code demo_load_bmpimage(GFX_DMAP **ret_dmap, u_int32 dmap_shade,
                              u_int32 pixmem_shade, char *image_name);
error_code demo_get_ucm_font(TXT_FONT **ret_font, u_int32 shade, char
	*fontname);
error_code demo_release_ucm_font(TXT_FONT *font);
int demo_random(int min, int max);
error_code demo_color_pixval(u_int16 color_index, GFX_CM coding_method,
	GFX_PIXEL *ret_pixval);
void demo_random_rect(GFX_RECT *ret_rect, GFX_POS x, GFX_POS y, GFX_DIMEN
	min_width, GFX_DIMEN min_height, GFX_DIMEN width, GFX_DIMEN height);
void demo_random_point(GFX_POINT *ret_point, GFX_POS x, GFX_POS y, GFX_DIMEN
	width, GFX_DIMEN height);
error_code demo_set_timeout(u_int32 time);
error_code demo_error(error_code ec, char *fmt, ...);
void demo_init();
void demo_term();
#if defined(_OS9THREAD)
void demo_lib_lock(semaphore *sem, demo_susp_type type);
void demo_lib_unlock(semaphore *sem, demo_susp_type type);
void demo_monitor_lock(demo_monitor *m, demo_susp_type type);
void demo_monitor_unlock(demo_monitor *m, demo_susp_type type);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _MAUIDEMO_H */
