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
 *   4 08/27/97 Bug fix                                                taka
 *   5 09/08/97 fixed os_irq()                                         taka
 *   7 12/03/97 Added check_access for netstat support                 jk
 *   8 01/21/98 Added irq_change since there is no prototype in regs.h jk
 *              added i82596totmem (chip.c)
 *              type of recv_irq changed from error_code to void
 *              type of xmit_irq changed from error_code to void
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

/*
**	Device Driver Entry Point Function Prototypes
*/

/*	C++ support	*/
#ifdef __cplusplus
extern "C" {
#endif

/* entry.c */
extern error_code dr_iniz(Dev_list deventry);
extern error_code dr_term(Dev_list deventry);
extern error_code dr_getstat(Dev_list deventry, Spf_ss_pb pb);
extern error_code dr_setstat(Dev_list deventry, Spf_ss_pb pb);
extern error_code dr_downdata(Dev_list deventry, Mbuf mb);
extern error_code dr_updata(Dev_list deventry, Mbuf mb);
extern error_code check_access(process_id, u_int32, u_int32, void *);

/*
**	Other Function Prototypes
*/
extern error_code _os_sleep(u_int32* tics, signal_code* sig);
extern error_code _os_ev_wait(event_id ev_id, int32* val, signal_code* sig, int32 minval, int32 maxval);
extern error_code _os_initdata(mh_com* mod_head, void* statics);
extern void irq_disable(void);
extern void irq_restore(status_reg stat_reg);
extern status_reg irq_save(void);
extern status_code irq_change(status_code);

/*
**	Function Prototypes
*/

/* init.c */
extern error_code 	init_drvr (Dev_list);
extern void 		get_eaddr (Spf_lustat);
extern signal_code	wait_complete (Dev_list);

/* isr.c */
extern error_code	hw_isr (Dev_list dev_entry);
extern error_code 	isr (Dev_list);

extern void		recv_irq (Dev_list);
extern void		xmit_irq (Dev_list);

extern void 	pitch_rbds (Spf_lustat, RBD *);
extern Mbuf 	supply_mbuf (Dev_list, Mbuf);
extern void 	start_ru (Dev_list);

/* chip.c */
extern void 		read_eaddr (Spf_lustat);
extern error_code 	i82596CMDAdd (Dev_list, u_int32, void*, u_int32);
extern error_code 	i82596Output (Dev_list, Mbuf);
extern error_code 	i82596meminit (Dev_list);
extern error_code 	i82596totmem (Spf_lustat);
extern void 		doport_command (Spf_lustat, u_int8 *, u_int32);

#ifdef _MPFSH
u_int32 	get_long (u_int16 *);
int 		cmp_long (u_int16 *, u_int32);
void 		copy_long (u_int16 *, u_int32 val);
u_int32 	SWAPWORDS(u_int32 orig);
#endif


#if defined (spdve7708)
extern void led1 (Spf_lustat , int);
extern void led2 (Spf_lustat , int);
#endif

/*	End C++	*/
#ifdef __cplusplus
}
#endif
