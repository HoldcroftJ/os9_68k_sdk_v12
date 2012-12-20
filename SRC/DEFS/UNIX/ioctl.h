/*--------------------------------------------------------------------------,
! edition history                                                           !
!   #   date     comments                                               by  !
!  -- -------- -------------------------------------------------------- --- !
!   1 95-05-10  Created                                                 rcb !
!	2 95-05-11	Took out sock_opt										rcb !
!   3 98-02-20  Added this header                                       nan !
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
 * Current Version: @(#)ioctl.h	1.4 1/14/93
 */

#if !defined(_IOCTL_H)
#define _IOCTL_H

#if !defined(_TYPES_H)
#include <types.h>
#endif

#define fIOC        ('f'<<8)
#define tIOC        ('t'<<8)
#define FIONREAD    (fIOC|127)     /* get # of bytes to read     */
#define FIONBIO     (fIOC|126)     /* set/clear non-blocking i/o */
#define TIOCFLUSH   (tIOC|16)      /* flush buffers              */
#define FIOSNBIO	FIONBIO        /* used on some systems (like HP) */

#endif /* _IOCTL_H */
