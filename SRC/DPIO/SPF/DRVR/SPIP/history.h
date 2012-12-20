/****************************************************************************
 *           Copyright 1989-2001 by Microware Systems Corporation           *
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
 *  01 ??/??/?? Created
 *  04 11/21/96 spip not getting packet chain size correct             rds
 *  05 12/22/96 Add next hop addr and bcast flag for new spenet        rds
 *  06 01/07/97 Routing and interface code added                       rds
 *  07 01/08/97 Added/Fixed interface info from inetdb                 tmd
 *  08 01/08/97 Zero loopback address structures                       rds
 *  09 01/14/97 Added ip_forwarding code                               rds
 *  10 01/15/97 Changed to use pull from pull2.c                       tmd
 *              Removed code that was duplicated in get_ints.c         rds
 *              and changed to use new SPF defines (e.g. SPF_SS_ATTIF) tmd
 *  11 01/16/97 Changed from pulling (pull2.c) to opening interfaces   rds
 *  12 01/17/97 Changed to set IP_ALLOWBROADCAST flag if UDP           tmd
 *     01/17/97 Added SPF_SS_DELADDR                                   rds
 *     01/17/97 ifdef'd spin calls                                     rds
 *  13 01/19/97 Changed to ignore SPF_SS_SETADDR downdriver errors     tmd
 *  14 01/20/97 Getting downdriver from pd instead of SPF_GS_DEVENTRY  tmd
 *  15 02/01/97 Setting ipforwarding based on GATEWAY flag in ip0      rds
 *  16 02/01/97 ifdef'd out calls to splnet and splimp                 rds
 *  17 02/03/97 Added support to print routing table                   rds
 *  18 02/09/97 Cleaned up txsize and txoffset code                    rds
 *              Add route to loopback in add_ip_address                rds
 *  19 02/10/97 Added code to add routes stored in inetdb (get_ints.c) tmd
 *  20 02/12/97 Keep track of max txoffset to use in SPF_SS_UPDATE     rds
 *  21 02/13/97 Return MTU along with local address in SPF_GS_SRCADDR  rds
 *  22 02/15/97 Added generic ioctl for ifconfig                       tmd
 *  23 02/16/97 Renamed spfdefs.h to defs.h and cleaned up code        rds
 *              Changed to single per path storage structure           rds
 *  24 02/17/97 Changed to add interface BEFORE push so PPP sees it    tmd
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *  25 02/26/97 Got rid of _modhead for x86                            tmd
 *     02/27/97 Fixed sa_len in get_ints.c                             tmd
 *     02/28/97 change to convert loopback address to network order    tmd
 *     03/03/97 Moved ntoh, hton, etc to endian.h & use netdb.l        tmd
 *  26 03/13/97 Fixed offset problem in get_fragbuf                    rds
 *  27 03/14/97 Merged ip_tmr into spip                                rds
 *  28 03/19/97 Fixed IP reassembly (mbuf's freed twice)               rds
 *              m_pullup calls checking wrong return codes             rds
 *  29 04/02/97 Work around assembly optimizer bug in dr_setstat       rds
 *  30 04/15/97 changed ifunit to avoid stomping on name (bad if ROM)  tmd
 *  31 04/30/97 Moved error codes to sys/errno.h                       rds
 *  32 05/04/97 Rewrote add_ip_address and del_ip_address              rds
 *              Stop incrementing IP length in dr_updata               rds
 *              Change SPF_SS_SETADDR and DELADDR to ifioctl calls     rds
 *              Change how loopback routes are handled for interfaces  rds
 *  33 05/10/97 Added SIOCSIFMTU to ifioctl                            rds
 *  34 05/15/97 Remove pathdesc check in dr_downdata                   rds
 *     05/15/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  35 06/05/97 Clean up unused/uneeded code                           rds
 *  36 06/12/97 Added SPF_GS_SYMBOLS for netstat support               tmd
 *  37 06/20/97 Added ifnet, rtstat & rt_tables symbols for netstat    tmd
 *  38 07/16/97 Added N_ABS to nlist structure in bsd_vars.c           tmd
 *     07/22/97 Get stats pointer from lower driver, and add to ifnet  tmd
 *  39 09/18/97 Major changes for new protocol and domain support      rds
 *  40 09/19/97 Added change_static to SOCKET_GS_TYPE in dr_getstat    tmd
 *              Added SNMP_GET_DATA and SNMP_PUT_DATA so netstat can   tmd
 *              run in user state.                                     tmd
 *  41 09/22/97 Added icmpstat to nlist symbols                        tmd
 *  42 09/23/97 Added BSDCALL to soshutdown in uipc_socket.c           rds
 *  43 09/24/97 Merged in FreeBSD MULTICAST changes (in.c)             tmd
 *     09/30/97 Added routing domain functions                         rds
 *     10/05/97 Check is_super() before creating raw sockets           rds
 *     10/10/97 Fix memory leak in route.c (free(old) and free(ifa))   rds
 *     10/14/97 Check if reassembled size is bigger than IP_MAXPACKET  rds
 *  44 10/16/97 Fix memory leak in accept (uipc_syscalls.c)            rds
 *  45 10/31/97 Moved get symbol stuff to if.c to also get link addr   tmd
 *  46 11/25/97 in_control checks BOTH src & dest for 0 on PTP links   tmd
 *  47 11/05/97 Added sysctl_int, copyin, and copyout                  rds
 *     11/06/97 Added net_sysctl and ip_sysctl                         rds
 *     11/09/97 Correctly set mb_len in sogetopt (SO_LINGER)           rds
 *     11/17/97 Fixed m_len_p problem after m_pullup in mk_pkt_hdr     rds
 *     11/21/97 added pr_statics for inetsw[0], removed temp ip_sysctl rds
 *     11/21/97 Set if_output to ipdev so dr_updata works for loopback rds
 *     11/22/97 Set RTF_STATIC when adding loopback route              rds
 *     11/26/97 Remove check for PKTHDR in sbappendaddr                rds
 *     11/26/97 Check IFF_UP flag, support routing domain sockets      rds
 *  48 11/28/97 Added syscopy to ip_funcs                              rds
 *              Change copyin and copyout to macros in spsocket.h      rds
 *              Replaced _os_findpd and _os_id with _os_getsys         rds
 *              Getsockopt and setsockopt are now in uipc_syscalls.c   rds
 *  49 12/02/97 Replace ADD_ENTRY which was corrupting address 0x4     rds
 *  50 12/12/97 added SIOCGIFMTU to ifioctl in if.c                    tmd
 *  51 01/07/98 Remove bld_mbuf, MWINTERFACE defines, and nop's        rds
 *              Replaced _os_id with _os_getsys in is_super()          rds
 *              Changed temp ctloutput functions to 0 in in_proto.c    rds
 *              Call ctloutput function and free mbuf in sosetopt()    rds
 *  52 01/08/98 Fixed problem in in_socktrim() with class A networks   rds
 *  53 01/15/98 len was not being initialized in in rt_output()        rds
 *              don't call routing domain in rt_output() if error      rds
 *              Check if MALLOC failed in socreate()                   rds
 *              Map INADDR_BROADCAST to first broadcastable interface  rds
 *              Added support for receiving all 1's broadcast          rds
 *              Change syscopy to return EFAULT if permission denied   rds
 *  54 01/21/98 Replaced in_socktrim() with FreeBSD routed version     rds
 *  55 02/03/98 Removed INET/netdb.h and INET/netdblib.h               rds
 *              Removed compiler warnings                              rds
 *  56 02/06/98 Check so->so_pd before setting socket_ptr(uipc_socket) rds
 *  57 02/14/98 Changed sogetopt to pass PRCO_GETOPT to pr_ctloutput   rds
 *              Fixed sock_sleep call if SO_LINGER is set in soclose   rds
 *  58 02/21/98 Clear si_code in sock_sleep, Remove (ifdef) selwakeup  rds
 *              Added is_super() check for SPF_SS_ROUTE                rds
 *              Removed pullup (mk_pkt_hdr, get_fragbuf, do_downdata)  rds
 *              Removed SO_RCVTIMEO and SO_SNDTIMEO from sogetopt      rds
 *              Set si_pid before calling sock_sleep in sbwait         rds
 *  59 03/18/98 Fixed so_linger with timeout of 0 (uipc_socket.c)      rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  60 04/21/98 Removed useless SPF_GS_DEVENTRY (done by SPF)          tmd
 *              Changed order of _asm labels (compiler bug MHQco03955) tmd
 *  61 05/22/98 Copy data from inetdb to local variables (get_ints.c)  rds
 *     05/28/98 Make netmask lengths consistant (MHQco0402)            rds
 *  62 06/10/98 Fixed listen backlog (MHQco04207 and MHQco04079)       rds
 *              Fixed incorrect if_ibytes count (DDTS MHQco04216)      tmd
 *  63 09/24/98 Added assembly version of in_cksum() for ARM targets   rds
 *  64 10/19/98 Fixed accept() to correctly set the foreign port/addr  rds
 *  65 10/30/98 Prevent needless pullups when fragmenting (ip_output)  rds
 *  66 11/04/98 Return any errors received from net_sysctl (entry.c)   rds
 *  67 11/17/98 Added newaddr to in_ifinit parameters (in.c)           rds
 *  68 12/04/98 Merged changes from lancomm3.3 branch                  rds
 *     07/10/98 Changed IP timer to 5 seconds and stop when not needed rds
 *     09/28/98 Added multicast support                                rds
 *     10/23/98 Added igmpstat to ip_symbols (bsd_vars.c, igmp.c)      rds
 *     10/28/98 Fixed multicast fragmentation problem (ip_output.c)    rds
 *     11/23/98 Stop IGMP timers when no longer needed (igmp.c)        rds
 *  69 12/08/98 Changed _os_setstat() to SMCALL_SS() in in.c           rds
 *  70 12/29/98 FreeBSD patch for CERT Advisory CA-98.13 (ip_input.c)  rds
 *  71 01/19/99 Set interface flags based on stk_int_flags (get_ints)  rds
 *  72 02/05/99 Zero spf_update_pb before SPF_GS_UPDATE (get_ints.c)   rds
 *  73 03/02/99 Added assembly version of in_cksum() for PPC targets   ajk
 *  74 04/09/99 In in_pcbnotify, changed notify to notify_item to      sr
 *              avoid name conflicts. (in_pcb.c) 
 *              In igmp_input, changed timer to igmp_timer to avoid
 *              name conflicts with defines in rom.h. (igmp.c)
 *              Added support to check and add ip address from the
 *              the low level boot system. (get_ints.c)
 *              Added parameter to add_ip_interface. (entry.c, proto.h)
 *  75 04/21/99 Cleaned so that it compiles for 68K.  get_sysglobs     sr
 *              function moved to p2lib.l (get_ints.c)
 *  76 04/22/99 Changed timer functions to file manager callups        rds
 *     04/26/99 Only call _os_getsys if required (uipc_socket2.c)      rds
 *     04/26/99 Always make max_linkhdr multiple of 4 bytes (get_ints) rds
 *     04/28/99 Removed _OSK DPIO ifdefs no longer needed (bsd_subr.c) rds
 *  77 05/11/99 In check_llip_address, check for either PVR_BOOTDEV    sr
 *              or PVR_DRIVER not necessarily both. (get_ints.c)
 *  78 06/24/99 Added sbwait and sock_sleep to ip_symbols (bsd_vars.c) rds
 *  79 10/13/99 Added getstats for getsockname/getpeername (cf6564)    rds
 *  80 06/12/00 Add BSDCALL around call to PRU_ATTACH in sonewconn1()  rds
 *     07/27/00 Added support for bypassing SPF when reading sockets   rds
 *  81 08/22/00 Removed ICMP mbuf allocation when forwarding packets   rds
 *              Added in_cksum_hdr routine to checksum IP headers      rds
 *  82 09/27/00 Always get current p_id before sleeping (bsd_subr.c)   rds
 *              Changed swap_globals to change_static (get_ints.c)     rds
 *  83 12/07/00 Merged lan_altvater branch (allow interface removal)   rds
 *  84 01/21/01 Fixed problem in ARM in_cksum_hdr (in_cksum.c)         rds
 *  85 02/04/01 Added support for IP_SS_RTMSG setstat                  rds
 *  86 02/07/01 Fixed mbuf corruption in sbdrop (cf10248)              rds
 *  87 02/13/01 No code change - linked with new socketvar.h/tcp_var.h rds
 *  88 02/22/01 Added ioman resource locking                           rds
 *  89 05/10/01 No code changes, recompiled with a fixed MGETHDR_N     rds
 *  90 06/08/01 CF4151 - Added more check for low-level IP address     mdu
 *              use in get_ints.c
 *  91 06/22/01 Added check to see whether we can get the subnet mask  mdu
 *              from bootp packet
 *  92 06/23/01 Fixed potential mbuf leak in m_dup (bsd_subr.c)        rds
 *  93 07/06/01 Improved parameter checking in sysctl_int (cf11810)    rds
 *  94 08/03/01 Block socket close when another thread is blocked in   gjb
 *              in a read or write.
 *  95 10/27/01 CF12565 - Added some checks in in_cksum_hdr() for      mdu
 *              non PPC/ARM code in in_cksum.c
 *9595 01/24/02 Fixed memory leak in accept (uipc_syscalls.c)(cf12944) rds
 ****************************************************************************/

/*
**	Edition/Revision Numbers
*/
#ifdef SYSEDIT

_asm("_sysedit: equ 9595");
_asm("_sysattr: equ 0xA000");

#endif /* SYSEDIT */

/* descriptor edition number */
#ifdef DESC_SYSEDIT

_asm("_sysedit: equ 4");
_asm("_sysattr: equ 0x8000");

#endif /* DESC_SYSEDIT */
