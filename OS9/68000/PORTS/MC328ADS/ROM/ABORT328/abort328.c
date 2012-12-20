/*
 * $Header:   /h0/MWOS/OS9/68000/PORTS/MC328ADS/ROM/ABORT328/VCS/abort328.c_v   1.1   24 Jul 1996 10:41:38   alexs  $
 */

/*--------------------------------------------------------------------------,
|                                                                           |
|  Copyright 1996 by Microware Systems Corporation                          |
|                          Reproduced Under License                         |
|                                                                           |
|  This source code is the proprietary confidential property of Microware   |
|  Systems Corporation, and is provided to licensee for documentation and   |
|  educational purposes only. Reproduction, publication, or distribution    |
|  in any form to any party other than the licensee is strictly prohibited. |
|                                                                           |
|---------------------------------------------------------------------------|
| abort328.c - ROM Abort switch module for MC328ADS                         |
|                                                                           |
| Edition History:                                                          |
|                                                                           |
| Ed#   Date    Changes                                                 By  |
| --- --------  ------------------------------------------------------- --- |
|   1 96/07/22  Created.                                                ats |
|   2 96/07/23  Call through ROM jump table if no dbg_svcs record.      ats |
|   3 96/07/30  Correct entry/return stack setup.                       ats |
`--------------------------------------------------------------------------*/

#define	EDITION		3

_asm("_sysedit: equ %0", __obj_constant(EDITION));

#define NEWINFO
#include <rom.h>

#include <types.h>
#include <const.h>

#include <ROM/rjt.h>

/* dbentry calling code */

u_int32 get_dbgentry(Rominfo rinf)
{
	Dbg_svcs debug = rinf->dbg;
	u_int32 dbg_entry = NULL;
	
	if (debug)
		dbg_entry = (u_int32)((void *)(debug->dbg_entry));
		
	if (!dbg_entry) {
		Rom_svcs romsvc = rinf->rom;
		New_rjt nrjt = (New_rjt)((void *)romsvc->rom_start);
		
		dbg_entry = (u_int32) &(nrjt->branch[DEBUG_ENTRY]);
	}
	return(dbg_entry);
}

/* abort switch handler - installed in RAM */

extern void abort_handler();
extern void handler_end();
extern void plug1();
extern void plug2();

_asm("
 use <defsfile>
 opt -b
abort_handler:
 lea.l -6(sp),sp adjust & align stack (because of 6-byte 68000 frame)
 move.w 6(sp),2(sp) move stacked sr
 movem.l d0/a0,-(sp) save the resgsters needed
 movea.l #SCRMap,a0 base address of the Systems Integration Module
 ori.l #SIRQ7,ISR(a0) clear IRQ7 interrupt
plug1:
 movea.l #0,a0 address of get_dbgentry()
plug2:
 move.l #0,d0 pointer to rinf
 jsr (a0) call it
 move.l d0,$0c(sp) continue at entry address
 movem.l (sp)+,d0/a0 restore registers
 lea.l 2(sp),sp adjust position of stack
 rtr enter debugger
handler_end:
 opt b
 ");

void clear_abtint()
{
}

/* module initialization */

error_code p2start(Rominfo rinf, u_char *glbls)
{
	u_char *begin, *end, *handler, *to;
	u_int32 size, stat;
	u_int32 abortvector, *vectorbase, *vectorslot;
	
	/* allocate RAM for handler code */
	begin = (u_char *)((void *)abort_handler);
	end = (u_char *)((void *)handler_end);
	size = end - begin;
	if ((stat = rinf->memory->rom_malloc(&size,
			(char **)&handler, rinf)) != SUCCESS)
		return(stat);
		
	/* copy handler code */
	for (to = handler; begin < end; *to++ = *begin++);
	
	/* patch in values */
	*((u_int32 *)(handler + (u_int32)plug1 - (u_int32)abort_handler + 2)) =
		(u_int32)((void *)get_dbgentry);
	*((u_int32 *)(handler + (u_int32)plug2 - (u_int32)abort_handler + 2)) =
		(u_int32)rinf;
	
	/* point RAM-based vector to handler */
	abortvector = 0x18 + 7;
	vectorbase = rinf->rom->rom_vectors;
	vectorslot = vectorbase + abortvector;
	*vectorslot = (u_int32)handler;
		
	return (SUCCESS);
}

