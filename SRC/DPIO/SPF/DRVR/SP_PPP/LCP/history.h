/****************************************************************************
 *  ID:		%A%
 *  Date:	%G%
 ****************************************************************************
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
 *  01 01/23/96  Saw the light of day                                  udayn
 *  16 03/27/97 Changes to use BSD neaders. (inetd_if.c)               sr
 *  17 04/30/97 Clean up Code.                                         sr
 *  18 05/09/97 Take care of MTU/MRU in gs_update. (entry.c)           sr
 *     05/09/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  19 06/17/97 In hdl_config_request, check ppp_mode for NO_CHAP      sr
 *              if protocol is CHAP. (lcp.c)
 *  20 06/23/97 In hdl_config_request, verify length field in magic    sr
 *              number option.  In hdl_config_requet, handle CONFIG_NAK
 *              cases also. Do not alter options in case of reject.
 *              (lcp.c)
 *  21 09/24/97 Changed chap not to use NOFREE bit                     tmd
 *  22 09/30/97 Changed layer_finish to free last_rx_mbuf              tmd
 *  23 10/01/97
 *  24 10/02/97 Addopted changes from Bill W's PPP                     tmd
 *              Fixed memory leak in dr_term (attach count)            tmd
 *  25 10/06/97 Call add_notify to remove ourselves from the list      sr
 *              in dr_term.
 *     10/17/97 Default PPP_MODE is Active Open.                       sr
 *  26 10/28/97 In hdl_config_reject, handle NAK and REJECT packets    sr
 *              differently.  In send_config_request, send accm option
 *              only if rx_accm is non-zero. (lcp.c)
 *  27 11/28/97 Fixed hdl_config_request to do NAK on bad auth algo    tmd
 *              Changed to pass optional isp name to get_chap_info     tmd
 *              and to call auth_get_chap instead of a_g_curr_chap     tmd
 *              Changed to use peer name if can't find secret using    tmd
 *              current ISP name (ala pppauth -h) (chap.c)             tmd
 *              Changed send_msg_down to copy packet on nofree bit     tmd
 *              since lower layers may change it. (lcp.c)              tmd
 *              Added fields to lcp_lustat for authentication          tmd
 *              Added code to do CHAP challenge, but not using it yet  tmd
 *              Made to compile without warnings.                      tmd
 *  28 12/01/97 Fixed to send NAK for unknown authentication.  Also    tmd
 *              fixed probable endian bug when setting auth type.      tmd
 *  29 01/19/98 Fixed compiler warnings for debug version              tmd
 *  30 03/11/98 Fixed so auth_timeout is set in seconds not ms         tmd
 *              Init ncp in get_up_drvr to avoid iopt warning          tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  31 01/18/99 Change event name to be unique per descriptor from lun mgh
 *  32 10/20/00 In dr_downdata call SMCALL_DNDATA directly. (entry.c)  sr    *
 *  33 11/08/00 Use _os9_id call instead of _os_id for OS9. (entry.c)  sr    *
 *  34 02/06/01 Update the length field in hdl_config_request correctly sr   *
 *              during option processing. (lcp.c)                            *
 ****************************************************************************/

#ifndef _history_h_
#define _history_h_


/*
**	Edition/Revision Numbers
*/

#ifdef SYSEDIT

_asm("_sysedit: equ 34");
_asm("_sysattr: equ 0xA000");

#endif /* SYSEDIT */

#ifdef DESC_SYSEDIT

_asm("_sysedit: equ 30");
_asm("_sysattr: equ 0x8000");

#endif /* DESC_SYSEDIT */

#endif /* _history_h_ */
