/****************************************************************************
 *  $Workfile:   sockmod.h  $
 *  $Revision:   1.7  $
 *  $Modtime:   10 Oct 1993 18:44:00  $
 *  $Date:   10 Oct 1993 18:46:14  $
 ****************************************************************************
 *	Copyright 1988 by Microware Systems Corporation							*
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
 *	  1 11/17/88	Birth												aln	*
 *      09/16/93	<***** ISP 2.0 Release ***********************>			*
 ****************************************************************************/

#ifndef _SOCKMOD_H
#define _SOCKMOD_H

/*
		Sockman socket module definition
*/
typedef struct sockmodule {
	struct modhcom	_mh;		/* a standard module header */
	u_int32			m_exec,		/* other fields in sbrtn module header */
					m_excpt,
					m_data;
	struct smod_disp {				/* offsets to functions */
		int32		sm_attach,		/* attached to sockman */
					sm_detach,		/* detach from sockman */
					sm_status,		/* status request */
					sm_socket,		/* generic request interface to sockman */
					sm_control,		/* generic control request */
					sm_input,		/* data coming in from lower modules */
					sm_output;		/* data coming in from higher modules */	
	} sockmod_dispatch;
} SOCKMODULE, *Sockmodule;

/*
	Module control block
*/
typedef struct mcb {
	struct scb	*mcb_scbnext;	/* list of scb's chained below this module */
	struct scb	*mcb_scbprev;	/* back pointer for scb list */
	struct sockmod	*mcb_smod;	/* back pointer to socket module list entry */
	u_int32		mcb_size;		/* size of this structure */
} MCB, *Mcb;

/*
		Socket module entry on socket module list
		
		This structure contains all the information sockman needs to use a
	particular socket module
	
*/

typedef struct sockmod {
	struct sockmod	*sm_next;	/* next module on socket module list */
	struct sockmod	*sm_prev;	/* previous module on list */
	struct sockmod	*sm_subnext;	/* socket module sublist */
	struct sockmod	*sm_subprev;	/* reverse sublist */
	u_int16		sm_family;		/* socket family this module is in */
	u_int16		sm_type;		/* socket type this module supports */
	u_int16		sm_proto;		/* socket module protocol number */
	u_int16		sm_modtype;		/* socket module type */
	u_int32		sm_flags;		/* module flags */
	u_int32		sm_state;		/* module state */
	u_int32		sm_link;		/* link count */
	u_int32		sm_scbsize;		/* space required for scb */
	u_int32		sm_mcbsize;		/* space required for mcb */
	char		sm_sndevname[4];	/* two character tag for send event */
	char		sm_rcvevname[4];	/* two character tag for recv event */
	char		*sm_name;		/* pointer to socket module name string */
	Sockmodule	sm_module;		/* pointer to the socket module */
	Mcb			sm_mcb;			/* mcb for this socket module */
	struct sockmanstat *sm_sockstat;	/* pointer to sockman static */
	u_int32		sm_size;		/* size from srqmem */
	int32		(*sm_attach)(),		/* socket module attach routine */
				(*sm_detach)(),		/* socket module detach routine */
				(*sm_status)(),		/* status request */
				(*sm_socket)(),		/* generic request interface to sockman */
				(*sm_control)(),	/* generic control request */
				(*sm_fasttime)(),	/* fast timer entry point */
				(*sm_slowtime)(),	/* slow timer entry point */
				(*sm_input)(),		/* data coming in from lower socket modules */
				(*sm_output)();		/* data coming in from higher socket modules */
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


#endif	/* _SOCKMOD_H */
