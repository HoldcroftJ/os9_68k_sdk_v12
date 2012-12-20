/*******************************************************************************
 *
 * Filename: spf_desc.h
 *  Project: IPCP
 *  Purpose: Definitions file for making SPF device descriptors.
 *
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
 *  1  10/21/96 Initial                                             billw
 *     02/19/97 <***** SPF LAN Comm Pack v3.0 Release*********> 
 *     05/07/97 <***** SPF LAN Comm Pack v3.1 Release ********>
 *     10/17/97 Default PPP_MODE is Active Open.                    sr         *
 *  2  12/01/97 Added IPCP_ACCEPT_REMOTE & IPCP_ACCEPT_LOCAL        tmd  
 *     04/03/98 <***** SPF LAN Comm Pack v3.2 Release **********>
 ******************************************************************************/

#ifndef _spf_desc_h_
#define _spf_desc_h_


#include <SPF/item.h>

/* See defs.h for the following definitions.                    */

/****************************************************************/
/* Default values for IPCP descriptors:                         */
/*                                                              */
/*      DESC_NAME               "ipcp0"                         */
/*      INTERFACE_NAME          "ppp0"                          */
/*      LOWER_LAYER_PATH        ""                              */
/*      PPP_MODE                NO_WAIT_ON_OPEN                 */
/*      MAX_SCR                 10 (messages)                   */
/*      MAX_STR                 2 (messages)                    */
/*      TIMEOUT                 3000 (milliseconds)             */
/*      TCP_COMPRESSION         COMPRESSED_TCP                  */
/*      MAX_SLOT_ID             15                              */
/*      COMP_SLOT_ID            1                               */
/*      DEFAULT_ROUTE           NO_DEFAULT_ROUTE                */
/*      IPCP_ACCEPT_REMOTE      TRUE                            */
/*      IPCP_ACCEPT_LOCAL       TRUE                            */
/****************************************************************/
 
/********************************************************/
/*  IPCP device descriptor for basic PPP client: /ipcp0 */
/*                                                      */
#ifdef ipcp0

#define LUN              0x00

#endif	/* ipcp0 */
/*                                                      */
/********************************************************/

/********************************************************/
/********************************************************/
/****** Do *NOT* change anything below this line! *******/
/********************************************************/

#define DRV_NAME    "spipcp"        /* Device driver    */
#define PROTTYPE    SPF_PR_PPP_IPCP /* Driver protocol  */
#define TXOFFSET    2               /* Header bytes     */


#include "SP_PPP/IPCP/defs.h"

#endif /* _spf_desc_h_ */
