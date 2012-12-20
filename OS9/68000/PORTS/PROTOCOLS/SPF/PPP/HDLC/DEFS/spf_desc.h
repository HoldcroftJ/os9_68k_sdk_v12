/*******************************************************************************
 *
 * Filename: spf_desc.h
 *  Project: HDLC
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
 *     10/17/97 Set BAUD_RATE to 19200 by default.                  sr      
 *     04/03/98 <***** SPF LAN Comm Pack v3.2 Release **********>
 ******************************************************************************/

#ifndef _spf_desc_h_
#define _spf_desc_h_


#include <SPF/item.h>

/* See ppp.h for baud rate, parity, word size and stop bits.	*/

/********************************************************/
/* Default values for HDLC descriptors:					*/
/*														*/
/*      RX_DEVICE_NAME: /t1                             */
/*      TX_DEVICE_NAME: value of RX_DEVICE_NAME         */
/*           BAUD_RATE: BAUDRATE_19200                  */
/*              PARITY: NO_PARITY                       */
/*           WORD_SIZE: WORD_SIZE_8                     */
/*           STOP_BITS: STOP_BITS_1                     */
/*          RTS_ENABLE: RTS_ENABLED                     */
/*        TX_BLOCKSIZE: 0 (No limit)                    */
/*    RECEIVE_BUF_SIZE: 100                             */
/*  TRANSPARENT_UPDATA: 0 (OFF)                         */
/*      DEBUG_MOD_SIZE: 0x2000                          */
/*            PPP_MODE: WAIT_FOR_MODEM                  */
/*       SENDSIG_DELAY: 0                               */
/*                 LUN: No default                      */
/*                                                      */
/*    (1) Select BAUD_RATE and PPP_MODE from the values */
/*                      defined in ppp.h                */
/********************************************************/
 
/********************************************************/
/*	HDLC device descriptor for basic PPP client: /hdlc0	*/
/*														*/
#ifdef hdlc0

#define LUN        			0x00	/* logical unit num	*/
#define BAUD_RATE BAUDRATE_19200	/* Default Baud rate */

#endif	/* hdlc0 */

/*														*/
/********************************************************/

/********************************************************/
/********************************************************/
/****** Do *NOT* change anything below this line! *******/
/********************************************************/

#define DRV_NAME    "sphdlc"		/* Device driver	*/
#define PROTTYPE	SPF_PR_HDLC		/* Driver protocol	*/
#define TXOFFSET	3				/* Header bytes		*/
#define TXTRAILER	3				/* Trailer bytes	*/


#include "SP_PPP/HDLC/defs.h"

#endif /* _spf_desc_h_ */
