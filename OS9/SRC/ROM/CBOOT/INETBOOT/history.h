/*--------------------------------------------------------------------------!
! history.h: history file for ROM bootp/tftp client:                        !
! EDITION Number printed in inetboot.c   									!
+---------------------------------------------------------------------------+
!   Edition History:                                                        !
!   #   Date    Comments                                                By  !
!   -- -------- ------------------------------------------------------- --- !
!               ---- OS-9/68K V2.4 released ----                            !
!   01 90/08/08 Cloned from diskboot                                     kk !
!   02 90/10/22 Improved protocol error handling and logic               kk !
!   03 91/12/02 memset() arguments in wrong order                        kk !
!   04 92/11/13 ack DUP pkt to avoid stuck in reACK what server             !
!               don't want.                                            ylee !
!   05 93/06/22 Modified for UCC compatibility mode.                    bwt !
!   06 93/08/09 replaced use of inetboot.h with explicit includes (4)   gkm !
!               ---- OS-9 V3.0 Released ----                                !
!   07 93/12/03 Align emsg to even address for 68000 & CPU32        ts/ylee !
!   08 94/04/13 Change tftprcv_port and dest_port from type short to        !
!               type u_int16. connected_to{} addr from type int to          !
!               type u_int32, port from type short to type u_int16          !
!               for Solaris 2.3 and Bootp 2.3.6 that had error              !
!               NON-CONNECTED server response.                         ylee !
!   09 96/06/21 Fixed calculation for expected blocks when boot is even mgh !
!               block size.                                                 !
!               changed ifdev variable to ifd for ISP 2.1 defs compat   mgh !
!   10 96/09/12 Client now accepts correct block count or block         mgh !
!               count+1 so we accept boots for new and old OS9 Servers      !
!               when boot is exactly a block boundry. Note: Old servers     !
!               count the terminating empty block, new ones don't.          !
!               ---- OS-9 V3.0.3 Released ----                              !
!   11 97/12/05 Added define for Edition for history for in lbootp      pjw !
!   12 98/01/29 Major rework of timer/loop counter controls             mgh !
!               Added conditional printing of rprintf messages via msgprint !
!               global variable.                                            !
!               Ethernet Booters:Removed 5 tick add to timer valuse         !
!   13 00/02/04 Added timestamp display, improved logging messages		kimk!
!---------------------------------------------------------------------------!
!                                                                           !
!              Copyright 1990,1993,2000 by Microware Systems Corporation    !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#define EDITION 13
