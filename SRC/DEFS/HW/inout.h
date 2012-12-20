/*
 * $Header:   /h0/MWOS/SRC/DEFS/HW/VCS/inout.h_v   1.29   22 Jun 1998 11:05:58   alexs  $
 */
/*--------------------------------------------------------------------------,
|                                                                           |
|           Copyright 1995, 1996 by Microware Systems Corporation           |
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
| inout.h : I/O macro definitions.                                          |
|																			|
| Edition History:															|
|																			|
| Ed#	Date	Changes													By	|
| --- --------	------------------------------------------------------- --- |
|  01 95/11/17  Created.                                                gdb |
|				--- OS-9/68K V3.0.2 System-state Debug Support Release ---  |
|               ---- OS-9000/PPC V2.0 Release ----                          | 
|  02 96/02/21  Added INW, OUTW, INL, OUTL                              sep |
|  03 96/04/02  Added include of regs.h for function prototypes.        gdb |
|  04 96/05/08  Added include of reg.h for OSK.                         ats |
|               ---- OS-9000/PPC V2.0.1 Release ----                        | 
|               ---- OS-9/68K V3.0.3 System-state Debug Support Release --- |
|               ---- OS-9000/x86 V2.1 Released ----                         |
|               ---- OS-9000/PPC V2.1.1 Released ----                       |
|               ---- OS-9000/ARMv3 V2.2 Released ----                       |
|               ---- OS-9000/ARMv3 V2.2.1 Released ----                     |
|               ---- OS-9000/ARMv4 Europa Board Port V1.0 Released ----     |
|               ---- OS-9000/ARMv4 Brutus Board Port V1.0 Released ----     |
|               ---- OS-9000/Europa Board Port V1.0.1 Released ----         |
|               ---- OS-9000/BRUTUS V1.0 Released ----                      |
|               ---- OS-9000/Brutus Board Support V1.0 Released ----        |
|               ---- OS-9000/SH3 V2.2.6 Released ----                       |
|               ---- MWKK OS-9000/SPARC V2.2 Alpha 2 2/24/98 Released ----  |
|               ---- OS-9000/SPARC V2.2.7 Released ----                     |
|               ---- OS-9000/SPARC V2.2.7 Released ----                     |
|               ---- OS-9000/Brutus Board Support V1.1 Released ----        |
|               ---- Chipset v1.1 Sub-component Released ----               |
|               ---- OS-9000/505EVB V1.0 Released ----                      |
|               ---- Chipset v1.2 Sub-component Released ----               |
|               $$                    <RELEASE_INFO>                     $$ |
`--------------------------------------------------------------------------*/

#ifdef	_OSK
#include <MACHINE/reg.h>
#else
#include <regs.h>
#endif

#ifndef IN
#define IN(p) inc(p)
#endif

#ifndef OUT
#define OUT(p,c) outc(p,c)
#endif

#ifndef INW
#define INW(p) inw(p)
#endif

#ifndef OUTW
#define OUTW(p,c) outw(p,c)
#endif

#ifndef INL
#define INL(p) inl(p)
#endif

#ifndef OUTL
#define OUTL(p,c) outl(p,c)
#endif

