/****************************************************************************
 *           Copyright 1989-1998 by Microware Systems Corporation           *
 *                           All Rights Reserved                            *
 *                         Reproduced Under License                         *
 *                                                                          *
 * This software is confidential property of Microware Systems Corporation, *
 * and is provided under license for internal development purposes only.    *
 * Reproduction, publication, distribution, or creation of derivative works *
 * in any form to any party other than the licensee is strictly prohibited, *
 * unless expressly authorized in writing by Microware Systems Corporation. *
 *                                                                          *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *   1 04/03/97 Creation (from ISP)                                    mil
 *   3 07/15/97 Ported to OS9/68k                                      jpd
 *   4 08/27/97 Bug fix                                                taka
 *   5 09/08/97 fixed os_irq()                                         taka
 *   6 09/29/97 Added MVME172 support                                  pjw
 *   7 12/03/97 Added code to init lustat->lu_item_addr for netstat    jk
 *   8 01/21/98 Removed compiler and linker warnings                   jk
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  11 06/22/99 Integrated Kei Thomsen's changes, fixed mbuf leak.     jk
 *  12 08/29/99 Fixed lustat->lu_item_addr initialization              rds
 ****************************************************************************/

#include <SP82596/defs.h>

#define I82596CMDAdd_Wait(a,b,c,d,e) \
  if (i82596CMDAdd((a), (b), (c), (d))) \
     return ((e)); \
  if (status = wait_complete((a))) \
     return status;

/* if_init */
error_code init_drvr(Dev_list dev_entry)
{
	Spf_drstat 	drstat = (Spf_drstat)(dev_entry->v_dr_stat);
	Spf_lustat 	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	u_int32 	tempsize, count;
	u_int8* 	ptr;
	u_int32 	status;
#if defined (spdve7708)
	u_int32 	*dga;
	u_int16 	irqlvl;
#endif

	/* verify available hardware */
#if defined(sp162) || defined(sp172)
	if (nolanc()) {
		/* sorry, no 82596 is installed */
		return E_NOTRDY;
	}
#endif
	I82596_RESET (lustat);

	_OS_IRQ(lustat->lu_vector,		/* vector number 			*/
			lustat->lu_priority,	/* polling priority 		*/
			hw_isr,					/* irq handler routine 		*/
			dev_entry,				/* static storage pointer 	*/
			lustat->lu_port			/* port address				*/
	);

	/* set up drstat pointer for use by iestat */
	lustat->lu_drstat = drstat;

	/* how much memory do we need? */
	tempsize = i82596totmem(lustat);

	/* make sure memsize given in descriptor will hold 82596's structs */
	if (lustat->lu_ramsiz && (tempsize > lustat->lu_ramsiz)) {
		return E_FULL;
	}

	/*
	** get shared ram pointer or allocate memory for one.
	** NOTE that shram address MUST be aligned on a 16 byte boundary.
	** This is guaranteed by the F$Srqmem call
	*/
	if (lustat->lu_shram == NULL) {
		if ((status=_os_srqmem(&tempsize,(void**)&lustat->lu_shram,0))!= SUCCESS) {
			return status;
		}

#if defined(ie7140)
		change_mem(lustat->lu_shram,lustat->lu_shram+tempsize,CM_CIS);
		change_mem(lustat,(unsigned)lustat + sizeof(struct ifs),CM_CIS);
#endif
	} else {
		lustat->lu_shram += lustat->lu_ramoffset;  /* possible base+offset */
	}

	lustat->lu_shramsize = tempsize;

#ifdef _MPFSH
#define MAKE_LONG_ACCESS(xx)	((u_int32)(xx) & 0xfff0ffff)
#else
#define MAKE_LONG_ACCESS(xx)	((u_int32)(xx))
#define PORT_OFFSET 0
#define IRQ_OFFSET 8
#define CA_OFFSET 4
#define DGA_ADDR (void *)0xfff42000
#endif

	/* initialize pointers to chip registers */
	lustat->lu_addr = (u_int8 *)lustat->lu_port + PORT_OFFSET;
	lustat->lu_irqclr = (void *)MAKE_LONG_ACCESS ((u_int8 *)lustat->lu_port + IRQ_OFFSET);
	lustat->lu_ca   = (void *)MAKE_LONG_ACCESS ((u_int8 *)lustat->lu_port + CA_OFFSET);
	lustat->lu_dga = DGA_ADDR;

#if defined(hkvf82596)
	if (lustat->lu_port == (void *)0xff010300) {
		lustat->lu_isV4d = 1;			/* use i82596 on V4D cpu */
	}
#endif

	get_eaddr (lustat);

	/* Now do some board initialization */
#if defined(sp167) || defined(sp162) || defined(sp177) || defined(sp172)/*double check this */

	*LANC_IRQ_CONTROL = (LANC_IEN|LANC_ICLR|LANC_EoL) | lustat->lu_irqlvl;
	*LANC_BEIRQ_CONTROL = LANC_SNOOP;

#elif defined(ifgenesis)

	*LANC_IRQ_CONTROL = (LANC_IEN|LANC_ICLR|LANC_EoL) | lustat->lu_irqlvl;
	*LANC_BEIRQ_CONTROL = 0;

#elif defined(mz8554)

	if (*MZ8554_ETHER_CONTROL(lustat->lu_addr) & MZ8554_INT_ASSERTED)
		*MZ8554_INT_CLEAR(lustat->lu_addr) = 0;

	ctemp = ETHER_CONTROL_ENABLE | (lustat->lu_irqlvl<<2) | ETHER_CONTROL_INT_EN;
	*MZ8554_ETHER_CONTROL(lustat->lu_addr) = ctemp;

	ctemp = RAM_ENABLE | ((u_int32)lustat->lu_shram >> 16);
	*MZ8554_RAM_BASE_ADRS(lustat->lu_addr) =	ctemp;

	*MZ8554_INT_VECTOR(lustat->lu_addr) = ddptr->dds._mvector; /*irq vector*/

#elif defined(ie7140)

	/* enable epld irqs */
	tempsize = *MZ7140_GCR;
	tempsize = tempsize	| MZ7140_ETHER_ENABLE;
	*MZ7140_GCR = tempsize;
	*MZ7140_INT_ENABLE |= MZ7140_ETHER_IRQ;

#elif defined(hkvf82596)

	if (lustat->lu_isV4d) {
		u_int8 licr_val;
		/* enable the irq on the VIC */
		licr_val = 0x80 | lustat->lu_desc.licr_mask | lustat->lu_irqlvl;
		/* don't enable IRQ just yet to prevent false trigger */
		*HV4D_LIRQ1 = licr_val;
		*HV4D_LIRQ1 = licr_val & ~0x80;		/* enable IRQ */
	} else {
		u_int8 licr_val;
		*CMSE_EIRQ_ENABLE(lustat->lu_addr) = 1;	/* enable I82596 IRQ on CM */
		*CMSE_EIRQ_A_ENABLE(lustat->lu_addr) = 1;/* ena I82596 abort IRQ on CM */

		licr_val = 0x80 | lustat->lu_desc.licr_mask | lustat->lu_irqlvl;
		*((u_int8 *)0xff000037) = licr_val;
		/* set vic068 lcl 5 irq enable */
		*((u_int8 *)0xff000037) = licr_val & ~0x80;	/* enable IRQ */
	}

#elif defined (spdve7708)
	irqlvl = lustat->lu_irqlvl>>1;
	if (irqlvl <1)
		irqlvl = 1;
	else if (irqlvl >6)
		irqlvl = 6;
	dga = DGA_ADDR;
	dga[25] = (dga[25] & ~GP0IRQ_LEVEL_MASK)|irqlvl;
	dga[21] |= GP0IRQ_ENABLE;
#endif

	/* clear out the shared ram */
	memset((u_int8 *)MAKE_P3_OFFSET(lustat->lu_shram), 0, lustat->lu_shramsize);

	/* now run selftest on device */
	((STR *)MAKE_P3_OFFSET(lustat->lu_shram))->str_romsig = ~0;
	((STR *)MAKE_P3_OFFSET(lustat->lu_shram))->str_result = ~0;
	I82596_SELFTEST(lustat, (void*)MAKE_P3_OFFSET(lustat->lu_shram));

	/* wait for it to complete*/
	drstat->dr_iestat.stloops = 65536;
	while (--drstat->dr_iestat.stloops &&
		((STR*)MAKE_P3_OFFSET(lustat->lu_shram))->str_result == ~0)
		nop ();
	if (drstat->dr_iestat.stloops == 0) {
		return E_DEADLK;
	}

	/* keep status for iestat */
	drstat->dr_iestat.str = *((STR *)MAKE_P3_OFFSET(lustat->lu_shram));

	/* initialize shared ram for chip */
	if (count = i82596meminit(dev_entry))
		return count;		/* errno */
	/*
	lustat->lu_ioenabled = DRVR_IOEN;
	*/
	/* set alternate SCP address for chip initialize */
	I82596_ASCP(lustat, (void*)lustat->lu_scp);

	/*
	** set sigproc to cause IRQ routine to send signals for this
	** initialization sequence.  These commands are normally fast, may
	** have delayed response depending on condition of the hardware or
	** tranceivers.
	*/
	lustat->lu_sigproc = dev_entry->v_cproc;

	/* tell chip to start...CU idle IRQ will (should) happen */
	I82596_CHANATTN(lustat);
	if (status = wait_complete(dev_entry)) return status;

	/* set bus throttle timers (already loaded by meminit) */
	lustat->lu_scb->scb_cmd = SCB_CU_BUSTHR;
	I82596_CHANATTN(lustat);
	nop();
	nop(); /* WHY THREE (one in CHANATTN also) ????? */
#if defined (Flushing)
	_os_cache(6);               /* Flush data cache and turn it off */
#endif
	while (lustat->lu_scb->scb_cmd) nop();
	/*
	** now the 82596 should be ready to accept commands
	** try DIAGNOSE and a couple a NOPs on for size...
	*/
	I82596CMDAdd_Wait (dev_entry, CB_CMD_DIAG, 0, 0, EOS_LOCK);

	I82596CMDAdd_Wait (dev_entry, CB_CMD_NOP, 0, 0, EOS_LOCK);

	I82596CMDAdd_Wait (dev_entry, CB_CMD_NOP, 0, 0, EOS_LOCK);

	/*
	** Descriptor editions 6 and earlier do not initialize the hardware
	** to pad short frames. In order to maintain compatibility with
	** previous descriptors we will update the config information here,
	** before we initialize the hardware. The lu_junk1[0] field used
	** to contain the first 4 bytes of the string "if_stat". Since
	** that is now 0 we can use it as a crude (but effective) check
	** if we have an old descriptor.
	*/
	if (lustat->lu_junk1[0] == 0x69665F73) {
		lustat->lu_config_data[0] = 0x8e;
		lustat->lu_config_data[11] = 0xfd;
	}

	/*
	** Build configure block to put chip in desired mode.  The documentation
	** is very unclear and contradictory on how to do this, so space is given
	** to ???
	*/
	I82596CMDAdd_Wait (dev_entry, CB_CMD_CONFIG, lustat->lu_config_data, 16, EOS_LOCK);

	/*
	** Now that is seems like we can take commands, issue
	** IA setup to set LAN physical address.
	*/
	I82596CMDAdd_Wait (dev_entry, CB_CMD_IASETUP, lustat->lu_item_addr.addr, 6, EOS_LOCK);

	/*
	** dump the chip guts
	*/
#ifdef PVTRAM
	ptr = (u_int8 *)lustat->lu_rfd_head->rfd_data;
	/* good place for dumped data (simplified mode) */
	status = (u_int32)MAKE_CPU_ADDR_SWAP((u_int32)ptr);
#else
	status = (u_int32)lustat->lu_rbd_head->rbd_leBuffer;
	ptr = (u_int8 *)MAKE_CPU_ADDR_SWAP(status);
	/* good place for dumped data (flexible mode) */
#endif
	I82596CMDAdd_Wait (dev_entry, CB_CMD_DUMP, &status,sizeof(status),EOS_LOCK);

	drstat->dr_iestat.chip_step = (ptr[0x11a]>>4)&0xf;
	/* from Intel step document 2/92: step byte at 118h, Bits 15:12 */

	/*
	** set mode override flag (for chip bugs)
	*/
#ifdef PVTRAM
	/* no choice here */
	lustat->lu_chipmode = CM_XMIT_SIMP | CM_RECV_SIMP;
#else
	/*
	**
	** CM_XMIT_SIMP is provided for use on the A-step chips that
	** transmit bad packets in flex mode under certain conditions
	** (Errata 2.0).  The driver will place the TCB at the beginning
	** of the mbuf and use the simplified mode.  If the TCB won't fit,
	** the mbuf is adjusted (copied) so it will.  Be sure
	** "sock_opt_mbpad" in the socket descriptor is adjusted to allow
	** for this.
	**
	** The mode can be explicitly set in the descriptor to override
	** the auto-config.
	*/
	if (lustat->lu_chipmode == 0) {			/* auto config? */
		if (drstat->dr_iestat.chip_step < 2) {		/* A-step chip? */
			lustat->lu_chipmode =  CM_XMIT_SIMP;
		} else {
			lustat->lu_chipmode = CM_XMIT_FLEX;
		}
	}
	lustat->lu_chipmode |= CM_RECV_FLEX;		/* always for non-PVTRAM */
	lustat->lu_chipmode &= ~CM_RECV_SIMP;	/* always for non-PVTRAM */
#endif

	/*
	** finished with test/init sequence
	*/
	lustat->lu_sigproc = 0;

	/*
	** now start the receiver...
	*/
	start_ru(dev_entry);

#if defined (Flushing)
	_os_cache(5);               /* Flush data cache and turn it on */
#endif

	return(SUCCESS);
}

signal_code wait_complete(Dev_list dev_entry)
{
	signal_code sigval = 0;
#ifdef _OSK
	u_int32 tics = 2;
#else
	u_int32 tics = 0;
#endif

	_os_sleep(&tics,&sigval);
	return sigval;
}

/*
**      Determine physical ethernet address
**
**      This can be done many ways.  Usually there is some ROM on the local
**      hardware containing the unique ethernet address.  One can
**      extract the ethernet address from the device descriptor if nothing
**      better is available.
*/
void get_eaddr(Spf_lustat lustat)
{
	u_int8 	mask, *ptr, *dst;
	u_int32 cnt;

	/*
	** Copy the descriptor defined address into the ITEM address
	** structure. Also set mask to determine if the descriptor
	** defined value was all zero's.
	*/
	mask = 0;
	ptr = (u_int8 *)lustat->lu_ether_addr;
	dst = (u_int8 *)lustat->lu_item_addr.addr;
	for (cnt = 0; mask == 0 && cnt < 6; ++cnt) {
		mask |= *ptr;
		*dst++ = *ptr++;
	}

	/*
	** If the descriptor did not contain an address then get it
	** from the board.
	*/
	if (mask == 0)
		read_eaddr(lustat);
}
