/*
	@(#)const.h	1.1.3	6/20/91
*/

/*--------------------------------------------------------------------------,
!                                                                           !
!              Copyright 1989 by Microware Systems Corporation              !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!---------------------------------------------------------------------------!
!																			!
!  Edition History:                                                         !
!   #   Date    Comments                                                By  !
!  -- -------- -------------------------------------------------------- --- !
!   1 87/02/23 Created                                                  rg  !
!	2 91/03/21 Brought to OS-9 and modified wrapper format.				rcb	!
!	3 98/02/27 Made OFFSET definition more correct (UCC2.0)				wwb	!
|				---- OS-9/68K V3.1 Release ----								|
!                                                                           !
`--------------------------------------------------------------------------*/

#if !defined(_CONST_H)
#define _CONST_H

#if !defined(_TYPES_H)
#include <types.h>
#endif /* _TYPES_H */

#if defined(SUCCESS)
#undef SUCCESS
#endif /* SUCCESS */

#define SUCCESS 	0

#if !defined(NULL)
#define NULL 		0
#endif /* NULL */

#if defined(ERROR)
#undef ERROR
#endif /* ERROR */

#define ERROR		(-1)

#define MAX_INT32 0x7FFFFFFF	/* maximum positive integer */
#define MIN_INT32 0x80000000	/* minimum negative integer */

/* fake queue head definitions */
#define OFFSET(Struct, field)	((u_int32)((char *)&((Struct)0)->field - (char *)0))
#define FAKEHD(Struc,head,lnk)	((Struc)((char*)&head - OFFSET(Struc, lnk)))

#endif /* _CONST_H */
