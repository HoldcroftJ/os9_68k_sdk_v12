/****************************************************************************
 *	$Workfile:   cnst.c  $
 *	$Revision:   1.0  $
 *	$Modtime:   15 Mar 1994 14:58:00  $
 *	$Date:   15 Mar 1994 14:58:08  $
 ****************************************************************************
 *	These are the MFM device descriptor definitions file.
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
 *      03/15/94    <***************** SPF v1.0 ******************>         *
 *      06/06/95    <***************** MFM v1.0 ******************>     cjv *
 *  200 10/14/95    <*************DAVID 2.0/MAUI 1.0**************>     srm *
 *  212 11/05/97    <**************** MAUI 2.1.2 *****************>     srm *
 *  240 02/07/00    <**************** MAUI 2.4.0 *****************>     srm *
 *  300 01/27/00    <***************** MAUI 3.0 ******************>     srm *
 ****************************************************************************/

_asm("_sysedit: equ 300");

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

#include <mfm_desc.h>
#include <MAUI/mfm.h> 
#include <drvr.h> 

/* Device descriptor constant data structure *******************************/

struct myconst {
  mfm_desc desc;
  char     fm_name[32];
  char     drv_name[32];
} myconst = {
  /* desc */
  {
	/* Device Descriptor Common Fields */
	(void*)PORTADDR,           /* dd_port: hardware base address */
	LUN,					   /* dd_lu_num: logical unit number */
	sizeof(mfm_path_desc),	   /* dd_pd_size: path descriptor size */
	DT_MFM,					   /* dd_type: device type */
	MODE,					   /* dd_mode: device mode capabilities	*/
	(u_int32)myconst.fm_name,  /* dd_fmgr: file mngr name offset */
	(u_int32)myconst.drv_name, /* dd_drvr: driver name offset */
	DC_SEQ,					   /* dd_class: device class */
	0,						   /* dd_dscres: reserved for future IOMAN */
	
	/* MFM specific fields */
	{
      DT_MFM,               /* Device class */
      sizeof(mfm_path_opt), /* Path options size */
	  { 0 }                 /* pd_rsv2[128-3]: RESERVED FOR FUTURE USE */
	},
	
	{ 0 }                      /* dd_rsv2[26]: RESERVED FOR FUTURE USE! */
  },
  
  {
	"mfm"                      /* fm_name[32]: file manager name */
  },

  {
	DRV_NAME                   /* drv_name[32]: device driver name */
  }
};




