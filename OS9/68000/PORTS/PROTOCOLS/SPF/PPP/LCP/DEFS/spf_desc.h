/*******************************************************************************
 *
 * Filename: spf_desc.h
 *  Project: LCP
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
 *     05/05/97 <***** SPF LAN Comm Pack v3.1 Release ********>
 *     10/17/97 Default PPP_MODE is Active Open.                    sr         
 *     04/03/98 <***** SPF LAN Comm Pack v3.2 Release **********>
 ******************************************************************************/

#ifndef _spf_desc_h_
#define _spf_desc_h_


#include <SPF/item.h>

/* See defs.h for values of these fields.				*/

/********************************************************/
/* Default values for LCP descriptors:					*/
/*														*/
/*      RX_MRU          1500 (bytes)                    */
/*      RX_PFC          1 (ON)                          */
/*      RX_ACFC         1 (ON)                          */
/*      MAX_SCR         10 (messages)                   */
/*      MAX_STR         2 (messages)                    */
/*      TIMEOUT         3000 (milliseconds)             */
/*      AUTH_TIMEOUT    3000 (milliseconds)             */
/*      AUTH_MAX_REQ    10                              */
/*      PPP_MODE        NO_WAIT_ON_OPEN                 */
/*      RX_ACCM         0x0                             */
/*      TX_ACCM         { 0xffffffff,0,0,0,0,0,0,0 }    */
/*      LUN     No default value.                       */
/********************************************************/
 
/********************************************************/
/*	LCP device descriptor for basic PPP client: /lcp0	*/
/*														*/
#ifdef lcp0

#define LUN			0				/* Logical unit number */

#endif	/* lcp0 */
/*														*/
/********************************************************/

/********************************************************/
/********************************************************/
/****** Do *NOT* change anything below this line! *******/
/********************************************************/

#define DRV_NAME    "splcp"			/* Device driver	*/
#define PROTTYPE	SPF_PR_PPP_LCP	/* Driver protocol	*/
#define TXOFFSET	3				/* Header bytes		*/
#define TXSIZE		MRU_DEFAULT		/* Transmit max size*/


#include <SP_PPP/LCP/defs.h>

#endif /* _spf_desc_h_ */
