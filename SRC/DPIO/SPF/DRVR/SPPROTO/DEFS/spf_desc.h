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
 *  #    Date                   Comments                                By  *
 * --- -------- ------------------------------------------------------- --- *
 *     10/13/95 <--------------------- SPF v2.0 ----------------------> dwj *
 *     03/17/97 Modifications for SPF v2.2 for clarity.                 cas *
 * 206 04/17/97 <--------------------- SPF v2.2 ----------------------> taka*
 * 211 07/19/98 <---------------- SPF v2.2.2 -------------------------> taka*
 ****************************************************************************/

#include <SPF/item.h>

		/************************************************************/
#ifdef proto	/* Macro definitions for the "proto" descriptor			*/

/************************************************************
 * Macros that initialize dd_desccom in spf_desc structure	*/

#define PORTADDR 0				/* HW Port addr: dd_port	*/
#define LUN      0x7F				/* log.unit num: dd_lu_num	*/
#define MODE     S_ISIZE | S_IREAD | S_IWRITE	/* device mode:  dd_mode	*/

/* End macros that initialize device descriptor common fields *
 **************************************************************/

/**********************************************************************
 * Macros that initialize dd_popts structure of the device descriptor */

/* Asynchronous flag. If an operation occurs that would normally cause the
 * path to block, an EWOULDBLOCK error will be returned instead.
 *
 * This macro initializes pd_ioasync in the spf_pdstat structure.
 * Options are: IO_SYNC: Synchronous (blocking) operation
 *              IO_WRITE_ASYNC: Non-blocking writes if mbuf pool empty
 *              IO_READ_ASYNC: return EWOULDBLOCK in no read data
 *              IO_ASYNC: non-blocking reads and writes
 */
#if 0
#define ASYNCFLAG	IO_SYNC
#endif

/* Packet oriented operation flag. This is a bit field that determines the
 * packet oriented operation of the path. The macro definitions can be
 * found in spf.h under the pd_iopacket variable in the path options
 * structure.
 *
 * This macro initializes pd_iopacket in the spf_pdstat structure.
 * Options are: IO_CHAR: Character oriented reads (historical OS-9 operation)
 *              IO_PACKET: Packet oriented operation
 *              IO_DGRAM_TOSS: Datagram operation
 *              IO_NEXTPKT_ONLY: Just get next packet in queue on read
 *              IO_PACKET_TRUNC: Toss extra in a current packet
 */
#if 0
#define PKTFLAG		IO_CHAR
#endif

/* The number of ticks to wait if pd_ioasync = 0 (blocking operation).
 *
 * This macro initializes pd_iotime in the spf_pdstat structure.
 */
#if 0
#define BLOCKTIME	0
#endif

/* Flow control mechanism. This determines the maximum amount of receive
 * data enqueued on the path. Once this threshold is reached, SPF will
 * issue a flow control message down to the drivers in an attempt to
 * stop receive data flow until the data is reead below this READSZ threshold.
 *
 * This macro initializes pd_readsz in the spf_pdstat structure.
 */
#if 0
#define READSZ		0	/* 0 = no read queue threshold	*/
#endif

/* Flow control mechanism on the transmit size. If flow control is implemented
 * in the driver for transmission, once this threshld is reached, any time
 * the writer does a write, the driver will return an EOS_PEER_BUSY error
 * until the transmit size goes below the WRITESZ threshold.
 *
 * This macro initializes pd_writesz in the spf_pdstat structure.
 */
#if 0
#define WRITESZ		0	/* 0 = no write queue threshold	*/
#endif

/* End macros that initialize dd_popts structure of the device descriptor *
 **************************************************************************/

/**************************************************************************
 * Define the ITEM device default values. All possible values to initialize
 * these macros to can be found in item.h in the device_type and addr_type
 * structures as defined in item.h.
 *
 * This section initializes the dd_item structure in the spf_desc structure.
 */
#define ITE_NETYPE_IN       ITE_NET_ANY		/* dev_netwk_in		*/
#define ITE_NETYPE_OUT      ITE_NET_ANY		/* dev_netwk_out	*/
#define ITE_CALL_STATE      ITE_CS_ACTIVE	/* dev_callstate	*/
#define OURADDR_CLASS       ITE_ADCL_NONE	/* dev_ournum.addr_class	*/
#define OURADDR_SUBCLASS    ITE_ADSUB_NONE	/* dev_ournum.addr_subclass	*/
#define OURADDR_SIZE        0			/* dev_ournum.addr_size		*/
#define OURADDR             ""			/* dev_ournum.addr		*/
#define THEIRADDR_CLASS     ITE_ADCL_NONE	/* dev_theirnum.addr_class	*/
#define THEIRADDR_SUBCLASS  ITE_ADSUB_NONE	/* dev_theirnum.addr_subclass	*/
#define THEIRADDR_SIZE      0			/* dev_theirnum.addr_size	*/
#define THEIRADDR           ""			/* dev_theirnum.addr		*/

/* End defining the ITEM device type structure in the descriptor *
 ****************************************************************/

/***********************************************************
 * Define the protocol stack in this descriptor if desired */
/* #define PROTSTAK " " */

#define DRV_NAME    "spproto"       /* device driver name                   */

/**************************************************************
 * Macros that set the logical unit generic sections in spf.h */

#define TXSIZE		0xFFFF	/* Maximum Transmission Unit (MTU): lu_txsize	*/
#if 0
#define TXOFFSET	0	/* Header requirements for protocol: lu_txoffset*/ 
#define TXTRAILER	0	/* Trailer req. for protocol: lu_txtrailer	*/
#endif

/* Causes SPF to hold onto the path even after the application has closed to
 * allow the protocol to send messaging back and forth to gracefully close.
 *
 * Initializes the lu_hold_on_close variable in the logical unit storage
 * Options are: PATH_NOHOLD = normal (default) operation.
 *              PATH_HOLD = perform the hold on the last path close.
 */
#if 0
#define PATH_HOLDONCLOSE	PATH_NOHOLD
#endif

#define PROTTYPE	SPF_PR_SPPROTO	/* Template (transparent) protocol ID	*/

/* Macros that set logical units specific information in SPPROTO/defs.h     */

#include <SPPROTO/defs.h>

#endif /* proto *************************************************************/

