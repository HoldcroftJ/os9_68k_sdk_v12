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
 *  01 06/29/97 Created                                                rds
 *  07 02/04/98 Added prototypes for syscopy and change_static         rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  09 09/30/98 Added prototype for m_dup                              rds
 *  12 07/27/00 Added prototype for soreceive                          rds
 ****************************************************************************/

/*
**	Device Driver Entry Point Function Prototypes
*/

extern error_code dr_iniz(Dev_list deventry);
extern error_code dr_term(Dev_list deventry);
extern error_code dr_getstat(Dev_list deventry, Spf_ss_pb pb);
extern error_code dr_setstat(Dev_list deventry, Spf_ss_pb pb);
extern error_code dr_downdata(Dev_list deventry, Mbuf mb);
extern error_code dr_updata(Dev_list deventry, Mbuf mb);

#if defined(_OS9000)
#include <regs.h>
void *change_static(void *);
#endif

extern int *ip_defttl;
extern int udpcksum;
extern struct inpcb udb;
extern struct udpstat udpstat;
extern char _bdata;

void panic(char *msg);
void m_freem(struct mbuf *m);
void do_updata(struct socket *so);
void bld_mbuf(struct bsd_mbuf *, int, caddr_t);
int syscopy(void *, void *, size_t, u_int32, void *);
struct bsd_mbuf *m_dup(struct bsd_mbuf *);

/*
** BSD Socket Functions
*/
extern void *ipglobs;
extern struct ip_funcs ipfuncs;

extern int (*soisconnected)();
extern int (*sosend)();
extern int (*sowakeup)();
extern int (*soreserve)();
extern int (*soisdisconnected)();
extern int (*socantsendmore)();
extern int (*sbappendaddr)();
extern int (*ip_output)();
extern void (*ip_stripoptions)();
extern int (*in_cksum)();
extern int (*in_pcballoc)();
extern int (*in_pcbbind)();
extern int (*in_pcbconnect)();
extern void (*in_pcbdisconnect)();
extern void (*in_pcbdetach)();
extern struct inpcb *(*in_pcblookup)();
extern int (*in_broadcast)();
extern void (*in_pcbnotify)();
extern void (*icmp_error)();
extern int (*soreceive)();
