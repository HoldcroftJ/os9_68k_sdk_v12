/****************************************************************************
 *  $Workfile:   proto.h  $
 *  $Revision:   1.1  $
 *  $Modtime:   15 Mar 1994 15:12:00  $
 *  $Date:   15 Mar 1994 15:12:20  $
 ****************************************************************************
 *  SPF LANCE Device Driver entry points.                                   *
 ****************************************************************************
 *  Copyright 1994 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date       Comments                                           By   *
 *  --- --------    ----------------------------------------------- ------- *
 *    0 01/11/94    Creation                                            bat *
 *    4 01/28/94    Moved hw definitions to ports directory             bat *
 *      03/15/94    <***************** SPF v1.0 ******************>         *
 *	27FEB96	    modified for LANCE               Bill West Inc.     bw  *
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release*********>         *
 *      05/05/97    <***** SPF LAN Comm Pack v3.1 Release*********>         *
 *   11 01/08/98    Removed unused params, dr_updata, init_rmds          jk *
 *     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              *
 *   12 10/02/01 Fixed prototypes for irq_* functions					rry	*
 ****************************************************************************/


/*
 *	Function Prototypes
 */

/* entry.c */
extern error_code dr_iniz(Dev_list dev_entry);
extern error_code dr_term(Dev_list dev_entry);
extern error_code dr_getstat(Dev_list dev_entry, Spf_ss_pb spb);
extern error_code dr_setstat(Dev_list dev_entry, Spf_ss_pb ss_pb);
extern error_code dr_downdata(Dev_list dev_entry, Mbuf mb);
extern error_code dr_updata();
extern error_code enable_lance(Spf_lustat lustat);
extern error_code disable_lance(Spf_lustat lustat);

/* isr.c */
extern void  isr_os9(Dev_list dev_entry);
extern int load_tmd(Ifs ifstat, Mbuf mb);
extern error_code init_rmds(Ifs ifstat);

/* misc.c */
extern error_code get_mbuf(Dev_list dev_entry, u_int32 size, Mbuf *mb);
extern error_code process_mbuf(Dev_list dev_entry, Mbuf mb);
#ifdef notdef
extern error_code return_mbuf(Mbuf mb); /* This is now a macro */
#endif

/* init.c */
extern error_code init_hdw(Dev_list dev_entry);
extern void get_eaddr(Dev_list deventry, u_char *eaddr, u_char *hwaddr);
extern void nop(void);
extern error_code freedmem(Spf_lustat lustat);
void swabytes( void *data);

/* term.c */
extern error_code term_hdw(Dev_list dev_entry);
extern error_code free_rmds(Ifs ifstat);

/* other */
extern status_code irq_change(status_code stat_reg);
extern void irq_restore(status_code stat_reg);
