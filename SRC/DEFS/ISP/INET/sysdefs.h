/*---------------------------------------------------------------------------!
!                                                                            !
!   File Name:      sysdefs_h                                                !
!                                                                            !
!   Function:		File to gather system definitions together               !
!																			 !
!	Note:			Macros are redefined for both OS's, instead of just		 !
!					redefining OS9's names as OS9000's, so that when reading !
!					code redefined stuff is obvious (IMHO).					 !
+----------------------------------------------------------------------------+
!  Edition History:                                                          !
!   #   Date    Comments                                                By   !
!  --- -------- ------------------------------------------------------- ---  !
!  001 90/04/05 Creation                                                DJN  !
!  002 90/07/20 --------------- ISP/OS9000 Release 1.1 ---------------- DJN  !
!  003 94/02/09 defined fatalsignal() macro for OS9						gkm	 !
!			  > define P_CONDEMN for OS9 & 9000								 !
!			  > for OS9 corrected PD_COUNT to xlate to path.pd_count		 !
!  004 94/04/26 dd_io becomes dds                                      ylee  !
!  005 94/06/14 added PD_LPROC                                          bat  !
!  006 94/10/07 added is_system_state_svc()                             bat  !
!  007 95/09/25 Changed 9000 V_USE_CNT from v_use_cnt to v_iuse_cnt     mgh  !
!               based on _OS9000_V1 conditional for o386 compatibility       !
!  008 95/12/13 added new macro to is_system_state macro for checking   mgh  !
!               if the IO call came from system/user state (OS9000/V2.0)     !
!               for os9000/386 and os9000/ppc                                !
!      96/02/15 --------------- ISP v.2.1 Release ------------------         !
!      97/02/19 <***** SPF LAN Comm Pack v3.0 Release ********>              !
!      97/04/29 <***** OS-9000 V2.2 Release (Low Level ROM)***********>      !
!               ---- OS-9000/Brutus Board Support V1.0 Released ----         !
!  009 01/10/15 fatalsignal() is now defined in signal.h for 68k        rds  !
!---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------!
!                                                                            !
!     Copyright 1988, 1989, 1990, 1994 by Microware Systems Corporation      !
!                          Reproduced Under License                          !
!                                                                            !
!  This source code is the proprietary  confidential property of Microware   !
!  Systems  Corporation, and is provided to licensee for documentation and   !
!  educational  purposes only. Reproduction, publication, or  distribution   !
!  in any form to any party other than the licensee is strictly prohibited.  !
!                                                                            !
!---------------------------------------------------------------------------*/
#ifndef _SYSDEFS_H
#define _SYSDEFS_H

/* System definitions for OS9 */
#ifdef _OSK

/* Get definitions files */
#include <MACHINE/reg.h>
#include <signal.h>
#include <procid.h>
#include <sysio.h>

/* Device list definitions. */
#define		DEV_LIST	Devicetbl
#define		V_DD_MOD	V_desc
#define		V_USE_CNT	V_usrs
#define		V_DR_STAT	V_stat

/* Path descriptor definitions. */
#define		PD_SKM		union pathdesc *
#define		PD_IFM		union pathdesc *
#define		PD_COUNT	path.pd_count
#define		PD_CPROC	path.pd_cpr
#define		PD_LPROC	path.pd_lproc
#define		PD_DEV		path.pd_dev
#define		PD_ID		path.pd_pd
#define		PD_SOCKPVT	path.fm_pvt.sock
#define		PD_SOCKET	path.fm_pvt.sock.pd_socket

/* Module definitions. */
#define		MH_DEV		mod_dev *
#define		M_NAME		_mh._mname
#define		MH_COM		mod_exec *
#define		M_EXEC		_mexec
#ifndef DD_COM
	#define		DD_COM		mod_dev
#endif
#define		DD_FMGR		dds._mfmgr
#define		DD_DRVR		dds._mpdev
#define		DD_PORT		dds._mport
#define		DD_VECTOR	dds._mvector
#define		DD_PRIORITY	dds._mpriority
#define		DD_IRQLVL	dds._mirqlvl
#define		GET_DD(x)	(x)

/* Process descriptor definitions. */
#define		PR_DESC		procid *
#define		P_PRIOR		_prior
#define		P_STATE		_state
#define		P_PMODUL	_pmodul
#define		P_QUEUEID	_queueid
#define		P_SP		_sp
#define		P_ID		_id
#define		P_MEMIMG_0	_memimg[0]
#define		P_BLKSIZ_0	_blksiz[0]
#define		PROC_STATE	0x8000
#define		P_CONDEMN	PS_CONDEMN

/* Architectural definitions. */
#define		REGS		REGISTERS
#define		STACK		u_char *

/* I/O Process descriptor definitions. */
#define		IO_PROC		procid *
#define		IOP_PATHS	_path

/* Signal definitions. */
#define		WAKE_SIGNAL	SIGWAKE
#define		KILL_SIGNAL	SIGKILL

/* Format of the first part of file manager static storage */
/* required by the KERNEL and/or IOMAN. */
typedef struct sysiost FM_STATIC;

#ifndef fatalsignal
#define fatalsignal(s) ((s > 0 ) && (s <= SIGDEADLY))
#endif /*fatalsignal*/

/* system-state calling process macros */
#include <MACHINE/reg.h>
#define is_system_state_svc(pd) ((pd->path.pd_rgs->sr & 0x2000) != 0)

#endif

/*********************************/
/* System definitions for OS9000 */
/*********************************/

#ifdef _OS9000

/* Get definitions files */
#include <io.h>

/* Device list definitions. */
#define		DEV_LIST	Dev_list
#ifdef _OS9000_V1
#define		V_USE_CNT	v_use_cnt		/* broken the but works */
#else
#define		V_USE_CNT	v_iuse_cnt		/* For use with IOMan ed>=15 */
#endif
#define		V_DD_MOD	v_dd_mod
#define		V_DR_STAT	v_dr_stat

/* Path descriptor definitions. */
#define		PD_SKM		struct pd_skm *
#define		PD_IFM		struct pd_ifm *
#define		PD_COUNT	pd_io.pd_count
#define		PD_CPROC	pd_io.pd_cproc
#define		PD_LPROC	pd_io.pd_cproc
#define		PD_DEV		pd_io.pd_dev
#define		PD_ID		pd_io.pd_id
#define		PD_SOCKET	sockpvt.pd_socket
#define		PD_SOCKPVT	sockpvt

/* Module definitions. */
#define		MH_COM		Mh_com
#if !defined(_EDITMOD)		/* hide this definition from EditMod */
#define		M_EXEC		m_exec
#endif
#define		MH_DEV		Mh_dev
#define		M_NAME		m_name
#ifndef DD_COM
	#define		DD_COM		dd_com
#endif
#define		DD_FMGR		dds.dd_fmgr
#define		DD_DRVR		dds.dd_drvr
#define		DD_PORT		dds.dd_port
#define		DD_VECTOR	dd_vector
#define		DD_PRIORITY	dd_priority
#define		DD_IRQLVL	dd_irqlevel
#define		GET_DD(x)	((u_char *)(x) + (x)->m_exec)

/* Process descriptor definitions. */
#define		PR_DESC		Pr_desc
#define		P_PRIOR		p_prior
#define		P_STATE		p_state
#define		P_PMODUL	p_pmodul
#define		P_QUEUEID	p_queueid
#define		P_SP		p_sp
#define		P_ID		p_id
#define		P_MEMIMG_0	p_memimg[2]
#define		P_BLKSIZ_0	p_memimg[3]
#define		PROC_STATE	SYSSTATE
#define		P_CONDEMN	CONDEMN

/* Architectural definitions. */
#define		REGS		regs
#define		STACK		Regs

/* I/O Process descriptor definitions. */
#define		IO_PROC		Io_proc
#define		IOP_PATHS	iop_paths

/* Signal definitions. */
#define		WAKE_SIGNAL	S_WAKE
#define		KILL_SIGNAL S_KILL

/* Process state definitions */
#define		P_CONDEMN	CONDEMN

/* Format of the first part of file manager static storage */
/* required by the KERNEL and/or IOMAN. */
typedef struct {
	u_int32			v_fncnt;		/* function count */
	error_code		(*v_attach)();	/* attach routine */
	error_code		(*v_chgdir)();	/* change directory routine */
	error_code		(*v_close)();	/* close routine */
	error_code		(*v_create)();	/* create routine */
	error_code		(*v_delete)();	/* delete routine */
	error_code		(*v_detach)();	/* detach routine */
	error_code		(*v_dup)();		/* duplicate path routine */
	error_code		(*v_getstat)();	/* get status routine */
	error_code		(*v_makdir)();	/* make directory routine */
	error_code		(*v_open)();	/* open routine */
	error_code		(*v_read)();	/* read routine */
	error_code		(*v_readln)();	/* read line routine */
	error_code		(*v_seek)();	/* seek routine */
	error_code		(*v_setstat)();	/* set status routine */
	error_code		(*v_write)();	/* write routine */
	error_code		(*v_writeln)();	/* write line routine */
} FM_STATIC;

/* system-state calling process macros */
#if defined(_MPFPOWERPC)
	#include <regppc.h>
	#define is_system_state_svc(pd) (systate_svc(pd->pd_io.pd_state)) 
#elif defined(_MPF386)
	#if defined(_OS9000_V2)
		#include <reg386.h>
		#define is_system_state_svc(pd) (systate_svc(pd->pd_io.pd_state)) 
	#endif
#elif defined(_MPF68020)
	#include <reg68k.h>
	#define is_system_state_svc(pd) (pd->pd_io.pd_state & SUPERVIS)
#endif

#endif


#endif /* _SYSDEFS_H */
