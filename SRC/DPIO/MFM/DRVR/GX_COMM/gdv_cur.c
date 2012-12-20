/*---------------------------------------------------------------------------
*
*
*              Copyright 1998 by Microware Systems Corporation
*                            All Rights Reserved
*                          Reproduced Under License
*
* 
*  This software is confidential property of Microware Systems Corporation, 
*  and is provided under license for internal development purposes only. 
*  Reproduction, publication, modification, distribution, or creation of 
*  derivative worksin any form to any party other than the Licensee is 
*  strictly prohibited, unless expressly provided for under the terms of a 
*  mutually executed written license agreement for this software between the 
*  end-user and Microware Systems Corporation, or otherwise expressly 
*  authorized in writing by Microware Systems Corporation.  Licensee shall 
*  reproduce this copyright notice exactly as provided herein on any copies 
*  of the software that Licensee is authorized to distribute. 
*---------------------------------------------------------------------------- 
* 
* Edition History: 
* #   Date     By  Comments         
* 1   8/12/98  gjb Initial Revision 
*/ 
 
#include <gdv_priv.h> 
#include <memory.h> 
#include <modes.h> 
 
/* code in this file is only active if the following is true */ 
#ifdef GDV_HW_CURSOR 
 
#ifdef GDV_CPATCH_SPECIFICS 
#define CURSOR_ALLOC_SIZE (sizeof(GFX_CURSOR) + sizeof(GDV_CPATCH)) 
#else 
#define CURSOR_ALLOC_SIZE (sizeof(GFX_CURSOR)) 
#endif 
 
 
/******************************************************************************* 
* NAME:		_fe_cursor_set() 
* 
* USAGE:	Set a cursor to be the newly visible one. 
* NOTE:         gfxdev parameter is valid.  cursor parameter may be NULL, but 
*               will be valid if not NULL. 
* 
*/ 
error_code _fe_cursor_set(GFX_DEV *gfxdev, GFX_CURSOR *cursor) 
{ 
	error_code ec=SUCCESS; 
#ifndef GDV_FE_SYSTATE 
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment; 
#endif 
	sema4 *lock = &(gfxdev->shared->modify_lock); 
 
#ifndef GDV_FE_SYSTATE 
	/* Change to the drivers static storage (for globals). Make sure we */ 
	/* change it back everywhere this function returns. */ 
	void *app_static = change_static(dvatch->gdv_static); 
#endif 
 
	/* Lock changes to the logical device. Make sure it is unlocked */ 
	/* everywhere this function returns. */ 
	if ((ec = sema_p(lock)) == SUCCESS) { 
 
		/* Call the driver specific function */ 
		ec = GDV_CURSOR_SET(gfxdev,cursor); 
 
		/* unlock semaphore */ 
		sema_v(lock); 
	} 
 
#ifndef GDV_FE_SYSTATE 
	change_static(app_static); 
#endif 
	return ec; 
} 
 
/******************************************************************************* 
* NAME:		_fe_set_cursor_pos() 
* 
* USAGE:	Set the current cursor position 
* 
* NOTE:		Since this may be a user_state call, do not dereference 
*		any GFX_CURSOR structure because you may not have permissions 
*		to read it. 
*/ 
error_code _fe_cursor_set_pos(GFX_DEV *gfxdev, GFX_POS x, GFX_POS y) 
{ 
	error_code ec=SUCCESS; 
#ifndef GDV_FE_SYSTATE 
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment; 
#endif 
	sema4 *lock = &(gfxdev->shared->modify_lock); 
 
#ifndef GDV_FE_SYSTATE 
	/* Change to the drivers static storage (for globals). Make sure we */ 
	/* change it back everywhere this function returns. */ 
	void *app_static = change_static(dvatch->gdv_static); 
#endif 
 
	/* Lock changes to the logical device. Make sure it is unlocked */ 
	/* everywhere this function returns. */ 
	if ((ec = sema_p(lock)) == SUCCESS) { 
 
		/* Call the driver specific function */ 
		if ((ec = GDV_CURSOR_SET_POS(gfxdev,x,y)) == SUCCESS) { 
			gfxdev->shared->cursor_pos.x = x; 
			gfxdev->shared->cursor_pos.y = y; 
		} 
 
		/* unlock semaphore */ 
		sema_v(lock); 
	} 
 
#ifndef GDV_FE_SYSTATE 
	change_static(app_static); 
#endif 
	return ec; 
} 
 
 
error_code _gdv_curcreate(GFX_DEV *gfxdev, GFX_CURSOR **ret_cursor, 
	const GFX_CURSOR_SPEC *cursor) 
{ 
	error_code ec=SUCCESS; 
	GFX_CURSOR *new_cursor; 
	u_int32 size; 
	sema4 *lock = &(gfxdev->shared->modify_lock); 
 
	/* allocate memory for ret_cursor */ 
	size = CURSOR_ALLOC_SIZE; 
	if ((ec = _os_srqmem(&size, (void*)&new_cursor, 0)) != SUCCESS) 
		return ec; 
 
	#if defined(GDV_MEM_TOP_BIT) 
	new_cursor = (GFX_CURSOR *)adrcvt(new_cursor);	/* SH3 */ 
	#endif 
 
	/* assign members to GFX_CURSOR structure */ 
	new_cursor->sync_code = _GFX_CURSOR_SYNC; 
	#ifdef GDV_CPATCH_SPECIFICS 
	new_cursor->gdv_cursor_attach = ((char*)new_cursor + sizeof(GFX_CURSOR)); 
	#else 
	new_cursor->gdv_cursor_attach = NULL; 
	#endif 
	new_cursor->gfxdev = gfxdev; 
	new_cursor->next = new_cursor->prev = NULL; 
	new_cursor->pid = gfxdev->pid; 
 
	/* Lock changes to the logical device. Make sure it is unlocked */ 
	/* everywhere this function returns. */ 
	if ((ec = sema_p(lock)) == SUCCESS) { 
 
		/* Call the driver specific function */ 
		if ((ec = GDV_CURSOR_CREATE(gfxdev, new_cursor, cursor)) == SUCCESS) { 
			 
			/* add item to gfxdev's cursor list */ 
			GFX_CURSOR *prev_head = gfxdev->shared->cursor_head; 
 
			new_cursor->next = prev_head; 
			gfxdev->shared->cursor_head = new_cursor; 
			if (prev_head != NULL) 
				prev_head->prev = new_cursor; 
		} 
 
		/* unlock semaphore */ 
		sema_v(lock); 
	} 
 
	if (ec == SUCCESS) { 
		_os_permit(new_cursor, CURSOR_ALLOC_SIZE, S_IREAD | S_IWRITE, 
			new_cursor->pid); 
		*ret_cursor = new_cursor; 
	} else { 
		_os_srtmem(CURSOR_ALLOC_SIZE, new_cursor); 
	} 
 
	return ec; 
} 
 
error_code _gdv_curdestroy(GFX_CURSOR *cursor) 
{ 
	error_code ec, result=SUCCESS; 
	sema4 *lock = &(cursor->gfxdev->shared->modify_lock); 
	GFX_DEV *gfxdev = cursor->gfxdev; 
	 
	/* Lock changes to the logical device. Make sure it is unlocked */ 
	/* everywhere this function returns. */ 
	if ((result = sema_p(lock)) == SUCCESS) { 
 
		/* remove cursor from list */ 
		if (cursor == gfxdev->shared->cursor_head) 
			gfxdev->shared->cursor_head = cursor->next; 
		else 
			cursor->prev->next = cursor->next; 
		if (cursor->next != NULL) 
			cursor->next->prev = cursor->prev; 
 
		/* unlock semaphore */ 
		sema_v(lock); 
 
		/* Call the driver specific function */ 
		result = GDV_CURSOR_DESTROY(cursor); 
 
		cursor->sync_code = 0; 
		_os_protect(cursor, CURSOR_ALLOC_SIZE, S_IREAD | S_IWRITE, 
			cursor->pid); 
		#if defined(GDV_MEM_TOP_BIT) 
		cursor = (GFX_CURSOR *)unadrcvt(cursor); 
		#endif 
 
		if ((ec = _os_srtmem(CURSOR_ALLOC_SIZE,cursor)) != SUCCESS) 
			result = ec; 
	} 
 
	return result; 
} 
 
#endif  /* GDV_HW_CURSOR */
