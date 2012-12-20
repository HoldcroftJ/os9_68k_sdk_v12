/*******************************************************************************
*
* DESCRIPTION :
*
*	THIS FILE IS REQUIRED BY THE TEMPLATE.
*
*	This file contains hardware specific definitions. This file is required by
*	the template, but the definitions here are not used by the template.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  12/22/94  TJE  First version
*/
#ifndef _HARDWARE_H
#define _HARDWARE_H

#include <types.h>

#include <MC6832X/mc68328.h>

/* HIGH and LOW defs also needed in mfm_desc.h */
#if !defined (HIGH) || !defined (LOW)
#undef HIGH
#undef LOW
#define HIGH 0
#define LOW  1
#endif

#define HI(reg,bit) reg |= bit;
#define LO(reg,bit) reg &= ~bit;
#define PORTBIT (1 << 0);

#if defined(PWR_AWARE)

#include <PWRMAN/pwrman.h>
#include <PWRMAN/68328.h>

error_code dr_pwrman(void *dev_entry,pwr_level pwrlvl, void *devpb);
error_code _os_pwr_reg(char gfx[PWR_IDLEN],error_code (*dr_pwrman)(void *param, pwr_level, void*), void *dev_entry, Pwr_devcond devpwrdef);
error_code _os_pwr_unreg(char gfx[PWR_IDLEN],error_code (*dr_pwrman)(void *param, pwr_level, void*), void *dev_entry, Pwr_devcond devpwrdef);

#endif /* PWR_AWARE */

#endif /* _HARDWARE_H */
