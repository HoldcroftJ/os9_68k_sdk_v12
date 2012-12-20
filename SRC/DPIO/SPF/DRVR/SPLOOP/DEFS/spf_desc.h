/****************************************************************************
 *	ID:     %A%
 *  Date:   %G%
 ****************************************************************************
 *	Definitions file for making SPF device descriptors.		*
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
 *     10/13/95 <------------------- SPF v2.0 ------------------>       dwj *
 * 204 06/26/97 <------------------- SPF v2.2 ------------------>       taka*
 * 211 07/19/98 <---------------- SPF v2.2.2 ------------------->       taka*
 ****************************************************************************/


#include <SPF/item.h>

#ifdef loop
/*****************************************************************************
**	/loop								**
**	Device Descriptor for SPF LoopBack driver 			**
**									**
** This driver acts like a true loopback driver. Any descriptor with a 	**
** LUN > MAX_CALL_ENDPTS will act like a true loopback			**
**									*/

/* Macros that initialize device descriptor common fields */
#define PORTADDR    0			/* No port address for loopback driver	*/

#define LUN         0x7F               /* logical unit number			*/
#define MODE        S_ISIZE | S_IREAD | S_IWRITE    /* device mode          */

/* Macros that initialize dd_popts structure of the device descriptor */
/* #define ASYNCFLAG    */
/* #define PKTFLAG      */
/* #define BLOCKTIME    */
/* #define READSZ       */

/* Define the ITEM default values. All possible values to initialize these
 * macros to can be found in item.h in the device_type and addr_type
 * structures.
 */
#define ITE_NETYPE_IN       ITE_NET_ANY
#define ITE_NETYPE_OUT      ITE_NET_ANY
#define ITE_CALL_STATE      ITE_CS_IDLE
#define OURADDR_CLASS       ITE_ADCL_NONE
#define OURADDR_SUBCLASS    ITE_ADSUB_NONE
#define OURADDR_SIZE        0
#define OURADDR             ""
#define THEIRADDR_CLASS     ITE_ADCL_NONE
#define THEIRADDR_SUBCLASS  ITE_ADSUB_NONE
#define THEIRADDR_SIZE      0
#define THEIRADDR           ""

/* Define the protocol stack in this descriptor if desired.
 */
/* #define PROTSTAK " " */

#define DRV_NAME	"sploop"		/* device driver name			*/

/* Macros that set the logical unit generic sections in spf.h */
#define TXSIZE		0xFFFF			/* transport packet max size		*/

#define PROTTYPE	SPF_PR_LOOP		/* Protocol ID defined in spf.h		*/

#define DEBUG_NAME	"dbg_sploop"	/* Debug data module name			*/

#include <SPLOOP/defs.h>

#endif /* /loop *************************************************************/

#ifdef loop1
/*****************************************************************************
**	/loop1									**
**	Device Descriptor for SPF LoopBack driver 				**
**										**
** This driver acts like a true loopback driver. Any descriptor with a 		**
** LUN > MAX_CALL_ENDPTS will act like a true loopback				**
**										*/

/* Macros that initialize device descriptor common fields */
#define PORTADDR    0			/* No port address for loopback driver	*/

#define LUN         0x7F		/* logical unit number			*/
#define DESCNUM     0x01
#define MODE        S_ISIZE | S_IREAD | S_IWRITE    /* device mode          */

/* Macros that initialize dd_popts structure of the device descriptor */
/* #define ASYNCFLAG    */
/* #define PKTFLAG      */
/* #define BLOCKTIME    */
/* #define READSZ       */

/* Define the ITEM default values. All possible values to initialize these
 * macros to can be found in item.h in the device_type and addr_type
 * structures.
 */
#define ITE_NETYPE_IN       ITE_NET_ANY
#define ITE_NETYPE_OUT      ITE_NET_ANY
#define ITE_CALL_STATE      ITE_CS_CONNLESS
#define OURADDR_CLASS       ITE_ADCL_NONE
#define OURADDR_SUBCLASS    ITE_ADSUB_NONE
#define OURADDR_SIZE        0
#define OURADDR             ""
#define THEIRADDR_CLASS     ITE_ADCL_NONE
#define THEIRADDR_SUBCLASS  ITE_ADSUB_NONE
#define THEIRADDR_SIZE      0
#define THEIRADDR           ""

/* Define the protocol stack in this descriptor if desired.
 */
/* #define PROTSTAK " " */

#define DRV_NAME	"sploop"		/* device driver name			*/

/* Macros that set the logical unit generic sections in spf.h */
#define TXSIZE		0xFFFF			/* transport packet max size		*/

#define PROTTYPE	SPF_PR_LOOP		/* Protocol ID defined in spf.h		*/

#define DEBUG_NAME	"dbg_sploop"	/* Debug data module name			*/

#include <SPLOOP/defs.h>

#endif /* /loop1 ************************************************************/

#ifdef loopc0
/*****************************************************************************
**	/loopc0									**
**	Device Descriptor for SPF LoopBack driver				**
**										**
** This device descriptor is connection oriented whose default far end 		**
** connection is specified by THEIRLUN below.					*/

/* Macros that initialize device descriptor common fields */
#define PORTADDR    0		/* No port address for loopback driver	*/

#define LUN         0           /* logical unit number                  */
#define THEIRLUN    1           /* Default far end logical unit number  */

#define MODE        S_ISIZE | S_IREAD | S_IWRITE    /* device mode          */

/* Macros that initialize dd_popts structure of the device descriptor */
/* #define ASYNCFLAG    */
/* #define PKTFLAG      */
/* #define BLOCKTIME    */
/* #define READSZ       */

/* Define the ITEM default values. All possible values to initialize these
 * macros to can be found in item.h in the device_type and addr_type
 * structures.
 */
#define ITE_NETYPE_IN       ITE_NET_ANY
#define ITE_NETYPE_OUT      ITE_NET_ANY
#define ITE_CALL_STATE      ITE_CS_IDLE
#define OURADDR_CLASS       ITE_ADCL_LPBK
#define OURADDR_SUBCLASS    ITE_ADSUB_LUN
#define OURADDR_SIZE        1
#define OURADDR             {LUN,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define THEIRADDR_CLASS     ITE_ADCL_LPBK
#define THEIRADDR_SUBCLASS  ITE_ADSUB_LUN
#define THEIRADDR_SIZE      1
#define THEIRADDR           {THEIRLUN,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

/* Define the protocol stack in this descriptor if desired.
 */
/* #define PROTSTAK " " */

#define DRV_NAME	"sploop"		/* device driver name			*/

/* Macros that set the logical unit generic sections in spf.h */
#define TXSIZE		0xFFFF			/* transport packet max size		*/

#define PROTTYPE	SPF_PR_LOOP		/* Protocol ID defined in spf.h		*/

#define DEBUG_NAME	"dbg_sploop"	/* Debug data module name			*/

#include <SPLOOP/defs.h>

#endif /* loopc0 ************************************************************/

#ifdef loopc1
/*****************************************************************************
**	/loopc1									**
**	Device Descriptor for SPF LoopBack driver				**
**										**
** This device descriptor is connection oriented whose default far end 		**
** connection is specified by THEIRLUN below.					**
**										*/

/* Macros that initialize device descriptor common fields */
#define PORTADDR    0		/* No port address for loopback driver	*/

/* For the loopback driver, the LUN corresponds to the OUTGO macro specified
 * later.
 */
#define LUN         1           /* logical unit number                  */
#define THEIRLUN    0           /* logical unit number                  */

#define MODE        S_ISIZE | S_IREAD | S_IWRITE    /* device mode          */

/* Macros that initialize dd_popts structure of the device descriptor */
/* #define ASYNCFLAG    */
/* #define PKTFLAG      */
/* #define BLOCKTIME    */
/* #define READSZ       */

/* Define the ITEM default values. All possible values to initialize these
 * macros to can be found in item.h in the device_type and addr_type
 * structures.
 */
#define ITE_NETYPE_IN       ITE_NET_ANY
#define ITE_NETYPE_OUT      ITE_NET_ANY
#define ITE_CALL_STATE      ITE_CS_IDLE
#define OURADDR_CLASS       ITE_ADCL_LPBK
#define OURADDR_SUBCLASS    ITE_ADSUB_LUN
#define OURADDR_SIZE        1
#define OURADDR             {LUN,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define THEIRADDR_CLASS     ITE_ADCL_LPBK
#define THEIRADDR_SUBCLASS  ITE_ADSUB_LUN
#define THEIRADDR_SIZE      1
#define THEIRADDR           {THEIRLUN,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

/* Define the protocol stack in this descriptor if desired.
 */
/* #define PROTSTAK " " */

#define DRV_NAME	"sploop"		/* device driver name			*/

/* Macros that set the logical unit generic sections in spf.h */
#define TXSIZE		0xFFFF			/* transport packet max size		*/

#define PROTTYPE	SPF_PR_LOOP		/* Protocol ID defined in spf.h		*/

#define DEBUG_NAME	"dbg_sploop"	/* Debug data module name			*/

#include <SPLOOP/defs.h>

#endif /* loopc1 ************************************************************/

#ifdef loopcl5
/*****************************************************************************
**	/loopcl5								**
**	Device Descriptor for SPF LoopBack driver				**
**										**
** This device descriptor is connectionless whose default far end 		**
** send to address is specified by THEIRLUN below.				**
**										*/

/* Macros that initialize device descriptor common fields */
#define PORTADDR    0			/* No port address for loopback driver	*/

/* For the loopback driver, the LUN corresponds to the OUTGO macro specified
 * later.
 */
#define LUN         5               /* logical unit number                  */
#define THEIRLUN    6               /* logical unit number                  */

#define MODE        S_ISIZE | S_IREAD | S_IWRITE    /* device mode          */

/* Macros that initialize dd_popts structure of the device descriptor */
/* #define ASYNCFLAG    */
/* #define PKTFLAG      */
/* #define BLOCKTIME    */
/* #define READSZ       */

/* Define the ITEM default values. All possible values to initialize these
 * macros to can be found in item.h in the device_type and addr_type
 * structures.
 */
#define ITE_NETYPE_IN       ITE_NET_ANY
#define ITE_NETYPE_OUT      ITE_NET_ANY
#define ITE_CALL_STATE      ITE_CS_CONNLESS
#define OURADDR_CLASS       ITE_ADCL_LPBK
#define OURADDR_SUBCLASS    ITE_ADSUB_LUN
#define OURADDR_SIZE        1
#define OURADDR             {LUN,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define THEIRADDR_CLASS     ITE_ADCL_LPBK
#define THEIRADDR_SUBCLASS  ITE_ADSUB_LUN
#define THEIRADDR_SIZE      1
#define THEIRADDR           {THEIRLUN,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

/* Define the protocol stack in this descriptor if desired.
 */
/* #define PROTSTAK " " */

#define DRV_NAME	"sploop"		/* device driver name			*/

/* Macros that set the logical unit generic sections in spf.h */
#define TXSIZE		0xFFFF			/* transport packet max size		*/

#define PROTTYPE	SPF_PR_LOOP		/* Protocol ID defined in spf.h		*/

#define DEBUG_NAME	"dbg_sploop"	/* Debug data module name			*/

#include <SPLOOP/defs.h>

#endif /* loopcl5 ***********************************************************/

#ifdef loopcl6
/*****************************************************************************
**	/loopcl6								**
**	Device Descriptor for SPF LoopBack driver				**
**										**
** This device descriptor is connectionless whose default far end 		**
** send to address is specified by THEIRLUN below.				**
**										*/

/* Macros that initialize device descriptor common fields */
#define PORTADDR    0			/* No port address for loopback driver	*/

/* For the loopback driver, the LUN corresponds to the OUTGO macro specified
 * later.
 */
#define LUN         6               /* logical unit number                  */
#define THEIRLUN    5               /* logical unit number                  */

#define MODE        S_ISIZE | S_IREAD | S_IWRITE    /* device mode          */

/* Macros that initialize dd_popts structure of the device descriptor */
/* #define ASYNCFLAG    */
/* #define PKTFLAG      */
/* #define BLOCKTIME    */
/* #define READSZ       */

/* Define the ITEM default values. All possible values to initialize these
 * macros to can be found in item.h in the device_type and addr_type
 * structures.
 */
#define ITE_NETYPE_IN       ITE_NET_ANY
#define ITE_NETYPE_OUT      ITE_NET_ANY
#define ITE_CALL_STATE      ITE_CS_CONNLESS
#define OURADDR_CLASS       ITE_ADCL_LPBK
#define OURADDR_SUBCLASS    ITE_ADSUB_LUN
#define OURADDR_SIZE        1
#define OURADDR             {LUN,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define THEIRADDR_CLASS     ITE_ADCL_LPBK
#define THEIRADDR_SUBCLASS  ITE_ADSUB_LUN
#define THEIRADDR_SIZE      1
#define THEIRADDR           {THEIRLUN,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

/* Define the protocol stack in this descriptor if desired.
 */
/* #define PROTSTAK " " */

#define DRV_NAME	"sploop"		/* device driver name			*/

/* Macros that set the logical unit generic sections in spf.h */
#define TXSIZE		0xFFFF			/* transport packet max size		*/

#define PROTTYPE	SPF_PR_LOOP		/* Protocol ID defined in spf.h		*/

#define DEBUG_NAME	"dbg_sploop"	/* Debug data module name			*/

#include <SPLOOP/defs.h>

#endif /* loopcl6 ***********************************************************/
