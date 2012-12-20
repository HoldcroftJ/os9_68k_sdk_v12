
/*******************************************************************************
 * FILENAME : term.c
 *
 * DESCRIPTION :
 *
 * 	This file contains functions for terminating the MAUI Process Protocol Module
 *
 * COPYRIGHT:
 *
 *	This source code is the proprietary confidential property of Microware
 *	Systems Corporation, and is provided to licensee solely for documentation
 *	and educational purposes. Reproduction, publication, or distribution in
 *	form to any party other than the licensee is strictly prohibited.
 *
 * EDITION  DATE      WHO  DETAIL
 *       1  01/22/95  srm  create
 */

#define _TERM_C

#include "_key.h"
#include <string.h>
#include <sg_codes.h>
#include <types.h>

/*******************************************************************************
 * mppm_term()
 *******************************************************************************/
error_code mppm_term(MP_MPPM *mppm)
{
	return SUCCESS;
}

/*******************************************************************************
 * mppm_detach()
 *******************************************************************************/
error_code mppm_detach(MP_DEV *mp_dev)
{
	error_code ec = SUCCESS;
	PMEM *pmem = mp_dev->mppm->pmod_mem;	
	u_int32 size = sizeof(scf_path_opts);
#ifdef _OS9000
	u_int32 l_size = sizeof(scf_lu_opts);
#endif

	/* Restoring old path and logical unit options */
	ec = _os_ss_popt(mp_dev->mppm->dev_path, size,
		(Scf_path_opts)&pmem->opts_old);
	if (ec != SUCCESS)
	{
		return ec;
	}
#ifdef _OS9000
	ec = _os_ss_luopt(mp_dev->mppm->dev_path,l_size,
		(Scf_lu_opts)&pmem->l_opts_old);
#endif

	return ec;
}

