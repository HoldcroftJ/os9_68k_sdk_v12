/*--------------------------------------------------------------------------,
! edition history                                                           !
!   #   date     comments                                               by  !
!  -- -------- -------------------------------------------------------- --- !
!   1 95-05-10  Created                                                 rcb !
!   2 98-02-20  Added this header                                       nan !
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
 * Current Version: @(#)un.h	1.4 1/14/93
 */

#if !defined(_UN_H)
#define _UN_H

struct	sockaddr_un {
	short	sun_family;
	char	sun_path[108];
};

#endif /* _UN_H */
