/*--------------------------------------------------------------------------!
! lbootp.c: make BOOTP request, send to network, receive resp. from server	!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!               ---- OS-9/68K V2.4 released ----                            !
!	01 90/08/08 Cloned from diskboot									kk	!
!	02 90/10/24 Changed timeout to allow at least 5 sec for server		kkk	!
!				response.  Unix bootp servers are just not that snappy!		!
!	03 91/12/19 Added call to bpt_bootp to allow cpu-specific	        kk	!
!				modification of the bootp header possibly from nvram		!
!   04 92/11/11 Retry after several rewaits, don't stuck in rewait     ylee !
!   05 93/06/17 Modified for UCC compatibility mode.                    bwt !
!   06 93/08/09 replaced use of inetboot.h with explicit includes (4)   gkm !
!               ---- OS-9 V3.0 Released ----                                !
!   07 93/10/05 Added retry/redo count limits in bootp()				bat	!
!   08 95/03/02 Changed bootp src IP address to INADDR_ANY              mgh !
!               Converted ifdev to ifd for compatibility with new defs      !
!	09 96/31/05 Added rfc1048 vendor magic cookie field loads           mgh !
!               ---- OS-9 V3.0.3 Released ----                              !
!   10 98/01/12 Messed with Transaction ID settings                     mgh !
!               Simplified retry/rewait to try/wait counts and printing     !
!               Found driver problem was adding 5 to timeout for each       !
!               packet pitched packet. (each entry to driver)               !
!               Added check for bootfile size of 0 (zero) length            !
!               NOTE: Removed sending of Name "OS9Boot" which caused some   !
!               some servers grief... couldn't overide the name.			!
!   12 98/02/17 Changed edition number to match printing inetboot.l     mgh !
!               edition number (See history.h)                              !
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


int TRYCNT=6;
/*#define RETRYCNT	2*/
int WAITCNT=5;
/*#define REWAITCNT	5*/

struct bootp bprp;		/* bootp response packet */
unsigned short in_bootsize;

static unsigned long xid;

char *show_ether(enaddr)
register u_char enaddr[6];
{
	static char buf[64];
	
	rsprintf(buf, "%x:%x:%x:%x:%x:%x",
		enaddr[0], enaddr[1], enaddr[2],
		enaddr[3], enaddr[4], enaddr[5]);
	return buf;
}

char *inetname(in, number)
struct in_addr in;
{
	static char buf[64];
	register char *cp = NULL;
	register u_char *ucp = (u_char *)&in;
	
	if (in.s_addr == INADDR_ANY) strcpy(buf, "*");
	else if (cp) strcpy(buf, cp);
	else rsprintf(buf, "%u.%u.%u.%u", ucp[0], ucp[1], ucp[2], ucp[3]);
	return buf;
}

make_bootp(buf, hwaddr)
char *buf;
char *hwaddr;
{
    char c;
    int base = 16;
    int i;
    u_long val=0;
	struct bootp *bph = (struct bootp *)buf;

/*
 * call cpu-specific routine to fill in the bootpc message
 * 
 */
	memset(bph, 0, sizeof(struct bootp));	/* zero entire message */
 	if (bpt_bootp(bph) == 0) {
		memcpy(bph->bp_chaddr, hwaddr, 6);	/* client hardware address */

/* We no longer request a specific filename .. must be set at server now. */
/*		strcpy(bph->bp_file, "os9boot");	/* bootfile name */
	}

/*
 * Comput unique Transaction ID from Ethernet (hwaddr) address 
 * This should be unique to each board and unique for each new request
 * (use of RT Clock is one possibility)
 */

	memcpy(&val, hwaddr+2, 4);  /* use low order 4 bytes of Enet addr */
    
/*
 * we'll always fill in these
 */
	bph->bp_op = BOOTREQUEST;			/* boot request */
	bph->bp_htype = ARPHRD_ETHER;		/* 10mb ethernet */
	bph->bp_hlen = 6;					/* hardware address length */
	bph->bp_xid = xid = bpt_rseed = val;		/* transaction id number */
	bph->bp_secs = bpt_curtick / 10;	/* time since starting to boot */
	bph->bp_vend[0] = VMC_0;			/* load vendor magic cookie rfc1048 */
	bph->bp_vend[1] = VMC_1;
	bph->bp_vend[2] = VMC_2;
	bph->bp_vend[3] = VMC_3;
	return sizeof(struct bootp);
}

/*
 * calculate exponention average backoff time for bootp retries
 */

static tbackoff(try)
register int try;
{
	register short itime;
	
	if (try > 6) try = 6;		/* limit to reasonableness (~60 sec.) */

	bpt_rseed = ((bpt_rseed * 39709 + 13) % 65537) & 0xffff;

	itime = bpt_rseed & ~((unsigned short)(~0 << try));

	return itime * 10;			/* convert to .1 sec */
}


/*
 * Create, transmit Bootp request packet and hankle reponses. 
 */
struct bootp *bootp(ifd)
Ifdev ifd;
{
	char *odata;
	struct ip_header *iphead;
	struct udp_header *udphead;
	struct bootp *bphr;
	int rlen, timeout, nto;
	struct in_addr ina;
	auto int net_time;
	auto int try_count;
	auto int wait_count;

/* make a bootp record */
	odata = net_outbuf + sizeof(UDP_IP_HEADER) + sizeof(struct ether_header);
	rlen = make_bootp(odata, ifd->if_paddr);	
	(IA_SIN(ifd)->sin_addr).s_addr = INADDR_ANY;
	(ID_SIN(ifd)->sin_addr).s_addr = INADDR_BROADCAST;

	try_count = 0;  

redo:
	timeout = nto = 0;
	
retry:
	bphr = (struct bootp *)odata;
	
	if (try_count++) {
		xid += xid; /* Create new packet id */
		bphr->bp_xid = xid; /* change id for each retrie */
		if (try_count > TRYCNT) {
			if (msgprint) TS(), rprintf("bootp: Exiting after %d trys\n", TRYCNT);
			return NULL;
		}
	}

	if (msgprint) 
		TS(), rprintf("bootp: %s broadcasting for server..try %d/%d\n", 
	 		show_ether(bphr->bp_chaddr),try_count,TRYCNT);

/* broadcast bootp request packet */
	udp_out(ifd, IPPORT_BOOTPC, IPPORT_BOOTPS, odata, rlen);

/* wait for response */
/*	timeout += 50;		/* wait at least 5 seconds */
	timeout += 60;		/* wait at least 6 seconds .. testing */

	wait_count = 0;

rewait:
	wait_count++;
/*	if (wait_count++) { /* note: first time thru counted as a wait */
/*		if (wait_count > WAITCNT) { 								*/
/*			rprintf("bootp: Retry after %d waits\n", WAITCNT); 		*/
/*			timeout = tbackoff(nto++); 								*/
/*			goto retry; 											*/
/*		} 															*/
/*	} 																*/

	if (msgprint>=2) 
		TS(), rprintf("bootp: recv wait %d for %d ticks (10/second)\n", 
			wait_count,timeout);
	
/* Go get a packet */
	iphead = (struct ip_header *)(*net_in)(ifd, &timeout, IPPROTO_UDP);

	if (iphead == 0) {			/* timeout */
		timeout = tbackoff(nto++);
		if (msgprint>=2) TS(), rprintf("bootp: Error or timeout .. Retry request\n");
		goto retry;
	}
	
	udphead = (void*)((char *)iphead + IPHEADSIZE);
	bphr = (void *)((char *)udphead + UDPHEADSIZE);

/* check for valid reply */
	/* THIS was an && .. changed to || (mgh) */
	if (udphead->source_port != IPPORT_BOOTPS ||
			udphead->dest_port != IPPORT_BOOTPC) {
		if (msgprint>=3) TS(), rprintf("bootp: got UDP packet src=%08x:%d dst=%08x:%d \n",
			iphead->source_addr, udphead->source_port,
			iphead->dest_addr,   udphead->dest_port);
		goto rewait;
	}

	memcpy(&ina, &bphr->bp_siaddr, 4);
	if (msgprint) 
		TS(), rprintf("GOT BOOTP RESPONSE from server %s!\n", inetname(ina, 1));

/* Verify returned packet is for us using ethernet address */
	if (memcmp(&bphr->bp_chaddr, &ifd->if_paddr, 6)) {	
		if (msgprint) TS(), rprintf("..but not for my ethernet address!\n");
		goto rewait;
	}

/* Verify returned packet is for us using xid */
	if (xid != bphr->bp_xid) {
		if (msgprint) TS(), rprintf("..but not my xid number! (expected %x, got %x)\n", xid, bphr->bp_xid);
		goto rewait; 
	}
	
/* handle BOOTP response */
	memcpy(&bprp, bphr, sizeof(struct bootp));
	memcpy(&ina, &bphr->bp_yiaddr, 4);
    if (msgprint) TS(), rprintf("My IP address will be: %s (0x%08x)\n", inetname(ina, 1), ntohl(bphr->bp_yiaddr));
/*	rprintf("My tftp bootfile is: %s\n", *bphr->bp_file ? (char *)bphr->bp_file : "<null>"); */

	if (*bphr->bp_file) {
		if (msgprint) TS(), rprintf("My tftp bootfile is: %s\n", (char *)bphr->bp_file);
	}
	else {
		u_int32 etime;
		if (msgprint) TS(), rprintf("bootp: fatal error - no bootfile name in BOOTP record\n");
		if (msgprint>=2) TS(), rprintf("bootp: delay 10 seconds...\n");
		etime = bpt_curtick + 100;			/* snooze for 10 seconds */
		while (bpt_curtick < etime);
		if (msgprint>=2) TS(), rprintf("done.\n");
		goto redo;
	} 
	
	if (getvend_1048(bphr->bp_vend, TAG_BOOTSIZE, &in_bootsize) == 0) {
		u_int32 etime;
		if (msgprint) TS(), rprintf("bootp: fatal error - no bootsize tag in BOOTP record\n");
		if (msgprint>=2) TS(), rprintf("bootp: delay 10 seconds...\n");
		etime = bpt_curtick + 100;			/* snooze for 10 seconds */
		while (bpt_curtick < etime);
		if (msgprint>=2) TS(), rprintf("done.\n");
		goto redo;
	} 

	if (in_bootsize == 0){
		u_int32 etime;
		if (msgprint) TS(), rprintf("bootp: error - bootfile size is zero\n");
		if (msgprint>=2) TS(), rprintf("bootp: delay 10 seconds...\n");
		etime = bpt_curtick + 100;			/* snooze for 10 seconds */
		while (bpt_curtick < etime);  
		if (msgprint>=2) TS(), rprintf("done.\n");
		goto redo;
	}
	
	if (msgprint) TS(), rprintf("My bootfile size is: %d (512-byte) blocks\n", in_bootsize);

	if (getvend_1048(bphr->bp_vend, TAG_SUBNET_MASK, &ina)) {
		if (msgprint) TS(), rprintf("My subnet mask is: %s\n", inetname(ina, 1));
	}
	else if (msgprint) TS(), rprintf("<<no subnetmask tag>>\n");

/*	if (getvend_1048(bphr->bp_vend, TAG_TIME_OFFSET, &net_time))         */
/*		if (msgprint) rprintf("Network time offset is: %u\n", net_time); */
/*	else if (msgprint) rprintf("<<no timeoffset tag>>\n");               */

	return bphr;
}


getvend_1048(vend, tag, ptr)
register unsigned char *vend;
char tag;
void *ptr;
{
	int doit = 0, len;
	unsigned char *end = vend + 64;
	
	if (!(vend[0] == 99 && vend[1] == 130 && vend[2] == 83 && vend[3] == 99))
		return 0;

	vend += 4;
	while (vend <= end && *vend != 255) {
		if (*vend == tag) ++doit;
		if (*vend++ != TAG_PAD) {
			len = *vend++;
			if (doit) {
				memcpy(ptr, vend, len);
				break;
			}
			vend += len;
			len = 0;
		}
	}
	return len;
}		

#if 0
dumpmap(array, size, head)
register unsigned char *array;
register int size;
{
	register int count = 0;

	if (head) {
		rprintf(
		"                                        1 1 1 1 1 1 1 1  1 1 1 1 1 1 1 1\n");
		rprintf(
		"Addr  0 1 2 3 4 5 6 7  8 9 a b c d e f  0 1 2 3 4 5 6 7  8 9 a b c d e f");
	}
	for (count = 0; size-- > 0; count++) {
		if ((count % 32) == 0) rprintf("\n%04X ", count);
		else if ((count % 8) == 0) putchar(' ');
		rprintf("%02X", *array++);
	}
	putchar('\n');
}
#endif
