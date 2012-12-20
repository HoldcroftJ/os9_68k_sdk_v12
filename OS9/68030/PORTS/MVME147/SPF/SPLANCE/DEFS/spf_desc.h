/****************************************************************************
 *	ID:     %A%
 *  Date:   %G%
 ****************************************************************************
 *	Definitions file for making SPF 2.0 device descriptors.					*
 ****************************************************************************
 *  Copyright 1995 by Microware Systems Corporation              
 *  Reproduced Under License                         
 *                                                                          
 *  This source code is the proprietary confidential property of Microware   
 *  Systems Corporation, and is provided to licensee for documentation and   
 *  educational purposes only. Reproduction, publication, or distribution    
 *  in any form to any party other than the licensee is strictly prohibited. 
 ****************************************************************************
 *     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              *
 *     10/06/00 Changed RX_Ring_CNT and TX_RING_CNT from 4 to 16        mgh  *
 *                                                                           *
 ****************************************************************************/
#define SPF_DIR_NONE	0xFF
#define SPF_DIR_IN		0x00
#define SPF_DIR_OUT		0x01

#include <SPF/item.h>

/****************************************************************************
 *  /sple0                                                                    *
 *                                                                          *
 *  Device Descriptor for SPF LANCE driver -- RAW PMod --                    *
 ****************************************************************************/

#ifdef sple0

/**************************************************************************
*  User configuration defines                                            */

/*
Set these to appropriate non-zero values. To test LCLMEM code on CPU
without real local memory, set LCLMEM_SIZE to zero, and driver will
allocate system memory to simulate local memory.
NOTE: Driver must be built with LCLMEM defined for Local Memory operation.
*/
#ifdef LCLMEM
#define LCLMEM_BASE		0	/* address of local mem for CPU 		*/
#define LCLMEM_SIZE		0	/* size of local mem 					*/
#define LCLMEM_TRANS	0	/* convert CPU address to LANCE address */
#endif

/**************************************************************************
*  Port configuration defines                                            */

/* Macros that initialize the device descriptor common fields */

#define PORTADDR	0xfffe1800	/* Base address of hardware */
#define LUN			1			/* logical unit number 		*/
#define VECTOR		68			/* Port vector 				*/
#define PRIORITY	5			/* IRQ polling priority		*/
#define IRQLEVEL	5			/* Port IRQ Level			*/
#define BBRAM_LANID	0xfffe0778	/*addr of board lan node id in BBRam */

/*  Macros that set the logical unit specifics section in spf.h */
#define IRQMASK		(IRQLEVEL << 8)	/* CPU interrupt mask	*/
#define IB_MODE		0
#define HIADDR		0
#define RX_RING_CNT	16
#define TX_RING_CNT	16
#define CSR3_INIT	4

/*********************************************************************
* Generic SPF configuration defines                                 */

#define DRV_NAME	"sp147"		/* device driver name */

/* Macros that set the logical unit generic sections in spf.h */
#define TXSIZE		1500
#define TXOFFSET 	4			/* mbuf address */

/********************************************************************
* Do not change the followings                                     */

#define MODE 			S_ISIZE | S_IREAD | S_IWRITE	/* device mode	*/
#define ITE_NETYPE_IN	ITE_NET_CTL
#define ITE_NETYPE_OUT	ITE_NET_CTL
#define ITE_CALL_STATE	ITE_CS_IDLE

#include <SPLANCE/defs.h>

#endif /* sple0 **********************************************************/


#ifdef sple1

/**************************************************************************
*  User configuration defines                                            */

/*
Set these to appropriate non-zero values. To test LCLMEM code on CPU
without real local memory, set LCLMEM_SIZE to zero, and driver will
allocate system memory to simulate local memory.
NOTE: Driver must be built with LCLMEM defined for Local Memory operation.
*/
#ifdef LCLMEM
#define LCLMEM_BASE		0	/* address of local mem for CPU 		*/
#define LCLMEM_SIZE		0	/* size of local mem 					*/
#define LCLMEM_TRANS	0	/* convert CPU address to LANCE address */
#endif

/**************************************************************************
*  Port configuration defines                                            */

/* Macros that initialize the device descriptor common fields */

#define PORTADDR	0xfffe1800	/* Base address of hardware */
#define LUN			1			/* logical unit number 		*/
#define VECTOR		68			/* Port vector 				*/
#define PRIORITY	5			/* IRQ polling priority		*/
#define IRQLEVEL	5			/* Port IRQ Level			*/
#define BBRAM_LANID	0xfffe0778	/*addr of board lan node id in BBRam */

/*  Macros that set the logical unit specifics section in spf.h */
#define IRQMASK		(IRQLEVEL << 8)	/* CPU interrupt mask	*/
#define IB_MODE		0
#define HIADDR		0
#define RX_RING_CNT	4
#define TX_RING_CNT	4
#define CSR3_INIT	4

/*********************************************************************
* Generic SPF configuration defines                                 */

#define DRV_NAME	"sp147"		/* device driver name */

/* Macros that set the logical unit generic sections in spf.h */
#define TXSIZE		1500
#define TXOFFSET 	4			/* mbuf address */

/********************************************************************
* Do not change the followings                                     */

#define MODE 			S_ISIZE | S_IREAD | S_IWRITE	/* device mode	*/
#define ITE_NETYPE_IN	ITE_NET_CTL
#define ITE_NETYPE_OUT	ITE_NET_CTL
#define ITE_CALL_STATE	ITE_CS_IDLE

#include <SPLANCE/defs.h>

#endif /* sple1 **********************************************************/

