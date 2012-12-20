/*--------------------------------------------------------------------------!
! if_init_ie: driver for I82596 if_init										!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 91/12/13 Cloned from other VME374 stuff							djl	!
!   01 93/06/17 Modified for UCC compatibility mode                     bat !
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
#include <inetboot_p.h>

extern void ie_irq();

if_init_ie(ifdev)
register struct ifdev *ifdev;
{
	register Ifs ifstat = (Ifs)ifdev->if_static;
	register struct ifdevdd *ddptr;
	register u_int8 *ptr, *idptr, ctemp;
	register struct ie_desc *ie_desc;
	auto u_int32 tempsize, count, status;
	register u_short old_sr;
	extern struct ie_desc ie_desc_fake;

	ie_desc = &ie_desc_fake;
	
/* important for knowing how much memory we need */	
	ifstat->v_ncbl = ie_desc->max_cbl;		/* number of command blocks */
	ifstat->v_nrfd = ie_desc->max_rfd;		/* number of receive blocks */

/* how much memory do we need? */
	ifstat->v_shramsize = i82596totmem(ifstat);

/* see if we have a ram area reserved.  if not, allocate it dynamically */

	if ((ifstat->v_shram = (void *) ie_desc->ramaddr) == 0) {
		if (extract(&ifstat->v_shramsize, &ifstat->v_shram) != SUCCESS) {
			rprintf("ie: Couldn't get %d bytes of memory for i82596 booter\n",
							ifstat->v_shramsize);
			while (1);
		}
	}
	
	ifstat->v_ramadjust = ie_desc->ramadjust;		/* get adjustment value */
	
/* initialize pointers to chip registers */

	ifstat->v_addr = (u_int8 *) ie_desc->port;		/* set the port address */
	ifstat->v_port = ifstat->v_addr + ie_desc->port_off;
	ifstat->v_ca   = ifstat->v_addr + ie_desc->ca_off;
	ifstat->v_irq  = ifstat->v_addr + ie_desc->irq_off;
	
/* get data from ID prom */
#if defined(mz8554)
	idptr = (u_int8 *)(ie_desc->idpromaddr + 3);
	for (ptr = ifstat->v_idprom, count=32; count--; idptr += 4)
		*ptr++ = *idptr;
#elif defined(hkvf82596)
	idptr = (u_int8 *)(IDPROMADDR + 0x2047);
	for (ptr = ifstat->v_idprom, count=8; count--; idptr += 8)
		*ptr++ = *idptr;
#endif

/* setup the physical ethernet address */
	get_eaddr_ie(ifdev, ifstat->v_idprom);
	
#if defined(mz8554)
/*
 * Now do some board initialization for the MZ8554
 */
	if (*MZ8554_ETHER_CONTROL(ifstat->v_addr) & MZ8554_INT_ASSERTED)
		*MZ8554_INT_CLEAR(ifstat->v_addr) = 0;

	ctemp = ETHER_CONTROL_ENABLE;
	*MZ8554_ETHER_CONTROL(ifstat->v_addr) = ctemp;

	ctemp = RAM_ENABLE | ((u_int32)ifstat->v_shram >> 16);
	*MZ8554_RAM_BASE_ADRS(ifstat->v_addr) =	ctemp;

#endif

/* reset the chip (just in case) */
	I82596_RESET(ifstat);
	nop(); nop();
			
/* clear out the shared ram */
	memset(ifstat->v_shram, 0, ifstat->v_shramsize);
	
/* now run selftest on device */
	((STR *)ifstat->v_shram)->str_romsig = ~0;
	((STR *)ifstat->v_shram)->str_result = ~0;
	I82596_SELFTEST(ifstat, (u_int32)ifstat->v_shram);

/* wait for it to complete*/
	ifstat->v_iestat.stloops = 65536;
	while (ifstat->v_iestat.stloops-- &&
			((STR *)ifstat->v_shram)->str_result == ~0)
		;
	if (((STR *)ifstat->v_shram)->str_result != 0)
		if (msgprint>=2) rprintf("ie: i82596 self test result: %08x\n", 
							((STR *)ifstat->v_shram)->str_result);

/* initialize shared ram for chip */
	if (count = i82596meminit(ifstat))
		return count;		/* errno */

/* set alternate SCB address for chip initialize */
	I82596_ASCP(ifstat, ifstat->v_scp);
	
/* tell chip to start! */
	I82596_CHANATTN(ifstat);
	count = 65536;
	while (count-- && ifstat->v_iscp->iscp_busy)
		;
	if (count == 0) return E_DEADLK;

/*
 * now the 82596 should be ready to accept commands
 * try DIAGNOSE and a couple a NOPs on for size...
 */
 	if (i82596CMDExec(ifstat, CB_CMD_DIAG, ifstat->v_cbnext))
 		return E_DEADLK;
	
 	if (i82596CMDExec(ifstat, CB_CMD_NOP, ifstat->v_cbnext))
 		return E_DEADLK;
	
 	if (i82596CMDExec(ifstat, CB_CMD_NOP, ifstat->v_cbnext))
 		return E_DEADLK;
	
/*
 * Now that is seems like we can take commands, issue
 * IA setup to set LAN physical address.
 */
 
	memcpy(ifstat->v_cbnext->cmd.ia, ifdev->if_paddr, 6);
 	if (i82596CMDExec(ifstat, CB_CMD_IASETUP, ifstat->v_cbnext))
 		return E_DEADLK;
	 	
/*
 * now start the receiver...
 */
 	ifstat->v_scb->scb_cmd |= SCB_RU_START_CMD;
	I82596_CHANATTN(ifstat);
 	
/* signal chip is initialized and is up and running */
	ifstat->v_running = 1;
}
