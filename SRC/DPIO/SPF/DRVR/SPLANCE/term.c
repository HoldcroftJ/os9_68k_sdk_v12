/****************************************************************************
 *  $Workfile:   term.c  $
 *  $Revision:   18.1  $
 *  $Modtime:   23 Sep 1993 11:40:00  $
 *  $Date:   23 Sep 1993 11:40:56  $
 *  $Author:   brentt  
 ****************************************************************************
 *  Copyright 1988 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date       Comments                                           By   *
 *  --- --------    ----------------------------------------------- ------- *
 *    0 ??/??/??    Birth                                               ??? *
 *      09/16/93    <***** ISP 2.0 Release ***********************>         *
 *	27FEB96	    modified for LANCE               Bill West Inc.     bw  *
 *    8 03/05/97    In free_rmds, account for the Dev_list ptrs to     sr   *
 *                  get to the start of the mbuf.                           *
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release*********>         *
 *      05/05/97    <***** SPF LAN Comm Pack v3.1 Release*********>         *
 *     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              *
 ****************************************************************************/
    

#include <SPLANCE/defs.h>
/* #include <setsys.h> */

error_code term_hdw(Dev_list deventry)
{
	Spf_lustat 	lustat = (Spf_lustat)(deventry->v_lu_stat);
	Ifs 		ifstat = (Ifs)(&lustat->ifstat);

/* if we have already been called, forget it! */
	if (!ifstat->v_running)
		return E_SEEK;		/* (sic) */
			
/* stop things... */
	ifstat->v_running = 0;
	ifstat->v_leregs->rdp = LE_TSTOP;
#ifdef notdef
	while (ifstat->v_sysio.v_busy) {				/* driver busy? */
		ifstat->v_sysio.v_dumm1[7] = (int)get_procd();	/* record our pid */
		f_nproc();									/* zzzz */
	}
#endif

#ifndef LCLMEM
/* get back our receive mbufs */
	free_rmds(ifstat);
#endif

/* free up the driver's memory requests */
	freedmem(lustat);
	
	return(SUCCESS);
}

#ifdef notdef
int get_procd()
{
	return (int)_getsys(D_Proc);
}
#ifdef _UCC
_asm("_getsys movem.l d0-d2,-(sp)");
_asm(" moveq.l #4,d1 size of variable");
_asm(" bset #31,d1 examine request");
_asm(" os9 F$SetSys");
_asm(" move.l d2,(sp)");
_asm(" movem.l (sp)+,d0-d2");
_asm(" rts");
#else
/*#asm*/
_getsys movem.l d0-d2,-(sp)
 moveq.l #4,d1 size of variable
 bset #31,d1 examine request
 os9 F$SetSys
 move.l d2,(sp)
 movem.l (sp)+,d0-d2
 rts
/*#endasm*/
#endif
#endif

#ifndef LCLMEM
/*
	Place a receive mbuf on a rmd entry.  Called at initialization time
	and by irq service when mbuf error occurs.
*/
error_code free_rmds(Ifs ifstat)
{
	Le_md mdp;
	Mbuf mb;
	u_char ncmd;
	/* ncnt = ifstat->v_rpcnt; */
	
	mdp = &(*ifstat->v_recv_ring)[0];

	for (ncmd = 0; ncmd < ifstat->v_rpcnt;  ncmd++, ++mdp) {
		if (mdp->md_flag) {
			/* Subtract 2 times the sizeof(Dev_list) for the two
			 * dev_entry pointers.
			 */
			mb = (Mbuf)(ifstat->v_hiaddr |
				((u_int32)mdp->md_hiaddr << 16) |
				 (u_int32)mdp->md_loaddr -
				 (2 * sizeof(Dev_list)));
			mb--;				/* back up to mbuf header */
			m_free_p(mb);
		}
	}
	return 0;
}
#endif
