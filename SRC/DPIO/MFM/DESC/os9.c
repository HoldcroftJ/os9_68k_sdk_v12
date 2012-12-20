/****************************************************************************
 *	$Workfile:   os9.c  $
 *	$Revision:   1.3  $
 *	$Modtime:   15 Mar 1994 14:58:00  $
 *	$Date:   15 Mar 1994 14:58:10  $
 ****************************************************************************
 *	This is the OS-9 section of the MFM device descriptor.
 ****************************************************************************
 * Copyright 1994 by Microware Systems Corporation              
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
 *      06/07/95    <***************** MFM v1.0 ******************>     cjv *
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

#include <mfm_desc.h>
#include <drvr.h>


/* Macro Definitions ********************************************************/

#define FM_OFFSET		0x56
#define DRVR_OFFSET		0x76
#define VECTOR			0		/* not used */
#define IRQLEVEL		0		/* not used */
#define PRIORITY		0		/* not used */

/* Device descriptor OS-9 section data structure ****************************/

struct myos9 {
  /* from mod_dev in <module.h> */
  char *		 _mport;	   /* device port address */
  unsigned char	 _mvector;	   /* trap vector number */
  unsigned char	 _mirqlvl;	   /* irq interrupt level */
  unsigned char	 _mpriority;   /* irq polling priority */
  unsigned char	 _mmode;	   /* device mode capabilities */
  short			 _mfmgr;	   /* file manager name offset */
  short			 _mpdev;	   /* device driver name offset */
  short			 _mdevcon;     /* device configuration offset */
  unsigned short _mdscres[1];  /* (reserved) */
  unsigned long	 _mdevflags;   /* reserved for future usage	*/
  unsigned short _mdscres2[1]; /* reserved */
  unsigned short _mopt;		   /* option table size */
  unsigned char	 _mdtype;	   /* device type code	*/
  
  /* other needed fields */
  char			rsv[13];	   /* for long-word alignment */
  char			fm_name[32];   /* file manager name */
  char			drv_name[32];  /* device driver name */
  
} myos9 = {
  /* OS-9 device descriptor section */
  (void*)(PORTADDR+LUN), /* _mport: device port address */
  VECTOR,				 /* _mvector: trap vector number */
  IRQLEVEL,				 /* _mirqlvl: irq interrupt level */
  PRIORITY,				 /* _mpriority: irq polling priority */
  MODE,					 /* _mmode: device mode capabilities */
  FM_OFFSET,			 /* _mfmgr: file manager name offset */
  DRVR_OFFSET,			 /* _mpdev: device driver name offset */
  0,					 /* _mdevcon: device configuration offset */
  { 0 },				 /* _mdscres[1]: reserved */
  0,					 /* _mdevflags: reserved */
  { 0 },				 /* _mdscres2[1]: reserved */
  1,					 /* _mopt: option table size */
  DT_MFM,				 /* _mdtype: device type code */

  {
	0                    /* rsv[7]: long-word alignment */
  },

  {
    "mfm"	             /* fm_name[32]: file manager name */
  },

  {
	DRV_NAME		     /* drv_name[32]: driver name */
  }
};



