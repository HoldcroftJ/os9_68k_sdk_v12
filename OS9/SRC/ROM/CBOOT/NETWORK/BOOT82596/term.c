/*--------------------------------------------------------------------------!
! if_term_ie: driver for I82596 if_term										!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 91/12/13 Cloned from other VME374 stuff							djl	!
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

#include <sysboot.h>
#include "ie.h"

if_term_ie(ifdev)
register struct ifdev *ifdev;
{
	register Ifs ifstat = (Ifs)ifdev->if_static;
	extern struct ie_desc ie_desc_fake;

	I82596_RESET(ifstat);
	if (ie_desc_fake.ramaddr == 0) 
		insert(ifstat->v_shramsize, ifstat->v_shram);
	
#if defined(mz8554)
	*MZ8554_ETHER_CONTROL(ifstat->v_addr) = 0;
#endif

	return 0;
}
