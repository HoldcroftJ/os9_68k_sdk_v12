/*--------------------------------------------------------------------------,
!                                                                           !
!               Copyright 1998-2000 by Microware Systems Corporation        !
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
!   1 94-09-02  Created                                                 rcb !
!	2 95-05-29	New C++ support											rcb !
!	3 96-05-12	Added support for OPT_PROTOS							rry !
!   4 98-02-20  Added this header                                       nan !
!   5 00-09-20  Added MEM_NOCACHE color.                                rkw !
!                                                                           !
`--------------------------------------------------------------------------*/

#if !defined(_MEMORY_H)
#define _MEMORY_H

/* This file defines the standard colored memory types */

#if defined(_OSK)
#define SYSRAM  	0x01
#define VIDEO1  	0x80
#define VIDEO2  	0x81
#define MEM_ANY 	0x00
#define MEM_SYS 	0x01
#endif

#if defined(_OS9000)
#define MEM_ANY		0x0000	/* general purpose RAM */
#define MEM_SYS		0x0001	/* system RAM */
#define MEM_NOCACHE	0x0002  /* Color for non-cached memory, needs corresponding cache list entry */ 

/* NOTE: all colors of shared memory should be defined within */
/*       the 0x8000 to 0xFFFF range.                          */
#define	MEM_SHARED	0x8000	/* shared system memory */
#endif

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

error_code _os_get_blkmap _OP((void *, void *, u_int32, u_int32 *, u_int32 *, u_int32 *, u_int32 *));
error_code _os_mem _OP((u_int32 *, void **));
error_code _os_move _OP((void *, void *, u_int32));
error_code _os_srqmem _OP((u_int32 *, void **, u_int32));
error_code _os_srtmem _OP((u_int32, void *));

#if defined(_OSK)
error_code _os9_allbit _OP((u_int32, u_int32, void *));
error_code _os9_delbit _OP((u_int32, u_int32, void *));
error_code _os9_schbit _OP((u_int16 *, u_int16 *, void *, void *));
error_code _os9_srqmem _OP((u_int32 *, void **));
error_code _os9_translate _OP((u_int32 *, u_int32, void **));
#endif

#if defined(_OPT_PROTOS)
char *srqcmem _OP((int, int));
char *_srqmem _OP((unsigned int));
int _srtmem _OP((unsigned int, char *));
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _MEMORY_H */
