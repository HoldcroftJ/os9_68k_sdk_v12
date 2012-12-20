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
!	2 92-06-24	Changes for _ANSI_EXT preprocessor symbol				rry !
!	3 93-01-21	New prototypes to avoid warning messages				rry !
!	4 93-03-31	Changed putc & getc to cast refs to FILE pointer		rry !
!	5 95-02-06	Added conditionals for C++								rcb !
!	6 96-05-13	Added optional prototypes (cleaned up existing ones)	rry !
!	7 96-06-03	Corrected prototype for string functions (for ANSI)		ra  !
!	8 96-11-04	New CONST support in function prototypes (ANSI strict)	tsb !
!   9 98-02-20  Added this header                                       nan !
!  10 98-02-25	Had to re-define some macros to avoid cpfe error		nan !
!  11 98-08-11  Thread support added                                    rry !
!  12 01/03/02	Add const as appropriate								mgh !
!                                                                           !
`--------------------------------------------------------------------------*/

#if !defined(_STDIO_H)
#define _STDIO_H

#if defined(NULL) && !defined(_ANSI_H) /* defined by non-ansi header, i.e const.h */
#undef NULL
#endif /* NULL */

/* get NULL and size_t */
#include "ansi_c.h"

#if defined(_SIZE_T)
typedef _SIZE_T	size_t;
#undef _SIZE_T
#endif /* _SIZE_T */

#define BUFSIZ 512
#define FOPEN_MAX 32
#define _NFILE FOPEN_MAX	/* our version of FOPEN_MAX */
#define FILENAME_MAX 28
#define L_tmpnam 14			/* tmpnam() only uses 14 bytes for name */
#define TMP_MAX 50

#define SEEK_SET 0			/* position relative to start of file */
#define SEEK_CUR 1			/* position relative to current position */
#define SEEK_END 2			/* position relative to end of file */

typedef unsigned long fpos_t;

typedef struct _iobuf {
    unsigned char *_ptr,	/* buffer pointer */
         		  *_base,	/* buffer base address */
         		  *_end;	/* buffer end address */
    unsigned short _flag;	/* file status */
    short _fd;				/* file path number */
	unsigned char _ungetc[2];	/* room for at least 2 ungetc characters */
	unsigned short _obufsiz;/* old buffer size (compatability, obsolete) */
	int (*_ifunc)();		/* function to use for input */
	int (*_ofunc)();		/* function to use for output */
    size_t _bufsiz;			/* size of data buffer */
	fpos_t _filepos;		/* current "real" file position of start of buffer */
	unsigned char _nungetc;	/* number of characters in _ungetc buffer */
    unsigned char *_wptr;	/* pointer to end of written data */
    unsigned char _save;	/* for 'ungetc' when unbuffered */
} FILE;

extern FILE _niob[FOPEN_MAX];

#if (defined(_MPF68K) || defined(_MPF386))
#if defined(__STDC__) || defined(_ANSI_EXT) || defined(__cplusplus)
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
extern FILE *_from_old(void *);
extern void _from_new(FILE *);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#else
extern FILE *_from_old();
extern void _from_new();
#endif
#endif

/*	the following are fake pointer variable defintions  */
#define stdin	(&_niob[0])
#define stdout	(&_niob[1])
#define stderr	(&_niob[2])

#define _READ		0x0001
#define _WRITE		0x0002
#define _UNBUF		0x0004
#define _BIGBUF		0x0008
#define _EOF		0x0010
#define _ERR		0x0020
#define _SCF		0x0040
#define _RBF		0x0080
#define _DEVMASK	(_UNBUF|_RBF|_SCF)
#define _WRITTEN	0x0100		/*	buffer written in update mode  */
#define _HEAPBUF	0x0200		/*	system allocated buffer  */
#define _BUFFREAD	0x0400		/*	buffer read (for seeks)  */
#define _BUFFWRITE	0x0800		/*	buffer write needed  */
#define _TMPFILE	0x1000		/*  file created by tmpfile() */
#define _APPEND		0x2000		/*  stream opened in append mode */
#define _INIT		0x8000		/*	_iob initialized  */
#define _BUFMASK	(_RBF|_SCF|_UNBUF)	/* mask to select buffering info */

/* buffer type macros */
#define _IOFBF _RBF			/* fully buffered */
#define _IOLBF _SCF			/* line buffered */
#define _IONBF _UNBUF		/* not buffered */


#define EOF (-1)
#define _EOL 13				/* end-of-line */

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#define _CONST const
#else
#define _OP(x) ()
#define _CONST 
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if !defined(_MPF68K) && !defined(_MPF386)
/* other processors need to see prototype for var arg functions regardless */
int		fprintf(struct _iobuf *, _CONST char *, ...);
int		fscanf(struct _iobuf *, _CONST char *, ...);
int		printf(_CONST char *, ...);
int		scanf(_CONST char *, ...);
int		sprintf(char *, _CONST char *, ...);
int		sscanf(_CONST char *, _CONST char *, ...);
#else
int		fprintf _OP((FILE *, const char *, ...));
int		fscanf _OP((FILE *, const char *, ...));
int		printf _OP((const char *, ...));
int		scanf _OP((const char *, ...));
int		sprintf _OP((char *, const char *, ...));
int		sscanf _OP((const char *, const char *, ...));
#endif

int 	remove _OP((const char *));
int 	rename _OP((const char *, const char *));
FILE 	*tmpfile _OP((void));
char 	*tmpnam _OP((char *));
int 	fclose _OP((FILE *));
int		fflush _OP((FILE *));
FILE	*fopen _OP((const char *, const char *));
FILE	*freopen _OP((const char *, const char *, FILE *));
void	setbuf _OP((FILE *, char *));
int		setvbuf _OP((FILE *, char *, int, size_t));
int		vfprintf _OP((FILE *, const char *, _VA_LIST));
int		vprintf _OP((const char *, _VA_LIST));
int		vsprintf _OP((char *, const char *, _VA_LIST));
int		fgetc _OP((FILE *));
char	*fgets _OP((char *, int, FILE *));
int		fputc _OP((int, FILE *));
int		fputs _OP((const char *, FILE *));
int		getc _OP((FILE *));
int		getchar _OP((void));
char	*gets _OP((char *));
int		putc _OP((int, FILE *));
int		putchar _OP((int));
int		puts _OP((const char *));
int		ungetc _OP((int, FILE *));
size_t	fread _OP((void *, size_t, size_t, FILE *));
size_t	fwrite _OP((const void *, size_t, size_t, FILE *));
int		fgetpos _OP((FILE *, fpos_t *));
int		fseek _OP((FILE *, long int, int));
int		fsetpos _OP((FILE *, const fpos_t *));
long int ftell _OP((FILE *));
void	rewind _OP((FILE *));
void	clearerr _OP((FILE *));
int		feof _OP((FILE *));
int		ferror _OP((FILE *));
void	perror _OP((const char *));

int _filbuf _OP((FILE *));
int _flshbuf _OP((FILE *, int));

#if defined(_OPT_PROTOS)
int _errmsg _OP((int, _CONST char *, unsigned int, unsigned int, unsigned int));
int getw _OP((FILE *));
char *mktemp _OP((_CONST char *));
int prerr _OP((int, int));
int putw _OP((int, FILE *));
#endif

/* special case since fdopen is not an ANSI function */
#if !defined(__STDC__) && !defined(__cplusplus)
FILE *fdopen _OP((int, _CONST char *));
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#if defined(__STDC__) || defined(__cplusplus)
#if (defined(_MPF68K) || defined(_MPF386))
#define clearerr(p)     do { FILE *__trans_fp = _from_old(p); __trans_fp->_flag &= ~(_ERR|_EOF); _from_new(__trans_fp); } while (0)
#else
#define clearerr(p)     ((p)->_flag &= ~(_ERR|_EOF))
#endif
#endif

#if !defined(__STDC__) && !defined(__cplusplus)

typedef short WORD;			/* always 16 bits (?) */
#define EOL 13
#define PMODE READ|WRITE    /* r/w for owner */

#if (defined(_MPF68K) || defined(_MPF386))
#define clearerr(p)     do { FILE *__trans_fp = _from_old(p); __trans_fp->_flag &= ~_ERR; _from_new(__trans_fp); } while (0)
#define cleareof(p)     do { FILE *__trans_fp = _from_old(p); __trans_fp->_flag &= ~_EOF; _from_new(__trans_fp); } while (0)
#else
#define clearerr(p)     ((p)->_flag &= ~_ERR)
#define cleareof(p)     ((p)->_flag &= ~_EOF)
#endif

#define fileno(p)       ((p)->_fd)

#endif /* !__STDC__ */

#define putc(c, p) \
	((((FILE *)(p))->_nungetc || ((FILE *)(p))->_ptr >= ((FILE *)(p))->_end || (((FILE *)(p))->_flag & (_ERR|_EOF))) ? \
		_flshbuf((FILE *)(p), (c)) : \
		(((FILE *)(p))->_flag |= _WRITTEN, \
		((((FILE *)(p))->_flag & _APPEND) ? (*(((FILE *)(p))->_ptr = ((FILE *)(p))->_wptr) = (c)) : (*((FILE *)(p))->_ptr = (c))), \
		((*((FILE *)(p))->_ptr == '\n') && (((FILE *)(p))->_flag & _IOLBF)) ? \
			_flshbuf((FILE *)(p), '\n') : \
			(((FILE *)(p))->_ptr >= ((FILE *)(p))->_wptr ? ((FILE *)(p))->_flag &= ~_BUFFWRITE, ((FILE *)(p))->_wptr = ++((FILE *)(p))->_ptr, *(((FILE *)(p))->_ptr - 1) : *((FILE *)(p))->_ptr++)))

#define getc(p)	\
	((((FILE *)(p))->_ptr >= (((FILE *)(p))->_flag & _BUFFWRITE ? ((FILE *)(p))->_wptr : ((FILE *)(p))->_end)) ? _filbuf((FILE *)(p)) : (((FILE *)(p))->_nungetc ? (int)((FILE *)(p))->_ungetc[--((FILE *)(p))->_nungetc] : (int)*(((FILE *)(p))->_ptr++)))

#define putchar(c)      putc(c,stdout)
#define getchar()       getc(stdin)
#if defined(_MPF68K) || defined(_MPF386)
#define ferror(p)       (_from_old(p)->_flag & _ERR)
#define feof(p)         (_from_old(p)->_flag & _EOF)
#define _cleareof(p)    do { FILE *__trans_fp = _from_old(p); __trans_fp->_flag &= ~_EOF; _from_new(__trans_fp); } while (0)
#else
#define ferror(p)       ((p)->_flag & _ERR)
#define feof(p)         ((p)->_flag & _EOF)
#define _cleareof(p)    ((p)->_flag &= ~_EOF)
#endif

#define _fileno(p)      ((p)->_fd)

#if !defined(_STDIO_MACROS)
#if defined(_OS9THREAD) || defined(_NO_STDIO_MACROS) || (defined(_TIME_FACTOR) && defined(_SPACE_FACTOR) && (_SPACE_FACTOR >= (2 * _TIME_FACTOR)))
#undef clearerr
#undef putc
#undef getc
#undef putchar
#undef getchar
#undef ferror
#undef feof
#endif
#endif

#endif /* _STDIO_H */
