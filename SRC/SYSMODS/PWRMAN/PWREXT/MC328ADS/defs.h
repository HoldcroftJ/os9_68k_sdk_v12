/*****************************************************************************
** Power Manager Extensions definitions                                     **
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
**   1 06/14/95 creation                                          bat       **
**   5 12/21/95 ported to PwrMan (for 68328 board)				  rmm	    **
**   6 01/05/96 added in the subroutine module stuff              rmm       **
**   7 01/10/96 changed subroutine module interface to P2         rmm       **
**   8 01/15/96 split into 3 files & modules                      rmm       **
**   9 05/31/96 updated for PwrMan v1.0.Beta structures           bat       **
**     01/17/97 <--------------- PwrMan v2.1 --------------->               **
*****************************************************************************/

#ifdef _SYSEDIT
_asm("_sysedit: equ 9");
#endif

/*
** Header Files
*/
#include <types.h>
#include <const.h>
#include <errno.h>
#include <memory.h>
#include <PWRMAN/pwrman.h>


/*
** Definitions
*/

#if defined(_OSK)
typedef u_int16 status_reg;
#define EOS_NOTME (-1)
#endif


typedef struct {
    error_code (*pwrman_func)(F_pwrman_pb pb, Pwrman_globals ldptr);
} localdata, *Localdata;


/*
** Function Prototypes
*/
error_code PwrExt_entry(F_pwrman_pb pb, Pwrman_globals ldptr);

/* other */
extern void irq_disable(void);
extern void irq_enable(void);
extern void irq_restore(status_reg newval);
extern status_reg irq_save(void);

