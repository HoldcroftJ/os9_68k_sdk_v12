/*******************************************************************************
* DESCRIPTION :
*
*	This file contains functions that deal with the graphics device structure
*	and its attachment.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  12/04/95  TJE  Converted from earlier work by BAT and CJV
*/
#include <gdv_priv.h>
#include <memory.h>
#include <modes.h>
#include <MAUI/sema4.h>

#ifdef _OSK
#define MODDATA(mod) ((u_int32)((Mh_exec)mod)->_mdata)
#define MODSIZE(mod) ((u_int32)((Mh_com)mod)->_msize)
#else
#define MODDATA(mod) ((u_int32)((Mh_exec)mod)->m_data)
#endif

#define SIZE1 sizeof(GFX_DEV)
#define SIZE2 (sizeof(GFX_DEV_SHARED) + sizeof(GDV_DVATCH))

static void _gdv_insert_dev(Mfm_lu_stat lustat, GFX_DEV *gfxdev, GFX_DEV
	*ref_gfxdev);
static void _gdv_unlink_dev(Mfm_lu_stat lustat, GFX_DEV *gfxdev);
void _gdv_set_topdev(Mfm_lu_stat lustat);

#ifdef GDV_INCLUDE_MEM
static u_int16 shade_hiword = 0;
#endif

#ifndef GDV_SET_VPINTEN
static error_code vpinten_nohwsupport(GFX_VPORT *vport, u_int8 intensity)
{
	return EOS_MAUI_NOHWSUPPORT;
}
#endif

/*******************************************************************************
* NAME:		strncpy()
*
* USAGE:	Copy string function. This function is provided here to avoid
*			pulling in standard library code that would be larger than
*			necessary or cause problems for system state code.
*/
static char *strncpy(char *dest, const char *src, size_t count)
{
	char *dst = dest;

	while (count--) {
		if ((*dst++ = *src++) == 0)
			break;
	}
	return dest;
}

/*******************************************************************************
* NAME:		_gdv_opendev()
*
* USAGE:	Open a logical device on the physical device. The new logical device
*			is placed on top of all others.
*/
error_code _gdv_opendev(Dev_list dev_entry, GFX_DEV **ret_gfxdev,
	Mfm_path_desc path_desc)
{
	error_code ec;
	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);
	u_int32 size;
	GFX_DEV *gfxdev;
	GFX_DEV_SHARED *gfxdev_shared;
	GDV_DVATCH *dvatch;
#ifdef GDV_INCLUDE_MEM
	GFX_DEV *temp_gfxdev;
	GDV_DVATCH *temp_dvatch;
	BOOLEAN hiword_inuse;
#endif

	/* Allocate the GFX_DEV, GFX_DEV_SHARED, and GDV_DVATCH structures. */
	size = SIZE1;
	if ((ec = _os_srqmem(&size, (void *)&gfxdev, 0)) != SUCCESS) {
		return ec;
	}
	#if defined(GDV_MEM_TOP_BIT)
		gfxdev = (GFX_DEV *)adrcvt(gfxdev);	/* SH3 */
	#endif
	size = SIZE2;
	if ((ec = _os_srqmem(&size, (void *)&gfxdev_shared, 0)) != SUCCESS) {
		#if defined(GDV_MEM_TOP_BIT)
			gfxdev = (GFX_DEV *)unadrcvt(gfxdev);	/* SH3 */
		#endif
		_os_srtmem(SIZE1, gfxdev);
		return ec;
	}
	#if defined(GDV_MEM_TOP_BIT)
		gfxdev_shared = (GFX_DEV_SHARED *)adrcvt(gfxdev_shared);	/* SH3 */
	#endif
	dvatch = (GDV_DVATCH *)((char *)gfxdev_shared + sizeof(GFX_DEV_SHARED));
	#if defined(GDV_MEM_TOP_BIT)
		dvatch = (GDV_DVATCH *)adrcvt(dvatch);	/* SH3 */
	#endif

	/* Set members of the GFX_DEV structure */
	gfxdev->api_compat_level = 0;		/* Will be set by the API code */
	gfxdev->gdv_compat_level = MAUI_COMPAT_LEVEL;
	gfxdev->sync_code = _GFX_DEV_SYNC;
	gfxdev->usr_path = 0;				/* Will be set by the API code */
	gfxdev->usr_retrace_evid = 0;		/* Will be set by the API code */
	gfxdev->shared = gfxdev_shared;
	#if defined(GDV_MEM_TOP_BIT)
		gfxdev->shared = (GFX_DEV_SHARED *)adrcvt(gfxdev->shared);	/* SH3 */
	#endif
	gfxdev->pid = path_desc->pd_common.pd_cproc;

	/* Set members of the GFX_DEV_SHARED structure */
	gfxdev_shared->gdv_attachment = dvatch;
	#if defined(GDV_MEM_TOP_BIT)
		gfxdev_shared->gdv_attachment = (GDV_DVATCH *)adrcvt(gfxdev_shared->gdv_attachment);	/* SH3 */
	#endif
	gfxdev_shared->link_count = 1;
	gfxdev_shared->devres = &(gdv_dev_cap.res_info[0]);
	#if defined(GDV_MEM_TOP_BIT)
		gfxdev_shared->devres = (GFX_DEV_RES *)adrcvt(gfxdev_shared->devres);	/* SH3 */
	#endif
	gfxdev_shared->vpmix = OFF;
	gfxdev_shared->extvid = OFF;
	gfxdev_shared->bkcol.color_type = GFX_COLOR_NONE;
	gfxdev_shared->bkcol.color.rgb = 0;
	gfxdev_shared->transcol.color_type = GFX_COLOR_NONE;
	gfxdev_shared->transcol.color.rgb = 0;

	/* Initializing the semaphore for OS-9 */
	ec = sema_init(&gfxdev->shared->modify_lock);
	if(ec != SUCCESS)
	{
	  _os_srtmem(SIZE1, gfxdev);
	  _os_srtmem(SIZE2, gfxdev_shared);
	  return ec;
	}

	#ifdef GDV_FE_SYSTATE
		gfxdev_shared->set_vport_pos = _os_ss_gfx_vppos;
		#if defined(GDV_MEM_TOP_BIT)
			gfxdev_shared->set_vport_pos = 
	        (error_code (*)(GFX_VPORT *, GFX_POS, GFX_POS))adrcvt(gfxdev_shared->set_vport_pos); /* SH3 */
		#endif
		gfxdev_shared->set_vport_size = _os_ss_gfx_vpsize;
		#if defined(GDV_MEM_TOP_BIT)
	        gfxdev_shared->set_vport_size =
	        (error_code (*)(GFX_VPORT *, GFX_DIMEN, GFX_DIMEN))adrcvt(gfxdev_shared->set_vport_size); /* SH3 */
		#endif
		gfxdev_shared->set_vport_state = _os_ss_gfx_vpstate;
		#if defined(GDV_MEM_TOP_BIT)
	        gfxdev_shared->set_vport_state = 
	        (error_code (*)(GFX_VPORT *, BOOLEAN))adrcvt(gfxdev_shared->set_vport_state); /* SH3 */
		#endif
		#ifdef GDV_SET_VPINTEN
			gfxdev_shared->set_vport_intensity = _os_ss_gfx_vpinten;
			#if defined(GDV_MEM_TOP_BIT)
				gfxdev_shared->set_vport_intensity =
	            (error_code (*)(GFX_VPORT *, u_int8))adrcvt(gfxdev_shared->set_vport_intensity); /* SH3 */
			#endif
		#else
			gfxdev_shared->set_vport_intensity = vpinten_nohwsupport;
			#if defined(GDV_MEM_TOP_BIT)
	            gfxdev_shared->set_vport_intensity = 
	            (error_code (*)(GFX_VPORT *, u_int8))adrcvt(gfxdev_shared->set_vport_intensity); /* SH3 */
			#endif
		#endif
		gfxdev_shared->set_vport_dmap = _os_ss_gfx_vpdmap;
		#if defined(GDV_MEM_TOP_BIT)
	        gfxdev_shared->set_vport_dmap = (error_code (*)(GFX_VPORT *, const GFX_DMAP *, GFX_POS, GFX_POS))adrcvt(gfxdev_shared->set_vport_dmap); /* SH3 */
		#endif
		gfxdev_shared->set_vport_dmpos = _os_ss_gfx_vpdmpos;
		#if defined(GDV_MEM_TOP_BIT)
        	gfxdev_shared->set_vport_dmpos = (error_code (*)(GFX_VPORT *, GFX_POS,  GFX_POS))adrcvt(gfxdev_shared->set_vport_dmpos); /* SH3 */ 
		#endif
		gfxdev_shared->restack_vport = _os_ss_gfx_restackvp;
		#if defined(GDV_MEM_TOP_BIT)
        	gfxdev_shared->restack_vport = (error_code (*)(GFX_VPORT *, GFX_VPORT_PLACEMENT, GFX_VPORT *))adrcvt(gfxdev_shared->restack_vport); /* SH3 */
		#endif
		gfxdev_shared->update_display = _os_ss_gfx_updatedpy;
		#if defined(GDV_MEM_TOP_BIT)
        	gfxdev_shared->update_display = (error_code (*)(GFX_DEV *, BOOLEAN))adrcvt(gfxdev_shared->update_display); /* SH3 */
		#endif
		gfxdev_shared->set_vport_colors = _os_ss_gfx_vpcolors;
		#if defined(GDV_MEM_TOP_BIT)
        	gfxdev_shared->set_vport_colors = (error_code (*)(GFX_VPORT *, u_int16, u_int16 , GFX_COLOR_TYPE, void *))adrcvt(gfxdev_shared->set_vport_colors);  /* SH3 */
		#endif

		#ifdef GDV_HW_CURSOR
		gfxdev_shared->set_cursor = _os_ss_gfx_set_cursor;
		#if defined(GDV_MEM_TOP_BIT)
		gfxdev_shared->set_cursor = (error_code (*)(GFX_DEV*, GFX_CURSOR*))adrcvt(gfxdev_shared->set_cursor); /* SH3 */
		#endif
		gfxdev_shared->set_cursor_pos = _os_ss_gfx_set_cursor_pos;
		#if defined(GDV_MEM_TOP_BIT)
		gfxdev_shared->set_cursor_pos = (error_code (*)(GFX_DEV*, GFX_POS,GFX_POS))adrcvt(gfxdev_shared->set_cursor_pos); /* SH3 */
		#endif
		#endif /* GDV_HW_CURSOR */
	#else
		gfxdev_shared->set_vport_pos = _fe_set_vppos;
		#if defined(GDV_MEM_TOP_BIT)
        	gfxdev_shared->set_vport_pos = (error_code (*)(GFX_VPORT *, GFX_POS, GFX_POS))adrcvt(gfxdev_shared->set_vport_pos); /* SH3 */
		#endif
		gfxdev_shared->set_vport_size = _fe_set_vpsize;
		#if defined(GDV_MEM_TOP_BIT)
        	gfxdev_shared->set_vport_size = (error_code (*)(GFX_VPORT *, GFX_DIMEN, GFX_DIMEN))adrcvt(gfxdev_shared->set_vport_size); /* SH3 */
		#endif
		gfxdev_shared->set_vport_state = _fe_set_vpstate;
		#if defined(GDV_MEM_TOP_BIT)
        	gfxdev_shared->set_vport_state = (error_code (*)(GFX_VPORT *, BOOLEAN))adrcvt(gfxdev_shared->set_vport_state); /* SH3 */  
		#endif
		#ifdef GDV_SET_VPINTEN
			gfxdev_shared->set_vport_intensity = _fe_set_vpinten;
			#if defined(GDV_MEM_TOP_BIT)
            	gfxdev_shared->set_vport_intensity = (error_code (*)(GFX_VPORT *, u_int8))adrcvt(gfxdev_shared->set_vport_intensity); /* SH3 */
			#endif
		#else
			gfxdev_shared->set_vport_intensity = vpinten_nohwsupport;
			#if defined(GDV_MEM_TOP_BIT)
            	gfxdev_shared->set_vport_intensity = (error_code (*)(GFX_VPORT *, u_int8))adrcvt(gfxdev_shared->set_vport_intensity); /* SH3 */
			#endif
		#endif
		gfxdev_shared->set_vport_dmap = _fe_set_vpdmap;
		#if defined(GDV_MEM_TOP_BIT)
        	gfxdev_shared->set_vport_dmap = (error_code (*)(GFX_VPORT *, const GFX_DMAP *, GFX_POS, GFX_POS))adrcvt(gfxdev_shared->set_vport_dmap); /* SH3 */
		#endif
		gfxdev_shared->set_vport_dmpos = _fe_set_vpdmpos;
		#if defined(GDV_MEM_TOP_BIT)
        	gfxdev_shared->set_vport_dmpos = (error_code (*)(GFX_VPORT *, GFX_POS,  GFX_POS))adrcvt(gfxdev_shared->set_vport_dmpos); /* SH3 */
		#endif
		gfxdev_shared->restack_vport = _fe_restack_vp;
		#if defined(GDV_MEM_TOP_BIT)
        	gfxdev_shared->restack_vport = (error_code (*)(GFX_VPORT *, GFX_VPORT_PLACEMENT, GFX_VPORT *))adrcvt(gfxdev_shared->restack_vport); /* SH3 */
		#endif
		gfxdev_shared->update_display = _fe_update_dpy;
		#if defined(GDV_MEM_TOP_BIT)
        	gfxdev_shared->update_display = (error_code (*)(GFX_DEV *, BOOLEAN))adrcvt(gfxdev_shared->update_display); /* SH3 */
		#endif
		gfxdev_shared->set_vport_colors = _fe_set_vpcolors;
		#if defined(GDV_MEM_TOP_BIT)
        	gfxdev_shared->set_vport_colors = (error_code (*)(GFX_VPORT *, u_int16, u_int16 , GFX_COLOR_TYPE, void *))adrcvt(gfxdev_shared->set_vport_colors); /* SH3 */
		#endif

		#ifdef GDV_HW_CURSOR
		gfxdev_shared->set_cursor = _fe_cursor_set;
		#if defined(GDV_MEM_TOP_BIT)
		gfxdev_shared->set_cursor = (error_code (*)(GFX_DEV*,GFX_CURSOR*))adrcvt(gfxdev_shared->set_cursor); /* SH3 */
		#endif
		gfxdev_shared->set_cursor_pos = _fe_cursor_set_pos;
		#if defined(GDV_MEM_TOP_BIT)
		gfxdev_shared->set_cursor_pos = (error_code (*)(GFX_DEV*,GFX_POS,GFX_POS))adrcvt(gfxdev_shared->set_cursor_pos);/* SH3 */
		#endif
		#endif /* GDV_HW_CURSOR */

	#endif
	gfxdev_shared->visible = FALSE;
	gfxdev_shared->device_name[0] = '\0';	/* Will be set by the API code */
	if (lustat->irq_evname == NULL) {
		gfxdev_shared->retrace_evname[0] = '\0';
	} else {
		strncpy(gfxdev_shared->retrace_evname, lustat->irq_evname, 11+1);
	}
	gfxdev_shared->vport_head = NULL;
	gfxdev_shared->vport_tail = NULL;
	gfxdev_shared->decode_dmap = NULL;
	gfxdev_shared->cursor_head = NULL;
	gfxdev_shared->cursor_pos.x = gfxdev_shared->cursor_pos.y = 0;

	/* Set members of the GDV_DVATCH structure */
	dvatch->lustat = lustat;
	#if defined(_OS9000_V1) || defined(_OSK)
		dvatch->gdv_static = dev_entry->v_dr_stat;
	#else
		dvatch->gdv_static = (void*)(dev_entry->v_dr_globs);
	#endif
	dvatch->dev_prev = NULL;
	dvatch->dev_next = NULL;
	dvatch->devres_cindex = 0;
	dvatch->devres_nindex = 0;

	#if defined(GDV_MEM_TOP_BIT)
	dvatch->lustat = (Mfm_lu_stat)adrcvt(dvatch->lustat); /* SH3 */
	dvatch->gdv_static = (void *)adrcvt(dvatch->gdv_static); /* SH3 */
        #endif

#ifdef GDV_INCLUDE_MEM
	/* Find next shade hiword that is not being used and assign it to this */
	/* logical device (GFX_DEV) */
	while (1) {		

		/* Increment the shade hiword and increment again if we roll over */
		/* to zero. */
		if (++shade_hiword == 0) {
			shade_hiword = 1;
		}

		/* Search each logical device to see if this hiword is in use */
		temp_gfxdev = lustat->dev_head;
		#if defined(GDV_MEM_TOP_BIT)
        	temp_gfxdev = (GFX_DEV*)adrcvt(temp_gfxdev); /* SH3 */
		#endif
		hiword_inuse = FALSE;
		while (temp_gfxdev != NULL) {
			temp_dvatch = temp_gfxdev->shared->gdv_attachment;
			#if defined(GDV_MEM_TOP_BIT)
            	temp_dvatch = (GDV_DVATCH*)adrcvt(temp_dvatch); /* SH3 */
			#endif
			if (shade_hiword == temp_dvatch->shade_hiword) {
				hiword_inuse = TRUE;
				break;
			}
			temp_gfxdev = temp_dvatch->dev_next;
			#if defined(GDV_MEM_TOP_BIT)
           		temp_gfxdev = (GFX_DEV*)adrcvt(temp_gfxdev); /* SH3 */
			#endif
		}

		/* If hiword is in use, then increment it and try again */
		if (hiword_inuse == TRUE) {
			continue;
		} else {
			/* This hiword is not in use, go ahead and assign it to this */
			/* logical device. */
			dvatch->shade_hiword = shade_hiword;
			break;
		}
	}
#endif

#ifdef GDV_DVATCH_SPECIFICS
	if ((ec = GDV_INIT_DVATCH(dev_entry, gfxdev)) != SUCCESS) {
		#if defined(GDV_MEM_TOP_BIT)
			gfxdev_shared = (GFX_DEV_SHARED *)unadrcvt(gfxdev_shared);	/* SH3 */
			gfxdev = (GFX_DEV *)unadrcvt(gfxdev);						/* SH3 */
		#endif
		_os_srtmem(SIZE2, gfxdev_shared);
		_os_srtmem(SIZE1, gfxdev);
		return ec;
	}
#endif

	/* Add this logical device to the head of the list of logical devices */
	/* on this physical device. */
	_gdv_insert_dev(lustat, gfxdev, NULL);

	/* Show the top logical device on the physical device */
	GDV_SHOW_TOPDEV(lustat);

	/* Permit access to the GFX_DEV and all attached structures */
	_os_permit(gfxdev, SIZE1, S_IREAD | S_IWRITE, gfxdev->pid);
	_os_permit(gfxdev_shared, SIZE2, S_IREAD | S_IWRITE, gfxdev->pid);

	/* Permit access to driver code/data, desc code, and logical unit static */
	_os_permit(dev_entry->v_dr_mod, MODSIZE(dev_entry->v_dr_mod),
		S_IREAD | S_IEXEC, gfxdev->pid); 
	_os_permit(dvatch->gdv_static, MODDATA(dev_entry->v_dr_mod),
		S_IREAD | S_IWRITE, gfxdev->pid); 
	_os_permit(dev_entry->v_dd_mod, MODSIZE(dev_entry->v_dd_mod),
		S_IREAD | S_IEXEC, gfxdev->pid); 
	_os_permit(lustat, dev_entry->v_lu_size, S_IREAD | S_IWRITE, gfxdev->pid);

	/* Set pointer to driver extensions in path descriptor */
	path_desc->pd_drvr_exten = gfxdev;

	/* Return device pointer */
	*ret_gfxdev = gfxdev;
	return SUCCESS;
}

/*******************************************************************************
* NAME:		_gdv_clonedev()
*
* USAGE:	Clone a logical device.
*/
error_code _gdv_clonedev(Dev_list dev_entry, GFX_DEV **ret_gfxdev, GFX_DEV
	*gfxdev, Mfm_path_desc path_desc)
{
	error_code ec;
	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);
	u_int32 size;
	GFX_DEV *new_gfxdev;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;

	/* Allocate the GFX_DEV structure */
	size = SIZE1;
	if ((ec = _os_srqmem(&size, (void *)&new_gfxdev, 0)) != SUCCESS) {
		return ec;
	}
	#if defined(GDV_MEM_TOP_BIT)
		new_gfxdev = (GFX_DEV *)adrcvt(new_gfxdev);		/* SH3 */
	#endif

	/* Set members of the GFX_DEV structure */
	new_gfxdev->api_compat_level = 0;		/* Will be set by the API code */
	new_gfxdev->gdv_compat_level = MAUI_COMPAT_LEVEL;
	new_gfxdev->sync_code = _GFX_DEV_SYNC;
	new_gfxdev->usr_path = 0;				/* Will be set by the API code */
	new_gfxdev->usr_retrace_evid = 0;		/* Will be set by the API code */
	new_gfxdev->shared = gfxdev->shared;
	#if defined(GDV_MEM_TOP_BIT)
    	new_gfxdev->shared = (GFX_DEV_SHARED*)adrcvt(new_gfxdev->shared); /* SH3 */
	#endif
	new_gfxdev->pid = path_desc->pd_common.pd_cproc;

#if defined(GDV_DVATCH_SPECIFICS) && defined(GDV_CLONE_DVATCH)
	if ((ec = GDV_CLONE_DVATCH(dev_entry, gfxdev)) != SUCCESS) {
		#if defined(GDV_MEM_TOP_BIT)
			new_gfxdev = (GFX_DEV *)unadrcvt(new_gfxdev); /* SH3 */
		#endif
		_os_srtmem(size, new_gfxdev);
		return ec;
	}
#endif

	/* Update link count for the logical device */
	gfxdev->shared->link_count++;

	/* Permit access to the GFX_DEV and all attached structures */
	_os_permit(new_gfxdev, SIZE1, S_IREAD | S_IWRITE, new_gfxdev->pid);
	_os_permit(gfxdev->shared, SIZE2, S_IREAD | S_IWRITE, new_gfxdev->pid);

	/* Permit access to driver code/data, desc code, and logical unit static */
	_os_permit(dev_entry->v_dr_mod, MODSIZE(dev_entry->v_dr_mod),
		S_IREAD | S_IEXEC, new_gfxdev->pid); 
	_os_permit(dvatch->gdv_static, MODDATA(dev_entry->v_dr_mod),
		S_IREAD | S_IWRITE, new_gfxdev->pid); 
	_os_permit(dev_entry->v_dd_mod, MODSIZE(dev_entry->v_dd_mod),
		S_IREAD | S_IEXEC, new_gfxdev->pid); 
	_os_permit(lustat, dev_entry->v_lu_size,
		S_IREAD | S_IWRITE, new_gfxdev->pid);

	/* Set pointer to driver extensions in path descriptor */
	path_desc->pd_drvr_exten = new_gfxdev;

	/* Return device pointer */
	*ret_gfxdev = new_gfxdev;
	return SUCCESS;
}

/*******************************************************************************
* NAME:		_gdv_closedev()
*
* USAGE:	Close the specified logical device.
*/
error_code _gdv_closedev(Dev_list dev_entry, GFX_DEV *gfxdev, Mfm_path_desc
	path_desc)
{
	error_code ec, result = SUCCESS;
	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
	GFX_VPORT *vport, *next_vport;
	process_id pid = gfxdev->pid;
        void *local_gdv_static = dvatch->gdv_static;

#ifdef GDV_HW_CURSOR
	GFX_CURSOR *cursors, *tmp;

	/* destroy all cursors created with gfxdev */
	cursors = gfxdev->shared->cursor_head;
	while (cursors != NULL) {
		tmp = cursors;
		cursors = cursors->next;

		if (tmp->gfxdev == gfxdev)
			_gdv_curdestroy(tmp);
	}
#endif

	/* This section is only executed if we are closing the last user of this */
	/* logical device. */
	if (--(gfxdev->shared->link_count) == 0) {
        
            /* Destroy all viewports attached to the device */
            vport = gfxdev->shared->vport_head;
            while (vport != NULL) {
               next_vport = vport->shared->next;
               _gdv_destroyvp(dev_entry, gfxdev, vport);
               vport = next_vport;
            }		
        
            /* Remove this logical device from the list of logical devices on */
            /* this physical device. */
            _gdv_unlink_dev(lustat, gfxdev);
        
            /* Show the top logical device on the physical device */
            GDV_SHOW_TOPDEV(lustat);
        
            /* Terminate use of the GDV_DEV attachment */
            #ifdef GDV_DVATCH_SPECIFICS
            if ((ec = GDV_TERM_DVATCH(dev_entry, gfxdev)) != SUCCESS) {
              result = ec;
            } 
            #endif

            /* Destroying the semaphore for OS-9 */
            ec = sema_term(&gfxdev->shared->modify_lock);
            if(ec != SUCCESS){
              result=ec;
            }

            /* Protect and deallocate memory used for the GDV_DEV_SHARED and */
            /* GDV_DVATCH structures. */
            #if defined(GDV_MEM_TOP_BIT)
                 gfxdev->shared = (GFX_DEV_SHARED *)unadrcvt(gfxdev->shared);
            #endif
 
            _os_protect(gfxdev->shared, SIZE2, S_IREAD | S_IWRITE, pid);
            if ((ec = _os_srtmem(SIZE2, gfxdev->shared)) != SUCCESS) {
               result = ec;
            }

	} else { 
	    #if defined(GDV_MEM_TOP_BIT)
		 gfxdev->shared = (GFX_DEV_SHARED *)unadrcvt(gfxdev->shared);
	    #endif
                 _os_protect(gfxdev->shared, SIZE2, S_IREAD | S_IWRITE, pid);
	}
    
	/* Protect access to driver code/data, desc code, and logical unit static */
	_os_protect(dev_entry->v_dr_mod, MODSIZE(dev_entry->v_dr_mod),
                S_IREAD | S_IEXEC, pid); 
	_os_protect(local_gdv_static, MODDATA(dev_entry->v_dr_mod),
                S_IREAD | S_IWRITE, pid); 
	_os_protect(dev_entry->v_dd_mod, MODSIZE(dev_entry->v_dd_mod),
                S_IREAD | S_IEXEC, pid); 
	_os_protect(lustat, dev_entry->v_lu_size, S_IREAD | S_IWRITE, pid); 
    
	/* Protect and deallocate memory used for the GDV_DEV and all attached */
	/* structures. */
	gfxdev->sync_code = 0;
	_os_protect(gfxdev, SIZE1, S_IREAD | S_IWRITE, pid);
	#if defined(GDV_MEM_TOP_BIT)
		gfxdev = (GFX_DEV *)unadrcvt(gfxdev);
	#endif
	if ((ec = _os_srtmem(SIZE1, gfxdev)) != SUCCESS) {
		result = ec;
	}
    
	/* Clear pointer to driver extensions in path descriptor */
	path_desc->pd_drvr_exten = 0;
    
	return result;
}

/*******************************************************************************
* NAME:		_gdv_restackdev()
*
* USAGE:	Restack a logical device within the stack of logical devices open
*			on this physical device.
*/
error_code _gdv_restackdev(Dev_list dev_entry, GFX_DEV *gfxdev,
	GFX_DEV_PLACEMENT placement, GFX_DEV *ref_gfxdev)
{
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
	Mfm_lu_stat lustat = dvatch->lustat;
	GFX_DEV *dptr;
	GFX_DEV *ins_gfxdev = NULL;
	BOOLEAN found_gfxdev = FALSE, found_ref_gfxdev = FALSE;

	/* Make sure both gfxdev and ref_gfxdev are assigned to this physical */
	/* device. */
	if (ref_gfxdev == NULL) {
		found_ref_gfxdev = TRUE;
	}
	dptr = lustat->dev_head;
	while (dptr != NULL) {
		if (gfxdev == dptr) {
			found_gfxdev = TRUE;
		}
		if (ref_gfxdev == dptr) {
			found_ref_gfxdev = TRUE;
		}
		dptr = ((GDV_DVATCH *)(dptr->shared->gdv_attachment))->dev_next;
	}
	if ((found_gfxdev == FALSE) || (found_ref_gfxdev == FALSE)) {
		return EOS_MAUI_NOTFOUND;
	}

	/* Find the insertion point. This is the device that will be immediately */
	/* in front of "gfxdev". Set it to NULL if "gfxdev" is to be in front. */
	switch (placement) {
	  case GFX_DEV_FRONT:		/* In front of all others */
	  	ins_gfxdev = NULL;
	  	break;
	  case GFX_DEV_BACK:		/* In back of all others */
	  	ins_gfxdev = lustat->dev_tail;
	  	break;
	  case GFX_DEV_FRONT_OF:	/* In front of "ref_gfxdev" */
	  	ins_gfxdev =
			((GDV_DVATCH *)(ref_gfxdev->shared->gdv_attachment))->dev_prev;
	  	break;
	  case GFX_DEV_BACK_OF:		/* In back of "ref_gfxdev" */
	  	ins_gfxdev = ref_gfxdev;
	  	break;
	}

	if(ins_gfxdev == gfxdev) /* Already in place */
		return SUCCESS;

	/* Unlink the logical device from its current location in the stack */
	_gdv_unlink_dev(lustat, gfxdev);
	
	/* Insert the logical device into its new location in the stack */
	_gdv_insert_dev(lustat, gfxdev, ins_gfxdev);

	/* Show the top device */
	GDV_SHOW_TOPDEV(lustat);

	return SUCCESS;
}

/*******************************************************************************
* NAME:		_gdv_insert_dev()
*
* USAGE:	Insert a logical device into the stack of logical devices currently
*			open on a physical device. If ref_deatch is NULL then insert the
*			device at the head of the list, otherwise insert it after this
*			reference device.
*/
static void _gdv_insert_dev(Mfm_lu_stat lustat, GFX_DEV *gfxdev, GFX_DEV
	*ref_gfxdev)
{
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
	GDV_DVATCH *ref_dvatch;
    sema4 *lock = &(gfxdev->shared->modify_lock);

    sema_p(lock);

	/* Link into linked list of "next" pointers */
	if (ref_gfxdev == NULL) {
		dvatch->dev_next = lustat->dev_head;
		lustat->dev_head = gfxdev;
		lustat->dev_head_shared = gfxdev->shared;
	} else {
		ref_dvatch = ref_gfxdev->shared->gdv_attachment;
		dvatch->dev_next = ref_dvatch->dev_next;
		ref_dvatch->dev_next = gfxdev;
		dvatch->dev_prev = ref_gfxdev;
	}

	/* Link into linked list of "previous" pointers */
	if (dvatch->dev_next == NULL) {
		lustat->dev_tail = gfxdev;
	} else {
		GDV_DVATCH *next_dvatch = dvatch->dev_next->shared->gdv_attachment;
		next_dvatch->dev_prev = gfxdev;
	}

	/* Set top visible device */
    _gdv_set_topdev(lustat);

    sema_v(lock);
}

/*******************************************************************************
* NAME:		_gdv_unlink_dev()
*
* USAGE:	Unlink a logical device from the stack of logical devices currently
*			open on a physical device.
*/
static void _gdv_unlink_dev(Mfm_lu_stat lustat, GFX_DEV *gfxdev)
{
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
    sema4 *lock = &(gfxdev->shared->modify_lock);

    sema_p(lock);

	/* Unlink "next" pointer */
	if (dvatch->dev_next == NULL) {
		lustat->dev_tail = dvatch->dev_prev;
	} else {
		GDV_DVATCH *next_dvatch = dvatch->dev_next->shared->gdv_attachment;
		next_dvatch->dev_prev = dvatch->dev_prev;
	}

	/* Unlink "prev" pointer */
	if (dvatch->dev_prev == NULL) {
		lustat->dev_head = dvatch->dev_next;
		lustat->dev_head_shared = dvatch->dev_next->shared;
	} else {
		GDV_DVATCH *prev_dvatch = dvatch->dev_prev->shared->gdv_attachment;
		prev_dvatch->dev_next = dvatch->dev_next;
	}

	/* Clear the "next" and "prev" pointers */
	dvatch->dev_next = dvatch->dev_prev = NULL;

	/* Set top visible device */
    _gdv_set_topdev(lustat);

    sema_v(lock);
}

/*******************************************************************************
* NAME:		_gdv_devres()
*
* USAGE:	Set the resolution for a logical device.
*/
error_code _gdv_devres(Dev_list dev_entry, GFX_DEV *gfxdev, GFX_DIMEN
	disp_width, GFX_DIMEN disp_height, GFX_INTL_MODE intl_mode, u_int16
	refresh_rate)
{
  /*	error_code ec, error;*/
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
	int i;
	BOOLEAN found_res = FALSE;
	u_int8 devres_index = 0;
	GFX_VPORT *vport;

	/* Determine index for the specified resolution */
	for (i = 0; i < gdv_dev_cap.num_res; i++) {
		if ((disp_width   == gdv_dev_cap.res_info[i].disp_width)   &&
			(disp_height  == gdv_dev_cap.res_info[i].disp_height)  &&
			(intl_mode	== gdv_dev_cap.res_info[i].intl_mode) &&
			(refresh_rate == gdv_dev_cap.res_info[i].refresh_rate)) {
			found_res = TRUE;
			devres_index = i;
			break;
		}
	}

	/* Return an error if the resolution is not valid */
	if (found_res == FALSE) {
		return EOS_MAUI_NOHWSUPPORT;
	}

	/* Return an error if this logical device has any active viewports */
	vport = gfxdev->shared->vport_head;
	while (vport != NULL) {
		if (vport->shared->active == TRUE) {
			return EOS_MAUI_CANTRESIZE;
		}
		vport = vport->shared->next;
	}

	/* Set the index to be used next time the display is updated */
	gfxdev->shared->devres = &gdv_dev_cap.res_info[devres_index];
	#if defined(GDV_MEM_TOP_BIT)
    	gfxdev->shared->devres = (GFX_DEV_RES*)adrcvt(gfxdev->shared->devres); /* SH3 */
	#endif
	dvatch->devres_nindex = devres_index;

	return SUCCESS;
}

/*******************************************************************************
* NAME:		_fe_update_dpy()
*
* USAGE:	Update the display.
*/
error_code _fe_update_dpy(GFX_DEV *gfxdev, BOOLEAN sync)
{
	error_code ec;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
    Mfm_lu_stat lustat = dvatch->lustat;
	sema4 *lock = &(gfxdev->shared->modify_lock);

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

	/* Mark the logical device as visible and re-compute the top visible */
	/* device. */
	gfxdev->shared->visible = TRUE;
    /* Instead of calling _gdv_set_topdev(dvatch->lustat) to set
       topdev (which is dangerous in user state), we can set
       directly. But we only need to set it when we are the top
       device. */
    if (gfxdev->shared == lustat->dev_head_shared) {
      lustat->topdev = gfxdev;
      /* place shared in a accessible memory for other processes */
      lustat->topdev_shared = gfxdev->shared;
    }

	/* Make the queued-up (next) resolution the current one before calling */
	/* the driver specific code. */
	dvatch->devres_cindex = dvatch->devres_nindex;
	
	/* Call the driver specific function */
	ec = GDV_UPDATE_DPY(gfxdev, sync);

	sema_v(lock);
#ifndef GDV_FE_SYSTATE
	change_static(app_static);
#endif
	return ec;
}

/*******************************************************************************
* NAME:		_gdv_set_topdev()
*
* USAGE:	Determine which logical device is now on top and set the "topdev"
*			member of the logical unit static.
*/
void _gdv_set_topdev(Mfm_lu_stat lustat)
{
	GFX_DEV *topdev = lustat->dev_head;

	/* Search for the first visible device */
	while (topdev != NULL) {
		if (topdev->shared->visible == TRUE) {
			/* found it */
			break;
		} else {
			/* try the next one */
			topdev = ((GDV_DVATCH *)(topdev->shared->gdv_attachment))->dev_next;
		}
	}

	/* Set the topdev member of the logical unit static */
	lustat->topdev = topdev;
    /* place shared in a accessible memory for other processes */
    lustat->topdev_shared = topdev->shared;
}
