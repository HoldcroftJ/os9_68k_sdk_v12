/*
 * Copyright 1990, 1993, 1996 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

/*--------------------------------------------------------------------------,
|                                                                           |
|                                                                           |
| Edition History:                                                          |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
| 01 96/08/14 History started.                                        ats   |
| 02 96/08/14 Use local memcopy routine.                              ats   |
| 03 96/08/14 Correct code/data relocation table scanning code.       ats   |
|             ---- OS-9/68K V3.0.3 released ----                            |
|             ---- OS-9/68K V3.1 released ----                              |
`--------------------------------------------------------------------------*/

#include <sysboot.h>

/*------------------------------------------------------------------!
! These are here because we don't want them to get out of reach		!
! causing the linker to make a jump table reference that won't		!
! work because we build it in this file. (i.e. no chicken then egg)	!
!------------------------------------------------------------------*/
#ifdef _UCC
_asm("
allocsize dc.l dsize
btextptr dc.l btext
etextptr dc.l etext
edataptr dc.l edata
");
#else
#asm
allocsize dc.l dsize
btextptr dc.l btext
etextptr dc.l etext
edataptr dc.l edata
#endasm
#endif

void cpidata(), adjptr(), execute(), clruinit();
extern void edataptr();		/* End of idata (where reloc information starts) */
extern void btextptr();		/* beginning  of code */
extern void etextptr();		/* End of code section (where idata starts) */
extern void allocsize();  	/* Size of data + stack */

u_char *_mtop,
	   *_stbot,
	   *_sttop,
	   *doextract();
	   
u_int32	_sbsize, _totmem;

initdata(memptr, relfac)
void	*memptr;
void	*relfac;
{
    setup(memptr, relfac);
	_sbsize = (u_int32) (*(u_int32 *)((void *)allocsize) + (u_char *)memptr);
	_totmem = *(u_int32 *)((void *)allocsize);
	_fixtop(_sbsize);
}

extern u_char end;

_fixtop(stack)
register u_char *stack;
{
	_mtop = &end;
	_sttop = _stbot = stack;
/*	_stkcheck(-252);	*/
}

setup(dataptr, relfac)
register u_int32 *dataptr;  /* Pointer to our vsect data area */
register u_int32 relfac;	/* relocation factor */
{
	register u_int32 *idataptr, *codptr;
    register u_int32 count, dptr, initdataloc, idatacnt;
	auto u_int32 *reloc;
    
	/* get the address where data starts */        
	idataptr = (u_int32 *) ((*(u_char **)((void *)etextptr)) + relfac);
	initdataloc = *idataptr;
    
    /* get how many bytes of idata */
    idatacnt = *((u_int32 *)idataptr + 1);  
    
   	/* get the address where reloc starts */
    reloc = (u_int32 *) ((*(u_char **)((void *)edataptr)) + relfac);
         
	/* dptr is location to copy idata to */        
    dptr = (u_int32)dataptr + initdataloc;  
    
	/* copy initialized data */
    cpidata(idataptr, (u_int32 *)dptr, idatacnt);
    
	/* dptr points to after initialized data */
    dptr += idatacnt;            
    
	/* adjust code ptrs */   
	codptr = (u_int32 *) ((*(u_char **)((void *)btextptr)) + relfac);
    adjptr(dataptr, codptr, &reloc);
    
    /* adjust data ptrs */
	adjptr(dataptr, dataptr, &reloc);    
}

void cpidata(from, to, cnt)
char *from, *to;
u_int32 cnt;
{
    from += 8;
	while (cnt--)
		*to++ = *from++;
}
	

void adjptr(dptr, base, reloc)
register u_int32 dptr;
register u_int32 base;
register u_int16 **reloc;
{
	register u_int16 count, *offset;
	register u_int32 upper, *modpoint;

	upper = (u_int32) (*(*reloc)++ << 16);	/* get upper part of address */
	while (count = *(*reloc)++) {
		for (; count > 0; --count) {
			modpoint = (u_int32 *)(dptr + upper + *(*reloc)++);
	        *modpoint += base;
	    }
		upper = (u_int32) (*(*reloc)++ << 16);
	}
}
