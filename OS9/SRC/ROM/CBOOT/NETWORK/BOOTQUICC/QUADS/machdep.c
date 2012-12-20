/*--------------------------------------------------------------------------!
! initdesc: initialize hardware-dependent params for backplane boot driver	!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 92/05/13 Cloned from other 81596 stuff							arb	!
!   01 99/12/17 Changed hard coded address to defs                      mgh !
!---------------------------------------------------------------------------!
!                                                                           !
!         Copyright 1993 by Microware Systems Inc.
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware       !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include <sysboot.h>
#include "../qedvr.h"
#include <inetboot_p.h>



qe_init_desc(fake, qe_static)
register struct qe_desc		*fake;
register struct ifs			*qe_static;
{
	fake->port = (u_int8 *)DPRBASES+SCC1_BASE;	/* port base address */
	fake->max_rbufs = 4;	/* number of rfd structures */
	memcpy(fake->my_ether_addr,get_enet_addr(),6);
	memcpy(&qe_static->v_desc,fake,sizeof(struct qe_desc));
} 
