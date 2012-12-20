/****************************************************************************
 *	File:	%A%
 *	ID:		%G%
 ****************************************************************************
 * Copyright (c) 1997 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of Microware 
 * Systems Corporation, and is provided to licensee solely for documentation 
 * and educational purposes. Reproduction, publication, or distribution in
 * any form to any party other than the licensee is strictly prohibited.
 *
 ****************************************************************************
 *
 * ed#   date   changes                                             by
 * --- -------- -------------------------------------------------   ---
 *  1  02/19/97 Created	                                            jmh
 *     06/26/97 <----------------- SPF V2.2 -------------------->   taka
 *  2  07/20/98 <----------------- SPF v2.2.2 ------------------>   taka
 ****************************************************************************/
#ifndef _PPSTAT_H
#define _PPSTAT_H

#include <io.h>

typedef struct spf_ppstat	spf_ppstat;	/* SPF Per-path static		*/
typedef struct spf_ppstat*	Spf_ppstat;

/* Driver allocated per path static Storage. */
struct spf_ppstat {
	Spf_ppstat	pp_next;		/* Next pointer in the list		*/
	void		*pp_pd;			/* Path descriptor pointer		*/
	Dev_list	pp_updrvr,		/* Dev_list of driver above		*/
			pp_dndrvr,		/* Dev_list of driver below		*/
			pp_deventry;		/* This driver's Dev_list for this path	*/

	#ifdef SPF_PPSTAT
		SPF_PPSTAT			/* Drvr specific per path stg variables	*/
	#endif
};

/********************************************/
/* Function Prototypes are defined in spf.h */
/********************************************/

#endif	/* _PPSTAT_H */
