/*******************************************************************************
*
* DESCRIPTION :
*
*	Functions that deal with the hardware device. This is usually done through
*	I/O (or memory mapped) registers.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  05/17/95  TJE  First version
*/
#include <defs.h>
#include <modes.h>

/*******************************************************************************
* NAME:		dr_init_hw()
*
* USAGE:	Initialize the hardware. The display should be enabled, and it
*			should be black until there is something to display.
*/
error_code dr_init_hw(Dev_list dev_entry)
{
	error_code ec;
	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);

	*?*?* INITIALIZE HARDWARE.

	*?*?* IF YOUR DRIVER IS RESPONSIBLE FOR THE ALLOCATION OF GRAPHICS MEMORY
	*?*?* (SEE "GDV_INCLUDE_MEM" IN "config.h"), THEN YOU MUST CREATE A COLOR
	*?*?* OF MEMORY FOR EACH BANK OF GRAPHICS RAM. FOLLOWING IS AN EXAMPLE OF
	*?*?* HOW TO DO THIS.

	/* Create a memory color for each bank of graphics RAM */
	if ((ec = gdv_create_mem_color(lustat->mem_color, MEM_SHADE_PSEUDO,
		lustat->mem_base_address, lustat->mem_size)) != SUCCESS) {
		return ec;
	}

	return SUCCESS;
}

/*******************************************************************************
* NAME:		dr_term_hw()
*
* USAGE:	Initialize use of the hardware.
*/
error_code dr_term_hw(Dev_list dev_entry)
{
	error_code ec, result = SUCCESS;
	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);

	*?*?* TERMINATE HARDWARE.

	*?*?* IF YOUR DRIVER IS RESPONSIBLE FOR THE DEALLOCATION OF GRAPHICS MEMORY
	*?*?* (SEE "GDV_INCLUDE_MEM" IN "config.h"), THEN YOU MUST DESTROY THE
	*?*?* MEMORY COLOR THAT WAS CREATED IN "dr_init_hw()". FOLLOWING IS AN
	*?*?* EXAMPLE OF HOW TO DO THIS.

	/* Destroy the memory color for each bank of graphics RAM */
	if ((ec = gdv_destroy_mem_color(lustat->mem_color)) != SUCCESS) {
		result = ec;
	}

	return result;
}

/*******************************************************************************
* NAME:		dr_show_topdev()
*
* USAGE:	Show the top logical device in the stack of logical devices open
*			on this physical device. If no logical devices are open and visible,
*			then setup the hardware to show black.
*/
void dr_show_topdev(Mfm_lu_stat lustat)
{
	*?*?* IF "lustat->topdev" IS NULL, THEN THERE IS NO VISIBLE LOGICAL DEVICE.
	*?*?* IN THIS CASE SETUP THE HARDWARE TO SHOW BLACK. IF IT IS NOT NULL,
	*?*?* THEN "lustat->topdev" IS THE TOP-MOST VISIBLE DEVICE. MAKE IT VISIBLE
	*?*?* ON THE DISPLAY.
}

*?*?*
*?*?* ADD OTHER HARDWARE RELATED FUNCTIONS HERE.
*?*?* 
