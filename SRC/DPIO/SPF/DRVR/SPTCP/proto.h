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
 *  01 ??/??/?? Created
 *     02/09/97 Added microware copyright                              rds
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  43 05/30/97 Remove sock_set_lustat                                 rds
 *     05/30/97 <--------- SPF LAN Comm Pack v3.1.1 Release -------->
 *  45 05/31/97 Remove send_notify, m_copym, get_cproc                 rds
 *  50 06/23/97 Change add_tcp_entry parmeters to include so           rds
 *              rename find_tcp_entry to find_socket                   rds
 *              change do_drop parameters to just a socket pointer     rds
 *              remove close_wait, del_tcp_entry, find_entry_by_addr   rds
 *  52 08/24/97 added function pointers for calling spip functions     rds
 *  53 10/15/97 Replaced soshutdown with in_losing                     rds
 *  58 01/06/98 Added prototype for change_static                      rds
 *  59 02/04/98 Added prototypes for syscopy and wakeup                rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  70 05/17/99 sbdrop is now a local function and not in spip         rds
 *     06/24/99 Added definitions for mbuf_minalloc, in_localaddr,     rds
 *              sbwait, and sock_sleep. Added prototypes for cp_cksum,
 *              in_partial_cksum, and  tcp_package.
 *  75 07/27/00 Added prototype for soreceive                          rds
 *  77 01/22/01 Change size param to pointer on tcp_package so we can  mgh
 *                return values for non-blocking sockets
 ****************************************************************************/

/*
**	Device Driver Entry Point Function Prototypes
*/

/* entry.c */
extern void tcp_close_timeout(Timer_pb tpb);

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

extern char _bdata;
extern int *ip_defttl;
extern int *sb_max;
extern int *max_linkhdr;
extern int mbuf_minalloc;

void panic(char *msg);
void m_freem(struct mbuf *m);
void do_updata(struct socket *so);
int syscopy(void *, void *, size_t, u_int32, void *);
void wakeup(struct selinfo *);
void sbdrop(struct sockbuf *, int);
int cp_cksum(u_char *, u_char *, int);
int in_partial_cksum(struct bsd_mbuf *, int, int);
error_code tcp_package(struct socket *, u_char *, long *);

/*
** BSD Functions
*/
extern void *ipglobs;
extern struct ip_funcs ipfuncs;
extern void (*sbappend)();
extern int (*sbflush)();
extern int (*soabort)();
extern int (*socantsendmore)();
extern int (*socantrcvmore)();
extern int (*soclose)();
extern int (*sohasoutofband)();
extern int (*soisconnected)();
extern int (*soisconnecting)();
extern int (*soisdisconnecting)();
extern int (*soisdisconnected)();
extern int (*soqremque)();
extern int (*soreserve)();
extern void (*in_losing)();
extern int (*sowakeup)();
extern int (*sosend)();
extern struct socket *(*sonewconn1)();
extern int (*ip_output)();

extern int (*in_cksum)();
extern void (*rtalloc)();
extern int (*sbreserve)();
extern void (*ip_stripoptions)();
extern int (*sbwait)();
extern int (*in_localaddr)();
extern int (*sock_sleep)();
extern int (*soreceive)();
