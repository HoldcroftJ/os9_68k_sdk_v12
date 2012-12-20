/*
 * parameters and defines for inetboot modules
 */

#define NET_BUFSIZE		2048

extern void *(*net_in)();		/* pointer to link-level handler */
extern char *ethernet_in();		/* the only handler so far... */

extern char *inetname(), *showether();
extern int arp_dest_ip;
extern char arp_dest_hwaddr[16];
extern char mbuffer[];
extern int mlen;
extern char net_inpbuf[NET_BUFSIZE];
extern char net_outbuf[NET_BUFSIZE];
extern struct bootp bprp;
extern unsigned short in_bootsize;

/*
 * the bpt_ timer code is responsible for the next two items.
 * bpt_rseed should be initialized to something random.
 * bpt_curtick should increase by one for each elasped 100ms.
 */
extern					int bpt_rseed;		/* RNG seed for tbackoff */
extern /*volatile*/ u_int32 bpt_curtick;	/* 100ms tick from timer irq */

/*
 * Control variable for printing messages..Higher numbers include lower too
 *      0 = Banner printing only
 *		1 = Basic messages (Normal Mode)
 *		2 = Detail/progress messages
 *		3 = Bad packet messages
 *		4 = IP packet info
 */
extern				u_int16  msgprint;		/* printing flag */
