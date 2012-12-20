/*--------------------------------------------------------------------------!
! if_init_qe: driver for qe if_init											!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 93/11/20 Cloned from other bootp stuff							arb	!
!   01 99/12/17 Changed 22c00 hard coded addres to DPRBASES+SCC1_BASE   mgh !
!   02 99/12/19 Get base address from fake descriptor                   mgh !
!---------------------------------------------------------------------------!
!                                                                           !
!                  Copyright 1993, Microware Systems Inc.                   !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/
#include <sysboot.h>
#include <qedvr.h>
#include <mc68360.h>
/*#include <systype.h> */


if_init_qe(ifd)
struct ifdev *ifd;
{
	register Ifs ifstat = (Ifs)ifd->if_static;
	register struct qe_desc *qe_desc;
	extern struct qe_desc qe_desc_fake;
	Mbuf mbuf;
	register int i;
	unsigned size;
			

	qe_desc = &qe_desc_fake;
	size = ifd->if_mtu;

	memcpy(&ifstat->v_desc,qe_desc,sizeof(struct qe_desc));
	memcpy(ifd->if_paddr,qe_desc->my_ether_addr,6);

/* Get base addres of SCC1 in Dual Ported Ram area */
/*	ifstat->v_port = (u_int8 *)DPRBASES+SCC1_BASE; */
	ifstat->v_port = qe_desc->port;
	
	qe_init(ifd);
/* signal chip is initialized and is up and running */
	ifstat->v_running = 1;
}
