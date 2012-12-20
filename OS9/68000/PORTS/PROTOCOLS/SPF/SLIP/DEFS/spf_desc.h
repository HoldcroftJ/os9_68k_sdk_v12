/****************************************************************************
 *  ID:         %A%
 *  Date:       %G%
 ****************************************************************************
 *           Copyright 1989-1998 by Microware Systems Corporation           *
 *                           All Rights Reserved                            *
 *                         Reproduced Under License                         *
 *                                                                          *
 * This software is confidential property of Microware Systems Corporation, *
 * and is provided under license for internal development purposes only.    *
 * Reproduction, publication, distribution, or creation of derivative works *
 * in any form to any party other than the licensee is strictly prohibited, *
 * unless expressly authorized in writing by Microware Systems Corporation. *
 *                                                                          *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *     10/13/95 <------------------- SPF v2.0 ------------------>       dwj *
 * 205 12/26/96 Added INBUFSIZE and OUTBUFSIZE macros.                  sr  *
 *     01/15/97 Changed /spsl1 to point to /t2                          pjw *
 *     02/19/97 <***** SPF LAN Comm Pack v3.0 Release*********>             *
 *     05/05/97 <***** SPF LAN Comm Pack v3.1 Release*********>             *
 *     04/03/98 <***** SPF LAN Comm Pack v3.2 Release*********> 
 ****************************************************************************/


#include <SPF/item.h>

/*
 * Magic number from RFC1055 (again)
 */
#define SLIPMTU		1006	/* IP level MTU */


#ifdef spsl0   /************************************************************
**	Device Descriptor for SPF Slip driver									*/

/* Macros that initialize device descriptor common fields */
#define PORTADDR    0               /* Base address of hardware             */
#define LUN         1               /* logical unit number                  */
#define MODE        S_ISIZE | S_IREAD | S_IWRITE    /* device mode          */

/* Macros that initialize dd_popts structure of the device descriptor */
/* #define ASYNCFLAG    */
/* #define PKTFLAG      */
/* #define BLOCKTIME    */
/* #define READSZ       */

/* Define the ITEM default values. All possible values to initialize these
 * macros to can be found in item.h in the device_type and addr_type
 * structures
 */
#define ITE_NETYPE_IN       ITE_NET_ANY
#define ITE_NETYPE_OUT      ITE_NET_ANY
#define ITE_CALL_STATE      ITE_CS_ACTIVE
#define OURADDR_CLASS       ITE_ADCL_NONE
#define OURADDR_SUBCLASS    ITE_ADSUB_NONE
#define OURADDR_SIZE        0
/* Address family AF_INET, Source port 0, IP address 10.0.0.1 */
#define OURADDR             0x00,2,0,0,0,0xa,0,0,0,0,0,0x1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
#define THEIRADDR_CLASS     ITE_ADCL_NONE
#define THEIRADDR_SUBCLASS  ITE_ADSUB_NONE
#define THEIRADDR_SIZE      0
#define THEIRADDR           ""

/* Define the protocol stack in this descriptor if desired.
 */
/* #define PROTSTAK " " */
/* #define TXOFFSET */
/* #define TXTRAILER */



#define DRV_NAME    "spslip"       /* device driver name */


/* Macros that set the logical unit generic sections in spf.h */
/* USER MODIFIABLE TO SUIT SYSTEM */

#define	MAXSLIPMTU		SLIPMTU

#define TXSIZE			MAXSLIPMTU	/* transmit packet max size	*/

/* Stole these "magic" values from ISP slip driver. */
#define OUTBUFSIZE  SLIPMTU * 2 + 36
#define INBUFSIZE   SLIPMTU + 32

/* thread priority */ 
#define SL_IN_PRIOR		128
#define SL_OUT_PRIOR	128

/* seria; device name */
#define I_DEV_NAME		"/t1"
#define O_DEV_NAME		"/t1"

/* serial data format */
#define SL_RCV_BUF_SIZ	4096		/* input raw receive buffer */
#define SL_PAR_BITS		0x00		/* parity/stopbits 			*/
#define SL_BAUD_RATE	0x0f		/* baud rate 				*/

/* END USER MODIFIABLE */

#define PROTTYPE	SPF_PR_SLIP	/* Template (transparent) protocol ID	*/

/* Macros that set logical units specific information in SPSLIP/defs.h     */

#include <SPSLIP/defs.h>

#endif /* spsl0 ************************************************************/


#ifdef spsl1   /************************************************************
**	Device Descriptor for SPF Slip driver									*/

/* Macros that initialize device descriptor common fields */
#define PORTADDR    1               /* Base address of hardware             */
#define LUN         2               /* logical unit number                  */
#define MODE        S_ISIZE | S_IREAD | S_IWRITE    /* device mode          */

/* Macros that initialize dd_popts structure of the device descriptor */
/* #define ASYNCFLAG    */
/* #define PKTFLAG      */
/* #define BLOCKTIME    */
/* #define READSZ       */

/* Define the ITEM default values. All possible values to initialize these
 * macros to can be found in item.h in the device_type and addr_type
 * structures
 */
#define ITE_NETYPE_IN       ITE_NET_ANY
#define ITE_NETYPE_OUT      ITE_NET_ANY
#define ITE_CALL_STATE      ITE_CS_ACTIVE
#define OURADDR_CLASS       ITE_ADCL_NONE
#define OURADDR_SUBCLASS    ITE_ADSUB_NONE
#define OURADDR_SIZE        0
/* Address family AF_INET, Source port 0, IP address 10.0.0.1 */
#define OURADDR             0x00,2,0,0,0,0xa,0,0,0,0,0,0x1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
#define THEIRADDR_CLASS     ITE_ADCL_NONE
#define THEIRADDR_SUBCLASS  ITE_ADSUB_NONE
#define THEIRADDR_SIZE      0
#define THEIRADDR           ""

/* Define the protocol stack in this descriptor if desired.
 */
/* #define PROTSTAK " " */
/* #define TXOFFSET */
/* #define TXTRAILER */



#define DRV_NAME    "spslip"       /* device driver name */


/* Macros that set the logical unit generic sections in spf.h */
/* USER MODIFIABLE TO SUIT SYSTEM */

#define	MAXSLIPMTU		SLIPMTU

#define TXSIZE			MAXSLIPMTU	/* transmit packet max size	*/

/* Stole these "magic" values from ISP slip driver. */
#define OUTBUFSIZE  SLIPMTU * 2 + 36
#define INBUFSIZE   SLIPMTU + 32

/* thread priority */ 
#define SL_IN_PRIOR		128
#define SL_OUT_PRIOR	128

/* seria; device name */
#define I_DEV_NAME		"/t2"
#define O_DEV_NAME		"/t2"

/* serial data format */
#define SL_RCV_BUF_SIZ	4096		/* input raw receive buffer */
#define SL_PAR_BITS		0x80		/* parity/stopbits 			*/
#define SL_BAUD_RATE	0x0f		/* baud rate 				*/

/* END USER MODIFIABLE */

#define PROTTYPE	SPF_PR_SLIP	/* Template (transparent) protocol ID	*/

/* Macros that set logical units specific information in SPSLIP/defs.h     */

#include <SPSLIP/defs.h>

#endif /* spsl1 ************************************************************/

