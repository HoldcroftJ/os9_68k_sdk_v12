/*--------------------------------------------------------------------------!
! ltftp.c: read an OS-9 bootfile from server using TFTP						!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!               ---- OS-9/68K V2.4 released ----                            !
!	01 90/08/08 Cloned from diskboot									 kk	!
!	02 90/10/22 Improved protocol error handling and logic				 kk	!
!	03 91/12/02	memset() arguments in wrong order						 kk	!
!	04 92/11/13	ack DUP pkt to avoid stuck in reACK what server             !
!               don't want.                                            ylee !
!   05 93/06/22 Modified for UCC compatibility mode.                    bwt !
!   06 93/08/09 replaced use of inetboot.h with explicit includes (4)   gkm !
!               ---- OS-9 V3.0 Released ----                                !
!	07 93/12/03	Align emsg to even address for 68000 & CPU32        ts/ylee !
!	08 94/04/13	Change tftprcv_port and dest_port from type short to        !
!               type u_int16. connected_to{} addr from type int to          !
!               type u_int32, port from type short to type u_int16          !
!               for Solaris 2.3 and Bootp 2.3.6 that had error              !
!               NON-CONNECTED server response.                         ylee !
!   09 96/06/21 Fixed calculation for expected blocks when boot is even mgh !
!               block size.  												!
!               changed ifdev variable to ifd for ISP 2.1 defs compat   mgh !
!   10 96/09/12 Client now accepts correct block count or block         mgh !
!               count+1 so we accept boots for new and old OS9 Servers      !
!               when boot is exactly a block boundry. Note: Old servers     !
!               count the terminating empty block, new ones don't.          !
!               ---- OS-9 V3.0.3 Released ----                              !
!	11 98/01/12 Added infinite hang breakout in While(1) loop in case	mgh !
!               timeout doesn't get us out correctly         				!
!   12 98/01/27 Reworked loop so exits are based on time-out not number mgh !
!               of bad packets received.  Matched edition number to 		!
!               number printed by inetboo.c  (See history.h)				!
!   13 00/02/04 Added timestamp display, improved logging messages		kimk!
!---------------------------------------------------------------------------!
!                                                                           !
!              Copyright 1990,1993 by Microware Systems Corporation         !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include <sysboot.h>
/*#include "if.h"*/
#include "lip.h"
#include "inetboot_p.h"
#include "bootp.h"
#include "tftp.h"

char *inetname(), *show_ether();

#define TIMEOUT 50 					/* 10 ticks per second */
#define TRYCNT 8					/* Number of timeouts before aborting */

#define TFTP_RCV_PORT	20312		/* good as any? */
#define TFTP_HDR_SIZE sizeof(struct ether_header) + sizeof(UDP_IP_HEADER)+\
	sizeof(struct tftphdr)

#define IPPORT_TFTP		69
#define MAX_REWAIT      50			/* number of bad packets to receive in a
                                       row before giving up receive */

static char omsg[TFTP_HDR_SIZE + 511];			/* buffer for out tftp data */
/* make next structure emsg start from even address to
 * avoid address error under 68000 and CPU32.
 */
int         dummy;
static char emsg[TFTP_HDR_SIZE + 511];			/* buffer for err tftp data */

static char *whirly[]= {			/* Packet receive whirly character sequence */
	"|\10",
	"/\10",
	"-\10",
	"\\\10"
};

make_tftp_ack(tp, block)
register struct tftphdr *tp;
{
	tp->th_opcode = TFTP_ACK;
	tp->th_block = block;
	return 4;
}

make_tftp_req(request, name, tp, mode)
int request;
char *name, *mode;
struct tftphdr *tp;
{
	register char *cp;
	int len;
	
	tp->th_opcode = htons((u_short)request);
	if (request == TFTP_ERROR) {
		tp->th_code = (int) mode;
		cp = tp->th_data;
		strcpy(cp, name);
		cp += strlen(name);
		*cp++ = '\0';
		len = (cp - (char *)tp);
	} else {
		cp = tp->th_stuff;
		strcpy(cp, name);
		cp += strlen(name);
		*cp++ = '\0';
		strcpy(cp, mode);
		cp += strlen(mode);
		*cp++ = '\0';
		len = (cp - (char *)tp);
	}
	return len;
}

tftp(ifd, buf, nblks)
register Ifdev ifd;
char *buf;
unsigned short nblks;
{
	register struct udp_header *udphead;
	struct ip_header *iphead;
	struct tftphdr *tinp, *tout, *terr;
	register int expected_block = 1, tlen, rlen;
	u_int16 tftprcv_port = TFTP_RCV_PORT, dest_port = IPPORT_TFTP;
	struct {
/*		int addr;
*		short port;
*/
		u_int32 addr;
		u_int16 port;
	} connected_to;
	int status = 0;		/* assume we will fail */
						/*	0 = fail, 
						/*	1 = end of trans, old server doesn't count null block *? 
						/*	2 = new style server counts null block */
	int tries,firstpk=1;
	struct in_addr ina;
	auto int timeout;
	unsigned short tblks = nblks;
	int rewait_count=0;
	int whirlycnt=0;
					
	tout = (struct tftphdr *)(omsg + TFTP_HDR_SIZE - sizeof(struct tftphdr));
	terr = (struct tftphdr *)(emsg + TFTP_HDR_SIZE - sizeof(struct tftphdr));
	connected_to.addr = 0;
	connected_to.port = 0;

	if (msgprint) TS(), rprintf("tftp: fetching %d blocks\n", nblks);

/* make a tftp read-request packet and send it */
	tlen = make_tftp_req(TFTP_RRQ, bprp.bp_file, tout, "octet");
	if (msgprint>=3) TS(), rprintf("tftp: RRQ %d\n", expected_block-1);
	udp_out(ifd, tftprcv_port, dest_port, tout, tlen);
	
	timeout = TIMEOUT;			/* Set timeout value..counted down by driver */
	tries = 1;					/* Timeout counter before aborting */
	
/* do the transfer loop */
	while (1) {

/* Get a response packet .. Handle timeouts and excess timeouts */
		if (msgprint>=2) TS(), rprintf("tftp: Recv wait, Timeout=%d\n",timeout);
		if ((iphead = (struct ip_header *) 
					(*net_in)(ifd, &timeout, IPPROTO_UDP)) == 0) {
			if (++tries > TRYCNT) { /* count try's on current packet */
				if (msgprint) TS(), rprintf("tftp: timeout count exceeded .. giving up\n");
				break;		/* no tftp response .. out of while(1) */
			} else {		/* reAck again */
				if ((msgprint && firstpk) || (msgprint >=2))
					TS(), rprintf("tftp: timeout .. try: %d/%d\n",tries,TRYCNT);
				timeout = TIMEOUT;  /*reset timer for next read try */
				if (msgprint>=3) TS(), rprintf("tftp: RRQ %d\n", expected_block-1);
				udp_out(ifd, tftprcv_port, dest_port, tout, tlen);
				continue;
			}
		}

/* Validate packet received */
/* ICMP packet? */
		if (icmp_check(iphead)) {
			if (msgprint>=2) TS(), rprintf("tftp: ICMP trouble! Aborting\n");
			break;	/* something is terribly wrong (maybe with this code!) */
		}
		
		udphead = (void*)((char *)iphead + IPHEADSIZE);
		tinp = (void *)((char *)udphead + UDPHEADSIZE);

/* packed destined to our port of interest? */
		if (udphead->dest_port != tftprcv_port) {
			if (msgprint>=3) TS(), rprintf("tftp: got UDP src=%08x:%d dst=%08x:%d rewaiting\n",
				iphead->source_addr, udphead->source_port,
				iphead->dest_addr,   udphead->dest_port);
/*			if (rewait_count++ > MAX_REWAIT) {
				if (msgprint>=2) rprintf("tftp: rewait count of %d exceeded .. aborting!\n",MAX_REWAIT);
				break;
			} */
			continue;
		}

/* if not yet connected, stamp connected server */
		if (connected_to.addr == 0) {
			if (tinp->th_opcode == TFTP_ERROR) {
				if (msgprint>=2) TS(), rprintf("tftp: error from server: (%d:%s) aborting!\n",
					tinp->th_code, tinp->th_msg);
				break;
			}
			if (tinp->th_opcode == TFTP_DATA && tinp->th_block == 1) {
				connected_to.addr = iphead->source_addr;
				connected_to.port = dest_port = udphead->source_port;
				memcpy(&ina, &iphead->source_addr, sizeof ina);
				if (msgprint) TS(), rprintf("tftp: server response %s port %d\n",
					inetname(ina, 1), dest_port);
			} else {
				goto tftp_nak;		/* stale data from old server */
			}
		}

/* make sure data came from connected server */
		if (connected_to.addr != iphead->source_addr ||
			connected_to.port != udphead->source_port) {
				memcpy(&ina, &iphead->source_addr, sizeof ina);
				if (msgprint>=2) TS(), rprintf("tftp: NON-CONNECTED server response %s port %d\n",
					inetname(ina, 1), dest_port);
			goto tftp_nak;			/* data from non-connected server */
		}

/* check for error from server */
		if (tinp->th_opcode == TFTP_ERROR) {
			if (msgprint) TS(), rprintf("tftp: error from server: (%d:%s) aborting!\n",
				tinp->th_code, tinp->th_msg);
			break;
		}

/* receive the data */
		if (expected_block == tinp->th_block) {
			rlen = udphead->length-UDPHEADSIZE-4;
			memcpy(buf, tinp->th_data, rlen);
			buf += rlen;
			expected_block++;
		} else {
			/*
			 * If we receive a block num *greater* than what we expect,
			 * it must be from a connection we aborted and restarted.
			 * Send an error response to the server to get him to shut
			 * down.
			 */
			if (tinp->th_block > expected_block) {
tftp_nak:		tlen = make_tftp_req(TFTP_ERROR, "Unknown transfer ID", terr, EBADID);
				if (msgprint>=3) TS(), rprintf("tftp: RRQ %d\n", expected_block-1);
				udp_out(ifd, tftprcv_port, udphead->source_port, terr, tlen);
				memcpy(&ina, &iphead->source_addr, sizeof ina);
				if (msgprint>=2) TS(), rprintf("tftp: stale server %s port %d\n",
					inetname(ina, 1), dest_port);
			} else {
				if (msgprint>=2) TS(), rprintf("tftp: DUP %d (expected %d)\n", tinp->th_block, expected_block);
				/* ack for DUP packet */
				if (msgprint>=3) TS(), rprintf("tftp: RRQ %d\n", expected_block-1);
				udp_out(ifd, tftprcv_port, dest_port, tout, tlen);
			}
			continue; /* get next input packet */
		}
		
/* Valid TFTP Packet received .. prepare for next block */
		tlen = make_tftp_ack(tout, tinp->th_block);
		if (msgprint>=3) TS(), rprintf("tftp: RRQ %d\n", expected_block-1);
		udp_out(ifd, tftprcv_port, dest_port, tout, tlen); /* Ack packet */
		rewait_count = 0; 	/* good packet restarts rewait count */
		timeout = TIMEOUT;  /* Restart timer for each new good packet */
		firstpk= tries = 0;	/* Reset Timeout count for each new good packet */

/* Print Step whirly for each good packet */
		if (msgprint>=2) 
			TS(), rprintf("tftp: Block received: %d\n",expected_block-1);
		else if (msgprint) {
			rprintf("%s",whirly[whirlycnt++]);
			if (whirlycnt > 3) whirlycnt = 0;
		}



/* check for end */
		if (rlen != 512) {
			status = 1;
			break;
		}
	}
	
	if (status) {
		expected_block --;   /* Decrement for previous expected increment 
                                this is now the total number of blocks
							     received, including a possible empty block */

		if (msgprint>=2) TS(), rprintf("tftp: expected %d: received ", nblks);

		/* if last block is zero length, it was counted by expected block but
		   not counted as a block by newer bootp servers, so we need to 
		   decrement the count to match nblks correctly.  Blocks with a few 
		   bytes were rounded up to next block count by server so expected_block
		   should match nblks. Early CMU Servers always "rounded up the block
		   count even when the boot size was an even block size (ie mod 0) */

		if (rlen) {   /* block has more than 0 bytes */
			memset(buf, 0, 512 - rlen);	/* clean out remainder of last block */
			/* Partial block was counted..Print # of Full blocks:remainder */
			if (msgprint>=2) rprintf("%d:%d", expected_block-1, rlen);
		} else {      /* boot was size mod 512 = 0 */
			/* if old bootp server told us one block too many, Fix count */
			/*   This opens a possible hole of loosing exactly one block but */
			/*   still accepting the boot as valid */
			/* At this point expected block counted the empty block ..  */
			/*   adjust count and then check for 2 possible values      */
			/* Real number of blocks = n								*/
			/* Old Server: nblks = n+1, expected = n+1 */
			/* New Server: nblks = n  , expected = n+1 */

			if (expected_block == (nblks + 1)) {
				expected_block--;
				status = 2;
			}
			/* print actual number of data blocks "DATA" received */
			/* Does not count an empty termination block */
			if (msgprint>=2) rprintf("%d", expected_block);  
		}
		
		if (msgprint>=2) rprintf(" blocks (%dKb)\n", expected_block / 2);


		if (status == 2)
			if (msgprint>=2) TS(), rprintf("tftp: New Server Mode assumed, Transfer accepted.\n");
		return ((expected_block != nblks) ? E_EOF : 0);
	}
	return E_EOF;
}

