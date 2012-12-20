/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1989-2001 by Microware Systems Corporation         |
|              Copyright 2001 by RadiSys Corporation                        |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, modification, distribution, or creation of    |
|  derivative works in any form to any party other than the Licensee is     |
|  strictly prohibited, unless expressly provided for under the terms of a  |
|  mutually executed written license agreement for this software between    |
|  the end-user and Microware Systems Corporation, or otherwise expressly   |
|  authorized in writing by Microware Systems Corporation.  Licensee shall  |
|  reproduce this copyright notice exactly as provided herein on any copies |
|  of the software that Licensee is authorized to distribute.               |
|                                                                           |
|  RadiSys is a registered trademark of RadiSys Corporation.                |
|---------------------------------------------------------------------------|
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *  01 ??/??/?? Creation                                               ???
 *  04 10/23/96 Added support code for setsockopt                      tmd
 *  05 11/13/96 ifdef'd out _os_sigmask calls                          rds
 *  06 12/12/96 Added MWINTERFACE code to support new style IP binding rds
 *              Added notification for connect and accept
 *  07 12/14/96 Check for connect state before setting lustat          rds
 *              Removed duplicate add_tcp_entry call
 *  08 12/16/96 Changed do_notify to set lu_pathdesc for signals       tmd
 *  09 12/18/96 Moved connect stuff from entry.c to soisconnected      tmd
 *              Also return EISCONN for NBIO if already connected      tmd
 *  10 12/20/96 Now setting LU_OURNUM in connect code                  rds
 *  11 01/03/97 Added more code for setsockopt, and did getsockopt     tmd
 *  12 01/06/97 Now setting LU_OURNUM in accept code                   rds
 *  13 01/10/97 Check socket state before setting callstatus in accept rds
 *  14 01/15/97 Using SPF_GS_SRCADDR to get source address             rds
 *  15 01/15/97 Returning error code for connect failures              rds
 *  16 01/17/97 ifdef'd spin calls                                     rds
 *              Changed SMCALL_SS to SMCALL_GS for SPF_GS_SRCADDR      rds
 *  17 01/31/97 Fixed DROP_ENTRY and remque macros                     rds
 *  18 01/31/97 Converted to DPIO lock functions                       rds
 *  19 01/31/97 Fixed ACK problem with SYN's (Stevens book 954-962)    rds
 *  20 02/02/97 ifdef'd out calls to splnet and splimp                 rds
 *  21 02/02/97 Now setting tcp_mssdflt from descriptor value          rds
 *  22 02/04/97 Flow control added                                     rds
 *  23 02/09/97 Fixed mbuf leak in do_downdata                         rds
 *  24 02/11/97 tcp_fasttimo was calling tcp_output twice              rds
 *              Fixed timestamp checking (Stevens book 870)            rds
 *              sorflush was clearing socket buffer lock descriptor    rds
 *              sock_sleep not checking for fatal signals              rds
 *              set lustat before calling tcp_output (tcp_usrreq)      rds
 *              set tcp_sendspace and tcp_recvspace from descriptor    rds
 *  25 02/15/97 use dr_max_offset instead of max_linkhdr               rds
 *              set mss based on MTU size of interface                 rds
 *  26 02/15/97 change defs.h to bsddefs.h                             rds
 *              remove socket buffer locks                             rds
 *  27 02/17/97 check for connection in flowoff                        rds
 *  28 02/21/97 check for no mbufs in tcp_output                       rds
 *     02/21/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *  29 03/04/97 Ported to X86                                          tmd
 *  30 03/18/97 Merged tcp_tmr into sptcp                              rds
 *  31 03/24/97 Added code for shutdown (ss_shut)                      tmd
 *              Added ss_gopt to dr_getstat.  This was mistakenly put  tmd
 *              in dr_setstat.  Leave it in dr_setstat until all apps  tmd
 *              can be linked with new socket lib.  This means apps    tmd
 *              using new socket.l won't work with old tcp.            tmd
 *  32 04/08/97 Set callstate in listen to support SPF edition 229     rds
 *  33 04/11/97 Allocate single mbuf instead of chain in tcp_output    rds
 *  34 04/14/97 Stop passing bind setstat to IP                        rds
 *              BSD sets local address if not binding to INADDR_ANY    rds
 *  35 04/16/97 tcp_output sometimes incorrectly sending FIN           rds
 *              Memory leak when accepting connections fixed           rds
 *  36 04/23/97 Removed STCP_* state variable                          rds
 *  37 04/29/97 Change MCLBYTES to 2048                                rds
 *  38 04/29/97 Fix endian problem in BSD code (tcp_input.c)           rds
 *  39 04/30/97 Moved error codes to sys/errno.h                       rds
 *  40 05/04/97 Stop decrementing IP length in dr_updata               rds
 *              Fix bus error when socreate fails                      rds
 *  41 05/05/97 Endian problem in tcp_input's dropwithreset code       rds
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  42 05/19/97 Fix m_pullup parameters in tcp_input.c                 rds
 *  43 05/30/97 Only set lu_pathdesc in do_updata and do_downdata      rds
 *  44 05/30/97 Fix nonblocking connect when TCP_RESET received        rds
 *     05/30/97 <--------- SPF LAN Comm Pack v3.1.1 Release -------->
 *  45 05/31/97 Numerous changes to signal and accept code             rds
 *              Clean up unused code                                   rds
 *  46 06/05/97 ifdef tcp_trace and other debug code                   rds
 *  47 06/09/97 Fix sogetopt (uipc_socket.c), bld_mbuf (entry.c)       rds
 *  48 06/11/97 Added assembly version of in_cksum for OS9 targets     rds
 *  49 06/16/97 Added SPF_GS_SYMBOLS for netstat support               tmd
 *  50 06/23/97 Merge per path storage into socket structure           rds
 *  51 07/16/97 Added N_ABS to nlist struct in bsd_vars.c              tmd
 *     07/22/97 Fixed all lustat->dndrvr to use dndrvr from sock stuct tmd
 *              Changed SPF_GS_SYMBOLS to pass to dndrvr               tmd
 *  52 08/24/97 Major changes for new protocol and domain support      rds
 *  53 10/15/97 Added call to in_losing (tcp_timer.c)                  rds
 *              Removed INIT_ENTRY in dr_iniz (entry.c)                rds
 *  54 10/16/97 Fix hold_on_close for non socket paths (entry.c)       rds
 *  55 11/06/97 Cleaned up after header change.  Don't include stat.h  tmd
 *  56 11/11/97 Set error_state when dropping connection or on error   rds
 *     11/14/97 Set error_state and callstate when RST received        rds
 *     11/17/97 RST fixes, and remove floating point code (entry.c)    rds
 *  57 12/02/97 Replaced ADD_ENTRY and DROP_ENTRY                      rds
 *  58 01/06/98 Fixed mbuf leak in tcp_ctloutput                       rds
 *  59 02/04/98 Added syscopy, removed compiler warnings               rds
 *              Add copyout calls for security in SPF_GS_SYMBOLS       rds
 *  60 02/14/98 Store template in regular memory instead of an mbuf    rds
 *  61 02/17/98 Fixed tcp_drop() NULL pointer problem (tcp_subr.c)     rds
 *              Check so_pd for NULL before setting pd values          rds
 *  62 02/21/98 tcp_ctloutput returns ENOPROTOOPT instead of E_UNKSVC  rds
 *  63 03/04/98 tcp_do_rfc1323 is now set using descriptor value       rds
 *  64 03/30/98 Fixed stack txsize so it is set correctly (entry.c)    rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  65 04/21/98 Changed order of _asm labels (compiler bug MHQco03955) tmd
 *  66 07/30/98 Set retransmit timer if tcp_output can't get an mbuf   rds
 *              <--------- SPF LAN Comm Pack v3.2.1 Release -------->
 *  67 10/28/98 Changed congestion window to open slower (MHQco04001)  rds
 *              <--------- SPF LAN Comm Pack v3.2.2 Release -------->
 *  68 07/02/98 Stop TCP timers when all sockets are closed (merged)   rds
 *              <--------- SPF LAN Comm Pack v3.3 Release ---------->
 *  69 04/29/99 Changed timers from functions to file manager callups  rds
 *  70 05/17/99 Receive user data directly, remove extra data copy     rds
 *              <--------- SPF LAN Comm Pack v3.3.1 Release -------->
 *  71 09/14/99 Change check for congestion window (tcp_input.c)       rds
 *     09/14/99 FreeBSD fix for pure ack checking (tcp_input.c)        rds
 *     09/14/99 Empty socket send buffer in tcp_drop (tcp_subr.c)      rds
 *  72 10/15/99 Fix packet storm in response to ICMP_UNREACH (cf4888)  rds
 *  73 12/14/99 Fix retransmission/Nagle problem (bsd_subr.c - cf7335) rds
 *  74 01/16/00 Fixed problem with sending a FIN too early             rds
 *  75 06/12/00 Added BSDCALL around call to sonewconn1                rds
 *     07/27/00 Added support for bypassing SPF when reading sockets   rds
 *  76 10/03/00 Removed _os_getsys to eliminate system call (bsd_subr) rds
 *  77 01/22/01 Change tcp_package to return count writen and ss_send  mgh
 *                to return SUCCESS for any partial writes
 *  78 02/07/01 Fixed memory corruption and mbuf leak (cf10248)        rds
 *  79 02/13/01 No code change - linked with new socketvar.h/tcp_var.h rds
 *  80 02/22/01 Added ioman resource locking                           rds
 *  81 03/17/01 Added call to ip_ctloutput (tcp_usrreq.c)              rds
 *  82 10/23/01 Added assembly checksum while copy for ARM             mab
 *  83 12/11/01 fix assembly for arm to not xao.                       gdw
 *  8301 03/18/02 Fixed tcp_package() to handle 0 byte writes.         dwj
 ****************************************************************************/

/*
**	Edition/Revision Numbers
*/
#ifdef SYSEDIT

_asm("_sysedit: equ 8301");
_asm("_sysattr: equ 0xA000");

#endif /* SYSEDIT */

/* descriptor edition number */
#ifdef DESC_SYSEDIT

_asm("_sysedit: equ 6");
_asm("_sysattr: equ 0x8000");

#endif /* DESC_SYSEDIT */
