/*****************************************************************************
** ID:     @(#) main.c 1.4@(#)
** Date:   3/14/96
******************************************************************************
 * Edition history and declaration of static storage.
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
**	 1 09/13/95 creation                                          rws       **
**   2 03/07/96 Modified so that LCD resolution is now contained  rws       **
**              in touchpad device descriptor's DevCon field                **
**   3 03/14/96 Modified so that cyclic alarm period and the      rws       **
**              number of samples per point are now in the device           **
**              descriptor's DevCon field                                   **
*****************************************************************************/

#ifdef _UCC
_asm("_sysedit: equ 3");
#else
@_sysedit: equ 3
#endif

#include "touchpad.h"

tstatics t_statics;		/* allocate static storage area */


/*

This is the touchpanel SCF driver for the MC68328ADS Appplication
Development System Board from Motorola.  

The interface for the MC68328ADS touchpanel is an MC145050
Analog-to-Digital Converter, connected to the MC68328's Master SPI bus,
and the MC68328 PENIRQn interrupt input.

As a resistive-film sensing touchpanel must be used, touchpanel debouncing
is accomplished by having the PENIRQn interrupt service routine creating
an alarm going off about 30 ms from the interrupt.  It is the alarm
handler's responsibility to format data and signal an interested
process.  

Touchpanel calibration is accomplished by having a known position on the
touchpanel touched at a known time.  This is done throuch a SS_DCmd
setstat command, GET_POINT.

*/
