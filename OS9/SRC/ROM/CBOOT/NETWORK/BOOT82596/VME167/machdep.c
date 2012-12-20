/*--------------------------------------------------------------------------!
! initdesc: initialize hardware-dependent parameters for LANCE boot driver	!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 91/12/13 Cloned from other VME147 stuff							djl	!
!	01 93/08/11 merged driver & booter ie.h and moved from .. to mwos/	gkm !
!				os9/src/defs/inet.  Chnaged include accordingly				!
!               ---- OS-9 V3.0 Released ----                                !
!---------------------------------------------------------------------------!
!                                                                           !
!         Copyright 1988,1989,1990,1993 by Microware Systems Corporation    !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/
#include <types.h>
#include <mcchip.h>
#include <sysboot.h>
#include <ie.h>
#include <inetboot_p.h>

init_ie_desc(fake, ie_static)
struct ie_desc		*fake;
struct ifs			ie_static;
{
	fake->port = 0xfff46000;		/* port base address */
	fake->ramaddr = (u_int32) 0;	/* base addr for A24 mem window */
	fake->ramadjust = 0;			/* NOT USED */
	fake->port_off = 0x00;			/* offset from device to port */
	fake->ca_off = 0x04;			/* offset from device to channel atn */
	fake->irq_off = 0x00;			/* offset from device to irq address */
	fake->idpromaddr = 0x0;			/* address of ID prom */
	fake->max_rfd = MAX_RFD;		/* number of rfd structures */
	fake->max_cbl = MAX_CBL;		/* number of command blocks */
} 

/* let the pipeline's clean out
 */
 
#ifdef _UCC
_asm ("
nop: nop
 rts
");
#else
#asm
nop: nop
 rts
#endasm
#endif
