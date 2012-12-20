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

#if defined(PWR_AWARE)
error_code PowerUpHardware(Dev_list dev_entry);
error_code PowerDnHardware(Dev_list dev_entry);
#endif

/*******************************************************************************
* NAME:		dr_init_hw()
*
* USAGE:	Initialize the hardware. The display should be enabled, and it
*			should be black until there is something to display.
*/
error_code dr_init_hw(Dev_list dev_entry)
{
#if defined(PWR_AWARE)
	char                    gfx[PWR_IDLEN] = "gfx"; /* name of the device to register with pwrman */
	error_code              err;     /* error code temp variable */
	Pwr_devcond             devpwrdef=NULL; /* Device Energy Condition Definition, NULL for this Example */
#endif

	/*	error_code ec;*/
	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);

	/* verify and initialize gdv_res_info width and height fields */
	if (lustat->default_res.disp_width == 0 ||
		lustat->default_res.disp_height == 0)
		return (EOS_MAUI_BADVALUE);

	gdv_res_info[0].disp_width = lustat->default_res.disp_width;
	gdv_res_info[0].disp_height = lustat->default_res.disp_height;

#if defined (PWR_AWARE)
	err = _os_pwr_reg(gfx,dr_pwrman,dev_entry,devpwrdef);
	if (err == SUCCESS){
		/* PwrMan in system and successful -- continue */
	} else if (err == EOS_UNKSVC) {
		/* no PwrMan in system -- continue */
	} else {
		/* PwrMan in system and unsuccessful -- return */
		return(err);
	}
#endif /* PWR_AWARE */

	
	return SUCCESS;
}

/*******************************************************************************
* NAME:		dr_term_hw()
*
* USAGE:	Initialize use of the hardware.
*/
error_code dr_term_hw(Dev_list dev_entry)
{
#if defined (PWR_AWARE)
	char			gfx[PWR_IDLEN] = "gfx"; /* name of the device to register */
	Pwr_devcond		devpwrdef=NULL; /* Device Energy Condition Definition, NULL for this Example */
#endif /* PWR_AWARE */

	error_code /*ec,*/ result = SUCCESS;
	/*	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);*/

#if defined (PWR_AWARE)
	/* tell PwrMan to forget our previous registration */
	(void)_os_pwr_unreg(gfx,dr_pwrman,dev_entry,devpwrdef);
#endif /* PWR_AWARE */

	return result;
}

/*******************************************************************************
* NAME:     dr_show_topdev()
*
* USAGE:    Show the top logical device in the stack of logical devices open
*           on this physical device. If no logical devices are open, then
*           setup the hardware to show black.
*/
void dr_show_topdev(Mfm_lu_stat lustat)
{
	return;
}

#if defined (PWR_AWARE)
error_code dr_pwrman(void *dev_entry, pwr_level pwrlvl, void *devpb)
{
	switch (pwrlvl){
		
		/* driver to power up */
		case MC68328_PWR_LCD_ON:
		{
			return(PowerUpHardware((Dev_list)dev_entry));
		}
		
		/* driver to power down */
		case MC68328_PWR_LCD_OFF:
		{
			return(PowerDnHardware((Dev_list)dev_entry));
		}
	}
	return(EOS_UNKSVC);
}

error_code PowerUpHardware(Dev_list dev_entry)
{
	Mfm_dr_stat drstat = (Mfm_dr_stat)(dev_entry->v_dr_stat);
	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);
	M68328_regs regs = (M68328_regs)(lustat->lu_port);
                        
	/* turn on LCD Controller module */
	HI(regs->lcdc_regs.lckcon,PORTBIT); /* clocking control */
	
	return(SUCCESS);
}

error_code PowerDnHardware(Dev_list dev_entry)
{
	Mfm_dr_stat drstat = (Mfm_dr_stat)(dev_entry->v_dr_stat);
	Mfm_lu_stat lustat = (Mfm_lu_stat)(dev_entry->v_lu_stat);
	M68328_regs regs = (M68328_regs)(lustat->lu_port);

	/* turn off LCD Controller module */
	LO(regs->lcdc_regs.lckcon,PORTBIT); /* clocking control */
	
	return(SUCCESS);
}
#endif
