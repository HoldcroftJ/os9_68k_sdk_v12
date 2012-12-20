/****************************************************************************
 * Copyright (c) 1994 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of Microware 
 * Systems Corporation, and is provided to licensee solely for documentation 
 * and educational purposes. Reproduction, publication, or distribution in
 * any form to any party other than the licensee is strictly prohibited.
 *
 ****************************************************************************
 *
 * ed#   date    changes                                            by
 * --- -------- -------------------------------------------------   ---
 *  1  03/08/95 Origination                                         cas
 *     06/30/95 <---------------- SPF v2.0(pre) ---------------->   cas
 *  2  09/12/96 Added PSI get/setstat codes for mpeg calls          jmh
 *  3  10/22/96 <---------------- SPF v2.1.1 ------------------->   cas
 *     10/30/96 Added ITE_MB_ANCHOR setstat code                    jmh
 *  4  06/17/97 Added socket_ptr using current_ch                   taka
 *  4  06/26/97 <---------------- SPF V2.2 --------------------->   taka
 *     11/03/97 Added ITE_GETADDR                                   taka
 *     11/04/97 <---------------- SPF v2.2.1 ------------------->   taka
 * 212 07/20/98 <---------------- SPF v2.2.2 ------------------->   taka
 * 263 06/28/99 Added lu_flags field and ite_st_pb structure        rds
 ****************************************************************************/

#ifndef _ITEM_PVT
#define _ITEM_PVT

/* Include files needed for this defs file */
#ifndef _OS9000
#include <defconv.h>
#endif
#include <sg_codes.h>
#include <SPF/item.h>
#include <const.h>
#include <io.h>
#include <errno.h>
#include <SPF/prot_ids.h>
#include <srvcb.h>
#include <modes.h>

/* Function prototypes */
#ifdef _OSK
error_code _os_setstat(path_id path, u_int32 spf_code, void *pb);
error_code _os_getstat(path_id path, u_int32 spf_code, void *pb);
#endif

/*
 * These are the definitions used internally by the ITEM code to implement
 * the ITEM APIs. There should be no reason why other code would need to
 * include this file. Structures in this file are allowed to change based on
 * later versions of ITEM.
 */

/* This macro is defined just so the _os_getstat/_os_setstat calls look more
 * logical. SPF uses SS_PMOD for protocol module codes. Any other goes to
 * the driver. So we set SS_ITE = SS_PMOD for conformancy.
 */
#define SS_ITE	SS_PMOD

/* General macros used for limits */
#define MAX_NOTIFY	16						/* Max # of nofication entries	*/

/* Path structure. This structure contains all the per-path information
 * needed for ITEM.
 */
typedef struct path_type {
	notify_type notify_list[MAX_NOTIFY];	/* Notification list			*/
		/* Notification ordinals are officially defined in item.h under
		 * the notify_on parameter in the notify_type structure definition.
		 *		#define ITE_ON_NONE		0x00
		 *		#define ITE_ON_LINKDOWN	0x01
		 * 		#define ITE_ON_INCALL	0x02
		 *		#define ITE_ON_CONN		0x03
		 *		#define ITE_ON_DATAVAIL	0x04
		 *		#define ITE_ON_ENDPGM	0x05
		 *		#define ITE_ON_FEHANGUP	0x06
		 *		#define ITE_ON_DNLDONE	0x07
		 *		#define ITE_ON_MSGCONF	0x08
		 *		#define ITE_ON_RESADD	0x09
		 *		#define ITE_ON_LINKUP	0x0A
		 *		#define ITE_ON_FCTLON	0x0B
		 *		#define ITE_ON_FCTLOFF	0x0C
		 */
	u_char		path_rsv1[2];
	u_int16		read_pid;					/* Read PID for this path		*/
	error_code	error_state;				/* Asynchronous error state		*/
	device_type conn_info;					/* path connection information	*/
	void		*current_ch;				/* current channel (Ch_type)	*/
		#define socket_ptr current_ch
} path_type, *Path_type;

/*	Setstat/Getstat codes: The range for all ITEM getstat/setstat codes is
 *	0x0800 - 0xFFFF. 
 *
 *	Sub-codes within ITEM are broken down per the system below. Each
 *	partition within ITEM has 256 codes available for use. Partitions
 *	0x01-0x20 are reserved for aysnchronous codes within that particular
 *	sub-code and will correspond to	the ordinal position in the notify_list
 *	array.
 */

/* Code Partitions. These codes are used in the spf_ss_pb structure, code
 * field. *Note* that these codes must be compatable with the contents of
 * the spf protocol ID definitions file.
 */
#define CALLCTL		SPF_PR_ITEM + (0x01 << 8)	/* Call control codes		*/
#define PGMCTL		SPF_PR_ITEM + (0x02 << 8)	/* Program control codes	*/
#define CHCTL		SPF_PR_ITEM + (0x03 << 8)	/* Channel Management		*/
#define DWNLDCTL	SPF_PR_ITEM + (0x04 << 8)	/* Download Management		*/
#define MBCTL		SPF_PR_ITEM + (0x05 << 8)	/* Mbuf oriented calls		*/
#define STRMCTL		SPF_PR_ITEM + (0x06 << 8)	/* Stream oriented calls	*/
#define UN_MSG		SPF_PR_ITEM + (0x07 << 8)	/* User-network messaging	*/
#define ASYNASGN	SPF_PR_ITEM + (0x08 << 8)	/* Notify assignment		*/
#define ASYNREM		SPF_PR_ITEM + (0x09 << 8)	/* Notify removal			*/
#define SOCKETCTL	SPF_PR_ITEM + (0x0A << 8)	/* Socket oriented calls	*/
#define TIMERCTL	SPF_PR_ITEM + (0x0B << 8)	/* Appl. timer svc calls	*/
#define TESTCTL		SPF_PR_ITEM + (0xF7 << 8)	/* Used for test codes		*/

/* Call control codes. Offsets 0x00-0x20 reserved for notification codes.
 */
#define ITE_DIAL			ASYNASGN + ITE_ON_CONN	/* Make a connection	*/
#define ITE_RCVR_ASGN		ASYNASGN + ITE_ON_INCALL/* Receiver assignment	*/

#define ITE_HANGUP			ASYNREM  + ITE_ON_CONN	/* Hangup a call		*/
#define ITE_RCVR_REM		ASYNREM  + ITE_ON_INCALL/* Remove incall notify	*/

#define ITE_NETWK_TYPE		CALLCTL + 0x20	/* Find network device type		*/
#define ITE_SET_CONN		CALLCTL + 0x21	/* Set connection information 	*/
#define ITE_CONN_STATUS		CALLCTL + 0x22	/* Call/connection status 		*/
#define ITE_ERR_STATUS		CALLCTL + 0x23	/* Check for async errors 		*/
#define ITE_ANSWER			CALLCTL + 0x24	/* Answer incoming call			*/
#define ITE_SUSPEND			CALLCTL + 0x25	/* Suspend a session			*/
#define ITE_RESUME			CALLCTL + 0x26	/* Resume a suspended session	*/
#define ITE_LINKDOWN		CALLCTL + 0x27	/* Tell upper driver link is dwn*/
#define ITE_LINKUP			CALLCTL + 0x28	/* Tell upper driver link is up */
#define ITE_SETADDR			CALLCTL + 0x29	/* Set new address pointer (IP)	*/
#define ITE_GETADDR			CALLCTL + 0x29	/* Get new address pointer (IP)	*/
#define ITE_PROFILE			CALLCTL + 0x30	/* Set/get protocol's profile	*/

/* Program oriented codes. Offsets 0x00-0x20 reserved for notification codes.
 */
#define ITE_ENDPGM_ASGN		ASYNASGN + ITE_ON_ENDPGM	/* End of program	*/

#define ITE_ENDPGM_REM		ASYNREM + ITE_ON_ENDPGM	/* Remove end of pgm	*/

/* Mbuf oriented codes. Offsets 0x00-0x20 reserved for notification codes.  */
#define ITE_RD_MBUF		PGMCTL + 0x20	/* Read mbuf packet chain 	*/
#define ITE_WR_MBUF		PGMCTL + 0x21	/* Write using mbuf chains	*/
#define ITE_ABT_PGM		PGMCTL + 0x22	/* Abort Program call 		*/
#define ITE_GET_PSI		PGMCTL + 0x23	/* Get PSI information		*/
#define ITE_GET_PGMINFO	PGMCTL + 0x24	/* Get program information	*/
#define ITE_PLAY_PGM	PGMCTL + 0x25	/* Set MPEG-2 program streams	*/
#define ITE_ABT_PSI		PGMCTL + 0x26	/* Remove Get PSI notification	*/
#define ITE_ABT_PGMINFO PGMCTL + 0x27	/* Remove program info assign   */
#define ITE_PGM_PREF	PGMCTL + 0x28	/* Get/Set preferences          */
#define ITE_CHANGE_PSI	PGMCTL + 0x29	/* Change a PSI table mask      */
#define ITE_FLUSH_PAT	PGMCTL + 0x2a	/* Flush the cached PAT         */
#define ITE_MB_ANCHOR	PGMCTL + 0x2b	/* Pass mbuf anchor pointer     */
#define	ITE_PSI_NOTIFY_ASGN	PGMCTL + 0x2c	/* Register a notification for PSI data */
#define	ITE_PSI_NOTIFY_REM	PGMCTL + 0x2d	/* Remove a PSI notification registration */
#define	ITE_PSI_NOTIFY_SEND	PGMCTL + 0x2e	/* Send a PSI notification */

/* Stream oriented codes. Offsets 0x00-0x20 reserved for notification codes.*/
#define ITE_ABT_STREAM	STRMCTL + 0x20	/* Abort stream 				*/
#define ITE_RD_STREAM	STRMCTL + 0x21	/* Read using SCBs				*/

/* User to Network messaging codes. Offsets 0x00-0x20 reserved for
 * notification codes. This code goes in the Spf_ss_pb.code field. See
 * the ite_un_pb structure below for definitions of the primitives to
 * go with this field.
 */
#define ITE_RESINFO_GET		UN_MSG + 0x01	/* Get resource information		*/
#define ITE_RESOURCE_LIST	UN_MSG + 0x02	/* List resources for a session	*/
#define ITE_JOIN			UN_MSG + 0x03	/* Join a session				*/
#define ITE_UMSG_SEND		UN_MSG + 0x04	/* Unconfirmed message send		*/
#define ITE_CANCEL			UN_MSG + 0x05	/* Cancel resource request		*/
#define ITE_IBRESCHG_ASGN	UN_MSG + 0x06	/* Notify on in-band res change	*/
#define ITE_IBRESCHG_REM	UN_MSG + 0x07	/* Remove in-band res notify	*/
#define ITE_UN_CMD			UN_MSG + 0x20	/* Generic User/net cmd			*/
#define ITE_STATUS			UN_MSG + 0x21	/* Session Status				*/
#define ITE_IBRES_CFG		UN_MSG + 0x22	/* Configure In-band path		*/

#define ITE_RESOURCE_ADD	ASYNASGN + ITE_ON_RESADD /* Add res to session	*/
#define ITE_RESOURCE_DEL	ASYNREM  + ITE_ON_RESADD /* Del res from session*/
#define ITE_MSG_SEND		ASYNASGN + ITE_ON_MSGCONF	/* Confirmed msg	*/

/* Other generic asynchronous notification codes */
#define ITE_DAVAIL_ASGN		ASYNASGN + ITE_ON_DATAVAIL	/* Data available	*/
#define ITE_LINKDWN_ASGN	ASYNASGN + ITE_ON_LINKDOWN	/* Link down		*/
#define ITE_LINKUP_ASGN		ASYNASGN + ITE_ON_LINKUP	/* Link up			*/
#define ITE_FEHANGUP_ASGN	ASYNASGN + ITE_ON_FEHANGUP	/* Far and hangup	*/
#define ITE_FCTLON_ASGN		ASYNASGN + ITE_ON_FCTLON	/* Flow control on	*/
#define ITE_FCTLOFF_ASGN	ASYNASGN + ITE_ON_FCTLOFF	/* Flow control off	*/
#define ITE_NOTIFY_ASGN		ASYNASGN + 0x20	/* Generic notify assignment	*/

/* Other generic asynchronous notification removal codes */
#define ITE_DAVAIL_REM		ASYNREM  + ITE_ON_DATAVAIL	/* Data available	*/
#define ITE_LINKDWN_REM		ASYNREM  + ITE_ON_LINKDOWN	/* Link down		*/
#define ITE_LINKUP_REM		ASYNREM  + ITE_ON_LINKUP	/* Link up			*/
#define ITE_FEHANGUP_REM	ASYNREM  + ITE_ON_FEHANGUP	/* Far and hangup	*/
#define ITE_FCTLON_REM		ASYNREM  + ITE_ON_FCTLON	/* Flow control on	*/
#define ITE_FCTLOFF_REM		ASYNREM  + ITE_ON_FCTLOFF	/* Flow control off	*/
#define ITE_NOTIFY_REM		ASYNREM  + 0x21	/* Generic notify removal		*/

/* Socket control getstat and setstat calls */
#define ITE_BOOTPMODE		SOCKETCTL + 0x01	/* Set/clear Bootp mode		*/

/* Timer control getstat and setstat calls. These are low level
 * calls without bindings that can be used to test the timer service
 * in SPF.
 */
#define ITE_TIMER_START		TIMERCTL + 0x22
#define ITE_TIMER_RESTART	TIMERCTL + 0x23
#define ITE_TIMER_STOP		TIMERCTL + 0x24

/* Test codes. Offsets 0x00-0x20 reserved for notification codes. */
#define ITE_TEST_SENDNOTIFY		TESTCTL + 0x20	/* Send a test notification	*/
#define ITE_TEST_SETCALLSTATE	TESTCTL + 0x21	/* Set the call state		*/
#define ITE_TEST_SENDUP			TESTCTL + 0x22	/* Send up an mbuf			*/
#define ITE_TEST_LULIST			TESTCTL + 0x23	/* Get lulist from driver	*/

/* These codes help support the rtnfm.l compatable library */
#define ITE_NETSTATUS			TESTCTL + 0x80	/* Get network statistics	*/
#define ITE_READOPTS			TESTCTL + 0x81	/* Read options (server)	*/
#define ITE_STB_MODE			TESTCTL + 0x82	/* Get set top mode(server)	*/

/* Getstat/setstat sub-parameter blocks that are used for ITEM _os_setstat()
 * and _os_getstat() calls.
 */

/* Generic ITEM list structure element so ITEM can traverse generic
 * lists.
 */
typedef struct ite_list {
	void	*next;
} ite_list, *Ite_list;

/* ITEM's device/path/call_control parameter block */
typedef struct ite_conn_pb {
	spf_ss_pb	spb;
	void		*cctl_pb;		/* Pointer to "Call control param blk"		*/
	Notify_type	notify_pb;		/* Pointer to user notification structure	*/
	Addr_type	our_num,
				their_num;
} ite_conn_pb, *Ite_conn_pb;

/* ITEM download control parameter block */
typedef struct ite_dwnld_pb {
	spf_ss_pb	spb;			/* Valid mod_code values are:	*/
		#define ITE_DWNLD_GO	DWNLDCTL + 0x01
		#define ITE_DWNLD_VAL	DWNLDCTL + 0x02
	u_int16		pgm_id,			/* MPEG-2 program number */
				rsv1;
	void		*buffer;
	u_int32		size;
	Notify_type	notify_pb;
} ite_dwnld_pb, *Ite_dwnld_pb;

/* ITEM profile parameter block */
typedef struct ite_profile_pb {
	spf_ss_pb	spb;		/* pb->param has the first parameter */
	void		*buffer;
	u_int32		size;
} ite_profile_pb, *Ite_profile_pb;

/* ITEM Resource (or in-band) parameter block */
typedef struct ite_rescfg_pb {
	spf_ss_pb	spb;
	u_int32		element_size;
} ite_rescfg_pb, *Ite_rescfg_pb;

/* ITEM readfrom parameter block */
typedef struct ite_rf_pb {
	i_read_pb	rpb;			/* Read pb: contains buffer and size	*/
	u_int32		flags;			/* Flags: defined in BSD/sys/socket.h	*/
	u_int32		*addr_len;		/* Pointer to length of address			*/
	u_char		*addr;			/* Pointer to receive from address		*/
	u_int32		spf_flags;		/* Flags used for SPF					*/
		#define SPF_READ		1	/* It's _os_read or ite_data_read	*/
		#define SPF_RECV		2	/* It's socket recv() function		*/
		#define SPF_RECVFROM	3	/* It's socket recvfrom() function	*/
} ite_rf_pb, *Ite_rf_pb;

/* ITEM sendto parameter block */
typedef struct ite_st_pb {
	i_write_pb	wpb;			/* Write pb: contains buffer and size	*/
	u_int32		flags;			/* Flags: defined in BSD/sys/socket.h	*/
	u_int32		*addr_len;		/* Pointer to length of address			*/
	u_char		*addr;			/* Pointer to receive from address		*/
	u_int32		spf_flags;		/* Flags used for SPF					*/
		#define SPF_WRITE		1	/* It's _os_write or ite_data_write	*/
		#define SPF_SEND		2	/* It's socket send() function		*/
		#define SPF_SENDTO		3	/* It's socket sendto() function	*/
} ite_st_pb, *Ite_st_pb;

/* User to network messaging structure. The session manager uses this
 * for primitive passing between itself and the session manager protocol
 * driver below.
 */
typedef struct ite_un_pb {
	spf_ss_pb	spb;
	u_int32		un_primitive;				/* The User/network primitive	*/
		#define ITE_UN_RESOURCEADDRESP	 0x4033	/* Add resource request	*/
		#define ITE_UN_RESOURCEDELRESP	 0x4043	/* Delete resource		*/
		#define ITE_UN_RELEASERESP		 0x4023	/* Release a session	*/
		#define ITE_UN_STATUSREQUEST	 0x4060	/* Obtain status		*/
		#define ITE_UN_STATUSRESP		 0x4063	/* Obtain status		*/
		#define ITE_UN_MSGINVOKE		 0xB050	/* Confirmed msg send	*/
		#define ITE_UN_MSGRESP			 0xB053	/* Confirmed msg send	*/
		#define ITE_UN_UMSGINVOKE		 0x4050	/* Unconfirmed msg send	*/
	u_int32		un_session;					/* Session number			*/
	void		*un_params;					/* Ptr to other parameters	*/
	u_int32		un_rsv1[2];					/* Reserved for future use	*/
	u_char		*un_buffer;					/* Message data				*/
	u_int32		un_count;					/* Size of message data		*/
	Notify_type	un_notify_pb;			/* Notification parameter block	*/
} ite_un_pb, *Ite_un_pb;

#endif /* _ITEM_PVT */
