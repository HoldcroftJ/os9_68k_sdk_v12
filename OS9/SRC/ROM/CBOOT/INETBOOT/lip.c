/*--------------------------------------------------------------------------!
! lip.c: low-level implementation of IP protocol							!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!               ---- OS-9/68K V2.4 released ----                            !
!	01 90/08/08 Cloned from diskboot									kk	!
!   02 93/06/17 Modified for UCC compatibility mode.                    bwt !
!   05 93/08/09 replaced use of inetboot.h with explicit includes (4)   gkm !
!               ---- OS-9 V3.0 Released ----                                !
!   06 94/08/04 Convert code to use merged header file.                ylee !
!   07 96/06/21 changed ifdev variable to ifd for ISP 2.1 defs compat   mgh !
!               Fixed rlen parms missing type, used udphead->length for mgh !
!               checksum calc instead of rlen.. some unix drivers add       !
!               trash bytes at end of udp area if packet < 64 bytes ..      !
!               these throw off rlen count coming up from driver            !
!               ---- OS-9 V3.0.3 Released ----                              !   
!	08 98/01/23 Added printing of ICMP Code to printed error msg        mgh !
!   12 98/02/17 Bumped edition number to match EDITION printed by       mgh !
!               inetboot.c (see history.c)                                  !
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


u_int16 in_checksum();

ip_inp(ifd, buffer, rlen)
Ifdev ifd;
char *buffer;
int rlen;
{
	register struct ip_header *iphead = (struct ip_header *)buffer;

	if (msgprint>=4) TS(), rprintf("IP INP: rlen=%d\n",rlen);
	
/* check header size */
	if (rlen < IPHEADSIZE) {
		if (msgprint>=4) TS(), rprintf("%%runt!\n");
		return 0;
	}
	
/* check IP version */
	if (((iphead->vers_ihl & IP_VERS_MASK) >> 4) != IPVERSION) {
		if (msgprint>=4) TS(), rprintf("%%version!\n");
		return 0;
	}

/* check checksum */
	if (in_checksum(0, iphead, (iphead->vers_ihl & IP_IHL_MASK) << 2)) {
		if (msgprint>=4) TS(), rprintf("%%checksum ip!\n");
		return 0;
	}	

	if ((ntohs(iphead->fl_fr_off) & IP_FRAG_MASK) ||
									(ntohs(iphead->fl_fr_off) & MORE_FRAGS)) {
		/* should handle frags here! */
		if (msgprint>=4) TS(), rprintf("%%frag!\n");
		return 0;
	}

#ifdef DEBUG
	{
	struct in_addr ina;
	if (msgprint>=4) {
		memcpy(&ina, &iphead->source_addr, sizeof ina);
        if (msgprint>=4) rprintf("%s->", inetname(&ina, 1));
		memcpy(&ina, &iphead->dest_addr, sizeof ina);
		if (msgprint>=4) rprintf("%s: ", inetname(&ina, 1));
	}
	}
#endif
	
	if (msgprint>=5) {
		int i;
		unsigned char *ptr = (void *)iphead;
		TS(), rprintf("IP INP:   data=");
		for (i = 0; i < 16; i++) {
			rprintf("%02x", ptr[i]);
			if (i & 1) rprintf(" ");
		}
		rprintf("\n");
		if (msgprint>=6) {
	 		TS(), rprintf("IP INP:   data=");
			for (; i < 32; i++) {
				rprintf("%02x", ptr[i]);
				if (i & 1) rprintf(" ");
			}
			rprintf("\n");
		}
	}
	switch (iphead->protocol) {
	case IPPROTO_ICMP:	return icmp_in(ifd, buffer, rlen);
	case IPPROTO_UDP:	return  udp_in(ifd, buffer, rlen);
	default:
	case IPPROTO_TCP:	return  -1; /*tcp_in(ifd, buffer, rlen);*/
	}
}

ip_out(ifd, buffer, rlen, proto)
Ifdev ifd;
char *buffer;
int rlen;
unsigned short proto;
{
	register struct ip_header *iphead;
	static unsigned short ip_ident = 0;
	
	iphead = (struct ip_header *)(buffer - sizeof(struct ip_header));
	rlen += sizeof(struct ip_header);
		
/* fill in IP header */
	iphead->vers_ihl = (IPVERSION << 4) + (sizeof(IP_HEADER) >> 2);
	iphead->tos = 0;
	iphead->ident = htons(ip_ident++);
	iphead->fl_fr_off = 0;
	iphead->ttl = -1;
	iphead->protocol = proto;
	iphead->length = rlen;
		
/* fill in our source IP address */
	iphead->source_addr = (IA_SIN(ifd)->sin_addr).s_addr;
	iphead->dest_addr = (ID_SIN(ifd)->sin_addr).s_addr;
		
/* IP header checksum */
	iphead->checksum = 0;  /* initialize to 0 */	
	iphead->checksum = htons(in_checksum(0, iphead,
									(iphead->vers_ihl & IP_IHL_MASK) << 2));
	
/* check if fragmentation required */
	if (rlen > ifd->if_mtu) {
		if (msgprint>=4) TS(), rprintf("IP OUT:frag req! len=%d, mtu=%d\n",rlen, ifd->if_mtu);
		if ((ntohs(iphead->fl_fr_off) & DONT_FRAG) == 0) {
			/* do fragment processing */
		}
	} else {
		if (msgprint>=4) TS(), rprintf("IP OUT: dest=%08x len=%d\n", iphead->dest_addr, rlen);
		if (msgprint>=5) {
			int i;
			unsigned char *ptr = (void *)iphead;
			TS(), rprintf("IP OUT:   data=");
			for (i = 0; i < 16; i++) {
				rprintf("%02x", ptr[i]);
				if (i & 1) rprintf(" ");
			}
			rprintf("\n");
			if (msgprint>=6) {
				TS(), rprintf("IP OUT:   data=");
				for (; i < 32; i++) {
					rprintf("%02x", ptr[i]);
					if (i & 1) rprintf(" ");
				}
				rprintf("\n");
			}
		}
		(*ifd->if_send)(ifd, iphead, rlen, iphead->dest_addr);
	}
}

icmp_in(ifd, buffer, rlen)
Ifdev ifd;
char *buffer;
int rlen;
{
	register struct ip_header *iphead = (struct ip_header *)buffer;
	register struct icmp_header *icmphead;
	int dest_ip, icmp_len;
	
	icmphead = (Icmp_header)(buffer + ((iphead->vers_ihl & IP_IHL_MASK) << 2));

	if (msgprint>=4) TS(), rprintf("ICMP ");

/* check ICMP header checksum */
	if (in_checksum(0, icmphead, icmp_len = rlen - 
									((iphead->vers_ihl & IP_IHL_MASK) <<2))) {
		if (msgprint>=4) rprintf("%%checksum icmp!\n");
		return 0;
	}

	if (msgprint>=4) rprintf("type = %d, code = %d", icmphead->icmp_type,icmphead->icmp_code);
	
/* handle ICMP request */
	switch (icmphead->icmp_type) {
	case ICMP_ECHOREQ:
		icmphead->icmp_type = ICMP_ECHOREP;
		dest_ip = iphead->source_addr;
		iphead->source_addr = iphead->dest_addr;
		iphead->dest_addr = dest_ip;
		break;
	default:
		dest_ip = 0;/* do nothing */
	}
	
	if (dest_ip) {
		icmphead->icmp_checksum = 0;
		icmphead->icmp_checksum = htons(in_checksum(0, icmphead, icmp_len));
		ip_out(ifd, buffer, rlen, IPPROTO_ICMP);
	}
	if (msgprint>=4) rprintf("\n");
	
	return IPPROTO_ICMP;
}

icmp_check(iphead)
register struct ip_header *iphead;
{
	register struct icmp_header *icmphead;
	register int trouble = 0;
		
	icmphead = (Icmp_header)(((char*)iphead) + 
								((iphead->vers_ihl & IP_IHL_MASK) <<2));

	switch (icmphead->icmp_type) {
	case ICMP_DESTUNREACH:		/* maybe others */
/*	case ICMP_REDIRECT: */
		trouble = 1;
	}
	return trouble;
}

udp_in(ifd, buffer, rlen)
Ifdev ifd;
char *buffer;
int rlen;
{
	register struct ip_header *iphead = (struct ip_header *)buffer;
	register struct udp_header *udphead;
	UDP_PSEUDOHEAD udp_phead;	/* for checksum calc. */

	udphead = (Udp_header)(buffer +	((iphead->vers_ihl & IP_IHL_MASK) <<2));
	rlen = rlen - ((iphead->vers_ihl & IP_IHL_MASK) <<2);	/* less IP header */
	
#ifdef DEBUG
	if (msgprint>=4) TS(), rprintf("UDP");
#endif

/* header length check */
	if (rlen < sizeof(UDP_HEADER)) {
		if (msgprint>=4) rprintf("%%header!\n");
		return 0;
	}
	
/* build pseudo header to calculate udp checksum */
	udp_phead.source_addr = ntohl(iphead->source_addr);
	udp_phead.dest_addr = ntohl(iphead->dest_addr);
	udp_phead.zero = 0;
	udp_phead.proto = IPPROTO_UDP;
	udp_phead.length = ntohs(udphead->length);
	
/* get data from UDP datagram */
#ifdef DEBUG
	if (msgprint>=4) rprintf("SP=%d DP=%d chksum=%x len=%d rlen=%d\n",
		udphead->source_port, udphead->dest_port, udphead->checksum,
		udphead->length, rlen);
	/* rprintf(" len=%d rlen=%d\n",udphead->length, rlen);  */
#endif

	if (udphead->checksum) {		/* bsd4.2 doan need no stinkink checksums */
/*		if (in_checksum(&udp_phead, udphead, rlen)) { */
		if (in_checksum(&udp_phead, udphead, udphead->length)) {
			if (msgprint>=4) rprintf("%%checksum udp!\n");
			return 0;
		}
	}

		
	return IPPROTO_UDP;
}

udp_out(ifd, src_port, dst_port, buffer, rlen)
Ifdev ifd;
unsigned short src_port, dst_port;
char *buffer;
int rlen;
{
	register struct udp_header *udphead;
	UDP_PSEUDOHEAD udp_phead;	/* for checksum calc. */
	int plen;					/* total length of udp message */
		
/* make header pointers */
	udphead = (struct udp_header *)(buffer - sizeof(struct udp_header));
	
	udphead->source_port = src_port;
	udphead->dest_port = dst_port;
	udphead->length = plen = rlen + sizeof(struct udp_header);
	udphead->checksum = 0;

	udp_phead.source_addr = (IA_SIN(ifd)->sin_addr).s_addr;
	udp_phead.dest_addr = (ID_SIN(ifd)->sin_addr).s_addr;
	udp_phead.zero = 0;
	udp_phead.proto = IPPROTO_UDP;
	udp_phead.length = ntohs(plen);	

	udphead->checksum = htons(in_checksum(&udp_phead, udphead, plen));
	
	ip_out(ifd, udphead, plen, IPPROTO_UDP);
}

#if 0
tcp_in(ifd, buffer, rlen)
Ifdev ifd;
char *buffer;
int rlen;
{
	register struct ip_header *iphead = (struct ip_header *)buffer;
	register struct tcp_header *tcphead;
	PSEUDOHEAD tcp_phead;	/* for checksum calc. */
	
	tcphead = (Tcp_header)(buffer + (iphead->ihl << 2));
	rlen -= (iphead->ihl << 2);	/* less IP header */
	
	if (msgprint>=4) TS(), rprintf("TCP ");

/* header length check */
	if (rlen < sizeof(TCP_HEADER)) {
		if (msgprint>=4) rprintf("%%header!\n");
		return 0;
	}
	
/* build pseudo header to calculate udp checksum */
	tcp_phead.source_addr = ntohl(iphead->source_addr);
	tcp_phead.dest_addr = ntohl(iphead->dest_addr);
	tcp_phead.zero = 0;
	tcp_phead.proto = IPPROTO_TCP;
	tcp_phead.length = ntohs(iphead->length) - sizeof(IP_HEADER);
	
	if (tcphead->checksum) {		/* bsd4.2 doan need no stinkink checksums */
		if (in_checksum(&tcp_phead, tcphead, tcp_phead.length)) {
			if (msgprint>=4) rprintf("%%checksum tcp!\n");
	dumpmap(&tcp_phead, sizeof(tcp_phead), 0);
			return 0;
		}
	}

/* get data from TCP datagram */
	if (msgprint>=4) rprintf("SP=%d DP=%d seq=%08X\n",
		tcphead->source_port, tcphead->dest_port, tcphead->seq_num);
		
	return IPPROTO_TCP;
}
#endif

u_int16 check16();

/*
		b_check
	get the checksum over a block of data of possibly odd size
*/
u_int16 b_check(start, block, size)
register u_int16 start;
register void *block;
register u_int16 size;
{
	register u_int16 temp;
	u_int16 pad = 0;
	
	temp = check16(start, block, size / 2);

	/* is padding necessary? */
	if (size % 2) {  /* yes */
		/* get last byte */
		pad |= (*(u_int8 *)((u_int32)block + (size - 1))) << 8;
		temp = check16(temp, &pad, 1);
	}
	
	return temp;

}  /* end of b_check */

/*
		in_checksum
	Find an internet checksum.  Phead points to a UCP or TCP psuedo-header,
	if any.  Psuedo-headers are assumed to be 12 bytes in all cases.
*/
u_int16 in_checksum(phead, block, size)
register void *phead;
register void *block;
register u_int16 size;
{
	register u_int16 temp = 0;
	
	/* start with the psuedo-header, known to be even size of 12 bytes */
	if (phead) {
		temp = check16(0, phead, 6);
	}

	/* get regular data */
	return ~b_check(temp, block, size);

}  /* end of in_checksum */

/*
		check16
		Calculate the checksum of some 16 bit words
	Simple algorithm  - take 16 bit one's complement sum of all words (add +
	all carries).  Start is a start value to begin with (for doing checksums
	over different chunks).
		
	check16(start, array, nel)
	u_int16 start;
	u_int16 *array;
	int32 nel;
		
*/
#if defined(_UCC)
_asm("
check16: movem.l d2/a0,-(sp)  save registers
 movea.l d1,a0  pointer to array
 move.l 12(sp),d2 get counter
 subq.l #1,d2  prepare for loop
 andi.b #$f,ccr  clear x before starting!
loop move.w (a0)+,d1  get next word
 addx.w d1,d0  add in next word
 dbra d2,loop
 eor.w d1,d1 clear d1
 addx.w d1,d0 pick up last carry if any
 movem.l (sp)+,d2/a0  restore register
 rts  all done!
");
#else
#asm
check16: movem.l d2/a0,-(sp)  save registers
 movea.l d1,a0  pointer to array
 move.l 12(sp),d2 get counter
 subq.l #1,d2  prepare for loop
 andi.b #$f,ccr  clear x before starting!
loop move.w (a0)+,d1  get next word
 addx.w d1,d0  add in next word
 dbra d2,loop
 eor.w d1,d1 clear d1
 addx.w d1,d0 pick up last carry if any
 movem.l (sp)+,d2/a0  restore register
 rts  all done!
#endasm
#endif

/* end of check16 */


