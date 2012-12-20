#include <sysboot.h>
#include "ie.h"

#ifdef _UCC	/* New Compiler *************************************************/

#include <const.h>

/*
 *	Put a longword value to shared memory
 *
 *	void ram_putlong(Ifs ifstat, u_int32 addr, u_int32 value)
 */
#define RPL_STK (3 * 4)						/* Stack: a1/a0/rts-pc/value	*/
_asm("ram_putlong: movem.l a0-a1,-(sp)");	/* save temp registers			*/
_asm(" movea.l d0,a0");						/* a0 = ifstat					*/
_asm(" movea.l d1,a1");						/* a1 = addr					*/
_asm(" move.l %0(sp),d0", RPL_STK);			/* d0 = value					*/
_asm(" cmpi.b #2,%0(a0)", OFFSET(Ifs,v_desc.ramwidth));	/* (ramwidth == 2)?	*/
_asm(" bne.s rpl_else");
_asm(" move.w d0,2(a1)");					/* move lo-word value to *addr	*/
_asm(" swap d0");
_asm(" move.w d0,(a1)");					/* move hi-word value to *addr	*/
_asm(" bra.s rpl_rts");						/* goto end						*/
_asm("rpl_else move.l d0,(a1)");			/* else move long value to *addr*/
_asm("rpl_rts movem.l (sp)+,a0-a1");		/* restore temp registers		*/
_asm(" rts");								/* return						*/

/*
 *	Get a longword value from shared memory
 *
 *	u_int32 ram_getlong(Ifs ifstat, u_int32 addr)
 */
_asm("ram_getlong: movem.l a0-a1,-(sp)");	/* save temp registers			*/
_asm(" movea.l d0,a0");						/* a0 = ifstat					*/
_asm(" movea.l d1,a1");						/* a1 = addr					*/
_asm(" cmpi.b #2,%0(a0)", OFFSET(Ifs,v_desc.ramwidth));	/* (ramwidth == 2)?	*/
_asm(" bne.s rgl_else");
_asm(" move.w (a1),d0");					/* move hi-word *addr to d0		*/
_asm(" swap d0");
_asm(" move.w 2(a1),d0");					/* move lo-word *addr to d0		*/
_asm(" bra.s rgl_rts");						/* goto end						*/
_asm("rgl_else move.l (a1),d0");			/* else move long *addr to d0	*/
_asm("rgl_rts movem.l (sp)+,a0-a1");		/* restore temp registers		*/
_asm(" rts");								/* return d0					*/

/*
 *	Swap words of longword
 *
 *	u_int32 ram_swapw(u_int32 value)
 */
_asm("ram_swapw: swap d0");					/* swap words of d0				*/
_asm(" rts");								/* return d0					*/

#else		/* Old Compiler *************************************************/

/*
	Put a longword value to shared memory
*/
void ram_putlong(ifstat, addr, value)
register Ifs ifstat;
register u_int32 addr;
u_int32 value;
{
	if (ifstat->v_desc.ramwidth == 2) {
#if 1
@ movea.l d4,a0
@ move.w _2(sp),(a0)
@ move.w _2+2(sp),2(a0)
#else
		*(((u_int16 *)addr)+0) = value >> 16;
		*(((u_int16 *)addr)+1) = value;
#endif
	} else *(u_int32 *)addr = value;
}

/*
	Get a longword value from shared memory
*/
u_int32 ram_getlong(ifstat, addr)
register Ifs ifstat;
register u_int32 addr;
{
	register u_int32 value;

	if (ifstat->v_desc.ramwidth == 2) {
#if 1
@ movea.l d4,a0
@ move.w (a0),d5
@ swap.w d5
@ move.w 2(a0),d5
#else
		value = *(((u_int16 *)addr)+0);
		value = (value << 16) | *(((u_int16 *)addr)+1);
#endif
	} else value = *(u_int32 *)addr;

	return value;
}

u_int32 ram_swapw(value)
register u_int32 value;
{
#if 1
@	swap d4
@	move.l d4,d0
#else
	return (value >> 16) | (value << 16);
#endif
}

#endif		/* Different Compilers ******************************************/

void ram_memset(ifstat, dst, val, cnt)
register Ifs ifstat;
register char *dst;
register u_int8 val;
register u_int32 cnt;
{
	if (ifstat->v_desc.ramwidth != 4) {
		while (cnt-- > 0) *dst++ = val;
	} else memset(dst, val, cnt);
}

void ram_movemem(ifstat, dst, src, cnt)
register Ifs ifstat;
register char *dst, *src;
register u_int32 cnt;
{
	if (ifstat->v_desc.ramwidth == 2) {
		register unsigned char one_more;
	
		if (cnt == 0) return;

		if ((u_int32)dst & 1) {
			*((u_int8 *)dst)++ = *((u_int8 *)src)++;
			cnt--;
		}
	
		if ((u_int32)src & 1) {
			while (cnt--) {
				*((u_int8 *)dst)++ = *((u_int8 *)src)++;
			}
			return;
		}

		one_more = (cnt & 1) != 0;
	
		for (cnt >>= 1; cnt; --cnt)
			*((u_int16 *)dst)++ = *((u_int16 *)src)++;

		if (one_more)
			*((u_int8 *)dst) = *((u_int8 *)src);
	} else memcpy(dst, src, cnt);
}
