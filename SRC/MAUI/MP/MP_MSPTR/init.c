
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
 *       1  01/22/95  srm  create */

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

	return SUCCESS;
}


/*******************************************************************************
 * mppm_attach()
 *******************************************************************************/
error_code mppm_attach(MP_DEV *mp_dev)
{
	error_code ec = SUCCESS;
	scf_path_opts opts;
	u_int32 size;
#ifdef _OS9000
	scf_lu_opts l_opts;
	u_int32 l_size;
#endif
	int i;
	
	PMEM *pmem = mp_dev->mppm->pmod_mem;
	
	/* obtain SCF path options twice (once for original and once for copy) */
	size = sizeof(scf_path_opts);
	ec = _os_gs_popt(mp_dev->mppm->dev_path, &size, &opts);
	if (ec != SUCCESS)
	{
		return ec;
	}
	size = sizeof(scf_path_opts);
	ec = _os_gs_popt(mp_dev->mppm->dev_path, &size, &(pmem->opts_old));
	if (ec != SUCCESS)
	{
		return ec;
	}

#ifdef _OS9000
	/* obtain SCF logical unit options twice (once for original and once for copy) */
	l_size = sizeof(scf_lu_opts);
	ec = _os_gs_luopt(mp_dev->mppm->dev_path, &l_size, &l_opts);
	if (ec != SUCCESS)
	{
		return ec;
	}
	l_size = sizeof(scf_lu_opts);
	ec = _os_gs_luopt(mp_dev->mppm->dev_path, &l_size, &(pmem->l_opts_old));
	if (ec != SUCCESS)
	{
		return ec;
	}

	/* clear out path and logical unit options */
	for (i = 0; i < 32; i++)
	{
		opts.pd_inmap[i].type=PASSTHRU;
	}

	opts.pd_eofch=0;
	opts.pd_echo=0;
	opts.pd_eorch=0;
	opts.pd_tabch=0;
	opts.pd_bellch=0;
	opts.pd_bspch=0;
	opts.pd_case=0;
	opts.pd_backsp=0;
	opts.pd_delete=0;
	opts.pd_alf=0;
	opts.pd_pause=0;
	opts.pd_insm=0;
	opts.pd_nulls=0;
	opts.pd_page=0;
	opts.pd_tabsiz=0;
	opts.pd_time=0;

	l_opts.v_intr = 0;
	l_opts.v_quit = 0;
	l_opts.v_psch = 0;
	l_opts.v_xon  = 0;
	l_opts.v_xoff = 0;
	l_opts.v_parity = NOPARITY;
	l_opts.v_baud=BAUD1200;
	l_opts.v_stopbits=ONESTOP;
	l_opts.v_wordsize=WORDSIZE7;
#else
	/* clear out path options */
	opts.pd_upc = opts.pd_bso = opts.pd_dlo = opts.pd_eko = 0;
	opts.pd_alf = opts.pd_nul = opts.pd_pau = opts.pd_pag = 0;
	opts.pd_bsp = opts.pd_del = opts.pd_eor = opts.pd_eof = 0;
	opts.pd_rpr = opts.pd_dup = opts.pd_psc = opts.pd_int = 0;
	opts.pd_qut = opts.pd_bse = opts.pd_ovf = opts.pd_par = 0;
	opts.pd_bau = opts.pd_d2p = opts.pd_xon = opts.pd_xoff = 0;
	opts.pd_Tab = opts.pd_Tabs = 0;

	/* set 1200-7-N-1 */
	opts.pd_bau = BAUD1200;
	opts.pd_par = WORDSIZE7 | NOPARITY | ONESTOP;
#endif

	ec = _os_ss_popt(mp_dev->mppm->dev_path,size, &opts);
	if (ec != SUCCESS)
	{
		return ec;
	}

#ifdef _OS9000
	ec = _os_ss_luopt(mp_dev->mppm->dev_path, l_size, &l_opts);
	if (ec != SUCCESS)
	{
		_os_ss_popt(mp_dev->mppm->dev_path, size, (Scf_lu_opts)&pmem->opts_old);
	}
#endif
	
	return ec;
}









