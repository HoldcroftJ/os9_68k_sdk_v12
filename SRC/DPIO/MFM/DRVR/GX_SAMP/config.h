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
*       2  12/18/00  SRM  CF2120 Add extended device capabilities
*/
#ifndef _CONFIG_H
#define _CONFIG_H

/*******************************************************************************
* OPTIONAL TEMPLATE HELPER FUNCTIONS TO INCLUDE
*******************************************************************************/

*?*?* DEFINE THE FOLLOWING ONLY IF YOUR DRIVER NEEDS TO SUPPORT THE ALLOCATION
*?*?* AND DEALLOCATION OF GRAPHICS MEMORY. THIS SHOULD BE HANDLED BY THE KERNEL
*?*?* IF IT CAN BE. IF THE KERNEL HANDLES MEMORY ALLOCATION, THEN DO NOT DEFINE
*?*?* THE FOLLOWING.

#define GDV_INCLUDE_MEM			/* Include memory allocation code */

*?*?* DEFINE THE FOLLOWING ONLY IF YOU DEFINED "GDV_INCLUDE_MEM" AND YOU WANT
*?*?* THE SEGMENTS ALLOCATED FROM THE DRIVER TO INCLUDE A PREFIX AREA
*?*?* IMMEDIATELY BEFORE THE SEGMENT AND/OR A POSTFIX AREA IMMEDIATELY AFTER
*?*?* EACH SEGMENT.

#define GDV_MEM_PREFIX  *?*?*	/* Prefix size attached to each allocation */
#define GDV_MEM_POSTFIX *?*?*	/* Postfix size attached to each allocation */

*?*?* DEFINE THE ENTRY BELOW (USUALLY ONLY ONE OF THEM) THAT MATCHES THE TYPE OF
*?*?* COLORS REQUIRED BY YOUR HARDWARE CLUT. IF YOUR HARDWARE DOES NOT HAVE A
*?*?* CLUT THEN DO NOT DEFINE ANY OF THEM.

#define GDV_INCLUDE_CVT2_RGB	/* Include code to convert colors to RGB */
#define GDV_INCLUDE_CVT2_YUV	/* Include code to convert colors to YUV */
#define GDV_INCLUDE_CVT2_YCBCR	/* Include code to convert colors to YCbCr */

*?*?* DEFINE THE FOLLOWING IF YOU HAVE CODE IN ANY FAST ENTRY POINTS THAT MUST
*?*?* EXECUTE IN SYSTEM STATE. THIS IS USUALLY CAUSED BY USING PRIVELEDGED
*?*?* INSTRUCTIONS, OR PERFORMING I/O WITH HARDWARE THAT REQUIRES THE I/O TO
*?*?* BE PERFORMED IN SYSTEM STATE.

#define GDV_FE_SYSTATE			/* Fast entry points must run in system state */

*?*?* DEFINE THE FOLLOWING IF YOU SUPPORT EXTENDED DEVICE CAPABILITIES
*?*?* INFORMATION. THIS IS RECOMMENDED.
#define GDV_DEVCAPEXTEN         /* Provide extended device capabilities */

/*******************************************************************************
* NAMES OF FUNCTIONS REQUIRED BY THE TEMPLATE
*
* These functions must be provided by the driver specific code. The names may
* be changed, but this is seldom necessary.
*******************************************************************************/

#define GDV_INIT_HW			dr_init_hw		/* Initialize hardware */
#define GDV_TERM_HW			dr_term_hw		/* Terminate hardware */
#define GDV_SHOW_TOPDEV		dr_show_topdev	/* Show top logical device */
#define GDV_SET_VPPOS		fe_set_vppos	/* Set viewport position */
#define GDV_SET_VPSIZE		fe_set_vpsize	/* Set viewport size */
#define GDV_SET_VPSTATE		fe_set_vpstate	/* Set viewport state */
#define GDV_SET_VPDMAP		fe_set_vpdmap	/* Set viewport drawmap */
#define GDV_SET_VPDMPOS		fe_set_vpdmpos	/* Set drawmap pos in viewport */
#define GDV_SET_VPCOLORS	fe_set_vpcolors	/* Set viewport colors */
#define GDV_RESTACK_VP		fe_restack_vp	/* Restack viewport */
#define GDV_UPDATE_DPY		fe_update_dpy	/* Update display */

/*******************************************************************************
* NAMES OF FUNCTIONS USED BY THE TEMPLATE IF DEFINED (OPTIONAL)
*
* These functions should be provided by the driver specific code if the specific
* features are present in the hardware. The names may be changed, but this is
* seldom necessary.
*******************************************************************************/

*?*?* REMOVE THE FUNCTIONS IN THE FOLLOWING SECTION THAT ARE NOT SUPPORTED,
*?*?* OR NOT REQUIRED BY YOUR DRIVER.

#define GDV_INIT_IRQS		dr_init_irqs	/* Initialize interrupts */
#define GDV_TERM_IRQS		dr_term_irqs	/* Terminate interrupts */
#define GDV_INIT_DVATCH		dr_init_dvatch	/* Init gfx device attachment */
#define GDV_TERM_DVATCH		dr_term_dvatch	/* Term gfx device attachment */
#define GDV_INIT_VPATCH		dr_init_vpatch	/* Initialize viewport attachment */
#define GDV_TERM_VPATCH		dr_term_vpatch	/* Terminate viewport attachment */
#define GDV_SET_BKCOL		ss_set_bkcol	/* Set backdrop color */
#define GDV_SET_EXTVID		ss_set_extvid	/* Set external video on/off */
#define GDV_SET_TRANSCOL	ss_set_transcol	/* Set transparency color */
#define GDV_SET_VPMIX		ss_set_vpmix	/* Set viewport mixing on/off */
#define GDV_SET_VPINTEN		fe_set_vpinten	/* Set viewport intensity */
#define GDV_SET_ATTRIBUTE   dr_set_devattr  /* Set a device attribute */
#define GDV_GET_ATTRIBUTE   dr_get_devattr  /* Get a device attribute */

#if 0 /* add for read/write support */
#define GDV_READ_HW		    dr_read_hw	    
#define GDV_WRITE_HW        dr_write_hw	    
#endif 

/*******************************************************************************
* ADDITIONAL VALUES FOR THE TEMPLATE TO USE (OPTIONAL)
*
* These values should be set if the default value shown is not valid for the
* driver.
*******************************************************************************/
#define GDV_MEM_TOP_BIT         /* Clear most significant bit of the pointers to the memory structures, allocated by driver common code and used by API in user-state mode */ 

#define GDV_PIXMEM_BNDRY *?*?*	/* Pixmem boundary size (default = 4) */

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

*?*?* IF YOUR DRIVER NEEDS TO HANDLE BIT-BLIT OPERATIONS TO/FROM GRAPHICS RAM
*?*?* USING I/O REGISTERS, THEN YOU MUST DEFINE ALL THE FOLLOWING. OTHERWISE
*?*?* YOU SHOULD DELETE THIS ENTIRE SECTION (INCLUDE THE COMMENT BLOCK ABOVE).

#define GDV_INCLUDE_IOBLT		/* Include code to Bit-BLT through I/O regs */
#define GDV_IOBLT_WORDSIZ *?*?*	/* Word size in bytes */
#define GDV_IOBLT_WORDSFT *?*?*	/* Shift value to mul/div by word size */
#define GDV_IOBLT_LINESIZ *?*?*	/* Maximum line size in bytes */

#define GDV_IOBLT_GFXRAM	fe_ioblt_gfxram	/* Return true if graphics RAM */
#define GDV_IOBLT_READ		fe_ioblt_read	/* Read pixels from graphics RAM */
#define GDV_IOBLT_WRITE		fe_ioblt_write	/* Write pixels to graphics RAM */
#define GDV_IOBLT_WRITE_PIX	fe_ioblt_write_pix
											/* Write solid pixels to gfx RAM */

/*******************************************************************************
* IOBLT CONFIGURATION
*
* The following definitions re-configure the way that the IOBLT code works. DO
* not define "GDV_IOBLT_OFFSETS" and "GDV_IOBLT_SEP_CHROMA" at the same time.
*******************************************************************************/

*?*?* THE FOLLOWING ENTRY POINT IS USED TO ALLOW THE DRIVER TO SPECIFY OFFSETS
*?*?* TO THE ODD AND EVEN LINES IN THE DRAWMAP WHEN THEY ARE SEPARATED INTO TWO
*?*?* SECTIONS. THIS SEPARATION IS ONLY LOGICAL. THE PHYSICAL DRAWMAPS IN SYSTEM
*?*?* MEMORY ARE ALWAYS INTERLEAVED WITH RESPECT TO ODD AND EVEN LINES. IF YOUR
*?*?* DRIVER DOES NOT NEED THIS CAPABILITY THEN DELETE THE FOLLOWING.

#define GDV_IOBLT_OFFSETS	fe_ioblt_offsets
											/* Compute odd/even offsets */

*?*?* DEFINE THE FOLLOWING IF YOUR DRIVER NEEDS TO DEAL WITH DRAWMAPS THAT HAVE
*?*?* SEPARATE LUMA AND CHROMA SECTIONS (e.g. YCrCb 4:2:0).

#define GDV_IOBLT_SEP_CHROMA				/* Separate Y and CrCb */

/*******************************************************************************
* DEFINITIONS FOR BIT-BLT SUPPORT THROUGH H/W ACCELERATION
*
* These definitions are used to control the behavior of the template code used
* to perform Bit-BLT operations using H/W acceleration mechanism. Depending on 
* the graphics hardware support for the Bit-BLT operations, driver can choose 
* which operations to support in H/W acceleration mode and let either BLT API 
* or IOBLT (if GDV_INCLUDE_IOBLT is defined) to support the rest of them. Both 
* IOBLT and HWBLT layers can independantly co-exist within the same driver, 
* with HWBLT layer making the final decision to support a BLT operation of 
* choice. HWBLT layer is fully responsible for the implementation of the BLT 
* operation, which it has chosen to support, there is no driver common code 
* for that like in IOBLT layer, driver specific functions will be directly 
* hooked into the API interface. All GDV_HWBLT_DRWMIX through GDV_HWBLT_DST 
* "set context" functions have to be implemented so they always restore the BLT
* operations function pointers to the H/W accelerated functions, when it is 
* applicable. This should be done on every "set context" call into the driver,
* becase both BLT API and IOBLT layers always reset these function pointers to
* their default values.  
*******************************************************************************/

*?*?* IF YOUR DRIVER WANTS TO HANDLE BIT-BLIT OPERATIONS TO/FROM GRAPHICS RAM
*?*?* USING H/W ACCELERATION MECHANISM, THEN YOU MUST DEFINE ALL THE FOLLOWING. 
*?*?* OTHERWISE YOU SHOULD DELETE THIS ENTIRE SECTION (INCLUDE THE COMMENT BLOCK ABOVE).

#define GDV_INCLUDE_HWBLT		/* Include code to Bit-BLT through H/W acceleration */

/*******************************************************************************
* NAMES OF FUNCTIONS REQUIRED BY THE TEMPLATE
*
* These functions must be provided by the driver specific code. The names may
* be changed, but this is seldom necessary.
*******************************************************************************/
#define GDV_HWBLT_DRWMIX        _gdv_hwblt_drwmix    /* Set draw mixing mode */
#define GDV_HWBLT_CPYMIX        _gdv_hwblt_cpymix    /* Set copy mixing mode */
#define GDV_HWBLT_EXPMIX        _gdv_hwblt_expmix    /* Set expand mixing mode */
#define GDV_HWBLT_PIX           _gdv_hwblt_pix       /* Set pixel */
#define GDV_HWBLT_SRC           _gdv_hwblt_src       /* Set source drawmap */
#define GDV_HWBLT_EXPTBL        _gdv_hwblt_exptbl    /* Set expansion table */
#define GDV_HWBLT_TRANS         _gdv_hwblt_trans     /* Set transparency */
#define GDV_HWBLT_MASK          _gdv_hwblt_mask      /* Set mask drawmap */
#define GDV_HWBLT_OFS           _gdv_hwblt_ofs       /* Set offset */
#define GDV_HWBLT_DST           _gdv_hwblt_dst       /* Set destination drawmap */

/*******************************************************************************
* NAMES OF FUNCTIONS USED BY THE TEMPLATE IF DEFINED (OPTIONAL)
*
* These functions should be provided by the driver specific code if the specific
* features are present in the hardware. The names may be changed, but this is
* seldom necessary.
*******************************************************************************/
/* If GDV_BCATCH_SPECIFICS is defined in "static.h", driver may need to initialize */
/* driver-specific portion of Bit-BLT attachment. The following functions are the */
/* "hooks" for that */
#define GDV_INIT_BCATCH         dr_init_bcatch       /* Initialize Bit-BLT context attachment */
#define GDV_TERM_BCATCH         dr_term_bcatch       /* Terminate Bit-BLT context attachment */
   
/* Driver can choose which Bit-BLT operations to support "on-the-fly", depending */
/* on the BLT context parameters, but function prototypes of those Bit-BLT operations */
/* which are going to be implemented in driver-specific portion of H/W acceleration */
/* layer should appear here. Delete those functions, which won't be implemented at all.*/
#define GDV_HWBLT_DRAWBLK       _gdv_hwblt_drawblk   /* Draw block of pixels */
#define GDV_HWBLT_DRAWHLINE     _gdv_hwblt_drawhline /* Draw horizontal line */
#define GDV_HWBLT_DRAWVLINE     _gdv_hwblt_drawvline /* Draw vertical line */
#define GDV_HWBLT_DRAWPIXEL     _gdv_hwblt_drawpixel /* Draw pixel */
#define GDV_HWBLT_COPYBLK       _gdv_hwblt_copyblk   /* Copy block */
#define GDV_HWBLT_COPYNBLK      _gdv_hwblt_copynblk  /* Copy next block */
#define GDV_HWBLT_EXPDBLK       _gdv_hwblt_expdblk   /* Expand block */
#define GDV_HWBLT_EXPDNBLK      _gdv_hwblt_expdnblk  /* Expand next block */
#define GDV_HWBLT_GETPIXEL      _gdv_hwblt_getpixel  /* Get pixel */


/*******************************************************************************
* DEFINITIONS FOR H/W CURSOR SUPPORT
*
* These definitions are used to control the behavior of the template code used
* to perform Bit-BLT operations using H/W acceleration mechanism. Depending on 
* the graphics hardware support for the Bit-BLT operations, driver can choose 
* which operations to support in H/W acceleration mode and let either BLT API 
* or IOBLT (if GDV_INCLUDE_IOBLT is defined) to support the rest of them. Both 
* IOBLT and HWBLT layers can independantly co-exist within the same driver, 
* with HWBLT layer making the final decision to support a BLT operation of 
* choice. HWBLT layer is fully responsible for the implementation of the BLT 
* operation, which it has chosen to support, there is no driver common code 
* for that like in IOBLT layer, driver specific functions will be directly 
* hooked into the API interface. All GDV_HWBLT_DRWMIX through GDV_HWBLT_DST 
* "set context" functions have to be implemented so they always restore the BLT
* operations function pointers to the H/W accelerated functions, when it is 
* applicable. This should be done on every "set context" call into the driver,
* becase both BLT API and IOBLT layers always reset these function pointers to
* their default values.  
*******************************************************************************/

*?*?* IF YOUR DRIVER WANTS TO SUPPORT H/W CURSOR, THEN YOU MUST DEFINE ALL THE 
*?*?* FOLLOWING. OTHERWISE YOU SHOULD DELETE THIS ENTIRE SECTION (INCLUDE THE COMMENT 
*?*?* BLOCK ABOVE).

#define GDV_HW_CURSOR		/* Include code for H/W cursor support */

/*******************************************************************************
* NAMES OF FUNCTIONS REQUIRED BY THE TEMPLATE
*
* These functions must be provided by the driver specific code. The names may
* be changed, but this is seldom necessary.
*******************************************************************************/
#define GDV_CURSOR_CREATE     dr_cursor_create     /* Create cursor */
#define GDV_CURSOR_DESTROY    dr_cursor_destroy    /* Desroy cursor */
#define GDV_CURSOR_SET        fe_cursor_set        /* Set cursor (shape) */
#define GDV_CURSOR_SET_POS    fe_cursor_set_pos    /* Set cursor (position) */


#endif /* _CONFIG_H_ */
