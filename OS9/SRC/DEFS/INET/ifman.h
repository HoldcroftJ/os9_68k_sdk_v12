/****************************************************************************
 *	ifman.h																	*
 ****************************************************************************
 *	Copyright 1993 by Microware Systems Corporation							*
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
 *	  1 ??/??/??	Birth											    ???	*
 *      09/16/93	<***** ISP 2.0 Release ***********************>			*
 ****************************************************************************/

#ifndef _IFMAN_H
#define _IFMAN_H


#include <sysio.h>

#define IFMAN 1
#define ifmpvt	path.fm_pvt.ifm
#define ifmopt	path.fm_opt.ifm

/*
	Layout of if device driver module
*/
struct ifdvrmod {
	struct modhcom _mh;		/* standard module header */
	u_int	_mexec;			/* offset to dispatch works */
	u_int	_mexcpt;		/* offset to exception handler */
	u_int	_mdata;			/* static storage requirement */
};

/*
	if driver dispatch table
*/
struct ifdvrdsp {
							/* standard IOMAN dispatch */
	int16	init;
	int16	read;
	int16	write;
	int16	getstat;
	int16	putstat;
	int16	term;
	int16	trap;
							/* ifman dispatch */
	int32	if_init;
	int32	if_term;
	int32	if_send;
	int32	if_xmit;
	int32	if_recv;
	int32	if_stat;
};

/*
	structure used for keeping track of the upper level protocols
	to pass data to.
*/
typedef struct ulp_list {
	int32	ulp_ptype;  /* the type of packet this guy is after */
	struct ulp_list	*ulp_next;
	int32	(*ulp_recv_func)();  /* pointer to the function to call */
	void	*ulp_recv_stat;  /* the first parameter, presumably some static storage */
	union pathdesc *ulp_pd;  /* the path descriptor of the guy who made this entry */
	int32	ulp_size;	/* the size of this piece of memory from srqmem */
} *Ulp_list;
#define PTYPE_ANY 0  /* wildcard packet type */
	
/*
	ifman static storage definitions
		Ifman keeps its required goodies in the static storage of
		the loopback driver (lo0) which must attached before anything
		else.
*/
typedef struct ifmanstat {
	sysioStatic	v_sysio;			/* loop device static for kernel */
	u_int16		v_padder;		/* alignment for v_sysio (0x2e) */
	struct ifdev
				*v_iflist,			/* list of known interfaces */
				*v_ifback;			/*  "   "   "     " backwards */
	Ulp_list	v_ulp_head;
	int32		(*v_recvfunc)();	/* routine to handle incoming packets */
	int32		(*v_arp_input)();	/* routine to handle incoming ARP packets */
	int32		(*v_arp_resolve)(); /* routine to map IP to EN address */
	int32		(*v_arp_whohas)(); 	/* routine to send ARP request */
	void		*v_arp_stat;		/* static storage for arp functions */
	struct devicetbl	*v_loop_devtbl;		/* loop driver device table entry */
	u_int32		v_nonloop;			/* count of if devices other than lo0 */
	procid		*v_netin_procid;	/* process id for the netin process */
	int32		v_netin_event;		/* network input ready event */
	u_int8		*v_netin_flag;		/* netin process flag */
	char		v_netin_evname[16];	/* network input ready event name */
	void		(*v_bootpcheck)();	/* driver bootp check routine */
} *Ifmanstat;

/*
	ifman path descriptor definitions
	
	Ifman does not normally perform I/O on a path basis but thru sockman
	via ifdev.  The path-related handling is for iniz/term and status
	operations only.
*/
struct ifm_pvt {
	void			*pd_stat;	/* ptr to driver's static storage */
	Ifmanstat		 pd_ifman;	/* ptr to ifman's static storage */
	struct ifdev	*pd_ifdev;	/* ptr to ifdev entry for device */
};

struct ifm_opt {
	u_char		pd_dtp;			/* device type */
	u_char		pd_padx;		/* padd */
	u_char		pd_device[32];	/* device descriptor name */
	char		pd_pad[62];		/* padding/scratch area */
};


#endif	/* _IFMAN_H */
