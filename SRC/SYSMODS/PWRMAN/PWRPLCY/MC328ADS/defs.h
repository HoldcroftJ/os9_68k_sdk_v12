/*****************************************************************************
** Power Manager Policy subroutine module definitions                       **
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
**   1 10/02/95 Creation                                          bat       **
**   2 09/29/95 added OS9000 support                              bat       **
**   3 01/08/96 recreation for the real thing                     rmm       **
**   9 05/30/96 modified for PwrMan v1.0.Beta structures          bat       **
**  10 01/18/97 added save fields to globals for DEINIT           ecm       **
**     01/17/97 <--------------- PwrMan v2.1 --------------->               **
*****************************************************************************/

#ifdef _SYSEDIT
_asm("_sysedit: equ 10");
#endif

/*
** Header Files
*/
#include <types.h>
#include <time.h>
#include <const.h>
#include <errno.h>
#include <const.h>
#include <process.h>
#include <sysglob.h>
#include <funcs.h>
#include <memory.h>
#include <alarm.h>
#include <string.h>

#if defined(_OSK)
#include <setsys.h>
#include <MACHINE/reg.h>
#elif defined(_OS9000)
#include <regs.h>
#endif

#include <PWRMAN/pwrman.h>
#include <PWRMAN/328ads.h>


/*
** Definitions
*/

#if defined(_OSK)
typedef u_int16 status_reg;
#define EOS_NOTME (-1)
#endif

/* number of ticks to detect a suspend condition */
#define SUSPEND_DELAY (8500)

/* number of ticks the idle routine uses to detect an idle condition*/
#define IDLE_DELAY    (6500)

/* power level to start the ON/IDLE/SUSPEND/OFF states */
#define START_STATE   PWRPLCY_STATE_ON

/* power state table initialization structure */
typedef struct devreg_init {
	pwr_level syslevel;
	char id[PWR_IDLEN];
	pwr_level devlevel;
	u_int32 priority;
} devreg_init, *Devreg_init;

/* power policy global variable structure */
typedef struct {
	u_int32 d_ticks;
	u_int32 old_d_switches;
	void *old_d_idledata,
		(*old_d_idle)();
} localdata, *Localdata;

/*
** Function Prototypes
*/

/* pwrplcy.c */
extern error_code PwrPlcy_entry(F_pwrman_pb pb, Pwrman_globals ldptr);
extern void idle(void);
extern error_code pwr_add(Pwrman_globals ldptr, char id[PWR_IDLEN], pwr_level syslevel, u_int32 priority, pwr_level devlevel, void* devpb);
extern error_code pwr_remove(Pwrman_globals ldptr, char id[PWR_IDLEN], pwr_level syslevel, u_int32 priority, pwr_level devlevel, void* devpb);
extern error_code pwr_change(Pwrman_globals ldptr, pwr_level syslev);

/* other */
extern void* get_static(void);
extern void irq_disable(void);
extern void irq_enable(void);
extern void irq_restore(status_reg newval);
extern status_reg irq_save(void);
