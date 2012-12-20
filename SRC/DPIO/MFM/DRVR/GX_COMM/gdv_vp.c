/*******************************************************************************
* DESCRIPTION :
*
*	This file contains functions that deal with viewports.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  12/07/95  TJE  Converted from earlier work by BAT and CJV
*/
#include <gdv_priv.h>
#include <memory.h>
#include <modes.h>

#define SIZE1 sizeof(GFX_VPORT)
#ifdef GDV_VPATCH_SPECIFICS
#define SIZE2 (sizeof(GFX_VPORT_SHARED) + sizeof(GDV_VPATCH))
#else
#define SIZE2 sizeof(GFX_VPORT_SHARED)
#endif

/*******************************************************************************
* NAME:		_gdv_createvp()
*
* USAGE:	Create a viewport. The GFX_VPORT structure is allocated by the API.
*			This function creates the attachment if there is one and links the
*			viewport into the stack of viewports for this logical device.
*/
error_code _gdv_createvp(Dev_list dev_entry, GFX_DEV *gfxdev, GFX_VPORT
	**ret_vport, GFX_POS x, GFX_POS y, GFX_DIMEN width, GFX_DIMEN height,
	GFX_VPORT_PLACEMENT placement, GFX_VPORT *ref_vport, Mfm_path_desc
	path_desc)
{
	error_code ec;
	GFX_DEV_RES *devres = gfxdev->shared->devres;
	u_int32 size;
	GFX_POS ex, ey;
	GFX_VPORT *vport;
	GFX_VPORT_SHARED *vport_shared;
	#ifdef GDV_VPATCH_SPECIFICS
	GDV_VPATCH *vpatch;
	#endif

	/* Verify parameters */
	if ((x < 0) || (x >= devres->disp_width) ||
		(y < 0) || (y >= devres->disp_height)) {
		return EOS_MAUI_BADPOS;
	}
	ex = x + width - 1;
	ey = y + height - 1;
	if ((width == 0) || (height == 0) || (ex >= devres->disp_width) ||
		(ey >= devres->disp_height)) {
		return EOS_MAUI_BADDIMEN;
	}

	/* Allocate the GFX_VPORT, GFX_VPORT_SHARED, and GDV_VPATCH structures. */
	size = SIZE1;
	if ((ec = _os_srqmem(&size, (void *)&vport, 0)) != SUCCESS) {
		return ec;
	}
	#if defined(GDV_MEM_TOP_BIT)
		vport = (GFX_VPORT *)adrcvt(vport);	/* SH3 */
	#endif
	size = SIZE2;
	if ((ec = _os_srqmem(&size, (void *)&vport_shared, 0)) != SUCCESS) {
		#if defined(GDV_MEM_TOP_BIT)
			vport = (GFX_VPORT *)unadrcvt(vport);	/* SH3 */
		#endif
		_os_srtmem(SIZE1, vport);
		return ec;
	}
	#if defined(GDV_MEM_TOP_BIT)
		vport_shared = (GFX_VPORT_SHARED *)adrcvt(vport_shared);
	#endif

	#ifdef GDV_VPATCH_SPECIFICS
	vpatch = (GDV_VPATCH *)((char *)vport_shared + sizeof(GFX_VPORT_SHARED));
	#endif

	/* Set members of the GFX_VPORT structure */
	vport->sync_code = _GFX_VPORT_SYNC;
	vport->gfxdev = gfxdev;
	vport->shared = vport_shared;
	vport->pid = path_desc->pd_common.pd_cproc;

	/* Set members of the GFX_VPORT_SHARED structure */
	#ifdef GDV_VPATCH_SPECIFICS
	vport_shared->gdv_attachment = vpatch;
	#else
	vport_shared->gdv_attachment = NULL;
	#endif
	vport_shared->link_count = 1;
	vport_shared->gfxdev_shared = gfxdev->shared;
	vport_shared->prev = NULL;
	vport_shared->next = NULL;
	vport_shared->active = FALSE;
	vport_shared->dmap = NULL;
	vport_shared->dmapx = 0;
	vport_shared->dmapy = 0;
	vport_shared->sx = x;
	vport_shared->ex = ex;
	vport_shared->sy = y;
	vport_shared->ey = ey;
	vport_shared->width = width;
	vport_shared->height = height;
	vport_shared->intensity = 100;
	vport_shared->coding_method = GFX_CM_UNKNOWN;
	vport_shared->dmap_width = 0;
	vport_shared->dmap_height = 0;
	vport_shared->dmap_line_size = 0;
	vport_shared->pixmem = NULL;
	vport_shared->dm2dp_xmul = 1;
	vport_shared->dm2dp_ymul = 1;

	/* Set members of the GDV_VPATCH structure */
	#ifdef GDV_VPATCH_SPECIFICS
	if ((ec = GDV_INIT_VPATCH(dev_entry, vport)) != SUCCESS) {
		#if defined(GDV_MEM_TOP_BIT)
			vport = (GFX_VPORT *)adrcvt(vport);
		#endif
		_os_srtmem(SIZE1, vport);
		return ec;
	}
	#endif

	/* Insert the viewport into its new location in the stack */
	gdv_insert_vport(vport, placement, ref_vport);

	/* Permit access to the GFX_VPORT and all attached structures */
	_os_permit(vport, SIZE1, S_IREAD | S_IWRITE, vport->pid);
	_os_permit(vport_shared, SIZE2, S_IREAD | S_IWRITE, vport->pid);

	/* Return viewport pointer */
	*ret_vport = vport;
	return SUCCESS;
}

/*******************************************************************************
* NAME:		_gdv_clonevp()
*
* USAGE:	Clone a viewport.
*/
error_code _gdv_clonevp(Dev_list dev_entry, GFX_VPORT **ret_vport, GFX_VPORT
	*vport, Mfm_path_desc path_desc)
{
	error_code ec;
	u_int32 size;
	/*	GFX_POS ex, ey;*/
	GFX_VPORT *new_vport;

	/* Allocate the GFX_VPORT structure */
	size = SIZE1;
	if ((ec = _os_srqmem(&size, (void *)&new_vport, 0)) != SUCCESS) {
		return ec;
	}
	#if defined(GDV_MEM_TOP_BIT)
		new_vport = (GFX_VPORT *)adrcvt(new_vport);
	#endif

	/* Set members of the GFX_VPORT structure */
	new_vport->sync_code = _GFX_VPORT_SYNC;
	new_vport->gfxdev = NULL; /* this must be set by the caller */
	new_vport->shared = vport->shared;
	new_vport->pid = path_desc->pd_common.pd_cproc;;

	/* Update link count for the shared viewport */
	vport->shared->link_count++;

	/* Permit access to the GFX_VPORT and all attached structures */
	_os_permit(new_vport, SIZE1, S_IREAD | S_IWRITE, new_vport->pid);
    _os_permit(new_vport->shared, SIZE2, S_IREAD | S_IWRITE, new_vport->pid); 


	/* Return viewport pointer */
	*ret_vport = new_vport;
	return SUCCESS;
}

/*******************************************************************************
* NAME:		_gdv_destroyvp()
*
* USAGE:	Destroy the specified viewport. Do not free the memory for GFX_VPORT
*			since it is allocated and freed by the API.
*/
error_code _gdv_destroyvp(Dev_list dev_entry, GFX_DEV *gfxdev, GFX_VPORT *vport)
{
	error_code ec, result = SUCCESS;
	process_id pid = vport->pid;

	/* This section is only executed if we are the last user of this viewport */
	if (--(vport->shared->link_count) == 0) {

		/* Unlink the viewport from its current location in the stack */
		gdv_unlink_vport(vport);

		/* Terminate use of the viewport attachment */
		#ifdef GDV_VPATCH_SPECIFICS
		if ((ec = GDV_TERM_VPATCH(dev_entry, vport)) != SUCCESS) {
			result = ec;
		}
		#endif

		/* Protect and deallocate memory used for the GDV_VPORT_SHARED and */
		/* GDV_VPATCH structures. */
		_os_protect(vport->shared, SIZE2, S_IREAD | S_IWRITE, pid);
		#if defined(GDV_MEM_TOP_BIT)
			vport->shared = (GFX_VPORT_SHARED *)unadrcvt(vport->shared);
		#endif
		if ((ec = _os_srtmem(SIZE2, vport->shared)) != SUCCESS) {
			result = ec;
		}
	}

	/* Protect and deallocate memory used for the GDV_VPORT structure. */
	vport->sync_code = 0;
	_os_protect(vport, SIZE1, S_IREAD | S_IWRITE, pid);
	#if defined(GDV_MEM_TOP_BIT)
		vport = (GFX_VPORT *)unadrcvt(vport);
	#endif
	if ((ec = _os_srtmem(SIZE1, vport)) != SUCCESS) {
		result = ec;
	}

	return result;
}

/*******************************************************************************
* NAME:		gdv_insert_vport()
*
* USAGE:	Insert the specified viewport into the stack of existing viewports.
*/
void gdv_insert_vport(GFX_VPORT *vport, GFX_VPORT_PLACEMENT placement, GFX_VPORT
	*ref_vport)
{
	GFX_VPORT *ins_vport = NULL;
	GFX_DEV *gfxdev = vport->gfxdev;
	
	/* Find the insertion point. This is the viewport that is to be */
	/* immediately in front of "vport". Set it to NULL if "vport" is to be */
	/* in front. */
	switch (placement) {
	  case GFX_VPORT_FRONT:		/* In front of all others */
	  	ins_vport = NULL;
	  	break;
	  case GFX_VPORT_BACK:		/* In back of all others */
	  	ins_vport = gfxdev->shared->vport_tail;
	  	break;
	  case GFX_VPORT_FRONT_OF:	/* In front of "ref_vport" */
	  	ins_vport = ref_vport->shared->prev;
	  	break;
	  case GFX_VPORT_BACK_OF:	/* In back of "ref_vport" */
	  	ins_vport = ref_vport;
	  	break;
	}

	/* Insert "vport" after "ins_vport" */
	if (ins_vport == NULL) {
		vport->shared->next = gfxdev->shared->vport_head;
		gfxdev->shared->vport_head = vport;
		vport->shared->prev = NULL;
	} else {
		vport->shared->next = ins_vport->shared->next;
		ins_vport->shared->next = vport;
		vport->shared->prev = ins_vport;
	}
	if (vport->shared->next == NULL) {
		gfxdev->shared->vport_tail = vport;
	} else {
		vport->shared->next->shared->prev = vport;
	}
}

/*******************************************************************************
* NAME:		gdv_unlink_vport()
*
* USAGE:	Unlink the specified viewport from the stack of existing viewports.
*/
void gdv_unlink_vport(GFX_VPORT *vport)
{
	GFX_DEV *gfxdev = vport->gfxdev;

	/* Unlink "next" pointer */
	if (vport->shared->next == NULL) {
		gfxdev->shared->vport_tail = vport->shared->prev;
	} else {
		vport->shared->next->shared->prev = vport->shared->prev;
	}

	/* Unlink "prev" pointer */
	if (vport->shared->prev == NULL) {
		gfxdev->shared->vport_head = vport->shared->next;
	} else {
		vport->shared->prev->shared->next = vport->shared->next;
	}

	/* Clear the "next" and "prev" pointers */
	vport->shared->next = vport->shared->prev = NULL;
}

/*******************************************************************************
* NAME:		_fe_set_vppos()
*
* USAGE:	Set the position of the specified viewport.
*/
error_code _fe_set_vppos(GFX_VPORT *vport, GFX_POS x, GFX_POS y)
{
	error_code ec;
	GFX_DEV *gfxdev = vport->gfxdev;
#ifndef GDV_FE_SYSTATE
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif
	sema4 *lock = &(vport->gfxdev->shared->modify_lock);

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif
	/* Lock changes to the logical device. Make sure it is unlocked */
	/* everywhere this function returns. */
	if ((ec = sema_p(lock)) != SUCCESS) {
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	/* Verify parameters */
	if ((x < 0) || (x >= gfxdev->shared->devres->disp_width) ||
		(y < 0) || (y >= gfxdev->shared->devres->disp_height)) {
		sema_v(lock);
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return EOS_MAUI_BADPOS;
	}
	if ((x + vport->shared->width > gfxdev->shared->devres->disp_width) ||
		(y + vport->shared->height > gfxdev->shared->devres->disp_height)) {
		sema_v(lock);
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return EOS_MAUI_BADDIMEN;
	}

	/* Call the driver specific function */
	if ((ec = GDV_SET_VPPOS(vport, x, y)) != SUCCESS) {
		sema_v(lock);
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	sema_v(lock);
#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return SUCCESS;
}

/*******************************************************************************
* NAME:		_fe_set_vpsize()
*
* USAGE:	Set the size of the specified viewport.
*/
error_code _fe_set_vpsize(GFX_VPORT *vport, GFX_DIMEN width, GFX_DIMEN height)
{
	error_code ec;
	GFX_DEV *gfxdev = vport->gfxdev;
#ifndef GDV_FE_SYSTATE
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
#endif
	sema4 *lock = &(vport->gfxdev->shared->modify_lock);

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif
	/* Lock changes to the logical device. Make sure it is unlocked */
	/* everywhere this function returns. */
	if ((ec = sema_p(lock)) != SUCCESS) {
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	/* Verify parameters */
    if ((width == 0) || (height == 0) ||
		(vport->shared->sx + width > gfxdev->shared->devres->disp_width) ||
		(vport->shared->sy + height > gfxdev->shared->devres->disp_height)) {
		sema_v(lock);
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return EOS_MAUI_BADDIMEN;
    }

	/* Call the driver specific function */
	if ((ec = GDV_SET_VPSIZE(vport, width, height)) != SUCCESS) {
		sema_v(lock);
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	sema_v(lock);
#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return SUCCESS;
}

/*******************************************************************************
* NAME:		_fe_set_vpstate()
*
* USAGE:	Set the state (active or not) of the specified viewport.
*/
error_code _fe_set_vpstate(GFX_VPORT *vport, BOOLEAN active)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GDV_DVATCH *dvatch = vport->gfxdev->shared->gdv_attachment;
#endif
	sema4 *lock = &(vport->gfxdev->shared->modify_lock);

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif
	/* Lock changes to the logical device. Make sure it is unlocked */
	/* everywhere this function returns. */
	if ((ec = sema_p(lock)) != SUCCESS) {
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	/* Make sure there is pixel memory for the viewport */
	if (active == TRUE) {
		if (vport->shared->pixmem == NULL) {
			sema_v(lock);
#ifndef GDV_FE_SYSTATE
			change_static(app_static);
#endif
			return EOS_MAUI_NODMAP;
		}
	}
	
	/* Call the driver specific function */
	if ((ec = GDV_SET_VPSTATE(vport, active)) != SUCCESS) {
		sema_v(lock);
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	sema_v(lock);
#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return SUCCESS;
}

/*******************************************************************************
* NAME:		_fe_set_vpcolors()
*
* USAGE:	Set the colors in a viewport.
*/
error_code _fe_set_vpcolors(GFX_VPORT *vport, u_int16 start_entry, u_int16
	num_colors, GFX_COLOR_TYPE color_type, void *colors)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GDV_DVATCH *dvatch = vport->gfxdev->shared->gdv_attachment;
#endif
	sema4 *lock = &(vport->gfxdev->shared->modify_lock);

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif
	/* Lock changes to the logical device. Make sure it is unlocked */
	/* everywhere this function returns. */
	if ((ec = sema_p(lock)) != SUCCESS) {
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	/* Call the driver specific function */
	if ((ec = GDV_SET_VPCOLORS(vport, start_entry, num_colors, color_type,
		colors)) != SUCCESS) {
		sema_v(lock);
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	sema_v(lock);
#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return SUCCESS;
}

/*******************************************************************************
* NAME:		_fe_set_vpdmap()
*
* USAGE:	Set the drawmap area to be displayed in a viewport.
*/
error_code _fe_set_vpdmap(GFX_VPORT *vport, const GFX_DMAP *dmap, GFX_POS x,
	GFX_POS y)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GDV_DVATCH *dvatch = vport->gfxdev->shared->gdv_attachment;
#endif
	sema4 *lock = &(vport->gfxdev->shared->modify_lock);

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif
	/* Lock changes to the logical device. Make sure it is unlocked */
	/* everywhere this function returns. */
	if ((ec = sema_p(lock)) != SUCCESS) {
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	/* If the drawmap is NULL then handle it and return */
	if (dmap == NULL) {
		if (vport->shared->active == FALSE) {
			vport->shared->dmap = NULL;
			vport->shared->dmapx = 0;
			vport->shared->dmapy = 0;
			vport->shared->coding_method = GFX_CM_UNKNOWN;
			vport->shared->dmap_width = 1;
			vport->shared->dmap_height = 1;
			vport->shared->dmap_line_size = 1;
			vport->shared->dm2dp_xmul = 1;
			vport->shared->dm2dp_ymul = 1;
		} else {
			sema_v(lock);
#ifndef GDV_FE_SYSTATE
			change_static(app_static);
#endif
			return EOS_MAUI_INUSE;
		}

	} else {

		/* Verify parameters */
		if ((x < 0) || (x >= dmap->width) || (y < 0) || (y >= dmap->height)) {
			sema_v(lock);
#ifndef GDV_FE_SYSTATE
			change_static(app_static);
#endif
			return EOS_MAUI_BADPOS;
		}
	}
	
	/* Call the driver specific function */
	if ((ec = GDV_SET_VPDMAP(vport, dmap, x, y)) != SUCCESS) {
		sema_v(lock);
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	sema_v(lock);
#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return SUCCESS;
}

/*******************************************************************************
* NAME:		_fe_set_vpdmpos()
*
* USAGE:	Set the position of the drawmap in the viewport.
*/
error_code _fe_set_vpdmpos(GFX_VPORT *vport, GFX_POS x, GFX_POS y)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GDV_DVATCH *dvatch = vport->gfxdev->shared->gdv_attachment;
#endif
	const GFX_DMAP *dmap = vport->shared->dmap;
	sema4 *lock = &(vport->gfxdev->shared->modify_lock);

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif
	/* Lock changes to the logical device. Make sure it is unlocked */
	/* everywhere this function returns. */
	if ((ec = sema_p(lock)) != SUCCESS) {
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	/* Verify parameters */
	if (vport->shared->dmap == NULL) {
		sema_v(lock);
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return EOS_MAUI_NODMAP;
	}
	if ((x < 0) || (x >= dmap->width) || (y < 0) || (y >= dmap->height)) {
		sema_v(lock);
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return EOS_MAUI_BADPOS;
	}

	/* Call the driver specific function */
	if ((ec = GDV_SET_VPDMPOS(vport, x, y)) != SUCCESS) {
		sema_v(lock);
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	sema_v(lock);
#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return SUCCESS;
}

#ifdef GDV_SET_VPINTEN

/*******************************************************************************
* NAME:		_fe_set_vpinten()
*
* USAGE:	Set the intensity of the specified viewport.
*/
error_code _fe_set_vpinten(GFX_VPORT *vport, u_int8 intensity)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GDV_DVATCH *dvatch = vport->gfxdev->shared->gdv_attachment;
#endif
	sema4 *lock = &(vport->gfxdev->shared->modify_lock);

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif
	/* Lock changes to the logical device. Make sure it is unlocked */
	/* everywhere this function returns. */
	if ((ec = sema_p(lock)) != SUCCESS) {
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	/* Call the driver specific function */
	if ((ec = GDV_SET_VPINTEN(vport, intensity)) != SUCCESS) {
		sema_v(lock);
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	sema_v(lock);
#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return SUCCESS;
}

#endif

/*******************************************************************************
* NAME:		_fe_restack_vp()
*
* USAGE:	Re-stack a viewport within the stack of viewports.
*/
error_code _fe_restack_vp(GFX_VPORT *vport, GFX_VPORT_PLACEMENT placement,
	GFX_VPORT *ref_vport)
{
	error_code ec;
#ifndef GDV_FE_SYSTATE
	GDV_DVATCH *dvatch = vport->gfxdev->shared->gdv_attachment;
#endif
	sema4 *lock = &(vport->gfxdev->shared->modify_lock);

#ifndef GDV_FE_SYSTATE
	/* Change to the drivers static storage (for globals). Make sure we */
	/* change it back everywhere this function returns. */
	void *app_static = change_static(dvatch->gdv_static);
#endif

	/* Lock changes to the logical device. Make sure it is unlocked */
	/* everywhere this function returns. */
	if ((ec = sema_p(lock)) != SUCCESS) {
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	/* Call the driver specific function */
	if ((ec = GDV_RESTACK_VP(vport, placement, ref_vport)) != SUCCESS) {
		sema_v(lock);
#ifndef GDV_FE_SYSTATE
		change_static(app_static);
#endif
		return ec;
	}

	sema_v(lock);
#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return SUCCESS;
}
