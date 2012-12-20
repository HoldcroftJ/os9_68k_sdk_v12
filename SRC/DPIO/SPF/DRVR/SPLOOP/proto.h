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
 * Edition History:                                                         *
 * #   Date     Comments                                          By        *
 * --- -------- ------------------------------------------------- --------- *
 *   1 01/07/94 Created from SPF v1.0.2                                 bat *
 *   2 03/28/95 Reorganization of logical unit & driver static          bat *
 *     06/30/95 <--------------- SPF 2.0 pre-release------------>       cas *
 *   3 08/28/95 Added C++ support                                       dwj *
 *     06/30/95 <-------------------- SPF 2.0 ------------------>       dwj *
 * 204 06/26/97 <--------------- SPF 2.2 ----------------------->       taka*
 * 208 11/04/97 <--------------- SPF v2.2.1 -------------------->       taka*
 * 211 07/19/98 <---------------- SPF v2.2.2 ------------------->       taka*
 * 212 10/02/01 Fixed prototypes for irq_* functions					rry	*
 ****************************************************************************/

/*
**	Device Driver Entry Point Function Prototypes
*/

/*	C++ Support	*/
#ifdef __cplusplus
extern "C" {
#endif

/* entry.c */
extern u_int32 send_notify(Notify_type notify_ptr);
extern error_code add_lpbk_list(Spf_drstat drstat, Spf_lustat lustat);
extern void del_lpbk_list(Spf_drstat drstat, Spf_lustat lustat);
extern Dev_list find_updrvr(Spf_drstat drstat, Spf_pdstat pd);
extern error_code set_new_updrvr(Spf_drstat drstat, Spf_pdstat pd, Dev_list new_updrvr);
extern error_code dr_iniz(Dev_list deventry);
extern error_code dr_term(Dev_list deventry);
extern error_code dr_getstat(Dev_list deventry, Spf_ss_pb pb);
extern error_code dr_setstat(Dev_list deventry, Spf_ss_pb pb);
extern error_code dr_downdata(Dev_list deventry, Mbuf mb);
extern error_code dr_updata(Dev_list deventry, Mbuf mb);

/*
**	Other Function Prototypes
*/

extern error_code _os_sleep(u_int32* tics, signal_code* sig);
extern error_code _os_ev_wait(event_id ev_id, int32* val, signal_code* sig, int32 minval, int32 maxval);
extern error_code _os_initdata(mh_com* mod_head, void* statics);
extern void irq_disable(void);
extern void irq_restore(status_code stat_reg);
extern status_code irq_save(void);


/*	End C++	*/
#ifdef __cplusplus
}
#endif

