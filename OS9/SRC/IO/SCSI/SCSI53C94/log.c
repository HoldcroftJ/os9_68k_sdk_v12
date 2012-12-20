#include <stdio.h>
/*#include <procid.h>*/
#include <setsys.h>
/*#include <module.h>*/

#include "scsi53c94.h"

#ifdef _UCC
_asm("_sysattr: equ $A001");
#else
@_sysattr: equ $A001
#endif

#define STDIN fileno(stdin)
#define STDOUT fileno(stdout)

main(argc,argv)
int argc;
char *argv[];
{
	register procid *prc = sysglob(procid *, D_Proc);
	register Datmod dmod;
	register Lstat	lst;
	register int i;
	int reset;
			
	/* fix up stdio for system state process */
	stdin->_fd = prc->_path[0]; prc->_path[0] = 0;
	stdout->_fd = prc->_path[1]; prc->_path[1] = 0;
	stderr->_fd = prc->_path[2]; prc->_path[2] = 0;

#ifndef RECORD
	exit(_errmsg(1,"no status record space in data module\n"));
#else
	if (argc == 1)
		reset = 0;
	else if (argc == 2 && argv[1][0] == '-' && argv[1][2] == 0) {
		switch(argv[1][1]) {
		  case 'r': reset = 1; break;
		  case '?': exit(_errmsg(1,"use -r flag to reset log\n"));
		  default:	exit(_errmsg(1,"unknown flag '-%c'\n",argv[1][1]));
		}
	}
	else
		exit(_errmsg(1,"unknown parameter(s)\n"));

	if ((dmod = (Datmod)modlink("dat16000000", mktypelang(MT_DATA, ML_ANY)))
		== (Datmod)(-1))
		exit(_errmsg(errno,"can't find data module\n"));

	dmod = (Datmod)((char*)dmod+((mod_exec *)dmod)->_mexec);
	lst = dmod->dm_statics;

	if (reset) {
		int old = mask_irq(0x2700);
		register u_char *p = lst->stats;
		while (p < &lst->stats[STATMAX])
			*p++ = 0;
		lst->statcount = 0;
		lst->nxtstat = lst->stats;
		mask_irq(old);
	}
	
	printf("Status count = %d, address = $%06X\n",lst->statcount,lst->stats);
	for (i = 0; i < lst->statcount;) {
		if (prc->_signal) {
			clearerr(stdout);
			putchar('\n');
			exit(prc->_signal);
		}
		if ((i%16)==0) {
			printf("%04X:",i);
		}
		printf(" %02X",lst->stats[i]);
		if (((++i)%16)==0)
			putchar('\n');
	}
	
	if (i%16) putchar('\n');
	printf("-----------------------------------------------\n");
#endif
}

/*
 * int mask_irq(mask) - mask interrupts to level of scsi device
 */
#ifdef OSK
/*
 * WARNING - THIS IS 68xxx SPECIFIC CODE
 * Note also that stack is kept long-aligned.
 */
#ifdef _UCC
_asm("
mask_irq:
 link a5,#0
 clr.l -(sp) keep stack long-aligned
 move.w sr,2(sp) save current status word
 move.w d0,sr mask interrupts
 move.l (sp)+,d0 return original mask
 unlk a5
 rts
");
#else
#asm
mask_irq:
 link a5,#0
 clr.l -(sp) keep stack long-aligned
 move.w sr,2(sp) save current status word
 move.w d0,sr mask interrupts
 move.l (sp)+,d0 return original mask
 unlk a5
 rts
#endasm
#endif
#endif OSK

