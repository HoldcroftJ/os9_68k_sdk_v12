/*******************************************************************************
*
* DESCRIPTION :
*
*	THIS FILE IS REQUIRED BY THE TEMPLATE.
*
*	This file contains the definitions that control the configuration of this
*	driver. This includes names of functions defined by the driver specific
*	code, and template features that this driver requires.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  11/20/95  TJE  First version
*/
#ifndef _CONFIG_H
#define _CONFIG_H

/*******************************************************************************
* OPTIONAL TEMPLATE HELPER FUNCTIONS TO INCLUDE
*******************************************************************************/

/*#define GDV_INCLUDE_MEM*/	/* Include memory allocation code */


/*#define GDV_INCLUDE_CVT2_RGB*/	/* Include code to convert colors to RGB */

/*#define GDV_FE_SYSTATE*/  /*need to execute system state code in fast entry point */
/*******************************************************************************
* NAMES OF FUNCTIONS REQUIRED BY THE TEMPLATE
*
* These functions must be provided by the driver specific code. The names may
* be changed, but this is seldom necessary.
*******************************************************************************/

#define GDV_INIT_HW		dr_init_hw	/* Initialize hardware */
#define GDV_TERM_HW		dr_term_hw	/* Terminate hardware */
#define GDV_SHOW_TOPDEV		dr_show_topdev	/* Show top logical device */
#define GDV_SET_VPPOS		fe_set_vppos	/* Set viewport position */
#define GDV_SET_VPSIZE		fe_set_vpsize	/* Set viewport size */
#define GDV_SET_VPSTATE		fe_set_vpstate	/* Set viewport state */
#define GDV_SET_VPDMAP		fe_set_vpdmap	/* Set viewport drawmap */
#define GDV_SET_VPDMPOS		fe_set_vpdmpos	/* Set drawmap pos in viewport */
#define GDV_SET_VPCOLORS	fe_set_vpcolors /* Set viewport colors */
#define GDV_RESTACK_VP		fe_restack_vp	/* Restack viewport */
#define GDV_UPDATE_DPY		fe_update_dpy	/* Update display */

/*******************************************************************************
* NAMES OF FUNCTIONS USED BY THE TEMPLATE IF DEFINED (OPTIONAL)
*
* These functions should be provided by the driver specific code if the specific
* features are present in the hardware. The names may be changed, but this is
* seldom necessary.
*******************************************************************************/


/*#define GDV_INIT_IRQS		dr_init_irqs*/	/* Initialize interrupts */
/*#define GDV_TERM_IRQS		dr_term_irqs*/	/* Terminate interrupts */
/*#define GDV_INIT_DVATCH	dr_init_dvatch*//* Init gfx device attachment */
/*#define GDV_TERM_DVATCH	dr_term_dvatch*/	/* Term gfx device attachment */
/*#define GDV_INIT_VPATCH	dr_init_vpatch*/	/* Initialize viewport attachment */
/*#define GDV_TERM_VPATCH	dr_term_vpatch*/	/* Terminate viewport attachment */

#endif /* _CONFIG_H_ */







