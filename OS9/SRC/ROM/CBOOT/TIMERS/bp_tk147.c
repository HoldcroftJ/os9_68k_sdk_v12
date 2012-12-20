/*--------------------------------------------------------------------------!
! bp_timer.c: timer routines for BOOTP code: VME147							!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!               ---- OS-9/68K V2.4 released ----                            !
!	01 90/08/08 Cloned from code originated by J. Laake					kk	!
!	02 92/09/24 made timer test setup conditional on -dTIMER_TEST.		wwb	!
!   03 93/06/10 "Ultrified"                                             bat !
!               ---- OS-9 V3.0 Released ----                                !
!   04 98/02/02 moved declaration for bpt_test to inetboot.c            mgh !
!               Removed conditional code to set variable                    !
!   05 99/09/20 changed to SPF defs, removed Lots of includes           mgh !
!---------------------------------------------------------------------------!
!                                                                           !
!              Copyright 1990,1993 by Microware Systems Corporation         !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

/*#include <sysboot.h> */
#include <types.h>
#include <SPF/BSD/netinet/in.h>
#include <SPF/BSD/protocols/bootp.h>


#define T1_PRELOAD	0xFFFE1010
#define T1_COUNTER	0xFFFE1012
#define T1_ICR		0xFFFE1018
#define T1_CTRL		0xFFFE1019
#define T1_VECT		0x48	/* assumes PCC base irq=0x40 */
#define T1_IRQLVL	0x05

       int		bpt_rseed;	/* seed rnd backoff generator */
static u_int32  otexhand;	/* original timer vector exception handler address	*/
	   u_int32	bpt_curtick;/* global 100ms timer counter		*/
static u_char	tmrvect;	/* global storage for actual timer vector	*/
void bpt_irqsvc();

extern void *exjmptab;

static u_int32 setexcpt(vector, irqsvc)
u_char vector;
void *irqsvc;
{
	register u_int32 *ejt_entry, original;
	
	ejt_entry = (void *)(((u_int32)exjmptab) + ((vector - 2) * 10) + 6);
	original = *ejt_entry;
	*ejt_entry = (u_int32)irqsvc;

	return original;
}

/*----------------------------------------------------------------------!
! init_timer: Initialize the timer to rollover, produce an interrupt,	!
! and increment a global variable once every "rollval" where "rollval"	!
! is the desired rollover time in NANOseconds.							!
!----------------------------------------------------------------------*/
bpt_init()
{
	
	/*----------------------------------------------------------!
	!The slowest you can get this guy to roll over is .4096 secs!
	! For .001 sec: T1_PRELOAD = 65376							!
	! For .010 sec: T1_PRELOAD = 63936							!
	! For .100 sec: T1_PRELOAD = 49536
	! For .250 sec: T1_PRELOAD = 25536							!
	! For .400 sec: T1_PRELOAD = 1536							!
	!----------------------------------------------------------*/
	tmrvect = T1_VECT;			/* timer irq vector */
	otexhand = setexcpt(tmrvect, bpt_irqsvc);	/* Set up exception handler	*/
	maskirq(7);					/* mask interrupts during timer setup	*/
	bpt_curtick = 0;			/* init counter */
	*((char *)T1_CTRL) = 0;		/* clear all status */
	*((char *)T1_ICR)  = 0;		/* clear all irqs */
	*((short *)T1_PRELOAD) = 49536;			/* load timer counter for .1 sec */
	*((char *)T1_ICR)  = 0x8 + T1_IRQLVL;	/* enable timer irqs and level */
	*((char *)T1_CTRL) = 0x3;				/* start counter */
	maskirq(T1_IRQLVL-1);		/* enable clock irqs... */

	bpt_rseed = 0xdeadface + *((short *)T1_COUNTER);	/* set seed for RNG */
}

bpt_term()
{
	maskirq(7);						/* mask irqs back up */
	*((char *)T1_CTRL) = 0;			/* clear all status */
	*((char *)T1_ICR)  = 0;			/* clear all irqs */
	
	setexcpt(tmrvect, otexhand);	/* remove exception handler	*/
}


bpt_bootp(bph)
register struct bootp *bph;
{
	register int status = 0;
	
	return status;
}

#ifdef _UCC

_asm("bpt_irqsvc: movem.l d0,-(sp)");
_asm(" addq.l #1,bpt_curtick(a6)");
_asm(" move.b 0xfffe1018,d0		; !!HARDCODE!!  (get IRQ flag)");
_asm(" move.b d0,0xfffe1018		; !!HARDCODE!!  (clear IRQ flag)");
_asm(" movem.l (sp)+,d0");
_asm(" addq.l #4,sp");
_asm(" rte");
_asm("*");
_asm("* u_short maskirq(short irqlvl);	/* set IRQ level in SR, return old */");
_asm("*");
_asm("maskirq movem.l d1/d2,-(sp)");
_asm(" lsl.w #8,d0 shift irq mask up to proper place");
_asm(" move.w sr,d2 save old sr value for return");
_asm(" move.w d2,d1 get temp sr value");
_asm(" andi.w #^0x700,d1 clear the irq mask");
_asm(" or.w d0,d1 set the new irq mask");
_asm(" move.w d1,sr save the new sr");
_asm(" move.w d2,d0 get the sr copy");
_asm(" lsr.w #8,d0 shift for the irq value");
_asm(" andi.l #7,d0 mask off all but old irq mask value");
_asm(" movem.l (sp)+,d1/d2");
_asm(" rts");

#else

#asm
bpt_irqsvc: movem.l d0,-(sp)
 addq.l #1,bpt_curtick(a6)
 move.b 0xfffe1018,d0		; !!HARDCODE!!  (get IRQ flag)
 move.b d0,0xfffe1018		; !!HARDCODE!!  (clear IRQ flag)
 movem.l (sp)+,d0
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
#endasm

#endif
