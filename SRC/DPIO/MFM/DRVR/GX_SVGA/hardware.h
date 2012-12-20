/*******************************************************************************
*
* DESCRIPTION :
*
*	THIS FILE IS REQUIRED BY THE TEMPLATE.
*
*	This file contains hardware specific definitions. This file is required by
*	the template, but the definitions here are not used by the template.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* SCCS VERSION: @(#)hardware.h	1.1 1/4/96
*
* EDITION  DATE      WHO  DETAIL
*       1  12/22/94  TJE  First version
*	2  04/09/96  AND  Adopted for SVGA template
*/
#ifndef _HARDWARE_H
#define _HARDWARE_H

#include <defs.h>

/* VESA Standard Mode numbers */
#define	MODE_UNDEFINED			0
#define MODE_3				3
#define MODE_101				101
#define	MODE_103				103
#define	MODE_105				105  
#define MODE_110				110
#define	MODE_113				113
#define	MODE_116				116  
#define MODE_112				112
#define	MODE_115				115
#define	MODE_118				118  

#define	BUS_UNDEFINED			0
#define	BUS_ISA					1
#define	BUS_VESA				2
#define	BUS_PCI					3

#endif /* _HARDWARE_H */

