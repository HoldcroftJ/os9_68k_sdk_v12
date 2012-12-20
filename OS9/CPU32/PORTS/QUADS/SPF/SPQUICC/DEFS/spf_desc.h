/****************************************************************************
 *  ID:		%A%
 *  Date:	%G%
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
 *     02/19/97 <***** SPF LAN Comm Pack v3.0 Release*********>             *
 *     05/05/97 <***** SPF LAN Comm Pack v3.1 Release*********>             *
 ****************************************************************************/

#include <SPF/item.h>

#ifdef spqe0    /************************************************************
**	Device Descriptor for SPF QUICC ethernet driver			*/

/**************************************************************************
*  User configuration defines                                            */

#define EA0 0
#define EA1 0x73
#define EA2 0x39
#define EA3 0x33
#define EA4 0x36
#define EA5 0x30

/**************************************************************************
*  Port configuration defines                                            */

/* Macros that initialize device descriptor common fields 	*/
#define PORTADDR	0x22c00		/* Base address of hardware */
#define LUN         0x7F        /* logical unit number      */
#define VECTOR		254			/* Port vector 				*/
#define PRIORITY	5			/* IRQ polling priority		*/
#define IRQLEVEL	5			/* Port IRQ Level			*/

/*  Macros that set the logical unit specifics section in spf.h */
#define IRQMASK		(IRQLEVEL << 8)	/* CPU interrupt mask	*/
#define IB_MODE		0
#define HIADDR		0
#define RX_RING_CNT	4
#define TX_RING_CNT	4
#define CSR3_INIT	4

/*********************************************************************
* Generic SPF configuration defines                                 */

#define DRV_NAME    "sp360"       /* device driver name             */

/* Macros that set the logical unit generic sections in spf.h */
#define TXSIZE		1500			/* transmit packet max size		*/
#define TXOFFSET 	4				/* mbuf address 				*/

/********************************************************************
* Do not change the followings                                     */

#define DEVMODE     	S_ISIZE | S_IREAD | S_IWRITE    /* device mode  */
#define PROTTYPE		SPF_PR_ETHER	/* Generic ethernet driver ID	*/
#define ITE_CALL_STATE  ITE_CS_ACTIVE

#include <SPQUICC/defs.h>

#endif /* spqe0 *************************************************************/

