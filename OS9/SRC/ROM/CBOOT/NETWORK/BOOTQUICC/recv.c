/*--------------------------------------------------------------------------!
! if_recv_qe: driver for Quads if_recv			    						!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 93/11/20 Cloned from other Cboot stuff							arb	!
!   01 00/02/04 Removed 5 tick polling delay							kimk!
!---------------------------------------------------------------------------!
!                                                                           !
!         Copyright 1993 Microware Systems Corporation       				!
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

if_recv_qe(ifd, buf, timeout)
register struct ifdev *ifd;
char *buf;
int *timeout;
{
	register Ifs 	ifstat = (Ifs) ifd->if_static;
	register int rlen = 0;
	register u_int32 etime, ntime;
	struct ether_header *eh;
							
/* initialize timer values */
	etime = bpt_curtick + *timeout;
/* wait until data comes in */	
	while(1)
		{
		ntime = bpt_curtick;
		if ((*timeout = etime - ntime) <= 0) 
			return 0;
		if(rlen = qe_recv(ifd,buf))
			break;
		}
	return rlen;
}

