
/*******************************************************************************
 * FILENAME : _key.c
 *
 * DESCRIPTION :
 *
 *	This file contains functions for accessing MAUI Process Protocol Modules
 *
 * COPYRIGHT:
 *
 *	This source code is the proprietary confidential property of Microware
 *	Systems Corporation, and is provided to licensee solely for documentation
 *	and educational purposes. Reproduction, publication, or distribution in
 *	form to any party other than the licensee is strictly prohibited.
 *
 * EDITION  DATE      WHO      DETAIL
 *       1  11/09/99  gjb,sbh  ported mp_msptr to Hampshire touchscreen
 */

#define __KEY_C

#include <MAUI/mppm.h>
#include <scf.h>

#ifdef _OSK
typedef struct scf_opt scf_path_opts, *Scf_path_opts;
#endif


/*
** Defaults
*/
#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 800
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 600
#endif

#ifndef DEFAULT_CALIBRATION_X
#define DEFAULT_CALIBRATION_X 3600
#endif

#ifndef DEFAULT_CALIBRATION_Y
#define DEFAULT_CALIBRATION_Y 3442
#endif

#ifndef DEFAULT_CALIBRATION_WIDTH
#define DEFAULT_CALIBRATION_WIDTH -3183
#endif

#ifndef DEFAULT_CALIBRATION_HEIGHT
#define DEFAULT_CALIBRATION_HEIGHT -2722
#endif


/* default status values */
#define DEFAULT_SIM_METH   INP_SIM_PTR
#define DEFAULT_PTR_MIN_X  0
#define DEFAULT_PTR_MIN_Y  0
#define DEFAULT_PTR_MAX_X  (SCREEN_WIDTH-1)
#define DEFAULT_PTR_MAX_Y  (SCREEN_WIDTH-1)
#define DEFAULT_SPEED_X    1
#define DEFAULT_SPEED_Y    1

#define NUM_CAL_PTS 8

/* calibration data module structure */
typedef struct {
  u_int32 sync;          /* 'HAM1'  */
  u_int16 x;             /* upper left x */
  u_int16 y;             /* upper left y */

  /* negative width, height indicates switched touchscreen wires.
  ** This situation is handled in the protocol module. */
  int16 width;           /* width of screen in hw pts. */
  int16 height;          /* height of screen in hw pts. */

  u_int16 screen_width;  /* screen width in pixels */
  u_int16 screen_height; /* screen height in pixels */
  int16 xpts[NUM_CAL_PTS];/* hardware calibration points */
  int16 ypts[NUM_CAL_PTS];/* hardware calibration points */
} ModData;

#define HAMPSHIRE_SYNC    ((u_int32)0x48414D31)
#define CALIBRATION_MOD_NAME "hamp.dat"
#define RAW_MODE 0xffff

/* default device capabilities values */
#define DEV_CAP_PTR_TYPE      INP_CLASS_TOUCHSCR
#define DEV_CAP_PTR_BUTTONS   1
#define NUM_PKT_BUF 4
#define SYNC_BYTE_SHIFT 7
#define DEV_CAP_FUNC_KEYS     0
#define DEV_CAP_KEY_MODIFIERS 0
#define DEV_CAP_KEY_CAP       0

#define NUM_IMSG    (DEV_CAP_PTR_BUTTONS+1)
#define OPT_SIZE 1024

typedef struct _PMEM {
	INP_MSG        imsg[NUM_IMSG];      /* application key and ptr messages */
	int16          imsgcnt;             /* count of overflow button messages */
	MP_DEV_MSG     xmsg;                /* device reply message buffer,
										 composit messages here */
	INP_DEV_STATUS status;              /* local status buffer */
	u_char         pktbuf[NUM_PKT_BUF]; /* buffer incomplete packets */
	int16          pktcnt;              /* count of characters in keybuf */
	GFX_POS        last_key_x, last_key_y;/* last X,Y returned in a key message */
	GFX_OFFSET     offset_x, offset_y;  /* horz and vert movement left */

    /* calibration values */
    u_int16        cal_x,               /* calibration values - these */
                   cal_y;               /* determine how to map the driver */
    int16          cal_width,           /* values to the screen.  */
                   cal_height;           
    u_int16        screen_width;        /* screen width in pixels */
    u_int16        screen_height;       /* screen height in pixels */

} PMEM;



void update_calibration(PMEM *pmem);

