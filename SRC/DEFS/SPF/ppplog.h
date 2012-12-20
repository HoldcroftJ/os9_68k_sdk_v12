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
 *  01 12/04/96 Initial                                                billw
 *     05/14/97 Uncommented defines that were if 0ed.                  sr
 *     05/14/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#ifndef _ppplog_h
#define _ppplog_h

	#include <const.h>
	#include <errno.h>
	#include <funcs.h>
	#include <module.h>
	#include <types.h>

	#include <SPF/ppp.h>


	typedef struct debug_mod	debug_mod, *Debug_mod;


	#ifndef ERR_MOD_NAME
		#define ERR_MOD_NAME		"ppp_log"
	#endif

	#ifndef MAX_ERRS
		#define MAX_ERRS			20
	#endif

	/*** Structures that make up a ppp error log module. ***/

	typedef struct
	{
		u_int32		driver : 3;
			#define DRV_HDLC		0x01
			#define DRV_RXTHD		0x02
			#define DRV_TXTHD		0x03
			#define DRV_LCP			0x04
			#define DRV_IPCP		0x05

		u_int32		location : 13;
			#define LOC_INIZ		0x01
			#define LOC_TERM		0x02
			#define LOC_GETSTAT		0x03
			#define LOC_SETSTAT		0x04
			#define LOC_UPDATA		0x05
			#define LOC_DNDATA		0x06
			#define LOC_TIMER		0x07
			#define LOC_MAIN		0x08
			#define LOC_THDEXIT		0x09
			#define LOC_THDLOOP		0x0a
			#define LOC_GETDATA		0x0b
			#define LOC_GETFRAME	0x0c
			#define LOC_CHKMSG		0x0d
			#define LOC_NEWMSG		0x0e
			#define LOC_OPENPORT	0x0f
			#define LOC_CLOSPORT	0x10
			#define LOC_NEWBUFR		0x11
			#define LOC_PROCMB		0x12
			#define LOC_FREEBUFR	0x13
			#define LOC_GETMB		0x14
			#define LOC_SCR			0x15
			#define LOC_SCA			0x16
			#define LOC_SCN			0x17
			#define LOC_STR			0x18
			#define LOC_STA			0x19
			#define LOC_SER			0x1a
			#define LOC_SEP			0x1b
			#define LOC_SCJ			0x1c
			#define LOC_SPJ			0x1d
			#define LOC_SDR			0x1e
			#define LOC_TMR_ON		0x1f
			#define LOC_TMR_OFF		0x20
			#define LOC_LYR_STRT	0x21
			#define LOC_GPP_PROT	0x22
			#define LOC_GNC_PROT	0x23
			#define LOC_HCR			0x24
			#define LOC_HCJ			0x25
			#define LOC_PAP_TO		0x26
			#define LOC_HPN			0x27
			#define LOC_SPR			0x28
			#define LOC_PAPFAIL		0x29
			#define LOC_HCC			0x2a
			#define LOC_CHAPFAIL	0x2b
			#define LOC_WAIT_FOR_CHILD	0x2c
			#define LOC_TIMESLICE	0x2d

		u_int32		systype : 16;
			#define STYP_NONE		0x00
			#define STYP_FUNC		0x01
			#define STYP_EVENT		0x02
			#define STYP_ALARM		0x03
			#define STYP_CODE		0x04
			#define STYP_IO			0x05
				/* Logical OR these codes with parameter code */
			#define STYP_LCP_PARAM	0x50
			#define STYP_IPCP_PARAM	0x60

		u_int32		syscall;
			#define SCALL_NONE		0xff

			/* See 'funcs.h' */

		error_code	err;
		u_int32		err_cnt;

	} ppp_error, *Ppp_error;

	typedef struct
	{
		u_int32		hdlc_enabled : 1;
		u_int32		lcp_enabled : 1;
		u_int32		ipcp_enabled : 1;

		u_int32		reserved : 29;

	} ppp_flags;

	struct debug_mod
	{
		ppp_flags	flags;

		/* Params from HDLC lustat. */

		ppp_modem_p	modem_p;
		u_int8		rx_baud_rate;
		u_int8		rx_parity;
		u_int8		rx_word_size;
		u_int8		rx_stop_bits;
		u_int8		rx_rts_enable;
		u_int8		tx_baud_rate;
		u_int8		tx_parity;
		u_int8		tx_word_size;
		u_int8		tx_stop_bits;
		u_int8		tx_rts_enable;
		u_int32		hdlc_sendsig_delay;
		u_int32		hdlc_ppp_mode;

		/* Params from LCP lustat. */

		ppp_lcp_p	lcp_p;
		ppp_sys_p	lcp_s;
		u_int8		lcp_auth_timeout;
		u_int8		lcp_auth_max_transmits;

		/* Params from IPCP lustat. */

		ppp_ipcp_p	ipcp_p;
		ppp_sys_p	ipcp_s;
		char		ipcp_desc_name[MAX_NAME_LEN];
		char		ipcp_ll_pathdesc[MAX_NAME_LEN];

		u_int32		rxthd_up_pkts,
					txthd_dn_pkts,
					hdlc_dn_pkts,
					lcp_up_pkts,
					lcp_dn_pkts,
					ipcp_up_pkts,
					ipcp_dn_pkts;

		u_int32		rxthd_up_pkt_errs,
					txthd_dn_pkt_errs,
					hdlc_up_pkt_errs,
					hdlc_dn_pkt_errs,
					lcp_up_pkt_errs,
					lcp_dn_pkt_errs,
					ipcp_up_pkt_errs,
					ipcp_dn_pkt_errs;

		u_int32		rxthd_bytes_in,
					txthd_bytes_out,
					rxthd_buf_ov_errs,
					rxthd_crc_errs,
					rxthd_line_errs,
					txthd_line_errs;

		u_int32		err_index,
					err_wrap;

		ppp_error	errors[MAX_ERRS];

	};


	/*** Additional Function Types not in the OS-9 file. ***/

	#if defined(_OSK)

		#define A_ATIME 	3
		#define A_CYCLE 	2
		#define A_DELET 	0
		#define A_RESET		4
		#define A_SET		1

		#define EV_ALLCLR 	15
		#define EV_ALLSET 	13
		#define EV_ANYCLR 	14
		#define EV_ANYSET 	12
		#define EV_CHANGE 	17
		#define EV_CREAT 	2
		#define EV_DELET 	3
		#define EV_INFO 	7
		#define EV_LINK 	0
		#define EV_PULSE 	9
		#define EV_READ 	6
		#define EV_SET 		10
		#define EV_SETAND 	19
		#define EV_SETOR 	18
		#define EV_SETR 	11
		#define EV_SETXOR 	20
		#define EV_SIGNL 	8
		#define EV_TSTSET 	16
		#define EV_UNLNK 	1
		#define EV_WAIT 	4
		#define EV_WAITR 	5

		#define F_ACQLK 	135
		#define F_ALTMDIR	71
		#define F_CAQLK 	136
		#define F_CHAINM	75
		#define F_CHMDIR	68
		#define F_CLRSIGS	7
		#define F_CMDPERM	70
		#define F_CRLK 		134
		#define F_DDLK 		139
		#define F_DELLK 	138
		#define F_DELMDIR	69
		#define F_DFORKM	76
		#define F_EXPEVT	87
		#define F_EXPPRT	88
		#define F_FINDFREE	85
		#define F_FMOD		19
		#define F_FORKM		74
		#define F_GETMDP	73
		#define F_GETSYS	41
		#define F_GETVPCMEM 89
		#define F_GETVPCPT	90
		#define F_GETVPCPTAB 93
		#define F_INITDATA	28
		#define F_LINKM		77
		#define F_MKMDIR	67
		#define F_MODADDR	80
		#define F_MSDOSFORK 82
		#define F_RELLK 	137
		#define F_SIGRS		84
		#define F_SLINK		72
		#define F_SLINKM	78
		#define F_TEXIT		63
		#define F_TLINKM	79
		#define F_TRANSADD	86
		#define F_VPCDMEMEV	92
		#define F_VPCGMEMEV 91
		#define F_VPERMIT	83
		#define F_WAITLK 	143

		#define I_ALIAS 	144
		#define I_ALLPD 	128
		#define I_CIOPROC 	141
		#define I_EXPIOPT	148
		#define I_GETDL		147
		#define I_GETPD 	129
		#define I_GETSTAT 	157
		#define I_GIOPROC 	140
		#define I_IODEL 	130
		#define I_IOEXIT	133
		#define I_IOFORK 	132
		#define I_ORG 		128
		#define I_RDALST 	146
		#define I_RETPD 	131
		#define I_SETSTAT 	163
		#define I_SGETSTAT 	145
		#define I_TRANPN 	142

	#else

		#define F_ACQLK 	135
		#define F_ALARM		1
		#define F_ALLPRC	2
		#define F_ALLTSK	3
		#define F_ALTMDIR	71
		#define F_APROC		4
		#define F_CAQLK 	136
		#define F_CCTL		5
		#define F_CHAIN		39
		#define F_CHAINM	75
		#define F_CHKMEM	6
		#define F_CHMDIR	68
		#define F_CLRSIGS	7
		#define F_CMDPERM	70
		#define F_CMPNAM	8
		#define F_CPYMEM	9
		#define F_CRC		10
		#define F_CRLK 		134
		#define F_DATMOD	11
		#define F_DATTACH	94
		#define F_DDLK 		139
		#define F_DELLK 	138
		#define F_DELMDIR	69
		#define F_DELTSK	14
		#define F_DEXEC		12
		#define F_DEXIT		13
		#define F_DFORK		15
		#define F_DFORKM	76
		#define F_EVENT		16
		#define F_EXIT		17
		#define F_EXPEVT	87
		#define F_EXPPRT	88
		#define F_FINDFREE	85
		#define F_FINDPD	18
		#define F_FMOD		19
		#define F_FORK		40
		#define F_FORKM		74
		#define F_GBLKMP	20
		#define F_GETMDP	73
		#define F_GETSYS	41
		#define F_GETVPCMEM 89
		#define F_GETVPCPT	90
		#define F_GETVPCPTAB 93
		#define F_GMODDR	21
		#define F_GPRDBT	23
		#define F_GPRDSC	22
		#define F_GREGOR	25
		#define F_ICPT		27
		#define F_ID		26
		#define F_INITDATA	28
		#define F_IRQ		29
		#define F_JULIAN	30
		#define F_LINK		32
		#define F_LINKM		77
		#define F_LOAD		33
		#define F_MBUF		95
		#define F_MEM		31
		#define F_MKMDIR	67
		#define F_MODADDR	80
		#define F_MOVE		34
		#define F_MSDOSFORK 82
		#define F_NPROC		38
		#define F_PERMIT	43
		#define F_PERR		42
		#define F_PROTECT	44
		#define F_PRSNAM	45
		#define F_RELLK 	137
		#define F_RETPD		46
		#define F_RTE		47
		#define F_SEMA		96
		#define F_SEND		48
		#define F_SETCRC	49
		#define F_SETSYS	81
		#define F_SIGMASK	53
		#define F_SIGRS		84
		#define F_SLEEP		54
		#define F_SLINK		72
		#define F_SLINKM	78
		#define F_SPRIOR	51
		#define F_SRQMEM	55
		#define F_SRTMEM	56
		#define F_SSPD		59
		#define F_SSVC		57
		#define F_STIME		50
		#define F_STRAP		58
		#define F_SUSER		52
		#define F_SYSDBG	60
		#define F_SYSID		61
		#define F_TEXIT		63
		#define F_TIME		35
		#define F_TLINK		62
		#define F_TLINKM	79
		#define F_TRANSADD	86
		#define F_UACCT		64
		#define F_UNLINK	36
		#define F_UNLOAD	37
		#define F_VMODUL	65
		#define F_VPCDMEMEV	92
		#define F_VPCGMEMEV 91
		#define F_VPERMIT	83
		#define F_WAIT		66
		#define F_WAITLK 	143

		#define I_ALIAS 	144
		#define I_ALLPD 	128
		#define I_ATTACH 	150
		#define I_CHGDIR 	151
		#define I_CIOPROC 	141
		#define I_CLOSE 	152
		#define I_CREATE 	153
		#define I_DELETE 	154
		#define I_DETACH 	155
		#define I_DUP 		156
		#define I_EXPIOPT	148
		#define I_GETDL		147
		#define I_GETPD 	129
		#define I_GETSTAT 	157
		#define I_GIOPROC 	140
		#define I_IODEL 	130
		#define I_IOEXIT	133
		#define I_IOFORK 	132
		#define I_MAKDIR 	158
		#define I_OPEN 		159
		#define I_ORG 		128
		#define I_RDALST 	146
		#define I_READ 		160
		#define I_READLN 	161
		#define I_RETPD 	131
		#define I_SEEK 		162
		#define I_SETSTAT 	163
		#define I_SGETSTAT 	145
		#define I_TRANPN 	142
		#define I_WRITE 	164
		#define I_WRITLN 	165

	#endif


#endif /*  _ppplog_h */
