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
 *  01 07/14/97 Origination.                                           dpt
 *  02 09/09/97 Updated to reflect changes in API.                     joelh
 *  03 10/10/97 Added ppp_option_block and get/set opts calls.         joelh
 *  04 09/18/98 Added sig_finish to conninfo struct to allow           kimk
 *              the application to determine ppp_connect() not
 *              established events.
 *  05 11/09/98 Changed library to use ppp_auth.h and the auth         kimk
 *              support routines in SP_PPP/COMMON/auth_mod.c
 *              rather than private duplicate versions in the alpha
 *              version of the ppplib code
 *  06 01/03/01 Added fields to ppp_option_block.                      sr    *
 ****************************************************************************/

#ifndef _PPPLIB_H_
#define _PPPLIB_H_


/*******************************************************************************
** INCLUDED FILES
*******************************************************************************/
#include <types.h>
#include <errno.h>
#include <module.h>

#include <SPF/ppp.h>
#include <SPF/ppp_auth.h>

/*******************************************************************************
** DEFINITIONS
*******************************************************************************/
#define PPP_CHAT_TYPE_NONE	(0x0)
#define PPP_CHAT_TYPE_MODULE (0x1)
#define PPP_CHAT_TYPE_FILE	(0x2)

/* these are defined in ppp_auth.h */
#define PPP_MAX_PEER_NAME	(MAX_PEER_NAME)
#define PPP_MAX_SECRET		(MAX_SECRET)
#define PPP_AUTH_MOD		AUTH_MOD
/* those were defined in ppp_auth.h */

#define PPP_MAX_NAME		(32)
#define PPP_NOLOG			0xffffffff	/* Value to turn off chat script logging... */

/* Bit definitions for layers... */
#define PPP_LAYER_IPCP		0x01
#define PPP_LAYER_LCP		0x02
#define PPP_LAYER_HDLC		0x04
#define PPP_LAYER_CHAT		0x08

/* Bit definitions for connection information flags... */
#define PPP_CIFLAG_CHATABORT 0x01


/*
** Error reporting structure used in the PPP connection
** information structure.
*/
typedef struct _ppp_error {
	u_int32 layer;

	union {
		struct {
			error_code err;
			u_int32 line;
			u_int32 abort_line;
		} chat;

		struct {
			u_int32 option;
			u_int32 my_request;
			u_int32 his_request;
		} lncp;
	} err_info;
} ppp_error, *Ppp_error;


/*
** Application allocated and passed in PPP connection
** information structure. This structure must be valid
** for the life of the PPP connection (until ppp_disconnect()
** is called).
*/
typedef struct _ppp_conninfo
{
	signal_code sig_lcp_up;
	signal_code sig_lcp_down;
	signal_code sig_ipcp_up;
	signal_code sig_ipcp_down;

	u_int32 flags;
	error_code last_err;
	u_int32 max_errors;
	Ppp_error error_array;

	signal_code	sig_lcp_finish;
	signal_code sig_ipcp_finish;
	u_int32 rsvd[4];
} ppp_conninfo, *Ppp_conninfo;


/*
** Block of the negotiated parameters for the three PPP drivers.
*/
typedef struct _ppp_param_block
{
	/* Generic stack parameters */
	u_int32		ppp_mode;

	/* IPCP-specific stack parameters */
	u_int32		rx_ip_cproto;
	u_int32		tx_ip_cproto;
	u_int32		rx_ipcp_cslot;
	u_int32		tx_ipcp_cslot;
	u_int32		rx_ipcp_mslot;
	u_int32		tx_ipcp_mslot;

	/* LCP-specific stack parameters */
	u_int32		rx_accm;
	u_int32		tx_accm[8];
	u_int32		rx_acfc;
	u_int32		tx_acfc;
	u_int32		rx_pfc;
	u_int32		tx_pfc;
	u_int32		rx_mru;
	u_int32		tx_mru;
	u_int32		local_magic;
	u_int32		remote_magic;

	/* I/O enabled flags */
	u_char		hdlc_io_enabled;
	u_char		lcp_io_enabled;
	u_char		ipcp_io_enabled;

	u_char		rsvd1;
	u_int32		rsvd2[3];
} ppp_param_block, *Ppp_param_block;


/*
** Definitions for PPP option priorities.
*/
#define PPP_OPT_NONE		0	/* Reserved for internal driver use */
#define PPP_OPT_DEFAULT		1	/* Use default or peer-specified value */
#define PPP_OPT_DESIRED		2	/* Attempt to negotiate desired value */
#define PPP_OPT_REQUIRED	3	/* Value MUST be negotiated or terminate link */


/*
** Option structures used in the ppp_option_block structure.
*/
struct pppopt_ui32
{
	u_int32 priority;
	u_int32 value;
};

struct pppopt_ui32_8
{
	u_int32 priority;
	u_int32 value[8];
};


/*
** Block of the configurable negotiation options for the three
** PPP drivers.
*/
typedef struct _ppp_option_block
{
	/* Generic stack settings */
	u_int32				ppp_mode;

	/* IPCP-specific stack options */
	u_int32				ipcp_timeout;
	u_int32				ipcp_max_configure;
	u_int32				ipcp_max_terminate;
	u_int32				ipcp_max_failure;
	struct pppopt_ui32	ipcp_accept_local;
	struct pppopt_ui32	ipcp_accept_remote;
	struct pppopt_ui32	rx_ip_cproto;
	struct pppopt_ui32	tx_ip_cproto;
	struct pppopt_ui32	rx_ipcp_cslot;
	struct pppopt_ui32	rx_ipcp_mslot;

	/* LCP-specific stack options */
	u_int32				lcp_timeout;
	u_int32				lcp_max_configure;
	u_int32				lcp_max_terminate;
	u_int32				lcp_max_failure;
	struct pppopt_ui32	rx_accm;
	struct pppopt_ui32_8 tx_accm;
	struct pppopt_ui32	rx_acfc;
	struct pppopt_ui32	rx_pfc;
	struct pppopt_ui32	rx_mru;
	struct pppopt_ui32	tx_mru;
	struct pppopt_ui32	auth_challenge;
	u_int32       ipcp_default_route;

	u_int32				rsvd[5];
} ppp_option_block, *Ppp_option_block;


/*
** PPP statistics
*/
typedef struct _ppp_hdlc_stats
{
	/* Receive statistics */
	u_int32 rx_bytes;
	u_int32 rx_frames;
	u_int32 rx_frames_compressed;
	u_int32 rx_frames_dropped;
	u_int32 rx_frames_overrun;
	u_int32 rx_errors;
	u_int32 rx_fcs_errors;

	/* Transmit statistics */
	u_int32 tx_bytes;
	u_int32 tx_frames;
	u_int32 tx_frames_compressed;
	u_int32 tx_frames_dropped;
	u_int32 tx_frames_overrun;
	u_int32 tx_errors;

	u_int32 rsvd[3];
} ppp_hdlc_stats, *Ppp_hdlc_stats;

/* Fake out the lcp/ipcp stats for now... */
typedef struct ppp_lcp_stats ppp_lcp_stats;
typedef struct ppp_lcp_stats *Ppp_lcp_stats;
typedef struct ppp_ipcp_stats ppp_ipcp_stats;
typedef struct ppp_ipcp_stats *Ppp_ipcp_stats;


/*
** PPP-specific error codes.
*/
#define EOS_PPP_BASE		(EOS_ENETBASE + 0xa0)
#define EOS_PPP_AUTHFAIL	(EOS_PPP_BASE + 0x00)
#define EOS_PPP_LCP_CONFIG	(EOS_PPP_BASE + 0x01)
#define EOS_PPP_IPCP_CONFIG	(EOS_PPP_BASE + 0x02)
#define EOS_PPP_CHAT_ABORT	(EOS_PPP_BASE + 0x03)
#define EOS_PPP_CHAT_BADSTR	(EOS_PPP_BASE + 0x04)
#define EOS_PPP_CHAT_APPABORT (EOS_PPP_BASE + 0x05)


/*******************************************************************************
** PPP API FUNCTION PROTOTYPES
******************************************************************************/

/* General init/term open/close functions... */
error_code ppp_init(void *rsvd);
error_code ppp_term(void *rsvd);
error_code ppp_open(char *stack_name, path_id *stack_path);
error_code ppp_close(path_id stack_path);

/* Get/Set functions... */
error_code ppp_get_params(path_id stack_path, ppp_param_block *params);
error_code ppp_get_asynch_params(path_id stack_path, ppp_modem_p *params);
error_code ppp_get_options(path_id stack_path, ppp_option_block *options);
error_code ppp_set_options(path_id stack_path, ppp_option_block *options);
error_code ppp_set_asynch_params(path_id stack_path, ppp_modem_p *params);
error_code ppp_get_statistics(path_id stack_path, ppp_ipcp_stats *ipcp_status,
                                                  ppp_lcp_stats  *lcp_status,
                                                  ppp_hdlc_stats *hdlc_status);
error_code ppp_reset_statistics(path_id stack_path, u_int16 layers);

/* Connect and disconnect functions */
error_code ppp_start(path_id stack_path, ppp_conninfo *connection_info);
error_code ppp_connect(path_id stack_path, path_id log_path, char *hdlc_name, 
                       u_int8 chat_type, char *chat_name, ppp_conninfo *connection_info);
error_code ppp_disconnect(path_id stack_path);

/* Authentication functions */
error_code ppp_auth_create_mod(u_int16 max_entries, auth_handle *hndl);
error_code ppp_auth_link_mod(auth_handle *hndl);
error_code ppp_auth_unlink_mod(auth_handle *hndl);
error_code ppp_auth_get_cur_chap(char *name, char *secret, auth_handle *hndl);
error_code ppp_auth_get_cur_pap(char *name, char *secret, auth_handle *hndl);
error_code ppp_auth_get_peer_name(char *name, auth_handle *hndl);
error_code ppp_auth_set_peer_name(char *name, auth_handle *hndl);
error_code ppp_auth_add_chap(char *peer_name, char *id, char *secret,
                              auth_handle *hndl);
error_code ppp_auth_add_pap(char *peer_name, char *id, char *secret,
                             auth_handle *hndl);
error_code ppp_auth_del_chap(char *peer_name, auth_handle *hndl);
error_code ppp_auth_del_pap(char *peer_name, auth_handle *hndl);

/* CHAT functions */
error_code ppp_chat_open(char *stack_name, path_id *chat_path);
error_code ppp_chat_close(path_id chat_path);
error_code ppp_chat_write(path_id chat_path, void *buffer, u_int32 *count);
error_code ppp_chat_read(path_id chat_path, void *buffer, u_int32 *count);
error_code ppp_chat_script(path_id chat_path, path_id log_path, u_int8 chat_type,
                       char *chat_name, ppp_conninfo *ci);


#endif /* _PPPLIB_H_ */

