/*****************************************************************************
** MC68328-ADS port definitions                                             **
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
** #   Date     Comments                                             By     **
** --- -------- ------------------------------------------------- --------- **
**   1 01/11/96 Creation                                            rws     **
*****************************************************************************/


#ifndef _MC328ADS_H
#define _MC328ADS_H


/* following definitions are used for tpaddrvr setstat/getstat(SS_DCmd) */

/* values for dcmd_struct.command */
#define GET_POINT  0x3  /* set ADC calibration to known pixel point */
#define CALIBRATE  0x4  /* get/set calibration values */

/* also thought about adding commands such that the driver would
   send a signal only upon pointer down, or pointer up, or
   pointer move
*/

typedef struct gfxstruct {
    u_int32 x_pos,      /* x & y position of GET_POINT call */
            y_pos;
} gfx_struct;

typedef struct calstruct {
    u_short ss_X_min,        /* calibration data used by driver */
            ss_Y_min,        /* put ss_ prefix on names so as not to */
            ss_X_max,        /* conflict with STATIC_SHORTCUT names */
            ss_Y_max;        /* (see wirlwind.h) */
} cal_struct;

typedef struct dcmdstruct {
    u_int8  command;
    union {
        gfx_struct gfx_point;
        cal_struct cal_data;
    } ss_specific;
} dcmd_struct;




#endif /* _MC328ADS_H */


