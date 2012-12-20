/****************************************************************************
 *	Copyright 2000 by Microware Systems Corporation							*
 *	Reproduced Under License												*
 *																			*
 *	This source code is the proprietary confidential property of			*
 *	Microware Systems Corporation, and is provided to licensee				*
 *	solely for documentation and educational purposes. Reproduction,		*
 *	publication, or distribution in any form to any party other than		*
 *	the licensee is strictly prohibited.									*
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date   	Comments                                       	   By	*
 *  --- --------	-----------------------------------------------	------- *
 *    1 00/02/04    Fixed RBD sequence problem on RX error            kimk  *
 ****************************************************************************/

#include <sysboot.h>
#include "qedvr.h"

/******** get these from systype.h **********/
#define SCC_CHAN	0			/* SCC channel being used for Ethernet */
#define	DPRO_SCC1	0x0			/* SCC1 DPRAM base offset GET FROM SYSTYPE.H!!! */
/******** get these from systype.h **********/

#define NUM_RX_BD	4			/* number of receive buffer descriptors */
#define NUM_TX_BD	4			/* number of transmit buffer descriptors */

#define ETH_PHYMIN	64			/* minimum physical size for Ethernet packet */
#define ETH_PHYMAX	1518		/* maximum physical size for Ethernet packet */
#define RX_MRU		ETH_PHYMAX 	/* maximum receive buffer size */

#define RND_LONG(x)	(((x) + 4) & -4)	/* round to longword size */

static u_int8 rxbuff[NUM_RX_BD][RND_LONG(RX_MRU)];	/* buffer for rx frames (rounded) */
static QUICC_BD *currRbd, *headRbd;	/* RX BD pointers */
static QUICC_BD	*currTbd, *headTbd;	/* TX BD pointers */

static	QUICC *qp;					/* pointer to quicc registers */
static	struct ethernet_pram *ep;	/* pointer to ethernet scc pram */
#ifdef DEBUG
static	u_int32 txcnt, rxcnt, txerr, rxerr, rxpoll;		/* statistic counters */
static	u_int32 rxErrCnt[8];
extern u_int16 msgprint, bpt_test;
extern u_int32 bpt_curtick;
#endif

#define PA_RXDx		(1<<0)
#define PA_TXDx		(1<<1)

#define PC_RTSx		(1<<0)
#define PC_CTSx		(1<<4)
#define PC_CDx		(1<<5)

/* perform chip level init of SCC1 for ethernet operation */
qe_init(struct ifdev *ifd)
	{

	register Ifs ifstat = (Ifs)ifd->if_static;
	register struct qe_desc *qe_desc;
	QUICC_BD *bd;				/* buffer desc pointer */
	int i;
	
	ep = (struct ethernet_pram *)ifstat->v_port;
	qp = (QUICC*)((unsigned)ep  & 0xfffff000);  /* point to quicc registers */

	/*
	** setup PAPAR, PADIR, PAODIR to enable IO for SCC1 Ethernet
	*  Enable RXD1 and TXD1 on SCC
	*/
	/* Configure port A for TXD and RXD pins */
	qp->pio_papar |=  (u_short)((PA_RXDx | PA_TXDx) <<  (2 * SCC_CHAN));
	qp->pio_padir &= ~(u_short)((PA_RXDx | PA_TXDx) <<  (2 * SCC_CHAN));
	qp->pio_paodr &= ~(u_short)(PA_RXDx << ((2 * SCC_CHAN) + 1));

	/* Configure port C for CTS (CLSN), and CD (RENA) pins */
	qp->pio_pcpar &= ~(u_short)((PC_CTSx | PC_CDx) << (2 * SCC_CHAN));
	qp->pio_pcdir &= ~(u_short)((PC_CTSx | PC_CDx) << (2 * SCC_CHAN));
	qp->pio_pcso  |=  (u_short)((PC_CTSx | PC_CDx) << (2 * SCC_CHAN));

	/*
	 * initialize Ethernet parameters
	 */
	ep->rbase = DPRO_SCC1;					/* rx BDs */
	ep->tbase = DPRO_SCC1 + NUM_RX_BD * 8;	/* tx BDs */
	ep->rfcr = 0x18;						/* FC for RX: SDMA */
	ep->tfcr = 0x18;						/* FC for TX: SDMA */
	ep->mrblr = RND_LONG(RX_MRU);			/* max receive buffer length */
	qp->cp_cr = INIT_RXTX_PARAMS+CMD_FLAG;	/* init params */
	ep->c_mask = 0xdebb20e3;	/* 32 bit ccitt crc */
	ep->c_pres = 0xffffffff;	/* 32 bit ccitt crc */
	ep->crcec = 0;				/* crc error counter */
	ep->alec = 0;				/* clignment error counter */
	ep->disfc = 0;				/* discard frame err counter */
	ep->pads = 0;				/* short frame pad chars */
	ep->ret_lim = 15;			/* retry limit */
	ep->mflr = ETH_PHYMAX;	 	/* max frame length */
	ep->minflr = ETH_PHYMIN; 	/* min frame length */
	ep->maxd1 = ETH_PHYMAX;	 	/* max dma1 length */
	ep->maxd2 = ETH_PHYMAX;	 	/* max dma2 length */
	ep->dma_cnt = ETH_PHYMAX;	/* rx max dma */
	ep->gaddr1 = ep->gaddr2 = ep->gaddr3 = 	ep->gaddr4 = 0; /* group address filter */
	ep->iaddr1 = ep->iaddr2 = ep->iaddr3 = 	ep->iaddr4 = 0; 
	ep->taddr_h = ep->taddr_m = ep->taddr_l = 0; 
	/* setup out physical address */
	ep->paddr_h = (ifd->if_paddr[5] << 8) | ifd->if_paddr[4];
	ep->paddr_m = (ifd->if_paddr[3] << 8) | ifd->if_paddr[2];
	ep->paddr_l = (ifd->if_paddr[1] << 8) | ifd->if_paddr[0];		
	
	/* now setup rx BD's before starting up ethernet */
	headRbd = currRbd = bd = (QUICC_BD *)((unsigned)qp + (unsigned)ep->rbase);
	for(i=0 ; i < NUM_RX_BD; i++)
		{
		bd->buf = rxbuff[i];		/* set rx buffer to here */
		bd->length = 0;
		bd->status = R_E;
		bd++;					/* point to next bd */
		}
	bd--;						/* backup one */
	bd->status |= R_W;			/* set wrap bit */

 	/* we are not xmitting yet so just set up */
	headTbd = currTbd = bd = (QUICC_BD *)((unsigned)qp + (unsigned)ep->tbase);
	for(i=0 ; i < NUM_TX_BD; i++)
		{
		bd->buf = 0;
		bd->length = 0;
		bd->status =  T_PAD+T_L+T_C+T_I;
		bd++;					/* point to next bd */
		}
	bd--;						/* backup one */
	bd->status |= T_W;			/* set wrap bit */

	qp->scc_regs[SCC_CHAN].scc_gsmrb = 0;
	/*
	** scc1 conf register is set to rx automatic control of cls & ren,
	** more setup time for eest and ethernet mode ENR & ENT are not enabled.
	*/
	qp->scc_regs[SCC_CHAN].scc_gsmra = 0x1088000c;	
	/*
	** ethernet protocol specific mode register
	** rx short frames, 32 bit CRC, reject all loop operation and begin
	** searching for the start frame delimiter 22 bits after 
	*/
	qp->scc_regs[SCC_CHAN].scc_psmr = 0x080a;		/* no short frm, no promiscuous */
	qp->scc_regs[SCC_CHAN].scc_dsr = 0xd555;

	/* Configure port C for RTS (TENA) pin */
	qp->pio_pcpar |=  (u_short)(PC_RTSx << SCC_CHAN);
	qp->pio_pcdir &= ~(u_short)(PC_RTSx << SCC_CHAN);

	qp->scc_regs[SCC_CHAN].scc_gsmra = 0x1088003c;		/* enable rx & tx */

#ifdef DEBUG
	txcnt = rxcnt = 0;		/* clear packet counters */
	txerr = rxerr = 0;		/* initialize error counters */
	rxpoll = 0;
#endif
	}

#ifdef DEBUG
void
dumpRXErr(void)
{
	TS(), rprintf("RERR CNT: B7=%u B6=%u LG=%u NO=%u SH=%u CR=%u OV=%u CL=%u\n",
		rxErrCnt[7], rxErrCnt[6], rxErrCnt[5], rxErrCnt[4],
		rxErrCnt[3], rxErrCnt[2], rxErrCnt[1], rxErrCnt[0]);
}
#endif

qe_term(struct ifdev *ifd)
	{
	qp->scc_regs[SCC_CHAN].scc_gsmra = 0x1088000c;		/* disable RX & TX */
	qp->pio_pcpar &= ~0x0001;	/* disable TENA */
#ifdef DEBUG
	if (msgprint)
	{
		TS(), rprintf("QE TERM: CRCEC=%u ALEC=%u DISFC=%u\n",
				ep->crcec, ep->alec, ep->disfc);
		TS(), rprintf("QE TERM: RXCNT=%u RXERR=%u TXCNT=%u TXERR=%u\n",
				rxcnt, rxerr, txcnt, txerr);
		dumpRXErr();
	}
#endif
	}
	
/* xmit a buffer pointed to by mbuf of mlen */
qe_xmit(struct ether_header *eh, Ifs ifstat, int mlen)
	{
	QUICC_BD *nextxbd;

	nextxbd = (QUICC_BD *)((unsigned)qp + (unsigned)ep->tbptr);		

	while(nextxbd->status & T_R);	/* wait for this packet to complete */

	/* just in case he was busy */
	nextxbd = (QUICC_BD *)((unsigned)qp + (unsigned)ep->tbptr);		

	nextxbd->length = mlen;
	nextxbd->buf = (unsigned char *)eh;
	
    nextxbd-> status &= T_W;
	nextxbd->status |= T_R+T_PAD+T_L+T_C;		/* BONZAI */

#ifdef DEBUG
	txcnt++;
#endif
	}


qe_recv(struct ifdev *ifd, void *buf)
	{
	register Ifs 	ifstat = (Ifs) ifd->if_static;		
	int rlen;


#ifdef DEBUG
	rxpoll++;
	/*
	 * turn on message when it's taking
	 * too long... >30 seconds
	 */
	if (msgprint < 10 && bpt_curtick >= 300) {
		msgprint = 10;
		bpt_test = 10;
		TS(), rprintf("MSG WAKE: RXCNT=%u RXERR=%u TXCNT=%u TXERR=%u RPOLL=%u\n",
				rxcnt, rxerr, txcnt, txerr, rxpoll);
		dumpRXErr();
	}
#endif

	if(currRbd->status & R_E)
		return(0);	/* this buffer not ready */

	if((currRbd->status & R_ERROR) == 0)
		{	/* we only want data from packets with no errors */
		rlen = currRbd->length - 4;		/* we don't need crc */
		memcpy(buf,currRbd->buf,rlen);
#ifdef DEBUG
		rxcnt++;
		if (msgprint>=6)
		{
			TS(), rprintf("QE RECV: rb=%08x rberr=%04x len=%d scce=%04x\n",
				currRbd, currRbd->status, currRbd->length, qp->scc_regs[0].scc_scce);
			if (msgprint>=7) {
				TS(), rprintf("QE RECV:   CRCEC=%u ALEC=%u DISFC=%u\n",
						ep->crcec, ep->alec, ep->disfc);
				dumpRXErr();
			}
		}
#endif
		}
	else
		{
		rlen = 0;
#ifdef DEBUG
		if (currRbd->status & (1<<0)) rxErrCnt[0]++;
		if (currRbd->status & (1<<1)) rxErrCnt[1]++;
		if (currRbd->status & (1<<2)) rxErrCnt[2]++;
		if (currRbd->status & (1<<3)) rxErrCnt[3]++;
		if (currRbd->status & (1<<4)) rxErrCnt[4]++;
		if (currRbd->status & (1<<5)) rxErrCnt[5]++;
		if (currRbd->status & (1<<6)) rxErrCnt[6]++;
		if (currRbd->status & (1<<7)) rxErrCnt[7]++;
		if (msgprint>=6)
		{
			if (msgprint>=7) {
				int i;
				QUICC_BD *bd;
				u_int16 bdstat[NUM_RX_BD];

				/*
				 * snarf status into temp array
				 */
				for (i=0, bd=headRbd; i < NUM_RX_BD; i++, bd++)
					bdstat[i] = bd->status;

				for (i=0, bd=headRbd; i < NUM_RX_BD; i++, bd++) {
					TS(), rprintf("QE RERR: %crb[%d]=%08x rberr=%04x len=%d CRCEC=%u ALEC=%u DISFC=%u\n",
								bd == currRbd ? '*' : ' ', i, bd, bdstat[i], bd->length, ep->crcec, ep->alec, ep->disfc);
				}
			} else {			
				TS(), rprintf("QE RERR: rb=%08x rberr=%04x len=%d CRCEC=%u ALEC=%u DISFC=%u\n",
				currRbd, currRbd->status, currRbd->length, ep->crcec, ep->alec, ep->disfc);
			}
			dumpRXErr();
		}
		rxerr++;
#endif
		}

/*	currRbd->length = 0; */
	if(currRbd->status & R_W)
	{
		currRbd->status = R_E | R_W;
		currRbd = headRbd;		
	} else
	{
		currRbd->status = R_E;
		currRbd++;
	}

	return(rlen);
	}
