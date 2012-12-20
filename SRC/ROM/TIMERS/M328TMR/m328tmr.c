/*
 * $Header:   /h0/MWOS/SRC/ROM/TIMERS/M328TMR/VCS/m328tmr.c_v   1.5   07 Aug 1996 09:29:54   alexs  
 */
  
  /*--------------------------------------------------------------------------,
  !                                                                           !
  !  Copyright 1995, 1996 by Microware Systems Corporation                    !
  !                          Reproduced Under License                         !
  !                                                                           !
  !  This source code is the proprietary confidential property of Microware   !
  !  Systems Corporation, and is provided to licensee for documentation and   !
  !  educational purposes only. Reproduction, publication, or distribution    !
  !  in any form to any party other than the licensee is strictly prohibited. !
  !                                                                           !
  !---------------------------------------------------------------------------!
  !                                                                           !
  ! m328tmr.c - OS-9 ROM timer module for MC68328 Chip                        !
  !                                                                           !
  ! edition history                                                           !
  !                                                                           !
  ! ed#   date    changes                                                by   !
  ! --- -------- ------------------------------------------------------- ---  !
  !   1 95/11/03  Copied from MCCTIMER                                   rws  !
  !   2 96/01/04  Corrected initialization code.                         rak  !
  |   3 96/01/10  Modified timer handling for 32 bit timer compare       rws  |
  |               quantities.                                            rws  |
  |   4 96/01/30  Moved timeout_remaining var into tim_data struct,      rws  |
  |               added code to clear COMP status bit in m328_timer_get.      |
  |               --- OS-9/68K MC328 Support Beta Release ---                 |
  |   5 96/08/06  Fixed use of large timeouts.                           ats  |
  `--------------------------------------------------------------------------*/

#define EDITION	4

_asm("_sysedit: equ %0", __obj_constant(EDITION));

#define NEWINFO
#include <rom.h>

#include <types.h>
#include <const.h>
#include <errno.h>

#include <p2lib.h>
#include <m328tmr.h>

/* Conditionalize access to a specific tick counter */

#ifdef	TIMER1
#define	ctl		ctl1
#define	prer	prer1
#define cmp		cmp1
#define cr		cr1
#define cn		cn1
#define stat	stat1	
#endif

#ifdef	TIMER2
#define	ctl		ctl2
#define	prer	prer2
#define cmp		cmp2
#define cr		cr2
#define cn		cn2
#define stat	stat2	
#endif

#ifndef EOS_TMRINUSE
#define EOS_TMRINUSE 0x90a
#endif

typedef struct {

	/* timer initialization flag */
	u_char		inizflag;		

	/* number of milliseconds remaining for timer */
	u_int32 	timeout_remaining;

} tim_data, *Tim_data;

/* Make the timer record global so space is saved
   in not calling the memory allocate routines. */
   
tim_svcs	timer_r;
tim_data	timr_data_r;

/* timer module services */

error_code m328_timer_init (Rominfo rinf)
{
	Tim_svcs	m328_timer = rinf->timer;
	Tim_data	m328_data = m328_timer->timer_data;
	
	M328Ctl tctl = (M328Ctl)&(M328Timer->ctl);

	if (!m328_data->inizflag) {
		tctl->TEN = 0;		/* disable Timer */
		tctl->IRQEN = 0;	/* disable interrupts */
		tctl->CLK_SRC = 2;	/* clksrc = sys clk / 16 */
		tctl->CAP_EDGE = 0;	/* disable IRQ on capture event */
		tctl->FRR = 0;		/* restart mode */
		m328_data->inizflag = 1;
		m328_data->timeout_remaining = 0;
		return(SUCCESS);
	} else
		return(EOS_TMRINUSE);
}

void m328_timer_deinit(Rominfo rinf)
{
	Tim_svcs	m328_timer = rinf->timer;
	Tim_data	m328_data = m328_timer->timer_data;
	
	M328Timer->ctl = 0;		/* disable timer */
	m328_data->inizflag = 0;
	return;
}

/*
 * MC68328ADS Target's system clock set up to be 32,768 Hz * 500 =
 * 16.38 MHz; timer's clock source set up to be system clock / 16 = 
 * 1.024 MHz => 0.976 microsecond per cycle.  So, will just load timeout
 * into the timer's compare register since each cycle counted is 
 * approximately a microsecond anyway.
 *
 * timeout ranges from 500,000 (0.5 seconds) to 5,000,000 (5 seconds), 
 * BUT only have 16 bit compare register => max value = 2 ^ 16 = 65,535.
 * Will keep number of cycles remaining in timeout_remaining static var.
 */
void m328_timer_set
(
	Rominfo rinf, 
	u_int32 timeout		/* number of microseconds */
)
{
	Tim_svcs	m328_timer = rinf->timer;
	Tim_data	m328_data = m328_timer->timer_data;
	
	M328Ctl tctl = (M328Ctl)&(M328Timer->ctl);
	
	tctl->TEN = 0;				/* disable timer */

	if (timeout > 0xFFFF)	/* timeout > 16 bit value */
	{
		m328_data->timeout_remaining = timeout - 0xFFFF;
		timeout = 0xFFFF;
	} else
		m328_data->timeout_remaining = 0;
	
	M328Timer->cmp = timeout;	/* setup compare register */
	tctl->TEN = 1;				/* enable timer */
	return;
}

u_int32 m328_timer_get(Rominfo rinf)
{
	Tim_svcs	m328_timer = rinf->timer;
	Tim_data	m328_data = m328_timer->timer_data;
	
	M328Ctl tctl = (M328Ctl)&(M328Timer->ctl);
	M328Stat tstat = (M328Stat)&(M328Timer->stat);
	u_int32 remains;

	/* find remaining time if time is left on counter */
	if (tstat->COMP == 0)
	{
		remains = M328Timer->cmp - M328Timer->cn;
		if (m328_data->timeout_remaining != 0)
			remains += m328_data->timeout_remaining;
	}

	else	/* timer has expired */
	{
		tctl->TEN = 0;			/* disable timer */
		*(u_int16 *)tstat = 0;	/* clear status bit */
		
		if (m328_data->timeout_remaining == 0)		/* done */
			remains = 0;
		else							/* more time to wait */
		{
			remains = m328_data->timeout_remaining;
			if (remains <= M328Timer->cn) /* count in any overflow */
				remains = 0;
			else
			{
				remains -= M328Timer->cn;
				if (remains > 0xFFFF)
				/* timeout > 16 bit value */
				{
					m328_data->timeout_remaining = remains - 0xFFFF;
					M328Timer->cmp = 0xFFFF;
				}
				else
				{
					M328Timer->cmp = remains;
					m328_data->timeout_remaining = 0;
				}
				tctl->TEN = 1;		/* enable timer */
			}
		}
	}
		
	return (remains);
}

/* timer module initialization */

error_code p2start(Rominfo rinf, u_char *glbls)
{
	Tim_svcs	m328_timer;

	/* check for an existing timer services module */
	
	if (rinf->timer != NULL)
		return (EOS_NOTRDY); /* yup, leave it alone and go bye bye */
	
	m328_timer = &timer_r;

	m328_timer->struct_id = TIMSVCID;
	m328_timer->struct_ver = TVC_VER_MAX;

	m328_timer->timer_init = &m328_timer_init;
	m328_timer->timer_deinit = &m328_timer_deinit;
	m328_timer->timer_set = &m328_timer_set;
	m328_timer->timer_get = &m328_timer_get;
	m328_timer->timer_data = &timr_data_r;
	
	rinf->timer = m328_timer;
	return (SUCCESS);
}

