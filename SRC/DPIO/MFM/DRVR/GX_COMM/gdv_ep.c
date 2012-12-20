/*******************************************************************************
* DESCRIPTION :
*
*	This file contains the main entry points for a MAUI graphics driver.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  11/20/95  TJE  Converted from earlier work by BAT and CJV
*       2  10/20/98  AND  H/W BLT layer
*       3  04/11/00  JPH  Added dr_read_hw and dr_write_hw
*       4  12/18/00  SRM  CF2120 Add extended device capabilities
*       5  01/02/01	 RRY  Fixing extended device capabilites when not defined
*/
#define GDV_MAIN
#include <gdv_priv.h>
#include <events.h>
#include <modes.h>

extern char _maui_version[];

/*******************************************************************************
* NAME:		dr_iniz()
*
* USAGE:	Initialize the graphics device.
*/
error_code dr_iniz(Dev_list dev_entry)
{
	error_code ec;
	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);
	u_int32 perm;

	/* THIS CODE IS ONLY HERE SO THAT THE VERSION LABEL IS REFERENCED IN */
	/* SUCH A WAY THAT THE OPTIMIZER WILL NEVER DISCARD IT. */
	if (_maui_version[0] != '@') {
		return EOS_MAUI_INTERNAL;
	}

	/* Force values in lustat that must have specific initial values */
	lustat->dev_head = NULL;
	lustat->dev_tail = NULL;
	lustat->topdev = NULL;
	lustat->irq_event_id = 0;

	/* Update globals from information in the lustat */
	gdv_dev_cap.hw_subtype = lustat->hw_subtype_name;
	#if defined(GDV_MEM_TOP_BIT)
		gdv_dev_cap.hw_subtype = (char *)adrcvt(gdv_dev_cap.hw_subtype); /* SH3 */
	#endif

	/* Initialize the Memory API if it is used */
	#ifdef GDV_INCLUDE_MEM
	if ((ec = mem_init()) != SUCCESS) {
		return ec;
	}
	#endif

	/* Setup the retrace IRQ event */
	if (lustat->irq_evname != NULL) {
		perm = MP_OWNER_READ | MP_OWNER_WRITE | MP_GROUP_READ | MP_GROUP_WRITE |
			   MP_WORLD_READ | MP_WORLD_WRITE;
		if ((ec = _os_ev_creat(0, 0, perm, &(lustat->irq_event_id),
			lustat->irq_evname, 0, 0)) != SUCCESS) {
			#ifdef GDV_INCLUDE_MEM
			mem_term();
			#endif
			return ec;
		}
	}

	/* Initialize the graphics hardware */
	if ((ec = GDV_INIT_HW(dev_entry)) != SUCCESS) {
		if (lustat->irq_evname != NULL) {
			_os_ev_unlink(lustat->irq_event_id);
			while ((ec = _os_ev_delete(lustat->irq_evname)) 
                                          == EOS_EVBUSY) {
                            _os_ev_unlink(lustat->irq_event_id);
			}
		}
		#ifdef GDV_INCLUDE_MEM
		mem_term();
		#endif
		return ec;
	}

	/* Initialize IRQs for the graphics device */
	#ifdef GDV_INIT_IRQS
	if (lustat->irq_evname != NULL) {
		if ((ec = GDV_INIT_IRQS(dev_entry)) != SUCCESS) {
			GDV_TERM_HW(dev_entry);
			_os_ev_unlink(lustat->irq_event_id);
			while ((ec = _os_ev_delete(lustat->irq_evname)) 
                                          == EOS_EVBUSY) {
                            _os_ev_unlink(lustat->irq_event_id);
			}
			#ifdef GDV_INCLUDE_MEM
			mem_term();
			#endif
			return ec;
		}
	}
	#endif

	/* Show the top logical device (which is currently NULL) */
	GDV_SHOW_TOPDEV(dev_entry->v_lu_stat);

	return SUCCESS;
}

/*******************************************************************************
* NAME:		dr_term()
*
* USAGE:	Terminate use of the graphics device.
*/
error_code dr_term(Dev_list dev_entry)
{
	error_code ec, result = SUCCESS;
	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);

	/* Terminate use of interrupts on the graphics device */
	#ifdef GDV_TERM_IRQS
	if ((ec = GDV_TERM_IRQS(dev_entry)) != SUCCESS) {
		result = ec;
	}
	#endif
	
	/* Terminate use of the graphics hardware */
	if ((ec = GDV_TERM_HW(dev_entry)) != SUCCESS) {
		result = ec;
	}

	/* Remove the retrace IRQ event */
	if (lustat->irq_evname != NULL) {
		if ((ec = _os_ev_unlink(lustat->irq_event_id)) != SUCCESS) {
			result = ec;
		}
		while ((ec = _os_ev_delete(lustat->irq_evname)) 
                                                           == EOS_EVBUSY) {
                        result = ec;
		        if ((ec = _os_ev_unlink(lustat->irq_event_id)) 
                                        != SUCCESS) {
			        result = ec;
		        }
		}
	}

	/* Initialize the Memory API if it is used */
	#ifdef GDV_INCLUDE_MEM
	if ((ec = mem_term()) != SUCCESS) {
		result = ec;
	}
	#endif

	return result;
}

/*******************************************************************************
* NAME:		dr_getstat()
*
* USAGE:	Get status entry point.
*/
error_code dr_getstat(Dev_list dev_entry, gs_mod_pb *gs_pb)
{
    error_code result = SUCCESS;
    i_getstat_pb *mfm_pb;
    u_int32 function_code;
    
    /* Switch based on getstat code */	
    switch (gs_pb->mod_code) {
  case SS_MAUI:
      
      /* First entry in the parameter block is the MAUI function code */
      mfm_pb = gs_pb->mod_param;
      function_code = *((u_int16 *)(mfm_pb->param_blk)); 
      
      /* Switch based on the MAUI function code */	
      switch (function_code) {
      case FC_GFX_DEVCAP: {
          gs_gfx_devcap_pb *func_pb = mfm_pb->param_blk;
          *(func_pb->ret_dev_cap) = &gdv_dev_cap;
		  #if defined(GDV_MEM_TOP_BIT)
			*(func_pb->ret_dev_cap) = (GFX_DEV_CAP *)adrcvt(*(func_pb->ret_dev_cap));
		  #endif		
          break;
      }
      case FC_GFX_DEVCAPEXTEN: {
          #ifdef GDV_DEVCAPEXTEN
            gs_gfx_devcapexten_pb *func_pb = mfm_pb->param_blk;

            *(func_pb->ret_dev_capexten) = &gdv_dev_capexten;
		    #if defined(GDV_MEM_TOP_BIT)
			  *(func_pb->ret_dev_capexten) = (GFX_DEV_CAPEXTEN *)adrcvt(*(func_pb->ret_dev_capexten));
		    #endif		
          #else
            result = EOS_MAUI_NODVSUPPORT; /* driver specific code does not support */
          #endif
          break;
      }
      case FC_GFX_COMPAT: {
          gs_gfx_compat_pb *func_pb = mfm_pb->param_blk;
          *(func_pb->ret_gdv_compat) = MAUI_COMPAT_LEVEL;
          if (func_pb->api_compat < 21) {
              result = EOS_MAUI_INCOMPATVER;
          }
          break;
      }

      #ifdef GDV_HW_CURSOR	
      case FC_GFX_CURCAP: {
          gs_gfx_curcap_pb *func_pb = mfm_pb->param_blk;
          *(func_pb->ret_cursor_cap) = &gdv_cursor_cap;
      #if defined(GDV_MEM_TOP_BIT)
           *(func_pb->ret_cursor_cap) = (GFX_CURSOR_CAP *)adrcvt(*(func_pb->ret_cursor_cap));
      #endif		
	   break;
      }
      #endif /* GDV_HW_CURSOR */

      #ifdef GDV_FE_SYSTATE
      #ifdef GDV_INCLUDE_IOBLT
      case FC_IOBLT_GETPIXEL: {
          gs_blt_getpixel_pb *func_pb = mfm_pb->param_blk;
          result = _gdv_ioblt_getpixel(func_pb->ret_pixel, func_pb->bc,
                                       func_pb->srcx, func_pb->srcy);
          break;
      }
      #endif  /**GDV_INCLUDE_IOBLT**/
      #ifdef GDV_INCLUDE_HWBLT
      #ifdef GDV_HWBLT_GETPIXEL
      case FC_HWBLT_GETPIXEL: {
          gs_blt_getpixel_pb *func_pb = mfm_pb->param_blk;
          result = GDV_HWBLT_GETPIXEL(func_pb->ret_pixel, func_pb->bc,
                                       func_pb->srcx, func_pb->srcy);
          break;
      }
      #endif
      #endif  /**GDV_INCLUDE_HWBLT**/
      #endif  /**GDV_FE_SYSTATE**/

      #ifdef GDV_GET_ATTRIBUTE
      case FC_GFX_ATTRIBUTE: {
          gs_gfx_attribute_pb *func_pb = mfm_pb->param_blk;
          result = GDV_GET_ATTRIBUTE(func_pb->gfxdev, func_pb->ret_dev_attr,
                                     func_pb->attr_type);
          break;
      }
      #endif

      default :				/* Unknown MAUI getstat */
#ifdef GDV_USER_GETSTAT
          result = GDV_USER_GETSTAT(dev_entry, gs_pb);
#else
          result = EOS_UNKSVC;
#endif
          break;
      }
      break;
      
    default :						/* Unknown getstat */
#ifdef GDV_USER_GETSTAT
        result = GDV_USER_GETSTAT(dev_entry, gs_pb);
#else
        result = EOS_UNKSVC;
#endif
        break;
	}
	
	return result;
}

/*******************************************************************************
* NAME:		dr_setstat()
*
* USAGE:	Set status entry point.
*/
error_code dr_setstat(Dev_list dev_entry, ss_mod_pb *ss_pb)
{
	error_code result = SUCCESS;
	i_setstat_pb *mfm_pb;
	u_int32 function_code;
	Mfm_path_desc path_desc = ss_pb->mod_pd;

	/* Switch based on setstat code */	
	switch (ss_pb->mod_code) {

	case SS_OPEN:
		/* Clear pointer to driver extensions just in case it doesn't get set */
		path_desc->pd_drvr_exten = 0;
		break;
	case SS_CLOSE:
		/* Close the device if it has not been done yet */
		if (path_desc->pd_drvr_exten != 0) {
			return _gdv_closedev(dev_entry, path_desc->pd_drvr_exten,
				path_desc);
		}
		break;

	case SS_MAUI:

		/* First entry in the parameter block is the MAUI function code */
		mfm_pb = ss_pb->mod_param;
		function_code = *((u_int16 *)(mfm_pb->param_blk)); 

		/* Switch based on the MAUI function code */	
		switch (function_code) {
			case FC_GFX_OPENDEV: {
				ss_gfx_opendev_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_opendev(dev_entry, func_pb->ret_gfxdev,
					path_desc);
				break;
			}
			case FC_GFX_CLONEDEV: {
				ss_gfx_clonedev_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_clonedev(dev_entry, func_pb->ret_gfxdev,
					func_pb->gfxdev, path_desc);
				break;
			}
			case FC_GFX_CLOSEDEV: {
				ss_gfx_closedev_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_closedev(dev_entry, func_pb->gfxdev, path_desc);
				break;
			}
			case FC_GFX_RESTACKDEV: {
				ss_gfx_restackdev_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_restackdev(dev_entry, func_pb->gfxdev,
					func_pb->placement, func_pb->ref_gfxdev);
				break;
			}
			case FC_GFX_DEVRES: {
				ss_gfx_devres_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_devres(dev_entry, func_pb->gfxdev,
					func_pb->disp_width, func_pb->disp_height,
					func_pb->intl_mode, func_pb->refresh_rate);
				break;
			}
			case FC_GFX_CREATEVP: {
				ss_gfx_createvp_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_createvp(dev_entry, func_pb->gfxdev,
 					func_pb->ret_vport, func_pb->x, func_pb->y, func_pb->width,
					func_pb->height, func_pb->placement, func_pb->ref_vport,
					path_desc);
				break;
			}
			case FC_GFX_CLONEVP: {
				ss_gfx_clonevp_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_clonevp(dev_entry, func_pb->ret_vport,
					func_pb->vport, path_desc);
				break;
			}
			case FC_GFX_DESTROYVP: {
				ss_gfx_destroyvp_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_destroyvp(dev_entry, func_pb->gfxdev,
					func_pb->vport);
				break;
			}
			#ifdef GDV_INCLUDE_MEM
			case FC_GFX_ALLOC: {
				ss_gfx_allocmem_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_alloc_mem(dev_entry, func_pb->gfxdev,
					func_pb->size, func_pb->mem_ptr, func_pb->color);
				break;
			}
			case FC_GFX_DEALLOC: {
				ss_gfx_deallocmem_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_dealloc_mem(dev_entry, func_pb->gfxdev,
					func_pb->size, func_pb->mem_ptr, func_pb->color);
				break;
			}
			#endif
			#ifdef GDV_SET_BKCOL
			case FC_GFX_BKCOL: {
				ss_gfx_bkcol_pb *func_pb = mfm_pb->param_blk;
				result = GDV_SET_BKCOL(func_pb->gfxdev, func_pb->bkcol);
				break;
			}
			#endif
			#ifdef GDV_SET_EXTVID
			case FC_GFX_EXTVID: {
				ss_gfx_extvid_pb *func_pb = mfm_pb->param_blk;
				result = GDV_SET_EXTVID(func_pb->gfxdev, func_pb->extvid);
				break;
			}
			#endif
			#ifdef GDV_SET_TRANSCOL
			case FC_GFX_TRANSCOL: {
				ss_gfx_transcol_pb *func_pb = mfm_pb->param_blk;
				result = GDV_SET_TRANSCOL(func_pb->gfxdev, func_pb->transcol);
				break;
			}
			#endif
			#ifdef GDV_SET_VPMIX
			case FC_GFX_VPMIX: {
				ss_gfx_vpmix_pb *func_pb = mfm_pb->param_blk;
				result = GDV_SET_VPMIX(func_pb->gfxdev, func_pb->vpmix);
				break;
			}
			#endif
			#ifdef GDV_DECODE_DST
			case FC_GFX_DECODEDST: {
				ss_gfx_decodedst_pb *func_pb = mfm_pb->param_blk;
				result = GDV_DECODE_DST(func_pb->gfxdev, func_pb->decode_dmap);
				break;
			}
			#endif
			#if defined(GDV_INCLUDE_IOBLT) || defined(GDV_INCLUDE_HWBLT)
			case FC_BLT_CREATEBC: {
				ss_blt_createbc_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_blt_createbc(dev_entry, func_pb->gfxdev,
					func_pb->context);
				break;
			}
			case FC_BLT_DESTROYBC: {
				ss_blt_destroybc_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_blt_destroybc(dev_entry, func_pb->gfxdev,
					func_pb->context);
				break;
			}
			#endif
			#ifdef GDV_FE_SYSTATE
			case FC_GFX_UPDATEDPY: {
				ss_gfx_updatedpy_pb *func_pb = mfm_pb->param_blk;
				result = _fe_update_dpy(func_pb->gfxdev, func_pb->sync);
				break;
			}
			case FC_GFX_VPPOS: {
				ss_gfx_vppos_pb *func_pb = mfm_pb->param_blk;
				result = _fe_set_vppos(func_pb->vport, func_pb->x, func_pb->y);
				break;
			}
			case FC_GFX_VPSIZE: {
				ss_gfx_vpsize_pb *func_pb = mfm_pb->param_blk;
				result = _fe_set_vpsize(func_pb->vport, func_pb->width,
					func_pb->height);
				break;
			}
			case FC_GFX_VPSTATE: {
				ss_gfx_vpstate_pb *func_pb = mfm_pb->param_blk;
				result = _fe_set_vpstate(func_pb->vport, func_pb->active);
				break;
			}
			#ifdef GDV_SET_VPINTEN
			case FC_GFX_VPINTEN: {
				ss_gfx_vpinten_pb *func_pb = mfm_pb->param_blk;
				result = _fe_set_vpinten(func_pb->vport, func_pb->intensity);
				break;
			}
			#endif
			case FC_GFX_VPCOLORS: {
				ss_gfx_vpcolors_pb *func_pb = mfm_pb->param_blk;
				result = _fe_set_vpcolors(func_pb->vport, func_pb->start_entry,
					func_pb->num_colors, func_pb->color_type, func_pb->colors);
				break;
			}
			case FC_GFX_VPDMAP: {
				ss_gfx_vpdmap_pb *func_pb = mfm_pb->param_blk;
				result = _fe_set_vpdmap(func_pb->vport, func_pb->dmap,
					func_pb->x, func_pb->y);
				break;
			}
			case FC_GFX_VPDMPOS: {
				ss_gfx_vpdmpos_pb *func_pb = mfm_pb->param_blk;
				result = _fe_set_vpdmpos(func_pb->vport, func_pb->x,
					func_pb->y);
				break;
			}
			case FC_GFX_RESTACKVP: {
				ss_gfx_restackvp_pb *func_pb = mfm_pb->param_blk;
				result = _fe_restack_vp(func_pb->vport, func_pb->placement,
					func_pb->ref_vport);
				break;
			}
			#ifdef GDV_HW_CURSOR
			case FC_GFX_SETCURSOR: {
				ss_gfx_setcursor_pb *func_pb = mfm_pb->param_blk;
				result = _fe_cursor_set(func_pb->gfxdev,func_pb->cursor);
				break;
			}
			case FC_GFX_SETCURSORPOS: {
				ss_gfx_setcursorpos_pb *func_pb = mfm_pb->param_blk;
				result = _fe_cursor_set_pos(func_pb->gfxdev,
					func_pb->x, func_pb->y);
				break;
			}
			case FC_GFX_CURCREATE: {
				ss_gfx_curcreate_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_curcreate(func_pb->gfxdev,
					func_pb->cursor, func_pb->cursor_spec);
				break;
			}
			case FC_GFX_CURDESTROY: {
				ss_gfx_curdestroy_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_curdestroy(func_pb->cursor);
				break;
			}
			#endif
			#if defined(GDV_INCLUDE_IOBLT) || defined(GDV_INCLUDE_HWBLT)
			case FC_BLT_SETCPYMIX: {
				ss_blt_setcpymix_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_blt_cpymix(func_pb->bc);
				break;
			}
			case FC_BLT_SETEXPMIX: {
				ss_blt_setexpmix_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_blt_expmix(func_pb->bc);
				break;
			}
			case FC_BLT_SETDRWMIX: {
				ss_blt_setdrwmix_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_blt_drwmix(func_pb->bc);
				break;
			}
			case FC_BLT_SETPIX: {
				ss_blt_setpix_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_blt_pix(func_pb->bc);
				break;
			}
			case FC_BLT_SETSRC: {
				ss_blt_setsrc_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_blt_src(func_pb->bc);
				break;
			}
			case FC_BLT_SETEXPTBL: {
				ss_blt_setexptbl_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_blt_exptbl(func_pb->bc);
				break;
			}
			case FC_BLT_SETTRANS: {
				ss_blt_settrans_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_blt_trans(func_pb->bc);
				break;
			}
			case FC_BLT_SETMASK: {
				ss_blt_setmask_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_blt_mask(func_pb->bc);
				break;
			}
			case FC_BLT_SETOFS: {
				ss_blt_setofs_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_blt_ofs(func_pb->bc);
				break;
			}
			case FC_BLT_SETDST: {
				ss_blt_setdst_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_blt_dst(func_pb->bc);
				break;
			}
                  #ifdef GDV_INCLUDE_IOBLT
			case FC_IOBLT_COPYBLK: {
				ss_blt_copyblk_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_ioblt_copyblk(func_pb->bc, func_pb->dstx,
					func_pb->dsty, func_pb->srcx, func_pb->srcy,
					func_pb->width, func_pb->height);
				break;
			}
			case FC_IOBLT_COPYNBLK: {
				ss_blt_copynblk_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_ioblt_copynblk(func_pb->bc, func_pb->dstx,
					func_pb->srcx, func_pb->height);
				break;
			}
			case FC_IOBLT_EXPDBLK: {
				ss_blt_expdblk_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_ioblt_expdblk(func_pb->bc, func_pb->dstx,
					func_pb->dsty, func_pb->srcx, func_pb->srcy,
					func_pb->width, func_pb->height);
				break;
			}
			case FC_IOBLT_EXPDNBLK: {
				ss_blt_expdnblk_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_ioblt_expdnblk(func_pb->bc, func_pb->dstx,
					func_pb->srcx, func_pb->height);
				break;
			}
			case FC_IOBLT_DRAWBLK: {
				ss_blt_drawblk_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_ioblt_drawblk(func_pb->bc, func_pb->dstx,
					func_pb->dsty, func_pb->width, func_pb->height);
				break;
			}
			case FC_IOBLT_DRAWHLINE: {
				ss_blt_drawhline_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_ioblt_drawhline(func_pb->bc, func_pb->dstx,
					func_pb->dsty, func_pb->width);
				break;
			}
			case FC_IOBLT_DRAWVLINE: {
				ss_blt_drawvline_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_ioblt_drawvline(func_pb->bc, func_pb->dstx,
					func_pb->dsty, func_pb->height);
				break;
			}
			case FC_IOBLT_DRAWPIXEL: {
				ss_blt_drawpixel_pb *func_pb = mfm_pb->param_blk;
				result = _gdv_ioblt_drawpixel(func_pb->bc, func_pb->dstx,
					func_pb->dsty);
				break;
			}
                  #endif  /**GDV_INCLUDE_IOBLT**/
			#ifdef GDV_INCLUDE_HWBLT
                  #ifdef GDV_HWBLT_COPYBLK
			case FC_HWBLT_COPYBLK: {
				ss_blt_copyblk_pb *func_pb = mfm_pb->param_blk;
				result = GDV_HWBLT_COPYBLK(func_pb->bc, func_pb->dstx,
					func_pb->dsty, func_pb->srcx, func_pb->srcy,
					func_pb->width, func_pb->height);
				break;
			}
                  #endif
                  #ifdef GDV_HWBLT_COPYNBLK
			case FC_HWBLT_COPYNBLK: {
				ss_blt_copynblk_pb *func_pb = mfm_pb->param_blk;
				result = GDV_HWBLT_COPYNBLK(func_pb->bc, func_pb->dstx,
					func_pb->srcx, func_pb->height);
				break;
			}
                  #endif
                  #ifdef GDV_HWBLT_EXPDBLK
			case FC_HWBLT_EXPDBLK: {
				ss_blt_expdblk_pb *func_pb = mfm_pb->param_blk;
				result = GDV_HWBLT_EXPDBLK(func_pb->bc, func_pb->dstx,
					func_pb->dsty, func_pb->srcx, func_pb->srcy,
					func_pb->width, func_pb->height);
				break;
			}
                  #endif
                  #ifdef GDV_HWBLT_EXPDNBLK
			case FC_HWBLT_EXPDNBLK: {
				ss_blt_expdnblk_pb *func_pb = mfm_pb->param_blk;
				result = GDV_HWBLT_EXPDNBLK(func_pb->bc, func_pb->dstx,
					func_pb->srcx, func_pb->height);
				break;
			}
                  #endif
                  #ifdef GDV_HWBLT_DRAWBLK
			case FC_HWBLT_DRAWBLK: {
				ss_blt_drawblk_pb *func_pb = mfm_pb->param_blk;
				result = GDV_HWBLT_DRAWBLK(func_pb->bc, func_pb->dstx,
					func_pb->dsty, func_pb->width, func_pb->height);
				break;
			}
                  #endif
                  #ifdef GDV_HWBLT_DRAWHLINE
			case FC_HWBLT_DRAWHLINE: {
				ss_blt_drawhline_pb *func_pb = mfm_pb->param_blk;
				result = GDV_HWBLT_DRAWHLINE(func_pb->bc, func_pb->dstx,
					func_pb->dsty, func_pb->width);
				break;
			}
                  #endif
                  #ifdef GDV_HWBLT_DRAWVLINE
			case FC_HWBLT_DRAWVLINE: {
				ss_blt_drawvline_pb *func_pb = mfm_pb->param_blk;
				result = GDV_HWBLT_DRAWVLINE(func_pb->bc, func_pb->dstx,
					func_pb->dsty, func_pb->height);
				break;
			}
                  #endif
                  #ifdef GDV_HWBLT_DRAWPIXEL
			case FC_HWBLT_DRAWPIXEL: {
				ss_blt_drawpixel_pb *func_pb = mfm_pb->param_blk;
				result = GDV_HWBLT_DRAWPIXEL(func_pb->bc, func_pb->dstx,
					func_pb->dsty);
				break;
			}
                  #endif
			#endif  /**GDV_INCLUDE_HWBLT**/
			#endif  /**GDV_INCLIDE_IOBLT||GDV_INCLUDE_HWBLT**/
			#endif
			#ifdef GDV_SET_ATTRIBUTE
			case FC_GFX_ATTRIBUTE: {
				ss_gfx_attribute_pb *func_pb = mfm_pb->param_blk;
				result = GDV_SET_ATTRIBUTE(func_pb->gfxdev, func_pb->attr_type, 
                                           func_pb->mode, func_pb->value);
				break;
			}
			#endif
			default :				/* Unknown MAUI setstat */
#ifdef GDV_USER_SETSTAT
                result = GDV_USER_SETSTAT(dev_entry, ss_pb);
#else
                result = EOS_UNKSVC;
#endif
				break;
		}
		break;
	
	default :						/* Unknown setstat */
#ifdef GDV_USER_SETSTAT
        result = GDV_USER_SETSTAT(dev_entry, ss_pb);
#else
        result = EOS_UNKSVC;
#endif
		break;
	}
	
	return result;
}

/*******************************************************************************
 * NAME:        dr_read_hw()
 *
 * USAGE:   Get read entry point.
 */
error_code dr_read_hw(Dev_list dev_entry, i_read_pb *rpb)
{
  error_code result = SUCCESS;
  
#ifdef GDV_USER_READ
  result = GDV_USER_READ(dev_entry, rpb);
#else
  result = EOS_UNKSVC;
#endif

  return result;
}


/*******************************************************************************
 * NAME:        dr_write()
 *
 * USAGE:   Get write entry point.
 */
error_code dr_write_hw(Dev_list dev_entry, i_write_pb *wpb)
{
  error_code result = SUCCESS;
  
#ifdef GDV_USER_WRITE
	result = GDV_USER_WRITE(dev_entry, wpb);
#else
      result = EOS_UNKSVC;
#endif

  return result;
}
