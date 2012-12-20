/*---------------------------------------------------------------------------,
|                                                                            |
|              Copyright 1999 by Microware Systems Corporation.              |
|              All Rights Reserved.   Reproduced Under License.              |
|                                                                            |
|  This software is confidential property of Microware Systems Corporation,  |
|  and is provided under license for internal development purposes only.     |
|  Reproduction, publication, modification, distribution, or creation of     |
|  derivative works in any form to any party other than the Licensee is      |
|  strictly prohibited, unless expressly provided for under the terms of a   |
|  mutually executed written license agreement for this software between     |
|  the end-user and Microware Systems Corporation, or otherwise expressly    |
|  authorized in writing by Microware Systems Corporation.  Licensee shall   |
|  reproduce this copyright notice exactly as provided herein on any copies  |
|  of the software that Licensee is authorized to distribute.                |
|                                                                            |
|----------------------------------------------------------------------------|
|                                                                            |
| Edition History:                                                           |
| #   Date     Comments                                                 By   |
| --- -------- -------------------------------------------------------- ---- |
|   1 95/05/10 Created                                                  rcb  |
|   2 95/06/26 Protected non-prototypes from ANSI-like modes            rry  |
|   3 96/05/13 Changed for optional prototypes                          rry  |
|   4 98/02/25 Added this header                                        nan  |
|   5 98/02/25 setpwent() return type changed from void to int          nan  |
|   6 99/09/20 Made (char *) args const in getpwnam() getnextpwnam().   cdh  |
|              $$                    <RELEASE_INFO>                     $$   |
`---------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------,
! edition history                                                           !
!   #   date     comments                                               by  !
!  -- -------- -------------------------------------------------------- --- !
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
 * Current Version: @(#)pwd.h	1.4 1/14/93
 */

#if !defined(_PWD_H)
#define _PWD_H

struct passwd {
  char *pw_name;
  char *pw_passwd;
  int  pw_uid;
  int  pw_gid;
  int  pw_prio;                 /* OS9: Priority of entry */
  char *pw_age;
  char *pw_comment;
  char *pw_gecos;
  char *pw_dir;
  char *pw_xdir;                /* OS9: Execution Directory */
  char *pw_shell;
  char *pw_junk;                /* everything else... */
};

#define PASSWD	"SYS/password"

#define PW_TIME     't'
#define PW_FLAG     'f'
#define PW_MOTD     'm'
#define PW_NAME     'n'
#define PW_COMMENT  'c'
#define PW_GECOS    'g'


struct gecos {
  char *fullname;
  char *organization;
  char *office_phone;
  char *home_phone;
};

#if defined(__cplusplus) || ((defined(_ANSI_EXT) || defined(__STDC__)) && defined(_OPT_PROTOS))
#if !defined(_STDIO_H)
#include <stdio.h>
#endif
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void endpwent _OP((void));
struct passwd *fgetpwent _OP((FILE *));
struct passwd *getnextpwnam _OP((const char *));
struct passwd *getpwent _OP((void));
int getpw _OP((int, char *));
struct passwd *getpwnam _OP((const char *));
struct passwd *getpwuid _OP((unsigned int));
int setpwent _OP((void));

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _PWD_H */
