/**************************************************************************
*
* DESCRIPTION :
*
*	THIS FILE IS REQUIRED BY THE TEMPLATE.
*
*	This file contains the definitions that control the configuration 
*       of this driver. This includes names of functions defined by the 
*       driver specific code and template features that the driver requires.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* SCCS VERSION: @(#)config.h	1.3 1/5/96
*
* EDITION  DATE      WHO  DETAIL
*       1  11/20/95  TJE  First version
*       2  04/08/98  AND  Adopted for SVGA template
*       3  02/10/00  gdw  Increase maximum ioblt line size to the maximum
*                         actually used for 1024x768 15 bit color mode.
*       4  12/22/00  SRM  CF2120 Add extended device capabilities
*/
#ifndef _CONFIG_H
#define _CONFIG_H

/*******************************************************************************
* OPTIONAL TEMPLATE HELPER FUNCTIONS TO INCLUDE
*******************************************************************************/
#define GDV_INCLUDE_MEM         /* Include memory allocation code */
#define GDV_INCLUDE_CVT2_RGB	/* Include code to convert colors to RGB */
#define GDV_FE_SYSTATE          /* Execute fast-entry points in system state */
#define GDV_DEVCAPEXTEN         /* Provide extended device capabilities */

/*******************************************************************************
* NAMES OF FUNCTIONS REQUIRED BY THE TEMPLATE
*
* These functions must be provided by the driver specific code. The names may
* be changed, but this is seldom necessary.
*******************************************************************************/
#define GDV_INIT_HW	      dr_init_hw      /* Initialize hardware */
#define GDV_TERM_HW	      dr_term_hw      /* Terminate hardware */
#define GDV_SHOW_TOPDEV	      dr_show_topdev  /* Show top logical device */
#define GDV_SET_VPPOS	      fe_set_vppos    /* Set viewport position */
#define GDV_SET_VPSIZE	      fe_set_vpsize   /* Set viewport size */
#define GDV_SET_VPSTATE	      fe_set_vpstate  /* Set viewport state */
#define GDV_SET_VPDMAP	      fe_set_vpdmap   /* Set viewport drawmap */
#define GDV_SET_VPDMPOS	      fe_set_vpdmpos  /* Set drawmap pos in viewport */
#define GDV_SET_VPCOLORS      fe_set_vpcolors /* Set viewport colors */
#define GDV_RESTACK_VP	      fe_restack_vp   /* Restack viewport */
#define GDV_UPDATE_DPY	      fe_update_dpy   /* Update display */

/*******************************************************************************
* NAMES OF FUNCTIONS USED BY THE TEMPLATE IF DEFINED (OPTIONAL)
*
* These functions should be provided by the driver specific code if the specific
* features are present in the hardware. The names may be changed, but this is
* seldom necessary.
*******************************************************************************/
#define GDV_INIT_IRQS	      dr_init_irqs    /* Initialize interrupts */
#define GDV_TERM_IRQS	      dr_term_irqs    /* Terminate interrupts */
#define GDV_INIT_DVATCH	      dr_init_dvatch  /* Init gfx device attachment */
#define GDV_TERM_DVATCH	      dr_term_dvatch  /* Term gfx device attachment */
#define GDV_INIT_VPATCH	      dr_init_vpatch  /* Initialize viewport attachment */
#define GDV_TERM_VPATCH	      dr_term_vpatch  /* Terminate viewport attachment */


#ifdef BANK_SWITCH
/*******************************************************************************
* DEFINITIONS FOR BIT-BLT SUPPORT THROUGH I/O REGISTERS
*
* These definitions are used to control the behavior of the template code used
* to perform Bit-BLT operations using I/O registers. The only driver specific
* functions required are functions to read and write a line of pixels. The I/O
* based Bit-BLT functions are used to support Bit-BLT operations that are not
* directly supported by the high-level Bit-BLT API. This becomes necessary for
* two reasons. The first reason is that the CPU does not have access to the
* graphics RAM because it is accessed through an I/O port in the graphics chip.
* The second reason is that the coding method used is not supported by the high-
* level Bit-BLT API (e.g. GFX_CM_YCRCB420).
*******************************************************************************/
#define GDV_INCLUDE_IOBLT       /* Include code to Bit-BLT through I/O regs */
#define GDV_IOBLT_WORDSIZ 4     /* Word size in bytes */
#define GDV_IOBLT_WORDSFT 2     /* Shift value to mul/div by word size */
#define GDV_IOBLT_LINESIZ 2048  /* Maximum line size in bytes */
 
#define GDV_IOBLT_GFXRAM        fe_ioblt_gfxram    /* Return true if graphics RAM */
#define GDV_IOBLT_READ          fe_ioblt_read      /* Read pixels from graphics RAM */
#define GDV_IOBLT_WRITE         fe_ioblt_write     /* Write pixels to graphics RAM */
#define GDV_IOBLT_WRITE_PIX     fe_ioblt_write_pix /* Write solid pixels to gfx RAM */

#endif

#endif /* _CONFIG_H_ */







