
/*******************************************************************************
 * Filename : _key.c
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
 *       1  09/01/95   srm  modified from GENREM
 *       2  10/18/01   and  CF2898: Added support for vt100 function keys
 */

#define __KEY_C

#include <MAUI/mppm.h>

#define MAX_RESERVES 101 /* Physically present on the keyboard */

/* default status values */
#define DEFAULT_SIM_METH   INP_SIM_KEY
#define DEFAULT_PTR_MIN_X  GFX_POS_MIN
#define DEFAULT_PTR_MIN_Y  GFX_POS_MIN
#define DEFAULT_PTR_MAX_X  GFX_POS_MAX
#define DEFAULT_PTR_MAX_Y  GFX_POS_MAX
#define DEFAULT_SPEED_X    1
#define DEFAULT_SPEED_Y    1

/* default device capabilities values */
#define DEV_CAP_PTR_TYPE      INP_CLASS_KEYBOARD
#define DEV_CAP_PTR_BUTTONS   0
#define DEV_CAP_FUNC_KEYS     0
#define DEV_CAP_KEY_MODIFIERS 0
#define DEV_CAP_KEY_CAP       (INP_KEYS_ASCII|INP_KEYS_UCASE|INP_KEYS_LCASE|INP_KEYS_NUM|INP_KEYS_4WAY)

typedef struct _RESERVE {
  wchar_t keysym;
  MP_DEV  *mp_dev;
} RESERVE;

#define NUM_PKT_BUF 5     /* the mp_kybrd's procdata.c has a 5 element
							 switch statment */

typedef struct _PMEM {
  INP_MSG        imsg;    /* application key and ptr messages */
  MP_DEV_MSG     xmsg;    /* device reply message buffer, composit messages here */
  INP_DEV_STATUS status;  /* local status buffer */
  RESERVE    reserve[MAX_RESERVES]; /* key reserve table */
  wchar_t    num_reserve;
  u_char     pktbuf[NUM_PKT_BUF];   /* buffer incomplete packets */
  int16      pktcnt;                /* count of characters in keybuf */
  u_int16    but_up_flag;           /* button unfinished flag */
} PMEM;
