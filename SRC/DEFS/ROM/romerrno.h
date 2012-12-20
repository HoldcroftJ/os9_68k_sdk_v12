/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1995-2001 by Microware Systems Corporation         |
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
|---------------------------------------------------------------------------|
|                                                                           |
| romerrno.h - ROM specific error codes                                     |
|                                                                           |
| edition history                                                           |
|                                                                           |
|   date     changes                                                    by  |
| -------- -----------------------------------------------------------  --- |
| 95/11/21  History started                                             ats |
|           ---- OS-9/68K V3.0.2 System-state Debug Support Release ----    |
| 96/01/05  Added EOS_NOVERMATCH definition.                            ats |
| 96/01/08  Added EOS_PSEUDOOK definition.                              ats |
|           ---- OS-9000/PPC V2.0 Release ----                              |
| 96/03/13  Added EOS_NTFYREGD definition.                              gdb |
|           ---- OS-9000/PPC V2.0.1 Released ----                           |
|           ---- OS-9/68K V3.0.3 System-state Debug Support Release     --- |
|           ---- OS-9000/x86 V2.1 Released ----                             |
|           ---- OS-9000/PPC V2.1.1 Released ----                           |
| 96/11/18  Added EXCP services error codes.                            gdb |
|           ---- OS-9000/ARMv3 V2.2 Released ----                           |
|           ---- OS-9000/ARMv3 V2.2.1 Released ----                         |
|           ---- Modular Rom Sub-system v1.0 Released ----                  |
|           ---- OS-9000/SH3 V2.2.6 Released ----                           |
|           ---- OS-9000/SPARC V2.2.7 Released ----                         |
|           ---- Modular Rom v1.1 Sub-component Released ----               |
|           ---- Modular ROM v1.2 Sub-component Released ----               |
| 01/06/13  Added compression services error codes.                     gdw |
`--------------------------------------------------------------------------*/
                                        
#if !defined(_ROMERRNO_H)
#define _ROMERRNO_H

/* 009:000 reserved for ROM code */
#if defined(_OS9000)
#define EOS_ROMBASE		0x90000				/* ROM error code base			*/
#endif /* _OS9000 */

#if defined(_OSK)
#define EOS_ROMBASE		0x900				/* ROM error code base			*/
#endif /* _OSK */

#define EOS_NOROMINFO	 EOS_ROMBASE		/* no rominfo table found		*/
#define EOS_BADPARAM	(EOS_ROMBASE+1)		/* bad parameter passed			*/
#define EOS_NOHW		(EOS_ROMBASE+2)		/* hardware not present			*/
#define EOS_ROMMNF		(EOS_ROMBASE+3)		/* rom module not found			*/
#define EOS_TOOSMALL	(EOS_ROMBASE+4)		/* first memory block too small */
#define EOS_NOTMINE		(EOS_ROMBASE+5)		/* not my interrupt				*/
#define EOS_NOMEMSVC	(EOS_ROMBASE+6)		/* memory services not found	*/
#define EOS_BDROMINF	(EOS_ROMBASE+7)		/* Bad Rominfo pointer			*/
#define EOS_NOOSSVC		(EOS_ROMBASE+8)		/* os services not found		*/
#define EOS_NOROMSVC	(EOS_ROMBASE+9)		/* ROM services not found		*/
#define EOS_NOVERMATCH	(EOS_ROMBASE+0x0a)	/* version mismatch				*/

#define EOS_CONSBASE	(EOS_ROMBASE+0x10)	/* ROM console error code base	*/
#define EOS_DUPCONS		 EOS_CONSBASE		/* duplicate console found		*/
#define EOS_NOCONS		(EOS_CONSBASE+1)	/* console module not found		*/
#define EOS_NO2CONS		(EOS_CONSBASE+2)	/* cons2 entry not found		*/
#define EOS_UNEXPINT	(EOS_CONSBASE+3)	/* unexpected interrupt			*/

#define EOS_BOOTBASE	(EOS_ROMBASE+0x20)	/* ROM bootsys error code base	*/
#define EOS_DUPBOOTSYS	 EOS_BOOTBASE		/* duplicate boot system found	*/
#define EOS_NOBOOTSYS	(EOS_BOOTBASE+1)	/* boot system not found		*/
#define EOS_NOBOOTERS	(EOS_BOOTBASE+2)	/* no booters configured		*/
#define EOS_NOBOOTLIST	(EOS_BOOTBASE+3)	/* bootlist entry not found		*/
#define EOS_NOROMLIST	(EOS_BOOTBASE+4)	/* romlist entry not found		*/
#define EOS_NOTKERNEL	(EOS_BOOTBASE+5)	/* not kernel module			*/
#define EOS_PSEUDOOK	(EOS_BOOTBASE+6)	/* pseudo booter completed OK	*/

#define EOS_PROTOBASE	(EOS_ROMBASE+0x30)	/* Protocols error code base 	*/
#define EOS_TCPCNF		 EOS_PROTOBASE		/* TCP conn entry not found 	*/
#define EOS_TCPCLF		(EOS_PROTOBASE+1)	/* TCP conn list full 			*/
#define EOS_HPTALREADY	(EOS_PROTOBASE+2)	/* HL Protoman already inited	*/

#define EOS_NTFYBASE	(EOS_ROMBASE+0x40)	/* Notification error code base	*/
#define EOS_NTFYALREADY	 EOS_NTFYBASE		/* Notify svcs already inited	*/
#define EOS_NONTFYRECS	(EOS_NTFYBASE+1)	/* No available notify records	*/
#define EOS_NONTFIER	(EOS_NTFYBASE+2)	/* Notifier not registered		*/
#define EOS_NTFYREGD	(EOS_NTFYBASE+3)	/* Notifier already registered	*/

#define EOS_CNFGBASE	(EOS_ROMBASE+0x50)	/* Configuration error codes	*/
#define EOS_DUPCNFG		 EOS_CNFGBASE		/* duplicate config services	*/
#define EOS_NOCNFGSVC	(EOS_CNFGBASE+1)	/* Config Services not found	*/
#define EOS_UNKCNFG		(EOS_CNFGBASE+2)	/* unknown configuration param	*/
#define EOS_NOIFACE		(EOS_CNFGBASE+3)	/* HW Interface not found 		*/

#define EOS_TIMERBASE	(EOS_ROMBASE+0x60)	/* Timer error code base 		*/
#define EOS_DUPTIMER	 EOS_TIMERBASE		/* duplicate timer module 		*/
#define EOS_TMRINUSE	(EOS_TIMERBASE+1)	/* timer already initialized	*/
#define EOS_NOTIMER		(EOS_TIMERBASE+2)	/* no timer module present		*/

#define EOS_DBGBASE		(EOS_ROMBASE+0x70)	/* Debugger error code base		*/
#define EOS_DBGENTFAIL	 EOS_DBGBASE		/* Debug entry init failure		*/

#define EOS_EXCPTBASE	(EOS_ROMBASE+0x80)	/* Exception error code base	*/
#define EOS_DUPEXCPT	 EOS_EXCPTBASE		/* duplicate excption module	*/
#define EOS_NOEXCPTSVC	(EOS_EXCPTBASE+1)	/* no exception module found	*/

#define EOS_COMPRBASE	(EOS_ROMBASE+0x90)	/* compression error code base	*/
#define EOS_NOCOMPR 	(EOS_COMPRBASE+0) 	/* no compression services      */
#define EOS_COMPRUKN	(EOS_COMPRBASE+1)	/* unknown service request  	*/

extern int errno;

#if defined(_ANSI_EXT) && defined(_OSK)

#if !defined(_TYPES_H)
#include <types.h>
#endif

error_code _os_perr(path_id, u_int32);
#endif

#endif /* _ROMERRNO_H */

