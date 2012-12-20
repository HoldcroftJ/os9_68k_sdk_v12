/*--------------------------------------------------------------------------!
! bp_timer.c: timer routines for BOOTP code: MZ68360							!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	01 93/11/25 from the ether it appears								arb !
!   02 98/02/02 moved declaration for bpt_test to inetboot.c            mgh !
!               Removed conditional code to set variable                    !
!   03 99/12/17 Moved hardcoded defines to systype.h and mc68360.h      mgh !
!---------------------------------------------------------------------------!
!                                                                           !
!                                                                           !
!                Copyright 1994 by Microware Systems Corporation            !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Mizar		!
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/
#ifndef _types
#include <types.h>			/* This is here for use by high level stuff		*/
#endif

#include <systype.h>
#include <mc68360.h>


       int		bpt_rseed;	/* seed rnd backoff generator */
static u_int32  otexhand;	/* original timer vector exception handler address	*/
	   u_int32	bpt_curtick;/* global 5ms timer counter		*/
	   u_int32  bpt_tick;
static u_char	tmrvect;	/* global storage for actual timer vector	*/
void bpt_irqsvc();




/*----------------------------------------------------------------------!
! init_timer: Initialize the timer to rollover, produce an interrupt,	!
! and increment a global variable once every "rollval" where "rollval"	!
! is the desired rollover time in NANOseconds.							!
!----------------------------------------------------------------------*/
bpt_init()
{
	register unsigned short *tptr	;
	
	tmrvect = T_VECT;			/* timer irq vector */
	otexhand = setexcpt(tmrvect, bpt_irqsvc);	/* Set up exception handler	*/
	maskirq(7);					/* mask interrupts during timer setup	*/
	bpt_curtick = 0;			/* init counter */
	tptr = 	(unsigned short*)(DPRBASE_S+PICR);
	*tptr = (T_IRQLVL << 8) | T_VECT;
	tptr = (unsigned short*)(DPRBASE_S+PITR);
	*tptr = 0x00f4;
	maskirq(T_IRQLVL-1);		/* enable clock irqs... */

	bpt_rseed = 0xdeadface;		/* set seed for RNG */
}



bpt_term()
{
	register unsigned short * tptr;
	
	maskirq(7);						/* mask irqs back up */
	tptr = (unsigned short*)PITR;
	tptr = 0;
	setexcpt(tmrvect, otexhand);	/* remove exception handler	*/
}


bpt_bootp(bph)
register struct bootp *bph;
{
	register int status = 0;
	
	return status;
}

_asm("
bpt_irqsvc: 
 addq.l #1,bpt_tick(a6)
 cmpi.l #20,bpt_tick(a6)
 blt.s	 irq_notyet
 addq.l #1,bpt_curtick(a6)
 clr.l	bpt_tick(a6)
irq_notyet:
 addq.l #4,sp
 rte
*
* u_short maskirq(short irqlvl);	/* set IRQ level in SR, return old */
*
maskirq movem.l d1/d2,-(sp)
 lsl.w #8,d0 shift irq mask up to proper place
 move.w sr,d2 save old sr value for return
 move.w d2,d1 get temp sr value
 andi.w #^0x700,d1 clear the irq mask
 or.w d0,d1 set the new irq mask
 move.w d1,sr save the new sr
 move.w d2,d0 get the sr copy
 lsr.w #8,d0 shift for the irq value
 andi.l #7,d0 mask off all but old irq mask value
 movem.l (sp)+,d1/d2
 rts
");
