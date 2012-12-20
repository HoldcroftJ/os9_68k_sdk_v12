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
 * Current Version: @(#)termio.h	1.4 1/14/93
 */

#if !defined(_TERMIO_H)
#define _TERMIO_H

#define TIOC        ('T'<<8)
#define TCGETA      (TIOC|1)
#define TCSETA      (TIOC|2)
#define TCSETAW     (TIOC|3)
#define TCSETAF     (TIOC|4)
#define TCSBRK      (TIOC|5)
#define TCXONC      (TIOC|6)
#define TCFLSH      (TIOC|7)


/*
 * control characters
 */

#define VINTR       0
#define VQUIT       1
#define VERASE      2
#define VKILL       3
#define VEOF        4
#define VMIN        4
#define VEOL        5
#define VTIME       5
#define VEOL2       6
#define VSWTCH      7
#define VSUSP       8

#define NCC         9    /* # of control chars */

/*
 * Baud rates
 */

#define B0          0000000
#define B50         0000001
#define B75         0000002
#define B110        0000003
#define B134        0000004
#define B150        0000005
#define B200        0000006
#define B300        0000007
#define B600        0000010
#define B1200       0000011
#define B1800       0000012
#define B2400       0000013
#define B4800       0000014
#define B9600       0000015
#define B19200      0000016
#define B38400      0000017
#define EXTA        0000016
#define EXTB        0000017
#define CBAUD       0000017

/*
 * character size (bits)
 */

#define CSIZE       0000060
#define CS5         0000000
#define CS6         0000020
#define CS7         0000040
#define CS8         0000060

/*
 * stop bits
 */

#define CSTOPB      0000100
#define CREAD       0000200

/*
 * parity
 */

#define PARENB      0000400
#define PARODD      0001000

/*
 * line discipline 0 modes
 */

#define ISIG        0000001
#define ICANON      0000002
#define XCASE       0000004
#define ECHO        0000010
#define ECHOE       0000020
#define ECHOK       0000040
#define ECHONL      0000100
#define NOFLSH      0000200

/*
 * input modes
 */

#define IGNBRK      0000001
#define BRKINT      0000002
#define IGNPAR      0000004
#define PARMRK      0000010
#define INPCK       0000020
#define ISTRIP      0000040
#define INLCR       0000100
#define IGNCR       0000200
#define ICRNL       0000400
#define IUCLC       0001000
#define IXON        0002000
#define IXANY       0004000
#define IXOFF       0010000

/*
 * output modes
 */

#define OPOST       0000001
#define OLCUC       0000002
#define ONLCR       0000004
#define OCRNL       0000010
#define ONOCR       0000020

/*
 * Ioctl control packet
 */

struct termio
     {
     unsigned short  c_iflag;      /* input modes           */
     unsigned short  c_oflag;      /* output modes          */
     unsigned short  c_cflag;      /* control modes         */
     unsigned short  c_lflag;      /* line discipline modes */
     char            c_line;       /* line discipline       */
     unsigned char   c_cc[NCC];    /* control chars         */
     } ;

#endif /* _TERMIO_H */
