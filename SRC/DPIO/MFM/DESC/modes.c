 /****************************************************************************
 *	$Workfile:   modes.c  $
 *	$Revision:   1.7  $
 *	$Modtime:   15 Mar 1994 14:58:00  $
 *	$Date:   15 Mar 1994 14:58:14  $
 ****************************************************************************
 *	These are the definitions for the MFM logical unit.
 *	Initializes LU static, LU options, path descriptor options sections.
 ****************************************************************************
 * Copyright 1992 by Microware Systems Corporation              
 * Reproduced Under License                         
 *                                                                          
 * This source code is the proprietary confidential property of Microware   
 * Systems Corporation, and is provided to licensee for documentation and   
 * educational purposes only. Reproduction, publication, or distribution    
 * in any form to any party other than the licensee is strictly prohibited. 
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date       Comments                                           By   *
 *  --- --------    ----------------------------------------------- ------- *
 *    0 01/03/94    Creation (from ISM)                                 bat *
 *    1 01/12/94    More initial fixes                                  bat *
 *    2 02/28/94    Added expanded logical unit options section         bat *
 *      03/15/94    <***************** SPF v1.0 ******************>         *
 ****************************************************************************/


/* Header Files *************************************************************/

#include <types.h>
#include <const.h>
#include <module.h>
#include <sg_codes.h>
#include <io.h>

#if defined(_OSK)
#include <reg68k.h>
#endif

#include <defs.h> 
