/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1998-2001 by Microware Systems Corporation         |
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
|  01 01/03/01 Added edition history and const where appropriate       mgh  |
|  02 01/10/03 CF9159: Change tputs prototype from "int" to "char"     and  |
|                      to match tputs() implementation in termlib.l         |
|                      Sending putchar() as a third parameter to tputs()    |
|                      from now on generates a warning.                     |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/
/* termcap.h - termimal capability database definitions */
#if !defined(_TERMCAP_H)
#define _TERMCAP_H

extern char PC_, *BC, *UP;
extern short ospeed;

#if defined(__cplusplus) || defined(_ANSI_EXT) || defined(__STDC__)
#define _CONST const
#else
#define _CONST
#endif

#if defined(__cplusplus) || ((defined(_ANSI_EXT) || defined(__STDC__)) && defined(_OPT_PROTOS))
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

int tgetent _OP((char *, _CONST char *));
int tgetflag _OP((_CONST char *));
int tgetnum _OP((_CONST char *));
char *tgetstr _OP((_CONST char *, char **));
char *tgoto _OP((_CONST char *, int, int));
void tputs _OP((_CONST char *, int, int (*)(char)));

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _TERMCAP_H */
