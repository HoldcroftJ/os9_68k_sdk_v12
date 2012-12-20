/*--------------------------------------------------------------------------!
! if_term_qe: driver for quads if_term										!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 93/11/20 Cloned from other bootp stuff							arb	!
!---------------------------------------------------------------------------!
!                                                                           !
!         Copyright 1993 by Microware Systems Corporation       !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include <sysboot.h>
#include "qedvr.h"

if_term_qe(ifd)
register struct ifdev *ifd;
{
	register Ifs ifstat = (Ifs)ifd->if_static;

	qe_term(ifstat);	
	return 0;
}
