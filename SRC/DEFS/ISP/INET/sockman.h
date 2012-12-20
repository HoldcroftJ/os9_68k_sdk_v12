/****************************************************************************
 *  $Workfile:   sockman.h  $
 *  $Revision:   1.18  $
 *  $Modtime:   23 Feb 1998 18:09:00  $
 *  $Date:   24 Feb 1998 13:41:10  $
 ****************************************************************************
!  Copyright 1988, 1989, 1990, 1993 by Microware Systems Corporation         !
!                          Reproduced Under License                          !
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
 * OS-9 History...
 *	  1	05/??/88	Birth												aln	*
 *	  2	08/03/93	Added so_ssig_pid/so_ssig_sig to struct socket		bat	*
 *      09/16/93	<***** ISP 2.0 Release ***********************>			*
   OS-9000 History...
!  001 88/05/15 Creation                                                ALN  !
!  002 89/12/15 Socket device and path descriptor definitions moved to       !
!               sockdesc.h                                              DJN  !
!  003 90/01/20 Completed modifications for OS-9000                     DJN  !
!  004 90/02/19 Added YLL's window update minimization changes          DJN  !
!  005 90/05/23 Changed sk_fm_hostname to array to allow setting of          !
!               host name.                                              DJN  !
!  006 90/07/20 --------------- ISP/OS9000 Release 1.1 ---------------- DJN  !
!  007 93/07/19 Added support for ss_sendsig/ss_release                 BAT  !
 Merge & Port Merge History ...
	8	931223	Merged again (9 & 9000).									gkm
			  >	SOCKMAXNAME & HOSTMAXNAME usage merged back to MAXNAME
			  > SOCKNAMESIZE deleted from 9000 (not used)
			  > for 9000 skm_fm_stat -> sockmanstat
			  > for 9000, in sockmanstat;	sk_fm_		-> v_
			  								if_fm_stat	-> Ifmanstat
			  								slotmr		-> slowtimer
			  								fsttmt		-> fasttimer
			  								tproc		-> timeout_procid
			  								tflag		-> timeout_flag
			  								devs		-> devices
       94/02/03 Add #define SOCKMAN 1 back                             ylee
                delete duplicate  EVNAMESIZE
                add v_pad for _OSK so it can be test on existing system
                moved sock_pvt here from sockdesc.
       94/02/07 condition MAXNAME for _OSK only. OS-9000 already defined it
       940209	define P_SIGNAL for OS9 & OS9000 to reduce 					gkm
				conditionalization in checking for eadly signals in
				accept.c, connect.c, read.c, write.c
			  > remove WINDOW_UPDATE definition for OS-9000. implemented
			  	better in OS9 using hiwat & lowat (per ylee)
			  > Added min macro (conditionalized)
       94/03/07 use HOSTMAXNAME to avoid conflict with MAXNAME in OS-9000  ylee
 *   9 94/06/14 Combined <sockdesc.h> & <sockmod.h> into <sockman.h>    bat *
 *  10 94/07/21 Combined (totally) OS9/OS9000 sock_pvt                  bat *
 *  11 95/01/08 Added PD_SOCKMAN_SIZE for EditMod descriptors           bat *
 *     96/01/17 ------------- ISP 2.1 Release ---------------------------   *
 *                  ---- OS-9000/Brutus Board Support V1.0 Released ----    *
 ****************************************************************************/

#ifndef _SOCKMAN_H
#define _SOCKMAN_H


#if defined(_OSK)
	#define SOCKMAN 1
	#include <sysio.h>
#endif

#include <INET/mbuf.h>				/* mbuf definitions. */
#include <INET/ifman.h>				/* IFMAN definitions. */
#include <INET/sysdefs.h>


/* Definition of socket module dispatch table. */
typedef struct smod_disp {			/* offsets to functions */
	int32			sm_attach;		/* attached to sockman */
	int32			sm_detach;		/* detach from sockman */
	int32			sm_status;		/* status request */
	int32			sm_socket;		/* generic request interface to sockman */
	int32			sm_control;		/* generic control request */
	int32			sm_input;		/* data coming in from lower modules */
	int32			sm_output;		/* data coming in from higher modules */	
} smod_disp, *Smod_disp;

#ifdef _OSK
/* Sockman socket module definition */
typedef struct sockmodule {
	struct modhcom	_mh;			/* a standard module header */
	u_int32			m_exec,			/* other fields in module header */
					m_excpt,
					m_data;
	smod_disp		sockmod_dispatch;
} SOCKMODULE, *Sockmodule;
#endif /* _OSK */

/* Module control block structure */
typedef struct mcb {
	struct scb		*mcb_scbnext;	/* list of scb's chained to this module */
	struct scb		*mcb_scbprev;	/* back pointer for scb list */
	struct sockmod	*mcb_smod;		/* pointer to socket module list entry */
	u_int32			mcb_size;		/* size of this structure */
} MCB, *Mcb;

/* Socket module list entry structure - this structure contains all the */
/* information sockman needs to use a particular socket module. */
typedef struct sockmod {
	struct sockmod	*sm_next;			/* next module on socket module list */
	struct sockmod	*sm_prev;			/* previous module on list */
	struct sockmod	*sm_subnext;		/* socket module sublist */
	struct sockmod	*sm_subprev;		/* reverse sublist */
	u_int16			sm_family;			/* socket family this module is in */
	u_int16			sm_type;			/* socket type this module supports */
	u_int16			sm_proto;			/* socket module protocol number */
	u_int16			sm_modtype;			/* socket module type */
	u_int32			sm_flags;			/* module flags */
	u_int32			sm_state;			/* module state */
	u_int32			sm_link;			/* link count */
	u_int32			sm_scbsize;			/* space required for scb */
	u_int32			sm_mcbsize;			/* space required for mcb */
	char			sm_sndevname[4]; 	/* two character tag for send event */
	char			sm_rcvevname[4]; 	/* two character tag for recv event */
	char			*sm_name;			/* ptr to socket module name string */
	MH_COM			sm_module;			/* pointer to the socket module */
	Mcb				sm_mcb;				/* mcb for this socket module */
	struct sockmanstat *sm_sockstat; 	/* pointer to sockman static */
	u_int32			sm_size;			/* size from srqmem */
	error_code		(*sm_attach)();		/* socket module attach routine */
	error_code		(*sm_detach)();		/* socket module detach routine */
	error_code		(*sm_status)();		/* status request */
	error_code		(*sm_socket)();		/* generic req. interface to sockman */
	error_code		(*sm_control)(); 	/* generic control request */
	error_code		(*sm_fasttime)();	/* fast timer entry point */
	error_code		(*sm_slowtime)();	/* slow timer entry point */
	error_code		(*sm_input)();		/* data in from lower socket modules */
	error_code		(*sm_output)();		/* data in from higher socket modules */
} SOCKMOD, *Sockmod;

/* socket module types */
#define SMTYPE_PROTOCOL		1
#define SMTYPE_SHARED		2
#define SMTYPE_FILTER		3

/*
 * socket module flag values
 * each module type may have different flag requirements
 * the lower 16 bits are module type dependent,
 * the upper 16 bits are not.
 */
/*	these are the protocol driver specific flags */
#define	SPF_ATOMIC			0x0001	/* exchange atomic messages only */
#define	SPF_ADDR			0x0002	/* addresses given with messages */
#define	SPF_CONNREQUIRED	0x0004	/* connection required by protocol */
#define	SPF_WANTRCVD		0x0008	/* want PRS_RCVD calls */
#define	SPF_RIGHTS			0x0010	/* passes capabilities */

/* socket module state flags */
/* Sockman will set the SMS_ATTACHED bit when the socket entry is attached,
 * having called the socket module's attach function.  This is useful if someone
 * tries to attach a module while it is already being attached, as in the
 * case of multiple module list entries working off the same module.  Sockman
 * will gladly call the module's attach function, but if that function wants
 * to access another entry for the module, it better wait until that entry is
 * attached (and not in the process of detaching!).
 * Sockman will clear the SMS_ATTACHED bit and set the SMS_DETACHING bit when
 * it begins to detach a module list entry.  The fact that the module is no
 * longer in the list implies that it is all done being detached.
 */
#define SMS_ATTACHED	0x00000001	/* module is attached */
#define SMS_DETACHING	0x00000002	/* module is detaching */

/*
 * parameter block structure for socket and control functions
 */
typedef struct {
	u_int32		pb_fcode;		/* the function code */
	u_int32		pb_status;		/* status 0 = ok, n = error code */
	union {
		struct scb *_scb;		/* pointer to socket scb or socket module */
		struct sockmod *_smod;	/* entry of the called module */
	} pb_cb;
	union {
		void *ptr;			/* parameters to called function */
		int32 val;
	} pb_parm[8];
	u_int32		pb_size;	/* size if it was srqmem'ed */
} PBLOCK, *Pblock;
#define pb_scb	pb_cb._scb
#define pb_smod	pb_cb._smod

/*
 * Function code requests for socket() interface function
 * The function code is passed in the parameter block appropriate for
 * the function.  The header for all parameter blocks is the same,
 * with a remaining section for parameters.
 * The socket function should return 0 on success and an appropriate
 * error code on failure.  This value should also be in the status
 * location in the parameter block.
 */
#define	PRS_ATTACH		1	/* attach protocol to socket */
#define	PRS_DETACH		2	/* detach protocol from socket */
#define	PRS_BIND		3	/* bind socket name */
#define	PRS_LISTEN		4	/* mark socket as passive */
#define	PRS_CONNECT		5	/* initiate a connection */
#define	PRS_ACCEPT		6	/* accept a connection */
#define	PRS_DISCONNECT	7	/* disconnect from peer */
#define	PRS_SHUTDOWN	8	/* shutdown this side of the connection */
#define PRS_RCVD		9	/* data has been read from socket */
#define	PRS_SEND		10	/* write data on socket */
#define	PRS_ABORT		11	/* abort connection */
#define	PRS_CONTROL		12	/* control operations on protocol */
#define	PRS_SENSE		13	/* return status */
#define	PRS_RCVOOB		14	/* retrieve out of band data */
#define	PRS_SENDOOB		15	/* send out of band data */
#define	PRS_NAME		16	/* get local connection name */
#define	PRS_PEER		17	/* get peer name */
#define	PRS_CONNECT2	18	/* connect two sockets */
/* begin for protocols internal use */
#define	PRS_FASTTIMO	19	/* 200ms timeout */
#define	PRS_SLOWTIMO	20	/* 500ms timeout */
#define	PRS_PROTORCV	21	/* receive from below */
#define	PRS_PROTOSEND	22	/* send to below */
#define PRS_CMP			23	/* compare two names */

#define	PRS_NREQ		23
#define PRS_UNREACHABLE 24  /* ICMP DESTUNREACHABLE support */

/*
		Definitions for control interface functions
*/
#define	PRC_IFDOWN		0	/* interface transition */
#define PRC_INSTALL		1	/* install on protocol search list */
#define PRC_REMOVE		2	/* remove from protocol search list */
#define PRC_INUSE		3	/* find out if protocol is in use already */
#define PRC_ICMP		4	/* an ICMP message */
#define PRC_ROUTE		5	/* a routing request */
#define PRC_XMITDONE	6	/* tell sockman a transmission is finished */
#define PRC_NEWCONN		7	/* request a new socket from sockman */
#define PRC_TIMER		8	/* tell sockman timer entry points are valid */

#define PRC_GETMOD		20	/* ask sockman for a pointer to a socket module */
#define PRC_LINKMOD		21	/* link a socket module */
#define PRC_UNLINKMOD	22	/* unlink module */
#define PRC_GETLINKMOD	23	/* link it after getting it */

#define	PRC_ROUTEDEAD			101	/* select new route if possible */
#define	PRC_QUENCH				104	/* someone said to slow down */
#define	PRC_MSGSIZE				105	/* message size forced drop */
#define	PRC_HOSTDEAD			106	/* normally from IMP */
#define	PRC_HOSTUNREACH			107	/* ditto */
#define	PRC_UNREACH_NET			108	/* no route to network */
#define	PRC_UNREACH_HOST		109	/* no route to host */
#define	PRC_UNREACH_PROTOCOL	110	/* dst says bad protocol */
#define	PRC_UNREACH_PORT		111	/* bad port # */
#define	PRC_UNREACH_NEEDFRAG	112	/* IP_DF caused drop */
#define	PRC_UNREACH_SRCFAIL		113	/* source route failed */
#define	PRC_REDIRECT_NET		114	/* net routing redirect */
#define	PRC_REDIRECT_HOST		115	/* host routing redirect */
#define	PRC_REDIRECT_TOSNET		116	/* redirect for type of service & net */
#define	PRC_REDIRECT_TOSHOST	117	/* redirect for tos & host */
#define	PRC_TIMXCEED_INTRANS	118	/* packet lifetime expired in transit */
#define	PRC_TIMXCEED_REASS		119	/* lifetime expired on reass q */
#define	PRC_PARAMPROB			120	/* header incorrect */

#define	PRC_NCMDS		21

/* Socket device descriptor definition */
typedef struct socketdd {
	DD_COM			sock_dds;			/* standard device descriptor */
	u_char			sock_opt_stp;		/* enp or sockman socket type flag */
	u_char			sock_opt_pad;
	u_int16			sock_opt_mbpad;		/* padding for mbufs */
	/* end of options section of descriptor */
	u_int32			sock_inetaddr;		/* primary internet address of host */
	u_int32			sock_stack;			/* size of socket stack */
	u_int32			sock_recvlimit;		/* maximum # of mbufs on recv queue */
	u_int32			sock_sendlimit;		/* maximum # of mbufs on send queue */
	u_int32			sock_tpriority;		/* timeout process priority */
	#if defined(_OSK)
		u_int16		sock_hostname;		/* offset to host name */
		u_int16		sock_devname;		/* offset to list of initial devices */
		u_int16		sock_smod;			/* offset to list of socket modules */
	#elif defined(_OS9000)
		u_int32		sock_hostname;		/* offset to host name */
		u_int32		sock_devname;		/* offset to list of initial devices */
		u_int32		sock_smod;			/* offset to list of socket modules */
	#endif
} SOCKETDD, *Socketdd;

/* Socket Module entries
 * The smod table is contained in the socket descriptor (socketdd).
 * sock_smod in socketdd contains an offset to a table of smod entries.  */
typedef struct smod {
	u_int16			smod_family;		/* address family */
	u_int16			smod_type;			/* socket type */
	u_int16			smod_protocol;		/* protocol supported */
	u_int16			smod_modtype;		/* socket module type */
	#if defined(_OSK)
		u_int16		smod_nameoff;		/* offset to protocol name */
	#elif defined(_OS9000)
		u_int32		smod_nameoff;		/* offset to protocol name */
	#endif
} SMOD, *Smod;

/* socket module types */
#define SMTYPE_PROTOCOL		1
#define SMTYPE_SHARED		2
#define SMTYPE_FILTER		3

/* Options section of the path descriptor. */
struct sock_opt {
	u_char			pd_dtp;				/* device type */
	u_char			pd_noblock;			/* 0 = blocking, 1 = non-blocking io */
	u_char			pd_stp;				/* socket type 0 = enp10, 1 = sockman */
	u_char			pd_res;				/* reserved for word aligmment */
	u_int16			pd_mbpad;			/* extra space to allocate in mbufs */
	char			pd_opts[90];		/* padding/scratch area */
	u_char			pd_device[32];		/* device name */
};

/* Path descriptor definitions for OS9 */
#ifdef _OSK
	#define sockpvt	path.fm_pvt.sock
	#define sockopt	path.fm_opt.sock
#endif

/* Miscellaneous manifest constants */

#ifndef MAXNAME
	#define MAXNAME		32			/* maximum name size of sockets/hosts */
#endif
#define HOSTMAXNAME		32			/* maximum name size of sockets/hosts */
#define MAXDEVS			8			/* max # of if devices in socket desc. */
#define QUEUELIMIT		5			/* max pndng cncts on passive socket */

#define EVNMASK		"S%2s%08x"
#define EVNAMESIZE	16

/*
**	sockman static storage definitions
**		The file manager requires some memory to keep socket-global
**		information.  This storage is attached to a "socket" device
**		driver which is never called.
*/
typedef struct sockmanstat {
	FM_STATIC		v_sysio;			/* kernel static storage variables */
	#ifdef _OSK
		int16		v_pad;				/* padding following sysio */
		u_int32		v_dummy;			/* was v_curproc not used now... */
	#endif								/* ... remove after validation etc. */
	u_int32			v_flags;			/* various flags (see below) */
	DEV_LIST		v_sockdtb;			/* socket device table ptr */
	#if defined(_OSK)
		int32		v_errno;			/* global errno value */
		u_int32		v_sysglob;			/* system global pointer */
	#endif
	Ifmanstat		v_ifmanstat;		/* pointer to ifman's static storage */
	#ifdef _OSK
		u_int32		v_dummy2;			/* was v_inetaddr not used now... */
	#endif								/* ... remove after validation etc. */
	char			v_hostname[HOSTMAXNAME];/* pointer to name of host */
	u_int32			(*v_socket)();		/* socket function */
	u_int32			(*v_control)();		/* control function */
	u_int32			v_slowtimer;		/* the slow timeout timer */
	u_int32			v_fasttimer;		/* the fast timeout timer */
	PR_DESC			v_timeout_procid;	/* timeout process descriptor ptr */
	u_int32			v_timeout_flag;		/* flag to communicate with timeout */
	int32			v_devices[MAXDEVS]; /* path numbers for if devices */
	Sockmod			v_smnext;			/* list of attached sock mod entries */
	Sockmod			v_smprev;			/* reverse link to socket mod entries */
	u_int16			v_mbpad;			/* extra space in mbufs for fizkl hdr */
	u_int32			v_atcnt;			/* Count of times attached. */
	u_int32			v_recvlimit; 		/* max # of mbufs on recv queue */
	u_int32			v_sendlimit;		/* max # of mbufs on send queue */
} sockmanstat, *Sockmanstat;

/* socket manager flags*/
/*
** The initializing flags are valuable since sockman auto initializes
** on opening the socket device.  If an open comes in while sockman is
** initializing, then it will wait until the initialization completes,
** and return.
*/
#define SOCK_INITING	0x00000001	/* device is initializing */
#define SOCK_INITED		0x00000002	/* device has been initialized */

/*
 *	socket manager PD definitions
 */
typedef struct sockbuf {
	u_int32			sb_cc;		/* characters available in buffers */
	u_int32			sb_hiwat;	/* maximum character count */
	u_int32			sb_mbcnt;	/* memory in use by mbufs */
	u_int32			sb_mbmax;	/* maximum allowable mbuf usage */
	u_int32			sb_lowat;	/* low water mark */
	u_int16			sb_frpad;	/* padding to allocate in front of mbuf */
	u_int16			sb_tlpad;	/* padding to allocate in tail of mbuf */
	Mbuf			sb_mb;		/* mbuf packet queue */
	u_int16			sb_timeout;
	u_int16			sb_flags;
	u_int32			sb_evid;				/* sockbuf event id */
	char			sb_evname[EVNAMESIZE];  /* sockbuf event name */
} SOCKBUF, *Sockbuf;

#define SB_MAX		65535  /* maximum bytes allocated to a sockbuf */

typedef struct scb {
	struct scb*		scb_snd;		/* next scb on list (for module stacking) */
	struct scb*		scb_rcv;		/* previous scb on list */
	Sockmod			scb_smod;		/* socket module entry at this level */
	struct socket*	scb_socket; /* back pointer to associated socket */
	struct scb*		scb_chainnext;	/* chain of scb's */
	struct scb*		scb_chainprev;	/* back pointer to chain */
	u_int16			scb_state;		/* state info regarding scb */
	u_int16			scb_pad;		/* pad to 32 bits */
	u_int32			scb_size;		/* size of this scb */
} SCB, *Scb;

/* scb state flags */
#define SCB_ATTACHED	0x0001	/* socket module is attached to scb */

typedef struct socket {
	PD_SKM			so_pd;			/* pointer to related path descriptor */
	Sockmanstat		so_sockstat;	/* pointer to sockman static storage */
	u_int16			so_type;		/* socket type */
	u_int16			so_protocol;	/* socket protocol */
	u_int16			so_linger;		/* time to linger while closing */
	u_int16 		so_options;		/* user-settable socket options */
	u_int16			pad;
	u_int16 		so_state;		/* socket state */
	Scb				so_scb;			/* socket control block list */
	SOCKBUF 		so_snd;			/* send buffers for this socket */
	SOCKBUF 		so_rcv;			/* receive buffers for this socket */
	struct socket*	so_head;		/* head of socket connection queue */
	struct socket*	so_q;		/* queue of complete incoming connections */
	struct socket*	so_q0;		/* queue of incomplete incoming connections */	
	u_int16			so_qlen;		/* number of connections on q */
	u_int16			so_q0len;		/* connections pending on q0 */
	u_int16			so_qlimit;		/* connection queue limit */
	u_int16			so_timeout;		/* connection timeout */
	u_int16			so_error;		/* error */
	u_int16			so_oobmark;		/* chars to out of band data */
	u_int32			so_size;		/* size of this structure from srqmem */
	process_id		so_ssig_pid;	/* process signaled on data ready */
	signal_code		so_ssig_sig;	/* signal sent on data ready */
} SOCKET, *Socket;

struct sock_pvt {
	event_id		pd_rdyevent;	/* event handle for data ready */
	#if defined(_OSK)
		u_int8		old_pad[8];		/* padding for OS-9 compatability */
	#endif
	struct socket*	pd_socket;		/* socket structure for this path */
};

/* Path descriptor definition for OS9000 */
#ifdef _OS9000
	typedef struct pd_skm {
		pd_com			pd_io;				/* IOMAN common section */
		struct sock_pvt	sockpvt;			/* SOCKMAN private section */
		struct sock_opt	sockopt;			/* SOCKMAN options section */
	} pd_skm, *Pd_skm;
	#define PD_SOCKMAN_SIZE 256
#endif

/* so_state values */
#define	SS_ISCONNECTING		0x0001	/* in process of connecting to peer */
#define	SS_ISCONNECTED		0x0002	/* socket connected to a peer */
#define	SS_ISDISCONNECTING	0x0004	/* in process of disconnecting */
#define	SS_CANTSENDMORE		0x0008	/* can't send more data to peer */
#define	SS_CANTRCVMORE		0x0010	/* can't receive more data from peer */
#define	SS_RCVATMARK		0x0020	/* at mark on input */
#define	SS_NBIO				0x0040	/* non-blocking ops, mirrors pd_noblock */
#define SS_CLOSING			0x0080	/* socket is closing */
#define	SS_PRIV				0x0100	/* privileged for broadcast, raw... */

#define	SS_NOFDREF			0x1000	/* no path descriptor attached */
#define SS_CHAIN			0x2000	/* scb has been chained */

#if 0
/*
 * no support for these unix socket states yet, maybe never?
 * SS_ASYNC is in the pd (pd_rdyevent or pd_pid)
 */
#define	SS_ASYNC		0x0200	/* async i/o notify */
#endif

/*
 *	Useful definitions for the timeout process
 */
#define TIMEOUT_PERIOD	(0x80000000 | 26)  /* 10 per second */
#define SLOW_PERIOD		5
#define FAST_PERIOD		2


#endif /* _SOCKMAN_H */
