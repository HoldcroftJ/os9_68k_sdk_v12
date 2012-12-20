/****************************************************************************
 * ID:    %A%
 * Date:  %G%
 ****************************************************************************
 * Stacked Protocol Manager interface & data structure definitions.			*
 ****************************************************************************
 * Copyright 1995 by Microware Systems Corporation                          *
 * Reproduced Under License                                                 *
 *                                                                          *
 * This source code is the proprietary confidential property of             *
 * Microware Systems Corporation, and is provided to licensee               *
 * solely for documentation and educational purposes. Reproduction,         *
 * publication, or distribution in any form to any party other than         *
 * the licensee is strictly prohibited.                                     *
 ****************************************************************************
 * Edition History:                                                         *
 * #   Date     Comments                                          By        *
 * --- -------- ------------------------------------------------- --------- *
 *  1  03/06/95 Creation (from SPF version 1.0.2)                  bat      *
 *  2  05/19/95 Moved out of spf.h so macros can be used in                 *
 *              spf_desc.h  to make descriptor properly            cas      *
 *	3  05/24/95	Changed granularity to x00 to allow for use with			*
 *				setstat/getstat code definitions                   cas		*
 *  4  06/26/95 This file only contains user defined IDs           cas      *
 *     06/30/95 <---------------- SPF v2.0(pre) ---------------->  cas      *
 *     06/26/97 <---------------- SPF v2.2 --------------------->  taka     *
 * 255 08/20/98 <---------------- SPF v2.2.2 ------------------->  taka     *
 ****************************************************************************/

#if !defined(_PROT_IDS_H)
#define _PROT_IDS_H

/* Protocol types: Note that when creating protocol specific setstats and
 * getstats, the protocol type + setstat/getstat number should be
 * used to generate the numbering for these codes
 *
 * NOTE AGAIN: These codes are used to identify a particular protocol
 * and should be used as is for that purpose. However, getstats and setstats
 * particular to a given protocol should use that protocol number << 16
 * as the base for all their setstat codes.
 *
 * For example, X.25 is identified by 0x2. All the setstat and getstat
 * codes specific to X.25 start at a base address of (0x2 << 16) or
 * 0x00020000.
 */

/* This file conatains only user defined protocol identifiers. The protocol
 * identifiers that Microware defines are in spf.h
 *
 * The user-defined protocol type range is 0x0900 - 0xffff.
 */


/** Begin user defined protocol ID definitions here **/







#endif /* _PROT_IDS_H */
