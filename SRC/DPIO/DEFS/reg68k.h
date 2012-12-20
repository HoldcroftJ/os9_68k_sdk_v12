/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1988 by Microware Systems Corporation              |
|                          Reproduced Under License                         |
|                                                                           |
|  This source code is the proprietary confidential property of Microware   |
|  Systems Corporation, and is provided to licensee for documentation and   |
|  educational purposes only. Reproduction, publication, or distribution    |
|  in any form to any party other than the licensee is strictly prohibited. |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
|  Edition History:                                                         |
|   #   Date    Comments                                                By  |
|  -- -------- -------------------------------------------------------- --- |
|   1 88/02/01 Created                                                  rg  |
|   2 89/09/06 Added definitions for stack frame format.                afh |
|   3 89/10/04 Added defs for format error and coprocessor prot viol.   rcb |
|   4 95/08/28 Added C++ support                                        dwj |
|   5 00/08/22 Added prototypes from /mwos/OS9/SRC/DEFS/MACHINE/reg.h   rds |
|   6 00/09/28 Added prototype for get_sysglobs                         rds |
|                                                                           |
`---------------------------------------------------------------------------*/

#if !defined(_REG_H)
#define _REG_H

#if !defined(_TYPES_H)
#include <types.h>
#endif

#define T_COLDSP	0
#define T_COLDPC	1
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
#define T_COPROTV	13
#define T_FMTERR	14
#define T_UNIRQ		15
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
#define TRAP_MIN	T_COLDSP	/* minimum user catchable trap number */
#define TRAP_MAX	T_UNIRQ		/* maximum user catchable trap number */
#define TRAP_TTL	16			/* total number of user catchable traps */

/* floating point coprocessor user exception handler definitions */
#define FTRAP_MIN	T_FPUNORDC	/* minimum user catchable FPU trap number */
#define FTRAP_MAX	T_FPNOTNUM	/* maximum user catchable FPU trap number */
#define FTRAP_TTL	7			/* total number of user catchable FPU traps */

#define status_reg	u_int16 /* status register image type */

/* Machine related definitions (68000-specific for now) */
typedef struct {
    u_int32 
    	r_d[8];		/* data registers */
    u_char 
    	*r_a[7];	/* address registers - less a7 */
    u_char 
    	*r_usp;		/* user stack pointer */
    status_reg 
    	r_sr;		/* status register */
    u_int32 
    	r_pc;		/* status register/program counter */
    u_int16 
    	r_fmt;		/* size of used execption stack */
} regs, *Regs;

#define stat_reg(sp) (sp->r_sr)
#define userstack(sp) (sp->r_usp)

typedef u_char freg[12];	/* 68881 80-bit (96 external) floating register */

typedef struct  {
	freg 
		fp[8];		/* 68881 registers */
	u_int32 
		fpcr, 
		fpsr, 
		fpiar;		/* fpu control registers */
	u_char 
		fpstate;	/* coprocessor frame state flag */
} fregs, *Fregs;

/* definitions for the fp_flags field below */
#define FPU_PRIM 0x00000001 /* identfies FPU context as a "mainline" context */

/* 68881/68882 floating point descriptor definitions */
typedef struct fpudesc *Fpu_desc, fpu_desc;

struct fpudesc  {
	u_int32
		fp_memsz;	/* size of fpu descriptor block */
	u_int32
		fp_icptcnt,	/* intercept routine recursion count */
		fp_flags;	/* bit flags field */
	Fpu_desc
		fp_next;	/* pointer for 2nd save area (used for signal handling) */
	fregs
		fp_regs;	/* FPU registers: fp0-fp7, fpcr, fpsr, fpiar, fpstate */
	u_char
		fp_contxt[216];/* FPU state frame (68881 = 184, 68882 = 216) */
};


/* Status Register Fields */
#define TRACEBIT 7 			/* trace bit in system byte */
#define SUPVRBIT 5 			/* supervisor bit in system byte */

#define TRACE 		0x8000		/* trace mode */
#define SUPERVIS	0x2000		/* supervisor state */
#define INTMASK 	0x0700		/* interrupt mask */
#define INTENAB 	~INTMASK
#define EXTEND		0x1000		/* Extend bit */
#define NEGATIVE 	0x0800		/* Negative flag */
#define ZERO 		0x0400		/* Zero flag */
#define TWOSOVFL	0x0200		/* Two's Comp Overflow flag */
#define CARRY		0x0100		/* Carry bit */
#define NOCARRY 	~Carry
#define DBGSRMASK	0xFF00		/* mask for copying sr for debug processes */
#define FMTMASK		0xF000		/* mask for stack frame format */
#define MIDIFMT		0x9000		/* mid-instruction stack frame format */

#define settrace(sr) (sr |= TRACE)
#define cleartrace(sr) (sr &= ~TRACE)
#define setsupervise(sp) (stat_reg(sp) |= SUPERVIS)
#define issupervise(sp) (stat_reg(sp) & SUPERVIS)

/* F$CCtl cache control definitions */
#define CACHENAB	0x01	/* enable cache */
#define CACHFRZ 	0x02	/* freeze cache */
#define CACHCLR 	0x08	/* clear cache */

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(_cplusplus)
#define _OP(x) x
#else
#define _OP(x) ()
#endif

/*	C++ support	*/
#ifdef __cplusplus
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
void *get_sysglobs _OP((void));	  /* returns ptr to a sysglobs struct */
#endif

error_code _os_cache _OP((u_int32));
error_code _os_irq _OP((u_int32, u_int32, void *, void *));
error_code _os_scache _OP((u_int32, void (**)(), void **));
error_code _os_sysid _OP((u_int32 *, u_int32 *, u_int32 *, u_int32 *,
		u_int32 *, int32 *, u_int32 *, u_int32 *, char *, char *, char *));

/*	End C++	*/
#ifdef __cplusplus
}
#endif

#undef _OP

#endif /* _REG_H */
