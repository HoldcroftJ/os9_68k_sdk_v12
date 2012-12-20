/*--------------------------------------------------------------------------,
!                                                                           !
!            Copyright 1992-2001 by Microware Systems Corporation           !
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
!	2 92-06-23	Changes for _ANSI_EXT pre-processor symbol				rcb !
!	3 92-08-14	Changed _chcode to __charcodes							rcb !
!	4 93-04-14	Now using value ranges instead of old table for kanji	rcb !
!	5 93-08-13	Changed _isascii; added euc macros						rcb !
!	6 95-02-06	Added conditionals for C++								rcb !
!	7 98-02-20	Added this header										nan !
!	8 00-03-21	Added support for threads.								ajk !
!   9 01-03-23  Added function proto visibility to back compatible mode jhd !
!  10 01-03-26  Removed _OS9THREAD conditional from function prototypes rry !
!  11 01-03-27  Added check for backwards-compat mode to fcn prototypes pjw !
`--------------------------------------------------------------------------*/

#if !defined(_CTYPE_H)
#define _CTYPE_H

extern  unsigned char    *__charcodes;      /* in chcodes.r */
extern	unsigned char	 __kanji_tmp;

#define _CONTROL       0x01
#define _UPPER         0x02
#define _LOWER         0x04
#define _DIGIT         0x08
#define _WHITE         0x10
#define _PUNCT         0x20
#define _HEXDIG        0x40
#define _SPACE		   0x80

#define _KANA		   0x01
#define _SJIS1		   0x02
#define _SJIS2		   0x04

#if !defined(__STDC__) && !defined(__cplusplus)
#define isascii(c)		((unsigned)(c)<=0x7f)
#define toascii(c)		((c)&0x7f)
#endif

#if defined(__STDC__) || defined(_ANSI_EXT) || defined(__cplusplus)
#define _OP(X) X
#else
#define _OP(X) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#if defined(_OPT_PROTOS) || defined(__STDC__) || defined(_ANSI_EXT) || defined(__cplusplus)
int isalnum _OP((int c));
int isalpha _OP((int c));
int iscntrl _OP((int c));
int isdigit _OP((int c));
int isgraph _OP((int c));
int islower _OP((int c));
int isprint _OP((int c));
int ispunct _OP((int c));
int isspace _OP((int c));
int isupper _OP((int c));
int isxdigit _OP((int c));
int tolower _OP((int c));
int toupper _OP((int c));

int _isjis_kana _OP((int c));
int _issjis1 _OP((int c));
int _iseuc1 _OP((int c));

#endif /* _OPT_PROTOS */

#undef _OP

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#define isalpha(c)		((__charcodes+1)[(c)]&(_UPPER|_LOWER))
#define isupper(c)		((__charcodes+1)[(c)]&_UPPER)
#define islower(c)		((__charcodes+1)[(c)]&_LOWER)
#define isdigit(c)		((__charcodes+1)[(c)]&_DIGIT)
#define isxdigit(c)		((__charcodes+1)[(c)]&_HEXDIG)
#define isspace(c)		((__charcodes+1)[(c)]&(_WHITE|_SPACE))
#define isalnum(c)		((__charcodes+1)[(c)]&(_UPPER|_LOWER|_DIGIT))
#define isprint(c)		((__charcodes+1)[(c)]&(_SPACE|_PUNCT|_UPPER|_LOWER|_DIGIT))
#define isgraph(c)		((__charcodes+1)[(c)]&(_PUNCT|_UPPER|_LOWER|_DIGIT))
#define iscntrl(c)		((__charcodes+1)[(c)]&_CONTROL)
#define ispunct(c)		((__charcodes+1)[(c)]&_PUNCT)

#define _toupper(c)		((c)&0xdf)
#define _tolower(c)		((c)|0x20)
#define _isascii(c)		((unsigned char)(c)<=0x7f)
#define _toascii(c)		((c)&0x7f)

#define _isjis_kana(c)	((unsigned char)(__kanji_tmp = c)>=0xa1 && \
						(unsigned char)(__kanji_tmp)<=0xdf)

#define _issjis1(c)		(((unsigned char)(__kanji_tmp = c)>=0x81 && \
						(unsigned char)(__kanji_tmp)<=0x9f) || \
						((unsigned char)(__kanji_tmp)>=0xe0 && \
						(unsigned char)(__kanji_tmp)<=0xfe))

#define _issjis2(c)		((unsigned char)(c)>=0x40)

/* support EUC code */
#define _iseuc_kana(c)	((unsigned char)(c)==0x8e)

#define _iseuc1(c)		(((unsigned char)(__kanji_tmp = c)>=0xa1 && \
						(unsigned char)(__kanji_tmp)<=0xfe))

#define _iseuc2(c)		_iseuc1(c)

#if !defined(_CTYPE_MACROS) && defined(_OS9THREAD)
/*
 * These macros are not thread-safe, eliminate the macro versions if
 * threading is being used.
 */
#undef _isjis_kana
#undef _issjis1
#undef _iseuc1
#endif /* !_CTYPE_MACROS && _OS9THREAD */

#endif /* _CTYPE_H */

