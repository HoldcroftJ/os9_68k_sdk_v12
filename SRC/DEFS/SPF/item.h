/****************************************************************************
 *	File:	%A%
 *	ID:		%G%
 ****************************************************************************
 * Copyright (c) 1995 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of Microware 
 * Systems Corporation, and is provided to licensee solely for documentation 
 * and educational purposes. Reproduction, publication, or distribution in
 * any form to any party other than the licensee is strictly prohibited.
 *
 ****************************************************************************
 *
 * ed#   date    changes                                          	by
 * --- -------- -------------------------------------------------	---
 *     YY/MM/DD
 *  1  95/03/08 Origination											cas
 *     96/06/30 <---------------- SPF v2.0(pre) ---------------->   cas
 *
 *  2  95/10/10 Added ite_init() and ite_term() prototypes         udayn
 *     96/07/18 Added convenient NPB fill-out macros                jmh
 *     96/09/12 Moved return_type structure from ch_mgr.h to here.  jmh
 *     96/09/26 Added ite_linkup_asgn() and ite_linkup_rmv().       jmh
 *  3  96/10/22 <---------------- SPF v2.1.1 ------------------->   cas
 *  3  97/03/18 Removed ite_path_pull(), ite_path_drop(),
 *              ite_notify_asgn() and ite_notify_rmv for SPF v2.2   taka
 *  3  97/03/19 <---------------- SPF v2.2 (pre) --------------->   taka
 *  3  97/04/04 mbuf.h is now in SPF                                taka
 *  3  97/06/26 <---------------- SPF v2.2 --------------------->   taka
 * 210 97/09/25 added ITE_ON_DR_DEFINE type on notify_type.notify_on
 *				for driver specific notification					taka
 * 211 97/10/23 mpeg api functionalities are removed from item library
 * 																	taka
 * 212 97/10/31 Add ethernet address class ITE_ADCL_ENET			taka
 *     97/11/04 <---------------- SPF v2.2.1 ------------------->   taka
 *     99/04/08 Call union notify in notify_type to notify_item     sr
 *              if notify has already been defined.
 ****************************************************************************/

#ifndef _ITEM
#define _ITEM

/*
 * This is the definitions file that enables programs to communicate with
 * SPF 2.0 using the ITEM interface.
 */

/* Other needed include files */
#include <SPF/spf.h>		/* No SPF_DRVR so only includes app level defs	*/
#include <SPF/mbuf.h>		/* Read mbuf/write mbuf prototypes use this		*/

/* Needed macros */
#define ITE_MAX_DISPLAYSIZE		32		/* Number of characters in display	*/
#define ITE_NO_IB_PATH			0xFFFF	/* Indicates on _ctl_ connections, no
										 * in band path returned.			*/
/* Define interactive TV Path Type macros (also labelled "index" for the
 * data carousel ITEM calls.
 */
#define DC_TYPE_CHANLIST	0x01	/* Channel List index				*/
#define DC_TYPE_VIPDIR		0x02	/* VIP Directory index				*/
#define DC_TYPE_L1PROTS		0x03	/* Level 1 data processors for STB	*/
#define DC_TYPE_L1NAV		0x04	/* Level 1 Nevigator for STB		*/

/* Format for the Address information (telephone #) parameter */
typedef struct addr_type {
	u_char	addr_class;						/* Address classification 	*/
		#define ITE_ADCL_NONE		0x00	/* No addressing for drvr	*/
		#define ITE_ADCL_UNKNOWN	0x01	/* Unknown address class	*/
		#define ITE_ADCL_E164		0x02	/* Per E.164 specification	*/
		#define ITE_ADCL_INET		0x03	/* Socket based addressing	*/
		#define ITE_ADCL_RSV1		0x04	/* Not used at this time	*/
		#define ITE_ADCL_X25		0x05	/* X.25 addressing			*/
		#define ITE_ADCL_ATM_ENDSYSTEM  0x06	/* For ATM sub address	*/
		#define ITE_ADCL_LPBK		0x07	/* Address loopback driver	*/ 
		#define ITE_ADCL_NSAP		0x08	/* DSM-CC and ATM NSAP (20 bytes)	*/
		#define	ITE_ADCL_DTE		0x09	/* Data Terminal Equipment	*/
		#define	ITE_ADCL_DCE		0x0A	/* Data Communication Equip.*/
		#define ITE_ADCL_LAPD		0x0B	/* LAP-D address (TEI/SAPI)	*/
		#define ITE_ADCL_ENET		0x0C	/* Ethernet address class	*/
	u_char	addr_subclass;					/* Address sub-classification	*/
		#define ITE_ADSUB_NONE		0x00	/* No addressing for drvr	*/
		#define ITE_ADSUB_UNKNOWN	0x01	/* Unknown subclass			*/
		#define ITE_ADSUB_VC		0x02	/* Virtual circuit subclass	*/
		#define ITE_ADSUB_PVC		0x03	/* Permanent Virtual circuit*/
		#define ITE_ADSUB_LUN		0x04	/* Logical unit # is address*/
		#define ITE_ADSUB_SLINK		0x05	/* Singlelink addr (DCE/DTE)*/
		#define ITE_ADSUB_MLINK		0x06	/* Multilink addr (DCE/DTE)	*/
	u_char	addr_rsv1;						/* Long word boundary		*/
	u_char	addr_size;			/* Size of address byte string (# bytes)	*/
	char	addr[32];							/* Address byte string		*/
} addr_type, *Addr_type;

/* Format of the user accessable parameters of the device/path */
typedef struct device_type {
	u_int16		dev_mode;
	u_char		dev_netwk_in,	/* Network device type, receive path		*/
				dev_netwk_out;	/* Network device type, transmit path		*/
		#define ITE_NET_NONE	0x00	/* This device is unidirectional	*/
		#define ITE_NET_CTL		0x01	/* Control channel	*/
		#define ITE_NET_DATA	0x02	/* DATA channel		*/
		#define ITE_NET_MPEG2	0x03	/* MPEG-2			*/
		#define ITE_NET_CHMGR 	0x04	/* Channel Mgr		*/
		#define ITE_NET_OOB		0x05	/* Out of band signalling	*/
		#define	ITE_NET_VIPDIR	0x06	/* VIP Directory device */
		#define	ITE_NET_SESCTL	0x07	/* Session control/mgmt		*/
		#define	ITE_NET_X25		0x08	/* X.25 network		*/
		#define ITE_NET_ANY		0xFF	/* Processes multiple types of data */
	u_int16		dev_callstate;		/* Path connection state (Bit fields)	*/
		#define ITE_CS_IDLE     0x0001	/* NULL state		*/
		#define ITE_CS_INCALL   0x0002	/* Incoming call 	*/
		#define ITE_CS_CONNEST  0x0004	/* Establishing conn*/
		#define ITE_CS_ACTIVE   0x0008	/* Connected 		*/
		#define ITE_CS_CONNTERM 0x0010	/* Terninating conn */
		#define ITE_CS_CONNLESS 0x0020	/* Connectionless	*/
		#define ITE_CS_SUSPEND	0x0040	/* Terninating conn */
	u_char		dev_rcvr_state;	/* Used for receiver assignment	*/
		#define ITE_ASGN_RSVD		0x00	/* No rcvr asgnmt available	*/
		#define ITE_ASGN_NONE		0x01	/* Receiver not assigned	*/
		#define ITE_ASGN_THEIRNUM	0x02	/* Only answer on theirnum	*/
		#define ITE_ASGN_ANY		0x03	/* Answer any call			*/
		#define ITE_ASGN_PROFILE	0x04	/* Only if profile matches	*/
	u_char		dev_rsv1;		/* Long word boundary						*/
	u_int32		dev_rsv2;		/* Reserved for future use					*/
	addr_type	dev_ournum,		/* Our connection number (address)			*/
				dev_theirnum;	/* Their connection number (address)		*/
	char		dev_display[ITE_MAX_DISPLAYSIZE];		/* Display screen	*/
} device_type, *Device_type;

/* Notify entry structure. This structure allows users the ability to
 * block on a call, have signals sent, or have events set.
 *
 * Variables in this structure the user NEED NOT be concerned with are:
 *    *next: The system uses this to chain notifications. Set to NULL.
 *    notify_on: Caller Only fills in for ite_notify_asgn() calls.
 *    notify_rsv1[]: Just there for long word boundary.
 *    proc_num: The system finds this out when you make asynchronous calls.
 */
typedef struct notify_type {
	struct notify_type	*ntfy_next;	/* Allow for chaining notifications	*/
	u_char				ntfy_class;	/* Class of notification requested	*/
		#define ITE_NCL_BLOCK	 0x00	/* Block until request fulfilled	*/
		#define ITE_NCL_SIGNAL	 0x01	/* Send a signal					*/
		#define ITE_NCL_EVENT	 0x02	/* Set an event						*/
		#define ITE_NCL_MMBOX	 0x03	/* MAUI Mailbox notification		*/
		#define ITE_NCL_CALLBACK 0x04	/* Callback function				*/
		#define ITE_NCL_SIGINC	 0x81	/* Incrementing signals				*/
		#define ITE_NCL_EVENTINC 0x82	/* Incrementing events				*/
	u_char				ntfy_on;	/* What to send the notification for	*/
		#define ITE_ON_NONE		 0x00	/* Not used/reserved				*/
		#define ITE_ON_LINKDOWN	 0x01	/* Notify on data link down			*/
		#define ITE_ON_INCALL	 0x02	/* Notify on incoming call			*/
		#define ITE_ON_CONN		 0x03	/* Notify on connect				*/
		#define ITE_ON_DATAVAIL	 0x04	/* Notify on data available			*/
		#define ITE_ON_ENDPGM	 0x05	/* Notify on end of MPEG-2 program	*/
		#define ITE_ON_FEHANGUP	 0x06	/* Notify on far end hangup			*/
		#define ITE_ON_DNLDONE	 0x07	/* Notify on download procedure done*/
		#define ITE_ON_MSGCONF	 0x08	/* Confirm receipt of sent message	*/
		#define ITE_ON_RESADD	 0x09	/* Ntfy on resource added to sess	*/
		#define ITE_ON_LINKUP	 0x0A	/* Ntfy on data link up				*/
		#define ITE_ON_FCTLON	 0x0B	/* Ntfy on flow control on			*/
		#define ITE_ON_FCTLOFF	 0x0C	/* Ntfy on flow control off			*/
		#define ITE_ON_DR_DEFINE 0xFF	/* Notification for driver specific */
	u_char				ntfy_rsv1;		/* Reserved for future use			*/
	u_char				ntfy_ctl_type;	/* Notification control type		*/
		#define NTYPE_NONE		0x00	/* ntfy_ctl field not used			*/
		#define NTYPE_SESSCTL	0x01	/* ntfy_ctl=ntfy_sesctl (sess_mgr.h)*/
		#define NTYPE_RESCTL	0x02	/* ntfy_ctl=ntfy_resctl (sess_mgr.h)*/
		#define NTYPE_CONNTYPE	0x03	/* ntfy_ctl=conn_type				*/
		#define NTYPE_RETURN 	0x04	/* ntfy)ctl=return_type             */
	void				*ntfy_ctl;		/* Pointer to user data				*/
	u_int32				ntfy_timeout;	/* Timeout value in seconds			*/
	u_int32				ntfy_rsv[2];	/* # elements in ibpath_list array	*/
	union {
		struct {
			u_int32		proc_id,		/* Process Identifier				*/
						sig2send;		/* Signal to send/event to set		*/
		} sig;
		struct {
			u_int32		ev_id;			/* Event Identifier					*/
			int32		ev_val;			/* Event value to set				*/
		} ev;
		struct {
			u_int32		ev_id;			/* Event Identifier					*/
			int32		ev_inc_val;		/* Event increment value			*/
		} inc_ev;
		struct {
			u_int32		mmbox_handle;		/* Mailbox handle				*/
			error_code	(*callback_func)();	/* Func to call on notification	*/
		} mmbox;
		struct {
			void		*callbk_param;		/* Parameter passed				*/
			error_code	(*callback_func)();	/* Drivers should doc the params*/
		} callbk;
		/* The following is done for compatibility issues arising because
		 * rom.h defines notify as new.notify or old.notify), thus causing 
		 * conflicts when used with rom.h.  If notify is defined we call the
		 * union notify_item.
		 */
#ifdef notify
	} notify_item;
		#define ntfy_procid		notify_item.sig.proc_id
		#define ntfy_sig2send	notify_item.sig.sig2send
		#define ntfy_evid		notify_item.ev.ev_id
		#define ntfy_eval		notify_item.ev.ev_val
		#define ntfy_evinc		notify_item.inc_ev.ev_inc_val
		#define ntfy_mmhandle	notify_item.mmbox.mmbox_handle
		#define ntfy_mmcall		notify_item.mmbox.callback_func
		#define ntfy_callback	notify_item.callbk.callback_func
		#define ntfy_param		notify_item.callbk.callbk_param
#else
	} notify;
		#define ntfy_procid		notify.sig.proc_id
		#define ntfy_sig2send	notify.sig.sig2send
		#define ntfy_evid		notify.ev.ev_id
		#define ntfy_eval		notify.ev.ev_val
		#define ntfy_evinc		notify.inc_ev.ev_inc_val
		#define ntfy_mmhandle	notify.mmbox.mmbox_handle
		#define ntfy_mmcall		notify.mmbox.callback_func
		#define ntfy_callback	notify.callbk.callback_func
		#define ntfy_param		notify.callbk.callbk_param
#endif
} notify_type, *Notify_type;

/* Macro to fill out notify_type structure allocated by the caller to
 * return a signal or set an event.
 */
#define NPB_INIT_SIG(n, timeout, sigval) \
			(n)->ntfy_class = ITE_NCL_SIGNAL; \
			(n)->ntfy_timeout = timeout; \
			(n)->ntfy_sig2send = sigval; \
			(n)->ntfy_next = NULL; \
			(n)->ntfy_ctl_type = NTYPE_NONE; \
			(n)->ntfy_ctl = NULL

#define NPB_INIT_EV(n, timeout, eid, eval) \
			(n)->ntfy_class = ITE_NCL_EVENT; \
			(n)->ntfy_timeout = timeout; \
			(n)->ntfy_evid = eid; \
			(n)->ntfy_eval = eval; \
			(n)->ntfy_next = NULL; \
			(n)->ntfy_ctl_type = NTYPE_NONE; \
			(n)->ntfy_ctl = NULL

#define NPB_INIT_EVINC(n, timeout, eid, eval) \
			(n)->ntfy_class = ITE_NCL_EVENTINC; \
			(n)->ntfy_timeout = timeout; \
			(n)->ntfy_evid = eid; \
			(n)->ntfy_eval = eval; \
			(n)->ntfy_next = NULL; \
			(n)->ntfy_ctl_type = NTYPE_NONE; \
			(n)->ntfy_ctl = NULL

#define NPB_INIT_CB(n, cb_func, cb_param) \
			(n)->ntfy_class = ITE_NCL_CALLBACK; \
			(n)->ntfy_timeout = 0; \
			(n)->ntfy_callback = cb_func; \
			(n)->ntfy_param = cb_param; \
			(n)->ntfy_next = NULL; \
			(n)->ntfy_ctl_type = NTYPE_NONE; \
			(n)->ntfy_ctl = NULL


/* Return Type for use with the Notification Parameter Block
 * (Used to return an error_code to an application upon completion
 * of an asynchronous call.
 */
typedef struct return_type {
    error_code rtn_status;
} return_type, *Return_type;


/* Parameter block for ite_ctl_disconnect() & ite_ctl_answer() calls */
typedef struct ite_cctl_pb {
	void	*ib_reslist;	/* In band resource list returned (for answer)	*/
	u_int16	response,		/* Response to send								*/
			reason,			/* Reason to send								*/
			rsv1,
			usr_data_cnt;	/* Number of bytes of user data to send			*/
	void	*usr_data;		/* Pointer to user data to send					*/
} ite_cctl_pb, *Ite_cctl_pb;

/** Function prototypes for all ITEM API calls **/

#ifdef __cplusplus
	extern "C" {
#endif

/* ITEM Initialization calls 
 * ite_init() must be called before any other ITEM call can be used
 */
error_code ite_init(void); 
error_code ite_term(void);

/* Device oriented API calls */
error_code ite_dev_attach(char *name, u_int32 mode, u_int32 *handle);
error_code ite_dev_detach(u_int32 handle);
error_code ite_dev_getmode(path_id path, u_int16 *mode);
error_code ite_dev_getname(path_id path, char *name);
error_code ite_dev_gettype(path_id path, u_char *type_in, u_char *type_out);
error_code ite_dev_setmode(path_id path, u_int16 mode);

/* Path oriented API calls */
error_code ite_path_clone(path_id dup_path, path_id *new_path,
															Notify_type npb);
error_code ite_path_close(path_id path);
error_code ite_path_dup(path_id dup_path, path_id *new_path);
error_code ite_path_open(char *dev_name, u_int32 mode, path_id *path_ptr,
														Addr_type our_num);
error_code ite_path_push(path_id path, char *dev_name);
error_code ite_path_pop(path_id);

/* This gets and sets the transmit message operational parameters for
 * out-of-band protocol drivers.
 */
error_code ite_path_txopget(path_id path, u_int16 *tx_msgtype_ptr);
error_code ite_path_txopset(path_id path, u_int16 tx_msgtype);

/* Call control oriented API calls */
error_code ite_ctl_addrset(path_id path, Addr_type our_num,
														Addr_type their_num);
error_code ite_ctl_answer(path_id path, Ite_cctl_pb ccpb, Notify_type npb);
error_code ite_ctl_connect(path_id path, Addr_type ournum, Addr_type theirnum,
															Notify_type npb);
error_code ite_ctl_connstat(path_id path, Device_type dev_info);
error_code ite_ctl_disconnect(path_id path, Ite_cctl_pb ccpb);

error_code ite_ctl_rcvrasgn(path_id path, Addr_type their_num,
															Notify_type npb);
error_code ite_ctl_rcvrrmv(path_id path);

/* Secondary signalling calls */
error_code ite_ctl_suspend(path_id path, u_int32 keep_resources_flag);
error_code ite_ctl_resume(path_id path, Notify_type npb);

/* Resource-oriented control and management calls */
error_code ite_ctl_resadd(path_id sess_path, Notify_type npb);
error_code ite_ctl_resdel(path_id sess_path, u_int16 res_count, void **reslist);
error_code ite_ctl_reslist(path_id sess_path, void **res_cfg_list,
													u_int32 *res_cfg_cnt);

/* Other generic asynchronous notification calls */
error_code ite_fehangup_asgn(path_id path, Notify_type npb);
error_code ite_fehangup_rmv(path_id path);
error_code ite_linkdown_asgn(path_id path, Notify_type npb);
error_code ite_linkdown_rmv(path_id path);
error_code ite_linkup_asgn(path_id path, Notify_type npb);
error_code ite_linkup_rmv(path_id path);
error_code ite_ibreschg_asgn(path_id path, Notify_type npb);
error_code ite_ibreschg_rmv(path_id path);

/* Data transmission/reception oriented API calls */
error_code ite_data_avail_asgn(path_id path, Notify_type npb);
error_code ite_data_avail_rmv(path_id path);
error_code ite_data_ready(path_id path, u_int32 *in_count);
error_code ite_data_read(path_id path, void *buffer, u_int32 *count);
error_code ite_data_readmbuf(path_id path, Mbuf *mb_ptr);
error_code ite_data_write(path_id path, void *buffer, u_int32 *count);
error_code ite_data_writembuf(path_id path, Mbuf mb_ptr);
error_code ite_data_sendto(path_id path, void *buffer, u_int32 size,
										u_int32 flags, Addr_type sendto_addr);
error_code ite_data_recvfrom(path_id path, void *buffer, u_int32 size,
									u_int32 flags, Addr_type recvfrom_addr);

/* Application/module download oriented API calls */
error_code ite_dnld_initiate(path_id path, void *exec_table,
										u_int32 *table_size, Notify_type npb);
error_code ite_dnld_modvalidate(path_id path, void *buffer, u_int32 size);

#ifdef __cplusplus
	}
#endif

#endif /* _ITEM */

