/*--------------------------------------------------------------------------!
! bootqe: driver for Quads ethernet contoller using internet BOOTP		!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!   00 93/11/20  Cloned from I82596 Bootp code                          arb !
!   01 00/02/04  Changed banner QUADS to QUICC							kimk!
!---------------------------------------------------------------------------!
!                                                                           !
!         Copyright 1993by Microware Systems Inc.
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
#include <inetboot_p.h>

/*--------------------------------------!
! Static storage variable declarations	!
!--------------------------------------*/

struct qe_desc qe_desc_fake;	/* fake descriptor structure */
struct ifs qe_static;			/* static storage for driver */
static char devname[64];		/* string for device name */
struct ifdev ifdev_qe;			/* ifdev structure for inetboot */

/*extern int32	if_init_qe(), if_term_qe(),
*			if_xmit_qe(), if_send_qe(), if_recv_qe();
*/
extern error_code	if_init_qe(), if_term_qe(),
			if_xmit_qe(), if_send_qe(), if_recv_qe();

bootqe()
{
	register error_code	error;

	qe_init_desc(&qe_desc_fake, &qe_static);	/* set up desc params */	
	
/* initialize some stuff */
	ifdev_qe.if_static = (void*)&qe_static;

	ifdev_qe.if_init = if_init_qe;
	ifdev_qe.if_recv = if_recv_qe;
	ifdev_qe.if_xmit = if_xmit_qe;
	ifdev_qe.if_send = if_send_qe;
	ifdev_qe.if_term = if_term_qe;
	rsprintf(devname, "QUICC Ethernet @ 0x%X", qe_desc_fake.port);
	ifdev_qe.if_name = devname;
	net_in = (void*)ethernet_in;
	
/*
 * set the mtu for the hardware.  IP frags (when supported by this code) allow
 * booting across arbitrary networks.
 */
	ifdev_qe.if_mtu = ETHERMTU;
	
	error = inetboot(&ifdev_qe);		/* bonzai... */
	
	return error;			/* return the error returned by "inetboot"	*/

}
