/*
 * $Header:   /h0/MWOS/SRC/ROM/TIMERS/M328TMR/VCS/m328tmr.h_v   1.2   17 May 1996 14:57:00   alexs  
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
  ! 328tmr.h - Definitions for the MC68328 Chip                               !
  !                                                                           !
  ! edition history                                                           !
  !                                                                           !
  ! ed#   date    changes                                                by   !
  ! --- -------- ------------------------------------------------------- ---  !
  !   1 95/08/11  History started                                        rws  !
  |   2 96/01/10  Corrected m68328_timer_struct                          rws  |
  |               --- OS-9/68K MC328 Support Beta Release ---                 |
  `--------------------------------------------------------------------------*/

#ifndef TimerBase
#define	TimerBase	0xFFFFF600
#endif

#if	!defined(__STDC__)&&!defined(_ANSI_EXT)
#ifndef	volatile
#define		volatile	/* the old compiler doesn't understand 'volatile' */
#endif
#endif

/* MC68328 Timer Block Registers */
typedef volatile struct m68328_timer_struct {
	
	u_int16 ctl1,	/* control register */
			prer1,	/* prescale register */
			cmp1,	/* compare register */
			cr1,	/* capture register */
			cn1,	/* counter register */
			stat1,	/* status register */
			
			ctl2,	/* dittos */
			prer2,	
			cmp2,  
			cr2,   
			cn2,   
			stat2;
			
} *Tmr_regs;

#define	M328Timer ((Tmr_regs)TimerBase)	/* properly casted constant */

/* Timer Control Register bit definitions */
typedef volatile struct m328ctl {	
	u_int	unused1:7,
			FRR:1,			/* Free Run/Restart */
			CAP_EDGE:2,		/* Capture Edge */
			OM:1,			/* Output Mode */
			IRQEN:1,		/* Interrupt Enable */
			CLK_SRC:3,		/* Clock Source */
			TEN:1;			/* Timer Enable */
} *M328Ctl;

/* Prescaler Register bit definitions */
typedef volatile struct m328prer {	
	u_int	unused1:8,
			PRESCALE:8;		/* Prescale divide value */
} *M328Prer;

/* Timer Status Register bit definitions */
typedef volatile struct m328stat {	
	u_int	unused:14,
			CAPT:1,			/* Capture Event */
			COMP:1;			/* Compare Event */
} *M328Stat;

