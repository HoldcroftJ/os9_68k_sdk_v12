/*
 * generic definitions for SCSI
 */

/*
 * Copyright 1990 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 * history:
 *
 *
 *  History:
 *  Ed.   Date    Who    Reason
 *  --  --------  --- ----------------------------------------------------
 *                    ---- OS-9/68k V3.0 Release ----
 *                    ---- OS-9/68k V3.1 Release ----
 *                    
 */
/*----------------------------------------------------------!
! General generic definitions for general generic purposes	!
!----------------------------------------------------------*/
#ifndef GENDEFS_H

/*------------------!
! Logic Definitions	!
!------------------*/
#ifndef TRUE
#define TRUE		1			/* boolean TRUE			*/
#endif

#ifndef FALSE
#define FALSE		0			/* boolean FALSE		*/
#endif

#ifndef ON
#define ON			1			/* common use logic on	*/
#endif

#ifndef OFF
#define OFF			0			/* common use logic off	*/
#endif

#define PLON		1			/* positive logic on	*/
#define PLOFF		0			/* positive logic off	*/

#define NLON		0			/* negative logic on	*/
#define NLOFF		1			/* negative logic off	*/

#ifndef NULL
#define NULL		0			/* pointer to nowhere	*/
#endif

#ifndef SUCCESS
#define SUCCESS		0
#endif

#ifndef ERROR
#define ERROR		-1			/* general error value	*/
#endif

#ifndef ERROR2
#define ERROR2		-3			/* another error value	*/
#endif

#ifndef BEEP
#define BEEP		0x07
#endif

#ifndef BACKSPACE
#define BACKSPACE	0x08
#endif

#ifndef BOOTFUNCS
#define BOOTFUNCS	16
#endif

/*----------------------------------------------!
! Typedefs for OS-9000 compatibility under OSK	!
!----------------------------------------------*/

#ifndef	_TYPES_H
typedef unsigned long	u_int32;
typedef unsigned short	u_int16;
typedef long			int32;
typedef short			int16;
typedef int				error_code;
#endif

#define GENDEFS_H
#endif
