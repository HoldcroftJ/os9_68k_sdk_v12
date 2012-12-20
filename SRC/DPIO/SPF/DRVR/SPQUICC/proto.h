/*****************************************************************************
 * SPF QUICC Driver function prototypes.                                     *
 *****************************************************************************
 *            Copyright 1989-1996 by Microware Systems Corporation           *
 *                            All Rights Reserved                            *
 *                          Reproduced Under License                         *
 *                                                                           *
 *  This software is confidential property of Microware Systems Corporation, *
 *  and is provided under license for internal development purposes only.    *
 *  Reproduction, publication, distribution, or creation of derivative works *
 *  in any form to any party other than the licensee is strictly prohibited, *
 *  unless expressly authorized in writing by Microware Systems Corporation. *
 *****************************************************************************
 *  Edition History:                                                         *
 *   #   Date       Comments                                           By    *
 *  --- --------    ----------------------------------------------- -------- *
 *  01 10/07/96 Creation                                                     *
 *     02/19/97 <***** SPF LAN Comm Pack v3.0 Release*********>              *
 *     05/05/97 <***** SPF LAN Comm Pack v3.1 Release*********>              *
 *  09 08/11/97 Added SCC channel dependent code                        taka *
 *     10/22/97 <***** OS-9000 MBX BSP Release v1.0 **********>              *
 *  12 11/25/97 Added check_access for netstat support                  jk   *
 *  14 01/21/98 Added _pic_enable, _pic_disable, irq_disable,                *
 *               irq_enable, config_pins, config_TENA, config_ETRX      jk   *
 *     04/02/98 <***** SPF LAN Comm Pack v3.2 Release*********>              *
 *  19 02/10/00 Removed unneeded prototypes                             rds  *
 *  20 02/11/00 Added CHECK_LOCK option, removed 3rd arg from get_eaddr kimk *
 ****************************************************************************/


#if !defined (_PROTO_H)
#define _PROTO_H

/* Function Prototypes */

#if defined(__STDC__) || defined(_ANSI_EXT)
#define P_(s) s
#else
#define P_(s) ()
#endif

/*
**	Function Prototypes
*/

/* entry.c */
extern error_code dr_iniz P_((Dev_list));
extern error_code dr_term P_((Dev_list));
extern error_code dr_getstat P_((Dev_list, Spf_ss_pb));
extern error_code dr_setstat P_((Dev_list, Spf_ss_pb));
extern error_code dr_downdata P_((Dev_list, Mbuf));
extern error_code dr_updata P_((Dev_list, Mbuf));
extern error_code check_access(process_id, u_int32, u_int32, void *);

/* isr.c */
extern error_code  qe_isr P_((Dev_list));
extern error_code isr P_((Dev_list));
extern error_code recv_irq P_((Dev_list));
extern error_code xmit_irq P_((Dev_list));
extern error_code qe_xmit P_((Dev_list, Mbuf));

/* init.c */
extern error_code init_hdw P_((Dev_list));
extern void get_eaddr P_((Dev_list, u_char *));
extern void outl(void *, u_int32);
extern void outw(void *, u_int16);
extern void outc(u_int8 *, u_int8);
u_int8 read_byte(void *);
extern u_int16 read_word(void *);
extern u_int32 read_long(void *);
extern void write_byte(void *, u_int8);
extern void write_word(void *, u_int16);
extern void write_long(void *, u_int32);
extern u_int32 inl(void *);
extern u_int16 inw(short *);
extern u_int8 inc(void *);

extern void config_pins(QUICC *, u_int32);
extern void config_TENA(QUICC *, u_int32);
extern void config_ETRX(u_int32, QUICC *, u_int32);

#ifdef HW_GET_EADDR
extern error_code hw_get_eaddr(u_int8 []);
#endif
#ifdef CHECK_LOCK
extern error_code getLock(Spf_lustat, u_int32);
extern error_code clrLock(Spf_lustat, u_int32);
#endif

/* term.c */
extern error_code term_hdw P_((Dev_list));

/* other */
extern error_code P_((*cache_cctl)(u_int32, void *, u_int32));
extern void irq_disable(void);
extern void irq_enable(void);
extern status_code irq_maskget(void);
extern void irq_restore(status_code);

/*
** The prototype for _os9_irq is in file OS9/SRC/DEFS/MACHINE/reg.h.
** However, reg68k.h (included by DPIO/DEFS/defconv.h) defines REG_H
** causing the interior of reg.h to NOT be executed.
*/
extern error_code _os9_irq(u_int32, u_int32, void *, void *, void *);

#endif /* End define _PROTO_H */
