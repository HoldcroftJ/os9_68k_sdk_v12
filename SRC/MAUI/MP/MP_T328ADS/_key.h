/*****************************************************************************
** ID:     @(#) _key.h 1.1@(#)
** Date:   1/19/96
******************************************************************************
 *	This file contains definitions and declarations used for accessing the 
 *  Motorola MC68328ADS Development Board's Touchpad's MAUI Process Protocol 
 *  Module
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
**	 1 01/18/96 creation--copied from Whirlwind mppm directory     rws      **
*****************************************************************************/


#define __KEY_C

#include <MAUI/mppm.h>

#define X_PIXELS 240	/* LCD # pixels in X dimension */
#define Y_PIXELS 160	/* LCD # pixels in Y dimension */
#define PKTBUF_PD_BYTE0 0x81    /* Pointer Down code in pktbuf[0] */
#define PKTBUF_PM_BYTE0 0x83    /* Pointer Move code in pktbuf[0] */
#define PKTBUF_PU_BYTE0 0x82    /* Pointer Up code in pktbuf[0] */


/* default status values */
#define DEFAULT_SIM_METH   INP_SIM_NATIVE	/* use native mode (pointer) */
#define DEFAULT_PTR_MIN_X  0
#define DEFAULT_PTR_MIN_Y  0
#define DEFAULT_PTR_MAX_X  (X_PIXELS-1)
#define DEFAULT_PTR_MAX_Y  (Y_PIXELS-1)
#define DEFAULT_SPEED_X    1
#define DEFAULT_SPEED_Y    1

/* default device capabilities values */
#define DEV_CAP_PTR_TYPE      INP_CLASS_TOUCHSCR
#define DEV_CAP_PTR_BUTTONS   1		
#define DEV_CAP_FUNC_KEYS     0
#define DEV_CAP_KEY_MODIFIERS 0
#define DEV_CAP_KEY_CAP       0

/* button definitions */
/* BT1 BT0 = TOUCHPANEL STATE: 01 = Pointer Down, 11 = Pointer Move,
 * 							   10 = Pointer Up,   00 = No Touch (Invalid) */
#define BT1 0x02	/* bit position 1 in pktbuf[0] */
#define BT0 0x01	/* bit position 0 in pktbuf[0] */

#define NUM_PKT_BUF 4

/* this protocol module's static storage memory definition */
typedef struct _PMEM {
  INP_MSG        imsg;      		  /* application ptr message */
  MP_DEV_MSG     xmsg;                /* device reply message buffer,
										 composit messages here */
  INP_DEV_STATUS status;              /* local status buffer */
  u_char         pktbuf[NUM_PKT_BUF]; /* buffer incomplete packets */
  int16          pktcnt;              /* count of characters in keybuf */
} PMEM;

