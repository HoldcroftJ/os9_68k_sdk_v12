/*
 * bootteac.c: Boot routines for the TEAC SCSI floppy drive
 *
 * Copyright 1990, 1991, 1995, 1996 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 *	Edition History:														
 *	#   Date    Comments												By
 *	-- -------- ------------------------------------------------------- ---
 *   00 90/05/03 Created from model provided by rb5400.                 Rwb
 *   01 90/06/27 Needed to change entry point name to allow files to
 *               link correctly                                         Rwb
 *   02 92/08/07 fixed problems from new defs.							wwb
 *   03 93/05/18 Made use of static storage definitions in rbteac.h     ats
 *   04 95/07/11 ANSI Version.                                          GbG
 *   05 96/03/07 RBTEAC now uses tables.								GbG
 *               ---- OS-9/68K V3.0.3 Released ----
 *               ---- OS-9/68K V3.1 Released ----
 */

#define		driver_static

#include	"rbteac.h"
#include	"rbproto.h"

extern	error_code	read_fteac(),
					init_fteac(),
					llterm();


/* bootfteac: Boot from an TEAC SCSI Floppy drive
 *
 * IMPORTANT NOTE: 	This code REQUIRES that a pseudo path descriptor
 *					pointer be initialized in the global static storage
 *					variable "defopts".
 */
#if defined (__STDC__) || defined (_ANSI_EXT)
int bootfteac(void)
#else
int bootfteac()
#endif
{
	inizdriver = init_fteac;
	readdriver = read_fteac;
	termdriver = llterm;
	return diskboot();
}
