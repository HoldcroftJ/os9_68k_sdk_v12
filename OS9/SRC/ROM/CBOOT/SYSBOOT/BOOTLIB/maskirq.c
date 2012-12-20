/*
 * Copyright 1990, 1993 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

/*----------------------------------------------!
! mask_irq: mask interrupts to level passed in	!
! C-syntax: u_int16 mask_irq(mask)				!
! Returns: Previous contents of status register	!
!----------------------------------------------*/
#ifdef OSK

/*----------------------------------------------!
! WARNING - THIS IS 68xxx SPECIFIC CODE			!
! Note also that stack is kept long-aligned.	!
!----------------------------------------------*/
#ifdef _UCC
_asm("
mask_irq:
 clr.l -(sp) keep stack long-aligned
 move.w sr,2(sp) save current status word
 move.w d0,sr change interrupt mask
 move.l (sp)+,d0 return original mask
 rts
");
#else
#asm
mask_irq:
 clr.l -(sp) keep stack long-aligned
 move.w sr,2(sp) save current status word
 move.w d0,sr change interrupt mask
 move.l (sp)+,d0 return original mask
 rts
#endasm
#endif

#endif OSK


