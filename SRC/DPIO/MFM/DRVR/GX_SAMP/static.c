/*******************************************************************************
*
* DESCRIPTION :
*
*	Initialize and terminate static storage areas used by the driver.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  12/05/95  TJE  First version
*/
#include <defs.h>
#include <memory.h>
#include <modes.h>

/*******************************************************************************
* NAME:		dr_init_dvatch()
*
* USAGE:	Initialize attachment (GDV_DVATCH_SPECIFICS) to graphics device
*			structure (GFX_DEV_SHARED). Do not allocate the memory for the
*			attachment itself. This is done by the template.
*/
error_code dr_init_dvatch(Dev_list dev_entry, GFX_DEV *gfxdev)
{
	error_code ec;
	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
	
	*?*?* INCLUDE CODE HERE TO INITIALIZE THE MEMBERS OF THE
	*?*?* "GDV_DVATCH_SPECIFICS" DATA STRUCTURE. THIS STRUCTURE IS DEFINED
	*?*?* IN "static.h".

	*?*?* IF YOU ALLOCATE MEMORY AND ATTACH IT TO THIS DATA STRUCTURE, YOU
	*?*?* SHOULD PERMIT THE MEMORY SO THAT IT MAY BE ACCESSED FROM THE FAST
	*?*?* ENTRY POINTS IN THE DRIVER. THE FOLLOWING CODE IS AN EXAMPLE OF HOW
	*?*?* TO DO THIS.

	char *ptr;
	u_int32 perm = S_IREAD | S_IWRITE;
	process_id pid = gfxdev->pid;

	/* Allocate and permit memory */
	size = sizeof(*?*?*);
	if ((ec = _os_srqmem(&size, (void *)&ptr, MEM_SYS)) == SUCCESS) {
		_os_permit(cptr, size, perm, pid);
		dvatch->*?*?* = (u_int32 *)ptr;
	} else {
		return ec;
	}

	*?*?* IF YOUR DRIVER IS RESPONSIBLE FOR THE ALLOCATION OF GRAPHICS MEMORY
	*?*?* (SEE "GDV_INCLUDE_MEM" IN "config.h"), THEN YOU MUST CREATE A SHADE
	*?*?* OF MEMORY FOR EACH BANK OF GRAPHICS RAM. FOLLOWING IS AN EXAMPLE OF
	*?*?* HOW TO DO THIS.

	/* Create a memory shade for each bank of graphics RAM */
	if ((ec = gdv_create_mem_shade(gfxdev, lustat->mem_color, MEM_SHADE_PSEUDO,
		lustat->mem_color, lustat->mem_initial_size, lustat->mem_grow_size))
		!= SUCCESS) {
		return ec;
	}

	return SUCCESS;
}

/*******************************************************************************
* NAME:		dr_term_dvatch()
*
* USAGE:	Terminate usage of the attachment (GDV_DVATCH_SPECIFICS) to a
*			graphics device structure (GFX_DEV_SHARED). Do not free the memory
*			for the attachment itself. This is done by the template.
*/
error_code dr_term_dvatch(Dev_list dev_entry, GFX_DEV *gfxdev)
{
	error_code ec, result = SUCCESS ;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;

	*?*?* INCLUDE CODE HERE TO TERMINATE USE OF THE "GDV_DVATCH_SPECIFICS"
	*?*?* DATA STRUCTURE. THIS STRUCTURE IS DEFINED IN "static.h".

	*?*?* IF YOU ALLOCATED MEMORY AND ATTACHED IT TO THIS DATA STRUCTURE IN
	*?*?* "dr_init_dvatch()", YOU SHOULD PROTECT AND DEALLOCATE IT HERE. THE
	*?*?* FOLLOWING CODE IS AN EXAMPLE OF HOW TO DO THIS.

	u_int32 perm = S_IREAD | S_IWRITE;
	process_id pid = gfxdev->pid;

	/* Protect and dellocate memory */
	_os_protect(dvatch->*?*?*, sizeof(*?*?*), perm, pid);
	if ((ec = _os_srtmem(sizeof(*?*?*), dvatch->*?*?*)) != SUCCESS) {
		result = ec;
	}

	*?*?* IF YOUR DRIVER IS RESPONSIBLE FOR THE DEALLOCATION OF GRAPHICS MEMORY
	*?*?* (SEE "GDV_INCLUDE_MEM" IN "config.h"), THEN YOU MUST DESTROY THE
	*?*?* MEMORY SHADE THAT WAS CREATED IN "dr_init_dvatch()". FOLLOWING IS AN
	*?*?* EXAMPLE OF HOW TO DO THIS.

	/* Destroy the memory color for each bank of graphics RAM */
	if ((ec = gdv_destroy_mem_shade(gfxdev, lustat->mem_color)) != SUCCESS) {
		result = ec;
	}

	return result;
}

/*******************************************************************************
* NAME:		dr_init_vpatch()
*
* USAGE:	Initialize attachment (GDV_VPATCH_SPECIFICS) to the viewport
*			structure (GFX_VPORT_SHARED). Do not allocate the memory for the
*			attachment itself. This is done by the template.
*/
error_code dr_init_vpatch(Dev_list dev_entry, GFX_VPORT *vport)
{

	*?*?* INCLUDE CODE HERE TO INITIALIZE THE MEMBERS OF THE
	*?*?* "GDV_VPATCH_SPECIFICS" DATA STRUCTURE. THIS STRUCTURE IS DEFINED
	*?*?* IN "static.h".

	*?*?* IF YOU ALLOCATE MEMORY AND ATTACH IT TO THIS DATA STRUCTURE, YOU
	*?*?* SHOULD PERMIT THE MEMORY SO THAT IT MAY BE ACCESSED FROM THE FAST
	*?*?* ENTRY POINTS IN THE DRIVER. SEE THE FUNCTION "dr_init_dvatch()"
	*?*?* ABOVE FOR AN EXAMPLE OF HOW TO DO THIS.

	return SUCCESS;
}

/*******************************************************************************
* NAME:		dr_term_vpatch()
*
* USAGE:	Terminate usage of the attachment (GDV_VPATCH_SPECIFICS) to the
*			viewport structure (GFX_VPORT_SHARED). Do not free the memory for
*			the attachment itself. This is done by the template.
*/
error_code dr_term_vpatch(Dev_list dev_entry, GFX_VPORT *vport)
{

	*?*?* INCLUDE CODE HERE TO TERMINATE USE OF THE "GDV_VPATCH_SPECIFICS"
	*?*?* DATA STRUCTURE. THIS STRUCTURE IS DEFINED IN "static.h".

	*?*?* IF YOU ALLOCATED MEMORY AND ATTACHED IT TO THIS DATA STRUCTURE IN
	*?*?* "dr_init_vpatch()", YOU SHOULD PROTECT AND DEALLOCATE IT HERE. SEE THE
	*?*?* FUNCTION "dr_term_vpatch()" ABOVE FOR AN EXAMPLE OF HOW TO DO THIS.

	return SUCCESS;
}

/*******************************************************************************
* NAME:		dr_init_bcatch()
*
* USAGE:	Initialize driver-specific attachment (GDV_BCATCH_SPECIFICS) to 
*                       the Bit-BLT context structure (BLT_CONTEXT). Do not allocate 
*                       the memory for the attachment itself. This is done by the 
*                       template.
*/
error_code dr_init_bcatch(Dev_list dev_entry, GFX_VPORT *vport)
{

	*?*?* INCLUDE CODE HERE TO INITIALIZE THE MEMBERS OF THE
	*?*?* "GDV_BCATCH_SPECIFICS" DATA STRUCTURE. THIS STRUCTURE IS DEFINED
	*?*?* IN "static.h".

	*?*?* IF YOU ALLOCATE MEMORY AND ATTACH IT TO THIS DATA STRUCTURE, YOU
	*?*?* SHOULD PERMIT THE MEMORY SO THAT IT MAY BE ACCESSED FROM THE FAST
	*?*?* ENTRY POINTS IN THE DRIVER. SEE THE FUNCTION "dr_init_dvatch()"
	*?*?* ABOVE FOR AN EXAMPLE OF HOW TO DO THIS.

	return SUCCESS;
}

/*******************************************************************************
* NAME:		dr_term_bcatch()
*
* USAGE:	Terminate usage of the driver-specific attachment (GDV_BCATCH_SPECIFICS)
*                       to the Bit-BLT context structure (BC_CONTEXT). Do not free the 
*                       memory for the attachment itself. This is done by the template.
*/
error_code dr_term_bcatch(Dev_list dev_entry, GFX_VPORT *vport)
{

	*?*?* INCLUDE CODE HERE TO TERMINATE USE OF THE "GDV_BCATCH_SPECIFICS"
	*?*?* DATA STRUCTURE. THIS STRUCTURE IS DEFINED IN "static.h".

	*?*?* IF YOU ALLOCATED MEMORY AND ATTACHED IT TO THIS DATA STRUCTURE IN
	*?*?* "dr_init_bcatch()", YOU SHOULD PROTECT AND DEALLOCATE IT HERE. SEE THE
	*?*?* FUNCTION "dr_term_dvatch()" ABOVE FOR AN EXAMPLE OF HOW TO DO THIS.

	return SUCCESS;
}
