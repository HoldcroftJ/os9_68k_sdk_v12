/*--------------------------------------------------------------------------!
! inetboot.c: read an OS-9 bootfile from BOOTP/TFTP server on internet		!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!               ---- OS-9/68K V2.4 released ----                            !
!	01 90/08/08 Cloned from diskboot									kk	!
!	02 90/20/25 Now retries forever.  Diskless nodes like this.			kk	!
!	03 91/12/02	memset() arguments in wrong order						kk	!
!	04 91/12/18 cleared out net_inp/outbufs to keep from confusing		kk	!
!				kernel during module search									!
!   05 93/06/17 Modified for UCC compatibility mode.                    bwt !
!   06 93/08/09 replaced use of inetboot.h with explicit includes (4)   gkm !
!				replaced usage of the save_bootp structure with 			!
!				bootp_save which is defined in the file bootpsave.h			!
!               ---- OS-9 V3.0 Released ----                                !
!	07 94/01/07 call if_term & bpt_term in case bootp fails to shut 	gkm !
!				down chip & timer.                                          !
!   08 96/06/21 changed ifdev variable to ifd for ISP 2.1 defs compat   mgh !
!      96/09/19 ---- OS-9 V3.0.3 Released ----                              !
!   11 97/12/07 Added history.h and EDITION printing                    pjw !
!   12 98/01/27 Added try counter to get out of infinite try on tftp    mgh !
!               part of "do" loop. ROM menu can force multiple entries.     !
!				Added printer control and timer test initialization runtime !
!               switch locations.  Added print control to messages          !
!               Adjusted loop structure for trys of bootp/tftp              !
!   13 98/01/27 Added timestamp display, more logging messages          kimk!
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

#include <sysboot.h>
/*#include "if.h"*/
#include "lip.h"
#include "inetboot_p.h"
#include "bootp.h"
#include <bootpsave.h>
#include "history.h"
void *(*net_in)();		/* network input call */

/*--------------------------------------!
! Static storage variable declarations	!
!--------------------------------------*/
char net_inpbuf[NET_BUFSIZE];
char net_outbuf[NET_BUFSIZE];


/*
 * A pointer to this struct will be available in D_Bootp when the
 * kernel is up.  Then some ifdev driver can use this to set it's
 * IP address to this one that bootp discovered.
 */
#if 0
 * Already defined in bootpsave.h as bootp_save
 *struct {
 *	u_int32 magic1, magic2;	/* some magic numbers to ID this struct */
 *	u_int32 myip;			/* IP address used by BOOTP */
 *	u_int32 bootsize;		/* size of bootfile we read */
 *	struct bootp *bprp;		/* ptr to BOOTP server response message */
 *	Ifdev if;				/* the device we used to boot */
 *} save_bootp;
 *
#endif
 
struct bootp_save save_bootp;
u_int16 msgprint=1;				/* Flag byte to control msg printing */
u_int16 bpt_test=0; 			/* Flag to control timer test */
extern VectTbl();		/* points to Rom Vector Table */

/*
 * display timestamp
 */
void
TS(void)
{
	if (bpt_test > 1)
		rprintf("%4u.%u |", bpt_curtick/10, bpt_curtick%10);
}

/*--------------------------------------------------------------!
! diskboot: Read a bootfile from a hard drive or floppy disk	!
!--------------------------------------------------------------*/
error_code inetboot(ifd)
Ifdev ifd;
{
	error_code error;
	register u_int32 etime;
	u_int16 *mpaddr;
	u_int16 *ttaddr;
	int try=0;	
#define TRYCOUNT 3			/* Number of times to try bootp/tftp sequence */
	
	mpaddr = &msgprint;
	ttaddr = &bpt_test;
	bpt_init();					/* initialize 100ms timebase */
	memset(&save_bootp, 0, sizeof(save_bootp));
	
	/*
	 * this code mainly used for debugging the timer irq routine.  The first
	 * goal is to get the timer ticking in the first place.  The next is to
	 * get the timer to tick at the correct rate (100ms/tick).  The bpt_test
	 * variable can be set in bpt_init() or just poked with rombug for a
	 * temporary test.  If the messages appear once per second, the timer
	 * is working correctly.
	 */
	if (bpt_test) {
		register u_int32 btime, nsec = 1;

		TS(), rprintf("inetboot: start 5 sec timer test\n");
		etime = (btime = bpt_curtick) + 50;
		while (bpt_curtick < etime)
			if (btime + nsec * 10 == bpt_curtick)
				TS(), rprintf("%d one-thousand...\n", nsec++);
		TS(), rprintf("inetboot: timer test finished\n");
	}

	TS(), rprintf("inetboot: (Edition %d) using %s MTU=%d\n", EDITION, ifd->if_name, ifd->if_mtu);
    TS(), rprintf("inetboot: msgpcntl=%x:16, ttcntl=%x:16\n",mpaddr,ttaddr);
	(*ifd->if_init)(ifd);		/* initialize driver */
	

	do {
tryagain:
		try++;
		if (msgprint) TS(), rprintf("inetboot: Bootp/Tftp sequence try: %d/%d\n",try,TRYCOUNT);
		/* use BOOTP to get boot info .. BOOTP Retrys internally*/
		if (bootp(ifd) == NULL) {
			if (try<TRYCOUNT)
				goto tryagain;			/* Allow lots of trys for autoboot folks */
			else {
				(*ifd->if_term)(ifd);	/* terminate driver */
				bpt_term();				/* shutdown timer */
				return ETIMEDOUT;		/* no bootp response */
			}
		}
		error = EINPROGRESS;
		
/* use IP addresses given by BOOTP response */
		memcpy(&(IA_SIN(ifd)->sin_addr).s_addr, &bprp.bp_yiaddr, 4);
		memcpy(&(ID_SIN(ifd)->sin_addr).s_addr, &bprp.bp_siaddr, 4);

/* allocate memory for the bootfile */
		if ((error = getbootmem(bootsize = in_bootsize * 512)) == SUCCESS)
			if ((error = tftp(ifd, bootram, in_bootsize)) == SUCCESS)
				break;

					

/* return the memory for retry */
		if (bootram != NULL && memsize != 0) {
			insert(memsize, bootram);
			bootram = NULL;
			memsize = 0;
		}
	} while ((error != SUCCESS) && (try < TRYCOUNT));	

	(*ifd->if_term)(ifd);		/* terminate driver */
	bpt_term();						/* shutdown timer */
	
/* now save some stuff we used in case some IP driver wants to know */
	if (error == SUCCESS) {
		memcpy(&save_bootp.magic1, BOOTP_MAGIC, 8);	  /*defined in bootpsave.h*/
		memcpy(&save_bootp.myip, &bprp.bp_yiaddr, 4);
		save_bootp.bprp = &bprp;
		save_bootp.ifdev = ifd;
		save_bootp.bootsize = bootsize;
		stash_ipaddr(&save_bootp);
	}

/*
 * the kernel seems to dig into rombug's memory looking for modules.
 * It can get confused by seeing a valid module in our I/O buffers so
 * zap them to be sure.  The kernel really shouldn't do this!  Took a
 * lot of debugging time to find this!
 */
 	memset(net_inpbuf, 0, sizeof(net_inpbuf));
 	memset(net_outbuf, 0, sizeof(net_outbuf));
 	
	return error;
}
#ifdef _UCC
_asm ("
D_BootP equ 0x7a8				/* defined in 2.4 kernel */
stash_ipaddr: movem.l a0,-(sp)
 move.l VectTbl,a0 get system global pointer
 move.l d0,D_BootP(a0) save into ioglobs: kernel doesn't clear these (whew!)
 movem.l (sp)+,a0
 rts
");
#else
#asm 
D_BootP equ 0x7a8				/* defined in 2.4 kernel */
stash_ipaddr: movem.l a0,-(sp)
 move.l VectTbl,a0 get system global pointer
 move.l d0,D_BootP(a0) save into ioglobs: kernel doesn't clear these (whew!)
 movem.l (sp)+,a0
 rts
#endasm
#endif
