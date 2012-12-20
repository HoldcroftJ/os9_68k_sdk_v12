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
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *     04/27/97 Removed srcip from send_arp_request and arpresolv      rds
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *     05/20/97 <--------- SPF LAN Comm Pack v3.1.1 Release -------->  
 *  32 04/01/98 Cleaned up code.                                       sr
 *     04/01/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  37 11/22/99 Added new prototypes and cleaned up old ones           rds
 *  39 10/19/00 Fix-up word/longword alignment                         taka
 ****************************************************************************/

/***********************/
/* Function Prototypes */
/***********************/

/*
**	Device Driver Entry Point Function Prototypes
*/

/* entry.c */
extern error_code dr_iniz(Dev_list);
extern error_code dr_term(Dev_list);
extern error_code dr_getstat(Dev_list, Spf_ss_pb);
extern error_code dr_setstat(Dev_list, Spf_ss_pb);
extern error_code dr_downdata(Dev_list, Mbuf);
extern error_code dr_updata(Dev_list, Mbuf);

error_code add_enet_entry(Dev_list, Spf_drstat, Spf_lustat);
Pp_enetlist find_enet_entry(Spf_drstat, Spf_pdstat);
void del_enet_entry(Dev_list, Spf_pdstat);

/* ethernet.c */
Mbuf process_enet(Dev_list, Pp_enetlist, Mbuf);
error_code add_enet(Dev_list, u_int8 *, u_int16, Pp_enetlist, Mbuf);

/* arp.c */
void process_arp(Dev_list, Pp_enetlist, Mbuf);
error_code send_arp_request(Dev_list, struct in_addr *, Pp_enetlist);
struct arptab *arptab_look(Dev_list, struct in_addr *);
struct arptab *arptnew(Dev_list, struct in_addr *);
struct arptab *arpresolve(Dev_list, Pp_enetlist, Mbuf, struct in_addr*, u_int8 *);
Pp_enetlist find_enet_ppstg(Spf_drstat, Dev_list);
int verify_protoaddr(struct in_addr *, Pp_enetlist, u_int32);
void arptick(Timer_pb);
int arp_change(Dev_list, struct arpreq *);
int arp_delete(Dev_list, struct arpreq *);
error_code arpent_copy(struct arpstat *, struct arptab *);
error_code arptbl_copy(struct arpstat *, struct arptab *, u_int32 *);
error_code check_access(Spf_drstat, void *, u_int32, u_int32);
