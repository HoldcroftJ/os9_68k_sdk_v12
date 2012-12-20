/****************************************************************************
 *	ID:     %A%
 *  Date:   %G%
 ****************************************************************************
 *	Definitions file for making SPF device descriptors.						*
 ****************************************************************************
 *  Copyright 1995 by Microware Systems Corporation              
 *  Reproduced Under License                         
 *                                                                          
 *  This source code is the proprietary confidential property of Microware   
 *  Systems Corporation, and is provided to licensee for documentation and   
 *  educational purposes only. Reproduction, publication, or distribution    
 *  in any form to any party other than the licensee is strictly prohibited. 
 ****************************************************************************
 * Edition History:                                                         *
 * #   Date     Comments                                          By        *
 * --- -------- ------------------------------------------------- --------- *
 *     09/26/97 Created						                                 *
 *     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              *
 *     04/28/99 MVME162-523 requires vector 0x57
 *              MVME167     requires vector 0x67                     Kehres *
 ****************************************************************************/


#include <SPF/item.h>

#define LCLMEM

#ifdef spie0
/************************************************************
**	Device Descriptor for SPF i82596 ethernet driver		*/

/**************************************************************************
*  User configuration defines                                            */

/* 
Set Ethernet Address in descriptor to desired value.
NOTE: Setting ethernet address in descriptor to non-zero value will
override any value in BBRAM.
*/
#define EA0 0  
#define EA1 0
#define EA2 0
#define EA3 0 
#define EA4 0
#define EA5 0

/*
Set these to appropriate non-zero values. To test LCLMEM code on CPU
without real local memory, set LCLMEM_SIZE to zero, and driver will
allocate system memory to simulate local memory.
NOTE: Driver must be built with LCLMEM defined for Local Memory operation.
*/
#ifdef LCLMEM
#define LCLMEM_BASE		0xc2400000	/* address of local mem for CPU 		*/
#define LCLMEM_SIZE		0x4000		/* size of local mem 					*/
#define LCLMEM_TRANS	0			/* convert CPU address to LANCE address */
#endif

/**************************************************************************
*  Port configuration defines                                            */

/* Macros that initialize device descriptor common fields */
#define PORTADDR	0xfff46000	/* Base address of hardware */
#define LUN         0x7F        /* logical unit number      */
#define VECTOR		0x57		/* Port vector 				*/
#define PRIORITY	5			/* IRQ polling priority		*/
#define IRQLEVEL	0x5			/* Port IRQ Level			*/

/*  Macros that set the logical unit specifics section in spf.h */
#define IRQMASK		(IRQLEVEL << 8)	/* CPU interrupt mask	*/
#define IB_MODE		0
#define HIADDR		0

/*********************************************************************
* Generic SPF configuration defines                                 */

#define DRV_NAME    "sp162"     /* device driver name         */

/* Macros that set the logical unit generic sections in spf.h */
#define TXSIZE		1500		/* transmit packet max size		*/
#define TXOFFSET 	4			/* mbuf address 				*/

/********************************************************************
* Do not change the followings                                     */

#define MODE        	S_ISIZE | S_IREAD | S_IWRITE    /* device mode	*/
#define PROTTYPE		SPF_PR_ETHER	/* Generic ethernet driver ID	*/
#define ITE_CALL_STATE  ITE_CS_ACTIVE

#include <SP82596/defs.h>

#endif /* spie0 *************************************************************/

