/*
 * misc - miscellaneous functions for ncr5380 low level module.
 */
 
/* Copyright 1988, 1989, 1990, 1991, 1993 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include	"scsi5380.h"

/*
 * clearmem: clear a number of bytes
 */
int clearmem(mptr, size)
register char	*mptr;			/* memory block ptr  */
register int	size;			/* memory size  */
{
	register int	i;			/* general purpose int  */
	
	for (i = 0; i < size; i++)
		*mptr++ = 0;
}

/*
 * lxtoa(stg, num) - convert long to 8 ascii hex digits 
 */
lxtoa(stg, num)
register char	*stg;
register long	num;
{
	register int	i;
	
	for (i = 7; i >= 0; i--, num >>= 4)
		stg[i] = hexchar(num & 0xf);
	stg[8] = '\0';		/* terminate  */
}

/*
 * int hexchar(n) - return ascii hex equivalent of n 
 */
int hexchar(n)
register int	n;
{
	if (n > 9)
		n += 0x27;		/* for range a - f  */
	return (n + 0x30);
}

/*
 * strcpy(s1, s2) - another version of string copy
 */
strcpy(s1, s2)
register char	*s1, *s2;
{
	while (*s1++ = *s2++);
}

/*
 * set3l(int3, num) - make num into 3 byte integer
 */
int set3l(int3, num)
register char	*int3;
register int	num;
{
	register int	i;
	
	for (i = 2; i >= 0; i--, num >>= 8)
		int3[i] = num & 0xff;
}

/*
 * strlen(str) - find the length of a null terminated string
 */
int strlen(str)
register char	*str;
{
	register int	count = 0;

	while(*str++ != 0)
		count++;
	return count;
}

/*
 * int adjaddr(addr) - translate local address to vme address
 */
int adjaddr(addr)
int addr;
{
	return addr;
}

#ifdef _UCC
_asm(" use miscasm.a");
#else
@ use miscasm.a
#endif

