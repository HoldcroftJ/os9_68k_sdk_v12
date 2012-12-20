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
 *  01 04/22/96 Initial Version                                        billw
 *  16 03/26/97 Changed to use BSD header files. (entry.c, ipcp.c,     sr
 *              defs.h, compress.c, compress.h)
 *  17 04/18/97 Modified to allow for PPP to specify default route     sr
 *              (defs.h, entry.c, ipcp.c, proto.h)
 *  18 04/24/97 Furthur code optimization.                             sr
 *  19 04/28/97 Modified upd_ip_info. Modified ss_pop/layer_down to    sr
 *              indicate to ip that it can delete entries for the
 *              address from the routing table. (entry.c, ipcp.c,
 *              inetd_if.c)
 *  20 04/30/97 if set_default route returns an error of EEXIST,       sr
 *              turn off default route flag, so we do not delete it
 *              during shutdown.  Also delete the default route in
 *              ss_pop/layer_down, if need be. (entry.c, ipcp.c,
 *              defs.h, proto.h)
 *  21 05/09/97 Created ss_update. (entry.c, ipcp.c)                   sr
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  22 09/25/97 Added call to handle_event EVENT_DOWN on close.        tmd
 *              Made calls using _os_sleep save lustat & pathdesc.     tmd
 *              Make SMCALL restore pathdesc.                          tmd
 *  23 10/06/97 Adopted changes Bill W. made to Beta PPP code          tmd
 *  24 10/10/97 Update def_route field in ss_ipcp_params and           sr
 *              gs_ipcp_params. (entry.c)
 *              Moved default route defines to ppp.h. (defs.h)
 *     10/17/97 Default PPP_MODE is Active Open. (defs.h)              sr
 *  25 10/28/97 In pr_cfg_rej, handle NAK and REJECT packets           sr
 *              differently. (ipcp.c)
 *  25 11/04/97 Added BSD statistics and SPF_GS_SYMBOLS getstat        tmd
 *  26 11/28/97 Made to compile without warnings.                      tmd
 *              Added ipcp_accept_remote & ipcp_accept_local flags.    tmd
 *              Added fields to save original IP addresses.            tmd
 *              Fixed to avoid infinite loops during negotiation       tmd
 *  27 12/11/97 Added check for PD_SYSTATE to skip check_access        tmd
 *  28 01/19/98 Fixed debug version of compiler warnings               tmd
 *  29 02/16/98 Added sys/ioctl.h header in defs.h to get bsdioctl     tmd
 *              Stopped header files from using INET directory         tmd
 *  30 03/02/98 Added do_ip_setstat function to fix problem that       tmd
 *              the pathdesc may not be set properly.                  tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  31 04/24/98 Stopped IP_ADDRESSES from sending wrong length         tmd
 *  32 01/16/99 Added unique event name creation from lun              mgh
 *  33 01/22/99 Set stk_int_flags, added multicast support (entry.c)   rds
 *  34 10/19/00 In dr_downdata call SMCALL_DNDATA directly. (entry.c)  sr    *
 *  35 11/08/00 Use _os9_id call instead of _os_id for OS9. (entry.c)  sr    *
 ****************************************************************************/

#ifndef _history_h_
#define _history_h_

/* Edition/Revision Numbers */

#ifdef SYSEDIT

_asm("_sysedit: equ 35");
_asm("_sysattr: equ 0xA000");

#endif /* SYSEDIT */

#ifdef DESC_SYSEDIT

_asm("_sysedit: equ 26");
_asm("_sysattr: equ 0x8000");

#endif /* DESC_SYSEDIT */

#endif /* _history_h_ */
