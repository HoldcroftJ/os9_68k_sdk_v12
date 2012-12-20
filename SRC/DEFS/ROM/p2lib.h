/*--------------------------------------------------------------------------,
|                                                                           |
|            Copyright 1995-2000 by Microware Systems Corporation           |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, distribution, or creation of derivative works |
|  in any form to any party other than the licensee is strictly prohibited, |
|  unless expressly authorized in writing by Microware Systems Corporation. |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| p2lib.h - public functions for P2 modules                                 |
|                                                                           |
| Edition History:                                                          |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
| 01 95/07/11 History started                                         gls   |
| 02 95/10/17 Fixed ISYNC_M() function prototype.                     ats   |
|             ---- OS-9/68K V3.0.2 System-state Debug Support Release ---   |
|             ---- OS-9000/PPC V2.0 Release ----                            |
|             ---- OS-9000/x86 V2.1 Released ----                           |
| 03 96/10/10 Added prototype for set_context().                      gdb   |
| 04 96/10/14 Changed set_context() to setcontext() for consistency.  gdb   |
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
|             ---- OS-9/68K V3.0.3 System-state Debug Support Release ---   |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
| 05 97/09/18 Added storexcpt() prototype for SPARC.                  gdb   |
| 06 97/10/10 Updated prototype for setcontext().                     ats   |
|             Added prototypes for findrinf() and os_getrinf().             |
|             ---- Modular Rom Sub-system v1.0 Released ----                |
|             ---- MBX_BSP_Beta Released ----                               |
| 07 98/02/03 Added rom_udiv() prototype.                             gdb   |
|             ---- MBX_BSP Released ----                                    |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- OS-9000/SPARC V2.2.7 Released ----                       |
|             ---- Modular Rom v1.1 Sub-component Released ----             |
|             ---- Modular ROM v1.2 Sub-component Released ----             |
| 08 04/28/99 Added prototype for get_sysbglobs.                      sr    |
| 09 06/06/00 Changed get_sysglobs to return (void *).                rkw   |
| 10 11/12/01 Changed get_sysglobs for 68K to return Sysglobs *		  rry   |
|             $$                      <RELEASE_INFO>                   $$   |
`--------------------------------------------------------------------------*/

extern error_code	findrinf(Rominfo *rinf_p);
extern error_code	getrinf(Rominfo *rinf_p);
extern error_code	os_getrinf(Rominfo *rinf_p);

/* probe types */
#define HWPROBE_8BITS	1
#define	HWPROBE_16BITS	2
#define	HWPROBE_32BITS	3
extern error_code	hwprobe(void *addr, u_int32 ptype, Rominfo rinf);

extern char			*inttoascii(u_int32 value, char *bufptr);

#ifdef	_MPFPOWERPC
extern void			ISYNC_M(void);
#else
#define	ISYNC_M()
#endif

extern void			outhex(u_char nibble, Rominfo rinf);
extern void			out1hex(u_char byte, Rominfo rinf);
extern void			out2hex(u_int16 word, Rominfo rinf);
extern void			out4hex(u_int32 longword, Rominfo rinf);

extern unsigned		rom_udiv(unsigned dividend, unsigned divisor);
#if		defined(_MPFSPARC)
#define	ROM_UDIV(a,b)	(rom_udiv(a, b))
#else
#define	ROM_UDIV(a,b)	(a/b)
#endif

extern u_int32 		setexcpt(u_int32 vector, u_int32 irqsvc, Rominfo rinf);
#if defined(_MPFPOWERPC) || defined(_MPFSPARC)
extern u_int32 		storexcpt(u_int32 vector, u_int32 irqsvc, Rominfo rinf);
#endif
#ifdef	_MPFPOWERPC
#define	resetexcpt	storexcpt
#else
#define	resetexcpt	setexcpt
#endif

extern u_char		*swap_globals(u_char *newglbls);

#ifdef _OS9000
extern void   		*get_sysglobs(void);
#else

#if !defined(_SYSGLOB_H)
#include <sysglob.h>
#endif

extern Sysglobs		get_sysglobs(void);

#endif

#ifdef NOROMINFO
error_code p2start(Sysglobs); /* only to be used by the rominfo P2 module */
#else
error_code p2start(Rominfo rinf, u_char *glbls);
#endif

#ifndef _SETJMP_H
#include <setjmp.h>
#endif
extern void setcontext(jmp_buf, u_int32, void*, Rominfo);
