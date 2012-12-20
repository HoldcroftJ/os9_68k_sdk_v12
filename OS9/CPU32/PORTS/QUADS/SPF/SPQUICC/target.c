/****************************************************************************
 *  Ethernet Transceiver control code for llquicc driver.                   *
 ****************************************************************************
 *  Copyright 1997 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 * Edition History:                                                         *
 * #   Date     Comments                                          By        *
 * --- -------- ------------------------------------------------- --------- *
 *  01 06/23/97 Created for 821ADS.                                ahms/ats *
 *     04/03/98 <***** SPF LAN Comm Pack v3.2 Release **********>
 ****************************************************************************/



#include <SPQUICC/defs.h>
#include <options.h>


/*	set up the Ethernet transceiver controls 'options.h'	*/
/*  this is totally hardware dependant !!!				  	*/

/*	Note that mode can be used to control several functions */
/*  of the EEST (68180) or other tranceiver if available  	*/

/*  Right now we will just turn it on and off				*/

void	config_ETRX(u_int32 mode, QUICC *qp, u_int32 channel)
{
}

