/*--------------------------------------------------------------------------,
! edition history                                                           !
!   #   date     comments                                               by  !
!  -- -------- -------------------------------------------------------- --- !
!   1 95-05-10  Created                                                 rcb !
!	2 95-06-26	Protected non-prototypes from ANSI-like modes			rry !
!	3 96-05-12	Added optional prototypes								rry !
!	4 97-06-19	"regexex" changed to "regexec"							tsb !
!   5 98-02-20  Added this header                                       nan !
!	6 01-03-05  Added const as appropriate								mgh !
!                                                                           !
`--------------------------------------------------------------------------*/

/*
 * OS-9/OS-9000 implementation specific code, enhancements and modifications:
 *
 * Copyright 1991 Microware Systems Corporation
 * All Rights Reserved
 *
 * Use and/or distribution subject to terms and conditions of applicable
 * Microware license agreement(s).
 *
 * Current Version: @(#)regexp.h	1.3 12/23/91
 */

#if !defined(_REGEXP_H)
#define _REGEXP_H

/*
 * The first byte of the regexp internal "program" is actually this magic
 * number; the start node begins in the second byte.
 */
#define	MAGIC	0234


/*
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 */

#define NSUBEXP  10
typedef struct regexp {
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} regexp;

#if defined(__cplusplus) || ((defined(_ANSI_EXT) || defined(__STDC__)) && defined(_OPT_PROTOS))
#define _OP(x) x
#define _CONST const
#else
#define _OP(x) ()
#define _CONST
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

regexp *regcomp _OP((_CONST char *));
void regerror _OP((_CONST char *));
int regex _OP((regexp *, char *));
int regexec _OP((regexp *, _CONST char *));

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _REGEXP_H */
