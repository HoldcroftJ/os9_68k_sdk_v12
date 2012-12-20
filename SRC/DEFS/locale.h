/* ANSI - <locale.h> */

#if !defined(_LOCALE_H)
#define _LOCALE_H

/*--------------------------------------------------------------------------,
!                                                                           !
!               Copyright 1998 by Microware Systems Corporation             !
!                           Reproduced Under License                        !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!---------------------------------------------------------------------------!
!                                                                           !
! edition history                                                           !
!   #   date     comments                                               by  !
!  -- -------- -------------------------------------------------------- --- !
!   1 92-03-17  Created                                                 rcb !
!	2 93-05-26	New euc support											rcb !
!	3 95-02-06	Added conditionals for C++								rcb !
!	4 96-10-21	Removed superflouos comma in ENUM (strict ANSI)			tsb !
!   5 98-02-20  Added this header                                       nan !
!                                                                           !
`--------------------------------------------------------------------------*/

#include "ansi_c.h" /* get NULL if not already defined */

#define	LC_ALL		0
#define	LC_COLLATE	1
#define	LC_CTYPE	2
#define	LC_MONETARY	3
#define	LC_NUMERIC	4
#define	LC_TIME		5
#define	_LC_COUNT	6				/* (must be last. Total of above options) */

struct lconv {
	char							/* LC_ALL sets this entire structure */
		*decimal_point,				/* LC_NUMERIC sets these two */
		*thousands_sep,
		*grouping,					/* LC_MONETARY sets from here to n_sign_posn */
		*int_curr_symbol,
		*currency_symbol,
		*mon_decimal_point,
		*mon_thousands_sep,
		*mon_grouping,
		*positive_sign,
		*negative_sign,
		int_frac_digits,
		frac_digits,
		p_cs_precedes,
		p_sep_by_space,
		n_cs_precedes,
		n_sep_by_space,
		p_sign_posn,
		n_sign_posn;
	unsigned char
		collate_method,		/* LC_COLLATE sets this */
		character_type,		/* LC_CTYPE sets this */
		time_representation;/* LC_TIME sets this */
};


/* ----- possible values for 'collate_method' ----- */

enum
{
	_LC_CCOLLATE						/* collate exactly as "C" locale does */
} ;

/* ----- possible values for 'character_type' ----- */

enum
{
	_LC_ASCII,						/* ascii characters */
	_LC_SJIS,						/* Shifted JIS and EUC - NOTE: only */
	_LC_EUC							/* available with the multibyte functions */
									/* strcoll etc. do not support this type. */
} ;

enum
{
	_LC_ANSITIME					/* ansi time format */
} ;

#if defined(__STDC__) || defined(_ANSI_EXT) || defined(__cplusplus)
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
char *setlocale(int, const char *);
struct lconv *localeconv(void);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#else
char *setlocale();
struct lconv *localeconv();
#endif /* __STDC__ */

#endif /* _LOCALE_H */


