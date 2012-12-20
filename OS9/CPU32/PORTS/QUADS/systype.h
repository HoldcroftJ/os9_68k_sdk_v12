/*--------------------------------------------------------------------------!
! systype.h: System specific defs for 'C' language code for the MZ68360     !
+---------------------------------------------------------------------------+
!    Edition History:                                                       !
!    #   Date    Comments                                                By !
!    -- -------- ------------------------------------------------------- ---!
!    00 93/10/07 history started.                                        arb!
!    01 00/10/18 added defines for bp_tk360.                             djc!
!                ---- OS-9/68K V3.0.1 Release ----                          !
!                                                                           !
!---------------------------------------------------------------------------!
!                                                                           !
!           Copyright 1993-2000 by Microware Systems Corporation.           !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#ifndef _types
#include <types.h>			/* This is here for use by high level stuff		*/
#endif

/* #include <gendefs.h>	*/	/* call in the general definitions file			*/
#include <nv.h>

/* SPQUICC DEFINES */

#define BD_SCC1         0x800   /* 16 bytes BD space for SCC1 */
#define BD_SCC2         0x810   /* 16 bytes BD space for SCC2 */
#define BD_SCC3         0x820   /* 16 bytes BD space for SCC3 */
#define BD_SCC4         0x830   /* 16 bytes BD space for SCC4 */
#define BD_SMC1         0x840   /* 16 bytes BD space for SMC1 */
#define BD_SMC2         0x850   /* 16 bytes BD space for SMC2 */
#define BD_SPI          0x860   /* 16 bytes BD space for SPI */
/*                      0x870      16 bytes Reserved  */
#define BD_I2C          0x880   /* 64 bytes BD space for I2C */
#define BD_RTIME        0x8C0   /* 64 bytes BD space for RISC Timer */

#define BD_SCC1_Eth     0x500   /* 128 bytes BD space for Ethernet on SCC1 */
#define BD_SCC2_Eth     0x580   /* 128 bytes BD space for Ethernet on SCC2 */
#define BD_SCC3_Eth     0x600   /* 128 bytes BD space for Ethernet on SCC3 */
#define BD_SCC4_Eth     0x680   /* 128 bytes BD space for Ethernet on SCC4 */
#define BD_Eth_Size     0x080   /* 128 bytes BD Space for Ethernet */

#define T_TXE 0x0010
#define R_BSY 0x0004
#define R_RXB 0x0002

/* Dual Ported Ram Base Address for this port */
#define DPRBASE   (0x20000)	    /* Base address of 360 Dual Ported Ram */
#define DPRBASE_S (0x22000)    	/* slave base addr */

/* Timer defines for bp_tk360 */
#define T_VECT 		(0xde)		/* Timer Vectore Number */
#define T_IRQLVL 	6			/* Timer IRQ Level */
