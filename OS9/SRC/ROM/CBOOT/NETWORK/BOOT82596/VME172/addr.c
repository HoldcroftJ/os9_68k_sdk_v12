/*--------------------------------------------------------------------------!
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	?? ??		created etc.											???	!
!	01 93/08/11 merged driver & booter ie.h and moved from .. to mwos/	gkm !
!				os9/src/defs/inet.  Chnaged include accordingly				!
!               ---- OS-9 V3.0 Released ----                                !
!--------------------------------------------------------------------------*/

#include <sysboot.h>
#include <ie.h>

/*
	Determine physical ethernet address
	
	This can be done many ways.  Usually there is some ROM on the local
	hardware containing the unique ethernet address.  One can
	extract the ethernet address from the device descriptor if nothing
	better is available.
*/

get_eaddr_ie(ifdev, eaddr)
register struct ifdev *ifdev;
char *eaddr;
{
	register u_char mask;
	
/*	get address from CSR rom if no address appears in descriptor */
	memcpy(ifdev->if_paddr, BBRAM_ETHERADDR, 6);
}
