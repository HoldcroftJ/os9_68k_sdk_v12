/****************************************************************************
 *  ID:     %A%
 *  Date:   %G%
 ****************************************************************************
 *  SPF Device Driver per path storage function calls                       *
 ****************************************************************************
 *  Copyright 1997 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 *                                                                          *
 * Edition History:                                                         *
 *                                                                          *
 * #   Date     Comments                                                By  *
 * --- --------	-----------------------------------------------------  ---  *
 *  1  03/19/97 Creation                                               jmh  *
 *     04/17/97 <------------ SPF v2.2 ----------------------------->  taka *
 *  2  07/14/98 Removed compiler warnings                              taka *
 *     07/19/98 <------------ SPF v2.2.2 --------------------------->  taka *
 ****************************************************************************/

/*
 *	Header Files
 */
#define SPF_DRVR
#include <SPF/spf.h>
#include <SPF/ppstat.h>
#include <memory.h>
#include <string.h>


/*
 * Add a per-path storage entry to the path list on open.
 */
error_code pps_add_entry(Dev_list deventry, Spf_ppstat *ret_pps)
{
	Spf_lustat	lustat = (Spf_lustat)(deventry->v_lu_stat);
	Spf_ppstat	memptr;
	u_int32		size;
	error_code  err;


	/* Allocate memory for per-path entry... */
	size = lustat->lu_pps_size;
	if (err = _os_srqmem(&size, (void **) &memptr, MEM_ANY)) return(err);

	/* Copy over initialized per-path data (if defined)... */
	if (lustat->lu_pps_idata)
		memcpy(memptr, lustat->lu_pps_idata, lustat->lu_pps_size);

	/* Fill in per-path fields we know about... */
	memptr->pp_next     = lustat->lu_pps_list;
	memptr->pp_pd       = lustat->lu_pathdesc;
	memptr->pp_dndrvr   = lustat->lu_dndrvr;
	memptr->pp_updrvr   = lustat->lu_updrvr;
	memptr->pp_deventry = deventry;

	/* Add the entry to the linked list... */
	lustat->lu_pps_list = memptr;

	/* If the caller has given a return ppstat address, give the
	 * pointer to the per-path storage...
	 */
	if (ret_pps) *ret_pps = memptr;

	/* Mission accomplished! */
	return(SUCCESS);
}


/* Find an entry to the driver's per path list using a path descriptor
 * pointer as the search field.  Pointer to per-path storage is returned
 * if found, NULL returned if search fails.
 */
error_code pps_find_entry(Spf_lustat lustat, Spf_pdstat pd, Spf_ppstat *ret_pps)
{
	Spf_ppstat	ppstat;


	/* Search until end of list... */
	for (ppstat = lustat->lu_pps_list; ppstat; ppstat = ppstat->pp_next)
	{
		if (ppstat->pp_pd == pd) break;
	}

	/* Return ppstat pointer (will be NULL if not found)... */
	*ret_pps = ppstat;
	if (!ppstat) return(EOS_PPS_NOTFND);

	return(SUCCESS);
}


/* Find an entry to the driver's per path list using an offset to a field
 * within the per-path storage structure.  Size is the size of the field
 * to be compared, and data is a pointer to the value to be found.
 */
error_code pps_find_entry_by_offset(Spf_lustat  lustat,
									u_int16    offset,
									u_int16    size,
									void       *cmp_val,
									Spf_ppstat *ret_pps)
{
	Spf_ppstat	ppstat;
	u_char		*ptr;


	/* Search until end of list... */
	for (ppstat = lustat->lu_pps_list; ppstat; ppstat = ppstat->pp_next)
	{
		ptr = ((u_char *)ppstat) + offset;
		if (!memcmp(ptr, cmp_val, size)) break;
	}

	/* Return an error if path not found... */
	*ret_pps = ppstat;
	if (!ppstat) return(EOS_PPS_NOTFND);

	return(SUCCESS);
}


/* Delete an entry from the driver's per path list. */
error_code pps_del_entry(Spf_lustat lustat, Spf_pdstat pd)
{
	Spf_ppstat	ppstat, prev;


	for (ppstat = lustat->lu_pps_list, prev = NULL;
		 ppstat;
		 prev = ppstat, ppstat = ppstat->pp_next)
	{
		if (ppstat->pp_pd == pd)
		{
			/* Found a matching pathdesc, so remove it from the list... */
			if (prev) prev->pp_next = ppstat->pp_next;
			else lustat->lu_pps_list = ppstat->pp_next;

			/* Deallocate memory and return... */
			_os_srtmem(lustat->lu_pps_size, (void *) ppstat);
			return(SUCCESS);
		}
	}

	/* Path was not found, return path-not-found error... */
	return(EOS_PPS_NOTFND);
}


error_code pps_chg_updrvr(Spf_lustat lustat, Spf_pdstat pd, Dev_list new_updrvr)
{
	Spf_ppstat	ppstat;


	/* Search until end of list... */
	for (ppstat = lustat->lu_pps_list; ppstat; ppstat = ppstat->pp_next)
	{
		if (ppstat->pp_pd == pd) break;
	}

	/* Return an error if path was not found... */
	if (!ppstat) return(EOS_PPS_NOTFND);

	/* Set new updriver and return... */
	ppstat->pp_updrvr = new_updrvr;
	return(SUCCESS);
}

