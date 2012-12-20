#include <sysboot.h>
#include "ie.h"

int i82596CMDExec(ifstat, cmd, cb)
register Ifs ifstat;
u_int16 cmd;
register CB *cb;
{
	register u_int32 loops = 0;
	u_int16 next;

	ifstat->v_cbnext = cb->cb_beNext;	/* setup next entry to use */
	
/*
 * load up another command
 */		
	cb->cb_status = 0;
	cb->cb_command = CB_LAST | cmd;
	
/*
 * add command to command chain
 */
	if (ifstat->v_cbhead) {				/* existing command chain? */
		if ((ifstat->v_cbtail->cb_status & (CB_COMPLETE | CB_BUSY)) == 0) {
					/* chip not here yet...link this CB to tail */
			ifstat->v_cbtail->cb_command &= ~(CB_LAST);
			ifstat->v_iestat.cmd_links++;
		} else {
			ifstat->v_iestat.needxirqstart++;
			/* xirq will restart this new chain */
		}
		ifstat->v_cbtail = cb;		/* move tail ptr to this CB */
	} else {							/* no existing chain: CU must be idle */
		ifstat->v_cbhead = ifstat->v_cbtail = cb;

		/*
		 * just in case: make sure scb is ready for a command
		 */
		while (next = ifstat->v_scb->scb_cmd) {	/* working on prev command? */
			loops++;
		}
		if (loops > ifstat->v_iestat.scb_busy_loops)
			ifstat->v_iestat.scb_busy_loops = loops;
		loops = 0;	

		/*
		 * start command unit
		 */
		ifstat->v_scb->cbl = (CB *)ram_swapw(cb);
		ifstat->v_scb->scb_cmd |= SCB_CU_START_CMD;
		I82596_CHANATTN(ifstat);
	}

/*
 * wait for command complete
 */
 	while ((cb->cb_status & CB_COMPLETE) == 0) ;
	while ((ifstat->v_scb->scb_status & SCB_STATUS_CNA) == 0) ;
	ifstat->v_scb->scb_cmd = SCB_CNA_ACK;
	I82596_CHANATTN(ifstat);
	
	
	ifstat->v_cbhead = ifstat->v_cbtail = NULL;
	return (!(cb->cb_status & CB_OK));
}
	

/************************************************************************
*
* i82596MemInit - initializes shared memory
*
* This routine breaks up the shared memory into a SCP, ISCP, SCB and two 
* ring lists:  the command block ring and the frame descriptor ring list.  
* The first element in each ring is recorded in the SCB and device descriptors
* for later use.  The simplified memory model of the chip is being used.
*
* After this routine the shared memory block will appear as follows:
*
* +---------------------+	<---	ifstat->v_shram
* |   64 bytes zeroes   |
* +---------------------+
* |        SCP          |
* +---------------------+
* |        ISCP         |
* +---------------------+
* |        SCB          |
* +---------------------+
* |		   CB			|	<---    cmd block list (with tbd's)
* +---------------------+
* |        RFA          |	<---	ring buffer of received frames
* +---------------------+
* |			...			|
* |			...			|
* |			...			|
* |			...			|
* |			...			|
* +---------------------+
* |						|
* |	XMIT DATA BUFFERS   |	<---	for LCLMEM only
* |						|
* +---------------------+
*
*/


/*
 * return size of memory required by
 * i82596 for data structures.
 *
 * Make sure this matches with "initialize base pointers to structures"
 * close below in i82596meminit.
 */
u_int32 i82596totmem(ifstat)
register Ifs ifstat;
{
	return
		sizeof(SCP) +
		sizeof(ISCP) +		
		sizeof(SCB) +
		ifstat->v_ncbl * sizeof(CB) +
		ifstat->v_nrfd * sizeof(RFD) +
#ifdef LCLMEM
		ifstat->v_ncbl * sizeof(TBD) +	/* for transmit buffers */
#else
		0 +
#endif
		64;			/* zeroes for short packets (really at beginning) */
}
	
u_int32 i82596meminit(ifstat)
register Ifs ifstat;
{
	register RFD *rfd, *lrfd;
	register CB  *cb, *lcb;
	register u_int16 ncount;
		
/* initialize base pointers to chip structures */
	ifstat->v_zpad = ifstat->v_shram;
	ifstat->v_scp  = (SCP *) (ifstat->v_shram + 64);
	ifstat->v_iscp = (ISCP *)(ifstat->v_scp  + 1);
	ifstat->v_scb  = (SCB *) (ifstat->v_iscp + 1);
	ifstat->v_cb   = (CB *)  (ifstat->v_scb  + 1);
	ifstat->v_rfd  = (RFD *) (ifstat->v_cb   + ifstat->v_ncbl);
#ifdef LCLMEM
	ifstat->v_xdhead = (TBD *)(ifstat->v_rfd + ifstat->v_nrfd);
#endif
	
/* ensure we run in linear mode */
	ifstat->v_scp->scp_sysbus =
		(ifstat->v_desc.sysbus & ~SYSBUS_MODEMASK) | SYSBUS_LINEAR;

/* point scp to iscp for chip */
	ifstat->v_scp->scp_leISCP = (ISCP *)ram_swapw(ifstat->v_iscp);
	ifstat->v_scp->scp_beISCP = ifstat->v_iscp;

	ifstat->v_iscp->iscp_busy = 1;	/* initialize */

/* point scp to iscp for chip */
	ifstat->v_iscp->iscp_leSCB = (SCB *)ram_swapw(ifstat->v_scb);
	ifstat->v_iscp->iscp_beSCB = ifstat->v_scb;

/* now setup the SCB */
	ifstat->v_scb->rfa = (RFD *)ram_swapw(ifstat->v_rfd);
	ifstat->v_scb->cbl = (CB *)ram_swapw(ifstat->v_cb);

/* now setup the CB list */
	lcb = (void *)0;
	for (cb = ifstat->v_cb, ncount = ifstat->v_ncbl; ncount--; cb++) {
		cb->cb_status = 0;
		cb->cb_command = CB_LAST;
		cb->cb_leNext = (CB *)ram_swapw(cb + 1);
		cb->cb_beNext = cb + 1;
		cb->cb_bePrev = lcb;
		lcb = cb;
	}
/* make last one point to first */
	cb--;
	cb->cb_leNext = (CB *)ram_swapw(ifstat->v_cb);
	cb->cb_beNext = ifstat->v_cbnext = ifstat->v_cb;
	ifstat->v_cbhead = ifstat->v_cbtail = NULL;	/* currently executing CB chain */
	ifstat->v_cb->cb_bePrev = cb;			/* make first cb point to last */
			
/* now setup the RFD list */
	lrfd = (void *)0;
	for (rfd = ifstat->v_rfd, ncount = ifstat->v_nrfd; ncount--; rfd++) {
		rfd->rfd_stat1 = rfd->rfd_stat2 = 0;
		rfd->rfd_leNext = (RFD *)ram_swapw(rfd + 1);
		rfd->rfd_beNext = rfd + 1;
		rfd->rfd_bePrev = lrfd;
		rfd->rfd_rbd = RFD_RBD_NOTUSED;
		rfd->rfd_acount = 0;
		rfd->rfd_size = ETHER_MAX;
		lrfd = rfd;
	}
/* make last one point to first */
	rfd--;
	rfd->rfd_stat2 = RFD_LAST;				/* mark end of list for chip */
	rfd->rfd_leNext = (RFD *)ram_swapw(ifstat->v_rfd);
	rfd->rfd_beNext = ifstat->v_rfd_head = ifstat->v_rfd;
	ifstat->v_rfd->rfd_bePrev = rfd;		/* make first rfd point to last */

#ifdef LCLMEM
/* setup the xmit buffer pointers */
	ifstat->v_xdnext = ifstat->v_xdhead;
	ifstat->v_xdlast = ifstat->v_xdhead + ifstat->v_ncbl;
#endif
	return 0;
}

#if defined(hkvf82596)
doport_command(ifstat, shram, opcode)
register Ifs    ifstat;		/* a2 */
register u_int8 *shram;		/* a3 */
u_int8 opcode;
{
	register u_int32 *port = ifstat->v_port; 		/* a4 */
	register u_int32 value = (u_int32)shram & ~0xf;	/* d4 */
	
	value |= opcode;		/* port command operation */

#ifdef _UCC
	value = ram_swapw(value);
#else
@ swap d4
#endif


	*port = value;
	*port = value;
	*port = value;
	*port = value;
}
#elif defined(ie167) || defined(ie162) || defined(ie172)
doport_command(ifstat, shram, opcode)
register Ifs    ifstat;		/* a2 */
register u_int8 *shram;		/* a3 */
u_int8 opcode;
{
	register u_int16 *port = ifstat->v_port; 		/* a4 */
	register u_int32 value = (u_int32)shram & ~0xf;	/* d4 */
	
	value |= opcode;		/* port command operation */

	nop();
	*port++ = value;
	*port++ = (value>>16);
}
#endif
