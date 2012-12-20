/****************************************************************************
 *           Copyright 1989-1997 by Microware Systems Corporation           *
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
 *  05 06/25/96 fixed ARP reply for not ours
 *  06 06/28/96 fixed m_size to be the ip packet size
 *  07 08/29/96 work for both socket and item call
 *     09/12/96 added _m_share in main.c
 *  08 10/11/96 Changed Protocol address fields in arp_msg struct       sr
 *              to u_int8 to avoid alignment/padding by complier
 *              for ppc. (arp.h)
 *              Updated arp_msg struct in arp.h to resolve alignment
 *              /padding issues on ppc.  Associated code changes.
 *              (arp.c)
 *  09 10/16/96 Fixed casting issues associated with changes in       sr/tmd
 *              struct of arp_msg in edition 8. (arp.c)
 *  10 10/22/96 Add HW txoffset in ARP request packet
 *  11 10/29/96 Remove the time out timer at close
 *  12 11/27/96 In del_enet_entry, check to make sure that arp_req_list sr
 *              is not NULL before accessing it. (entry.c)
 *  15 12/26/96 New style spenet, with only one common desc.            sr
 *  16 01/13/97 Set the M_BCAST flag for ethernet broadcast packets.    sr
 *              (ethernet.c)
 *  17 01/17/97 Zero out memory allocated by srqmem. (entry.c)          sr
 *  18 01/23/97 Changes to ppstg to make new style spenet work.         sr
 *              (defs.h, entry.c, arp.c)
 *  19 01/28/97 Added setstats for ARP ioctl. (entry.c)                 sr
 *  20 01/30/97 In dr_setstat, enable/disable HW only if hardware       sr
 *              exists. (entry.c)
 *  21 02/02/97 Return sensible values on errors. (entry.c, arp.c)      sr
 *  22 02/17/97 Changed entry.c for 403 alignment problems.            pjw
 *              Verify enough space in mbuf to prepend ethernet header rds
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *  23 04/27/97 Get arp source address from per path storage           rds
 *              Convert from ISP to BSD header files                   rds
 *  24 04/28/97 Split part of defs.h into bsddefs.h                    rds
 *  25 04/29/97 Fix host to net conversions in send_arp (arp.c)         sr
 *  26 05/03/97 Fix source address code added in edition 23            rds
 *              Replace get_mbuf with m_getn, clean up unused code     rds
 *  27 05/05/97 Fix endian problems in ethernet.c                      rds
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  28 05/19/97 Fix RARP problem with freeing mbuf twice               rds
 *     05/20/97 <--------- SPF LAN Comm Pack v3.1.1 Release -------->
 *  29 12/05/97 Fixed family check in arp_delete & arp_change          tmd
 *     12/09/97 Made process_arp_request answer "published" entries    tmd
 *     12/09/97 Nuked trailer code that was #if 0-ed                   tmd
 *     12/09/97 Fixed a lot of confusion in process_arp_reply where    tmd
 *              the code stolen from ISP was handling an arp request   tmd
 *     12/10/97 Set pathdesc on SMCALL_UPDATA so IP knows which intfc  tmd
 *  30 12/10/97 Changed is_super to use _os_getsys instead of _os_id   tmd
 *              Added code to retrieve a single arp entry              tmd
 *              Added check_access function to verify memory access    tmd
 *              arptbl_copy now sets size of arp table in SPF pb       tmd
 *  31 02/03/98 Removed spsin.h, removed compiler warnings             rds
 *  32 04/01/98 Cleaned up code.                                       sr
 *     04/01/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  33 09/25/98 Added SPENET define for use in BSD header files        rds
 *     09/25/98 Check for multicast packets in dr_downdata (entry.c)   rds
 *     09/25/98 Set M_MCAST flag on multicast packets (ethernet.c)     rds
 *  34 12/14/98 Fixed potential mbuf leak in dr_updata (entry.c)       rds
 *  35 01/24/99 Set arp_op to ARP_REPLY in proxy arp response (arp.c)  rds
 *  36 02/02/99 Fixed default case of dr_setstat, dr_getstat (entry.c) rds
 *  37 11/22/99 Many parts rewritten to improve performance            rds
 *              Fixed mbuf leaks (CF6718), arp flooding (CF2734)       rds
 *  38 01/19/00 Leave extra space in mbuf if we do a pullup (entry.c)  rds
 *  39 10/19/00 Fix-up word/longword alignment                         taka
 *  40 10/31/00 The pullup code added in edition 38 didn't set m_size  rds
 *              Removed setting of packet size based on IP header      rds
 *  41 02/04/01 Send routing socket update message for arp add/delete  rds
 *  42 02/26/01 CF10443: added SPF_GS_PROTID getstat                   sbh
 *  43 06/04/01 CF11330: increase held mbuf to 6                       taka
 *  44 08/10/01 CF12061: arp cache now updated when HW addr changes,   kimk
 *                       update arp cache for new responses.
 *  45 08/16/01 Fixed alignment problems in edition 44 changes         rds
 ****************************************************************************/

/*
**	Edition/Revision Numbers
*/

/* driver edition number */
#ifdef SYSEDIT

_asm("_sysedit: equ 45");
_asm("_sysattr: equ 0xA000");

#endif

/* descriptor edition number */
#ifdef DESC_SYSEDIT

_asm("_sysedit: equ 18");
_asm("_sysattr: equ 0x8000");

#endif

