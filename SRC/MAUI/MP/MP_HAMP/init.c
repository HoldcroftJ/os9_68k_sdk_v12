
/*******************************************************************************
 * FILENAME : init.c
 *
 * DESCRIPTION :
 *
 * 	This file contains functions for initing the MAUI Process Protocol Module
 *
 * COPYRIGHT:
 *
 *	This source code is the proprietary confidential property of Microware
 *	Systems Corporation, and is provided to licensee solely for documentation
 *	and educational purposes. Reproduction, publication, or distribution in
 *	form to any party other than the licensee is strictly prohibited.
 *
 * EDITION  DATE      WHO  DETAIL
 *       1  11/09/99  gjb,sbh  ported mp_msptr to Hampshire touchscreen
 */

#define _INIT_C

#include "_key.h"
#include <string.h>

#include <sg_codes.h>
#include <types.h>
#include <modes.h>
#include <process.h>


/*******************************************************************************
 * mppm_initsize()
 *******************************************************************************/
error_code mppm_initsize(MP_MPPM *mppm, size_t *mem_size)
{
	/* save the MAUI Protocol Module's compat_level */
	mppm->pmod_compat_level = MAUI_COMPAT_LEVEL;

	*mem_size = sizeof (PMEM);
  
	return SUCCESS;
}


/*******************************************************************************
 * mppm_init()
 *******************************************************************************/
error_code mppm_init(MP_MPPM *mppm, void *mem_buf, size_t mem_size)
{
	PMEM *pmem = mppm->pmod_mem;
	INP_DEV_STATUS *status = &pmem->status;
  
	memset(pmem, 0, sizeof(PMEM));

	status->sim_meth   = DEFAULT_SIM_METH;
	status->ptr_min.x  = DEFAULT_PTR_MIN_X;
	status->ptr_min.y  = DEFAULT_PTR_MIN_Y;
	status->ptr_max.x  = DEFAULT_PTR_MAX_X;
	status->ptr_max.y  = DEFAULT_PTR_MAX_Y;
	status->speed.x    = DEFAULT_SPEED_X;
	status->speed.y    = DEFAULT_SPEED_Y;
	status->write_mask = MSG_TYPE_ANY; /* this is the MAUI default */

    /* set driver default calibration values */
	pmem->cal_x = DEFAULT_CALIBRATION_X;
	pmem->cal_y = DEFAULT_CALIBRATION_Y;
	pmem->cal_width = DEFAULT_CALIBRATION_WIDTH;
	pmem->cal_height = DEFAULT_CALIBRATION_HEIGHT;
	pmem->screen_width = SCREEN_WIDTH;
	pmem->screen_height = SCREEN_HEIGHT;

	/* get calibration values from data module */
	update_calibration(pmem);

	return SUCCESS;
}


/*******************************************************************************
 * mppm_attach()
 *******************************************************************************/
error_code mppm_attach(MP_DEV *mp_dev)
{
	error_code ec = SUCCESS;

	return ec;
}









