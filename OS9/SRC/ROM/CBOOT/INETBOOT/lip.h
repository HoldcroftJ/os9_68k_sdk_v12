/*#include <sockmod.h>*/
#include <sockman.h>
#include <inet.h>
#include <ip.h>
/*#include <tcp_seq.h>*/
/*#include <tcp_states.h>*/
/*#include <tcp_timer.h>*/
/*#include <tcp.h>*/
#include <udp.h>
#include <icmp.h>
#ifndef ETHERTYPE_ARP
#include <arp.h>
#include <ether.h>
#endif
#define	BP_TIMESEC(x,y)	((x)*10+(y))	/* x=tens, y=units */
#ifndef ETHER_MIN
#define ETHER_MIN	(ETHERMIN + sizeof(struct ether_header))
#define ETHER_MAX	(ETHERMTU + sizeof(struct ether_header))
#endif
