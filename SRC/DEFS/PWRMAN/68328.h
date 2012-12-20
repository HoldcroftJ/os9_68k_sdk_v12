/*****************************************************************************
** MC68328 CPU Power Management definitions                                 **
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
**   1 05/31/96 Creation                                          bat       **
**     01/17/97 <--------------- PwrMan v2.1 --------------->               **
*****************************************************************************/

#ifndef _PWRMAN_68328_H
#define _PWRMAN_68328_H


/* MC68328 CPU State Definitions */
#define MC68328_PWR_CPU_ON          0x10     /* cpu in full power state */
#define MC68328_PWR_CPU_IDLE        0x20     /* cpu in idle power state */
#define MC68328_PWR_CPU_SUSPEND     0x30     /* cpu in suspend power state */
#define MC68328_PWR_CPU_TERM        0x40	 /* cpu to terminate */

/* MC68328 LCD State Definitions */
#define MC68328_PWR_LCD_ON          0xff     /* lcd screen on */
#define MC68328_PWR_LCD_OFF         0x00     /* lcd screen off */


#endif /* _PWRMAN_68328_H */
