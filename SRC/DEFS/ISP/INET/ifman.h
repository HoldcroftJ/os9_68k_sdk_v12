/****************************************************************************
 *  $Workfile:   ifman.h  $
 *  $Revision:   200.13  $
 *  $Modtime:   17 Feb 1998 10:22:00  $
 *  $Date:   17 Feb 1998 10:26:14  $
 *	Function:	Various structure definitions for IFMAN file manager.
 ****************************************************************************
 *	Copyright 1988, 1989, 1990, 1993, 1994 by Microware Systems Corporation	*
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
 * OS-9 History...
 *	  1 ??/??/??	Birth											    ???	*
 *      09/16/93	<***** ISP 2.0 Release ***********************>			*
 * OS-9000 History...
 !  001 ??/??/?? Creation                                                ???  !
 !  002 89/12/15 Path descriptor definitions moved to ifdesc.h           DJN  !
 !  003 90/01/20 Completed modifications for OS-9000                     DJN  !
 !  004 90/03/21 Moved contents of ifdev.h here                          DJN  !
 !  005 90/03/30 Modified for combining OS9/OS9000 versions of IFMAN     DJN  !
 !  006 90/07/20 --------------- ISP/OS9000 Release 1.1 ---------------- DJN  !
 Merge & Post Merge History ...
 07	940104	OS9, 9000 merged again.  ifdev.h merged into ifman.h for OS9	gkm
 		  > renamed ifm_fm_stat to ifmanstat for OS9000 & use all OS9 type
 		  	identifiers within the structures.  Attach count is new for
 		  	OS9 & bootpcheck() for OS9
 		  > ifm_drvr_stat, no equivalent found in OS9 code.  Included only
 		  	for OS9000 ISP for now.
 		  > ulp_list, retain OS9/ISP id names as usual
 		  > if_this & if_ddmod new in ifdev from OS9000/ISP
 		  > if_outq is not used in ifdev for OS9, removed.
 		  > in ifdev function types changed to error_code from int32 for OS9

          > Moved ulp_list {} above ifmanstat{}.                       ylee
          > In ifdev{}, Separate if_man for OS-9 and OS-9000 so        ylee
            ifstat module displays correct message on current software.
          > _OSK defines ifm_pvt in ifman.h because Ifmanstat pd_ifman
            needs struct Ifmanstat to be defined. _OS9000 doesn't need
            pd_ifman so it is left in ifdesc.h.                        ylee
          > Add ifdvrmod{}, ifdvrdsp{} for _OSK only                   ylee
        94/05/02 if_mtu change from type int16 to u_int16           ts/ylee
        94/05/31 Get merged ifman.h works on OS-9 and OS-9000          ylee
 08 940621	Added ifman/ifloop structures from <sockcb.h>				bat
 09	950119	replaced COMMON_DEFS by _ISP_COMMON_DEFS					gkm
 10 950209	removed if_ddmod from ifdev structure (was for future merge)gkm
 11 950517  Added bootpcheck routine pointer to OS9000 IFMan static     mgh
    960117  <********* ISP 2.1 Release ***********************>
 12	960909	Added pci_specific definition and conn_code definitions		tad
    960812  <********* ISP 2.1.2 Release *********************>
    970219  <***** SPF LAN Comm Pack v3.0 Release ***********>      
    970429  <***** OS-9000 V2.2 Release (Low Level ROM)***********> 
            ---- OS-9000/Brutus Board Support V1.0 Released ----
 ****************************************************************************/

#ifndef _IFMAN_H
#define _IFMAN_H


#include <types.h>			/* Get system definitions */
#include <const.h>			/* -do- */
#include <memory.h>			/* -do- */
/*#include <structid.h>*/
#ifdef _OSK
	#include <sysio.h>
#endif


#include <INET/sysdefs.h>
#include <INET/socket.h>	/* Get socket definitions */


/*
 * Structure used for keeping track of the upper level protocols
 * to pass data to.
 */
typedef struct ulp_list {
	u_int32			ulp_ptype;			/* packet type desired by ULP */
	struct ulp_list	*ulp_next;			/* next element in ULP list */
	u_int32			(*ulp_recv_func)();	/* ULP's input function */
	void			*ulp_recv_stat;		/* parameter to ULP input function */
	PD_IFM			ulp_pd;				/* path used to create entry */
	u_int32			ulp_size;			/* size of this element */
} ulp_list, *Ulp_list;


/*
 *	ifman static storage definitions
 *	OS9 Ifman keeps its required goodies in the static storage of
 *	the loopback driver (lo0) which must attached before anything
 *	else.
 */
typedef struct ifmanstat {
	FM_STATIC		v_sysio;		/* required storage.  Diff for OS9 & 9000 */
	#ifdef _OSK
		u_int16		v_padder;		/* alignment for v_sysio (0x2e) */
									/* WARNING: Used by os9 bootpcheck calls */
	#endif
	struct ifdev	*v_iflist;		/* list of known interfaces forward*/
	struct ifdev	*v_ifback;		/* list of known interfaces backwards */
	Ulp_list		v_ulp_head;		/* head of ULP list */
	error_code		(*v_recvfunc)();	/* routine to handle incoming packets */
	error_code		(*v_arp_input)();	/* routine to handle incoming ARP packets */
	error_code		(*v_arp_resolve)(); /* routine to map IP to EN address */
	error_code		(*v_arp_whohas)(); 	/* routine to send ARP request */
	void			*v_arp_stat;		/* static storage for arp functions */
	#ifdef _OSK
		struct devicetbl	*v_loop_devtbl;		/* loop driver device table entry */
		u_int32		v_nonloop;		/* count of if devices other than lo0 */
	#endif
	PR_DESC			v_netin_procid;		/* process id ptr for the netin process */
	event_id		v_netin_event;		/* network input ready event */
	u_int8			*v_netin_flag;		/* netin process flag */
	char			v_netin_evname[16];	/* network input ready event name */
#ifdef _OSK
	void			(*v_bootpcheck)();	/* driver bootp check routine OS9 */
#endif
	u_int32			v_atcnt;			/* attach count */
#ifdef _OS9000
	void			(*v_bootpcheck)();	/* driver bootp check routine 9000*/
	u_int32			v_resvd[4];			/* reserved storage */	
#endif
} ifmanstat, *Ifmanstat;


/*
 * Layout of IF device driver static storage.
 */
typedef struct {
#if defined(_OSK)
	sysioStatic		if_dr_sysio;		/* KERNAL required storage */
#endif
	error_code		(*if_dr_init)();	/* Driver initialize routine */
	error_code		(*if_dr_term)();	/* Driver terminate routine */
	error_code		(*if_dr_send)();	/* Driver send routine */
	error_code		(*if_dr_xmit)();	/* Driver transmit routine */
	error_code		(*if_dr_recv)();	/* Driver receive routine */
	error_code		(*if_dr_stat)();	/* Driver status routine */
	process_id		if_dr_wait;			/* ID of waiting process */
	u_int16			if_dr_busy;			/* Driver busy flag */
	u_int16			if_dr_resv[9];		/* Reserved storage */
} ifm_drvr_stat, *Ifm_drvr_stat;

#define	PTYPE_ANY	0					/* wildcard packet type */


/* Interface description structure */
typedef struct ifdev {
	struct ifdev	*if_next;		/* next known interface */
	struct ifdev 	*if_prev;		/* previous interface */
	#ifdef _OS9000
		struct ifdev	*if_this;		/* this interface */
		MH_DEV			if_ddmod;		/* device desc ptr for device */
	#endif
		DEV_LIST		if_devtbl;		/* device tbl ptr for device */
	#ifdef _OSK
		sysioStatic		*if_static;		/* static storage ptr for device */
	#elif defined (_OS9000)
		Ifm_drvr_stat	if_static;		/* static storage ptr for device */
	#endif
	Ifmanstat		if_man;			/* ptr to ifman's static storage */
	char			*if_name;		/* ptr to interface name */
	u_int32			if_size;		/* size of this structure */
	u_int16			if_mtu;			/* maximum transmission unit */
	u_int16			if_flags;		/* interface flags */
	struct sockaddr	if_addr;		/* interface address (sockaddr) */
	u_char			if_paddr[16];	/* interface physical address */
	u_int32			if_subnet;		/* subnet mask */
	union {
		struct sockaddr	ifu_broadcast;	/* broadcast address filter */
		struct sockaddr	ifu_dstaddr;	/* point-to-point conn. dest. addr. */
	} if_ifu;
	#ifdef _OSK
		struct ifqueue	*if_outq;		/* output queue */
	#endif
	error_code		(*if_init)();	/* attach device */
	error_code		(*if_term)();	/* detach device */
	error_code		(*if_send)();	/* send a datagram */
	error_code		(*if_xmit)();	/* transmit raw packet */
	error_code		(*if_recv)();	/* input data siphon function */
	error_code		(*if_stat)();	/* determine status of interface */
	u_int32			if_ipackets;	/* num. input packets received */
	u_int32			if_opackets;	/* num. output packets sent */
	u_int32			if_ierrors;		/* num. input errors */
	u_int32			if_oerrors;		/* num. output errors */
	u_int32			if_collisions;	/* num. collisions on csma devices */
	#ifdef _OSK
		struct ifdev	*if_this;		/* this interface */
	#endif
	char 			if_devnam[32];	/* copied device name string */
	char 			if_dvrnam[32];	/* copied driver name string */
} ifdev, *Ifdev;

#define IFF_UP			0x1		/* interface is up */
#define IFF_BROADCAST	0x2		/* interface can broadcast */
#define IFF_DEBUG		0x4		/* debugging mode */
#define IFF_ROUTE		0x8		/* routing entry installed */
#define IFF_PT_TO_PT	0x10	/* point to point link */
#define IFF_NOTRAILERS	0x20	/* don't use trailers */
#define IFF_RUNNING		0x40	/* online.. */
#define IFF_TAKE_NFWD	0x80	/* accept "no forward" packets */
#define IFF_NOARP		0x100	/* no address resolution protocol */
#define IFF_NOTETHER	0x200	/* not an ethernet interface (Kludge) */

#define	IA_SIN(ia) ((struct sockaddr_in *)(&(ia)->if_addr))
#define ID_SIN(ia) ((struct sockaddr_in *)(&(ia)->if_ifu.ifu_dstaddr))


/*
 * Device descriptor layout for an if device
 * Think about reconciling OS9 & 9000 structures later (1/4/94)
 */
typedef struct ifdevdd {
	DD_COM		dds;			/* standard device descriptor */
	#if defined(_OSK)
		u_int32	dd_name;		/* (offset to) interface (descriptor) name */
	#elif defined(_OS9000)
		u_int16	dd_vector;		/* IRQ vector */
		u_int16	dd_priority;	/* IRQ polling priority */
		u_int16	dd_irqlevel;	/* Hardware IRQ level */
	#endif
	u_int16		dd_mtu;			/* maximum transmission unit */
	u_int16		dd_flags;		/* interface flags */
	#if defined (_OS9000)
		u_int16	dd_resvd;		/* reserved */
		u_int32		dd_addr;		/* (offset to) struct sockaddr logical address */
	#elif defined(_OSK)
		u_int16		dd_addr;		/* (offset to) struct sockaddr logical address */
	#endif
	u_int32		dd_subnet;		/* subnet mask */
	#if defined(_OSK)
		u_int16		dd_bdaddr;		/* (offset to) broadcast/destination address */
		u_int16		dd_linkaddr;	/* (offset to) link-level address */
	#elif defined(_OS9000)
		u_int32		dd_bdaddr;		/* (offset to) broadcast/destination address */
		u_int32		dd_linkaddr;	/* (offset to) link-level address */
	#endif
	u_int32		dd_spare[5];	/* reserved */
} IFDEVDD, *Ifdevdd;

/*
 *	ifloop device specific section
 *	used to pass tunable parameters to ifman
 */
typedef struct ifloopdevdd {
	u_int32		if_priority;	/* priority for ifman's net_in process */
} IFLOOPDEVDD, *Ifloopdevdd;

/* Path descriptor options section
 */
struct ifm_opt {
	u_char			pd_dtp;				/* device type */
	u_char			pd_padx;			/* padd */
	u_char			pd_device[32];		/* device descriptor name */
	char			pd_pad[62];			/* padding/scratch area */
};

/*
**	Path descriptor definition
**
**	Ifman does not normally perform I/O on a path basis but thru sockman
**	via ifdev.  The path-related handling is for iniz/term and status
**	operations only.
*/
#if defined(_OSK)

	struct ifm_pvt {
		void*			pd_stat;		/* ptr to driver's static storage */
		Ifmanstat		pd_ifman;		/* ptr to ifman's static storage */
		Ifdev			pd_ifdev;		/* ptr to ifdev entry for device */
	};

	#define		IFMAN			1
	
	#define		Pd_ifm			Pathdesc
	#define		ifmpvt			path.fm_pvt.ifm
	#define		ifmopt			path.fm_opt.ifm

#elif defined(_OS9000)

	struct ifm_pvt {
		struct ifdev	*pd_ifdev;	/* ptr to ifdev entry for device */
	};

	typedef struct pd_ifm {
		pd_com			pd_io;			/* IOMAN common section */
		struct ifm_pvt	ifmpvt;			/* IFMAN private section */
		struct ifm_opt	ifmopt;			/* IFMAN options section */
	} pd_ifm, *Pd_ifm;
	#define PD_IFM_SIZE 200				/* sizeof(pd_ifm) */

#endif


/*
**	IFMAN Control Block Structures
*/

#if defined(_OSK)
	#define SS_DSTAT	SS_DStat
	#define SS_DCMD		SS_DCmd
#endif

typedef struct {
	u_int32		ptype;				/* Packet type. */
	error_code	(*hfunc)();			/* Handler function. */
	void		*hstat;				/* Handler memory. */
} ss_install_pb, *Ss_install_pb;

typedef struct {
#if defined(_OSK) && !defined(_ISP_COMMON_DEFS)
	/* This looks like OS-9 REGISTERS structure for backward compatability */
	u_int32				pad1[2];	/* d0/d1 */
	union {							/* d2 */
		u_int32			buflen;		/* -- buffer length */
		u_int32			dcode;		/* -- SS_DCMD code */
	} len_cod;
	u_int32				buflen;		/* d3 -- buffer length */
	u_int32				pad2[4];	/* d4/d5/d6/d7 */
	void *				buffer;		/* a0 -- route table/entry copied to/from */
	u_int32				pad4[7];	/* a1/a2/a3/a4/a5/a6/a7 */
#elif defined(_OS9000) || defined(_ISP_COMMON_DEFS)
	union {
		u_int32			buflen;
		u_int32			dcode;
	} len_cod;
	void *				buffer;
	u_int32				buflen;
#endif
} ss_ifloop_pb, *Ss_ifloop_pb;

typedef struct {
	u_int32				code;
	void*				buf;
	u_int32				buflen;
} gs_dcmd_pb, *Gs_dcmd_pb;

typedef struct {
#if defined(_OSK) && !defined(_ISP_COMMON_DEFS)
	u_int32				pad1[2];	/* d0/d1 */
	u_int32				buflen;		/* d2 -- buffer length */
	u_int32				pad2[5];	/* d3/d4/d5/d6/d7 */
	void*				buf;		/* a0 -- buffer pointer */
	void*				pad4[7];	/* a1/a2/a3/a4/a5/a6/a7 */
#elif defined(_OS9000) || defined(_ISP_COMMON_DEFS)
	void*				buf;
	u_int32				buflen;
#endif
} gs_dstat_pb, *Gs_dstat_pb;


#define	IMB_VER_MIN		1
#define	IMB_VER_MAX		1

typedef struct {
	u_int16		id;
	u_int16		ver;
	char		ifDescr[128];
	u_int32		ifType;
		#define IFTYPE_OTHER		    				1
		#define IFTYPE_REGULAR1822			    		2
		#define IFTYPE_HDH1822		    				3
		#define IFTYPE_DDNX25		    				4
		#define IFTYPE_RFC877X25		    			5
		#define IFTYPE_ETHERNETCSMACD	    			6
		#define IFTYPE_ISO88023CSMACD	    			7
		#define IFTYPE_ISO88024TOKENBUS	    			8
		#define IFTYPE_ISO88025TOKENRING	    		9
		#define IFTYPE_ISO88026MAN		    			10
		#define IFTYPE_STARLAN		    				11
		#define IFTYPE_PROTEON10MBIT	    			12
		#define IFTYPE_PROTEON80MBIT	    			13
		#define IFTYPE_HYPERCHANNEL		    			14
		#define IFTYPE_FDDI			    				15
		#define IFTYPE_LAPB			    				16
		#define IFTYPE_SDLC			    				17
		#define IFTYPE_T1CARRIER		    			18
		#define IFTYPE_CEPT			    				19
		#define IFTYPE_BASICISDN		    			20
		#define IFTYPE_PRIMARYISDN		    			21
		#define IFTYPE_PROPPOINTTOPOINTSERIAL   		22
	u_int32		ifMtu;
	u_int32		ifSpeed;
    u_char		ifPhysAddress[11];	/* interface's address */
    u_char		PhysAddrLen;    	/* length of physAddr */
    u_int32		ifOperStatus;   	/* current operational status */
		#define MIB_IFSTATUS_UP								1
		#define MIB_IFSTATUS_DOWN							2
		#define MIB_IFSTATUS_TESTING						3
    u_int32		ifLastChange;   	/* value of sysUpTime when current state entered */
    u_int32		ifInOctets;	    	/* number of octets received on interface */
    u_int32		ifInUcastPkts;  	/* number of unicast packets delivered */
    u_int32		ifInNUcastPkts; 	/* number of broadcasts or multicasts */
    u_int32		ifInDiscards;   	/* number of packets discarded with no error */
    u_int32		ifInErrors;	    	/* number of packets containing errors */
    u_int32		ifInUnknownProtos;	/* number of packets with unknown protocol */
    u_int32		ifOutOctets;    	/* number of octets transmitted */
    u_int32		ifOutUcastPkts; 	/* number of unicast packets sent */
    u_int32		ifOutNUcastPkts;	/* number of broadcast or multicast pkts */
    u_int32		ifOutDiscards;  	/* number of packets discarded with no error */
    u_int32		ifOutErrors;    	/* number of pkts discarded with an error */
    u_int32		ifOutQLen;	    	/* number of packets in output queue */
    u_char		ifSpecific[16];		/* OBJECT IDENTIFIER of interface */
    u_char		SpecificLen;		/* length of ifSpecific */
} ifcomstats, *Ifcomstats;

#if defined(_OSK)

/*
**	Layout of IFMAN device driver module
*/
struct ifdvrmod {
	struct modhcom _mh;		/* standard module header */
	u_int	_mexec;			/* offset to dispatch works */
	u_int	_mexcpt;		/* offset to exception handler */
	u_int	_mdata;			/* static storage requirement */
};

/*
**	IFMAN driver dispatch table
*/
typedef struct ifdvrdsp {
							/* standard IOMAN dispatch */
	int16	init;
	int16	read;
	int16	write;
	int16	getstat;
	int16	putstat;
	int16	term;
	int16	trap;
							/* ifman dispatch */
#if 0
	error_code if_init(Ifdev ifd, u_int32 prcid, Sysglobs sysglob);
	error_code if_send(Ifdev ifd, Mbuf mbp, struct sockaddr* dst);
	error_code if_xmit(Ifdev ifd, Mbuf mbp);
	error_code if_recv(Ifdev ifd);
	error_code if_stat(Ifdev ifd, Pd_ifm pd, void* pb, u_int32 code);
	error_code if_term(Ifdev ifd);
#else
	int32	if_init;
	int32	if_term;
	int32	if_send;
	int32	if_xmit;
	int32	if_recv;
	int32	if_stat;
#endif
} ifdrvdsp, *Ifdrvdsp;

#elif defined(_OS9000)


#endif

	/* conn_code -- used to specify the XCVR type to use on ethernet adapters
		with more than one. */

typedef u_int16 conn_code;
		
/*interface numbers */

#define INF_AUTO		0x00	/* auto configure interface (needs driver 
												support)*/
#define	INF_BNC			0x01	/* coaxial cable  (10BASE2*/
#define	INF_AUI			0x02	/* AUI connector */
#define INF_UTP			0x03	/* unshielded twisted pair (10BASET)*/
#define INF_STP			0x04	/* shielded twisted pair */
#define INF_EXT			0xff	/* defined external to driver. (NVRAM, etc.) */


	/* pci_specific struct -- used to specify location of pci adapters */

typedef struct pci_specific {

	u_int8			index,		/* PCI device index */
					bus,		/* PCI bus line */
					dev,		/* PCI device number (hardware line)*/
					resv[5];	/* RESERVED */
					
} pci_specific, *Pci_specific;


#endif /* _IFMAN_H */
