
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
 *       1  9/01/95   srm  modified from GENREM
 */

#define _TERM_C

#include "_key.h"
#include <string.h>


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
  wchar_t i;
  PMEM *pmem = (PMEM*)mp_dev->mppm->pmod_mem;

  /* the only task it to release any reserved keys */
    for (i = 0;  i < MAX_RESERVES;  i++)
    {
        if(pmem->reserve[i].mp_dev== mp_dev)
        {
            pmem->reserve[i].keysym=0;
            pmem->reserve[i].mp_dev=NULL;
        }
    }

  return SUCCESS;
}

