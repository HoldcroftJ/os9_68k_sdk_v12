/*****************************************************************************
** ID:     @(#) init.c 1.1@(#)
** Date:   1/19/96
******************************************************************************
 * 	This file contains functions for initing the MAUI Process Protocol Module
******************************************************************************
** Copyright 1996 by Microware Systems Corporation                          **
** Reproduced Under License                                                 **
**                                                                          **
** This source code is the proprietary confidential property of             **
** Microware Systems Corporation, and is provided to licensee               **
** solely for documentation and educational purposes. Reproduction,         **
** publication, or distribution in any form to any party other than         **
** the licensee is strictly prohibited.                                     **
******************************************************************************
** Edition History:                                                         **
** #   Date     Comments                                          By        **
** --- -------- ------------------------------------------------- --------- **
**	 1 01/19/96 creation--copied from Wirlwind and modeled        rws       **
**              modeled PHILMOUS                                            **
*****************************************************************************/

#define _INIT_C

#include "_key.h"
#include <string.h>



/*******************************************************************************
 * Function that returns static memory requirements
 *		Call with:
 *			Pointer to device and protocol module data
 *			Pointer to place size of this module's data requirements
 *		Return:
 *			Error code (will always be SUCCESS)
 *
 *	Note: the MAUI input process uses this size to allocate this module's
 *	memory, and a pointer to our static storage is then passed to other
 *	functions in this module via the pmod_mem pointer in the MP_MPPM structure
 *	see mppm_init() for example)
 *******************************************************************************/
error_code mppm_initsize
(
	MP_MPPM *mppm, 
	size_t *mem_size
)
{
	/* save the MAUI Protocol Module's compat_level */
	mppm->pmod_compat_level = MAUI_COMPAT_LEVEL;
	
	*mem_size = sizeof (PMEM);

	return (SUCCESS);
}



/*******************************************************************************
 * Function that initializes Protocol Module's static memory
 *		Call with:
 *			Pointer to device and protocol module memory
 *			Pointer to memory buffer (not used)
 *			size of memory (not used)
 *		Return:
 *			Error code (will always be SUCCESS)
 *******************************************************************************/
error_code mppm_init
(
	MP_MPPM *mppm, 
	void *mem_buf, 
	size_t mem_size
)
{
	PMEM *pmem = mppm->pmod_mem;	/* pointer to this module's statics */
	INP_DEV_STATUS *status = &pmem->status;

	/* clear this module's static memory in total */
	memset (pmem, 0, sizeof (PMEM));

	status->sim_meth   = DEFAULT_SIM_METH;
	status->ptr_min.x  = DEFAULT_PTR_MIN_X;
	status->ptr_min.y  = DEFAULT_PTR_MIN_Y;
	status->ptr_max.x  = DEFAULT_PTR_MAX_X;
	status->ptr_max.y  = DEFAULT_PTR_MAX_Y;
	status->speed.x    = DEFAULT_SPEED_X;
	status->speed.y    = DEFAULT_SPEED_Y;
	status->write_mask = MSG_TYPE_ANY; /* this is the MAUI default */

	return (SUCCESS);
}



/*******************************************************************************
 * mppm_attach()
 *******************************************************************************/
error_code mppm_attach(MP_DEV *inp_dev)
{
  return SUCCESS;
}


