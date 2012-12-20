/*--------------------------------------------------------------------------,
! edition history                                                           !
!   #   date     comments                                               by  !
!  -- -------- -------------------------------------------------------- --- !
!   1 95-05-10  Created                                                 rcb !
!   2 95-06-23  Changed to EOS versions & added prototypes for vararg   rry !
!   3 96-05-13  Added optional prototypes                               rry !
!   4 97-04-08  Prototypes now defined when using C++                   tag !
!   5 97-06-06  Max # of files for OS-9000 increased to 256             tsb !
!   6 98-02-20  Added this header                                       nan !
!   7 00-05-25  Removed error mappings which are now in errno.h         rds !
!   8 01/03/05  Add const as appropriate								mgh !
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
 * Current Version: @(#)os9def.h	1.3 12/23/91
 */

#if !defined(_OS9DEF_H)
#define _OS9DEF_H

#define F_OK   0
#define X_OK   S_IEXEC
#define W_OK   S_IWRITE
#define R_OK   S_IREAD

#if defined(_OSK)
#define O_RDWR   S_IREAD|S_IWRITE
#define	O_WRONLY S_IWRITE
#define O_RDONLY S_IREAD
#define	O_CREAT	 0
#define	O_TRUNC	 0
#endif

#define seteuid(uid) setuid(uid)
#define geteuid() getuid()
#define vfork() fork()
#define srandom(seed) srand(seed)
#define random() rand()
#define sigsetmask(mask) sigmask(mask)
#define killpg(pid,sigcode) kill(pid,sigcode)

#ifdef _OSK
#define NOFILE  32
#elif defined(_OS9000)
#define NOFILE  256
#else
#error Unknown OS
#endif

#define MAXPATHLEN 1024 /* not really a limit on OS9 */

#if	!defined(_CADDR_T)
#	define	_CADDR_T		
typedef char *  caddr_t;
#endif	/* _CADDR_T */

struct iovec {
	caddr_t   iov_base;
	int  iov_len;
};

#include <UNIX/os9time.h>

#define LowerCase(a, b) OS9LowerCase(a, b, sizeof(b))

/*
 * Macros for fast min/max.
 */
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#if defined(__cplusplus)
extern "C" {
#	define _OPT_PROTOS			/* C++ needs prototypes */
#endif /* __cplusplus */

#if defined(_OPT_PROTOS)
#	include <stdio.h>			/* <stdio.h> undefs _OP */
#endif

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#define _CONST const
#else
#define _OP(x) ()
#define _CONST
#endif


#if defined(_OPT_PROTOS)

unsigned int alarm _OP((unsigned int));
void *alloca _OP((unsigned int));
int bcmp _OP((void *, void *, unsigned int));
int bcopy _OP((void *, void *, unsigned int));
char *buildpath _OP((_CONST char *));
void bzero _OP((void *, unsigned int));
double cbrt _OP((double));
void crtolf _OP((char *, unsigned int));
int dup2 _OP((int, int));
int execv _OP((_CONST char *, char * _CONST[]));
int execve _OP((_CONST char *, char * _CONST[], char * _CONST[]));
int execvp _OP((_CONST char *, char * _CONST[]));
int ffs _OP((unsigned int));
unsigned int getgid _OP((void));
int getopt _OP((int, char**, char *));
unsigned int getppid _OP((void));
char *getwd _OP((char[]));
int ioctl _OP((unsigned int, unsigned int, caddr_t));
int isatty _OP((int));
void lftocr _OP((char *, int));
int pclose _OP((FILE *));
int pipe _OP((unsigned int [2]));
FILE *popen _OP((_CONST char *, _CONST char *));
int putenv _OP((_CONST char *));
int readv _OP((unsigned int, struct iovec *, unsigned int));
int setgid _OP((unsigned int));
char *tempnam _OP((char *, char *));
int writev _OP((unsigned int, struct iovec *, unsigned int));
#endif

int execl(_CONST char *path, _CONST char *arg0, ...);
int execle(_CONST char *path, _CONST char *arg0, ...);

#if defined(__cplusplus)
}
#	undef _OPT_PROTOS
#endif /* __cplusplus */

#undef _OP

#endif /* _OS9DEF_H */
