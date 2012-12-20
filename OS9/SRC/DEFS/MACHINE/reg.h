/*--------------------------------------------------------------------------,
|                                                                           |
|            Copyright 1998-2001 by Microware Systems Corporation           |
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
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
|  01 98/11/16 Added edition history.                                  wwb  |
|				---- OS-9/68K V3.1 Release ----								|
|  02 00/09/28 Added prototype for get_sysglobs                        rds  |
|  03 01/02/06 Added _PROC_HAS_CP, *_const, and *_addr				   rry  |
|  04 01/07/18 Change get_sysglobs prototype to return Sysglobs        mgh  |
|              Added include of sysglob.h                                   |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

#if !defined(_REG_H)
#define _REG_H

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if !defined(_SYSGLOB_H)
#include <sysglob.h>
#endif

#define T_CLDSP		0
#define T_CLDPC		1
#define T_BUSERR	2
#define T_ADDERR	3
#define T_ILLINS	4
#define T_ZERDIV	5
#define T_CHK		6
#define T_TRAPV		7
#define T_PRIV		8
#define T_TRACE		9
#define T_1010		10
#define T_1111		11
#define T_CPPROTV	13
#define T_FMTERROR	14
#define T_UNINIRQ	15
#define T_SPURIO	24
#define T_AUTIRQ	25
#define T_TRAP		32
#define T_FPUNORDC	48
#define T_FPINXACT	49
#define T_FPDIVZER	50
#define T_FPUNDRFL	51
#define T_FPOPRERR	52
#define T_FPOVERFL	53
#define T_FPNOTNUM	54
#define T_VCTIRQ	64
#define T_END		256
#define T_SIZE		256	/* 256 4 byte entries */

/* user exception handler definitions */
#define TRAP_MIN	T_BUSERR	/* minimum user catchable trap number */
#define TRAP_MAX	T_1111		/* maximum user catchable trap number */
#define TRAP_TTL	10			/* total number of user catchable traps */

/* floating point coprocessor user exception handler definitions */
#define FTRAP_MIN	T_FPUNORDC	/* minimum user catchable FPU trap number */
#define FTRAP_MAX	T_FPNOTNUM	/* maximum user catchable FPU trap number */
#define FTRAP_TTL	7			/* total number of user catchable FPU traps */

/* Machine related definitions (68000-specific for now) */
typedef struct {
    u_int32 d[8];				/* data registers */
    void *a[8];				/* address registers */
    u_int16 sr;				/* status register */
    u_int32 pc;				/* status register/program counter */
    u_int32 ssp,usp,isp,msp;	/* stack pointers */
    u_int32 vbr,cacr,caar;	/* cache control registers */
    u_int32 sfc,dfc;			/* alternate function registers */
	u_int16 xxx;			/* <unused> to make this struct%4==0 */
} REGISTERS;

typedef u_char freg[12];	/* 68881 80-bit (96 external) floating register */

typedef struct  {
	freg fp[8];				/* 68881 registers */
	u_int32 fpcr, fpsr, fpiar;/* fpu control registers */
	u_char fpstate;			/* coprocessor frame state flag */
} FLOATREGS;

/* This define indicates that this processor does not have a constant pointer */
#define _PROC_HAS_CP 0

/* To avoid ifdef'ing, these macros will replace calls with non-operations */
#define _get_const() ((void *)0)
#define _change_const(x) ((void *)0)
#define get_const() ((void *)0)
#define change_const(x) ((void *)0)

/* This processor does not have this concept, replace uses with plain address */
#define user_addr(x) ((void *)(x))
#define system_addr(x) ((void *)(x))

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(_cplusplus)
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(_OPT_PROTOS)
u_int8 inc _OP((void *));
u_int16 inw _OP((void *));
u_int32 inl _OP((void *));
void outc _OP((void *, u_int32));
void outw _OP((void *, u_int32));
void outl _OP((void *, u_int32));
void *change_static _OP((void *));
void *get_static _OP((void ));
status_code irq_change _OP((status_code));
void irq_disable _OP((void));
void irq_enable _OP((void));
status_code irq_maskget _OP((void));
void irq_restore _OP((status_code));
status_code irq_save _OP((void));
#endif

Sysglobs get_sysglobs _OP((void));	  /* returns a (sysglobs *) */

error_code _os9_irq _OP((u_int32, u_int32, void *, void *, void *));
error_code _os_firq _OP((u_int32, u_int32, void *, void *));
error_code _os_sysid _OP((u_int32 *, u_int32 *, u_int32 *, u_int32 *, u_int32 *, int32 *, u_int32 *, u_int32 *, char *, char *, char *));

#if defined(__cplusplus)
}
#endif

#undef _OP

#endif /* _REG_H */
