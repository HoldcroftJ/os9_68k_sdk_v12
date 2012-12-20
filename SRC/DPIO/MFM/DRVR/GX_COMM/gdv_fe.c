/*******************************************************************************
* DESCRIPTION :
*
*	This file contains code that allows the fast entry points to execute in
*	system state. This is done by calling a real get/setstat function, which
*	slows it down, but is only done for drivers that must execute this code
*	in system state.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  01/31/96  TJE  First version
*       2  10/20/98  AND  HW BLT fast-entry points
*/
#include <gdv_priv.h>

#ifdef _OSK
extern error_code _os_getstat(path_id path, u_int32 code, void *pb);
extern error_code _os_setstat(path_id path, u_int32 code, void *pb);
#endif

/*******************************************************************************
* WARNING - DO NOT PUT ANY GLOBALS IN THIS CODE. IT GETS EXECUTED AS PART OF THE
* APPLICATION. THEREFORE, IT USES THE PROCESSES (NOT THE DRIVERS) DATA AREA.
*******************************************************************************/


/*******************************************************************************
* BINDING FUNCTIONS FOR GFX FAST ENTRY POINTS
*******************************************************************************/

error_code _os_ss_gfx_updatedpy(GFX_DEV *gfxdev, BOOLEAN sync)
{
	path_id path = gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_gfx_updatedpy_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_GFX_UPDATEDPY;
	pb.gfxdev = gfxdev;
	pb.sync = sync;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_gfx_vppos(GFX_VPORT *vport, GFX_POS x, GFX_POS y)
{
	path_id path = vport->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_gfx_vppos_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_GFX_VPPOS;
	pb.vport = vport;
	pb.x = x;
	pb.y = y;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_gfx_vpsize(GFX_VPORT *vport, GFX_DIMEN width, GFX_DIMEN height)
{
	path_id path = vport->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_gfx_vpsize_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_GFX_VPSIZE;
	pb.vport = vport;
	pb.width = width;
	pb.height = height;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_gfx_vpstate(GFX_VPORT *vport, BOOLEAN active)
{
	path_id path = vport->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_gfx_vpstate_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_GFX_VPSTATE;
	pb.vport = vport;
	pb.active = active;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_gfx_vpcolors(GFX_VPORT *vport, u_int16 start_entry, u_int16
	num_colors, GFX_COLOR_TYPE color_type, void *colors)
{
	path_id path = vport->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_gfx_vpcolors_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_GFX_VPCOLORS;
	pb.vport = vport;
	pb.start_entry = start_entry;
	pb.num_colors = num_colors;
	pb.color_type = color_type;
	pb.colors = colors;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
	
error_code _os_ss_gfx_vpdmap(GFX_VPORT *vport, const GFX_DMAP *dmap, GFX_POS x,
	GFX_POS y)
{
	path_id path = vport->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_gfx_vpdmap_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_GFX_VPDMAP;
	pb.vport = vport;
	pb.dmap = dmap;
	pb.x = x;
	pb.y = y;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
	
error_code _os_ss_gfx_vpdmpos(GFX_VPORT *vport, GFX_POS x, GFX_POS y)
{
	path_id path = vport->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_gfx_vpdmpos_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_GFX_VPDMPOS;
	pb.vport = vport;
	pb.x = x;
	pb.y = y;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_gfx_restackvp(GFX_VPORT *vport, GFX_VPORT_PLACEMENT placement,
	GFX_VPORT *ref_vport)
{
	path_id path = vport->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_gfx_restackvp_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_GFX_RESTACKVP;
	pb.vport = vport;
	pb.placement = placement;
	pb.ref_vport = ref_vport;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
	

#ifdef GDV_SET_VPINTEN
error_code _os_ss_gfx_vpinten(GFX_VPORT *vport, u_int8 intensity)
{
	path_id path = vport->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_gfx_vpinten_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_GFX_VPINTEN;
	pb.vport = vport;
	pb.intensity = intensity;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

#endif

#ifdef GDV_HW_CURSOR
#ifdef GDV_FE_SYSTATE
/*******************************************************************************
* BINDING FUNCTIONS FOR CURSOR FAST ENTRY POINTS
*******************************************************************************/

error_code _os_ss_gfx_set_cursor(GFX_DEV *gfxdev, GFX_CURSOR *cursor)
{
	path_id path = gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_gfx_setcursor_pb pb;

	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_GFX_SETCURSOR;
	pb.gfxdev = gfxdev;
	pb.cursor = cursor;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_gfx_set_cursor_pos(GFX_DEV *gfxdev, GFX_POS x, GFX_POS y)
{
	path_id path = gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_gfx_setcursorpos_pb pb;

	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_GFX_SETCURSORPOS;
	pb.gfxdev = gfxdev;
	pb.x = x;
	pb.y = y;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
#endif /* GDV_SYSTATE */
#endif /* GDV_HW_CURSOR */

/*******************************************************************************
* BINDING FUNCTIONS FOR BOTH IOBLT AND HWBLT FAST ENTRY POINTS
*******************************************************************************/
#if defined(GDV_INCLUDE_IOBLT) || defined(GDV_INCLUDE_HWBLT)
error_code _os_ss_blt_cpymix(BLT_CONTEXT *bc)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_setcpymix_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_BLT_SETCPYMIX;
	pb.bc = bc;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_blt_expmix(BLT_CONTEXT *bc)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_setexpmix_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_BLT_SETEXPMIX;
	pb.bc = bc;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_blt_drwmix(BLT_CONTEXT *bc)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_setdrwmix_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_BLT_SETDRWMIX;
	pb.bc = bc;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_blt_pix(BLT_CONTEXT *bc)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_setpix_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_BLT_SETPIX;
	pb.bc = bc;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_blt_src(BLT_CONTEXT *bc)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_setsrc_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_BLT_SETSRC;
	pb.bc = bc;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_blt_exptbl(BLT_CONTEXT *bc)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_setexptbl_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_BLT_SETEXPTBL;
	pb.bc = bc;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_blt_trans(BLT_CONTEXT *bc)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_settrans_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_BLT_SETTRANS;
	pb.bc = bc;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_blt_mask(BLT_CONTEXT *bc)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_setmask_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_BLT_SETMASK;
	pb.bc = bc;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_blt_ofs(BLT_CONTEXT *bc)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_setofs_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_BLT_SETOFS;
	pb.bc = bc;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_blt_dst(BLT_CONTEXT *bc)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_setdst_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_BLT_SETDST;
	pb.bc = bc;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

/*******************************************************************************
* BINDING FUNCTIONS FOR IOBLT FAST ENTRY POINTS ONLY
*******************************************************************************/
#ifdef GDV_INCLUDE_IOBLT

error_code _os_ss_ioblt_copyblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_copyblk_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_IOBLT_COPYBLK;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.dsty = dsty;
	pb.srcx = srcx;
	pb.srcy = srcy;
	pb.width = width;
	pb.height = height;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
	
error_code _os_ss_ioblt_copynblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx,
	GFX_DIMEN height)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_copynblk_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_IOBLT_COPYNBLK;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.srcx = srcx;
	pb.height = height;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
	
error_code _os_ss_ioblt_expdblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_expdblk_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_IOBLT_EXPDBLK;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.dsty = dsty;
	pb.srcx = srcx;
	pb.srcy = srcy;
	pb.width = width;
	pb.height = height;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
	
error_code _os_ss_ioblt_expdnblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx,
	GFX_DIMEN height)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_expdnblk_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_IOBLT_EXPDNBLK;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.srcx = srcx;
	pb.height = height;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
	

error_code _os_ss_ioblt_drawblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN width, GFX_DIMEN height)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_drawblk_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_IOBLT_DRAWBLK;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.dsty = dsty;
	pb.width = width;
	pb.height = height;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_ss_ioblt_drawhline(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN width)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_drawhline_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_IOBLT_DRAWHLINE;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.dsty = dsty;
	pb.width = width;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
	
error_code _os_ss_ioblt_drawvline(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN height)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_drawvline_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_IOBLT_DRAWVLINE;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.dsty = dsty;
	pb.height = height;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
	
error_code _os_ss_ioblt_drawpixel(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_drawpixel_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_IOBLT_DRAWPIXEL;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.dsty = dsty;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}

error_code _os_gs_ioblt_getpixel(GFX_PIXEL *ret_pixel, BLT_CONTEXT *bc, GFX_POS
	srcx, GFX_POS srcy)
{
	path_id path = bc->gfxdev->usr_path;
	i_getstat_pb fm_gs_pb;
	gs_blt_getpixel_pb pb;
	
	fm_gs_pb.path = path;
	fm_gs_pb.gs_code = SS_MAUI;
	fm_gs_pb.param_blk = &pb;

	pb.func_code = FC_IOBLT_GETPIXEL;
	pb.ret_pixel = ret_pixel;
	pb.bc = bc;
	pb.srcx = srcx;
	pb.srcy = srcy;

	return _os_getstat(path, SS_MAUI, &fm_gs_pb);
}
#endif  /**GDV_INCLUDE_IOBLT**/

/*******************************************************************************
* BINDING FUNCTIONS FOR HWBLT FAST ENTRY POINTS
*******************************************************************************/
#ifdef GDV_INCLUDE_HWBLT

#ifdef GDV_HWBLT_COPYBLK
error_code _os_ss_hwblt_copyblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_copyblk_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_HWBLT_COPYBLK;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.dsty = dsty;
	pb.srcx = srcx;
	pb.srcy = srcy;
	pb.width = width;
	pb.height = height;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
#endif

#ifdef GDV_HWBLT_COPYNBLK	
error_code _os_ss_hwblt_copynblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx,
	GFX_DIMEN height)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_copynblk_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_HWBLT_COPYNBLK;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.srcx = srcx;
	pb.height = height;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
#endif
	
#ifdef GDV_HWBLT_EXPDBLK
error_code _os_ss_hwblt_expdblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_POS srcx, GFX_POS srcy, GFX_DIMEN width, GFX_DIMEN height)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_expdblk_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_HWBLT_EXPDBLK;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.dsty = dsty;
	pb.srcx = srcx;
	pb.srcy = srcy;
	pb.width = width;
	pb.height = height;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
#endif
	
#ifdef GDV_HWBLT_EXPDNBLK
error_code _os_ss_hwblt_expdnblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS srcx,
	GFX_DIMEN height)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_expdnblk_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_HWBLT_EXPDNBLK;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.srcx = srcx;
	pb.height = height;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
#endif
	
#ifdef GDV_HWBLT_DRAWBLK
error_code _os_ss_hwblt_drawblk(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN width, GFX_DIMEN height)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_drawblk_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_HWBLT_DRAWBLK;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.dsty = dsty;
	pb.width = width;
	pb.height = height;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
#endif

#ifdef GDV_HWBLT_DRAWHLINE
error_code _os_ss_hwblt_drawhline(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN width)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_drawhline_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_HWBLT_DRAWHLINE;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.dsty = dsty;
	pb.width = width;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
#endif
	
#ifdef GDV_HWBLT_DRAWVLINE
error_code _os_ss_hwblt_drawvline(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty,
	GFX_DIMEN height)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_drawvline_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;

	pb.func_code = FC_HWBLT_DRAWVLINE;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.dsty = dsty;
	pb.height = height;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
#endif
	
#ifdef GDV_HWBLT_DRAWPIXEL
error_code _os_ss_hwblt_drawpixel(BLT_CONTEXT *bc, GFX_POS dstx, GFX_POS dsty)
{
	path_id path = bc->gfxdev->usr_path;
	i_setstat_pb fm_ss_pb;
	ss_blt_drawpixel_pb pb;
	
	fm_ss_pb.path = path;
	fm_ss_pb.ss_code = SS_MAUI;
	fm_ss_pb.param_blk = &pb;


	pb.func_code = FC_HWBLT_DRAWPIXEL;
	pb.bc = bc;
	pb.dstx = dstx;
	pb.dsty = dsty;

	return _os_setstat(path, SS_MAUI, &fm_ss_pb);
}
#endif

#ifdef GDV_HWBLT_GETPIXEL
error_code _os_gs_hwblt_getpixel(GFX_PIXEL *ret_pixel, BLT_CONTEXT *bc, GFX_POS
	srcx, GFX_POS srcy)
{
	path_id path = bc->gfxdev->usr_path;
	i_getstat_pb fm_gs_pb;
	gs_blt_getpixel_pb pb;
	
	fm_gs_pb.path = path;
	fm_gs_pb.gs_code = SS_MAUI;
	fm_gs_pb.param_blk = &pb;

	pb.func_code = FC_HWBLT_GETPIXEL;
	pb.ret_pixel = ret_pixel;
	pb.bc = bc;
	pb.srcx = srcx;
	pb.srcy = srcy;

	return _os_getstat(path, SS_MAUI, &fm_gs_pb);
}
#endif

#endif  /**GDV_INCLUDE_HWBLT**/

#endif  /**GDV_INCLUDE_IOBLT||GDV_INCLUDE_HWBLT**/
