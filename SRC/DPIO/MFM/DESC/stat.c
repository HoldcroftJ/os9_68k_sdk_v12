 /****************************************************************************
 *	$Workfile:   stat.c  $
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
#include <modes.h>

#if defined(_OSK)
#include <reg68k.h>
#endif

#include <defs.h> 

/* Logical Unit Static Storage Declarations *********************************/

#ifdef MFM_DESC

mfm_lu_stat my_lu =   {
  /* General Logical Unit Information Fields */
  SHARE,  /* Path sharing flag */
  { 0 },  /* RESERVED FOR FUTURE USE! */
  0,      /* Number of attaches using logical unit */
  0,      /* Path use count */
  LUN,    /* lu_num: logical unit number */
  
  (void*)PORTADDR, /* lu_port: port address	*/
  
  { 0 }, /* lu_rsv2[2]: RESERVED FOR FUTURE USE! */

  { 0 }, /* lu_rsv3[40]: RESERVED FOR FUTURE USE! */
  
  /* lu_luopts: logical unit options section */
  {
	sizeof(mfm_lu_opt),			/* lu_optsize: logical unit options size*/
	DT_MFM,						/* lu_class: device type				*/
	0,							/* lu_errs: accumulated I/O errors		*/
	{ 0 },						/* lu_rsv2[27]: RESERVED FOR FUTURE USE!*/
  },
  
#ifdef LU_SPECIFICS_INIT
  LU_SPECIFICS_INIT			/* logical unit specific static storage */
#endif
	
};

#else

mfm_lu_stat my_lu =   {
  /* General Logical Unit Information Fields */
  0,  /* Path sharing flag */
  { 0 },  /* RESERVED FOR FUTURE USE! */
  0,      /* Number of attaches using logical unit */
  0,      /* Path use count */
  0,    /* lu_num: logical unit number */
  
  (void*)0, /* lu_port: port address	*/
  
  { 0 }, /* lu_rsv2[2]: RESERVED FOR FUTURE USE! */

  { 0 }, /* lu_rsv3[40]: RESERVED FOR FUTURE USE! */
  
  /* lu_luopts: logical unit options section */
  {
	sizeof(mfm_lu_opt),			/* lu_optsize: logical unit options size*/
	DT_MFM,						/* lu_class: device type				*/
	0,							/* lu_errs: accumulated I/O errors		*/
	{ 0 },						/* lu_rsv2[27]: RESERVED FOR FUTURE USE!*/
  },
  
};

#endif
