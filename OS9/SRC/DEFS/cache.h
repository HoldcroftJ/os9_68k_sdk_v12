/*
	@(#)cache.h	1.1.3	3/21/91
*/
#if !defined(_CACHE_H)
#define _CACHE_H

/*--------------------------------------------------------------------------,
!																			!
!				Copyright 1991 by Microware Systems Corporation				!
!							Reproduced Under License						!
!																			!
!  This source code is the proprietary confidential property of Microware	!
!  Systems Corporation, and is provided to licensee for documentation and	!
!  educational purposes only. Reproduction, publication, or distribution	!
!  in any form to any party other than the licensee is strictly prohibited.	!
!																			!
!---------------------------------------------------------------------------!
!																			!
! OS-9 Cache control definitions.											!
!																			!
! edition history															!
!	#	date     comments												by  !
!  -- -------- -------------------------------------------------------- --- !
!	1 91-03-15	Created													rcb !
|				---- OS-9/68K V3.1 Release ----								|
!																			!
`--------------------------------------------------------------------------*/

/* Logical cache control definitions for the F_CCTL service request. */
#define C_FLUSH		0x00000000	/* generic flush operation (flush all caches) */
#define C_ENDATA 	0x00000001	/* enable data cache */
#define C_DISDATA	0x00000002	/* disable data cache */
#define C_FLDATA	0x00000004	/* flush data cache */
#define C_ENINST	0x00000010	/* enable instruction cache */
#define C_DISINST	0x00000020	/* disable instruction cache */
#define C_FLINST	0x00000040	/* flush intruction cache */

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
error_code _os_cache(u_int32);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#else
error_code _os_cache();
#endif

#endif /* _CACHE_H */
