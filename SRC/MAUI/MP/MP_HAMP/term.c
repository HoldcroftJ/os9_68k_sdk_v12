
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
 *       1  11/09/99  gjb,sbh  ported mp_msptr to Hampshire touchscreen
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

	return ec;
}

