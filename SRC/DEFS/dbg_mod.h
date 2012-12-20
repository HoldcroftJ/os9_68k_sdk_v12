/****************************************************************************
 *	File:	@(#) dbg_mod.h 1.1@(#)
 *  ID:		5/19/95
 ****************************************************************************
 *	Debug data module header file. This file should be included for any		*
 *	modules wishing to use the standard dbg_mod.c file for making debugging	*
 *	data module history.													*
 ****************************************************************************
 *  Copyright 1993, 1994 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date       Comments                                           By   *
 *  --- --------    ----------------------------------------------- ------- *
 *    0 07/08/94    "Formalization"                                   cas	*
 *	  1 10/17/94	changed func decl's to match func def's			  gkm   *
 *					Added debug_timestamp call                              *
 *					Added wrap count to the dbg_stat structure              *
 *					Added debug_4data() to print 4 longs of debug data      *
 *	  2 06/19/95	Changed file name to dbg_mod.h					  dwj   *
 *	  3 08/28/95	Added compatibility for C++						  dwj	*
 *    4 02/06/97    Added fields for signals                          jmh   *
 *    5 12/17/97    Added instruction for OS-9000                     taka  *
 ****************************************************************************/

#ifndef _DEBUG_H
#define _DEBUG_H

/* The library calls in the calling code can simply be compiled in or out 
 * by using the -dDEBUG option on the compile line. This one makes it so it
 * doesn't pull in anything if the caller doesn't have -dDEBUG on the compile
 * line.
 */

#include <types.h>
#include <const.h>
#include <module.h>
#include <errno.h>

/****************************************************************************
 * This is the structure of the debug static area dbg_mod.c uses to do its
 * thing. the debug_init() entry point returns a pointer to this structure
 * that the caller needs to pass in to make the debug calls. This way,
 * multiple modules can write to the same data module for real time analysis.
 *
 * **This structure is only kept by the caller.** The contents of the
 * structure should NEVER be modified by the caller. The debug library
 * will keep track of everything within.
 */
typedef struct dbg_stat {
	u_char	*dbg_base,
			*dbg_ptr;
	u_int32	dbg_size,
			dbg_wrap;	/* count of how many times dbg_ptr wrapped around */
	u_int32	dbg_procid,	/* Process ID of external debugging process */
			dbg_signal, /* Signal to send to external debugging process */
			dbg_rsvd1,
			dbg_rsvd2;	/* Reserved fields for 16-byte boundary */
} dbg_stat, *Dbg_stat;


/****************************************************************************
 * Function prototpes for the dbg_mod.l library. All dbg_mod.l calls in the 
 * calling code must be in the form:
 *
 *	#ifdef DEBUG
 *		< The library call >
 *	#endif
 *
 *	This will enable the code to be compiled in debug or normal mode easily.
 */

/*	The following is needed for compatibility for C++	*/
#ifdef __cplusplus
extern "C" {
#endif

/* This is the entry point where the caller tells the debug library which
 * data module name to use and how big the data module should be. It is
 * recommended that these two parameters be kept somewhere like a device
 * descriptor where the calling code can get them out without being hard-
 * coded anywhere within the module.
 *
 *	preC:	size = desired size of debug module > 64 bytes.
 *			mod_name = name of desired data module to use != NULL.
 *
 *	postC:	(mod_name exists) ^ (link count mod_name++) ^ 
 *						(dbg_ptr points to Dbg_stat of mod_name)
 */
error_code debug_init(u_int32 size, Dbg_stat *dbg_ptr, char *mod_name);


/* This call writes a character string no bigger than 10 characters and 
 * one, u_int32 into the debug module. It is created so it uses one full
 * line if you dump the debug module in rombug.
 *
 *	preC: debug_stat = pointer to module's debug_stat structure
 *
 *	postC: sptr & data written to next dump line in data module.
 */
void debug_data(Dbg_stat debug_stat, char *sptr, u_int32 data);


/* This call writes a character string of size bytes into the data module.
 * the function will round up to the nearest dump line padding with zeros
 * so the debug module remains readable when dumped in rombug.
 *
 *	preC: debug_stat = pointer to module's debug_stat structure
 *
 *	postC: size bytes pointed to by dptr are written to next dump line(s)
 *	in data module as well as padding with 0's to the next 4-long word
 *	boundary.
 */
void debug_string(Dbg_stat debug_stat, u_char *dptr, u_int32 size);


/****************************************************************************
 * This function writes a timestamp into the debug data module.  The
 * time stamp has the following layout:
 *	byte:	0	1	2	3	4	5	6	7	8	9	a	b	c	d	e	f
 *			seconds-elaps- system- current  ----- x -----   ----- y -----
 *			-ed since Jan-  tick    clock
 *			- 01 1970       rate    tick
 * x and y (int) are 2 unsigned longs that may be passed in for debugging
 */
void debug_timestamp(Dbg_stat debug_stat, u_int32 x, u_int32 y);


/****************************************************************************
 * This function writes 4 longs of debug data.
 */
void debug_4data(Dbg_stat debug_stat,
				 u_int32 a, u_int32 b, u_int32 c, u_int32 d);


/*************************************************************************
 * General usage for debuggers:
 * 
 *	When the data module is to be used, follow the steps below:
 *
 *	1.	Break into rombug by typing "break".
 *
 *	2.	Type "link X" at the rombug prompt where X is the name of the 
 *		debug data module to be looked at.  This links the .r7 relocation
 *      register pointer to the beginning of the debug module.
 *
 *	3.	Type ".r7 .r7+<offset>" at the prompt. This sets the r7 register to
 *		the start of the debug static area of the module and past the module
 *		header so things look right in the data module when dumping.
 *           for OS-9, the offset is 0x34        ".r7 .r7+34"
 *           for OS-9000, the offset is 0x64     ".r7 .r7+64"
 *
 *	4.	Type ".r6 [.r7+4]" at the prompt. This sets register r6 line after
 *		the last debug statement put into the debug module. Remember, the
 *		writing in the debug module rolls over after writing to the end
 *		so you'll never know where the lst statement really is without
 *		doing this.
 *
 *	5.	Now you can analyze the contents of the messages in the debug module
 *		by looking at the statements behind the statement pointed to by r6
 *		to find out what happened leading up to the crash or events under
 *		investigation.
 ****************************************************************************/

/*	End up with C++ support	*/
#ifdef __cplusplus
}
#endif

#endif /* ifndef _DEBUG_H */
