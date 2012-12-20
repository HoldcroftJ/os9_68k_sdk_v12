/*--------------------------------------------------------------------------!
! bp_timer.c: timer routines for BOOTP code: VME133/68901					!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!               ---- OS-9/68K V2.4 released ----                            !
!	01 90/08/08 Cloned from code originated by J. Laake					kk	!
!   02 93/06/10 "Ultrified"                                             bat !
!               ---- OS-9 V3.0 Released ----                                !
!   03 98/02/02 moved declaration for bpt_test to inetboot.c            mgh !
!               Removed conditional code to set variable                    !
!   04 99/09/20 removed Lots of includes                                mgh !
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

#include <types.h>

#define TMRBASE		0xFFF80000
#define TMRVECT		0x4D
#define TMRLEVEL	0x05

#define IERA	0x07 /* offset to Interrupt enable reg 	*/
#define ISRA	0x0F /* offset to In-service Enable reg	*/
#define IMRA	0x13 /* offset to Interrupt Mask reg		*/
#define VR		0x17 /* offset to Vector reg				*/
#define TACR	0x19 /* offset to Timer A Contl Reg		*/
#define TADR	0x1F /* offset to Timer A data Reg		*/

#define IRQ_ENA	0x20 /* allows timer A to IRQ			*/

       int		bpt_rseed;	/* seed rnd backoff generator */
static u_int32  otexhand;	/* original timer vector exception handler address	*/
	   u_int32	bpt_curtick;/* global 100ms timer counter		*/
static u_char	tmrvect;	/* global storage for actual timer vector	*/
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
! init_timer: Initialize the timer to rollover, produce an interrupt,	!
! and increment a global variable once every "rollval" where "rollval"	!
! is the desired rollover time in NANOseconds.							!
!----------------------------------------------------------------------*/
bpt_init()
{
	register char	*hw = (char *) TMRBASE;
	register short	irqval;
	
	/*------------------------------!
	! Get the REAL device vector	!
	!------------------------------*/
	if ((tmrvect = (hw[VR] & 0xF0)) == 0) {
		hw[VR] = TMRVECT & 0xF0;
		tmrvect = TMRVECT;
	} else tmrvect |= (TMRVECT & 0x0F);

	/*----------------------------------------------------------!
	!The slowest you can get this guy to roll over				!
	! is .0416256 secs. If you want, you can hack all this out	!
	! and raw stuff these easy to plug values:					!
	! For .01 sec: hw[TACR] = 6; and hw[TADR] = 123; (100 hz)	!
	! For .025 sec: hw[TACR] = 7; and hw[TADR] = 154; (40 hz)	!
	! For .040 sec: hw[TACR] = 7; and hw[TADR] = 246; (25 hz)	!
	!----------------------------------------------------------*/
	otexhand = setexcpt(tmrvect, bpt_irqsvc);	/* Set up exception handler	*/
	irqval = maskirq(7);	/* mask interrupts during timer setup	*/
	bpt_curtick = 0;		/* init counter */
	hw[TADR] = 123;			/* move count value to data register	*/
	hw[TACR] = 6;			/* sets timer division value and mode	*/
	hw[IERA] |= IRQ_ENA;	/* enable IRQs	*/
	hw[IMRA] |= IRQ_ENA;	/* set mask		*/
	maskirq(TMRLEVEL-1);		/* enable clock irqs... */

	bpt_rseed = 0xdeadface + (hw[TADR] << 16) + (hw[TADR] << 8) + hw[TADR];
}

bpt_term()
{
	register char	*hw = (char *) TMRBASE;
	short			irqval;
	
	irqval = maskirq(7);			/* mask irqs back up */
	hw[IMRA] &= ~IRQ_ENA;			/* clear mask		*/
	hw[IERA] &= ~IRQ_ENA;			/* disable IRQs		*/
	setexcpt(tmrvect, otexhand);	/* remove exception handler	*/
}

#ifdef _UCC

_asm(" vsect");
_asm("tdec dc.b 10");
_asm(" ends");
_asm("bpt_irqsvc: subq.b #1,tdec(a6)");
_asm(" bne.s bpt10");
_asm(" addq.l #1,bpt_curtick(a6)");
_asm(" move.b #10,tdec(a6)");
_asm("bpt10 addq.l #4,sp");
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
 vsect
tdec dc.b 10
 ends
bpt_irqsvc: subq.b #1,tdec(a6)
 bne.s bpt10
 addq.l #1,bpt_curtick(a6)
 move.b #10,tdec(a6)
bpt10 addq.l #4,sp
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
