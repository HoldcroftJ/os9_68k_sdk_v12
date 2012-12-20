/*****************************************************************************
** 328ADS System Module definitions                                         **
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
**   1 02/01/96 Creation                                          jgm       **
**   2 06/05/96 Cleaned up to ship out with the Beta release      jgm       **
*****************************************************************************/

/*
** Edition Number
*/

#if defined(_SYSEDIT)
_asm("_sysedit: equ 2");
#endif


/*
** Header Files
*/

#include <types.h>
#include <time.h>
#include <const.h>
#include <errno.h>
#include <process.h>
#include <sysglob.h>
#include <funcs.h>
#include <alarm.h>

#if defined(_OSK)
#include <MACHINE/reg.h>
#endif

#include <MC6832X/mc68328.h>
#include <PWRMAN/pwrman.h>
#include <PWRMAN/328ads.h>

/*
** Definitions
*/

#if defined(_OSK)
typedef u_int16 status_reg;
#define EOS_NOTME (-1)
#endif

#ifndef SUCCESS
#define SUCCESS (0)
#endif

#define HIGH(reg,bit) reg |= bit
#define LOW(reg,bit) reg &= ~bit

#define RTC_IRQ_VECTOR		0x1c		/* irq vector, user defined */
#define RTC_IRQ_PRIORITY	5			/* irq polling priority, user defined */

#define REGBASE				(M68328_regs)(0xFFFFF000) /* Base address of Memory Map */
#define RTCLOCK				(M68328_regs)(0xFFFFFB00) /* rtc port adddress */
#define SIM_PITR			(volatile u_int16*)(0xfffff600) /* timer 1 port */
#define MAXSEC				(86400) /* Maximum number of seconds in one day == 24 hours */

#define PORTBIT (1 << 3)    /* bit to disable the PLL in the PLL control register */
#define SYSCLK (1 << 8)     /* bit to change the master frequency in the PLL control register to divide by 4 */
#define SYSCLK_LOW (1<< 10) /* bit to change the master frequency in the PLL control register to divide by 4 */
#define MBIT (1<<1)         /* the minute bit for the RTC interrupt enable register */
#define HBIT (1<<3)         /* the hour bit for the RTC interrupt enable register */
#define ABIT (1<<2)         /* the alarm bit for the RTC interrupt enable register */

#ifdef REV2G15E
#define MINSLEEP			(3) /* 3 seconds */
#else
#define MINSLEEP			(62) /* 62 seconds */
#endif

typedef struct localdata {
	Sysglobs sysglob;			/* system global variables ptr */
	M68328_regs regs;			/* 68328 internal registers ptr */
	u_int8 sec;					/* second (before STOP) */
	u_int8 min;					/* minute (before STOP) */
	u_int8 hour;				/* hour (before STOP) */
	u_int8 hour_flag;			/* 0 or 1, depending on if the 24 hour interrupt goes off */
	u_int32 date;				/* save the Julian date */
#ifdef REV2G15E
#else
	u_int32 wakeupmins;			/* number of times to have the 1 min interrupt go off */
#endif

#define MIN_FLAG 0x02			/* enable the 1 minute interrupt flag */
#define HOUR_FLAG 0x08			/* enable the 24 hour interrupt flag */
#define ALM_FLAG 0x04			/* enable the alarm interrupt flag */
#define RTC_ENABLE 0x80			/* enable the RTC */
#define IMR_RTC 0x00000010		/* enable the RTC interrupt from the IMR register */
#define IMR_RTC_DIS 0x003EFFFF	/* disable the RTC interrupt from the IMR register */
#define IWR_RTC 0x00FFFFFF		/* enable all interrupts to start the power-control wakeup */
#define PLL_PCTLR 0x00CF		/* power duty cycle of 0% */
#define PLL_DCTLR 0x001F		/* disable the power control */
#define SECONDBITS 0x0000003F	/* mask off seconds */
#define MINUTEBITS 0x003F0000	/* mask off minutes */
#define HOURBITS 0x1F000000		/* mask off hours */
} localdata, *Localdata;


/* Static storage variables */
extern localdata   local_data;
extern Localdata   ldptr;

/*
** Function Prototypes
*/

/* cpu.c */
extern error_code cpu_init(Localdata);
extern error_code cpu_term(Localdata);
extern error_code cpu_on(Localdata);
extern error_code cpu_idle(Localdata);
extern error_code cpu_suspend(Localdata);
extern error_code _os_pwr_reg(char cpu[PWR_IDLEN],error_code (*dr_pwrman)(void *,pwr_level,void *),void *,Pwr_devcond);
extern error_code _os_pwr_unreg(char cpu[PWR_IDLEN],error_code (*dr_pwrman)(void *,pwr_level,void *),void *,Pwr_devcond);
extern void downtime(Localdata);
extern void currentime(Localdata);
extern error_code dr_pwrman(void *,pwr_level,void *);
extern void common(Localdata,u_int16);
extern u_int32 check(Localdata);

/* rtclock.c */
extern error_code rtc_init(Localdata);
extern error_code rtc_term(Localdata);
extern error_code rtc_hour_set(Localdata);
extern error_code rtc_stop(Localdata);
extern error_code rtc_isr_glue(Localdata);
extern error_code rtc_isr(Localdata);
#ifdef REV2G15E
extern error_code rtc_alarm_set(Localdata,u_int32,u_int32,u_int32);
#else
extern error_code rtc_min_set(Localdata);
#endif

/* other */
extern void irq_disable(void);
extern void irq_restore(status_reg);
extern status_reg irq_save(void);
