/*******************************************************************************
*
* DESCRIPTION :
*
*	Update the display with queued up changes. Synchronize with vertical retrace
*	if specified.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  12/23/94  TJE  First version
*/
#include <defs.h>
#include <events.h>

error_code fe_update_dpy(GFX_DEV *gfxdev, BOOLEAN sync)
{
    GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
    Mfm_lu_stat lustat = dvatch->lustat;

    /* If this is the top path, i.e. the active one, do the calls   **
    ** which touch the hardware otherwise don't touch the hardware  */
/*    if ((dvatch != NULL) && (dvatch->virtual_existance == FALSE)) {*/
    if ((dvatch != NULL)) {

        M68328_regs regs = (M68328_regs)(lustat->lu_port);
        GFX_VPORT *vp;

        /* no vertical retrace event to check on this device */

        /* turn off current display */
        regs->lcdc_regs.lckcon &= ~CKCON_LCDCON;

        /* find an active viewport (there will be at most one) */
        for (vp = gfxdev->shared->vport_head; vp != NULL; vp = vp->shared->next) {
            if (vp->shared->active == TRUE) {
                break;
            }
        }

        /* for no active viewport or drawmap: turn off display and return */
        if ((vp == NULL) || (vp->shared->dmap == NULL)) {
            return(SUCCESS);
        }

        /** at this point vp points to an active viewport **/

        /* turn on new display based on current drawmap */
        if (vp->shared->coding_method == GFX_CM_1BIT) {

            /* 1-bit encoding */

            /* screen boundry check */
            /**** CODE HERE ***/

            /* update screen drawmap address */
            regs->lcdc_regs.lssa = (u_char *)(
                    (u_int32)(vp->shared->pixmem) +
                    (u_int32)(vp->shared->dmap_line_size * vp->shared->dmapy) +
                    (u_int32)(vp->shared->dmapx / 8) );

            /* update drawmap virtual width */
            regs->lcdc_regs.lvpw = vp->shared->dmap_line_size / 2;

            /* update drawmap width */
            regs->lcdc_regs.lxmax = vp->shared->width - 1;

            /* update drawmap height */
            regs->lcdc_regs.lymax = vp->shared->height - 1;

            /* update drawmap panning offset */
            regs->lcdc_regs.lposr = LPOSR_BOS_FIRST;

            /* set non gray-scale mode */
			regs->lcdc_regs.lpicf = PICF_PBSIZ4;
            regs->lcdc_regs.lpicf &= ~PICF_GSON;

            /* update drawmap last-buffer-address */
            regs->lcdc_regs.llbar = regs->lcdc_regs.lxmax/16 - 1;

            /* update drawmap octet terminal count */
            regs->lcdc_regs.lotcr = regs->lcdc_regs.llbar + 4;

			/* configure polarity configuration register for pixel polarity */
			regs->lcdc_regs.lpolcf = 0;
			regs->lcdc_regs.lpolcf +=
			 	(lustat->pixel_polarity == HIGH ?
				 POLCF_PIXHI : POLCF_PIXLO);

			/* update pixel clock divider register */
			regs->lcdc_regs.lpxcd = 1;
		
            /* turn on hardware */
            regs->lcdc_regs.lckcon = CKCON_LCDCON | CKCON_DMA16 | CKCON_WS4;

            return(SUCCESS);

        } else if (vp->shared->coding_method == GFX_CM_2BIT) {

            /* 2-bit encoding */

            /* screen boundry check */
            /**** CODE HERE ***/

            /* update screen drawmap address */
            regs->lcdc_regs.lssa = (u_char *)(
                    (u_int32)(vp->shared->pixmem) +
                    (u_int32)(vp->shared->dmap_line_size * vp->shared->dmapy) +
                    (u_int32)(vp->shared->dmapx / 4) );

            /* update drawmap virtual width */
            regs->lcdc_regs.lvpw = vp->shared->dmap_line_size / 2;

            /* update drawmap width */
            regs->lcdc_regs.lxmax = vp->shared->width - 1;

            /* update drawmap height */
            regs->lcdc_regs.lymax = vp->shared->height - 1;

            /* update drawmap panning offset */
            regs->lcdc_regs.lposr = LPOSR_BOS_FIRST;

            /* set gray-scale mode */
			regs->lcdc_regs.lpicf = PICF_PBSIZ4;
            regs->lcdc_regs.lpicf |= PICF_GSON;

            /* update drawmap last-buffer-address */
            regs->lcdc_regs.llbar = regs->lcdc_regs.lxmax/8;

            /* update drawmap octet terminal count */
            regs->lcdc_regs.lotcr = regs->lcdc_regs.llbar + 8;

			/* update pixel clock divider register */
			regs->lcdc_regs.lpxcd = 1;
		
            /* turn on hardware */
            regs->lcdc_regs.lckcon = CKCON_LCDCON | CKCON_DMA16 | CKCON_WS2;
            
			/* configure polarity configuration register for pixel polarity */
			regs->lcdc_regs.lpolcf = 0;
			regs->lcdc_regs.lpolcf +=
			 	(lustat->pixel_polarity == HIGH ?
				 POLCF_PIXHI : POLCF_PIXLO);

            return(SUCCESS);

        } else {

            /* unsupported encoding method */
            return (EOS_MAUI_NOHWSUPPORT);
        }
    }

    /* at this point either there is no gfxdev, the gfxdev's attachment
    ** has not been made (so we can't tell if it's virtual or real),
    ** or the gfxdev is virtual (so it does not have possession of the
    ** display)
    */
    return (SUCCESS);
}
