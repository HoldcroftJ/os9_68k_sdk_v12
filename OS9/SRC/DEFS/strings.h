#if !defined(_STRINGS_H)
#define _STRINGS_H

/*
 *	External function definitions for string handling functions
 *
 */

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(_OPT_PROTOS) && !defined(_STRING_H)
#include <string.h>
#endif

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#define _CONST const
#else
#define _OP(x) ()
#define _CONST
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

error_code _os_cmpnam _OP((_CONST char *, _CONST char *, u_int32, int32 *));
error_code _os_prsnam _OP((_CONST char *, u_int32 *, char *, char **));

#if defined(_OPT_PROTOS)
int _cmpnam _OP((_CONST char *, _CONST char *, int));
int findnstr _OP((int, _CONST char *, _CONST char *, int));
int findstr _OP((int, _CONST char *, _CONST char *));
char *index _OP((_CONST char *, int));
int _parsepath _OP((_CONST char *));
int _prsnam _OP((_CONST char *));
char *rindex _OP((_CONST char *, int));
void _strass _OP((void *, void *, int));
char *strhcpy _OP((char *, _CONST char *));
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#if !defined(_OPT_PROTOS)
extern char *strcat(), *strncat(), *strcpy();
extern char *strncpy(), *index(), *rindex();
extern int strcmp(), strncmp();
extern unsigned long strlen();
#endif

#endif
