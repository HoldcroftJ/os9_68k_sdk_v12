/*******************************************************************************
* DESCRIPTION :
*
*	Driver entry point table. Do not add any other definitions to this file.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  11/20/95  TJE  Converted from earlier work by BAT and CJV
 *         04/11/00  JPH  Added dr_read_hw and dr_write_hw                        
*/
#include <gdv_priv.h>

#ifndef GDV_USER_READ
#define GDV_USER_READ 0
#endif
#ifndef GDV_USER_WRITE
#define GDV_USER_WRITE 0
#endif
/*
* NAME:		dr_stat
*
* USAGE:	Driver static storage definition
*/
mfm_dr_stat dr_stat = {
	dr_iniz,					/* driver initialize entry point */
	dr_term,					/* driver terminate entry point */
	dr_getstat,					/* driver getstat entry point */
	dr_setstat,					/* driver setstat entry point */
	0,							/* attach count (dr_att_cnt) */
	GDV_USER_READ,			 	/* driver read entry point */
	GDV_USER_WRITE,				/* driver write entry point */
	{ 0 },						/* RESERVED FOR FUTURE USE. DO NOT USE. */

	/* Device Driver specific stuff here */
	#if defined(DR_SPECIFICS_INIT)
		DR_SPECIFICS_INIT
	#endif
};

/*******************************************************************************
* NAME:		_m_share or _m_usage
*
* USAGE:	Pointer to static storage. THIS MUST IMMEDIATELY FOLLOW THE DATA
*			dr_stat DATA STRUCTURE. NO OTHER DEFINITIONS OR CODE MAY BE PLACED
*			IN THIS FILE.
*/
#ifdef _OS9000
_asm("_m_share: equ dr_stat");
#else
_asm("_m_usage: equ dr_stat+0x8000");
#endif
