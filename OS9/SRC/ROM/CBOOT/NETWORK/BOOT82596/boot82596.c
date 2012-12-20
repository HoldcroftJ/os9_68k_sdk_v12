/*--------------------------------------------------------------------------!
! bootcnp: driver for I82596 ethernet contoller using internet BOOTP		!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!   00 91/12/12  Cloned from VME374 Bootp code                          djl !
!   01 93/06/17  Modified for UCC compatibility mode                    bat !
!               ---- OS-9 V3.0 Released ----                                !
!---------------------------------------------------------------------------!
!                                                                           !
!         Copyright 1988,1989,1991,1993 by Microware Systems Corporation    !
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
#include <inetboot_p.h>

/*--------------------------------------!
! Static storage variable declarations	!
!--------------------------------------*/

struct ie_desc ie_desc_fake;	/* fake descriptor structure */
struct ifs ie_static;			/* static storage for driver */
static char devname[64];		/* string for device name */
struct ifdev ifdev_ie;			/* ifdev structure for inetboot */

extern int	if_init_ie(), if_term_ie(),
			if_xmit_ie(), if_send_ie(), if_recv_ie();

boot_ie()
{
	register error_code	error;

	init_ie_desc(&ie_desc_fake, &ie_static);	/* set up desc params */	
	
/* initialize some stuff */
	ifdev_ie.if_static = (void*)&ie_static;

	ifdev_ie.if_init = if_init_ie;
	ifdev_ie.if_recv = if_recv_ie;
	ifdev_ie.if_xmit = if_xmit_ie;
	ifdev_ie.if_send = if_send_ie;
	ifdev_ie.if_term = if_term_ie;
	rsprintf(devname, "Intel 82596 @ 0x%X", ie_desc_fake.port);
	ifdev_ie.if_name = devname;
	net_in = (void*)ethernet_in;
	
/*
 * set the mtu for the hardware.  IP frags (when supported by this code) allow
 * booting across arbitrary networks.
 */
	ifdev_ie.if_mtu = ETHER_MAX;
	
/* perform hardware probe, as the 82596 may not be present in the system */

	if (hw_probe(ie_desc_fake.port)) {
		rprintf("boot82596: error accessing %s\n", devname);
    	return E_BUSERR;				/* hardware not present */
    }
    	
	error = inetboot(&ifdev_ie);		/* bonzai... */
	
	return error;			/* return the error returned by "inetboot"	*/

}
