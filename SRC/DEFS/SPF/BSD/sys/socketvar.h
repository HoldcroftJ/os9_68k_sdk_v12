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
 *  01 10/07/96 Creation
 *  02 10/21/96 Added setsockopt_args structure.                       tmd
 *  03 01/03/97 Added getsockopt_args structure.                       tmd
 *     01/31/97 Converted to DPIO lock functions                       rds
 *  04 02/04/97 Added sb_flow to socket structure                      rds
 *  05 02/15/97 change INIT_9LOCK and TERM_9LOCK to nothing            rds
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  06 05/30/97 Change so_error from u_short to u_long                 rds
 *     05/30/97 <--------- SPF LAN Comm Pack v3.1.1 Release -------->
 *  07 05/31/97 Add so_sel, remove sb_cc_wake from socket structure    rds
 *  08 06/04/97 Call so_upcall from sorwakeup                          rds
 *  09 06/30/97 Added new copyright header                             apw
 *  10 08/21/97 Add per path variables to socket structure             rds
 *  11 01/06/98 Add missing ; to soreserved in socket structure        rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  12 02/12/01 Added reserved fields to socket buffers                rds
 *  13 02/22/01 Uncommented INIT_9LOCK and TERM_9LOCK                  rds
 ****************************************************************************/
/*-
 * Copyright (c) 1982, 1986, 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)socketvar.h	8.1 (Berkeley) 6/2/93
 */

#ifndef _SYS_SOCKETVAR_H
#define  _SYS_SOCKETVAR_H

#include <sys/select.h>			/* for struct selinfo */
#if (defined(_OSK)||defined(_OS9000))
struct devtab;
struct sock_funcs;
#include <lock.h>
#endif

/*
 * Kernel structure per socket.
 * Contains send and receive buffer queues,
 * handle on protocol and pointer to protocol
 * private data and error information.
 */
struct socket {
	short	so_type;		/* generic type, see socket.h */
	short	so_options;		/* from socket call, see socket.h */
	short	so_linger;		/* time to linger while closing */
	short	so_state;		/* internal state flags SS_*, below */
	caddr_t	so_pcb;			/* protocol control block */
	struct	protosw *so_proto;	/* protocol handle */
/*
 * Variables for connection queueing.
 * Socket where accepts occur is so_head in all subsidiary sockets.
 * If so_head is 0, socket is not related to an accept.
 * For head socket so_q0 queues partially completed connections,
 * while so_q is a queue of connections ready to be accepted.
 * If a connection is aborted and it has so_head set, then
 * it has to be pulled out of either so_q0 or so_q.
 * We allow connections to queue up based on current queue lengths
 * and limit on number of queued connections for this socket.
 */
	struct	socket *so_head;	/* back pointer to accept socket */
	struct	socket *so_q0;		/* queue of partial connections */
	struct	socket *so_q;		/* queue of incoming connections */
	short	so_q0len;		/* partials on so_q0 */
	short	so_qlen;		/* number of connections on so_q */
	short	so_qlimit;		/* max number queued connections */
	short	so_timeo;		/* connection timeout */
	u_long	so_error;		/* error affecting connection */
	/*pid_t	so_pgid;*/		/* pgid for signals */
	u_long	so_oobmark;		/* chars to oob mark */
/*
 * Variables for socket buffering.
 */
	struct	sockbuf {
		u_long	sb_cc;		/* actual chars in buffer */
		u_long	sb_hiwat;	/* max actual char count */
		u_long	sb_mbcnt;	/* chars of mbufs used */
		u_long	sb_mbmax;	/* max chars of mbufs to use */
		long	sb_lowat;	/* low water mark */
		struct	bsd_mbuf *sb_mb;	/* the mbuf chain */
		struct	selinfo sb_sel;	/* process selecting read/write */
		short	sb_flags;	/* flags, see below */
		short	sb_timeo;	/* timeout for read/write */
		Lk_desc	sb_lock;	/* os9 lock descriptor */
		short	sb_flow;	/* SPF flow control flag */
		short	res1;		/* Reserved */
		u_long	res2[2];	/* Reserved - 4 bytes will be a tail pointer */
#define SB_FLOWON	0x01	/* flow control enabled */
	} so_rcv, so_snd;
#define	SB_MAX		(256*1024)	/* default for max chars in sockbuf */
#define	SB_LOCK		0x01		/* lock on data queue */
#define	SB_WANT		0x02		/* someone is waiting to lock */
#define	SB_WAIT		0x04		/* someone is waiting for data/space */
#define	SB_SEL		0x08		/* someone is selecting */
#define	SB_ASYNC	0x10		/* ASYNC I/O, need signals */
#define	SB_NOTIFY	(SB_WAIT|SB_SEL|SB_ASYNC)
#define	SB_NOINTR	0x40		/* operations not interruptible */

	caddr_t	so_tpcb;			/* Wisc. protocol control block XXX */
	void	(*so_upcall) __P((struct socket *so));
	u_int32				so_hoc;			/* hole_on_close protocol */
	void				*os9_pp;		/* additional OS9 per-path storage */
	struct selinfo		so_sel;			/* process waiting for signal */
	struct spf_pdstat	*so_pd;			/* SPF path descriptor */
	struct sockaddr_in	*so_local;		/* points to ournum addr[] */
	struct sockaddr_in	*so_remote;		/* points to theirnum addr[] */
	u_int32				so_reserved[2];	/* Reserved space */
};

#if (defined(_OSK)||defined(_OS9000))
/* taken from BSD's uipc_syscalls.c */
struct setsockopt_args {
    int s;
    int level;
    int name;
    caddr_t val;
    int valsize;
};

struct getsockopt_args {
    int s;
    int level;
    int name;
    caddr_t val;
    int *avalsize;
};
#endif /* _OSK||_OS9000 */

/*
 * Socket state bits.
 */
#define	SS_NOFDREF		0x001	/* no file table ref any more */
#define	SS_ISCONNECTED		0x002	/* socket connected to a peer */
#define	SS_ISCONNECTING		0x004	/* in process of connecting to peer */
#define	SS_ISDISCONNECTING	0x008	/* in process of disconnecting */
#define	SS_CANTSENDMORE		0x010	/* can't send more data to peer */
#define	SS_CANTRCVMORE		0x020	/* can't receive more data from peer */
#define	SS_RCVATMARK		0x040	/* at mark on input */

#define	SS_PRIV			0x080	/* privileged for broadcast, raw... */
#define	SS_NBIO			0x100	/* non-blocking ops */
#define	SS_ASYNC		0x200	/* async i/o notify */
#define	SS_ISCONFIRMING		0x400	/* deciding to accept connection req */
#define SS_CLOSING_SOCKET 0x800   /* closing the socket */

/*
 * Macros for sockets and socket buffering.
 */

/*
 * How much space is there in a socket buffer (so->so_snd or so->so_rcv)?
 * This is problematical if the fields are unsigned, as the space might
 * still be negative (cc > hiwat or mbcnt > mbmax).  Should detect
 * overflow and return 0.  Should use "lmin" but it doesn't exist now.
 */
#define	sbspace(sb) \
    ((long) imin((int)((sb)->sb_hiwat - (sb)->sb_cc), \
	 (int)((sb)->sb_mbmax - (sb)->sb_mbcnt)))

/* do we have to send all at once on a socket? */
#define	sosendallatonce(so) \
    ((so)->so_proto->pr_flags & PR_ATOMIC)

/* can we read something from so? */
#define	soreadable(so) \
    ((so)->so_rcv.sb_cc >= (so)->so_rcv.sb_lowat || \
	((so)->so_state & SS_CANTRCVMORE) || \
	(so)->so_qlen || (so)->so_error)

/* can we write something to so? */
#define	sowriteable(so) \
    (sbspace(&(so)->so_snd) >= (so)->so_snd.sb_lowat && \
	(((so)->so_state&SS_ISCONNECTED) || \
	  ((so)->so_proto->pr_flags&PR_CONNREQUIRED)==0) || \
     ((so)->so_state & SS_CANTSENDMORE) || \
     (so)->so_error)

/* adjust counters in sb reflecting allocation of m */
#define	sballoc(sb, m) { \
	(sb)->sb_cc += (m)->mb_len; \
	(sb)->sb_mbcnt += (m)->mb_alloc; \
}

/* adjust counters in sb reflecting freeing of m */
#define	sbfree(sb, m) { \
	(sb)->sb_cc -= (m)->mb_len; \
	(sb)->sb_mbcnt -= (m)->mb_alloc; \
}

/*
** Initialize and terminate os9 lock descriptors.
*/
#define INIT_9LOCK(sb) (_os_crlk((Lk_desc *)&(sb.sb_lock)))
#define TERM_9LOCK(sb) (_os_dellk((Lk_desc)(sb.sb_lock)))

#define sorwakeup(so) { \
			sowakeup((so), &(so)->so_rcv); \
			if ((so)->so_upcall) \
				(*((so)->so_upcall))((so)); \
		}

#define sorwakeup2(globs,so) { \
			sowakeup((so), &(so)->so_rcv); \
			if ((so)->so_upcall){ \
				void *old_static = (void *)change_static(globs); \
				(*((so)->so_upcall))((so)); \
				change_static(old_static); \
			} \
		}

#define	sowwakeup(so) sowakeup((so), &(so)->so_snd)

#ifdef SPF_DRVR

#ifdef SPIP
extern u_long	sb_max;
#endif /* SPIP */

/*
** None of the following definitions are used for OS9.
*/
#if 0
/* to catch callers missing new second argument to sonewconn: */
/*#define	sonewconn(head, connstatus)	sonewconn1((head), (connstatus))*/
/*extern struct	socket *sonewconn1 __P((struct socket *head, int connstatus));*/

/* strings for sleep message: */
extern	char netio[], netcon[], netcls[];

/*
 * File operations on sockets.
 */
extern int	soo_read __P((struct file *fp, struct uio *uio, struct ucred *cred));
extern int	soo_write __P((struct file *fp, struct uio *uio, struct ucred *cred));
extern int	soo_ioctl __P((struct file *fp, int com, caddr_t data, struct proc *p));
extern int	soo_select __P((struct file *fp, int which, struct proc *p));
extern int 	soo_close __P((struct file *fp, struct proc *p));
#endif /* 0 */

#endif /* SPF_DRVR */

#endif /*_SYS_SOCKETVAR_H*/
