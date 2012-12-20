/****************************************************************************
 *  File:	@(#) qedvr.h 1.10@(#)
 *	Date:	10/22/97
 ***************************************************************************
 *  (Old PVCS Header)
 *  $Workfile:   qedvr.h  $
 *  $Revision:   1.5  $
 *  $Modtime:   20 Sep 1996 15:42:00  $
 *  $Date:   20 Sep 1996 16:33:40  $
 *****************************************************************************
 *   QUICC Driver specific defs.                                             *
 *****************************************************************************
 *            Copyright 1989-1996 by Microware Systems Corporation           *
 *                            All Rights Reserved                            *
 *                          Reproduced Under License                         *
 *                                                                           *
 *  This software is confidential property of Microware Systems Corporation, *
 *  and is provided under license for internal development purposes only.    *
 *  Reproduction, publication, distribution, or creation of derivative works *
 *  in any form to any party other than the licensee is strictly prohibited, *
 *  unless expressly authorized in writing by Microware Systems Corporation. *
 *****************************************************************************
 *  Edition History:                                                         *
 *   #   Date       Comments                                           By    *
 *  --- --------    ----------------------------------------------- -------- *
 *  01 10/07/96 Creation                                                     *
 *     02/19/97 <***** SPF LAN Comm Pack v3.0 Release*********>              *
 *  06 04/02/97 Modified BD_BASE to avoid conflict with Microcode area. sr   *
 *     04/25/97 Removed INET references, moved includes files local     pjw  *
 *  08 05/19/97 Removed C_INVDATA define from here. It is in cache.h    sr   *
 *              Fixed Microcode confict issue.  Change has to be to          *
 *              BD_OFFSET and not BD_BASE.                                   *
 *     05/19/97 <***** SPF LAN Comm Pack v3.1 Release*********>              *
 *  10 05/19/97 Fixed Microcode conflict issue correctly.  Change has   sr   *
 *              to be to BD_OFFSET and not BD_BASE.                          *
 *              BD_OFFSET replaced by BD_SCC?_Eth to support                 *
 *              any/many channels                                            *
 *  09 08/11/97 Added SCC channel dependent code                        taka *
 *  10 09/24/97 added OS9 68360 support                                 taka *
 *     10/22/97 <***** OS-9000 MBX BSP Release v1.0 **********>              *
 *  15 03/05/98 if 0ed SCCE defines (T_TXE etc), use the real ones     jk/sr *
 *              defined in enet360.h                                         *
 *     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              *
 *  20 02/11/00 Include systype.h allow parameter override.             kimk *
 *              Some files (pins.c, target.c) included when needed but       *
 *              it's useful to always include it to allow systype.h override.*
 *              $$                    <RELEASE_INFO>                     $$  *
 ****************************************************************************/

#if !defined (_QEDVR_H)
#define _QEDVR_H
 
#include <SPQUICC/quicc.h>
#include <SPQUICC/enet360.h>
/*
 * If the device descriptor won't build, be sure that the
 * directory containing systype.h is given in the compile
 * search path.  Also systype.h may need the multiple-include
 * protection #ifdef _SYSTYPE_H
 */
#include <systype.h>

#if defined(_MPFPPC821)
#define BD_BASE 	0x2000		/* base of bd memory		*/
#define PRAMSCC_MASK 0xffff0000	/* Mask for SCC in the PRAM */
#else
#define BD_BASE 	0x0
#define PRAMSCC_MASK 0xfffff000	/* Mask for SCC in the PRAM */
#endif

	/* driver constant definitions
	 */
#if !defined(TRUE)
#	define TRUE (1==1)				/* let the compiler decide */
#	define FALSE (1==0)
#endif

/*	Definitions for the tranceiver mode, passed from llquicc.c to pins.c	*/
/*	These functions, if implemented, are hardware dependant					*/

#define	ETRX_TR_EN		0x0001	/*	Enable the tranceiver */
#define	ETRX_TR_DE		0x0002	/*  Disable the transceiver */
#define ETRX_LP_EN		0x0004	/*  Enable loopback */
#define ETRX_LP_DE		0x0008	/*  Disable loopback */


/*  Microware definitions for where the different buffer descriptors reside in DPRAM */
#if 0
/*
 * THESE ARE NOT USED HERE.  THEY SHOULD BE IN
 * SYSTYPE.H ANYWAY
 */
#define BD_SCC1         0x800   /* 16 bytes BD space for SCC1 */
#define BD_SMC1         0x840   /* 16 bytes BD space for SMC1 */
#define BD_SMC2         0x850   /* 16 bytes BD space for SMC2 */
#define BD_SPI          0x860   /* 16 bytes BD space for SPI */
/*                      0x870      16 bytes Reserved  */
#define BD_I2C          0x880   /* 64 bytes BD space for I2C */
#define BD_RTIME        0x8C0   /* 64 bytes BD space for RISC Timer */
#endif

/*
 * These can be overridden from systype.h
 */
#ifndef BD_SCC1_Eth
#define BD_SCC1_Eth     0x900   /* 128 bytes BD space for Ethernet on SCC1 */
#endif
#ifndef BD_Eth_Size
#define BD_Eth_Size     0x080   /* 128 bytes BD Space for Ethernet */
#endif

struct qe_stat {
	u_int   scoops;				/* packet scoops */
	u_int	unk_irq;			/* unknown irqs */
	u_int	dev_irq;			/* device irq */
	u_int	recv_irq;			/* receive interrupts */
	u_int	recv_lg;			/* large frame */
	u_int   recv_align;			/* alignment error */
	u_int	recv_short;			/* short frame */
	u_int	recv_crc;			/* crc error */
	u_int	recv_ov;			/* overrun */	
	u_int	recv_col;			/* collision */		
	u_int	recv_busy;			/* receive frames lost, no buff */
	u_int	recv_shrt;			/* short receive frame */
	u_int	recv_addr;			/* non-broadcast, not mine */
	u_int	xmit_irq;			/* transmit interrupts */
	u_int	recv_count;			/* number of successful receives */
	u_int	xmit_count;			/* number of successful transmits */
	u_int	xmit_lcol;			/* late collision error */
	u_int	xmit_lcar;			/* lost carrier on xmit */
	u_int	xmit_udma;			/* DMA underrun */
	u_int	xmit_defr;			/* deferral required for xmit */
	u_int	xmit_err;			/* Transmit Error */
	u_int	xmit_rlim;			/* Re-tranmsit limit reached */
	u_int   xmit_drop;          /* dropped due to no buffer space */
};

#endif	/* Endif _QEDVR_H */

