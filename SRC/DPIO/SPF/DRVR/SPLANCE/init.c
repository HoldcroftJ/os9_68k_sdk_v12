/****************************************************************************
 *	am7990 lance driver						    							*
 ****************************************************************************
 *  Copyright 1988 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date       Comments                                           By   *
 *  --- --------    ----------------------------------------------- ------- *
 *	  1 08/02/88	Birth			                                	kkk *
 *	  2 07/14/89	Added v_running flag for init/term status			kkk *
 *	  3 04/28/93	Modified for UCC                                   ylee *
 *	  4 06/16/93	Mo Mod (No, not Gaddafi) for UCC. changed ifdef 		*
 *					mc68000 which was defined by the old compiler to 		*
 *					_MPF68K used by ucc									gkm	*
 *	  5 09/02/93	call ifman's bootpcheck routine to let boot ip 			*
 *					addr override descriptor if this device was booter.  	*
 *					Since that requires access to sysglobs, modify 			*
 *					paramter list passed in to if_init to allow procid 		*
 *					& sysglobs (IFMAN used to pass them in all along 		*
 *					anyway).											gkm	*
 *      09/16/93    <***** ISP 2.0 Release ***********************>         *
 *																			*
 * SPF support Created														*
 *		27FEB96	    modified for LANCE               Bill West Inc.     bw  *
 *    5 09/24/96     Updated get_eaddr to obtain eth addr from BBRAM 	pjw * 
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release*********>         *
 *      05/05/97    <***** SPF LAN Comm Pack v3.1 Release*********>         *
 *   10 10/23/97    Initialize eth addr in item struct for ITE_GETADDR  tmd *
 *   11 01/08/98    Changed "mem_ptr" param, init_hdw call to				*
 *                  _os_srqmem. Added comment to init_hdw, line 280      jk *
 *      04/02/98 	<***** SPF LAN Comm Pack v3.2 Release*********>         *
 *	 12 03/24/01	Added support for ET003 3u card						mgh *
 *																			*
 ****************************************************************************/


#include <SPLANCE/defs.h>

error_code freedmem(Spf_lustat lustat)
{
	Ifs 		ifstat = (Ifs)(&lustat->ifstat);
#ifdef	LCLMEM
	if (lustat->le_ram_size == 0 ) 		/* return mem */
		_os_srtmem(ifstat->v_ram_size, ifstat->v_cpu_base);
#else
	if (ifstat->v_rpmem) _os_srtmem(ifstat->v_rpmem, ifstat->v_recv_ring);
	if (ifstat->v_tpmem) _os_srtmem(ifstat->v_tpmem, ifstat->v_xmit_ring);
#endif
	return(SUCCESS);
}

unsigned char clog2(v)
register unsigned char v;
{
	register unsigned char n;
	
	for (n=0; v >>= 1; n++);

	return n;
}

error_code init_hdw(Dev_list deventry)
{
	Spf_lustat 	lustat = (Spf_lustat)(deventry->v_lu_stat);
	Ifs 		ifstat = (Ifs)(&lustat->ifstat);
	struct le_init_block *ibp;
	struct leregs	*leregs;
	Le_md mdp;
	u_int32		tempsize;
#ifdef  LCLMEM
        u_int32          count;
#endif
#ifdef	SUN380
	unsigned short	regval;
#endif
	error_code	err;
		
	ifstat->v_rpmem = ifstat->v_tpmem = 0;			/* clear memory indicators */
	ifstat->v_leregs =
		leregs = (struct leregs *)lustat->lu_port;	/* get lance port address */
		
#ifdef LCLMEM
/* initialized variables */
/* copy descriptor values into static storage */
	ifstat->v_cpu_base = lustat->le_cpu_base;
	ifstat->v_ram_size = lustat->le_ram_size;
	ifstat->v_lan_adj = lustat->le_lan_adj;
	
/*
	The local lance memory is allocated as follows (lowest to highest addr):
		- Initialization block
		- Recv message descriptors
		- Xmit message descriptors
		- Receive data buffers
		- Transmit data buffers
		
	Notice that the rx/tx buffers are of fixed size and number.  This is
	likely to be changed in the future.  Fixed size buffers avoid the
	complexity of LANCE buffer chaining. Anyone ever tried this?
	The following code performs a lame sanity check to ensure that the
	total size of the mds and buffers will fit into the lance-local memory.
*/

 	If lustat->le_ram_size == 0, then we'll request the memory we need.
 	That amount is the size of le_init_block, v_rpcnt * sizeof (struct le_md)
 	and v_tpcnt * sizeof (struct le_md).

/* set correct ring sizes */

	ifstat->v_rpcnt = (lustat->recv_ring_cnt > 16) ? 16 : lustat->recv_ring_cnt;
	ifstat->v_tpcnt = (lustat->xmit_ring_cnt > 16) ? 16 : lustat->xmit_ring_cnt;
	
	if (lustat->le_ram_size == 0) {
		ifstat->v_ram_size = INIT_BLK_SIZE + 
			((ifstat->v_rpcnt + ifstat->v_tpcnt) * 
			(sizeof(struct le_md) + LANCE_ETHER_MAX));
		if((err = _os_srqmem(&(ifstat->v_ram_size), &(ifstat->v_cpu_base), 0)) != 0)
			return (err);
#ifdef	SUN380
		set_iomapper(ifstat->v_cpu_base, ifstat->v_ram_size);
#endif
	}
	
	tempsize = INIT_BLK_SIZE; /* sizeof(struct le_init_block); */
	mdp = (Le_md)(ifstat->v_cpu_base + tempsize);
	ifstat->v_rpmem = ifstat->v_rpcnt * sizeof(struct le_md);
	ifstat->v_recv_ring = (Md_ap)mdp;
	memset(mdp, 0, ifstat->v_rpmem);
	tempsize += ifstat->v_rpmem;
#else
/* allocate memory for receive message descriptor rings */
	tempsize = sizeof(struct le_md)*lustat->recv_ring_cnt;
	if ((err = _os_srqmem(&tempsize, (void **)&mdp, 0)) != 0) {
		return (err);
	} else {
		ifstat->v_rpmem = tempsize;
		ifstat->v_recv_ring = (Md_ap)mdp;
		memset(mdp, 0, tempsize);
	}
#endif

#ifdef LCLMEM
/* no need to request memory; just fill in the pointers */
	mdp = (Le_md)(ifstat->v_cpu_base + tempsize);
	ifstat->v_tpmem = ifstat->v_tpcnt * sizeof(struct le_md);
	ifstat->v_xmit_ring = (Md_ap)mdp;
	memset(mdp, 0, ifstat->v_tpmem);
	tempsize += ifstat->v_tpmem;
#else
/* allocate memory for transmit message descriptor ring */
	tempsize = sizeof(struct le_md)*lustat->xmit_ring_cnt;
	if ((err = _os_srqmem(&tempsize, (void **)&mdp, 0)) != 0) {
		freedmem(lustat);
		return (err);
	} else {
		ifstat->v_tpmem = tempsize;
		ifstat->v_xmit_ring = (Md_ap)mdp;
		memset(mdp, 0, tempsize);
	}
#endif

#ifdef LCLMEM
/* fill in the rmd's with buffer addresses from the known local RAM */
	/* convert tempsize from pure offset to lance-view pointer */
	tempsize += ifstat->v_cpu_base - ifstat->v_lan_adj;
	for (count = 0; count < ifstat->v_rpcnt; count++) {
		(*ifstat->v_recv_ring)[count].md_loaddr = (u_int32) tempsize;
		(*ifstat->v_recv_ring)[count].md_hiaddr = ((u_int32) tempsize) >> 16;
		(*ifstat->v_recv_ring)[count].md_bcnt = -((u_int32) LANCE_ETHER_MAX);
		(*ifstat->v_recv_ring)[count].md_flag = MD_OWN;
		tempsize += LANCE_ETHER_MAX;
	}
/* fill in the tmd's with buffer addresses from the known local RAM */
	for (count = 0; count < ifstat->v_tpcnt; count++) {
		(*ifstat->v_xmit_ring)[count].md_loaddr = (u_int32) tempsize;
		(*ifstat->v_xmit_ring)[count].md_hiaddr = ((u_int32) tempsize) >> 16;
		(*ifstat->v_xmit_ring)[count].md_bcnt = -((u_int32) LANCE_ETHER_MAX);
		tempsize += LANCE_ETHER_MAX;
	}
#else
/* initialize ring counts */
	ifstat->v_rpcnt = lustat->recv_ring_cnt;
	ifstat->v_tpcnt = lustat->xmit_ring_cnt;
#endif
	
/* initialize the high-byte of the local memory address.  A little theory
   is appropriate here.  The LANCE is a 16-bit device.  It can only address
   16MB.  It is typically wired up to access the CPU's local memory.  Such
   local memory is typically in the lower address range so the LANCE can
   access it.  If not, the high-byte is disregarded and the LANCE accesses
   the memory modulo 16MB.  This high byte is recorded below for future
   reference by the CPU.  If the hardware in question operates directly
   over the the VMEBus or has a pathological memory mapping, it is likely
   that this driver will have to be completely re-written.
*/
	ifstat->v_hiaddr = (((u_int32)lustat->le_hiaddr) << 24);
	
/* point to and clear initialization block */
	ibp = &ifstat->v_ib;
	memset(ibp, 0, sizeof(struct le_init_block));

/* initialize mode values */
	ibp->ib_mode = lustat->ib_mode;
	
/* setup the physical ethernet address */
	get_eaddr(deventry, ibp->padr, (u_char *)lustat->bbram_lanid);

#ifdef NOT_YET
/* call ifman's bootpcheck routine (if available) to see if this was boot
 * device and if boot provided a different ip addr to override descriptor 
 */	if (ifdev->if_man->v_padder && ifdev->if_man->v_bootpcheck)
		(*ifdev->if_man->v_bootpcheck)(ifdev, sysglob);
#endif
		
/* set up LANCE link-level address in init block */
#ifdef _MPF68K
/* swap bytes required for hi-end mpu */
	swabytes(&ibp->padr[0]);
	swabytes(&ibp->padr[2]);
	swabytes(&ibp->padr[4]);
#endif
	
/* setup the receive logical address filter */
	memset(ibp->ladr, 0, 8);
	
/* setup the receive descriptor pointer */
	ibp->recv_drp.laddr = (u_int32)ifstat->v_recv_ring;
	ibp->recv_drp.rlen = clog2(ifstat->v_rpcnt);
	ibp->recv_drp.haddr = ((u_int32)ifstat->v_recv_ring) >> 16;

/* setup the transmit descriptor pointer */
	ibp->xmit_drp.laddr = (u_int32)ifstat->v_xmit_ring;
	ibp->xmit_drp.rlen = clog2(ifstat->v_tpcnt);
	ibp->xmit_drp.haddr = ((u_int32)ifstat->v_xmit_ring) >> 16;

#ifdef LCLMEM
/* now copy the loaded initialization block to the local lance RAM */
	memcpy((char *) ifstat->v_cpu_base,(char *) ibp,
			sizeof(struct le_init_block));
#else
/* initialize the receive buffers */
	if (err = init_rmds(ifstat)) {
		/* choke... */
		freedmem(lustat);
		return (err);
	}
#endif

/* ensure LANCE is accessable */
#if defined(TP22V)
	*((char *)LANRESET) = 1;			/* deassert LANRESET on TP22V */
#endif

#if defined(TP32V)
	*((char *)LANRESET) = 1;			/* deassert LANRESET on TP22V */
#endif

/* cause lance to initialize itself */
	leregs->rap = LE_CSR0;		/* select lance csr 0 */
#ifndef		SUN380
	leregs->rdp |= LE_TSTOP;		/* reset lance */
#else
	regval = LE_TSTOP;				/* get a word value */
	leregs->rdp |= regval;
#endif
	
	leregs->rap = LE_CSR1;
#ifdef LCLMEM
	leregs->rdp = (u_int16)(ifstat->v_cpu_base - ifstat->v_lan_adj);
#else
	leregs->rdp = (u_int16)ibp;	/* stuff lo 16-bits of ibp */
        /* this generates compiler warning:
              "conversion from pointer to smaller integer"   
	*/
#endif

	leregs->rap = LE_CSR2;
#ifdef LCLMEM
	leregs->rdp = ((u_int32)(ifstat->v_cpu_base - ifstat->v_lan_adj)) >> 16;
#else
	leregs->rdp = ((u_int32)ibp) >> 16;	/* stuff hi 8-bits of ibp */
#endif	

	leregs->rap = LE_CSR3;
	leregs->rdp = lustat->le_csr3;	/* set up csr3 */

/* initialize lance */
	leregs->rap = LE_CSR0;
	leregs->rdp = LE_INIT;

#ifndef	SUN380

	while ((leregs->rdp & LE_IDON) == 0)	/* wait for init */
		nop();
		
#else	/* SUN380 */

	while (((regval = leregs->rdp) & LE_IDON) == 0)	/* wait for init */
		nop();
		
#endif	/* SUN380 */

/* initialize driver internal stuff */
	ifstat->v_recv_head = &(*ifstat->v_recv_ring)[0];
	ifstat->v_xmit_head = &(*ifstat->v_xmit_ring)[0];
	ifstat->v_xmit_tail = NULL;
	ifstat->v_recv_rend = &(*ifstat->v_recv_ring)[ifstat->v_rpcnt];
	ifstat->v_xmit_rend = &(*ifstat->v_xmit_ring)[ifstat->v_tpcnt];

/* now set the hardware to allow the am7990 to interrupt.
   Since there is no standard way to do this for all CPU's,
   the conditional code is placed here.
*/
#if defined(ELTEC_E5)
/*
	The IRQ vector number register is located at
	leregs + 0x101.  It is write only.
*/
	*(((char *)leregs) + 0x101) = (char)lustat->lu_vector;
#elif defined(VME147)
/*
	The IRQ vector number is programmed into the LANICR
	register.
*/
	*((char *)LANICR) = ((char)lustat->lu_irqlvl) | LANICR_IE;
#elif defined(TP22V)
/*
	The LANCE irq is fixed to Autovector Level 3
*/
#elif defined(TP32V)
/*
	The LANCE irq is fixed to Autovector Level 3
*/
#elif defined(FORCE37)
/*
	The LANCE irq is fixed to Autovector Level 2
*/
#elif defined(JNMS)
/*
	This does something....
*/
	*((u_short *)0x1ffc56) = 
		(((0xF8 | ((char)lustat->lu_irqlvl)) << 8) |
			(char)lustat->lu_vector);
#endif

/* start receiver */
	leregs->rdp = LE_IDON | LE_INEA | LE_STRT;

/* that should be it... */
	ifstat->v_running = 1;

#ifdef NOT_YET
/*
	Send an ARP request for ourselves when we come up.  Since we expect
	others to talk to us, this lets all others know about us so they
	don't have to ask.  Here we form an ARP request from us asking about
	us, which is essentially a no-op.
*/
	if ((ifstat->ib_mode & LEM_DTX) == 0 &&	/* transmitting?*/
			(ifstat->ib_mode & LEM_DRX) == 0)	/* ...and receiving? */
		(*ifdev->if_man->v_arp_whohas)(ifdev, &IA_SIN(ifdev)->sin_addr);
#endif
	return(SUCCESS);
}

#ifdef	SUN380

set_iomapper(addr, size)
u_int32		addr;
int			size;
{
	register u_int32	*iomapper = (u_int32 *) SUN380_IOMAPPER,
						pages,				/* number of pages to map */
						index;				/* page index */
					
	pages = ((((addr + size) + (SUN380_PAGE_SIZE - 1) &
				~(SUN380_PAGE_SIZE - 1)) - 
					(addr & ~(SUN380_PAGE_SIZE - 1))) /
						SUN380_PAGE_SIZE);
	index = ((unsigned int) addr & 
			((SUN380_IOMAPSIZE * SUN380_PAGE_SIZE) - 1)) / SUN380_PAGE_SIZE;
	addr &= ~(SUN380_PAGE_SIZE - 1);		/* strip unneeded bits */
	for (; pages > 0; pages--, index++, addr += SUN380_PAGE_SIZE) {
		if (index >= SUN380_IOMAPSIZE)				/* wrap around */
			index = 0;
		iomapper[index] = logtophys(addr) | SUN380_PAGEDESC;
	}
}

#ifdef _UCC
_asm("logtophys: movem.l d1/a0,-(a7)");
_asm(" ptestr #5,(d0),#3,a0 get the page descriptor");
_asm(" andi.l #0x003fffff,d0 keep lower four meg");
_asm(" move.l (a0),d1 get the physical page address");
_asm(" andi.l #0xffc00000,d1 keep four meg index");
_asm(" or.l d1,d0");
_asm(" movem.l (a7)+,d1/a0");
_asm(" rts");
#else
/*#asm*/

logtophys: movem.l d1/a0,-(a7)
 ptestr #5,(d0),#3,a0 get the page descriptor
 andi.l #0x003fffff,d0 keep lower four meg
 move.l (a0),d1 get the physical page address
 andi.l #0xffc00000,d1 keep four meg index
 or.l d1,d0
 movem.l (a7)+,d1/a0
 rts
	
/*#endasm*/
#endif


#endif	/* SUN380 */

void get_eaddr(Dev_list deventry, u_char *eaddr, u_char *hwaddr)
{

	Spf_lustat 	lustat = (Spf_lustat)(deventry->v_lu_stat);
	int		i;
	u_char		flag = 0;

/*
Non-null address in descriptor overrides hardware address from
BBRAM or PROM.
*/
	for(i = 5; i >= 0; i--)
		{
		eaddr[i] = lustat->ether_addr[i];
		flag |= eaddr[i];
		}

#ifdef SPET003

	if((flag == 0) && (hwaddr != NULL))
		{
		u_int16 *nodeid = (u_int16 *)hwaddr;
		/* Ethernet mac address is read on d8-D15 from x00800, x00802... */
		eaddr[0] = lustat->ether_addr[0] = (u_char)((*nodid++ >> 8) & 0xFF);
		eaddr[1] = lustat->ether_addr[1] = (u_char)((*nodid++ >> 8) & 0xFF);
		eaddr[2] = lustat->ether_addr[2] = (u_char)((*nodid++ >> 8) & 0xFF);
		eaddr[3] = lustat->ether_addr[3] = (u_char)((*nodid++ >> 8) & 0xFF);
		eaddr[4] = lustat->ether_addr[4] = (u_char)((*nodid++ >> 8) & 0xFF);
		eaddr[5] = lustat->ether_addr[5] = (u_char)((*nodid++ >> 8) & 0xFF);
		}
#else
	if((flag == 0) && (hwaddr != NULL))
		{
		eaddr[0] = lustat->ether_addr[0] = 0x08;
		eaddr[1] = lustat->ether_addr[1] = 0x00;
		eaddr[2] = lustat->ether_addr[2] = 0x3e;
		eaddr[3] = lustat->ether_addr[3] = (0x20 | hwaddr[0]);
		eaddr[4] = lustat->ether_addr[4] =  hwaddr[1];
		eaddr[5] = lustat->ether_addr[5] =  hwaddr[2];
		}
#endif

	/* set up ITEM address structure for ITE_GETADDR */
	memset(&lustat->lu_item_addr, 0, sizeof(lustat->lu_item_addr));
	lustat->lu_item_addr.addr_class = ITE_ADCL_INET; /* FIX - make ENET */
	lustat->lu_item_addr.addr_subclass = ITE_ADSUB_NONE;
	lustat->lu_item_addr.addr_size = 6;
	memcpy(&lustat->lu_item_addr.addr, eaddr, 6);
}

_asm("
nop:	nop
	rts
");
