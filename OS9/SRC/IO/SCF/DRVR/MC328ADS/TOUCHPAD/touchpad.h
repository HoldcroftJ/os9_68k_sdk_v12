/*****************************************************************************
** ID:     @(#) touchpad.h 1.6@(#)
** Date:   3/23/96
******************************************************************************
 * definitions for MC68328ADS Evalution Board's touchpanel device
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
**	 1 09/13/95 creation                                          rws   **
**       2 05/17/99 included alarm.h and stdlib.h                     djc   **
*****************************************************************************/


#ifndef _TOUCHPAD_H
#define _TOUCHPAD_H


#include    <scf.h> 
#include    <errno.h> 
#include    <sg_codes.h> 
#include    <MACHINE/reg.h> 
#include    <procid.h> 
#include    <path.h> 
#include    <stddef.h> 
#include 	<const.h> 
#include    <types.h> 
#include    <sysglob.h>
#include    <signal.h>
#include    <module.h>

#include	<mc328ads.h>		/* in port's DEFS directory */
#include    <mc68328.h>		
#include    <alarm.h>
#include    <stdlib.h>


/* 
 *	Hardware Definitions 
 */

/* 
 * Touchpanel Description:
 *
 * Pins on Port J, K, and M are used to interface to the touchpanel:
 * _Port_ _function_
 *  PJ0   Power Switching
 *  PJ1   Power Switching
 *  PJ2   Power Switching
 *  PJ3   Power Switching
 *  PJ7   A/D Chip Select
 *  PK0   Port alternate function usage: SPIM TxD
 *  PK1   Port alternate function usage: SPIM RxD
 *  PK2   Port alternate function usage: SPIM CLK0
 *  PM6   Port alternate function usage: PENIRQn
 *
 *                       Vcc
 *                       /
 *   PJ0------res------Q6                 
 *                       \                
 *                        +-----X (AN1)   
 *                        |                   ______
 *                        +-------------------PENIRQ (PM6)
 *                        |
 *               Vcc     res
 *               /        |
 *   PJ2--res--Q5         |
 *               \        |
 *   Y (AN0)--------res---+---res---\
 *                        |          Q3-------PJ3
 *                        |         /
 *                       res       |
 *                        |       com
 *                        |
 *                         \      
 *                          Q4----------------PJ1
 *                         /
 *                        |
 *                       com
 *  _________
 * |   A/D   |
 * |         |
 * |      AN1|---X 
 * |         |
 * |      AN0|---Y
 * |       __|
 * |       CS|---PJ7
 * |         |
 * |     DOUT|---SPIM RxD (PK1)
 * |         |
 * |      DIN|---SPIM TxD (PK0)
 * |         |
 * |     SCLK|---SPIM CLK0 (PK2)
 * |         |
 * |    ADCLK|---ADCLK = CLK0 / 8 = 16.384 MHz / 8 = 2.048 MHz
 * |_________|
 *
 * While waiting for an interrupt, Q3 is ON, and Q5, Q4, and Q6 are OFF,
 * PJ[3:0] = 1101, and PM6 is configured as PENIRQn with pull-up
 * resistor enabled.  Upon getting an interrupt, the interrupt service 
 * routine must first disable the PENIRQn pullup resistor.
 *
 * To read the X-coordinate, Q3 and Q5 are ON and Q4 and Q6 are OFF,
 * PJ[3:0] = 1001, and AN1 is read from the A/D.
 *
 * To read the Y-coordinate, Q3 and Q5 are OFF and Q4 and Q6 are ON, 
 * PJ[3:0] = 0110, and AN0 is read from the A/D.
 */

/* 
 * PENIRQ (touchpanel interrupt) IRQ level = 5 (nonprogrammable)
 * This is the only interrupt source at that level for the MC68328ADS
 */
#define PENIRQ 5	

/* register bit definitions */

#define MASKPEN	(0x00100000)	/* used to mask/unmask PENIRQ in sim_imr */
#define SPEN	(0x00100000)	/* used to check for PENIRQ status in sim_isr */

#define PJIRQWAIT	(0x8D)	/* configures PJ[7,3:0] to wait for IRQ in pjdata */
#define PJREADX		(0x09)	/* configures PJ[7,3:0] to read AN1 in pjdata */
#define PJREADY		(0x06)	/* configures PJ[7,3:0] to read AN0 in pjdata */
#define PJDIROUT	(0x8F)	/* configures PJ[7,3:0] as outputs in pjdir */
#define PJSELPORT	(0x8F)	/* configures PJ[7,3:0] as Port I/O in pjsel */

#define PKSPIM		(0xF8)	/* configures PK[2:0] as SPIM in pksel */

/* #define PENPU_EN	(0x40) */	/* enables PENIRQ pull-up resistor in pmpuen */
#define PENPU_EN	(0x02)	/* from errata: bits are reversed */
#define PENIRQ_SEL	(0xBF)	/* configires PM6 as PENIRQ in pmsel */

#define BIT_COUNT	9		/* bit count (10 bits) in spim_regs.cont */
#define Y_CHANNEL	(1<<6)	/* selects AN1, to be put in spim_regs.data */
#define X_CHANNEL	(0<<6)	/* selects AN0, to be put in spim_regs.data */

/* in pixels, used for filtering data, will assume noise can 
 * alter consecutive touchpanel readings by this many pixels */
#define TOLERANCE (u_short) 2

#define NO_TOUCH 0xFF	/* used if touchpanel has not been touched */

#define POWER_DELAY 10	/* used in delay() in irqs.c */


/*
 * 	Data Formatting Definitions
 */

/* following defines are used in init() to initialize X_min, Y_min, X_max, 
 * and Y_max (ADC reading is 10 bits) */
#define X_MIN_DEFAULT 0x0090	/* these values were obtained by */
#define Y_MIN_DEFAULT 0x0060	/* examination */
#define X_MAX_DEFAULT 0x03A0
#define Y_MAX_DEFAULT 0x0382

/* these defines are used for sanity check in get_coor() during calibration */
#define X_MIN_CHECK 0x0C0
#define Y_MIN_CHECK 0x0C0
#define X_MAX_CHECK 0x300
#define Y_MAX_CHECK 0x300

typedef u_int32 status_reg;

#if !defined (TRUE) || !defined (FALSE)
#undef  TRUE
#undef  FALSE
#define TRUE  1
#define FALSE 0
#endif

#if !defined (YES) || !defined (NO)
#undef  YES
#undef  NO
#define YES 1
#define NO  0
#endif

/* 
 * Following is the format of the data packet sent to a process
 * upon a valid touch input:
 *
 *                      _bit_
 *  _byte_  d7  d6  d5  d4  d3  d2  d1  d0
 *  Byte 0  1   0   0   0   0   0   bt1 bt0
 *  Byte 1  0   0   Y9  Y8  Y7  X9  X8  X7
 *  Byte 2  0   X6  X5  X4  X3  X2  X1  X0
 *  Byte 3  0   Y6  Y5  Y4  Y3  Y2  Y1  Y0
 *  Byte 4  0   0   0   0   T31 T30 T29 T28  time data
 *  Byte 5  0   T27 T26 T25 T24 T23 T22 T21  available if
 *  Byte 6  0   T20 T19 T18 T17 T16 T15 T14  TIME_STAMP
 *  Byte 7  0   T13 T12 T11 T10 T9  T8  T7   is defined
 *  Byte 8  0   T6  T5  T4  T3  T2  T1  T0
 *   
 *  bt1 bt0 = TOUCHPAD state: 01 = Pointer Down, 11 = Pointer Move,
 *							  10 = Pointer Up,   00 = No Touch (Invalid)
 *
 *  X9..X0: x-pixel location of touch (between 0 and x_pixels-1) 
 *  Y9..Y0: y-pixel location of touch (between 0 and y_pixels-1)
 *
 *  If TIME_STAMP is defined:
 *  T31..T0: system time, from OS9/DEFS/sysglob.h, will use
 *           u_int32 d_ticks = system heartbeat (tick counter)  
 *
 * Referring to the tstatics_struct below, Byte 0 is pktbuf[0], Byte 1 is 
 * pktbuf[1], etc.  pktbuf[] is filled in by isr()
 * and its index, pktbuf_index, is manipulated in read().
 */

#if defined (TIME_STAMP)
#define PKTBUFSIZ 9		/* # bytes in data packet */
#else
#define PKTBUFSIZ 4
#endif
#define MAX_BUFF  10		/* maximum # of buffers used to filter data */

#define PKTBUF_PD_BYTE0 0x81	/* Pointer Down code in pktbuf[0] */
#define PKTBUF_PM_BYTE0 0x83	/* Pointer Move code in pktbuf[0] */
#define PKTBUF_PU_BYTE0 0x82	/* Pointer Up code in pktbuf[0] */

typedef struct tstatics_struct
{
	struct scfstatic scfstat;	/* SCF required static */ 

	m68328_regs *port;			/* pointer to MC68328 registers */
	
	/* SS_SSig variables */
	process_id  datapid;		/* process ID to send on _os_send() */
	path_id     dataid;			/* path ID of process requesting a signal */
	signal_code datasig;		/* signal code to send on _os_send() */
	
	/* touchpanel data related variables */
	u_int bytes_avail;			/* number of bytes available to be read */
	u_char pktbuf[PKTBUFSIZ];	/* data packet returned to calling process */
	u_int pktbuf_index;			/* index of pktbuf */

	/* LCD display related variables
	 * These two definitions are used to obtain delta_x and delta_y
	 * in init().  delta_x and delta_y are used to transform the raw binary
	 * ADC readings into corresponding pixel locations in irqs.c. */
	u_int32 x_pixels,			/* LCD # pixels in X dimension */
			y_pixels;			/* LCD # pixels in Y dimension */

	/* cyclic alarm related variables */
	alarm_id cyc_alm_ident;		/* touchpanel cyclic alarm ID */
	u_int32 cyc_alm_period;		/* passed in from tpad descriptor */
	u_int pointer_down_flag;	/* flag indicating first occurrance of 
								 * alarm handler */
	u_int32 samples_per_point;	/* passed in from tpad descriptor */
	u_short x_buff[MAX_BUFF],	/* buffer used for filtering data in irqs.c */
		    y_buff[MAX_BUFF],
			last_x,				/* buffer used to hold last point received */
			last_y;

	status_reg mask_tpad_level;	/* touchpanel IRQ level to set in mask_irq() */

	/* Touchpanel A/DC calibration data, this data is determined by a 
	 * calibration process to be run by an application.  
	 * Note: X_min, Y_min, X_max, Y_max are binary numbers, not pixels. */
	u_short X_min,			/* minimum possible X-axis touch position */
		    Y_min,			/* minimum possible Y-axis touch position */
            X_max,			/* maximum possible X-axis touch position */
            Y_max;			/* maximum possible Y-axis touch position */

	u_int32 delta_x,		/* factors used to transform a touch coordinate */
            delta_y;		/* into a pixel coordinate */
       
	/* CALIBRATION related variables */
    u_int8 cal_flag;		/* set to TRUE for setstat (SS_DCmd = GET_POINT) */
    u_int32 ss_x0_pos, 		/* x & y position of setstat (SS_DCmd = GET_POINT) call */
    		ss_y0_pos,
			ss_x1_pos,		/* second point */
			ss_y1_pos;
	u_short ss_adc_x0,		/* corresponding ADC readings of both points */
		    ss_adc_y0,
		    ss_adc_x1,
		    ss_adc_y1;
	u_int32 ss_ptr;			/* will point to one of the two calibration points */
         
} tstatics, *TStatics;

extern tstatics t_statics; 	/* allocated in main.c */

#ifndef NO_STATICS_SHORTCUTS 
#define scfstat t_statics.scfstat
#define port t_statics.port
#define datapid t_statics.datapid
#define dataid t_statics.dataid
#define datasig t_statics.datasig
#define bytes_avail t_statics.bytes_avail
#define pktbuf t_statics.pktbuf
#define pktbuf_index t_statics.pktbuf_index
#define x_pixels t_statics.x_pixels
#define y_pixels t_statics.y_pixels
#define cyc_alm_ident t_statics.cyc_alm_ident
#define cyc_alm_period t_statics.cyc_alm_period
#define pointer_down_flag t_statics.pointer_down_flag
#define samples_per_point t_statics.samples_per_point
#define x_buff t_statics.x_buff
#define y_buff t_statics.y_buff
#define last_x t_statics.last_x
#define last_y t_statics.last_y
#define mask_tpad_level t_statics.mask_tpad_level
#define X_min t_statics.X_min
#define Y_min t_statics.Y_min
#define X_max t_statics.X_max
#define Y_max t_statics.Y_max
#define delta_x t_statics.delta_x
#define delta_y t_statics.delta_y
#define cal_flag t_statics.cal_flag
#define ss_x0_pos t_statics.ss_x0_pos
#define ss_y0_pos t_statics.ss_y0_pos
#define ss_x1_pos t_statics.ss_x1_pos
#define ss_y1_pos t_statics.ss_y1_pos
#define ss_adc_x1 t_statics.ss_adc_x1
#define ss_adc_y1 t_statics.ss_adc_y1
#define ss_adc_x2 t_statics.ss_adc_x2
#define ss_adc_y2 t_statics.ss_adc_y2
#define ss_ptr t_statics.ss_ptr
#endif

/* definitions used to set scfstat.v_err (a char) */
#define INP_BUF_OVERRUN 0x12
#define NO_ERROR        0x00

/* touchpanel cyclic alarm period (once per NUMBER_OF_TICKS) */
/* #define NUMBER_OF_TICKS 10 */	/* 10 alarms per second */
/* #define NUMBER_OF_TICKS 3 */		/* 33 alarms per second */


#endif	/* _TOUCHPAD_H */
