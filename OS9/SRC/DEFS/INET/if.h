/****************************************************************************
 *	if.h																	*
 *																			*
 *	An include file for the common parts of IFman drivers.  All of the junk	*
 *	an IF driver needs is included here.  Any private stuff a driver 		*
 *	requires shold be included after this file.								*
 ****************************************************************************
 *	Copyright 1993 by Microware Systems Corporation							*
 *	Reproduced Under License												*
 *																			*
 *	This source code is the proprietary confidential property of			*
 *	Microware Systems Corporation, and is provided to licensee				*
 *	solely for documentation and educational purposes. Reproduction,		*
 *	publication, or distribution in any form to any party other than		*
 *	the licensee is strictly prohibited.									*
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date   	Comments                                       	   By	*
 *  --- --------	-----------------------------------------------	------- *
 *	  1 ??/??/??	Original if.h									    ???	*
 *	  2 08/05/93	Combinded INET/../if.h & INETBOOT/../if.h		bat/gkm	*
 *	  3 09/10/93	Added comments for bpt_curtick						gkm *
 *      09/16/93	<***** ISP 2.0 Release ***********************>			*
 ****************************************************************************/

#include <types.h>
#include <const.h>
#include <errno.h>

#include <MACHINE/reg.h>
#include <modes.h>
#include <procid.h>
#include <sg_codes.h>
#include <sysio.h>
#include <path.h>

#include <INET/socket.h>
#include <INET/in.h>
#include <INET/mbuf.h>
#include <ifman.h>
#include <ifdev.h>
#include <ifdesc.h>


#define busy_driver(ifstat) (ifstat->v_sysio.v_busy++)
#define free_driver(ifstat) { register int i; if (--ifstat->v_sysio.v_busy \
 == 0) if (i = ifstat->v_sysio.v_dumm1[7]) f_aproc(i); }

 
/*********** FOR BOOT-CODE USE ONLY *****************************************
 * the bpt_ timer code is responsible for the next two items.				*
 * bpt_rseed should be initialized to something random.						*
 * bpt_curtick should increase by one for each elasped 100ms.				*
 ****************************************************************************/
extern                  int bpt_rseed;      /* RNG seed for tbackoff */
extern /*volatile*/ u_int32 bpt_curtick;    /* 100ms tick from timer irq */
    
/* since use of if.h has been discontinued for backplane driver and booter,
 * "extern bpt_curtick" has been explicitly declared in recv.c of the booter.
 * if you change the declaration here, check if it needs to be changed there
 * too. 93/09/10.
 */
