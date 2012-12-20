/* _________________________ misc.c _________________________
 * 
 *  This module contains the following miscellaneous low level 
 * functions for the wd33c93:
 *
 * clearmem(), clears a number of bytes
 * lxtoa(), converts a long to 8 ascii hex digits
 * hexchar(n), returns an ascii hex equivalent of n
 * strcpy(), another version of string copy
 * set3l(), makes a number into a 3 byte integer
 * adjaddr(), translates a local address to a VMEbus address
 * mask_irq(), masks interrupts to level of scsi device
 * wd_command(), issues a command to the wd33c93 chip
 * _T$UMul, local copy of T$UMul
 * qcopy() from miscqcpy.a is included below using the use
 *      assembly directive, quick copy for under 256 bytes
 * mask_irq() from miscirq.a is included below using the use
 *      assembly directive, mask interrupts to level of scsi device
 */
 
/*
 * Copyright 1988, 1989, 1990, 1993, 1995 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	"scsi33c93.h"

/*
 * clearmem(mptr, size) - clear a number of bytes
 *		Call with:
 *			mptr, memory block pointer
 *			size, memory size
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int clearmem
(
	register char   *mptr,
	register int    size
)
#else
int clearmem(mptr, size)
register char	*mptr;	
register int	size;	
#endif
{
	register int i;		/* general purpose int */
	
	for (i = 0; i < size; i++)
		*mptr++ = 0;
}

/*
 * function to convert a long to 8 ascii hex digits 
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void lxtoa
(
	register char   *stg,
	register long   num
)
#else 
lxtoa(stg, num)
register char	*stg;
register long	num;
#endif
{
	register int	i;
	
	for (i = 7; i >= 0; i--, num >>= 4)
		stg[i] = hexchar(num & 0xf);
	stg[8] = '\0';		/* terminate */
}

/*
 * function that returns an ascii hex equivalent of a number 
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int hexchar
(
	register int n
)
#else
int hexchar(n)
register int n;
#endif
{
	if (n > 9)
		n += 0x27;		/* for range a - f */
	return (n + 0x30);
}

/*
 * another version of string copy
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void strcpy
(
	register char *s1, 
	register char *s2
)
#else 
strcpy(s1, s2)
register char *s1, *s2;
#endif
{
	while (*s1++ = *s2++);
}

/*
 * function to make a number into a 3 byte integer
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int set3l
(
	register char   *int3,
	register int    num
)
#else 
int set3l(int3, num)
register char	*int3;
register int	num;
#endif
{
	register int	i;
	
	for (i = 2; i >= 0; i--, num >>= 8)
		int3[i] = num & 0xff;
}

/*
 * function to translate a local address to a VMEbus address
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int adjaddr
(
	int addr
)
#else 
int adjaddr(addr)
int addr;
#endif
{
	return addr;
}

/*
 * int mask_irq(mask) - mask interrupts to level of scsi device
 */
#ifdef _OSK
#ifdef _UCC
_asm(" use ./miscirq.a");
#else
@ use ./miscirq.a
#endif
#endif

/*
 * function that issues a one-byte command to the wd33c93 chip
 *		Call with:
 *			cmd, wd33c93 command to issue
 *			lst, low-level static storage
 *			hw, wd33c93 hardware pointer
 *		Return:
 *			Nothing
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
void wd_command
(
	int			cmd,
	Lstat      	lst,    
	Hardware	hw
)
#else 
void wd_command(cmd, lst, hw)
int			cmd;
Lstat		lst;
Hardware	hw;	
#endif
{
	int old_mask;		/* save of 68xxx sr */
	int mask_irq();		/* from above "use" directive */
 	
	old_mask = mask_irq(lst->mask_level);	/* mask interrupts */
	HW(address_reg) = (char) COMMAND;		/* point at command register */
	HW(reg_file) = (char) cmd;				/* issue command */
	mask_irq(old_mask);						/* restore interrupt mask */
}

/*  
 *  Local copy of T$UMul
 *
 *   Some forms of access reguarding taking the address of a structure
 *      require that a multiply be performed.  In our case, both 
 *      values are 16 bits and hence the 32 bit result is ok.  This
 *      is not a true 
 */
#ifndef _UCC
#asm
_T$UMul:
	link a5,#0
	mulu d1,d0    d1.w * d0.w 
	unlk a5
	 rts
#endasm	 
#endif

#ifdef _UCC
_asm(" use ./miscqcpy.a");
#else
#asm
 use ./miscqcpy.a
#endasm
#endif
