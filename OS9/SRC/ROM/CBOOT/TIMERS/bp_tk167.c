/*--------------------------------------------------------------------------!
! bp_timer.c: timer routines for BOOTP code: VME167							!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	01 91/12/05 Modeled from code for VME147							wwb	!
!	02 91/12/17 Added TIMER_TEST conditional.							wwb	!
!	03 91/12/18 Added bpt_bootp() nvram check							kk	!
!   04 93/06/10 "Ultrified"                                             bat !
!	05 93/08/27 replaced "include inetboot.h" w/ if.h & bootp.h			gkm	!
!               ---- OS-9 V3.0 Released ----                                !
!   06 98/02/02 moved declaration for bpt_test to inetboot.c            mgh !
!               Removed conditional code to set variable                    !
!   07 99/09/20 changed to SPF defs, removed Lots of includes           mgh !
!---------------------------------------------------------------------------!
!                                                                           !
!              Copyright 1991,1993 by Microware Systems Corporation         !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include <types.h>
#include <SPF/BSD/netinet/in.h>
#include <SPF/BSD/protocols/bootp.h>

/* PCC2 Timer #2 definitions */

#define T_PRELOAD	0xFFF4200C	/* Timer #2 Compare register */
#define T_COUNTER	0xFFF42010	/* Timer #2 Counter register */
#define T_CTRL		0xFFF42016	/* Timer #2 Control register */

#define T_VECT		0x68		/* assumes PCC2 base irq=0x60 */

#define	TIMER_GO	0x7			/* timer enable, clear on compare, clear ovf */

#define T_IRQCTRL	0xFFF4201A	/* Timer #2 Interrupt control */
#define T_IRQENAB	(1<<4)		/* enable Timer #2 irqs */
#define T_IRQCLR	(1<<3)		/* clear Timer #2 irq */

#ifdef _UCC
_asm("IrqCtrl equ 0xFFF4201A			above in assembler style");
_asm("b_IrqClr equ 3					IRQ Clear bit number");
#else
@IrqCtrl equ 0xFFF4201A			above in assembler style
@b_IrqClr equ 3					IRQ Clear bit number
#endif

#define T_LCMASK	0x7			/* bits 2-0 define the actual level */

#define T_IRQLVL	0x05		/* use irq level 5 */

#define TIME_PERIOD (100000)	/* number of uSec in timer period */	

#define TICKSEC	(1000000/TIME_PERIOD)		/* ticks per second */

		int		bpt_rseed;		/* seed rnd backoff generator */
static	u_int32	otexhand;		/* original timer vector exception handler address	*/
		u_int32	bpt_curtick;	/* global 100mS timer counter */
static	u_char	tmrvect;		/* global storage for actual timer vector	*/

static	u_int32	origlvl;		/* original irq level at init time */

void bpt_irqsvc();


extern void *exjmptab;

u_int32 setexcpt(vector, irqsvc)
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
! bpt_init(): Initialize the timer to rollover, produce an interrupt,	!
! and increment a global variable once every "rollval" where "rollval"	!
! is the desired rollover time in NANOseconds.							!
!----------------------------------------------------------------------*/
bpt_init()
{
	/*----------------------------------------------------------!
	! The PCC2 Timers rattle away at 1uS increments, thus you	!
	! only need to load the "number of uSecs" required for the	!
	! timer period (e.g. 1000000 for 100mS).					!
	!----------------------------------------------------------*/

	tmrvect = T_VECT;			/* timer irq vector */
	otexhand = setexcpt(tmrvect, bpt_irqsvc);	/* Set up exception handler	*/
	origlvl = maskirq(7);		/* mask interrupts during timer setup	*/
	bpt_curtick = 0;			/* init counter */


	/* setup Timer #2 in PCC2 */
	*((char *)T_CTRL) = 0;			/* PCC2: clear all status/irq enables */
	*((u_int32 *)T_COUNTER) = 0;	/* zero the initial counter */
	*((u_int32 *)T_PRELOAD) = TIME_PERIOD;	/* set timer period */
	*((u_char *)T_CTRL) = TIMER_GO;	/* start the timer */

	/* clear any possible pending irqs, disable irq level/enable */
	*((u_char *)T_IRQCTRL) = T_IRQCLR;

	/* set hardware level, enable irqs */
	*((u_char *)T_IRQCTRL) = T_IRQLVL | T_IRQENAB;

	maskirq(T_IRQLVL-1);			/* enable clock irqs... */

	bpt_rseed = 0xdeadface + *((u_int32 *)T_COUNTER);	/* set seed for RNG */
}

bpt_term()
{
	maskirq(7);							/* mask irqs back up */
	
	/* shut down Timer #2 in PCC2 */
	*((char *)T_CTRL) = 0;				/* disable timer operation */
	*((char *)T_IRQCTRL) = T_IRQCLR;	/* clear )any) irq, disable ints */

	setexcpt(tmrvect, otexhand);		/* remove exception handler	*/
	
	maskirq(origlvl);					/* restore original irq level */
}

/*
 * this routine is called before sending the bootpc request
 * this function should fill in the bootp request message as desired
 * (possibly from NVRAM) and return 1.  Or do nothing and return 0.
 *
 * the implementor is free to do *anything* here
 */

#define BBRAMBASE	0xfffc0000
#define BBRAM_BOOTP	(BBRAMBASE + 512)
#define BOOTP_COOKIE	"BpNVHaHa"

bpt_bootp(bph)
register struct bootp *bph;
{
	register int status = 0;
	
/*
 * if we see our "cookie", struct copy the prototype bootp message
 */
	if (memcmp(BBRAM_BOOTP, BOOTP_COOKIE, sizeof(BOOTP_COOKIE)-1) == 0) {
		*bph = *(struct bootp *)(BBRAM_BOOTP + sizeof(BOOTP_COOKIE)-1);
		status = 1;
	}

	return status;
}
	
#ifdef _UCC

_asm("*");
_asm("* timer interrupt service routine:");
_asm("* NOTE that this routine assumes that only a valid Timer IRQ will");
_asm("* occur on this vector");
_asm("*");
_asm("bpt_irqsvc: move.l d0,-(sp)	; save reg");
_asm(" addq.l #1,bpt_curtick(a6)		; count the tick");
_asm(" move.b IrqCtrl,d0				; read irq control reg");
_asm(" bset.l #b_IrqClr,d0			; set IRQ clear bit");
_asm(" move.b d0,IrqCtrl				; tell the hardware");
_asm(" move.l (sp)+,d0				; restore reg");
_asm(" addq.l #4,sp					; toss stacked vector (from jump table)");
_asm(" rte							; resume");
_asm("*");
_asm("* set IRQ level in SR, return old irq level.");
_asm("* NOTE that this code takes care to preserve the TRACE bits");
_asm("*");
_asm("* u_int32 maskirq(irqlvl);");
_asm("*");
_asm("maskirq movem.l d0/d1,-(sp) save regs");
_asm(" lsl.w #8,d0 move to correct position for sr's irq bits");
_asm(" andi.l #IntMask,d0 isolate irq mask level");
_asm(" moveq.l #0,d1 sweep register");
_asm(" move.w sr,d1 get current sr");
_asm(" move.l d1,(sp) save it in stacked d0");
_asm(" andi.w #IntEnab,d1 clear current irq mask level");
_asm(" or.w d0,d1 add in requested level");
_asm(" move.w d1,sr set desired irq mask level");
_asm(" movem.l (sp)+,d0/d1 recover old sr, d1");
_asm(" andi.w #IntMask,d0 isolate old level");
_asm(" lsr.w #8,d0 move old level to low byte");
_asm(" rts");

#else

#asm
*
* timer interrupt service routine:
* NOTE that this routine assumes that only a valid Timer IRQ will
* occur on this vector
*
bpt_irqsvc: move.l d0,-(sp)	; save reg
 addq.l #1,bpt_curtick(a6)		; count the tick
 move.b IrqCtrl,d0				; read irq control reg
 bset.l #b_IrqClr,d0			; set IRQ clear bit
 move.b d0,IrqCtrl				; tell the hardware
 move.l (sp)+,d0				; restore reg
 addq.l #4,sp					; toss stacked vector (from jump table)
 rte							; resume
*
* set IRQ level in SR, return old irq level.
* NOTE that this code takes care to preserve the TRACE bits
*
* u_int32 maskirq(irqlvl);
*
maskirq movem.l d0/d1,-(sp) save regs
 lsl.w #8,d0 move to correct position for sr's irq bits
 andi.l #IntMask,d0 isolate irq mask level
 moveq.l #0,d1 sweep register
 move.w sr,d1 get current sr
 move.l d1,(sp) save it in stacked d0
 andi.w #IntEnab,d1 clear current irq mask level
 or.w d0,d1 add in requested level
 move.w d1,sr set desired irq mask level
 movem.l (sp)+,d0/d1 recover old sr, d1
 andi.w #IntMask,d0 isolate old level
 lsr.w #8,d0 move old level to low byte
 rts
#endasm

#endif
