/*--------------------------------------------------------------------------\
!																			!
!	Name: Swap.c															!
!																			!
!	Function: Swap bytes and words for processors like the 386.				!
!																			!
\--------------------------------------------------------------------------*/

/****************************************************************************
*																			*
*              Copyright 1989 by Microware Systems Corporation              *
*                         Reproduced Under License.                         *
*																			*
* This  source  code is  the proprietary confidential property of Microware	*
* Systems Corporation, and is  provided  to licensee  for documentation and	*
* educational purposes only. Reproduction, publication, or  distribution in	*
* any form to any party other than the licensee is strictly prohibited.		*
*																			*
****************************************************************************/

#include <iffrw.h>

/* These routines both swap for little endian and cope with alignment
   issues */

/****************************************************************************
 * swap longs																*
 ***************************************************************************/

IFF_U_32 iff_get32(u_char *s)
{
#ifndef _LIL_END
  if (((u_int32)s) & 0x3) /* four byte aligned? */
#endif
    return (*s<<24 | *(s+1)<<16 | *(s+2)<<8 | *(s+3));
#ifndef _LIL_END
  else
    return *((IFF_U_32*)s);
#endif
}

void iff_set32(u_char *d, IFF_U_32 s)
{
#ifndef _LIL_END
  if (((u_int32)s) & 0x3) { /* four byte aligned? */
#endif
    *d++ = (s & 0xff000000) >> 24;
    *d++ = (s & 0x00ff0000) >> 16;
    *d++ = (s & 0x0000ff00) >> 8;
    *d   = (s & 0x000000ff);
#ifndef _LIL_END
  } else
    *((IFF_U_32*)d) = s;
#endif
}

/****************************************************************************
 * swap words																*
 ***************************************************************************/

IFF_U_16 iff_get16(u_char *s)
{
#ifndef _LIL_END
  if (((u_int32)s) & 0x1) /* two byte aligned? */
#endif
    return (*s<<8 | *(s+1));
#ifndef _LIL_END
  else
    return *((IFF_U_16*)s);
#endif
}

void iff_set16(u_char *d, IFF_U_16 s)
{
#ifndef _LIL_END
  if (((u_int32)s) & 0x1) { /* two byte aligned? */
#endif
    *d++ = (s & 0xff00) >> 8;
    *d   = (s & 0x00ff);
#ifndef _LIL_END
  } else
    *((IFF_U_16*)d) = s;
#endif
}
