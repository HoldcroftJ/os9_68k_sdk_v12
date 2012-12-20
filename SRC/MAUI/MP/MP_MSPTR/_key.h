
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
 * EDITION  DATE      WHO  DETAIL
 *       1  01/22/95  srm  create
 */

#define __KEY_C

#include <MAUI/mppm.h>
#include <scf.h>

#ifdef _OSK
typedef struct scf_opt scf_path_opts, *Scf_path_opts;
#endif

/* default status values */
#define DEFAULT_SIM_METH   INP_SIM_PTR
#define DEFAULT_PTR_MIN_X  GFX_POS_MIN
#define DEFAULT_PTR_MIN_Y  GFX_POS_MIN
#define DEFAULT_PTR_MAX_X  GFX_POS_MAX
#define DEFAULT_PTR_MAX_Y  GFX_POS_MAX
#define DEFAULT_SPEED_X    1
#define DEFAULT_SPEED_Y    1

/* default device capabilities values */
#define DEV_CAP_PTR_TYPE      INP_CLASS_MOUSE
#define DEV_CAP_PTR_BUTTONS   2
#define DEV_CAP_FUNC_KEYS     0
#define DEV_CAP_KEY_MODIFIERS 0
#define DEV_CAP_KEY_CAP       0

#define NUM_IMSG    (DEV_CAP_PTR_BUTTONS+1)
#define NUM_PKT_BUF 3
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
	scf_path_opts  opts_old;            /* Store initial path options */
#ifdef _OS9000
	scf_lu_opts    l_opts_old;          /* Store initial logical unit options */
#endif
} PMEM;
